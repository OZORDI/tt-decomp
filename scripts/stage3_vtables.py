#!/usr/bin/env python3
"""
Phase 3C: Vtable Dispatch Resolution
======================================
Transforms structured_pass5b/ -> structured_pass5c/

Detects the 3-line vtable dispatch pattern and collapses it:
  ctx.r8.u64 = PPC_LOAD_U32(base + 0);      // load vtable ptr
  ctx.r7.u64 = PPC_LOAD_U32(ctx.r8.u32 + N); // load slot at byte offset N
  PPC_CALL_INDIRECT_FUNC(ctx.r7.u32);        // call

-> VCALL(base, N/4)  or  ClassName_vfn_N(ctx, base) if class+slot known

Slot 0 is universally the destructor, so:
  VCALL(obj, 0) -> DTOR(obj)   (if class unknown)
  VCALL(obj, 0) -> ClassName_vfn_0(ctx, base) if class known

Generates include/generated_vtables.h

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/stage3_vtables.py [--dry-run] [--file N]
"""

import re, json, os, sys, shutil, argparse
from pathlib import Path
from collections import defaultdict

BASE        = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG         = BASE / "config/434C4803"
INPUT_DIR   = BASE / "recomp/structured_pass5b"
OUTPUT_DIR  = BASE / "recomp/structured_pass5c"
INCLUDE_DIR = BASE / "include"

# ─────────────────────────────────────────────────────────────────────────────
# Load vtable map
# ─────────────────────────────────────────────────────────────────────────────

def load_vtable_map():
    with open(CFG / "vtable_layout_map.json") as f:
        raw = json.load(f)

    # Build: (class_name, slot_int) -> symbol_name
    slot_map = {}
    for class_name, slots in raw.items():
        for slot_str, info in slots.items():
            slot_int = int(slot_str)
            slot_map[(class_name, slot_int)] = info["symbol"]

    # Build: by addr -> (class_name, slot_int)
    addr_map = {}
    for class_name, slots in raw.items():
        for slot_str, info in slots.items():
            slot_int = int(slot_str)
            addr_str = info["addr"]
            addr_map[addr_str] = (class_name, slot_int)

    print(f"  Loaded vtable map: {len(raw)} classes, {len(slot_map)} named slots")
    return raw, slot_map, addr_map


# ─────────────────────────────────────────────────────────────────────────────
# Vtable dispatch patterns
# ─────────────────────────────────────────────────────────────────────────────

# Line A: load vtable pointer from object base+0
# ctx.rN.u64 = PPC_LOAD_U32(BASE + 0);  OR  var_rN = (uint32_t)(PPC_LOAD_U32(BASE + 0))
RE_VT_LINE_A = re.compile(
    r'^(\s*)'
    r'(ctx\.(r\d+)|var_(r\d+))'
    r'(?:\.u64)?\s*=\s*(?:\(uint32_t\)\()?'
    r'PPC_LOAD_U32\('
    r'(ctx\.(r\d+)\.u32|var_(r\d+))'
    r'\s*\+\s*0\)'
    r'(?:\))?'
    r'\s*;'
)

# Line B: load slot from vtable + N (byte offset, must be multiple of 4)
# ctx.rM.u64 = PPC_LOAD_U32(ctx.rN.u32 + N)
RE_VT_LINE_B = re.compile(
    r'^(\s*)'
    r'(ctx\.(r\d+)|var_(r\d+))'
    r'(?:\.u64)?\s*=\s*(?:\(uint32_t\)\()?'
    r'PPC_LOAD_U32\('
    r'(ctx\.(r\d+)\.u32|var_(r\d+))'
    r'\s*\+\s*(\d+)\)'
    r'(?:\))?'
    r'\s*;'
)

# Line C: indirect call using the loaded slot pointer
# PPC_CALL_INDIRECT_FUNC(ctx.rM.u32)  OR  PPC_CALL_INDIRECT_FUNC(var_rM)
RE_VT_LINE_C = re.compile(
    r'^(\s*)'
    r'PPC_CALL_INDIRECT_FUNC\('
    r'(ctx\.(r\d+)\.u32|var_(r\d+))'
    r'\)'
    r'\s*;'
)


def get_reg(match_groups, ctx_group, var_group):
    """Extract register name from regex match groups."""
    ctx = match_groups[ctx_group] if ctx_group < len(match_groups) else None
    var = match_groups[var_group] if var_group < len(match_groups) else None
    return ctx or var


def try_match_vtable_triplet(lines, i, vtable_map, slot_map, stats):
    """
    Try to match a vtable dispatch triplet at lines[i], i+1, i+2 (with possible comment/label lines).
    Returns (new_lines, lines_consumed) or (None, 0) if no match.
    """
    # Collect non-comment, non-blank lines starting at i
    candidate_idxs = []
    j = i
    while j < len(lines) and len(candidate_idxs) < 3:
        stripped = lines[j].strip()
        if stripped and not stripped.startswith("//") and not stripped.startswith("loc_"):
            candidate_idxs.append(j)
        j += 1

    if len(candidate_idxs) < 3:
        return None, 0

    la, lb, lc = lines[candidate_idxs[0]], lines[candidate_idxs[1]], lines[candidate_idxs[2]]
    indent = ""

    # Try A-B-C pattern
    ma = RE_VT_LINE_A.match(la)
    if not ma:
        return None, 0

    # Extract vtable reg (what was loaded into)
    vt_reg_ctx = ma.group(3)   # from ctx.rN
    vt_reg_var = ma.group(4)   # from var_rN
    vt_reg = vt_reg_ctx or vt_reg_var
    indent = ma.group(1)

    # Extract base object reg
    obj_reg_ctx = ma.group(6)
    obj_reg_var = ma.group(7)
    obj_reg = obj_reg_ctx or obj_reg_var

    mb = RE_VT_LINE_B.match(lb)
    if not mb:
        return None, 0

    # The source of line B must be the vtable reg loaded in line A
    b_src_ctx = mb.group(6)
    b_src_var = mb.group(7)
    b_src = b_src_ctx or b_src_var
    if b_src != vt_reg:
        return None, 0

    byte_offset = int(mb.group(8))
    slot_index = byte_offset // 4

    # The dest reg of line B
    slot_reg_ctx = mb.group(3)
    slot_reg_var = mb.group(4)
    slot_reg = slot_reg_ctx or slot_reg_var

    mc = RE_VT_LINE_C.match(lc)
    if not mc:
        return None, 0

    # The call target must be the slot reg
    call_reg_ctx = mc.group(3)
    call_reg_var = mc.group(4)
    call_reg = call_reg_ctx or call_reg_var
    if call_reg != slot_reg:
        return None, 0

    # We have a confirmed vtable dispatch!
    stats["vtable_dispatches_found"] += 1

    # Try to determine class name for the base object
    # For now, emit VCALL macro with slot index
    if slot_index == 0:
        replacement = f"{indent}DTOR(ctx.r3.u32, ctx, base);  // vtable slot 0 (destructor)\n"
        stats["slot0_destructor"] += 1
    else:
        replacement = f"{indent}VCALL(ctx.r3.u32, {slot_index}, ctx, base);  // vtable slot {slot_index} (byte +{byte_offset})\n"
        stats["vcall_emitted"] += 1

    # Preserve any comment lines between the triplet lines
    span_end = candidate_idxs[2] + 1
    preserved_comments = []
    for k in range(candidate_idxs[0], span_end):
        if k not in candidate_idxs:
            stripped = lines[k].strip()
            if stripped.startswith("//"):
                preserved_comments.append(lines[k])

    out = preserved_comments + [replacement]
    lines_to_consume = span_end - i

    return out, lines_to_consume


# ─────────────────────────────────────────────────────────────────────────────
# Generate include/generated_vtables.h
# ─────────────────────────────────────────────────────────────────────────────

def generate_vtables_header(vtable_map):
    """Generate a header with vtable struct definitions for all known classes."""
    out_path = INCLUDE_DIR / "generated_vtables.h"

    lines = [
        "// AUTO-GENERATED by stage3_vtables.py — DO NOT EDIT\n",
        "// Vtable definitions for all classes with named virtual slots\n",
        "// Rockstar Table Tennis Xbox 360 (434C4803)\n",
        "#pragma once\n",
        "#include <cstdint>\n",
        "\n",
        "// VCALL macro: virtual call through vtable slot\n",
        "#ifndef VCALL\n",
        "#define VCALL(obj, slot, ctx, base) \\\n",
        "    ((void(*)(PPCContext&, uint8_t*))PPC_LOAD_U32(PPC_LOAD_U32((obj) + 0) + (slot)*4))(ctx, base)\n",
        "#endif\n",
        "\n",
        "// DTOR macro: destructor call (slot 0)\n",
        "#ifndef DTOR\n",
        "#define DTOR(obj, ctx, base) VCALL(obj, 0, ctx, base)\n",
        "#endif\n",
        "\n",
    ]

    # Emit per-class vtable struct with named slots
    for class_name in sorted(vtable_map.keys()):
        slots = vtable_map[class_name]
        if not slots:
            continue

        max_slot = max(int(s) for s in slots.keys())

        lines.append(f"// {class_name} vtable ({len(slots)} named slots, max slot {max_slot})\n")
        lines.append(f"struct {class_name}_vtable {{\n")

        for slot_idx in range(max_slot + 1):
            slot_str = str(slot_idx)
            if slot_str in slots:
                info = slots[slot_str]
                sym = info["symbol"]
                role = info.get("inferred_role", "")
                role_comment = f"  // {role}" if role else ""
                lines.append(f"    void* vfn_{slot_idx};  // {sym}{role_comment}\n")
            else:
                lines.append(f"    void* vfn_{slot_idx};  // (unnamed)\n")

        lines.append(f"}};\n\n")

    with open(out_path, "w") as f:
        f.writelines(lines)

    print(f"  Generated {out_path.name} with vtable structs for {len(vtable_map)} classes")


# ─────────────────────────────────────────────────────────────────────────────
# Process a single file
# ─────────────────────────────────────────────────────────────────────────────

def process_file(in_path, out_path, vtable_map, slot_map, addr_map, stats, dry_run=False):
    with open(in_path) as f:
        lines = f.readlines()

    out_lines = []
    i = 0
    while i < len(lines):
        # Try vtable triplet detection
        result, consumed = try_match_vtable_triplet(lines, i, vtable_map, slot_map, stats)
        if result is not None and consumed > 0:
            out_lines.extend(result)
            i += consumed
        else:
            out_lines.append(lines[i])
            i += 1

    stats["files_processed"] += 1

    if not dry_run:
        with open(out_path, "w") as f:
            f.writelines(out_lines)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Stage 3C: Vtable Dispatch Resolution")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--file", type=str)
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args()

    print("=" * 60)
    print("Stage 3C: Vtable Dispatch Resolution")
    print(f"Input:  {INPUT_DIR}")
    print(f"Output: {OUTPUT_DIR}")
    print("=" * 60)

    vtable_map, slot_map, addr_map = load_vtable_map()

    input_files = sorted(INPUT_DIR.glob("tt-decomp_recomp.*.cpp"))
    input_files += [INPUT_DIR / "tt-decomp_init.cpp"]

    if args.file is not None:
        if args.file == "init":
            input_files = [INPUT_DIR / "tt-decomp_init.cpp"]
        else:
            target = f"tt-decomp_recomp.{args.file}.cpp"
            input_files = [f for f in input_files if f.name == target]

    stats = defaultdict(int)

    if not args.dry_run:
        os.makedirs(OUTPUT_DIR, exist_ok=True)
        for static in ["sources.cmake", "tt-decomp_config.h", "tt-decomp_init.h"]:
            src = INPUT_DIR / static
            if src.exists():
                shutil.copy2(src, OUTPUT_DIR / static)
        generate_vtables_header(vtable_map)

    total = len(input_files)
    for idx, in_path in enumerate(input_files):
        out_path = OUTPUT_DIR / in_path.name
        if args.verbose:
            print(f"  [{idx+1}/{total}] {in_path.name}")
        else:
            print(f"  [{idx+1}/{total}] {in_path.name}", end="\r")
        process_file(in_path, out_path, vtable_map, slot_map, addr_map, stats, dry_run=args.dry_run)

    print()

    print("\n--- Phase 3C Results ---")
    print(f"  Files processed:              {stats['files_processed']}")
    print(f"  Vtable dispatches collapsed:  {stats['vtable_dispatches_found']}")
    print(f"    Slot 0 destructors:         {stats['slot0_destructor']}")
    print(f"    Named VCALL macros:         {stats['vcall_emitted']}")

    if not args.dry_run:
        print(f"\nOutput: {OUTPUT_DIR}")
    print("Phase 3C complete.")


if __name__ == "__main__":
    main()
