#!/bin/bash
set -e
echo "Checking environment inside WSL Debian..."
uname -a
python3 -V
gcc -v 2>&1 | tail -n 1 || echo "gcc not installed"
xorriso -version | head -n 1
mksquashfs -version | head -n 1
echo "Environment check successful!"
