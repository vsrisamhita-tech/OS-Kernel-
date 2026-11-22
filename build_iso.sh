#!/bin/bash
set -e

BUILD_DIR="/tmp/cyberlab_build"
ISO_OUT="/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/build"
mkdir -p "$BUILD_DIR"
mkdir -p "$ISO_OUT"

echo "=== CYBERLAB MicroOS Bootable ISO Generator ==="

# 1. Download CorePlus base if not already cached
if [ ! -f /tmp/CorePlus-current.iso ]; then
    echo "[1/6] Downloading lightweight desktop live base (249 MB)..."
    curl -L -o /tmp/CorePlus-current.iso http://tinycorelinux.net/15.x/x86/release/CorePlus-current.iso
else
    echo "[1/6] Base ISO cached at /tmp/CorePlus-current.iso"
fi

# 2. Extract ISO contents
echo "[2/6] Extracting base ISO contents..."
rm -rf "$BUILD_DIR/iso" "$BUILD_DIR/initrd"
mkdir -p "$BUILD_DIR/iso"
xorriso -osirrox on -indev /tmp/CorePlus-current.iso -extract / "$BUILD_DIR/iso" 2>/dev/null

chmod -R u+w "$BUILD_DIR/iso"

# 3. Unpack core.gz initramfs
echo "[3/6] Unpacking live initramfs..."
mkdir -p "$BUILD_DIR/initrd"
cd "$BUILD_DIR/initrd"
zcat "$BUILD_DIR/iso/boot/core.gz" | cpio -idm 2>/dev/null

# 4. Inject CYBERLAB OS custom binaries & desktop configuration
echo "[4/6] Injecting CYBERLAB OS custom binaries & desktop configuration..."

cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/research-lab/deadlock-lab "$BUILD_DIR/initrd/usr/local/bin/deadlock-lab"
cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/scripts/snake-game "$BUILD_DIR/initrd/usr/local/bin/snake-game"
cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/wifi/wifi-manager "$BUILD_DIR/initrd/usr/local/bin/wifi-manager"
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/"*

# Setup user tc home directory with desktop shortcuts and autostart
mkdir -p "$BUILD_DIR/initrd/home/tc/.local/share/applications"
mkdir -p "$BUILD_DIR/initrd/home/tc/Desktop"
mkdir -p "$BUILD_DIR/initrd/home/tc/.X.d"

# 1. Desktop shortcut: Deadlock Research Lab
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/Deadlock_Lab.desktop"
[Desktop Entry]
Name=Deadlock Research Lab
Comment=Safe-State Analysis Benchmark by Vuppalapati Sri Samhita
Exec=aterm -title "CYBERLAB - Deadlock Research Lab" -geometry 95x35 -hold -e /usr/local/bin/deadlock-lab
Icon=applications-science
Terminal=false
Type=Application
Categories=Development;Science;
EOF

# 2. Desktop shortcut: Retro Snake Arcade
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/Snake_Arcade.desktop"
[Desktop Entry]
Name=Retro Snake Arcade
Comment=Classic Retro Snake Game
Exec=aterm -title "Retro Snake Arcade" -geometry 45x28 -e /usr/local/bin/snake-game
Icon=applications-games
Terminal=false
Type=Application
Categories=Game;
EOF

# 3. Desktop shortcut: Wi-Fi Quick Settings
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/WiFi_Settings.desktop"
[Desktop Entry]
Name=Wi-Fi Quick Settings
Comment=Wireless Manager and VirtualBox Network Status
Exec=aterm -title "CYBERLAB - Wi-Fi Quick Settings" -geometry 65x25 -hold -e /usr/local/bin/wifi-manager
Icon=network-wireless
Terminal=false
Type=Application
Categories=Settings;Network;
EOF

# 4. Desktop shortcut: CyberLab Terminal
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/CyberLab_Terminal.desktop"
[Desktop Entry]
Name=CyberLab Terminal
Comment=Engineering Command Prompt
Exec=aterm -title "CYBERLAB Engineering Console" -geometry 80x25
Icon=utilities-terminal
Terminal=false
Type=Application
Categories=System;
EOF

chmod +x "$BUILD_DIR/initrd/home/tc/Desktop/"*.desktop
chmod +x "$BUILD_DIR/initrd/home/tc/.local/share/applications/"*.desktop 2>/dev/null || true

# Autostart greeting and display lab banner
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.X.d/cyberlab_welcome"
#!/bin/sh
aterm -title "CYBERLAB OS - Dual Architecture Platform" -geometry 95x32 -hold -e /usr/local/bin/deadlock-lab &
EOF
chmod +x "$BUILD_DIR/initrd/home/tc/.X.d/cyberlab_welcome"

chown -R 1001:50 "$BUILD_DIR/initrd/home/tc" 2>/dev/null || true

# 5. Repack initramfs core.gz
echo "[5/6] Repacking custom core.gz..."
find . | cpio -o -H newc 2>/dev/null | gzip -9 > "$BUILD_DIR/iso/boot/core.gz"
cd "$BUILD_DIR"

# Configure isolinux.cfg for fast, branded 1-second boot
cat << 'EOF' > "$BUILD_DIR/iso/boot/isolinux/isolinux.cfg"
default cyberlab
timeout 10
prompt 0

label cyberlab
	kernel /boot/vmlinuz
	initrd /boot/core.gz
	append loglevel=3 cde showapps waitusb=5 vga=791 desktop=flwm_top
EOF

# 6. Generate hybrid bootable ISO using xorriso
echo "[6/6] Generating final bootable hybrid ISO..."
xorriso -as mkisofs -l -J -R -V "CYBERLAB_OS" -no-emul-boot -boot-load-size 4 \
    -boot-info-table -b boot/isolinux/isolinux.bin -c boot/isolinux/boot.cat \
    -isohybrid-mbr /usr/lib/ISOLINUX/isohdpfx.bin \
    -o "$ISO_OUT/cyberlab_microos.iso" "$BUILD_DIR/iso" 2>/dev/null || \
xorriso -as mkisofs -l -J -R -V "CYBERLAB_OS" -no-emul-boot -boot-load-size 4 \
    -boot-info-table -b boot/isolinux/isolinux.bin -c boot/isolinux/boot.cat \
    -o "$ISO_OUT/cyberlab_microos.iso" "$BUILD_DIR/iso"

ls -lh "$ISO_OUT/cyberlab_microos.iso"
echo "=== Build Complete! Generated ISO at $ISO_OUT/cyberlab_microos.iso ==="
