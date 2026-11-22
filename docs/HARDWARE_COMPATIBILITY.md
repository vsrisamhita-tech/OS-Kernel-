# Hardware Compatibility & Virtualization Guide

## 1. Physical vs. Virtual Hardware Support Matrix

| Hardware Component | Physical Laptop/PC | Oracle VirtualBox 7.x | Notes |
| :--- | :--- | :--- | :--- |
| **CPU Architecture** | x86_64 (Intel Core, AMD Ryzen) | Virtualized 64-bit CPU (2 cores) | Hardware virtualization (VT-x / AMD-V) enabled |
| **RAM Footprint** | >= 1 GB DDR3/DDR4/DDR5 | 2048 MB assigned | MicroOS runs entirely in RAM |
| **Display / GPU** | Intel UHD/Iris, AMD Radeon, NVIDIA | VBoxSVGA / VBoxVGA (128 MB VRAM) | Auto-detects 1024x768 / 1920x1080 resolution |
| **Wireless Networking** | Intel AX200/210, Atheros, Realtek | Virtual Intel PRO/1000 MT NAT | In VirtualBox, guest sees virtual Ethernet (`eth0`) |
| **USB Wi-Fi Passthrough** | Supported natively | Supported via VirtualBox USB filter | Connect physical USB Wi-Fi dongle directly to VM |
| **Audio** | Realtek ALC / Intel HDA | ICH AC97 / Intel HD Audio | Supported via ALSA backend |
| **Storage / Boot** | USB Flash Drive (Rufus, Ventoy) | Virtual IDE / SATA Controller | Hybrid ISO boots on both BIOS and UEFI |

## 2. Why VirtualBox Hides Host Wi-Fi SSIDs
VirtualBox virtualizes a standard PCIe Ethernet controller (`eth0`) to guest operating systems and translates guest network packets to the host OS socket layer via Network Address Translation (NAT). Consequently, the guest Linux kernel has no direct access to the physical wireless radio unless a USB wireless adapter is explicitly attached through **Devices -> USB -> Select Wireless Dongle**.
