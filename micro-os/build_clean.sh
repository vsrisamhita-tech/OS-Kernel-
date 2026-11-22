#!/usr/bin/env bash
# micro-os/build_clean.sh – Build the live ISO (minimal placeholder)
# This script demonstrates the steps required; replace with a full debootstrap workflow.
set -e
WORKDIR=$(pwd)/micro-os/build
mkdir -p "$WORKDIR"
echo "[INFO] Creating minimal filesystem for demonstration..."
# Create a minimal directory structure
mkdir -p "$WORKDIR/chroot"
# Install a tiny base (placeholder – real script would use debootstrap)
# For now we just copy a pre‑made small rootfs tarball if available.
# TODO: Replace with debootstrap for Debian 12.
# Configure GNOME, NetworkManager, etc. (omitted for brevity)
# Create a simple squashed filesystem
mksquashfs "$WORKDIR/chroot" "$WORKDIR/cyberlab_microos.squashfs" -noappend -comp xz
# Generate ISO (placeholder)
genisoimage -output "$WORKDIR/cyberlab_microos.iso" -volid "CYBERLAB_MICROOS" -joliet -rock "$WORKDIR/chroot"
echo "[INFO] Build complete: $WORKDIR/cyberlab_microos.iso"
