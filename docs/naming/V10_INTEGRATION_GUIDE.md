# auto_namer_v10 Integration Guide - GTA IV Seeds

**Quick Start**: How to integrate the 5 GTA IV cross-match seeds into v10

---

## Files You Need

1. **`cross_match_seeds_v4.py`** - Python dict with 5 seeds (ready to import)
2. **`cross_match_gta4_v4_report.txt`** - Detailed match report
3. **`CROSS_MATCH_GTA4_FINDINGS.md`** - Full analysis (this summary)

---

## The 5 Seeds

All from `rage::grcSetup` vtable (graphics device initialization):

```python
CROSS_MATCH_SEEDS = {
    0x82161D40: ('grcSetup_vfn_2',  'GTA4:0x826F26B8 cls=rage::grcSetup vtable_slot conf=85'),
    0x82161DB8: ('grcSetup_vfn_3',  'GTA4:0x82484858 cls=rage::grcSetup vtable_slot conf=85'),
    0x821620D0: ('grcSetup_vfn_6',  'GTA4:0x82126C28 cls=rage::grcSetup vtable_slot conf=85'),
    0x82162178: ('grcSetup_vfn_5',  'GTA4:0x82484B88 cls=rage::grcSetup vtable_slot conf=85'),
    0x82162420: ('grcSetup_vfn_13', 'GTA4:0x82126C68 cls=rage::grcSetup vtable_slot conf=85'),
}
```

**Current TT2 names**: `vt5594_5`, `vt5594_6`, `vt5594_9`, `vt5594_8`, `vt5594_16`  
**Vtable ID**: 5594 (in TT2 symbols)

---

## Integration Steps

### Step 1: Import Seeds (Priority 1)

```python
# At top of auto_namer_v10.py
from cross_match_seeds_v4 import CROSS_MATCH_SEEDS

# In main naming loop (high priority, before other heuristics)
def apply_cross_match_seeds(symbols):
    """Apply GTA IV cross-binary match seeds."""
    renamed = 0
    
    for addr, (new_name, reason) in CROSS_MATCH_SEEDS.items():
        if addr in symbols:
            old_name = symbols[addr]['name']
            
            # Only rename if current name is generic
            if old_name.startswith('vt') or old_name.startswith('fn_'):
                symbols[addr]['name'] = new_name
                symbols[addr]['reason'] = f"GTA4_cross_match: {reason}"
                symbols[addr]['confidence'] = 85
                renamed += 1
                print(f"  0x{addr:08X}: {old_name} → {new_name}")
    
    return renamed
```

### Step 2: Propagate to Same Vtable (Priority 2)

```python
def propagate_grcSetup_vtable(symbols, vtable_map):
    """Propagate grcSetup class to other functions in vtable 5594."""
    
    # Find vtable 5594 (grcSetup)
    grcSetup_vtable_id = 5594
    
    # Get all functions in this vtable
    vtable_functions = [
        addr for addr, info in vtable_map.items()
        if info['vtable_id'] == grcSetup_vtable_id
    ]
    
    renamed = 0
    for addr in vtable_functions:
        if addr not in CROSS_MATCH_SEEDS:  # Don't overwrite seeds
            slot = vtable_map[addr]['slot']
            new_name = f"grcSetup_vfn_{slot}"
            
            if addr in symbols:
                old_name = symbols[addr]['name']
                if old_name.startswith('vt'):
                    symbols[addr]['name'] = new_name
                    symbols[addr]['reason'] = "Propagated from GTA4 seeds (same vtable)"
                    symbols[addr]['confidence'] = 75
                    renamed += 1
                    print(f"  0x{addr:08X}: {old_name} → {new_name}")
    
    return renamed
```

### Step 3: Validate Against Existing Names

```python
def validate_seeds(symbols):
    """Check if seeds conflict with existing high-confidence names."""
    
    conflicts = []
    for addr, (new_name, reason) in CROSS_MATCH_SEEDS.items():
        if addr in symbols:
            old_name = symbols[addr]['name']
            old_conf = symbols[addr].get('confidence', 0)
            
            # Check for conflicts
            if old_conf > 85 and old_name != new_name:
                conflicts.append({
                    'addr': addr,
                    'old_name': old_name,
                    'old_conf': old_conf,
                    'new_name': new_name,
                    'new_conf': 85
                })
    
    if conflicts:
        print(f"⚠️  {len(conflicts)} seed conflicts detected:")
        for c in conflicts:
            print(f"  0x{c['addr']:08X}: {c['old_name']} (conf={c['old_conf']}) "
                  f"vs {c['new_name']} (conf={c['new_conf']})")
    
    return conflicts
```

---

## Expected Results

### Before (v9)
```
0x82161D40 = vt5594_5   ; vtable 5594 slot 5
0x82161DB8 = vt5594_6   ; vtable 5594 slot 6
0x821620D0 = vt5594_9   ; vtable 5594 slot 9
0x82162178 = vt5594_8   ; vtable 5594 slot 8
0x82162420 = vt5594_16  ; vtable 5594 slot 16
```

### After (v10 with seeds)
```
0x82161D40 = grcSetup_vfn_2   ; GTA4:0x826F26B8 rage::grcSetup vtable slot 2
0x82161DB8 = grcSetup_vfn_3   ; GTA4:0x82484858 rage::grcSetup vtable slot 3
0x821620D0 = grcSetup_vfn_6   ; GTA4:0x82126C28 rage::grcSetup vtable slot 6
0x82162178 = grcSetup_vfn_5   ; GTA4:0x82484B88 rage::grcSetup vtable slot 5
0x82162420 = grcSetup_vfn_13  ; GTA4:0x82126C68 rage::grcSetup vtable slot 13
```

### After (v10 with propagation)
```
# All ~20 functions in vtable 5594 renamed:
0x82161xxx = grcSetup_vfn_0
0x82161xxx = grcSetup_vfn_1
0x82161D40 = grcSetup_vfn_2   ← seed
0x82161DB8 = grcSetup_vfn_3   ← seed
0x82161xxx = grcSetup_vfn_4
0x82162178 = grcSetup_vfn_5   ← seed
0x821620D0 = grcSetup_vfn_6   ← seed
...
0x82162420 = grcSetup_vfn_13  ← seed
...
```

**Estimated gain**: 5 direct renames + 15-20 propagated = **20-25 total renames**

---

## Confidence Levels

| Source | Confidence | Rationale |
|--------|------------|-----------|
| **GTA IV seed (direct)** | 85% | Vtable slot match + class match + size validation |
| **Vtable propagation** | 75% | Same vtable as confirmed seed |
| **Class propagation** | 60% | Call graph analysis (existing v9 logic) |

**Rule**: Higher confidence overwrites lower confidence.

---

## Testing Checklist

- [ ] Import `cross_match_seeds_v4.py` successfully
- [ ] 5 seeds applied without errors
- [ ] No conflicts with existing high-confidence names (>85%)
- [ ] Vtable 5594 identified correctly
- [ ] Propagation renames 15-20 additional functions
- [ ] Output symbols file validates (no syntax errors)
- [ ] Spot-check: `grcSetup_vfn_2` at `0x82161D40` in output

---

## Troubleshooting

### Issue: "Module not found: cross_match_seeds_v4"
**Solution**: Ensure `cross_match_seeds_v4.py` is in same directory as `auto_namer_v10.py`, or add to Python path:
```python
import sys
sys.path.append('/Users/Ozordi/Downloads/tt-decomp/naming')
from cross_match_seeds_v4 import CROSS_MATCH_SEEDS
```

### Issue: "Vtable 5594 not found"
**Solution**: Check vtable ID mapping. May be different in your vtable analysis. Search for functions at seed addresses:
```python
# Find vtable ID for seed addresses
for addr in CROSS_MATCH_SEEDS.keys():
    if addr in vtable_map:
        print(f"0x{addr:08X} → vtable {vtable_map[addr]['vtable_id']}")
```

### Issue: "Seed conflicts with existing name"
**Solution**: Review conflict report from `validate_seeds()`. If existing name has higher confidence (>85%), keep it. Otherwise, prefer GTA IV seed.

---

## Next Steps After v10

1. **Validate seeds manually**: Disassemble the 5 functions and verify they're graphics-related
2. **Expand vtable matching**: Try relaxed criteria (2-3 candidates) for more matches
3. **Try string literal matching**: Extract debug strings from both binaries
4. **Try alternative source games**: Bully (2006), Max Payne 3 (2012)

See `CROSS_MATCH_GTA4_FINDINGS.md` for detailed recommendations.

---

## Performance Notes

- Cross-match script runtime: **7.6 seconds** (optimized)
- Seeds integration: **<0.1 seconds** (just dict lookup)
- Vtable propagation: **<0.5 seconds** (depends on vtable size)

**Total v10 overhead**: ~0.5 seconds (negligible)

---

## Questions?

Refer to:
- **Full analysis**: `CROSS_MATCH_GTA4_FINDINGS.md`
- **Match report**: `cross_match_gta4_v4_report.txt`
- **Script source**: `cross_match_gta4_v4_fast.py`

Or contact the analyst who generated this report.

---

**Good luck with v10!** 🚀
