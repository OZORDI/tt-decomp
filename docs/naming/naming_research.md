# Function Naming Research

## What we have to work with

**Binary:** `orig/434C4803/pong_xenon_final.pe`
**Unnamed functions (`fn_8...`):** 8,225
**Already named:** 8,428

## The debug string approach — what actually works

### What the strings are

The binary has **419 Class::Method strings** embedded in `.rdata`, all of the form:
```
pongCreature::StartEmote() - trying to play nonexistent anim '%s'
pgStreamer::Read - invalid handle
gmLogicSinglesMatch::MidGame(): Unknown PointsPerGame enum value = %d
```
These are **assert/diagnostic strings** that RAGE-engine games embed so that
crashes print meaningful output. They are *not* encrypted or obfuscated.

### How code references them — the bug that broke Pass 1

PPC Xbox 360 loads a 32-bit constant via a **lis / addi pair**:
```asm
lis  r11, 0x8202         ; load high 16 bits into r11
addi r20, r11, 0x79D4   ; r20 = 0x82020000 + 0x79D4 = 0x820279D4
```
Two things tripped up the first auto_namer.py:

1. **Wrong hi calculation.**
   For a VA like `0x820279D4`: `lo = 0x79D4` (< 0x8000, so no sign compensation).
   Correct: `hi = (VA + 0x8000) >> 16` handles the general case for both
   lo < 0x8000 and lo >= 0x8000. The naive `VA >> 16` is wrong when lo >= 0x8000.

2. **Wrong register constraint.**
   The result register of addi does NOT have to be the same as the lis destination.
   `lis r11, hi` then `addi r20, r11, lo` is perfectly legal PPC. The constraint is
   only that addi's *source* (ra field) equals the lis destination register.
   The original check `rd2 == rd` should have been `ra2 == rd`.

### Verified results

After fixing both bugs, running on `pongCreature::StartEmote` at `0x820279D4`:
- Found 1 reference: `lis` at `0x820C7840`, `addi` at `0x820C7848`
- Owning function: `game_7530` at `0x820C7530`
- Confirmed correct: the function IS `pongCreature::StartEmote`

Full census with corrected search:
- **376 Class::Method strings** (after filtering to proper string-start boundaries)
- **385 references resolved** in .text
- **195 unique functions identified** by class via string reference
- **33 of those are currently unnamed** (`fn_8...`) — direct renames

### Why only 33 direct renames?

Most of the 195 class-matched functions already have partial names (`game_XXXX`,
`ph_XXXX`, `pg_XXXX`). The string approach doesn't give us NEW names for them,
it gives us their TRUE class identity. That matters for the next step.

---

## The real scale: call-graph propagation

**4,939 unnamed functions call at least one already-named function.**

This is the actual lever. The strategy:

1. **Seed set (195 functions):** string-matched functions whose class is now known.
2. **Call graph expansion:** for each unnamed function, look at what it calls.
   If ≥2 of its callees belong to the same class (e.g. `pongCreature`), that
   function is very likely also a `pongCreature` method.
3. **Vtable slot matching:** functions at vtable address positions inherit the
   class name of the vtable owner, with a slot-index suffix.
4. **Thunks:** 85 four-byte functions that are a single `b target` — name them
   `thunk_<target_name>`.

### Expected yield per pass

| Pass | Method | Expected names |
|------|--------|---------------|
| 1 | String reference (corrected) | ~33 direct + rename 162 partial |
| 2 | Thunks (single `b target`) | ~85 |
| 3 | Call-graph propagation (≥2 same-class callees) | ~500–1500 |
| 4 | Vtable slot inheritance | ~500–2000 |
| 5 | NOP/stub (4-byte `blr`) | ~100 |

Total realistic: **1,200–3,700** new names from ~8,225 unnamed.

---

## What NOT to use

**The pseudocode file** (`table_tennis_all_pseudocode_symbols_raw.c`) has broken
control flow — `JUMPOUT()`, `LODWORD()`, and fragmented basic blocks mean the
function boundaries are wrong and the call sites are unreliable. Do not use it
for automated analysis. It is useful only for manual reading of individual
functions you already know the address of.

---

## Implementation plan for auto_namer_v2.py

The script must do these things in order:

### Pass 1 — String reference (corrected algorithm)
```
For each Class::Method string in .rdata:
  Compute hi = (VA + 0x8000) >> 16,  lo = VA & 0xFFFF
  Look up hi in pre-built lis_index
  For each matching lis at offset i with register rD:
    Scan forward ≤20 instructions for: addi rX, rD, lo  (ra==rD, any rd)
    If found: record owning_function -> (Class, Method, full_string)
```

### Pass 2 — Thunks
```
For each unnamed 4-byte function:
  If the single instruction is 'b target' (op=18, LK=0):
    Name it thunk_<target_name>
```

### Pass 3 — Call-graph propagation
```
Build: fn_va -> set of called fn_vas  (from all bl instructions)
Build: fn_va -> class  (from Pass 1 + existing named functions)

For each unnamed fn:
  callees = set of functions it calls
  callee_classes = Counter of classes of named callees
  if dominant_class appears >= 2 times AND >= 50% of named callees:
    prefix function with that class name
```

### Pass 4 — Vtable slot inheritance  
```
Scan .rdata for pointer arrays (≥4 consecutive valid .text pointers)
For each vtable:
  Identify owning class from known functions already in that vtable
  Name unnamed slots as ClassName_vtXXXX_slotN
```

### Pass 5 — Stubs
```
For each unnamed function with size ≤ 8:
  If body is just blr (0x4E800020) -> nop_XXXX
  If body contains trap (0x7FE00008) -> pure_XXXX
```
