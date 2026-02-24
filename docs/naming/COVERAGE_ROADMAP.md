# Naming Coverage Roadmap: Path to 100%

## State after auto_namer_v9 (current)

| Metric | Value |
|---|---|
| Text-section coverage | **84.2%** (14,306 / 16,998 named) |
| Remaining `fn_8` | **2,692** |
| Generic-named (`game_`, `rage_`, `ph_`, etc.) | **~3,600** (843 `game_` + `rage_`/`ph_`/etc.) |
| Well-named functions | **~10,600** |
| v9 fn_8 fixed | **1,122** |
| v9 game_ reclassified | **1,870** |

**Previous state (v8): 77.6% — v9 gained +6.6% in one run.**

### What v9 did
- **2 manual bridge seeds** (`atSingleton_Find_90D0`, `snSession_AddNode_C068`) provided the
  class identity needed to unlock their 99 + 34 fn_8 neighborhoods via cascade.
- **P10 game_-transparent BFS** propagated class labels through `game_` nodes as pass-through,
  reclassifying **1,870** of 2,715 `game_` functions and naming **306** fn_8 directly.
- **P4 BFS re-run** with the expanded name set added another **561** (fw=367, rh=147).
- **P6 2-hop re-run** added **255** more from newly connected islands.

### Why the two seeds were so impactful
- `game_90D0` had **214 callers** across every major subsystem — once it was identified
  as `atSingleton_Find_90D0`, all 214 calling functions gained a class signal that cascaded
  into their fn_8 neighbors.
- `game_C068` (linked-list insert) had **66 callers** all within `sn*Machine` — naming it
  `snSession_AddNode_C068` connected the entire session state machine cluster.

---

## The Remaining 2,692 fn_8 — Revised Populations

After v9, the remaining fn_8 fall into harder categories:

- **~900** have generic-prefix (`rage_`, `ph_`, `grc_`) neighbors only — engine subsystem
  functions where we know the class family but not the specific method
- **~700** are truly isolated (zero bl or fn-ptr edges at all)
- **~400** only call stdlib wrappers (memcpy/memset) — no class signal
- **~692** remain as bridge functions with game_ chains that still resolve to nothing

---

## Updated Approach Stack

Starting from **84.2%** (v9):

```
84.2%  ← v9 current
  + 3.0%  GTA IV Xbox 360 cross-binary matching (Diaphora/BinDiff)
            → names rage_/ph_/grc_ engine generics directly
  ─────
87.2%  ← achievable with GTA IV comparison (fully legal)

  + 1.5%  Manual naming of next tier of bridge functions
            → top-20 remaining game_ nodes with ≥5 fn_8 neighbors
            → ~10 hours work
  ─────
88.7%

  + 2.0%  Ghidra type propagation from known RAGE structs
            → resolves the 900 rage_/ph_ generics that have struct context
  ─────
90.7%

  + 2.0%  Public RAGE documentation + rage-parser-dumps/gtaiv.dev research
  ─────
92.7%

  + 1.0%  Pattern analysis of remaining small functions
  ─────
~93.7%  ← practical ceiling with legitimate static analysis

  HARD CEILING: ~94-95%
  Remaining ~5-6%: dead code, obfuscated callbacks, compiler artifacts
```

---

## Next Bridge Functions to Manually Analyze

After v9, the new top `game_` bridges (by fn_8 cascade potential) — run the
bridge analysis script to get the updated list, as many previous targets are now resolved.

Key remaining unlocked `rage_` clusters:
- `rage_XXXX` functions with `ph_` neighbors — physics engine boundary
- `xe_XXXX` functions — Xbox kernel adapter layer
- `grc_XXXX` functions — graphics command buffer layer

---

## v9 Auto-Namer Architecture

`auto_namer_v9.py` introduced three new passes over v8's foundation:

**MANUAL_SEEDS** — a dict of address → (name, reason) applied before any BFS.
  Add entries here as functions are manually analyzed.

**P10: game_-transparent BFS** — treats `game_XXXX` as pass-through nodes.
  When a game_ function has a dominant class in its 1-hop neighborhood (score ≥ 1.5,
  ratio ≥ 20%), it gets reclassified with that class. This cascades across game_→game_
  chains over multiple iterations. fn_8 nodes use a slightly higher threshold (score ≥ 2.0).

**P11: Dominant-caller threshold** — fn_8 with ≥5 callers where ≥80% share a class
  get named directly. (Yielded 0 in v9 — P10 already handled these cases.)

Both passes feed back into P4 and P6 re-runs for full cascade propagation.

---

## On the GTA V PC Source Code

**Still not recommended** for direct use. The legal position hasn't changed:
Rockstar/Take-Two actively pursues projects that use the 2023 leak.

**GTA IV Xbox 360 XEX remains the recommended approach** — same ~2007 RAGE engine,
no legal risk, covers the same class hierarchy. At 84.2% coverage, this is now the
highest-ROI next step since the low-hanging game_ fruit has been largely picked.

---

## File Locations

| File | Description |
|---|---|
| `config/434C4803/symbols_autonamed_v9.txt` | Current best output |
| `config/434C4803/symbols.txt` | Active symbols file (copy v9 here to apply) |
| `auto_namer_v9.py` | v9 script with manual seeds + P10 |
| `auto_namer_v9_report.txt` | Full rename log for v9 |
| `naming/COVERAGE_ROADMAP.md` | This document |
| `naming/archive/` | v1–v8 historical versions |

To apply v9: `cp config/434C4803/symbols_autonamed_v9.txt config/434C4803/symbols.txt`

---

*Last updated: Post-v9 run. Coverage: 84.2% (2,692 fn_8 remaining, 843 game_ remaining)*
