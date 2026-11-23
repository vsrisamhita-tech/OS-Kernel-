#!/bin/bash
set -e
apt-get install -y squashfs-tools >/dev/null 2>&1 || true
cd /tmp/inspect_tc/cde/optional
for f in *.tcz; do
    matches=$(unsquashfs -l "$f" 2>/dev/null | grep -Ei "logo|background|wall|\.png|\.jpg|\.xpm" || true)
    if [ -n "$matches" ]; then
        echo "=== Package: $f ==="
        echo "$matches"
    fi
done
