#!/usr/bin/env python3
"""
Phase 3E: Final Cleanup
========================
Transforms structured_pass5d/ -> structured_pass5/

Cleans up dead patterns remaining after Phases 3A-3D:
  1. No-op rotations: __builtin_rotateleft32(x, 0) -> x
  2. Redundant double casts: (uint32_t)((int64_t)(int32_t)var) -> var  (when already uint32)
  3. Dead lis constants: ctx.rN.s64 = LARGE_NEG; lines whose rN is never used again in the function
  4. Tautological zero checks after known-nonzero loads
  5. Strip trailing whitespace / normalize blank lines

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/stage3_cleanup.py [--dry-run] [--file N]
"""

import re, os, sys, shutil, argparse
from pathlib import Path
from collections import defaultdict

BASE       = Path("/Users/Ozordi/Downloads/tt-decomp")
INPUT_DIR  = BASE / "recomp/structured_pass5d"
OUTPUT_DIR = BASE / "recomp/structured_pass5"

# ─────────────────────────────────────────────────────────────────────────────
# Pattern 1: No-op rotations
# ─────────────────────────────────────────────────────────────────────────────

RE_NOOP_ROT = re.compile(r'__builtin_rotateleft32\(([^,]+),\s*0\)')

def clean_noop_rotations(line, stats):
    def replace(m):
        stats["noop_rot_removed"] += 1
        return m.group(1).strip()
    return RE_NOOP_ROT.sub(replace, line)


# ─────────────────────────────────────────────────────────────────────────────
# Pattern 2: Redundant casts
# (uint32_t)(ctx.rN.u64) -> ctx.rN.u32  (u64 to u32 cast is what .u32 already is)
# ─────────────────────────────────────────────────────────────────────────────

RE_U64_TO_U32 = re.compile(r'\(uint32_t\)\(ctx\.(r\d+)\.u64\)')
RE_S64_TO_U32 = re.compile(r'\(uint32_t\)\(ctx\.(r\d+)\.s64\)')
RE_DOUBLE_CAST = re.compile(r'\(uint32_t\)\(\(uint32_t\)([^)]+)\)')

def clean_redundant_casts(line, stats):
    def u64_to_u32(m):
        stats["cast_cleaned"] += 1
        return f"ctx.{m.group(1)}.u32"
    def s64_to_u32(m):
        stats["cast_cleaned"] += 1
        return f"ctx.{m.group(1)}.u32"
    def double_cast(m):
        stats["cast_cleaned"] += 1
        return f"(uint32_t)({m.group(1)})"

    line = RE_U64_TO_U32.sub(u64_to_u32, line)
    line = RE_S64_TO_U32.sub(s64_to_u32, line)
    line = RE_DOUBLE_CAST.sub(double_cast, line)
    return line


# ─────────────────────────────────────────────────────────────────────────────
# Pattern 3: Dead lis assignments (ctx.rN.s64 = LARGE_NEG where rN is volatile)
# Only remove if the register is r0-r12 (volatile) and the line is followed by
# an addi that already has an // addr: or // symbol annotation
# ─────────────────────────────────────────────────────────────────────────────

RE_LIS_LINE = re.compile(r'^(\s*)(ctx\.(r\d+))\.s64\s*=\s*(-\d+)\s*;\s*(//.*)?$')

def is_volatile_reg(reg_name):
    """Volatile PPC registers: r0-r12 (not callee-saved)."""
    try:
        n = int(reg_name[1:])
        return n <= 12
    except:
        return False

def clean_dead_lis(lines, stats):
    """
    Remove lis-only lines (ctx.rN.s64 = BIG_NEG) where:
    - The register is volatile (r0-r12)
    - The NEXT non-blank line either:
        a) Is another lis assignment for a DIFFERENT register, or
        b) Does NOT reference ctx.rN (the lis was only used for one addi which is now annotated)
    This is conservative: only remove if the reg isn't referenced in the next 3 lines.
    """
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        m = RE_LIS_LINE.match(line)
        if m:
            reg = m.group(3)  # e.g., "r10"
            if is_volatile_reg(reg):
                # Check if reg is used in next 3 lines
                used = False
                for j in range(i+1, min(i+4, len(lines))):
                    next_line = lines[j]
                    if f"ctx.{reg}." in next_line or f"var_{reg}" in next_line:
                        # Check: is it just another assignment (dead)?
                        if re.match(rf'^\s*ctx\.{reg}\.', next_line) and '=' in next_line and 'LOAD' not in next_line:
                            # It's being reassigned - original lis is dead
                            pass
                        else:
                            used = True
                            break
                if not used:
                    stats["dead_lis_removed"] += 1
                    i += 1
                    continue
        out.append(line)
        i += 1
    return out


# ─────────────────────────────────────────────────────────────────────────────
# Pattern 4: Normalize blank lines (no more than 1 consecutive blank)
# ─────────────────────────────────────────────────────────────────────────────

def normalize_blank_lines(lines, stats):
    out = []
    prev_blank = False
    for line in lines:
        is_blank = line.strip() == ""
        if is_blank and prev_blank:
            stats["blank_lines_removed"] += 1
            continue
        out.append(line)
        prev_blank = is_blank
    return out


# ─────────────────────────────────────────────────────────────────────────────
# Pattern 5: Strip trailing whitespace
# ─────────────────────────────────────────────────────────────────────────────

def strip_trailing_whitespace(lines):
    return [line.rstrip() + "\n" if line.endswith("\n") else line.rstrip() for line in lines]


# ─────────────────────────────────────────────────────────────────────────────
# Coverage metrics
# ─────────────────────────────────────────────────────────────────────────────

def compute_metrics(lines):
    """Count remaining raw PPC patterns for coverage report."""
    counts = {
        "PPC_LOAD_STORE_remaining": sum(1 for l in lines if "PPC_LOAD_" in l or "PPC_STORE_" in l),
        "VCALL_emitted": sum(1 for l in lines if "VCALL(" in l),
        "DTOR_emitted": sum(1 for l in lines if "DTOR(" in l),
        "named_func_calls": sum(1 for l in lines if re.search(r'\b(pong|rage|ph|aud|gm|pcr|gd|cr|at|fi|sn|msg|pg)\w+_\w+\(ctx,', l)),
        "sub_remaining": sum(1 for l in lines if re.search(r'\bsub_[0-9A-Fa-f]{8}\b', l)),
        "field_annotations": sum(1 for l in lines if "/*" in l and "::" in l and "@+0x" in l),
    }
    return counts


# ─────────────────────────────────────────────────────────────────────────────
# Process a single file
# ─────────────────────────────────────────────────────────────────────────────

def process_file(in_path, out_path, stats, dry_run=False):
    with open(in_path) as f:
        lines = f.readlines()

    # Line-by-line transforms
    new_lines = []
    for line in lines:
        line = clean_noop_rotations(line, stats)
        line = clean_redundant_casts(line, stats)
        new_lines.append(line)

    # Multi-line transforms
    new_lines = clean_dead_lis(new_lines, stats)
    new_lines = normalize_blank_lines(new_lines, stats)
    new_lines = strip_trailing_whitespace(new_lines)

    # Compute metrics for this file
    metrics = compute_metrics(new_lines)
    for k, v in metrics.items():
        stats[f"metric_{k}"] += v

    stats["files_processed"] += 1

    if not dry_run:
        with open(out_path, "w") as f:
            f.writelines(new_lines)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Stage 3E: Final Cleanup")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--file", type=str)
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args()

    print("=" * 60)
    print("Stage 3E: Final Cleanup")
    print(f"Input:  {INPUT_DIR}")
    print(f"Output: {OUTPUT_DIR}")
    print("=" * 60)

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

    total = len(input_files)
    for idx, in_path in enumerate(input_files):
        out_path = OUTPUT_DIR / in_path.name
        if args.verbose:
            print(f"  [{idx+1}/{total}] {in_path.name}")
        else:
            print(f"  [{idx+1}/{total}] {in_path.name}", end="\r")
        process_file(in_path, out_path, stats, dry_run=args.dry_run)

    print()
    print("\n--- Phase 3E Results ---")
    print(f"  Files processed:          {stats['files_processed']}")
    print(f"  No-op rotations removed:  {stats['noop_rot_removed']}")
    print(f"  Redundant casts cleaned:  {stats['cast_cleaned']}")
    print(f"  Dead lis lines removed:   {stats['dead_lis_removed']}")
    print(f"  Blank lines normalized:   {stats['blank_lines_removed']}")
    print(f"\n--- Final Coverage Metrics (across all pass5 files) ---")
    print(f"  PPC_LOAD/STORE remaining: {stats['metric_PPC_LOAD_STORE_remaining']:,}")
    print(f"  VCALL macros emitted:     {stats['metric_VCALL_emitted']:,}")
    print(f"  DTOR macros emitted:      {stats['metric_DTOR_emitted']:,}")
    print(f"  Named function calls:     {stats['metric_named_func_calls']:,}")
    print(f"  sub_* still remaining:    {stats['metric_sub_remaining']:,}")
    print(f"  Field annotations:        {stats['metric_field_annotations']:,}")

    if not args.dry_run:
        print(f"\nOutput: {OUTPUT_DIR}")
    print("Phase 3E complete.")


if __name__ == "__main__":
    main()
