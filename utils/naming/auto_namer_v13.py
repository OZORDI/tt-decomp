#!/usr/bin/env python3
"""
auto_namer_v13 - Hotspot analysis naming pass
Targets:
  P21: Rename util_8D60 -> xam_singleton_init_8D60
  P22: Name 99 isolated xam_static_init wrappers (callers of util_8D60)
  P23: Name 8 PongNetGameModeCoordinator fn_8 callees
  P24: BFS from newly named fns, class-vote remaining fn_8s
"""
import struct, bisect, collections, re, shutil
from pathlib import Path

BASE = '/Users/Ozordi/Downloads/tt-decomp'
SYM_IN  = f'{BASE}/config/434C4803/symbols_autonamed_v12.txt'
SYM_OUT = f'{BASE}/config/434C4803/symbols_autonamed_v13.txt'
XEX     = f'{BASE}/orig/434C4803/pong_xenon_final.pe'

data = open(XEX,'rb').read()
e_lfanew = int.from_bytes(data[0x3C:0x40], 'little')
coff_off = e_lfanew + 4; opt_off = coff_off + 20
image_base = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18], 'little')
num_sects = int.from_bytes(data[coff_off+2:coff_off+4], 'little')
sect_tbl = opt_off + opt_hdr_size
sections = []
for i in range(num_sects):
    s = data[sect_tbl+i*40:sect_tbl+i*40+40]
    sname = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize=int.from_bytes(s[8:12],'little'); vaddr=int.from_bytes(s[12:16],'little'); raw_off=int.from_bytes(s[20:24],'little')
    sections.append((sname, image_base+vaddr, vsize, raw_off))
text_sec = next(s for s in sections if s[0]=='.text')
_, tvaddr, tvsize, traw = text_sec

sym_map = {}; fn_meta = {}; original_lines = []
with open(SYM_IN) as f:
    for L in f:
        original_lines.append(L)
        if '= .text:0x' in L:
            name = L.split('=')[0].strip()
            addr = int(L.split(':')[1].split(';')[0].strip(), 16)
            m = re.search(r'size:0x([0-9A-Fa-f]+)', L)
            sym_map[addr] = name
            if m: fn_meta[addr] = int(m.group(1), 16)

fn_addrs = sorted(fn_meta.keys()); fn_set = set(fn_addrs)
unnamed = {a for a in fn_addrs if sym_map[a].startswith('fn_8')}
fn_offsets = [a - tvaddr for a in fn_addrs]
print(f"Input: {len(sym_map)} symbols, {len(unnamed)} fn_8")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr + fn_offsets[idx]) if idx >= 0 else None

forward_cg = collections.defaultdict(set); reverse_cg = collections.defaultdict(set)
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    if (w >> 26) & 0x3F == 18 and (w & 1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i); tgt_va = tvaddr + i + li
        if caller_va and tgt_va in fn_set:
            forward_cg[caller_va].add(tgt_va); reverse_cg[tgt_va].add(caller_va)

renames = {}  # addr -> new_name

# ── P21: Rename util_8D60 ──────────────────────────────────────────────────
UTIL_VA = 0x82238D60
# It's a singleton init that: checks flag at 0x8271B2A0, inits struct at 0x8271A860, calls atexit
# Named callers: SinglesNetworkClient (3x), pongNetMessageHolder (1x)
# → xam_singleton_init since it's used by the xam-region stubs
renames[UTIL_VA] = 'xam_singleton_init_8D60'
print(f"P21: util_8D60 → xam_singleton_init_8D60")

# ── P22: 99 xam_static_init wrappers ─────────────────────────────────────
fn8_callers_util = sorted(c for c in reverse_cg.get(UTIL_VA, set()) if c in unnamed)
p22_count = 0
for va in fn8_callers_util:
    new_name = f'xam_static_init_{va & 0xFFFF:04X}'
    renames[va] = new_name
    p22_count += 1
print(f"P22: {p22_count} xam_static_init wrappers named")

# ── P23: PongNetGameModeCoordinator fn_8 callees ─────────────────────────
PONG_VA = 0x8239ECA8; PONG_SZ = fn_meta.get(PONG_VA, 0x964)
pong_end = PONG_VA + PONG_SZ
pong_fn8_callees = {}
for src in forward_cg:
    if PONG_VA <= src < pong_end:
        for tgt in forward_cg[src]:
            if tgt in unnamed and tgt not in renames:
                # vote on class from their other neighbors
                v = collections.Counter()
                for nb in set(forward_cg.get(tgt,set())) | set(reverse_cg.get(tgt,set())):
                    nm = sym_map.get(nb,'')
                    if nm and not nm.startswith('fn_8') and '_' in nm:
                        cls = nm.split('_')[0]
                        if len(cls) > 3: v[cls] += 1
                best = v.most_common(1)[0] if v else None
                pong_fn8_callees[tgt] = best

p23_count = 0
for tgt, best in sorted(pong_fn8_callees.items()):
    if best:
        cls, score = best
        new_name = f'{cls}_{tgt & 0xFFFF:04X}_p23'
        renames[tgt] = new_name
        p23_count += 1
        print(f"  P23: 0x{tgt:08X} → {new_name}  (score={score})")
    else:
        # No class votes - still name as PongNetGameModeCoordinator based on position
        new_name = f'PongNetGameModeCoordinator_{tgt & 0xFFFF:04X}_p23'
        renames[tgt] = new_name
        p23_count += 1
        print(f"  P23: 0x{tgt:08X} → {new_name}  (proximity only)")
print(f"P23: {p23_count} PongNetGameModeCoordinator callees named")

# ── P24: BFS class vote pass - remaining fn_8 with clear signals ─────────
def get_class_votes(va, exclude_set=None):
    v = collections.Counter()
    for nb in set(forward_cg.get(va,set())) | set(reverse_cg.get(va,set())):
        nm = sym_map.get(nb,'') or (renames.get(nb,'') if nb in renames else '')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls) > 3 and cls not in ('xam','util','game','rage','nop','ref',
                                              'jumptable','memcpy','memset','atexit'):
                v[cls] += 1
    return v

p24_count = 0
remaining = unnamed - set(renames.keys())
# Multiple rounds of BFS, refreshing renames each round
for round_num in range(4):
    new_this_round = 0
    for va in sorted(remaining):
        if va in renames: continue
        v = get_class_votes(va)
        if not v: continue
        best_cls, best_score = v.most_common(1)[0]
        total = sum(v.values())
        # Require clear majority
        if best_score >= 2 and best_score / total >= 0.7:
            new_name = f'{best_cls}_{va & 0xFFFF:04X}_p24'
            renames[va] = new_name
            p24_count += 1
            new_this_round += 1
            # Update sym_map for BFS propagation
            sym_map[va] = new_name
    print(f"  P24 round {round_num+1}: {new_this_round} new renames")
    if new_this_round == 0: break
print(f"P24 total: {p24_count}")

# ── P25: Special cases - atSingleton callers by secondary class ───────────
AT9420_VA = next((k for k,v in sym_map.items() if v == 'atSingleton_9420'), None)
if AT9420_VA:
    fn8_c = [c for c in reverse_cg.get(AT9420_VA, set()) if c in unnamed and c not in renames]
    for va in sorted(fn8_c):
        v = get_class_votes(va)
        if v:
            best_cls, best_score = v.most_common(1)[0]
            if best_score >= 1:
                new_name = f'{best_cls}_{va & 0xFFFF:04X}_p25'
                renames[va] = new_name
                sym_map[va] = new_name

# ── cmSampleCamActions callers near util_5380 ─────────────────────────────
for va in [0x82271B60, 0x82272030]:
    if va in unnamed and va not in renames:
        v = get_class_votes(va)
        if v:
            best_cls, best_score = v.most_common(1)[0]
            new_name = f'{best_cls}_{va & 0xFFFF:04X}_p25'
            renames[va] = new_name
            sym_map[va] = new_name

# ── Additional P24 round after all above ──────────────────────────────────
remaining = unnamed - set(renames.keys())
extra = 0
for va in sorted(remaining):
    v = get_class_votes(va)
    if not v: continue
    best_cls, best_score = v.most_common(1)[0]
    total = sum(v.values())
    if best_score >= 2 and best_score / total >= 0.65:
        new_name = f'{best_cls}_{va & 0xFFFF:04X}_p24b'
        renames[va] = new_name
        sym_map[va] = new_name
        extra += 1
print(f"P24b extra: {extra}")

# ── Summary ────────────────────────────────────────────────────────────────
print(f"\nTotal renames: {len(renames)}")
fn8_fixed = sum(1 for va in renames if va in unnamed)
print(f"fn_8 fixed: {fn8_fixed}")
print(f"Remaining fn_8: {len(unnamed) - fn8_fixed}")

# ── Write output symbols file ─────────────────────────────────────────────
shutil.copy(SYM_IN, SYM_IN + '.bak_v12')
lines_out = []
for L in original_lines:
    if '= .text:0x' in L:
        try:
            addr = int(L.split(':')[1].split(';')[0].strip(), 16)
            if addr in renames:
                old_name = L.split('=')[0].strip()
                new_name = renames[addr]
                L = L.replace(old_name + ' =', new_name + ' =', 1)
        except: pass
    lines_out.append(L)

with open(SYM_OUT, 'w') as f:
    f.writelines(lines_out)
print(f"\nWrote {SYM_OUT}")

# Coverage calc
total_fns = len(fn_meta)
remaining_fn8 = len(unnamed) - fn8_fixed
named_count = total_fns - remaining_fn8
old_named = total_fns - len(unnamed)
print(f"Coverage: {old_named/total_fns*100:.2f}% → {named_count/total_fns*100:.2f}%  (+{fn8_fixed/total_fns*100:.2f}%)")
