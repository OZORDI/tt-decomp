#!/usr/bin/env python3
"""auto_namer_v16 — string-confirmed names, hotspot callees, spatial w=20 final sweep"""
import struct, bisect, collections, re, shutil

BASE   = '/Users/Ozordi/Downloads/tt-decomp'
SYM_IN = f'{BASE}/config/434C4803/symbols_autonamed_v15.txt'
SYM_OUT= f'{BASE}/config/434C4803/symbols_autonamed_v16.txt'
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
print(f"Input: {len(sym_map)} symbols, {len(unnamed)} fn_8  ({(len(sym_map)-len(unnamed))/len(sym_map)*100:.2f}%)")

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

def spatial_class(va, window=15):
    idx = bisect.bisect_left(fn_offsets, va - tvaddr)
    v = collections.Counter()
    for j in range(max(0,idx-window), min(len(fn_addrs),idx+window+1)):
        nm = sym_map.get(fn_addrs[j],'')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls)>3 and cls not in SKIP: v[cls]+=1
    return v

def cg_votes(va):
    v = collections.Counter()
    for nb in set(forward_cg.get(va,set())) | set(reverse_cg.get(va,set())):
        nm = sym_map.get(nb,'')
        if nm and not nm.startswith('fn_8') and '_' in nm:
            cls = nm.split('_')[0]
            if len(cls)>3 and cls not in SKIP: v[cls]+=1
    return v

renames = {}

# ── P41: High-confidence specific names (string + spatial confirmed) ──────
SPECIFIC = {
    # Isolated fn_8 with strings
    0x820DEB38: 'pcrAnimEvent_EB38',           # spatial pcrAnimEvent 4, string "Unknown AnimEvent type"
    0x82276958: 'cmNamedValueSet_6958',         # spatial cmNamedValueSet 7, string "Unknown tuning type"
    0x823F8CC0: 'SinglesNetworkClient_8CC0',    # sp SinglesNetworkClient 8, strings PlaySound/looped/oneshot
    0x823F8CF8: 'SinglesNetworkClient_8CF8',    # sp SinglesNetworkClient 8, string StopSound
    0x823FDF20: 'phBoundCapsule_DF20',          # sp phBoundCapsule 12, strings setRGB/_target
    0x824C6E18: 'phBoundCapsule_6E18',          # sp phBoundCapsule 12, string DBF
    # game_2610 callees
    0x820D23A8: 'pongBlendLookAtDriver_23A8',   # sp pongBlendLookAtDriver 4/8
    0x820D2730: 'pongLocomotionController_2730',# sp pongLocomotionController 3/7
    0x820D2828: 'pongLocomotionController_2828',# sp pongLocomotionController 4/7
    # util_1110 callees (evtInstance spatial 3, tied with atSingleton but evtInstance more specific)
    0x82220E70: 'evtInstance_0E70',
    0x82221410: 'evtInstance_1410',
    0x82221B50: 'evtInstance_1B50',
    # CMtoCSAdapter callees
    0x82271EF0: 'cmCachedOperator_1EF0',        # string "Invalid operator type for cached operator"
    0x822721B8: 'cmSampleCamMachineBank_21B8',  # sp cmSampleCamMachineBank 8/14
    0x82276EB8: 'cmMetafileTuningSet_6EB8',     # sp cmMetafileTuningSet 6/13
    # Jumptable callers
    0x82186690: 'cmOperatorCtor_6690',          # sp cmOperatorCtor 6/12
    0x8223A5D0: 'parStreamInXml_A5D0',          # sp parStreamInXml 7/10
    0x8246F758: 'vt6BE8_F758',                  # sp vt6BE8 7 (more specific than atSingleton)
    0x82491B18: 'vt8918_1B18',                  # sp vt8918 9/17
    0x8249EB00: 'vt8918_EB00',                  # sp vt8918 5/12 (more specific than atSingleton)
    0x824AE858: 'phBoundCapsule_E858',          # sp phBoundCapsule 10/16
    0x824C08C0: 'fiAsciiTokenizer_08C0',        # sp fiAsciiTokenizer 5/12
    0x824C9880: 'atSingleton_9880',             # sp atSingleton 13/21
    # Non-isolated with strong strings + spatial
    0x820F3010: 'fragDrawable_3010',            # sp fragDrawable 7/10, string memory:$%p
    0x82106DA0: 'stateInit_6DA0',               # sp stateInit 5/8, string SetPostPointDelay
    0x82118C58: 'pongCreatureInst_8C58',        # string "Can't find self collision pair", sp pongCreatureInst 2
    0x82122798: 'Holder_2798',                  # sp Holder 7/13, string "cannot find bone"
    0x8215B060: 'hudFlashBase_B060',            # sp hudFlashBase 5/7
    0x82168DC0: 'pongInlineCinematics_8DC0',    # sp pongInlineCinematics 11/16
    0x82177D88: 'pongHairData_7D88',            # sp pongHairData 7/11
    0x822106A8: 'PongNetRoundRobinCoordinator_06A8', # sp PongNetRRCoord 13/19, string match winner
    0x82223840: 'gmLogic_3840',                 # sp gmLogic 4/8, string 'none'
    0x822E2830: 'fiDeviceMemory_2830',          # sp fiDeviceMemory 6/10, string xHair::Init
    0x82351A58: 'phJoint3Dof_351A58',           # sp phJoint3Dof 6/9, string "cannot find bone"
    0x8239BF30: 'SinglesNetworkClient_BF30',    # sp SinglesNetworkClient 12/17
    0x823F5AC8: 'dcamPolarCam_5AC8',            # sp dcamPolarCam 7/12
    0x823FDCF8: 'phBoundCapsule_DCF8',          # sp phBoundCapsule 12/15
    # vtable hit
    0x8256B628: 'vt8308_B628',                  # vtable analysis: vt8308 4/4 votes
}
p41 = 0
for va, nm in SPECIFIC.items():
    if va in unnamed:
        renames[va] = nm; sym_map[va] = nm; p41 += 1
print(f"P41 specific/string/hotspot-callee names: {p41}")

# ── P42: Relaxed spatial cascade w=20, 50% threshold ─────────────────────
p42 = 0
for rnd in range(10):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        sp = spatial_class(va, 20)
        if not sp: continue
        best_cls, best_cnt = sp.most_common(1)[0]
        total = sum(sp.values())
        if best_cnt >= 2 and best_cnt/total >= 0.50:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p42'
            sym_map[va] = renames[va]
            p42 += 1; added += 1
    print(f"  P42 round {rnd+1}: {added}")
    if added == 0: break
print(f"P42 spatial w=20/50%: {p42}")

# ── P43: CG BFS with relaxed threshold ────────────────────────────────────
p43 = 0
for rnd in range(8):
    added = 0
    for va in sorted(unnamed):
        if va in renames: continue
        v = cg_votes(va)
        if not v: continue
        best_cls, best_score = v.most_common(1)[0]
        total = sum(v.values())
        if best_score >= 2 and best_score/total >= 0.60:
            renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p43'
            sym_map[va] = renames[va]
            p43 += 1; added += 1
    print(f"  P43 BFS round {rnd+1}: {added}")
    if added == 0: break
print(f"P43 CG BFS: {p43}")

# ── P44: Very last resort — single strongest spatial vote, any count ──────
p44 = 0
for va in sorted(unnamed):
    if va in renames: continue
    sp = spatial_class(va, 25)  # very wide window
    if not sp: continue
    best_cls, best_cnt = sp.most_common(1)[0]
    total = sum(sp.values())
    # Accept even weak signal if it's clearly dominant
    if best_cnt >= 4 and best_cnt/total >= 0.45:
        renames[va] = f'{best_cls}_{va & 0xFFFF:04X}_p44'
        sym_map[va] = renames[va]
        p44 += 1
print(f"P44 last-resort spatial: {p44}")

# ── Summary ───────────────────────────────────────────────────────────────
fn8_fixed = sum(1 for va in renames if va in unnamed)
remaining = len(unnamed) - fn8_fixed
cov = (len(sym_map) - remaining) / len(sym_map) * 100
print(f"\nTotal renames: {len(renames)}, fn_8 fixed: {fn8_fixed}")
print(f"Remaining fn_8: {remaining}")
print(f"Coverage: {cov:.2f}%")

# ── Write ─────────────────────────────────────────────────────────────────
shutil.copy(SYM_IN, SYM_IN + '.bak_v15')
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
print(f"Wrote {SYM_OUT}")
