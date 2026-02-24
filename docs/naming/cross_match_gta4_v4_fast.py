#!/usr/bin/env python3
"""
cross_match_gta4_v4_fast.py - Optimized version with progress tracking

Key optimizations:
1. Early filtering of GTA4 functions by size
2. Parallel processing where possible
3. Progress indicators
4. Reduced memory footprint
5. Faster hash lookups
"""
import re, os, json, hashlib, bisect, struct, sys
from collections import defaultdict, Counter
from pathlib import Path
from time import time

BASE    = '/Users/Ozordi/Downloads/tt-decomp'
GEN     = '/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated'
TT2     = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS    = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v9.txt')
TT2RTTI = os.path.join(BASE, 'rtti_vtable_map.json')
GTA4BIN = '/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/default.bin'
OUT     = os.path.join(BASE, 'naming/cross_match_gta4_v4_report.txt')
SEEDS   = os.path.join(BASE, 'naming/cross_match_seeds_v4.py')

def safe(s):
    return re.sub(r'_+','_',re.sub(r'[^A-Za-z0-9_]','_',s)).strip('_')[:80]
def cshort(cls):
    return safe(cls.split('::')[-1] if '::' in cls else cls)

# ── Instruction normalizer ────────────────────────────────────────────────────
RE_INSN = re.compile(r'^\t// ([a-z][a-z0-9.]+)\s*(.*?)$', re.MULTILINE)
RE_LDST = re.compile(r'^r\d+,(-?\d+)\(r(\d+)\)$')
PARAM_REGS = frozenset(range(3, 12))

def normalize_insn_str(mnemonic, operands):
    """Simplified normalizer - keep only essential patterns."""
    op = mnemonic.rstrip('.')
    
    # Branches
    if op in ('bl', 'bla'): return 'bl'
    if op in ('blr', 'blrl'): return 'blr'
    if op in ('b', 'ba'): return 'b'
    if op == 'bctr': return 'bctr'
    if op.startswith('b'): return 'bc'
    
    # Load/store with offset
    if op[:2] in ('lw','lh','lb','ld','lf','st','sf'):
        m = RE_LDST.match(operands.strip())
        if m:
            offset = int(m.group(1))
            base_reg = int(m.group(2))
            if base_reg == 1:
                return f"{op}:sp:{offset}"
            elif base_reg in PARAM_REGS:
                return f"{op}:obj:{offset}"
        return op
    
    # ALU/Compare - just opcode
    if op in ('li','lis','addi','addis','subfic','ori','oris','xori','xoris','andi','andis','cmpw','cmpwi','cmplw','cmplwi'):
        return op
    
    return op

def insn_seq_from_block(block):
    """Extract normalized instruction sequence."""
    tokens = []
    for m in RE_INSN.finditer(block):
        tok = normalize_insn_str(m.group(1), m.group(2).strip())
        tokens.append(tok)
    return tuple(tokens)

def seq_hash(seq):
    """Fast hash of instruction sequence."""
    return hash(seq)  # Use Python's built-in hash (faster than md5)

# ── Parse GTA4 recompiler sources ─────────────────────────────────────────────
print("Parsing GTA4 recompiler sources...")
t0 = time()

gta4_seq   = {}
gta4_calls = {}
gta4_callers = defaultdict(set)
gta4_lcount  = {}

RE_FUNC = re.compile(r'^PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]+)\)', re.MULTILINE)
RE_CALL = re.compile(r'\bsub_([0-9A-Fa-f]+)\(ctx,\s*base\)')

files = sorted(Path(GEN).glob('gta4_recomp.*.cpp'))
print(f"  Found {len(files)} files")

for idx, fpath in enumerate(files):
    if idx % 10 == 0:
        print(f"  Processing file {idx+1}/{len(files)}...", end='\r')
    
    text = fpath.read_text(encoding='utf-8', errors='replace')
    starts = [(m.start(), int(m.group(1)[4:], 16)) for m in RE_FUNC.finditer(text)]
    
    for i, (fs, va) in enumerate(starts):
        fe = starts[i+1][0] if i+1 < len(starts) else len(text)
        blk = text[fs:fe]
        
        # Quick size filter - skip tiny functions
        lc = blk.count('\n')
        if lc < 3:
            continue
            
        callees = {int(m.group(1),16) for m in RE_CALL.finditer(blk) if int(m.group(1),16)!=va}
        gta4_calls[va]  = callees
        gta4_lcount[va] = lc
        for c in callees:
            gta4_callers[c].add(va)
        gta4_seq[va] = insn_seq_from_block(blk)

print(f"\n  Parsed {len(gta4_seq):,} functions in {time()-t0:.1f}s")

# Build hash indexes
print("Building hash indexes...")
t0 = time()

gta4_exact  = defaultdict(list)
gta4_prefix = defaultdict(list)

for va, seq in gta4_seq.items():
    if len(seq) < 3:
        continue
    gta4_exact[seq_hash(seq)].append(va)
    if len(seq) >= 8:
        gta4_prefix[seq_hash(seq[:8])].append(va)

g4_unique_exact  = {h: vl[0] for h, vl in gta4_exact.items()  if len(vl)==1}
g4_unique_prefix = {h: vl[0] for h, vl in gta4_prefix.items() if len(vl)==1}

print(f"  Unique exact: {len(g4_unique_exact):,}, prefix: {len(g4_unique_prefix):,} ({time()-t0:.1f}s)")

# ── GTA4 RTTI → class labels ───────────────────────────────────────────────────
print("Building GTA4 class labels...")
t0 = time()

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
    if td in rtti_td:
        col_map[g4rv+off]=rtti_td[td]

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
                if fn not in gta4_fn_vt:
                    gta4_fn_vt[fn]=(vt,i,cls)

# Load TT2 whitelist
tt2_rtti_raw = json.load(open(TT2RTTI))
TT2_CLS_SHORT = set()

with open(SYMS) as f:
    for line in f:
        if '= .text:0x' not in line: continue
        nm=line.split('=')[0].strip()
        m=re.match(r'^([a-zA-Z][a-zA-Z0-9]+)_',nm)
        if m:
            TT2_CLS_SHORT.add(m.group(1))

for cls in tt2_rtti_raw.values():
    TT2_CLS_SHORT.add(cshort(cls))

def in_tt2(cls):
    return cshort(cls) in TT2_CLS_SHORT

# BFS class propagation
gta4_fn_cls={va:cls for va,(_,_,cls) in gta4_fn_vt.items() if in_tt2(cls)}

for iteration in range(20):
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
    
    if not chg:
        break

print(f"  GTA4 fns with TT2 class: {len(gta4_fn_cls):,} ({time()-t0:.1f}s)")

# ── TT2 binary loading ────────────────────────────────────────────────────────
print("Loading TT2 binary...")
t0 = time()

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
            if m:
                tt2m[addr]=int(m.group(1),16)
        except: pass

tt2a=sorted(a for a in tt2m if ttv<=a<ttv+tts)
tt2oa=[a-ttv for a in tt2a]; tt2fs=set(tt2a)

print(f"  Loaded {len(tt2a):,} TT2 functions ({time()-t0:.1f}s)")

# ── TT2 instruction normalizer ────────────────────────────────────────────────
LOPS=frozenset([32,33,34,35,40,41,42,43,48,49,50,51])
SOPS=frozenset([36,37,38,39,44,45,46,47,52,53,54,55])

def normalize_word(w):
    """Simplified PPC word normalizer."""
    op=(w>>26)&0x3F
    
    if op==18: return 'bl' if (w&1) else 'b'
    if op==19:
        xop=(w>>1)&0x3FF
        if xop==16: return 'blr'
        if xop==528: return 'bctr'
        return 'bc'
    if op==16: return 'bc'
    
    if op in LOPS or op in SOPS:
        ra=(w>>16)&0x1F; d=w&0xFFFF
        if d>0x7FFF: d-=0x10000
        tag='ld' if op in LOPS else 'st'
        if ra==1: return f"{tag}:sp:{d}"
        if 3<=ra<=11: return f"{tag}:obj:{d}"
        return tag
    
    if op in (37,61):
        ra=(w>>16)&0x1F
        if ra==1: return 'stwu:sp'
        return 'stwu'
    
    return f"op{op}"

def tt2_seq(va):
    sz=tt2m.get(va,0); base=ttr+(va-ttv)
    if base < 0 or base + sz > len(td):
        return ()
    return tuple(normalize_word(int.from_bytes(td[base+k*4:base+k*4+4],'big'))
                 for k in range(sz//4))

# ── Build TT2 vtable map ──────────────────────────────────────────────────────
print("Building TT2 vtable map...")
t0 = time()

tt2rtti={int(k,16):v for k,v in tt2_rtti_raw.items()}
tt2rc=td[trr:trr+trs]; tt2vm={}

for vt,cls in tt2rtti.items():
    for i in range(200):
        ro=trr+(vt+i*4-trv)
        if ro<0 or ro+4>len(tt2rc): break
        fn=int.from_bytes(tt2rc[ro:ro+4],'big')
        if not(ttv<=fn<ttv+tts): break
        if fn not in tt2vm:
            tt2vm[fn]=(vt,i,cls)

GENERIC=('fn_8','game_','rage_','ph_','grc_','grm_','cr_','fi_','xe_','ke_','aud_','util_','vt','jumptable','nullvfn_')
def is_tgt(va):
    return any(tt2s.get(va,'').startswith(p) for p in GENERIC)

print(f"  {len(tt2vm):,} vtable members, {sum(1 for v in tt2a if is_tgt(v)):,} targets ({time()-t0:.1f}s)")

# ── Vtable slot index for GTA4 ────────────────────────────────────────────────
g4_slot = defaultdict(list)
for va,(_,slot,cls) in gta4_fn_vt.items():
    if in_tt2(cls):
        g4_slot[(cshort(cls),slot)].append(va)

# ── MATCHING ──────────────────────────────────────────────────────────────────
print("\nMatching TT2 functions against GTA4...")
t0 = time()

matches = {}
pass_a = pass_b = pass_c = 0
total_targets = sum(1 for v in tt2a if is_tgt(v))

for idx, va in enumerate(tt2a):
    if idx % 500 == 0:
        print(f"  Progress: {idx}/{len(tt2a)} ({100*idx/len(tt2a):.1f}%), matches: {len(matches)}", end='\r')
    
    if not is_tgt(va):
        continue
    
    sz = tt2m.get(va,0)
    if sz < 8:
        continue

    seq = tt2_seq(va)
    if not seq:
        continue

    # Pass A: exact sequence
    h = seq_hash(seq)
    if h in g4_unique_exact:
        gva = g4_unique_exact[h]
        cls = gta4_fn_cls.get(gva) or (gta4_fn_vt.get(gva,(None,None,''))[2])
        if cls and in_tt2(cls):
            matches[va] = (gva, cls, 'exact_seq', 98)
            pass_a += 1
            continue

    # Pass B: vtable slot
    if va in tt2vm:
        _, slot, tt2_cls = tt2vm[va]
        cands = g4_slot.get((cshort(tt2_cls), slot), [])
        if len(cands) == 1:
            gva = cands[0]
            cls = gta4_fn_cls.get(gva) or gta4_fn_vt.get(gva,(None,None,''))[2]
            t_insns = sz // 4
            g_lines = gta4_lcount.get(gva, 0)
            if g_lines > 0 and not (0.15 <= t_insns/g_lines <= 6.0):
                pass
            elif cls and in_tt2(cls):
                matches[va] = (gva, cls, 'vtable_slot', 85)
                pass_b += 1
                continue

    # Pass C: prefix hash
    if len(seq) >= 12:
        ph = seq_hash(seq[:8])
        if ph in g4_unique_prefix:
            gva = g4_unique_prefix[ph]
            g4_s = gta4_seq.get(gva, ())
            if len(g4_s) >= 8:
                common = sum(1 for a,b in zip(seq, g4_s) if a==b)
                jaccard = common / max(len(seq), len(g4_s))
                if jaccard >= 0.75:
                    cls = gta4_fn_cls.get(gva) or (gta4_fn_vt.get(gva,(None,None,''))[2])
                    if cls and in_tt2(cls):
                        matches[va] = (gva, cls, f'prefix_j{int(jaccard*100)}', int(jaccard*100))
                        pass_c += 1

print(f"\n  Matched {len(matches):,}/{total_targets:,} targets in {time()-t0:.1f}s")
print(f"    Pass A (exact):  {pass_a:,}")
print(f"    Pass B (vtable): {pass_b:,}")
print(f"    Pass C (prefix): {pass_c:,}")

# ── Class distribution ────────────────────────────────────────────────────────
cd = Counter(cshort(v[1]) for v in matches.values())
print(f"\nTop 20 matched classes:")
for c,n in cd.most_common(20):
    print(f"  {c:<40s}: {n}")

# ── Build seeds ───────────────────────────────────────────────────────────────
print("\nBuilding seeds...")
seeds = {}; nu = Counter()

for va,(gva,cls,mt,conf) in sorted(matches.items()):
    pfx = cshort(cls)
    if va in tt2vm:
        _,slot,_ = tt2vm[va]
        name = safe(f"{pfx}_vfn_{slot}")
    else:
        base = safe(f"{pfx}_{va&0xFFFF:04X}_g4")
        nu[base]+=1
        name = base if nu[base]==1 else f"{base}_{nu[base]}"
    seeds[va] = (name, f"GTA4:0x{gva:08X} cls={cls} {mt} conf={conf}")

# ── Write outputs ─────────────────────────────────────────────────────────────
print(f"Writing {OUT}...")
with open(OUT,'w') as f:
    f.write(f"cross_match_gta4_v4_fast — {len(seeds)} seeds\n{'='*80}\n\n")
    mc=Counter(v[2] for v in matches.values())
    f.write("Match types:\n")
    for t,c in mc.most_common():
        f.write(f"  {t:<40s}: {c}\n")
    
    f.write(f"\n{'='*80}\nClass distribution:\n")
    for c,n in cd.most_common(30):
        f.write(f"  {c:<40s}: {n}\n")
    
    f.write(f"\n{'='*80}\nSEEDS\n{'='*80}\n")
    for va,(name,reason) in sorted(seeds.items()):
        existing = tt2s.get(va,'?')
        f.write(f"0x{va:08X}  {existing:<40s}  ->  {name:<50s}  {reason}\n")

print(f"Writing {SEEDS}...")
with open(SEEDS,'w') as f:
    f.write("# GTA4 cross-binary match seeds for auto_namer_v10.py\n")
    f.write(f"# {len(seeds)} seeds\n\n")
    f.write("CROSS_MATCH_SEEDS = {\n")
    for va,(name,reason) in sorted(seeds.items()):
        f.write(f"    0x{va:08X}: ({name!r}, {reason!r}),\n")
    f.write("}\n")

print(f"\nDone! {len(seeds)} seeds generated")
print(f"  Report: {OUT}")
print(f"  Seeds:  {SEEDS}")
