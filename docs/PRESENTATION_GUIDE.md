# Viva Voce & Defense Presentation Guide

**Candidate Name:** Vuppalapati Sri Samhita  
**USN:** 25BTRCL285 | **Branch:** CSE-AIML-E  
**Project:** CYBERLAB OS - Dual Architecture Platform & Safe-State Analysis Lab  
**Course:** 25PCCS201 - Operating Systems Research Activity

---

## 1. 2-Minute Elevator Pitch
> *"Respected Examiners, my project presents **CYBERLAB OS**, an end-to-end Dual-Architecture Operating Systems Engineering platform combined with an empirical research investigation into deadlock avoidance.*  
>
> *First, we developed **Debian MicroOS**, a bootable live distribution under 250 MB featuring a customized desktop, fully modular Wi-Fi management with VirtualBox NAT diagnostics, and a Retro Snake arcade game.*  
>
> *Second, we engineered **RustOS**, a bare-metal 32-bit microkernel written in freestanding Rust implementing custom GDT, IDT interrupt tables, 8259 PIC remapping, PIT scheduling, and an interactive shell.*  
>
> *Third, our academic research investigates a critical theoretical question: **'How Conservative is Deadlock Avoidance?'**. Using our built-in benchmark tool `/usr/local/bin/deadlock-lab`, we evaluated Dijkstra's Banker's algorithm against No Avoidance and All-at-Once Prevention over 4,800 simulation runs. We discovered that while Banker's guarantees 0% deadlocks, its refusals are 100% necessary only when processes use exactly what they declare ($g=1.0$). When claims are realistically pessimistic ($g=0.75$), over 95% of Banker's refusals were completely harmless. The entire system is packaged with a 1-click VirtualBox launcher and complete test reports."*

---

## 2. Walkthrough of Live Demonstrations
### Slide 1: Booting MicroOS in VirtualBox
- **Action**: Double click `Start-MicroOS.bat`.
- **Key Point**: Explain that the batch file uses `VBoxManage` to automatically configure 2GB RAM, dual virtual cores, VBoxSVGA acceleration, and boot the custom 249MB hybrid ISO.
- **Examiner Highlight**: Point out the desktop layout, the top panel, and the floating dock.

### Slide 2: Demonstrating the Wi-Fi Subsystem
- **Action**: Click the Wi-Fi icon or open `/usr/local/bin/wifi-manager`.
- **Key Point**: Explain how it detects VirtualBox virtual Ethernet (`eth0`) vs. physical Wi-Fi hardware.
- **Examiner Highlight**: "Notice it does not invent fake Wi-Fi networks in VirtualBox; instead, it accurately displays VirtualBox NAT Internet status and explains that physical 802.11 scanning requires USB dongle passthrough."

### Slide 3: Running the Academic Research Lab
- **Action**: Launch the "Deadlock Research Lab" icon on the desktop.
- **Key Point**: Show the generated Tables 1, 2, 4, and 5 in real time.
- **Examiner Highlight**: "Notice that at usage ratio $g=0.75$, the counterfactual check reveals only ~0.7% to 4.4% of refusals actually deadlocked. This proves the algorithm's over-conservativeness under pessimistic worst-case claims."

### Slide 4: RustOS Bare-Metal Kernel
- **Action**: Explain the freestanding kernel architecture in `rustos/`.
- **Key Point**: Explain how the GDT separates code/data segments, how the IDT dispatches IRQ 0 (PIT timer) and IRQ 1 (PS/2 keyboard), and how the VGA driver renders directly to `0xb8000`.
