#!/usr/bin/env python3
"""
auto_namer_v5.py
================
Key improvements over v4:
  - FULL RTTI seeding: seeds fn_class_map for ALL vtable slot functions (named + unnamed)
  - REVERSE propagation: unnamed fn called by named-class fns → gets that class
  - FORWARD propagation: unnamed fn that calls named-class fns → gets that class
  - SIBLING propagation: unnamed fn in same vtable as named fn → gets that class  
  - Multi-round interleaved forward+reverse until convergence
"""
import re, sys, os, struct, bisect, json
from collections import defaultdict, Counter

BASE  = '/Users/Ozordi/Downloads/tt-decomp'
XEX   = os.path.join(BASE,'orig/434C4803/pong_xenon_final.pe')
SYMS  = os.path.join(BASE,'config/434C4803/symbols.txt')
OUT   = os.path.join(BASE,'config/434C4803/symbols_autonamed.txt')
REPORT= os.path.join(BASE,'auto_namer_v5_report.txt')
RTTI  = os.path.join(BASE,'rtti_vtable_map.json')

print("Loading binary...")
data = open(XEX,'rb').read()
e_lfanew     = int.from_bytes(data[0x3C:0x40],'little')
coff_off     = e_lfanew + 4
opt_off      = coff_off + 20
image_base   = int.from_bytes(data[opt_off+28:opt_off+32],'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18],'little')
num_sects    = int.from_bytes(data[coff_off+2:coff_off+4],'little')
sect_tbl     = opt_off + opt_hdr_size
sections = []
for i in range(num_sects):
    s       = data[sect_tbl+i*40:sect_tbl+i*40+40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize   = int.from_bytes(s[8:12],'little')
    vaddr   = int.from_bytes(s[12:16],'little')
    raw_off = int.from_bytes(s[20:24],'little')
    sections.append((sname, image_base+vaddr, vsize, raw_off))
    print(f"  {sname:12s}  VA=0x{image_base+vaddr:08X}  size=0x{vsize:X}")

text_sec  = next(s for s in sections if s[0]=='.text')
rdata_sec = next(s for s in sections if s[0]=='.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec
rdata_chunk = data[rraw:rraw+rvsize]

def va_to_file(va):
    for _,abs_va,vsize,raw_off in sections:
        if abs_va<=va<abs_va+vsize: return raw_off+(va-abs_va)
    return None

print("\nLoading symbols...")
sym_map = {}; sym_lines = []; fn_meta = {}
with open(SYMS) as f:
    for line in f:
        sym_lines.append(line)
        s = line.strip()
        for sect in ['.text','.rdata','.data']:
            if '= '+sect+':0x' in s:
                parts = s.split('=',1); name = parts[0].strip()
                try:
                    addr = int(parts[1].split(';')[0].strip().split(':')[1],16)
                    sym_map[addr] = name
                    m = re.search(r'size:0x([0-9A-Fa-f]+)',s)
                    if m: fn_meta[addr] = int(m.group(1),16)
                except: pass

fn_addrs   = sorted(a for a in fn_meta if tvaddr<=a<tvaddr+tvsize)
fn_offsets = sorted(a-tvaddr for a in fn_addrs)
unnamed    = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
print(f"  Total functions: {len(fn_addrs)}, Unnamed: {len(unnamed)}")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets,off)-1
    return (tvaddr+fn_offsets[idx]) if idx>=0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]','_',s)
    return re.sub(r'_+','_',s).strip('_')[:80]

new_names = {}; confidence = {}; pass_tag = {}
used_names = set(sym_map.values())
fn_class_map = {}  # fn_va -> class_name (string)

def propose(fn_va, name, reason, tag):
    if fn_va in new_names: return False
    if name in used_names:
        name = f"{name}_{fn_va&0xFFFF:04X}"
    if name in used_names: return False
    new_names[fn_va]  = name
    confidence[fn_va] = reason
    pass_tag[fn_va]   = tag
    used_names.add(name)
    return True

def get_class(fn_va):
    if fn_va in fn_class_map: return fn_class_map[fn_va]
    nm = new_names.get(fn_va, sym_map.get(fn_va,''))
    if not nm or nm.startswith('fn_8'): return None
    return extract_class_from_name(nm)

KNOWN_PREFIXES = [
    'pongLookAtController','pongCreatureInst','pongCreatureType','pongCreature',
    'pongAnimationInfo','pongHairData','pongLocomotionController','pongBlender',
    'pongBlendLookAtDriver','pongSkelLookAtDriver','pongLookAtDriver','pongMover',
    'pongPlayer','pongShell','pongFrontendState','pongCutSceneManager',
    'pongCSActionCamAnim','pongBallInstance','pongDrawBucketManager','pongDrawBucket',
    'pongGameContext','pongLiveManager','pongPresence','pongNetMessageHolder',
    'pongBlendShapeController',
    'gdGameData','gdLadder','gdLadderMatch','gdDifficultyLadder','gdBallPredictData',
    'gdCameraDef','gdCameraInst','gdCameraDest','gdVibEvent','gdRivalry',
    'gdTierMember','gdTier','gdLadderCommonData','gdCSCharCamShotName',
    'gdCSCharCamShotNameSeq','gdCSCharAnimData','gdCSCharAnimNames',
    'gdCSCamAnimShotName','gdCSCamAnimShotNameSeq','gdCSNameData',
    'gdCutSceneData','gdCSActionIfData','gdCSActionCamAnimData',
    'gdCSActionCharAnimData','gdCSActionCharVisibleData','gdCSActionPlayAudioData',
    'gdCSActionLvlAnimData','gdCSActionLvlAmbAnimData','gdCSActionCharAmbAnimData',
    'gdStatsMgr',
    'gmBallNode','gmLogicSinglesMatch','gmLogicSpectator',
    'hudHUD','hudTrainingHUD','hudLeaderboard','hudTrainingPopUp',
    'fxAmbAnimSet','fxString','fxBallSpinLines','fxReticle',
    'fiDevice','fiStream','fiPackfile','fiAsciiTokenizer','fiBaseTokenizer',
    'phBoundBox','phBoundGeometry','phBoundOctree','phBoundComposite','phBound',
    'phArchetype','phSimulator','phLevelNew','phPolytree','phUpdateObject',
    'pcrFaceAnimBlender','pcrAnimObserved','pcrAnimObserver','pcrSwingSolver',
    'pcrSwingBlender','pcrServeBlender','pcrActivationCategory','pcrEmoteBlender',
    'grmEdgeModel','grmModel','grmGeometry','grmDrawable','grmShader',
    'grcTexture','grcTextureStore','grcRenderTarget','grcEffect',
    'grcVertexDeclaration','grcVertexBuffer','grcIndexBuffer',
    'pgBase','pgDictionary','pgStreamer',
    'datBase','datOwner','datRef',
    'crAnimDofVector3','crAnimDofQuaternion','crAnimDof','crSkelMeshAsset',
    'atSingleton','atArray','atDList','atSafeDLListSimple',
    'swfOBJECT','swfCMD','SYMTAB',
    'cmOperatorCtor','cmRefreshableCtor','cmWorldRefreshableCtor','cmReporterCtor',
    'cmApproach2Ctor','cmWorldApproach2Ctor','cmNaryOperator','cmMetafileTuningSet',
    'cmCond','cmLookup','cmSwitch','cmLinearRemap','cmSmoothRemap',
    'cvCurveNurbsWM','cvCurveNurbs','cvCurve',
    'hsmState','FloatAverager','Holder',
    'msgMsgSink','MatchData','NetTimer','netHardware',
    'NetBallHitManagerFreeList','PongBallHitDataStack',
    'AckHandling','SessionMessageRequester','PongNetGameModeCoordinator',
    'SinglesNetworkClient','HitMessage','BallHitMessage','SpectatorBallHitMessage',
    'CCalChannel','CCalMoviePlayer',
    'xmlNodeStructAnimHashEntryArray','xmlNodeStructAnimHashEntry',
    'xmlNodeStruct','shShell','fsmMachine',
]
KNOWN_PREFIXES_SORTED = sorted(KNOWN_PREFIXES, key=len, reverse=True)

def extract_class_from_name(nm):
    nl = nm.lower()
    for cls in KNOWN_PREFIXES_SORTED:
        if nl.startswith(cls.lower()+'_') or nl.startswith(cls.lower()):
            return cls
    # Try splitting on _
    parts = nm.split('_')
    if len(parts)>=2 and not parts[0].startswith('fn'):
        return parts[0]
    return None

# ── PASS 1: String refs ──────────────────────────────────────────────────────
print("\n=== PASS 1: String refs ===")
CLASS_METHOD = re.compile(rb'[A-Za-z][A-Za-z0-9_]{2,40}::[A-Za-z][A-Za-z0-9_]{1,60}')
p1=0; p1r=0
for m in CLASS_METHOD.finditer(rdata_chunk):
    if m.start()>0 and rdata_chunk[m.start()-1]!=0: continue
    s = m.group().decode('ascii','replace')
    cls,meth = s.split('::',1)
    fn_name = safe_name(f"{cls}_{meth}")
    str_va  = rvaddr + m.start()
    hi_target = ((str_va+0x8000)>>16)&0xFFFF
    lo_target = str_va & 0xFFFF
    for i in range(0, tvsize-8, 4):
        w0 = int.from_bytes(data[traw+i:traw+i+4],'big')
        w1 = int.from_bytes(data[traw+i+4:traw+i+8],'big')
        if (w0>>26)&0x3F==15 and (w0>>16)&0x1F==0 and (w0&0xFFFF)==hi_target:
            rd  = (w0>>21)&0x1F
            op1 = (w1>>26)&0x3F; ra1=(w1>>16)&0x1F; lo1=w1&0xFFFF
            if op1==14 and ra1==rd and lo1==lo_target:
                caller_va = owning_fn(i)
                if not caller_va: continue
                old = sym_map.get(caller_va,'')
                if old.startswith('fn_8'):
                    if propose(caller_va, fn_name, f"string ref: '{s}'", "P1"):
                        fn_class_map[caller_va]=cls; p1+=1
                elif old:
                    better = safe_name(f"{cls}_{meth}")
                    if better!=old and better not in used_names:
                        new_names[caller_va]=better
                        confidence[caller_va]=f"string ref (was {old}): '{s}'"
                        pass_tag[caller_va]="P1r"
                        used_names.add(better)
                        fn_class_map[caller_va]=cls; p1r+=1
                if caller_va not in fn_class_map:
                    fn_class_map[caller_va]=cls
print(f"  P1: {p1} fn_8, P1r: {p1r} partial")

# ── PASS 2: Thunks ───────────────────────────────────────────────────────────
print("\n=== PASS 2: Thunks ===")
p2=0
for fn_va in unnamed:
    if fn_va in new_names: continue
    if fn_meta.get(fn_va,0)!=4: continue
    off = va_to_file(fn_va)
    if not off: continue
    w = int.from_bytes(data[off:off+4],'big')
    if (w>>26)&0x3F==18 and not (w&1):
        li=w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        tgt=fn_va+li
        tname=sym_map.get(tgt,'') or new_names.get(tgt,'')
        if tname and not tname.startswith('fn_8'):
            if propose(fn_va,safe_name(f"thunk_{tname[:50]}"),f"thunk->0x{tgt:08X}","P2"):
                p2+=1
                cls=fn_class_map.get(tgt,'')
                if cls: fn_class_map[fn_va]=cls
print(f"  P2: {p2}")

# ── BUILD CALLGRAPHS ─────────────────────────────────────────────────────────
print("\nBuilding callgraphs (forward + reverse)...")
forward_cg  = defaultdict(set)  # caller -> set of callees
reverse_cg  = defaultdict(set)  # callee -> set of callers
bl_count=0
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    op=(w>>26)&0x3F
    if op==18 and (w&1):
        li=w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        caller_va=owning_fn(i)
        tgt_va=tvaddr+i+li
        if caller_va and tgt_va in sym_map:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
            bl_count+=1
print(f"  {bl_count} BL edges")

# ── RTTI: Full vtable seeding ────────────────────────────────────────────────
print("\n=== RTTI: Full vtable class seeding ===")
rtti_data = json.load(open(RTTI))
rtti_map  = {int(k,16): v for k,v in rtti_data.items()}
# Map from fn_va -> set of (vtable_va, slot_idx, class_name)
vtable_slots = {}   # fn_va -> [(vt_va, idx, cls), ...]
vtable_members = defaultdict(list)  # vt_va -> [fn_va, ...]
rtti_seeded=0

for vt_va, cls_name in sorted(rtti_map.items()):
    off = vt_va - rvaddr
    if not (0 <= off < rvsize): continue
    idx=0; j=off
    while j+4<=rvsize:
        ptr=struct.unpack_from('>I',rdata_chunk,j)[0]
        if not (tvaddr<=ptr<tvaddr+tvsize): break
        j+=4
        # Seed class for every slot
        if ptr not in fn_class_map:
            fn_class_map[ptr]=cls_name; rtti_seeded+=1
        elif fn_class_map[ptr]!=cls_name:
            pass  # already has a different class (keep original)
        vtable_slots.setdefault(ptr,[]).append((vt_va,idx,cls_name))
        vtable_members[vt_va].append(ptr)
        idx+=1

print(f"  Seeded {rtti_seeded} new fn_class_map entries from RTTI")
print(f"  Total fn_class_map entries: {len(fn_class_map)}")

# Name unnamed vtable slots
p6=0
for fn_va in list(unnamed):
    if fn_va in new_names: continue
    if fn_va not in fn_class_map: continue
    cls=fn_class_map[fn_va]
    slots=vtable_slots.get(fn_va,[])
    if slots:
        vt_va,idx,_ = slots[0]
        safe_cls=safe_name(cls.split('::')[-1])
        proposed=safe_name(f"{safe_cls}_rtti_{vt_va&0xFFFF:04X}_{idx}")
        if propose(fn_va, proposed, f"RTTI vtable 0x{vt_va:08X} slot {idx} class={cls}", "P6"):
            p6+=1
print(f"  P6 (RTTI vtable slot naming): {p6}")

# ── CONSTRUCTOR DETECTION ────────────────────────────────────────────────────
print("\n=== Pass 7: Constructor detection ===")
rtti_va_set = set(rtti_map.keys())
rtti_hi_lo = {}
for vt_va in rtti_va_set:
    hi=((vt_va+0x8000)>>16)&0xFFFF; lo=vt_va&0xFFFF
    rtti_hi_lo[(hi,lo)]=vt_va
p7=0
i=0
while i<tvsize-16:
    w0=int.from_bytes(data[traw+i:traw+i+4],'big')
    op0=(w0>>26)&0x3F; rd0=(w0>>21)&0x1F; ra0=(w0>>16)&0x1F; imm0=w0&0xFFFF
    if op0==15 and ra0==0:
        hi=imm0
        for j in range(1,8):
            if i+j*4+4>tvsize: break
            w1=int.from_bytes(data[traw+(i+j*4):traw+(i+j*4)+4],'big')
            op1=(w1>>26)&0x3F; rd1=(w1>>21)&0x1F; ra1=(w1>>16)&0x1F; imm1=w1&0xFFFF
            if op1 in (14,24) and ra1==rd0:
                lo=imm1
                if (hi,lo) in rtti_hi_lo:
                    vt_va=rtti_hi_lo[(hi,lo)]
                    cls_name=rtti_map[vt_va]
                    for k in range(j+1,j+6):
                        if i+k*4+4>tvsize: break
                        w2=int.from_bytes(data[traw+(i+k*4):traw+(i+k*4)+4],'big')
                        op2=(w2>>26)&0x3F; rs2=(w2>>21)&0x1F; ra2=(w2>>16)&0x1F
                        if op2 in (36,37) and rs2==rd1 and ra2==3:
                            caller_va=owning_fn(i)
                            if caller_va and caller_va in unnamed and caller_va not in new_names:
                                safe_cls=safe_name(cls_name.split('::')[-1])
                                proposed=safe_name(f"{safe_cls}_ctor_{caller_va&0xFFFF:04X}")
                                if propose(caller_va,proposed,f"stores vtable 0x{vt_va:08X} ({cls_name}) into 0(r3)","P7"):
                                    fn_class_map[caller_va]=cls_name; p7+=1
                            break
                    break
    i+=4
print(f"  P7: {p7} constructors")

# ── PROPAGATION: Forward + Reverse alternating ───────────────────────────────
print("\n=== Pass 8: Interleaved forward+reverse propagation ===")
p8f_total=0; p8r_total=0; p8w_total=0

for iteration in range(30):
    rc=0

    # Forward: unnamed caller calls many class-X callees -> caller = class X
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees = forward_cg.get(fn_va, set())
        if not callees: continue
        cc=Counter()
        for c in callees:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        total_callees=len(callees)
        ratio=cnt/total_callees
        if cnt>=2 and ratio>=0.30:
            safe_cls=safe_name(dominant.split('::')[-1])
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"fwd {cnt}/{total_callees} '{dominant}'","P8f"):
                fn_class_map[fn_va]=dominant; rc+=1; p8f_total+=1
        elif cnt==1 and fn_meta.get(fn_va,0)<=48:
            safe_cls=safe_name(dominant.split('::')[-1])
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_wrap"),
                       f"fwd tiny wrapper '{dominant}' sz={fn_meta.get(fn_va,0)}","P8fw"):
                fn_class_map[fn_va]=dominant; rc+=1; p8f_total+=1; p8w_total+=1

    # Reverse: unnamed callee called by many class-X callers -> callee = class X
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callers = reverse_cg.get(fn_va, set())
        if not callers: continue
        cc=Counter()
        for c in callers:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        total_callers=len(callers)
        ratio=cnt/total_callers
        if cnt>=2 and ratio>=0.40:
            safe_cls=safe_name(dominant.split('::')[-1])
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"rev {cnt}/{total_callers} callers '{dominant}'","P8r"):
                fn_class_map[fn_va]=dominant; rc+=1; p8r_total+=1
        elif cnt>=1 and total_callers==1 and fn_meta.get(fn_va,0)<=64:
            safe_cls=safe_name(dominant.split('::')[-1])
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h"),
                       f"rev private helper '{dominant}'","P8rh"):
                fn_class_map[fn_va]=dominant; rc+=1; p8r_total+=1

    print(f"  Round {iteration+1}: {rc} (cumfwd={p8f_total} cumrev={p8r_total})")
    if rc==0: break

print(f"  Forward total: {p8f_total}, Reverse total: {p8r_total}, Wraps: {p8w_total}")

# ── Sibling vtable propagation ────────────────────────────────────────────────
print("\n=== Pass 9: Sibling vtable propagation ===")
p9=0
for vt_va, members in vtable_members.items():
    # Find dominant class in this vtable
    cc=Counter()
    for fn_va in members:
        if fn_va not in unnamed or fn_va in new_names:
            nm=new_names.get(fn_va, sym_map.get(fn_va,''))
            cls=get_class(fn_va)
            if cls: cc[cls]+=1
    if not cc: continue
    dominant,cnt=cc.most_common(1)[0]
    if cnt<1: continue
    # Name unnamed siblings
    for idx,fn_va in enumerate(members):
        if fn_va not in unnamed or fn_va in new_names: continue
        safe_cls=safe_name(dominant.split('::')[-1])
        if propose(fn_va,safe_name(f"{safe_cls}_vt{vt_va&0xFFFF:04X}_{idx}"),
                   f"sibling in vtable 0x{vt_va:08X} dom={dominant} ({cnt} named)","P9"):
            fn_class_map[fn_va]=dominant; p9+=1
print(f"  P9: {p9} siblings")

# ── Stubs ──────────────────────────────────────────────────────────────────
print("\n=== Pass 10: Stubs ===")
BLR=0x4E800020; TRAP=0x7FE00008; NOP=0x60000000
p10=0
for fn_va in unnamed:
    if fn_va in new_names: continue
    sz=fn_meta.get(fn_va,0)
    if sz>8: continue
    off=va_to_file(fn_va)
    if not off: continue
    words=[int.from_bytes(data[off+k*4:off+k*4+4],'big') for k in range(sz//4)]
    real=[w for w in words if w!=NOP]
    if real==[BLR]:
        if propose(fn_va,safe_name(f"stub_{fn_va&0xFFFF:04X}"),"blr-only stub","P10"):
            p10+=1
    elif TRAP in words:
        if propose(fn_va,safe_name(f"pure_{fn_va&0xFFFF:04X}"),"trap=pure virtual","P10"):
            p10+=1
print(f"  P10: {p10}")

# ── Summary ───────────────────────────────────────────────────────────────────
total_unnamed_fixed = sum(1 for va in new_names if va in unnamed)
total_partial       = sum(1 for va in new_names if va not in unnamed)
total               = len(new_names)
pc=Counter(pass_tag.values())
remaining = len(unnamed) - total_unnamed_fixed

print(f"\n{'='*60}")
for tag in ['P1','P1r','P2','P6','P7','P8f','P8fw','P8r','P8rh','P9','P10']:
    print(f"  {tag:<6s}: {pc.get(tag,0):5d}")
print(f"  {'─'*30}")
print(f"  TOTAL:      {total:5d}  (unnamed fixed: {total_unnamed_fixed})")
print(f"  Remaining unnamed: {remaining}")
print(f"  Coverage:   {total_unnamed_fixed/max(len(unnamed),1)*100:.1f}%")
print(f"{'='*60}")

# Write output
print(f"\nWriting {OUT}...")
out_lines=[]
for line in sym_lines:
    s=line.strip(); matched=False
    for sect in ['.text','.rdata','.data']:
        if '= '+sect+':0x' in s:
            parts=s.split('=',1); name=parts[0].strip()
            try:
                addr=int(parts[1].split(';')[0].strip().split(':')[1],16)
                if addr in new_names:
                    out_lines.append(line.replace(name,new_names[addr],1))
                    matched=True; break
            except: pass
    if not matched: out_lines.append(line)
with open(OUT,'w') as f: f.writelines(out_lines)

print(f"Writing {REPORT}...")
with open(REPORT,'w') as f:
    f.write("auto_namer_v5.py Results\n"+"="*70+"\n\n")
    for tag in ['P1','P1r','P2','P6','P7','P8f','P8fw','P8r','P8rh','P9','P10']:
        f.write(f"Pass {tag:<6s}: {pc.get(tag,0):5d}\n")
    f.write(f"{'─'*45}\n")
    f.write(f"Total renames:              {total:5d}\n")
    f.write(f"  Unnamed (fn_8) fixed:     {total_unnamed_fixed:5d}\n")
    f.write(f"  Partial names fixed:      {total_partial:5d}\n")
    f.write(f"Previously unnamed:         {len(unnamed):5d}\n")
    f.write(f"Remaining unnamed:          {remaining:5d}\n")
    f.write(f"Coverage:                   {total_unnamed_fixed/max(len(unnamed),1)*100:.1f}%\n\n")
    f.write("="*70+"\nDETAIL\n"+"="*70+"\n")
    for addr in sorted(new_names):
        tag=pass_tag[addr]
        f.write(f"  [{tag:5s}] 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"\nTo apply:")
print(f"  cp {OUT} {SYMS}")
