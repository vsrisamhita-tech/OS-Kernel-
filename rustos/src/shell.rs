use crate::{print, println};
use spin::Mutex;

const BUFFER_SIZE: usize = 64;
static INPUT_BUFFER: Mutex<InputBuffer> = Mutex::new(InputBuffer {
    buffer: [0; BUFFER_SIZE],
    len: 0,
});

struct InputBuffer {
    buffer: [u8; BUFFER_SIZE],
    len: usize,
}

pub fn print_prompt() {
    print!("cyberlab-rustos> ");
}

pub fn handle_char(c: char) {
    let mut ib = INPUT_BUFFER.lock();
    match c {
        '\n' => {
            println!();
            execute_command(&ib.buffer[..ib.len]);
            ib.len = 0;
            print_prompt();
        }
        '\x08' => {
            // Backspace
            if ib.len > 0 {
                ib.len -= 1;
                print!("\x08 \x08");
            }
        }
        c if c.is_ascii() && !c.is_ascii_control() => {
            if ib.len < BUFFER_SIZE - 1 {
                ib.buffer[ib.len] = c as u8;
                ib.len += 1;
                print!("{}", c);
            }
        }
        _ => {}
    }
}

fn execute_command(cmd_bytes: &[u8]) {
    if let Ok(cmd) = core::str::from_utf8(cmd_bytes) {
        let trimmed = cmd.trim();
        match trimmed {
            "" => {}
            "help" => {
                println!("CYBERLAB RustOS Kernel Shell v0.1.0");
                println!("Available commands:");
                println!("  help      - Display this list of kernel commands");
                println!("  info      - System architecture & interrupt status");
                println!("  deadlock  - Summarize Safe-State Research Lab findings");
                println!("  clear     - Clear VGA text screen buffer");
                println!("  reboot    - Reset processor via 8042 keyboard controller");
                println!("  panic     - Trigger a test kernel panic");
            }
            "info" => {
                println!("Kernel: RustOS 32-bit Freestanding Kernel");
                println!("Architecture: x86 Protected Mode (GDT loaded, IDT remapped)");
                println!("Hardware: 8259 PIC (IRQs 32-47), PIT Timer @ 18.2Hz, PS/2 Keyboard");
                println!("Developer: CYBERLAB OS Engineering Team");
            }
            "deadlock" => {
                println!("=== Safe-State Analysis Lab Findings ===");
                println!("Author: Vuppalapati Sri Samhita (USN: 25BTRCL285)");
                println!("Key Result: Under pessimistic declarations (g=0.75),");
                println!("only ~0.7% to 4.4% of Banker's refusals were true deadlocks.");
                println!("Avoidance is conservative by a factor of >20x!");
            }
            "clear" => {
                crate::vga_buffer::WRITER.lock().clear_screen();
            }
            "panic" => {
                panic!("User requested test kernel panic!");
            }
            "reboot" => {
                println!("Rebooting CPU via 8042 reset line...");
                unsafe {
                    use x86_64::instructions::port::Port;
                    let mut port = Port::new(0x64);
                    port.write(0xFEu8);
                }
            }
            unknown => {
                println!("Unknown command: '{}'. Type 'help' for available commands.", unknown);
            }
        }
    }
}
