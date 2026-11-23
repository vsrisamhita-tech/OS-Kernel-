#!/usr/bin/env python3
"""Generate CYBERLAB OS custom dark wallpaper — no TinyCore branding."""
try:
    from PIL import Image, ImageDraw, ImageFont
    HAS_PIL = True
except ImportError:
    HAS_PIL = False

W, H = 1024, 768

if HAS_PIL:
    img = Image.new("RGB", (W, H), (8, 12, 24))
    draw = ImageDraw.Draw(img)

    # Subtle grid lines
    for x in range(0, W, 48):
        draw.line([(x, 0), (x, H)], fill=(18, 30, 55), width=1)
    for y in range(0, H, 48):
        draw.line([(0, y), (W, y)], fill=(18, 30, 55), width=1)

    # Grid intersection dots
    for x in range(0, W, 48):
        for y in range(0, H, 48):
            draw.ellipse([(x-2, y-2), (x+2, y+2)], fill=(28, 50, 85))

    # Try to load fonts
    font_paths = [
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
        "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
    ]
    font_big = None
    font_med = None
    font_sm  = None
    for fp in font_paths:
        try:
            font_big = ImageFont.truetype(fp, 72)
            font_med = ImageFont.truetype(fp.replace("Bold", ""), 24)
            font_sm  = ImageFont.truetype(fp.replace("Bold", ""), 15)
            break
        except Exception:
            continue
    if font_big is None:
        font_big = ImageFont.load_default()
        font_med = font_big
        font_sm  = font_big

    title = "CYBERLAB OS"
    bbox = draw.textbbox((0, 0), title, font=font_big)
    tw = bbox[2] - bbox[0]
    tx = (W - tw) // 2
    ty = H // 2 - 130

    # Cyan glow layers
    for off in [8, 5, 3]:
        c = (0, 100, 140)
        draw.text((tx - off, ty), title, fill=c, font=font_big)
        draw.text((tx + off, ty), title, fill=c, font=font_big)
        draw.text((tx, ty - off), title, fill=c, font=font_big)
        draw.text((tx, ty + off), title, fill=c, font=font_big)

    # Main title bright cyan
    draw.text((tx, ty), title, fill=(0, 220, 255), font=font_big)

    # Divider line
    draw.line([(W//2 - 290, ty + 88), (W//2 + 290, ty + 88)], fill=(0, 160, 200), width=2)

    # Subtitle
    sub = "Dual Architecture Engineering Platform"
    bbox2 = draw.textbbox((0, 0), sub, font=font_med)
    sw = bbox2[2] - bbox2[0]
    draw.text(((W - sw) // 2, ty + 100), sub, fill=(160, 210, 240), font=font_med)

    # Research paper line
    res = "Research: Safe-State Analysis — How Conservative Is Deadlock Avoidance?"
    bbox3 = draw.textbbox((0, 0), res, font=font_sm)
    rw = bbox3[2] - bbox3[0]
    draw.text(((W - rw) // 2, ty + 142), res, fill=(70, 100, 145), font=font_sm)

    # Bottom bar
    draw.rectangle([(0, H - 42), (W, H)], fill=(5, 8, 18))
    draw.line([(0, H - 42), (W, H - 42)], fill=(0, 90, 130), width=1)
    bot = "CSE-AIML  |  25BTRCL285  |  VirtualBox VM  |  TinyCore Linux"
    bbox4 = draw.textbbox((0, 0), bot, font=font_sm)
    bw = bbox4[2] - bbox4[0]
    draw.text(((W - bw) // 2, H - 28), bot, fill=(55, 85, 130), font=font_sm)

    img.save("/tmp/cyberlab_bg.png")
    print("SUCCESS: Wallpaper saved to /tmp/cyberlab_bg.png")

else:
    # Fallback: create a minimal valid PNG with solid dark color using raw bytes
    import struct, zlib

    def png_chunk(chunk_type, data):
        c = chunk_type + data
        return struct.pack('>I', len(data)) + c + struct.pack('>I', zlib.crc32(c) & 0xffffffff)

    def make_solid_png(w, h, r, g, b):
        raw_rows = b''
        for _ in range(h):
            row = b'\x00' + bytes([r, g, b] * w)
            raw_rows += row
        compressed = zlib.compress(raw_rows, 9)
        sig = b'\x89PNG\r\n\x1a\n'
        ihdr_data = struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)
        ihdr = png_chunk(b'IHDR', ihdr_data)
        idat = png_chunk(b'IDAT', compressed)
        iend = png_chunk(b'IEND', b'')
        return sig + ihdr + idat + iend

    with open('/tmp/cyberlab_bg.png', 'wb') as f:
        f.write(make_solid_png(W, H, 8, 12, 24))
    print("FALLBACK: Solid dark wallpaper saved to /tmp/cyberlab_bg.png")
