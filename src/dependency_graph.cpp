#include "dependency_graph.hpp"
#include <queue>
#include <algorithm>

namespace bootscope {

void DependencyGraph::buildFromServices(const std::vector<ServiceUnit>& services) {
    m_nodes.clear();

    // 1. Initialize all nodes
    for (const auto& svc : services) {
        GraphNode node;
        node.name = svc.name;
        node.duration_sec = svc.duration_sec;
        m_nodes[svc.name] = node;
    }

    // 2. Add edges based on After / Before
    for (const auto& svc : services) {
        // If svc runs After X, then X must finish before svc (edge: X -> svc)
        for (const auto& dep : svc.after) {
            if (m_nodes.find(dep) != m_nodes.end()) {
                addEdge(dep, svc.name);
            }
        }
        // If svc runs Before Y, edge: svc -> Y
        for (const auto& dep : svc.before) {
            if (m_nodes.find(dep) != m_nodes.end()) {
                addEdge(svc.name, dep);
            }
        }
    }
}

void DependencyGraph::addEdge(const std::string& from_node, const std::string& to_node) {
    m_nodes[from_node].dependents.push_back(to_node);
    m_nodes[to_node].dependencies.push_back(from_node);
}

void DependencyGraph::setNodeDuration(const std::string& name, double duration_sec) {
    m_nodes[name].duration_sec = duration_sec;
}

bool DependencyGraph::hasCycles() const {
    auto sorted = topologicalSort();
    return sorted.size() != m_nodes.size();
}

std::vector<std::string> DependencyGraph::topologicalSort() const {
    std::unordered_map<std::string, int> in_degree;
    for (const auto& [name, node] : m_nodes) {
        in_degree[name] = node.dependencies.size();
    }

    std::queue<std::string> q;
    for (const auto& [name, deg] : in_degree) {
        if (deg == 0) {
            q.push(name);
        }
    }

    std::vector<std::string> result;
    while (!q.empty()) {
        std::string u = q.front();
        q.pop();
        result.push_back(u);

        for (const auto& v : m_nodes.at(u).dependents) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    return result;
}

CriticalChainResult DependencyGraph::computeCriticalPath(const std::string& target_node) const {
    CriticalChainResult res;
    if (m_nodes.empty()) return res;

    auto order = topologicalSort();
    if (order.size() != m_nodes.size()) {
        res.cycle_detected = true;
        return res;
    }

    // Longest Path in DAG using Dynamic Programming
    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> parent;

    for (const auto& name : order) {
        dist[name] = m_nodes.at(name).duration_sec;
        parent[name] = "";
    }

    for (const auto& u : order) {
        for (const auto& v : m_nodes.at(u).dependents) {
            double cost = dist[u] + m_nodes.at(v).duration_sec;
            if (cost > dist[v]) {
                dist[v] = cost;
                parent[v] = u;
            }
        }
    }

    // Determine target node
    std::string best_node = target_node;
    if (best_node.empty() || m_nodes.find(best_node) == m_nodes.end()) {
        double max_dist = -1.0;
        for (const auto& [name, d] : dist) {
            if (d > max_dist) {
                max_dist = d;
                best_node = name;
            }
        }
    }

    res.total_chain_duration_sec = dist[best_node];

    // Reconstruct critical path backwards
    std::string curr = best_node;
    while (!curr.empty()) {
        res.path.push_back(curr);
        curr = parent[curr];
    }
    std::reverse(res.path.begin(), res.path.end());

    return res;
}

} // namespace bootscope
