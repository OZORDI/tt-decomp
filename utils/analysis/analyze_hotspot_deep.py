#!/usr/bin/env python3
"""
Deep hotspot analysis for util_8D60 and related functions.
Uses raw binary + symbol table to produce semantic analysis.
"""
import struct, sys, collections

# ── Binary & section layout ───────────────────────────────────────────────
DATA = open('orig/434C4803/pong_xenon_final.pe','rb').read()
TEXT_VA   = 0x820C0000; TEXT_RAW  = 0x4000
RDATA_VA  = 0x82000600; RDATA_RAW = 0x600; RDATA_SIZE = 0x8209B000 - 0x82000600
BSS_VA    = 0x825C0000  # approximate start of BSS/DATA globals

def ppc_lis_addi(lis_imm, addi_imm):
    """Compute PPC32 lis + addi address."""
    base = (lis_imm & 0xFFFF) << 16
    if base >= 0x80000000: base -= 0x100000000
    result = (base + addi_imm) & 0xFFFFFFFF
    return result

def va_to_raw(va):
    """Convert virtual address to raw file offset (text section only)."""
    if TEXT_VA <= va < TEXT_VA + (RDATA_VA - TEXT_VA + 0x10000000):
        return TEXT_RAW + (va - TEXT_VA)
    return None

def read_u32(va):
    off = va_to_raw(va)
    if off and off + 4 <= len(DATA):
        return struct.unpack('>I', DATA[off:off+4])[0]
    return None

def read_rdata_str(va):
    if RDATA_VA <= va < RDATA_VA + RDATA_SIZE:
        off = RDATA_RAW + (va - RDATA_VA)
        s = []
        while off < len(DATA) and data[off] != 0:
            c = DATA[off]
            if 32 <= c <= 126: s.append(chr(c))
            else: s.append(f'\\x{c:02x}')
            off += 1
        return ''.join(s)
    return None

# ── Load symbols ──────────────────────────────────────────────────────────
sym_by_addr = {}  # addr -> name
sym_by_name = {}  # name -> addr
fn8_set = set()

for fn in ['config/434C4803/symbols_autonamed_v12.txt',
           'config/434C4803/symbols.txt']:
    try:
        for L in open(fn):
            L = L.strip()
            if '= .text:0x' in L:
                name = L.split('=')[0].strip()
                va   = int(L.split(':')[1].split(';')[0].strip(), 16)
                if va not in sym_by_addr:
                    sym_by_addr[va] = name
                    sym_by_name[name] = va
                if 'fn_8' in name:
                    fn8_set.add(va)
        break
    except FileNotFoundError:
        continue

print(f"Loaded {len(sym_by_addr)} symbols, {len(fn8_set)} fn_8 remaining")

# ── Build call graph (BL edges) ────────────────────────────────────────────
text_size = 0x82586A00 - TEXT_VA
callers_of  = collections.defaultdict(set)   # target -> set of callers
callees_of  = collections.defaultdict(set)   # src -> set of targets

for i in range(0, text_size - 4, 4):
    w = struct.unpack('>I', DATA[TEXT_RAW + i: TEXT_RAW + i + 4])[0]
    opcode = (w >> 26) & 0x3F
    if opcode == 18:  # B/BL/BA/BLA
        is_link = w & 1
        if is_link:  # BL
            li = w & 0x3FFFFFC
            if li & 0x2000000: li |= ~0x3FFFFFF
            src_va  = TEXT_VA + i
            tgt_va  = (src_va + li) & 0xFFFFFFFF
            callers_of[tgt_va].add(src_va)
            callees_of[src_va].add(tgt_va)

print(f"Built call graph: {sum(len(v) for v in callers_of.values())} BL edges")

# ── Helper: print top callers/callees ─────────────────────────────────────
def describe_fn(va, title, n=20):
    print(f"\n{'='*70}")
    print(f"  {title}")
    print(f"  VA: 0x{va:08X}  name: {sym_by_addr.get(va,'???')}")
    print(f"{'='*70}")
    
    callers = callers_of.get(va, set())
    print(f"\n  CALLERS ({len(callers)} total):")
    named = [(sym_by_addr.get(c,'?'), c) for c in callers]
    named.sort()
    for nm, c in named[:n]:
        is8 = ' [fn_8]' if c in fn8_set else ''
        print(f"    0x{c:08X}  {nm}{is8}")
    if len(named) > n:
        fn8_count = sum(1 for c in callers if c in fn8_set)
        print(f"    ... ({len(named)-n} more, {fn8_count} fn_8 among all callers)")

    callees = callees_of.get(va, set())
    print(f"\n  CALLEES ({len(callees)} total):")
    named2 = [(sym_by_addr.get(c,'?'), c) for c in callees]
    named2.sort()
    for nm, c in named2[:n]:
        is8 = ' [fn_8]' if c in fn8_set else ''
        print(f"    0x{c:08X}  {nm}{is8}")

# ── Disassemble a function (simplified) ───────────────────────────────────
def disasm_fn(va, max_bytes=512):
    """Print key instructions with resolved addresses."""
    raw = TEXT_RAW + (va - TEXT_VA)
    lines = []
    i = 0
    while i < max_bytes:
        if raw + i + 4 > len(DATA): break
        w = struct.unpack('>I', DATA[raw+i:raw+i+4])[0]
        cur_va = va + i
        
        opcode = (w >> 26) & 0x3F
        
        # BL
        if opcode == 18 and (w & 1):
            li = w & 0x3FFFFFC
            if li & 0x2000000: li |= ~0x3FFFFFF
            tgt = (cur_va + li) & 0xFFFFFFFF
            tgt_name = sym_by_addr.get(tgt, f'0x{tgt:08X}')
            lines.append(f"  0x{cur_va:08X}  bl   {tgt_name}")
        # BLR
        elif w == 0x4E800020:
            lines.append(f"  0x{cur_va:08X}  blr")
        # B (branch to known sym)
        elif opcode == 18 and not (w & 1):
            li = w & 0x3FFFFFC
            if li & 0x2000000: li |= ~0x3FFFFFF
            tgt = (cur_va + li) & 0xFFFFFFFF
            tgt_name = sym_by_addr.get(tgt, f'0x{tgt:08X}')
            lines.append(f"  0x{cur_va:08X}  b    {tgt_name}")
            if tgt != cur_va + 4:  # real branch
                break
        # MFLR r12
        elif w == 0x7D8802A6:
            lines.append(f"  0x{cur_va:08X}  mflr r12  <<PROLOGUE>>")
        # LIS
        elif opcode == 15:
            rd = (w >> 21) & 0x1F
            imm = w & 0xFFFF
            if imm >= 0x8000: imm -= 0x10000
            lines.append(f"  0x{cur_va:08X}  lis  r{rd}, 0x{(imm&0xFFFF):04X}  ; r{rd} = 0x{((imm&0xFFFF)<<16)&0xFFFFFFFF:08X}")
        # LWZ load
        elif opcode == 32:
            rd = (w>>21)&0x1F; ra = (w>>16)&0x1F; d = w & 0xFFFF
            if d >= 0x8000: d -= 0x10000
            lines.append(f"  0x{cur_va:08X}  lwz  r{rd}, {d}(r{ra})")
        # STW store  
        elif opcode == 36:
            rs = (w>>21)&0x1F; ra = (w>>16)&0x1F; d = w & 0xFFFF
            if d >= 0x8000: d -= 0x10000
            lines.append(f"  0x{cur_va:08X}  stw  r{rs}, {d}(r{ra})")
        # STWU
        elif opcode == 37:
            rs = (w>>21)&0x1F; ra = (w>>16)&0x1F; d = w & 0xFFFF
            if d >= 0x8000: d -= 0x10000
            if rs == 1 and ra == 1:
                lines.append(f"  0x{cur_va:08X}  stwu r1, {d}(r1)  ; frame={-d}")
        # BCTR
        elif w & 0xFC00FFFF == 0x4C000420:
            lines.append(f"  0x{cur_va:08X}  bctr  <<virtual call>>")
        i += 4
        # Stop at second BLR
        if w == 0x4E800020 and i > 8:
            break
    return lines

# ── Analysis targets ───────────────────────────────────────────────────────
HOTSPOTS = {
    'util_8D60':                       0x82238D60,
    'atSingleton_Find_90D0':           0x820F90D0,
    'PongNetGameModeCoordinator_vfn_6':0x8239ECA8,
}

# ── util_8D60 deep dive ────────────────────────────────────────────────────
print("\n" + "█"*70)
print("  DEEP ANALYSIS: util_8D60  (0x82238D60)")
print("  The #1 hotspot — 99 fn_8 connections")
print("█"*70)

util_va = 0x82238D60
callers  = list(callers_of.get(util_va, set()))

print(f"\n  Total direct callers: {len(callers)}")
fn8_callers  = [c for c in callers if c in fn8_set]
named_callers = [c for c in callers if c not in fn8_set]
print(f"  - fn_8 callers:  {len(fn8_callers)}")
print(f"  - named callers: {len(named_callers)}")

# Class distribution of named callers
class_votes = collections.Counter()
for c in named_callers:
    nm = sym_by_addr.get(c,'?')
    parts = nm.split('_')
    if len(parts) > 0:
        class_votes[parts[0]] += 1

print(f"\n  Named caller class distribution (top 10):")
for cls, cnt in class_votes.most_common(10):
    print(f"    {cls:30s}  {cnt}")

# What does util_8D60 call?
callees = callees_of.get(util_va, set())
print(f"\n  Callees of util_8D60: {len(callees)}")
for c in sorted(callees):
    print(f"    0x{c:08X}  {sym_by_addr.get(c,'???')}")

# Show assembly at the actual function start (where mflr is)
print(f"\n  KEY INSTRUCTIONS at 0x82238DE8 (actual prologue):")
for line in disasm_fn(0x82238DE8, max_bytes=160):
    print(line)

# What are the first 4 bytes at 0x82238D60?
raw8 = DATA[TEXT_RAW + (util_va - TEXT_VA): TEXT_RAW + (util_va - TEXT_VA) + 16]
print(f"\n  First 16 bytes at 0x82238D60: {raw8.hex()}")
w0 = struct.unpack('>I', raw8[:4])[0]
print(f"  First instruction word: 0x{w0:08X} — opcode {(w0>>26)&0x3F}")

# ── atSingleton_Find deep dive ─────────────────────────────────────────────
print("\n" + "█"*70)
print("  DEEP ANALYSIS: atSingleton_Find_90D0  (0x820F90D0)")
print("█"*70)

at_va = 0x820F90D0
callers_at = list(callers_of.get(at_va, set()))
fn8_at = [c for c in callers_at if c in fn8_set]
named_at = [c for c in callers_at if c not in fn8_set]
print(f"\n  Total direct callers: {len(callers_at)}")
print(f"  - fn_8 callers:  {len(fn8_at)}")
print(f"  - named callers: {len(named_at)}")
print(f"\n  fn_8 callers (first 20):")
for c in sorted(fn8_at)[:20]:
    print(f"    0x{c:08X}  {sym_by_addr.get(c,'fn_8')}")

# Read actual bytes at 0x820F90D0 to see if this is code or data  
raw_at = DATA[TEXT_RAW + (at_va - TEXT_VA): TEXT_RAW + (at_va - TEXT_VA) + 16]
print(f"\n  First 16 bytes at 0x820F90D0: {raw_at.hex()}")
# Decode as string: is this ASCII?
as_ascii = ''.join(chr(b) if 32<=b<=126 else '.' for b in raw_at)
print(f"  As ASCII: '{as_ascii}'")
# This might be an error string embedded in a function

# Look for the actual function — it may be that the symbol points past a data blob
# The real function entry may be found by scanning for mflr (7D8802A6)
print(f"\n  Scanning for mflr pattern near 0x820F90D0...")
for offset in range(-32, 256, 4):
    check_va = at_va + offset
    if check_va < TEXT_VA: continue
    raw_check = TEXT_RAW + (check_va - TEXT_VA)
    if raw_check + 4 > len(DATA): break
    w = struct.unpack('>I', DATA[raw_check:raw_check+4])[0]
    if w == 0x7D8802A6:  # mflr r12
        print(f"    mflr r12 found at 0x{check_va:08X} (offset {offset:+d})")

# ── PongNetGameModeCoordinator deep dive ─────────────────────────────────
print("\n" + "█"*70)
print("  DEEP ANALYSIS: PongNetGameModeCoordinator_vfn_6  (0x8239ECA8)")
print("█"*70)

pong_va = 0x8239ECA8
callers_p = list(callers_of.get(pong_va, set()))
fn8_p = [c for c in callers_p if c in fn8_set]
named_p = [c for c in callers_p if c not in fn8_set]
print(f"\n  Total direct callers: {len(callers_p)}")
print(f"  - fn_8 callers:  {len(fn8_p)}")

# Functions called by the vfn_6 region (0x8239ECA8 to 0x8239ECA8 + 0x964)
pong_end = pong_va + 0x964
pong_callees = {}
for c_va in callees_of:
    if pong_va <= c_va < pong_end:
        for tgt in callees_of[c_va]:
            nm = sym_by_addr.get(tgt, f'0x{tgt:08X}')
            pong_callees[tgt] = nm

print(f"\n  Functions called within vfn_6 region:")
for tgt_va, nm in sorted(pong_callees.items(), key=lambda x: x[1]):
    is8 = ' [fn_8]' if tgt_va in fn8_set else ''
    print(f"    0x{tgt_va:08X}  {nm}{is8}")

# ── Hotspot fn_8 callers class analysis ──────────────────────────────────
print("\n" + "█"*70)
print("  NAMING OPPORTUNITY: fn_8 callers of util_8D60 by class signal")
print("█"*70)

# For each fn_8 caller of util_8D60, look at THEIR callers/callees 
# to find the best class vote
fn8_with_class = []
for c in fn8_callers:
    votes = collections.Counter()
    for nb in list(callers_of.get(c,set())) + list(callees_of.get(c,set())):
        nm = sym_by_addr.get(nb,'')
        if nm and 'fn_8' not in nm and not nm.startswith('0x'):
            parts = nm.split('_')
            if len(parts) > 0 and len(parts[0]) > 3:
                votes[parts[0]] += 1
    if votes:
        best_class, best_score = votes.most_common(1)[0]
        fn8_with_class.append((best_score, c, best_class, dict(votes)))

fn8_with_class.sort(reverse=True)

print(f"\n  Top 30 fn_8 callers of util_8D60 with class signal:")
print(f"  {'Address':12s}  {'Score':6s}  {'Class':35s}  {'Other classes'}")
for score, va, cls, all_votes in fn8_with_class[:30]:
    others = {k:v for k,v in all_votes.items() if k != cls}
    other_str = ', '.join(f'{k}:{v}' for k,v in sorted(others.items(),key=lambda x:-x[1])[:3])
    print(f"  0x{va:08X}  {score:6.1f}  {cls:35s}  {other_str}")

print(f"\n  fn_8 callers with no class signal: {len([x for x in fn8_callers if x not in [v[1] for v in fn8_with_class]])}")

# ── Summary of best naming candidates ─────────────────────────────────────
print("\n" + "█"*70)
print("  NAMING CANDIDATES: Top fn_8 to name based on hotspot analysis")
print("█"*70)

# Functions with score >= 2 and dominant single class
candidates = [(s,v,c,d) for s,v,c,d in fn8_with_class 
              if s >= 2 and sum(d.values()) > 0 
              and s / max(sum(d.values()), 1) >= 0.6]

print(f"\n  Nameable candidates (score >= 2, ratio >= 0.6): {len(candidates)}")
for score, va, cls, all_votes in candidates[:40]:
    total = sum(all_votes.values())
    ratio = score / total if total > 0 else 0
    callees_count = len(callees_of.get(va, set()))
    print(f"  0x{va:08X}  {cls}_{va&0xFFFF:04X}  score={score:.1f}/{total:.1f} ratio={ratio:.2f}  callees={callees_count}")

print(f"\nAnalysis complete.")
