# BootScope Diagnostic Report

## System Profile
- **OS**: Ubuntu 26.04.1 LTS
- **Kernel**: `7.0.0-38-generic` (`aarch64`)
- **CPU**:  (4 cores)
- **RAM**: 3383 MB
- **Firmware**: UEFI

## Boot Phase Timings

| Phase | Duration | Percentage |
| :--- | :--- | :--- |
| Firmware | 0.0ms | 0.0% |
| Loader | 0.0ms | 0.0% |
| Kernel | 203.0ms | 3.8% |
| Initrd | 1.356s | 25.2% |
| Userspace | 3.812s | 71.0% |
| **Total** | **5.371s** | 100.0% |

## Critical Path

Critical Path Duration: **5.197s**

1. `fwupd-refresh.service`

## Bottlenecks & Optimization Advice

### `fwupd-refresh.service` (5.197s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status fwupd-refresh.service`

### `fwupd.service` (2.594s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status fwupd.service`

### `dev-rfkill.device` (1.782s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-rfkill.device`

### `sys-devices-virtual-misc-rfkill.device` (1.782s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-virtual-misc-rfkill.device`

### `dev-ttyprintk.device` (1.624s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-ttyprintk.device`

### `sys-devices-virtual-tty-ttyprintk.device` (1.624s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-virtual-tty-ttyprintk.device`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `sys-devices-pci0000` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-pci0000`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-vda3.device` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-vda3.device`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.593s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `sys-devices-pci0000` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-pci0000`

### `dev-vda1.device` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-vda1.device`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.586s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.585s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.575s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.575s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.575s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `sys-devices-pci0000` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-pci0000`

### `dev-disk-by` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-vda2.device` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-vda2.device`

### `dev-disk-by` (1.574s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.568s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-vda.device` (1.568s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-vda.device`

### `dev-disk-by` (1.568s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `sys-devices-pci0000` (1.568s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-pci0000`

### `dev-disk-by` (1.568s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `dev-disk-by` (1.568s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status dev-disk-by`

### `sys-module-fuse.device` (1.556s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-module-fuse.device`

### `sys-module-configfs.device` (1.553s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-module-configfs.device`

### `sys-devices-pci0000` (1.508s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-devices-pci0000`

### `sys-subsystem-net-devices-enp0s1.device` (1.508s)
- **Diagnosis**: Service duration exceeds configured threshold (1.000000s).
- **Recommendation**: Evaluate whether this service can use systemd socket activation or Type=idle.
- **Remediation**: `systemctl status sys-subsystem-net-devices-enp0s1.device`

### `snapd.seeded.service` (1.095s)
- **Diagnosis**: Snap loopback squashfs mounting and AppArmor rule compilation delay boot.
- **Recommendation**: Disable snapd.seeded.service or migrate heavily used utilities to native packages.
- **Remediation**: `sudo systemctl disable snapd.seeded.service`

### `snapd.service` (1.047s)
- **Diagnosis**: Snap loopback squashfs mounting and AppArmor rule compilation delay boot.
- **Recommendation**: Disable snapd.seeded.service or migrate heavily used utilities to native packages.
- **Remediation**: `sudo systemctl disable snapd.seeded.service`

