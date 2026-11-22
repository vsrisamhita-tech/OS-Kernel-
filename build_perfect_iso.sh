#!/bin/bash
set -e

echo "========================================================================"
echo "   CYBERLAB OS :: MASTER ISO BUILDER (Custom Dark Wallpaper + Wi-Fi)    "
echo "========================================================================"

BUILD_DIR="/tmp/cyberlab_perfect_build"
ISO_OUT="/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/micro-os/build"
SRC_DIR="/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS"

mkdir -p "$BUILD_DIR"
mkdir -p "$ISO_OUT"

# 1. Ensure Base CorePlus ISO exists
if [ ! -f /tmp/CorePlus-current.iso ]; then
    echo "[1/6] Downloading official CorePlus-current.iso..."
    curl -L -o /tmp/CorePlus-current.iso http://tinycorelinux.net/15.x/x86/release/CorePlus-current.iso
fi

# 2. Extract base ISO contents
echo "[2/6] Extracting base ISO contents..."
rm -rf "$BUILD_DIR/iso" "$BUILD_DIR/initrd"
mkdir -p "$BUILD_DIR/iso"
xorriso -osirrox on -indev /tmp/CorePlus-current.iso -extract / "$BUILD_DIR/iso" 2>/dev/null
chmod -R u+w "$BUILD_DIR/iso"

# 3. Unpack live initramfs as root (preserving all permissions)
echo "[3/6] Unpacking live initramfs..."
mkdir -p "$BUILD_DIR/initrd"
cd "$BUILD_DIR/initrd"
zcat "$BUILD_DIR/iso/boot/core.gz" | cpio -idm 2>/dev/null

echo "   Unpacked initramfs: $(find . | wc -l) files"

# 4. Inject CYBERLAB OS custom binaries & tools
echo "[4/6] Injecting CYBERLAB OS custom binaries, wallpaper, & UI environment..."
mkdir -p "$BUILD_DIR/initrd/usr/local/bin"
mkdir -p "$BUILD_DIR/initrd/usr/local/share/pixmaps"

# Copy 32-bit static binaries
cp "$SRC_DIR/research-lab/deadlock-lab" "$BUILD_DIR/initrd/usr/local/bin/deadlock-lab"
cp "$SRC_DIR/micro-os/scripts/snake-game" "$BUILD_DIR/initrd/usr/local/bin/snake-game"
cp "$SRC_DIR/micro-os/wifi/wifi-manager" "$BUILD_DIR/initrd/usr/local/bin/wifi-manager"
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/"*

# Relative symlinks
cd "$BUILD_DIR/initrd/usr/local/bin"
ln -sf wifi-manager wifi
ln -sf deadlock-lab deadlock
ln -sf deadlock-lab research-lab
ln -sf snake-game snake
cd "$BUILD_DIR/initrd"

# Copy generated custom dark wallpaper
cp "$SRC_DIR/cyberlab_wallpaper.png" "$BUILD_DIR/initrd/usr/local/share/pixmaps/cyberlab_wallpaper.png"
# Also replace any default TinyCore logo with our wallpaper
cp "$SRC_DIR/cyberlab_wallpaper.png" "$BUILD_DIR/initrd/usr/local/share/pixmaps/logo.png" 2>/dev/null || true
cp "$SRC_DIR/cyberlab_wallpaper.png" "$BUILD_DIR/initrd/usr/local/share/pixmaps/core.png" 2>/dev/null || true

# Create /usr/local/bin/terminal launcher
cat << 'EOF' > "$BUILD_DIR/initrd/usr/local/bin/terminal"
#!/bin/sh
exec aterm -title "CYBERLAB OS :: Engineering Terminal" -geometry 100x32+60+40 -bg black -fg green -cr white "$@"
EOF
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/terminal"

# Create /usr/local/bin/help
cat << 'HELP_EOF' > "$BUILD_DIR/initrd/usr/local/bin/help"
#!/bin/sh
cat << 'HELP_BANNER'

========================================================================
     CYBERLAB OS :: DUAL ARCHITECTURE ENGINEERING TERMINAL
   Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?
   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
========================================================================
  [AVAILABLE COMMANDS IN THIS TERMINAL]:
    wifi                     - Open Wi-Fi Control Center & Network Status
    wifi scan                - Scan surrounding 802.11 access points
    wifi connect <SSID> [pw] - Connect to a wireless network
    wifi disconnect          - Disconnect from active network
    deadlock-lab             - Run Safe-State Research Benchmark (Tables 1-5)
    snake                    - Launch Retro Snake Arcade Game
    terminal                 - Open another terminal window
    help                     - Display this command manual
    ifconfig / ip a          - View all active network adapter interfaces
    ping 8.8.8.8             - Test live Internet connectivity
    top / ps                 - Monitor process and memory usage
    reboot / poweroff        - Power and system reset controls
========================================================================

HELP_BANNER
HELP_EOF
chmod 755 "$BUILD_DIR/initrd/usr/local/bin/help"

# MOTD banner on login
cat << 'MOTD_EOF' > "$BUILD_DIR/initrd/etc/cyberlab_motd"

========================================================================
     CYBERLAB OS :: DUAL ARCHITECTURE ENGINEERING TERMINAL
   Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?
   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
========================================================================
  Type 'help' to see all available commands.
  Type 'wifi' for the Wi-Fi Control Center & Wireless Status.
  Type 'deadlock-lab' to run the research paper benchmark.
  Type 'snake' to play the Retro Snake Arcade Game.
========================================================================

MOTD_EOF

# Setup profile for interactive shells
cat << 'EOF' >> "$BUILD_DIR/initrd/etc/skel/.profile"
clear
cat /etc/cyberlab_motd
export PATH=/usr/local/bin:/usr/bin:/bin:/sbin:/usr/sbin
export PS1='cyberlab-os:\w\$ '
alias wifi="/usr/local/bin/wifi"
alias deadlock="/usr/local/bin/deadlock-lab"
alias snake="/usr/local/bin/snake-game"
alias help="/usr/local/bin/help"
EOF

# Setup tc user home directory
mkdir -p "$BUILD_DIR/initrd/home/tc/Desktop"
mkdir -p "$BUILD_DIR/initrd/home/tc/.X.d"
mkdir -p "$BUILD_DIR/initrd/usr/local/etc/X.d"
mkdir -p "$BUILD_DIR/initrd/etc/skel/.X.d"

# Custom .setbackground script: OVERRIDES default TinyCore "core" background
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.setbackground"
#!/bin/sh
# Set CYBERLAB OS dark branding wallpaper - No TinyCore core logo!
hsetroot -fill /usr/local/share/pixmaps/cyberlab_wallpaper.png
EOF
chmod 755 "$BUILD_DIR/initrd/home/tc/.setbackground"
cp "$BUILD_DIR/initrd/home/tc/.setbackground" "$BUILD_DIR/initrd/etc/skel/.setbackground"

# Autostart wallpaper in .X.d as well (runs right after window manager)
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.X.d/00-wallpaper"
#!/bin/sh
hsetroot -fill /usr/local/share/pixmaps/cyberlab_wallpaper.png &
EOF
chmod 755 "$BUILD_DIR/initrd/home/tc/.X.d/00-wallpaper"
cp "$BUILD_DIR/initrd/home/tc/.X.d/00-wallpaper" "$BUILD_DIR/initrd/usr/local/etc/X.d/00-wallpaper"

# Autostart terminal in .X.d
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.X.d/01-terminal"
#!/bin/sh
aterm -title "CYBERLAB OS :: Engineering Terminal" -geometry 100x32+60+40 -bg black -fg green -cr white &
EOF
chmod 755 "$BUILD_DIR/initrd/home/tc/.X.d/01-terminal"
cp "$BUILD_DIR/initrd/home/tc/.X.d/01-terminal" "$BUILD_DIR/initrd/usr/local/etc/X.d/01-terminal"

# Configure bottom floating dock (.wbar)
cat << 'EOF' > "$BUILD_DIR/initrd/home/tc/.wbar"
i: /usr/local/share/wbar/osxbarback.png
t: /usr/local/lib/X11/fonts/TTF/luxisr/11
c: wbar -bpress -pos bottom -zoomf 2 -isize 36 -bdist 10

i: /usr/local/share/pixmaps/aterm.png
t: Terminal
c: aterm -title "CYBERLAB OS :: Engineering Terminal" -geometry 100x32+60+40 -bg black -fg green -cr white

i: /usr/local/share/pixmaps/aterm.png
t: Wi-Fi Settings
c: aterm -title "CYBERLAB - Wi-Fi Quick Settings" -geometry 75x26 -hold -e /usr/local/bin/wifi

i: /usr/local/share/pixmaps/aterm.png
t: Deadlock Lab
c: aterm -title "CYBERLAB - Deadlock Research Lab" -geometry 95x35 -hold -e /usr/local/bin/deadlock-lab

i: /usr/local/share/pixmaps/aterm.png
t: Snake Arcade
c: aterm -title "Retro Snake Arcade" -geometry 45x28 -e /usr/local/bin/snake-game
EOF

# Ensure tc user ownership
chown -R 1001:50 "$BUILD_DIR/initrd/home/tc"
chmod 755 "$BUILD_DIR/initrd/home/tc"

# 5. Repack custom core.gz as root
echo "[5/6] Repacking custom core.gz..."
cd "$BUILD_DIR/initrd"
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
    -o "$ISO_OUT/cyberlab_microos.iso" "$BUILD_DIR/iso" 2>/dev/null

ls -lh "$ISO_OUT/cyberlab_microos.iso"
echo "========================================================================"
echo "=== BUILD COMPLETE! ISO ready at $ISO_OUT/cyberlab_microos.iso ==="
echo "========================================================================"
