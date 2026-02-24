#!/usr/bin/env python3
"""auto_namer_v14 — Hotspot #2-8 naming pass"""
import struct, bisect, collections, re, shutil

BASE    = '/Users/Ozordi/Downloads/tt-decomp'
SYM_IN  = f'{BASE}/config/434C4803/symbols_autonamed_v13.txt'
SYM_OUT = f'{BASE}/config/434C4803/symbols_autonamed_v14.txt'
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

def spatial_class(va, window=12):
    idx = bisect.bisect_left(fn_offsets, va - tvaddr)
    v = collections.Counter()
    skip = {'xam','util','game','rage','nop','ref','jumptable','memcpy','memset',
            'atexit','Rtl','fn','thunk','RtlEnterCriticalSection','RtlLeaveCriticalSection'}
    for j in range(max(0, idx-window), min(len(fn_addrs), idx+window+1)):
        nm = sym_map.get(fn_addrs[j], '')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls) > 3 and cls not in skip: v[cls] += 1
    return v

def cg_votes(va):
    v = collections.Counter()
    skip = {'xam','util','game','rage','nop','ref','jumptable','memcpy','memset',
            'atexit','Rtl','fn','thunk'}
    for nb in set(forward_cg.get(va,set())) | set(reverse_cg.get(va,set())):
        nm = sym_map.get(nb,'')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls) > 3 and cls not in skip: v[cls] += 1
    return v

renames = {}  # addr -> new_name

# ── P26: atSingleton destructors (rage_free AND atSingleton_Find callers) ──
RAGE_VA    = 0x820C00C0
ATFIND_VA  = 0x820F90D0
rage_fn8   = set(c for c in reverse_cg.get(RAGE_VA,  set()) if c in unnamed)
atfind_fn8 = set(c for c in reverse_cg.get(ATFIND_VA, set()) if c in unnamed)
at_dtors   = sorted(rage_fn8 & atfind_fn8)
for va in at_dtors:
    renames[va] = f'atSingleton_dtor_{va & 0xFFFF:04X}'
print(f"P26 atSingleton_dtor: {len(at_dtors)}")
for va in at_dtors: print(f"  0x{va:08X} → {renames[va]}")

# ── P27: rage_free-only callers — named by callees ────────────────────────
rage_only = sorted(rage_fn8 - atfind_fn8)
p27 = 0
# Specific high-confidence cases from analysis:
RAGE_ONLY_MAP = {
    0x820F5000: 'atSingleton_5000',       # calls atSingleton_5338_g
    0x82124D30: 'fragDrawable_4D30',       # calls fragDrawable_5688, ph_5908
    0x82163080: 'atSingleton_3080',        # calls atSingleton_A818_g
    0x821C5260: 'pongSaveFile_5260',       # calls util_CE30, pongSaveFile context
    0x8222D150: 'fiAsciiTokenizer_2150',   # calls fiAsciiTokenizer_76D8_g + memcpy
    0x82272778: 'atSingleton_2778',        # already in atfind set but rage_only fallback
    0x8225D240: 'atSingleton_D240',
    0x82272C88: 'atSingleton_2C88',
    0x822E2290: 'atSingleton_E290',
    0x82415618: 'atSingleton_5618',        # calls atSingleton_8398_g
    0x8229A1F0: 'atSingleton_A1F0',        # spatial/context
    0x823887D8: 'atSingleton_87D8',        # spatial
    0x820C8620: 'atSingleton_8620',        # small fn only calling rage_free
    0x820E74B8: 'atSingleton_74B8',
    0x821350B8: 'atSingleton_50B8',
    0x822BB448: 'atSingleton_B448',
}
for va in rage_only:
    if va in RAGE_ONLY_MAP and va in unnamed:
        renames[va] = RAGE_ONLY_MAP[va]
        p27 += 1
# Also apply for the overlap ones not already caught
for va in at_dtors:
    if va in RAGE_ONLY_MAP and va not in renames:
        renames[va] = RAGE_ONLY_MAP[va]
print(f"P27 rage_free named: {p27}")

# ── P28: RTL critical section cluster → vt8308 ────────────────────────────
RTL_CLUSTER = [0x824609F0, 0x82460CB0, 0x82460D18, 0x82460D70,
               0x82460DC8, 0x82460E98, 0x82460F10, 0x82461168]
p28 = 0
for va in RTL_CLUSTER:
    if va in unnamed and va not in renames:
        renames[va] = f'vt8308_{va & 0xFFFF:04X}_p28'
        p28 += 1
print(f"P28 vt8308 RTL cluster: {p28}")
for va in RTL_CLUSTER:
    if va in renames: print(f"  0x{va:08X} → {renames[va]}")

# ── P29: memcpy spatial clusters ─────────────────────────────────────────
MEMCPY_SPATIAL = {
    # PongNetExhibitionCoordinator (spatial dominant ≥10/15)
    0x823A6590: 'PongNetExhibitionCoordinator_6590',
    0x823A6698: 'PongNetExhibitionCoordinator_6698',
    0x823A67A0: 'PongNetExhibitionCoordinator_67A0',
    0x823A68A8: 'PongNetExhibitionCoordinator_68A8',
    0x823A6AB8: 'PongNetExhibitionCoordinator_6AB8',
    0x823A6CC8: 'PongNetExhibitionCoordinator_6CC8',
    # lvlTable (spatial dominant 7-10/15)
    0x82336AE8: 'lvlTable_6AE8',
    0x82336D78: 'lvlTable_6D78',
    0x82336E58: 'lvlTable_6E58',
    # phBoundTaperedCapsule (spatial 5/15)
    0x8233F3D8: 'phBoundTaperedCapsule_F3D8',
    # atSingleton (spatial 4/15) — weaker, still worth naming
    0x824B31D8: 'atSingleton_31D8',
    0x824B34A8: 'atSingleton_34A8',
}
p29 = 0
for va, nm in MEMCPY_SPATIAL.items():
    if va in unnamed and va not in renames:
        renames[va] = nm
        p29 += 1
print(f"P29 memcpy spatial: {p29}")

# ── P30: Large __savegprlr callers with strong signals ────────────────────
SAVEGPR_MAP = {
    0x823300B0: 'hudLeaderboard_00B0',     # STRING: "hudLeaderboard - EXH YEAR TRUESKILL"
    0x823F6408: 'dcamPolarCam_6408',       # spatial: dcamPolarCam dominant (6/10)
    0x824A8638: 'vt8918_8638',             # spatial: vt8918 dominant (4)
    0x82340900: 'pongCreatureInst_0900',   # cg vote: pongCreatureInst (from earlier pass)
    0x82340DB8: 'pongCreatureInst_0DB8',
    0x8221F090: 'SinglesNetworkClient_F090',
    # atSingleton cluster
    0x8254DA10: 'atSingleton_DA10',
    0x8254ECD8: 'atSingleton_ECD8',
    0x825511F8: 'atSingleton_11F8',
    0x8255EAF8: 'atSingleton_EAF8',
    0x824F4108: 'atSingleton_4108',
    0x824F46C8: 'atSingleton_46C8',
    0x824F5438: 'atSingleton_5438',
    0x824F58A0: 'atSingleton_58A0',
    0x824FC050: 'atSingleton_C050',
    0x824FDA80: 'atSingleton_DA80',
    0x82563080: 'phBoundCapsule_3080',
    0x82563AA8: 'atSingleton_3AA8',
    0x82564320: 'atSingleton_4320',
    0x82508D50: 'phBoundCapsule_8D50',
}
p30 = 0
for va, nm in SAVEGPR_MAP.items():
    if va in unnamed and va not in renames:
        renames[va] = nm
        p30 += 1
print(f"P30 __savegprlr spatial/string: {p30}")

# ── P31: remaining memset fn_8 callers ───────────────────────────────────
MEMSET_MAP = {
    0x82118D30: None,  # no votes - check spatial
    0x8229B208: None,
    0x82357098: None,  # weak spatial: hudFlashBase/msgMsgSink/vt8308
    0x82498988: None,
    0x82498BC8: 'phBoundCapsule_8BC8',  # cg vote: phBoundCapsule 1/2
    0x824B28A8: None,
    0x824BEE18: None,
    0x824BF178: None,
    0x824C9880: None,
    0x824CCEA8: None,
}
p31 = 0
for va, nm in MEMSET_MAP.items():
    if va in unnamed and va not in renames:
        if nm:
            renames[va] = nm
            p31 += 1
        else:
            sp = spatial_class(va, 12)
            if sp:
                best_cls, best_cnt = sp.most_common(1)[0]
                total = sum(sp.values())
                if best_cnt >= 3 and best_cnt/total >= 0.5:
                    renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p31'
                    p31 += 1
print(f"P31 memset spatial: {p31}")

# ── P32: BFS cascade — remaining fn_8 with strong CG votes ───────────────
# Update sym_map with all renames first
for va, nm in renames.items():
    sym_map[va] = nm

skip_cls = {'xam','util','game','rage','nop','ref','jumptable','memcpy','memset',
            'atexit','Rtl','fn','thunk','RtlEnterCriticalSection','RtlLeaveCriticalSection'}
p32 = 0
for rnd in range(6):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        v = collections.Counter()
        for nb in set(forward_cg.get(va,set())) | set(reverse_cg.get(va,set())):
            nm = sym_map.get(nb,'')
            if nm and not nm.startswith('fn_8') and '_' in nm:
                cls = nm.split('_')[0]
                if len(cls) > 3 and cls not in skip_cls: v[cls] += 1
        if not v: continue
        best_cls, best_score = v.most_common(1)[0]
        total = sum(v.values())
        if best_score >= 2 and best_score/total >= 0.7:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p32'
            sym_map[va] = renames[va]
            p32 += 1; added += 1
    print(f"  P32 round {rnd+1}: {added}")
    if added == 0: break
print(f"P32 BFS total: {p32}")

# ── P33: spatial sweep for remaining high-confidence regions ─────────────
p33 = 0
for va in sorted(unnamed):
    if va in renames: continue
    sp = spatial_class(va, 8)
    if not sp: continue
    best_cls, best_cnt = sp.most_common(1)[0]
    total = sum(sp.values())
    if best_cnt >= 5 and best_cnt/total >= 0.75:  # strong spatial signal
        renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p33'
        sym_map[va] = renames[va]
        p33 += 1
print(f"P33 spatial sweep: {p33}")

# ── Summary ────────────────────────────────────────────────────────────────
fn8_fixed = sum(1 for va in renames if va in unnamed)
print(f"\nTotal renames: {len(renames)}, fn_8 fixed: {fn8_fixed}")
print(f"Remaining fn_8: {len(unnamed) - fn8_fixed}")
print(f"Coverage: {(len(sym_map)-(len(unnamed)-fn8_fixed))/len(sym_map)*100:.2f}%")

# ── Write output ──────────────────────────────────────────────────────────
shutil.copy(SYM_IN, SYM_IN + '.bak_v13')
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
print(f"\nWrote {SYM_OUT}")
