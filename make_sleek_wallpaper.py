#!/usr/bin/env python3
"""
Generate a sleek, minimalist, professional dark desktop wallpaper for MicroOS.
No cluttered boxes. Clean, modern abstract design.
"""

from PIL import Image, ImageDraw, ImageFont
import math

WIDTH = 1024
HEIGHT = 768

# Create rich dark obsidian/midnight background
img = Image.new("RGB", (WIDTH, HEIGHT), (8, 14, 28))
draw = ImageDraw.Draw(img)

# Smooth multi-stop dark gradient
for y in range(HEIGHT):
    # Curve factor for richer depth
    t = y / HEIGHT
    r = int(7 + 10 * math.sin(t * math.pi * 0.8))
    g = int(12 + 18 * math.sin(t * math.pi * 0.8))
    b = int(24 + 36 * math.sin(t * math.pi * 0.8))
    draw.line([(0, y), (WIDTH, y)], fill=(r, g, b))

# Subtle geometric wave / circuit polygons (minimalist tech aesthetic)
poly_color1 = (14, 28, 52)
poly_color2 = (18, 36, 68)
poly_color3 = (24, 48, 88)

# Flowing abstract polygonal mountain/wave ribbons at bottom-middle
points1 = [(0, 480), (180, 440), (380, 510), (620, 430), (840, 490), (WIDTH, 420), (WIDTH, HEIGHT), (0, HEIGHT)]
draw.polygon(points1, fill=poly_color1)

points2 = [(0, 540), (220, 500), (440, 560), (700, 480), (880, 530), (WIDTH, 470), (WIDTH, HEIGHT), (0, HEIGHT)]
draw.polygon(points2, fill=poly_color2)

points3 = [(0, 620), (280, 570), (520, 620), (760, 550), (920, 590), (WIDTH, 540), (WIDTH, HEIGHT), (0, HEIGHT)]
draw.polygon(points3, fill=poly_color3)

# Fine accent glow lines on crests
draw.line([(0, 480), (180, 440), (380, 510), (620, 430), (840, 490), (WIDTH, 420)], fill=(0, 180, 220), width=1)
draw.line([(0, 540), (220, 500), (440, 560), (700, 480), (880, 530), (WIDTH, 470)], fill=(0, 140, 190), width=1)
draw.line([(0, 620), (280, 570), (520, 620), (760, 550), (920, 590), (WIDTH, 540)], fill=(0, 210, 240), width=2)

# Load fonts
font_dir = "/usr/share/fonts/truetype/dejavu/"
font_brand = ImageFont.truetype(font_dir + "DejaVuSans-Bold.ttf", 38)
font_sub = ImageFont.truetype(font_dir + "DejaVuSans.ttf", 16)
font_small = ImageFont.truetype(font_dir + "DejaVuSansMono.ttf", 12)

# Clean, elegant top-left branding (modern Linux style, not in the way of terminal)
draw.text((36, 32), "MICRO-OS", fill=(0, 220, 245), font=font_brand)
draw.text((38, 76), "Dual-Architecture Operating Systems Engineering Platform", fill=(170, 200, 230), font=font_sub)

# Subtle top-right system pill
pill_text = "Debian MicroOS • Live 6.6.8"
bbox = draw.textbbox((0, 0), pill_text, font=font_small)
pw = bbox[2] - bbox[0]
draw.rounded_rectangle([(WIDTH - pw - 56, 34), (WIDTH - 32, 62)], radius=14, fill=(16, 28, 50), outline=(0, 160, 200), width=1)
draw.text((WIDTH - pw - 44, 41), pill_text, fill=(0, 210, 240), font=font_small)

# Discreet bottom info bar (academic identification)
draw.rectangle([(0, HEIGHT - 30), (WIDTH, HEIGHT)], fill=(6, 12, 22))
draw.line([(0, HEIGHT - 30), (WIDTH, HEIGHT - 30)], fill=(25, 45, 75), width=1)

draw.text((28, HEIGHT - 22), "Vuppalapati Sri Samhita | 25BTRCL285 | CSE-AIML-E", fill=(140, 170, 205), font=font_small)
draw.text((WIDTH - 420, HEIGHT - 22), "Paper: Safe-State Analysis — Deadlock Avoidance", fill=(110, 150, 190), font=font_small)

img.save("/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/sleek_wallpaper.png")
print("SUCCESS: sleek_wallpaper.png generated!")
