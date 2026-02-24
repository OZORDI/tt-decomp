#!/usr/bin/env python3
"""
Phase 4A: VCALL and Virtual Dispatch Full Resolution
======================================================
Transforms recomp/structured_pass5/ -> recomp/structured_pass5_ph4/

Two independent passes on each file:

  PASS 1 — Pattern B: Wider-window PPC_CALL_INDIRECT_FUNC detection
    The original stage3_vtables.py required 3 consecutive lines.
    This pass looks up to WINDOW=15 lines back for the vtable+slot pattern:
      Line A:  ctx.rJ.u64 = PPC_LOAD_U32(OBJ + 0)      <- vtable load (offset MUST be 0)
      Line B:  ctx.rK.u64 = PPC_LOAD_U32(ctx.rJ.u32 + N) <- slot load (N must be mult of 4)
      Line C:  PPC_CALL_INDIRECT_FUNC(ctx.rK.u32)        <- indirect call
    → Replace A+B+C with: VCALL(OBJ, N/4, ctx, base);   (or named symbol if known)
    → Also handles var_rN forms.

  PASS 2 — Function-scope type tracking for VCALL(ctx.r3.u32, slot) resolution
    For any function named ClassName_HEXADDR:
      - ctx.r3 at entry = ClassName* (the 'this' pointer)
      - Track: var_rN = ctx.r3.u32 → var_rN typed as ClassName*
      - Track: ctx.r3.u64 = var_rN → restore, re-type ctx.r3 as ClassName*
      - At each VCALL(ctx.r3.u32, slot): if ctx.r3 is typed, look up named slot
      - Also: after ClassName2_X(ctx,base) call, mark ctx.r3 = ClassName2 if ctor pattern

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/phase4a_vcall_resolve.py [--dry-run] [--file N] [--window W]
"""

import re, json, os, sys, shutil, argparse
from pathlib import Path
from collections import defaultdict

BASE        = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG         = BASE / "config/434C4803"
INPUT_DIR   = BASE / "recomp/structured_pass5"
OUTPUT_DIR  = BASE / "recomp/structured_pass5_ph4"
INCLUDE_DIR = BASE / "include"

WINDOW = 15  # lines to look back for Pattern B

# ─────────────────────────────────────────────────────────────────────────────
# Load lookup tables
# ─────────────────────────────────────────────────────────────────────────────

def load_tables():
    with open(CFG / "vtable_layout_map.json") as f:
        raw = json.load(f)

    # (class_name, slot_int) -> symbol_name
    slot_to_sym = {}
    for class_name, slots in raw.items():
        for slot_str, info in slots.items():
            slot_to_sym[(class_name, int(slot_str))] = info["symbol"]

    # vtable_addr_hex -> (class_name, slot_int)  — for named resolution
    addr_to_slot = {}
    for class_name, slots in raw.items():
        for slot_str, info in slots.items():
            addr_to_slot[info["addr"]] = (class_name, int(slot_str))

    print(f"  Loaded: {len(raw)} classes, {len(slot_to_sym)} named vtable slots")
    return raw, slot_to_sym, addr_to_slot


# ─────────────────────────────────────────────────────────────────────────────
# Regex patterns
# ─────────────────────────────────────────────────────────────────────────────

# PPC_FUNC_IMPL header line
RE_FUNC_IMPL = re.compile(
    r'^(?:__attribute__\(\(alias\("[^"]+"\)\)\)\s+PPC_WEAK_FUNC\([^)]+\);\s*\n)?'
    r'PPC_FUNC_IMPL\(__imp__([a-zA-Z_][a-zA-Z0-9_]*)\)'
)
RE_FUNC_IMPL_LINE = re.compile(r'^PPC_FUNC_IMPL\(__imp__([a-zA-Z_][a-zA-Z0-9_]*)\)')

# Register extraction helpers — matches ctx.rN or var_rN
RE_CTX_REG  = re.compile(r'ctx\.(r\d+)')
RE_VAR_REG  = re.compile(r'var_(r\d+)')
RE_ANY_REG  = re.compile(r'(?:ctx\.(r\d+)|var_(r\d+))')

# Line A: ctx.rJ.u64 = PPC_LOAD_U32(OBJ + 0)  [vtable pointer load]
#   OBJ can be: ctx.rN.u32, var_rN, or an annotated symbol
RE_VT_LINE_A = re.compile(
    r'^(\s*)'
    r'(ctx\.(r\d+)|var_(r\d+))\.u64\s*=\s*(?:\(uint32_t\)\()?'
    r'PPC_LOAD_U32\('
    r'(ctx\.(r\d+)\.u32|var_(r\d+)|[A-Za-z_][A-Za-z0-9_]*)'  # OBJ
    r'\s*\+\s*0\)'
    r'(?:\))?'
    r'\s*;'
)

# Line B: ctx.rK.u64 = PPC_LOAD_U32(ctx.rJ.u32 + N)  [vtable slot load]
RE_VT_LINE_B = re.compile(
    r'^(\s*)'
    r'(ctx\.(r\d+)|var_(r\d+))\.u64\s*=\s*(?:\(uint32_t\)\()?'
    r'PPC_LOAD_U32\('
    r'(ctx\.(r\d+)\.u32|var_(r\d+))'  # vtable reg
    r'\s*\+\s*(\d+)\)'
    r'(?:\))?'
    r'\s*;'
)

# Line C: PPC_CALL_INDIRECT_FUNC(ctx.rK.u32) or (var_rK)
RE_VT_LINE_C = re.compile(
    r'^(\s*)PPC_CALL_INDIRECT_FUNC\('
    r'(ctx\.(r\d+)\.u32|var_(r\d+))'
    r'\)\s*;'
)

# VCALL(ctx.r3.u32, slot, ctx, base)
RE_VCALL = re.compile(
    r'^(\s*)VCALL\(ctx\.r3\.u32,\s*(\d+),\s*ctx,\s*base\)\s*;(.*)'
)

# ctx.r3.u64 assignment (to track type flow)
RE_R3_ASSIGN = re.compile(
    r'^\s*ctx\.r3\.(u64|s64|u32)\s*=\s*(.+?)\s*;'
)

# var_rN = ctx.r3.u32  (this-pointer save)
RE_THIS_SAVE = re.compile(
    r'^\s*(var_(r\d+))\s*=\s*ctx\.r3\.u32\s*;'
)

# ctx.r3.u64 = var_rN or ctx.r3.u64 = (uint64_t)var_rN  (type restore)
RE_R3_RESTORE = re.compile(
    r'^\s*ctx\.r3\.u64\s*=\s*(?:\(uint64_t\))?(var_r\d+)\s*;'
)

# Named function call ending in (ctx, base)
RE_NAMED_CALL = re.compile(
    r'^\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*\(ctx,\s*base\)\s*;'
)


# ─────────────────────────────────────────────────────────────────────────────
# Helper: extract class name from function name
# E.g. "pongPlayer_5FB8_g" -> "pongPlayer"
#      "msgMsgSink_1234"    -> "msgMsgSink"
# ─────────────────────────────────────────────────────────────────────────────

def extract_class(func_name, vtable_classes):
    """Extract class name prefix from a function name. Returns None if not a known class."""
    # Try longest prefix match first
    best = None
    for cls in vtable_classes:
        if func_name.startswith(cls + "_") or func_name.startswith(cls + "_vfn"):
            if best is None or len(cls) > len(best):
                best = cls
    return best


# ─────────────────────────────────────────────────────────────────────────────
# Helper: get canonical register name from a regex match
# ─────────────────────────────────────────────────────────────────────────────

def get_canon_reg(match, ctx_grp, var_grp):
    """Return 'ctx.rN' or 'var_rN' from match groups."""
    ctx = match.group(ctx_grp) if ctx_grp <= match.lastindex else None
    var = match.group(var_grp) if var_grp <= match.lastindex else None
    if ctx:
        return f"ctx.{ctx}"
    if var:
        return f"var_{var}"
    return None


def get_reg_from_str(s):
    """Extract 'ctx.rN' or 'var_rN' from a string like 'ctx.r3.u32' or 'var_r31'."""
    m = re.match(r'ctx\.(r\d+)', s)
    if m:
        return f"ctx.{m.group(1)}"
    m = re.match(r'var_(r\d+)', s)
    if m:
        return f"var_{m.group(1)}"
    return None


# ─────────────────────────────────────────────────────────────────────────────
# PASS 1: Pattern B — Wider-window PPC_CALL_INDIRECT_FUNC → VCALL
# ─────────────────────────────────────────────────────────────────────────────

def reg_overwritten_between(lines, start_i, end_i, reg_name):
    """
    Returns True if reg_name (e.g. 'ctx.r8' or 'var_r28') is
    assigned to in lines[start_i+1 : end_i].
    """
    # Build pattern: ctx.r8.u64 = ..., ctx.r8.u32 = ..., var_r28 = ...
    if reg_name.startswith("ctx."):
        r = reg_name[4:]  # e.g. 'r8'
        pat = re.compile(r'\bctx\.' + re.escape(r) + r'\.(u64|s64|u32)\s*=')
    else:
        pat = re.compile(r'\b' + re.escape(reg_name) + r'\s*=')

    for i in range(start_i + 1, end_i):
        if pat.search(lines[i]):
            return True
    return False


def pass1_pattern_b(lines, slot_to_sym, vtable_classes, stats):
    """
    Scan lines for Pattern B vtable dispatch and replace with VCALL/named calls.
    Returns new lines list.
    """
    out = list(lines)
    used = set()  # lines to delete (replaced by VCALL)

    for i, line in enumerate(out):
        if i in used:
            continue

        m_c = RE_VT_LINE_C.match(line)
        if not m_c:
            continue

        # Line C found: PPC_CALL_INDIRECT_FUNC(call_reg)
        indent_c = m_c.group(1)
        call_reg_str = m_c.group(2)
        call_reg = get_reg_from_str(call_reg_str)
        if not call_reg:
            continue

        # Search backwards for Line B: where call_reg was last set from a vtable load
        line_b_idx = None
        vtable_reg = None
        byte_offset = None

        for j in range(i - 1, max(-1, i - WINDOW - 1), -1):
            if j in used:
                continue
            m_b = RE_VT_LINE_B.match(out[j])
            if not m_b:
                continue

            # Check if this line sets call_reg
            dst_str = m_b.group(2)
            dst_reg = get_reg_from_str(dst_str)
            if dst_reg != call_reg:
                continue

            # Check call_reg not overwritten between j and i
            if reg_overwritten_between(out, j, i, call_reg):
                continue

            vtable_reg_str = m_b.group(5)
            vtable_reg = get_reg_from_str(vtable_reg_str)
            byte_offset = int(m_b.group(8))
            line_b_idx = j
            break

        if line_b_idx is None:
            continue

        # Sanity: byte_offset must be multiple of 4
        if byte_offset % 4 != 0:
            continue

        # Search backwards from line_b_idx for Line A: vtable_reg loaded from obj+0
        line_a_idx = None
        obj_str = None

        for j in range(line_b_idx - 1, max(-1, line_b_idx - WINDOW - 1), -1):
            if j in used:
                continue
            m_a = RE_VT_LINE_A.match(out[j])
            if not m_a:
                continue

            dst_str = m_a.group(2)
            dst_reg = get_reg_from_str(dst_str)
            if dst_reg != vtable_reg:
                continue

            # Check vtable_reg not overwritten between j and line_b_idx
            if reg_overwritten_between(out, j, line_b_idx, vtable_reg):
                continue

            # Extract OBJ: could be ctx.rN.u32 or var_rN or symbol name
            obj_groups = [m_a.group(6), m_a.group(7)]  # ctx.rN, var_rN
            obj_ctx = m_a.group(6)  # e.g. 'r3' from 'ctx.r3.u32'
            obj_var = m_a.group(7)  # e.g. 'r31' from 'var_r31'
            if obj_ctx:
                obj_str = f"ctx.{obj_ctx}.u32"
            elif obj_var:
                obj_str = f"var_{obj_var}"
            else:
                obj_str = m_a.group(5)  # symbol name

            line_a_idx = j
            break

        if line_a_idx is None:
            continue

        # We have a valid triplet! Emit VCALL
        slot = byte_offset // 4

        # Try to find a named symbol for slot via some class context
        # (We'll try to use the current function's class in Pass 2;
        #  here just emit the generic VCALL macro)
        replacement = f"{indent_c}VCALL({obj_str}, {slot}, ctx, base);  // pattern-B slot {slot} (byte +{byte_offset})\n"

        # Mark A and B as used (delete), replace C
        out[line_a_idx] = f"{out[line_a_idx].rstrip()} // vtable-ptr-load -> collapsed into VCALL below\n"
        # Actually: completely remove A and B, keep replacement at C
        # But we can't easily delete in one pass — mark A/B as blank comment
        out[line_a_idx] = f"  // [ph4a] vtable load collapsed\n"
        out[line_b_idx] = f"  // [ph4a] slot load collapsed\n"
        out[i] = replacement

        stats["pattern_b_resolved"] += 1

    return out


# ─────────────────────────────────────────────────────────────────────────────
# PASS 2: Function-scope type tracking → VCALL(ctx.r3.u32, slot) resolution
# ─────────────────────────────────────────────────────────────────────────────

def pass2_vcall_type_resolve(lines, vtable_classes, slot_to_sym, stats):
    """
    Process a single function's lines. Returns new lines with typed VCALL annotations
    and direct symbol calls where possible.
    """
    out = list(lines)
    func_name = None
    class_name = None

    # Find function name
    for line in out:
        m = RE_FUNC_IMPL_LINE.match(line)
        if m:
            func_name = m.group(1)
            class_name = extract_class(func_name, vtable_classes)
            break

    if not class_name:
        return out  # No class context

    # State machine
    var_types = {}     # var_name -> class_name
    ctx_r3_type = class_name  # At function entry, r3 IS the this-pointer
    this_var = None    # Which var holds the this-pointer
    this_var_found = False

    for i, line in enumerate(out):
        # Track this-pointer save (first var_rN = ctx.r3.u32)
        m = RE_THIS_SAVE.match(line)
        if m and not this_var_found:
            this_var = m.group(1)
            var_types[this_var] = ctx_r3_type
            this_var_found = True
            continue

        # var_rN = ctx.r3.u32 anywhere (save current ctx.r3 type)
        if m and this_var_found and ctx_r3_type:
            var_types[m.group(1)] = ctx_r3_type
            continue

        # Track ctx.r3 restore from typed var
        m_r = RE_R3_RESTORE.match(line)
        if m_r:
            restored_var = m_r.group(1)
            ctx_r3_type = var_types.get(restored_var)  # may be None
            continue

        # Track ctx.r3 overwrite (general)
        m_assign = RE_R3_ASSIGN.match(line)
        if m_assign:
            rhs = m_assign.group(2).strip()
            # Check if it's restoring from a typed var
            m_var = re.match(r'(?:\(uint64_t\))??(var_r\d+)$', rhs)
            if m_var:
                ctx_r3_type = var_types.get(m_var.group(1))
            elif rhs.startswith('ctx.r3'):
                pass  # no-op, type unchanged
            else:
                ctx_r3_type = None  # unknown — sub-object load etc.
            continue

        # VCALL(ctx.r3.u32, slot, ctx, base) — try to resolve
        m_v = RE_VCALL.match(line)
        if m_v:
            indent = m_v.group(1)
            slot = int(m_v.group(2))
            suffix = m_v.group(3)  # any trailing comment

            if ctx_r3_type:
                # Look up named symbol
                sym = slot_to_sym.get((ctx_r3_type, slot))
                if sym:
                    # Replace with direct named call
                    out[i] = f"{indent}{sym}(ctx, base);{suffix}  // {ctx_r3_type}::vfn_{slot}\n"
                    stats["vcall_resolved_named"] += 1
                else:
                    # Can't name it but we know the class — annotate
                    out[i] = f"{indent}VCALL(ctx.r3.u32, {slot}, ctx, base);  // {ctx_r3_type}::vfn_{slot} (unnamed){suffix}\n"
                    stats["vcall_annotated"] += 1
            # else: leave as-is

    return out


# ─────────────────────────────────────────────────────────────────────────────
# Per-file processing
# ─────────────────────────────────────────────────────────────────────────────

def split_into_functions(lines):
    """
    Split file lines into segments: [(func_name_or_None, [lines]), ...]
    The first segment is the file header (before first function).
    """
    segments = []
    current_lines = []
    in_func = False

    for line in lines:
        m = RE_FUNC_IMPL_LINE.match(line)
        if m:
            if current_lines:
                segments.append(current_lines)
            current_lines = [line]
        else:
            current_lines.append(line)

    if current_lines:
        segments.append(current_lines)

    return segments


def process_file(path_in, path_out, vtable_classes, slot_to_sym, stats, dry_run=False):
    with open(path_in, 'r', errors='replace') as f:
        content = f.read()

    lines = content.splitlines(keepends=True)

    # Pass 1: Pattern B (file-wide, not function-scoped)
    lines = pass1_pattern_b(lines, slot_to_sym, vtable_classes, stats)

    # Pass 2: function-scoped VCALL type resolution
    # Split into function blocks, process each
    segments = split_into_functions(lines)
    new_lines = []
    for seg in segments:
        new_seg = pass2_vcall_type_resolve(seg, vtable_classes, slot_to_sym, stats)
        new_lines.extend(new_seg)

    new_content = "".join(new_lines)

    if not dry_run:
        path_out.parent.mkdir(parents=True, exist_ok=True)
        with open(path_out, 'w') as f:
            f.write(new_content)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--file', type=int, default=None, help='Process only file N')
    ap.add_argument('--window', type=int, default=15, help='Pattern B lookback window')
    args = ap.parse_args()

    global WINDOW
    WINDOW = args.window

    print("=" * 60)
    print("Phase 4A: VCALL + Virtual Dispatch Resolution")
    print(f"  Input:  {INPUT_DIR}")
    print(f"  Output: {OUTPUT_DIR}")
    print(f"  Window: {WINDOW} lines")
    print("=" * 60)

    raw_vtmap, slot_to_sym, addr_to_slot = load_tables()
    vtable_classes = set(raw_vtmap.keys())

    if not args.dry_run:
        if OUTPUT_DIR.exists():
            shutil.rmtree(OUTPUT_DIR)
        shutil.copytree(INPUT_DIR, OUTPUT_DIR)

    # Collect cpp files
    cpp_files = sorted(INPUT_DIR.glob("tt-decomp_recomp.*.cpp"))

    if args.file is not None:
        cpp_files = [f for f in cpp_files if f.stem == f"tt-decomp_recomp.{args.file}"]

    stats = defaultdict(int)

    for path_in in cpp_files:
        n = path_in.stem.split(".")[-1]
        path_out = OUTPUT_DIR / path_in.name
        print(f"  [{n:>2}] {path_in.name} ...", end=' ', flush=True)

        file_stats = defaultdict(int)
        process_file(path_in, path_out, vtable_classes, slot_to_sym, file_stats,
                     dry_run=args.dry_run)

        pb = file_stats["pattern_b_resolved"]
        vr = file_stats["vcall_resolved_named"]
        va = file_stats["vcall_annotated"]
        print(f"pattern-B:{pb}  VCALL-named:{vr}  VCALL-annotated:{va}")

        for k, v in file_stats.items():
            stats[k] += v

    print()
    print("=" * 60)
    print("Phase 4A Summary:")
    print(f"  Pattern-B PPC_CALL_INDIRECT_FUNC resolved: {stats['pattern_b_resolved']}")
    print(f"  VCALL -> named symbol resolved:            {stats['vcall_resolved_named']}")
    print(f"  VCALL annotated with class::slot:          {stats['vcall_annotated']}")
    total = stats['pattern_b_resolved'] + stats['vcall_resolved_named']
    print(f"  Total hard resolutions:                    {total}")
    if not args.dry_run:
        print(f"  Output: {OUTPUT_DIR}")
    print("=" * 60)


if __name__ == "__main__":
    main()
