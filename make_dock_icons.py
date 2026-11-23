#!/usr/bin/env python3
"""
Generate distinct 48x48 icons for CYBERLAB OS dock:
- wifi.png : Glowing cyan Wi-Fi antenna with radiating signal arcs
- deadlock.png : Scientific atomic/graph deadlock analysis icon
- snake.png : Arcade controller / retro snake icon
- terminal.png : High-tech cyan-bordered terminal console with '>_'
"""
import math
from PIL import Image, ImageDraw

def create_wifi_icon(path):
    img = Image.new("RGBA", (48, 48), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    # Background circular plate
    d.ellipse([2, 2, 45, 45], fill=(8, 24, 38, 240), outline=(0, 200, 220, 255), width=2)
    
    # Radiating arcs (cyan glowing)
    # Arc 1 (outer)
    d.arc([10, 10, 37, 37], start=215, end=325, fill=(0, 240, 255, 255), width=3)
    # Arc 2 (middle)
    d.arc([15, 15, 32, 32], start=215, end=325, fill=(0, 255, 200, 255), width=3)
    # Arc 3 (inner)
    d.arc([20, 20, 27, 27], start=215, end=325, fill=(0, 255, 150, 255), width=2)
    # Center dot
    d.ellipse([21, 31, 26, 36], fill=(0, 255, 180, 255))
    
    img.save(path)
    print("Saved:", path)

def create_deadlock_icon(path):
    img = Image.new("RGBA", (48, 48), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    # Background plate
    d.ellipse([2, 2, 45, 45], fill=(28, 12, 36, 240), outline=(220, 80, 255, 255), width=2)
    
    # Deadlock cycle / graph (RAG graph with 4 nodes)
    # Nodes
    nodes = [(24, 11), (37, 24), (24, 37), (11, 24)]
    # Directed cycle edges
    for i in range(4):
        p1 = nodes[i]
        p2 = nodes[(i + 1) % 4]
        d.line([p1, p2], fill=(255, 120, 220, 230), width=2)
    
    for x, y in nodes:
        d.ellipse([x-4, y-4, x+4, y+4], fill=(0, 255, 220, 255), outline=(255, 255, 255, 255), width=1)
    
    # Center deadlock lock symbol
    d.rectangle([21, 22, 27, 28], fill=(255, 215, 0, 255))
    d.arc([21, 17, 26, 24], start=180, end=0, fill=(255, 215, 0, 255), width=2)
    
    img.save(path)
    print("Saved:", path)

def create_snake_icon(path):
    img = Image.new("RGBA", (48, 48), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    # Rounded background
    d.ellipse([2, 2, 45, 45], fill=(12, 34, 18, 240), outline=(50, 220, 80, 255), width=2)
    
    # Snake body in green blocks
    coords = [
        (12, 30), (18, 30), (24, 30), (24, 24), (24, 18),
        (30, 18), (34, 18)
    ]
    for x, y in coords:
        d.rectangle([x, y, x+5, y+5], fill=(80, 255, 100, 255), outline=(20, 100, 40, 255))
    # Snake eyes
    d.rectangle([34, 19, 35, 20], fill=(255, 255, 255, 255))
    # Red apple target
    d.ellipse([14, 16, 20, 22], fill=(255, 50, 50, 255), outline=(180, 20, 20, 255))
    
    img.save(path)
    print("Saved:", path)

def create_terminal_icon(path):
    img = Image.new("RGBA", (48, 48), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    # Terminal screen outline
    d.rounded_rectangle([3, 5, 44, 42], radius=6, fill=(10, 14, 20, 245), outline=(0, 220, 255, 255), width=2)
    # Top bar with window dots
    d.line([(3, 14), (44, 14)], fill=(0, 150, 180, 255), width=1)
    d.ellipse([7, 8, 10, 11], fill=(255, 90, 90, 255))
    d.ellipse([13, 8, 16, 11], fill=(255, 200, 50, 255))
    d.ellipse([19, 8, 22, 11], fill=(50, 220, 90, 255))
    
    # Prompt >_
    # '>'
    d.line([(10, 22), (18, 27)], fill=(0, 255, 180, 255), width=2)
    d.line([(18, 27), (10, 32)], fill=(0, 255, 180, 255), width=2)
    # '_' cursor
    d.line([(22, 32), (32, 32)], fill=(0, 255, 180, 255), width=3)
    
    img.save(path)
    print("Saved:", path)

if __name__ == "__main__":
    out_dir = "/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS"
    create_wifi_icon(f"{out_dir}/wifi.png")
    create_deadlock_icon(f"{out_dir}/deadlock.png")
    create_snake_icon(f"{out_dir}/snake.png")
    create_terminal_icon(f"{out_dir}/term_icon.png")
