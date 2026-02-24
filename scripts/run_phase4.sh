#!/bin/bash
# Phase 4 Pipeline Runner
# Rockstar Table Tennis — tt-decomp
# Runs all Phase 4 scripts in sequence.
#
# Input:   recomp/structured_pass5/   (Stage 3 output)
# Output:  recomp/structured_pass5_final/   (Phase 4 output, ready for hand-lifting)
#
# Pipeline:
#   Phase 4A: VCALL + PPC_CALL_INDIRECT_FUNC resolution (pass5 -> pass5_ph4)
#   Phase 4B: Struct offset clustering (analysis + generated_structs.h update)
#   Phase 4C: lis+addi Pattern B globals fix (pass5_ph4 -> pass5_ph4b)
#   Finalize: Copy pass5_ph4b -> pass5_final

set -e
cd /Users/Ozordi/Downloads/tt-decomp

echo "============================================================"
echo "PHASE 4: Architectural Refactoring Pipeline"
echo "tt-decomp — Rockstar Table Tennis Xbox 360"
echo "============================================================"
echo ""

START=$(date +%s)

echo "[Phase 4A] VCALL + virtual dispatch resolution..."
python3 scripts/phase4a_vcall_resolve.py
echo ""

echo "[Phase 4B] Struct offset clustering..."
python3 scripts/phase4b_offset_cluster.py --top 20
echo ""

echo "[Phase 4C] lis+addi Pattern B globals fix..."
python3 scripts/phase4c_globals_pattern_b.py
echo ""

echo "[Finalize] Copying pass5_ph4b -> pass5_final..."
if [ -d recomp/structured_pass5_final ]; then
  rm -rf recomp/structured_pass5_final
fi
cp -r recomp/structured_pass5_ph4b recomp/structured_pass5_final
echo "  Done."
echo ""

END=$(date +%s)
ELAPSED=$((END - START))

echo "============================================================"
echo "PHASE 4 COMPLETE in ${ELAPSED}s"
echo ""
echo "Output:  recomp/structured_pass5_final/"
echo ""
echo "Generated artifacts:"
echo "  config/434C4803/offset_cluster_map.json"
echo "  include/generated_structs.h  (Phase 4B clusters)"
echo ""
echo "Key metrics (start → end):"
echo "  PPC_CALL_INDIRECT_FUNC: $(grep -rc 'PPC_CALL_INDIRECT_FUNC' recomp/structured_pass5/*.cpp 2>/dev/null | awk -F: '{s+=$2}END{print s}') → $(grep -rc 'PPC_CALL_INDIRECT_FUNC' recomp/structured_pass5_final/*.cpp 2>/dev/null | awk -F: '{s+=$2}END{print s}')"
echo "  VCALL macros:           $(grep -rc 'VCALL(' recomp/structured_pass5/*.cpp 2>/dev/null | awk -F: '{s+=$2}END{print s}') → $(grep -rc 'VCALL(' recomp/structured_pass5_final/*.cpp 2>/dev/null | awk -F: '{s+=$2}END{print s}')"
echo "  glob: annotations:      $(grep -rc 'glob:' recomp/structured_pass5_final/*.cpp 2>/dev/null | awk -F: '{s+=$2}END{print s}') new"
echo "============================================================"
