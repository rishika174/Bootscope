#include "bottleneck_detector.hpp"
#include <unordered_set>

namespace bootscope {

BottleneckDetector::BottleneckDetector(double warning_threshold, double critical_threshold)
    : m_warning_sec(warning_threshold), m_critical_sec(critical_threshold) {}

std::vector<BottleneckFinding> BottleneckDetector::analyze(
    const std::vector<ServiceUnit>& services,
    const CriticalChainResult& critical_chain
) {
    std::vector<BottleneckFinding> findings;
    std::unordered_set<std::string> critical_set(critical_chain.path.begin(), critical_chain.path.end());

    for (const auto& svc : services) {
        bool is_critical = critical_set.count(svc.name) > 0;
        if (svc.duration_sec >= m_warning_sec || is_critical) {
            inspectSpecificBottlenecks(svc, is_critical, findings);
        }
    }

    return findings;
}

void BottleneckDetector::inspectSpecificBottlenecks(
    const ServiceUnit& unit,
    bool is_critical,
    std::vector<BottleneckFinding>& findings
) {
    BottleneckFinding f;
    f.service_name = unit.name;
    f.duration_sec = unit.duration_sec;
    f.on_critical_chain = is_critical;

    if (unit.duration_sec >= m_critical_sec) {
        f.severity = Severity::Critical;
    } else if (unit.duration_sec >= m_warning_sec) {
        f.severity = Severity::High;
    } else {
        f.severity = Severity::Medium;
    }

    // Heuristic 1: NetworkManager or networkd wait online
    if (unit.name.find("wait-online") != std::string::npos) {
        f.diagnosis = "Synchronous wait for carrier detect blocks graphical/multi-user target transition.";
        f.recommendation = "Mask the service if network availability is not required prior to desktop login.";
        f.remediation_cli = "sudo systemctl mask " + unit.name;
        f.estimated_savings_sec = unit.duration_sec * 0.95;
        findings.push_back(f);
        return;
    }

    // Heuristic 2: Snap daemon or seeded
    if (unit.name.find("snapd") != std::string::npos) {
        f.diagnosis = "Snap loopback squashfs mounting and AppArmor rule compilation delay boot.";
        f.recommendation = "Disable snapd.seeded.service or migrate heavily used utilities to native packages.";
        f.remediation_cli = "sudo systemctl disable snapd.seeded.service";
        f.estimated_savings_sec = unit.duration_sec * 0.6;
        findings.push_back(f);
        return;
    }

    // Heuristic 3: Plymouth splash screen wait
    if (unit.name.find("plymouth") != std::string::npos) {
        f.diagnosis = "Plymouth graphical bootsplash holds display switchover until timeout.";
        f.recommendation = "Disable plymouth in kernel cmdline or disable plymouth-quit-wait.service.";
        f.remediation_cli = "sudo systemctl disable plymouth-quit-wait.service";
        f.estimated_savings_sec = unit.duration_sec;
        findings.push_back(f);
        return;
    }

    // Heuristic 4: Cloud-init on bare metal / static setups
    if (unit.name.find("cloud-init") != std::string::npos) {
        f.diagnosis = "Cloud instance metadata polling delays boot on provisioned static systems.";
        f.recommendation = "Disable cloud-init once instance initialization is complete.";
        f.remediation_cli = "sudo touch /etc/cloud/cloud-init.disabled";
        f.estimated_savings_sec = unit.duration_sec;
        findings.push_back(f);
        return;
    }

    // Heuristic 5: General slow service
    if (unit.duration_sec >= m_warning_sec) {
        f.diagnosis = "Service duration exceeds configured threshold (" + std::to_string(m_warning_sec) + "s).";
        f.recommendation = "Evaluate whether this service can use systemd socket activation or Type=idle.";
        f.remediation_cli = "systemctl status " + unit.name;
        f.estimated_savings_sec = unit.duration_sec * 0.5;
        findings.push_back(f);
    }
}

} // namespace bootscope
