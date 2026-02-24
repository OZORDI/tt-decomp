import os, re
from collections import defaultdict, Counter

BASE = '/Users/Ozordi/Downloads/tt-decomp'
XEX = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v12.txt')

print("Loading binary...")
data = open(XEX, 'rb').read()
e_lfanew = int.from_bytes(data[0x3C:0x40], 'little')
coff_off = e_lfanew + 4
opt_off = coff_off + 20
image_base = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18], 'little')
num_sects = int.from_bytes(data[coff_off+2:coff_off+4], 'little')
sect_tbl = opt_off + opt_hdr_size
sections = []
for i in range(num_sects):
    s = data[sect_tbl+i*40 : sect_tbl+i*40+40]
    sname = s[:8].rstrip(b'\x00').decode('ascii', 'replace')
    vsize = int.from_bytes(s[8:12], 'little')
    vaddr = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((sname, image_base + vaddr, vsize, raw_off))

text_sec = next(s for s in sections if s[0] == '.text')
rdata_sec = next(s for s in sections if s[0] == '.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def va_to_file(va):
    for _, abs_va, vsize, raw_off in sections:
        if abs_va <= va < abs_va + vsize:
            return raw_off + (va - abs_va)
    return None

def read_be32_rdata(va):
    off = rraw + (va - rvaddr)
    if 0 <= off < rvsize - 3:
        return int.from_bytes(data[off:off+4], 'big')
    return None

print("Loading symbols...")
sym_map = {}
fn_meta = {}
with open(SYMS) as f:
    for line in f:
        s = line.strip()
        if '= .text:0x' in s:
            parts = s.split('=', 1)
            name = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                sym_map[addr] = name
                m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                if m: fn_meta[addr] = int(m.group(1), 16)
            except:
                pass

fn_addrs = sorted(a for a in fn_meta if tvaddr <= a < tvaddr + tvsize)
fn_set = set(fn_addrs)
unnamed = {a for a in fn_addrs if sym_map[a].startswith('fn_8')}
print(f"Total fns: {len(fn_addrs)}, fn_8: {len(unnamed)}")

import bisect
fn_offsets = [a - tvaddr for a in fn_addrs]
def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr + fn_offsets[idx]) if idx >= 0 else None

print("Building callgraphs...")
forward_cg = defaultdict(set)
reverse_cg = defaultdict(set)
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4], 'big')
    if (w >> 26) & 0x3F == 18 and (w & 1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va = tvaddr + i + li
        if caller_va and tgt_va in fn_set:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)

fn_ptr_fwd = defaultdict(set)
fn_ptr_rev = defaultdict(set)
pending = {}
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op = (w >> 26) & 0x3F; rd = (w >> 21) & 0x1F
    ra = (w >> 16) & 0x1F; imm = w & 0xFFFF
    if op == 15 and ra == 0:
        signed_hi = imm if imm < 0x8000 else imm - 0x10000
        pending[rd] = signed_hi << 16
    elif op == 14 and ra in pending:
        lo = imm if imm < 0x8000 else imm - 0x10000
        addr = (pending[ra] + lo) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr + tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr); fn_ptr_rev[addr].add(caller)
    elif op == 24 and ra in pending:
        addr = (pending[ra] | imm) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr + tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr); fn_ptr_rev[addr].add(caller)
    else:
        if rd in pending and op not in (15, 14, 24):
            pending.pop(rd, None)

def all_edges(fn_va):
    return (forward_cg.get(fn_va, set()) | reverse_cg.get(fn_va, set()) |
            fn_ptr_fwd.get(fn_va, set()) | fn_ptr_rev.get(fn_va, set()))

# Identifying isolated dead code
dead_code_candidates = []
for fn_va in unnamed:
    edges = all_edges(fn_va)
    if not edges: # No BL or fn_ptr in/out edges
        dead_code_candidates.append(fn_va)

# Further filter: remove ones that are in rdata themselves (vtable callbacks?)
# We already parsed vtables using RTTI, but we can do a naive check.
rdata_refs = set()
for off in range(0, rvsize, 4):
    val = int.from_bytes(data[rraw+off:rraw+off+4], 'big')
    if tvaddr <= val < tvaddr + tvsize and val in fn_set:
        rdata_refs.add(val)
        
true_dead_code = [va for va in dead_code_candidates if va not in rdata_refs]

print(f"\n--- Dead Code Analysis ---")
print(f"Functions with 0 edges in call graph: {len(dead_code_candidates)}")
print(f"Functions with 0 edges AND NOT in .rdata: {len(true_dead_code)}")


print(f"\n--- Hotspot Analysis ---")
# Hotspot: A named function or generic function that is called by or calls many fn_8s
# This gives us a "central orientation"
hotspots = Counter()
for fn_va in unnamed:
    if fn_va in true_dead_code: continue
    
    for c in forward_cg.get(fn_va, set()):
        if c not in unnamed: hotspots[sym_map[c]] += 1
    for c in reverse_cg.get(fn_va, set()):
        if c not in unnamed: hotspots[sym_map[c]] += 1

print("Top Named Neighbors of Remaining fn_8 (Central Orientation Hotspots):")
for name, cnt in hotspots.most_common(20):
    print(f"  {name:40s} : {cnt} remaining fn_8 connections")

with open("dead_code_analysis.txt", "w") as f:
    f.write(f"True Dead Code Candidates ({len(true_dead_code)} functions)\n")
    for va in sorted(true_dead_code):
        f.write(f"0x{va:08X} size: 0x{fn_meta[va]:X}\n")
    
    f.write(f"\nHotspots (Named Neighbors connected to fn_8)\n")
    for name, cnt in hotspots.most_common(50):
        f.write(f"{name:40s} : {cnt}\n")
