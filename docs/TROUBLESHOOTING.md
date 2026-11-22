# Troubleshooting & Operational Guide

## 1. VirtualBox Virtualization Issues
### Issue: "VirtualBox not found in PATH"
- **Solution**: The 1-click launcher `Start-MicroOS.bat` checks both the system `PATH` and default location `C:\Program Files\Oracle\VirtualBox\VBoxManage.exe`. Ensure VirtualBox 7.x is installed.

### Issue: "VT-x / AMD-V is not available in host"
- **Solution**: Enable Intel Virtualization Technology (VT-x) or AMD SVM in your computer's BIOS/UEFI settings, and disable Windows Hyper-V features if conflict occurs.

## 2. Networking Diagnostics
### Issue: "Nearby Wi-Fi networks do not appear inside VirtualBox"
- **Explanation**: This is normal virtualization behavior. VirtualBox routes internet through a virtual Ethernet adapter (`eth0`).
- **Solution**: To scan physical Wi-Fi SSIDs from within the VM, plug in a USB Wi-Fi dongle and attach it to the VM via **VirtualBox Menu -> Devices -> USB -> Select Wireless Adapter**.

## 3. Screen Resolution Adjustment
- MicroOS defaults to standard VESA/VBoxSVGA modes. You can resize the VirtualBox window dynamically or adjust resolution in terminal:
```bash
xrandr -s 1280x800
```
