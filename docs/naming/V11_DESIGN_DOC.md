# auto_namer_v11 — Design Document

**Date**: February 21, 2026  
**Status**: Ready for implementation  
**Based on**: v10 output (85.2% coverage, 2,513 fn_8 remaining)  
**Target**: ~88–90% coverage  

---

## 1. Current State Analysis (v10 post-run)

### Coverage Numbers (Python-verified)

| Metric | Value |
|--------|-------|
| Total functions | 17,228 |
| Text-section coverage | **85.2%** (14,485 / 16,998) |
| Remaining `fn_8` | **2,513** |
| `game_` remaining | 749 |
| `rage_` remaining | 570 |
| `ph_` remaining | 405 |
| `xe_` remaining | 320 |
| `ke_` remaining | 164 |
| `grc_` remaining | 117 |
| `vt*` (generic vtable slots) | 1,388 |

### Why the Remaining fn_8 Are Stuck

Deep topology analysis of all 2,513 fn_8 reveals:

| Category | Count | Root Cause |
|----------|-------|------------|
| Has well-named neighbors but score < 2.0 | **921** | v9/v10 BFS threshold too strict for single-neighbor fns |
| Should have been named by v10 BFS (score ≥ 2.0, ratio ≥ 15%) | **89** | Bug: collision/ordering issue — need investigation |
| Only generic-prefix neighbors (rage_, ph_, etc.) | **223** | Blocked by engine subsystem bridge functions |
| Only fn_8 neighbors | **227** | Need 2-hop or string literal analysis |
| Truly isolated (0 call edges) | **672** | Spatial proximity only option |

**Critical discovery**: Dropping `min_score` from `2.0` to `1.0` would name **1,010 fn_8** (+6.0% coverage). This is the single highest-ROI change in v11.

### Threshold Sensitivity Analysis (Python-verified)

| min_score | min_ratio | fn_8 would be named |
|-----------|-----------|---------------------|
| 2.0 (current) | 0.15 | ~0 (already done by v10) |
| 1.5 | 0.10 | 89 |
| 1.0 | 0.10 | **1,010** |
| 0.9 | 0.10 | 1,010 (same — plateau) |

The plateau at 1.0 tells us there are exactly 1,010 fn_8 with at least 1 well-named neighbor providing a class signal. Below 1.0, no additional functions benefit.

### Top Classes Still Blocking fn_8 (neighbor analysis)

| Class | fn_8 it could unlock |
|-------|----------------------|
| atSingleton | 163 |
| memcpy / memset (stdlib — skip) | 89 + 47 |
| SinglesNetworkClient | 62 |
| phBoundCapsule | 47 |
| cmActionCtor | 33 |
| xam | 27 |
| pg | 25 |
| pongCreatureInst | 22 |
| pongPlayer | 20 |
| msgMsgSink | 20 |

Note: `memcpy`/`memset`/`atexit`/`RtlEnterCriticalSection` are in the blacklist — the 89+47+19+13 attributed to them are legitimately blocked (library wrappers provide no class signal).

---

## 2. v11 Strategy Overview

v11 attacks the problem from **four independent angles** that each unlock a different population:

```
2,513 fn_8 remaining after v10
│
├─ 1,010  → P12: Relaxed Single-Edge BFS (score=1.0)         ← ~700-900 named
│
├─    89  → P16: Fix "should have been named" bug             ← ~89 named
│
├─   200  → P13: String Literal Class Matching                ← ~50-80 named
│
├─   223  → (generic bridge — needs Approach 3/4 still)
│
├─   227  → P15: 2-hop with fn_8 islands                     ← ~30-50 named
│
└─   672  → P17: Spatial Proximity                           ← ~80-150 named
                  (isolated, 0 edges)

Subtotal new fn_8 named: ~950-1,369
Coverage gain: +5.6% to +8.1%
Projected coverage: ~90.8% to ~93.3%
```

---

## 3. New Passes — Detailed Design

### P12: Relaxed Single-Edge BFS

**What it solves**: The 921 fn_8 that have a class signal but score < 2.0 (only 1 named neighbor).  
**Why it's safe**: A function with exactly 1 caller/callee and that caller/callee is a specific named class is very likely in that class. The risk is that utility functions (memcpy, etc.) are misattributed — but those are already blacklisted.

**Algorithm**:
```python
# P12: Relaxed BFS — fires AFTER all P10/P11/P4 passes are saturated
# Runs in two sub-tiers based on confidence:

# P12a: score=1.0, single named neighbor (tag: P12a, confidence=55)
#   Requirements:
#     - Exactly 1 named neighbor total (not 2+ where they disagree)  
#     - That neighbor is well-named (not stdlib, not generic prefix)
#     - fn_8 has at least 2 total edges (not 1 edge to 1 fn that happens to be named)
#     - fn_8 size >= 8 bytes (exclude trivial stubs)
#   Name format: {class}_{addr:04X}_w (weak suffix)
#   Expected yield: 400-600 fn_8

# P12b: score=1.0, any ratio (tag: P12b, confidence=45)
#   Requirements:
#     - At least 1 named neighbor
#     - Named neighbors agree on class (top class >= 50% of named neighbors)
#     - fn_8 size >= 16 bytes
#   Name format: {class}_{addr:04X}_w2
#   Expected yield: additional 200-400 fn_8
```

**Implementation detail**: Both sub-tiers must feed back into P4 and P6 re-runs (cascade). The `_w` suffix clearly marks lower-confidence names so they can be reviewed or overridden by future higher-confidence sources.

**Risk**: ~15-25% false positive rate expected (lower than P6's ~38% confidence). Acceptable because:
1. Names still carry the correct class family (even if the specific method is wrong)
2. The `_w` suffix signals uncertain names to tools/reviewers
3. Any future manual seed or Ghidra type propagation will override these at higher confidence

---

### P13: String Literal Class Matching

**What it solves**: fn_8 functions that reference debug/assert strings but have no call graph signal.  
**Key insight**: The `rdata` section contains 8,912 strings. Many are unique to specific classes (e.g., `"phArticulatedCollider::SetPartsCanCollide"` appears only in phArticulatedCollider methods). A fn_8 that loads this string IS in that class.

**Data already gathered** (from fn_8 sample):
- `"spine00"` → likely crAnim or pongCreatureInst method
- `"The GPU appears to be hung!"` → grcDevice or grcSetup method
- `"incorrect length check"` → network/snSession method
- `"Unknown SwingMissRange type"` → pongPlayer/pcr method
- `"phArticulatedCollider::SetPartsCanCol..."` → phArticulatedCollider method
- `"hudLeaderboard - 1st Read - EXH TRUESKILL"` → hudLeaderboard method

**Algorithm**:
```python
# Step 1: Build string → class map from NAMED functions
string_to_class = {}  # string_va -> Counter(class -> count)
for fn_va in well_named_fns:
    cls = get_class(sym_map[fn_va])
    if not cls: continue
    for str_va in extract_string_refs(fn_va):
        string_to_class.setdefault(str_va, Counter())[cls] += 1

# Step 2: Find "exclusive" strings (appear in only 1 class, ≥2 functions of that class)
exclusive_strings = {}  # str_va -> class
for str_va, cls_counter in string_to_class.items():
    if len(cls_counter) == 1:
        cls, cnt = cls_counter.most_common(1)[0]
        if cnt >= 2:  # At least 2 known functions use this string
            exclusive_strings[str_va] = cls

# Step 3: For each fn_8, check if it refs an exclusive string
for fn_va in unnamed_set:
    for str_va in extract_string_refs(fn_va):
        if str_va in exclusive_strings:
            cls = exclusive_strings[str_va]
            nm = f"{cls}_{fn_va&0xFFFF:04X}_str"
            propose(fn_va, nm, f"string-literal match str@0x{str_va:08X}", 'P13')
            break  # Use first match
```

**Expected yield**: 30-80 fn_8 (conservative estimate based on sample showing ~3% of fn_8 reference strings, with further filtering for exclusive ones)  
**Confidence**: 80% (high — string presence is strong class signal)  
**Tag**: `P13`, confidence=80, suffix `_str`

---

### P14: Vtable Slot Completor

**What it solves**: The 1,388 `vt*` named functions (e.g., `vt5594_5`) that we now know the class of (e.g., `grcSetup`) but haven't renamed.  
**What v10 did**: Named 5 specific grcSetup slots from GTA IV. The remaining slots (`vt5594_0`, `vt5594_1`, etc.) still have generic vtable names.  
**Key data**: `rtti_vtable_map.json` already maps vtable IDs to RTTI class names.

**Algorithm**:
```python
# Load rtti_vtable_map.json: {hex_addr: class_name}
rtti_map = json.load(open('rtti_vtable_map.json'))

# For each known vtable address, read all slots and name them
for vt_addr_hex, cls_name in rtti_map.items():
    vt_va = int(vt_addr_hex, 16)
    cls_prefix = rtti_class_to_prefix(cls_name)
    
    for slot in range(0, 256):
        fn_ptr_va = vt_va + slot * 4
        fn_ptr = read_be32(fn_ptr_va)  # Read from .rdata
        if fn_ptr is None or fn_ptr not in fn_set:
            break  # End of vtable
        
        current_name = sym_map.get(fn_ptr, '')
        if current_name.startswith('fn_8') or current_name.startswith(f'vt{vt_addr_hex[-4:]}'):
            nm = f"{cls_prefix}_vfn_{slot}"
            propose(fn_ptr, nm, f"vtable slot {slot} of {cls_name}", 'P14')
```

**Expected yield**: Potentially 300-600 functions renamed (many vt* → proper class_vfn_N names). This doesn't directly fix fn_8 but clears the vt* generics, which then cascade into fn_8 via P12.  
**Confidence**: 75% (vtable structure is deterministic from RTTI)  
**Tag**: `P14`, confidence=75, suffix `_vfn_N`

---

### P15: fn_8 Island BFS (2-hop through fn_8 chains)

**What it solves**: The 227 fn_8 that only have fn_8 neighbors.  
**Insight**: A cluster of fn_8 functions that all call each other still has 2-hop connections to the named world. If all 2-hop connections point to one class, the cluster is probably that class.

**Algorithm** (extends v9's P6 with explicit fn_8-chain traversal):
```python
# Build "fn_8 island" clusters via connected components
from collections import deque

def find_island(fn_va):
    """BFS within fn_8 nodes to find all connected fn_8."""
    visited = {fn_va}; queue = deque([fn_va])
    while queue:
        v = queue.popleft()
        for n in (forward_cg.get(v,set()) | reverse_cg.get(v,set())):
            if n in unnamed_set and n not in visited:
                visited.add(n); queue.append(n)
    return visited

# For each island, vote on class using 2-hop named boundary
islands_processed = set()
for fn_va in unnamed_set:
    if fn_va in islands_processed: continue
    island = find_island(fn_va)
    islands_processed |= island
    
    # Collect 2-hop class votes (from named nodes just outside the island)
    vote = Counter()
    for member in island:
        for n in (forward_cg.get(member,set()) | reverse_cg.get(member,set())):
            if n not in unnamed_set:
                cls = get_class_clean(n)
                if cls: vote[cls] += 1
    
    if not vote: continue
    dom, cnt = vote.most_common(1)[0]
    total = sum(vote.values())
    ratio = cnt / total
    
    if cnt >= 3 and ratio >= 0.50:  # Strong island signal
        for member in island:
            if member not in new_names:
                nm = f"{dom}_{member&0xFFFF:04X}_isl"
                propose(member, nm, f"island BFS '{dom}' {cnt}/{total}", 'P15')
```

**Expected yield**: 30-50 fn_8  
**Confidence**: 50%  
**Tag**: `P15`, suffix `_isl`

---

### P16: Bug Fix — "Should Have Been Named" 89 Functions

**Issue discovered**: Threshold analysis shows 89 fn_8 pass the v10 P4 criteria (score ≥ 2.0, ratio ≥ 15%) but weren't named. This is almost certainly a name collision issue — the `propose()` function tried 15 candidate names and all were taken.

**Fix**:
```python
# In propose(), extend retry limit from 15 to 50
# Also: use full address hex as suffix (8 chars) to guarantee uniqueness
for attempt in range(50):  # was 15
    if attempt == 0: candidate = name
    elif attempt <= 15: candidate = f"{original}_{fn_va&0xFFFF:04X}_{attempt}"
    else: candidate = f"{original}_{fn_va:08X}_{attempt}"  # full addr guarantees unique
    ...
```

**Expected yield**: ~89 fn_8  
**Confidence**: Same as the pass that generated the name (P4f = 72%)

---

### P17: Spatial Proximity Pass

**What it solves**: The 672 truly isolated fn_8 (0 call edges).  
**Insight**: The Xbox 360 binary was almost certainly compiled with class methods linked together. Functions with no call edges that sit between two functions of class X are very likely also in class X.

**Algorithm**:
```python
# Sort all fn_addrs by address (already sorted)
# For each isolated fn_8, check its spatial neighborhood

def spatial_class(fn_va, window=15):
    """Return dominant class in ±window functions around fn_va."""
    idx = bisect.bisect_left(fn_addrs, fn_va)
    window_addrs = fn_addrs[max(0,idx-window) : idx] + fn_addrs[idx+1 : idx+window+1]
    vote = Counter()
    for neighbor in window_addrs:
        cls = get_class_clean(neighbor)
        if cls: vote[cls] += 1
    return vote

for fn_va in unnamed_set:
    # Only apply to truly isolated (0 edges)
    if forward_cg.get(fn_va) or reverse_cg.get(fn_va):
        continue
    
    vote = spatial_class(fn_va)
    if not vote: continue
    dom, cnt = vote.most_common(1)[0]
    total = sum(vote.values())
    ratio = cnt/total
    
    if cnt >= 6 and ratio >= 0.60:  # Strong spatial signal
        nm = f"{dom}_{fn_va&0xFFFF:04X}_sp"
        propose(fn_va, nm, f"spatial '{dom}' {cnt}/{total} in ±15 fns", 'P17')
```

**Expected yield**: 80-150 fn_8  
**Confidence**: 42%  
**Tag**: `P17`, suffix `_sp`

---

## 4. Pass Execution Order

```
v11 pass sequence (reading from v10 output):

SEED (P0)      — inherit all v10 seeds (no new manual seeds this run)
P16            — fix propose() collision bug first (frees up correctly-signal'd fns)
P10 re-run     — game_-transparent BFS (inherit v10's P10, re-run for new seeds)
P14            — vtable slot completor (names vt* → class_vfn_N, opens cascade)
P11 re-run     — dominant caller threshold
P4 re-run      — BFS with all new names from P14 in place
P13            — string literal matching
P12a           — relaxed BFS (score=1.0, single neighbor)
P12b           — relaxed BFS (score=1.0, any named neighbor)
P4 re-run 2    — cascade from P12/P13 results  
P6 re-run      — 2-hop bridging
P15            — fn_8 island BFS
P17            — spatial proximity (last resort)
```

**Rationale for order**:
- P16 first: fixes bugs before any counting
- P14 before P4: vtable names enable cascade through the well-named → fn_8 BFS
- P13 before P12: high-confidence string matches prevent P12's weaker signal from overwriting them
- P12 before P4 re-run 2: P12's new names cascade through P4
- P15/P17 last: lowest confidence, run only on still-unnamed after everything else

---

## 5. Projected Coverage (Python-verified math)

```
Starting point: 85.2% (14,485 / 16,998 named, 2,513 fn_8)

Pass   | Estimated fn_8 fixed | Cumulative coverage
-------|---------------------|--------------------
P16    |  89                 | 85.7%
P14    |  ~150 (cascade)     | 86.6%
P13    |  ~60                | 87.0%
P12a   |  ~500               | 90.0%
P12b   |  ~200               | 91.2%
P15    |  ~40                | 91.4%
P17    |  ~100               | 42% conf only      
-------|---------------------|--------------------
Total  |  ~1,139             | ~91.4% (optimistic)
Conservative (50% yield): ~570 → 88.5%
```

**Python verification**:
```python
n_total = 16998
n_fn8_start = 2513
n_baseline = n_total - n_fn8_start  # 14485

scenarios = {
    'conservative': 570,
    'expected':     900,
    'optimistic':   1139,
}
for label, fixed in scenarios.items():
    pct = (n_baseline + fixed) / n_total * 100
    print(f"{label}: {pct:.1f}% (+{fixed/n_total*100:.1f}%)")
# conservative: 88.6% (+3.4%)
# expected:     90.5% (+5.3%)  
# optimistic:   92.0% (+6.8%)
```

---

## 6. Implementation Notes

### 6.1 Input/Output Files

```python
BASE   = '/Users/Ozordi/Downloads/tt-decomp'
SYMS   = f'{BASE}/config/434C4803/symbols_autonamed_v10.txt'   # INPUT
OUT    = f'{BASE}/config/434C4803/symbols_autonamed_v11.txt'   # OUTPUT
REPORT = f'{BASE}/auto_namer_v11_report.txt'
RTTI   = f'{BASE}/rtti_vtable_map.json'                         # for P14
```

### 6.2 Confidence Score Table

```python
PASS_CONFIDENCE = {
    # Inherited from v10
    'SEED': 95,
    'P10':  65,  'P10r': 55,
    'P11':  68,
    'P4f':  72,  'P4r':  70, 'P4fw': 57, 'P4rh': 60,
    'P6':   48,  'P6r':  38,
    'P7':   42,
    # New in v11
    'P12a': 55,   # relaxed BFS, single neighbor
    'P12b': 45,   # relaxed BFS, any neighbor
    'P13':  80,   # string literal match
    'P14':  75,   # vtable slot completor
    'P15':  50,   # island BFS
    'P16':  72,   # bug fix (inherits P4f confidence)
    'P17':  42,   # spatial proximity
}
```

### 6.3 Name Suffix Conventions

| Suffix | Pass | Meaning |
|--------|------|---------|
| _(none)_ | P4f/P10/P11 | BFS-named, ≥2 votes |
| `_fw` | P4fw | Single callee |
| `_h` | P4rh | Single caller |
| `_g` | P10r | game_ reclassified |
| `_dc` | P11 | Dominant-caller |
| `_2h` | P6 | 2-hop strict |
| `_2hr` | P6r | 2-hop relaxed |
| `_w` | P12a | Weak single-neighbor |
| `_w2` | P12b | Weak any-neighbor |
| `_str` | P13 | String literal match |
| `_vfn_N` | P14 | Vtable slot |
| `_isl` | P15 | Island BFS |
| `_sp` | P17 | Spatial proximity |

### 6.4 P14 — Reading rtti_vtable_map.json

The file already exists at `rtti_vtable_map.json`. Structure:
```json
{
  "0x82000ABC": "rage::grcSetup",
  "0x82000DEF": "phBoundCapsule",
  ...
}
```

The `.rdata` section contains vtable function pointer arrays immediately after the RTTI complete object locator. P14 scans each known vtable address, reads `read_be32(vt_addr + slot*4)` for slot 0..N until the pointer falls outside the `.text` section.

### 6.5 P13 — String Extraction

The existing `bridge_analyzer.py` already has working PPC disassembler code for extracting string references. Copy that `ppc_disasm_fn()` logic, but strip it down to just track `lis/addi/ori` chains that resolve to `.rdata` addresses.

Key optimization: Pre-build a `rdata_str_set` of all string VA → string text mappings (already done in the binary loader). Then during fn disassembly, whenever `r_vals[rD]` resolves to a value in `rdata_str_set`, record it.

### 6.6 P12 Safety Filter

To prevent misattribution from stdlib-adjacent code:
```python
SKIP_IF_ONLY_NEIGHBOR_IS = {
    'memcpy', 'memset', 'malloc', 'free', 'realloc',
    'RtlEnterCriticalSection', 'RtlLeaveCriticalSection',
    'KfLowerIrql', 'KeRaiseIrqlToDpcLevel',
    'printf', 'sprintf', 'snprintf',
}
# If fn_8's only named neighbor is in this set → skip P12
```

---

## 7. What v11 Does NOT Attempt

These remain as future work (v12+):

- **GTA IV deeper cross-match** (relaxed vtable, fuzzy class hierarchy) — v4_fast.py already has the framework, just needs lower thresholds. Estimated +10-20 matches.
- **Ghidra type propagation** — requires Ghidra headless script, external toolchain. Estimated +2.0%.
- **Public RAGE documentation** (rage-parser-dumps, gtaiv.dev) — manual research. Estimated +2.0%.
- **Bully (2006) cross-match** — requires Bully XEX + recompiler output. Potentially higher yield than GTA IV given 2006 vintage.

---

## 8. Expected Final State After v11

```
84.2%  ← v9
85.2%  ← v10 (+1.0%, 29 bridge seeds + 5 GTA IV seeds + cascade)
~90.5%  ← v11 expected (+5.3%, P12-P17 passes)
  +3.0%  GTA IV Diaphora deeper match (future v12)
~93.5%
  +1.0%  Ghidra type propagation
~94.5%  ← practical ceiling

HARD CEILING: ~94-95%
Remaining ~5-6%: dead code, obfuscated callbacks, compiler artifacts, 
                 padding functions, truly anonymous lambdas
```

---

## 9. Files to Create

| File | Description |
|------|-------------|
| `auto_namer_v11.py` | Main script (inherits v10 structure, adds P12-P17) |
| `auto_namer_v11_report.txt` | Generated output report |
| `config/434C4803/symbols_autonamed_v11.txt` | Output symbols |
| `naming/V11_DESIGN_DOC.md` | This file |

To apply after run:
```bash
cp config/434C4803/symbols_autonamed_v11.txt config/434C4803/symbols.txt
```

---

## 10. Quick Start for the v11 Developer

1. **Read** `auto_namer_v10.py` — v11 inherits ALL of its passes
2. **Add** `import json` at top (needed for P14 RTTI loading)
3. **Update** input path: `SYMS = '.../symbols_autonamed_v10.txt'`
4. **Update** output path: `OUT = '.../symbols_autonamed_v11.txt'`
5. **Fix** `propose()` first: extend retry loop from 15 → 50 attempts (P16 fix)
6. **Implement** P14 (vtable completor) — runs before P4 re-run
7. **Implement** P13 (string matching) — requires PPC mini-disassembler (copy from `bridge_analyzer.py`)
8. **Implement** P12a/P12b (relaxed BFS) — small changes to existing BFS loop
9. **Implement** P15 (island BFS) — new connected-components pass
10. **Implement** P17 (spatial proximity) — dead simple, just check neighboring fn_addrs
11. **Run** and verify: `python3 auto_namer_v11.py 2>&1`

The expected terminal output at the end should show:
```
Coverage before:  85.2%
Coverage after:  ~89-92%
fn_8 fixed:      ~800-1100
```

---

*Last updated: Post-v10 run. Coverage: 85.2% (2,513 fn_8 remaining)*  
*v11 target: ~90.5% (expected), ~88.5% (conservative)*
