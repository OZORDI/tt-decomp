#!/usr/bin/env python3
"""
auto_namer_v7.py — RTTI-driven reclassification + island bridging
Reads from symbols_autonamed.txt (v6 output), writes symbols_autonamed_v7.txt.

New in v7:
  P0   — vt-encoded name decode: game_vtXXXX_N → ClassName_vfn_N (3,633 quality renames)
  P0b  — Full RTTI vtable class seeding (30K+ fn_class seeds vs v6's ~5K)
  P9   — 2-hop island bridging (fn_8 → fn_8 → named → propagate back)
  P11  — Tiny pattern naming (null virtuals, const returners)
  Expanded KNOWN_PREFIXES (42 more classes)
  Confidence-based overwrite (P0 overwrites generic vt-encoded names)
"""
import re, os, struct, bisect, json
from collections import defaultdict, Counter

BASE   = '/Users/Ozordi/Downloads/tt-decomp'
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols_autonamed.txt')   # v6 output
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v7.txt')
REPORT = os.path.join(BASE, 'auto_namer_v7_report.txt')
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
print("\nLoading symbols (from v6 output)...")
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
unnamed    = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
print(f"  Total: {len(fn_addrs)}, Unnamed: {len(unnamed)}")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr+fn_offsets[idx]) if idx >= 0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

# ── Naming state ──────────────────────────────────────────────────────────────
new_names   = {}    # fn_va -> proposed name
confidence  = {}    # fn_va -> reason string
pass_tag    = {}    # fn_va -> tag string
conf_score  = {}    # fn_va -> numeric confidence (for overwrite logic)
used_names  = set(sym_map.values())
fn_class    = {}    # fn_va -> class_name (best known class)

PASS_CONFIDENCE = {
    'P0':  100,   # vt-decoded — structural, unambiguous
    'P0b':  95,   # direct RTTI vtable membership rename
    'P1':   90,   # Class::Method string ref
    'P1r':  88,
    'P2':   85,   # thunk
    'P3':   85,   # constructor
    'P4f':  70,   # tier-1 forward BFS
    'P4r':  68,   # tier-1 reverse BFS
    'P4fw': 55,   # tiny forward
    'P4rh': 58,   # single-caller helper
    'P7':   65,   # standalone class string
    'P7r':  60,
    'P7rh': 55,
    'P9':   45,   # 2-hop island
    'P10':  50,   # utility
    'P11':  20,   # tiny pattern
}

def propose(fn_va, name, reason, tag, overwrite=False):
    new_conf = PASS_CONFIDENCE.get(tag, 50)
    if fn_va in new_names:
        if not overwrite: return False
        old_conf = conf_score.get(fn_va, -1)
        if new_conf <= old_conf: return False
        # Remove old name from used_names
        used_names.discard(new_names[fn_va])
    # Try name, add suffix if collision
    original = name
    for attempt in range(15):
        candidate = name if attempt == 0 else f"{original}_{fn_va&0xFFFF:04X}_{attempt}"
        if candidate not in used_names or fn_va in new_names and new_names[fn_va] == candidate:
            new_names[fn_va]  = candidate
            confidence[fn_va] = reason
            pass_tag[fn_va]   = tag
            conf_score[fn_va] = new_conf
            used_names.add(candidate)
            return True
    return False

# ── KNOWN_PREFIXES (expanded with 42 new classes) ────────────────────────────
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
    'ServeStartedMessage','SpectatorBallHitMessage',
    'ForceMatchTimeSyncMessage','ForceSessionTimeSyncMessage',
    'GamerReadyMessage','GamerUpdateMessage','Gamer',
    'CCalChannel','CCalMoviePlayer',
    'xmlNodeStructAnimHashEntryArray','xmlNodeStructAnimHashEntry',
    'xmlNodeStruct','xmlNodeStructVector2','xmlNodeStructMatrix34',
    'xmlTree','parStreamInRbf','parStreamInXml','parStreamOutRbf','parStreamOutXml',
    'parMemberArray','parStructure',
    'shShell','fsmMachine',
    'LocomotionStateAnim','LocomotionStateMf','LocomotionState',
    'Timer','audBank','audVoiceSfx','audVoiceStream','audControl3d',
    'audVoice','audBank',
    'ballPredict','geomBoxes','grmModelFactory',
    'sysMemSimpleAllocator',
    'rmPtxSystem','lvlTable','pg',
    'CMtoCSAdapter','charViewCS','gmBallRobot',
    'hudHUD','dcamPolarCam',
    'snSession','snJoinMachine','snLeaveMachine','snMigrateMachine',
    'snCreateMachine','snArbMachine','snSessionFinder','rlSession','rlGamer',
    'CBiquadFilterEffect','CCompEffect','CDelayEffect','CPeakMeterEffect','CShelvingFilterEffect',
    'NetStateSync','NetDataQuery','InternalMessageRelay','hsmContext',
    'PongNetGameModeCoordinator','gdUnlockConditionChar','gdUnlockConditionVersus',
], key=len, reverse=True)

GENERIC_PFX = {'game','ph','xe','ke','grc','aud','rage','jumptable','ref','nt',
               'util','thunk','fn','vt'}

def extract_class(nm):
    if not nm or nm.startswith('fn_8'): return None
    nl = nm.lower()
    for cls in KNOWN_PREFIXES:
        if nl.startswith(cls.lower()+'_') or nl == cls.lower():
            return cls
    parts = nm.split('_')
    if len(parts) >= 2 and parts[0] and parts[0] not in GENERIC_PFX:
        return parts[0]
    return None

def get_class(fn_va):
    if fn_va in fn_class: return fn_class[fn_va]
    nm = new_names.get(fn_va, sym_map.get(fn_va,''))
    return extract_class(nm)

# ── PASS 0: vt-Encoded Name Decode ───────────────────────────────────────────
print("\n=== PASS 0: vt-Encoded Name Decode ===")
rtti_raw = json.load(open(RTTI))
rtti_map = {int(k,16): v for k,v in rtti_raw.items()}

# Build lo16 lookup
lo16_map = defaultdict(list)
for vt_va, cls in rtti_map.items():
    lo16_map[vt_va & 0xFFFF].append((vt_va, cls))

VT_ENC = re.compile(r'^(?:game|ph|rage|xe|ke|grc|aud)_vt([0-9A-Fa-f]+)_(\d+)$')

def rtti_class_to_prefix(cls):
    parts = cls.split('::')
    # Keep full path for nested: "rage::snSession::snRoot" -> "snSession_snRoot"
    # But for common case just use last part
    return safe_name(parts[-1]) if len(parts) <= 2 else safe_name('_'.join(parts[1:]))

p0 = 0; p0b = 0
vt_decode_map = {}  # fn_va -> (new_name, class, rtti_va)

for fn_va, nm in sym_map.items():
    m = VT_ENC.match(nm)
    if not m: continue
    vt_lo     = int(m.group(1), 16)
    slot_idx  = int(m.group(2))
    candidates = lo16_map.get(vt_lo, [])
    if not candidates: continue

    resolved = None
    if len(candidates) == 1:
        resolved = candidates[0]
    else:
        for rtti_va, cls in candidates:
            slot_fn = read_be32(rtti_va + slot_idx * 4)
            if slot_fn == fn_va:
                resolved = (rtti_va, cls)
                break

    if not resolved: continue
    rtti_va, cls = resolved
    safe_cls  = rtti_class_to_prefix(cls)
    new_nm    = safe_name(f"{safe_cls}_vfn_{slot_idx}")

    vt_decode_map[fn_va] = (new_nm, cls, rtti_va)
    fn_class[fn_va] = cls

    # Propose with overwrite=True — P0 overrides existing generic vt-names
    if propose(fn_va, new_nm, f"vt-decoded from {nm} class={cls}", 'P0', overwrite=True):
        p0 += 1

print(f"  P0: {p0} vt-encoded names decoded")

# ── PASS 0b: Full RTTI Vtable Seeding ────────────────────────────────────────
print("\n=== PASS 0b: RTTI Full Vtable Seeding ===")
vtable_members   = defaultdict(list)  # vt_va -> [fn_va, ...]
vtable_slots_map = defaultdict(list)  # fn_va -> [(vt_va, idx, cls), ...]
seeded = 0

for vt_va, cls in sorted(rtti_map.items()):
    for i in range(50):
        fn_va = read_be32(vt_va + i*4)
        if fn_va is None: break
        if not (tvaddr <= fn_va < tvaddr+tvsize): break
        if fn_va not in fn_class:
            fn_class[fn_va] = cls
            seeded += 1
        vtable_slots_map[fn_va].append((vt_va, i, cls))
        vtable_members[vt_va].append(fn_va)

    # For non-vt-encoded generic named functions in this vtable: propose better name
    for i, fn_va in enumerate(vtable_members.get(vt_va, [])):
        nm = new_names.get(fn_va, sym_map.get(fn_va,''))
        # Skip already-decoded (P0 handled them), skip fn_8, skip meaningful names
        if nm.startswith('fn_8') or fn_va in vt_decode_map: continue
        if extract_class(nm): continue  # already meaningful
        # Generic-named vtable function: propose class-based name
        safe_cls = rtti_class_to_prefix(cls)
        new_nm   = safe_name(f"{safe_cls}_vfn_{i}")
        if propose(fn_va, new_nm, f"RTTI vtable slot {i} class={cls}", 'P0b', overwrite=True):
            p0b += 1

print(f"  Seeded {seeded} new fn_class entries (total: {len(fn_class)})")
print(f"  P0b: {p0b} generic vtable functions reclassified")

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

# ── Build Callgraphs ──────────────────────────────────────────────────────────
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
        if caller_va and tgt_va in sym_map:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
print(f"  forward edges from {len(forward_cg)} functions")

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

# ── PASS 4: Tier-1 BFS (multi-round forward+reverse) ─────────────────────────
print("\n=== PASS 4: Tier-1 BFS (RTTI-seeded) ===")
p4f=0; p4r=0; p4fw=0; p4rh=0

for iteration in range(60):
    rc = 0
    # Forward: fn_va calls many class-X functions → fn_va is class X
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees = forward_cg.get(fn_va, set())
        if not callees: continue
        cc = Counter()
        for c in callees:
            cls = get_class(c)
            if cls: cc[cls] += 1
        if not cc: continue
        dominant, cnt = cc.most_common(1)[0]
        n = len(callees)
        safe_cls = rtti_class_to_prefix(dominant)
        if cnt >= 2 and cnt/n >= 0.20:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"fwd {cnt}/{n} '{dominant}'", 'P4f'):
                fn_class[fn_va]=dominant; rc+=1; p4f+=1
        elif cnt == 1 and fn_meta.get(fn_va,0) <= 160:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_w"),
                       f"fwd tiny '{dominant}' sz={fn_meta.get(fn_va,0)}", 'P4fw'):
                fn_class[fn_va]=dominant; rc+=1; p4fw+=1
    # Reverse: fn_va called by many class-X functions → fn_va is class X
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callers = reverse_cg.get(fn_va, set())
        if not callers: continue
        cc = Counter()
        for c in callers:
            cls = get_class(c)
            if cls: cc[cls] += 1
        if not cc: continue
        dominant, cnt = cc.most_common(1)[0]
        n = len(callers)
        safe_cls = rtti_class_to_prefix(dominant)
        if cnt >= 2 and cnt/n >= 0.28:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"rev {cnt}/{n} '{dominant}'", 'P4r'):
                fn_class[fn_va]=dominant; rc+=1; p4r+=1
        elif cnt == 1 and n == 1 and fn_meta.get(fn_va,0) <= 160:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h"),
                       f"priv helper '{dominant}' sz={fn_meta.get(fn_va,0)}", 'P4rh'):
                fn_class[fn_va]=dominant; rc+=1; p4rh+=1
    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc}  (fwd={p4f} rev={p4r} fw={p4fw} rh={p4rh})")

# ── PASS 7: Standalone Class-Name String Scan ────────────────────────────────
print("\n=== PASS 7: Standalone class-name string scan + re-BFS ===")
# Build a UNIFIED hi→[(lo,cls)] map across ALL classes — single text pass
# (old approach did one full .text scan per class: ~200 × 4.7M iters = too slow)
unified_hi_map = defaultdict(list)   # hi → [(lo, cls), ...]
unified_hi_set = set()
for cls in KNOWN_PREFIXES:
    if len(cls) < 5: continue
    pat = cls.encode('ascii')
    off = 0
    while True:
        pos = rdata_chunk.find(pat, off)
        if pos == -1: break
        end = pos + len(pat)
        if end < len(rdata_chunk) and rdata_chunk[end] not in (0, ord(':'), ord('_'), ord(' ')):
            off = pos+1; continue
        if pos > 0 and rdata_chunk[pos-1] not in (0, 10, 13):
            off = pos+1; continue
        str_va = rvaddr + pos
        hi = ((str_va+0x8000)>>16)&0xFFFF
        lo = str_va & 0xFFFF
        unified_hi_map[hi].append((lo, cls))
        unified_hi_set.add(hi)
        off = pos+1

# Single pass through .text  — O(tvsize) not O(classes × tvsize)
pending = {}
cls_code_hits = []  # (caller_va, cls)
for i in range(0, tvsize, 4):
    w  = int.from_bytes(data[traw+i:traw+i+4],'big')
    op = (w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
    if op==15 and ra==0:
        if imm in unified_hi_set: pending[rd] = (imm, i)
        else: pending.pop(rd, None)
    elif op==14:
        if ra in pending:
            hi_val, _ = pending[ra]
            for lo, cls in unified_hi_map.get(hi_val, []):
                if imm == lo:
                    caller_va = owning_fn(i)
                    if caller_va: cls_code_hits.append((caller_va, cls))
        pending.pop(rd, None)
    else:
        if rd in pending and op not in (15, 14): pending.pop(rd, None)

p7=0; p7r=0; p7rh=0
# Process hits
class_code_count = Counter()
for caller_va, cls in cls_code_hits:
    class_code_count[(caller_va,cls)] += 1

for (caller_va, cls), cnt in class_code_count.items():
    if caller_va not in fn_class: fn_class[caller_va] = cls

# BFS with class-string context
for iteration in range(30):
    rc = 0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees = forward_cg.get(fn_va, set())
        if not callees: continue
        cc = Counter()
        for c in callees:
            cls = get_class(c)
            if cls: cc[cls] += 1
        if not cc: continue
        dominant, cnt = cc.most_common(1)[0]
        n = len(callees)
        safe_cls = rtti_class_to_prefix(dominant)
        if cnt >= 2 and cnt/n >= 0.18:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"fwd {cnt}/{n} '{dominant}'", 'P7'):
                fn_class[fn_va]=dominant; rc+=1; p7+=1
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callers = reverse_cg.get(fn_va, set())
        if not callers: continue
        cc = Counter()
        for c in callers:
            cls = get_class(c)
            if cls: cc[cls] += 1
        if not cc: continue
        dominant, cnt = cc.most_common(1)[0]
        n = len(callers)
        safe_cls = rtti_class_to_prefix(dominant)
        if cnt >= 2 and cnt/n >= 0.25:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"rev {cnt}/{n} '{dominant}'", 'P7r'):
                fn_class[fn_va]=dominant; rc+=1; p7r+=1
        elif cnt == 1 and n == 1 and fn_meta.get(fn_va,0) <= 160:
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h"),
                       f"P7 priv helper '{dominant}' sz={fn_meta.get(fn_va,0)}", 'P7rh'):
                fn_class[fn_va]=dominant; rc+=1; p7rh+=1
    if rc == 0: break

print(f"  P7 totals: fwd={p7} rev={p7r} helper={p7rh}")

# ── PASS 9: 2-Hop Island Bridging ────────────────────────────────────────────
print("\n=== PASS 9: 2-Hop Island Bridging ===")
p9 = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    neighbors = forward_cg.get(fn_va,set()) | reverse_cg.get(fn_va,set())
    if not neighbors: continue
    # Only target islands (no directly-named 1-hop neighbor)
    named_1hop = [n for n in neighbors if not sym_map.get(n,'').startswith('fn_8')]
    if named_1hop: continue  # 1-hop already handled by P4
    # Look 2 hops out through fn_8 midpoints
    two_hop = Counter()
    for mid in neighbors:
        if not sym_map.get(mid,'').startswith('fn_8'): continue
        for far in forward_cg.get(mid,set()) | reverse_cg.get(mid,set()):
            if far == fn_va: continue
            cls = get_class(far)
            if cls: two_hop[cls] += 1
    if not two_hop: continue
    dominant, cnt = two_hop.most_common(1)[0]
    total = sum(two_hop.values())
    if cnt >= 2 and cnt/total >= 0.40:
        safe_cls = rtti_class_to_prefix(dominant)
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_2h")
        if propose(fn_va, nm, f"2-hop via '{dominant}' {cnt}/{total}", 'P9'):
            fn_class[fn_va]=dominant; p9+=1

print(f"  P9: {p9} islands bridged")

# ── PASS 10: Cross-Class Utility Detection ───────────────────────────────────
print("\n=== PASS 10: Utility Detection ===")
p10 = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    callers = reverse_cg.get(fn_va, set())
    if len(callers) < 3: continue
    classes = Counter()
    for c in callers:
        cls = get_class(c)
        if cls: classes[cls] += 1
    if len(classes) >= 2:
        top = [c for c,_ in classes.most_common(2)]
        if propose(fn_va, f"util_{fn_va&0xFFFF:04X}",
                   f"utility fn called by {len(classes)} classes: {','.join(top[:2])}", 'P10'):
            p10 += 1
print(f"  P10: {p10}")

# ── PASS 11: Tiny Pattern-Based Naming ───────────────────────────────────────
print("\n=== PASS 11: Tiny Pattern Naming ===")
p11 = 0
BLR_INST  = 0x4E800020
LI_MASK   = 0xFC1F0000  # li rX, N  (addi rX, r0, N)
LI_OP     = 0x38000000
STW_OP    = 0x90000000

for fn_va in unnamed:
    if fn_va in new_names: continue
    sz = fn_meta.get(fn_va, 0)
    if sz == 0 or sz > 16: continue
    # No call edges (isolated tiny functions)
    if forward_cg.get(fn_va) or reverse_cg.get(fn_va): continue
    off = va_to_file(fn_va)
    if not off: continue

    words = []
    for k in range(sz//4):
        if off+k*4+4 <= len(data):
            words.append(int.from_bytes(data[off+k*4:off+k*4+4],'big'))

    if not words: continue

    # Pattern: just blr → null virtual
    if words == [BLR_INST]:
        if propose(fn_va, f"nullvfn_{fn_va&0xFFFF:04X}", "isolated blr-only (null virtual)", 'P11'):
            p11 += 1; continue

    # Pattern: blr at end, 1-3 words before
    if words[-1] == BLR_INST:
        # li r3, N ; blr → constant returner
        if len(words) == 2 and (words[0]&LI_MASK)==LI_OP and ((words[0]>>21)&0x1F)==3:
            if propose(fn_va, f"const_ret_{fn_va&0xFFFF:04X}", "li r3,N; blr (const returner)", 'P11'):
                p11 += 1; continue
        # Single load + blr → getter
        if len(words) == 2 and (words[0]>>26)&0x3F in (32,34,40,42,48,50):  # lXX loads
            if propose(fn_va, f"get_{fn_va&0xFFFF:04X}", "load+blr (getter)", 'P11'):
                p11 += 1; continue

print(f"  P11: {p11}")

# ── Final Summary ─────────────────────────────────────────────────────────────
uf    = sum(1 for v in new_names if v in unnamed)
total = len(new_names)
pc    = Counter(pass_tag.values())
remaining = len(unnamed) - uf

print(f"\n{'='*70}")
print(f"auto_namer_v7 Results")
print(f"{'='*70}")
for tag in ['P0','P0b','P1','P1r','P2','P3','P4f','P4r','P4fw','P4rh',
            'P7','P7r','P7rh','P9','P10','P11']:
    if pc.get(tag,0): print(f"  {tag:<8s}: {pc.get(tag,0):5d}")
print(f"  {'─'*36}")
print(f"  Total renames    : {total:5d}")
print(f"  fn_8 fixed       : {uf:5d}")
print(f"  Remaining fn_8   : {remaining:5d}")
print(f"  Coverage gain    : {uf/max(len(unnamed),1)*100:.1f}%")
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
    f.write(f"auto_namer_v7 Results\n{'='*70}\n\n")
    for tag in ['P0','P0b','P1','P1r','P2','P3','P4f','P4r','P4fw','P4rh',
                'P7','P7r','P7rh','P9','P10','P11']:
        f.write(f"Pass {tag:<8s}: {pc.get(tag,0):5d}\n")
    f.write(f"\nTotal renames:    {total}\nfn_8 fixed:       {uf}\n")
    f.write(f"Remaining fn_8:   {remaining}\n")
    f.write(f"Quality renames:  {total-uf}\n\n")
    f.write(f"{'='*70}\nDETAIL\n{'='*70}\n")
    for addr in sorted(new_names):
        tag = pass_tag[addr]
        was_fn8 = '*' if addr in unnamed else ' '
        f.write(f"  [{tag:5s}]{was_fn8} 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"Done! Report: {REPORT}")
print(f"To apply v7: cp {OUT} {SYMS.replace('_autonamed','')}")
