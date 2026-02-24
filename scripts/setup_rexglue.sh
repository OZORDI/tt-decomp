#!/bin/bash
# Setup script for ReXGlue static recompilation tool

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
REXGLUE_DIR="$PROJECT_ROOT/utils/rexglue-sdk-main"

echo "=========================================="
echo "ReXGlue Setup for Table Tennis Decomp"
echo "=========================================="
echo

# Check prerequisites
echo "Checking prerequisites..."

if ! command -v cmake &> /dev/null; then
    echo "❌ cmake not found. Install with: brew install cmake"
    exit 1
fi
echo "✅ cmake found: $(which cmake)"

if ! command -v ninja &> /dev/null; then
    echo "❌ ninja not found. Install with: brew install ninja"
    exit 1
fi
echo "✅ ninja found: $(which ninja)"

if ! command -v clang++ &> /dev/null; then
    echo "❌ clang++ not found. Install Xcode Command Line Tools"
    exit 1
fi

CLANG_VERSION=$(clang++ --version | head -n1 | grep -oE '[0-9]+\.[0-9]+' | head -n1)
CLANG_MAJOR=$(echo $CLANG_VERSION | cut -d. -f1)
if [ "$CLANG_MAJOR" -lt 18 ]; then
    echo "⚠️  clang++ version $CLANG_VERSION found (need 18+)"
    echo "   Install with: brew install llvm@18"
    echo "   Then add to PATH: export PATH=\"/opt/homebrew/opt/llvm@18/bin:\$PATH\""
else
    echo "✅ clang++ version $CLANG_VERSION"
fi

# Check vcpkg
if [ -z "$VCPKG_ROOT" ]; then
    echo "❌ VCPKG_ROOT not set"
    echo
    echo "Install vcpkg:"
    echo "  cd ~"
    echo "  git clone https://github.com/microsoft/vcpkg.git"
    echo "  cd vcpkg"
    echo "  ./bootstrap-vcpkg.sh"
    echo
    echo "Then add to ~/.zshrc or ~/.bash_profile:"
    echo "  export VCPKG_ROOT=~/vcpkg"
    echo
    exit 1
fi

if [ ! -d "$VCPKG_ROOT" ]; then
    echo "❌ VCPKG_ROOT points to non-existent directory: $VCPKG_ROOT"
    exit 1
fi

echo "✅ vcpkg found: $VCPKG_ROOT"
echo

# Check if ReXGlue is already built
if [ -f "$REXGLUE_DIR/out/macos-arm64/Release/rexglue" ]; then
    echo "✅ ReXGlue already built!"
    echo "   Binary: $REXGLUE_DIR/out/macos-arm64/Release/rexglue"
    echo
    read -p "Rebuild? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Skipping build."
        exit 0
    fi
fi

# Build ReXGlue
echo "Building ReXGlue (this may take 10-30 minutes on first build)..."
echo

cd "$REXGLUE_DIR"

echo "Configuring CMake..."
cmake --preset macos-arm64

echo
echo "Building Release configuration..."
cmake --build out/build/macos-arm64 --config Release -j$(sysctl -n hw.ncpu)

echo
echo "=========================================="
echo "✅ ReXGlue built successfully!"
echo "=========================================="
echo
echo "Binary location:"
echo "  $REXGLUE_DIR/out/macos-arm64/Release/rexglue"
echo
echo "Next steps:"
echo "  1. Review config: $PROJECT_ROOT/rexglue_config.toml"
echo "  2. Run codegen:   ./scripts/run_rexglue.sh"
echo "  3. See docs:      $PROJECT_ROOT/docs/tools/rexglue_setup.md"
echo
