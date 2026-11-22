#!/usr/bin/env python3
from PIL import Image

img = Image.open('/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/debug_screen.png')
# Crop bottom dock area: (0, 700, 1024, 768)
bottom = img.crop((0, 700, 1024, 768))
bottom.save('/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/bottom_dock.png')

# Crop top bar area: (0, 0, 1024, 50)
top = img.crop((0, 0, 1024, 50))
top.save('/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/top_bar.png')

# Crop center area: (200, 200, 824, 600)
center = img.crop((200, 200, 824, 600))
center.save('/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/center.png')

print("Cropped sections saved!")
