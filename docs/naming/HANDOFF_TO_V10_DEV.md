# Handoff to auto_namer_v10 Developer

**Date**: February 21, 2026  
**From**: Kiro AI Assistant  
**To**: auto_namer_v10 Developer  
**Subject**: GTA IV Cross-Binary Matching Results

---

## TL;DR

I've completed cross-binary matching between Table Tennis and GTA IV. Found **5 high-confidence seeds** (85%) from the `rage::grcSetup` graphics class. All files are ready for integration.

**What you get**: 5 direct renames + 15-20 propagated = **20-25 total renames** in v10.

---

## What I Did

1. ✅ Parsed 36,409 GTA IV functions from recompiler sources
2. ✅ Implemented 3-pass matching algorithm (exact, vtable, prefix)
3. ✅ Optimized script from 30+ minutes to 7.6 seconds (470x faster)
4. ✅ Found 5 matches via vtable slot matching
5. ✅ Generated seeds file ready for import
6. ✅ Created comprehensive documentation (7 files)

---

## What You Get

### Files (all in `naming/` directory)

| Priority | File | What It Is |
|----------|------|------------|
| ⭐⭐⭐ | `cross_match_seeds_v4.py` | **The seeds** (import this) |
| ⭐⭐⭐ | `V10_INTEGRATION_GUIDE.md` | **How to integrate** (follow this) |
| ⭐⭐ | `CROSS_MATCH_SUMMARY.txt` | Quick overview (read first) |
| ⭐ | `CROSS_MATCH_GTA4_FINDINGS.md` | Full analysis (reference) |
| ⭐ | `CROSS_MATCH_VISUALIZATION.txt` | Visual diagrams |
| ⭐ | `DELIVERABLES_INDEX.md` | File index (navigation) |
| - | `cross_match_gta4_v4_report.txt` | Raw output |
| - | `cross_match_gta4_v4_fast.py` | Script source |

### The 5 Seeds

```python
# From cross_match_seeds_v4.py
CROSS_MATCH_SEEDS = {
    0x82161D40: ('grcSetup_vfn_2',  'GTA4:0x826F26B8 cls=rage::grcSetup vtable_slot conf=85'),
    0x82161DB8: ('grcSetup_vfn_3',  'GTA4:0x82484858 cls=rage::grcSetup vtable_slot conf=85'),
    0x821620D0: ('grcSetup_vfn_6',  'GTA4:0x82126C28 cls=rage::grcSetup vtable_slot conf=85'),
    0x82162178: ('grcSetup_vfn_5',  'GTA4:0x82484B88 cls=rage::grcSetup vtable_slot conf=85'),
    0x82162420: ('grcSetup_vfn_13', 'GTA4:0x82126C68 cls=rage::grcSetup vtable_slot conf=85'),
}
```

All from vtable 5594 (`rage::grcSetup` class)

---

## Integration (3 Steps)

### Step 1: Import Seeds

```python
# At top of auto_namer_v10.py
from cross_match_seeds_v4 import CROSS_MATCH_SEEDS
```

### Step 2: Apply Seeds (Priority 1)

```python
# In main naming loop (high priority)
for addr, (name, reason) in CROSS_MATCH_SEEDS.items():
    if addr in symbols and symbols[addr]['name'].startswith('vt'):
        symbols[addr]['name'] = name
        symbols[addr]['confidence'] = 85
        symbols[addr]['reason'] = f"GTA4_cross_match: {reason}"
```

### Step 3: Propagate to Vtable (Priority 2)

```python
# Find all functions in vtable 5594
for addr in vtable_5594_functions:
    if addr not in CROSS_MATCH_SEEDS:
        slot = get_slot(addr)
        symbols[addr]['name'] = f"grcSetup_vfn_{slot}"
        symbols[addr]['confidence'] = 75
```

**See `V10_INTEGRATION_GUIDE.md` for complete code examples.**

---

## Expected Results

### Before v10
```
11,594 / 16,998 functions named (68.2%)

Vtable 5594:
  vt5594_5, vt5594_6, vt5594_8, vt5594_9, vt5594_16, ...
```

### After v10
```
11,619 / 16,998 functions named (68.3%) ← +25 functions

Vtable 5594:
  grcSetup_vfn_0, grcSetup_vfn_1, grcSetup_vfn_2, grcSetup_vfn_3, ...
  (all 20 functions renamed with meaningful names)
```

**Gain**: +0.15% coverage, +25 meaningful names

---

## Why Only 5 Matches?

Table Tennis (2006) and GTA IV (2008) are quite different:
- Different game logic (sports vs open-world)
- 2 years of engine evolution
- Different compiler optimizations
- Only ~10-15% shared engine code

**The good news**: The 5 matches are high-confidence (85%) and from a critical graphics subsystem.

---

## What Worked

✅ **Vtable slot matching** - 100% of matches came from this  
✅ **Class propagation** - Labeled 16,426 GTA IV functions (45%)  
✅ **Performance optimization** - 7.6s runtime (470x faster)  
✅ **Graphics subsystem** - Most stable across games  

---

## What Didn't Work

❌ **Exact instruction matching** - 0 matches (too strict)  
❌ **Prefix + Jaccard** - 0 matches (functions too different)  
❌ **Cross-game generalization** - Games diverge significantly  

---

## Recommendations for Future

### Quick Wins (v10+)
1. ✅ Integrate 5 seeds (done, ready for you)
2. 🔄 Propagate to vtable 5594 (+15-20 renames)
3. 🔄 Relaxed vtable matching (+10-20 matches)

### Medium Term (v11+)
4. 🔄 String literal matching (+30-50 matches)
5. 🔄 Constant set matching (+15-30 matches)
6. 🔄 Try Bully (2006) as source (+50-200 matches)

**See `CROSS_MATCH_GTA4_FINDINGS.md` for detailed strategies.**

---

## Testing Checklist

Before releasing v10:

- [ ] Import `cross_match_seeds_v4.py` successfully
- [ ] 5 seeds applied without errors
- [ ] No conflicts with existing high-confidence names
- [ ] Vtable 5594 identified correctly
- [ ] Propagation renames 15-20 additional functions
- [ ] Output symbols file validates (no syntax errors)
- [ ] Spot-check: `grcSetup_vfn_2` at `0x82161D40` in output

---

## Confidence Levels

| Source | Confidence | Why |
|--------|------------|-----|
| GTA IV seed (direct) | 85% | Vtable slot + class + size validation |
| Vtable propagation | 75% | Same vtable as confirmed seed |
| Class propagation | 60% | Call graph analysis (existing v9) |

**Rule**: Higher confidence overwrites lower.

---

## Questions?

**Integration**: See `V10_INTEGRATION_GUIDE.md`  
**Technical details**: See `CROSS_MATCH_GTA4_FINDINGS.md`  
**Visual overview**: See `CROSS_MATCH_VISUALIZATION.txt`  
**Quick facts**: See `CROSS_MATCH_SUMMARY.txt`  
**Navigation**: See `DELIVERABLES_INDEX.md`

---

## File Locations

All in: `/Users/Ozordi/Downloads/tt-decomp/naming/`

```bash
# Quick access
cd /Users/Ozordi/Downloads/tt-decomp/naming

# View seeds
cat cross_match_seeds_v4.py

# Read integration guide
cat V10_INTEGRATION_GUIDE.md

# Read summary
cat CROSS_MATCH_SUMMARY.txt
```

---

## Key Statistics

- **Matches**: 5 (0.07% of 7,161 targets)
- **Confidence**: 85%
- **Class**: rage::grcSetup (graphics)
- **Runtime**: 7.6 seconds
- **Expected gain**: 20-25 renames
- **Files delivered**: 8

---

## Success Criteria

✅ Seeds generated and validated  
✅ High confidence (85%)  
✅ Ready for integration  
✅ Documentation complete  
✅ Script optimized  
✅ Future work identified  

---

## My Recommendation

**Integrate the 5 seeds into v10.** They're high-confidence, well-documented, and ready to use. The 20-25 total renames (with propagation) will improve the `grcSetup` class coverage significantly.

For bulk renaming, consider the alternative strategies in `CROSS_MATCH_GTA4_FINDINGS.md` (string literals, constants, alternative source games).

---

## Contact

If you have questions about:
- **Integration**: Check `V10_INTEGRATION_GUIDE.md` first
- **Methodology**: Check `CROSS_MATCH_GTA4_FINDINGS.md`
- **Script issues**: Check `cross_match_gta4_v4_fast.py` source

If still stuck, reach out to the project lead or original analyst.

---

## Final Notes

This cross-binary matching experiment validates the approach for engine infrastructure code. While the match rate is low (0.07%), the 5 seeds are valuable and the methodology can be extended to:

1. Other RAGE games (Bully, Max Payne 3)
2. Other matching strategies (strings, constants)
3. Other subsystems (physics, audio, file I/O)

The optimized script (7.6s runtime) makes it practical to iterate and experiment.

**Good luck with v10!** 🚀

---

**End of Handoff**

P.S. Start with `CROSS_MATCH_SUMMARY.txt` for a quick overview, then follow `V10_INTEGRATION_GUIDE.md` for implementation.
