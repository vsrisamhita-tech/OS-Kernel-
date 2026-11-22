#!/bin/bash
for f in /tmp/cyberlab_build/iso/cde/optional/*.tcz; do
    if unsquashfs -l "$f" 2>/dev/null | grep -q 'startx'; then
        echo "Found startx in: $f"
    fi
    if unsquashfs -l "$f" 2>/dev/null | grep -q '\.xsession'; then
        echo "Found .xsession in: $f"
    fi
done
