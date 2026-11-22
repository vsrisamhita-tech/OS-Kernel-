#!/usr/bin/env python3
from PIL import Image
import sys

img = Image.open('/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/debug_screen.png')
print("Image size:", img.size, "Mode:", img.mode)
# Sample pixel colors or check bounding box
bbox = img.getbbox()
print("Non-zero bounding box:", bbox)
