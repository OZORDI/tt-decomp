#!/usr/bin/env python3
"""
auto_namer.py  --  Bulk function naming for tt-decomp
=======================================================
Runs 4 passes over the binary + symbols.txt and writes a patched
symbols.txt with as many fn_8XXXXXXX entries renamed as possible.

Pass 1 – String-reference naming
  The XEX contains embedded assert/log strings like
  "pongCreature::StartEmote - ...".  The function that loads such a
  string's address almost certainly IS pongCreature::StartEmote.
  We scan every fn_8 for any 'lis/addi' pair that resolves to a
  .rdata address whose content is "ClassName::MethodName...".

Pass 2 – Thunk / relay naming
  Functions whose entire body is a single 'b target' or
  'lis r11,X / lwz r11,Y(r11) / mtctr r11 / bctr' (2-5 insns)
  are named  thunk_<target>.

Pass 3 – Call-pattern / module prefix
  Functions that only call into already-named functions get a module
  prefix derived from the most common prefix of their callees.

Pass 4 – Pure-virtual / nop stubs
  Functions that are a single blr/nop/trap are named
  nop_XXXX / pure_XXXX.

Output: config/434C4803/symbols_autonamed.txt  (a copy with additions)
        auto_namer_report.txt                   (stats + what changed)

Usage: python3 auto_namer.py
"""

import re, sys, os, struct, collections

# ── paths ────────────────────────────────────────────────────────────────────
BASE   = os.path.dirname(os.path.abspath(__file__))
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols.txt')
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed.txt')
REPORT = os.path.join(BASE, 'auto_namer_report.txt')

# ── load binary ──────────────────────────────────────────────────────────────
print("Loading binary...")
data = open(XEX, 'rb').read()

e_lfanew     = int.from_bytes(data[0x3C:0x40], 'little')
coff_off     = e_lfanew + 4
opt_off      = coff_off + 20
image_base   = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
num_sects    = int.from_bytes(data[coff_off+2:coff_off+4],  'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18],'little')
sect_tbl     = opt_off + opt_hdr_size

sections = []
for i in range(num_sects):
    s       = data[sect_tbl + i*40 : sect_tbl + i*40 + 40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii', errors='replace').rstrip('\x00')
    vsize   = int.from_bytes(s[8:12],  'little')
    vaddr   = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((sname, vaddr, vsize, raw_off))
    print(f"  Section {sname!r:12s}  VA=0x{image_base+vaddr:08X}  size=0x{vsize:X}")

def va_to_file(va):
    rel = va - image_base
    for _, vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

def rbe(va):
    off = va_to_file(va)
    if off is None or off + 4 > len(data): return None
    return int.from_bytes(data[off:off+4], 'big')

def read_str(va):
    """Read null-terminated ASCII string at VA."""
    off = va_to_file(va)
    if off is None: return None
    end = data.find(b'\x00', off)
    if end == -1 or end - off > 512: return None
    try:
        return data[off:end].decode('ascii')
    except:
        return None

# ── load symbols ──────────────────────────────────────────────────────────────
print("\nLoading symbols...")
sym_map   = {}   # addr -> name
sym_by_name = {} # name -> addr
sym_lines   = [] # raw lines
fn_meta     = {} # addr -> {size, section_hint}

with open(SYMS) as f:
    for line in f:
        sym_lines.append(line)
        s = line.strip()
        for sect in ['.text', '.rdata', '.data']:
            marker = '= ' + sect + ':0x'
            if marker in s:
                parts = s.split('=', 1)
                name  = parts[0].strip()
                try:
                    rest = parts[1].strip()
                    addr_s = rest.split(';')[0].strip().split(':')[1]
                    addr   = int(addr_s, 16)
                    sym_map[addr]       = name
                    sym_by_name[name]   = addr
                    m = re.search(r'size:0x([0-9A-Fa-f]+)', rest)
                    if m: fn_meta[addr] = int(m.group(1), 16)
                except: pass

# get all functions sorted by address
all_fns = sorted(
    [(a, n) for a, n in sym_map.items() if 'type:function' not in sym_lines[0]],
    key=lambda x: x[0]
)
# actually rebuild from meta (fn_meta has the ones with size)
fn_addrs = sorted(fn_meta.keys())

# which ones are unnamed?
unnamed = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
print(f"  Total symbols: {len(sym_map)}")
print(f"  Functions with size: {len(fn_meta)}")
print(f"  Unnamed (fn_8...): {len(unnamed)}")

# ── helpers ──────────────────────────────────────────────────────────────────
def disasm_fn(start_va, max_insns=200):
    """Returns list of (va, word) pairs."""
    insns = []
    va    = start_va
    sz    = fn_meta.get(start_va, max_insns * 4)
    limit = min(max_insns, sz // 4)
    for _ in range(limit):
        w = rbe(va)
        if w is None: break
        insns.append((va, w))
        va += 4
        op = (w >> 26) & 0x3F
        # stop at blr / unconditional branch / bctr
        if w == 0x4E800020: break           # blr
        if w in (0x4E800420, 0x4E800421): break  # bctr/bctrl
        if op == 18 and not (w & 1): break  # unconditional b (not bl)
    return insns

def decode_branch_target(va, w):
    op = (w >> 26) & 0x3F
    if op == 18:
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        aa = (w >> 1) & 1
        return li if aa else va + li
    return None

def safe_name(s):
    """Make a string safe to use as a symbol name."""
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    s = re.sub(r'_+', '_', s).strip('_')
    return s[:80]

# ── collect all .rdata strings with their VA ─────────────────────────────────
print("\nIndexing .rdata strings...")
rdata_strings = {}  # va -> string
for sname, vaddr, vsize, raw_off in sections:
    if sname != '.rdata': continue
    i = 0
    while i < vsize:
        # look for printable run
        start = i
        while i < vsize and 32 <= data[raw_off+i] < 127:
            i += 1
        length = i - start
        if length >= 8:
            try:
                s = data[raw_off+start : raw_off+i].decode('ascii')
                va = image_base + vaddr + start
                rdata_strings[va] = s
            except: pass
        # skip non-printable
        while i < vsize and not (32 <= data[raw_off+i] < 127):
            i += 1

print(f"  Found {len(rdata_strings)} strings in .rdata")

# Build a lookup: va -> string for strings that look like "Class::Method"
class_method_strings = {}  # va -> (class_name, method_name, full_string)
for va, s in rdata_strings.items():
    m = re.match(r'^([A-Za-z][A-Za-z0-9_]{2,40})::([A-Za-z][A-Za-z0-9_]{1,60})', s)
    if m:
        class_method_strings[va] = (m.group(1), m.group(2), s)

print(f"  Of which Class::Method style: {len(class_method_strings)}")

# ── PASS 1: String-reference naming ─────────────────────────────────────────
print("\n=== PASS 1: String-reference naming ===")
new_names  = {}   # addr -> proposed_name
confidence = {}   # addr -> confidence string

def resolve_lis_addi(insns, idx):
    """
    At insns[idx] we have a 'lis rD, hi'.
    Look for addi rD, rD, lo nearby to form a full 32-bit address.
    Returns resolved VA or None.
    """
    _, w0 = insns[idx]
    op0   = (w0 >> 26) & 0x3F
    if op0 != 15: return None           # must be lis (addis with ra=0)
    rd    = (w0 >> 21) & 0x1F
    hi    = w0 & 0xFFFF
    # scan forward up to 8 insns for addi rd, rd, lo
    for j in range(idx+1, min(idx+12, len(insns))):
        _, w1 = insns[j]
        op1   = (w1 >> 26) & 0x3F
        r_d1  = (w1 >> 21) & 0x1F
        r_a1  = (w1 >> 16) & 0x1F
        if op1 == 14 and r_d1 == rd and r_a1 == rd:
            lo  = w1 & 0xFFFF
            if lo & 0x8000: lo -= 0x10000
            return (hi << 16) + lo
        # also handle ori rD, rD, lo
        if op1 == 24 and r_d1 == rd and r_a1 == rd:
            lo = w1 & 0xFFFF
            return (hi << 16) | lo
    return None

named_by_pass1 = 0
for fn_addr in unnamed:
    insns = disasm_fn(fn_addr)
    if not insns: continue
    hits = []
    for idx, (va, w) in enumerate(insns):
        op = (w >> 26) & 0x3F
        if op == 15:  # lis
            resolved = resolve_lis_addi(insns, idx)
            if resolved and resolved in class_method_strings:
                hits.append(class_method_strings[resolved])
    if not hits: continue
    # pick the most specific hit (longest full_string usually = most specific)
    hits.sort(key=lambda x: -len(x[2]))
    cls, meth, full = hits[0]
    proposed = f"{cls}_{meth}"
    proposed = safe_name(proposed)
    if proposed and proposed not in sym_by_name:
        new_names[fn_addr]  = proposed
        confidence[fn_addr] = f"string: {full[:60]!r}"
        named_by_pass1 += 1

print(f"  Named {named_by_pass1} functions from string references")

# ── PASS 2: Thunk / relay detection ─────────────────────────────────────────
print("\n=== PASS 2: Thunk detection ===")
named_by_pass2 = 0
for fn_addr in unnamed:
    if fn_addr in new_names: continue
    insns = disasm_fn(fn_addr)
    if not insns: continue
    # Pattern A: single unconditional branch  b target
    if len(insns) == 1:
        va, w = insns[0]
        op = (w >> 26) & 0x3F
        if op == 18 and not (w & 1):
            tgt = decode_branch_target(va, w)
            if tgt and tgt in sym_map:
                proposed = 'thunk_' + sym_map[tgt]
                if proposed not in sym_by_name:
                    new_names[fn_addr]  = proposed
                    confidence[fn_addr] = f"thunk -> {sym_map[tgt]}"
                    named_by_pass2 += 1
                    continue
    # Pattern B: 4-insn vtable thunk: lis/lwz/mtctr/bctr
    if 2 <= len(insns) <= 6:
        words = [w for _, w in insns]
        ops   = [(w >> 26) & 0x3F for w in words]
        # ends with bctr?
        if words[-1] in (0x4E800420, 0x4E800421):
            # is there a bl somewhere? if so it's more than a thunk
            has_bl = any((w >> 26) & 0x3F == 18 and (w & 1) for w in words[:-1])
            if not has_bl:
                proposed = f'vtthunk_{fn_addr:08X}'
                if proposed not in sym_by_name:
                    new_names[fn_addr]  = proposed
                    confidence[fn_addr] = f"vtable thunk ({len(insns)} insns, ends bctr)"
                    named_by_pass2 += 1

print(f"  Named {named_by_pass2} functions as thunks")

# ── PASS 3: Call-pattern / module prefix ─────────────────────────────────────
print("\n=== PASS 3: Call-pattern / module prefix ===")
# Build prefix frequency map for already-named functions
named_by_pass3 = 0

def get_prefix(name):
    """Extract module prefix: first token before _ or capital letter boundary."""
    # e.g. pongCreature_StartEmote -> pong or pongCreature
    # e.g. grcDevice_init -> grc
    # Try known prefixes first
    known = ['pong','grc','aud','rage','ph','ke','xam','nt','pg','gm','cr',
             'gd','fx','fi','sys','net','io','game','gfx','pcr']
    for p in known:
        if name.lower().startswith(p):
            return p
    m = re.match(r'^([a-z]{2,6})', name)
    return m.group(1) if m else None

for fn_addr in unnamed:
    if fn_addr in new_names: continue
    insns = disasm_fn(fn_addr)
    if not insns: continue
    callee_prefixes = collections.Counter()
    for va, w in insns:
        op = (w >> 26) & 0x3F
        if op == 18 and (w & 1):  # bl
            tgt = decode_branch_target(va, w)
            if tgt and tgt in sym_map:
                callee_name = sym_map[tgt]
                if not callee_name.startswith('fn_8'):
                    p = get_prefix(callee_name)
                    if p: callee_prefixes[p] += 1
    if not callee_prefixes: continue
    dominant, count = callee_prefixes.most_common(1)[0]
    if count >= 2:  # need at least 2 calls to same module
        proposed = f"{dominant}_{fn_addr & 0xFFFF:04X}"
        if proposed not in sym_by_name:
            new_names[fn_addr]  = proposed
            confidence[fn_addr] = f"calls {count}x into '{dominant}' module"
            named_by_pass3 += 1

print(f"  Named {named_by_pass3} functions by call pattern")

# ── PASS 4: Pure virtual / NOP stubs ─────────────────────────────────────────
print("\n=== PASS 4: NOP / pure-virtual stubs ===")
named_by_pass4 = 0
NOP_WORD  = 0x60000000
TRAP_WORD = 0x7FE00008
BLR_WORD  = 0x4E800020

for fn_addr in unnamed:
    if fn_addr in new_names: continue
    sz = fn_meta.get(fn_addr, 0)
    if sz > 16: continue  # only tiny functions
    insns = disasm_fn(fn_addr)
    if not insns: continue
    words = [w for _, w in insns]
    if words == [BLR_WORD] or all(w == NOP_WORD for w in words) + (words[-1] == BLR_WORD):
        proposed = f"nop_{fn_addr & 0xFFFF:04X}"
        if proposed not in sym_by_name:
            new_names[fn_addr]  = proposed
            confidence[fn_addr] = "stub/nop body"
            named_by_pass4 += 1
    elif TRAP_WORD in words:
        proposed = f"pure_{fn_addr & 0xFFFF:04X}"
        if proposed not in sym_by_name:
            new_names[fn_addr]  = proposed
            confidence[fn_addr] = "trap = pure virtual"
            named_by_pass4 += 1

print(f"  Named {named_by_pass4} functions as stubs")

# ── write output symbols file ─────────────────────────────────────────────────
print(f"\nWriting {OUT} ...")
total_new = len(new_names)

with open(SYMS) as f:
    lines = f.readlines()

out_lines = []
for line in lines:
    s = line.strip()
    matched = False
    for sect in ['.text', '.rdata', '.data']:
        marker = '= ' + sect + ':0x'
        if marker in s:
            parts = s.split('=', 1)
            name  = parts[0].strip()
            try:
                rest = parts[1].strip()
                addr_s = rest.split(';')[0].strip().split(':')[1]
                addr   = int(addr_s, 16)
                if addr in new_names and name.startswith('fn_8'):
                    new = new_names[addr]
                    new_line = line.replace(name, new, 1)
                    out_lines.append(new_line)
                    matched = True
                    break
            except: pass
    if not matched:
        out_lines.append(line)

with open(OUT, 'w') as f:
    f.writelines(out_lines)

# ── write report ──────────────────────────────────────────────────────────────
print(f"Writing {REPORT} ...")
with open(REPORT, 'w') as f:
    f.write("auto_namer.py  –  Results\n")
    f.write("=" * 60 + "\n\n")
    f.write(f"Pass 1  (string refs):    {named_by_pass1:5d}\n")
    f.write(f"Pass 2  (thunks):         {named_by_pass2:5d}\n")
    f.write(f"Pass 3  (call patterns):  {named_by_pass3:5d}\n")
    f.write(f"Pass 4  (stubs/nops):     {named_by_pass4:5d}\n")
    f.write(f"{'─'*30}\n")
    f.write(f"Total named this run:     {total_new:5d}\n")
    f.write(f"Previously unnamed:       {len(unnamed):5d}\n")
    f.write(f"Coverage:                 {total_new/max(len(unnamed),1)*100:.1f}%\n\n")
    f.write("=" * 60 + "\n")
    f.write("DETAIL (sorted by address)\n")
    f.write("=" * 60 + "\n")
    for addr in sorted(new_names):
        f.write(f"  0x{addr:08X}  {new_names[addr]:<50s}  [{confidence[addr]}]\n")

print(f"\n{'='*60}")
print(f"  Pass 1  (string refs):    {named_by_pass1}")
print(f"  Pass 2  (thunks):         {named_by_pass2}")
print(f"  Pass 3  (call patterns):  {named_by_pass3}")
print(f"  Pass 4  (stubs/nops):     {named_by_pass4}")
print(f"  ──────────────────────────────")
print(f"  TOTAL:                    {total_new}")
print(f"  Previously unnamed:       {len(unnamed)}")
print(f"  Coverage:                 {total_new/max(len(unnamed),1)*100:.1f}%")
print(f"\nOutput:  {OUT}")
print(f"Report:  {REPORT}")
print(f"{'='*60}")
print("\nReview auto_namer_report.txt, then copy symbols_autonamed.txt")
print("over symbols.txt when you're happy with the results.")
