#include "boot_collector.hpp"
#include "service_analyzer.hpp"
#include "dependency_graph.hpp"
#include "bottleneck_detector.hpp"
#include "report_generator.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

void printHelp() {
    std::cout << "BootScope v1.0.0 — Linux Boot-Time Profiler & Dependency Analyzer\n"
              << "Usage: bootscope [options]\n\n"
              << "Options:\n"
              << "  -a, --analyze               Run complete boot analysis\n"
              << "  -b, --blame                 List services sorted by initialization time\n"
              << "  -t, --threshold <sec>       Duration filter threshold in seconds (default: 1.0s)\n"
              << "  -c, --critical-path         Compute and display the critical boot dependency chain\n"
              << "  --target <target>           Specify target milestone (default: graphical.target)\n"
              << "  --bottlenecks               Detect performance bottlenecks and output fix advice\n"
              << "  -e, --export <format>       Export report (terminal, markdown, json, html)\n"
              << "  -o, --output <file>         Output filepath for export\n"
              << "  -h, --help                  Show this help menu\n"
              << "  -v, --version               Display version information\n";
}

int main(int argc, char* argv[]) {
    bool run_blame = false;
    bool run_critical = false;
    bool run_bottlenecks = false;
    bool run_full_analysis = false;
    double threshold_sec = 1.0;
    std::string export_format = "terminal";
    std::string output_file = "";
    std::string target_name = "graphical.target";

    if (argc < 2) {
        run_full_analysis = true;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp();
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "BootScope 1.0.0 (Linux C++17)\n";
            return 0;
        } else if (arg == "-a" || arg == "--analyze") {
            run_full_analysis = true;
        } else if (arg == "-b" || arg == "--blame") {
            run_blame = true;
        } else if (arg == "-c" || arg == "--critical-path") {
            run_critical = true;
        } else if (arg == "--bottlenecks") {
            run_bottlenecks = true;
        } else if ((arg == "-t" || arg == "--threshold") && i + 1 < argc) {
            threshold_sec = std::stod(argv[++i]);
        } else if ((arg == "-e" || arg == "--export") && i + 1 < argc) {
            export_format = argv[++i];
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg == "--target" && i + 1 < argc) {
            target_name = argv[++i];
        }
    }

    std::cout << "[BootScope] Initiating Linux boot profile scan...\n";

    // 1. Collect system & hardware data
    bootscope::BootCollector collector;
    collector.collectAll();

    // 2. Parse service blame data
    bootscope::ServiceAnalyzer analyzer;
    analyzer.parseBlameData();

    // 3. Build Dependency Graph
    bootscope::DependencyGraph graph;
    graph.buildFromServices(analyzer.getAllServices());

    // 4. Compute Critical Chain
    bootscope::CriticalChainResult chain = graph.computeCriticalPath(target_name);

    // 5. Detect Bottlenecks
    bootscope::BottleneckDetector detector(threshold_sec);
    auto bottlenecks = detector.analyze(analyzer.getAllServices(), chain);

    // If specific sub-command requested, print targeted view
    if (run_blame) {
        std::cout << "\n[BootScope] Service Blame Ranking (threshold >= " << threshold_sec << "s):\n";
        auto slow = analyzer.getSlowServices(threshold_sec);
        if (slow.empty()) {
            std::cout << "  No services exceeded threshold (" << threshold_sec << "s).\n";
        } else {
            for (const auto& svc : slow) {
                std::cout << "  " << std::fixed << std::setprecision(3) << svc.duration_sec << "s  "
                          << svc.name << (svc.in_critical_path ? "  [CRITICAL CHAIN]" : "") << "\n";
            }
        }
        return 0;
    }

    if (run_critical) {
        std::cout << "\n[BootScope] Critical Boot Chain to " << target_name << ":\n";
        std::cout << "  Total Chain Duration: " << chain.total_chain_duration_sec << "s\n";
        for (size_t i = 0; i < chain.path.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] -> " << chain.path[i] << "\n";
        }
        return 0;
    }

    if (run_bottlenecks) {
        std::cout << "\n[BootScope] Bottleneck Findings (" << bottlenecks.size() << " detected):\n";
        for (const auto& b : bottlenecks) {
            std::cout << "  * [" << (b.severity == bootscope::Severity::Critical ? "CRITICAL" : "WARNING") << "] "
                      << b.service_name << " (" << b.duration_sec << "s)\n"
                      << "    Diagnosis: " << b.diagnosis << "\n"
                      << "    Fix:       " << b.remediation_cli << "\n";
        }
        return 0;
    }

    // Default or --analyze: Generate Full Report
    (void)run_full_analysis;
    bootscope::ReportGenerator reporter(
        collector.getTimings(),
        collector.getHardware(),
        analyzer.getAllServices(),
        chain,
        bottlenecks
    );

    bootscope::ReportFormat fmt = bootscope::ReportFormat::Terminal;
    if (export_format == "markdown" || export_format == "md") fmt = bootscope::ReportFormat::Markdown;
    else if (export_format == "json") fmt = bootscope::ReportFormat::Json;
    else if (export_format == "html") fmt = bootscope::ReportFormat::Html;

    if (!output_file.empty()) {
        if (reporter.exportToFile(output_file, fmt)) {
            std::cout << "[BootScope] Report exported successfully to: " << output_file << "\n";
        } else {
            std::cerr << "[BootScope Error] Failed to write report to: " << output_file << "\n";
            return 1;
        }
    } else {
        std::cout << reporter.generate(fmt);
    }

    return 0;
}
