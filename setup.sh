#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VCPKG_DIR="$PROJECT_DIR/vcpkg"

if [ ! -d "$VCPKG_DIR" ]; then
    echo "Cloning vcpkg into $VCPKG_DIR"
    git clone https://github.com/Microsoft/vcpkg.git "$VCPKG_DIR"
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
else
    echo "vcpkg already installed"
fi

# Dependencies are declared in vcpkg.json (manifest mode) and are installed
# automatically the first time you configure with CMake — no manual install
# step needed:
#
#   cmake --preset macos-debug     (or linux-debug / x64-debug on Windows)
#   cmake --build --preset macos-debug
#
# Note: the first configure compiles Qt and Assimp from source and can take
# a while. Subsequent configures reuse vcpkg's binary cache and are fast.

echo "Setup completed. Configure with: cmake --preset <preset-name>"
