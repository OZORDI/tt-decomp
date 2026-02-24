#!/usr/bin/env python3
"""
bridge_analyzer.py — Manual bridge function analysis for Approach 4
Analyzes top game_ bridge functions to identify them and generate manual seeds.
"""
import re, bisect, os
from collections import defaultdict, Counter

BASE = '/Users/Ozordi/Downloads/tt-decomp'
XEX  = f'{BASE}/orig/434C4803/pong_xenon_final.pe'
SYMS = f'{BASE}/config/434C4803/symbols_autonamed_v9.txt'
OUT  = f'{BASE}/naming/bridge_analysis.txt'

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
    s       = data[sect_tbl+i*40:sect_tbl+i*40+40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize   = int.from_bytes(s[8:12],'little')
    vaddr   = int.from_bytes(s[12:16],'little')
    raw_off = int.from_bytes(s[20:24],'little')
    sections.append((sname, image_base+vaddr, vsize, raw_off))

text_sec  = next(s for s in sections if s[0]=='.text')
rdata_sec = next(s for s in sections if s[0]=='.rdata')
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def va_to_file(va):
    for _, abs_va, vsize, raw_off in sections:
        if abs_va <= va < abs_va+vsize:
            return raw_off+(va-abs_va)
    return None

def read_str_at(va, maxlen=80):
    off = va_to_file(va)
    if off is None: return None
    end = min(off+maxlen, len(data))
    chunk = data[off:end]
    try:
        nul = chunk.index(0)
        s = chunk[:nul].decode('ascii','replace')
        if len(s) >= 3 and all(32 <= ord(c) < 127 for c in s):
            return s
    except ValueError:
        pass
    return None

# ── Symbol loading ────────────────────────────────────────────────────────────
print("Loading symbols...")
sym_map  = {}
fn_meta  = {}
with open(SYMS) as f:
    for line in f:
        s = line.strip()
        if '= .text:0x' in s:
            parts = s.split('=', 1)
            name  = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                sym_map[addr] = name
                m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                if m: fn_meta[addr] = int(m.group(1), 16)
            except:
                pass

fn_addrs   = sorted(a for a in fn_meta if tvaddr <= a < tvaddr+tvsize)
fn_offsets = [a-tvaddr for a in fn_addrs]
fn_set     = set(fn_addrs)
unnamed    = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
game_named = {a for a in fn_addrs if sym_map.get(a,'').startswith('game_')}
print(f"  fn_8: {len(unnamed)}, game_: {len(game_named)}, total: {len(fn_addrs)}")
print(f"  Coverage: {(len(fn_addrs)-len(unnamed))/len(fn_addrs)*100:.1f}%")

def owning_fn(off):
    idx = bisect.bisect_right(fn_offsets, off) - 1
    return (tvaddr+fn_offsets[idx]) if idx >= 0 else None

# ── Build Callgraph ───────────────────────────────────────────────────────────
print("Building callgraph...")
forward_cg = defaultdict(set)
reverse_cg = defaultdict(set)
for i in range(0, tvsize, 4):
    w = int.from_bytes(data[traw+i:traw+i+4],'big')
    if (w>>26)&0x3F == 18 and (w&1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va    = (tvaddr + i + li) & 0xFFFFFFFF
        if caller_va and tgt_va in fn_set and caller_va != tgt_va:
            forward_cg[caller_va].add(tgt_va)
            reverse_cg[tgt_va].add(caller_va)
print(f"  Call edges from {len(forward_cg)} functions")

# ── Scan rdata for strings ───────────────────────────────────────────────────
print("Scanning rdata for strings...")
rdata_strings = {}
chunk = data[rraw:rraw+rvsize]
i = 0
while i < len(chunk)-3:
    if 32 <= chunk[i] < 127:
        j = i
        while j < len(chunk) and 32 <= chunk[j] < 127:
            j += 1
        if j < len(chunk) and chunk[j] == 0 and (j-i) >= 4:
            s = chunk[i:j].decode('ascii','replace')
            rdata_strings[rvaddr+i] = s
        i = j+1
    else:
        i += 1
print(f"  {len(rdata_strings)} strings found")

# ── PPC Disassembler ──────────────────────────────────────────────────────────
def sign_ext16(v):
    return v if v < 0x8000 else v - 0x10000

def ppc_disasm_fn(fn_va):
    """
    Returns (r3_offsets, str_vas, bl_targets)
    r3_offsets: Counter of signed offsets accessed via r3
    str_vas: list of (va, string) for strings referenced
    bl_targets: set of VA of called functions
    """
    size = fn_meta.get(fn_va, 256)
    foff = va_to_file(fn_va)
    if foff is None:
        return Counter(), [], set()

    r_vals     = {}       # reg -> int value (None = unknown/loaded)
    r3_offsets = Counter()
    str_vas    = []
    seen_strs  = set()
    bl_targets = set()

    for i in range(0, min(size, 8192), 4):
        if foff+i+4 > len(data): break
        w    = int.from_bytes(data[foff+i:foff+i+4],'big')
        op   = (w>>26) & 0x3F
        rD   = (w>>21) & 0x1F   # destination / source
        rA   = (w>>16) & 0x1F
        imm  = w & 0xFFFF
        simm = sign_ext16(imm)

        # lis rD, UIMM  (addis rD, r0, UIMM)
        if op == 15 and rA == 0:
            hi = imm if imm < 0x8000 else imm - 0x10000
            r_vals[rD] = hi << 16

        # addi rD, rA, SIMM
        elif op == 14:
            if rA == 0:
                r_vals[rD] = simm
            elif rA in r_vals and r_vals[rA] is not None:
                r_vals[rD] = (r_vals[rA] + simm) & 0xFFFFFFFF
            else:
                r_vals.pop(rD, None)

        # ori rD, rS, UIMM  (but encoding: ori rA, rS, UIMM)
        elif op == 24:
            if rA in r_vals and r_vals[rA] is not None:
                r_vals[rD] = (r_vals[rA] | imm) & 0xFFFFFFFF
            else:
                r_vals.pop(rD, None)

        # mr rD, rA (or rD, rA, rA)
        elif op == 31 and (w & 0x7FF) == (444 << 1 | 0):
            if rA in r_vals:
                r_vals[rD] = r_vals[rA]
            else:
                r_vals.pop(rD, None)

        # lwz/lbz/lhz/lwzu — load from struct field
        elif op in (32, 33, 34, 40):
            if rA == 3:
                r3_offsets[simm] += 1
            # If loading from a known address, track result
            if rA in r_vals and r_vals[rA] is not None:
                load_va = (r_vals[rA] + simm) & 0xFFFFFFFF
                if op == 32:  # lwz — may load a pointer
                    pass
            r_vals[rD] = None  # result unknown after load

        # stw/stb/sth — store to struct field
        elif op in (36, 37, 38, 44):
            if rA == 3:
                r3_offsets[simm] += 1

        # bl
        elif op == 18 and (w & 1):
            li = w & 0x3FFFFFC
            if li & 0x2000000: li |= ~0x3FFFFFF
            tgt = (fn_va + i + li) & 0xFFFFFFFF
            if tvaddr <= tgt < tvaddr+tvsize:
                bl_targets.add(tgt)

        # After computing rD, check if any reg in r4..r10 is a rdata string ptr
        # (string args loaded into parameter registers)
        if rD in (3,4,5,6) and rD in r_vals and r_vals[rD] is not None:
            va_c = r_vals[rD] & 0xFFFFFFFF
            if va_c in rdata_strings and va_c not in seen_strs:
                str_vas.append((va_c, rdata_strings[va_c]))
                seen_strs.add(va_c)
            # Also check nearby rdata
            elif rvaddr <= va_c < rvaddr+rvsize:
                s = read_str_at(va_c)
                if s and va_c not in seen_strs:
                    str_vas.append((va_c, s))
                    seen_strs.add(va_c)

        # Invalidate rD on instructions that modify it in unknown ways
        elif op not in (15,14,24,31,32,33,34,40,36,37,38,44,18):
            r_vals.pop(rD, None)

    return r3_offsets, str_vas, bl_targets

# ── Helpers ───────────────────────────────────────────────────────────────────
GENERIC_PFX = frozenset({'game','ph','xe','ke','grc','aud','rage','jumptable',
    'ref','nt','util','thunk','vt','nop','const','get','nullvfn','fn','io',
    'stub','sp','2h','2hr','loc','pdata','BINK','set','return'})
STDLIB_BLACKLIST = {
    'memcpy','memset','malloc','free','realloc','calloc','printf','sprintf',
    'snprintf','vsnprintf','sscanf','strlen','strcpy','strcat','strcmp',
    'strncmp','strncpy','memmove','memcmp','rage_free','rage_malloc','rage_stricmp',
}

def get_class(name):
    if not name or name.startswith('fn_8'): return None
    if name in STDLIB_BLACKLIST: return None
    if name.startswith('__') or name.startswith('game_'): return None
    parts = name.split('_')
    if not parts or parts[0] in GENERIC_PFX: return None
    return parts[0]

# ── Find Top Bridge Functions ─────────────────────────────────────────────────
print("Finding bridge functions...")
bridges = []
for fn_va in game_named:
    neighbors = forward_cg.get(fn_va,set()) | reverse_cg.get(fn_va,set())
    cnt = sum(1 for n in neighbors if n in unnamed)
    if cnt > 0:
        bridges.append((cnt, fn_va))
bridges.sort(reverse=True)
print(f"  {len(bridges)} game_ functions with fn_8 neighbors")
print(f"  Top 5: {[(c, hex(v)) for c,v in bridges[:5]]}")

# ── Analyze Top 50 ────────────────────────────────────────────────────────────
print(f"Analyzing top {min(50,len(bridges))} bridge functions...")
results = []
for cnt, fn_va in bridges[:50]:
    r3_offs, str_vas, bl_tgts = ppc_disasm_fn(fn_va)
    
    # Caller analysis
    callers = list(reverse_cg.get(fn_va, set()))
    caller_classes = Counter()
    caller_examples = []
    for c in callers:
        nm = sym_map.get(c,'')
        cls = get_class(nm)
        if cls: caller_classes[cls] += 1
        if nm and not nm.startswith('fn_8') and nm not in caller_examples:
            caller_examples.append(nm)
    caller_examples = sorted(set(caller_examples))[:12]

    # Named callees
    named_bl = []
    for t in bl_tgts:
        nm = sym_map.get(t,'')
        if nm and not nm.startswith('fn_8') and not nm.startswith('game_'):
            named_bl.append(nm)

    # 2-hop class voting
    two_hop = Counter()
    for n in (forward_cg.get(fn_va,set()) | reverse_cg.get(fn_va,set())):
        for far in (forward_cg.get(n,set()) | reverse_cg.get(n,set())):
            if far == fn_va: continue
            cls = get_class(sym_map.get(far,''))
            if cls: two_hop[cls] += 1

    results.append({
        'va':            fn_va,
        'name':          sym_map.get(fn_va,''),
        'fn8_count':     cnt,
        'size':          fn_meta.get(fn_va,0),
        'caller_cnt':    len(callers),
        'callee_cnt':    len(forward_cg.get(fn_va,set())),
        'r3_offs':       dict(r3_offs.most_common(12)),
        'strings':       str_vas[:6],
        'named_bl':      named_bl[:8],
        'caller_cls':    dict(caller_classes.most_common(6)),
        'caller_ex':     caller_examples,
        'two_hop':       dict(two_hop.most_common(6)),
        'dom_1hop':      caller_classes.most_common(1)[0] if caller_classes else None,
    })

# ── Build Report ──────────────────────────────────────────────────────────────
os.makedirs(f'{BASE}/naming', exist_ok=True)
L = []

L.append("=" * 80)
L.append("BRIDGE FUNCTION ANALYSIS REPORT — Approach 4")
L.append(f"v9 state: 84.2% coverage | {len(unnamed)} fn_8 remaining | {len(game_named)} game_ remaining")
L.append("=" * 80)
L.append("")

# Summary table
L.append(f"{'#':>3} {'Address':>12}  {'Name':<25} {'fn8':>4} {'Size':>6} {'Callers':>7}  DominantClass(votes)")
L.append("-" * 80)
for i, r in enumerate(results):
    dom = r['dom_1hop'][0] if r['dom_1hop'] else (list(r['two_hop'].keys())[0] if r['two_hop'] else '?')
    sc  = r['dom_1hop'][1] if r['dom_1hop'] else 0
    L.append(f"{i+1:>3} 0x{r['va']:08X}  {r['name']:<25} {r['fn8_count']:>4} {r['size']:>6}B {r['caller_cnt']:>7}  {dom}({sc})")
L.append("")

# Python-verified math
top10 = sum(r['fn8_count'] for r in results[:10])
top20 = sum(r['fn8_count'] for r in results[:20])
top50 = sum(r['fn8_count'] for r in results[:50])
n = len(unnamed)
L.append(f"MATH (Python-verified):")
L.append(f"  Direct fn_8 unlock — top 10: {top10} ({top10/n*100:.2f}%), top 20: {top20} ({top20/n*100:.2f}%), top 50: {top50} ({top50/n*100:.2f}%)")
L.append(f"  Cascade estimate (2x-3x multiplier): top 10 → {top10*2}-{top10*3} fn_8 total")
L.append(f"  Expected coverage after naming top 10+cascade: {84.2 + top10*2/16998*100:.1f}% to {84.2 + top10*3/16998*100:.1f}%")
L.append(f"  Expected coverage after naming top 50+cascade: {84.2 + top50*2/16998*100:.1f}% to {84.2 + top50*3/16998*100:.1f}%")
L.append("")

# Detailed analysis
L.append("=" * 80)
L.append("DETAILED ANALYSIS PER FUNCTION")
L.append("=" * 80)

for i, r in enumerate(results):
    L.append("")
    L.append(f"━━ #{i+1:02d} 0x{r['va']:08X} — {r['name']} {'━'*(55-len(r['name']))}")
    L.append(f"   Size: {r['size']}B  |  fn_8 neighbors: {r['fn8_count']}  |  callers: {r['caller_cnt']}  |  callees: {r['callee_cnt']}")
    
    if r['caller_cls']:
        cc = ', '.join(f"{c}({n})" for c,n in sorted(r['caller_cls'].items(), key=lambda x:-x[1]))
        L.append(f"   Caller classes: {cc}")
    
    if r['caller_ex']:
        L.append(f"   Named callers:  {', '.join(r['caller_ex'][:8])}")
    
    if r['named_bl']:
        L.append(f"   Calls (named):  {', '.join(r['named_bl'])}")
    
    if r['r3_offs']:
        offs = sorted(r['r3_offs'].keys())
        L.append(f"   r3 offsets:     {', '.join('['+str(o)+']' for o in offs[:10])}")
    
    if r['strings']:
        for va, s in r['strings'][:3]:
            L.append(f"   String:         \"{s}\"  (0x{va:08X})")
    
    if r['two_hop']:
        th = ', '.join(f"{c}({n})" for c,n in sorted(r['two_hop'].items(), key=lambda x:-x[1])[:4])
        L.append(f"   2-hop classes:  {th}")
    
    # Inference
    dom_cls   = r['dom_1hop'][0] if r['dom_1hop'] else None
    dom_score = r['dom_1hop'][1] if r['dom_1hop'] else 0
    dom_total = sum(r['caller_cls'].values())
    
    if dom_cls:
        pct = dom_score/dom_total*100 if dom_total else 0
        L.append(f"   ★ Dominant class: {dom_cls} ({pct:.0f}% of {dom_total} named callers, score={dom_score})")
    elif r['two_hop']:
        dom_2h = list(r['two_hop'].items())[0]
        L.append(f"   ★ 2-hop class: {dom_2h[0]} (score={dom_2h[1]})")
    
    # Function character
    chars = []
    if r['size'] <= 8:    chars.append("TINY (<=8B): pure getter/trampoline")
    elif r['size'] <= 32: chars.append("SMALL: simple accessor/helper")
    elif r['size'] >= 3000: chars.append("LARGE: complex method or init")
    if r['callee_cnt'] == 0 and r['caller_cnt'] >= 10:
        chars.append("LEAF with many callers: utility/calculation")
    if r['r3_offs'] and max(r['r3_offs'].keys(), default=0) <= 16:
        chars.append(f"Accesses low r3 offsets: likely vtable/header fields")
    if r['strings']:
        chars.append(f"References strings → likely has debug/assert output")
    for ch in chars:
        L.append(f"   → {ch}")
    
    # Suggested name
    addr_hex = f"{r['va']&0xFFFF:04X}"
    if dom_cls:
        if r['size'] <= 8:
            suggested = f"{dom_cls}_{addr_hex}_get"
            reason = f"TINY fn, dominant caller class {dom_cls} ({dom_score} votes)"
        elif r['callee_cnt'] == 0:
            suggested = f"{dom_cls}_{addr_hex}_leaf"
            reason = f"Leaf fn, {dom_score}/{dom_total} callers are {dom_cls}"
        elif r['strings']:
            s_hint = re.sub(r'[^A-Za-z0-9]','_',r['strings'][0][1])[:20].strip('_')
            suggested = f"{dom_cls}_{s_hint}_{addr_hex}"
            reason = f"String hint + class {dom_cls}"
        else:
            suggested = f"{dom_cls}_{addr_hex}"
            reason = f"Class {dom_cls} dominant ({dom_score}/{dom_total} callers)"
        L.append(f"   SUGGESTED NAME: {suggested}")
        L.append(f"   REASON:         {reason}")
    else:
        L.append(f"   SUGGESTED NAME: (insufficient signal — manual analysis needed)")

# Seeds output for v10
L.append("")
L.append("=" * 80)
L.append("MANUAL_SEEDS FOR auto_namer_v10.py (high-confidence only)")
L.append("=" * 80)
L.append("")
L.append("MANUAL_SEEDS = {")
seed_count = 0
for r in results:
    dom_cls   = r['dom_1hop'][0] if r['dom_1hop'] else None
    dom_score = r['dom_1hop'][1] if r['dom_1hop'] else 0
    dom_total = sum(r['caller_cls'].values())
    pct = dom_score/dom_total*100 if dom_total else 0
    
    # Only emit seeds with strong signal
    if not dom_cls: continue
    if dom_score < 3 and pct < 60: continue
    
    addr_hex = f"{r['va']&0xFFFF:04X}"
    if r['size'] <= 8:
        nm = f"{dom_cls}_{addr_hex}_get"
    elif r['callee_cnt'] == 0 and r['caller_cnt'] >= 6:
        nm = f"{dom_cls}_{addr_hex}_leaf"
    else:
        nm = f"{dom_cls}_{addr_hex}"
    
    callers_str = ', '.join(r['caller_ex'][:3])
    reason = f"manual: {dom_score}/{dom_total} callers={dom_cls} ({pct:.0f}%) | callers: {callers_str}"
    L.append(f"    0x{r['va']:08X}: ('{nm}', '{reason}'),")
    seed_count += 1

L.append("}")
L.append(f"# {seed_count} high-confidence seeds generated")

with open(OUT,'w') as f:
    f.write('\n'.join(L))

print(f"\nDone! Report written to {OUT}")
print(f"\nTop 10 bridges in current state (v9):")
for i, r in enumerate(results[:10]):
    dom = r['dom_1hop'][0] if r['dom_1hop'] else '?'
    print(f"  {i+1:2d}. 0x{r['va']:08X} {r['name']:<26} fn8={r['fn8_count']} size={r['size']}B callers={r['caller_cnt']} class={dom}")

print(f"\nMath summary (Python-verified):")
print(f"  Top 10 direct unlock: {top10} fn_8 ({top10/n*100:.2f}%)")
print(f"  Top 50 direct unlock: {top50} fn_8 ({top50/n*100:.2f}%)")
print(f"  Expected coverage (top 10 + cascade 2x): {84.2 + top10*2/16998*100:.2f}%")
