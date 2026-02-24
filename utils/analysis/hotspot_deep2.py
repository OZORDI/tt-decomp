import struct, bisect, collections, re

BASE = '/Users/Ozordi/Downloads/tt-decomp'
data = open(f'{BASE}/orig/434C4803/pong_xenon_final.pe','rb').read()
e_lfanew = int.from_bytes(data[0x3C:0x40], 'little')
coff_off = e_lfanew + 4
opt_off = coff_off + 20
image_base = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18], 'little')
num_sects = int.from_bytes(data[coff_off+2:coff_off+4], 'little')
sect_tbl = opt_off + opt_hdr_size
sections = []
for i in range(num_sects):
    s = data[sect_tbl+i*40:sect_tbl+i*40+40]
    sname = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize = int.from_bytes(s[8:12],'little')
    vaddr = int.from_bytes(s[12:16],'little')
    raw_off = int.from_bytes(s[20:24],'little')
    sections.append((sname, image_base+vaddr, vsize, raw_off))

text_sec  = next(s for s in sections if s[0]=='.text')
rdata_sec = next(s for s in sections if s[0]=='.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def read_str(va):
    if rvaddr <= va < rvaddr + rvsize:
        off = rraw + (va - rvaddr)
        s = []
        while off < len(data) and data[off] and len(s) < 64:
            c = data[off]
            s.append(chr(c) if 32 <= c <= 126 else '?')
            off += 1
        return ''.join(s)
    return None

sym_map = {}
fn_meta = {}
for L in open(f'{BASE}/config/434C4803/symbols_autonamed_v12.txt'):
    if '= .text:0x' in L:
        name = L.split('=')[0].strip()
        addr = int(L.split(':')[1].split(';')[0].strip(), 16)
        m = re.search(r'size:0x([0-9A-Fa-f]+)', L)
        sym_map[addr] = name
        if m: fn_meta[addr] = int(m.group(1), 16)

fn_addrs   = sorted(fn_meta.keys())
fn_set     = set(fn_addrs)
unnamed    = {a for a in fn_addrs if sym_map[a].startswith('fn_8')}
fn_offsets = [a - tvaddr for a in fn_addrs]
print(f"Symbols: {len(sym_map)}, fn_8: {len(unnamed)}")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr + fn_offsets[idx]) if idx >= 0 else None

forward_cg = collections.defaultdict(set)
reverse_cg = collections.defaultdict(set)
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    if (w >> 26) & 0x3F == 18 and (w & 1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va = tvaddr + i + li
        if caller_va and tgt_va in fn_set:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
print(f"Call graph: {sum(len(v) for v in forward_cg.values())} edges")

def class_votes_for(va):
    v = collections.Counter()
    for nb in list(forward_cg.get(va,set())) | list(reverse_cg.get(va,set())):
        nm = sym_map.get(nb,'')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls) > 3: v[cls] += 1
    return v

def disasm_key(va, nbytes=200):
    raw = traw + (va - tvaddr)
    lines = []
    regs = {}
    for k in range(0, nbytes, 4):
        w = int.from_bytes(data[raw+k:raw+k+4], 'big')
        cur = va + k
        op = (w>>26)&0x3F; rd = (w>>21)&0x1F; ra = (w>>16)&0x1F
        imm = w & 0xFFFF
        if imm >= 0x8000: imm -= 0x10000
        note = ''
        if w == 0x7D8802A6: note = 'ENTRY mflr r12'
        elif w == 0x4E800020: note = 'RETURN blr'
        elif op == 18:
            li = w & 0x3FFFFFC
            if li & 0x2000000: li |= ~0x3FFFFFF
            tgt = (cur + li) & 0xFFFFFFFF
            nm = sym_map.get(tgt, '0x{:08X}'.format(tgt))
            note = ('BL ' if w&1 else 'B  ') + nm
        elif op == 15 and ra == 0:
            regs[rd] = imm << 16
            note = 'lis r{} = 0x{:08X}'.format(rd, (imm<<16)&0xFFFFFFFF)
        elif op == 14 and ra in regs:
            val = (regs[ra] + imm) & 0xFFFFFFFF
            regs[rd] = val
            s = read_str(val)
            note = 'addi r{} = 0x{:08X}{}'.format(rd, val, ('  STR:"'+s+'"') if s else '')
        elif op == 32:
            note = 'lwz r{}, {}(r{})'.format(rd, imm, ra)
        elif (w & 0xFC00FFFF) == 0x4C000420:
            note = 'BCTR (virtual call)'
        if note: lines.append('  0x{:08X}  {}'.format(cur, note))
        if w == 0x4E800020 and k > 8: break
    return lines

# ─── util_8D60 ───────────────────────────────────────────────────────────
UTIL = 0x82238D60
print("\n" + "="*70)
print(f"  util_8D60 @ 0x{UTIL:08X}  size=0x{fn_meta.get(UTIL,0x94):X}")
print("="*70)
util_callers = reverse_cg.get(UTIL, set())
util_callees = forward_cg.get(UTIL, set())
fn8_c = sorted(c for c in util_callers if c in unnamed)
named_c = sorted(c for c in util_callers if c not in unnamed)
print(f"Callers: {len(util_callers)} ({len(fn8_c)} fn_8, {len(named_c)} named)")
print(f"Callees: {len(util_callees)}")
for c in sorted(util_callees):
    print(f"  callee 0x{c:08X}  {sym_map.get(c,'?')[:60]}")
fn8_via = set(fn8 for fn8 in unnamed if UTIL in forward_cg.get(fn8,set()) | reverse_cg.get(fn8,set()))
print(f"\nfn_8 neighbors of util_8D60: {len(fn8_via)}")
v = collections.Counter()
for fn8 in fn8_via:
    for cls, cnt in class_votes_for(fn8).items(): v[cls] += cnt
print("Class votes among those fn_8:")
for cls, cnt in v.most_common(15): print(f"  {cls:35s} {cnt}")
print("\nDisassembly (key instructions):")
for line in disasm_key(UTIL, fn_meta.get(UTIL,0x94)): print(line)

# ─── atSingleton_Find_90D0 ───────────────────────────────────────────────
AT_VA = 0x820F90D0
print("\n" + "="*70)
print(f"  atSingleton_Find_90D0 @ 0x{AT_VA:08X}  size=0x{fn_meta.get(AT_VA,0xA4):X}")
print("="*70)
at_callers = reverse_cg.get(AT_VA, set())
at_fn8 = sorted(c for c in at_callers if c in unnamed)
print(f"Callers: {len(at_callers)} ({len(at_fn8)} fn_8)")
raw_at = traw + (AT_VA - tvaddr)
s = ''.join(chr(b) if 32<=b<=126 else '.' for b in data[raw_at:raw_at+16])
print(f"First 16 bytes ASCII: '{s}'")
print(f"NOTE: This looks like string data, not code entry. Scanning for true entry...")
for k in range(-128, 512, 4):
    check = AT_VA + k
    if check < tvaddr: continue
    r = traw + (check - tvaddr)
    if r+4 > len(data): break
    w = int.from_bytes(data[r:r+4], 'big')
    if w == 0x7D8802A6:
        print(f"  MFLR at 0x{check:08X} (offset {k:+d})")
        for line in disasm_key(check, 96): print('  ' + line)
        break
print("\nfn_8 callers of atSingleton_Find:")
for fn8 in at_fn8:
    v2 = class_votes_for(fn8)
    best = v2.most_common(1)[0] if v2 else ('?', 0)
    print(f"  0x{fn8:08X}  class={best[0]} score={best[1]}")

# ─── PongNetGameModeCoordinator_vfn_6 ────────────────────────────────────
PONG_VA = 0x8239ECA8
PONG_SZ = fn_meta.get(PONG_VA, 0x964)
pong_end = PONG_VA + PONG_SZ
print("\n" + "="*70)
print(f"  PongNetGameModeCoordinator_vfn_6 @ 0x{PONG_VA:08X} size=0x{PONG_SZ:X}")
print("="*70)
pong_callers = reverse_cg.get(PONG_VA, set())
pong_fn8 = sorted(c for c in pong_callers if c in unnamed)
print(f"Callers: {len(pong_callers)} ({len(pong_fn8)} fn_8)")
pong_callees_all = set()
for src in forward_cg:
    if PONG_VA <= src < pong_end:
        pong_callees_all |= forward_cg[src]
print(f"Callees from region: {len(pong_callees_all)}")
for c in sorted(pong_callees_all):
    nm = sym_map.get(c, '0x{:08X}'.format(c))
    print(f"  0x{c:08X}  {nm}")
print("\nStrings referenced in pong region:")
raw_p = traw + (PONG_VA - tvaddr)
regs2 = {}
for k in range(0, PONG_SZ, 4):
    w = int.from_bytes(data[raw_p+k:raw_p+k+4], 'big')
    cur = PONG_VA + k
    op=(w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F
    imm = w & 0xFFFF
    if imm >= 0x8000: imm -= 0x10000
    if op == 15 and ra == 0: regs2[rd] = imm << 16
    elif op == 14 and ra in regs2:
        val = (regs2[ra] + imm) & 0xFFFFFFFF
        regs2[rd] = val
        s2 = read_str(val)
        if s2 and len(s2) >= 3:
            print(f"  0x{cur:08X}: r{rd}=0x{val:08X}  \"{s2}\"")
    elif op == 24 and ra in regs2:
        regs2[rd] = (regs2[ra] | (w&0xFFFF)) & 0xFFFFFFFF
    else:
        if rd in regs2 and op not in (15,14,24): regs2.pop(rd,None)

print("\nSub-functions inside pong region:")
for fn_va in fn_addrs:
    if PONG_VA < fn_va < pong_end:
        nm = sym_map.get(fn_va,'?')
        sz = fn_meta.get(fn_va, 0)
        nc = len(reverse_cg.get(fn_va, set()))
        print(f"  0x{fn_va:08X}  {nm:50s} size=0x{sz:X} callers={nc}")

# ─── All hotspots summary ─────────────────────────────────────────────────
print("\n" + "="*70)
print("  ALL HOTSPOTS > 4 fn_8 connections")
print("="*70)
h = collections.Counter()
for fn_va in unnamed:
    for nb in list(forward_cg.get(fn_va,set())) | list(reverse_cg.get(fn_va,set())):
        nm = sym_map.get(nb,'')
        if nm and not nm.startswith('fn_8'):
            h[nm] += 1
for nm, cnt in h.most_common(35):
    if cnt < 5: break
    print(f"  {nm:55s} {cnt}")

print("\nDone.")
