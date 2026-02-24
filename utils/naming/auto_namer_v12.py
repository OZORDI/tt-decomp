#!/usr/bin/env python3
"""
auto_namer_v12.py — V12 improvements
Reads symbols_autonamed_v11.txt → writes symbols_autonamed_v12.txt

New in v12 (from Path to 100% Coverage Research):
  P18: V12 BFS Re-convergence pass — catches remaining threshold boundary functions
  P19: Atexit / static init stub pass — pattern-matching lis/addi/b atexit (300+ targets)
  P20: Generic rename + cascade — renames rage_, ph_ bridge functions to unlock fn_8

Math: Projected to push coverage from 91.99% toward >94%.
"""
import re, os, json, struct, bisect
from collections import defaultdict, Counter, deque

BASE     = '/Users/Ozordi/Downloads/tt-decomp'
XEX      = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS     = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v11.txt')
OUT      = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v12.txt')
REPORT   = os.path.join(BASE, 'auto_namer_v12_report.txt')
RTTI_MAP = os.path.join(BASE, 'rtti_vtable_map.json')

# ── Binary loading ─────────────────────────────────────────────────────────────
print("Loading binary...")
data = open(XEX, 'rb').read()
e_lfanew     = int.from_bytes(data[0x3C:0x40], 'little')
coff_off     = e_lfanew + 4
opt_off      = coff_off + 20
image_base   = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18], 'little')
num_sects    = int.from_bytes(data[coff_off+2:coff_off+4], 'little')
sect_tbl     = opt_off + opt_hdr_size
sections     = []
for i in range(num_sects):
    s       = data[sect_tbl+i*40 : sect_tbl+i*40+40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii', 'replace')
    vsize   = int.from_bytes(s[8:12], 'little')
    vaddr   = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((sname, image_base + vaddr, vsize, raw_off))
    print(f"  {sname:12s}  VA=0x{image_base+vaddr:08X}")

text_sec  = next(s for s in sections if s[0] == '.text')
rdata_sec = next(s for s in sections if s[0] == '.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def va_to_file(va):
    for _, abs_va, vsize, raw_off in sections:
        if abs_va <= va < abs_va + vsize:
            return raw_off + (va - abs_va)
    return None

def read_be32_rdata(va):
    off = rraw + (va - rvaddr)
    if 0 <= off < rvsize - 3:
        return int.from_bytes(data[off:off+4], 'big')
    return None

# ── Symbol loading ─────────────────────────────────────────────────────────────
print("\nLoading symbols (from v10 output)...")
sym_map   = {}
sym_lines = []
fn_meta   = {}
with open(SYMS) as f:
    for line in f:
        sym_lines.append(line)
        s = line.strip()
        for sect in ['.text', '.rdata', '.data']:
            if '= ' + sect + ':0x' in s:
                parts = s.split('=', 1)
                name  = parts[0].strip()
                try:
                    addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                    sym_map[addr] = name
                    m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                    if m: fn_meta[addr] = int(m.group(1), 16)
                except:
                    pass

fn_addrs   = sorted(a for a in fn_meta if tvaddr <= a < tvaddr + tvsize)
fn_offsets = [a - tvaddr for a in fn_addrs]
fn_set     = set(fn_addrs)
unnamed    = {a for a in fn_addrs if sym_map.get(a, '').startswith('fn_8')}
game_named = {a for a in fn_addrs if sym_map.get(a, '').startswith('game_')}
print(f"  Total: {len(fn_addrs)}, fn_8: {len(unnamed)}, game_: {len(game_named)}")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr + fn_offsets[idx]) if idx >= 0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

# ── STDLIB BLACKLIST ───────────────────────────────────────────────────────────
STDLIB_BLACKLIST = {
    'memcpy', 'memset', 'malloc', 'free', 'realloc', 'calloc',
    'printf', 'sprintf', 'snprintf', 'vsnprintf', 'sscanf',
    'strlen', 'strcpy', 'strcat', 'strcmp', 'strncmp', 'strncpy',
    'memmove', 'memcmp', 'memchr', 'strchr', 'strrchr',
    'atexit', 'exit', 'abort', 'raise', 'longjmp', 'setjmp',
    'atoi', 'atol', 'atof', 'strtol', 'strtof', 'strtod',
    'qsort', 'bsearch', 'rand', 'srand',
    'fopen', 'fclose', 'fread', 'fwrite', 'fseek', 'ftell', 'fflush', 'feof',
    'fgets', 'fputs', 'puts', 'gets',
    'rage_free', 'rage_malloc', 'rage_stricmp',
    '__cxa_atexit', '__cxa_pure_virtual', '__pure_virtual',
    'XGetVideoMode', 'XGetAVPackFlags', 'XGetGameRegion',
    'XInputGetState', 'XInputSetState',
    'KfLowerIrql', 'KeRaiseIrqlToDpcLevel',
    'RtlEnterCriticalSection', 'RtlLeaveCriticalSection',
} | {f'__savegprlr_{i}' for i in range(14, 31)} \
  | {f'__restgprlr_{i}' for i in range(14, 31)} \
  | {'__savefpr', '__restfpr', '__savevmx', '__restvmx', '__savevecreg', '__restvecreg'}

# v10 generic prefixes (BFS opaque)
GENERIC_PFX = frozenset({
    'game', 'ph', 'xe', 'ke', 'grc', 'aud', 'rage', 'jumptable', 'ref', 'nt',
    'util', 'thunk', 'vt', 'nop', 'const', 'get', 'nullvfn', 'fn', 'io',
    'stub', 'sp', '2h', '2hr', 'loc', 'pdata', 'BINK', 'set', 'return',
})

# v11 extended transparent set (BFS looks through these)
# Does NOT include thunk, jumptable, nop — those have no class signal
TRANSPARENT_PFX = frozenset({
    'game', 'rage', 'ph', 'xe', 'ke', 'grc', 'aud', 'util', 'ref', 'nt', 'loc',
})

# ── Naming state ───────────────────────────────────────────────────────────────
new_names  = {}
confidence = {}
pass_tag   = {}
conf_score = {}
used_names = set(sym_map.values())
fn_class   = {}

PASS_CONFIDENCE = {
    # Inherited
    'SEED': 95,
    'P10': 65, 'P10r': 55,
    'P11': 68,
    'P4f': 72, 'P4r': 70, 'P4fw': 57, 'P4rh': 60,
    'P6': 48, 'P6r': 38,
    'P7': 42,
    # New v11
    'P12': 55, 'P12fw': 50, 'P12rh': 52,
    'P13': 80,
    'P14': 75,
    'P15': 50,
    'P16': 72,
    'P17': 42,
    # New v12
    'P18': 45,
    'P19': 95, 'P19s': 85,
    'P20': 75,
}

def propose(fn_va, name, reason, tag, overwrite=False):
    """P16 fix: extended retry from 15 → 50, full address for late attempts."""
    new_conf = PASS_CONFIDENCE.get(tag, 50)
    if fn_va in new_names:
        if not overwrite: return False
        if new_conf <= conf_score.get(fn_va, -1): return False
        used_names.discard(new_names[fn_va])
    original = name
    for attempt in range(50):          # P16 FIX: was 15
        if attempt == 0:
            candidate = name
        elif attempt <= 20:
            candidate = f"{original}_{fn_va&0xFFFF:04X}_{attempt}"
        else:
            candidate = f"{original}_{fn_va:08X}_{attempt}"  # guaranteed unique
        if candidate not in used_names or (fn_va in new_names and new_names[fn_va] == candidate):
            new_names[fn_va]  = candidate
            confidence[fn_va] = reason
            pass_tag[fn_va]   = tag
            conf_score[fn_va] = new_conf
            used_names.add(candidate)
            return True
    return False

def get_class_clean(fn_va, extra=None):
    if fn_va in fn_class: return fn_class[fn_va]
    nm = (extra or {}).get(fn_va) or new_names.get(fn_va) or sym_map.get(fn_va, '')
    if not nm or nm.startswith('fn_8'): return None
    if nm in STDLIB_BLACKLIST: return None
    if nm.startswith('__'): return None
    parts = nm.split('_')
    if not parts or parts[0] in GENERIC_PFX: return None
    return parts[0]

def is_transparent(fn_va):
    """True if this node should be looked-through in extended BFS."""
    nm = new_names.get(fn_va) or sym_map.get(fn_va, '')
    if not nm or nm.startswith('fn_8'): return False
    p = nm.split('_')[0]
    return p in TRANSPARENT_PFX

def rtti_class_to_prefix(cls):
    parts = cls.split('::')
    return safe_name(parts[-1]) if len(parts) <= 2 else safe_name('_'.join(parts[1:]))

# ── Build Callgraphs ───────────────────────────────────────────────────────────
print("\nBuilding callgraphs...")
forward_cg = defaultdict(set)
reverse_cg = defaultdict(set)
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4], 'big')
    if (w >> 26) & 0x3F == 18 and (w & 1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va    = tvaddr + i + li
        if caller_va and tgt_va in fn_set:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
print(f"  bl edges from {len(forward_cg)} functions")

fn_ptr_fwd = defaultdict(set)
fn_ptr_rev = defaultdict(set)
pending = {}
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op = (w >> 26) & 0x3F; rd = (w >> 21) & 0x1F
    ra = (w >> 16) & 0x1F; imm = w & 0xFFFF
    if op == 15 and ra == 0:
        signed_hi = imm if imm < 0x8000 else imm - 0x10000
        pending[rd] = signed_hi << 16
    elif op == 14 and ra in pending:
        lo = imm if imm < 0x8000 else imm - 0x10000
        addr = (pending[ra] + lo) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr + tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr); fn_ptr_rev[addr].add(caller)
    elif op == 24 and ra in pending:
        addr = (pending[ra] | imm) & 0xFFFFFFFF
        pending[rd] = addr
        if tvaddr <= addr < tvaddr + tvsize and addr in fn_set:
            caller = owning_fn(i)
            if caller and caller != addr:
                fn_ptr_fwd[caller].add(addr); fn_ptr_rev[addr].add(caller)
    else:
        if rd in pending and op not in (15, 14, 24):
            pending.pop(rd, None)

def all_edges(fn_va):
    return (forward_cg.get(fn_va, set()) | reverse_cg.get(fn_va, set()) |
            fn_ptr_fwd.get(fn_va, set()) | fn_ptr_rev.get(fn_va, set()))

# ── PASS 14: RTTI Vtable Completor ────────────────────────────────────────────
print("\n=== PASS 14: RTTI vtable completor ===")
p14 = 0
p14_fn8 = 0

try:
    rtti = json.load(open(RTTI_MAP))
    print(f"  Loaded {len(rtti)} RTTI vtable entries")
except FileNotFoundError:
    rtti = {}
    print("  WARNING: rtti_vtable_map.json not found, skipping P14")

for k, cls_name in rtti.items():
    try:
        vt_va = int(k, 16)
    except:
        continue
    if not (rvaddr <= vt_va < rvaddr + rvsize):
        continue
    cls_prefix = rtti_class_to_prefix(cls_name)

    for slot in range(256):
        fn_ptr = read_be32_rdata(vt_va + slot * 4)
        if fn_ptr is None:
            break
        if not (tvaddr <= fn_ptr < tvaddr + tvsize):
            break
        if fn_ptr not in fn_set:
            break
        nm = new_names.get(fn_ptr) or sym_map.get(fn_ptr, '')
        if nm.startswith('vt') or nm.startswith('fn_8'):
            was_fn8 = nm.startswith('fn_8')
            base_nm = safe_name(f"{cls_prefix}_vfn_{slot}")
            reason  = f"RTTI vtable {cls_name} slot {slot} @ 0x{vt_va:08X}"
            if propose(fn_ptr, base_nm, reason, 'P14'):
                fn_class[fn_ptr] = cls_prefix
                p14 += 1
                if was_fn8: p14_fn8 += 1

print(f"  P14: {p14} vtable slots renamed ({p14_fn8} were fn_8)")

# ── PASS 19: Atexit / Static Init Stubs ───────────────────────────────────────
print("\n=== PASS 19: Atexit / Static Init Stubs ===")
p19 = 0; p19s = 0

atexit_va = None
for va, nm in sym_map.items():
    if nm == 'atexit':
        atexit_va = va
        break

if atexit_va:
    for fn_va in sorted(unnamed):
        if fn_va in new_names: continue
        size = fn_meta.get(fn_va, 0)
        
        raw = va_to_file(fn_va)
        if not raw: continue
        
        dtor_va = None
        for off in range(0, size, 4):
            if raw + off + 12 > len(data): break
            ins_w = int.from_bytes(data[raw+off : raw+off+4], 'big')
            op = (ins_w >> 26) & 0x3F; rd = (ins_w >> 21) & 0x1F; ra = (ins_w >> 16) & 0x1F; imm = ins_w & 0xFFFF
            
            if op == 15 and ra == 0:  # lis rX, HI
                hi = imm if imm < 0x8000 else imm - 0x10000
                ins2 = int.from_bytes(data[raw+off+4 : raw+off+8], 'big')
                op2 = (ins2 >> 26) & 0x3F; rd2 = (ins2 >> 21) & 0x1F; ra2 = (ins2 >> 16) & 0x1F; imm2 = ins2 & 0xFFFF
                if op2 == 14 and rd2 == 3 and ra2 == rd:  # addi r3, rX, LO
                    lo = imm2 if imm2 < 0x8000 else imm2 - 0x10000
                    target_addr = ((hi << 16) + lo) & 0xFFFFFFFF
                    
                    ins3 = int.from_bytes(data[raw+off+8 : raw+off+12], 'big')
                    op3 = (ins3 >> 26) & 0x3F
                    if op3 == 18: # b or bl
                        li = ins3 & 0x3FFFFFC
                        if li & 0x2000000: li |= ~0x3FFFFFF
                        b_target = (fn_va + off + 8 + li) & 0xFFFFFFFF
                        if b_target == atexit_va:
                            dtor_va = target_addr
                            break
                            
        if dtor_va is not None:
            cls = get_class_clean(dtor_va)
            if not cls and dtor_va in new_names:
                cls = get_class_clean(dtor_va, new_names)
            
            if cls:
                nm = safe_name(f"static_atexit_stub_{cls}_{fn_va&0xFFFF:04X}")
                if propose(fn_va, nm, f"atexit stub to dtor '{cls}'", 'P19'):
                    fn_class[fn_va] = cls; p19 += 1
            else:
                if fn_va >= 0x8257BBE0 and fn_va <= 0x825A0000:
                    nm = safe_name(f"xam_static_dtor_{fn_va&0xFFFF:04X}")
                    if propose(fn_va, nm, f"xam static dtor mega-cluster", 'P19s'):
                        fn_class[fn_va] = 'xam'; p19s += 1
                else:
                    nm = safe_name(f"static_atexit_stub_{fn_va&0xFFFF:04X}")
                    if propose(fn_va, nm, f"atexit stub to unknown dtor 0x{dtor_va:08X}", 'P19s'):
                        p19s += 1
print(f"  P19: {p19} semantically named, {p19s} generically named")

# ── PASS 10: game_ Semi-Transparent BFS (inherited from v10) ──────────────────
print("\n=== PASS 10: game_ semi-transparent BFS (re-run) ===")
p10 = 0; p10r = 0

def bfs_vote_game_transparent(fn_va):
    cc = Counter()
    for c in forward_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls:
            cc[cls] += 1.0
        elif sym_map.get(c, '').startswith('game_') or new_names.get(c, '').startswith('game_'):
            for cc2 in (list(forward_cg.get(c, set())) + list(reverse_cg.get(c, set()))):
                if cc2 == fn_va: continue
                cls2 = get_class_clean(cc2)
                if cls2: cc[cls2] += 0.3
    for c in reverse_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls:
            cc[cls] += 0.8
        elif sym_map.get(c, '').startswith('game_') or new_names.get(c, '').startswith('game_'):
            for cc2 in (list(forward_cg.get(c, set())) + list(reverse_cg.get(c, set()))):
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
        nm = sym_map.get(fn_va, '')
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
                tag = 'P10'; nm_new = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}")
            else:
                tag = 'P10r'; nm_new = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_g")
            if propose(fn_va, nm_new, f"game-transparent BFS score={score:.1f}/{total:.1f} class='{dominant}'", tag):
                fn_class[fn_va] = dominant; rc += 1
                if is_fn8: p10 += 1
                else: p10r += 1
    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc} (fn8={p10} game_={p10r} cumulative)")
print(f"  P10 total: {p10} fn_8 named, {p10r} game_ reclassified")

# ── PASS 11: Dominant-Caller Threshold ────────────────────────────────────────
print("\n=== PASS 11: Dominant-caller threshold ===")
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
    total_wc = sum(cc.values())
    if total_wc < 4: continue
    ratio = cnt / total_wc
    if ratio >= 0.80:
        safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_dc")
        if propose(fn_va, nm, f"dominant-caller {cnt}/{total_wc} ({ratio:.0%}) class='{dominant}'", 'P11'):
            fn_class[fn_va] = dominant; p11 += 1
print(f"  P11: {p11}")

# ── P4 re-run (Round 1) ───────────────────────────────────────────────────────
print("\n=== P4 re-run (round 1): standard BFS ===")
p4f = 0; p4fw = 0; p4rh = 0

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
        total = sum(cc.values()); ratio = score / total
        safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant.split('_')[0] if '_' in dominant else dominant
        if score >= 2.0 and ratio >= 0.15:
            nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}")
            if propose(fn_va, nm, f"BFS re-run {score:.1f}/{total:.1f} '{dominant}'", 'P4f'):
                fn_class[fn_va] = dominant; rc += 1; p4f += 1
            continue
        fwd_calls = forward_cg.get(fn_va, set())
        rev_callers = reverse_cg.get(fn_va, set())
        if len(fwd_calls) == 1:
            cls = get_class_clean(list(fwd_calls)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_fw")
                if propose(fn_va, nm, f"single callee re-run '{dominant}'", 'P4fw'):
                    fn_class[fn_va] = dominant; rc += 1; p4fw += 1
                continue
        if len(rev_callers) == 1:
            cls = get_class_clean(list(rev_callers)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h")
                if propose(fn_va, nm, f"single caller re-run '{dominant}'", 'P4rh'):
                    fn_class[fn_va] = dominant; rc += 1; p4rh += 1
                continue
    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc} (f={p4f} fw={p4fw} rh={p4rh})")
print(f"  P4 round 1 totals: f={p4f} fw={p4fw} rh={p4rh}")

# ── PASS 12: Extended-Transparency BFS ────────────────────────────────────────
print("\n=== PASS 12: Extended-transparency BFS (all generic prefixes) ===")
p12 = 0; p12fw = 0; p12rh = 0

def bfs_vote_extended(fn_va):
    """Like P10 but transparent for ALL generic prefixes, not just game_."""
    cc = Counter()
    for c in forward_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls:
            cc[cls] += 1.0
        elif is_transparent(c):
            for cc2 in (list(forward_cg.get(c, set())) + list(reverse_cg.get(c, set()))):
                if cc2 == fn_va: continue
                cls2 = get_class_clean(cc2)
                if cls2: cc[cls2] += 0.3
    for c in reverse_cg.get(fn_va, set()):
        cls = get_class_clean(c)
        if cls:
            cc[cls] += 0.8
        elif is_transparent(c):
            for cc2 in (list(forward_cg.get(c, set())) + list(reverse_cg.get(c, set()))):
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

for iteration in range(100):
    rc = 0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        cc = bfs_vote_extended(fn_va)
        if not cc: continue
        dominant, score = cc.most_common(1)[0]
        total = sum(cc.values()); ratio = score / total
        safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant.split('_')[0] if '_' in dominant else dominant

        # Main P12 threshold (weaker than P10)
        if score >= 1.0 and ratio >= 0.15:
            nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_w")
            if propose(fn_va, nm, f"extended-BFS score={score:.1f}/{total:.1f} class='{dominant}'", 'P12'):
                fn_class[fn_va] = dominant; rc += 1; p12 += 1
            continue

        # P12 sub-passes: single fw/rh
        fwd_calls   = forward_cg.get(fn_va, set())
        rev_callers = reverse_cg.get(fn_va, set())
        if len(fwd_calls) == 1:
            cls = get_class_clean(list(fwd_calls)[0])
            if cls and score >= 0.7:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_wfw")
                if propose(fn_va, nm, f"extended single-callee '{dominant}'", 'P12fw'):
                    fn_class[fn_va] = dominant; rc += 1; p12fw += 1
                continue
        if len(rev_callers) == 1:
            cls = get_class_clean(list(rev_callers)[0])
            if cls and score >= 0.7:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_wrh")
                if propose(fn_va, nm, f"extended single-caller '{dominant}'", 'P12rh'):
                    fn_class[fn_va] = dominant; rc += 1; p12rh += 1
                continue
    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc} (p12={p12} fw={p12fw} rh={p12rh} cumulative)")
print(f"  P12 totals: main={p12} fw={p12fw} rh={p12rh}")

# ── P4 re-run (Round 2: cascade from P12) ────────────────────────────────────
print("\n=== P4 re-run (round 2): cascade from P12 ===")
p4f2 = 0; p4fw2 = 0; p4rh2 = 0
for iteration in range(80):
    rc = 0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        cc = bfs_class_vote(fn_va)
        if not cc: continue
        dominant, score = cc.most_common(1)[0]
        total = sum(cc.values()); ratio = score / total
        safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant.split('_')[0] if '_' in dominant else dominant
        if score >= 2.0 and ratio >= 0.15:
            nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}")
            if propose(fn_va, nm, f"BFS cascade2 {score:.1f}/{total:.1f} '{dominant}'", 'P4f'):
                fn_class[fn_va] = dominant; rc += 1; p4f2 += 1
            continue
        fwd_calls = forward_cg.get(fn_va, set())
        rev_callers = reverse_cg.get(fn_va, set())
        if len(fwd_calls) == 1:
            cls = get_class_clean(list(fwd_calls)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_fw")
                if propose(fn_va, nm, f"cascade2 single-callee '{dominant}'", 'P4fw'):
                    fn_class[fn_va] = dominant; rc += 1; p4fw2 += 1
                continue
        if len(rev_callers) == 1:
            cls = get_class_clean(list(rev_callers)[0])
            if cls and score >= 0.9:
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_h")
                if propose(fn_va, nm, f"cascade2 single-caller '{dominant}'", 'P4rh'):
                    fn_class[fn_va] = dominant; rc += 1; p4rh2 += 1
                continue
    if rc == 0: break
    print(f"  Round {iteration+1}: +{rc}")
print(f"  P4 round 2 totals: f={p4f2} fw={p4fw2} rh={p4rh2}")

# ── P6 re-run ─────────────────────────────────────────────────────────────────
print("\n=== P6 re-run: 2-hop bridging ===")
p6 = 0; p6r = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    neighbors = all_edges(fn_va)
    if not neighbors: continue
    named_1hop = [n for n in neighbors
                  if not sym_map.get(n, '').startswith('fn_8')
                  and not new_names.get(n, '').startswith('fn_8')]
    if named_1hop: continue
    two_hop = Counter()
    for mid in neighbors:
        for far in all_edges(mid):
            if far == fn_va: continue
            cls = get_class_clean(far)
            if cls: two_hop[cls] += 1
    if not two_hop: continue
    dom, cnt = two_hop.most_common(1)[0]
    total = sum(two_hop.values()); ratio = cnt / total
    safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom
    if cnt >= 2 and ratio >= 0.40:
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_2h")
        if propose(fn_va, nm, f"2-hop re-run '{dom}' {cnt}/{total}", 'P6'):
            fn_class[fn_va] = dom; p6 += 1
    elif cnt >= 1 and ratio >= 0.30:
        nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_2hr")
        if propose(fn_va, nm, f"2-hop relaxed re-run '{dom}' {cnt}/{total}", 'P6r'):
            fn_class[fn_va] = dom; p6r += 1
print(f"  P6 re-run: {p6} strict, {p6r} relaxed")

# ── PASS 20: Generic Rename + Cascade ─────────────────────────────────────────
print("\n=== PASS 20: Generic rename + cascade ===")
p20 = 0; p20c = 0

for fn_va in fn_addrs:
    nm = new_names.get(fn_va) or sym_map.get(fn_va, '')
    if nm.startswith('fn_8') or nm.startswith('sub_'): continue
    
    p = nm.split('_')[0]
    if p not in GENERIC_PFX: continue
    if p in ('thunk', 'nop', 'vt', 'jumptable'): continue
    
    cc = bfs_class_vote(fn_va)
    if not cc: continue
    
    dom, score = cc.most_common(1)[0]
    total = sum(cc.values()); ratio = score / total
    
    if score >= 3.0 and ratio >= 0.80:
        safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom.split('_')[0] if '_' in dom else dom
        nm_new = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_gen")
        if propose(fn_va, nm_new, f"generic rename {score:.1f}/{total:.1f} '{dom}'", 'P20', overwrite=True):
            fn_class[fn_va] = dom
            p20 += 1

if p20 > 0:
    for iteration in range(20):
        rc = 0
        for fn_va in unnamed:
            if fn_va in new_names: continue
            cc = bfs_class_vote(fn_va)
            if not cc: continue
            dom, score = cc.most_common(1)[0]
            total = sum(cc.values()); ratio = score / total
            if score >= 2.0 and ratio >= 0.15:
                safe_cls = rtti_class_to_prefix(dom) if '::' in dom else dom.split('_')[0] if '_' in dom else dom
                nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_gcas")
                if propose(fn_va, nm, f"cascade from P20 {score:.1f}/{total:.1f} '{dom}'", 'P20'):
                    fn_class[fn_va] = dom; rc += 1; p20c += 1
        if rc == 0: break
print(f"  P20: {p20} generics renamed, {p20c} fn_8 cascade unlocked")

# ── PASS 15: Island BFS ────────────────────────────────────────────────────────
print("\n=== PASS 15: Island BFS (fn_8-connected components) ===")
p15 = 0

def get_unnamed_now():
    return {va for va in unnamed if va not in new_names}

unnamed_now = get_unnamed_now()

def find_island(fn_va):
    visited = {fn_va}; queue = deque([fn_va])
    while queue:
        v = queue.popleft()
        for n in all_edges(v):
            if n in unnamed_now and n not in visited:
                visited.add(n); queue.append(n)
    return visited

islands_done = set()
island_proposals = []

for fn_va in sorted(unnamed_now):
    if fn_va in islands_done: continue
    island = find_island(fn_va)
    islands_done |= island

    vote = Counter()
    for member in island:
        for n in all_edges(member):
            if n not in unnamed_now:
                cls = get_class_clean(n)
                if cls: vote[cls] += 1

    if not vote: continue
    dom, cnt = vote.most_common(1)[0]
    total = sum(vote.values()); ratio = cnt / total

    if cnt >= 3 and ratio >= 0.50:
        for member in sorted(island):
            nm = safe_name(f"{dom}_{member&0xFFFF:04X}_isl")
            if propose(member, nm, f"island BFS '{dom}' {cnt}/{total}", 'P15'):
                fn_class[member] = dom; p15 += 1

print(f"  P15: {p15}")

# ── PASS 18: V12 BFS Re-convergence ───────────────────────────────────────────
print("\n=== PASS 18: V12 BFS Re-convergence ===")
p18 = 0
unnamed_now_p18 = {va for va in unnamed if va not in new_names}

for iteration in range(50):
    rc = 0
    for fn_va in list(unnamed_now_p18):
        if fn_va in new_names:
            unnamed_now_p18.remove(fn_va)
            continue
        cc = bfs_vote_extended(fn_va)
        if not cc: continue
        dominant, score = cc.most_common(1)[0]
        total = sum(cc.values()); ratio = score / total
        
        if score >= 0.8 and ratio >= 0.65:
            safe_cls = rtti_class_to_prefix(dominant) if '::' in dominant else dominant.split('_')[0] if '_' in dominant else dominant
            nm = safe_name(f"{safe_cls}_{fn_va&0xFFFF:04X}_v12")
            if propose(fn_va, nm, f"V12 reconvergence {score:.1f}/{total:.1f} '{dominant}'", 'P18'):
                fn_class[fn_va] = dominant; rc += 1; p18 += 1
                unnamed_now_p18.remove(fn_va)
    if rc == 0: break
print(f"  P18: {p18}")

# ── PASS 13: String Literal Class Matching ────────────────────────────────────
print("\n=== PASS 13: String literal class matching ===")
p13 = 0

# Extract rdata strings
rdata_strings = {}
for off in range(0, rvsize - 1):
    va = rvaddr + off
    raw = data[rraw + off: rraw + off + 200]
    try:
        null = raw.index(0)
    except ValueError:
        continue
    if null < 5: continue
    try:
        s = raw[:null].decode('ascii')
        if all(0x20 <= ord(c) <= 0x7E for c in s):
            rdata_strings[va] = s
    except:
        pass

# Build string → function references
str_to_fns = defaultdict(set)
fn_to_strs = defaultdict(set)
pending2 = {}
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op = (w >> 26) & 0x3F; rd = (w >> 21) & 0x1F
    ra = (w >> 16) & 0x1F; imm = w & 0xFFFF
    if op == 15 and ra == 0:
        hi = imm if imm < 0x8000 else imm - 0x10000
        pending2[rd] = hi << 16
    elif op == 14 and ra in pending2:
        lo = imm if imm < 0x8000 else imm - 0x10000
        addr = (pending2[ra] + lo) & 0xFFFFFFFF
        pending2[rd] = addr
        if addr in rdata_strings:
            fn = owning_fn(i)
            if fn: str_to_fns[addr].add(fn); fn_to_strs[fn].add(addr)
    elif op == 24 and ra in pending2:
        addr = (pending2[ra] | imm) & 0xFFFFFFFF
        pending2[rd] = addr
        if addr in rdata_strings:
            fn = owning_fn(i)
            if fn: str_to_fns[addr].add(fn); fn_to_strs[fn].add(addr)
    else:
        if rd in pending2 and op not in (15, 14, 24): pending2.pop(rd, None)

# Find exclusive strings (appear in exactly 1 class, from ≥2 named functions)
exclusive_strings = {}
for str_va, fns in str_to_fns.items():
    votes = Counter()
    for fn in fns:
        cls = get_class_clean(fn)
        if cls: votes[cls] += 1
    if len(votes) == 1:
        cls, cnt = votes.most_common(1)[0]
        if cnt >= 2:
            exclusive_strings[str_va] = cls

# Name fn_8 that reference exclusive strings
for fn_va in sorted(unnamed):
    if fn_va in new_names: continue
    for str_va in fn_to_strs.get(fn_va, set()):
        if str_va in exclusive_strings:
            cls = exclusive_strings[str_va]
            nm  = safe_name(f"{cls}_{fn_va&0xFFFF:04X}_str")
            if propose(fn_va, nm, f"string-literal class='{cls}' str@0x{str_va:08X}:'{rdata_strings[str_va][:40]}'", 'P13'):
                fn_class[fn_va] = cls; p13 += 1
            break

print(f"  P13: {p13} (from {len(exclusive_strings)} exclusive strings)")

# ── PASS 17: Spatial Proximity ────────────────────────────────────────────────
print("\n=== PASS 17: Spatial proximity (isolated fn_8 only) ===")
p17 = 0

for fn_va in sorted(unnamed):
    if fn_va in new_names: continue
    # Only truly isolated (no call/ptr edges)
    if all_edges(fn_va): continue

    idx = bisect.bisect_left(fn_addrs, fn_va)
    window = fn_addrs[max(0, idx-15):idx] + fn_addrs[idx+1:idx+16]

    vote = Counter()
    for nb in window:
        cls = get_class_clean(nb)
        if cls: vote[cls] += 1

    if not vote: continue
    dom, cnt = vote.most_common(1)[0]
    total = sum(vote.values()); ratio = cnt / total

    if cnt >= 6 and ratio >= 0.60:
        nm = safe_name(f"{dom}_{fn_va&0xFFFF:04X}_sp")
        if propose(fn_va, nm, f"spatial '{dom}' {cnt}/{total} in ±15 fns", 'P17'):
            fn_class[fn_va] = dom; p17 += 1

print(f"  P17: {p17}")

# ── Final Summary ──────────────────────────────────────────────────────────────
uf    = sum(1 for v in new_names if v in unnamed)
total = len(new_names)
pc    = Counter(pass_tag.values())
remaining = len(unnamed) - uf

n_start = len(unnamed)
n_total_fns = len(fn_addrs)
n_named_before = n_total_fns - n_start
n_named_after  = n_named_before + uf
pct_before = n_named_before / n_total_fns * 100
pct_after  = n_named_after  / n_total_fns * 100

print(f"\n{'='*70}")
print(f"auto_namer_v11 Results")
print(f"{'='*70}")
for tag in ['SEED', 'P10', 'P10r', 'P11', 'P4f', 'P4fw', 'P4rh',
            'P6', 'P6r', 'P12', 'P12fw', 'P12rh', 'P13', 'P14', 'P15', 'P17',
            'P18', 'P19', 'P19s', 'P20']:
    if pc.get(tag, 0):
        print(f"  {tag:<8s}: {pc.get(tag, 0):5d}")
print(f"  {'─'*36}")
print(f"  Total renames    : {total:5d}")
print(f"  fn_8 fixed       : {uf:5d}")
print(f"  Remaining fn_8   : {remaining:5d}")
print(f"  Coverage before  : {pct_before:.2f}% ({n_named_before}/{n_total_fns})")
print(f"  Coverage after   : {pct_after:.2f}% ({n_named_after}/{n_total_fns})")
print(f"  Coverage gain    : +{pct_after-pct_before:.2f}%")
print(f"{'='*70}")

# ── Write Output ───────────────────────────────────────────────────────────────
print(f"\nWriting output to {OUT}...")
out_lines = []
for line in sym_lines:
    s = line.strip(); matched = False
    for sect in ['.text', '.rdata', '.data']:
        if '= ' + sect + ':0x' in s:
            parts = s.split('=', 1)
            name  = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                if addr in new_names:
                    out_lines.append(line.replace(name, new_names[addr], 1))
                    matched = True; break
            except:
                pass
    if not matched:
        out_lines.append(line)
with open(OUT, 'w') as f:
    f.writelines(out_lines)

# ── Write Report ───────────────────────────────────────────────────────────────
with open(REPORT, 'w') as f:
    f.write(f"auto_namer_v12 Results\n{'='*70}\n\n")
    f.write(f"Input:  symbols_autonamed_v10.txt  ({pct_before:.2f}% coverage)\n")
    f.write(f"Output: symbols_autonamed_v11.txt  ({pct_after:.2f}% coverage)\n\n")
    f.write(f"New passes summary:\n")
    p20_eval = pc.get('P20', 0)
    # Number of renamed generics might just be stored in a global variable p20 if scope permits
    f.write(f"  P18 (V12 Reconvergence):        {pc.get('P18',0)} fn_8\n")
    f.write(f"  P19 (Atexit/Static Init):       {pc.get('P19',0)} semantic, {pc.get('P19s',0)} generic fn_8\n")
    f.write(f"  P20 (Generic cascade):          {p20_eval} items tagged P20 (generics + fn_8 cascade)\n")
    f.write(f"  P14 (RTTI vtable completor):    {pc.get('P14',0)} renames ({p14_fn8} were fn_8)\n")
    f.write(f"  P12 (extended BFS):             {pc.get('P12',0)+pc.get('P12fw',0)+pc.get('P12rh',0)} fn_8\n")
    f.write(f"  P15 (island BFS):               {pc.get('P15',0)} fn_8\n")
    f.write(f"  P13 (string literal):           {pc.get('P13',0)} fn_8\n")
    f.write(f"  P17 (spatial proximity):        {pc.get('P17',0)} fn_8\n")
    f.write(f"\nPass counts:\n")
    for tag in ['SEED', 'P10', 'P10r', 'P11', 'P4f', 'P4fw', 'P4rh',
                'P6', 'P6r', 'P12', 'P12fw', 'P12rh', 'P13', 'P14', 'P15', 'P17']:
        if pc.get(tag, 0):
            f.write(f"  {tag:<8s}: {pc.get(tag, 0):5d}\n")
    f.write(f"\nTotal renames:      {total}\n")
    f.write(f"fn_8 fixed:         {uf}\n")
    f.write(f"Remaining fn_8:     {remaining}\n")
    f.write(f"Coverage gain:      +{pct_after-pct_before:.2f}% ({pct_before:.2f}% -> {pct_after:.2f}%)\n\n")
    f.write(f"{'='*70}\nDETAIL\n{'='*70}\n")
    for addr in sorted(new_names):
        tag = pass_tag[addr]
        was_fn8 = '*' if addr in unnamed else (' G' if sym_map.get(addr, '').startswith('game_') else '  ')
        f.write(f"  [{tag:6s}]{was_fn8} 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"Done! Report: {REPORT}")
