#!/usr/bin/env python3
"""
Phase 4C: lis+addi Pattern B Fix
==================================
Transforms recomp/structured_pass5_ph4/ -> recomp/structured_pass5_ph4b/

Stage 3A's lis+addi resolver only caught the 'addi via var cast' form:
  var_rN = (uint32_t)(ctx.rM.s64 + OFFSET);   <- caught by stage3_globals

But missed the direct ctx.rN usage form:
  ctx.rN.s64 = -2107654144;                   // lis  (stored in ctx.rN directly)
  PPC_LOAD_U32(ctx.rN.u32 + OFFSET)            // DIRECT use — previously missed!

This script adds annotations to the missed form.

Also catches:
  ctx.r0.s64 = -2107654144;
  ctx.r3.u64 = (uint64_t)(ctx.r0.s64 + OFFSET);  // another form of addi
  -> annotate the second line with the resolved global name

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/phase4c_globals_pattern_b.py [--dry-run]
"""

import re, json, shutil, argparse
from pathlib import Path
from collections import defaultdict

BASE        = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG         = BASE / "config/434C4803"
INPUT_DIR   = BASE / "recomp/structured_pass5_ph4"
OUTPUT_DIR  = BASE / "recomp/structured_pass5_ph4b"

WINDOW = 10  # lines to look back for lis before the direct access

# ─────────────────────────────────────────────────────────────────────────────
# Load address → name table
# ─────────────────────────────────────────────────────────────────────────────

def load_addr_map():
    with open(CFG / "master_symbol_map.json") as f:
        msm = json.load(f)
    return {int(k, 16): v["name"] for k, v in msm["by_address"].items()}


# ─────────────────────────────────────────────────────────────────────────────
# Patterns
# ─────────────────────────────────────────────────────────────────────────────

# lis: ctx.rN.s64 = LARGE_NEGATIVE  (only Xbox 360 address range ~0x8200_0000)
RE_LIS = re.compile(
    r'^(\s*)(ctx\.(r\d+)|var_(r\d+))\.s64\s*=\s*(-\d+)\s*;(\s*//[^\n]*)?\s*$'
)

# Direct PPC_LOAD/STORE with ctx.rN.u32  (Pattern B)
RE_DIRECT_LOAD = re.compile(
    r'PPC_(LOAD|STORE)_(U8|S8|U16|S16|U32|S32|F32|F64|U64|S64)\('
    r'ctx\.(r\d+)\.u32\s*\+\s*(-?\d+)'
)

# addi form: ctx.rM.u64 = (uint64_t)(ctx.rN.s64 + OFFSET)  or cast variants
RE_ADDI_U64 = re.compile(
    r'^(\s*)(ctx\.(r\d+)|var_(r\d+))\.(u64|u32)\s*=\s*'
    r'(?:\(uint64_t\))?\(?(ctx\.(r\d+)|var_(r\d+))\.s64\s*\+\s*(-?\d+)\)?'
    r'\s*;(\s*//[^\n]*)?'
)


def is_valid_address(val):
    u = val & 0xFFFFFFFF
    return 0x82000000 <= u <= 0x82FFFFFF


def resolve_addr(lis_val, offset):
    addr = (lis_val & 0xFFFFFFFF + offset) & 0xFFFFFFFF
    # Handle sign-extended offset
    if offset < 0:
        addr = (lis_val + offset) & 0xFFFFFFFF
    else:
        addr = (lis_val + offset) & 0xFFFFFFFF
    return addr


# ─────────────────────────────────────────────────────────────────────────────
# Per-file processing
# ─────────────────────────────────────────────────────────────────────────────

def process_file(path_in, path_out, addr_map, stats, dry_run=False):
    with open(path_in, errors='replace') as f:
        lines = f.readlines()

    out = list(lines)
    # pending: {reg_name: (lis_value, line_idx)}
    pending = {}

    for i, line in enumerate(out):
        # Track lis assignments
        m_lis = RE_LIS.match(line)
        if m_lis:
            reg = m_lis.group(3) or m_lis.group(4)  # rN
            val = int(m_lis.group(5))
            if is_valid_address(val):
                pending[reg] = (val, i)
            else:
                pending.pop(reg, None)
            continue

        # Any assignment to a register clears its pending lis
        # (simple heuristic: if ctx.rN or var_rN appears on the LHS of =, invalidate)
        for reg in list(pending.keys()):
            lhs_pat = f"ctx.{reg}."
            var_pat = f"var_{reg}"
            if (lhs_pat in line and "=" in line and line.index(lhs_pat) < line.index("=")) or \
               (var_pat in line and "= " in line and line.strip().startswith(var_pat)):
                # Check if it's the LIS line itself being re-assigned something else
                if not RE_LIS.match(line):
                    pending.pop(reg, None)

        # Pattern B: PPC_LOAD/STORE(ctx.rN.u32 + offset)
        # Look for direct uses of pending lis registers
        modified = line
        for m in RE_DIRECT_LOAD.finditer(line):
            reg = m.group(3)    # rN from ctx.rN.u32
            offset = int(m.group(4))
            if reg in pending:
                lis_val, _ = pending[reg]
                addr = resolve_addr(lis_val, offset)
                if addr in addr_map:
                    name = addr_map[addr]
                    # Only annotate if not already annotated
                    if f"/* {name}" not in modified and f"// {name}" not in modified:
                        modified = modified.rstrip().rstrip("*/").rstrip()
                        modified += f"  /* glob:{name} @ {hex(addr)} */\n"
                        stats["pattern_b_annotated"] += 1
                elif is_valid_address(addr):
                    if f"addr:{hex(addr)}" not in modified:
                        modified = modified.rstrip().rstrip("*/").rstrip()
                        modified += f"  /* glob:0x{addr:08x} */\n"
                        stats["pattern_b_unresolved"] += 1

        if modified != line:
            out[i] = modified
            continue

        # addi form: ctx.rM.u64 = (uint64_t)(ctx.rN.s64 + OFFSET)
        m_addi = RE_ADDI_U64.match(line)
        if m_addi:
            src_reg = m_addi.group(6) or m_addi.group(7)  # rN from ctx.rN or var_rN
            if src_reg and src_reg in pending:
                offset = int(m_addi.group(8))
                lis_val, _ = pending[src_reg]
                addr = resolve_addr(lis_val, offset)
                if addr in addr_map:
                    name = addr_map[addr]
                    if f"// {name}" not in line:
                        out[i] = line.rstrip() + f"  // {name} @ {hex(addr)}\n"
                        stats["addi_u64_resolved"] += 1
                elif is_valid_address(addr):
                    out[i] = line.rstrip() + f"  // addr:{hex(addr)}\n"
                    stats["addi_u64_unresolved"] += 1

    if not dry_run:
        with open(path_out, 'w') as f:
            f.writelines(out)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    print("=" * 60)
    print("Phase 4C: lis+addi Pattern B Globals Fix")
    print(f"  Input:  {INPUT_DIR}")
    print(f"  Output: {OUTPUT_DIR}")
    print("=" * 60)

    addr_map = load_addr_map()
    print(f"  Address map: {len(addr_map):,} entries")

    if not args.dry_run:
        if OUTPUT_DIR.exists():
            shutil.rmtree(OUTPUT_DIR)
        shutil.copytree(INPUT_DIR, OUTPUT_DIR)

    cpp_files = sorted(INPUT_DIR.glob("tt-decomp_recomp.*.cpp"))
    stats = defaultdict(int)

    for path_in in cpp_files:
        n = path_in.stem.split(".")[-1]
        path_out = OUTPUT_DIR / path_in.name
        fstats = defaultdict(int)
        process_file(path_in, path_out, addr_map, fstats, dry_run=args.dry_run)
        pb = fstats["pattern_b_annotated"]
        pu = fstats["pattern_b_unresolved"]
        au = fstats["addi_u64_resolved"]
        if pb + pu + au > 0:
            print(f"  [{n:>2}] pattern-B-named:{pb}  pattern-B-addr:{pu}  addi-u64:{au}")
        for k, v in fstats.items():
            stats[k] += v

    print()
    print("=" * 60)
    print("Phase 4C Summary:")
    print(f"  Pattern-B global annotations (named):  {stats['pattern_b_annotated']}")
    print(f"  Pattern-B global annotations (addr):   {stats['pattern_b_unresolved']}")
    print(f"  addi-u64 form resolved:                {stats['addi_u64_resolved']}")
    total = stats['pattern_b_annotated'] + stats['addi_u64_resolved']
    print(f"  Total new named annotations:           {total}")
    if not args.dry_run:
        print(f"  Output: {OUTPUT_DIR}")
    print("=" * 60)


if __name__ == "__main__":
    main()
