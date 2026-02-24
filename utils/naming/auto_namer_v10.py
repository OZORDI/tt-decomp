#!/usr/bin/env python3
"""
auto_namer_v10.py — Approach 4: Manual bridge seeds + GTA IV cross-match seeds + cascade
Reads symbols_autonamed_v9.txt → writes symbols_autonamed_v10.txt

New in v10:
  MANUAL_SEEDS     — 17 bridge seeds from bridge_analyzer.py (Approach 4)
                     + 8 additional high-signal seeds from 2-hop analysis
  CROSS_MATCH_SEEDS — 5 seeds from GTA IV vtable cross-match (grcSetup class)
  Same passes as v9 (SEED → P10 → P11 → P4 re-run → P6 re-run)

Math (Python-verified at design time):
  Top 10 bridge seeds → 52 direct fn_8 + cascade 2x–3x → 104–156 total
  5 cross-match seeds → ~20–25 propagated (grcSetup vtable neighborhood)
  Expected coverage: 84.2% + ~0.7–1.0% = ~85.0–85.2%
"""
import re, os, struct, bisect
from collections import defaultdict, Counter

BASE   = '/Users/Ozordi/Downloads/tt-decomp'
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v9.txt')
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v10.txt')
REPORT = os.path.join(BASE, 'auto_namer_v10_report.txt')

# ── Manual Bridge Seeds (Approach 4 — bridge_analyzer output) ─────────────────
# 17 high-confidence seeds from bridge_analysis.txt + 8 additional seeds
# derived from 2-hop and string analysis on the next tier of bridges.
MANUAL_SEEDS = {
    # ── Tier 1: Direct dominant-caller matches (100% of named callers) ──────
    0x823764A0: ('grmEdgeModel_64A0',
                 'manual: 1/1 callers=grmEdgeModel (100%) + strings "verts:" "tris:" "edges:" | callers: grmEdgeModel_Create'),
    0x820D7F40: ('pongPlayer_7F40',
                 'manual: 1/1 callers=pongPlayer (100%) | callers: pongPlayer_BF18_g; calls pongPlayer_6AA0_g, phBoundCapsule_9730_fw'),
    0x8247EE00: ('vt8308_EE00',
                 'manual: 1/1 callers=vt8308 (100%) | callers: game_D720, vt8308_D138_g'),
    0x8222B1E8: ('LocomotionStateMf_B1E8',
                 'manual: 1/1 callers=LocomotionStateMf (100%) | callers: LocomotionStateMf_Load; calls fiAsciiTokenizer_D278_g'),
    0x82174298: ('phBoundCapsule_4298',
                 'manual: 1/1 callers=phBoundCapsule (100%) | callers: phBoundCapsule_AB30_g; calls pongCameraMgr_2F00'),
    0x823D3828: ('AckHandling_3828',
                 'manual: 1/1 callers=AckHandling (100%) | callers: AckHandling_ProcessSequence; r3[26] accessed; leaf fn'),
    0x82432A70: ('parStreamOutXml_2A70',
                 'manual: 1/1 callers=parStreamOutXml (100%) | callers: parStreamOutXml_3E40; 12B leaf'),
    0x823DC7E8: ('SinglesNetworkClient_C7E8',
                 'manual: 1/1 callers=SinglesNetworkClient (100%) | callers: SinglesNetworkClient_B200_g; r3[72,76,80] struct access'),
    0x82224AD8: ('crAnimDofFloat_4AD8',
                 'manual: 1/1 callers=crAnimDofFloat (100%) | callers: crAnimDofFloat_75C0_g; calls fxCloth_CreateClothSim'),
    0x822110F8: ('PongNetRoundRobinCoordinator_10F8',
                 'manual: 1/1 callers=PongNetRoundRobinCoordinator (100%) | callers: PongNetRoundRobinCoordinator_00E0_g; r3[29]'),
    0x8219B848: ('atSingleton_B848',
                 'manual: 1/1 callers=atSingleton (100%) | callers: atSingleton_rtti_92D4_1; 3292B large init'),
    0x82485DF0: ('vt8A48_5DF0',
                 'manual: 1/1 callers=vt8A48 (100%) | callers: vt8A48_30; calls vt8308_34D0_g, atSingleton_vfn_46'),
    0x82460C58: ('vt8308_0C58',
                 'manual: 1/1 callers=vt8308 (100%) | callers: vt8308_53; calls RtlEnterCriticalSection, RtlLeaveCriticalSection'),
    0x82460A48: ('phBoundCapsule_0A48',
                 'manual: 1/1 callers=phBoundCapsule (100%) | callers: phBoundCapsule_E5B0_h; calls RtlEnterCriticalSection'),
    0x8244C928: ('phBoundCapsule_C928',
                 'manual: 1/1 callers=phBoundCapsule (100%) | callers: phBoundCapsule_2A30_g; r3[0] access'),
    0x82446330: ('grmShaderFx_6330',
                 'manual: 1/1 callers=grmShaderFx (100%) | callers: grmShaderFx_vfn_2; r3[608,620] struct fields'),
    0x8245A150: ('phBoundCapsule_A150',
                 'manual: phBoundCapsule+msgMsgSink callers, phBoundCapsule_2A30_g dominant; r3[0]'),

    # ── Tier 2: Dominant via stringer / multi-caller analysis ────────────────
    0x82222800: ('hsmContext_SetNextState_2800',
                 'manual: string "Attempting to set next machine state to %s" + "Invalid state transition" | '
                 '100 callers all HSM state vfns (stateInit, stateReturnHit, statePreInit etc.)'),
    0x8225E408: ('SinglesNetworkClient_GetPlayerID_E408',
                 'manual: string "GetPlayerID - msg tgt not player 0 ^ 1!" + 2-hop SinglesNetworkClient(220) dominant | '
                 '24 callers across SinglesNetworkClient, atSingleton, pongFrontendContext'),
    0x821A9420: ('atSingleton_9420',
                 'manual: 128 callers; 2-hop atSingleton(55) strongly dominant; r3[0] vtable/header access | '
                 'callers: assetVersions_vfn_0 family, atSingleton_6490_g, cellData_vfn_0 etc.'),
    0x822EDA08: ('SinglesNetworkClient_DA08',
                 'manual: 2-hop SinglesNetworkClient(28) dominant; 12 callers incl SinglesNetworkClient_54F8_g; '
                 'calls ke_1B00 (Xbox kernel)'),
    0x820D6D48: ('LocomotionStateAnim_6D48',
                 'manual: LocomotionStateAnim_BBC8_g caller; r3[304-314] large struct; '
                 '2-hop pongPlayer(6)+LocomotionStateAnim(5)'),
    0x820D5040: ('pongCreatureInst_5040',
                 'manual: pongCreatureInst_vfn_28 caller (100% named callers); '
                 '2-hop pongCreatureInst(6) dominant'),
    0x824608B8: ('vt8308_08B8',
                 'manual: vt8308_43 dominant named caller; calls RtlEnterCriticalSection/RtlLeaveCriticalSection; '
                 '2-hop msgMsgSink(62) via critical section pair'),
    0x8222B1E8: ('LocomotionStateMf_B1E8',   # duplicate guard — already above, skipped if dupe
                 'manual: LocomotionStateMf_Load caller'),

    # ── GTA IV Cross-Binary Match Seeds (rage::grcSetup vtable, conf=85) ─────
    0x82161D40: ('grcSetup_vfn_2',
                 'GTA4 cross-match: GTA4:0x826F26B8 cls=rage::grcSetup vtable_slot conf=85'),
    0x82161DB8: ('grcSetup_vfn_3',
                 'GTA4 cross-match: GTA4:0x82484858 cls=rage::grcSetup vtable_slot conf=85'),
    0x821620D0: ('grcSetup_vfn_6',
                 'GTA4 cross-match: GTA4:0x82126C28 cls=rage::grcSetup vtable_slot conf=85'),
    0x82162178: ('grcSetup_vfn_5',
                 'GTA4 cross-match: GTA4:0x82484B88 cls=rage::grcSetup vtable_slot conf=85'),
    0x82162420: ('grcSetup_vfn_13',
                 'GTA4 cross-match: GTA4:0x82126C68 cls=rage::grcSetup vtable_slot conf=85'),
}
# Remove duplicate key (B1E8 appears twice due to copy guard above — Python keeps last)
# The second entry for 0x8222B1E8 is identical in name so no harm done.

# ── Binary loading ─────────────────────────────────────────────────────────────
print("Loading binary...")
data = open(XEX,'rb').read()
e_lfanew     = int.from_bytes(data[0x3C:0x40],'little')
coff_off     = e_lfanew + 4
opt_off      = coff_off + 20
image_base   = int.from_bytes(data[opt_off+28:opt_off+32],'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18],'little')
num_sects    = int.from_bytes(data[coff_off+2:coff_off+4],'little')
sect_tbl     = opt_off + opt_hdr_size
sections     = []
for i in range(num_sects):
    s       = data[sect_tbl+i*40 : sect_tbl+i*40+40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize   = int.from_bytes(s[8:12],'little')
    vaddr   = int.from_bytes(s[12:16],'little')
    raw_off = int.from_bytes(s[20:24],'little')
    sections.append((sname, image_base+vaddr, vsize, raw_off))
    print(f"  {sname:12s}  VA=0x{image_base+vaddr:08X}")

text_sec  = next(s for s in sections if s[0]=='.text')
rdata_sec = next(s for s in sections if s[0]=='.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def va_to_file(va):
    for _, abs_va, vsize, raw_off in sections:
        if abs_va <= va < abs_va+vsize: return raw_off+(va-abs_va)
    return None

# ── Symbol loading ─────────────────────────────────────────────────────────────
print("\nLoading symbols (from v9 output)...")
sym_map   = {}
sym_lines = []
fn_meta   = {}
with open(SYMS) as f:
    for line in f:
        sym_lines.append(line)
        s = line.strip()
        for sect in ['.text','.rdata','.data']:
            if '= '+sect+':0x' in s:
                parts = s.split('=',1)
                name  = parts[0].strip()
                try:
                    addr = int(parts[1].split(';')[0].strip().split(':')[1],16)
                    sym_map[addr] = name
                    m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                    if m: fn_meta[addr] = int(m.group(1),16)
                except: pass

fn_addrs   = sorted(a for a in fn_meta if tvaddr <= a < tvaddr+tvsize)
fn_offsets = [a-tvaddr for a in fn_addrs]
fn_set     = set(fn_addrs)
unnamed    = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
game_named = {a for a in fn_addrs if sym_map.get(a,'').startswith('game_')}
print(f"  Total: {len(fn_addrs)}, fn_8: {len(unnamed)}, game_: {len(game_named)}")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr+fn_offsets[idx]) if idx >= 0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

# ── STDLIB BLACKLIST ───────────────────────────────────────────────────────────
STDLIB_BLACKLIST = {
    'memcpy','memset','malloc','free','realloc','calloc',
    'printf','sprintf','snprintf','vsnprintf','sscanf',
    'strlen','strcpy','strcat','strcmp','strncmp','strncpy',
    'memmove','memcmp','memchr','strchr','strrchr',
    'atexit','exit','abort','raise','longjmp','setjmp',
    'atoi','atol','atof','strtol','strtof','strtod',
    'qsort','bsearch','rand','srand',
    'fopen','fclose','fread','fwrite','fseek','ftell','fflush','feof','ferror',
    'fgets','fputs','puts','gets',
    'rage_free','rage_malloc','rage_stricmp',
    '__cxa_atexit','__cxa_pure_virtual','__pure_virtual',
    'XGetVideoMode','XGetAVPackFlags','XGetGameRegion',
    'XInputGetState','XInputSetState',
} | {f'__savegprlr_{i}' for i in range(14,31)} \
  | {f'__restgprlr_{i}' for i in range(14,31)} \
  | {'__savefpr','__restfpr','__savevmx','__restvmx','__savevecreg','__restvecreg'}

GENERIC_PFX = frozenset({
    'game','ph','xe','ke','grc','aud','rage','jumptable','ref','nt',
    'util','thunk','vt','nop','const','get','nullvfn','fn','io',
    'stub','sp','2h','2hr','loc','pdata','BINK','set','return','thunk',
})

# ── Naming state ───────────────────────────────────────────────────────────────
new_names  = {}
confidence = {}
pass_tag   = {}
conf_score = {}
used_names = set(sym_map.values())
fn_class   = {}

PASS_CONFIDENCE = {
    'SEED': 95,
    'P10':  65,  'P10r': 55,
    'P11':  68,
    'P4f':  72,  'P4r':  70, 'P4fw': 57, 'P4rh': 60,
    'P6':   48,  'P6r':  38,
    'P7':   42,
}

def propose(fn_va, name, reason, tag, overwrite=False):
    new_conf = PASS_CONFIDENCE.get(tag, 50)
    if fn_va in new_names:
        if not overwrite: return False
        if new_conf <= conf_score.get(fn_va, -1): return False
        used_names.discard(new_names[fn_va])
    original = name
    for attempt in range(15):
        candidate = name if attempt == 0 else f"{original}_{fn_va&0xFFFF:04X}_{attempt}"
        if candidate not in used_names or (fn_va in new_names and new_names[fn_va]==candidate):
            new_names[fn_va]  = candidate
            confidence[fn_va] = reason
            pass_tag[fn_va]   = tag
            conf_score[fn_va] = new_conf
            used_names.add(candidate)
            return True
    return False

def get_class_clean(fn_va, extra_names=None):
    if fn_va in fn_class: return fn_class[fn_va]
    nm = (extra_names or {}).get(fn_va) or new_names.get(fn_va) or sym_map.get(fn_va,'')
    if not nm or nm.startswith('fn_8'): return None
    if nm in STDLIB_BLACKLIST: return None
    if nm.startswith('__'): return None
    parts = nm.split('_')
    if not parts or parts[0] in GENERIC_PFX: return None
    return parts[0]

def rtti_class_to_prefix(cls):
    parts = cls.split('::')
    return safe_name(parts[-1]) if len(parts) <= 2 else safe_name('_'.join(parts[1:]))

# ── Build Callgraphs ───────────────────────────────────────────────────────────
print("\nBuilding callgraphs...")
forward_cg = defaultdict(set)
reverse_cg = defaultdict(set)
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    if (w>>26)&0x3F==18 and (w&1):
        li = w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va    = tvaddr + i + li
        if caller_va and tgt_va in fn_set:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
print(f"  bl edges from {len(forward_cg)} functions")

# fn-ptr edges
fn_ptr_fwd = defaultdict(set)
fn_ptr_rev = defaultdict(set)
pending = {}
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    op=(w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
    if op==15 and ra==0:
        signed_hi = imm if imm < 0x8000 else imm - 0x10000
        pending[rd] = signed_hi << 16
    elif op==14 and ra in pending:
        lo = imm if imm < 0x8000 else imm - 0x10000
        addr = (pending[ra] + lo) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr+tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr)
                fn_ptr_rev[addr].add(caller)
    elif op==24 and ra in pending:
        addr = (pending[ra] | imm) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr+tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr)
                fn_ptr_rev[addr].add(caller)
    else:
        if rd in pending and op not in (15, 14, 24):
            pending.pop(rd, None)

def all_edges(fn_va):
    return (forward_cg.get(fn_va,set()) | reverse_cg.get(fn_va,set()) |
            fn_ptr_fwd.get(fn_va,set()) | fn_ptr_rev.get(fn_va,set()))

# ── PASS SEED: Apply Manual + GTA IV Seeds ─────────────────────────────────────
print("\n=== PASS SEED: Manual bridge seeds + GTA IV cross-match ===")
seed_count = 0
for addr, (name, reason) in MANUAL_SEEDS.items():
    if addr not in fn_set:
        print(f"  WARNING: 0x{addr:08X} not in fn_set")
        continue
    nm = sym_map.get(addr, '')
    if not nm.startswith('game_') and not nm.startswith('fn_8') and not nm.startswith('vt'):
        # For GTA IV seeds, override vt* names but not well-named ones
        if not nm.startswith('grc') and not nm.startswith('rage_'):
            print(f"  SKIP: 0x{addr:08X} already named '{nm}'")
            continue
    if propose(addr, name, reason, 'SEED', overwrite=True):
        parts = name.split('_')
        if parts:
            fn_class[addr] = parts[0]
        seed_count += 1
        print(f"  SEEDED: 0x{addr:08X} -> {name}")
print(f"  Seeds applied: {seed_count}")

# ── PASS 10: game_ Semi-Transparent BFS ───────────────────────────────────────
print("\n=== PASS 10: game_ semi-transparent BFS ===")

p10 = 0; p10r = 0

def bfs_vote_game_transparent(fn_va):
    cc = Counter()
    for c in forward_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls:
            cc[cls] += 1.0
        elif sym_map.get(c,'').startswith('game_') or new_names.get(c,'').startswith('game_'):
            for cc2 in (list(forward_cg.get(c,set())) + list(reverse_cg.get(c,set()))):
                if cc2 == fn_va: continue
                cls2 = get_class_clean(cc2)
                if cls2: cc[cls2] += 0.3
    for c in reverse_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls:
            cc[cls] += 0.8
        elif sym_map.get(c,'').startswith('game_') or new_names.get(c,'').startswith('game_'):
            for cc2 in (list(forward_cg.get(c,set())) + list(reverse_cg.get(c,set()))):
                if cc2 == fn_va: continue
                cls2 = get_class_clean(cc2)
                if cls2: cc[cls2] += 0.25
    for c in fn_ptr_fwd.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.7
    for c in fn_ptr_rev.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.5
    return cc

for iteration in range(60):
    rc = 0
    targets = (unnamed | game_named) - set(new_names.keys())
    for fn_va in targets:
        nm = sym_map.get(fn_va,'')
        is_game = nm.startswith('game_')
        is_fn8  = nm.startswith('fn_8')
        if not (is_game or is_fn8): continue
        if fn_va in new_names: continue

        cc = bfs_vote_game_transparent(fn_va)
        if not cc: continue
        dominant, score = cc.most_common(1)[0]
        total = sum(cc.values())
        ratio = score / total

        min_score = 1.5 if is_game else 2.0
        min_ratio = 0.20 if is_game else 0.15

        if score >= min_score and ratio >= min_ratio:
            safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant.split('_')[0] if '_' in dominant else dominant
            if is_fn8:
                tag = 'P10'
                nm_new = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}")
            else:
                tag = 'P10r'
                nm_new = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_g")
            if propose(fn_va, nm_new, f"game-transparent BFS score={score:.1f}/{total:.1f} class='{dominant}'", tag):
                fn_class[fn_va] = dominant
                rc += 1
                if is_fn8: p10 += 1
                else: p10r += 1

    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc} (fn8={p10} game_={p10r} cumulative)")

print(f"  P10 total: {p10} fn_8 named, {p10r} game_ reclassified")

# ── PASS 11: Dominant-Caller Threshold ────────────────────────────────────────
print("\n=== PASS 11: Dominant-caller threshold (>=5 callers, >=80% class) ===")
p11 = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    callers = reverse_cg.get(fn_va, set())
    if len(callers) < 5: continue
    cc = Counter()
    for c in callers:
        cls = get_class_clean(c)
        if cls: cc[cls] += 1
    if not cc: continue
    dominant, cnt = cc.most_common(1)[0]
    total_with_class = sum(cc.values())
    if total_with_class < 4: continue
    ratio = cnt / total_with_class
    if ratio >= 0.80:
        safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_dc")
        if propose(fn_va, nm, f"dominant-caller {cnt}/{total_with_class} ({ratio:.0%}) class='{dominant}'", 'P11'):
            fn_class[fn_va] = dominant; p11 += 1
print(f"  P11: {p11}")

# ── Re-run P4: BFS with all new seeds in place ─────────────────────────────────
print("\n=== P4 re-run: BFS with new seeds ===")
p4f=0; p4r=0; p4fw=0; p4rh=0

def bfs_class_vote(fn_va):
    cc = Counter()
    for c in forward_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 1
    for c in fn_ptr_fwd.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.7
    for c in reverse_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.8
    for c in fn_ptr_rev.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.5
    return cc

for iteration in range(80):
    rc = 0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        cc = bfs_class_vote(fn_va)
        if not cc: continue
        dominant, score = cc.most_common(1)[0]
        total = sum(cc.values())
        ratio = score / total
        safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant.split('_')[0] if '_' in dominant else dominant
        if score >= 2.0 and ratio >= 0.15:
            nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}")
            if propose(fn_va, nm, f"BFS re-run {score:.1f}/{total:.1f} '{dominant}'", 'P4f'):
                fn_class[fn_va]=dominant; rc+=1; p4f+=1
            continue
        fwd_calls = forward_cg.get(fn_va, set())
        rev_callers = reverse_cg.get(fn_va, set())
        if len(fwd_calls) == 1:
            cls = get_class_clean(list(fwd_calls)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_fw")
                if propose(fn_va, nm, f"single callee re-run '{dominant}'", 'P4fw'):
                    fn_class[fn_va]=dominant; rc+=1; p4fw+=1
                continue
        if len(rev_callers) == 1:
            cls = get_class_clean(list(rev_callers)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h")
                if propose(fn_va, nm, f"single caller re-run '{dominant}'", 'P4rh'):
                    fn_class[fn_va]=dominant; rc+=1; p4rh+=1
                continue
    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc} (f={p4f} fw={p4fw} rh={p4rh})")

print(f"  P4 re-run totals: fwd={p4f} fw={p4fw} rh={p4rh}")

# ── Re-run P6: 2-hop bridging with updated names ──────────────────────────────
print("\n=== P6 re-run: 2-hop bridging ===")
p6=0; p6r=0
for fn_va in unnamed:
    if fn_va in new_names: continue
    neighbors = all_edges(fn_va)
    if not neighbors: continue
    named_1hop = [n for n in neighbors if not sym_map.get(n,'').startswith('fn_8') and not new_names.get(n,'').startswith('fn_8')]
    if named_1hop: continue
    two_hop = Counter()
    for mid in neighbors:
        for far in all_edges(mid):
            if far == fn_va: continue
            cls = get_class_clean(far)
            if cls: two_hop[cls] += 1
    if not two_hop: continue
    dom, cnt = two_hop.most_common(1)[0]
    total = sum(two_hop.values())
    ratio = cnt / total
    safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom
    if cnt >= 2 and ratio >= 0.40:
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_2h")
        if propose(fn_va, nm, f"2-hop re-run '{dom}' {cnt}/{total}", 'P6'):
            fn_class[fn_va]=dom; p6+=1
    elif cnt >= 1 and ratio >= 0.30:
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_2hr")
        if propose(fn_va, nm, f"2-hop relaxed re-run '{dom}' {cnt}/{total}", 'P6r'):
            fn_class[fn_va]=dom; p6r+=1
print(f"  P6 re-run: {p6} strict, {p6r} relaxed")

# ── Final Summary ──────────────────────────────────────────────────────────────
uf    = sum(1 for v in new_names if v in unnamed)
total = len(new_names)
pc    = Counter(pass_tag.values())
remaining = len(unnamed) - uf

# Math verification (Python-verified)
n_start = len(unnamed)
n_total_fns = len(fn_addrs)
n_named_before = n_total_fns - n_start
n_named_after  = n_named_before + uf
pct_before = n_named_before / n_total_fns * 100
pct_after  = n_named_after  / n_total_fns * 100

print(f"\n{'='*70}")
print(f"auto_namer_v10 Results")
print(f"{'='*70}")
for tag in ['SEED','P10','P10r','P11','P4f','P4fw','P4rh','P6','P6r']:
    if pc.get(tag,0): print(f"  {tag:<8s}: {pc.get(tag,0):5d}")
print(f"  {'─'*36}")
print(f"  Total renames    : {total:5d}")
print(f"  fn_8 fixed       : {uf:5d}")
print(f"  Remaining fn_8   : {remaining:5d}")
print(f"  Coverage before  : {pct_before:.1f}% ({n_named_before}/{n_total_fns})")
print(f"  Coverage after   : {pct_after:.1f}% ({n_named_after}/{n_total_fns})")
print(f"  Coverage gain    : +{pct_after-pct_before:.2f}%")
print(f"  game_ reclassified: {pc.get('P10r',0):5d}")
print(f"{'='*70}")

# ── Write Output ───────────────────────────────────────────────────────────────
print(f"\nWriting output to {OUT}...")
out_lines = []
for line in sym_lines:
    s = line.strip(); matched = False
    for sect in ['.text','.rdata','.data']:
        if '= '+sect+':0x' in s:
            parts = s.split('=',1)
            name  = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1],16)
                if addr in new_names:
                    out_lines.append(line.replace(name, new_names[addr], 1))
                    matched = True; break
            except: pass
    if not matched: out_lines.append(line)
with open(OUT,'w') as f: f.writelines(out_lines)

# ── Write Report ───────────────────────────────────────────────────────────────
with open(REPORT,'w') as f:
    f.write(f"auto_namer_v10 Results\n{'='*70}\n\n")
    f.write(f"Input:  symbols_autonamed_v9.txt  ({pct_before:.1f}% coverage)\n")
    f.write(f"Output: symbols_autonamed_v10.txt ({pct_after:.1f}% coverage)\n\n")
    f.write(f"Manual seeds ({len(MANUAL_SEEDS)}):\n")
    for addr, (name, reason) in MANUAL_SEEDS.items():
        applied = "  [APPLIED]" if addr in new_names else "  [SKIPPED]"
        f.write(f"  0x{addr:08X}{applied}  {name:<55s}  {reason[:80]}\n")
    f.write(f"\n")
    for tag in ['SEED','P10','P10r','P11','P4f','P4fw','P4rh','P6','P6r']:
        f.write(f"Pass {tag:<8s}: {pc.get(tag,0):5d}\n")
    f.write(f"\nTotal renames:      {total}\n")
    f.write(f"fn_8 fixed:         {uf}\n")
    f.write(f"Remaining fn_8:     {remaining}\n")
    f.write(f"Coverage gain:      {pct_after-pct_before:.2f}% ({pct_before:.1f}% -> {pct_after:.1f}%)\n\n")
    f.write(f"{'='*70}\nDETAIL\n{'='*70}\n")
    for addr in sorted(new_names):
        tag = pass_tag[addr]
        was_fn8 = '*' if addr in unnamed else (' G' if sym_map.get(addr,'').startswith('game_') else '  ')
        f.write(f"  [{tag:6s}]{was_fn8} 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"Done! Report: {REPORT}")
