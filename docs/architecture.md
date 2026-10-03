# System Architecture Specification
## BootScope Modular Decomposition & Linux Interfaces

BootScope is designed with strict modular separation conforming to SOLID principles. The architecture is partitioned into four primary layers:

```
+--------------------------------------------------------------------------+
|                        CLI Presentation Layer (main.cpp)                |
|           [Argument Parser]  [Color Formatter]  [Report Dispatcher]      |
+--------------------------------------------------------------------------+
                                    |
                                    v
+--------------------------------------------------------------------------+
|                         Analysis & Diagnostic Engine                     |
|  +--------------------+   +---------------------+   +-----------------+  |
|  | DependencyGraph    |   | BottleneckDetector  |   | ReportGenerator |  |
|  | - Topological Sort |   | - Threshold Filter  |   | - Terminal/MD   |  |
|  | - Critical Chain   |   | - Heuristic Rules   |   | - JSON/HTML     |  |
|  +--------------------+   +---------------------+   +-----------------+  |
+--------------------------------------------------------------------------+
                                    |
                                    v
+--------------------------------------------------------------------------+
|                          Data Ingestion Layer                            |
|  +--------------------+   +---------------------+                        |
|  | BootCollector      |   | ServiceAnalyzer     |                        |
|  | - Hardware DMI     |   | - Blame Parser      |                        |
|  | - Kernel Cmdline   |   | - Unit Properties   |                        |
|  +--------------------+   +---------------------+                        |
+--------------------------------------------------------------------------+
                                    |
                                    v
+--------------------------------------------------------------------------+
|                          Linux System Interfaces                         |
|   /proc/uptime    /proc/cmdline   /sys/firmware/efi   systemd-analyze    |
|   /proc/cpuinfo   /etc/os-release  systemctl show      systemctl blame   |
+--------------------------------------------------------------------------+
```

### Module Responsibilities:
1. **BootCollector**: Reads `/proc/uptime` and executes `systemd-analyze time` to isolate Firmware, Loader, Kernel, Initrd, and Userspace timings. Probes `/sys/firmware/efi` to detect UEFI vs Legacy BIOS.
2. **ServiceAnalyzer**: Parses `systemd-analyze blame` stdout using regex. For each unit, invokes `systemctl show` to retrieve `After=`, `Before=`, `Requires=`, and `Wants=`.
3. **DependencyGraph**: Implements adjacency-list graph representation. Calculates the Critical Path via DAG Longest Path dynamic programming:
   $$Dist[v] = \max_{(u,v) \in E} (Dist[u] + Duration[v])$$
4. **BottleneckDetector**: Applies heuristic rule matching to pinpoint synchronous delays, deprecated blockers (`systemd-udev-settle`), and offline network delays.
5. **ReportGenerator**: Serializes audit data into clean ASCII tables, Markdown documentation, JSON schemas, or HTML.
