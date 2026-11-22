#!/bin/bash
set -e

BUILD_DIR="/tmp/cyberlab_build"
ISO_OUT="/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/build"
mkdir -p "$BUILD_DIR"
mkdir -p "$ISO_OUT"

echo "=== CYBERLAB MicroOS Bootable ISO Generator with Full Terminal Support ==="

# 1. Base ISO
if [ ! -f /tmp/CorePlus-current.iso ]; then
    echo "[1/6] Downloading base ISO..."
    curl -L -o /tmp/CorePlus-current.iso http://tinycorelinux.net/15.x/x86/release/CorePlus-current.iso
fi

# 2. Extract base ISO contents
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
echo "[4/6] Injecting CYBERLAB OS custom binaries & terminal environment..."

cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/research-lab/deadlock-lab "$BUILD_DIR/initrd/usr/local/bin/deadlock-lab"
cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/scripts/snake-game "$BUILD_DIR/initrd/usr/local/bin/snake-game"
cp /mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/wifi/wifi-manager "$BUILD_DIR/initrd/usr/local/bin/wifi-manager"
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/"*

# Create symlinks using relative paths
cd "$BUILD_DIR/initrd/usr/local/bin"
ln -sf wifi-manager wifi
ln -sf deadlock-lab deadlock
ln -sf deadlock-lab research-lab
ln -sf snake-game snake
cd "$BUILD_DIR/initrd"

# Create /usr/local/bin/help
cat << 'EOF' > "$BUILD_DIR/initrd/usr/local/bin/help"
#!/bin/sh
cat << 'HELP_BANNER'

========================================================================
     CYBERLAB OS :: DUAL ARCHITECTURE ENGINEERING TERMINAL
   Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?
   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
========================================================================
  [AVAILABLE COMMANDS]:
    wifi                     - Open Wi-Fi Manager & VirtualBox NAT Status
    wifi scan                - Scan surrounding 802.11 access points
    wifi status              - Show active connection diagnostics
    wifi connect <SSID> [pw] - Authenticate and connect to Wi-Fi
    wifi disconnect          - Disconnect from wireless network
    deadlock-lab             - Run Safe-State Research Benchmark (Tables 1-5)
    snake                    - Launch Retro Snake Arcade Game
    help                     - Display this command manual
    ifconfig / ip a          - View all active network adapter interfaces
    ping 8.8.8.8             - Test live Internet connectivity
    top / ps                 - Monitor process and memory usage
    reboot / poweroff        - Power and system reset controls
========================================================================

HELP_BANNER
EOF
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/"*

# Setup motd banner
cat << 'EOF' > "$BUILD_DIR/initrd/etc/cyberlab_motd"

========================================================================
     CYBERLAB OS :: DUAL ARCHITECTURE ENGINEERING TERMINAL
   Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?
   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
========================================================================
  Type 'help' to see all available commands.
  Type 'wifi' for the Wi-Fi Manager & VirtualBox Network Status.
  Type 'deadlock-lab' to run the research paper benchmark.
  Type 'snake' to play the Retro Snake Arcade Game.
========================================================================

EOF

# Setup profile and ashrc
cat << 'EOF' >> "$BUILD_DIR/initrd/etc/skel/.profile"
cat /etc/cyberlab_motd
export PATH=/usr/local/bin:/usr/bin:/bin:/sbin:/usr/sbin
alias wifi="/usr/local/bin/wifi"
alias deadlock="/usr/local/bin/deadlock-lab"
alias snake="/usr/local/bin/snake-game"
alias help="/usr/local/bin/help"
EOF

cat << 'EOF' >> "$BUILD_DIR/initrd/etc/profile"
export PATH=/usr/local/bin:/usr/bin:/bin:/sbin:/usr/sbin
alias wifi="/usr/local/bin/wifi"
alias deadlock="/usr/local/bin/deadlock-lab"
alias snake="/usr/local/bin/snake-game"
alias help="/usr/local/bin/help"
EOF

# Setup user tc home directory with desktop shortcuts and autostart
mkdir -p "$BUILD_DIR/initrd/home/tc/.local/share/applications"
mkdir -p "$BUILD_DIR/initrd/home/tc/Desktop"
mkdir -p "$BUILD_DIR/initrd/home/tc/.X.d"
mkdir -p "$BUILD_DIR/initrd/usr/local/etc/X.d"

# Configure bottom macOS-style floating dock (.wbar)
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.wbar"
i: /usr/local/share/wbar/osxbarback.png
t: /usr/local/lib/X11/fonts/TTF/luxisr/11
c: wbar -bpress -pos bottom -zoomf 2 -isize 36 -bdist 10

i: /usr/local/share/pixmaps/aterm.png
t: Terminal
c: aterm -title "CYBERLAB OS :: Terminal Console" -geometry 105x34+60+40 -bg black -fg green -cr white -e /bin/sh -l

i: /usr/local/share/pixmaps/aterm.png
t: Wi-Fi Settings
c: aterm -title "CYBERLAB - Wi-Fi Quick Settings" -geometry 70x26 -hold -e /usr/local/bin/wifi

i: /usr/local/share/pixmaps/aterm.png
t: Deadlock Lab
c: aterm -title "CYBERLAB - Deadlock Research Lab" -geometry 95x35 -hold -e /usr/local/bin/deadlock-lab

i: /usr/local/share/pixmaps/aterm.png
t: Snake Arcade
c: aterm -title "Retro Snake Arcade" -geometry 45x28 -e /usr/local/bin/snake-game
EOF

# Desktop shortcut files
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/Terminal.desktop"
[Desktop Entry]
Name=CyberLab Terminal
Comment=Engineering Console
Exec=aterm -title "CYBERLAB OS :: Terminal Console" -geometry 105x34+60+40 -bg black -fg green -cr white -e /bin/sh -l
Icon=aterm
Terminal=false
Type=Application
EOF

cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/WiFi_Settings.desktop"
[Desktop Entry]
Name=Wi-Fi Quick Settings
Comment=Wireless Manager and VirtualBox Network Status
Exec=aterm -title "CYBERLAB - Wi-Fi Quick Settings" -geometry 70x26 -hold -e /usr/local/bin/wifi
Icon=aterm
Terminal=false
Type=Application
EOF

cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/Deadlock_Lab.desktop"
[Desktop Entry]
Name=Deadlock Research Lab
Comment=Safe-State Analysis Benchmark by Vuppalapati Sri Samhita
Exec=aterm -title "CYBERLAB - Deadlock Research Lab" -geometry 95x35 -hold -e /usr/local/bin/deadlock-lab
Icon=aterm
Terminal=false
Type=Application
EOF

cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/Desktop/Snake_Arcade.desktop"
[Desktop Entry]
Name=Retro Snake Arcade
Comment=Classic Retro Snake Game
Exec=aterm -title "Retro Snake Arcade" -geometry 45x28 -e /usr/local/bin/snake-game
Icon=aterm
Terminal=false
Type=Application
EOF

chmod +x "$BUILD_DIR/initrd/home/tc/Desktop/"*.desktop 2>/dev/null || true

# AUTOSTART TERMINAL ON DESKTOP LOAD
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.X.d/00-terminal"
#!/bin/sh
aterm -title "CYBERLAB OS :: Terminal Console" -geometry 105x34+60+40 -bg black -fg green -cr white -e /bin/sh -l &
EOF
chmod +x "$BUILD_DIR/initrd/home/tc/.X.d/00-terminal"
cp "$BUILD_DIR/initrd/home/tc/.X.d/00-terminal" "$BUILD_DIR/initrd/usr/local/etc/X.d/00-terminal"

chown -R 1001:50 "$BUILD_DIR/initrd/home/tc" 2>/dev/null || true

# 5. Repack initramfs core.gz
echo "[5/6] Repacking custom core.gz..."
find . | cpio -o -H newc 2>/dev/null | gzip -9 > "$BUILD_DIR/iso/boot/core.gz"
cd "$BUILD_DIR"

# Configure isolinux.cfg
cat << 'EOF' > "$BUILD_DIR/iso/boot/isolinux/isolinux.cfg"
default cyberlab
timeout 10
prompt 0

label cyberlab
	kernel /boot/vmlinuz
	initrd /boot/core.gz
	append loglevel=3 cde showapps waitusb=5 vga=791 desktop=flwm_topside
EOF

# 6. Generate final bootable hybrid ISO
echo "[6/6] Generating final bootable hybrid ISO..."
xorriso -as mkisofs -l -J -R -V "CYBERLAB_OS" -no-emul-boot -boot-load-size 4 \
    -boot-info-table -b boot/isolinux/isolinux.bin -c boot/isolinux/boot.cat \
    -o "$ISO_OUT/cyberlab_microos.iso" "$BUILD_DIR/iso" 2>/dev/null || \
xorriso -as mkisofs -l -J -R -V "CYBERLAB_OS" -no-emul-boot -boot-load-size 4 \
    -boot-info-table -b boot/isolinux/isolinux.bin -c boot/isolinux/boot.cat \
    -o "$ISO_OUT/cyberlab_microos.iso" "$BUILD_DIR/iso"

ls -lh "$ISO_OUT/cyberlab_microos.iso"
echo "=== Build Complete! Generated ISO at $ISO_OUT/cyberlab_microos.iso ==="
