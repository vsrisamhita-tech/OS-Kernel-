# CYBERLAB OS: System Architecture

## 1. Dual-Architecture Diagram
```
+---------------------------------------------------------------------------+
|                          CYBERLAB OS PLATFORM                             |
+---------------------------------------------------------------------------+
|                                                                           |
|   +------------------------------------+   +--------------------------+   |
|   |          DEBIAN MICROOS            |   |          RUSTOS          |   |
|   |         (Live Linux Host)          |   |   (Bare-Metal 32-bit)    |   |
|   +------------------------------------+   +--------------------------+   |
|   | - Linux Kernel 6.x (vmlinuz)       |   | - Freestanding #![no_std]|   |
|   | - Compressed SquashFS / Initramfs  |   | - Custom GDT & TSS Stack |   |
|   | - X11 Graphical Server             |   | - Custom IDT & Exceptions|   |
|   | - Top Panel & Modern Floating Dock |   | - 8259 PIC Remapped IRQs |   |
|   | - Modular Wi-Fi Manager (nmcli)    |   | - PIT Timer & Keyboard   |   |
|   | - Retro Snake Arcade Engine        |   | - VGA 0xb8000 Text Frame |   |
|   | - Academic Research Lab Engine     |   | - Interactive Shell      |   |
|   +------------------------------------+   +--------------------------+   |
|                     |                                    |                |
|                     +-----------------+------------------+                |
|                                       |                                   |
|                +-----------------------------------------+                |
|                |   ORACLE VIRTUALBOX 7.x VIRTUALIZATION  |                |
|                |   (VMSVGA, 2 vCPUs, 2GB RAM, NAT Net)   |                |
|                +-----------------------------------------+                |
|                                       |                                   |
|                +-----------------------------------------+                |
|                |      WINDOWS 11 / x86_64 HOST HARDWARE  |                |
|                +-----------------------------------------+                |
+---------------------------------------------------------------------------+
```

## 2. Kernel and Subsystem Responsibilities
- **Debian MicroOS**: Serves as the complete user-facing research and networking platform. It hosts the graphical environment, active NetworkManager connection daemon, and the empirical simulation runtime.
- **RustOS**: Serves as the bare-metal demonstrator, proving low-level architectural control over segmentation, interrupt descriptor tables, programmable interrupt controllers, and timer-driven task switches without relying on standard runtime libraries.
