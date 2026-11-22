#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]

use core::panic::PanicInfo;

mod gdt;
mod interrupts;
mod shell;
mod vga_buffer;

#[no_mangle]
pub extern "C" fn _start() -> ! {
    vga_buffer::WRITER.lock().clear_screen();

    println!("========================================================");
    println!("     CYBERLAB RustOS - 32-bit Bare-Metal Microkernel    ");
    println!("     Safe-State Analysis & Dual-OS Engineering Lab     ");
    println!("========================================================");

    print!("[INFO] Initializing Global Descriptor Table (GDT)... ");
    gdt::init();
    println!("OK");

    print!("[INFO] Initializing Interrupt Descriptor Table (IDT)... ");
    interrupts::init_idt();
    println!("OK");

    print!("[INFO] Initializing 8259 PIC (Remapping IRQs 32-47)... ");
    unsafe { interrupts::PICS.lock().initialize() };
    println!("OK");

    print!("[INFO] Enabling Hardware Interrupts (STI)... ");
    x86_64::instructions::interrupts::enable();
    println!("OK");

    println!("[INFO] Kernel boot completed successfully.");
    println!("Type 'help' to see shell commands, or 'deadlock' for lab findings.\n");

    shell::print_prompt();

    loop {
        x86_64::instructions::hlt();
    }
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    println!("\n\n\x1b[31;1mKERNEL PANIC: {}\x1b[0m", info);
    loop {
        x86_64::instructions::hlt();
    }
}
