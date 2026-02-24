#!/bin/bash
# Run ReXGlue codegen on Table Tennis XEX

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
REXGLUE_BIN="$PROJECT_ROOT/utils/rexglue-sdk-main/out/macos-arm64/Release/rexglue"
CONFIG_FILE="$PROJECT_ROOT/rexglue_config.toml"

echo "=========================================="
echo "Running ReXGlue Codegen"
echo "=========================================="
echo

# Check if ReXGlue is built
if [ ! -f "$REXGLUE_BIN" ]; then
    echo "❌ ReXGlue not found at: $REXGLUE_BIN"
    echo
    echo "Build it first:"
    echo "  ./scripts/setup_rexglue.sh"
    echo
    exit 1
fi

# Check if config exists
if [ ! -f "$CONFIG_FILE" ]; then
    echo "❌ Config file not found: $CONFIG_FILE"
    exit 1
fi

# Check if XEX exists
XEX_PATH=$(grep '^file_path' "$CONFIG_FILE" | cut -d'"' -f2)
if [ ! -f "$XEX_PATH" ]; then
    echo "❌ XEX file not found: $XEX_PATH"
    echo
    echo "Update file_path in: $CONFIG_FILE"
    exit 1
fi

echo "Configuration:"
echo "  Config:  $CONFIG_FILE"
echo "  XEX:     $XEX_PATH"
echo "  Output:  $(grep '^out_directory_path' "$CONFIG_FILE" | cut -d'"' -f2)"
echo

# Parse command line options
FORCE=""
NO_EXCEPTION=""
VERBOSE=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --force)
            FORCE="--force"
            shift
            ;;
        --no-exception-handlers)
            NO_EXCEPTION="--no_exception_handlers"
            shift
            ;;
        --verbose)
            VERBOSE="1"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo
            echo "Usage: $0 [--force] [--no-exception-handlers] [--verbose]"
            exit 1
            ;;
    esac
done

# Set verbose logging if requested
if [ -n "$VERBOSE" ]; then
    export REXCVAR_log_verbose=1
fi

# Run ReXGlue
echo "Running codegen..."
echo

cd "$PROJECT_ROOT"
"$REXGLUE_BIN" codegen "$CONFIG_FILE" $FORCE $NO_EXCEPTION

echo
echo "=========================================="
echo "✅ Codegen complete!"
echo "=========================================="
echo
echo "Output directory:"
echo "  $(grep '^out_directory_path' "$CONFIG_FILE" | cut -d'"' -f2)"
echo
echo "Next steps:"
echo "  1. Review analysis_report.json for function discovery"
echo "  2. Compare with symbols.txt:"
echo "     python utils/compare_rexglue_functions.py \\"
echo "            config/434C4803/symbols.txt \\"
echo "            rexglue_output/analysis_report.json"
echo "  3. Use generated code as reference for manual decompilation"
echo
