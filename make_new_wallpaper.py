#!/usr/bin/env python3
"""
CYBERLAB OS - New Wallpaper: Deep Space Aurora Theme
1024x768, dark deep space with glowing aurora bands + CYBERLAB branding
"""
from PIL import Image, ImageDraw, ImageFont
import math, random, os

W, H = 1024, 768
img = Image.new("RGB", (W, H), (0, 0, 0))
draw = ImageDraw.Draw(img)

# ── 1. Deep space background gradient (deep purple → black → deep blue) ──
for y in range(H):
    t = y / H
    if t < 0.5:
        r = int(8 + t * 20)
        g = int(0 + t * 5)
        b = int(20 + t * 40)
    else:
        r = int(18 - (t-0.5)*10)
        g = int(2 + (t-0.5)*8)
        b = int(60 - (t-0.5)*20)
    draw.line([(0, y), (W, y)], fill=(r, g, b))

# ── 2. Stars ──
random.seed(42)
for _ in range(350):
    sx = random.randint(0, W-1)
    sy = random.randint(0, H//2)
    brightness = random.randint(140, 255)
    size = random.choice([1, 1, 1, 2])
    draw.ellipse([sx-size, sy-size, sx+size, sy+size],
                 fill=(brightness, brightness, brightness))

# ── 3. Aurora bands (glowing horizontal waves) ──
for band_i, (base_y, color, alpha_max) in enumerate([
    (H//4,       (0, 255, 180),  60),   # cyan-green
    (H//3,       (100, 80, 255), 45),   # purple
    (H//2 - 40,  (0, 200, 255),  50),   # blue
]):
    for x in range(W):
        wave = int(18 * math.sin(x * 0.012 + band_i * 1.3)
                 + 10 * math.sin(x * 0.027 + band_i * 0.9))
        for dy in range(-30, 31):
            dist = abs(dy - wave)
            if dist > 28:
                continue
            alpha = int(alpha_max * (1 - dist / 28.0) ** 2)
            py = base_y + dy
            if 0 <= py < H:
                px = img.getpixel((x, py))
                nr = min(255, px[0] + color[0] * alpha // 255)
                ng = min(255, px[1] + color[1] * alpha // 255)
                nb = min(255, px[2] + color[2] * alpha // 255)
                img.putpixel((x, py), (nr, ng, nb))

# ── 4. Subtle hex-grid overlay (bottom half) ──
hex_color = (0, 60, 80)
hex_size = 38
for row in range(10):
    for col in range(20):
        cx = col * hex_size * 2 + (hex_size if row % 2 else 0)
        cy = H // 2 + row * int(hex_size * 1.1)
        pts = []
        for angle_deg in range(0, 360, 60):
            angle = math.radians(angle_deg)
            pts.append((cx + hex_size * math.cos(angle),
                        cy + hex_size * math.sin(angle)))
        draw.polygon(pts, outline=hex_color)

# ── 5. Glowing grid lines (bottom section) ──
for x in range(0, W, 64):
    for y in range(H//2, H, 1):
        px = img.getpixel((x, y))
        img.putpixel((x, y), (min(255, px[0]+15), min(255, px[1]+30), min(255, px[2]+40)))
for y in range(H//2, H, 48):
    for x in range(0, W, 1):
        px = img.getpixel((x, y))
        img.putpixel((x, y), (min(255, px[0]+10), min(255, px[1]+20), min(255, px[2]+30)))

# ── 6. Central info box ──
bx1, by1, bx2, by2 = 230, 270, 794, 460
# Semi-transparent box using rectangle layering
for layer in range(12):
    alpha = 15 - layer
    draw.rectangle([bx1-layer, by1-layer, bx2+layer, by2+layer],
                   outline=(0, int(200 + layer*4), int(180 + layer*5)))
draw.rectangle([bx1, by1, bx2, by2], fill=(2, 8, 18))

# Glow border
draw.rectangle([bx1, by1, bx2, by2], outline=(0, 220, 200), width=2)
draw.rectangle([bx1+3, by1+3, bx2-3, by2-3], outline=(0, 120, 110), width=1)

# ── 7. Text ──
font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
try:
    f_title  = ImageFont.truetype(font_path, 36)
    f_sub    = ImageFont.truetype(font_path, 16)
    f_small  = ImageFont.truetype(font_path, 13)
    f_tiny   = ImageFont.truetype(font_path, 11)
except:
    f_title = f_sub = f_small = f_tiny = ImageFont.load_default()

def centered(text, y, font, color):
    bbox = draw.textbbox((0, 0), text, font=font)
    tw = bbox[2] - bbox[0]
    draw.text(((W - tw) // 2, y), text, font=font, fill=color)

# Title
centered("CYBERLAB OS", by1 + 22, f_title, (0, 255, 220))

# Subtitle
centered("Dual Architecture Operating Systems Engineering Platform", by1 + 70, f_sub, (120, 220, 200))

# Divider
draw.line([(bx1 + 30, by1 + 100), (bx2 - 30, by1 + 100)], fill=(0, 160, 140), width=1)

# Paper
centered("Paper: Safe-State Analysis: How Conservative Is Deadlock Avoidance?", by1 + 115, f_small, (80, 200, 180))
centered("Researcher: Vuppalapati Sri Samhita  |  USN: 25BTRCL285  |  CSE-AIML-E", by1 + 138, f_small, (160, 230, 220))

# Divider 2
draw.line([(bx1 + 30, by1 + 162), (bx2 - 30, by1 + 162)], fill=(0, 100, 90), width=1)

# Features row
features = "Core: Linux 6.6.8 Live  |  Bare-Metal Rust Kernel  |  Wi-Fi Manager  |  Snake Arcade"
centered(features, by1 + 175, f_tiny, (60, 170, 155))

# ── 8. Top bar ──
draw.rectangle([0, 0, W, 28], fill=(2, 10, 22))
draw.line([(0, 28), (W, 28)], fill=(0, 160, 140), width=1)
draw.text((10, 7), "CYBERLAB MICRO-OS v2.5", font=f_tiny, fill=(0, 200, 180))
ver_text = "AURORA EDITION  |  DUAL ARCHITECTURE KERNEL LAB"
bbox = draw.textbbox((0, 0), ver_text, font=f_tiny)
draw.text((W - bbox[2] + bbox[0] - 10, 7), ver_text, font=f_tiny, fill=(0, 160, 140))

# ── 9. Bottom status bar ──
draw.rectangle([0, H-28, W, H], fill=(2, 10, 22))
draw.line([(0, H-28), (W, H-28)], fill=(0, 160, 140), width=1)
draw.text((10, H-20), "Status: Dual-OS Operational", font=f_tiny, fill=(0, 200, 100))
res_text = "Resolution: 1024x768 32bpp"
bbox = draw.textbbox((0, 0), res_text, font=f_tiny)
draw.text(((W - bbox[2] + bbox[0]) // 2, H-20), res_text, font=f_tiny, fill=(80, 140, 130))
wifi_text = "Wi-Fi: Connected [VirtualBox]"
bbox = draw.textbbox((0, 0), wifi_text, font=f_tiny)
draw.text((W - bbox[2] + bbox[0] - 10, H-20), wifi_text, font=f_tiny, fill=(0, 180, 120))

# ── Save ──
out = "/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/aurora_wallpaper.png"
img.save(out)
print(f"Saved: {out}  ({W}x{H})")
