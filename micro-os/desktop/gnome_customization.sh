#!/usr/bin/env bash
# micro-os/desktop/gnome_customization.sh – Apply a macOS‑style GNOME theme
# This script installs the "WhiteSur" GTK theme and icon set, sets the dock layout, and configures the panel.
set -e

# Install required packages (Debian‑based)
sudo apt-get update
sudo apt-get install -y gnome-shell gnome-tweaks gnome-shell-extensions

# Install WhiteSur theme and icons (using git clone)
THEME_DIR="$HOME/.themes"
ICON_DIR="$HOME/.icons"
mkdir -p "$THEME_DIR" "$ICON_DIR"
git clone https://github.com/vinceliuice/WhiteSur-gtk-theme.git "$THEME_DIR/WhiteSur"
git clone https://github.com/vinceliuice/WhiteSur-icon-theme.git "$ICON_DIR/WhiteSur"

# Apply theme via gsettings
gsettings set org.gnome.desktop.interface gtk-theme "WhiteSur"
gsettings set org.gnome.desktop.interface icon-theme "WhiteSur"

gsettings set org.gnome.shell.extensions.dash-to-dock dock-position "BOTTOM"
gsettings set org.gnome.shell.extensions.dash-to-dock show-apps-at-top true

echo "[INFO] GNOME macOS‑style theme applied. Log out and back in to see changes."
