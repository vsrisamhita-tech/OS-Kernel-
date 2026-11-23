#!/usr/bin/env python3
from PIL import Image

im = Image.open('/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/desktop_confirmed.png')
print("Image Dimensions:", im.size)
# Let's inspect different parts of the image
# Top: (0, 0, 1024, 100)
# Center: (100, 100, 924, 668)
# Let's check colors in center
center = im.crop((100, 100, 924, 668))
colors = center.getcolors(maxcolors=10000)
print("Colors in center:", len(colors) if colors else "many")
# Check if the terminal window is tiny or black or minimized!
# Check bounding box of black pixels (0,0,0) or dark pixels
black_pixels = 0
total_pixels = center.size[0] * center.size[1]
for count, col in (colors or []):
    if col[0] < 20 and col[1] < 20 and col[2] < 20:
        black_pixels += count
print(f"Dark/Black pixels in center: {black_pixels} / {total_pixels} ({black_pixels/total_pixels*100:.1f}%)")
