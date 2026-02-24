#!/usr/bin/env python3
"""
cross_match_gta4.py — Cross-binary function matching: Table Tennis 2 <-> GTA IV

Strategy:
  1. Load both PPC binaries
  2. For each function in both, compute instruction-opcode hash
     (strips all addresses/immediates, keeps only opcode+register structure)
  3. Build RTTI vtable map for GTA IV (named classes)
  4. Match by: (a) exact opcode hash, (b) size-bucketed structural similarity
  5. For each TT2 function with a match in GTA IV vtable -> propagate class name

Output: manual seeds dict for auto_namer_v10.py
"""
import re, os, struct, bisect, json, hashlib
from collections import defaultdict, Counter

BASE  = '/Users/Ozordi/Downloads/tt-decomp'
GTA4  = '/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/default.bin'
TT2   = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS  = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v9.txt')
RTTI  = os.path.join(BASE, 'rtti_vtable_map.json')
OUT   = os.path.join(BASE, 'cross_match_gta4_results.json')
SEEDS = os.path.join(BASE, 'cross_match_seeds.py')

# ── PE loader ────────────────────────────────────────────────────────────────
def load_pe(path):
    data = open(path,'rb').read()
    e_lfanew   = int.from_bytes(data[0x3C:0x40],'little')
    coff_off   = e_lfanew + 4
    opt_off    = coff_off + 20
    image_base = int.from_bytes(data[opt_off+28:opt_off+32],'little')
    opt_sz     = int.from_bytes(data[coff_off+16:coff_off+18],'little')
    num_sects  = int.from_bytes(data[coff_off+2:coff_off+4],'little')
    sect_tbl   = opt_off + opt_sz
    sections = []
    for i in range(num_sects):
        s = data[sect_tbl+i*40:sect_tbl+i*40+40]
        sname   = s[:8].rstrip(b'\x00').decode('ascii','replace')
        vsize   = int.from_bytes(s[8:12],'little')
        vaddr   = int.from_bytes(s[12:16],'little')
        raw_off = int.from_bytes(s[20:24],'little')
        raw_sz  = int.from_bytes(s[16:20],'little')
        sections.append((sname, image_base+vaddr, min(vsize,raw_sz), raw_off))
    
    def va_to_file(va):
        for _, abs_va, vsz, raw_off in sections:
            if abs_va <= va < abs_va+vsz:
                off = raw_off+(va-abs_va)
                return off if off < len(data) else None
        return None
    
    def read_be32(va):
        off = va_to_file(va)
        if off is None or off+4>len(data): return None
        return int.from_bytes(data[off:off+4],'big')
    
    return data, image_base, sections, va_to_file, read_be32

print("Loading binaries...")
tt2_data,  tt2_base,  tt2_sects,  tt2_v2f,  tt2_r32  = load_pe(TT2)
gta4_data, gta4_base, gta4_sects, gta4_v2f, gta4_r32 = load_pe(GTA4)

tt2_text  = next(s for s in tt2_sects  if s[0]=='.text')
gta4_text = next(s for s in gta4_sects if s[0]=='.text')
_, tt2_tvaddr,  tt2_tvsize,  tt2_traw  = tt2_text
_, gta4_tvaddr, gta4_tvsize, gta4_traw = gta4_text
print(f"  TT2:  .text 0x{tt2_tvaddr:08X}  size=0x{tt2_tvsize:08X}")
print(f"  GTA4: .text 0x{gta4_tvaddr:08X}  size=0x{gta4_tvsize:08X}")

# ── PPC opcode normalizer ─────────────────────────────────────────────────────
def normalize_insn(w):
    """
    Return (opcode_key) for a PPC instruction, masking out addresses and
    absolute immediates while keeping structural info (opcode, register roles).
    Returns None for pure data words.
    """
    op = (w>>26)&0x3F
    
    # Branch with link (bl) -> just 'bl'
    if op == 18:
        aa = (w>>1)&1
        lk = w&1
        return (18, lk)  # keep link-bit, discard address
    
    # bc (conditional branch) -> keep BO+BI structure
    if op == 16:
        bo = (w>>21)&0x1F
        bi = (w>>16)&0x1F
        return (16, bo&0x1E, bi&0x1C)  # mask out prediction bits
    
    # bclr/bcctr -> keep
    if op == 19:
        xop = (w>>1)&0x3FF
        return (19, xop)
    
    # Load/store: keep op + register roles, mask offset
    if op in (32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
              48,49,50,51,52,53,54,55):
        rd = (w>>21)&0x1F
        ra = (w>>16)&0x1F
        # Keep whether base is r1 (stack) or r3 (this ptr)
        ra_role = 1 if ra==1 else (3 if ra==3 else 0)
        return (op, ra_role)
    
    # ALU ops: keep opcode + dest/src register roles
    if op == 14:  # addi
        rd = (w>>21)&0x1F
        ra = (w>>16)&0x1F
        ra_role = 1 if ra==1 else (3 if ra==3 else (0 if ra==0 else 9))
        return (14, ra_role)
    
    if op == 15:  # lis
        return (15,)
    
    if op in (24,25,26,27,28,29):  # ori/xori etc
        return (op,)
    
    if op == 31:  # extended ops (add, subf, cmp, etc.)
        xop = (w>>1)&0x3FF
        return (31, xop)
    
    if op in (7,8,10,11,12,13):  # mulli, subfic, cmpli, cmpi, addic, addic.
        return (op,)
    
    # mflr, mtlr, mtctr, mfctr
    if op == 31 and (w>>1)&0x3FF == 339: return (31,339)
    if op == 31 and (w>>1)&0x3FF == 467: return (31,467)
    
    # blr
    if w == 0x4E800020: return ('blr',)
    if w == 0x4E800021: return ('blrl',)
    
    return (op,)

def fn_opcode_hash(binary_data, traw, fn_va, fn_size, tvaddr):
    """Compute opcode-normalized hash of a function."""
    off = traw + (fn_va - tvaddr)
    opcodes = []
    for k in range(fn_size//4):
        w = int.from_bytes(binary_data[off+k*4:off+k*4+4],'big')
        opcodes.append(normalize_insn(w))
    return hashlib.md5(str(opcodes).encode()).hexdigest(), tuple(opcodes)

def fn_size_bucket(sz):
    if sz <= 8:   return 'tiny'
    if sz <= 32:  return 'small'
    if sz <= 128: return 'medium'
    if sz <= 512: return 'large'
    return 'xlarge'

# ── Load TT2 symbols + function sizes ────────────────────────────────────────
print("\nLoading TT2 symbols...")
tt2_sym_map = {}
tt2_fn_meta = {}
with open(SYMS) as f:
    for line in f:
        s = line.strip()
        for sect in ['.text']:
            if '= .text:0x' in s:
                parts = s.split('=',1)
                name = parts[0].strip()
                try:
                    addr = int(parts[1].split(';')[0].strip().split(':')[1],16)
                    tt2_sym_map[addr] = name
                    m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                    if m: tt2_fn_meta[addr] = int(m.group(1),16)
                except: pass

tt2_fn_addrs = sorted(a for a in tt2_fn_meta if tt2_tvaddr <= a < tt2_tvaddr+tt2_tvsize)
tt2_fn_set   = set(tt2_fn_addrs)
print(f"  TT2 functions: {len(tt2_fn_addrs):,}")

# ── Build GTA IV function list from .pdata ───────────────────────────────────
print("\nBuilding GTA IV function list from .pdata...")
gta4_pdata = next(s for s in gta4_sects if s[0]=='.pdata')
_, pv, pvsize, praw = gta4_pdata
gta4_fns = {}  # addr -> size

for i in range(0, pvsize, 8):
    off = praw + i
    if off+8 > len(gta4_data): break
    start = int.from_bytes(gta4_data[off:off+4],'big')
    end   = int.from_bytes(gta4_data[off+4:off+8],'big')
    if gta4_tvaddr <= start < end <= gta4_tvaddr+gta4_tvsize:
        sz = end - start
        if 4 <= sz <= 0x10000:
            gta4_fns[start] = sz

print(f"  GTA IV functions: {len(gta4_fns):,}")

# ── Build GTA IV RTTI vtable map ─────────────────────────────────────────────
print("\nBuilding GTA IV RTTI -> vtable map...")
gta4_data_sec = next(s for s in gta4_sects if s[0]=='.data')
_, dv, dvsize, draw = gta4_data_sec
data_chunk = gta4_data[draw:draw+dvsize]

# Find all RTTI type descriptor strings
rtti_str_map = {}  # typeinfo_va -> class_name
for m in re.finditer(rb'\.\?AV([A-Za-z][A-Za-z0-9_<>*,`\' ]+)@([A-Za-z0-9_]+@rage@@|rage@@|)', data_chunk):
    file_off = draw + m.start()
    str_va   = dv + m.start()
    typeinfo_va = str_va - 8  # TypeDescriptor: [0]=vftable_ptr [4]=spare [8]=name
    inner = m.group(1).decode('ascii','replace')
    rest  = m.group(2).decode('ascii','replace')
    ns_m  = re.match(r'([A-Za-z0-9_]+)@rage@@', rest)
    if ns_m:
        cls = f"{ns_m.group(1)}::{inner}"
    elif rest == 'rage@@':
        cls = f"rage::{inner}"
    else:
        cls = inner
    rtti_str_map[typeinfo_va] = cls

print(f"  TypeDescriptors: {len(rtti_str_map)}")

# Scan .rdata for vtables: find COL -> typeinfo references
gta4_rdata = next(s for s in gta4_sects if s[0]=='.rdata')
_, rv, rvsize, rraw = gta4_rdata
rdata_chunk = gta4_data[rraw:rraw+rvsize]

# Build typeinfo VA -> class lookup
td_to_cls = rtti_str_map  # typeinfo_va -> cls

# Scan rdata for Complete Object Locators: [sig=0][offset][cdOffset][typeDescr_ptr][hier_ptr]
# Then vtable[-1] = ptr to COL
col_map = {}  # col_va -> cls
for off in range(0, rvsize-20, 4):
    sig = int.from_bytes(rdata_chunk[off:off+4],'big')
    if sig != 0: continue
    td_va = int.from_bytes(rdata_chunk[off+12:off+16],'big')
    if td_va in td_to_cls:
        col_va = rv + off
        col_map[col_va] = td_to_cls[td_va]

print(f"  COLs found: {len(col_map)}")

# Now find vtable starts: scan rdata for pointers to known COLs
gta4_vtable_map = {}  # vtable_va -> cls
for off in range(0, rvsize-8, 4):
    ptr = int.from_bytes(rdata_chunk[off:off+4],'big')
    if ptr in col_map:
        # vtable starts at off+4
        vtbl_va = rv + off + 4
        next_ptr = int.from_bytes(rdata_chunk[off+4:off+8],'big')
        if gta4_tvaddr <= next_ptr < gta4_tvaddr+gta4_tvsize:
            gta4_vtable_map[vtbl_va] = col_map[ptr]

print(f"  GTA4 vtables: {len(gta4_vtable_map)}")

# Build GTA4 fn_va -> class from vtables
gta4_fn_class = {}  # fn_va -> class_name
for vt_va, cls in gta4_vtable_map.items():
    for i in range(100):
        ptr_off = gta4_v2f(vt_va + i*4)
        if ptr_off is None: break
        fn_va = int.from_bytes(gta4_data[ptr_off:ptr_off+4],'big')
        if not (gta4_tvaddr <= fn_va < gta4_tvaddr+gta4_tvsize): break
        if fn_va not in gta4_fn_class:
            gta4_fn_class[fn_va] = cls

print(f"  GTA4 functions with class: {len(gta4_fn_class):,}")

# ── Compute opcode hashes for GTA IV ─────────────────────────────────────────
print("\nHashing GTA IV functions...")
gta4_hash_map   = {}  # hash -> list of fn_va
gta4_opcodes    = {}  # fn_va -> opcodes tuple
gta4_size_buckets = defaultdict(list)  # bucket -> [fn_va]

for fn_va, fn_sz in gta4_fns.items():
    if fn_sz < 8 or fn_sz > 0x4000: continue
    try:
        h, ops = fn_opcode_hash(gta4_data, gta4_traw, fn_va, fn_sz, gta4_tvaddr)
        gta4_hash_map.setdefault(h, []).append(fn_va)
        gta4_opcodes[fn_va] = ops
        gta4_size_buckets[fn_size_bucket(fn_sz)].append(fn_va)
    except: pass

# Only keep hashes that are unique in GTA4 (unambiguous match)
gta4_unique_hashes = {h: vlist[0] for h, vlist in gta4_hash_map.items() if len(vlist)==1}
print(f"  GTA4 unique hashes: {len(gta4_unique_hashes):,}")

# ── Match TT2 functions against GTA IV ───────────────────────────────────────
print("\nMatching TT2 functions against GTA IV...")
matches = []  # (tt2_va, gta4_va, cls, match_type)
match_types = Counter()
STDLIB_BLACKLIST = {
    'memcpy','memset','malloc','free','__savegprlr','__restgprlr',
}

# Determine which TT2 functions are worthwhile targets
def tt2_is_generic(va):
    nm = tt2_sym_map.get(va,'')
    return nm.startswith('fn_8') or nm.startswith('game_') or nm.startswith('rage_') \
        or nm.startswith('ph_') or nm.startswith('grc_') or nm.startswith('grm_') \
        or nm.startswith('cr_') or nm.startswith('fi_')

for fn_va in tt2_fn_addrs:
    if not tt2_is_generic(fn_va): continue
    fn_sz = tt2_fn_meta.get(fn_va, 0)
    if fn_sz < 8 or fn_sz > 0x4000: continue
    try:
        h, ops = fn_opcode_hash(tt2_data, tt2_traw, fn_va, fn_sz, tt2_tvaddr)
    except: continue
    
    # Exact hash match
    if h in gta4_unique_hashes:
        gta4_va = gta4_unique_hashes[h]
        cls = gta4_fn_class.get(gta4_va)
        if cls:
            matches.append((fn_va, gta4_va, cls, 'exact', h))
            match_types['exact_with_class'] += 1
        else:
            matches.append((fn_va, gta4_va, None, 'exact_no_class', h))
            match_types['exact_no_class'] += 1
        continue
    
    # For multi-hit hashes, still record if all hits have same class
    if h in gta4_hash_map:
        hits = gta4_hash_map[h]
        if len(hits) <= 5:
            classes = [gta4_fn_class.get(v) for v in hits if gta4_fn_class.get(v)]
            if classes and len(set(classes)) == 1:
                matches.append((fn_va, hits[0], classes[0], 'exact_multi_same_class', h))
                match_types['exact_multi_same_class'] += 1

print(f"\nMatch results:")
for mtype, cnt in match_types.most_common():
    print(f"  {mtype:<30s}: {cnt:,}")
print(f"  Total matches: {len(matches):,}")

# Useful matches = those with a class
useful = [(t2va, g4va, cls, mtype, h) for t2va, g4va, cls, mtype, h in matches if cls]
print(f"  With class info: {len(useful):,}")

# ── Build vtable-slot names ───────────────────────────────────────────────────
# For matched TT2 functions that are in TT2 vtables, give them slot names
print("\nBuilding vtable slot names for matched functions...")
tt2_rtti = json.load(open(RTTI))
tt2_rtti_map = {int(k,16): v for k,v in tt2_rtti.items()}

# Build TT2 vtable membership
tt2_vt_membership = {}  # fn_va -> (vt_va, slot, cls)
tt2_rdata = next(s for s in tt2_sects if s[0]=='.rdata')
_, tt2_rv, tt2_rvsize, tt2_rraw = tt2_rdata
tt2_rdata_chunk = tt2_data[tt2_rraw:tt2_rraw+tt2_rvsize]
for vt_va, cls in tt2_rtti_map.items():
    for i in range(80):
        ptr_off = tt2_rv + (vt_va + i*4 - tt2_rv)
        if ptr_off < 0 or ptr_off+4 > tt2_rvsize: break
        fn_va = int.from_bytes(tt2_rdata_chunk[ptr_off:ptr_off+4],'big')
        if not (tt2_tvaddr <= fn_va < tt2_tvaddr+tt2_tvsize): break
        if fn_va not in tt2_vt_membership:
            tt2_vt_membership[fn_va] = (vt_va, i, cls)

# ── Build output seeds ─────────────────────────────────────────────────────── 
print("\nBuilding name seeds...")

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

def rtti_class_to_prefix(cls):
    if '::' in cls:
        parts = cls.split('::')
        return safe_name(parts[-1])
    return safe_name(cls.split('_')[0] if '_' in cls else cls)

seeds = {}  # tt2_va -> (name, reason)
name_usage = Counter()

for tt2_va, gta4_va, cls, mtype, h in useful:
    existing = tt2_sym_map.get(tt2_va,'')
    
    # Skip already well-named
    if not (existing.startswith('fn_8') or existing.startswith('game_') or 
            existing.startswith('rage_') or existing.startswith('ph_') or 
            existing.startswith('grc_') or existing.startswith('grm_') or 
            existing.startswith('cr_') or existing.startswith('fi_')):
        continue
    
    safe_cls = rtti_class_to_prefix(cls)
    
    # Check if in TT2 vtable
    if tt2_va in tt2_vt_membership:
        vt_va, slot, vt_cls = tt2_vt_membership[tt2_va]
        name = safe_name(f"{safe_cls}_vfn_{slot}")
    else:
        base_name = safe_name(f"{safe_cls}_{tt2_va&0xFFFF:04X}_gta4")
        name_usage[base_name] += 1
        cnt = name_usage[base_name]
        name = base_name if cnt == 1 else f"{base_name}_{cnt}"
    
    reason = f"GTA4 match: 0x{gta4_va:08X} class={cls} type={mtype}"
    seeds[tt2_va] = (name, reason)

print(f"  Seeds generated: {len(seeds):,}")

# ── Write output ──────────────────────────────────────────────────────────────
# JSON results for inspection
results = {
    'matches': [(f'0x{t2}', f'0x{g4}', cls, mt) for t2,g4,cls,mt,_ in useful[:500]],
    'total_matches': len(useful),
    'gta4_vtables': len(gta4_vtable_map),
    'gta4_fn_class': len(gta4_fn_class),
}
with open(OUT, 'w') as f:
    json.dump(results, f, indent=2)

# Python seeds file for v10
print(f"\nWriting seeds to {SEEDS}...")
with open(SEEDS, 'w') as f:
    f.write("# GTA IV cross-binary match seeds for auto_namer_v10.py\n")
    f.write("# Generated by cross_match_gta4.py\n")
    f.write(f"# Total seeds: {len(seeds)}\n\n")
    f.write("CROSS_MATCH_SEEDS = {\n")
    for va, (name, reason) in sorted(seeds.items()):
        f.write(f"    0x{va:08X}: ({name!r}, {reason!r}),\n")
    f.write("}\n")

print(f"\nDone!")
print(f"Results: {OUT}")
print(f"Seeds:   {SEEDS}")
print(f"\nTop 30 matches by class:")
cls_counter = Counter(cls for _,_,cls,_,_ in useful if cls)
for cls, cnt in cls_counter.most_common(30):
    print(f"  {cls:<50s}: {cnt} matches")
