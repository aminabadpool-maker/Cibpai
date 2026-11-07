#!/bin/bash
set -e

echo "========================================"
echo "      Cibpai: Installation Script       "
echo "========================================"

# بررسی سیستم‌عامل و نصب پکیج‌های مورد نیاز (پشتیبانی از Termux و لینوکس‌های مبتنی بر Apt)
if command -v pkg &> /dev/null; then
    echo "[*] Detected Termux environment (ARM64/Android)..."
    pkg update -y
    pkg install -y clang make ffmpeg curl git
elif command -v apt &> /dev/null; then
    echo "[*] Detected Debian/Ubuntu/Linux Mint environment..."
    sudo apt update
    sudo apt install -y build-essential clang make ffmpeg libcurl4-openssl-dev git
else
    echo "[!] Warning: Unknown package manager. Please ensure clang, make, ffmpeg, and libcurl are installed."
fi

# ساخت پوشه‌های ضروری پروژه
echo "[*] Creating output directories..."
mkdir -p frames_out

# کامپایل پروژه
echo "[*] Cleaning and compiling Cibpai..."
make clean && make

echo "========================================"
echo "[+] Installation completed successfully!"
echo "========================================"
echo "To run the pipeline, use:"
echo "  export AI_API_KEY=\"your_gemini_api_key\""
echo "  ./cibpai <video_file> frames_out"
echo "========================================"
