# BootScope Verification & Test Report
**Date:** 2026-10-02
**Target Platform:** Linux x86_64 (Kernel 6.x)
**Compiler:** GCC 13.2 / Clang 17 (C++17)

## 1. Unit Test Execution Results
All unit tests located in `tests/test_bootscope.cpp` were executed against static mock systemd data and DAG test topologies:

| Test Case Name | Target Under Test | Input Vector | Expected Output | Status |
| :--- | :--- | :--- | :--- | :--- |
| `testBlameParsing` | `ServiceAnalyzer::parseBlameData` | Sample multi-line blame output with ms, s, and min tokens | 3 units correctly parsed with matching durations | **PASS** |
| `testGraphCriticalPath` | `DependencyGraph::computeCriticalPath` | Diamond DAG (A=2s, B=3s, C=1s, D=4s) | Longest path A->B->D (9.0s) identified | **PASS** |
| `testCycleDetection` | `DependencyGraph::hasCycles` | Circular graph (X -> Y -> X) | Flagged `hasCycles() == true` via Kahn's sort | **PASS** |

## 2. Integration & Interface Validation
- `/proc/uptime`: Accurately parses kernel uptime and idle durations.
- `/sys/firmware/efi`: Differentiates between UEFI EFI runtime services and Legacy MBR BIOS.
- `systemd-analyze time`: Regex correctly extracts decimal seconds and milliseconds across all 5 boot phases.
- Kahn's Algorithm: Runs in $O(V + E)$ time and avoids stack overflows associated with naive recursive DFS.
