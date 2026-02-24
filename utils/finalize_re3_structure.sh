#!/bin/bash
# Final cleanup to match re3 structure exactly

set -e

echo "=== Final re3 structure cleanup ==="
echo ""

# Move remaining tools/ content to utils/
echo "Moving remaining tools/ content..."
if [ -d "tools" ]; then
    # Move binaries to utils/
    if [ -f "tools/manual_conversion" ]; then
        mv tools/manual_conversion utils/
    fi
    if [ -f "tools/table_tennis_converted" ]; then
        mv tools/table_tennis_converted utils/
    fi
    if [ -d "tools/xextool" ]; then
        mv tools/xextool utils/
    fi
    if [ -f "tools/README.md" ]; then
        mv tools/README.md utils/TOOLS_README.md
    fi
    # Remove __pycache__
    rm -rf tools/__pycache__
    # Remove empty tools directory
    rmdir tools 2>/dev/null || true
fi
echo "✓ tools/ content moved"
echo ""

# Move cleanup scripts to utils/
echo "Moving cleanup scripts to utils/..."
if [ -f "cleanup_phase1.sh" ]; then
    mv cleanup_phase1.sh utils/cleanup_phase1.sh
fi
if [ -f "cleanup_phase2.sh" ]; then
    mv cleanup_phase2.sh utils/cleanup_phase2.sh
fi
if [ -f "reorganize.sh" ]; then
    mv reorganize.sh utils/reorganize_old.sh
fi
if [ -f "reorganize_to_re3.sh" ]; then
    mv reorganize_to_re3.sh utils/reorganize_to_re3.sh
fi
echo "✓ Cleanup scripts moved"
echo ""

# Create CODING_STYLE.md like re3
echo "Creating CODING_STYLE.md..."
cat > CODING_STYLE.md << 'EOF'
# Coding Style

This project follows the coding style of the original Rockstar Table Tennis codebase as closely as possible.

## General Rules

- Match the original assembly's behavior exactly
- Preserve ABI compatibility (calling conventions, stack frames, struct layout)
- Use C++17 standard features where appropriate
- Keep function boundaries as they appear in the original binary

## Naming Conventions

- Classes: PascalCase (e.g., `grcDevice`, `rage_render_default`)
- Functions: camelCase or snake_case matching original symbols
- Variables: camelCase for local, snake_case for globals
- Constants: UPPER_SNAKE_CASE
- Namespaces: lowercase (e.g., `rage::`, `grc::`)

## Code Organization

- One function per assembly function (no merging)
- Headers in `include/` directory
- Implementation in `src/` directory
- Group by subsystem (grc/, rage/, game/, physics/, crt/)

## Comments

- Document ABI considerations
- Note correspondence with assembly addresses
- Explain non-obvious arithmetic or bit manipulation
- Reference original Xbox 360 behavior where relevant

## Formatting

- 4 spaces for indentation (no tabs)
- Opening braces on same line for functions
- Use `#pragma once` for header guards
- Keep lines under 120 characters where reasonable

## Cross-Platform

- Use SDL2 for window/input/audio
- Use OpenGL 3.3 Core for graphics
- Avoid platform-specific code outside `crt/` directory
- Test on Windows, macOS, and Linux

## Verification

- Build with CMake + Ninja
- Run getDiagnostics for syntax/type checking
- Use objdiff for binary comparison where possible
- Test cross-platform compilation regularly
EOF
echo "✓ CODING_STYLE.md created"
echo ""

# Update .gitignore to match re3 style
echo "Updating .gitignore..."
cat >> .gitignore << 'EOF'

# re3-style ignores
*.exe
*.dll
*.so
*.dylib
*.a
*.lib
*.obj
*.o
*.d
*.log
*.ninja
*.ninja_deps
*.ninja_log
compile_commands.json
build.ninja
.ninja_*

# Build directories
build/
bin/
obj/
out/

# IDE
.vscode/
.idea/
*.swp
*.swo
*~

# Python
__pycache__/
*.pyc
*.pyo
*.pyd
.Python
*.egg-info/
dist/
*.egg

# macOS
.DS_Store
.AppleDouble
.LSOverride

# Original game files
orig/
*.xex
*.iso

# Backup files
*-backup-*.tar.gz
EOF
echo "✓ .gitignore updated"
echo ""

# Create README for utils/ like re3
echo "Creating utils/README.md..."
cat > utils/README.md << 'EOF'
# Utils

This directory contains analysis tools, naming scripts, and decompilation utilities.

## Subdirectories

- `analysis/` - Binary analysis scripts for function identification
- `naming/` - Automated function naming tools (v1-v17)
- `decompiled_wip/` - Work-in-progress decompiled functions

## Key Scripts

- `project.py` - Main project configuration and build system
- `auto_name.py` - Automated function naming
- `analyze_crt.py` - CRT function analysis
- `build_chat_pdf.py` - Documentation generation

## Decompilation Workflow

1. Run analysis scripts to identify functions
2. Use naming tools to assign meaningful names
3. Decompile functions in `decompiled_wip/`
4. Move completed functions to `src/`
5. Verify with objdiff and cross-platform builds
EOF
echo "✓ utils/README.md created"
echo ""

# Create vendor/README.md
echo "Creating vendor/README.md..."
cat > vendor/README.md << 'EOF'
# Vendor Dependencies

This directory contains third-party libraries and dependencies.

## Expected Dependencies

- SDL2 - Window management, input, audio
- OpenGL/GLEW - Graphics rendering
- pthreads - Threading (POSIX systems)

## Installation

Dependencies are managed via system package managers:

### macOS (Homebrew)
```bash
brew install sdl2 glew
```

### Linux (apt)
```bash
sudo apt-get install libsdl2-dev libglew-dev
```

### Windows
Download SDL2 and GLEW development libraries and place in vendor/
EOF
echo "✓ vendor/README.md created"
echo ""

# Create gamefiles/README.md
echo "Creating gamefiles/README.md..."
cat > gamefiles/README.md << 'EOF'
# Game Files

This directory contains game assets and data files.

## Structure

- `data/` - Game data files (scripts, configs)
- `models/` - Textures and models
- `TEXT/` - Localized text files

## Note

Original game assets are required to run the game. These files should be extracted from a legitimate copy of Rockstar Presents Table Tennis.

Place extracted assets here to run the compiled executable.
EOF
echo "✓ gamefiles/README.md created"
echo ""

echo "=== Final Structure Complete ==="
echo ""
echo "Directory structure now matches re3:"
tree -L 1 -d . 2>/dev/null || ls -d */ | sed 's|/$||'
echo ""
echo "Root files match re3 style:"
ls -1 *.md *.txt CMakeLists.txt LICENSE 2>/dev/null || true
