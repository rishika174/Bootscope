#pragma once

#include "service_analyzer.hpp"
#include "dependency_graph.hpp"
#include <string>
#include <vector>

namespace bootscope {

enum class Severity {
    Low,
    Medium,
    High,
    Critical
};

struct BottleneckFinding {
    std::string service_name;
    double duration_sec{0.0};
    Severity severity{Severity::Medium};
    std::string diagnosis;
    std::string recommendation;
    std::string remediation_cli;
    double estimated_savings_sec{0.0};
    bool on_critical_chain{false};
};

class BottleneckDetector {
public:
    BottleneckDetector(double warning_threshold = 1.0, double critical_threshold = 3.0);

    // Run automated diagnostics
    std::vector<BottleneckFinding> analyze(
        const std::vector<ServiceUnit>& services,
        const CriticalChainResult& critical_chain
    );

    void setWarningThreshold(double sec) { m_warning_sec = sec; }
    void setCriticalThreshold(double sec) { m_critical_sec = sec; }

private:
    double m_warning_sec;
    double m_critical_sec;

    void inspectSpecificBottlenecks(
        const ServiceUnit& unit,
        bool is_critical,
        std::vector<BottleneckFinding>& findings
    );
};

} // namespace bootscope
