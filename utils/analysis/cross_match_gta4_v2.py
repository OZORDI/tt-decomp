#!/usr/bin/env python3
"""
cross_match_gta4_v2.py — GTA4 recompiler call-graph + vtable cross-matching
Reads:  GTA4 recompiler .cpp files (73K functions with call graphs)
        TT2 symbols_autonamed_v9.txt
        TT2 binary (for offset fingerprints)
        GTA4 binary (for RTTI vtable map)
Writes: cross_match_seeds_v2.py  (seeds dict for auto_namer_v10.py)
        cross_match_gta4_v2_results.txt  (full match log)
"""
import re, os, json, hashlib, bisect
from collections import defaultdict, Counter
from pathlib import Path

BASE    = '/Users/Ozordi/Downloads/tt-decomp'
GEN     = '/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated'
GTA4    = '/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/default.bin'
TT2     = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS    = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v9.txt')
TT2RTTI = os.path.join(BASE, 'rtti_vtable_map.json')
OUT     = os.path.join(BASE, 'cross_match_gta4_v2_results.txt')
SEEDS   = os.path.join(BASE, 'cross_match_seeds_v2.py')

RE_FUNC  = re.compile(r'^PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]+)\)', re.MULTILINE)
RE_CALL  = re.compile(r'\bsub_([0-9A-Fa-f]+)\(ctx,\s*base\)')
RE_LOAD  = re.compile(r'PPC_LOAD_U(?:8|16|32|64)\(ctx\.r(\d+)\.u32\s*\+\s*(-?\d+)\)')
RE_STORE = re.compile(r'PPC_STORE_U(?:8|16|32|64)\(ctx\.r(\d+)\.u32\s*\+\s*(-?\d+)\s*,')

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]','_',s)
    return re.sub(r'_+','_',s).strip('_')[:80]

def cls_prefix(cls):
    if '::' in cls:
        return safe_name(cls.split('::')[-1])
    return safe_name(cls)

# ── Parse GTA4 recompiler sources ─────────────────────────────────────────────
print("Parsing GTA4 recompiler sources (74 files)...")
gta4_calls   = {}          # fn_va -> set(callee_va)
gta4_callers = defaultdict(set)
gta4_offsets = {}          # fn_va -> frozenset of (op, offset)
gta4_linecount = {}        # fn_va -> line count (size proxy)
gta4_all_fns = set()

for fpath in sorted(Path(GEN).glob('gta4_recomp.*.cpp')):
    text = fpath.read_text(encoding='utf-8', errors='replace')
    func_starts = [(m.start(), int(m.group(1)[4:], 16)) for m in RE_FUNC.finditer(text)]
    for i, (fstart, fn_va) in enumerate(func_starts):
        fend  = func_starts[i+1][0] if i+1 < len(func_starts) else len(text)
        block = text[fstart:fend]
        gta4_all_fns.add(fn_va)
        callees = set()
        for m in RE_CALL.finditer(block):
            c = int(m.group(1), 16)
            if c != fn_va: callees.add(c)
        gta4_calls[fn_va] = callees
        for c in callees: gta4_callers[c].add(fn_va)
        offsets = set()
        for m in RE_LOAD.finditer(block):
            reg, off = int(m.group(1)), int(m.group(2))
            if reg == 3: offsets.add(('L', off))
        for m in RE_STORE.finditer(block):
            reg, off = int(m.group(1)), int(m.group(2))
            if reg == 3: offsets.add(('S', off))
        gta4_offsets[fn_va]   = frozenset(offsets)
        gta4_linecount[fn_va] = block.count('\n')

print(f"  Parsed: {len(gta4_all_fns):,} functions, "
      f"{sum(len(v) for v in gta4_calls.values()):,} call edges")

# ── Load GTA4 binary → RTTI vtable map ───────────────────────────────────────
print("Building GTA4 RTTI vtable map...")
gta4_data = open(GTA4,'rb').read()

def parse_pe(data):
    e=int.from_bytes(data[0x3C:0x40],'little'); co=e+4; oo=co+20
    ib=int.from_bytes(data[oo+28:oo+32],'little')
    os_=int.from_bytes(data[co+16:co+18],'little')
    ns=int.from_bytes(data[co+2:co+4],'little'); st=oo+os_; secs=[]
    for i in range(ns):
        s=data[st+i*40:st+i*40+40]
        nm=s[:8].rstrip(b'\x00').decode('ascii','replace')
        vs=int.from_bytes(s[8:12],'little'); va=int.from_bytes(s[12:16],'little')
        ro=int.from_bytes(s[20:24],'little'); rz=int.from_bytes(s[16:20],'little')
        secs.append((nm,ib+va,vs,ro,min(vs,rz)))
    return secs

g4sects = parse_pe(gta4_data)
g4tv = next(s[1] for s in g4sects if s[0]=='.text')
g4ts = next(s[2] for s in g4sects if s[0]=='.text')
g4rv = next(s[1] for s in g4sects if s[0]=='.rdata')
g4rs = next(s[2] for s in g4sects if s[0]=='.rdata')
g4rr = next(s[3] for s in g4sects if s[0]=='.rdata')
g4dv = next(s[1] for s in g4sects if s[0]=='.data')
g4ds = next(s[2] for s in g4sects if s[0]=='.data')
g4dr = next(s[3] for s in g4sects if s[0]=='.data')
rchunk = gta4_data[g4rr:g4rr+g4rs]
dchunk = gta4_data[g4dr:g4dr+g4ds]

# Type descriptors in .data
rtti_td = {}
for m in re.finditer(rb'\.\?AV([A-Za-z][A-Za-z0-9_<>*,`\' ]+)@([A-Za-z0-9_]+@rage@@|rage@@|)', dchunk):
    td_va = g4dv + m.start() - 8
    inner = m.group(1).decode('ascii','replace')
    rest  = m.group(2).decode('ascii','replace')
    ns_m  = re.match(r'([A-Za-z0-9_]+)@rage@@', rest)
    cls   = f"{ns_m.group(1)}::{inner}" if ns_m else (f"rage::{inner}" if rest=='rage@@' else inner)
    rtti_td[td_va] = cls

# COLs → vtables
col_map = {}
for off in range(0, g4rs-20, 4):
    td = int.from_bytes(rchunk[off+12:off+16],'big')
    if td in rtti_td: col_map[g4rv+off] = rtti_td[td]

gta4_fn_vtable = {}   # fn_va -> (vt_va, slot, cls)
gta4_vtable_cls = {}  # vt_va -> cls
for off in range(0, g4rs-8, 4):
    ptr = int.from_bytes(rchunk[off:off+4],'big')
    if ptr in col_map:
        vt_va   = g4rv + off + 4
        next_fn = int.from_bytes(rchunk[off+4:off+8],'big')
        if g4tv <= next_fn < g4tv+g4ts:
            cls = col_map[ptr]; gta4_vtable_cls[vt_va] = cls
            for i in range(200):
                so = off+4+i*4
                if so+4 > g4rs: break
                fn = int.from_bytes(rchunk[so:so+4],'big')
                if not (g4tv <= fn < g4tv+g4ts): break
                if fn not in gta4_fn_vtable: gta4_fn_vtable[fn] = (vt_va, i, cls)

# BFS: propagate class labels through call graph
gta4_fn_class = {va: cls for va, (_, _, cls) in gta4_fn_vtable.items()}
for _round in range(15):
    changed = False
    for fn_va, callees in gta4_calls.items():
        if fn_va in gta4_fn_class: continue
        cc = Counter(gta4_fn_class[c] for c in callees if c in gta4_fn_class)
        if cc:
            dom, cnt = cc.most_common(1)[0]
            if cnt / sum(cc.values()) >= 0.60 and cnt >= 2:
                gta4_fn_class[fn_va] = dom; changed = True
    for fn_va, callers_set in gta4_callers.items():
        if fn_va in gta4_fn_class: continue
        cc = Counter(gta4_fn_class[c] for c in callers_set if c in gta4_fn_class)
        if cc:
            dom, cnt = cc.most_common(1)[0]
            if cnt / sum(cc.values()) >= 0.70 and cnt >= 2:
                gta4_fn_class[fn_va] = dom; changed = True
    if not changed: break

print(f"  Vtables: {len(gta4_vtable_cls)}, vtable fns: {len(gta4_fn_vtable)}, "
      f"with class (after BFS): {len(gta4_fn_class):,}")

# ── Load TT2 ──────────────────────────────────────────────────────────────────
print("Loading TT2...")
tt2_data = open(TT2,'rb').read()
tt2sects = parse_pe(tt2_data)
tt2tv = next(s[1] for s in tt2sects if s[0]=='.text')
tt2ts = next(s[2] for s in tt2sects if s[0]=='.text')
tt2tr = next(s[3] for s in tt2sects if s[0]=='.text')
tt2rv = next(s[1] for s in tt2sects if s[0]=='.rdata')
tt2rs = next(s[2] for s in tt2sects if s[0]=='.rdata')
tt2rr = next(s[3] for s in tt2sects if s[0]=='.rdata')

tt2_sym = {}; tt2_meta = {}
with open(SYMS) as f:
    for line in f:
        s = line.strip()
        if '= .text:0x' in s:
            parts = s.split('=',1); name = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1],16)
                tt2_sym[addr] = name
                m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                if m: tt2_meta[addr] = int(m.group(1),16)
            except: pass

tt2_addrs   = sorted(a for a in tt2_meta if tt2tv <= a < tt2tv+tt2ts)
tt2_offsets_arr = [a-tt2tv for a in tt2_addrs]
tt2_fn_set  = set(tt2_addrs)

def owning_tt2(off):
    idx = bisect.bisect_right(tt2_offsets_arr, off) - 1
    return (tt2tv+tt2_offsets_arr[idx]) if idx >= 0 else None

# TT2 call graph
tt2_calls = defaultdict(set); tt2_callers = defaultdict(set)
for i in range(0, tt2ts, 4):
    w = int.from_bytes(tt2_data[tt2tr+i:tt2tr+i+4],'big')
    if (w>>26)&0x3F==18 and (w&1):
        li = w&0x3FFFFFC
        if li&0x2000000: li|=~0x3FFFFFF
        ca = owning_tt2(i); tg = tt2tv+i+li
        if ca and tg in tt2_fn_set:
            tt2_calls[ca].add(tg); tt2_callers[tg].add(ca)

# TT2 struct offset fingerprint
LOPS=frozenset([32,33,34,35,40,41,42,43,48,49,50,51])
SOPS=frozenset([36,37,38,39,44,45,46,47,52,53,54,55])
def tt2_off_fp(fn_va):
    sz = tt2_meta.get(fn_va,0)
    base = tt2tr+(fn_va-tt2tv); res=set()
    for k in range(sz//4):
        w=int.from_bytes(tt2_data[base+k*4:base+k*4+4],'big')
        op=(w>>26)&0x3F; ra=(w>>16)&0x1F; d=w&0xFFFF
        if d>0x7FFF: d-=0x10000
        if op in LOPS and ra==3: res.add(('L',d))
        elif op in SOPS and ra==3: res.add(('S',d))
    return frozenset(res)

# TT2 RTTI vtable membership
tt2_rtti_raw = json.load(open(TT2RTTI))
tt2_rtti_map = {int(k,16):v for k,v in tt2_rtti_raw.items()}
tt2_rc = tt2_data[tt2rr:tt2rr+tt2rs]
tt2_vt_member = {}
for vt_va, cls in tt2_rtti_map.items():
    for i in range(200):
        ro = tt2rr+(vt_va+i*4-tt2rv)
        if ro < 0 or ro+4 > len(tt2_rc): break
        fn = int.from_bytes(tt2_rc[ro:ro+4],'big')
        if not (tt2tv <= fn < tt2tv+tt2ts): break
        if fn not in tt2_vt_member: tt2_vt_member[fn] = (vt_va, i, cls)

print(f"  TT2: {len(tt2_addrs):,} fns, {len(tt2_vt_member):,} vtable members")

# ── Fingerprint index for GTA4 ────────────────────────────────────────────────
print("Indexing GTA4 fingerprints...")

def g4_fp(fn_va):
    nc = len(gta4_calls.get(fn_va, set()))
    nr = len(gta4_callers.get(fn_va, set()))
    of = gta4_offsets.get(fn_va, frozenset())
    ch = hashlib.md5(str(sorted(gta4_calls.get(fn_va,set()))).encode()).hexdigest()[:8]
    oh = hashlib.md5(str(sorted(of)).encode()).hexdigest()[:8]
    return (nc, nr, ch, oh)

g4_fp_idx = defaultdict(list)
for va in gta4_all_fns:
    g4_fp_idx[g4_fp(va)].append(va)

g4_unique = {fp: vl[0] for fp, vl in g4_fp_idx.items() if len(vl)==1}

# Separate index: by (n_calls, n_callers) bucket for partial matching
g4_by_nc = defaultdict(list)   # (n_calls, n_callers) -> [va]
for va in gta4_all_fns:
    nc = len(gta4_calls.get(va,set()))
    nr = len(gta4_callers.get(va,set()))
    g4_by_nc[(nc,nr)].append(va)

print(f"  GTA4 unique fingerprints: {len(g4_unique):,}")

# ── Match ─────────────────────────────────────────────────────────────────────
print("Matching...")

GENERIC = ('fn_8','game_','rage_','ph_','grc_','grm_','cr_','fi_',
           'xe_','ke_','aud_','util_','vt','jumptable','nullvfn_',
           'stub_','return_','get_','set_','const_ret_')
STDLIB  = frozenset(['memcpy','memset','malloc','free','strlen',
                     '_locale_register','rage_free','rage_malloc'])

def is_target(va):
    nm = tt2_sym.get(va,'')
    return any(nm.startswith(p) for p in GENERIC)

matches = {}  # tt2_va -> (gta4_va, cls, mtype, conf)

total_targets = sum(1 for v in tt2_addrs if is_target(v))
print(f"  TT2 target functions: {total_targets:,}")

# Pass A: exact fingerprint (callee set + caller set + offset set all match)
pA = 0
for fn_va in tt2_addrs:
    if not is_target(fn_va): continue
    nc = len(tt2_calls.get(fn_va, set()))
    nr = len(tt2_callers.get(fn_va, set()))
    if nc == 0 and nr == 0: continue  # no edges → too many possible matches
    of = tt2_off_fp(fn_va)
    oh = hashlib.md5(str(sorted(of)).encode()).hexdigest()[:8]
    ch = hashlib.md5(str(sorted(tt2_calls.get(fn_va,set()))).encode()).hexdigest()[:8]
    # NB: ch uses TT2 VAs which won't match GTA4 VAs — we use offset hash only for exact match
    fp = (nc, nr, oh)
    # Find GTA4 candidates with same nc, nr, offset hash
    for gva in g4_by_nc.get((nc, nr), []):
        g_of = gta4_offsets.get(gva, frozenset())
        g_oh = hashlib.md5(str(sorted(g_of)).encode()).hexdigest()[:8]
        if oh == g_oh and oh != hashlib.md5(b'frozenset()').hexdigest()[:8]:
            cls = gta4_fn_class.get(gva,'')
            if cls:
                if fn_va not in matches or 90 > matches[fn_va][3]:
                    matches[fn_va] = (gva, cls, 'exact_offset_fp', 90)
                    pA += 1
                break

print(f"  Pass A (exact offset fingerprint + edge counts): {pA:,}")

# Pass B: vtable slot position match (same class, same slot index)
pB = 0
# Build GTA4 vtable slot index: (cls_short, slot) -> [gta4_va]
g4_slot_idx = defaultdict(list)
for gva, (vt, slot, cls) in gta4_fn_vtable.items():
    short = cls.split('::')[-1] if '::' in cls else cls
    g4_slot_idx[(short, slot)].append(gva)

for fn_va, (tt2_vt, slot, tt2_cls) in tt2_vt_member.items():
    if not is_target(fn_va): continue
    if fn_va in matches: continue
    short = tt2_cls.split('::')[-1] if '::' in tt2_cls else tt2_cls
    candidates = g4_slot_idx.get((short, slot), [])
    if len(candidates) == 1:
        gva = candidates[0]
        cls = gta4_fn_class.get(gva, gta4_fn_vtable.get(gva,(None,None,''))[2])
        if cls:
            tt2_sz    = tt2_meta.get(fn_va, 0) // 4   # instructions
            gta4_lines= gta4_linecount.get(gva, 0)
            if tt2_sz > 0 and gta4_lines > 0:
                ratio = tt2_sz / max(gta4_lines, 1)
                if not (0.2 <= ratio <= 5.0): continue
            matches[fn_va] = (gva, cls, 'vtable_slot_unique', 85)
            pB += 1

print(f"  Pass B (vtable slot + unique in class): {pB:,}")

# Pass C: high-overlap struct offset Jaccard (≥0.75, min 4 offsets each)
pC = 0
for fn_va in tt2_addrs:
    if not is_target(fn_va): continue
    if fn_va in matches: continue
    tt2_of = tt2_off_fp(fn_va)
    if len(tt2_of) < 4: continue
    nc = len(tt2_calls.get(fn_va, set()))
    nr = len(tt2_callers.get(fn_va, set()))
    best_j = 0.75; best_match = None
    for delta_c in range(-1, 2):
        for delta_r in range(-1, 2):
            for gva in g4_by_nc.get((nc+delta_c, nr+delta_r), []):
                g_of = gta4_offsets.get(gva, frozenset())
                if len(g_of) < 4: continue
                inter = tt2_of & g_of; union = tt2_of | g_of
                j = len(inter) / len(union)
                if j > best_j:
                    cls = gta4_fn_class.get(gva,'')
                    if cls: best_j = j; best_match = (gva, cls)
    if best_match:
        gva, cls = best_match
        matches[fn_va] = (gva, cls, f'jaccard_{best_j:.2f}', int(best_j*100))
        pC += 1

print(f"  Pass C (Jaccard offset ≥0.75, ≥4 offsets): {pC:,}")

# Pass D: call-count + caller-count + class propagation from already-matched neighbors
pD = 0
changed = True
while changed:
    changed = False
    for fn_va in tt2_addrs:
        if not is_target(fn_va): continue
        if fn_va in matches: continue
        # What classes are in its 1-hop neighborhood?
        neighbor_classes = Counter()
        for c in tt2_calls.get(fn_va, set()):
            if c in matches: neighbor_classes[matches[c][1]] += 1
        for c in tt2_callers.get(fn_va, set()):
            if c in matches: neighbor_classes[matches[c][1]] += 0.5
        if not neighbor_classes: continue
        dom, score = neighbor_classes.most_common(1)[0]
        total = sum(neighbor_classes.values())
        if score/total >= 0.70 and score >= 2:
            # Find GTA4 fn in that class with same edge counts
            nc = len(tt2_calls.get(fn_va, set()))
            nr = len(tt2_callers.get(fn_va, set()))
            pfx = cls_prefix(dom)
            candidates = [v for v in g4_by_nc.get((nc,nr),[])
                         if gta4_fn_class.get(v,'') == dom]
            if len(candidates) == 1:
                gva = candidates[0]
                matches[fn_va] = (gva, dom, 'neighbor_propagate', 60)
                pD += 1; changed = True

print(f"  Pass D (neighbor propagation): {pD:,}")

print(f"\n  Total matched: {len(matches):,}")

# ── Build seeds ───────────────────────────────────────────────────────────────
seeds = {}
name_used = Counter()

for fn_va, (gva, cls, mtype, conf) in sorted(matches.items()):
    pfx = cls_prefix(cls)
    if fn_va in tt2_vt_member:
        _, slot, _ = tt2_vt_member[fn_va]
        name = safe_name(f"{pfx}_vfn_{slot}")
    else:
        base = safe_name(f"{pfx}_{fn_va&0xFFFF:04X}_g4")
        name_used[base] += 1
        name = base if name_used[base]==1 else f"{base}_{name_used[base]}"
    seeds[fn_va] = (name, f"GTA4:0x{gva:08X} class={cls} type={mtype} conf={conf}")

print(f"  Seeds: {len(seeds):,}")

# Class distribution
cls_dist = Counter(cls_prefix(v[1].split('class=')[1].split()[0]) for v in seeds.values())
print(f"\nTop 30 classes in seeds:")
for c,n in cls_dist.most_common(30):
    print(f"  {c:<40s}: {n}")

# ── Write output ──────────────────────────────────────────────────────────────
with open(OUT,'w') as f:
    f.write(f"cross_match_gta4_v2 — {len(seeds)} seeds\n{'='*60}\n\n")
    mc = Counter(v[2] for v in matches.values())
    for t,c in mc.most_common(): f.write(f"  {t:<40s}: {c}\n")
    f.write(f"\n{'='*60}\nSEEDS\n{'='*60}\n")
    for va,(name,reason) in sorted(seeds.items()):
        existing = tt2_sym.get(va,'?')
        f.write(f"0x{va:08X}  {existing:<35s}  ->  {name:<55s}  {reason}\n")

with open(SEEDS,'w') as f:
    f.write("# GTA4 cross-binary match seeds for auto_namer_v10.py\n")
    f.write(f"# {len(seeds)} seeds\n\n")
    f.write("CROSS_MATCH_SEEDS = {\n")
    for va,(name,reason) in sorted(seeds.items()):
        f.write(f"    0x{va:08X}: ({name!r}, {reason!r}),\n")
    f.write("}\n")

print(f"\nDone!\n  Results: {OUT}\n  Seeds:   {SEEDS}")
