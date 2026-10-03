#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace bootscope {

struct BootTimings {
    double firmware_sec{0.0};
    double loader_sec{0.0};
    double kernel_sec{0.0};
    double initrd_sec{0.0};
    double userspace_sec{0.0};
    double total_sec{0.0};
};

struct SystemHardware {
    std::string os_release;
    std::string kernel_version;
    std::string architecture;
    std::string cpu_model;
    uint32_t cpu_cores{1};
    uint64_t total_memory_mb{0};
    std::string firmware_type; // UEFI or BIOS
    std::string kernel_cmdline;
};

class BootCollector {
public:
    BootCollector();

    // Collect all system boot information from /proc, /sys, and systemd
    bool collectAll();

    // Individual collectors
    bool collectTimings();
    bool collectHardware();
    bool collectKernelCmdline();

    // Data accessors
    const BootTimings& getTimings() const { return m_timings; }
    const SystemHardware& getHardware() const { return m_hardware; }

    // Helper: Execute a system command and capture stdout
    static std::string executeCommand(const std::string& cmd);

private:
    BootTimings m_timings;
    SystemHardware m_hardware;

    void parseSystemdAnalyzeTime(const std::string& output);
    void readProcUptime();
    void readProcCpuInfo();
    void readProcMemInfo();
    void detectFirmwareType();
};

} // namespace bootscope
