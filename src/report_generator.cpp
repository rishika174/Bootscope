#include "report_generator.hpp"
#include <iomanip>
#include <sstream>
#include <fstream>

namespace bootscope {

ReportGenerator::ReportGenerator(
    const BootTimings& timings,
    const SystemHardware& hardware,
    const std::vector<ServiceUnit>& services,
    const CriticalChainResult& critical_chain,
    const std::vector<BottleneckFinding>& bottlenecks
) : m_timings(timings),
    m_hardware(hardware),
    m_services(services),
    m_chain(critical_chain),
    m_bottlenecks(bottlenecks) {}

std::string ReportGenerator::generate(ReportFormat format) const {
    switch (format) {
        case ReportFormat::Terminal: return renderTerminal();
        case ReportFormat::Markdown: return renderMarkdown();
        case ReportFormat::Json: return renderJson();
        case ReportFormat::Html: return renderHtml();
    }
    return "";
}

bool ReportGenerator::exportToFile(const std::string& filepath, ReportFormat format) const {
    std::ofstream out(filepath);
    if (!out.is_open()) return false;
    out << generate(format);
    return true;
}

std::string ReportGenerator::formatDuration(double sec) {
    std::ostringstream ss;
    if (sec >= 1.0) {
        ss << std::fixed << std::setprecision(3) << sec << "s";
    } else {
        ss << std::fixed << std::setprecision(1) << (sec * 1000.0) << "ms";
    }
    return ss.str();
}

std::string ReportGenerator::renderTerminal() const {
    std::ostringstream ss;
    ss << "\n======================================================\n";
    ss << "          BOOTSCOPE: LINUX BOOT PROFILER              \n";
    ss << "======================================================\n";
    ss << "OS:        " << m_hardware.os_release << "\n";
    ss << "Kernel:    " << m_hardware.kernel_version << " (" << m_hardware.architecture << ")\n";
    ss << "CPU:       " << m_hardware.cpu_model << " (" << m_hardware.cpu_cores << " cores)\n";
    ss << "Memory:    " << m_hardware.total_memory_mb << " MB\n";
    ss << "Firmware:  " << m_hardware.firmware_type << "\n";
    ss << "------------------------------------------------------\n";
    ss << "BOOT TIMELINE BREAKDOWN:\n";
    ss << "  Firmware (POST): " << formatDuration(m_timings.firmware_sec) << "\n";
    ss << "  Loader (GRUB):   " << formatDuration(m_timings.loader_sec) << "\n";
    ss << "  Kernel:          " << formatDuration(m_timings.kernel_sec) << "\n";
    ss << "  Initrd:          " << formatDuration(m_timings.initrd_sec) << "\n";
    ss << "  Userspace:       " << formatDuration(m_timings.userspace_sec) << "\n";
    ss << "  TOTAL BOOT TIME: " << formatDuration(m_timings.total_sec) << "\n";
    ss << "------------------------------------------------------\n";
    ss << "CRITICAL CHAIN DURATION: " << formatDuration(m_chain.total_chain_duration_sec) << "\n";
    for (size_t i = 0; i < m_chain.path.size(); ++i) {
        ss << "  " << (i + 1) << ". " << m_chain.path[i] << "\n";
    }
    ss << "------------------------------------------------------\n";
    ss << "BOTTLENECK ADVISOR (" << m_bottlenecks.size() << " findings):\n";
    for (const auto& b : m_bottlenecks) {
        ss << "  * [" << (b.severity == Severity::Critical ? "CRITICAL" : "WARNING") << "] "
           << b.service_name << " (" << formatDuration(b.duration_sec) << ")\n"
           << "    Diagnosis:  " << b.diagnosis << "\n"
           << "    Fix:        " << b.remediation_cli << "\n";
    }
    ss << "======================================================\n";
    return ss.str();
}

std::string ReportGenerator::renderMarkdown() const {
    std::ostringstream ss;
    ss << "# BootScope Diagnostic Report\n\n";
    ss << "## System Profile\n";
    ss << "- **OS**: " << m_hardware.os_release << "\n";
    ss << "- **Kernel**: `" << m_hardware.kernel_version << "` (`" << m_hardware.architecture << "`)\n";
    ss << "- **CPU**: " << m_hardware.cpu_model << " (" << m_hardware.cpu_cores << " cores)\n";
    ss << "- **RAM**: " << m_hardware.total_memory_mb << " MB\n";
    ss << "- **Firmware**: " << m_hardware.firmware_type << "\n\n";

    ss << "## Boot Phase Timings\n\n";
    ss << "| Phase | Duration | Percentage |\n";
    ss << "| :--- | :--- | :--- |\n";
    double total = (m_timings.total_sec > 0.0) ? m_timings.total_sec : 1.0;
    ss << "| Firmware | " << formatDuration(m_timings.firmware_sec) << " | " << std::fixed << std::setprecision(1) << (m_timings.firmware_sec / total * 100) << "% |\n";
    ss << "| Loader | " << formatDuration(m_timings.loader_sec) << " | " << std::fixed << std::setprecision(1) << (m_timings.loader_sec / total * 100) << "% |\n";
    ss << "| Kernel | " << formatDuration(m_timings.kernel_sec) << " | " << std::fixed << std::setprecision(1) << (m_timings.kernel_sec / total * 100) << "% |\n";
    ss << "| Initrd | " << formatDuration(m_timings.initrd_sec) << " | " << std::fixed << std::setprecision(1) << (m_timings.initrd_sec / total * 100) << "% |\n";
    ss << "| Userspace | " << formatDuration(m_timings.userspace_sec) << " | " << std::fixed << std::setprecision(1) << (m_timings.userspace_sec / total * 100) << "% |\n";
    ss << "| **Total** | **" << formatDuration(m_timings.total_sec) << "** | 100.0% |\n\n";

    ss << "## Critical Path\n\n";
    ss << "Critical Path Duration: **" << formatDuration(m_chain.total_chain_duration_sec) << "**\n\n";
    for (size_t i = 0; i < m_chain.path.size(); ++i) {
        ss << (i + 1) << ". `" << m_chain.path[i] << "`\n";
    }

    ss << "\n## Bottlenecks & Optimization Advice\n\n";
    for (const auto& b : m_bottlenecks) {
        ss << "### `" << b.service_name << "` (" << formatDuration(b.duration_sec) << ")\n";
        ss << "- **Diagnosis**: " << b.diagnosis << "\n";
        ss << "- **Recommendation**: " << b.recommendation << "\n";
        ss << "- **Remediation**: `" << b.remediation_cli << "`\n\n";
    }

    return ss.str();
}

std::string ReportGenerator::renderJson() const {
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"system\": {\n";
    ss << "    \"os\": \"" << m_hardware.os_release << "\",\n";
    ss << "    \"kernel\": \"" << m_hardware.kernel_version << "\",\n";
    ss << "    \"arch\": \"" << m_hardware.architecture << "\",\n";
    ss << "    \"cores\": " << m_hardware.cpu_cores << ",\n";
    ss << "    \"memory_mb\": " << m_hardware.total_memory_mb << "\n";
    ss << "  },\n";
    ss << "  \"timings\": {\n";
    ss << "    \"firmware_sec\": " << m_timings.firmware_sec << ",\n";
    ss << "    \"loader_sec\": " << m_timings.loader_sec << ",\n";
    ss << "    \"kernel_sec\": " << m_timings.kernel_sec << ",\n";
    ss << "    \"initrd_sec\": " << m_timings.initrd_sec << ",\n";
    ss << "    \"userspace_sec\": " << m_timings.userspace_sec << ",\n";
    ss << "    \"total_sec\": " << m_timings.total_sec << "\n";
    ss << "  },\n";
    ss << "  \"critical_chain_sec\": " << m_chain.total_chain_duration_sec << ",\n";
    ss << "  \"bottleneck_count\": " << m_bottlenecks.size() << "\n";
    ss << "}\n";
    return ss.str();
}

std::string ReportGenerator::renderHtml() const {
    std::ostringstream ss;
    ss << "<!DOCTYPE html><html><head><meta charset='utf-8'><title>BootScope Report</title>";
    ss << "<style>body{font-family:sans-serif;background:#0f172a;color:#f8fafc;padding:2rem;}";
    ss << "table{width:100%;border-collapse:collapse;margin:1rem 0;}th,td{padding:8px;border:1px solid #334155;text-align:left;}";
    ss << "pre{background:#1e293b;padding:1rem;border-radius:4px;overflow-x:auto;}</style></head><body>";
    ss << "<h1>BootScope Diagnostic Report</h1>";
    ss << "<p>System: " << m_hardware.os_release << " (" << m_hardware.kernel_version << ")</p>";
    ss << "<h2>Total Boot Duration: " << formatDuration(m_timings.total_sec) << "</h2>";
    ss << "</body></html>";
    return ss.str();
}

} // namespace bootscope
