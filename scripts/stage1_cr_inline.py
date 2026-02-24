#!/usr/bin/env python3
"""
Stage 1A: CR Register Inlining
================================
Transforms verbose CR compare+branch pairs into direct C++ boolean expressions.

Before:
    ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
    if (ctx.cr6.eq) goto loc_820C0030;

After:
    if (ctx.r11.u32 == 0) goto loc_820C0030;

Safety rule: Only inline when compare and branch are "adjacent"
(no intervening code statements between them, only blank lines and // comments).
Intervening labels flush the pending compare (labels = potential multi-predecessor joins).

Usage:
    python3 scripts/stage1_cr_inline.py [--dry-run] [--file recomp.0.cpp]
"""

import re
import os
import sys
import glob
import argparse
from pathlib import Path

# ---------------------------------------------------------------------------
# Operator mapping table
# ---------------------------------------------------------------------------
# (negated, flag, type) -> C++ operator
# type is 'int32_t' or 'uint32_t' (or 'int64_t'/'uint64_t' but rare)
OP_MAP = {
    # .eq
    (False, 'eq', 'int32_t'):   '==',
    (True,  'eq', 'int32_t'):   '!=',
    (False, 'eq', 'uint32_t'):  '==',
    (True,  'eq', 'uint32_t'):  '!=',
    (False, 'eq', 'int64_t'):   '==',
    (True,  'eq', 'int64_t'):   '!=',
    (False, 'eq', 'uint64_t'):  '==',
    (True,  'eq', 'uint64_t'):  '!=',
    # .lt  (signed: less than; unsigned: below)
    (False, 'lt', 'int32_t'):   '<',
    (True,  'lt', 'int32_t'):   '>=',
    (False, 'lt', 'uint32_t'):  '<',
    (True,  'lt', 'uint32_t'):  '>=',
    (False, 'lt', 'int64_t'):   '<',
    (True,  'lt', 'int64_t'):   '>=',
    (False, 'lt', 'uint64_t'):  '<',
    (True,  'lt', 'uint64_t'):  '>=',
    # .gt  (signed: greater than; unsigned: above)
    (False, 'gt', 'int32_t'):   '>',
    (True,  'gt', 'int32_t'):   '<=',
    (False, 'gt', 'uint32_t'):  '>',
    (True,  'gt', 'uint32_t'):  '<=',
    (False, 'gt', 'int64_t'):   '>',
    (True,  'gt', 'int64_t'):   '<=',
    (False, 'gt', 'uint64_t'):  '>',
    (True,  'gt', 'uint64_t'):  '<=',
}

# ---------------------------------------------------------------------------
# Regex patterns
# ---------------------------------------------------------------------------

# Matches: ctx.cr6.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);
# Groups: (indent)(cr_field)(type)(lhs)(rhs)
RE_COMPARE = re.compile(
    r'^(\s*)ctx\.(cr\d+)\.compare<(\w+)>\((.+),\s*(.+),\s*ctx\.xer\);'
)

# Matches: if (ctx.cr6.eq) goto loc_820C0030;
#       or if (!ctx.cr6.eq) goto loc_820C0030;
# Groups: (indent)(negation: '' or '!')(cr_field)(flag)(label)
RE_BRANCH = re.compile(
    r'^(\s*)if \((!?)ctx\.(cr\d+)\.(eq|lt|gt|so)\) goto (loc_\w+);'
)

# Matches a label line: loc_820C0030:  (may have trailing whitespace or comment)
RE_LABEL = re.compile(r'^\s*loc_\w+:')

# Matches a blank line or pure comment line
RE_BLANK_OR_COMMENT = re.compile(r'^\s*(//.*)?$')


def infer_operator(negated: bool, flag: str, T: str) -> str | None:
    """Return the C++ operator, or None if we can't inline (e.g. .so flag)."""
    return OP_MAP.get((negated, flag, T))


def process_lines(lines: list[str]) -> tuple[list[str], dict]:
    """
    Process a list of lines (one function body) and return
    (transformed_lines, stats_dict).
    """
    out = []
    stats = {
        'inlined': 0,
        'flushed_label': 0,
        'flushed_code': 0,
        'skipped_so': 0,
        'skipped_multi_use': 0,
    }

    # Pending compare state: None or dict
    # {cr, T, lhs, rhs, orig_line, indent, intervening: [str]}
    pending: dict | None = None

    def flush(reason: str):
        nonlocal pending
        if pending is None:
            return
        # Emit original compare line
        out.append(pending['orig_line'])
        # Emit any buffered intervening lines
        out.extend(pending['intervening'])
        stats[f'flushed_{reason}'] += 1
        pending = None

    i = 0
    while i < len(lines):
        line = lines[i]
        raw = line.rstrip('\n')

        # --- Check for a new compare ---
        m_cmp = RE_COMPARE.match(raw)
        if m_cmp:
            indent, cr, T, lhs, rhs = m_cmp.groups()
            lhs = lhs.strip()
            rhs = rhs.strip()
            # If there's a pending compare for the same CR, flush it first
            if pending and pending['cr'] == cr:
                flush('code')  # CR overwrite
            elif pending:
                flush('code')  # Different CR, still flush (conservative)
            pending = {
                'cr': cr,
                'T': T,
                'lhs': lhs,
                'rhs': rhs,
                'orig_line': raw,
                'indent': indent,
                'intervening': [],
            }
            i += 1
            continue

        # --- If we have a pending compare, handle subsequent lines carefully ---
        if pending:
            # Check for matching branch
            m_br = RE_BRANCH.match(raw)
            if m_br:
                br_indent, neg_str, br_cr, flag, label = m_br.groups()
                negated = (neg_str == '!')
                if br_cr == pending['cr']:
                    if flag == 'so':
                        # Can't inline .so — flush and emit branch as-is
                        flush('code')
                        stats['skipped_so'] += 1
                        out.append(raw)
                        i += 1
                        continue
                    op = infer_operator(negated, flag, pending['T'])
                    if op:
                        # SUCCESS: inline! Emit any intervening non-code lines first
                        out.extend(pending['intervening'])
                        # Build the inlined line preserving branch indentation
                        inlined = f'{br_indent}if ({pending["lhs"]} {op} {pending["rhs"]}) goto {label};'
                        out.append(inlined)
                        stats['inlined'] += 1
                        pending = None
                        i += 1
                        continue
                    else:
                        flush('code')
                        out.append(raw)
                        i += 1
                        continue
                else:
                    # Branch on different CR field — flush pending, handle branch normally
                    # The branch itself might be a standalone (non-compare) branch
                    flush('code')
                    out.append(raw)
                    i += 1
                    continue

            # Check for label line — labels are join points, flush pending
            if RE_LABEL.match(raw):
                flush('label')
                out.append(raw)
                i += 1
                continue

            # Blank line or pure comment — buffer into intervening (don't flush)
            if RE_BLANK_OR_COMMENT.match(raw):
                pending['intervening'].append(raw)
                i += 1
                continue

            # Any other code line — flush pending (not safe to skip over code)
            flush('code')
            out.append(raw)
            i += 1
            continue

        # --- No pending compare — emit line as-is ---
        out.append(raw)
        i += 1

    # End of function — flush any remaining pending
    if pending:
        flush('code')

    return out, stats


def process_file(src_path: str, dst_path: str, dry_run: bool = False) -> dict:
    """Process one .cpp file. Returns aggregate stats."""
    with open(src_path, 'r', encoding='utf-8') as f:
        content = f.read()

    lines = content.split('\n')
    out_lines, stats = process_lines(lines)

    out_content = '\n'.join(out_lines)

    if not dry_run:
        os.makedirs(os.path.dirname(dst_path), exist_ok=True)
        with open(dst_path, 'w', encoding='utf-8') as f:
            f.write(out_content)

    return stats


def main():
    parser = argparse.ArgumentParser(description='Stage 1A: CR Register Inliner')
    parser.add_argument('--dry-run', action='store_true',
                        help='Process files but do not write output')
    parser.add_argument('--file', type=str, default=None,
                        help='Process a single specific file (basename only)')
    parser.add_argument('--project-root', type=str,
                        default='/Users/Ozordi/Downloads/tt-decomp',
                        help='Path to tt-decomp project root')
    args = parser.parse_args()

    root = Path(args.project_root)
    src_dir = root / 'recomp' / 'generated'
    dst_dir = root / 'recomp' / 'structured'

    # Gather files
    if args.file:
        candidates = [src_dir / args.file]
    else:
        candidates = sorted(src_dir.glob('tt-decomp_recomp.*.cpp'))
        # Also include init
        init_file = src_dir / 'tt-decomp_init.cpp'
        if init_file.exists():
            candidates = [init_file] + list(candidates)

    if not candidates:
        print(f'ERROR: No files found in {src_dir}')
        sys.exit(1)

    # Create structured dir and copy non-cpp files (headers, cmake, etc.)
    if not args.dry_run:
        dst_dir.mkdir(parents=True, exist_ok=True)
        for f in src_dir.iterdir():
            if f.suffix != '.cpp':
                dst = dst_dir / f.name
                if not dst.exists():
                    import shutil
                    shutil.copy2(str(f), str(dst))
                    print(f'  Copied {f.name}')

    # Process each file
    total_inlined = 0
    total_flushed = 0
    total_skipped_so = 0
    files_processed = 0

    print(f'\n{"="*60}')
    print(f'Stage 1A: CR Register Inliner')
    print(f'Source : {src_dir}')
    print(f'Output : {dst_dir}')
    print(f'Dry run: {args.dry_run}')
    print(f'{"="*60}\n')

    for src_path in candidates:
        dst_path = dst_dir / src_path.name
        stats = process_file(str(src_path), str(dst_path), dry_run=args.dry_run)
        
        total_inlined += stats['inlined']
        total_flushed += stats['flushed_label'] + stats['flushed_code']
        total_skipped_so += stats['skipped_so']
        files_processed += 1
        
        action = '(dry-run)' if args.dry_run else '→ written'
        print(f'  {src_path.name:<45} inlined={stats["inlined"]:>5}  '
              f'flushed={stats["flushed_label"]+stats["flushed_code"]:>5}  '
              f'skipped_so={stats["skipped_so"]:>3}  {action}')

    print(f'\n{"="*60}')
    print(f'SUMMARY')
    print(f'  Files processed : {files_processed}')
    print(f'  Total inlined   : {total_inlined:,}')
    print(f'  Total flushed   : {total_flushed:,}  (kept as-is due to intervening code/labels)')
    print(f'  Skipped (.so)   : {total_skipped_so:,}')
    print(f'{"="*60}\n')

    if not args.dry_run:
        print('✓ Output written to recomp/structured/')
        print()
        print('Next steps:')
        print('  1. Spot-check: diff recomp/generated/tt-decomp_recomp.0.cpp recomp/structured/tt-decomp_recomp.0.cpp | head -80')
        print('  2. Compile check: clang++ -std=c++20 -fsyntax-only -I../../include recomp/structured/tt-decomp_recomp.0.cpp')
        print('  3. Count verification: grep -c "ctx.cr6.compare" recomp/generated/tt-decomp_recomp.0.cpp')
        print('                         grep -c "ctx.cr6.compare" recomp/structured/tt-decomp_recomp.0.cpp')


if __name__ == '__main__':
    main()
