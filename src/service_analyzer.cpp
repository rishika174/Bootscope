#include "service_analyzer.hpp"
#include "boot_collector.hpp"
#include <sstream>
#include <regex>
#include <algorithm>

namespace bootscope {

ServiceAnalyzer::ServiceAnalyzer() = default;

bool ServiceAnalyzer::parseBlameData(const std::string& blame_output) {
    std::string data = blame_output;
    if (data.empty()) {
        data = BootCollector::executeCommand("systemd-analyze blame 2>/dev/null");
    }

    if (data.empty()) {
        return false;
    }

    m_services.clear();
    std::istringstream stream(data);
    std::string line;

    // Pattern: "  3.410s NetworkManager-wait-online.service" or " 890ms snapd.service"
    std::regex line_regex("^\\s*([0-9.]+)(s|ms|min)\\s+([a-zA-Z0-9_@.-]+)");

    while (std::getline(stream, line)) {
        std::smatch match;
        if (std::regex_search(line, match, line_regex)) {
            double val = std::stod(match[1].str());
            std::string unit = match[2].str();
            std::string service_name = match[3].str();

            if (unit == "ms") val /= 1000.0;
            else if (unit == "min") val *= 60.0;

            ServiceUnit svc;
            svc.name = service_name;
            svc.duration_sec = val;
            m_services.push_back(svc);
        }
    }

    return !m_services.empty();
}

bool ServiceAnalyzer::inspectUnitDependencies(ServiceUnit& unit) {
    std::string cmd = "systemctl show " + unit.name + " --property=After,Before,Requires,Wants,Description 2>/dev/null";
    std::string output = BootCollector::executeCommand(cmd);

    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if (key == "Description") {
            unit.description = val;
        } else if (key == "After") {
            unit.after = splitTokens(val);
        } else if (key == "Before") {
            unit.before = splitTokens(val);
        } else if (key == "Requires") {
            unit.requires_units = splitTokens(val);
        } else if (key == "Wants") {
            unit.wants = splitTokens(val);
        }
    }
    return true;
}

std::vector<ServiceUnit> ServiceAnalyzer::getSlowServices(double threshold_sec) const {
    std::vector<ServiceUnit> result;
    for (const auto& svc : m_services) {
        if (svc.duration_sec >= threshold_sec) {
            result.push_back(svc);
        }
    }
    return result;
}

const ServiceUnit* ServiceAnalyzer::findService(const std::string& name) const {
    for (const auto& svc : m_services) {
        if (svc.name == name) return &svc;
    }
    return nullptr;
}

void ServiceAnalyzer::addService(const ServiceUnit& unit) {
    m_services.push_back(unit);
}

std::vector<std::string> ServiceAnalyzer::splitTokens(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace bootscope
