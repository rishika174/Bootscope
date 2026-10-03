#!/usr/bin/env bash
# BootScope Local Build & Verification Script
set -e

echo "=== [BootScope] Setting up project on Linux ==="

# Check cmake and g++
if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed. Run: sudo apt update && sudo apt install -y cmake build-essential"
    exit 1
fi

if ! command -v g++ &> /dev/null; then
    echo "Error: g++ is not installed. Run: sudo apt update && sudo apt install -y build-essential"
    exit 1
fi

echo "[1/3] Configuring CMake build..."
cmake -B build -DCMAKE_BUILD_TYPE=Release

echo "[2/3] Compiling BootScope (C++17)..."
cmake --build build -j$(nproc)

echo "[3/3] Running automated unit test suite..."
./build/bootscope_tests

echo ""
echo "=== Build and Tests Successful! ==="
echo "You can now run:"
echo "  ./build/bootscope --analyze"
echo "  ./build/bootscope --blame --threshold 1.0s"
echo "  ./build/bootscope --critical-path"
echo "  ./build/bootscope --bottlenecks"
