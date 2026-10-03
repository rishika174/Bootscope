#pragma once

#include "service_analyzer.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace bootscope {

struct GraphNode {
    std::string name;
    double duration_sec{0.0};
    std::vector<std::string> dependencies; // Nodes that must finish Before this node
    std::vector<std::string> dependents;   // Nodes that run After this node
};

struct CriticalChainResult {
    std::vector<std::string> path; // Sequence of service names in order
    double total_chain_duration_sec{0.0};
    bool cycle_detected{false};
};

class DependencyGraph {
public:
    DependencyGraph() = default;

    // Populate graph from parsed services
    void buildFromServices(const std::vector<ServiceUnit>& services);

    // Explicit edge addition
    void addEdge(const std::string& from_node, const std::string& to_node);
    void setNodeDuration(const std::string& name, double duration_sec);

    // Topological Sort & Cycle Detection (Kahn's Algorithm)
    bool hasCycles() const;
    std::vector<std::string> topologicalSort() const;

    // Critical Path Calculation: Computes longest weighted path to target
    CriticalChainResult computeCriticalPath(const std::string& target_node = "") const;

    // Graph queries
    const std::unordered_map<std::string, GraphNode>& getNodes() const { return m_nodes; }
    size_t nodeCount() const { return m_nodes.size(); }

private:
    std::unordered_map<std::string, GraphNode> m_nodes;
};

} // namespace bootscope
