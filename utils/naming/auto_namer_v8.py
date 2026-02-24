#!/usr/bin/env python3
"""
auto_namer_v8.py — Stdlib-filtered BFS + fn-ptr edges + spatial locality + 2-hop upgrade
Reads symbols_autonamed_v7.txt → writes symbols_autonamed_v8.txt

New in v8:
  STDLIB_BLACKLIST  — eliminates memcpy/memset/savegprlr as false class signals
  get_class_clean() — replaces get_class() everywhere; generic prefix filter expanded
  P0c (NEW)         — Non-RTTI vtable class inference (quality renames)
  P4 UPGRADED       — BFS with stdlib filter, NO size cap on cnt=1 (single-caller)
                       + fn-ptr load edges included in voting
  P5 (NEW)          — fn-ptr load signal pass (lis+addi patterns, standalone)
  P6 UPGRADED       — 2-hop island bridging with fn-ptr edges, strict + relaxed tiers
  P7 (NEW)          — Spatial locality (compiler groups class methods contiguously)
  P9 UPGRADED       — Expanded tiny patterns (setter, stub, return_true/false)
"""
import re, os, struct, bisect, json
from collections import defaultdict, Counter

BASE   = '/Users/Ozordi/Downloads/tt-decomp'
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v7.txt')
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v8.txt')
REPORT = os.path.join(BASE, 'auto_namer_v8_report.txt')
RTTI   = os.path.join(BASE, 'rtti_vtable_map.json')

# ── Binary loading ────────────────────────────────────────────────────────────
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
rdata_chunk = data[rraw:rraw+rvsize]

def va_to_file(va):
    for _, abs_va, vsize, raw_off in sections:
        if abs_va <= va < abs_va+vsize: return raw_off+(va-abs_va)
    return None

def read_be32(va):
    off = va_to_file(va)
    if off is None or off+4 > len(data): return None
    return int.from_bytes(data[off:off+4],'big')

# ── Symbol loading ────────────────────────────────────────────────────────────
print("\nLoading symbols (from v7 output)...")
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
print(f"  Total: {len(fn_addrs)}, Unnamed (fn_8): {len(unnamed)}")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr+fn_offsets[idx]) if idx >= 0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

# ── STDLIB BLACKLIST (the critical v8 addition) ───────────────────────────────
STDLIB_BLACKLIST = {
    # C stdlib
    'memcpy','memset','malloc','free','realloc','calloc',
    'printf','sprintf','snprintf','vsnprintf','sscanf',
    'strlen','strcpy','strcat','strcmp','strncmp','strncpy',
    'memmove','memcmp','memchr','strchr','strrchr',
    'atexit','exit','abort','raise','longjmp','setjmp',
    'atoi','atol','atof','strtol','strtof','strtod',
    'qsort','bsearch','rand','srand',
    'fopen','fclose','fread','fwrite','fseek','ftell','fflush','feof','ferror',
    'fgets','fputs','puts','gets',
    # CRT/runtime
    'rage_free','rage_malloc','rage_stricmp',
    '__cxa_atexit','__cxa_pure_virtual','__pure_virtual',
    # Xbox kernel API stubs (single-line thunks, not class methods)
    'XGetVideoMode','XGetAVPackFlags','XGetGameRegion',
    'XInputGetState','XInputSetState',
} | {f'__savegprlr_{i}' for i in range(14,31)} \
  | {f'__restgprlr_{i}' for i in range(14,31)} \
  | {'__savefpr','__restfpr','__savevmx','__restvmx','__savevecreg','__restvecreg'}

GENERIC_PFX = frozenset({
    'game','ph','xe','ke','grc','aud','rage','jumptable','ref','nt',
    'util','thunk','vt','nop','const','get','nullvfn','fn','io',
    'stub','sp','2h','2hr','loc','pdata','BINK','set','return',
    'thunk','util',
})

# ── Naming state ──────────────────────────────────────────────────────────────
new_names  = {}
confidence = {}
pass_tag   = {}
conf_score = {}
used_names = set(sym_map.values())
fn_class   = {}

PASS_CONFIDENCE = {
    'P0':  100, 'P0b':  95, 'P0c':  92,
    'P1':   90, 'P1r':  88,
    'P2':   85, 'P3':   85,
    'P4f':  72, 'P4r':  70, 'P4fw': 57, 'P4rh': 60,
    'P5':   55, 'P5r':  50,
    'P6':   48, 'P6r':  38,
    'P7':   42,
    'P8':   52,
    'P9':   22,
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

# ── RTTI loading ──────────────────────────────────────────────────────────────
rtti_raw = json.load(open(RTTI))
rtti_map = {int(k,16): v for k,v in rtti_raw.items()}
rtti_va_set = set(rtti_map.keys())

# ── get_class_clean() — the core improvement over v7's get_class() ────────────
def get_class_clean(fn_va, extra_names=None):
    """Return the class prefix for fn_va, or None if not meaningful."""
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

# ── KNOWN_PREFIXES (same as v7, used for P1 string scan) ─────────────────────
KNOWN_PREFIXES = sorted([
    'pongLookAtController','pongCreatureInst','pongCreatureType','pongCreature',
    'pongAnimationInfo','pongHairData','pongLocomotionController','pongBlender',
    'pongBlendLookAtDriver','pongSkelLookAtDriver','pongLookAtDriver','pongMover',
    'pongPlayer','pongShell','pongFrontendState','pongCutSceneManager',
    'pongCSActionCamAnim','pongBallInstance','pongDrawBucketManager','pongDrawBucket',
    'pongGameContext','pongLiveManager','pongPresence','pongNetMessageHolder',
    'pongBlendShapeController','pongCSAction',
    'pongDrillCounterSpin','pongDrillSmash','pongDrillSoftShot',
    'pongDrillSpin','pongLadder','pongPlayerShaderParams',
    'pongRemotePlayer','pongShadowMap','pongTrainingDrill',
    'pongInlineCinematics','pongAttractContext','pongScrnTransFreezeAndFadeOut',
    'pongCSActionLvlAmbAnim','pongCameraMgr','pongLerpQueue',
    'gdGameData','gdLadder','gdLadderMatch','gdDifficultyLadder','gdBallPredictData',
    'gdCameraDef','gdCameraInst','gdCameraDest','gdVibEvent','gdRivalry',
    'gdTierMember','gdTier','gdLadderCommonData','gdCSCharCamShotName',
    'gdCSCharAnimData','gdCSCharAnimNames','gdCSCamAnimShotName',
    'gdCSCamAnimShotNameSeq','gdCSNameData','gdCutSceneData','gdCSActionIfData',
    'gdCSActionCamAnimData','gdCSActionCharAnimData','gdCSActionCharVisibleData',
    'gdCSActionPlayAudioData','gdCSActionLvlAnimData','gdCSActionLvlAmbAnimData',
    'gdCSActionCharAmbAnimData','gdStatsMgr',
    'gdCSCharCamShotNameSeq','gdCrAttachmentData','gdCrData',
    'gdDrillChargingData','gdDrillCounterSpinData','gdDrillFocusShotData',
    'gdDrillMovementData','gdDrillServingData','gdDrillSoftShotData','gdDrillSpinData',
    'gdLevelData','gdRivalryData','gdTrainingDrillData','gdTourneySaveData','gdTourneySaveValues',
    'gmBallNode','gmLogicSinglesMatch','gmLogicSpectator','gmLogicSwingPractice',
    'hudHUD','hudTrainingHUD','hudLeaderboard','hudTrainingPopUp','hudTrainingLoadScreen',
    'fxAmbAnimSet','fxString','fxBallSpinLines','fxReticle',
    'fxAmbient','fxAmbientMgr','fxBallSpinTex','fxBallSplash2D',
    'fxBallSpinLineData','fxBallSpinTexData','fxCloth','fxHair',
    'fiDevice','fiStream','fiPackfile','fiAsciiTokenizer','fiBaseTokenizer',
    'fiDeviceLocal','fiDeviceTcpIp',
    'phBoundBox','phBoundGeometry','phBoundOctree','phBoundComposite','phBound',
    'phBoundCapsule','phBoundTaperedCapsule','phArchetype','phSimulator',
    'phLevelNew','phPolytree','phUpdateObject','phJoint3Dof','phJoint1Dof',
    'phConstrainedCollider','phArticulatedCollider','phInst','phClothVerletInst',
    'phBoundRibbon','phDemoWorld',
    'pcrFaceAnimBlender','pcrAnimObserved','pcrAnimObserver','pcrSwingSolver',
    'pcrSwingBlender','pcrServeBlender','pcrActivationCategory','pcrEmoteBlender',
    'pcrPostPointBlender','pcrAnimEvent','pcrAnimEventFootAud','pcrAnimEventMsg',
    'grmEdgeModel','grmModel','grmGeometry','grmDrawable','grmShader',
    'grmModelFactory','grmShaderFx',
    'grcTexture','grcTextureStore','grcRenderTarget','grcEffect',
    'grcVertexDeclaration','grcVertexBuffer','grcIndexBuffer','grcDevice',
    'grcTextureDictionaryReference',
    'pgBase','pgDictionary','pgStreamer',
    'datBase','datOwner','datRef',
    'crAnimDofVector3','crAnimDofQuaternion','crAnimDof','crSkelMeshAsset',
    'crAnimChannelQuantizeFloat','crSkeletonData',
    'crIKBodyBase','crIKHead','crIKLimb','crIKSpine',
    'crIKSolverIterativeLimb','crIKSolverGleicherLimb','crIKSolverSimpleSpine',
    'atSingleton','atArray','atDList','atSafeDLListSimple',
    'swfOBJECT','swfCMD','swfINSTANCE','swfBUTTON','swfSCRIPTOBJECT','SYMTAB',
    'swfCMD_DoInitAction',
    'cmOperatorCtor','cmRefreshableCtor','cmWorldRefreshableCtor','cmReporterCtor',
    'cmApproach2Ctor','cmWorldApproach2Ctor','cmNaryOperator','cmMetafileTuningSet',
    'cmCond','cmLookup','cmSwitch','cmLinearRemap','cmSmoothRemap',
    'cmDifferential','cmNormalizedTimer','cmMultiply','cmOr','cmAngleLerp',
    'cmPowerApproach','cmMemory','cmNamedValueSet','cmAnglePowerApproach',
    'cmMinPauseApproach','cmExpandPauseApproach','cmNormalProbe','cmDepthProbe',
    'cmSampleCamActions',
    'cvCurveNurbsWM','cvCurveNurbs','cvCurve',
    'hsmState','FloatAverager','Holder',
    'msgMsgSink','MatchData','NetTimer','netHardware',
    'NetBallHitManagerFreeList','PongBallHitDataStack',
    'AckHandling','SessionMessageRequester','PongNetGameModeCoordinator',
    'PongNetExhibitionCoordinator','PongNetRoundRobinCoordinator',
    'SinglesNetworkClient','SpectatorNetworkClient','NetworkClient',
    'HitMessage','BallHitMessage','SpectatorBallHitMessage','HitDataMessage',
    'ServeStartedMessage','ForceMatchTimeSyncMessage','ForceSessionTimeSyncMessage',
    'GamerReadyMessage','GamerUpdateMessage','Gamer',
    'CCalChannel','CCalMoviePlayer',
    'xmlNodeStructAnimHashEntryArray','xmlNodeStructAnimHashEntry',
    'xmlNodeStruct','xmlNodeStructVector2','xmlNodeStructMatrix34',
    'xmlTree','parStreamInRbf','parStreamInXml','parStreamOutRbf','parStreamOutXml',
    'parMemberArray','parStructure',
    'shShell','fsmMachine',
    'LocomotionStateAnim','LocomotionStateMf','LocomotionState',
    'Timer','audBank','audVoiceSfx','audVoiceStream','audControl3d','audVoice',
    'ballPredict','geomBoxes',
    'sysMemSimpleAllocator','rmPtxSystem','lvlTable',
    'CMtoCSAdapter','charViewCS','gmBallRobot','dcamPolarCam',
    'snSession','snJoinMachine','snLeaveMachine','snMigrateMachine',
    'snCreateMachine','snArbMachine','snSessionFinder','rlSession','rlGamer',
    'CBiquadFilterEffect','CCompEffect','CDelayEffect','CPeakMeterEffect','CShelvingFilterEffect',
    'NetStateSync','NetDataQuery','InternalMessageRelay','hsmContext',
    'gdUnlockConditionChar','gdUnlockConditionVersus',
], key=len, reverse=True)

# ── PASS 0: vt-Encoded Name Decode (carry-over from v7) ─────────────────────
print("\n=== PASS 0: vt-Encoded Name Decode ===")
lo16_map = defaultdict(list)
for vt_va, cls in rtti_map.items():
    lo16_map[vt_va & 0xFFFF].append((vt_va, cls))

VT_ENC = re.compile(r'^(?:game|ph|rage|xe|ke|grc|aud|io)_vt([0-9A-Fa-f]+)_(\d+)$')

p0 = 0
vt_decode_map = {}
for fn_va, nm in sym_map.items():
    m = VT_ENC.match(nm)
    if not m: continue
    vt_lo = int(m.group(1), 16)
    slot_idx = int(m.group(2))
    candidates = lo16_map.get(vt_lo, [])
    if not candidates: continue
    resolved = None
    if len(candidates) == 1:
        resolved = candidates[0]
    else:
        for rtti_va, cls in candidates:
            slot_fn = read_be32(rtti_va + slot_idx * 4)
            if slot_fn == fn_va:
                resolved = (rtti_va, cls); break
    if not resolved: continue
    rtti_va, cls = resolved
    safe_cls = rtti_class_to_prefix(cls)
    new_nm = safe_name(f"{safe_cls}_vfn_{slot_idx}")
    vt_decode_map[fn_va] = (new_nm, cls, rtti_va)
    fn_class[fn_va] = cls
    if propose(fn_va, new_nm, f"vt-decoded from {nm} class={cls}", 'P0', overwrite=True):
        p0 += 1
print(f"  P0: {p0} vt-encoded names decoded")

# ── PASS 0b: Full RTTI Vtable Seeding (carry-over from v7) ──────────────────
print("\n=== PASS 0b: RTTI Full Vtable Seeding ===")
vtable_members   = defaultdict(list)
vtable_slots_map = defaultdict(list)
seeded = 0
p0b = 0
for vt_va, cls in sorted(rtti_map.items()):
    for i in range(60):
        fn_va = read_be32(vt_va + i*4)
        if fn_va is None: break
        if not (tvaddr <= fn_va < tvaddr+tvsize): break
        if fn_va not in fn_class:
            fn_class[fn_va] = cls; seeded += 1
        vtable_slots_map[fn_va].append((vt_va, i, cls))
        vtable_members[vt_va].append(fn_va)
    for i, fn_va in enumerate(vtable_members.get(vt_va, [])):
        nm = new_names.get(fn_va, sym_map.get(fn_va,''))
        if nm.startswith('fn_8') or fn_va in vt_decode_map: continue
        if get_class_clean(fn_va): continue
        safe_cls = rtti_class_to_prefix(cls)
        new_nm   = safe_name(f"{safe_cls}_vfn_{i}")
        if propose(fn_va, new_nm, f"RTTI vtable slot {i} class={cls}", 'P0b', overwrite=True):
            p0b += 1
print(f"  Seeded {seeded} fn_class entries, P0b: {p0b} reclassified")

# ── PASS 0c (NEW): Non-RTTI Vtable Class Inference ───────────────────────────
print("\n=== PASS 0c: Non-RTTI Vtable Class Inference ===")
# Detect vtable clusters in .rdata not in RTTI map
all_vtables = {}
cluster = []
for off in range(0, rvsize, 4):
    ptr = int.from_bytes(rdata_chunk[off:off+4],'big')
    if tvaddr <= ptr < tvaddr+tvsize and ptr in fn_set:
        cluster.append((rvaddr+off, ptr))
    else:
        if len(cluster) >= 3:
            vt_va = cluster[0][0]
            all_vtables[vt_va] = cluster[:]
        cluster = []
if len(cluster) >= 3:
    all_vtables[cluster[0][0]] = cluster[:]

non_rtti_vtables = {va: cl for va, cl in all_vtables.items() if va not in rtti_va_set}
p0c = 0
p0c_q = 0
for vt_va, members in non_rtti_vtables.items():
    classes = Counter()
    for rva, fn_va in members:
        cls = get_class_clean(fn_va)
        if cls: classes[cls] += 1
    if not classes: continue
    dom, cnt = classes.most_common(1)[0]
    if cnt < max(2, len(members) // 5): continue  # need >=20% agreement
    # Rename generic-named and fn_8 members
    for i, (rva, fn_va) in enumerate(members):
        nm = new_names.get(fn_va, sym_map.get(fn_va,''))
        if fn_va in vt_decode_map: continue
        if nm.startswith('fn_8'):
            safe_cls = rtti_class_to_prefix(dom)
            if propose(fn_va, safe_name(f"{safe_cls}_vfn_{i}"),
                       f"non-RTTI vtable at 0x{vt_va:08X} inferred class={dom}", 'P0c', overwrite=True):
                fn_class[fn_va] = dom; p0c += 1
        elif not get_class_clean(fn_va):
            safe_cls = rtti_class_to_prefix(dom)
            if propose(fn_va, safe_name(f"{safe_cls}_vfn_{i}"),
                       f"non-RTTI vtable quality rename class={dom}", 'P0c', overwrite=True):
                p0c_q += 1

print(f"  P0c: {p0c} fn_8 named, {p0c_q} quality renames ({len(non_rtti_vtables)} non-RTTI vtables)")

# ── PASS 1: Class::Method String Scan ────────────────────────────────────────
print("\n=== PASS 1: String ref scan ===")
CLASS_METHOD = re.compile(rb'[A-Za-z][A-Za-z0-9_]{2,40}::[A-Za-z][A-Za-z0-9_]{1,60}')
hi_lookup = defaultdict(list)
for m in CLASS_METHOD.finditer(rdata_chunk):
    if m.start() > 0 and rdata_chunk[m.start()-1] != 0: continue
    s = m.group().decode('ascii','replace')
    if '::' not in s: continue
    cls, meth = s.split('::',1)
    str_va = rvaddr + m.start()
    hi = ((str_va+0x8000)>>16)&0xFFFF
    lo = str_va & 0xFFFF
    hi_lookup[hi].append((lo, cls, meth))

pending = {}; hits = []
for i in range(0, tvsize, 4):
    w  = int.from_bytes(data[traw+i:traw+i+4],'big')
    op = (w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
    if op==15 and ra==0:
        if imm in hi_lookup: pending[rd]=(imm,i)
        else: pending.pop(rd,None)
    elif op==14:
        if ra in pending:
            hi_val, lis_off = pending[ra]
            for match_lo, cls, meth in hi_lookup.get(hi_val,[]):
                if imm == match_lo:
                    caller_va = owning_fn(i)
                    if caller_va: hits.append((caller_va,cls,meth))
        pending.pop(rd,None)
    else:
        if rd in pending and op not in (15,14): pending.pop(rd,None)

p1=0; p1r=0
for caller_va, cls, meth in hits:
    fn_nm = safe_name(f"{cls}_{meth}")
    if caller_va not in fn_class: fn_class[caller_va] = cls
    old = sym_map.get(caller_va,'')
    if old.startswith('fn_8'):
        if propose(caller_va, fn_nm, f"string ref '{cls}::{meth}'", 'P1'):
            fn_class[caller_va]=cls; p1+=1
    elif old and old != fn_nm:
        if fn_nm not in used_names:
            new_names[caller_va]=fn_nm; confidence[caller_va]=f"string ref (was {old})"
            pass_tag[caller_va]='P1r'; conf_score[caller_va]=88
            used_names.add(fn_nm); fn_class[caller_va]=cls; p1r+=1
print(f"  P1: {p1} new, P1r: {p1r} renames")

# ── PASS 2: Thunks ────────────────────────────────────────────────────────────
print("\n=== PASS 2: Thunks ===")
p2 = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    if fn_meta.get(fn_va,0) != 4: continue
    off = va_to_file(fn_va)
    if not off: continue
    w = int.from_bytes(data[off:off+4],'big')
    if (w>>26)&0x3F==18 and not (w&1):
        li = w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        tgt  = fn_va + li
        tnm  = new_names.get(tgt,'') or sym_map.get(tgt,'')
        if tnm and not tnm.startswith('fn_8'):
            if propose(fn_va, safe_name(f"thunk_{tnm[:50]}"), f"thunk->0x{tgt:08X}", 'P2'):
                p2 += 1
                cls = fn_class.get(tgt)
                if cls: fn_class[fn_va] = cls
print(f"  P2: {p2}")

# ── Build Callgraphs (bl-based) ───────────────────────────────────────────────
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

# ── Build fn-ptr Load Edges (lis+addi patterns) ───────────────────────────────
print("\nBuilding fn-ptr load edges (lis+addi)...")
fn_ptr_fwd = defaultdict(set)   # fn_va -> set of fn addresses it loads as data
fn_ptr_rev = defaultdict(set)   # fn_va -> set of fn addresses that load it
pending = {}
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    op=(w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
    if op==15 and ra==0:           # lis rD, hi
        signed_hi = imm if imm < 0x8000 else imm - 0x10000
        pending[rd] = signed_hi << 16
    elif op==14 and ra in pending: # addi rD, rA, lo (signed)
        lo = imm if imm < 0x8000 else imm - 0x10000
        addr = (pending[ra] + lo) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr+tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr)
                fn_ptr_rev[addr].add(caller)
    elif op==24 and ra in pending: # ori rD, rA, lo (unsigned)
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

total_ptr_edges = sum(len(v) for v in fn_ptr_fwd.values())
print(f"  fn-ptr forward edges: {total_ptr_edges} from {len(fn_ptr_fwd)} functions")

# ── PASS 3: Constructor Detection ────────────────────────────────────────────
print("\n=== PASS 3: Constructors ===")
rtti_hi_lo = {}
for vt_va in rtti_map:
    hi = ((vt_va+0x8000)>>16)&0xFFFF
    lo = vt_va & 0xFFFF
    rtti_hi_lo[(hi,lo)] = vt_va
p3 = 0; i = 0
while i < tvsize-16:
    w0  = int.from_bytes(data[traw+i:traw+i+4],'big')
    op0 = (w0>>26)&0x3F; rd0=(w0>>21)&0x1F; ra0=(w0>>16)&0x1F; imm0=w0&0xFFFF
    if op0==15 and ra0==0 and imm0 in {k[0] for k in rtti_hi_lo}:
        for j in range(1,8):
            if i+j*4+4 > tvsize: break
            w1  = int.from_bytes(data[traw+(i+j*4):traw+(i+j*4)+4],'big')
            op1 = (w1>>26)&0x3F; rd1=(w1>>21)&0x1F; ra1=(w1>>16)&0x1F; imm1=w1&0xFFFF
            if op1 in (14,24) and ra1==rd0:
                lo = imm1
                if (imm0,lo) in rtti_hi_lo:
                    vt_va = rtti_hi_lo[(imm0,lo)]; cls_name = rtti_map[vt_va]
                    for k in range(j+1,j+6):
                        if i+k*4+4 > tvsize: break
                        w2  = int.from_bytes(data[traw+(i+k*4):traw+(i+k*4)+4],'big')
                        op2 = (w2>>26)&0x3F; rs2=(w2>>21)&0x1F; ra2=(w2>>16)&0x1F
                        if op2 in (36,37) and rs2==rd1 and ra2==3:
                            caller_va = owning_fn(i)
                            if caller_va and caller_va in unnamed and caller_va not in new_names:
                                safe_cls = rtti_class_to_prefix(cls_name)
                                if propose(caller_va, safe_name(f"{safe_cls}_ctor_{caller_va&0xFFFF:04X}"),
                                           f"stores vtable 0x{vt_va:08X} ({cls_name})", 'P3'):
                                    fn_class[caller_va]=cls_name; p3+=1
                            break
                break
    i += 4
print(f"  P3: {p3} constructors")

# ── PASS 4: Upgraded BFS — Stdlib-filtered, no size cap, fn-ptr edges ────────
print("\n=== PASS 4: BFS (stdlib-filtered, fn-ptr augmented, no size cap) ===")
p4f=0; p4r=0; p4fw=0; p4rh=0

def bfs_class_vote(fn_va):
    """Get class votes for fn_va from all edge types."""
    cc = Counter()
    # Forward bl-calls (what does fn_va call?)
    for c in forward_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 1
    # fn-ptr loads (what fn addresses does fn_va load?)
    for c in fn_ptr_fwd.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.7  # slight downweight for ptr loads
    # Reverse bl-calls (who calls fn_va?)
    for c in reverse_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls: cc[cls] += 0.8
    # fn-ptr reverse (who loads fn_va as pointer?)
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

        # Strong multi-vote signal
        if score >= 2.0 and ratio >= 0.15:
            tag = 'P4f'
            nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}")
            if propose(fn_va, nm, f"BFS {score:.1f}/{total:.1f} '{dominant}'", tag):
                fn_class[fn_va]=dominant; rc+=1
                if tag=='P4f': p4f+=1
                else: p4r+=1
            continue

        # Single meaningful signal — NO size cap (v8 critical change)
        fwd_calls = forward_cg.get(fn_va, set())
        rev_callers = reverse_cg.get(fn_va, set())
        # Single forward callee with class
        if len(fwd_calls) == 1:
            cls = get_class_clean(list(fwd_calls)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_fw")
                if propose(fn_va, nm, f"single callee '{dominant}' (no size cap)", 'P4fw'):
                    fn_class[fn_va]=dominant; rc+=1; p4fw+=1
                continue
        # Single caller with class
        if len(rev_callers) == 1:
            cls = get_class_clean(list(rev_callers)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h")
                if propose(fn_va, nm, f"single caller '{dominant}' (no size cap)", 'P4rh'):
                    fn_class[fn_va]=dominant; rc+=1; p4rh+=1
                continue

    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc}  (f={p4f} r={p4r} fw={p4fw} rh={p4rh})")

print(f"  P4 totals: fwd={p4f} rev={p4r} fw={p4fw} rh={p4rh}")

# ── PASS 5 (NEW): Standalone fn-ptr Signal Pass ────────────────────────────────
print("\n=== PASS 5: fn-ptr standalone signal ===")
p5=0; p5r=0

for fn_va in unnamed:
    if fn_va in new_names: continue
    # Only for functions with no bl-call edges
    if forward_cg.get(fn_va) or reverse_cg.get(fn_va): continue

    cc = Counter()
    # What does fn_va load as function pointers?
    for tgt in fn_ptr_fwd.get(fn_va, set()):
        cls = get_class_clean(tgt)
        if cls: cc[cls] += 1
    # Who loads fn_va as a function pointer?
    for src in fn_ptr_rev.get(fn_va, set()):
        cls = get_class_clean(src)
        if cls: cc[cls] += 0.5

    if not cc: continue
    dom, score = cc.most_common(1)[0]
    total = sum(cc.values())
    ratio = score / total

    if score >= 1.5 and ratio >= 0.50:
        safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_p")
        if propose(fn_va, nm, f"fn-ptr signal score={score:.1f} '{dom}'", 'P5'):
            fn_class[fn_va]=dom; p5+=1
    elif score >= 1.0 and total <= 2:
        safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_p")
        if propose(fn_va, nm, f"fn-ptr single signal '{dom}'", 'P5r'):
            fn_class[fn_va]=dom; p5r+=1

print(f"  P5: {p5} strong, P5r: {p5r} single-ptr")

# ── PASS 6 (NEW): Upgraded 2-Hop Island Bridging ─────────────────────────────
print("\n=== PASS 6: 2-hop island bridging (upgraded) ===")
p6=0; p6r=0

def all_edges(fn_va):
    return (forward_cg.get(fn_va,set()) | reverse_cg.get(fn_va,set()) |
            fn_ptr_fwd.get(fn_va,set()) | fn_ptr_rev.get(fn_va,set()))

for fn_va in unnamed:
    if fn_va in new_names: continue
    neighbors = all_edges(fn_va)
    if not neighbors: continue
    # Must have no directly-named 1-hop neighbors with class signal
    named_1hop = [n for n in neighbors if not sym_map.get(n,'').startswith('fn_8') and not new_names.get(n,'').startswith('fn_8')]
    if named_1hop: continue  # 1-hop should have been handled by P4/P5

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
        if propose(fn_va, nm, f"2-hop '{dom}' {cnt}/{total}", 'P6'):
            fn_class[fn_va]=dom; p6+=1
    elif cnt >= 1 and ratio >= 0.30:
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_2hr")
        if propose(fn_va, nm, f"2-hop relaxed '{dom}' {cnt}/{total}", 'P6r'):
            fn_class[fn_va]=dom; p6r+=1

print(f"  P6: {p6} strict, P6r: {p6r} relaxed")

# ── PASS 7 (NEW): Spatial Locality Naming ────────────────────────────────────
print("\n=== PASS 7: Spatial locality (compiler function grouping) ===")
# Pre-compute sorted list of (va, class) for all classified named functions
named_classified = []
for va in fn_addrs:
    nm = new_names.get(va, sym_map.get(va,''))
    if nm.startswith('fn_8'): continue
    cls = get_class_clean(va)
    if cls: named_classified.append((va, cls))
named_classified.sort()
nc_vas = [x[0] for x in named_classified]
print(f"  Named+classified functions: {len(named_classified)}")

p7 = 0
WINDOW = 4096    # ±4KB window
MIN_SCORE = 8    # need strong spatial consensus

for fn_va in unnamed:
    if fn_va in new_names: continue
    # Only for truly isolated (no bl or ptr edges)
    if forward_cg.get(fn_va) or reverse_cg.get(fn_va): continue
    if fn_ptr_fwd.get(fn_va) or fn_ptr_rev.get(fn_va): continue

    lo_idx = bisect.bisect_left(nc_vas, fn_va - WINDOW)
    hi_idx = bisect.bisect_right(nc_vas, fn_va + WINDOW)

    nearby = Counter()
    context_count = 0
    for j in range(lo_idx, hi_idx):
        cva, cls = named_classified[j]
        if cva == fn_va: continue
        dist = abs(cva - fn_va)
        if dist > WINDOW: continue
        context_count += 1
        weight = max(1, 10 - dist // 512)  # closer = more weight
        nearby[cls] += weight

    if not nearby or context_count < 2: continue
    dom, score = nearby.most_common(1)[0]
    total_score = sum(nearby.values())
    ratio = score / total_score

    if score >= MIN_SCORE and ratio >= 0.50:
        safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_sp")
        if propose(fn_va, nm, f"spatial score={score}/{total_score} class='{dom}'", 'P7'):
            fn_class[fn_va]=dom; p7+=1

print(f"  P7: {p7} spatially named")

# ── PASS 8: Utility Detection (refined with stdlib filter) ───────────────────
print("\n=== PASS 8: Utility Detection ===")
p8 = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    callers = reverse_cg.get(fn_va, set())
    if len(callers) < 3: continue
    classes = Counter()
    for c in callers:
        cls = get_class_clean(c)
        if cls: classes[cls] += 1
    if len(classes) >= 2:
        top = [c for c, _ in classes.most_common(2)]
        if propose(fn_va, f"util_{fn_va&0xFFFF:04X}",
                   f"utility called by {len(classes)} classes: {','.join(top[:2])}", 'P8'):
            p8 += 1
print(f"  P8: {p8}")

# ── PASS 9: Tiny Pattern Naming (expanded) ───────────────────────────────────
print("\n=== PASS 9: Tiny Pattern Naming ===")
p9 = 0
BLR_INST = 0x4E800020
B_MASK   = 0xFC000003
B_OP     = 0x48000000   # unconditional b (not bl)

for fn_va in unnamed:
    if fn_va in new_names: continue
    sz = fn_meta.get(fn_va, 0)
    if sz == 0 or sz > 32: continue
    off = va_to_file(fn_va)
    if not off: continue
    words = []
    for k in range(sz//4):
        if off+k*4+4 <= len(data):
            words.append(int.from_bytes(data[off+k*4:off+k*4+4],'big'))
    if not words: continue

    # Pattern: just blr → null virtual
    if words == [BLR_INST]:
        if propose(fn_va, f"nullvfn_{fn_va&0xFFFF:04X}", "isolated blr-only", 'P9'):
            p9 += 1; continue

    # 4-byte unconditional branch (not bl) → stub/trampoline
    if sz == 4 and len(words) == 1:
        if (words[0] & B_MASK) == B_OP:
            if propose(fn_va, f"stub_{fn_va&0xFFFF:04X}", "unconditional-b stub", 'P9'):
                p9 += 1; continue

    if words and words[-1] == BLR_INST:
        # li r3, 0; blr → return_false
        if len(words) == 2 and words[0] == 0x38600000:
            if propose(fn_va, f"return_false_{fn_va&0xFFFF:04X}", "li r3,0; blr", 'P9'):
                p9 += 1; continue
        # li r3, 1; blr → return_true
        if len(words) == 2 and words[0] == 0x38600001:
            if propose(fn_va, f"return_true_{fn_va&0xFFFF:04X}", "li r3,1; blr", 'P9'):
                p9 += 1; continue
        # li r3, N; blr → const returner
        if len(words) == 2 and (words[0] & 0xFFE00000) == 0x38600000:
            if propose(fn_va, f"const_ret_{fn_va&0xFFFF:04X}", "li r3,N; blr", 'P9'):
                p9 += 1; continue
        # lwz/lhz/lbz r3, N(r3); blr → getter
        if len(words) == 2 and (words[0]>>26)&0x3F in (32,40,34,42,48,50):
            if (words[0]>>21)&0x1F == 3:  # dest is r3
                if propose(fn_va, f"get_{fn_va&0xFFFF:04X}", "load r3; blr (getter)", 'P9'):
                    p9 += 1; continue
        # stw/sth/stb rX, N(r3); blr → setter
        if len(words) == 2 and (words[0]>>26)&0x3F in (36,44,38,46,52,54):
            if (words[0]>>16)&0x1F == 3:  # base is r3
                if propose(fn_va, f"set_{fn_va&0xFFFF:04X}", "store to r3; blr (setter)", 'P9'):
                    p9 += 1; continue
        # 3-word: mflr/stwu + body + blr
        if len(words) >= 2 and len(words) <= 4:
            # Check for single load + blr pattern (getter variant)
            if (words[-2]>>26)&0x3F in (32,40,34) and (words[-2]>>21)&0x1F == 3:
                if propose(fn_va, f"get_{fn_va&0xFFFF:04X}", "load r3+blr (getter multi)", 'P9'):
                    p9 += 1; continue

print(f"  P9: {p9}")

# ── Final Summary ─────────────────────────────────────────────────────────────
uf    = sum(1 for v in new_names if v in unnamed)
total = len(new_names)
pc    = Counter(pass_tag.values())
remaining = len(unnamed) - uf

print(f"\n{'='*70}")
print(f"auto_namer_v8 Results")
print(f"{'='*70}")
for tag in ['P0','P0b','P0c','P1','P1r','P2','P3','P4f','P4r','P4fw','P4rh',
            'P5','P5r','P6','P6r','P7','P8','P9']:
    if pc.get(tag,0): print(f"  {tag:<8s}: {pc.get(tag,0):5d}")
print(f"  {'─'*36}")
print(f"  Total renames    : {total:5d}")
print(f"  fn_8 fixed       : {uf:5d}")
print(f"  Remaining fn_8   : {remaining:5d}")
pct_fixed = (uf / max(len(unnamed),1)) * 100
print(f"  Coverage gain    : {pct_fixed:.1f}% of starting fn_8")
named_total = len([a for a in fn_addrs if not sym_map.get(a,'').startswith('fn_8')]) + uf
total_fns = len(fn_addrs)
pct_coverage = (named_total / total_fns) * 100
print(f"  Text-section cov : {pct_coverage:.1f}% ({named_total}/{total_fns})")
print(f"  Quality renames  : {total-uf:5d}  (existing names improved)")
print(f"{'='*70}")

# ── Write Output ──────────────────────────────────────────────────────────────
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

# ── Write Report ──────────────────────────────────────────────────────────────
with open(REPORT,'w') as f:
    f.write(f"auto_namer_v8 Results\n{'='*70}\n\n")
    f.write(f"Input:  symbols_autonamed_v7.txt\n")
    f.write(f"Output: symbols_autonamed_v8.txt\n\n")
    for tag in ['P0','P0b','P0c','P1','P1r','P2','P3','P4f','P4r','P4fw','P4rh',
                'P5','P5r','P6','P6r','P7','P8','P9']:
        f.write(f"Pass {tag:<8s}: {pc.get(tag,0):5d}\n")
    f.write(f"\nTotal renames:    {total}\n")
    f.write(f"fn_8 fixed:       {uf}\n")
    f.write(f"Remaining fn_8:   {remaining}\n")
    f.write(f"Quality renames:  {total-uf}\n\n")
    f.write(f"{'='*70}\nDETAIL\n{'='*70}\n")
    for addr in sorted(new_names):
        tag = pass_tag[addr]
        was_fn8 = '*' if addr in unnamed else ' '
        f.write(f"  [{tag:5s}]{was_fn8} 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"Done! Report: {REPORT}")
print(f"To apply: cp {OUT} {SYMS.replace('autonamed_v7','')}")
