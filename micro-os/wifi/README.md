# CYBERLAB OS - Wi-Fi Architecture & Subsystem

The CYBERLAB OS Wireless Networking subsystem provides an enterprise-grade, desktop-integrated Wi-Fi management suite tailored for both physical laptops and virtualization platforms (VirtualBox).

## Modules
- `wifi_manager.c`: Central state controller and environment detector.
- `wifi_scanner.c`: High-speed 802.11 active scanning engine interfacing with NetworkManager.
- `wifi_connector.c`: WPA2-Personal and WPA3 authentication handler.
- `wifi_ui.c`: Graphical quick settings panel and password authentication dialog.
- `wifi_notifications.c`: Desktop notification dispatcher via libnotify / D-Bus.
- `wifi_manager.py`: High-level Python GTK/Tkinter graphical frontend.

## Physical Hardware vs. VirtualBox Handling
- **Physical Machine**: Automatically interrogates Linux kernel wireless drivers via `iw`, `rfkill`, and `wpa_supplicant`. Scans SSIDs, displays signal strength in dBm, and conducts 4-way WPA handshakes.
- **VirtualBox Guest**: VirtualBox exposes a virtual Intel PRO/1000 or PCnet PCI Ethernet adapter (`eth0`) rather than passing raw 802.11 radios. The subsystem detects this and informs the user:
  > "Connected via VirtualBox NAT Virtual Ethernet. For physical Wi-Fi scanning, boot on bare metal or attach a USB Wi-Fi dongle."
