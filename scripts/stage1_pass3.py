#!/usr/bin/env python3
"""
Stage 1 — Pass 3: Residual Goto Elimination
=============================================

Three validated techniques applied in order per file:

  Technique A — Return-block inlining
      Gotos targeting a label whose entire block (≤10 code lines) leads
      directly to return; with no internal gotos.  Block inlined at every
      call-site; label deleted when pred-count hits zero.
      Validated elimination: ~3,905 gotos.

  Technique B — goto → break inside structured loops
      A goto LOC (or if (C) goto LOC) whose target label immediately follows
      a "} while (...);" line is a semantic break.  Replace with break;.
      Validated elimination: ~153 gotos.

  Technique C — N-way cascade → nested if-else
      A merge label with N≥2 forward-only preds where the region
      [first_pred … merge) is a self-contained flat DAG:
        • no backward jumps
        • no forward escapes past merge
        • no external entries to interior labels
        • region is brace-flat (each arm individually balanced — no
          existing structured if/else crossing arm boundaries)
      Rewrites into nested if-else, eliminating all N gotos.
      Validated elimination: ~2,889 gotos (flat subset).

Usage:
    python3 scripts/stage1_pass3.py [--dry-run] [--file NAME]
                                    [--skip-c] [--max-region N] [--verbose]

Input:   recomp/structured_final/
Output:  recomp/structured_pass3/
"""

import re, os, sys, shutil, argparse, collections
from pathlib import Path

# ─────────────────────────────────────────────────────────────────────────────
# Config
# ─────────────────────────────────────────────────────────────────────────────
INPUT_DIR  = Path("recomp/structured_final")
OUTPUT_DIR = Path("recomp/structured_pass3")

MAX_INLINE_BLOCK   = 10    # max code lines in a return-block to inline
MAX_CASCADE_REGION = 800   # max source lines in a cascade region
MAX_C_PREDS        = 40    # skip cascade if more than this many preds

# ─────────────────────────────────────────────────────────────────────────────
# Regex
# ─────────────────────────────────────────────────────────────────────────────
RE_LABEL     = re.compile(r'^(\s*)(loc_[0-9A-Fa-f]+):\s*$')
RE_BRANCH    = re.compile(r'^(\s*)if \((.+?)\) goto (loc_[0-9A-Fa-f]+);\s*$')
RE_GOTO      = re.compile(r'^(\s*)goto (loc_[0-9A-Fa-f]+);\s*$')
RE_RETURN    = re.compile(r'^\s*return;\s*$')
RE_FUNC_IMPL = re.compile(r'^PPC_FUNC_IMPL\(')
RE_BLANK_CMT = re.compile(r'^\s*(//.*)?$')
RE_WHILE_END = re.compile(r'^(\s*)\} while \(.+?\);\s*$')

# ─────────────────────────────────────────────────────────────────────────────
# Helpers
# ─────────────────────────────────────────────────────────────────────────────

def build_label_map(lines):
    m = {}
    for i, l in enumerate(lines):
        r = RE_LABEL.match(l)
        if r:
            m[r.group(2)] = i
    return m


def build_pred_map(lines):
    preds = collections.defaultdict(list)
    for i, l in enumerate(lines):
        r = RE_BRANCH.match(l)
        if r:
            preds[r.group(3)].append(i)
            continue
        r = RE_GOTO.match(l)
        if r:
            preds[r.group(2)].append(i)
    return preds


def is_code(line):
    s = line.strip()
    return bool(s) and not s.startswith('//')


def negate_cond(cond):
    cond = cond.strip()
    if cond.startswith('!(') and cond.endswith(')'):
        return cond[2:-1]
    if cond.startswith('!') and ' ' not in cond[1:3]:
        return cond[1:]
    return f'!({cond})'


def brace_delta(lines):
    """Net brace depth change across a list of lines."""
    delta = 0
    for l in lines:
        delta += l.count('{') - l.count('}')
    return delta


# ─────────────────────────────────────────────────────────────────────────────
# TECHNIQUE A — Return-block inlining
# ─────────────────────────────────────────────────────────────────────────────

def _return_block(lines, label_idx):
    """
    Collect lines of the block starting just after label_idx.
    Returns list of raw lines if eligible, else None.
    Eligible = ends with return;, ≤MAX_INLINE_BLOCK code lines, no internal gotos.

    Tracks running brace depth; stops when depth goes negative (we've hit a
    closing brace opened before the label — e.g. the function-closing '}').
    This fixes a bug where end-of-function labels were ineligible because
    the function '}' was included as a code line, preventing return; detection.
    """
    block = []
    depth = 0
    for j in range(label_idx + 1, min(label_idx + MAX_INLINE_BLOCK * 5, len(lines))):
        l = lines[j]
        depth += l.count('{') - l.count('}')
        if depth < 0:
            break                      # closing a brace opened before this label
        if RE_LABEL.match(l) or RE_FUNC_IMPL.match(l):
            break
        block.append(l)

    code = [l for l in block if is_code(l)]
    if not code:
        return None
    if not RE_RETURN.match(code[-1]):
        return None
    if len(code) > MAX_INLINE_BLOCK:
        return None
    if any(RE_GOTO.match(l) for l in code):
        return None
    # Reject blocks containing conditional branches whose target is outside the block.
    # Such blocks may contain loop back-edges or multi-path structures; inlining them
    # would corrupt the brace structure (e.g. producing 'do {' without '} while').
    block_labels = set()
    for l in block:
        rl = RE_LABEL.match(l)
        if rl:
            block_labels.add(rl.group(2))
    for l in code:
        rb = RE_BRANCH.match(l)
        if rb and rb.group(3) not in block_labels:
            return None  # branch escapes outside block — unsafe to inline
    return block


def _has_unsafe_internal_label(lines, label_idx, block, pred_map):
    """
    True if any label WITHIN block has a predecessor outside the block.
    Inlining would leave those external gotos dangling.
    """
    block_start = label_idx + 1
    block_end   = block_start + len(block)
    for line in block:
        r = RE_LABEL.match(line)
        if r:
            for p in pred_map.get(r.group(2), []):
                if p < block_start or p >= block_end:
                    return True
    return False


def _has_implicit_fallthrough(lines, label_idx):
    """
    True if execution can fall through to label_idx implicitly — i.e., the
    line immediately before the label (ignoring blanks/comments) is NOT an
    unconditional terminator.

    Unconditional terminators that PREVENT fall-through:
      return;
      goto loc_X;    (unconditional goto)
      __builtin_trap()

    Everything else allows fall-through, including:
      }              (closing brace of if/else/loop — execution continues)
      if (C) goto X; (conditional branch — false path falls through)
      regular code   (assignments, function calls, etc.)
    """
    for j in range(label_idx - 1, -1, -1):
        s = lines[j].strip()
        if not s or s.startswith('//'):
            continue                        # skip blanks and comments
        if s == 'return;':
            return False
        if RE_GOTO.match(lines[j]):
            return False
        if '__builtin_trap()' in s:
            return False
        return True                         # anything else = fall-through
    return False                            # top of file, no fall-through


def technique_a(lines, verbose=False):
    """Return (new_lines, n_eliminated)."""
    total = 0
    for _pass in range(20):
        label_map = build_label_map(lines)
        pred_map  = build_pred_map(lines)

        # inline_eligible: labels whose block can be copied at goto/if-goto sites
        # removable: subset where the label+body can also be deleted after inlining
        #            (requires no implicit fall-through predecessor so that stripping
        #            the body doesn't sever that path — Bug B fix)
        inline_eligible = {}
        removable       = set()

        for lbl, idx in label_map.items():
            blk = _return_block(lines, idx)
            if blk is None:
                continue
            if _has_unsafe_internal_label(lines, idx, blk, pred_map):
                continue
            if not pred_map.get(lbl):
                continue
            inline_eligible[lbl] = blk
            if not _has_implicit_fallthrough(lines, idx):
                removable.add(lbl)

        if not inline_eligible:
            break

        pass_count = 0
        pred_consumed = collections.defaultdict(int)
        new_lines = []
        i = 0
        while i < len(lines):
            line = lines[i]

            rg = RE_GOTO.match(line)
            rb = RE_BRANCH.match(line)
            tgt = None; is_cond = False; cond_str = None; indent = ''

            if rg and rg.group(2) in inline_eligible:
                tgt, indent = rg.group(2), rg.group(1)
            elif rb and rb.group(3) in inline_eligible:
                tgt, indent, cond_str, is_cond = rb.group(3), rb.group(1), rb.group(2), True

            if tgt:
                blk = inline_eligible[tgt]
                pred_consumed[tgt] += 1
                pass_count += 1
                total += 1
                if is_cond:
                    # FIX (Bug A): use the ORIGINAL condition, not its negation.
                    # if (C) goto LABEL  →  if (C) { block; return; }
                    # The false path falls through past the if block — correct semantics.
                    new_lines.append(f'{indent}if ({cond_str}) {{\n')
                    for bl in blk:
                        new_lines.append((indent + '\t' + bl.lstrip('\t')) if bl.strip() else bl)
                    new_lines.append(f'{indent}}}\n')
                else:
                    for bl in blk:
                        new_lines.append((indent + bl.lstrip('\t')) if bl.strip() else bl)
                i += 1
                continue

            # Drop label if all its preds were just inlined away
            rl_m = RE_LABEL.match(line)
            if rl_m:
                lbl = rl_m.group(2)
                orig = len(pred_map.get(lbl, []))
                done = pred_consumed.get(lbl, 0)
                if (orig > 0 and orig == done
                        and lbl in inline_eligible
                        and lbl in removable):   # FIX (Bug B): only remove if no fall-through
                    # Skip the label line itself
                    i += 1
                    # CRITICAL: also strip the orphaned body lines that follow.
                    # Without this, the inlined return-block body stays in place
                    # as unreachable dead code at every former call-site.
                    # Stop at the next structural boundary: another label,
                    # function boundary, closing brace, or switch-case keyword.
                    while i < len(lines):
                        s = lines[i].strip()
                        if not s or s.startswith('//'):
                            i += 1   # blank / comment inside old body — skip
                            continue
                        if (RE_LABEL.match(lines[i]) or
                                RE_FUNC_IMPL.match(lines[i]) or
                                s in ('}', '};') or
                                s.startswith('} while') or
                                s.startswith('} else') or
                                s.startswith('case ') or
                                s == 'default:'):
                            break    # structural boundary — stop, keep this line
                        i += 1       # ordinary body code line — drop it
                    continue

            new_lines.append(line)
            i += 1

        lines = new_lines
        if verbose:
            print(f'  [A] pass {_pass+1}: inlined {pass_count}')
        if pass_count == 0:
            break

    return lines, total


# ─────────────────────────────────────────────────────────────────────────────
# TECHNIQUE B — goto → break inside structured loops
# ─────────────────────────────────────────────────────────────────────────────

def _find_loop_start(lines, while_end_idx):
    """Find the index of the 'do {' or '{' that matches the '} while' at while_end_idx.
    Returns the line index, or -1 if not found within a reasonable range."""
    balance = 0
    for k in range(while_end_idx, max(-1, while_end_idx - 2000), -1):
        balance += lines[k].count('}') - lines[k].count('{')
        if balance == 0 and k < while_end_idx:
            return k
    return -1


def technique_b(lines, verbose=False):
    """Replace goto / if-goto to loop-exit label with break / if-break."""
    label_map = build_label_map(lines)

    # Map label → index of "} while (...);" line that it immediately follows
    loop_exit_label = {}
    for i, line in enumerate(lines):
        if RE_WHILE_END.match(line):
            for j in range(i + 1, min(i + 6, len(lines))):
                if not lines[j].strip():
                    continue
                rl_m = RE_LABEL.match(lines[j])
                if rl_m:
                    loop_exit_label[rl_m.group(2)] = i
                break

    eliminated = 0
    new_lines = []
    for i, line in enumerate(lines):
        rg = RE_GOTO.match(line)
        rb = RE_BRANCH.match(line)
        tgt = None; is_cond = False; cond_str = None; indent = ''

        if rg:
            tgt, indent = rg.group(2), rg.group(1)
        elif rb:
            tgt, indent, cond_str, is_cond = rb.group(3), rb.group(1), rb.group(2), True

        if tgt and tgt in loop_exit_label:
            while_end_idx = loop_exit_label[tgt]
            tgt_pos = label_map.get(tgt, -1)
            # Only replace if the goto is inside the loop (before the } while)
            if i < while_end_idx < tgt_pos:
                # Additional safety: verify the goto is inside the SPECIFIC do-while
                # loop by finding the matching 'do {' for the '} while' line.
                # This prevents converting gotos that happen to precede an unrelated
                # (possibly malformed) loop structure.
                loop_start = _find_loop_start(lines, while_end_idx)
                if loop_start >= 0 and loop_start <= i:
                    eliminated += 1
                    if is_cond:
                        new_lines.append(f'{indent}if ({cond_str}) break;\n')
                    else:
                        new_lines.append(f'{indent}break;\n')
                    continue

        new_lines.append(line)

    if verbose and eliminated:
        print(f'  [B] eliminated {eliminated} goto→break')
    return new_lines, eliminated


# ─────────────────────────────────────────────────────────────────────────────
# TECHNIQUE C — N-way cascade → nested if-else
# ─────────────────────────────────────────────────────────────────────────────

def _validate_cascade(lines, first_pred, tgt_pos, label_map, pred_map):
    """
    Safety conditions for a cascade region [first_pred … tgt_pos):

    1. No goto within region escapes past tgt_pos (forward escape)
    2. No goto within region jumps to before first_pred (backward = loop)
    3. No label within region has a pred from outside the region
    4. BRACE-FLAT: the entire region has net brace delta == 0  AND
       each individual arm (lines between consecutive preds) is also
       individually balanced (delta == 0).  This guards against
       rewriting across existing if/else structure.

    Returns True if all conditions pass.
    """
    labels_in_region = set()
    for j in range(first_pred, tgt_pos):
        rl_m = RE_LABEL.match(lines[j])
        if rl_m:
            labels_in_region.add(rl_m.group(2))

    for j in range(first_pred, tgt_pos):
        rg = RE_GOTO.match(lines[j])
        rb = RE_BRANCH.match(lines[j])
        ref = rg or rb
        if ref:
            ref_tgt = rg.group(2) if rg else rb.group(3)
            ep = label_map.get(ref_tgt, -1)
            if ep > tgt_pos:              # forward escape past merge
                return False
            if 0 <= ep < first_pred:      # backward jump before region
                return False

    for il in labels_in_region:
        for p in pred_map.get(il, []):
            if p < first_pred or p >= tgt_pos:
                return False              # external entry to interior label

    # Brace-flatness check: the region as a whole must be balanced
    if brace_delta(lines[first_pred:tgt_pos]) != 0:
        return False

    return True


def _arm_is_flat(arm_lines):
    """True if the arm's brace delta is 0 (no net open/close braces)."""
    return brace_delta(arm_lines) == 0


def _rewrite_cascade(lines, first_pred, tgt_pos, preds_list, merge_label):
    """
    Rewrite a flat cascade region into a nested if-else chain.
    All preds must be forward branches or gotos to merge_label.
    Returns replacement lines for [first_pred … tgt_pos).
    """
    sorted_preds = sorted(set(preds_list))

    # Determine base indent from first pred
    first_line = lines[sorted_preds[0]]
    rg = RE_GOTO.match(first_line)
    rb = RE_BRANCH.match(first_line)
    indent = (rg or rb).group(1) if (rg or rb) else '\t'

    out = []
    depth = 0  # number of pending else-blocks to close

    for k, pred_idx in enumerate(sorted_preds):
        is_last = (k == len(sorted_preds) - 1)
        next_boundary = sorted_preds[k + 1] if not is_last else tgt_pos
        arm_lines = lines[pred_idx + 1 : next_boundary]

        # Strip trailing blank/comment lines from arm
        while arm_lines and RE_BLANK_CMT.match(arm_lines[-1]):
            arm_lines = arm_lines[:-1]

        # Remove trailing goto-to-merge from arm (becomes implicit fall-through)
        arm_code = [l for l in arm_lines if is_code(l)]
        if arm_code:
            last_rg = RE_GOTO.match(arm_code[-1])
            if last_rg and last_rg.group(2) == merge_label:
                for ridx in range(len(arm_lines) - 1, -1, -1):
                    if is_code(arm_lines[ridx]):
                        arm_lines = arm_lines[:ridx] + arm_lines[ridx + 1:]
                        break
                arm_code = [l for l in arm_lines if is_code(l)]

        cur_indent = indent + '\t' * depth
        line = lines[pred_idx]
        rg = RE_GOTO.match(line)
        rb = RE_BRANCH.match(line)

        if rb:
            cond = rb.group(2)
            br_tgt = rb.group(3)

            if br_tgt == merge_label:
                # Pattern: if (C) goto MERGE → if (!C) { arm_body } [else {]
                neg = negate_cond(cond)
                has_body = bool(arm_code)

                if not has_body:
                    # Arm is empty — guard with if(!C) {} or just nothing
                    pass  # nothing to emit for empty arm
                elif is_last:
                    # Last arm — no else needed
                    out.append(f'{cur_indent}if ({neg}) {{\n')
                    for bl in arm_lines:
                        out.append((cur_indent + '\t' + bl.lstrip('\t')) if bl.strip() else bl)
                    out.append(f'{cur_indent}}}\n')
                else:
                    out.append(f'{cur_indent}if ({neg}) {{\n')
                    for bl in arm_lines:
                        out.append((cur_indent + '\t' + bl.lstrip('\t')) if bl.strip() else bl)
                    depth += 1
                    out.append(f'{cur_indent}}} else {{\n')
            else:
                # Branch to somewhere else — emit as-is with arm body
                out.append(cur_indent + line.lstrip('\t'))
                for bl in arm_lines:
                    out.append((cur_indent + bl.lstrip('\t')) if bl.strip() else bl)

        elif rg:
            # Unconditional goto to merge — arm body just falls through
            # (the goto is removed; arm body is the fall-through path)
            for bl in arm_lines:
                out.append((cur_indent + bl.lstrip('\t')) if bl.strip() else bl)

    # Close all open else blocks
    for d in range(depth, 0, -1):
        out.append(indent + '\t' * (d - 1) + '}\n')

    return out


def technique_c(lines, verbose=False, max_region=MAX_CASCADE_REGION):
    """Apply N-way cascade → nested if-else. Returns (new_lines, n_eliminated)."""
    total = 0

    for _pass in range(60):  # each pass removes at least one label
        label_map = build_label_map(lines)
        pred_map  = build_pred_map(lines)

        # Pick the smallest valid cascade region to transform first
        best = None  # (region_size, tgt, first_pred, tgt_pos, preds_list)

        for tgt, preds_list in pred_map.items():
            tgt_pos = label_map.get(tgt, -1)
            if tgt_pos < 0:
                continue
            n = len(preds_list)
            if n < 2 or n > MAX_C_PREDS:
                continue
            # All preds must be strictly forward
            if any(p >= tgt_pos for p in preds_list):
                continue
            first_pred = min(preds_list)
            region_size = tgt_pos - first_pred
            if region_size > max_region or region_size < 2:
                continue

            # All preds must be a branch-to-merge or goto-to-merge
            valid = True
            for p in preds_list:
                rg = RE_GOTO.match(lines[p])
                rb = RE_BRANCH.match(lines[p])
                if rg:
                    if rg.group(2) != tgt:
                        valid = False; break
                elif rb:
                    if rb.group(3) != tgt:
                        valid = False; break
                else:
                    valid = False; break
            if not valid:
                continue

            if not _validate_cascade(lines, first_pred, tgt_pos, label_map, pred_map):
                continue

            # Additional per-arm flatness check
            sorted_preds = sorted(set(preds_list))
            arms_flat = True
            for k, pi in enumerate(sorted_preds):
                nb = sorted_preds[k + 1] if k + 1 < len(sorted_preds) else tgt_pos
                if not _arm_is_flat(lines[pi + 1 : nb]):
                    arms_flat = False; break
            if not arms_flat:
                continue

            if best is None or region_size < best[0]:
                best = (region_size, tgt, first_pred, tgt_pos, preds_list)

        if best is None:
            break

        _, tgt, first_pred, tgt_pos, preds_list = best
        replacement = _rewrite_cascade(lines, first_pred, tgt_pos, preds_list, tgt)
        lines = lines[:first_pred] + replacement + lines[tgt_pos:]
        total += len(set(preds_list))

        if verbose:
            print(f'  [C] pass {_pass+1}: -{len(set(preds_list))} gotos → {tgt}'
                  f' (region {tgt_pos - first_pred} lines)')

    return lines, total


# ─────────────────────────────────────────────────────────────────────────────
# Per-file driver
# ─────────────────────────────────────────────────────────────────────────────

def process_file(src_path, dst_path, args):
    with open(src_path) as f:
        lines = f.readlines()

    before = sum(1 for l in lines if 'goto loc_' in l)

    lines, elim_a = technique_a(lines, verbose=args.verbose)
    lines, elim_b = technique_b(lines, verbose=args.verbose)
    if not args.skip_c:
        lines, elim_c = technique_c(lines, verbose=args.verbose, max_region=args.max_region)
    else:
        elim_c = 0

    after = sum(1 for l in lines if 'goto loc_' in l)

    if not args.dry_run:
        dst_path.parent.mkdir(parents=True, exist_ok=True)
        with open(dst_path, 'w') as f:
            f.writelines(lines)

    return dict(file=src_path.name, before=before, after=after,
                elim_a=elim_a, elim_b=elim_b, elim_c=elim_c,
                delta=before - after)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser(description='Stage 1 Pass 3: Residual goto elimination')
    ap.add_argument('--dry-run',    action='store_true')
    ap.add_argument('--file',       metavar='NAME')
    ap.add_argument('--skip-c',     action='store_true', help='Skip Technique C')
    ap.add_argument('--max-region', type=int, default=MAX_CASCADE_REGION)
    ap.add_argument('--verbose',    action='store_true')
    args = ap.parse_args()

    src_files = sorted(INPUT_DIR.glob('*.cpp'))
    if args.file:
        src_files = [f for f in src_files if f.name == args.file]
        if not src_files:
            sys.exit(f'ERROR: {args.file} not found in {INPUT_DIR}')

    if not args.dry_run:
        OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
        for other in INPUT_DIR.iterdir():
            if other.suffix != '.cpp':
                shutil.copy2(other, OUTPUT_DIR / other.name)

    total_before = total_after = 0
    total_a = total_b = total_c = 0
    rows = []

    for src in src_files:
        dst = OUTPUT_DIR / src.name
        if args.verbose:
            print(f'\n=== {src.name} ===')
        r = process_file(src, dst, args)
        rows.append(r)
        total_before += r['before']; total_after += r['after']
        total_a += r['elim_a'];      total_b   += r['elim_b']
        total_c += r['elim_c']
        if r['delta'] > 0 or args.verbose:
            print(f"  {r['file']:<40}  before={r['before']:>5}  after={r['after']:>5}  "
                  f"A={r['elim_a']:>4} B={r['elim_b']:>3} C={r['elim_c']:>4}  delta={r['delta']:>4}")

    print()
    print('=' * 70)
    print(f'  Files processed : {len(rows)}')
    print(f'  Gotos before    : {total_before:,}')
    print(f'  Gotos after     : {total_after:,}')
    print(f'  Eliminated      : {total_before - total_after:,}  '
          f'(A={total_a:,}  B={total_b:,}  C={total_c:,})')
    pct_pass    = (total_before - total_after) / total_before * 100 if total_before else 0
    pct_overall = (117661 - total_after) / 117661 * 100
    print(f'  Pass 3 reduction: {pct_pass:.1f}%')
    print(f'  Overall reduction (from 117,661): {pct_overall:.1f}%')
    if args.dry_run:
        print('\n  (dry-run: no files written)')


if __name__ == '__main__':
    main()
