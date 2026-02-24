#!/usr/bin/env python3
"""
Phase 3A: Global Address Resolution
=====================================
Transforms structured_pass4/ -> structured_pass5a/

Resolves two patterns:
  1. SDA access:  PPC_LOAD/STORE(ctx.r13.u32 + N)  ->  SYMBOL_NAME
  2. lis+addi:    ctx.rN.s64 = -2106XXXXXX; ... + OFFSET  ->  address constant

Also annotates all sub_XXXXXXXX calls with their source file from splits.txt.

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/stage3_globals.py [--dry-run] [--file N]
"""

import re, json, os, sys, shutil, argparse
from pathlib import Path
from collections import defaultdict

BASE       = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG        = BASE / "config/434C4803"
INPUT_DIR  = BASE / "recomp/structured_pass4"
OUTPUT_DIR = BASE / "recomp/structured_pass5a"

SDA_BASE   = 0x82600000  # r13 value, validated

# ─────────────────────────────────────────────────────────────────────────────
# Load lookup tables
# ─────────────────────────────────────────────────────────────────────────────

def load_tables():
    with open(CFG / "master_symbol_map.json") as f:
        msm = json.load(f)
    # Build addr(int) -> name lookup for fast access
    addr_to_name = {}
    addr_to_type = {}
    for addr_hex, entry in msm["by_address"].items():
        addr_to_name[int(addr_hex, 16)] = entry["name"]
        addr_to_type[int(addr_hex, 16)] = entry["type"]

    with open(CFG / "sda_resolution_map.json") as f:
        sda_map = json.load(f)  # str(signed_offset) -> {addr, name}

    with open(CFG / "splits_addr_map.json") as f:
        splits_raw = json.load(f)
    splits = [(int(r["start"], 16), int(r["end"], 16), r["source"]) for r in splits_raw]
    splits.sort()

    return addr_to_name, addr_to_type, sda_map, splits


def lookup_splits(addr, splits):
    """Return source file for address, or None."""
    for start, end, source in splits:
        if start <= addr < end:
            return source
        if addr < start:
            break
    return None


# ─────────────────────────────────────────────────────────────────────────────
# Regex patterns
# ─────────────────────────────────────────────────────────────────────────────

# SDA pattern: ctx.r13.u32 + N  (N is signed decimal)
RE_SDA_LOAD  = re.compile(r'PPC_LOAD_U32\(ctx\.r13\.u32 \+ (-?\d+)\)')
RE_SDA_STORE = re.compile(r'PPC_STORE_U32\(ctx\.r13\.u32 \+ (-?\d+),')
RE_SDA_LOAD8 = re.compile(r'PPC_LOAD_U8\(ctx\.r13\.u32 \+ (-?\d+)\)')
RE_SDA_LOAD16= re.compile(r'PPC_LOAD_U16\(ctx\.r13\.u32 \+ (-?\d+)\)')
RE_SDA_LOADF = re.compile(r'PPC_LOAD_F32\(ctx\.r13\.u32 \+ (-?\d+)\)')
RE_SDA_STOREF= re.compile(r'PPC_STORE_F32\(ctx\.r13\.u32 \+ (-?\d+),')
RE_SDA_STORE8= re.compile(r'PPC_STORE_U8\(ctx\.r13\.u32 \+ (-?\d+),')

# lis pattern: ctx.rN.s64 = LARGE_NEGATIVE_INT;
RE_LIS_ASSIGN = re.compile(r'^(\s*)(ctx\.(r\d+)|var_(r\d+))\.s64\s*=\s*(-\d+)\s*;(\s*//.*)?\s*$')

# addi after lis (on same or nearby var): var_rN = (uint32_t)(ctx.rM.s64 + OFFSET)
RE_ADDI_CAST  = re.compile(r'^(\s*)(var_(r\d+))\s*=\s*\(uint32_t\)\((ctx\.(r\d+)|var_(r\d+))\.s64 \+ (-?\d+)\)\s*;')

# Direct address literal in PPC_LOAD/STORE: 0x82XXXXXX (already resolved addresses)
RE_DIRECT_ADDR = re.compile(r'0x82([0-9A-Fa-f]{6})u')

# sub_ call pattern (for splits annotation)
RE_SUB_CALL = re.compile(r'\bsub_([0-9A-Fa-f]{8})\b')

# PPC_FUNC_IMPL line
RE_FUNC_IMPL = re.compile(r'^PPC_FUNC_IMPL\(__imp__sub_([0-9A-Fa-f]{8})\)')


# ─────────────────────────────────────────────────────────────────────────────
# Per-line SDA resolver
# ─────────────────────────────────────────────────────────────────────────────

def resolve_sda_in_line(line, sda_map, addr_to_name, stats):
    """Replace SDA accesses with named symbol references."""
    def sda_replacer(m, pattern_type):
        offset = int(m.group(1))
        key = str(offset)
        if key in sda_map:
            entry = sda_map[key]
            name = entry["name"]
            addr = entry["addr"]
            stats["sda_resolved"] += 1
            # Return the symbol name as a comment annotation
            # We don't replace the PPC_LOAD macro itself (behavioral safety)
            # but add a trailing comment with the symbol name
            return m.group(0) + f"  /* {name} @ {addr} */"
        else:
            addr = hex((SDA_BASE + offset) & 0xFFFFFFFF)
            # Check if this addr is in addr_to_name directly
            addr_int = int(addr, 16)
            if addr_int in addr_to_name:
                name = addr_to_name[addr_int]
                stats["sda_resolved"] += 1
                return m.group(0) + f"  /* {name} @ {addr} */"
            stats["sda_unresolved"] += 1
            return m.group(0) + f"  /* SDA:{addr} */"

    # Apply all SDA patterns
    for pattern in [RE_SDA_LOAD, RE_SDA_STORE, RE_SDA_LOAD8, RE_SDA_LOAD16,
                    RE_SDA_LOADF, RE_SDA_STOREF, RE_SDA_STORE8]:
        line = pattern.sub(lambda m: sda_replacer(m, "sda"), line)

    return line


# ─────────────────────────────────────────────────────────────────────────────
# Direct address resolver
# ─────────────────────────────────────────────────────────────────────────────

def resolve_direct_addrs(line, addr_to_name, stats):
    """Add symbol comments to literal 0x82XXXXXXu addresses."""
    already_commented = "/*" in line  # don't double-annotate
    if already_commented:
        return line

    def direct_replacer(m):
        addr_int = int("0x82" + m.group(1), 16)
        if addr_int in addr_to_name:
            name = addr_to_name[addr_int]
            stats["direct_resolved"] += 1
            return m.group(0) + f"/* {name} */"
        return m.group(0)

    return RE_DIRECT_ADDR.sub(direct_replacer, line)


# ─────────────────────────────────────────────────────────────────────────────
# lis+addi resolver (multi-line, stateful)
# ─────────────────────────────────────────────────────────────────────────────

class LisAddiState:
    """Track pending lis assignments to resolve subsequent addi."""
    def __init__(self):
        self.pending = {}  # reg_name -> (upper_val, line_index)
        self.window = 8    # search window for addi after lis

    def try_lis(self, line):
        """If line is a lis assignment, store it. Returns True if matched."""
        m = RE_LIS_ASSIGN.match(line)
        if m:
            reg = m.group(3) or m.group(4)  # rN from ctx.rN or var_rN
            val = int(m.group(5))
            # Only track values that look like PPC upper halves (0x8200xxxx range)
            upper_u32 = val & 0xFFFFFFFF
            if 0x82000000 <= upper_u32 <= 0x82FFFFFF:
                self.pending[reg] = val
                return True
        return False

    def try_addi(self, line, addr_to_name, stats):
        """If line is an addi using a pending lis register, annotate it."""
        m = RE_ADDI_CAST.match(line)
        if m:
            src_reg = m.group(5) or m.group(6)  # source register
            offset = int(m.group(7))
            if src_reg in self.pending:
                upper = self.pending[src_reg] & 0xFFFFFFFF
                addr = (upper + offset) & 0xFFFFFFFF
                addr_hex = hex(addr)
                if addr in addr_to_name:
                    name = addr_to_name[addr]
                    stats["lisaddi_resolved"] += 1
                    # Annotate with a comment
                    line = line.rstrip() + f"  // {name} @ {addr_hex}\n"
                else:
                    # Unresolved but valid address
                    if 0x82000000 <= addr <= 0x82FFFFFF:
                        stats["lisaddi_unresolved"] += 1
                        line = line.rstrip() + f"  // addr:{addr_hex}\n"
        return line

    def decay(self, reg=None):
        """Clear stale pending entries."""
        if reg:
            self.pending.pop(reg, None)
        else:
            self.pending.clear()


# ─────────────────────────────────────────────────────────────────────────────
# Splits annotation helper
# ─────────────────────────────────────────────────────────────────────────────

def get_func_source(func_addr_hex, splits):
    """Return source file for a function address."""
    try:
        addr = int(func_addr_hex, 16)
    except ValueError:
        return None
    return lookup_splits(addr, splits)


# ─────────────────────────────────────────────────────────────────────────────
# Process a single file
# ─────────────────────────────────────────────────────────────────────────────

def process_file(in_path, out_path, addr_to_name, addr_to_type, sda_map, splits, stats, dry_run=False):
    with open(in_path) as f:
        lines = f.readlines()

    out_lines = []
    lis_state = LisAddiState()
    current_func_addr = None
    current_func_source = None

    for i, line in enumerate(lines):
        # Track current function for source annotation
        m_func = RE_FUNC_IMPL.match(line.strip())
        if m_func:
            current_func_addr = m_func.group(1)
            current_func_source = get_func_source(current_func_addr, splits)
            if current_func_source:
                stats["funcs_with_source"] += 1

        # Phase A: lis+addi resolution (stateful)
        lis_state.try_lis(line)
        line = lis_state.try_addi(line, addr_to_name, stats)

        # Phase B: SDA resolution (ctx.r13 + offset)
        if "ctx.r13.u32 +" in line:
            line = resolve_sda_in_line(line, sda_map, addr_to_name, stats)

        # Phase C: Direct address annotation
        if "0x82" in line and "/*" not in line:
            line = resolve_direct_addrs(line, addr_to_name, stats)

        out_lines.append(line)

    stats["files_processed"] += 1

    if not dry_run:
        os.makedirs(out_path.parent, exist_ok=True)
        with open(out_path, "w") as f:
            f.writelines(out_lines)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Stage 3A: Global Address Resolution")
    parser.add_argument("--dry-run", action="store_true", help="Don't write output files")
    parser.add_argument("--file", type=str, help="Process only file N (e.g. '0' or 'init')")
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args()

    print("=" * 60)
    print("Stage 3A: Global Address Resolution")
    print(f"Input:  {INPUT_DIR}")
    print(f"Output: {OUTPUT_DIR}")
    print("=" * 60)

    # Load tables
    addr_to_name, addr_to_type, sda_map, splits = load_tables()
    print(f"Loaded: {len(addr_to_name)} symbols, {len(sda_map)} SDA entries, {len(splits)} split ranges")

    # Find files to process
    input_files = sorted(INPUT_DIR.glob("tt-decomp_recomp.*.cpp"))
    input_files += [INPUT_DIR / "tt-decomp_init.cpp"]

    if args.file is not None:
        if args.file == "init":
            input_files = [INPUT_DIR / "tt-decomp_init.cpp"]
        else:
            target = f"tt-decomp_recomp.{args.file}.cpp"
            input_files = [f for f in input_files if f.name == target]
            if not input_files:
                print(f"ERROR: No file matching {target}")
                sys.exit(1)

    stats = defaultdict(int)

    if not args.dry_run:
        os.makedirs(OUTPUT_DIR, exist_ok=True)
        # Copy static files
        for static in ["sources.cmake", "tt-decomp_config.h", "tt-decomp_init.h"]:
            src = INPUT_DIR / static
            if src.exists():
                shutil.copy2(src, OUTPUT_DIR / static)

    total = len(input_files)
    for idx, in_path in enumerate(input_files):
        out_path = OUTPUT_DIR / in_path.name
        if args.verbose:
            print(f"  [{idx+1}/{total}] {in_path.name}...")
        else:
            print(f"  [{idx+1}/{total}] {in_path.name}", end="\r")

        process_file(in_path, out_path, addr_to_name, addr_to_type,
                     sda_map, splits, stats, dry_run=args.dry_run)

    print()
    print("\n--- Phase 3A Results ---")
    print(f"  Files processed:        {stats['files_processed']}")
    print(f"  SDA accesses resolved:  {stats['sda_resolved']}")
    print(f"  SDA accesses unknown:   {stats['sda_unresolved']}")
    print(f"  lis+addi resolved:      {stats['lisaddi_resolved']}")
    print(f"  lis+addi unresolved:    {stats['lisaddi_unresolved']}")
    print(f"  Direct addrs annotated: {stats['direct_resolved']}")
    print(f"  Funcs with source attr: {stats['funcs_with_source']}")
    if not args.dry_run:
        print(f"\nOutput: {OUTPUT_DIR}")
    print("Phase 3A complete.")


if __name__ == "__main__":
    main()
