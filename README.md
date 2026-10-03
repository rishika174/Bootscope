# BootScope — Linux Boot-Time Profiler & Dependency Analyzer

A lightweight, high-performance C++17 command-line diagnostic tool designed for Linux systems engineering. BootScope interrogates systemd, `/proc`, and `/sys` hardware interfaces to calculate boot phase durations, construct service dependency directed acyclic graphs (DAG), compute the boot critical chain, and detect latency bottlenecks.

## 🚀 Features
- **Boot Information Collector**: Extracts timings from `systemd-analyze time`, kernel parameters from `/proc/cmdline`, and hardware DMI info from `/sys/class/dmi/id`.
- **Startup Time Analyzer**: Parses `systemd-analyze blame` to rank slow services by initialization duration and CPU/memory metrics.
- **Service Dependency Graph**: Builds an in-memory DAG using adjacency lists, resolves `Requires=`, `Wants=`, `After=`, and `Before=` constraints, and calculates the exact **Critical Chain** (longest latency path).
- **Bottleneck Detector**: Evaluates configurable latency thresholds and flags common Linux boot stalls (e.g., NetworkManager-wait-online, synchronous cloud-init, disk fsck waits).
- **Multi-Format Report Generator**: Exports comprehensive audit reports in ANSI Terminal formatting, Markdown, JSON, and HTML.

## 🛠️ Building & Installing

### Prerequisites
- Modern Linux Distribution (Ubuntu 22.04+, Debian 12+, Arch, Fedora 39+)
- GCC 9+ or Clang 10+ (supporting C++17)
- CMake 3.16+
- systemd (version 240+)

```bash
# 1. Clone repository
git clone https://github.com/developer/bootscope.git
cd bootscope

# 2. Configure build with CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 3. Compile project
cmake --build build -j$(nproc)

# 4. Run automated test suite
./build/bootscope_tests

# 5. Install system-wide (optional)
sudo cmake --install build
```

## 💻 CLI Usage Examples

```bash
# Full boot summary analysis
./build/bootscope --analyze

# Top slow services exceeding 1.5 seconds
./build/bootscope --blame --threshold 1.5s

# Compute and visualize the critical path to graphical.target
./build/bootscope --critical-path --target graphical.target

# Run automated bottleneck analysis with remediation commands
./build/bootscope --bottlenecks

# Export diagnostic report to Markdown and JSON
./build/bootscope --export markdown -o boot-audit.md
./build/bootscope --export json -o boot-audit.json
```
