# CYBERLAB OS

A dual‑architecture engineering platform consisting of:

- **MicroOS** – a lightweight live Linux distribution with a macOS‑styled GNOME desktop, real Wi‑Fi integration, and a Snake arcade game.
- **RustOS** – a 32‑bit bare‑metal OS kernel written in Rust.
- **Research Lab** – a benchmark suite implementing the paper *Safe‑State Analysis: How Conservative Is Deadlock Avoidance?*.
- **One‑click VirtualBox launcher** for quickly booting MicroOS.

All source code, build scripts, and documentation are provided in a ready‑to‑push Git repository.

## Repository Layout
```
CYBERLAB_OS/
├─ .gitignore
├─ README.md
├─ Start-MicroOS.bat
├─ micro-os/
│   ├─ build_clean.sh
│   ├─ wifi/
│   │   ├─ wifi_manager.py
│   │   └─ README.md
│   ├─ desktop/
│   │   └─ gnome_customization.sh
│   └─ scripts/
│       └─ install_snake.sh
├─ rustos/
│   ├─ Cargo.toml
│   ├─ linker.ld
│   └─ src/
│       └─ main.rs
├─ research-lab/
│   ├─ Cargo.toml
│   └─ src/
│       └─ main.rs
├─ docs/
│   ├─ PROJECT_OVERVIEW.md
│   ├─ ARCHITECTURE.md
│   ├─ DEADLOCK_ANALYSIS.md
│   ├─ PRESENTATION_GUIDE.md
│   ├─ WIFI_IMPLEMENTATION.md
│   ├─ HARDWARE_COMPATIBILITY.md
│   ├─ BOOT_BENCHMARKS.md
│   ├─ TESTING_REPORT.md
│   ├─ TROUBLESHOOTING.md
│   └─ VIVA_QUESTIONS.md
└─ screenshots/
```

Refer to the `docs/` files for detailed design, experiment results, and viva preparation.
