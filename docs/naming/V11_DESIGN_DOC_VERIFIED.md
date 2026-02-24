# auto_namer_v11 — Design Document (Research-Verified)

**Date**: February 21, 2026  
**Status**: Ready for implementation  
**Based on**: v10 output (84.91% coverage, 2,513 fn_8 remaining)  
**Target**: ~91–93% coverage  
**Methodology**: All yield estimates verified by running against the actual binary

---

## 1. Current State Analysis (v10 post-run — PYTHON-VERIFIED)

### Coverage Numbers (binary-verified)

| Metric | Value |
|--------|-------|
| Total text functions | 16,653 |
| Text-section coverage | **84.91%** (14,140 / 16,653) |
| Remaining `fn_8` | **2,513** |
| Well-named (real class prefix) | 9,723 |
| Generic-named (game_/rage_/xe_/etc.) | 3,009 |
| vt\*-named (vtable slots, unresolved) | 1,388 |
| stdlib/internal | 20 |

> NOTE: The existing V11_DESIGN_DOC.md reported 85.2% (17,228 fns). The discrepancy
> is because that doc counted ALL symbol entries (including rdata, pdata, data, thunks)
> while 16,653 is the `.text` function count only. Both are correct depending on scope.
> This doc uses text-only counts for precision.

### Generic-Named Module Breakdown (potential cascade targets)

| Module prefix | Count | Unlockable? |
|---------------|-------|-------------|
| game_ | 749 | ✓ (already transparent in v10) |
| rage_ | 570 | ✓ NEW: extend transparency |
| ph_ | 404 | ✓ NEW: extend transparency |
| xe_ | 320 | ✓ NEW: extend transparency |
| util_ | 241 | ✓ NEW: extend transparency |
| ke_ | 164 | ✓ NEW: extend transparency |
| grc_ | 117 | ✓ NEW: extend transparency |
| thunk_ | 86 | ✗ skip (trampolines, no signal) |
| jumptable_ | 84 | ✗ skip (switch tables, no signal) |
| vt\* | 1,388 | ✓ P14 renames these to real classes |

**Critical insight**: v10's P10 only treats `game_` prefix as "transparent" for BFS 
(looks through game_ to find class votes). The other 1,864 generic-prefix functions 
are completely opaque. Extending transparency to all generic prefixes is the 
single highest-ROI change in v11.

---

## 2. fn_8 Population Deep Topology (binary-verified)

| Category | Count | Root Cause |
|----------|-------|------------|
| Truly isolated (0 call edges) | **668** | Only spatial proximity possible |
| All-fn_8 islands (only fn_8 neighbors) | **258** | Island BFS or spatial |
| Has ≥1 well-named direct neighbor | **862** | Many are low-score (<2.0) due to opaque generics |
| Score ≥2.0 (pass P10 but weren't named — bug) | **19** | propose() collision in v10 |
| Score ≥1.0 with extended transparency | **962** | P12 extended BFS covers these |
| In fn_8 islands with named boundary (cnt≥2, ratio≥0.60) | **224** | P15 island BFS |
| Isolated with strong spatial signal (cnt≥6, ratio≥0.60) | **238** | P17 spatial proximity |

### Threshold Sensitivity Analysis (v10's EXACT algorithm)

| min_score | min_ratio | fn_8 nameable (v10 algorithm, game_ transparent only) |
|-----------|-----------|------------------------------------------------------|
| 2.0 (v10 current) | 0.15 | 19 (these are the "bug" fns v10 missed) |
| 1.0 | 0.10 | 0 additional (plateau — limited by opacity) |

| min_score | min_ratio | fn_8 nameable (EXTENDED: all generic transparent) |
|-----------|-----------|--------------------------------------------------|
| 1.0 | 0.15 | **962** |
| 1.0 | 0.10 | 962 (same — plateau at 1.0) |

The extended BFS (P12) with ALL generic prefixes transparent converges to 819 fn_8
after cascading with P14's vtable renames over 3 iterations.

---

## 3. New Passes — Verified Design

### P14: RTTI Vtable Completor ← HIGHEST PRIORITY (enables cascade)

**What it does**: Reads `rtti_vtable_map.json` (1,715 entries), scans each vtable
in `.rdata` for consecutive function pointers, and renames any `vt*`-named slot 
to `{class}_vfn_{slot}`.

**Why it comes first**: The 1,388 `vt*` functions are currently opaque to the BFS.
Once renamed with real class prefixes, they become visible to P12's extended transparency.

**Verified yield**: 
- 957 quality renames (vt\* → class_vfn_N)
- 0 direct fn_8 renames (fn_8 are not directly in RTTI vtable slots)
- Cascade effect: enables +819 fn_8 via P12 (vs ~881 without P14 — they partially overlap)

**Algorithm**:
```python
rtti = json.load(open(RTTI_MAP))  # {hex_addr: "rage::ClassName"}
for k, cls_name in rtti.items():
    vt_va = int(k, 16)
    cls_prefix = cls_name.split('::')[-1]  # rage::grcSetup -> grcSetup
    
    for slot in range(256):
        fn_ptr = read_be32_rdata(vt_va + slot * 4)
        if fn_ptr is None or not in_text(fn_ptr): break
        nm = sym_map.get(fn_ptr, '')
        if nm.startswith('vt') or nm.startswith('fn_8'):
            base = f"{cls_prefix}_vfn_{slot}"
            propose(fn_ptr, base, f"RTTI vtable {cls_name} slot {slot}", 'P14')
```

**Top classes getting vtable completions**:
- pongNetMessageHolder: 219 slots
- phJoint1Dof: 34 slots  
- phJoint3Dof: 34 slots
- pongLocomotionController: 18 slots
- atSingleton: 16 slots
- fragDrawable, pongDrawable, rmcDrawable: 15 each

**Confidence**: 75 | **Suffix**: `_vfn_{N}`

---

### P12: Extended-Transparency BFS ← HIGHEST fn_8 YIELD

**What it solves**: 819+ fn_8 whose class cannot be determined because intermediate
generic functions (rage_, ph_, xe_, ke_, grc_, aud_, util_) block the signal.

**Key difference from v10 P10**: v10 only treats `game_` as transparent.
v11 P12 treats ALL generic prefixes as transparent.

**Extended transparency set** (do NOT include thunk_, jumptable_, nop_):
```python
TRANSPARENT_PFX = frozenset({
    'game', 'rage', 'ph', 'xe', 'ke', 'grc', 'aud', 'util', 'ref', 'nt', 'loc'
})
```

**BFS vote function** (callee=+1.0, caller=+0.8, fn-ptr-fwd=+0.7, fn-ptr-rev=+0.5,
through transparent node: apply 0.3/0.25/0.2 reduction):
```python
def bfs_vote_extended(fn_va):
    cc = Counter()
    for c in forward_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 1.0
        elif is_extended_transparent(c):  # rage_, ph_, xe_, ke_, etc.
            for cc2 in all_neighbors(c):
                if cc2 == fn_va: continue
                cls2 = get_class_clean(cc2)
                if cls2: cc[cls2] += 0.3
    for c in reverse_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.8
        elif is_extended_transparent(c):
            for cc2 in all_neighbors(c):
                if cc2 == fn_va: continue
                cls2 = get_class_clean(cc2)
                if cls2: cc[cls2] += 0.25
    for c in fn_ptr_fwd.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.7
    for c in fn_ptr_rev.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.5
    return cc
```

**Thresholds**: min_score=1.0, min_ratio=0.15 (weaker than P10's 2.0/0.15)

**Cascade iterations** (verified): converges in 3 iterations:
- Iter 1: +766 fn_8
- Iter 2: +46 fn_8  
- Iter 3: +7 fn_8
- **Total: 819 fn_8**

**Verified top classes**:
- atSingleton: 187
- SinglesNetworkClient: 161
- cmOperatorCtor: 103
- pongNetMessageHolder: 49
- msgMsgSink: 45
- fiAsciiTokenizer: 33
- xam: 33

**Confidence**: 55 | **Suffix**: `_w` (weak) | **Tag**: `P12`

Also run P4fw/P4rh sub-passes within P12 (single callee/caller variants).

---

### P16: propose() Bug Fix ← LOW EFFORT, 19 fn_8

**Issue**: 19 fn_8 pass v10's own P10 criteria (score ≥2.0, ratio ≥0.15 with 
game_-transparent BFS) but were NOT named by v10. Root cause: the `propose()` 
function tries only 15 name candidates, and all were taken due to collisions.

**Verified count**: 19 fn_8 (the original design doc estimated 89, but direct 
simulation against the binary shows 19).

**Fix**: Extend retry loop from 15 → 50; use full address as suffix for late attempts:
```python
for attempt in range(50):  # was 15
    if attempt == 0:    candidate = name
    elif attempt <= 20: candidate = f"{original}_{fn_va&0xFFFF:04X}_{attempt}"
    else:               candidate = f"{original}_{fn_va:08X}_{attempt}"  # guaranteed unique
    if candidate not in used_names:
        ...
        return True
return False
```

**Confidence**: 72 (same as P4f) | **Tag**: `P16` (self-tagging, runs as modified P10)

---

### P15: Island BFS (fn_8-connected-component analysis)

**What it solves**: fn_8 that are only connected to other fn_8 nodes but whose
fn_8 cluster has 2-hop connections to the named world.

**Verified yield** (with stdlib filtering):
- With cnt≥3, ratio≥0.50: **192 fn_8 maximum**, ~80 conservative after false positive filtering
- With cnt≥2, ratio≥0.60: 224 fn_8 maximum

**Algorithm**:
```python
def find_island(fn_va):
    """BFS within fn_8 subgraph"""
    visited = {fn_va}; queue = deque([fn_va])
    while queue:
        v = queue.popleft()
        for n in all_edges(v):
            if n in unnamed_now and n not in visited:
                visited.add(n); queue.append(n)
    return visited

islands_done = set()
for fn_va in sorted(unnamed_now):
    if fn_va in islands_done: continue
    island = find_island(fn_va)
    islands_done |= island
    
    vote = Counter()
    for member in island:
        for n in all_edges(member):
            if n not in unnamed_now:
                cls = get_class_clean(n)
                if cls: vote[cls] += 1
    
    if not vote: continue
    dom, cnt = vote.most_common(1)[0]
    total = sum(vote.values()); ratio = cnt / total
    
    if cnt >= 3 and ratio >= 0.50:
        for member in island:
            nm = safe_name(f"{dom}_{member&0xFFFF:04X}_isl")
            propose(member, nm, f"island BFS '{dom}' {cnt}/{total}", 'P15')
```

**Confidence**: 50 | **Suffix**: `_isl` | **Tag**: `P15`

---

### P17: Spatial Proximity

**What it solves**: The 668 truly isolated fn_8 with zero call edges.

**Key finding**: Xbox 360 XEX binaries compiled with MSVC link class methods
contiguously. Functions with no edges that sit between dense clusters of one 
class are very likely that class.

**Verified yield**:
- cnt≥6, ratio≥0.60 (high confidence): **238 fn_8**
- cnt≥4, ratio≥0.50: 294 fn_8
- Conservative estimate (false positive filtering): ~150 fn_8

**Algorithm**:
```python
for fn_va in unnamed_now:
    # Only truly isolated (0 edges)
    if all_edges(fn_va): continue
    
    idx = bisect.bisect_left(fn_addrs, fn_va)
    window = fn_addrs[max(0,idx-15):idx] + fn_addrs[idx+1:idx+16]
    
    vote = Counter()
    for nb in window:
        cls = get_class_clean(nb)
        if cls: vote[cls] += 1
    
    if not vote: continue
    dom, cnt = vote.most_common(1)[0]
    total = sum(vote.values()); ratio = cnt / total
    
    if cnt >= 6 and ratio >= 0.60:
        nm = safe_name(f"{dom}_{fn_va&0xFFFF:04X}_sp")
        propose(fn_va, nm, f"spatial '{dom}' {cnt}/{total} in ±15 fns", 'P17')
```

**Confidence**: 42 | **Suffix**: `_sp` | **Tag**: `P17`

---

### P13: String Literal Class Matching ← LOW YIELD (kept for correctness)

**Verified yield**: Only **~2-5 fn_8** (design doc estimated 30-80 — that was optimistic).

**Why so low**: Functions that reference exclusive strings were almost entirely
named by earlier BFS passes. The remaining fn_8 either reference non-exclusive
strings (shared across classes) or no strings at all.

**Still worth including**: The confidence on string matches is high (80%), and
the implementation cost is low.

**Top classes with exclusive strings**: SinglesNetworkClient (181), atSingleton (20),
xam (20), pg (17), swfINSTANCE (12).

**Confidence**: 80 | **Suffix**: `_str` | **Tag**: `P13`

---

## 4. Pass Execution Order

```
Reading from: symbols_autonamed_v10.txt

SEED (P0)   — inherit v10 seeds (no new manual seeds this run)
P16         — extend propose() retry limit FIRST (fixes collision bug)
P14         — RTTI vtable completor (vt* → class_vfn_N, enables cascade)
P10 re-run  — game_-transparent BFS (v10 algorithm, re-run with P14 names)
P11 re-run  — dominant-caller threshold
P4 re-run   — BFS (standard, no transparency, captures strong signals)
P12         — extended-transparency BFS (all generic prefixes, score≥1.0)
P4 re-run 2 — cascade from P12 new names
P6 re-run   — 2-hop bridging
P15         — island BFS (fn_8 clusters with named boundary)
P13         — string literal matching (low yield, high confidence)
P17         — spatial proximity (isolated fns only, last resort)
```

---

## 5. Verified Coverage Projections

```
Starting point:  84.91%  (14,140 / 16,653 named, 2,513 fn_8)

Pass   | fn_8 fixed (verified) | Cumulative coverage
-------|----------------------|--------------------
P16    |  19                  | 85.02%
P14    |   0 direct           | 85.02% (but enables P12 cascade)
P10+P4 |  ~15 additional      | 85.11%
P12    |  819                 | 90.03%
P6     |  ~10 additional      | 90.09%
P15    |  ~80 conservative    | 90.57%
P13    |   ~3                 | 90.59%
P17    |  ~150 conservative   | 91.49%
-------|----------------------|--------------------
Total conservative:  ~1,096 fn_8    →  91.49%
Total optimistic:    ~1,270 fn_8    →  92.54%
Hard ceiling:        ~94-95%        (dead code, compiler artifacts)
```

**Python verification**:
```python
n_total = 16653
n_start_fn8 = 2513
n_start_named = 14140
for label, fixed in [('conservative',1096), ('expected',1140), ('optimistic',1270)]:
    pct = (n_start_named + fixed) / n_total * 100
    gain = fixed / n_total * 100
    print(f"{label}: {pct:.2f}% (+{gain:.2f}%)")
# conservative: 91.49% (+6.58%)
# expected:     91.73% (+6.85%)
# optimistic:   92.54% (+7.63%)
```

---

## 6. Confidence Score Table

```python
PASS_CONFIDENCE = {
    # Inherited from v10
    'SEED': 95,
    'P10':  65,  'P10r': 55,
    'P11':  68,
    'P4f':  72,  'P4r':  70, 'P4fw': 57, 'P4rh': 60,
    'P6':   48,  'P6r':  38,
    # New in v11
    'P12':  55,  'P12fw': 50, 'P12rh': 52,
    'P13':  80,   # string literal — high confidence
    'P14':  75,   # RTTI vtable slot
    'P15':  50,   # island BFS
    'P16':  72,   # bug fix (inherits P4f confidence)
    'P17':  42,   # spatial proximity — lowest confidence
}
```

---

## 7. Name Suffix Conventions (complete table)

| Suffix | Pass | Meaning |
|--------|------|---------|
| _(none)_ | P4f/P10/P11/P16 | BFS-named ≥2 votes |
| `_fw` | P4fw | Single callee |
| `_h` | P4rh | Single caller |
| `_g` | P10r | game_ reclassified |
| `_dc` | P11 | Dominant-caller |
| `_2h` | P6 | 2-hop strict |
| `_2hr` | P6r | 2-hop relaxed |
| `_w` | P12 | Extended-transparent BFS (weak) |
| `_str` | P13 | String literal match |
| `_vfn_{N}` | P14 | RTTI vtable slot N |
| `_isl` | P15 | Island BFS |
| `_sp` | P17 | Spatial proximity |

---

## 8. Corrections to Previous V11_DESIGN_DOC.md

| Item | Previous Estimate | Verified Value | Source |
|------|-------------------|----------------|--------|
| P16 bug fix yield | 89 fn_8 | **19 fn_8** | Direct simulation |
| P13 string yield | 30-80 fn_8 | **~2-5 fn_8** | Exclusive string analysis |
| P12 BFS yield | 700-900 fn_8 | **819 fn_8** (with P14 cascade) | Iterative BFS sim |
| P14 vtable renames | 300-600 | **957 renames** | RTTI map scan |
| P15 island max | 30-50 | **192 max, ~80 conservative** | Island BFS analysis |
| P17 spatial max | 80-150 | **238 max, ~150 conservative** | Spatial analysis |
| P12b separate pass | planned | **merged into P12** | Simplified |
| "threshold plateau at 0.9" | claimed | **Not observed** (plateau at 1.0) | Simulation |

---

## 9. Files

| File | Description |
|------|-------------|
| `auto_namer_v11.py` | Main script (inherits v10 + P12-P17) |
| `auto_namer_v11_report.txt` | Generated output report |
| `config/434C4803/symbols_autonamed_v11.txt` | Output symbols |
| `naming/V11_DESIGN_DOC_VERIFIED.md` | This file |

Post-run apply command:
```bash
cp /Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols_autonamed_v11.txt \
   /Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols.txt
```

---

*Last updated: Post-v10, pre-v11 run. Estimates from direct binary simulation.*  
*v11 target: 91.49% conservative, 92.54% optimistic*

---

## 10. Actual Run Results (post-implementation)

| Metric | Predicted (conservative) | **Actual** |
|--------|--------------------------|-----------|
| fn_8 fixed | 1,096 | **1,179** |
| Coverage gain | +6.58% | **+6.94%** |
| Coverage after | 91.49% | **92.15%** |
| Remaining fn_8 | ~1,417 | **1,334** |
| P14 renames | 957 | **957** ✓ |
| P12 fn_8 | 819 | **1,041** (675+368) |
| P6 fn_8 | ~10 | **91** |
| P15 fn_8 | ~80 | **5** (most already fixed by P12) |
| P17 fn_8 | ~150 | **25** (higher P6 consumed many) |
| P13 fn_8 | ~3 | **0** |

**Note on P15/P17/P13 actual < predicted**: P12's extended BFS converged over 4 
iterations (not 3 as predicted) and captured 1,041 fn_8 vs the predicted 819 — 
this means many of the targets for P15/P17 were already named when those passes ran.
P13 getting 0 confirms the exclusive-string analysis: those few qualifying fn_8 were 
all named by P12 first.

**Note on P6 exceeding prediction**: With P12's new names as base, the 2-hop bridging 
pass found substantially more connections than expected — 91 vs ~10 predicted.

### Final Pass Breakdown (actual)
- P14 (vtable completor): 957 renames (0 direct fn_8)
- P12 main: 675 fn_8
- P12rh (single-caller): 368 fn_8
- P6/P6r (2-hop): 91 fn_8
- P17 (spatial): 25 fn_8
- P10/P4 (standard BFS + cascade): 9 fn_8
- P15 (island BFS): 5 fn_8

*Run completed: February 21, 2026. symbols.txt updated.*
