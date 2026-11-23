#!/usr/bin/env python3
"""Generate a 48x48 Snap Store shopping bag icon."""
from PIL import Image, ImageDraw

img = Image.new("RGBA", (48, 48), (0, 0, 0, 0))
draw = ImageDraw.Draw(img)

# Orange/coral Canonical snap store gradient bag
# Bag body
draw.rounded_rectangle([(6, 14), (42, 44)], radius=6, fill=(233, 84, 32))

# Bag handle
draw.arc([(14, 4), (34, 22)], start=180, end=0, fill=(255, 255, 255), width=3)

# Canonical snap arrow / bird mark in center of bag
draw.polygon([(24, 20), (33, 30), (27, 30), (27, 38), (21, 38), (21, 30), (15, 30)], fill=(255, 255, 255))

img.save("/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/snapstore.png")
print("SUCCESS: snapstore.png generated!")
