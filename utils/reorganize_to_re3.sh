#!/bin/bash
# Reorganize tt-decomp to match re3 structure EXACTLY
# Based on Reference Projects/re3-main structure

set -e

echo "=== Reorganizing tt-decomp to match re3 structure ==="
echo ""

# Create backup first
BACKUP_NAME="tt-decomp-backup-pre-re3-$(date +%Y%m%d-%H%M%S).tar.gz"
echo "Creating backup: $BACKUP_NAME"
tar -czf "../$BACKUP_NAME" \
    --exclude='.git' \
    --exclude='build' \
    --exclude='orig' \
    --exclude='Reference Projects' \
    --exclude='*.o' \
    --exclude='*.obj' \
    --exclude='__pycache__' \
    .
echo "✓ Backup created"
echo ""

# Create re3-style directory structure
echo "Creating re3-style directories..."
mkdir -p cmake
mkdir -p gamefiles/data
mkdir -p gamefiles/models
mkdir -p utils
mkdir -p vendor
echo "✓ Directories created"
echo ""

# Move documentation files
echo "Moving documentation..."
# Keep docs/ but rename to match re3 style (they don't have docs/ but we need it)
# Actually re3 doesn't have a docs folder - they keep everything at root
# Move key docs to root
if [ -f "docs/CODEBASE_CLEANUP_PLAN.md" ]; then
    mv docs/CODEBASE_CLEANUP_PLAN.md CLEANUP_HISTORY.md
fi
if [ -f "docs/CLEANUP_COMPLETE.md" ]; then
    cat docs/CLEANUP_COMPLETE.md >> CLEANUP_HISTORY.md
    rm docs/CLEANUP_COMPLETE.md
fi
if [ -f "docs/REORGANIZATION_PLAN.md" ]; then
    mv docs/REORGANIZATION_PLAN.md REORGANIZATION_HISTORY.md
fi
if [ -f "docs/REORGANIZATION_SUMMARY.md" ]; then
    cat docs/REORGANIZATION_SUMMARY.md >> REORGANIZATION_HISTORY.md
    rm docs/REORGANIZATION_SUMMARY.md
fi

# Move all other docs to a subdirectory (re3 doesn't have this but we need it for research)
mkdir -p docs/research
if [ -d "docs" ]; then
    find docs -maxdepth 1 -type f -name "*.md" -exec mv {} docs/research/ \;
fi
echo "✓ Documentation organized"
echo ""

# Handle tools/ → utils/ (re3 uses utils/)
echo "Reorganizing tools to utils..."
if [ -d "tools/analysis" ]; then
    mkdir -p utils/analysis
    mv tools/analysis/* utils/analysis/ 2>/dev/null || true
fi
if [ -d "tools/naming" ]; then
    mkdir -p utils/naming
    mv tools/naming/* utils/naming/ 2>/dev/null || true
fi
if [ -d "tools/decompiled_wip" ]; then
    mkdir -p utils/decompiled_wip
    mv tools/decompiled_wip/* utils/decompiled_wip/ 2>/dev/null || true
fi
# Move any remaining tool scripts
if [ -d "tools" ]; then
    find tools -maxdepth 1 -type f \( -name "*.py" -o -name "*.sh" \) -exec mv {} utils/ \; 2>/dev/null || true
fi
echo "✓ Tools moved to utils/"
echo ""

# Handle scripts/ → utils/ (re3 doesn't have scripts/)
echo "Moving scripts to utils..."
if [ -d "scripts" ]; then
    find scripts -type f -exec mv {} utils/ \; 2>/dev/null || true
fi
echo "✓ Scripts moved to utils/"
echo ""

# Handle config/ (re3 doesn't have this - it's decompilation-specific)
# Keep it but document it
echo "Keeping config/ (decompilation toolkit specific)"
echo ""

# src/ and include/ stay as-is (re3 has same structure)
echo "✓ src/ and include/ structure preserved (matches re3)"
echo ""

# Clean up empty directories
echo "Cleaning up empty directories..."
rmdir tools/analysis 2>/dev/null || true
rmdir tools/naming 2>/dev/null || true
rmdir tools/decompiled_wip 2>/dev/null || true
rmdir tools 2>/dev/null || true
rmdir scripts 2>/dev/null || true
echo "✓ Empty directories removed"
echo ""

# Create .gitkeep files for important empty directories
touch gamefiles/data/.gitkeep
touch gamefiles/models/.gitkeep
touch vendor/.gitkeep
echo "✓ .gitkeep files created"
echo ""

echo "=== Reorganization Complete ==="
echo ""
echo "New structure matches re3:"
echo "  cmake/          - CMake modules (like re3)"
echo "  gamefiles/      - Game assets (like re3)"
echo "  src/            - Source code (like re3)"
echo "  include/        - Headers (tt-decomp specific, re3 has headers in src/)"
echo "  utils/          - Analysis and naming tools (like re3)"
echo "  vendor/         - Third-party dependencies (like re3)"
echo "  config/         - Decompilation toolkit config (tt-decomp specific)"
echo "  docs/research/  - Research documentation (tt-decomp specific)"
echo ""
echo "Backup saved as: ../$BACKUP_NAME"
