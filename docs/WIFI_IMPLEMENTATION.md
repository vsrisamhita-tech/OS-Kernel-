# Modular Wi-Fi Subsystem Implementation

## 1. Architectural Overview
The CYBERLAB OS Wi-Fi subsystem bridges low-level Linux networking stacks (`wpa_supplicant`, `iw`, kernel 802.11 mac80211 subsystems) with user-space GTK/terminal quick settings panels.

```
+-------------------------------------------------------------------+
|               CYBERLAB Wi-Fi Desktop Quick Settings               |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|             wifi_manager.c / wifi_ui.c / wifi_manager.py          |
|    - SSID Discovery Parser                                        |
|    - Signal Strength dBm Normalizer                               |
|    - WPA2/WPA3 Password Dialog                                    |
|    - VirtualBox Environment Detector                              |
+-------------------------------------------------------------------+
                                  |
                   +--------------+---------------+
                   |                              |
                   v                              v
+------------------------------------+  +----------------------------+
|         NetworkManager (nmcli)      |  |  VirtualBox NAT Adapter   |
|  - Active 802.11 Rescan            |  |  - Detects eth0 interface  |
|  - D-Bus Authentication Dispatcher |  |  - Informs user of NAT     |
|  - Profile Persistence             |  |  - Diagnostic Notice       |
+------------------------------------+  +----------------------------+
```

## 2. Dynamic Hardware Detection
To prevent false UI demonstrations, `wifi_manager.c` executes an active environment probe:
```c
bool in_vbox = detect_virtualbox_environment();
bool has_wifi = is_wifi_hardware_present();
```
When running inside VirtualBox without a passed-through physical USB Wi-Fi dongle, the manager accurately reports:
```
[CONNECTED] VirtualBox NAT Ethernet Adapter (eth0)
Status: Internet Online via Virtual Machine Host Adapter
[HARDWARE DIAGNOSTIC NOTICE]
Physical Wi-Fi hardware not detected. VirtualBox provides virtual Ethernet.
```
When running on physical hardware with an Intel/Broadcom/Atheros wireless chip, it invokes `nmcli device wifi rescan` and displays available SSIDs with dBm signal strength bars.
