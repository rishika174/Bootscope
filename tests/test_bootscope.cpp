#include "service_analyzer.hpp"
#include "dependency_graph.hpp"
#include "bottleneck_detector.hpp"
#include <iostream>
#include <cassert>

void testBlameParsing() {
    bootscope::ServiceAnalyzer analyzer;
    std::string sample = 
        " 3.410s NetworkManager-wait-online.service\n"
        " 2.140s snapd.service\n"
        "  890ms plymouth-quit-wait.service\n";

    bool ok = analyzer.parseBlameData(sample);
    (void)ok;
    assert(ok && "Should parse sample blame data");
    assert(analyzer.getAllServices().size() == 3 && "Expected 3 parsed services");
    assert(analyzer.findService("snapd.service") != nullptr);
    assert(analyzer.findService("snapd.service")->duration_sec == 2.140);
    std::cout << "[PASS] testBlameParsing\n";
}

void testGraphCriticalPath() {
    bootscope::DependencyGraph graph;

    // Diamond DAG: A (2s) -> B (3s) and C (1s) -> D (4s)
    graph.setNodeDuration("A", 2.0);
    graph.setNodeDuration("B", 3.0);
    graph.setNodeDuration("C", 1.0);
    graph.setNodeDuration("D", 4.0);

    graph.addEdge("A", "B");
    graph.addEdge("A", "C");
    graph.addEdge("B", "D");
    graph.addEdge("C", "D");

    assert(!graph.hasCycles() && "Graph must be acyclic");

    auto result = graph.computeCriticalPath("D");
    assert(result.path.size() == 3 && "Path should be A -> B -> D");
    assert(result.total_chain_duration_sec == 9.0 && "Path weight 2 + 3 + 4 = 9.0s");

    std::cout << "[PASS] testGraphCriticalPath\n";
}

void testCycleDetection() {
    bootscope::DependencyGraph graph;
    graph.setNodeDuration("X", 1.0);
    graph.setNodeDuration("Y", 1.0);
    graph.addEdge("X", "Y");
    graph.addEdge("Y", "X"); // circular dependency

    assert(graph.hasCycles() && "Should flag cycle");
    std::cout << "[PASS] testCycleDetection\n";
}

int main() {
    std::cout << "Running BootScope Test Suite...\n";
    testBlameParsing();
    testGraphCriticalPath();
    testCycleDetection();
    std::cout << "All BootScope unit tests PASSED successfully!\n";
    return 0;
}
