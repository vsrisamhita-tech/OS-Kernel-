# RustOS: 32-bit Bare-Metal Microkernel

A freestanding x86 operating system microkernel written in Rust (`#![no_std]`, `#![no_main]`).

## Architecture & Subsystems
- **VGA Text Driver**: Directly maps to physical video memory buffer `0xb8000`, providing 80x25 terminal output with VT-style colors, line scrolling, and atomic Mutex locking.
- **Custom GDT**: Initializes segment registers and loads a 64-bit Task State Segment (TSS) with a dedicated Interrupt Stack Table (IST) for double fault containment.
- **Custom IDT**: Hardware exception and external IRQ dispatch table handling CPU interrupts.
- **8259 PIC Remapping**: Shifts master PIC (IRQs 0-7) to CPU vectors 32-39 and slave PIC (IRQs 8-15) to vectors 40-47 to avoid collision with x86 CPU exception traps (0-31).
- **PIT Timer**: Programmable Interval Timer configured on IRQ 0 for tick scheduling.
- **Keyboard Driver**: Scancode Set 1 decoding from PS/2 Port `0x60`, feeding characters into an asynchronous line editor buffer.
- **Interactive Shell**: Built-in shell with `help`, `info`, `deadlock`, `clear`, `reboot`, and `panic` commands.

## Build Instructions
```bash
cargo build --target x86_64-unknown-none --release
```
