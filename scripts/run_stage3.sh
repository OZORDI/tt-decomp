#!/bin/bash
# Stage 3 Master Pipeline Runner
# Rockstar Table Tennis Xbox 360 Decompilation
# Runs all Stage 3 phases in sequence: Pre → 3A → 3B → 3C → 3D → 3E
# Output: recomp/structured_pass5/

set -e
cd /Users/Ozordi/Downloads/tt-decomp

echo "============================================================"
echo "STAGE 3: Type Inference & Memory Lifting"
echo "tt-decomp — Rockstar Table Tennis Xbox 360"
echo "============================================================"
echo ""

START=$(date +%s)

echo "[Phase 3-Pre] Building lookup tables..."
python3 scripts/stage3_pre_build_tables.py
echo ""

echo "[Phase 3A] Global address resolution (lis+addi / SDA)..."
python3 scripts/stage3_globals.py
echo ""

echo "[Phase 3B] Function call renaming (sub_X -> real names)..."
python3 scripts/stage3_rename.py
echo ""

echo "[Phase 3C] Vtable dispatch resolution..."
python3 scripts/stage3_vtables.py
echo ""

echo "[Phase 3D] Struct member lifting..."
python3 scripts/stage3_structs.py
echo ""

echo "[Phase 3E] Final cleanup..."
python3 scripts/stage3_cleanup.py
echo ""

END=$(date +%s)
ELAPSED=$((END - START))

echo "============================================================"
echo "STAGE 3 COMPLETE in ${ELAPSED}s"
echo "Output: recomp/structured_pass5/"
echo "Generated headers:"
echo "  include/generated_symbols.h   (16,653 extern declarations)"
echo "  include/generated_vtables.h   (1,115 vtable structs)"
echo "  include/generated_structs.h   (struct definitions with field annotations)"
echo "============================================================"
