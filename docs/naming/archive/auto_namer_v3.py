#!/usr/bin/env python3
"""
auto_namer_v3.py
================
Improvements over v2:
  Task 2 - Massively expanded KNOWN_CLASSES list (200+ classes)
  Task 3 - .pdata parsing: cross-reference function boundaries, report gaps
  Task 4 - mtctr/bctr indirect call detection added to call graph
  Task 5 - Tiered Pass 3 thresholds (high/medium confidence + _wrap suffix)
  Task 6 - Vtable allows up to 2 NULL slots, minimum run length 3

Output:
  config/434C4803/symbols_autonamed.txt
  auto_namer_v3_report.txt
  auto_namer_v3_pdata_report.txt
"""

import re, sys, os, struct, bisect
from collections import defaultdict, Counter

BASE   = os.path.dirname(os.path.abspath(__file__))
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols.txt')
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed.txt')
REPORT = os.path.join(BASE, 'auto_namer_v3_report.txt')
PDATA_REPORT = os.path.join(BASE, 'auto_namer_v3_pdata_report.txt')

# load binary
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
    s       = data[sect_tbl+i*40 : sect_tbl+i*40+40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii','replace')
    vsize   = int.from_bytes(s[8:12],  'little')
    vaddr   = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((sname, vaddr, vsize, raw_off))
    print(f"  {sname:12s}  VA=0x{image_base+vaddr:08X}  size=0x{vsize:X}")

text_sec  = next(s for s in sections if s[0]=='.text')
rdata_sec = next(s for s in sections if s[0]=='.rdata')
pdata_sec = next((s for s in sections if s[0]=='.pdata'), None)
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def va_to_file(va):
    rel = va - image_base
    for _, vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

# load symbols
print("\nLoading symbols...")
sym_map   = {}
sym_lines = []
fn_meta   = {}

with open(SYMS) as f:
    for line in f:
        sym_lines.append(line)
        s = line.strip()
        for sect in ['.text', '.rdata', '.data']:
            if '= '+sect+':0x' in s:
                parts = s.split('=', 1)
                name  = parts[0].strip()
                try:
                    addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                    sym_map[addr] = name
                    m = re.search(r'size:0x([0-9A-Fa-f]+)', s)
                    if m: fn_meta[addr] = int(m.group(1), 16)
                except: pass

fn_addrs   = sorted(a for a in fn_meta if 0x820C0000 <= a <= 0x82580000)
fn_offsets = sorted((a-(image_base+tvaddr)) for a in fn_addrs
                     if 0 <= (a-(image_base+tvaddr)) < tvsize)
unnamed    = {a for a in fn_addrs if sym_map.get(a,'').startswith('fn_8')}
print(f"  Total functions:   {len(fn_addrs)}")
print(f"  Unnamed (fn_8...): {len(unnamed)}")

def owning_fn(text_off):
    idx = bisect.bisect_right(fn_offsets, text_off) - 1
    return (image_base+tvaddr+fn_offsets[idx]) if idx >= 0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

new_names  = {}
confidence = {}
pass_tag   = {}
used_names = set(sym_map.values())

def propose(fn_va, name, reason, tag):
    if fn_va in new_names: return False
    if name in used_names:
        name = f"{name}_{fn_va & 0xFFFF:04X}"
    if name in used_names: return False
    new_names[fn_va]  = name
    confidence[fn_va] = reason
    pass_tag[fn_va]   = tag
    used_names.add(name)
    return True

# TASK 3: .pdata analysis
print("\n=== TASK 3: .pdata analysis ===")
if pdata_sec:
    _, pdata_vaddr, pdata_vsize, pdata_raw = pdata_sec
    pdata_abs_va = image_base + pdata_vaddr
    num_entries  = pdata_vsize // 12
    print(f"  .pdata at file 0x{pdata_raw:X}, VA 0x{pdata_abs_va:X}, {num_entries} entries")
    pdata_fns = {}
    for i in range(num_entries):
        off = pdata_raw + i * 12
        begin_va, end_va, unwind_va = struct.unpack_from('>III', data, off)
        if 0x820C0000 <= begin_va < 0x82580000:
            pdata_fns[begin_va] = end_va
    sym_set  = set(sym_map.keys())
    in_syms  = {va for va in pdata_fns if va in sym_set}
    not_in   = {va for va in pdata_fns if va not in sym_set}
    size_mismatch = []
    for va, end_va in pdata_fns.items():
        pdata_size = end_va - va
        sym_size   = fn_meta.get(va)
        if sym_size is not None and sym_size != pdata_size:
            size_mismatch.append((va, sym_size, pdata_size))
    print(f"  .pdata entries in range:   {len(pdata_fns)}")
    print(f"  Matched in symbols.txt:    {len(in_syms)}")
    print(f"  NOT in symbols.txt:        {len(not_in)}  <- hidden functions")
    print(f"  Size mismatches:           {len(size_mismatch)}")
    with open(PDATA_REPORT, 'w') as f:
        f.write("=== .pdata Cross-Reference Report ===\n\n")
        f.write(f"Total .pdata entries in range:  {len(pdata_fns)}\n")
        f.write(f"Matched in symbols.txt:         {len(in_syms)}\n")
        f.write(f"NOT in symbols.txt (hidden):    {len(not_in)}\n")
        f.write(f"Size mismatches:                {len(size_mismatch)}\n\n")
        f.write("--- HIDDEN FUNCTIONS (in .pdata, not in symbols.txt) ---\n")
        for va in sorted(not_in):
            sz = pdata_fns[va] - va
            f.write(f"  0x{va:08X}  size=0x{sz:X}\n")
        f.write("\n--- SIZE MISMATCHES ---\n")
        for va, sym_sz, pdata_sz in sorted(size_mismatch):
            f.write(f"  0x{va:08X}  sym_size=0x{sym_sz:X}  pdata_size=0x{pdata_sz:X}  delta={pdata_sz-sym_sz:+d}\n")
    print(f"  Written: {PDATA_REPORT}")
else:
    print("  WARNING: no .pdata section found")
    pdata_fns = {}

# Build call graph: bl + mtctr/bctr (Tasks 1+4)
print("\nBuilding call graph (bl + mtctr/bctr indirect)...")
callgraph = defaultdict(set)
bl_edges = 0
indirect_edges = 0

# Direct bl
for i in range(0, tvsize, 4):
    w  = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op = (w >> 26) & 0x3F
    if op == 18 and (w & 1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va    = (image_base+tvaddr+i) + li
        if caller_va and tgt_va in sym_map:
            callgraph[caller_va].add(tgt_va)
            bl_edges += 1

# Task 4: lis/addi/mtctr/bctr indirect calls
reg_hi  = {}   # reg -> (hi_val, lis_offset)
reg_va  = {}   # reg -> resolved_va

for i in range(0, tvsize, 4):
    w   = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op  = (w >> 26) & 0x3F
    rd  = (w >> 21) & 0x1F
    ra  = (w >> 16) & 0x1F
    imm = w & 0xFFFF

    if op == 15 and ra == 0:   # lis rD, hi
        reg_hi[rd] = (imm, i)
        reg_va.pop(rd, None)

    elif op == 14:              # addi rD, rA, lo
        if ra in reg_hi:
            hi_val, _ = reg_hi[ra]
            lo_s = imm if imm < 0x8000 else imm - 0x10000
            resolved = ((hi_val << 16) + lo_s) & 0xFFFFFFFF
            reg_va[rd] = resolved

    elif op == 31 and ((w >> 1) & 0x3FF) == 467:  # mtctr rS
        rs = rd  # bits 21:26
        if rs in reg_va:
            resolved = reg_va[rs]
            if i + 4 < tvsize:
                nw = int.from_bytes(data[traw+i+4:traw+i+8], 'big')
                if nw == 0x4E800420:   # bctr
                    caller_va = owning_fn(i)
                    if caller_va and resolved in sym_map:
                        callgraph[caller_va].add(resolved)
                        indirect_edges += 1

    # Flush stale reg state (older than 32 instrs)
    if i % 128 == 0:
        reg_hi = {k: v for k, v in reg_hi.items() if v[1] >= i - 128}
        live = set(reg_hi.keys())
        reg_va = {k: v for k, v in reg_va.items() if k in live}

print(f"  Direct bl edges:      {bl_edges}")
print(f"  Indirect mtctr edges: {indirect_edges}")
print(f"  Total call edges:     {sum(len(v) for v in callgraph.values())}")

# PASS 1: string-reference naming
print("\n=== PASS 1: String-reference naming ===")
string_pattern = re.compile(rb'[A-Za-z][A-Za-z0-9_]{2,40}::[A-Za-z][A-Za-z0-9_]{1,60}')
all_strings = {}
chunk = data[rraw:rraw+rvsize]
for m in string_pattern.finditer(chunk):
    if m.start() > 0 and 32 <= chunk[m.start()-1] < 127:
        continue
    va   = image_base + rvaddr + m.start()
    text = m.group().decode('ascii', 'replace')
    cls, rest = text.split('::', 1)
    meth = re.split(r'[^A-Za-z0-9_]', rest)[0]
    if len(cls) >= 3 and len(meth) >= 2:
        all_strings[va] = (cls, meth, text)

print(f"  Class::Method strings in .rdata: {len(all_strings)}")

lis_index = defaultdict(list)
for i in range(0, tvsize, 4):
    w   = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op  = (w >> 26) & 0x3F
    rd  = (w >> 21) & 0x1F
    ra  = (w >> 16) & 0x1F
    imm = w & 0xFFFF
    if op == 15 and ra == 0:
        lis_index[imm].append((i, rd))

fn_class_map = {}

for str_va, (cls, meth, full) in all_strings.items():
    lo = str_va & 0xFFFF
    hi = ((str_va + 0x8000) >> 16) & 0xFFFF
    if hi not in lis_index: continue
    for lis_off, lis_rd in lis_index[hi]:
        for j in range(1, 20):
            idx = lis_off + j * 4
            if idx + 4 > tvsize: break
            w2   = int.from_bytes(data[traw+idx:traw+idx+4], 'big')
            op2  = (w2>>26)&0x3F; rd2=(w2>>21)&0x1F; ra2=(w2>>16)&0x1F; imm2=w2&0xFFFF
            if op2 == 14 and ra2 == lis_rd and imm2 == lo:
                fn_va = owning_fn(lis_off)
                if fn_va is None: break
                if fn_va not in fn_class_map:
                    fn_class_map[fn_va] = (cls, meth, full)
                break
            if op2 == 15 and rd2 == lis_rd: break

pass1_unnamed = 0
pass1_partial = 0
for fn_va, (cls, meth, full) in fn_class_map.items():
    cur = sym_map.get(fn_va, '')
    proposed = safe_name(f"{cls}_{meth}")
    if fn_va in unnamed:
        if propose(fn_va, proposed, f"string ref: {full[:60]!r}", "P1"):
            pass1_unnamed += 1
    elif not cur.startswith('fn_8'):
        if propose(fn_va, proposed, f"string ref (was {cur}): {full[:60]!r}", "P1r"):
            pass1_partial += 1

print(f"  Functions identified by class: {len(fn_class_map)}")
print(f"  Unnamed (fn_8...) renamed:     {pass1_unnamed}")
print(f"  Partial names renamed:         {pass1_partial}")

# PASS 2: thunks
print("\n=== PASS 2: Thunk detection ===")
pass2_count = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    sz = fn_meta.get(fn_va, 0)
    if sz != 4: continue
    off = va_to_file(fn_va)
    if off is None: continue
    w  = int.from_bytes(data[off:off+4], 'big')
    op = (w >> 26) & 0x3F
    if op == 18 and not (w & 1):
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        tgt = fn_va + li
        if tgt in sym_map:
            tgt_name = new_names.get(tgt, sym_map[tgt])
            proposed = safe_name('thunk_' + tgt_name)
            if propose(fn_va, proposed, f"thunk -> {tgt_name} (0x{tgt:08X})", "P2"):
                pass2_count += 1
print(f"  Named {pass2_count} thunks")

# PASS 3: call-graph propagation - tiered thresholds (Task 5)
print("\n=== PASS 3: Call-graph class propagation (tiered, iterative) ===")

# Task 2 - expanded KNOWN_CLASSES
KNOWN_CLASSES = [
    'pongBlendShapeController','pongSkelLookAtDriver','pongBlendLookAtDriver',
    'pongLookAtDriver','pongTrainingDrill','pongLiveManager','pongLerpQueue',
    'pongFrontendState','pongCutSceneManager','pongCSActionCamAnim',
    'pongDrawBucketManager','pongDrawBucket','pongGameContext',
    'pongRemotePlayer','pongBallInstance','pongShadowMap',
    'pongCreature','pongPlayer','pongShell','pongPresence','pongMover',
    'gmLogicSinglesMatch','gmLogicSwingPractice','gmLogicSpectator',
    'gmBallNode','gmLogic',
    'gdCSCharCamShotNameSeq','gdCSCharCamShotName','gdCSCharAnimNames',
    'gdCSCharAnimData','gdCSCamAnimShotNameSeq','gdCSCamAnimShotName',
    'gdCSNameData','gdCutSceneData','gdCSActionIfData',
    'gdCSActionCamAnimData','gdCSActionCharAnimData',
    'gdCSActionCharVisibleData','gdCSActionPlayAudioData',
    'gdCSActionLvlAmbAnimData','gdCSActionLvlAnimData',
    'gdCSActionCharAmbAnimData',
    'gdDrillCounterSpinData','gdDrillFocusShotData','gdDrillChargingData',
    'gdDrillSoftShotData','gdDrillServingData','gdDrillMovementData',
    'gdDrillSpinData','gdDifficultyLadder','gdLadderCommonData',
    'gdLadderMatch','gdLadder','gdRivalryData','gdRivalry',
    'gdTierMember','gdTier','gdCameraDest','gdCameraInst','gdCameraDef',
    'gdVibEvent','gdBallPredictData','gdCrAttachmentData','gdCrData',
    'gdTrainingDrillData','gdLevelData','gdReplayMgr','gdGameData',
    'gdBallData','gdMatchData',
    'pcrFaceAnimBlender','pcrSwingBlender','pcr',
    'crSkeletonData','crSkeleton','crIKBodyBase','crIKSpine',
    'crIKHead','crIKLimb','crFrame',
    'LocomotionStateAnim','LocomotionStateMf','LocomotionState',
    'phBoundGeometry','phBoundOctree','phBoundBox','phPolytree',
    'phSimulator','phLevelNew','phArchetype','phBound','phLevel',
    'grcTextureFactory','grcRenderTarget','grcDevice',
    'grmEdgeModel','grmModelFactory',
    'audVoiceStream','audVoiceSfx','audChannel','audManager','audBank','audVoice',
    'fxBallSpinLines','fxAmbAnimSet','fxString','fxCloth','fxHair','fxEffect',
    'fiDevice','fiStream','fiPackfile',
    'pgStreamer','pgPackfile',
    'xmlNodeStructAnimHashEntryArray','xmlNodeStruct',
    'SinglesNetworkClient','PongNetGameModeCoordinator',
    'NetBallHitManagerFreeList','PongBallHitDataStack','NetTimer',
    'SessionMessageRequester','SpectatorBallHitMessage',
    'BallHitMessage','HitMessage','NetworkClient',
    'AckHandling','netHardware',
    'CCalMoviePlayer','CCalChannel',
    'cmNaryOperator','cmMetafileTuningSet',
    'hudTrainingPopUp','hudLeaderboard',
    'swfOBJECT','swfCMD',
    'geomBoxes','geomSphere',
    'sysMemSimpleAllocator','sysMem',
    'SYMTAB','Timer','gameLoop',
    'rage','grc','aud','ke','ph','pg','gm','cr','gd','fx','fi',
    'sys','net','io','gfx','pcr','pong','xe','nt','xam',
]

def get_class(fn_va):
    if fn_va in fn_class_map:
        return fn_class_map[fn_va][0]
    name = new_names.get(fn_va, sym_map.get(fn_va, ''))
    if not name or name.startswith('fn_8'): return None
    nl = name.lower()
    for cls in KNOWN_CLASSES:
        if nl.startswith(cls.lower()):
            return cls
    return None

pass3_total = 0
pass3_wrap  = 0
for iteration in range(15):
    round_count = 0
    for fn_va in unnamed:
        if fn_va in new_names: continue
        callees = callgraph.get(fn_va, set())
        if not callees: continue
        callee_classes = Counter()
        for callee in callees:
            cls = get_class(callee)
            if cls: callee_classes[cls] += 1
        if not callee_classes: continue
        dominant, cnt = callee_classes.most_common(1)[0]
        named_callee_count = sum(1 for c in callees
                                  if not sym_map.get(c,'').startswith('fn_8'))
        ratio = cnt / named_callee_count if named_callee_count > 0 else 0

        # High confidence: >=2 calls AND >=40% ratio
        if cnt >= 2 and ratio >= 0.40:
            proposed = safe_name(f"{dominant}_{fn_va & 0xFFFF:04X}")
            reason   = f"calls {cnt}x into '{dominant}' ({cnt}/{named_callee_count} named callees)"
            if propose(fn_va, proposed, reason, "P3"):
                round_count += 1
                fn_class_map[fn_va] = (dominant, f"sub_{fn_va & 0xFFFF:04X}", "")
        # Medium: 1 call, small function (<=48 bytes)
        elif cnt == 1 and fn_meta.get(fn_va, 0) <= 48:
            proposed = safe_name(f"{dominant}_{fn_va & 0xFFFF:04X}_wrap")
            reason   = f"small wrapper calls 1x into '{dominant}' (size={fn_meta.get(fn_va,0)})"
            if propose(fn_va, proposed, reason, "P3w"):
                round_count += 1
                pass3_wrap += 1
                fn_class_map[fn_va] = (dominant, f"sub_{fn_va & 0xFFFF:04X}", "")
        # Legacy fallback: >=2 calls any ratio
        elif cnt >= 2:
            proposed = safe_name(f"{dominant}_{fn_va & 0xFFFF:04X}")
            reason   = f"calls {cnt}x into '{dominant}' ({cnt}/{named_callee_count} named callees)"
            if propose(fn_va, proposed, reason, "P3"):
                round_count += 1
                fn_class_map[fn_va] = (dominant, f"sub_{fn_va & 0xFFFF:04X}", "")

    pass3_total += round_count
    print(f"  Round {iteration+1}: named {round_count}")
    if round_count == 0: break

print(f"  Pass 3 total: {pass3_total}  (of which _wrap: {pass3_wrap})")

# PASS 4: vtable slot inheritance - NULL-tolerant (Task 6)
print("\n=== PASS 4: Vtable slot inheritance (NULL-tolerant, min-run=3) ===")
text_va_lo = image_base + tvaddr
text_va_hi = text_va_lo + tvsize
vtables = []
rdata_chunk = data[rraw:rraw+rvsize]
i = 0
while i < rvsize - 12:
    run = []; null_count = 0; j = i
    while j + 4 <= rvsize:
        ptr = int.from_bytes(rdata_chunk[j:j+4], 'big')
        if text_va_lo <= ptr < text_va_hi:
            run.append(ptr); null_count = 0; j += 4
        elif ptr == 0:
            if null_count < 2: null_count += 1; run.append(0); j += 4
            else: break
        else: break
    while run and run[-1] == 0: run.pop()
    if len(run) >= 3:
        vtables.append((image_base + rvaddr + i, run)); i = j
    else:
        i += 4

print(f"  Found {len(vtables)} vtable candidates")
pass4_count = 0
for vt_va, slots in vtables:
    slot_classes = Counter()
    for fn_va in slots:
        if fn_va == 0: continue
        cls = get_class(fn_va)
        if cls: slot_classes[cls] += 1
    if not slot_classes: continue
    dominant, cnt = slot_classes.most_common(1)[0]
    if cnt < 2: continue
    for slot_idx, fn_va in enumerate(slots):
        if fn_va == 0: continue
        if fn_va not in unnamed or fn_va in new_names: continue
        proposed = safe_name(f"{dominant}_vt{vt_va & 0xFFFF:04X}_{slot_idx}")
        reason   = f"vtable 0x{vt_va:08X} slot {slot_idx}, class '{dominant}' ({cnt} named slots)"
        if propose(fn_va, proposed, reason, "P4"):
            pass4_count += 1
            fn_class_map[fn_va] = (dominant, f"vt_{slot_idx}", "")
print(f"  Named {pass4_count} vtable slots")

# PASS 5: NOP / pure-virtual stubs
print("\n=== PASS 5: NOP / pure-virtual stubs ===")
BLR=0x4E800020; TRAP=0x7FE00008; NOP=0x60000000
pass5_count = 0
for fn_va in unnamed:
    if fn_va in new_names: continue
    sz  = fn_meta.get(fn_va, 0)
    if sz > 8: continue
    off = va_to_file(fn_va)
    if off is None: continue
    words = [int.from_bytes(data[off+k*4:off+k*4+4],'big') for k in range(sz//4)]
    real  = [w for w in words if w != NOP]
    if real == [BLR]:
        if propose(fn_va, safe_name(f"stub_{fn_va & 0xFFFF:04X}"), "blr-only stub", "P5"):
            pass5_count += 1
    elif TRAP in words:
        if propose(fn_va, safe_name(f"pure_{fn_va & 0xFFFF:04X}"), "trap=pure virtual", "P5"):
            pass5_count += 1
print(f"  Named {pass5_count} stubs/pure-virtuals")

# Summary & output
total_unnamed_renamed = sum(1 for va in new_names if va in unnamed)
total_partial_renamed = sum(1 for va in new_names if va not in unnamed)
total = len(new_names)
pass_counts = Counter(pass_tag.values())

print(f"\nWriting {OUT} ...")
out_lines = []
for line in sym_lines:
    s = line.strip(); matched = False
    for sect in ['.text', '.rdata', '.data']:
        if '= '+sect+':0x' in s:
            parts = s.split('=', 1); name = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                if addr in new_names:
                    out_lines.append(line.replace(name, new_names[addr], 1))
                    matched = True; break
            except: pass
    if not matched: out_lines.append(line)

with open(OUT, 'w') as f: f.writelines(out_lines)

print(f"Writing {REPORT} ...")
with open(REPORT, 'w') as f:
    f.write("auto_namer_v3.py Results\n")
    f.write("=" * 70 + "\n\n")
    f.write(f"Pass 1  string refs - fn_8 renames:    {pass_counts.get('P1',0):5d}\n")
    f.write(f"Pass 1r string refs - partial renames: {pass_counts.get('P1r',0):5d}\n")
    f.write(f"Pass 2  thunks:                        {pass_counts.get('P2',0):5d}\n")
    f.write(f"Pass 3  call-graph (high conf):        {pass_counts.get('P3',0):5d}\n")
    f.write(f"Pass 3w call-graph (wrap, med conf):   {pass_counts.get('P3w',0):5d}\n")
    f.write(f"Pass 4  vtable slots:                  {pass_counts.get('P4',0):5d}\n")
    f.write(f"Pass 5  stubs/pure-virtuals:           {pass_counts.get('P5',0):5d}\n")
    f.write(f"{'─'*45}\n")
    f.write(f"Total renames:                         {total:5d}\n")
    f.write(f"  Of which unnamed (fn_8...) fixed:   {total_unnamed_renamed:5d}\n")
    f.write(f"  Of which partial names fixed:        {total_partial_renamed:5d}\n")
    f.write(f"Previously unnamed:                    {len(unnamed):5d}\n")
    f.write(f"Coverage (fn_8 fixed):                 {total_unnamed_renamed/max(len(unnamed),1)*100:.1f}%\n\n")
    f.write("=" * 70 + "\n")
    f.write("DETAIL (sorted by address)\n")
    f.write("=" * 70 + "\n")
    for addr in sorted(new_names):
        tag = pass_tag[addr]
        f.write(f"  [{tag:3s}] 0x{addr:08X}  {new_names[addr]:<55s}  {confidence[addr]}\n")

print(f"\n{'='*60}")
print(f"  Pass 1  (string refs, fn_8):       {pass_counts.get('P1',0)}")
print(f"  Pass 1r (string refs, partials):   {pass_counts.get('P1r',0)}")
print(f"  Pass 2  (thunks):                  {pass_counts.get('P2',0)}")
print(f"  Pass 3  (call-graph high):         {pass_counts.get('P3',0)}")
print(f"  Pass 3w (call-graph wrap):         {pass_counts.get('P3w',0)}")
print(f"  Pass 4  (vtable slots):            {pass_counts.get('P4',0)}")
print(f"  Pass 5  (stubs):                   {pass_counts.get('P5',0)}")
print(f"  {'─'*40}")
print(f"  TOTAL renames:                     {total}")
print(f"  Unnamed (fn_8...) fixed:           {total_unnamed_renamed}")
print(f"  Partial names fixed:               {total_partial_renamed}")
print(f"  Coverage (fn_8 → named):           {total_unnamed_renamed/max(len(unnamed),1)*100:.1f}%")
print(f"\n  Output:   {OUT}")
print(f"  Report:   {REPORT}")
print(f"  .pdata:   {PDATA_REPORT}")
print(f"{'='*60}")
print("\nReview auto_namer_v3_report.txt, then to apply:")
print("  cp config/434C4803/symbols_autonamed.txt config/434C4803/symbols.txt")
