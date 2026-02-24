#!/usr/bin/env python3
"""
Stage 2: Register and Boilerplate Cleanup
==========================================

Transforms recomp/structured_pass3/ into recomp/structured_pass4/ by
removing PPC-ABI noise in five ordered phases per function.

Phase A — CTR Dispatch Collapse  (11,975 instances)
    ctx.ctr.u64 = ctx.rN.u64;
    ctx.lr = 0xADDR;
    PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    →  PPC_CALL_INDIRECT_FUNC(ctx.rN.u32);

Phase B — Call-site LR Removal  (~60 K instances after Phase A)
    ctx.lr = 0xADDR;
    sub_XXXX(ctx, base);
    →  sub_XXXX(ctx, base);

Phase C — Frame Boilerplate Collapse  (11,061 frames)
    Removes mflr + optional savegprlr/manual-spill + stwu prologue.
    Removes frame-pop + restgprlr/LR-restore epilogue (every exit path).
    Emits a // FRAME: comment summarising what was removed.

Phase D — Residual CR Comparison Inlining  (~20 K cases)
    Handles the cases Stage 1 cr_inline.py missed: compare and branch
    separated by ≤5 non-CR, non-label code lines.
    ctx.cr6.compare<T>(lhs, rhs, ctx.xer);  ...intervening...  if (ctx.cr6.eq)
    →  if (lhs OP rhs)

Phase E — Callee-Saved Register Promotion  (r14–r31, f14–f31)
    For each callee-saved register used inside the function body
    (after Phase C has removed spill/restore boilerplate):
      uint32_t var_r31 = 0;  inserted after PPC_FUNC_PROLOGUE()
      ctx.r31.u64 = X  →  var_r31 = (uint32_t)(X)
      ctx.r31.u32  →  var_r31
      ctx.r31.s64  →  (int64_t)(int32_t)var_r31
      ctx.r31.s32  →  (int32_t)var_r31
    (FPR: double var_f31 = 0.0)

Usage:
    python3 scripts/stage2_cleanup.py [--dry-run] [--file recomp.N.cpp]
                                      [--no-phase LETTER] [--verbose]

Input:   recomp/structured_pass3/
Output:  recomp/structured_pass4/
"""

import re, os, sys, shutil, argparse, collections
from pathlib import Path

# ─────────────────────────────────────────────────────────────────────────────
# Config
# ─────────────────────────────────────────────────────────────────────────────
INPUT_DIR  = Path("recomp/structured_pass3")
OUTPUT_DIR = Path("recomp/structured_pass4")

# ─────────────────────────────────────────────────────────────────────────────
# Shared Regex
# ─────────────────────────────────────────────────────────────────────────────

RE_FUNC_IMPL  = re.compile(r'^PPC_FUNC_IMPL\(')
RE_CLOSE_BRACE = re.compile(r'^\}\s*$')
RE_BLANK_CMT  = re.compile(r'^\s*(//.*)?$')
RE_LABEL      = re.compile(r'^\s*loc_[0-9A-Fa-f]+:\s*')

# ── Phase A ──────────────────────────────────────────────────────────────────
RE_CTR_ASSIGN = re.compile(
    r'^(\s*)ctx\.ctr\.u64\s*=\s*(ctx\.r\d+)\.u64\s*;'
)
RE_LR_HEX     = re.compile(r'^\s*ctx\.lr\s*=\s*0x[0-9A-Fa-f]+\s*;\s*$')
RE_INDIRECT   = re.compile(
    r'^(\s*)PPC_CALL_INDIRECT_FUNC\(ctx\.ctr\.u32\)\s*;'
)

# ── Phase B ──────────────────────────────────────────────────────────────────
RE_SUB_CALL   = re.compile(r'^\s*(sub_[0-9A-Fa-f]+|__imp__\w+|__save(?:fpr|vmx)_\d+|__rest(?:fpr|vmx)_\d+)\s*\(ctx,\s*base\)\s*;\s*$')
RE_SAVEGPR    = re.compile(r'^\s*__savegprlr_\d+\s*\(ctx,\s*base\)\s*;\s*$')
RE_RESTGPR    = re.compile(r'^\s*__restgprlr_\d+\s*\(ctx,\s*base\)\s*;\s*$')

# ── Phase C ──────────────────────────────────────────────────────────────────
RE_MFLR       = re.compile(r'^\s*ctx\.r12\.u64\s*=\s*ctx\.lr\s*;\s*$')
RE_STW_LR     = re.compile(
    r'^\s*PPC_STORE_U32\s*\(\s*ctx\.r1\.u32\s*\+\s*-8\s*,\s*ctx\.r12\.u32\s*\)\s*;\s*$'
)
RE_STD_REG    = re.compile(
    r'^\s*PPC_STORE_U64\s*\(\s*ctx\.r1\.u32\s*\+\s*(-\d+)\s*,\s*ctx\.(r\d+)\.u64\s*\)\s*;\s*$'
)
RE_STWU_EA    = re.compile(
    r'^\s*ea\s*=\s*(-\d+)\s*\+\s*ctx\.r1\.u32\s*;\s*$'
)
RE_STWU_STORE = re.compile(
    r'^\s*PPC_STORE_U32\s*\(\s*ea\s*,\s*ctx\.r1\.u32\s*\)\s*;\s*$'
)
RE_STWU_UPD   = re.compile(r'^\s*ctx\.r1\.u32\s*=\s*ea\s*;\s*$')
RE_FRAME_POP  = re.compile(
    r'^\s*ctx\.r1\.s64\s*=\s*ctx\.r1\.s64\s*\+\s*(\d+)\s*;\s*$'
)
RE_LWZ_LR     = re.compile(
    r'^\s*ctx\.r12\.u64\s*=\s*PPC_LOAD_U32\s*\(\s*ctx\.r1\.u32\s*\+\s*-8\s*\)\s*;\s*$'
)
RE_MTLR       = re.compile(r'^\s*ctx\.lr\s*=\s*ctx\.r12\.u64\s*;\s*$')
RE_LDR_REG    = re.compile(
    r'^\s*ctx\.(r\d+)\.u64\s*=\s*PPC_LOAD_U64\s*\(\s*ctx\.r1\.u32\s*\+\s*(-\d+)\s*\)\s*;\s*$'
)
RE_RETURN     = re.compile(r'^\s*return\s*;\s*$')

# ── Phase C FPR spill/restore (savegprlr+stfd prologues) ─────────────────────
RE_FPR_SPILL   = re.compile(
    r'^\s*PPC_STORE_U64\s*\(\s*ctx\.r1\.u32\s*\+\s*(-\d+)\s*,\s*ctx\.(f\d+)\.u64\s*\)\s*;'
)
RE_FPR_RESTORE = re.compile(
    r'^\s*ctx\.(f\d+)\.u64\s*=\s*PPC_LOAD_U64\s*\(\s*ctx\.r1\.u32\s*\+\s*(-\d+)\s*\)\s*;'
)
RE_FPSCR_LINE  = re.compile(r'^\s*ctx\.fpscr\.')
RE_ADDI_R1     = re.compile(
    r'^\s*ctx\.(r\d+)\.(s64|u64)\s*=\s*ctx\.r1\.(s64|u64)\s*\+\s*(-\d+)\s*;'
)

# ── Phase D ──────────────────────────────────────────────────────────────────
RE_COMPARE    = re.compile(
    r'^(\s*)ctx\.(cr\d+)\.compare(?:<(\w+)>)?\((.+?),\s*(.+?),\s*ctx\.xer\)\s*;\s*$'
)
RE_COMPARE_FP = re.compile(
    r'^(\s*)ctx\.(cr\d+)\.compare\((.+?),\s*(.+?)\)\s*;\s*$'
)
RE_CR_BRANCH  = re.compile(
    r'^(\s*)if\s*\((!?)ctx\.(cr\d+)\.(eq|lt|gt|so)\)\s*(\{?)\s*$'
)
RE_CR_GOTO    = re.compile(
    r'^(\s*)if\s*\((!?)ctx\.(cr\d+)\.(eq|lt|gt|so)\)\s+goto\s+(loc_\w+)\s*;\s*$'
)
# Also match the parenthesized-negation form: if (!(ctx.crN.flag)) {
RE_CR_BRANCH2 = re.compile(
    r'^(\s*)if\s*\((!?)\(ctx\.(cr\d+)\.(eq|lt|gt|so)\)\)\s*(\{?)\s*$'
)
RE_CR_GOTO2   = re.compile(
    r'^(\s*)if\s*\((!?)\(ctx\.(cr\d+)\.(eq|lt|gt|so)\)\)\s+goto\s+(loc_\w+)\s*;\s*$'
)
# Conditional return: if (ctx.crN.flag) return;   (beqlr / bnelr / etc.)
RE_CR_RETURN  = re.compile(
    r'^(\s*)if\s*\((!?)ctx\.(cr\d+)\.(eq|lt|gt|so)\)\s+return\s*;\s*$'
)
# do-while closer: } while (ctx.crN.flag);   (bdnz / bdz / etc.)
RE_CR_DOWHILE = re.compile(
    r'^(\s*)\}\s*while\s*\((!?)ctx\.(cr\d+)\.(eq|lt|gt|so)\)\s*;\s*$'
)
# while loop opener: while (ctx.crN.flag) {
RE_CR_WHILE   = re.compile(
    r'^(\s*)while\s*\((!?)ctx\.(cr\d+)\.(eq|lt|gt|so)\)\s*(\{?)\s*$'
)
RE_CR_USES    = re.compile(r'ctx\.(cr\d+)\.')

OP_TABLE = {
    (False,'eq','int32_t'):'==',  (True,'eq','int32_t'):'!=',
    (False,'eq','uint32_t'):'==', (True,'eq','uint32_t'):'!=',
    (False,'eq','int64_t'):'==',  (True,'eq','int64_t'):'!=',
    (False,'eq','uint64_t'):'==', (True,'eq','uint64_t'):'!=',
    (False,'eq',None):'==',       (True,'eq',None):'!=',
    (False,'lt','int32_t'):'<',   (True,'lt','int32_t'):'>=',
    (False,'lt','uint32_t'):'<',  (True,'lt','uint32_t'):'>=',
    (False,'lt','int64_t'):'<',   (True,'lt','int64_t'):'>=',
    (False,'lt','uint64_t'):'<',  (True,'lt','uint64_t'):'>=',
    (False,'lt',None):'<',        (True,'lt',None):'>=',
    (False,'gt','int32_t'):'>',   (True,'gt','int32_t'):'<=',
    (False,'gt','uint32_t'):'>',  (True,'gt','uint32_t'):'<=',
    (False,'gt','int64_t'):'>',   (True,'gt','int64_t'):'<=',
    (False,'gt','uint64_t'):'>',  (True,'gt','uint64_t'):'<=',
    (False,'gt',None):'>',        (True,'gt',None):'<=',
}

# ── Phase E ──────────────────────────────────────────────────────────────────
CALLEE_GPR = set(f'r{n}' for n in range(14, 32))
CALLEE_FPR = set(f'f{n}' for n in range(14, 32))

RE_GPR_ACCESS = re.compile(
    r'ctx\.(r(?:1[4-9]|2[0-9]|3[01]))\.(u64|s64|u32|s32|u16|s16|u8)'
)
RE_FPR_ACCESS = re.compile(
    r'ctx\.(f(?:1[4-9]|2[0-9]|3[01]))\.(f64|f32)'
)
RE_ASSIGN_LHS = re.compile(
    r'^(\s*)ctx\.(r(?:1[4-9]|2[0-9]|3[01])|f(?:1[4-9]|2[0-9]|3[01]))'
    r'\.(u64|s64|u32|s32|u16|s16|u8|f64|f32)\s*=\s*(.*?)\s*;\s*$'
)
RE_PROLOGUE_MARKER = re.compile(r'^\s*PPC_FUNC_PROLOGUE\(\)\s*;\s*$')

# ─────────────────────────────────────────────────────────────────────────────
# Utility
# ─────────────────────────────────────────────────────────────────────────────

def strip_indent(line):
    return line.lstrip('\t ')

def get_indent(line):
    return line[:len(line) - len(line.lstrip('\t '))]

def is_code_line(line):
    s = line.strip()
    return bool(s) and not s.startswith('//')

def is_blank_or_comment(line):
    return not is_code_line(line)

# ─────────────────────────────────────────────────────────────────────────────
# Phase A: CTR Dispatch Collapse
# ─────────────────────────────────────────────────────────────────────────────

def phase_a(lines):
    """
    Collapse the 3-line CTR dispatch sequence:
      ctx.ctr.u64 = ctx.rN.u64;
      ctx.lr = 0xADDR;
      PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    →
      PPC_CALL_INDIRECT_FUNC(ctx.rN.u32);
    """
    stats = {'collapsed': 0}
    out = list(lines)

    i = len(out) - 1
    while i >= 0:
        m_call = RE_INDIRECT.match(out[i])
        if not m_call:
            i -= 1
            continue

        indent = m_call.group(1)
        call_idx = i

        j = i - 1
        lr_idx = None
        while j >= 0 and is_blank_or_comment(out[j]):
            j -= 1
        if j >= 0 and RE_LR_HEX.match(out[j]):
            lr_idx = j
            j -= 1
        else:
            i -= 1
            continue

        while j >= 0 and is_blank_or_comment(out[j]):
            j -= 1
        if j < 0:
            i -= 1
            continue
        m_ctr = RE_CTR_ASSIGN.match(out[j])
        if not m_ctr:
            i -= 1
            continue
        ctr_idx = j
        reg = m_ctr.group(2)

        mtctr_idx = None
        if ctr_idx > 0 and re.match(r'^\s*//\s*mtctr\s', out[ctr_idx - 1]):
            mtctr_idx = ctr_idx - 1

        bctrl_idx = None
        for k in range(lr_idx + 1, call_idx):
            if re.match(r'^\s*//\s*bctrl\s*', out[k]):
                bctrl_idx = k
                break

        new_call = f'{indent}PPC_CALL_INDIRECT_FUNC({reg}.u32);\n'

        to_delete = set()
        to_delete.add(ctr_idx)
        to_delete.add(lr_idx)
        if mtctr_idx is not None:
            to_delete.add(mtctr_idx)
        if bctrl_idx is not None:
            to_delete.add(bctrl_idx)

        out[call_idx] = new_call

        for idx in sorted(to_delete, reverse=True):
            del out[idx]

        stats['collapsed'] += 1
        i = call_idx - len([x for x in to_delete if x < call_idx]) - 1

    return out, stats

# ─────────────────────────────────────────────────────────────────────────────
# Phase B: Call-site LR Removal
# ─────────────────────────────────────────────────────────────────────────────

def phase_b(lines):
    """
    Remove  ctx.lr = 0xADDR;  lines immediately before sub_XXXX(ctx, base) calls.
    Does NOT remove LR lines before __savegprlr.
    """
    stats = {'removed': 0}
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        if RE_LR_HEX.match(line):
            j = i + 1
            interleaved = []
            while j < len(lines) and is_blank_or_comment(lines[j]):
                interleaved.append(lines[j])
                j += 1
            if j < len(lines):
                nxt = lines[j]
                if RE_SUB_CALL.match(nxt):
                    out.extend(interleaved)
                    i = j
                    stats['removed'] += 1
                    continue
        out.append(line)
        i += 1
    return out, stats

# ─────────────────────────────────────────────────────────────────────────────
# Phase C: Frame Boilerplate Collapse
# ─────────────────────────────────────────────────────────────────────────────

def _collect_prologue(lines, start):
    n = len(lines)
    i = start
    prologue_indices = set()
    frame_type = None
    frame_size = None
    savegpr_n = None

    if not RE_MFLR.match(lines[i]):
        return None
    prologue_indices.add(i)
    i += 1

    while i < n and is_blank_or_comment(lines[i]):
        i += 1
    if i >= n:
        return None

    if RE_LR_HEX.match(lines[i]):
        lr_idx = i
        i += 1
        while i < n and is_blank_or_comment(lines[i]):
            i += 1
        if i < n and RE_SAVEGPR.match(lines[i]):
            m = re.search(r'__savegprlr_(\d+)', lines[i])
            savegpr_n = int(m.group(1)) if m else None
            prologue_indices.add(lr_idx)
            prologue_indices.add(i)
            frame_type = 'savegprlr'
            i += 1
            # Allow FPR saves (stfd fN,-O(r1)) between savegprlr and stwu
            while i < n:
                while i < n and is_blank_or_comment(lines[i]):
                    i += 1
                if i < n and (RE_FPR_SPILL.match(lines[i]) or RE_FPSCR_LINE.match(lines[i])):
                    prologue_indices.add(i)  # FPR spill — remove as part of prologue
                    i += 1
                elif i < n and RE_ADDI_R1.match(lines[i]):
                    i += 1  # addi rN,r1,-offset (frame ptr) — skip but DO NOT delete
                else:
                    break
        else:
            return None
    elif RE_STW_LR.match(lines[i]):
        frame_type = 'manual'
        prologue_indices.add(i)
        i += 1
        while i < n:
            while i < n and is_blank_or_comment(lines[i]):
                i += 1
            if i < n and RE_STD_REG.match(lines[i]):
                prologue_indices.add(i)
                i += 1
            else:
                break
    else:
        return None

    while i < n and is_blank_or_comment(lines[i]):
        i += 1
    if i >= n:
        return None

    m_ea = RE_STWU_EA.match(lines[i])
    if not m_ea:
        return None
    frame_size = abs(int(m_ea.group(1)))
    prologue_indices.add(i)
    i += 1

    while i < n and is_blank_or_comment(lines[i]):
        i += 1
    if i >= n or not RE_STWU_STORE.match(lines[i]):
        return None
    prologue_indices.add(i)
    i += 1

    while i < n and is_blank_or_comment(lines[i]):
        i += 1
    if i >= n or not RE_STWU_UPD.match(lines[i]):
        return None
    prologue_indices.add(i)

    return prologue_indices, frame_size, frame_type, savegpr_n


def _find_epilogues(lines, frame_size, frame_type):
    epilogues = []
    n = len(lines)

    for i in range(n):
        m_pop = RE_FRAME_POP.match(lines[i])
        if not m_pop:
            continue
        if int(m_pop.group(1)) != frame_size:
            continue

        epilogue_indices = {i}
        j = i + 1

        while j < n and is_blank_or_comment(lines[j]):
            j += 1
        if j >= n:
            continue

        if frame_type == 'savegprlr':
            # Allow FPR restores (lfd fN,-O(r1)) between frame_pop and restgprlr
            while j < n:
                while j < n and is_blank_or_comment(lines[j]):
                    j += 1
                if j < n and (RE_FPR_RESTORE.match(lines[j]) or RE_FPSCR_LINE.match(lines[j])):
                    epilogue_indices.add(j)
                    j += 1
                else:
                    break
            if j < n and RE_RESTGPR.match(lines[j]):
                epilogue_indices.add(j)
                j += 1
                while j < n and is_blank_or_comment(lines[j]):
                    j += 1
                if j < n and RE_RETURN.match(lines[j]):
                    epilogues.append(epilogue_indices)
        elif frame_type == 'manual':
            if not RE_LWZ_LR.match(lines[j]):
                continue
            epilogue_indices.add(j)
            j += 1
            while j < n and is_blank_or_comment(lines[j]):
                j += 1
            if j >= n or not RE_MTLR.match(lines[j]):
                continue
            epilogue_indices.add(j)
            j += 1
            while j < n:
                while j < n and is_blank_or_comment(lines[j]):
                    j += 1
                if j < n and RE_LDR_REG.match(lines[j]):
                    epilogue_indices.add(j)
                    j += 1
                else:
                    break
            while j < n and is_blank_or_comment(lines[j]):
                j += 1
            if j < n and RE_RETURN.match(lines[j]):
                epilogues.append(epilogue_indices)

    return epilogues


def phase_c(lines):
    stats = {'frames_collapsed': 0, 'epilogues_removed': 0}
    n = len(lines)

    for start in range(n):
        if not RE_MFLR.match(lines[start]):
            continue
        result = _collect_prologue(lines, start)
        if result is None:
            continue
        prologue_indices, frame_size, frame_type, savegpr_n = result

        asm_comment_indices = set()
        for pi in prologue_indices:
            k = pi - 1
            if k >= 0 and re.match(r'^\s*//', lines[k]) and k not in prologue_indices:
                asm_comment_indices.add(k)

        all_prologue = prologue_indices | asm_comment_indices

        if frame_type == 'savegprlr':
            frame_desc = f'savegprlr_{savegpr_n}' if savegpr_n else 'savegprlr'
        else:
            frame_desc = 'manual'
        indent_p = get_indent(lines[start])
        frame_comment = f'{indent_p}// FRAME: size={frame_size}, {frame_desc}\n'

        epilogues = _find_epilogues(lines, frame_size, frame_type)

        all_epilogue_sets = []
        for epi in epilogues:
            epi_cmts = set()
            for pi in epi:
                k = pi - 1
                if k >= 0 and re.match(r'^\s*//', lines[k]) and k not in epi:
                    epi_cmts.add(k)
            all_epilogue_sets.append(epi | epi_cmts)

        to_delete = set(all_prologue)
        for epi_set in all_epilogue_sets:
            to_delete |= epi_set

        new_lines = []
        prologue_comment_emitted = False
        for idx, line in enumerate(lines):
            if idx in to_delete:
                if idx == start and not prologue_comment_emitted:
                    new_lines.append(frame_comment)
                    prologue_comment_emitted = True
            else:
                new_lines.append(line)

        stats['frames_collapsed'] = 1
        stats['epilogues_removed'] = len(epilogues)
        return new_lines, stats

    return lines, stats

# ─────────────────────────────────────────────────────────────────────────────
# Phase D: Residual CR Comparison Inlining
# ─────────────────────────────────────────────────────────────────────────────

def _cr_operator(negated, flag, T):
    return OP_TABLE.get((negated == '!', flag, T))


def phase_d(lines):
    stats = {'inlined': 0, 'skipped': 0}
    out = []
    pending = None
    MAX_INTERVENING = 10

    def flush():
        nonlocal pending
        if pending is None:
            return
        out.append(pending['orig'])
        out.extend(pending['buffered'])
        pending = None

    for line in lines:
        raw = line.rstrip('\n')

        m = RE_COMPARE.match(raw)
        if m:
            flush()
            indent, cr, T, lhs, rhs = m.group(1), m.group(2), m.group(3), m.group(4).strip(), m.group(5).strip()
            pending = dict(cr=cr, T=T, lhs=lhs, rhs=rhs,
                           orig=line, indent=indent, buffered=[])
            continue

        m = RE_COMPARE_FP.match(raw)
        if m:
            flush()
            indent, cr, lhs, rhs = m.group(1), m.group(2), m.group(3).strip(), m.group(4).strip()
            pending = dict(cr=cr, T=None, lhs=lhs, rhs=rhs,
                           orig=line, indent=indent, buffered=[])
            continue

        if pending is None:
            out.append(line)
            continue

        m_br  = RE_CR_BRANCH.match(raw) or RE_CR_BRANCH2.match(raw)
        m_gt  = RE_CR_GOTO.match(raw) or RE_CR_GOTO2.match(raw)
        m_rt  = RE_CR_RETURN.match(raw)
        m_dw  = RE_CR_DOWHILE.match(raw)
        m_wh  = RE_CR_WHILE.match(raw)
        branch_match = m_br or m_gt or m_rt or m_dw or m_wh
        if branch_match and branch_match.group(3) == pending['cr']:
            neg, flag = branch_match.group(2), branch_match.group(4)
            op = _cr_operator(neg, flag, pending['T'])
            if op is not None:
                out.extend(pending['buffered'])
                if m_rt:
                    new_line = f"{pending['indent']}if ({pending['lhs']} {op} {pending['rhs']}) return;\n"
                elif m_dw:
                    new_line = f"{pending['indent']}}} while ({pending['lhs']} {op} {pending['rhs']});\n"
                elif m_wh:
                    brace = m_wh.group(5)
                    tail = (' ' + brace) if brace else ''
                    new_line = f"{pending['indent']}while ({pending['lhs']} {op} {pending['rhs']}){tail}\n"
                elif m_br:
                    brace = m_br.group(5)  # captures '{' or ''
                    tail = (' ' + brace) if brace else ''
                    new_line = f"{pending['indent']}if ({pending['lhs']} {op} {pending['rhs']}){tail}\n"
                else:
                    label = branch_match.group(5)
                    new_line = f"{pending['indent']}if ({pending['lhs']} {op} {pending['rhs']}) goto {label};\n"
                out.append(new_line)
                stats['inlined'] += 1
                pending = None
                continue
            else:
                flush()
                out.append(line)
                continue

        cr_uses = RE_CR_USES.findall(raw)
        if any(cr == pending['cr'] for cr in cr_uses):
            flush()
            out.append(line)
            continue

        if RE_LABEL.match(raw):
            flush()
            out.append(line)
            continue

        if is_code_line(line):
            if len([l for l in pending['buffered'] if is_code_line(l)]) >= MAX_INTERVENING:
                flush()
                out.append(line)
                continue

        pending['buffered'].append(line)

    flush()
    return out, stats

# ─────────────────────────────────────────────────────────────────────────────
# Phase E: Callee-Saved Register Promotion
# ─────────────────────────────────────────────────────────────────────────────

GPR_READ_MAP = {
    'u64': '{v}',
    'u32': '{v}',
    's64': '(int64_t)(int32_t){v}',
    's32': '(int32_t){v}',
    'u16': '(uint16_t){v}',
    's16': '(int16_t){v}',
    'u8':  '(uint8_t){v}',
    's8':  '(int8_t){v}',
}
FPR_READ_MAP = {
    'f64': '{v}',
    'f32': '(float){v}',
}

def _make_read_substitution(reg, suffix, var):
    if reg.startswith('r'):
        tmpl = GPR_READ_MAP.get(suffix, '{v}')
    else:
        tmpl = FPR_READ_MAP.get(suffix, '{v}')
    return tmpl.format(v=var)


def _apply_reads(text, promoted):
    for reg, var in sorted(promoted.items(), key=lambda x: -int(x[0][1:])):
        if reg.startswith('r'):
            def replace_gpr(m, var=var):
                return _make_read_substitution(reg, m.group(1), var)
            pattern = re.compile(r'ctx\.' + re.escape(reg) + r'\.(u64|s64|u32|s32|u16|s16|u8)')
            text = pattern.sub(replace_gpr, text)
        else:
            def replace_fpr(m, var=var):
                return _make_read_substitution(reg, m.group(1), var)
            pattern = re.compile(r'ctx\.' + re.escape(reg) + r'\.(f64|f32)')
            text = pattern.sub(replace_fpr, text)
    return text


def phase_e(lines):
    stats = {'vars_promoted': 0, 'lines_changed': 0}

    used_gpr = collections.OrderedDict()
    used_fpr = collections.OrderedDict()

    for line in lines:
        for m in RE_GPR_ACCESS.finditer(line):
            reg = m.group(1)
            if reg not in used_gpr:
                used_gpr[reg] = f'var_{reg}'
        for m in RE_FPR_ACCESS.finditer(line):
            reg = m.group(1)
            if reg not in used_fpr:
                used_fpr[reg] = f'var_{reg}'

    all_promoted = dict(used_gpr)
    all_promoted.update(used_fpr)

    if not all_promoted:
        return lines, stats

    gpr_decls = [f'\tuint32_t {v} = 0;\n' for v in used_gpr.values()]
    fpr_decls = [f'\tdouble {v} = 0.0;\n' for v in used_fpr.values()]
    all_decls = gpr_decls + fpr_decls
    stats['vars_promoted'] = len(all_promoted)

    out = []
    decls_inserted = False

    for line in lines:
        if not decls_inserted and RE_PROLOGUE_MARKER.match(line):
            out.append(line)
            out.extend(all_decls)
            decls_inserted = True
            continue

        has_gpr = bool(RE_GPR_ACCESS.search(line))
        has_fpr = bool(RE_FPR_ACCESS.search(line))
        if not (has_gpr or has_fpr):
            out.append(line)
            continue

        raw = line.rstrip('\n')
        indent = get_indent(raw)
        transformed = raw
        changed = False

        m_lhs = RE_ASSIGN_LHS.match(raw)
        if m_lhs:
            lhs_indent = m_lhs.group(1)
            reg = m_lhs.group(2)
            suffix = m_lhs.group(3)
            rhs = m_lhs.group(4)

            if reg in all_promoted:
                var = all_promoted[reg]
                rhs_sub = _apply_reads(rhs, all_promoted)

                if reg.startswith('r'):
                    if suffix in ('u64', 's64'):
                        rhs_final = f'(uint32_t)({rhs_sub})'
                    elif suffix == 's32':
                        rhs_final = f'(uint32_t)(int32_t)({rhs_sub})'
                    else:
                        rhs_final = rhs_sub
                else:
                    rhs_final = rhs_sub

                transformed = f'{lhs_indent}{var} = {rhs_final};'
                changed = True
            else:
                rhs_sub = _apply_reads(rhs, all_promoted)
                if rhs_sub != rhs:
                    transformed = f'{lhs_indent}ctx.{reg}.{suffix} = {rhs_sub};'
                    changed = True
        else:
            new = _apply_reads(raw, all_promoted)
            if new != raw:
                transformed = new
                changed = True

        if changed:
            stats['lines_changed'] += 1
        out.append(transformed + '\n')

    return out, stats

# ─────────────────────────────────────────────────────────────────────────────
# Phase F: Cleanup
# ─────────────────────────────────────────────────────────────────────────────

def phase_f(lines):
    stats = {'ea_removed': 0}
    ea_still_used = any(
        re.search(r'\bea\b', l) and not re.match(r'^\s*uint32_t\s+ea\s*\{', l)
        for l in lines
    )

    out = []
    prev_blank = False
    for line in lines:
        if re.match(r'^\s*uint32_t\s+ea\s*\{\s*\}\s*;\s*$', line):
            if not ea_still_used:
                stats['ea_removed'] += 1
                continue

        if line.strip() == '':
            if prev_blank:
                continue
            prev_blank = True
        else:
            prev_blank = False

        out.append(line)

    return out, stats


# ─────────────────────────────────────────────────────────────────────────────
# Phase G: Cast Expression Simplification
# ─────────────────────────────────────────────────────────────────────────────

# Pre-compiled patterns for Phase G
_RE_G1 = re.compile(r'(var_r\d+) = \(uint32_t\)\((\d+)\);')
_RE_G2 = re.compile(r'\(uint32_t\)\(\(int64_t\)\(int32_t\)(var_r\d+) \+ (-?\d+)\)')
_RE_G3 = re.compile(
    r'\(uint32_t\)\(\(int64_t\)\(int32_t\)(var_r\d+) - \(int64_t\)\(int32_t\)(var_r\d+)\)'
)
_RE_G4 = re.compile(r'\s*& 0xFFFFFFFFFFFFFFFF')
_RE_G5 = re.compile(r'\(uint32_t\)\(\(int64_t\)\(int32_t\)(var_r\d+) \+ \(int64_t\)\(int32_t\)(var_r\d+)\)')


def phase_g(lines):
    """
    Simplify verbose cast expressions generated by Phase E:
      Rule 1: var_rN = (uint32_t)(LITERAL)  →  var_rN = LITERAL
      Rule 2: (uint32_t)((int64_t)(int32_t)V + LITERAL)  →  (uint32_t)(V + LITERAL)
      Rule 3: (uint32_t)((int64_t)(int32_t)V - (int64_t)(int32_t)W)  →  V - W
      Rule 4: expr & 0xFFFFFFFFFFFFFFFF  →  expr  (no-op 64-bit mask)
      Rule 5: (uint32_t)((int64_t)(int32_t)V + (int64_t)(int32_t)W)  →  V + W
    """
    stats = {'simplified': 0}
    out = []
    for line in lines:
        orig = line
        s = line
        # Rule 1: pure literal cast on var assignment
        s, n1 = _RE_G1.subn(r'\1 = \2;', s)
        # Rule 2: (uint32_t)((i64)(i32)V + literal)
        s, n2 = _RE_G2.subn(r'(uint32_t)(\1 + \2)', s)
        # Rule 3: (uint32_t)((i64)(i32)V - (i64)(i32)W)
        s, n3 = _RE_G3.subn(r'\1 - \2', s)
        # Rule 4: no-op mask
        s, n4 = _RE_G4.subn('', s)
        # Rule 5: (uint32_t)((i64)(i32)V + (i64)(i32)W)
        s, n5 = _RE_G5.subn(r'\1 + \2', s)
        if n1 + n2 + n3 + n4 + n5:
            stats['simplified'] += n1 + n2 + n3 + n4 + n5
        out.append(s)
    return out, stats

# ─────────────────────────────────────────────────────────────────────────────
# Function-level splitter
# ─────────────────────────────────────────────────────────────────────────────

def split_into_functions(lines):
    """Split a file into (type, lines) segments where type is 'prefix', 'func', or 'suffix'.

    Handles pass3 files where a function may have imbalanced braces (e.g. sub_825340F0
    in recomp.30.cpp). When a PPC_FUNC_IMPL is encountered while already tracking a
    function, the current function is flushed and a new one starts — this is safe because
    PPC_FUNC_IMPL never legitimately nests inside another PPC_FUNC_IMPL body.
    """
    segments = []
    current = []
    in_func = False
    brace_depth = 0

    for line in lines:
        if not in_func:
            if RE_FUNC_IMPL.match(line):
                in_func = True
                brace_depth = 0
                if current:
                    segments.append(('prefix', current))
                    current = []
                current.append(line)
                # Count braces in the opening line (e.g. "PPC_FUNC_IMPL(...) {")
                brace_depth += line.count('{') - line.count('}')
            else:
                current.append(line)
        else:
            # If we encounter a new PPC_FUNC_IMPL while already in a function,
            # the previous function had imbalanced braces (pass3 bug). Flush it
            # and start tracking the new function.
            if RE_FUNC_IMPL.match(line) and brace_depth > 0:
                segments.append(('func', current))
                current = []
                brace_depth = 0
                current.append(line)
                brace_depth += line.count('{') - line.count('}')
                continue
            current.append(line)
            brace_depth += line.count('{') - line.count('}')
            if brace_depth <= 0:
                segments.append(('func', current))
                current = []
                in_func = False

    if current:
        segments.append(('suffix', current))

    return segments

# ─────────────────────────────────────────────────────────────────────────────
# Per-function pipeline
# ─────────────────────────────────────────────────────────────────────────────

def process_function(lines, enabled_phases, verbose=False, func_name='?'):
    total_stats = collections.defaultdict(int)

    if 'A' in enabled_phases:
        lines, s = phase_a(lines)
        total_stats['A_collapsed'] += s['collapsed']

    if 'B' in enabled_phases:
        lines, s = phase_b(lines)
        total_stats['B_removed'] += s['removed']

    if 'C' in enabled_phases:
        lines, s = phase_c(lines)
        total_stats['C_frames'] += s['frames_collapsed']
        total_stats['C_epilogues'] += s['epilogues_removed']

    if 'D' in enabled_phases:
        lines, s = phase_d(lines)
        total_stats['D_inlined'] += s['inlined']

    if 'E' in enabled_phases:
        lines, s = phase_e(lines)
        total_stats['E_vars'] += s['vars_promoted']
        total_stats['E_lines'] += s['lines_changed']

    if 'F' in enabled_phases:
        lines, s = phase_f(lines)
        total_stats['F_ea_removed'] += s['ea_removed']

    if 'G' in enabled_phases:
        lines, s = phase_g(lines)
        total_stats['G_simplified'] += s['simplified']

    return lines, total_stats

# ─────────────────────────────────────────────────────────────────────────────
# File processor
# ─────────────────────────────────────────────────────────────────────────────

def process_file(src_path, dst_path, enabled_phases, dry_run=False, verbose=False):
    with open(src_path, 'r', encoding='utf-8') as f:
        all_lines = f.readlines()

    segments = split_into_functions(all_lines)
    out_lines = []
    file_stats = collections.defaultdict(int)
    func_count = 0

    for seg_type, seg_lines in segments:
        if seg_type == 'func':
            fname = '?'
            m = re.search(r'__imp__(sub_[0-9A-Fa-f]+)', seg_lines[0])
            if m:
                fname = m.group(1)

            new_lines, stats = process_function(
                seg_lines, enabled_phases, verbose=verbose, func_name=fname
            )
            out_lines.extend(new_lines)
            func_count += 1
            for k, v in stats.items():
                file_stats[k] += v
        else:
            out_lines.extend(seg_lines)

    if not dry_run:
        dst_path.parent.mkdir(parents=True, exist_ok=True)
        with open(dst_path, 'w', encoding='utf-8') as f:
            f.writelines(out_lines)

    in_lines = len(all_lines)
    out_lines_count = len(out_lines)
    return func_count, in_lines, out_lines_count, file_stats

# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser(description='Stage 2: PPC boilerplate cleanup')
    ap.add_argument('--dry-run', action='store_true',
                    help='Process but do not write output files')
    ap.add_argument('--file', metavar='NAME',
                    help='Process only this filename (e.g. tt-decomp_recomp.0.cpp)')
    ap.add_argument('--no-phase', metavar='LETTERS',
                    help='Comma-separated phases to skip (e.g. E or A,B)')
    ap.add_argument('--verbose', '-v', action='store_true')
    ap.add_argument('--input-dir',  default=str(INPUT_DIR))
    ap.add_argument('--output-dir', default=str(OUTPUT_DIR))
    args = ap.parse_args()

    input_dir  = Path(args.input_dir)
    output_dir = Path(args.output_dir)

    skip = set()
    if args.no_phase:
        for s in args.no_phase.split(','):
            skip.add(s.strip().upper())
    enabled_phases = [p for p in 'ABCDEFG' if p not in skip]

    if not input_dir.exists():
        print(f"ERROR: input directory {input_dir} does not exist")
        sys.exit(1)

    cpp_files = sorted(input_dir.glob('*.cpp'))
    if args.file:
        cpp_files = [f for f in cpp_files if args.file in f.name]

    if not args.dry_run:
        output_dir.mkdir(parents=True, exist_ok=True)
        for f in input_dir.iterdir():
            if f.suffix not in ('.cpp',):
                dst = output_dir / f.name
                if not dst.exists():
                    shutil.copy2(f, dst)

    total_funcs = 0
    total_in_lines = 0
    total_out_lines = 0
    grand_stats = collections.defaultdict(int)

    for src in cpp_files:
        dst = output_dir / src.name
        fn_count, in_lines, out_lines, fstats = process_file(
            src, dst,
            enabled_phases=enabled_phases,
            dry_run=args.dry_run,
            verbose=args.verbose,
        )
        total_funcs += fn_count
        total_in_lines += in_lines
        total_out_lines += out_lines
        for k, v in fstats.items():
            grand_stats[k] += v

        delta = in_lines - out_lines
        print(f"  {src.name}: {fn_count} funcs, "
              f"{in_lines:,}→{out_lines:,} lines (-{delta:,})")

    print()
    print("=" * 60)
    print(f"Phase A  CTR collapsed    : {grand_stats['A_collapsed']:>8,}")
    print(f"Phase B  LR lines removed : {grand_stats['B_removed']:>8,}")
    print(f"Phase C  Frames collapsed : {grand_stats['C_frames']:>8,}")
    print(f"Phase C  Epilogues removed: {grand_stats['C_epilogues']:>8,}")
    print(f"Phase D  CR comparisons   : {grand_stats['D_inlined']:>8,}")
    print(f"Phase E  Vars promoted    : {grand_stats['E_vars']:>8,}")
    print(f"Phase E  Lines changed    : {grand_stats['E_lines']:>8,}")
    print(f"Phase F  ea decls removed : {grand_stats['F_ea_removed']:>8,}")
    print(f"Phase G  casts simplified : {grand_stats['G_simplified']:>8,}")
    print("─" * 60)
    print(f"Total functions           : {total_funcs:>8,}")
    print(f"Total lines in            : {total_in_lines:>8,}")
    print(f"Total lines out           : {total_out_lines:>8,}")
    reduction = total_in_lines - total_out_lines
    pct = 100.0 * reduction / total_in_lines if total_in_lines else 0
    print(f"Lines eliminated          : {reduction:>8,}  ({pct:.1f}%)")
    if args.dry_run:
        print("\n[DRY RUN — no files written]")


if __name__ == '__main__':
    main()
