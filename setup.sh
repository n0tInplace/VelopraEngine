#!/bin/bash
set -e

# ── Prerequisite check ───────────────────────────────────────────────────────
# vcpkg's bootstrap and port builds fail with cryptic errors when these are
# missing, so check up front and say exactly what to install.

missing=()
for tool in git curl zip unzip tar cmake pkg-config autoconf automake libtool ninja; do
    command -v "$tool" >/dev/null 2>&1 || missing+=("$tool")
done
# libtoolize ships separately on macOS (part of libtool) — check it explicitly
command -v libtoolize >/dev/null 2>&1 || command -v glibtoolize >/dev/null 2>&1 || missing+=("libtoolize")
if ! command -v cc >/dev/null 2>&1 && ! command -v clang >/dev/null 2>&1 \
   && ! command -v gcc >/dev/null 2>&1; then
    missing+=("C++ compiler")
fi

if [ ${#missing[@]} -gt 0 ]; then
    echo "Missing prerequisites: ${missing[*]}"
    case "$(uname -s)" in
        Darwin)
            echo "Install with:"
            echo "  xcode-select --install"
            echo "  brew install cmake pkg-config autoconf autoconf-archive automake libtool ninja"
            ;;
        Linux)
            echo "Debian/Ubuntu:"
            echo "  sudo apt install build-essential cmake curl zip unzip tar pkg-config autoconf autoconf-archive automake libtool ninja-build"
            echo "Plus the X11 development headers listed in the README."
            ;;
    esac
    exit 1
fi

# ── Bootstrap vcpkg ──────────────────────────────────────────────────────────

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
#   cmake --preset macos-debug     (or linux-debug)
#   cmake --build --preset macos-debug
#
# Note: the first configure compiles Qt and Assimp from source and can take
# a while. Subsequent configures reuse vcpkg's binary cache and are fast.

echo "Setup completed. Configure with: cmake --preset <preset-name>"
