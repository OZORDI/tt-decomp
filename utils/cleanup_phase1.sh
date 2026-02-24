#!/bin/bash
# Phase 1: Root Directory Cleanup
# Moves misplaced files to appropriate directories

set -e

echo "════════════════════════════════════════════════════════════════"
echo "  Phase 1: Root Directory Cleanup"
echo "════════════════════════════════════════════════════════════════"
echo ""

# Move analysis outputs
echo "1. Moving analysis outputs..."
[ -f "cross_match_gta4_results.json" ] && mv cross_match_gta4_results.json tools/naming/ && echo "  ✓ cross_match_gta4_results.json → tools/naming/"
[ -f "rtti_vtable_map.json" ] && mv rtti_vtable_map.json config/434C4803/ && echo "  ✓ rtti_vtable_map.json → config/434C4803/"
[ -f "objdiff.json" ] && mv objdiff.json config/ && echo "  ✓ objdiff.json → config/"
echo ""

# Move disassembly
echo "2. Moving disassembly..."
[ -f "tt_disasm.s" ] && mv tt_disasm.s tools/analysis/ && echo "  ✓ tt_disasm.s → tools/analysis/"
echo ""

# Move patch
echo "3. Moving patch..."
[ -f "rpcs3_macos_psmove.patch" ] && mv rpcs3_macos_psmove.patch docs/guides/rpcs3_macos/ && echo "  ✓ rpcs3_macos_psmove.patch → docs/guides/rpcs3_macos/"
echo ""

# Move conversion attempts
echo "4. Moving conversion directories..."
[ -d "manual_conversion" ] && mv manual_conversion/ tools/ && echo "  ✓ manual_conversion/ → tools/"
[ -d "table_tennis_converted" ] && mv table_tennis_converted/ tools/ && echo "  ✓ table_tennis_converted/ → tools/"
echo ""

# Delete obsolete files
echo "5. Deleting obsolete files..."
[ -f "converted_functions_auto.cpp" ] && rm -f converted_functions_auto.cpp && echo "  ✓ Deleted converted_functions_auto.cpp"
[ -f "converted_sample.cpp" ] && rm -f converted_sample.cpp && echo "  ✓ Deleted converted_sample.cpp"
[ -f "nohup.out" ] && rm -f nohup.out && echo "  ✓ Deleted nohup.out"
[ -f "e" ] && rm -f e && echo "  ✓ Deleted 'e' (mystery file)"
echo ""

# Update .gitignore
echo "6. Updating .gitignore..."
if [ ! -f ".gitignore" ]; then
    touch .gitignore
fi

# Check if entries already exist before adding
grep -q "compile_commands.json" .gitignore 2>/dev/null || echo "compile_commands.json" >> .gitignore
grep -q "build.ninja" .gitignore 2>/dev/null || echo "build.ninja" >> .gitignore
grep -q ".ninja_log" .gitignore 2>/dev/null || echo ".ninja_log" >> .gitignore
grep -q "__pycache__" .gitignore 2>/dev/null || echo "__pycache__/" >> .gitignore
grep -q ".DS_Store" .gitignore 2>/dev/null || echo ".DS_Store" >> .gitignore

echo "  ✓ .gitignore updated"
echo ""

echo "════════════════════════════════════════════════════════════════"
echo "  ✅ Phase 1 Complete!"
echo "════════════════════════════════════════════════════════════════"
echo ""
echo "Root directory now contains:"
ls -1 | grep -v "^build$" | grep -v "^orig$" | head -20
echo ""
echo "Next: Run cleanup_phase2.sh to create missing headers"
