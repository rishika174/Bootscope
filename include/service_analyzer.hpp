#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>

namespace bootscope {

struct ServiceUnit {
    std::string name;
    double duration_sec{0.0};
    double cpu_time_sec{0.0};
    double memory_mb{0.0};
    std::string active_state{"active"};
    std::string sub_state{"running"};
    std::string description;
    
    // Dependencies declared in unit file
    std::vector<std::string> requires_units;
    std::vector<std::string> wants;
    std::vector<std::string> after;
    std::vector<std::string> before;

    bool in_critical_path{false};
};

class ServiceAnalyzer {
public:
    ServiceAnalyzer();

    // Collect service timing blame data via systemd-analyze blame
    bool parseBlameData(const std::string& blame_output = "");

    // Retrieve dependency details for a specific unit via systemctl show
    bool inspectUnitDependencies(ServiceUnit& unit);

    // Filter services exceeding a specified duration threshold
    std::vector<ServiceUnit> getSlowServices(double threshold_sec) const;

    // Accessors
    const std::vector<ServiceUnit>& getAllServices() const { return m_services; }
    const ServiceUnit* findService(const std::string& name) const;

    // Mock/feed helper for unit testing
    void addService(const ServiceUnit& unit);

private:
    std::vector<ServiceUnit> m_services;

    static double parseTimeString(const std::string& time_str);
    static std::vector<std::string> splitTokens(const std::string& line);
};

} // namespace bootscope
