#!/usr/bin/env python3
"""auto_namer_v17 — final push: permissive spatial w=25 on remaining 162 fn_8"""
import bisect, collections, re, shutil, struct

BASE   = '/Users/Ozordi/Downloads/tt-decomp'
SYM_IN = f'{BASE}/config/434C4803/symbols_autonamed_v16.txt'
SYM_OUT= f'{BASE}/config/434C4803/symbols_autonamed_v17.txt'
XEX    = f'{BASE}/orig/434C4803/pong_xenon_final.pe'

data = open(XEX,'rb').read()
e_lfanew = int.from_bytes(data[0x3C:0x40],'little')
coff_off = e_lfanew+4; opt_off = coff_off+20
image_base = int.from_bytes(data[opt_off+28:opt_off+32],'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18],'little')
num_sects = int.from_bytes(data[coff_off+2:coff_off+4],'little')
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
            addr = int(L.split(':')[1].split(';')[0].strip(),16)
            m = re.search(r'size:0x([0-9A-Fa-f]+)', L)
            sym_map[addr] = name
            if m: fn_meta[addr] = int(m.group(1),16)

fn_addrs = sorted(fn_meta.keys()); fn_set = set(fn_addrs)
unnamed = {a for a in fn_addrs if sym_map[a].startswith('fn_8')}
fn_offsets = [a - tvaddr for a in fn_addrs]
print(f"Input: {len(sym_map)} symbols, {len(unnamed)} fn_8  → {(len(sym_map)-len(unnamed))/len(sym_map)*100:.2f}%")

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

SKIP = {'xam','util','game','rage','nop','ref','jumptable','memcpy','memset',
        'atexit','Rtl','fn','thunk','ke','xe','net','snr','sn'}

def spatial_class(va, window=25):
    idx = bisect.bisect_left(fn_offsets, va - tvaddr)
    v = collections.Counter()
    for j in range(max(0,idx-window), min(len(fn_addrs),idx+window+1)):
        nm = sym_map.get(fn_addrs[j],'')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls)>3 and cls not in SKIP: v[cls]+=1
    return v

renames = {}

# ── P45: Permissive spatial w=25, best-class ≥ 3 votes, ratio ≥ 35% ─────
p45 = 0
for rnd in range(8):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        sp = spatial_class(va, 25)
        if not sp: continue
        best_cls, best_cnt = sp.most_common(1)[0]
        total = sum(sp.values())
        if best_cnt >= 3 and best_cnt/total >= 0.35:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p45'
            sym_map[va] = renames[va]
            p45 += 1; added += 1
    print(f"  P45 round {rnd+1}: {added}")
    if added == 0: break
print(f"P45 spatial w=25/35%: {p45}")

# ── P46: Winner-takes-all for anything with ≥ 2 votes ────────────────────
p46 = 0
for va in sorted(unnamed):
    if va in renames: continue
    sp = spatial_class(va, 30)
    if not sp: continue
    best_cls, best_cnt = sp.most_common(1)[0]
    if best_cnt >= 2:
        renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p46'
        sym_map[va] = renames[va]
        p46 += 1
print(f"P46 winner-takes-all ≥2 votes: {p46}")

# ── P47: CG propagation one final round ───────────────────────────────────
p47 = 0
for rnd in range(6):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        v = collections.Counter()
        for nb in set(forward_cg.get(va,set())) | set(reverse_cg.get(va,set())):
            nm = sym_map.get(nb,'')
            if nm and not nm.startswith('fn_8') and '_' in nm:
                cls = nm.split('_')[0]
                if len(cls)>3 and cls not in SKIP: v[cls]+=1
        if not v: continue
        best_cls, best_score = v.most_common(1)[0]
        total = sum(v.values())
        if best_score >= 1 and best_score/total >= 0.50:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p47'
            sym_map[va] = renames[va]
            p47 += 1; added += 1
    print(f"  P47 CG round {rnd+1}: {added}")
    if added == 0: break
print(f"P47 CG final: {p47}")

# ── Summary ───────────────────────────────────────────────────────────────
fn8_fixed = sum(1 for va in renames if va in unnamed)
remaining = len(unnamed) - fn8_fixed
cov = (len(sym_map) - remaining) / len(sym_map) * 100
print(f"\nTotal renames: {len(renames)}, fn_8 fixed: {fn8_fixed}")
print(f"Remaining fn_8: {remaining}")
print(f"Coverage: {cov:.2f}%")

# show remaining
still = sorted(va for va in unnamed if va not in renames)
print(f"\nStill unnamed ({len(still)}):")
for va in still:
    sz = fn_meta.get(va,0)
    sp = spatial_class(va, 30)
    print(f"  0x{va:08X}  sz=0x{sz:X}  sp={sp.most_common(2)}")

# ── Write ─────────────────────────────────────────────────────────────────
shutil.copy(SYM_IN, SYM_IN + '.bak_v16')
lines_out = []
for L in original_lines:
    if '= .text:0x' in L:
        try:
            addr = int(L.split(':')[1].split(';')[0].strip(),16)
            if addr in renames:
                old = L.split('=')[0].strip()
                L = L.replace(old + ' =', renames[addr] + ' =', 1)
        except: pass
    lines_out.append(L)
with open(SYM_OUT,'w') as f: f.writelines(lines_out)
print(f"\nWrote {SYM_OUT}")
