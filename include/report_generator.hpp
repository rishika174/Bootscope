#pragma once

#include "boot_collector.hpp"
#include "service_analyzer.hpp"
#include "dependency_graph.hpp"
#include "bottleneck_detector.hpp"
#include <string>
#include <ostream>

namespace bootscope {

enum class ReportFormat {
    Terminal,
    Markdown,
    Json,
    Html
};

class ReportGenerator {
public:
    ReportGenerator(
        const BootTimings& timings,
        const SystemHardware& hardware,
        const std::vector<ServiceUnit>& services,
        const CriticalChainResult& critical_chain,
        const std::vector<BottleneckFinding>& bottlenecks
    );

    // Generate output string for the requested format
    std::string generate(ReportFormat format) const;

    // Direct file export helper
    bool exportToFile(const std::string& filepath, ReportFormat format) const;

private:
    BootTimings m_timings;
    SystemHardware m_hardware;
    std::vector<ServiceUnit> m_services;
    CriticalChainResult m_chain;
    std::vector<BottleneckFinding> m_bottlenecks;

    std::string renderTerminal() const;
    std::string renderMarkdown() const;
    std::string renderJson() const;
    std::string renderHtml() const;

    static std::string formatDuration(double sec);
};

} // namespace bootscope
