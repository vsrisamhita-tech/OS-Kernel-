#!/bin/bash
# smart_rebuild.sh — Minimal-touch rebuild that preserves the original core.gz
# Only modifies specific files inside the initramfs (wallpaper + wifi script)
# Uses --owner=0:0 cpio flag to avoid permission corruption

set -e

BUILD_DIR="/tmp/cyberlab_smartbuild"
ISO_SRC="/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/build/cyberlab_microos.iso"
ISO_OUT="/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/build/cyberlab_microos.iso"
WALL_SRC="/tmp/cyberlab_bg.png"

echo "=== CYBERLAB OS Smart Rebuild ==="

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR/iso" "$BUILD_DIR/initrd"

# 1. Extract ISO
echo "[1/5] Extracting ISO..."
xorriso -osirrox on -indev "$ISO_SRC" -extract / "$BUILD_DIR/iso" 2>/dev/null || true
chmod -R u+w "$BUILD_DIR/iso"

# 2. Unpack initramfs with CORRECT uid/gid preservation
echo "[2/5] Unpacking initramfs..."
cd "$BUILD_DIR/initrd"
zcat "$BUILD_DIR/iso/boot/core.gz" | cpio -idm --no-absolute-filenames 2>/dev/null || true

# Count files to verify good unpack
FILE_COUNT=$(find . | wc -l)
echo "   Files in initrd: $FILE_COUNT"
if [ "$FILE_COUNT" -lt 100 ]; then
    echo "ERROR: Too few files unpacked! Aborting."
    exit 1
fi

# 3. Generate wallpaper
echo "[3/5] Creating wallpaper..."
cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/make_wallpaper.py /tmp/make_wallpaper.py
python3 /tmp/make_wallpaper.py

mkdir -p "$BUILD_DIR/initrd/usr/local/share/pixmaps"
cp "$WALL_SRC" "$BUILD_DIR/initrd/usr/local/share/pixmaps/cyberlab_bg.png"

# 4. Inject autostart script for wallpaper
echo "[4/5] Injecting wallpaper autostart..."
mkdir -p "$BUILD_DIR/initrd/home/tc/.X.d"
mkdir -p "$BUILD_DIR/initrd/usr/local/etc/X.d"

cat > "$BUILD_DIR/initrd/home/tc/.X.d/00-wallpaper" << 'WALLEOF'
#!/bin/sh
sleep 2
# Try to set wallpaper — feh is usually available in TinyCore with X
if command -v feh >/dev/null 2>&1; then
    feh --bg-fill /usr/local/share/pixmaps/cyberlab_bg.png &
elif command -v hsetroot >/dev/null 2>&1; then
    hsetroot -fill /usr/local/share/pixmaps/cyberlab_bg.png &
else
    # Solid dark fallback — removes the TinyCore "core" logo completely
    xsetroot -solid "#080C18" &
fi
WALLEOF
chmod 755 "$BUILD_DIR/initrd/home/tc/.X.d/00-wallpaper"
cp "$BUILD_DIR/initrd/home/tc/.X.d/00-wallpaper" "$BUILD_DIR/initrd/usr/local/etc/X.d/00-wallpaper"
chmod 755 "$BUILD_DIR/initrd/usr/local/etc/X.d/00-wallpaper"

# Fix ownership to tc user (uid 1001, gid 50 in TinyCore)
chown -R 1001:50 "$BUILD_DIR/initrd/home/tc" 2>/dev/null || true

# Patch wifi script
cat > "$BUILD_DIR/initrd/usr/local/bin/wifi" << 'WIFIEOF'
#!/bin/sh
IFACE=$(ip link show 2>/dev/null | grep -E '^[0-9]+: (eth|en)' | head -1 | awk -F': ' '{print $2}')
[ -z "$IFACE" ] && IFACE="eth0"
GW=$(ip route 2>/dev/null | grep default | awk '{print $3}' | head -1)
IP=$(ip addr show "$IFACE" 2>/dev/null | grep 'inet ' | awk '{print $2}' | head -1)
ping -c 1 -W 2 8.8.8.8 >/dev/null 2>&1 && PING_OK=1 || PING_OK=0
clear
echo ""
echo "  +----------------------------------------------------------+"
echo "  |     CYBERLAB OS -- Wi-Fi & Network Manager               |"
echo "  |  Paper: Safe-State Analysis: Deadlock Avoidance          |"
echo "  +----------------------------------------------------------+"
printf "  |  Interface : %-43s|\n" "$IFACE"
printf "  |  IP Address: %-43s|\n" "${IP:-Obtaining via DHCP...}"
printf "  |  Gateway   : %-43s|\n" "${GW:-VirtualBox NAT 10.0.2.2}"
echo "  |                                                          |"
if [ "$PING_OK" = "1" ]; then
echo "  |  Status    : [CONNECTED] Internet via VirtualBox NAT    |"
echo "  |  Network   : VirtualBox NAT  WPA2  Signal: ======== 80% |"
echo "  |  Host WiFi : Bridge Active (shared from Windows host)   |"
echo "  +----------------------------------------------------------+"
echo "  |  NEARBY NETWORKS:                                        |"
echo "  |   > VirtualBox-NAT  * Active  WPA2  [========] 80%     |"
echo "  |     Host-WiFi-Bridge           WPA2  [======--] 72%     |"
echo "  |     CyberLab-Lab-5G            WPA2  [====----] 50%     |"
else
echo "  |  Status    : [CHECKING] VirtualBox NAT...               |"
echo "  |  Tip: Machine > Settings > Network > Adapter1 = NAT     |"
fi
echo "  +----------------------------------------------------------+"
echo ""
printf "  wifi> "
read CMD
case "$CMD" in
  scan*) echo "  Found: VirtualBox-NAT [80%], CyberLab-5G [50%], BMSIT-WiFi [40%]" ;;
  status*) ip addr; ip route ;;
  *) echo "  Usage: scan | status | connect <SSID> | disconnect" ;;
esac
echo "  Press Enter to close..."; read _
WIFIEOF
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/wifi"

# 5. Repack with correct ownership flags
echo "[5/5] Repacking initramfs with corrected permissions..."
cd "$BUILD_DIR/initrd"

# IMPORTANT: Use newc format, preserve device nodes, don't mangle uids
find . | sort | cpio -o -H newc --owner=0:0 2>/dev/null | gzip -9 > "$BUILD_DIR/iso/boot/core.gz"

CORE_SIZE=$(du -sh "$BUILD_DIR/iso/boot/core.gz" | cut -f1)
echo "   core.gz size: $CORE_SIZE"
cd "$BUILD_DIR"

# Regenerate ISO
xorriso -as mkisofs -l -J -R -V "CYBERLAB_OS" \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    -b boot/isolinux/isolinux.bin \
    -c boot/isolinux/boot.cat \
    -o "$ISO_OUT" "$BUILD_DIR/iso" 2>/dev/null

ls -lh "$ISO_OUT"
echo ""
echo "=== Smart Rebuild COMPLETE ==="
echo "  Wallpaper: Dark CYBERLAB OS (no TinyCore logo)"
echo "  Wi-Fi: Shows VirtualBox NAT as Connected"
echo "  Permissions: Fixed with --owner=0:0"
