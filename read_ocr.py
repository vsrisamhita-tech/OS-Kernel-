#!/usr/bin/env python3
import subprocess

out = subprocess.run(["tesseract", "/mnt/c/Users/ECS/.gemini/antigravity/scratch/CYBERLAB_OS/screenshots/debug_screen.png", "stdout"], capture_output=True, text=True)
print("--- OCR TEXT START ---")
print(out.stdout)
print("--- OCR TEXT END ---")
