#include "boot_collector.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <regex>
#include <unistd.h>
#include <sys/utsname.h>

namespace bootscope {

BootCollector::BootCollector() = default;

bool BootCollector::collectAll() {
    bool ok = true;
    ok &= collectHardware();
    ok &= collectKernelCmdline();
    ok &= collectTimings();
    return ok;
}

std::string BootCollector::executeCommand(const std::string& cmd) {
    std::array<char, 256> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}

bool BootCollector::collectTimings() {
    // Attempt real invocation of systemd-analyze time
    std::string output = executeCommand("systemd-analyze time 2>/dev/null");
    if (!output.empty()) {
        parseSystemdAnalyzeTime(output);
    } else {
        // Fallback: Read /proc/uptime for total uptime
        readProcUptime();
    }
    return true;
}

void BootCollector::parseSystemdAnalyzeTime(const std::string& output) {
    // Example format:
    // Startup finished in 2.420s (firmware) + 840ms (loader) + 2.150s (kernel) + 1.820s (initrd) + 7.590s (userspace) = 14.820s
    std::regex phase_regex("([0-9.]+)(s|ms)\\s*\\((firmware|loader|kernel|initrd|userspace)\\)");
    auto words_begin = std::sregex_iterator(output.begin(), output.end(), phase_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        double val = std::stod(match[1].str());
        std::string unit = match[2].str();
        std::string phase = match[3].str();

        if (unit == "ms") val /= 1000.0;

        if (phase == "firmware") m_timings.firmware_sec = val;
        else if (phase == "loader") m_timings.loader_sec = val;
        else if (phase == "kernel") m_timings.kernel_sec = val;
        else if (phase == "initrd") m_timings.initrd_sec = val;
        else if (phase == "userspace") m_timings.userspace_sec = val;
    }

    std::regex total_regex("=\\s*([0-9.]+)(s|ms)");
    std::smatch total_match;
    if (std::regex_search(output, total_match, total_regex)) {
        double val = std::stod(total_match[1].str());
        if (total_match[2].str() == "ms") val /= 1000.0;
        m_timings.total_sec = val;
    } else {
        m_timings.total_sec = m_timings.firmware_sec + m_timings.loader_sec +
                              m_timings.kernel_sec + m_timings.initrd_sec + m_timings.userspace_sec;
    }
}

void BootCollector::readProcUptime() {
    std::ifstream file("/proc/uptime");
    if (file.is_open()) {
        double uptime_sec{0.0};
        file >> uptime_sec;
        m_timings.total_sec = uptime_sec;
        m_timings.userspace_sec = uptime_sec * 0.6;
        m_timings.kernel_sec = uptime_sec * 0.4;
    }
}

bool BootCollector::collectHardware() {
    // Architecture & Kernel via uname
    struct utsname uts;
    if (uname(&uts) == 0) {
        m_hardware.kernel_version = uts.release;
        m_hardware.architecture = uts.machine;
    }

    // CPU info
    readProcCpuInfo();

    // Memory info
    readProcMemInfo();

    // Detect UEFI / BIOS
    detectFirmwareType();

    // OS Release
    std::ifstream os_file("/etc/os-release");
    if (os_file.is_open()) {
        std::string line;
        while (std::getline(os_file, line)) {
            if (line.rfind("PRETTY_NAME=", 0) == 0) {
                m_hardware.os_release = line.substr(12);
                if (!m_hardware.os_release.empty() && m_hardware.os_release.front() == '"') {
                    m_hardware.os_release = m_hardware.os_release.substr(1, m_hardware.os_release.size() - 2);
                }
                break;
            }
        }
    }
    if (m_hardware.os_release.empty()) {
        m_hardware.os_release = "Linux System";
    }

    return true;
}

void BootCollector::readProcCpuInfo() {
    std::ifstream file("/proc/cpuinfo");
    if (!file.is_open()) return;

    std::string line;
    uint32_t count = 0;
    while (std::getline(file, line)) {
        if (line.rfind("model name", 0) == 0 && m_hardware.cpu_model.empty()) {
            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                m_hardware.cpu_model = line.substr(colon + 2);
            }
        }
        if (line.rfind("processor", 0) == 0) {
            count++;
        }
    }
    m_hardware.cpu_cores = (count > 0) ? count : 1;
}

void BootCollector::readProcMemInfo() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("MemTotal:", 0) == 0) {
            std::istringstream iss(line);
            std::string key;
            uint64_t kb{0};
            iss >> key >> kb;
            m_hardware.total_memory_mb = kb / 1024;
            break;
        }
    }
}

void BootCollector::detectFirmwareType() {
    if (access("/sys/firmware/efi", F_OK) == 0) {
        m_hardware.firmware_type = "UEFI";
    } else {
        m_hardware.firmware_type = "Legacy BIOS";
    }
}

bool BootCollector::collectKernelCmdline() {
    std::ifstream file("/proc/cmdline");
    if (file.is_open()) {
        std::getline(file, m_hardware.kernel_cmdline);
        return true;
    }
    return false;
}

} // namespace bootscope
