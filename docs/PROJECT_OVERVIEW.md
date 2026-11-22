# CYBERLAB OS: Dual Architecture Operating Systems Engineering Platform

## 1. Executive Summary
**CYBERLAB OS** is a hybrid Dual-OS engineering platform unifying an ultra-lightweight Live Linux distribution (**Debian/Core MicroOS**, 249 MB ISO footprint) with a bare-metal 32-bit freestanding kernel (**RustOS**) and an integrated **Academic Research Paper Experimental Lab**.

Developed specifically for academic research on:
> **"Safe-State Analysis: How Conservative Is Deadlock Avoidance?"**  
> **Author**: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)  
> **Institution**: Jain (Deemed-to-be University), Faculty of Engineering and Technology (FET)  
> **Course**: 25PCCS201 - Operating Systems Research Activity

## 2. Key Pillars
1. **Debian MicroOS Live Distribution**:
   - Packaged as a hybrid bootable ISO under 450 MB (`micro-os/build/cyberlab_microos.iso`, actual size: 249 MB).
   - Graphical desktop with top panel, floating application dock, and automated user auto-login.
   - Built-in Retro Snake Arcade game (`/usr/local/bin/snake-game`).
   - Integrated Deadlock Research Lab benchmark dashboard (`/usr/local/bin/deadlock-lab`).
2. **Modular Wireless Networking Interface**:
   - Dynamic SSID active discovery, signal dBm profiling, WPA2/WPA3 authentication.
   - Intelligent VirtualBox detection: gracefully flags virtualized NAT Ethernet interfaces (`eth0`) vs. physical 802.11 radios without false scanning artifacts.
3. **RustOS Freestanding 32-bit Microkernel**:
   - Built in pure Rust (`#![no_std]`, `#![no_main]`).
   - Custom GDT, IDT exception dispatching, 8259 PIC IRQ remapping, PIT timer tick scheduling, VGA text mode console, and an interactive kernel shell.
4. **1-Click VirtualBox Launcher (`Start-MicroOS.bat` / `Start-CyberLabOS.bat`)**:
   - Auto-detects Oracle VirtualBox on Windows host.
   - Provisions 2048 MB RAM, 2 CPUs, 128 MB VRAM, VBoxSVGA accelerator, and starts the VM in GUI mode with zero manual configuration.
