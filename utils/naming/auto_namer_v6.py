#!/usr/bin/env python3
"""
auto_namer_v6.py — improved propagation with relaxed thresholds and better collision handling
"""
import re, os, struct, bisect, json
from collections import defaultdict, Counter

BASE  = '/Users/Ozordi/Downloads/tt-decomp'
XEX   = os.path.join(BASE,'orig/434C4803/pong_xenon_final.pe')
SYMS  = os.path.join(BASE,'config/434C4803/symbols.txt')
OUT   = os.path.join(BASE,'config/434C4803/symbols_autonamed.txt')
REPORT= os.path.join(BASE,'auto_namer_v6_report.txt')
RTTI  = os.path.join(BASE,'rtti_vtable_map.json')

print("Loading binary...")
data = open(XEX,'rb').read()
e_lfanew=int.from_bytes(data[0x3C:0x40],'little')
coff_off=e_lfanew+4; opt_off=coff_off+20
image_base=int.from_bytes(data[opt_off+28:opt_off+32],'little')
opt_hdr_size=int.from_bytes(data[coff_off+16:coff_off+18],'little')
num_sects=int.from_bytes(data[coff_off+2:coff_off+4],'little')
sect_tbl=opt_off+opt_hdr_size
sections=[]
for i in range(num_sects):
    s=data[sect_tbl+i*40:sect_tbl+i*40+40]
    sname=s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize=int.from_bytes(s[8:12],'little')
    vaddr=int.from_bytes(s[12:16],'little')
    raw_off=int.from_bytes(s[20:24],'little')
    sections.append((sname,image_base+vaddr,vsize,raw_off))
    print(f"  {sname:12s}  VA=0x{image_base+vaddr:08X}")

text_sec =next(s for s in sections if s[0]=='.text')
rdata_sec=next(s for s in sections if s[0]=='.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec
rdata_chunk = data[rraw:rraw+rvsize]

def va_to_file(va):
    for _,abs_va,vsize,raw_off in sections:
        if abs_va<=va<abs_va+vsize: return raw_off+(va-abs_va)
    return None

print("\nLoading symbols...")
sym_map={}; sym_lines=[]; fn_meta={}
with open(SYMS) as f:
    for line in f:
        sym_lines.append(line)
        s=line.strip()
        for sect in ['.text','.rdata','.data']:
            if '= '+sect+':0x' in s:
                parts=s.split('=',1); name=parts[0].strip()
                try:
                    addr=int(parts[1].split(';')[0].strip().split(':')[1],16)
                    sym_map[addr]=name
                    m=re.search(r'size:0x([0-9A-Fa-f]+)',s)
                    if m: fn_meta[addr]=int(m.group(1),16)
                except: pass

fn_addrs  =sorted(a for a in fn_meta if tvaddr<=a<tvaddr+tvsize)
fn_offsets=sorted(a-tvaddr for a in fn_addrs)
unnamed   ={a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
print(f"  Total: {len(fn_addrs)}, Unnamed: {len(unnamed)}")

def owning_fn(off):
    idx=bisect.bisect_right(fn_offsets,off)-1
    return (tvaddr+fn_offsets[idx]) if idx>=0 else None

def safe_name(s):
    s=re.sub(r'[^A-Za-z0-9_]','_',s)
    return re.sub(r'_+','_',s).strip('_')[:80]

new_names={}; confidence={}; pass_tag={}
used_names=set(sym_map.values())
fn_class_map={}  # fn_va -> class_name

def propose(fn_va,name,reason,tag):
    if fn_va in new_names: return False
    # Better collision handling: try up to 10 suffixes
    original_name = name
    for attempt in range(10):
        if attempt == 0:
            candidate = name
        else:
            candidate = f"{original_name}_{fn_va&0xFFFF:04X}_{attempt}"
        if candidate not in used_names:
            new_names[fn_va]=candidate
            confidence[fn_va]=reason
            pass_tag[fn_va]=tag
            used_names.add(candidate)
            return True
    return False

KNOWN_PREFIXES=sorted([
    'pongLookAtController','pongCreatureInst','pongCreatureType','pongCreature',
    'pongAnimationInfo','pongHairData','pongLocomotionController','pongBlender',
    'pongBlendLookAtDriver','pongSkelLookAtDriver','pongLookAtDriver','pongMover',
    'pongPlayer','pongShell','pongFrontendState','pongCutSceneManager',
    'pongCSActionCamAnim','pongBallInstance','pongDrawBucketManager','pongDrawBucket',
    'pongGameContext','pongLiveManager','pongPresence','pongNetMessageHolder',
    'pongBlendShapeController','pongCSAction',
    'gdGameData','gdLadder','gdLadderMatch','gdDifficultyLadder','gdBallPredictData',
    'gdCameraDef','gdCameraInst','gdCameraDest','gdVibEvent','gdRivalry',
    'gdTierMember','gdTier','gdLadderCommonData','gdCSCharCamShotName',
    'gdCSCharAnimData','gdCSCharAnimNames','gdCSCamAnimShotName',
    'gdCSCamAnimShotNameSeq','gdCSNameData','gdCutSceneData','gdCSActionIfData',
    'gdCSActionCamAnimData','gdCSActionCharAnimData','gdCSActionCharVisibleData',
    'gdCSActionPlayAudioData','gdCSActionLvlAnimData','gdCSActionLvlAmbAnimData',
    'gdCSActionCharAmbAnimData','gdStatsMgr',
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
], key=len, reverse=True)

def extract_class(nm):
    if not nm or nm.startswith('fn_8'): return None
    nl=nm.lower()
    for cls in KNOWN_PREFIXES:
        if nl.startswith(cls.lower()+'_') or nl==cls.lower():
            return cls
    parts=nm.split('_')
    if len(parts)>=2 and parts[0] and not parts[0].startswith('fn'):
        return parts[0]
    return None

def get_class(fn_va):
    if fn_va in fn_class_map: return fn_class_map[fn_va]
    nm=new_names.get(fn_va,sym_map.get(fn_va,''))
    return extract_class(nm)

# ── PASS 1: Fast single-pass string ref scan ────────────────────────────────
print("\n=== PASS 1: Fast string ref scan ===")
CLASS_METHOD=re.compile(rb'[A-Za-z][A-Za-z0-9_]{2,40}::[A-Za-z][A-Za-z0-9_]{1,60}')
hi_lookup=defaultdict(list)
for m in CLASS_METHOD.finditer(rdata_chunk):
    if m.start()>0 and rdata_chunk[m.start()-1]!=0: continue
    s=m.group().decode('ascii','replace')
    if '::' not in s: continue
    cls,meth=s.split('::',1)
    str_va=rvaddr+m.start()
    hi=((str_va+0x8000)>>16)&0xFFFF; lo=str_va&0xFFFF
    hi_lookup[hi].append((lo,cls,meth))

pending={}; hits=[]
for i in range(0,tvsize,4):
    w=int.from_bytes(data[traw+i:traw+i+4],'big')
    op=(w>>26)&0x3F; rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
    if op==15 and ra==0:
        if imm in hi_lookup: pending[rd]=(imm,i)
        else: pending.pop(rd,None)
    elif op==14:
        if ra in pending:
            hi_val,lis_off=pending[ra]
            lo=imm
            for match_lo,cls,meth in hi_lookup.get(hi_val,[]):
                if lo==match_lo:
                    caller_va=owning_fn(i)
                    if caller_va: hits.append((caller_va,cls,meth))
            pending.pop(rd,None)
    else:
        if rd in pending and op not in (15,14): pending.pop(rd,None)

p1=0; p1r=0
for caller_va,cls,meth in hits:
    fn_name=safe_name(f"{cls}_{meth}")
    old=sym_map.get(caller_va,'')
    if caller_va not in fn_class_map:
        fn_class_map[caller_va]=cls
    if old.startswith('fn_8'):
        if propose(caller_va,fn_name,f"string ref: '{cls}::{meth}'","P1"):
            fn_class_map[caller_va]=cls; p1+=1
    elif old and old!=fn_name:
        if fn_name not in used_names:
            new_names[caller_va]=fn_name; confidence[caller_va]=f"string ref (was {old})"
            pass_tag[caller_va]="P1r"; used_names.add(fn_name)
            fn_class_map[caller_va]=cls; p1r+=1
print(f"  P1: {p1} new, P1r: {p1r} partial")

# ── PASS 2: Thunks ───────────────────────────────────────────────────────────
print("\n=== PASS 2: Thunks ===")
p2=0
for fn_va in unnamed:
    if fn_va in new_names: continue
    if fn_meta.get(fn_va,0)!=4: continue
    off=va_to_file(fn_va)
    if not off: continue
    w=int.from_bytes(data[off:off+4],'big')
    if (w>>26)&0x3F==18 and not (w&1):
        li=w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        tgt=fn_va+li
        tname=sym_map.get(tgt,'') or new_names.get(tgt,'')
        if tname and not tname.startswith('fn_8'):
            if propose(fn_va,safe_name(f"thunk_{tname[:50]}"),f"thunk->0x{tgt:08X}","P2"):
                p2+=1; cls=fn_class_map.get(tgt)
                if cls: fn_class_map[fn_va]=cls
print(f"  P2: {p2}")

# ── BUILD CALLGRAPHS ─────────────────────────────────────────────────────────
print("\nBuilding callgraphs...")
forward_cg=defaultdict(set); reverse_cg=defaultdict(set)
for i in range(0,tvsize,4):
    w=int.from_bytes(data[traw+i:traw+i+4],'big')
    if (w>>26)&0x3F==18 and (w&1):
        li=w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        caller_va=owning_fn(i); tgt_va=tvaddr+i+li
        if caller_va and tgt_va in sym_map:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
print(f"  forward edges from {len(forward_cg)} fns")

# ── RTTI: Full vtable seeding ────────────────────────────────────────────────
print("\n=== RTTI: Full vtable seeding ===")
rtti_data=json.load(open(RTTI))
rtti_map={int(k,16):v for k,v in rtti_data.items()}
vtable_members=defaultdict(list)
vtable_slots={}
seeded=0
for vt_va,cls_name in sorted(rtti_map.items()):
    off=vt_va-rvaddr
    if not(0<=off<rvsize): continue
    idx=0; j=off
    while j+4<=rvsize:
        ptr=struct.unpack_from('>I',rdata_chunk,j)[0]
        if not(tvaddr<=ptr<tvaddr+tvsize): break
        j+=4
        if ptr not in fn_class_map:
            fn_class_map[ptr]=cls_name; seeded+=1
        vtable_slots.setdefault(ptr,[]).append((vt_va,idx,cls_name))
        vtable_members[vt_va].append(ptr)
        idx+=1
print(f"  Seeded {seeded} entries ({len(fn_class_map)} total)")

p6=0
for fn_va in list(unnamed):
    if fn_va in new_names: continue
    if fn_va not in fn_class_map: continue
    cls=fn_class_map[fn_va]
    slots=vtable_slots.get(fn_va,[])
    if slots:
        vt_va,idx,_=slots[0]
        safe_cls=safe_name(cls.split('::')[-1])
        proposed=safe_name(f"{safe_cls}_rtti_{vt_va&0xFFFF:04X}_{idx}")
        if propose(fn_va,proposed,f"RTTI vt=0x{vt_va:08X} slot={idx} class={cls}","P6"):
            p6+=1
print(f"  P6 (RTTI slot naming): {p6}")

# ── PASS 7: Constructors ─────────────────────────────────────────────────────
print("\n=== PASS 7: Constructors ===")
rtti_hi_lo={}
for vt_va in rtti_map:
    hi=((vt_va+0x8000)>>16)&0xFFFF; lo=vt_va&0xFFFF
    rtti_hi_lo[(hi,lo)]=vt_va
p7=0; i=0
while i<tvsize-16:
    w0=int.from_bytes(data[traw+i:traw+i+4],'big')
    op0=(w0>>26)&0x3F; rd0=(w0>>21)&0x1F; ra0=(w0>>16)&0x1F; imm0=w0&0xFFFF
    if op0==15 and ra0==0 and imm0 in {k[0] for k in rtti_hi_lo}:
        for j in range(1,8):
            if i+j*4+4>tvsize: break
            w1=int.from_bytes(data[traw+(i+j*4):traw+(i+j*4)+4],'big')
            op1=(w1>>26)&0x3F; rd1=(w1>>21)&0x1F; ra1=(w1>>16)&0x1F; imm1=w1&0xFFFF
            if op1 in(14,24) and ra1==rd0:
                lo=imm1
                if(imm0,lo) in rtti_hi_lo:
                    vt_va=rtti_hi_lo[(imm0,lo)]; cls_name=rtti_map[vt_va]
                    for k in range(j+1,j+6):
                        if i+k*4+4>tvsize: break
                        w2=int.from_bytes(data[traw+(i+k*4):traw+(i+k*4)+4],'big')
                        op2=(w2>>26)&0x3F; rs2=(w2>>21)&0x1F; ra2=(w2>>16)&0x1F
                        if op2 in(36,37) and rs2==rd1 and ra2==3:
                            caller_va=owning_fn(i)
                            if caller_va and caller_va in unnamed and caller_va not in new_names:
                                safe_cls=safe_name(cls_name.split('::')[-1])
                                if propose(caller_va,safe_name(f"{safe_cls}_ctor_{caller_va&0xFFFF:04X}"),
                                           f"stores vtable 0x{vt_va:08X} ({cls_name}) into 0(r3)","P7"):
                                    fn_class_map[caller_va]=cls_name; p7+=1
                            break
                break
    i+=4
print(f"  P7: {p7} constructors")

# ── PASS 8: Interleaved forward+reverse propagation (RELAXED THRESHOLDS) ────
print("\n=== PASS 8: Forward + Reverse propagation (relaxed) ===")
p8f=0; p8r=0; p8fw=0; p8rh=0
for iteration in range(50):  # More rounds
    rc=0
    # Forward: caller calls many class-X fns → caller = X
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees=forward_cg.get(fn_va,set())
        if not callees: continue
        cc=Counter()
        for c in callees:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        n=len(callees)
        safe_cls=safe_name(dominant.split('::')[-1])
        # RELAXED: 20% threshold instead of 25%
        if cnt>=2 and cnt/n>=0.20:
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"fwd {cnt}/{n} '{dominant}'","P8f"):
                fn_class_map[fn_va]=dominant; rc+=1; p8f+=1
        elif cnt==1 and fn_meta.get(fn_va,0)<=80:  # Increased from 64
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_w"),
                       f"fwd tiny '{dominant}' sz={fn_meta.get(fn_va,0)}","P8fw"):
                fn_class_map[fn_va]=dominant; rc+=1; p8fw+=1
    # Reverse: callee called by many class-X fns → callee = X
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callers=reverse_cg.get(fn_va,set())
        if not callers: continue
        cc=Counter()
        for c in callers:
            cls=get_class(c)
            if cls: cc[cls]+=1
        if not cc: continue
        dominant,cnt=cc.most_common(1)[0]
        n=len(callers)
        safe_cls=safe_name(dominant.split('::')[-1])
        # RELAXED: 30% threshold instead of 35%
        if cnt>=2 and cnt/n>=0.30:
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}"),
                       f"rev {cnt}/{n} callers '{dominant}'","P8r"):
                fn_class_map[fn_va]=dominant; rc+=1; p8r+=1
        elif cnt==1 and n==1 and fn_meta.get(fn_va,0)<=100:  # Increased from 80
            if propose(fn_va,safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h"),
                       f"rev private helper '{dominant}' sz={fn_meta.get(fn_va,0)}","P8rh"):
                fn_class_map[fn_va]=dominant; rc+=1; p8rh+=1
    print(f"  Round {iteration+1}: {rc}  (fwd={p8f} rev={p8r} fw={p8fw} rh={p8rh})")
    if rc==0: break

# ── PASS 9: Sibling vtable ──────────────────────────────────────────────────
print("\n=== PASS 9: Sibling vtable propagation ===")
p9=0
for vt_va,members in vtable_members.items():
    cc=Counter()
    for fn_va in members:
        cls=get_class(fn_va)
        if cls: cc[cls]+=1
    if not cc: continue
    dominant,cnt=cc.most_common(1)[0]
    if cnt<1: continue
    for idx,fn_va in enumerate(members):
        if fn_va not in unnamed or fn_va in new_names: continue
        safe_cls=safe_name(dominant.split('::')[-1])
        if propose(fn_va,safe_name(f"{safe_cls}_vt{vt_va&0xFFFF:04X}_{idx}"),
                   f"sibling vt=0x{vt_va:08X} dom={dominant} ({cnt})","P9"):
            fn_class_map[fn_va]=dominant; p9+=1
print(f"  P9: {p9}")

# ── PASS 10: Utility functions (called by multiple classes) ─────────────────
print("\n=== PASS 10: Utility functions ===")
p10=0
for fn_va in unnamed:
    if fn_va in new_names: continue
    callers=reverse_cg.get(fn_va,set())
    if len(callers) < 3: continue  # Need at least 3 callers
    classes = Counter()
    for c in callers:
        cls = get_class(c)
        if cls: classes[cls] += 1
    if len(classes) >= 2:  # Called by 2+ different classes
        # This is likely a utility function
        top_classes = [cls for cls,_ in classes.most_common(2)]
        util_name = f"util_{fn_va&0xFFFF:04X}"
        if propose(fn_va, util_name, f"utility fn called by {len(classes)} classes: {','.join(top_classes[:2])}", "P10"):
            p10+=1
print(f"  P10: {p10}")

# ── Summary ──────────────────────────────────────────────────────────────────
uf=sum(1 for v in new_names if v in unnamed)
up=sum(1 for v in new_names if v not in unnamed)
total=len(new_names)
pc=Counter(pass_tag.values())
remaining=len(unnamed)-uf

print(f"\n{'='*60}")
for tag in ['P1','P1r','P2','P6','P7','P8f','P8fw','P8r','P8rh','P9','P10']:
    print(f"  {tag:<6s}: {pc.get(tag,0):5d}")
print(f"  {'─'*30}")
print(f"  TOTAL renamed:    {total:5d}")
print(f"  fn_8 fixed:       {uf:5d}")
print(f"  Remaining unnamed:{remaining:5d}")
print(f"  Coverage:         {uf/max(len(unnamed),1)*100:.1f}%")
print(f"{'='*60}")

print(f"\nWriting output...")
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

with open(REPORT,'w') as f:
    f.write("auto_namer_v6 Results\n"+"="*70+"\n\n")
    for tag in ['P1','P1r','P2','P6','P7','P8f','P8fw','P8r','P8rh','P9','P10']:
        f.write(f"Pass {tag:<6s}: {pc.get(tag,0):5d}\n")
    f.write(f"Total renames:    {total}\nfn_8 fixed:       {uf}\nRemaining:        {remaining}\nCoverage:         {uf/max(len(unnamed),1)*100:.1f}%\n\n")
    f.write("="*70+"\nDETAIL\n"+"="*70+"\n")
    for addr in sorted(new_names):
        f.write(f"  [{pass_tag[addr]:5s}] 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"Done. To apply: cp {OUT} {SYMS}")
