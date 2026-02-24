#!/usr/bin/env python3
"""auto_namer_v15 — hotspot #2-8 (new generation): rage_A980, _locale_register,
   util_5380, memcpy spatial, jumptable callers, and relaxed spatial cascade."""
import struct, bisect, collections, re, shutil

BASE    = '/Users/Ozordi/Downloads/tt-decomp'
SYM_IN  = f'{BASE}/config/434C4803/symbols_autonamed_v14.txt'
SYM_OUT = f'{BASE}/config/434C4803/symbols_autonamed_v15.txt'
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
print(f"Input: {len(sym_map)} symbols, {len(unnamed)} fn_8  ({len(sym_map)-len(unnamed)}/{len(sym_map)} = {(len(sym_map)-len(unnamed))/len(sym_map)*100:.2f}%)")

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

def spatial_class(va, window=12):
    idx = bisect.bisect_left(fn_offsets, va - tvaddr)
    v = collections.Counter()
    for j in range(max(0, idx-window), min(len(fn_addrs), idx+window+1)):
        nm = sym_map.get(fn_addrs[j], '')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls) > 3 and cls not in SKIP: v[cls] += 1
    return v

def cg_votes(va):
    v = collections.Counter()
    for nb in set(forward_cg.get(va,set())) | set(reverse_cg.get(va,set())):
        nm = sym_map.get(nb,'')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls) > 3 and cls not in SKIP: v[cls] += 1
    return v

renames = {}

# ── P35: rage_A980 callers split by spatial/CG ─────────────────────────────
# 4 in phJoint3Dof/phArticulatedCollider zone, 3 in vt6BE8 zone, 1 in vt8B80
RAGE_A980_MAP = {
    # phJoint3Dof spatial dominant (4 votes)
    0x82352360: 'phJoint3Dof_2360',
    0x823525E8: 'phJoint3Dof_25E8',
    # phArticulatedCollider spatial (weaker but best signal)
    0x82352DB8: 'phArticulatedCollider_2DB8',
    0x823533C8: 'phArticulatedCollider_33C8',
    # vt8B80 spatial zone
    0x8235CD88: 'vt8B80_CD88',
    # vt6BE8 spatial zone (3 fns)
    0x82362048: 'vt6BE8_2048',
    0x823625A0: 'vt6BE8_25A0',
    0x82362718: 'vt6BE8_2718',
}
p35 = 0
for va, nm in RAGE_A980_MAP.items():
    if va in unnamed:
        renames[va] = nm; sym_map[va] = nm; p35 += 1
print(f"P35 rage_A980 callers: {p35}")

# ── P36: _locale_register callers → msgMsgSink ─────────────────────────────
# All 5 have msgMsgSink spatial dominance (5-7 votes)
LOCALE_MAP = {
    0x8244BC40: 'msgMsgSink_BC40',
    0x82451450: 'msgMsgSink_1450',  # also snNotifyGamerAdded spatial but msgMsgSink dominant
    0x82451880: 'msgMsgSink_1880',
    0x82451930: 'msgMsgSink_1930',
    0x8256C728: 'msgMsgSink_C728',
}
p36 = 0
for va, nm in LOCALE_MAP.items():
    if va in unnamed:
        renames[va] = nm; sym_map[va] = nm; p36 += 1
print(f"P36 _locale_register callers (msgMsgSink): {p36}")

# ── P37: util_5380 callers → cmSampleCamMachineBank ─────────────────────────
# All 4 same size 0xCC, same spatial cluster cmSampleCamMachineBank (5)/cmCond (4)
UTIL5380_MAP = {
    0x822723E8: 'cmSampleCamMachineBank_23E8',
    0x822724B8: 'cmSampleCamMachineBank_24B8',
    0x82272588: 'cmSampleCamMachineBank_2588',
    0x82272658: 'cmSampleCamMachineBank_2658',
}
p37 = 0
for va, nm in UTIL5380_MAP.items():
    if va in unnamed:
        renames[va] = nm; sym_map[va] = nm; p37 += 1
print(f"P37 util_5380 callers (cmSampleCamMachineBank): {p37}")

# ── P38: memcpy remaining with spatial ──────────────────────────────────────
# Clear spatial signals identified per-function
MEMCPY_SPATIAL = {
    0x823F4088: 'SinglesNetworkClient_4088',  # spatial: SinglesNetworkClient 4
    0x82493E68: 'vt8918_3E68',                # spatial: vt8918 9
    0x824AEC48: 'phBoundCapsule_EC48',         # spatial: phBoundCapsule 6
    0x824B2750: 'atSingleton_2750',            # spatial: atSingleton 8
    0x824BB4C0: 'atSingleton_B4C0',            # spatial: atSingleton 8
    0x824C1AD0: 'fiAsciiTokenizer_1AD0',       # spatial: fiAsciiTokenizer 6
    0x824C2B38: 'atSingleton_2B38',            # spatial: atSingleton 4
    0x824C2E20: 'atSingleton_2E20',            # spatial: atSingleton 4
    0x824C33E8: 'phBoundCapsule_33E8',         # spatial: phBoundCapsule 9
    0x824C36D8: 'phBoundCapsule_36D8',         # spatial: phBoundCapsule 10
    0x824FF628: 'atSingleton_FF28',            # spatial: atSingleton 10
    0x8255F3E0: 'atSingleton_F3E0',            # spatial: atSingleton 13
    0x8256B1B8: 'vt8308_B1B8',                # spatial: vt8308 3
    0x8256BB38: 'phBoundCapsule_BB38',         # spatial: phBoundCapsule 4
}
p38 = 0
for va, nm in MEMCPY_SPATIAL.items():
    if va in unnamed:
        renames[va] = nm; sym_map[va] = nm; p38 += 1
print(f"P38 memcpy spatial: {p38}")

# ── P39: Relaxed spatial cascade — w=15, 60% threshold ─────────────────────
p39 = 0
for rnd in range(8):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        sp = spatial_class(va, 15)
        if not sp: continue
        best_cls, best_cnt = sp.most_common(1)[0]
        total = sum(sp.values())
        if best_cnt >= 3 and best_cnt/total >= 0.60:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p39'
            sym_map[va] = renames[va]
            p39 += 1; added += 1
    print(f"  P39 round {rnd+1}: {added}")
    if added == 0: break
print(f"P39 spatial cascade total: {p39}")

# ── P40: CG BFS cascade with new names ──────────────────────────────────────
p40 = 0
for rnd in range(8):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        v = cg_votes(va)
        if not v: continue
        best_cls, best_score = v.most_common(1)[0]
        total = sum(v.values())
        if best_score >= 2 and best_score/total >= 0.70:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p40'
            sym_map[va] = renames[va]
            p40 += 1; added += 1
    print(f"  P40 BFS round {rnd+1}: {added}")
    if added == 0: break
print(f"P40 BFS total: {p40}")

# ── Summary ─────────────────────────────────────────────────────────────────
fn8_fixed = sum(1 for va in renames if va in unnamed)
remaining = len(unnamed) - fn8_fixed
coverage = (len(sym_map) - remaining) / len(sym_map) * 100
print(f"\nTotal renames: {len(renames)}, fn_8 fixed: {fn8_fixed}")
print(f"Remaining fn_8: {remaining}")
print(f"New coverage: {coverage:.2f}%")

# ── Write output ─────────────────────────────────────────────────────────────
shutil.copy(SYM_IN, SYM_IN + '.bak_v14')
lines_out = []
for L in original_lines:
    if '= .text:0x' in L:
        try:
            addr = int(L.split(':')[1].split(';')[0].strip(), 16)
            if addr in renames:
                old = L.split('=')[0].strip()
                L = L.replace(old + ' =', renames[addr] + ' =', 1)
        except: pass
    lines_out.append(L)
with open(SYM_OUT, 'w') as f:
    f.writelines(lines_out)
print(f"Wrote {SYM_OUT}")
