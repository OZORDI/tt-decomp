#!/usr/bin/env python3
"""
cross_match_gta4_v3.py

Matching strategy:
  For each GTA4 function, extract the PPC instruction sequence from
  the recompiler comment lines (e.g. "// lwz r3,8(r3)").
  Normalize: strip branch targets and pure immediates, keep opcodes
  and struct-access offsets.  Hash → compare against TT2 binary.

  TT2 matching uses the same normalization on raw PPC words.
"""
import re, os, json, hashlib, bisect, struct
from collections import defaultdict, Counter
from pathlib import Path

BASE    = '/Users/Ozordi/Downloads/tt-decomp'
GEN     = '/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated'
TT2     = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS    = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v9.txt')
TT2RTTI = os.path.join(BASE, 'rtti_vtable_map.json')
GTA4BIN = '/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/default.bin'
OUT     = os.path.join(BASE, 'cross_match_gta4_v3_results.txt')
SEEDS   = os.path.join(BASE, 'cross_match_seeds_v3.py')

def safe(s):
    return re.sub(r'_+','_',re.sub(r'[^A-Za-z0-9_]','_',s)).strip('_')[:80]
def cshort(cls):
    return safe(cls.split('::')[-1] if '::' in cls else cls)

# ── Instruction normalizer (from PPC mnemonic comment strings) ────────────────
# We keep: opcode, struct-offset (rN+offset where N is 3/4/5 = param regs)
# We strip: branch targets, plain immediates (lis/addi/li), register numbers
RE_INSN = re.compile(r'^\t// ([a-z][a-z0-9.]+)\s*(.*?)$', re.MULTILINE)
# Load/store with offset: "lwz r3,8(r4)" → ('lwz', 8)
RE_LDST = re.compile(r'^r\d+,(-?\d+)\(r(\d+)\)$')
# Load/store indexed: "lwzx r3,r4,r5" → no useful offset info
RE_BRANCH = re.compile(r'^0x[0-9a-f]+$')

PARAM_REGS = frozenset(range(3, 12))  # r3-r11 are caller-saved / param regs

def normalize_insn_str(mnemonic, operands):
    """Return a normalized token for one PPC instruction."""
    op = mnemonic.rstrip('.')
    
    # Branches: keep only whether it's a call (bl) or conditional/unconditional
    if op in ('bl', 'bla'):      return ('bl',)
    if op in ('blr', 'blrl'):    return ('blr',)
    if op in ('b', 'ba'):        return ('b',)
    if op == 'bctr':             return ('bctr',)
    if op.startswith('b'):       return ('bc',)   # all conditional branches
    
    # Prologue/epilogue: mflr/mtlr/stw r12/-8(r1) etc — normalize away
    if op in ('mflr','mtlr','mfctr','mtctr'): return (op,)
    if op in ('stwu', 'stdu'):
        m = RE_LDST.match(operands.strip())
        if m and int(m.group(2))==1: return (op,'sp')  # stack frame alloc
        
    # Load/store with base register offset
    if op[:2] in ('lw','lh','lb','ld','lf','st','sf') or op in ('lwz','lhz','lbz','lfs','lfd','stw','sth','stb','std','stfs','stfd','lwzu','lfsu','stfsu','lwzx'):
        m = RE_LDST.match(operands.strip())
        if m:
            offset = int(m.group(1))
            base_reg = int(m.group(2))
            if base_reg == 1:
                return (op, 'sp', offset)  # keep stack offsets (frame layout)
            elif base_reg in PARAM_REGS:
                return (op, 'obj', offset)  # struct access with offset
            else:
                return (op, 'reg', offset)
        return (op,)
    
    # ALU with immediate: li, addi, addis, lis — strip immediate
    if op in ('li','lis','addi','addis','subfic','ori','oris','xori','xoris','andi','andis'):
        return (op,)
    
    # Comparisons
    if op in ('cmpw','cmpwi','cmplw','cmplwi','cmpd','cmpdi','cmpldi','cmpldi'):
        return ('cmp',)
    
    # Everything else: just the opcode
    return (op,)

def insn_seq_from_block(block):
    """Extract normalized instruction sequence from a recompiler function block."""
    tokens = []
    for m in RE_INSN.finditer(block):
        mnemonic = m.group(1)
        operands = m.group(2).strip()
        # Skip pure data comments (lr assignments from ctx.lr = 0x...)
        tok = normalize_insn_str(mnemonic, operands)
        tokens.append(tok)
    return tuple(tokens)

def seq_hash(seq):
    return hashlib.md5(str(seq).encode()).hexdigest()

# ── Parse GTA4 recompiler sources ─────────────────────────────────────────────
print("Parsing GTA4 recompiler sources (74 files, extracting instruction sequences)...")

gta4_seq   = {}   # fn_va -> normalized insn sequence
gta4_calls = {}   # fn_va -> set(callee_va)
gta4_callers = defaultdict(set)
gta4_lcount  = {}

RE_FUNC = re.compile(r'^PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]+)\)', re.MULTILINE)
RE_CALL = re.compile(r'\bsub_([0-9A-Fa-f]+)\(ctx,\s*base\)')

for fpath in sorted(Path(GEN).glob('gta4_recomp.*.cpp')):
    text = fpath.read_text(encoding='utf-8', errors='replace')
    starts = [(m.start(), int(m.group(1)[4:], 16)) for m in RE_FUNC.finditer(text)]
    for i, (fs, va) in enumerate(starts):
        fe = starts[i+1][0] if i+1 < len(starts) else len(text)
        blk = text[fs:fe]
        callees = {int(m.group(1),16) for m in RE_CALL.finditer(blk) if int(m.group(1),16)!=va}
        gta4_calls[va]  = callees
        gta4_lcount[va] = blk.count('\n')
        for c in callees: gta4_callers[c].add(va)
        gta4_seq[va] = insn_seq_from_block(blk)

print(f"  {len(gta4_seq):,} functions parsed")

# Build hash indexes
gta4_exact  = defaultdict(list)   # exact hash -> [va]
gta4_prefix = defaultdict(list)   # 8-insn prefix hash -> [va]  (for partial match)

for va, seq in gta4_seq.items():
    if len(seq) < 3: continue
    gta4_exact[seq_hash(seq)].append(va)
    gta4_prefix[seq_hash(seq[:8])].append(va)

g4_unique_exact  = {h: vl[0] for h, vl in gta4_exact.items()  if len(vl)==1}
g4_unique_prefix = {h: vl[0] for h, vl in gta4_prefix.items() if len(vl)==1}
print(f"  Unique exact hashes:    {len(g4_unique_exact):,}")
print(f"  Unique 8-insn prefixes: {len(g4_unique_prefix):,}")

# ── GTA4 RTTI → class labels ───────────────────────────────────────────────────
print("Building GTA4 class labels from RTTI...")

gd = open(GTA4BIN,'rb').read()
def pe_sects(data):
    e=int.from_bytes(data[0x3C:0x40],'little'); co=e+4; oo=co+20
    ib=int.from_bytes(data[oo+28:oo+32],'little')
    os_=int.from_bytes(data[co+16:co+18],'little')
    ns=int.from_bytes(data[co+2:co+4],'little'); st=oo+os_; s=[]
    for i in range(ns):
        r=data[st+i*40:st+i*40+40]
        nm=r[:8].rstrip(b'\x00').decode('ascii','replace')
        vs=int.from_bytes(r[8:12],'little'); va=int.from_bytes(r[12:16],'little')
        ro=int.from_bytes(r[20:24],'little'); rz=int.from_bytes(r[16:20],'little')
        s.append((nm,ib+va,vs,ro,min(vs,rz)))
    return s

g4s=pe_sects(gd)
g4tv=next(s[1] for s in g4s if s[0]=='.text'); g4ts=next(s[2] for s in g4s if s[0]=='.text')
g4rv=next(s[1] for s in g4s if s[0]=='.rdata'); g4rs=next(s[2] for s in g4s if s[0]=='.rdata')
g4rr=next(s[3] for s in g4s if s[0]=='.rdata')
g4dv=next(s[1] for s in g4s if s[0]=='.data')
g4dr=next(s[3] for s in g4s if s[0]=='.data'); g4ds=next(s[2] for s in g4s if s[0]=='.data')
rc=gd[g4rr:g4rr+g4rs]; dc=gd[g4dr:g4dr+g4ds]

rtti_td={}
for m in re.finditer(rb'\.\?AV([A-Za-z][A-Za-z0-9_<>*,`\' ]+)@([A-Za-z0-9_]+@rage@@|rage@@|)',dc):
    td=g4dv+m.start()-8; inner=m.group(1).decode('ascii','replace')
    rest=m.group(2).decode('ascii','replace')
    ns_m=re.match(r'([A-Za-z0-9_]+)@rage@@',rest)
    cls=f"{ns_m.group(1)}::{inner}" if ns_m else (f"rage::{inner}" if rest=='rage@@' else inner)
    rtti_td[td]=cls

col_map={}
for off in range(0,g4rs-20,4):
    td=int.from_bytes(rc[off+12:off+16],'big')
    if td in rtti_td: col_map[g4rv+off]=rtti_td[td]

gta4_fn_vt={}
for off in range(0,g4rs-8,4):
    ptr=int.from_bytes(rc[off:off+4],'big')
    if ptr in col_map:
        vt=g4rv+off+4; nxt=int.from_bytes(rc[off+4:off+8],'big')
        if g4tv<=nxt<g4tv+g4ts:
            cls=col_map[ptr]
            for i in range(200):
                so=off+4+i*4
                if so+4>g4rs: break
                fn=int.from_bytes(rc[so:so+4],'big')
                if not(g4tv<=fn<g4tv+g4ts): break
                if fn not in gta4_fn_vt: gta4_fn_vt[fn]=(vt,i,cls)

# Load TT2 whitelist
tt2_rtti_raw = json.load(open(TT2RTTI))
TT2_CLS_SHORT = set()
with open(SYMS) as f:
    for line in f:
        if '= .text:0x' not in line: continue
        nm=line.split('=')[0].strip()
        m=re.match(r'^([a-zA-Z][a-zA-Z0-9]+)_',nm)
        if m: TT2_CLS_SHORT.add(m.group(1))
for cls in tt2_rtti_raw.values():
    TT2_CLS_SHORT.add(cshort(cls))

def in_tt2(cls): return cshort(cls) in TT2_CLS_SHORT

# BFS class propagation (whitelist only)
gta4_fn_cls={va:cls for va,(_,_,cls) in gta4_fn_vt.items() if in_tt2(cls)}
for _ in range(20):
    chg=False
    for va,cs in gta4_calls.items():
        if va in gta4_fn_cls: continue
        cc=Counter(gta4_fn_cls[c] for c in cs if c in gta4_fn_cls)
        if cc:
            d,n=cc.most_common(1)[0]
            if n/sum(cc.values())>=0.60 and n>=2 and in_tt2(d):
                gta4_fn_cls[va]=d; chg=True
    for va,crs in gta4_callers.items():
        if va in gta4_fn_cls: continue
        cc=Counter(gta4_fn_cls[c] for c in crs if c in gta4_fn_cls)
        if cc:
            d,n=cc.most_common(1)[0]
            if n/sum(cc.values())>=0.70 and n>=2 and in_tt2(d):
                gta4_fn_cls[va]=d; chg=True
    if not chg: break

print(f"  GTA4 fns with TT2-compatible class label: {len(gta4_fn_cls):,}")

# ── TT2 instruction normalizer (from raw PPC binary) ──────────────────────────
print("Loading TT2 binary...")
td = open(TT2,'rb').read()
ts = pe_sects(td)
ttv=next(s[1] for s in ts if s[0]=='.text'); tts=next(s[2] for s in ts if s[0]=='.text')
ttr=next(s[3] for s in ts if s[0]=='.text')
trv=next(s[1] for s in ts if s[0]=='.rdata'); trs=next(s[2] for s in ts if s[0]=='.rdata')
trr=next(s[3] for s in ts if s[0]=='.rdata')

tt2s={}; tt2m={}
with open(SYMS) as f:
    for line in f:
        if '= .text:0x' not in line: continue
        parts=line.split('=',1); nm=parts[0].strip()
        try:
            addr=int(parts[1].split(';')[0].strip().split(':')[1],16)
            tt2s[addr]=nm
            m=re.search(r'size:0x([0-9A-Fa-f]+)',line)
            if m: tt2m[addr]=int(m.group(1),16)
        except: pass

tt2a=sorted(a for a in tt2m if ttv<=a<ttv+tts)
tt2oa=[a-ttv for a in tt2a]; tt2fs=set(tt2a)

LOPS=frozenset([32,33,34,35,40,41,42,43,48,49,50,51])
SOPS=frozenset([36,37,38,39,44,45,46,47,52,53,54,55])
ALU ={14:'addi',15:'lis',7:'mulli',8:'subfic',12:'addic',13:'addic.',
      24:'ori',25:'oris',26:'xori',27:'xoris',28:'andi',29:'andis'}
CMP ={10:'cmpli',11:'cmpi'}

def normalize_word(w):
    """Normalize one PPC word to a canonical token, matching the recompiler normalizer."""
    op=(w>>26)&0x3F
    
    if op==18:   # b/bl/ba/bla
        lk=w&1; aa=(w>>1)&1
        return ('bl',) if lk else ('b',)
    if op==19:   # bclr/bcctr etc
        xop=(w>>1)&0x3FF
        if xop==16: return ('blr',)
        if xop==528: return ('bctr',)
        return ('bc',)
    if op==16:   # bc
        return ('bc',)
    
    # mflr/mtlr/mfctr/mtctr (op31, xop339/467/339/467)
    if op==31:
        xop=(w>>1)&0x3FF
        if xop==339: return ('mflr',) if (w>>21)&0x1F==0 else ('mfctr',)
        if xop==467: return ('mtlr',) if (w>>16)&0x1F==8 else ('mtctr',)
        return (31, xop)   # other extended
    
    # Load/store
    if op in LOPS or op in SOPS:
        ra=(w>>16)&0x1F; d=w&0xFFFF
        if d>0x7FFF: d-=0x10000
        tag='ld' if op in LOPS else 'st'
        if ra==1: return (tag,'sp',d)
        if 3<=ra<=11: return (tag,'obj',d)
        return (tag,)
    
    # stwu/stdu (stack frame)
    if op in (37,61):
        ra=(w>>16)&0x1F
        if ra==1: return ('stwu','sp')
        return ('stwu',)
    
    if op in ALU: return (ALU[op],)
    if op in CMP: return ('cmp',)
    return (op,)

def tt2_seq(va):
    sz=tt2m.get(va,0); base=ttr+(va-ttv)
    return tuple(normalize_word(int.from_bytes(td[base+k*4:base+k*4+4],'big'))
                 for k in range(sz//4))

# ── Build TT2 call graph + vtable membership ──────────────────────────────────
def owning(off):
    idx=bisect.bisect_right(tt2oa,off)-1
    return (ttv+tt2oa[idx]) if idx>=0 else None

tt2c=defaultdict(set); tt2cr=defaultdict(set)
for i in range(0,tts,4):
    w=int.from_bytes(td[ttr+i:ttr+i+4],'big')
    if (w>>26)&0x3F==18 and (w&1):
        li=w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        ca=owning(i); tg=ttv+i+li
        if ca and tg in tt2fs: tt2c[ca].add(tg); tt2cr[tg].add(ca)

tt2rtti={int(k,16):v for k,v in tt2_rtti_raw.items()}
tt2rc=td[trr:trr+trs]; tt2vm={}
for vt,cls in tt2rtti.items():
    for i in range(200):
        ro=trr+(vt+i*4-trv)
        if ro<0 or ro+4>len(tt2rc): break
        fn=int.from_bytes(tt2rc[ro:ro+4],'big')
        if not(ttv<=fn<ttv+tts): break
        if fn not in tt2vm: tt2vm[fn]=(vt,i,cls)

GENERIC=('fn_8','game_','rage_','ph_','grc_','grm_','cr_','fi_',
         'xe_','ke_','aud_','util_','vt','jumptable','nullvfn_')
def is_tgt(va): return any(tt2s.get(va,'').startswith(p) for p in GENERIC)

print(f"  TT2: {len(tt2a):,} fns, {len(tt2vm):,} vtable members, targets: {sum(1 for v in tt2a if is_tgt(v)):,}")

# ── Vtable slot index for GTA4 ────────────────────────────────────────────────
g4_slot = defaultdict(list)  # (cls_short, slot) -> [va]
for va,(_,slot,cls) in gta4_fn_vt.items():
    if in_tt2(cls): g4_slot[(cshort(cls),slot)].append(va)

# ── MATCHING ──────────────────────────────────────────────────────────────────
print("Matching TT2 functions against GTA4...")
matches = {}   # tt2_va -> (gta4_va, cls, mtype, conf)

total_hashed = 0; pass_a = pass_b = pass_c = 0

for va in tt2a:
    if not is_tgt(va): continue
    sz = tt2m.get(va,0)
    if sz < 8: continue

    seq = tt2_seq(va)
    if not seq: continue
    total_hashed += 1

    # ── Pass A: exact full-sequence hash ──────────────────────────────────────
    h = seq_hash(seq)
    if h in g4_unique_exact:
        gva = g4_unique_exact[h]
        cls = gta4_fn_cls.get(gva) or (gta4_fn_vt.get(gva,(None,None,''))[2])
        if cls and in_tt2(cls):
            matches[va] = (gva, cls, 'exact_seq', 98)
            pass_a += 1
            continue

    # ── Pass B: vtable slot position ──────────────────────────────────────────
    if va in tt2vm:
        _, slot, tt2_cls = tt2vm[va]
        cands = g4_slot.get((cshort(tt2_cls), slot), [])
        if len(cands) == 1:
            gva = cands[0]
            cls = gta4_fn_cls.get(gva) or gta4_fn_vt.get(gva,(None,None,''))[2]
            # Size sanity: TT2 instructions vs GTA4 line count (≈ instructions)
            t_insns = sz // 4
            g_lines = gta4_lcount.get(gva, 0)
            if g_lines > 0 and not (0.15 <= t_insns/g_lines <= 6.0):
                pass
            elif cls and in_tt2(cls):
                matches[va] = (gva, cls, 'vtable_slot', 85)
                pass_b += 1
                continue

    # ── Pass C: prefix hash (first 8 instructions) for longer functions ───────
    if len(seq) >= 12:
        ph = seq_hash(seq[:8])
        if ph in g4_unique_prefix:
            gva = g4_unique_prefix[ph]
            # Verify: suffix must also match reasonably (Jaccard on rest)
            g4_s = gta4_seq.get(gva, ())
            if len(g4_s) >= 8:
                min_l = min(len(seq), len(g4_s))
                common = sum(1 for a,b in zip(seq, g4_s) if a==b)
                jaccard = common / max(len(seq), len(g4_s))
                if jaccard >= 0.75:
                    cls = gta4_fn_cls.get(gva) or (gta4_fn_vt.get(gva,(None,None,''))[2])
                    if cls and in_tt2(cls):
                        matches[va] = (gva, cls, f'prefix+jaccard_{jaccard:.2f}', int(jaccard*100))
                        pass_c += 1

print(f"  Hashed TT2 targets: {total_hashed:,}")
print(f"  Pass A (exact sequence):     {pass_a:,}")
print(f"  Pass B (vtable slot):        {pass_b:,}")
print(f"  Pass C (prefix+Jaccard):     {pass_c:,}")
print(f"  Total:                       {len(matches):,}")

# ── Class distribution ────────────────────────────────────────────────────────
cd = Counter(cshort(v[1]) for v in matches.values())
print(f"\nTop 25 matched classes:")
for c,n in cd.most_common(25):
    print(f"  {c:<40s}: {n}")

# ── Sample a few matches for manual inspection ────────────────────────────────
print("\nSample Pass A matches:")
shown = 0
for va,(gva,cls,mt,conf) in sorted(matches.items()):
    if mt != 'exact_seq': continue
    t_seq = tt2_seq(va); g_seq = gta4_seq.get(gva,())
    print(f"  TT2:0x{va:08X} ({tt2s.get(va,'?')}) <- GTA4:0x{gva:08X} [{cls}]  insns={len(t_seq)}/{len(g_seq)}")
    shown += 1
    if shown >= 20: break

# ── Build seeds ───────────────────────────────────────────────────────────────
seeds = {}; nu = Counter()
for va,(gva,cls,mt,conf) in sorted(matches.items()):
    pfx = cshort(cls)
    if va in tt2vm:
        _,slot,_ = tt2vm[va]
        name = safe(f"{pfx}_vfn_{slot}")
    else:
        base = safe(f"{pfx}_{va&0xFFFF:04X}_g4")
        nu[base]+=1; name = base if nu[base]==1 else f"{base}_{nu[base]}"
    seeds[va] = (name, f"GTA4:0x{gva:08X} class={cls} type={mt} conf={conf}")

# ── Write outputs ─────────────────────────────────────────────────────────────
with open(OUT,'w') as f:
    f.write(f"cross_match_gta4_v3 — {len(seeds)} seeds\n{'='*60}\n\n")
    mc=Counter(v[2] for v in matches.values())
    for t,c in mc.most_common(): f.write(f"  {t:<40s}: {c}\n")
    f.write(f"\n{'='*60}\nSEEDS\n{'='*60}\n")
    for va,(name,reason) in sorted(seeds.items()):
        existing = tt2s.get(va,'?')
        t_seq = tt2_seq(va); g_seq = gta4_seq.get(matches[va][0],())
        f.write(f"0x{va:08X}  {existing:<40s}  ->  {name:<50s}  insns={len(t_seq)}/{len(g_seq)}  {reason}\n")

with open(SEEDS,'w') as f:
    f.write("# GTA4 cross-binary match seeds for auto_namer_v10.py\n")
    f.write(f"# {len(seeds)} seeds\n\n")
    f.write("CROSS_MATCH_SEEDS = {\n")
    for va,(name,reason) in sorted(seeds.items()):
        f.write(f"    0x{va:08X}: ({name!r}, {reason!r}),\n")
    f.write("}\n")

print(f"\nDone!\n  {OUT}\n  {SEEDS}")
