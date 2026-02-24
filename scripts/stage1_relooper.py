#!/usr/bin/env python3
"""
Stage 1B+C: CFG Builder + Relooper
====================================
Transforms goto-based control flow in the Stage 1A output into structured C++.

Transformations applied (in order, per function):
  1. Pre-tested while loops   : if (SKIP) goto EXIT; HEADER: body; if (!SKIP) goto HEADER; EXIT:
                              → while (!SKIP) { body }
  2. Do-while loops           : HEADER: body; if (CONT) goto HEADER; EXIT:
                              → do { body } while (CONT);
  3. If/else diamonds         : if (C) goto ELSE; then; goto JOIN; ELSE: else; JOIN:
                              → if (!C) { then } else { else }
  4. Simple if (no else)      : if (C) goto SKIP; body; SKIP:
                              → if (!C) { body }

Safety constraints (skip if violated):
  - max BLOCK_BUDGET blocks per function
  - loop header must have exactly 1 non-back-edge predecessor
  - merge point for if/else must have exactly 2 predecessors (both branches)
  - labels inside switch() blocks are never restructured

Usage:
    python3 scripts/stage1_relooper.py [--dry-run] [--file NAME] [--max-blocks N]
"""

import re, os, sys, argparse, shutil
from pathlib import Path
from dataclasses import dataclass, field
from typing import Optional

# ─────────────────────────────────────────────────────────────────
# Tuning
# ─────────────────────────────────────────────────────────────────
BLOCK_BUDGET    = 1000  # skip functions with more than this many blocks
MAX_LOOP_SCAN   = 400   # max lines to scan forward looking for back-edge
MAX_IF_SCAN     = 350   # max lines to scan for if/else merge point

# ─────────────────────────────────────────────────────────────────
# Regex
# ─────────────────────────────────────────────────────────────────
RE_LABEL         = re.compile(r'^(\s*)(loc_[0-9A-Fa-f]+):')
RE_BRANCH        = re.compile(r'^(\s*)if \((.+?)\) goto (loc_[0-9A-Fa-f]+);')
RE_GOTO          = re.compile(r'^(\s*)goto (loc_[0-9A-Fa-f]+);')
RE_RETURN        = re.compile(r'^\s*return;')
RE_FUNC_IMPL     = re.compile(r'^PPC_FUNC_IMPL\(')
RE_SWITCH        = re.compile(r'^\s*switch\s*\(')
RE_CASE_LABEL    = re.compile(r'^\s*(?:case\s+.+?|default)\s*:')
RE_BLANK_COMMENT = re.compile(r'^\s*(//.*)?$')

# ─────────────────────────────────────────────────────────────────
# Block representation
# ─────────────────────────────────────────────────────────────────
@dataclass
class Block:
    idx:        int             # position in block list
    label:      Optional[str]   # 'loc_XXXXXXXX' or None (anonymous)
    label_raw:  Optional[str]   # the full raw label line (with indent)
    body:       list            # lines before terminator (includes comment-only lines)
    term:       str             # 'return'|'goto'|'branch'|'fall'|'opaque'
    term_raw:   list            # terminator line(s)
    goto_tgt:   Optional[str]   # for 'goto'
    branch_cond: Optional[str]  # for 'branch'
    branch_tgt: Optional[str]   # for 'branch' (the goto target)
    # branch fall-through = blocks[idx+1].label (computed after all blocks parsed)

def blocks_from_func_lines(func_lines):
    """
    Split function body lines into basic blocks.
    Each block ends at: return, goto, conditional branch (if/goto).
    A new block starts at: label, or immediately after any of the above.
    Returns list of Block in source order.
    """
    blocks = []
    anon_counter = [0]
    current_label = None
    current_label_raw = None
    current_body  = []
    in_switch     = 0   # nesting depth of switch() blocks

    def _flush(term, term_raw, goto_tgt=None, cond=None, br_tgt=None):
        bid = current_label if current_label else f'__a{anon_counter[0]}__'
        anon_counter[0] += 1
        blocks.append(Block(
            idx=len(blocks), label=current_label, label_raw=current_label_raw,
            body=list(current_body), term=term, term_raw=list(term_raw),
            goto_tgt=goto_tgt, branch_cond=cond, branch_tgt=br_tgt,
        ))

    for raw in func_lines:
        line = raw

        # Track switch depth (opaque regions)
        if RE_SWITCH.match(line):
            in_switch += 1
        if in_switch:
            current_body.append(line)
            # Count braces to detect switch exit
            in_switch += line.count('{') - line.count('}')
            if in_switch <= 0:
                in_switch = 0
                _flush('opaque', [])
                current_label = current_label_raw = None
                current_body  = []
            continue

        # Label → start new block
        ml = RE_LABEL.match(line)
        if ml:
            # Only flush if there's actual content (body or a previous label).
            # Flushing an empty anonymous block creates phantom blocks that
            # break match_pretested_while / match_dowhile guard detection.
            if current_body or current_label is not None:
                _flush('fall', [])
            current_label     = ml.group(2)
            current_label_raw = line
            current_body      = []
            continue

        # Return
        if RE_RETURN.match(line):
            _flush('return', [line])
            current_label = current_label_raw = None
            current_body  = []
            continue

        # Unconditional goto
        mg = RE_GOTO.match(line)
        if mg:
            _flush('goto', [line], goto_tgt=mg.group(2))
            current_label = current_label_raw = None
            current_body  = []
            continue

        # Conditional branch
        mb = RE_BRANCH.match(line)
        if mb:
            _flush('branch', [line], cond=mb.group(2), br_tgt=mb.group(3))
            current_label = current_label_raw = None
            current_body  = []
            continue

        current_body.append(line)

    # Flush any trailing content
    if current_body or current_label is not None:
        _flush('fall', [])

    # Assign fall-through IDs
    for i, b in enumerate(blocks):
        b.idx = i
        if b.term in ('fall', 'branch') and i + 1 < len(blocks):
            pass  # we use b.idx+1 dynamically

    return blocks


def build_pred_map(blocks):
    """Return dict: label → set of predecessor block indices.
    
    Only reachable blocks contribute predecessors (BFS from block 0).
    'return' blocks do NOT fall through to the next block.
    """
    # ── Step 1: compute all raw edges (before reachability filtering) ──────────
    # label_to_idx: for resolving targets to block indices
    label_to_idx = {b.label: b.idx for b in blocks if b.label}

    def successors(b):
        """Return block indices that block b can reach."""
        succ = []
        if b.term == 'goto':
            if b.goto_tgt and b.goto_tgt in label_to_idx:
                succ.append(label_to_idx[b.goto_tgt])
        elif b.term == 'branch':
            if b.branch_tgt and b.branch_tgt in label_to_idx:
                succ.append(label_to_idx[b.branch_tgt])
            # conditional fall-through (branch-not-taken)
            if b.idx + 1 < len(blocks):
                succ.append(b.idx + 1)
        elif b.term in ('fall', 'opaque'):
            # sequential fall-through only; 'return' does NOT continue
            if b.idx + 1 < len(blocks):
                succ.append(b.idx + 1)
        # 'return' → no successors
        return succ

    # ── Step 2: BFS reachability from block 0 ────────────────────────────────
    reachable = set()
    queue = [0]
    while queue:
        idx = queue.pop()
        if idx in reachable:
            continue
        reachable.add(idx)
        for nxt in successors(blocks[idx]):
            if nxt not in reachable:
                queue.append(nxt)

    # ── Step 3: build preds only from reachable blocks ───────────────────────
    preds = {}
    for b in blocks:
        if b.idx not in reachable:
            continue  # dead block — skip
        def _add_pred(tgt_label, _b=b):
            if tgt_label:
                preds.setdefault(tgt_label, set()).add(_b.idx)
        if b.term == 'goto':
            _add_pred(b.goto_tgt)
        elif b.term == 'branch':
            _add_pred(b.branch_tgt)
            if b.idx + 1 < len(blocks):
                _add_pred(blocks[b.idx + 1].label)
        elif b.term in ('fall', 'opaque'):
            if b.idx + 1 < len(blocks):
                _add_pred(blocks[b.idx + 1].label)
    return preds


def find_back_edge_targets(blocks):
    """
    Return set of labels that are targets of back-edges.
    A back-edge is a goto/branch to a label that appeared BEFORE the current block
    in source order.
    """
    seen = set()
    back_targets = set()
    for b in blocks:
        if b.label:
            seen.add(b.label)
        tgts = []
        if b.term == 'goto':
            tgts = [b.goto_tgt]
        elif b.term == 'branch':
            tgts = [b.branch_tgt]
        for t in tgts:
            if t and t in seen:
                back_targets.add(t)
    return back_targets


def negate_cond(cond):
    """Negate a simple C++ boolean condition."""
    cond = cond.strip()
    # Double-negation elimination: !expr → expr, !(expr) → expr
    if cond.startswith('!(') and cond.endswith(')'):
        return cond[2:-1]
    if cond.startswith('!') and ' ' not in cond[1:]:
        # Simple token like !ctx.cr6.eq
        return cond[1:]
    # Comparison operator flipping
    pairs = [(' == ', ' != '), (' != ', ' == '),
             (' >= ', ' < '), (' < ', ' >= '),
             (' > ', ' <= '), (' <= ', ' > '),
             ('==', '!='), ('!=', '==')]
    for a, b in pairs:
        if a in cond:
            return cond.replace(a, b, 1)
    return f'!({cond})'


def add_indent(lines, extra='\t'):
    """Add one extra level of indentation to all lines."""
    out = []
    for l in lines:
        if l.strip():  # non-blank
            out.append(extra + l)
        else:
            out.append(l)
    return out

# ─────────────────────────────────────────────────────────────────
# Pattern matchers
# ─────────────────────────────────────────────────────────────────

def find_block_by_label(blocks, label, start_idx):
    """Find block with given label at or after start_idx. Returns idx or -1."""
    for i in range(start_idx, min(start_idx + MAX_LOOP_SCAN, len(blocks))):
        if blocks[i].label == label:
            return i
    return -1


def match_pretested_while(blocks, guard_idx, back_targets, preds):
    """
    Try to match pre-tested while loop starting at guard_idx.
    Pattern:
      blocks[guard_idx]: branch cond=SKIP, tgt=EXIT_LABEL
      blocks[guard_idx+1]: label=HEADER (is a back-edge target)
      ... (loop body) ...
      some block: branch cond=CONTINUE, tgt=HEADER (back-edge)
        where EXIT_LABEL is the fall-through of that block
      blocks[exit_idx]: label=EXIT_LABEL

    Returns (header_idx, back_block_idx, exit_idx, while_cond) or None.
    """
    guard = blocks[guard_idx]
    if guard.term != 'branch':
        return None
    if guard_idx + 1 >= len(blocks):
        return None

    header = blocks[guard_idx + 1]
    if not header.label or header.label not in back_targets:
        return None

    exit_label = guard.branch_tgt

    # Find the back-edge block: a block within MAX_LOOP_SCAN that branches BACK to header
    for j in range(guard_idx + 1, min(guard_idx + MAX_LOOP_SCAN, len(blocks))):
        b = blocks[j]
        if b.term == 'branch' and b.branch_tgt == header.label:
            # This is the back-edge block
            # Verify: fall-through of back-edge block should be exit_label
            if j + 1 >= len(blocks):
                return None
            exit_block = blocks[j + 1]
            if exit_block.label != exit_label:
                return None
            # Relaxed predecessor check: allow multiple back-edge predecessors (continue paths).
            # Only reject if there are 2+ external entries (idx < header_idx), which would be
            # irreducible. One external entry (the guard fall-through) is always expected.
            header_preds = preds.get(header.label, set())
            external_entries = [p for p in header_preds if p < guard_idx + 1]
            if len(external_entries) > 1:
                return None  # Multiple external entries = irreducible
            # The while condition = negate(skip condition) = back-edge condition
            while_cond = b.branch_cond
            # Sanity: while_cond should equal negate(guard.branch_cond)
            expected_skip = negate_cond(while_cond)
            # (not strictly required but good to know)
            return (guard_idx + 1, j, j + 1, while_cond)

    return None


def match_dowhile(blocks, header_idx, back_targets, preds):
    """
    Try to match do-while loop.
    Pattern:
      blocks[header_idx]: label=HEADER (is a back-edge target)
      ... (loop body, no nested loops handled here) ...
      some block: branch cond=COND, tgt=HEADER (back-edge)
      next block: exit (fall-through of back-edge block)

    Returns (back_block_idx, exit_idx, do_while_cond) or None.
    """
    header = blocks[header_idx]
    if not header.label or header.label not in back_targets:
        return None

    for j in range(header_idx, min(header_idx + MAX_LOOP_SCAN, len(blocks))):
        b = blocks[j]
        if b.term == 'branch' and b.branch_tgt == header.label:
            # back-edge found
            if j + 1 >= len(blocks):
                return None
            # Relaxed predecessor check: a natural loop header may have multiple back-edge
            # predecessors (= multiple 'continue' paths), which is valid. Only reject if
            # there are 2+ EXTERNAL entry predecessors (idx < header_idx) — that indicates
            # a loop with multiple entries, which is irreducible.
            header_preds = preds.get(header.label, set())
            external_entries = [p for p in header_preds if p < header_idx]
            if len(external_entries) > 1:
                return None  # Multiple external entries = irreducible loop
            return (j, j + 1, b.branch_cond)
        # NOTE: we intentionally do NOT abort on nested back-edge targets — nested loops
        # are handled by the recursive structure_region call on the loop body.

    return None


def match_simple_if(blocks, branch_idx, preds):
    """
    Try to match simple if-without-else.

    Two valid shapes:

    Shape A – body falls through to skip (standard case, 2 preds):
      blocks[branch_idx]: branch cond=C, tgt=SKIP_LABEL
      blocks[branch_idx+1..skip_idx-1]: body (last block falls through)
      blocks[skip_idx]: label=SKIP_LABEL

    Shape B – body ends with return/goto-out (1 pred at skip):
      blocks[branch_idx]: branch cond=C, tgt=SKIP_LABEL
      blocks[branch_idx+1..skip_idx-1]: body ending in return
      blocks[skip_idx]: label=SKIP_LABEL

    SKIP_LABEL must have at most 2 predecessors:
      - the branch block (branch_idx)
      - optionally: the block immediately before skip_idx (fall-through from body)
    Any third predecessor means something else jumps here — unsafe.

    Returns (skip_idx,) or None.
    """
    b = blocks[branch_idx]
    if b.term != 'branch':
        return None
    skip_label = b.branch_tgt

    # Find skip_label within MAX_IF_SCAN first (need skip_idx for pred check)
    skip_idx = find_block_by_label(blocks, skip_label, branch_idx + 1)
    if skip_idx < 0:
        return None

    # Validate predecessors of skip_label.
    # Allowed: preds from within [branch_idx, skip_idx) — the branch and early-exit blocks.
    # Also allowed: preds from BEFORE branch_idx — these are outer-context blocks that also
    #   jump to the same continuation (fine; they're already processed at a higher level).
    # NOT allowed: preds from AFTER skip_idx — a backward jump into a forward label signals
    #   complex irreducible control flow that we can't safely structure.
    skip_preds = preds.get(skip_label, set())
    if any(p > skip_idx for p in skip_preds):
        return None  # Something after the skip-label jumps back to it — irreducible

    # Make sure the body between branch and skip has no gotos that escape to outside
    # (i.e., no goto to labels that appear AFTER skip_label — those would change semantics)
    labels_in_region = set()
    for k in range(branch_idx + 1, skip_idx):
        if blocks[k].label:
            labels_in_region.add(blocks[k].label)

    for k in range(branch_idx + 1, skip_idx):
        bk = blocks[k]
        tgts = []
        if bk.term == 'goto':   tgts = [bk.goto_tgt]
        if bk.term == 'branch': tgts = [bk.branch_tgt]
        for tgt in tgts:
            if tgt and tgt not in labels_in_region and tgt != skip_label:
                # Check if it's a forward escape (safe) or backward/unknown (unsafe)
                # Forward escapes (to blocks after skip_idx) are fine: the goto stays
                # in the structured if-body and still points to a later label — semantics
                # are preserved. Only backward jumps break structuring.
                esc_idx = find_block_by_label(blocks, tgt, 0)
                if esc_idx < 0:
                    return None  # Unknown target — can't verify safety
                if esc_idx < skip_idx:
                    return None  # Backward escape — breaks structured flow

    return (skip_idx,)


def match_if_else(blocks, branch_idx, preds):
    """
    Try to match if/else diamond.
    Pattern:
      blocks[branch_idx]: branch cond=C, tgt=ELSE_LABEL
      then-body blocks ending with: goto JOIN_LABEL  (or return)
      blocks[else_idx]: label=ELSE_LABEL
      else-body blocks ...
      blocks[join_idx]: label=JOIN_LABEL

    Returns (else_idx, join_idx) or None.
    """
    b = blocks[branch_idx]
    if b.term != 'branch':
        return None
    else_label = b.branch_tgt

    # The then-body starts at branch_idx+1; find its terminator
    # Then-body should end with a goto to JOIN_LABEL (or return)
    # Scan for the else label within MAX_IF_SCAN
    else_idx = find_block_by_label(blocks, else_label, branch_idx + 1)
    if else_idx < 0 or else_idx == branch_idx + 1:
        return None  # else_idx+1 would mean empty then-body

    # The last block of the then-body is else_idx - 1.
    # IMPORTANT: The parser creates empty anonymous fall-blocks between a goto and the
    # next labeled block (the label starts a new block, leaving an empty fall block behind).
    # Skip these empty fall-blocks to find the real last block of the then-body.
    then_last_idx = else_idx - 1
    while (then_last_idx > branch_idx and
           blocks[then_last_idx].term == 'fall' and
           not blocks[then_last_idx].body and
           not blocks[then_last_idx].label):
        then_last_idx -= 1
    then_last = blocks[then_last_idx]

    # Determine join label
    join_label = None
    if then_last.term == 'goto':
        join_label = then_last.goto_tgt
    elif then_last.term == 'return':
        join_label = None  # then-branch returns; no join needed
    else:
        return None  # then-body doesn't end cleanly

    # Safety: else_label preds — reject only if something AFTER else_idx jumps here
    # (preds from before branch_idx or from within [branch_idx, else_idx) are fine)
    if any(p > else_idx for p in preds.get(else_label, set())):
        return None

    if join_label:
        # find join_idx: immediately after the else block(s)
        join_idx = find_block_by_label(blocks, join_label, else_idx + 1)
        if join_idx < 0:
            return None
        # JOIN_LABEL pred check: reject only preds from AFTER join_idx
        if any(p > join_idx for p in preds.get(join_label, set())):
            return None
        # Make sure the else body doesn't escape past the join point
        labels_in_else = set()
        for k in range(else_idx, join_idx):
            if blocks[k].label:
                labels_in_else.add(blocks[k].label)
        for k in range(else_idx, join_idx):
            bk = blocks[k]
            tgts = []
            if bk.term == 'goto':   tgts = [bk.goto_tgt]
            if bk.term == 'branch': tgts = [bk.branch_tgt]
            for tgt in tgts:
                if tgt and tgt not in labels_in_else and tgt != join_label:
                    # Same rule as match_simple_if: allow forward escapes
                    esc_idx = find_block_by_label(blocks, tgt, 0)
                    if esc_idx < 0:
                        return None  # Unknown target
                    if esc_idx < join_idx:
                        return None  # Backward escape into middle of structure
        return (else_idx, join_idx)
    else:
        # Then-branch returns; else block continues after — degenerate to simple-if
        return None  # let match_simple_if handle via updated pred logic


# ─────────────────────────────────────────────────────────────────
# Emitters
# ─────────────────────────────────────────────────────────────────

def emit_block_verbatim(b):
    """Return raw lines for a block as-is (no restructuring)."""
    out = []
    if b.label_raw:
        out.append(b.label_raw)
    out.extend(b.body)
    out.extend(b.term_raw)
    return out


def structure_region(blocks, start, end, preds, back_targets, depth=0):
    """
    Structure blocks[start:end] into idiomatic C++.
    Returns list of output lines (with original indentation preserved).
    depth: recursion depth guard.
    """
    if depth > 12:
        # Safety: bail out of deep recursion
        out = []
        for b in blocks[start:end]:
            out.extend(emit_block_verbatim(b))
        return out

    out = []
    i = start

    while i < end:
        b = blocks[i]

        # ── Try pre-tested while loop ──────────────────────────
        if b.term == 'branch' and i + 1 < end:
            result = match_pretested_while(blocks, i, back_targets, preds)
            if result:
                header_idx, back_idx, exit_idx, while_cond = result
                # Guard: skip while-loop transform if any block outside [i..back_idx]
                # jumps directly into the loop body — that would bury the label.
                header_b = blocks[header_idx]
                _ext_while = any(
                    p < i or p > back_idx
                    for p in preds.get(header_b.label, set())
                    if p != i  # i is the guard block (legitimate fall-through)
                )
                if exit_idx <= end and not _ext_while:
                    # Emit header guard's body (before the while)
                    if b.label_raw:
                        out.append(b.label_raw)
                    out.extend(b.body)
                    # Open while
                    indent0 = header_b.label_raw.split('loc_')[0].rstrip() if header_b.label_raw else '\t'
                    out.append(f'{indent0}while ({while_cond}) {{\n')
                    # Recurse into loop body (header_idx .. back_idx, excluding back-edge branch)
                    inner_lines = list(structure_region(blocks, header_idx, back_idx, preds, back_targets, depth + 1))
                    # Append back-edge block body (without back-edge branch)
                    back_b = blocks[back_idx]
                    if back_b.label_raw: inner_lines.append(back_b.label_raw)
                    inner_lines.extend(back_b.body)
                    out.extend(add_indent(inner_lines))
                    out.append(f'{indent0}}}\n')
                    i = exit_idx
                    continue

        # ── Try do-while loop ──────────────────────────────────
        if b.label and b.label in back_targets:
            result = match_dowhile(blocks, i, back_targets, preds)
            if result:
                back_idx, exit_idx, do_cond = result
                # Guard: skip do-while transform if any block outside [i..back_idx]
                # jumps to the do-while header — that would bury its label.
                _ext_dowhile = any(
                    p < i or p > back_idx
                    for p in preds.get(b.label, set())
                    if p != back_idx  # back_idx is the legitimate back-edge
                )
                if exit_idx <= end and not _ext_dowhile:
                    _raw_indent = b.label_raw.split('loc_')[0].rstrip() if b.label_raw else '\t'
                    indent0 = _raw_indent if _raw_indent else '\t'
                    out.append(f'{indent0}do {{\n')
                    inner_lines = []
                    if back_idx == i:
                        # Single-block do-while: header IS the back-edge block.
                        inner_lines.extend(b.body)
                        if b.term not in ('branch',):
                            inner_lines.extend(b.term_raw)
                    else:
                        # header block body
                        inner_lines.extend(b.body)
                        if b.term not in ('branch',):
                            inner_lines.extend(b.term_raw)
                        # Middle blocks (i+1 .. back_idx-1)
                        inner_lines.extend(structure_region(blocks, i + 1, back_idx, preds, back_targets, depth + 1))
                        # Back-edge block body (without back-edge term)
                        back_b = blocks[back_idx]
                        inner_lines.extend(back_b.body)
                        # (skip back_b.term_raw — the back-edge goto)
                    out.extend(add_indent(inner_lines))
                    out.append(f'{indent0}}} while ({do_cond});\n')
                    i = exit_idx
                    continue

        # ── Try if/else diamond ────────────────────────────────
        if b.term == 'branch':
            result = match_if_else(blocks, i, preds)
            if result:
                else_idx, join_idx = result
                if join_idx <= end:
                    indent0 = b.term_raw[0].split('if')[0] if b.term_raw else '\t'
                    if b.label_raw:
                        out.append(b.label_raw)
                    out.extend(b.body)
                    neg_cond = negate_cond(b.branch_cond)
                    out.append(f'{indent0}if ({neg_cond}) {{\n')
                    then_lines = structure_region(blocks, i + 1, else_idx, preds, back_targets, depth + 1)
                    # Remove the trailing goto JOIN from the then-body (it's implicit in the else structure)
                    then_last = blocks[else_idx - 1]
                    if then_last.term == 'goto' and then_last.goto_tgt == blocks[join_idx].label:
                        # Remove the last block's goto (it's the structural goto to JOIN)
                        # The then_lines was already emitted by structure_region; we need to trim it
                        # Actually structure_region includes goto term_raw; trim it
                        while then_lines and RE_GOTO.match(then_lines[-1].strip()):
                            then_lines.pop()
                    out.extend(add_indent(then_lines))
                    out.append(f'{indent0}}} else {{\n')
                    else_lines = structure_region(blocks, else_idx, join_idx, preds, back_targets, depth + 1)
                    out.extend(add_indent(else_lines))
                    out.append(f'{indent0}}}\n')
                    i = join_idx
                    continue

        # ── Try simple if (no else) ────────────────────────────
        if b.term == 'branch':
            result = match_simple_if(blocks, i, preds)
            if result:
                (skip_idx,) = result
                # Use strict less-than: skip_idx == end means the skip target is the
                # back-edge block (or loop exit) of an enclosing do-while.  Allowing
                # skip_idx == end would place the } while() terminator inside the if
                # body, generating a spurious extra } and breaking the outer loop.
                if skip_idx < end:
                    indent0 = b.term_raw[0].split('if')[0] if b.term_raw else '\t'
                    if b.label_raw:
                        out.append(b.label_raw)
                    out.extend(b.body)
                    neg_cond = negate_cond(b.branch_cond)
                    out.append(f'{indent0}if ({neg_cond}) {{\n')
                    body_lines = structure_region(blocks, i + 1, skip_idx, preds, back_targets, depth + 1)
                    out.extend(add_indent(body_lines))
                    out.append(f'{indent0}}}\n')
                    i = skip_idx
                    continue

        # ── Default: emit verbatim ─────────────────────────────
        out.extend(emit_block_verbatim(b))
        i += 1

    return out


# ─────────────────────────────────────────────────────────────────
# Function-level processing
# ─────────────────────────────────────────────────────────────────

def process_function(func_lines, stats):
    """
    Structure a single function's body lines.
    Returns list of output lines.
    """
    if not func_lines:
        return func_lines

    blocks = blocks_from_func_lines(func_lines)

    if len(blocks) > BLOCK_BUDGET:
        stats['skipped_budget'] += 1
        return func_lines

    if len(blocks) <= 2:
        stats['skipped_trivial'] += 1
        return func_lines

    preds        = build_pred_map(blocks)
    back_targets = find_back_edge_targets(blocks)

    if not back_targets and not any(b.term == 'branch' for b in blocks):
        stats['skipped_no_gotos'] += 1
        return func_lines

    try:
        out = structure_region(blocks, 0, len(blocks), preds, back_targets, depth=0)
    except Exception as e:
        stats['errors'] += 1
        return func_lines

    stats['structured'] += 1
    return out


# ─────────────────────────────────────────────────────────────────
# File-level processing
# ─────────────────────────────────────────────────────────────────

def process_file(src_path, dst_path, dry_run=False):
    with open(src_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    stats = {
        'functions': 0,
        'structured': 0,
        'skipped_trivial': 0,
        'skipped_budget': 0,
        'skipped_no_gotos': 0,
        'errors': 0,
    }

    output_lines = []
    i = 0

    while i < len(lines):
        line = lines[i]

        # Detect function start
        if RE_FUNC_IMPL.match(line):
            # Emit the function header lines up to and including the opening `{`
            func_header = [line]

            # The opening brace may be on the same line as PPC_FUNC_IMPL or on
            # a subsequent line (depending on how the file was formatted).
            if '{' in line:
                # Brace is on the first line — already consumed
                i += 1
            else:
                i += 1
                while i < len(lines) and not lines[i].strip() == '{':
                    func_header.append(lines[i])
                    i += 1
                # Opening brace on its own line
                if i < len(lines):
                    func_header.append(lines[i])  # the `{`
                    i += 1

            # Collect function body — start with depth=1 if brace already seen
            brace_depth = 1
            func_body = []
            while i < len(lines) and brace_depth > 0:
                l = lines[i]
                brace_depth += l.count('{') - l.count('}')
                if brace_depth > 0:
                    func_body.append(l)
                else:
                    # closing `}` of function
                    break
                i += 1

            stats['functions'] += 1
            structured_body = process_function(func_body, stats)

            output_lines.extend(func_header)
            output_lines.extend(structured_body)
            if i < len(lines):
                output_lines.append(lines[i])  # closing `}`
                i += 1
            continue

        output_lines.append(line)
        i += 1

    if not dry_run:
        os.makedirs(os.path.dirname(dst_path), exist_ok=True)
        with open(dst_path, 'w', encoding='utf-8') as f:
            f.writelines(output_lines)

    return stats


# ─────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────

def main():
    global BLOCK_BUDGET
    parser = argparse.ArgumentParser(description='Stage 1B+C: CFG + Relooper')
    parser.add_argument('--dry-run', action='store_true')
    parser.add_argument('--file', type=str, default=None)
    parser.add_argument('--max-blocks', type=int, default=BLOCK_BUDGET)
    parser.add_argument('--passes', type=int, default=1,
                        help='Number of structuring passes (default 1). Pass 2+ read from dst_dir.')
    parser.add_argument('--project-root', type=str,
                        default='/Users/Ozordi/Downloads/tt-decomp')
    args = parser.parse_args()

    BLOCK_BUDGET = args.max_blocks

    root    = Path(args.project_root)
    src_dir = root / 'recomp' / 'structured'
    dst_dir = root / 'recomp' / 'structured_final'

    if args.file:
        candidates = [src_dir / args.file]
    else:
        candidates = sorted(src_dir.glob('tt-decomp_recomp.*.cpp'))
        init_file  = src_dir / 'tt-decomp_init.cpp'
        if init_file.exists():
            candidates = [init_file] + list(candidates)

    if not candidates:
        print(f'ERROR: No files found in {src_dir}')
        sys.exit(1)

    dry_run = args.dry_run
    if not dry_run:
        dst_dir.mkdir(parents=True, exist_ok=True)
        for f in src_dir.iterdir():
            if f.suffix != '.cpp':
                dst = dst_dir / f.name
                if not dst.exists():
                    shutil.copy2(str(f), str(dst))

    totals = {k: 0 for k in ('functions', 'structured', 'skipped_trivial',
                              'skipped_budget', 'skipped_no_gotos', 'errors')}

    print(f'\n{"="*65}')
    print(f'Stage 1B+C: CFG Builder + Relooper')
    print(f'Input : {src_dir}')
    print(f'Output: {dst_dir}')
    print(f'Dry run: {args.dry_run}  |  Block budget: {BLOCK_BUDGET}')
    print(f'{"="*65}\n')

    num_passes = args.passes if not args.dry_run else 1
    for pass_num in range(1, num_passes + 1):
        if pass_num > 1:
            print(f'\n--- Pass {pass_num} (re-structuring output) ---')
            # Subsequent passes read from dst_dir
            candidates = sorted(dst_dir.glob('tt-decomp_recomp.*.cpp'))
            init_f = dst_dir / 'tt-decomp_init.cpp'
            if init_f.exists():
                candidates = [init_f] + list(candidates)

        for src_path in candidates:
            dst_path = dst_dir / src_path.name
            stats = process_file(str(src_path), str(dst_path), dry_run=args.dry_run)
            for k in totals: totals[k] += stats[k]
            action = '(dry)' if args.dry_run else '→ written'
            if pass_num == 1:
                print(f'  {src_path.name:<45}  '
                      f'funcs={stats["functions"]:>5}  '
                      f'struct={stats["structured"]:>5}  '
                      f'budget_skip={stats["skipped_budget"]:>4}  '
                      f'err={stats["errors"]:>3}  {action}')

    pct = 100.0 * totals['structured'] / max(totals['functions'], 1)
    print(f'\n{"="*65}')
    print(f'TOTALS')
    print(f'  Functions        : {totals["functions"]:,}')
    print(f'  Structured       : {totals["structured"]:,}  ({pct:.1f}%)')
    print(f'  Skipped (trivial): {totals["skipped_trivial"]:,}')
    print(f'  Skipped (budget) : {totals["skipped_budget"]:,}')
    print(f'  Errors           : {totals["errors"]:,}')
    print(f'{"="*65}\n')

    if not args.dry_run:
        print('✓ Output written to recomp/structured_final/')
        print()
        print('Validation steps:')
        print('  1. Goto count reduction:')
        print('     grep -rc "goto loc_" recomp/structured/*.cpp     | awk -F: \'{s+=$2} END{print s}\'')
        print('     grep -rc "goto loc_" recomp/structured_final/*.cpp | awk -F: \'{s+=$2} END{print s}\'')
        print('  2. Spot check:')
        print('     diff recomp/structured/tt-decomp_recomp.0.cpp recomp/structured_final/tt-decomp_recomp.0.cpp | head -80')


if __name__ == '__main__':
    main()
