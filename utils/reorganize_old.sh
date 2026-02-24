#!/bin/bash
# Repository Reorganization Script
# Cleans up tt-decomp structure to follow re3/reVC conventions

set -e

echo "════════════════════════════════════════════════════════════════"
echo "  Repository Reorganization - tt-decomp"
echo "════════════════════════════════════════════════════════════════"
echo ""
echo "This will:"
echo "  • Move 21 root .md files to docs/"
echo "  • Consolidate decompiled/ directory"
echo "  • Move analysis scripts to tools/"
echo "  • Clean up root clutter"
echo ""
read -p "Continue? (y/n) " -n 1 -r
echo ""
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Aborted."
    exit 1
fi

# Create backup
echo "Creating backup..."
BACKUP="tt-decomp-backup-$(date +%Y%m%d-%H%M%S).tar.gz"
cd ..
tar -czf "$BACKUP" --exclude='build' --exclude='orig' --exclude='__pycache__' tt-decomp/
cd tt-decomp
echo "✓ Backup created: ../$BACKUP"
echo ""

# Create new directory structure
echo "Creating new directory structure..."
mkdir -p docs/{naming,research,guides/rpcs3_macos}
mkdir -p tools/{naming,analysis,decompiled_wip}
echo "✓ Directories created"
echo ""

# Move root .md files to docs/
echo "Moving documentation files..."

# Research docs
[ -f "ARCHITECTURE_FIXES.md" ] && mv "ARCHITECTURE_FIXES.md" docs/research/architecture_fixes.md
[ -f "CODE_REVIEW.md" ] && mv "CODE_REVIEW.md" docs/research/code_review.md
[ -f "COVERAGE_ROADMAP.md" ] && mv "COVERAGE_ROADMAP.md" docs/research/coverage_roadmap.md
[ -f "DECOMPILATION_SUMMARY.md" ] && mv "DECOMPILATION_SUMMARY.md" docs/research/decompilation_summary.md
[ -f "RESEARCH_ANALYSIS.md" ] && mv "RESEARCH_ANALYSIS.md" docs/research/research_analysis.md
[ -f "Analyzing Hotspot Assembly.md" ] && mv "Analyzing Hotspot Assembly.md" docs/research/hotspot_analysis_wip.md
[ -f "Hotspot Analysis Complete - v17 Final Report.md" ] && mv "Hotspot Analysis Complete - v17 Final Report.md" docs/research/hotspot_analysis_final.md

# Progress tracking
[ -f "IMPLEMENTATION_PROGRESS.md" ] && mv "IMPLEMENTATION_PROGRESS.md" docs/progress.md

# Naming research
[ -f "NAMING_RESEARCH.md" ] && mv "NAMING_RESEARCH.md" docs/naming/naming_research.md
[ -f "NAMING_RESEARCH_COMPLETE.md" ] && mv "NAMING_RESEARCH_COMPLETE.md" docs/naming/naming_research_complete.md

# RPCS3 guides
[ -f "RPCS3_MACOS_IMPLEMENTATION_GUIDE.md" ] && mv "RPCS3_MACOS_IMPLEMENTATION_GUIDE.md" docs/guides/rpcs3_macos/implementation_guide.md
[ -f "RPCS3_MACOS_PSMOVE_IMPLEMENTATION_PLAN.md" ] && mv "RPCS3_MACOS_PSMOVE_IMPLEMENTATION_PLAN.md" docs/guides/rpcs3_macos/implementation_plan.md
[ -f "RPCS3_MACOS_PSMOVE_INDEX.md" ] && mv "RPCS3_MACOS_PSMOVE_INDEX.md" docs/guides/rpcs3_macos/index.md
[ -f "RPCS3_MACOS_PSMOVE_SUMMARY.md" ] && mv "RPCS3_MACOS_PSMOVE_SUMMARY.md" docs/guides/rpcs3_macos/summary.md
[ -f "QUICK_START_RPCS3_MACOS_PSMOVE.md" ] && mv "QUICK_START_RPCS3_MACOS_PSMOVE.md" docs/guides/rpcs3_macos/quick_start.md

# Starting point guides
[ -f "BEST_STARTING_POINT.md" ] && mv "BEST_STARTING_POINT.md" docs/guides/best_starting_point.md
[ -f "TABLE_TENNIS_BEST_STARTING_POINT.md" ] && mv "TABLE_TENNIS_BEST_STARTING_POINT.md" docs/guides/table_tennis_starting_point.md

echo "✓ Documentation files moved"
echo ""

# Move naming directory
if [ -d "naming" ]; then
    echo "Moving naming/ directory..."
    # Move entire naming directory to docs/
    cp -r naming/* docs/naming/ 2>/dev/null || true
    # Move scripts to tools/naming/
    mv naming/*.py tools/naming/ 2>/dev/null || true
    # Keep docs in docs/naming/
    echo "✓ Naming directory migrated"
    echo ""
fi

# Move analysis scripts
echo "Moving analysis scripts..."
mv analyze*.py tools/analysis/ 2>/dev/null || true
echo "✓ Analysis scripts moved"
echo ""

# Handle decompiled/ directory
if [ -d "decompiled" ]; then
    echo "Processing decompiled/ directory..."
    
    # Move WIP decompiled functions
    mv decompiled/game_*.cpp tools/decompiled_wip/ 2>/dev/null || true
    mv decompiled/fn_*.cpp tools/decompiled_wip/ 2>/dev/null || true
    
    # Move clean implementations from decompiled/src/ to src/
    if [ -d "decompiled/src" ]; then
        echo "  Moving clean implementations to src/..."
        for dir in decompiled/src/*/; do
            if [ -d "$dir" ]; then
                dirname=$(basename "$dir")
                mkdir -p "src/$dirname"
                cp -r "$dir"* "src/$dirname/" 2>/dev/null || true
                
                # Also copy headers if they exist
                if [ -f "$dir"*.hpp ]; then
                    mkdir -p "include/$dirname"
                    cp "$dir"*.hpp "include/$dirname/" 2>/dev/null || true
                fi
            fi
        done
    fi
    
    # Move decompiled docs
    [ -f "decompiled/README.md" ] && mv "decompiled/README.md" tools/decompiled_wip/README.md
    [ -f "decompiled/README_PROFESSIONAL.md" ] && mv "decompiled/README_PROFESSIONAL.md" tools/decompiled_wip/README_PROFESSIONAL.md
    
    echo "✓ Decompiled directory processed"
    echo ""
fi

# Clean up obsolete files
echo "Cleaning up obsolete files..."
rm -f README.example.md README_REWRITE.md 2>/dev/null || true
rm -rf __pycache__ 2>/dev/null || true
rm -f all_files_list.txt 2>/dev/null || true
rm -f .ninja_log 2>/dev/null || true
echo "✓ Cleanup complete"
echo ""

# Create index files
echo "Creating index files..."

# docs/README.md
cat > docs/README.md << 'DOCREADME'
# Documentation Index

## Getting Started
- [Getting Started](getting_started.md) - How to build and run
- [Best Starting Point](guides/best_starting_point.md) - Where to begin
- [Table Tennis Starting Point](guides/table_tennis_starting_point.md) - Game-specific guide

## Progress & Status
- [Implementation Progress](progress.md) - Current implementation status
- [Naming Milestone v17](naming/V17_FINAL_MILESTONE.md) - 99.99% coverage achieved

## Research & Analysis
- [Decompilation Summary](research/decompilation_summary.md)
- [Coverage Roadmap](research/coverage_roadmap.md)
- [Architecture Fixes](research/architecture_fixes.md)
- [Code Review](research/code_review.md)
- [Hotspot Analysis](research/hotspot_analysis_final.md)

## Naming Research
- [Naming Research](naming/naming_research.md)
- [Naming Quality Assessment](naming/NAMING_QUALITY_ASSESSMENT.md)
- [Cross-Match GTA IV Findings](naming/CROSS_MATCH_GTA4_FINDINGS.md)

## Guides
- [RPCS3 macOS Quick Start](guides/rpcs3_macos/quick_start.md)
- [RPCS3 macOS Implementation](guides/rpcs3_macos/implementation_guide.md)

## Reverse Engineering Docs
See [../alt-docs/INDEX.md](../alt-docs/INDEX.md) for 40+ system analysis documents.
DOCREADME

# tools/README.md
cat > tools/README.md << 'TOOLREADME'
# Tools Directory

## Naming Scripts
- `naming/auto_namer_v17.py` - Final auto-namer (99.99% coverage)
- `naming/cross_match_gta4_v4_fast.py` - Cross-binary matching with GTA IV
- See [naming/README.md](naming/README.md) for details

## Analysis Scripts
- `analysis/analyze*.py` - Various analysis utilities
- `analysis/README.md` - Analysis tool documentation

## Decompiled WIP
- `decompiled_wip/` - Work-in-progress decompiled functions
- Not production-ready, for reference only
TOOLREADME

echo "✓ Index files created"
echo ""

# Summary
echo "════════════════════════════════════════════════════════════════"
echo "  Reorganization Complete!"
echo "════════════════════════════════════════════════════════════════"
echo ""
echo "Summary:"
echo "  • Moved $(find docs -name '*.md' | wc -l) documentation files to docs/"
echo "  • Moved $(find tools/analysis -name '*.py' 2>/dev/null | wc -l) analysis scripts to tools/analysis/"
echo "  • Moved $(find tools/naming -name '*.py' 2>/dev/null | wc -l) naming scripts to tools/naming/"
echo "  • Moved $(find tools/decompiled_wip -name '*.cpp' 2>/dev/null | wc -l) WIP functions to tools/decompiled_wip/"
echo ""
echo "Next steps:"
echo "  1. Review the new structure: ls -R docs/ tools/"
echo "  2. Update CMakeLists.txt if needed"
echo "  3. Test build: ninja"
echo "  4. Delete old directories:"
echo "     rm -rf decompiled/ naming/"
echo "  5. Commit changes:"
echo "     git add -A"
echo "     git commit -m 'Reorganize repository structure (re3/reVC style)'"
echo ""
echo "Backup saved at: ../$BACKUP"
echo ""
