#!/usr/bin/env python3
"""
Generate a professional, high-tech desktop wallpaper for CYBERLAB OS.
Replaces the default TinyCore 'core' logo with university research branding.
"""

from PIL import Image, ImageDraw, ImageFont
import math

WIDTH = 1024
HEIGHT = 768

# Create dark gradient background: #0B111E to #152238
img = Image.new("RGB", (WIDTH, HEIGHT), (11, 17, 30))
draw = ImageDraw.Draw(img)

# Vertical subtle gradient
for y in range(HEIGHT):
    factor = y / HEIGHT
    r = int(11 + factor * (20 - 11))
    g = int(17 + factor * (32 - 17))
    b = int(30 + factor * (52 - 30))
    draw.line([(0, y), (WIDTH, y)], fill=(r, g, b))

# Grid overlay (tech aesthetics)
grid_color = (25, 40, 65)
for x in range(0, WIDTH, 64):
    draw.line([(x, 0), (x, HEIGHT)], fill=grid_color, width=1)
for y in range(0, HEIGHT, 64):
    draw.line([(0, y), (WIDTH, y)], fill=grid_color, width=1)

# Subtle corner circuit accents
accent_cyan = (0, 210, 230)
dim_cyan = (0, 140, 160)
card_bg = (16, 26, 44)
border_cyan = (0, 180, 205)

# Load fonts
font_dir = "/usr/share/fonts/truetype/dejavu/"
font_title = ImageFont.truetype(font_dir + "DejaVuSans-Bold.ttf", 46)
font_subtitle = ImageFont.truetype(font_dir + "DejaVuSans-Bold.ttf", 20)
font_body = ImageFont.truetype(font_dir + "DejaVuSans.ttf", 15)
font_body_bold = ImageFont.truetype(font_dir + "DejaVuSans-Bold.ttf", 15)
font_small = ImageFont.truetype(font_dir + "DejaVuSansMono.ttf", 13)

# Top Banner Bar
draw.rectangle([(0, 0), (WIDTH, 36)], fill=(14, 22, 38))
draw.line([(0, 36), (WIDTH, 36)], fill=border_cyan, width=1)
draw.text((24, 9), "CYBERLAB MICRO-OS v2.4", fill=accent_cyan, font=font_small)
draw.text((WIDTH - 380, 9), "DUAL ARCHITECTURE KERNEL LAB", fill=(140, 170, 200), font=font_small)

# Center Branding Badge (Glassmorphic Card)
card_w = 680
card_h = 240
card_x1 = (WIDTH - card_w) // 2
card_y1 = 180
card_x2 = card_x1 + card_w
card_y2 = card_y1 + card_h

# Draw card background & double border
draw.rectangle([(card_x1, card_y1), (card_x2, card_y2)], fill=(12, 20, 36), outline=border_cyan, width=2)
draw.rectangle([(card_x1 - 4, card_y1 - 4), (card_x2 + 4, card_y2 + 4)], outline=(0, 70, 90), width=1)

# Corner markers
corner_len = 16
# Top-Left
draw.line([(card_x1 - 8, card_y1 - 8), (card_x1 - 8 + corner_len, card_y1 - 8)], fill=accent_cyan, width=2)
draw.line([(card_x1 - 8, card_y1 - 8), (card_x1 - 8, card_y1 - 8 + corner_len)], fill=accent_cyan, width=2)
# Top-Right
draw.line([(card_x2 + 8, card_y1 - 8), (card_x2 + 8 - corner_len, card_y1 - 8)], fill=accent_cyan, width=2)
draw.line([(card_x2 + 8, card_y1 - 8), (card_x2 + 8, card_y1 - 8 + corner_len)], fill=accent_cyan, width=2)
# Bottom-Left
draw.line([(card_x1 - 8, card_y2 + 8), (card_x1 - 8 + corner_len, card_y2 + 8)], fill=accent_cyan, width=2)
draw.line([(card_x1 - 8, card_y2 + 8), (card_x1 - 8, card_y2 + 8 - corner_len)], fill=accent_cyan, width=2)
# Bottom-Right
draw.line([(card_x2 + 8, card_y2 + 8), (card_x2 + 8 - corner_len, card_y2 + 8)], fill=accent_cyan, width=2)
draw.line([(card_x2 + 8, card_y2 + 8), (card_x2 + 8, card_y2 + 8 - corner_len)], fill=accent_cyan, width=2)

# Title: CYBERLAB OS
title_text = "CYBERLAB OS"
bbox = draw.textbbox((0, 0), title_text, font=font_title)
tw = bbox[2] - bbox[0]
draw.text(((WIDTH - tw) // 2, card_y1 + 22), title_text, fill=accent_cyan, font=font_title)

# Subtitle: Dual Architecture Operating Systems Engineering Platform
sub_text = "Dual Architecture Operating Systems Engineering Platform"
bbox = draw.textbbox((0, 0), sub_text, font=font_subtitle)
tw = bbox[2] - bbox[0]
draw.text(((WIDTH - tw) // 2, card_y1 + 82), sub_text, fill=(240, 248, 255), font=font_subtitle)

# Horizontal separator inside card
draw.line([(card_x1 + 40, card_y1 + 120), (card_x2 - 40, card_y1 + 120)], fill=(35, 60, 95), width=1)

# Research Paper Info
paper_title = "Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?"
bbox = draw.textbbox((0, 0), paper_title, font=font_body_bold)
tw = bbox[2] - bbox[0]
draw.text(((WIDTH - tw) // 2, card_y1 + 138), paper_title, fill=(100, 210, 255), font=font_body_bold)

researcher = "Researcher: Vuppalapati Sri Samhita | USN: 25BTRCL285 | CSE-AIML-E"
bbox = draw.textbbox((0, 0), researcher, font=font_body)
tw = bbox[2] - bbox[0]
draw.text(((WIDTH - tw) // 2, card_y1 + 168), researcher, font=font_body)

specs = "Core: Linux 6.6.8 Live | Bare-Metal Rust Kernel | Wi-Fi Manager | Snake Arcade"
bbox = draw.textbbox((0, 0), specs, font=font_small)
tw = bbox[2] - bbox[0]
draw.text(((WIDTH - tw) // 2, card_y1 + 198), specs, fill=(120, 150, 185), font=font_small)

# Bottom Status Footer Bar
draw.rectangle([(0, HEIGHT - 28), (WIDTH, HEIGHT)], fill=(10, 16, 28))
draw.line([(0, HEIGHT - 28), (WIDTH, HEIGHT - 28)], fill=(30, 50, 80), width=1)
draw.text((20, HEIGHT - 20), "Status: Dual-OS Operational", fill=(0, 220, 130), font=font_small)
draw.text((WIDTH // 2 - 100, HEIGHT - 20), "Resolution: 1024x768 32bpp", fill=(110, 140, 175), font=font_small)
draw.text((WIDTH - 240, HEIGHT - 20), "Wi-Fi: Connected [VirtualBox]", fill=(0, 220, 130), font=font_small)

img.save("/tmp/cyberlab_wallpaper.png")
print("SUCCESS: /tmp/cyberlab_wallpaper.png created successfully!")
