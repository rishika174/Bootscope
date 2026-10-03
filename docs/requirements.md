# Product Requirements Document (PRD)
## BootScope — Linux Boot-Time Profiler & Dependency Analyzer

### 1. Executive Summary
BootScope is a high-performance C++ command-line profiling and diagnostics utility designed for Linux systems engineering. It provides precise visibility into system startup latency by decomposing boot phases, mapping systemd service dependencies into a Directed Acyclic Graph (DAG), computing the critical path to boot targets (e.g., `graphical.target`), and flagging performance bottlenecks with actionable systemd mitigation recommendations.

### 2. Problem Statement
Modern Linux startup relies on systemd parallelization, socket activation, and declarative dependency ordering (`After=`, `Requires=`, `Wants=`). However, unexpected boot delays frequently arise from:
1. Blocking synchronous waits (e.g., `NetworkManager-wait-online.service`, `systemd-udev-settle.service`).
2. Heavy startup daemons on the boot critical path (e.g., snap squashfs loop mounts, cloud metadata polling).
3. Complex transitive dependency chains obscuring which single service delays the ultimate target.

Traditional tools like `systemd-analyze blame` show raw durations without dependency context—a 10-second background service running in parallel may not delay boot at all, whereas a 2-second service on the critical path directly blocks the user login screen. BootScope bridges this gap.

### 3. Key Functional Requirements (FR)
- **FR-01: Phase Breakdown Extraction**: Query and parse boot phases (Firmware/UEFI, Bootloader, Linux Kernel, Initrd, Userspace).
- **FR-02: Service Blame Ingestion**: Interrogate `systemd-analyze blame` and parse execution durations and memory/CPU footprint.
- **FR-03: Dependency Graph Construction**: Construct in-memory DAG modeling dependencies (`Requires`, `Wants`, `After`, `Before`).
- **FR-04: Critical Path Dynamic Programming**: Identify the longest sequential dependency chain dictating minimum boot time.
- **FR-05: Cycle & Deadlock Detection**: Run Kahn's topological sort to verify DAG acyclicity and flag circular dependencies.
- **FR-06: Bottleneck Diagnostic Engine**: Compare service durations against configurable warning thresholds (default 1.0s, critical 3.0s).
- **FR-07: Actionable Mitigation Guidance**: Provide exact Linux commands (e.g., `systemctl mask`, socket activation config) to resolve identified bottlenecks.
- **FR-08: Multi-Format Reporting**: Output results to ANSI terminal, Markdown, JSON, and standalone HTML.

### 4. Non-Functional Requirements (NFR)
- **Zero External Runtime Dependencies**: Pure C++17 utilizing standard POSIX interfaces (`/proc`, `/sys`, `popen`).
- **Execution Overhead**: Complete audit execution in under 250ms on standard Linux hardware.
- **Portability**: Compatible with Ubuntu, Debian, Fedora, Arch, CentOS Stream, and embedded Linux running systemd.
