#!/usr/bin/env python3
"""
auto_namer_v4.py
================
Adds over v3:
  Pass 6  - RTTI vtable → class seed + vtable slot naming
  Pass 6b - Re-run pass 3 with RTTI-seeded fn_class_map
  Pass 7  - Constructor detection (vtable store pattern)
  Pass 8  - RTTI-seeded class propagation to callers

Output:
  config/434C4803/symbols_autonamed.txt
  auto_namer_v4_report.txt
"""
import re, sys, os, struct, bisect, json
from collections import defaultdict, Counter

BASE   = os.path.dirname(os.path.abspath(__file__))
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols.txt')
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed.txt')
REPORT = os.path.join(BASE, 'auto_namer_v4_report.txt')
RTTI   = os.path.join(BASE, 'rtti_vtable_map.json')

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
    s = data[sect_tbl+i*40:sect_tbl+i*40+40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize   = int.from_bytes(s[8:12],'little')
    vaddr   = int.from_bytes(s[12:16],'little')
    raw_off = int.from_bytes(s[20:24],'little')
    sections.append((sname,image_base+vaddr,vsize,raw_off))
    print(f"  {sname:12s}  VA=0x{image_base+vaddr:08X}  size=0x{vsize:X}")

text_sec  = next(s for s in sections if s[0]=='.text')
rdata_sec = next(s for s in sections if s[0]=='.rdata')
data_sec  = next(s for s in sections if s[0]=='.data')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec
_, dvaddr, dvsize, draw = data_sec

def va_to_file(va):
    for _,abs_va,vsize,raw_off in sections:
        if abs_va<=va<abs_va+vsize: return raw_off+(va-abs_va)
    return None

def off_to_va(off):
    for _,abs_va,vsize,raw_off in sections:
        if raw_off<=off<raw_off+vsize: return abs_va+(off-raw_off)
    return None

print("\nLoading symbols...")
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
                    m = re.search(r'size:0x([0-9A-Fa-f]+)',s)
                    if m: fn_meta[addr] = int(m.group(1),16)
                except: pass

fn_addrs   = sorted(a for a in fn_meta if 0x820C0000<=a<=0x82580000)
fn_offsets = sorted((a-(tvaddr)) for a in fn_addrs if 0<=(a-tvaddr)<tvsize)
unnamed    = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
print(f"  Total functions: {len(fn_addrs)}")
print(f"  Unnamed (fn_8): {len(unnamed)}")

def owning_fn(text_off):
    idx = bisect.bisect_right(fn_offsets,text_off)-1
    return (tvaddr+fn_offsets[idx]) if idx>=0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]','_',s)
    return re.sub(r'_+','_',s).strip('_')[:80]

new_names  = {}
confidence = {}
pass_tag   = {}
used_names = set(sym_map.values())
fn_class_map = {}  # fn_va -> (class_name, method_hint, reason)

def propose(fn_va, name, reason, tag):
    if fn_va in new_names: return False
    base = name
    if name in used_names:
        name = f"{base}_{fn_va&0xFFFF:04X}"
    if name in used_names: return False
    new_names[fn_va]  = name
    confidence[fn_va] = reason
    pass_tag[fn_va]   = tag
    used_names.add(name)
    return True

# ── PASS 1: Debug string refs ──────────────────────────────────────────────
print("\n=== PASS 1: Debug string refs ===")
rdata_chunk = data[rraw:rraw+rvsize]
CLASS_METHOD = re.compile(rb'[A-Za-z][A-Za-z0-9_]{2,40}::[A-Za-z][A-Za-z0-9_]{1,60}')
p1_count = 0; p1r_count = 0

for m in CLASS_METHOD.finditer(rdata_chunk):
    if m.start()>0 and rdata_chunk[m.start()-1]!=0: continue
    s = m.group().decode('ascii','replace')
    cls,meth = s.split('::',1)
    fn_name  = safe_name(f"{cls}_{meth}")
    str_va   = rvaddr + m.start()
    # scan .text for lis/addi loading this VA
    hi_target = ((str_va+0x8000)>>16)&0xFFFF
    lo_target = str_va & 0xFFFF
    for i in range(0, tvsize-8, 4):
        w0 = int.from_bytes(data[traw+i:traw+i+4],'big')
        w1 = int.from_bytes(data[traw+i+4:traw+i+8],'big')
        if (w0>>26)&0x3F==15 and (w0>>16)&0x1F==0 and (w0&0xFFFF)==hi_target:
            rd  = (w0>>21)&0x1F
            op1 = (w1>>26)&0x3F
            ra1 = (w1>>16)&0x1F
            lo1 = w1&0xFFFF
            if op1==14 and ra1==rd and lo1==lo_target:
                caller_va = owning_fn(i)
                if caller_va:
                    old = sym_map.get(caller_va,'')
                    if old.startswith('fn_8'):
                        if propose(caller_va, fn_name, f"string ref: '{s}'", "P1"):
                            fn_class_map[caller_va]=(cls,meth,"P1")
                            p1_count+=1
                    elif old and not old.startswith('fn_8'):
                        # partial rename improvement
                        better = safe_name(f"{cls}_{meth}")
                        if better!=old and better not in used_names:
                            new_names[caller_va]=better
                            confidence[caller_va]=f"string ref (was {old}): '{s}'"
                            pass_tag[caller_va]="P1r"
                            used_names.add(better)
                            fn_class_map[caller_va]=(cls,meth,"P1r")
                            p1r_count+=1
                    if caller_va and caller_va not in fn_class_map:
                        fn_class_map[caller_va]=(cls,meth,"P1-hint")

print(f"  P1: {p1_count} fn_8 renames, {p1r_count} partial renames")

# ── PASS 2: Thunks ─────────────────────────────────────────────────────────
print("\n=== PASS 2: Thunks ===")
p2_count = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    sz  = fn_meta.get(fn_va,0)
    if sz!=4: continue
    off = va_to_file(fn_va)
    if off is None: continue
    w   = int.from_bytes(data[off:off+4],'big')
    op  = (w>>26)&0x3F
    if op==18 and not (w&1):
        li = w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        tgt = fn_va+li
        tname = sym_map.get(tgt,'') or new_names.get(tgt,'')
        if tname and not tname.startswith('fn_8'):
            proposed = safe_name(f"thunk_{tname[:50]}")
            if propose(fn_va, proposed, f"thunk->0x{tgt:08X} ({tname})", "P2"):
                p2_count+=1
                cls = fn_class_map.get(tgt,('','',''))[0]
                if cls: fn_class_map[fn_va]=(cls,'thunk','P2')
print(f"  P2: {p2_count} thunks")

# ── BUILD CALL GRAPH (bl + mtctr/bctr) ─────────────────────────────────────
print("\nBuilding call graph...")
callgraph = defaultdict(set)
reg_hi = {}; reg_va = {}
bl_edges=0; ind_edges=0
for i in range(0, tvsize, 4):
    w  = int.from_bytes(data[traw+i:traw+i+4],'big')
    op = (w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
    if op==15 and ra==0:
        reg_hi[rd]=(imm,i); reg_va.pop(rd,None)
    elif op==14:
        if ra in reg_hi:
            hi_val,_=reg_hi[ra]
            lo_s=imm if imm<0x8000 else imm-0x10000
            reg_va[rd]=((hi_val<<16)+lo_s)&0xFFFFFFFF
    elif op==31 and ((w>>1)&0x3FF)==467:
        rs=rd
        if rs in reg_va: reg_hi['ctr']=(0,i); reg_va['ctr']=reg_va[rs]
    elif w==0x4E800420:
        if 'ctr' in reg_va:
            tgt_va=reg_va['ctr']
            caller_va=owning_fn(i)
            if caller_va and tgt_va in sym_map:
                callgraph[caller_va].add(tgt_va); ind_edges+=1
    if op==18 and (w&1):
        li=w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        caller_va=owning_fn(i)
        tgt_va=tvaddr+i+li
        if caller_va and tgt_va in sym_map:
            callgraph[caller_va].add(tgt_va); bl_edges+=1
print(f"  bl: {bl_edges}  indirect: {ind_edges}")

# ── PASS 3: Call-graph propagation ─────────────────────────────────────────
KNOWN_CLASSES = [
    'atSingleton','atArray','atDList','atDLList','atSafeDLListSimple',
    'datBase','datOwner','datRef',
    'pgBase','pgDictionary','pgStreamer',
    'grmModel','grmEdgeModel','grmGeometry','grmDrawable','grmShader',
    'grcTexture','grcTextureStore','grcRenderTarget','grcEffect',
    'grcVertexDeclaration','grcVertexBuffer','grcIndexBuffer',
    'phBound','phBoundBox','phBoundGeometry','phBoundOctree','phBoundComposite',
    'phArchetype','phSimulator','phLevelNew','phPolytree','phUpdateObject',
    'crAnimDof','crAnimDofVector3','crAnimDofQuaternion',
    'crSkelMeshAsset','crSkeletonData',
    'pcrFaceAnimBlender','pcrAnimObserved','pcrAnimObserver','pcrSwingSolver',
    'pcrSwingBlender','pcrServeBlender','pcrActivationCategory','pcrEmoteBlender',
    'pongCreature','pongCreatureInst','pongCreatureType','pongPlayer','pongBlender',
    'pongAnimationInfo','pongHairData','pongLocomotionController',
    'pongLookAtController','pongLookAtDriver','pongBlendLookAtDriver',
    'pongSkelLookAtDriver','pongMover',
    'pongShell','pongFrontendState','pongCutSceneManager','pongCSActionCamAnim',
    'pongBallInstance','pongDrawBucket','pongDrawBucketManager',
    'pongGameContext','pongLiveManager','pongPresence',
    'gdGameData','gdLadder','gdLadderMatch','gdDifficultyLadder',
    'gdBallPredictData','gdCameraDef','gdCameraInst','gdCameraDest',
    'gdVibEvent','gdRivalry','gdTier','gdTierMember','gdLadderCommonData',
    'gdCSCharCamShotName','gdCSCharCamShotNameSeq','gdCSCharAnimData',
    'gdCSCharAnimNames','gdCSCamAnimShotName','gdCSCamAnimShotNameSeq',
    'gdCSNameData','gdCutSceneData','gdCSActionIfData','gdCSActionCamAnimData',
    'gdCSActionCharAnimData','gdCSActionCharVisibleData',
    'gdCSActionPlayAudioData','gdCSActionLvlAnimData','gdCSActionLvlAmbAnimData',
    'gdCSActionCharAmbAnimData','gdStatsMgr','gdUnlo',
    'gmBallNode','gmLogicSinglesMatch','gmLogicSpectator',
    'fxAmbAnimSet','fxString','fxBallSpinLines','fxReticle',
    'fiDevice','fiStream','fiPackfile','fiAsciiTokenizer','fiBaseTokenizer',
    'hudHUD','hudTrainingHUD','hudLeaderboard','hudTrainingPopUp',
    'swfOBJECT','swfCMD','SYMTAB',
    'cmOperatorCtor','cmRefreshableCtor','cmWorldRefreshableCtor',
    'cmReporterCtor','cmApproach2Ctor','cmWorldApproach2Ctor',
    'cmNaryOperator','cmMetafileTuningSet','cmCond','cmLookup','cmSwitch',
    'cmLinearRemap','cmSmoothRemap',
    'cvCurve','cvCurveNurbs','cvCurveNurbsWM',
    'hsmState','FloatAverager','Holder',
    'msgMsgSink','MatchData','NetTimer',
    'netHardware','NetBallHitManagerFreeList','PongBallHitDataStack',
    'AckHandling','SessionMessageRequester','PongNetGameModeCoordinator',
    'pongLiveManager','pongNetMessageHolder',
    'HitMessage','BallHitMessage','SpectatorBallHitMessage',
    'SinglesNetworkClient','gdRivalry','gdStatsMgr',
    'CCalChannel','CCalMoviePlayer',
    'geomBoxes','geomSphere','sysMemSimpleAllocator','sysMem',
    'shShell','fsmMachine','gdStatsMgr',
    'rage','grc','aud','ph','pg','gm','cr','gd','fx','fi',
    'sys','net','io','gfx','pcr','pong','xe','nt','xam','ke',
]

def get_class(fn_va):
    if fn_va in fn_class_map: return fn_class_map[fn_va][0]
    name = new_names.get(fn_va, sym_map.get(fn_va,''))
    if not name or name.startswith('fn_8'): return None
    nl = name.lower()
    for cls in KNOWN_CLASSES:
        if nl.startswith(cls.lower()): return cls
    return None

print("\n=== PASS 3: Call-graph propagation ===")
pass3_total=0; pass3_wrap=0
for iteration in range(20):
    rc=0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees=callgraph.get(fn_va,set())
        if not callees: continue
        cc=Counter()
        for c in callees:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        named_cnt=sum(1 for c in callees if not sym_map.get(c,'').startswith('fn_8'))
        ratio=cnt/named_cnt if named_cnt else 0
        if cnt>=2 and ratio>=0.40:
            if propose(fn_va, safe_name(f"{dominant}_{fn_va&0xFFFF:04X}"),
                       f"calls {cnt}x into '{dominant}' ({cnt}/{named_cnt})", "P3"):
                fn_class_map[fn_va]=(dominant,f"sub_{fn_va&0xFFFF:04X}","P3")
                rc+=1
        elif cnt==1 and fn_meta.get(fn_va,0)<=48:
            if propose(fn_va, safe_name(f"{dominant}_{fn_va&0xFFFF:04X}_wrap"),
                       f"small wrapper 1x '{dominant}' (size={fn_meta.get(fn_va,0)})", "P3w"):
                fn_class_map[fn_va]=(dominant,f"wrap_{fn_va&0xFFFF:04X}","P3w")
                rc+=1; pass3_wrap+=1
        elif cnt>=2:
            if propose(fn_va, safe_name(f"{dominant}_{fn_va&0xFFFF:04X}"),
                       f"calls {cnt}x into '{dominant}' ({cnt}/{named_cnt})", "P3"):
                fn_class_map[fn_va]=(dominant,f"sub_{fn_va&0xFFFF:04X}","P3")
                rc+=1
    pass3_total+=rc
    print(f"  Round {iteration+1}: {rc}")
    if rc==0: break
print(f"  Pass 3 total: {pass3_total}")

# ── PASS 4: Vtable slots (NULL-tolerant) ────────────────────────────────────
print("\n=== PASS 4: Vtable slot inheritance ===")
text_va_lo=tvaddr; text_va_hi=tvaddr+tvsize
vtables=[]; i=0
while i<rvsize-12:
    run=[]; null_count=0; j=i
    while j+4<=rvsize:
        ptr=int.from_bytes(rdata_chunk[j:j+4],'big')
        if text_va_lo<=ptr<text_va_hi:
            run.append(ptr); null_count=0; j+=4
        elif ptr==0:
            if null_count<2: null_count+=1; run.append(0); j+=4
            else: break
        else: break
    while run and run[-1]==0: run.pop()
    if len(run)>=3:
        vtables.append((rvaddr+i,run)); i=j
    else: i+=4
print(f"  {len(vtables)} vtable candidates")
p4_count=0
for vt_va,slots in vtables:
    sc=Counter()
    for fn in slots:
        if fn==0: continue
        cls=get_class(fn)
        if cls: sc[cls]+=1
    if not sc: continue
    dom,cnt=sc.most_common(1)[0]
    if cnt<2: continue
    for idx,fn in enumerate(slots):
        if fn==0 or fn not in unnamed or fn in new_names: continue
        if propose(fn, safe_name(f"{dom}_vt{vt_va&0xFFFF:04X}_{idx}"),
                   f"vtable 0x{vt_va:08X} slot {idx}", "P4"):
            fn_class_map[fn]=(dom,f"vt_{idx}","P4")
            p4_count+=1
print(f"  Named {p4_count}")

# ── PASS 5: Stubs ────────────────────────────────────────────────────────────
print("\n=== PASS 5: Stubs ===")
BLR=0x4E800020; TRAP=0x7FE00008; NOP=0x60000000
p5_count=0
for fn_va in unnamed:
    if fn_va in new_names: continue
    sz=fn_meta.get(fn_va,0)
    if sz>8: continue
    off=va_to_file(fn_va)
    if off is None: continue
    words=[int.from_bytes(data[off+k*4:off+k*4+4],'big') for k in range(sz//4)]
    real=[w for w in words if w!=NOP]
    if real==[BLR]:
        if propose(fn_va,safe_name(f"stub_{fn_va&0xFFFF:04X}"),"blr-only stub","P5"):
            p5_count+=1
    elif TRAP in words:
        if propose(fn_va,safe_name(f"pure_{fn_va&0xFFFF:04X}"),"trap=pure virtual","P5"):
            p5_count+=1
print(f"  Named {p5_count}")

# ── PASS 6: RTTI vtable → class seeding + slot naming ───────────────────────
print("\n=== PASS 6: RTTI vtable class seeding ===")
rtti_data = json.load(open(RTTI)) if os.path.exists(RTTI) else {}
# rtti_data: {"0xVA": "ClassName"}
rtti_map = {int(k,16): v for k,v in rtti_data.items()}

# For each RTTI vtable, read its slots
p6_count=0; p6_seed=0
rdata_data = data[rraw:rraw+rvsize]
for vt_va, cls_name in sorted(rtti_map.items()):
    # Read vtable slots starting at vt_va
    offset_in_rdata = vt_va - rvaddr
    if not (0 <= offset_in_rdata < rvsize): continue
    # Read slots until non-.text pointer
    slot_idx=0
    j=offset_in_rdata
    while j+4<=rvsize:
        ptr=struct.unpack_from('>I', rdata_data, j)[0]
        if not (text_va_lo<=ptr<text_va_hi): break
        j+=4
        # Seed fn_class_map for every slot function
        if ptr not in fn_class_map:
            fn_class_map[ptr]=(cls_name, f"rtti_slot_{slot_idx}", "P6-seed")
            p6_seed+=1
        # Name the slot if it's unnamed
        if ptr in unnamed and ptr not in new_names:
            # Use cls_name, sanitized - template classes get simplified names
            safe_cls = safe_name(cls_name.split('::')[-1])
            proposed = safe_name(f"{safe_cls}_rtti_{vt_va&0xFFFF:04X}_{slot_idx}")
            reason   = f"RTTI vtable 0x{vt_va:08X} slot {slot_idx} class={cls_name}"
            if propose(ptr, proposed, reason, "P6"):
                p6_count+=1
        slot_idx+=1

print(f"  Seeded {p6_seed} function class hints from RTTI")
print(f"  Named {p6_count} vtable slots")

# ── PASS 6b: Re-run call graph propagation with RTTI seeds ─────────────────
print("\n=== PASS 6b: Call-graph re-propagation with RTTI seeds ===")
p6b_total=0; p6b_wrap=0
for iteration in range(25):
    rc=0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees=callgraph.get(fn_va,set())
        if not callees: continue
        cc=Counter()
        for c in callees:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        named_cnt=sum(1 for c in callees if
                      not sym_map.get(c,'').startswith('fn_8') or c in new_names or c in fn_class_map)
        ratio=cnt/max(named_cnt,1)
        if cnt>=2 and ratio>=0.35:
            if propose(fn_va, safe_name(f"{dominant}_{fn_va&0xFFFF:04X}"),
                       f"RTTI-seeded {cnt}x '{dominant}' ({cnt}/{named_cnt})", "P6b"):
                fn_class_map[fn_va]=(dominant,f"sub_{fn_va&0xFFFF:04X}","P6b")
                rc+=1
        elif cnt==1 and fn_meta.get(fn_va,0)<=48:
            safe_cls=safe_name(dominant.split('::')[-1])
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_wrap"),
                       f"RTTI small wrapper 1x '{dominant}' size={fn_meta.get(fn_va,0)}", "P6bw"):
                fn_class_map[fn_va]=(dominant,f"wrap_{fn_va&0xFFFF:04X}","P6bw")
                rc+=1; p6b_wrap+=1
        elif cnt>=2:
            if propose(fn_va, safe_name(f"{dominant}_{fn_va&0xFFFF:04X}"),
                       f"RTTI-seeded {cnt}x '{dominant}'", "P6b"):
                fn_class_map[fn_va]=(dominant,f"sub_{fn_va&0xFFFF:04X}","P6b")
                rc+=1
    p6b_total+=rc
    print(f"  Round {iteration+1}: {rc}")
    if rc==0: break
print(f"  Pass 6b total: {p6b_total} (wraps: {p6b_wrap})")

# ── PASS 7: Constructor detection via vtable store pattern ───────────────────
print("\n=== PASS 7: Constructor detection (vtable store pattern) ===")
# Pattern: lis rX, hi16(vtable_va); addi rX, rX, lo16; stw rX, 0(r3)
# or:      lis rX, hi16; ori rX, rX, lo16; stw rX, 0(r3)
rtti_va_set = set(rtti_map.keys())
rtti_hi_lo = {}  # (hi,lo) -> vtable_va
for vt_va in rtti_va_set:
    hi = ((vt_va+0x8000)>>16)&0xFFFF
    lo = vt_va & 0xFFFF
    rtti_hi_lo[(hi,lo)] = vt_va

p7_count=0
i=0
while i < tvsize-12:
    w0 = int.from_bytes(data[traw+i:traw+i+4],'big')
    op0=(w0>>26)&0x3F; rd0=(w0>>21)&0x1F; ra0=(w0>>16)&0x1F; imm0=w0&0xFFFF
    if op0==15 and ra0==0:  # lis
        hi=imm0
        # Look ahead for addi or ori with same target reg
        for j in range(1,8):
            if i+j*4+4>tvsize: break
            w1=int.from_bytes(data[traw+(i+j*4):traw+(i+j*4)+4],'big')
            op1=(w1>>26)&0x3F; rd1=(w1>>21)&0x1F; ra1=(w1>>16)&0x1F; imm1=w1&0xFFFF
            if op1 in (14,24) and ra1==rd0:  # addi or ori
                lo=imm1
                if (hi,lo) in rtti_hi_lo:
                    vt_va=rtti_hi_lo[(hi,lo)]
                    cls_name=rtti_map[vt_va]
                    # Look ahead for stw rX, 0(r3)
                    for k in range(j+1,j+6):
                        if i+k*4+4>tvsize: break
                        w2=int.from_bytes(data[traw+(i+k*4):traw+(i+k*4)+4],'big')
                        op2=(w2>>26)&0x3F; rs2=(w2>>21)&0x1F
                        ra2=(w2>>16)&0x1F; disp=(w2>>0)&0xFFFF
                        # stw (op=36) or stwu (op=37)
                        if op2 in (36,37) and rs2==rd1 and ra2==3 and disp in (0,0xFFFF0000):
                            caller_va=owning_fn(i)
                            if caller_va and caller_va in unnamed and caller_va not in new_names:
                                safe_cls=safe_name(cls_name.split('::')[-1])
                                proposed=safe_name(f"{safe_cls}_ctor_{caller_va&0xFFFF:04X}")
                                reason=f"stores vtable 0x{vt_va:08X} ({cls_name}) into 0(r3)"
                                if propose(caller_va, proposed, reason, "P7"):
                                    fn_class_map[caller_va]=(cls_name,'ctor','P7')
                                    p7_count+=1
                            break
                    break
    i+=4
print(f"  Named {p7_count} constructors")

# ── PASS 8: Final propagation sweep ─────────────────────────────────────────
print("\n=== PASS 8: Final call-graph sweep ===")
p8_total=0
for iteration in range(20):
    rc=0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees=callgraph.get(fn_va,set())
        if not callees: continue
        cc=Counter()
        for c in callees:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        named_cnt=sum(1 for c in callees if
                      (not sym_map.get(c,'').startswith('fn_8') or
                       c in new_names or c in fn_class_map))
        if cnt>=2:
            if propose(fn_va, safe_name(f"{dominant}_{fn_va&0xFFFF:04X}"),
                       f"pass8 {cnt}x '{dominant}' ({cnt}/{named_cnt})", "P8"):
                fn_class_map[fn_va]=(dominant,f"sub_{fn_va&0xFFFF:04X}","P8")
                rc+=1
        elif cnt==1 and fn_meta.get(fn_va,0)<=32:
            safe_cls=safe_name(dominant.split('::')[-1])
            if propose(fn_va, safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_wrap"),
                       f"pass8 tiny wrapper '{dominant}'", "P8w"):
                fn_class_map[fn_va]=(dominant,'wrap','P8w')
                rc+=1
    p8_total+=rc
    print(f"  Round {iteration+1}: {rc}")
    if rc==0: break
print(f"  Pass 8 total: {p8_total}")

# ── Summary & output ─────────────────────────────────────────────────────────
total_unnamed_renamed = sum(1 for va in new_names if va in unnamed)
total_partial_renamed = sum(1 for va in new_names if va not in unnamed)
total = len(new_names)
pc = Counter(pass_tag.values())

print(f"\nWriting {OUT} ...")
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

print(f"Writing {REPORT} ...")
with open(REPORT,'w') as f:
    f.write("auto_namer_v4.py Results\n")
    f.write("="*70+"\n\n")
    for tag in ['P1','P1r','P2','P3','P3w','P4','P5','P6','P6b','P6bw','P7','P8','P8w']:
        f.write(f"Pass {tag:<5s}: {pc.get(tag,0):5d}\n")
    f.write(f"{'─'*45}\n")
    f.write(f"Total renames:                         {total:5d}\n")
    f.write(f"  Unnamed (fn_8) fixed:                {total_unnamed_renamed:5d}\n")
    f.write(f"  Partial names fixed:                 {total_partial_renamed:5d}\n")
    f.write(f"Previously unnamed:                    {len(unnamed):5d}\n")
    f.write(f"Coverage (fn_8 fixed):                 {total_unnamed_renamed/max(len(unnamed),1)*100:.1f}%\n\n")
    f.write("="*70+"\nDETAIL (sorted by address)\n"+"="*70+"\n")
    for addr in sorted(new_names):
        tag=pass_tag[addr]
        f.write(f"  [{tag:4s}] 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

# Console summary
print(f"\n{'='*60}")
print(f"  P1  (string refs fn_8):          {pc.get('P1',0)}")
print(f"  P1r (string refs partial):        {pc.get('P1r',0)}")
print(f"  P2  (thunks):                     {pc.get('P2',0)}")
print(f"  P3  (call-graph):                 {pc.get('P3',0)}")
print(f"  P3w (call-graph wrap):            {pc.get('P3w',0)}")
print(f"  P4  (vtable slots):               {pc.get('P4',0)}")
print(f"  P5  (stubs):                      {pc.get('P5',0)}")
print(f"  P6  (RTTI vtable slots):          {pc.get('P6',0)}")
print(f"  P6b (RTTI call-graph):            {pc.get('P6b',0)}")
print(f"  P6bw(RTTI call-graph wrap):       {pc.get('P6bw',0)}")
print(f"  P7  (constructors):               {pc.get('P7',0)}")
print(f"  P8  (final sweep):                {pc.get('P8',0)}")
print(f"  P8w (final sweep wrap):           {pc.get('P8w',0)}")
print(f"  {'─'*40}")
print(f"  TOTAL:                            {total}")
print(f"  Unnamed (fn_8) fixed:             {total_unnamed_renamed}")
print(f"  Coverage:                         {total_unnamed_renamed/max(len(unnamed),1)*100:.1f}%")
print(f"\n  Output:  {OUT}")
print(f"  Report:  {REPORT}")
print(f"{'='*60}")
print("\nTo apply:")
print("  cp config/434C4803/symbols_autonamed.txt config/434C4803/symbols.txt")
