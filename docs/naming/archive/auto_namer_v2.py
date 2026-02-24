#!/usr/bin/env python3
"""
auto_namer_v2.py  (corrected & improved)
=========================================
Fixes over original v2:
  - Pass 1 now renames ALL class-matched functions (not just fn_8...)
    This transforms game_7530 -> pongCreature_StartEmote etc.
  - get_class() uses fn_class_map EXACT class names first (pongCreature, pgStreamer)
    before falling back to crude prefix matching.
  - Pass 3 (call-graph) runs iteratively until convergence, not just once.
  - Pass 4 (vtable) benefits from the richer class map from the fixed passes.
  - All deduplication: proposed name suffix includes low address bits to avoid collisions.

Output:
  config/434C4803/symbols_autonamed.txt
  auto_namer_v2_report.txt
"""

import re, sys, os, bisect
from collections import defaultdict, Counter

BASE   = os.path.dirname(os.path.abspath(__file__))
XEX    = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS   = os.path.join(BASE, 'config/434C4803/symbols.txt')
OUT    = os.path.join(BASE, 'config/434C4803/symbols_autonamed.txt')
REPORT = os.path.join(BASE, 'auto_namer_v2_report.txt')

# ── load binary ───────────────────────────────────────────────────────────────
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
_, tvaddr, tvsize, traw = text_sec
_, rvaddr, rvsize, rraw = rdata_sec

def va_to_file(va):
    rel = va - image_base
    for _, vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

# ── load symbols ──────────────────────────────────────────────────────────────
print("\nLoading symbols...")
sym_map   = {}   # addr -> current name
sym_lines = []
fn_meta   = {}   # addr -> size in bytes

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
def is_unnamed(addr):
    name = sym_map.get(addr, '')
    if not name: return True
    if name.startswith('fn_8'): return True
    # If name is just 'rage_1234' it might be worth refining, but for now 
    # let's stick to 'fn_8' or 'game_XXXX' or 'pg_XXXX' as potentially refineable
    if re.match(r'^(game|pg|ph|grc|aud|ke|nt|xam|xe|rage|cr|gd|fx|fi|sys|net|io|gfx|pcr|pong)_[0-9A-F]{4}$', name):
        return True
    return False

# Re-define unnamed set with broader criteria
unnamed = {a for a in fn_addrs if is_unnamed(a)}
print(f"  Refined unnamed count: {len(unnamed)}")

def owning_fn(text_off):
    idx = bisect.bisect_right(fn_offsets, text_off) - 1
    return (image_base+tvaddr+fn_offsets[idx]) if idx >= 0 else None

def safe_name(s):
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    return re.sub(r'_+', '_', s).strip('_')[:80]

# Track all renames
new_names  = {}    # fn_va -> proposed name
confidence = {}    # fn_va -> explanation
pass_tag   = {}    # fn_va -> pass label

# Keep a running set of used names to avoid duplicates
used_names = set(sym_map.values())

def propose(fn_va, name, reason, tag):
    """Record a rename only if we haven't already named this function."""
    if fn_va in new_names:
        return False
    if name in used_names:
        # Append low address bits to disambiguate
        name = f"{name}_{fn_va & 0xFFFF:04X}"
    if name in used_names:
        return False
    new_names[fn_va]  = name
    confidence[fn_va] = reason
    pass_tag[fn_va]   = tag
    used_names.add(name)
    return True

# ── pre-build call graph ──────────────────────────────────────────────────────
print("\nBuilding call graph...")
callgraph = defaultdict(set)   # fn_va -> set of callee fn_va

# Pre-track lis/addi state for mtctr resolving
last_lis = {} # rd -> (va, offset)

for i in range(0, tvsize, 4):
    w  = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op = (w >> 26) & 0x3F
    
    # 1. Direct calls (bl)
    if op == 18 and (w & 1):   # bl
        li = w & 0x3FFFFFC
        if li & 0x2000000: li |= ~0x3FFFFFF
        caller_va = owning_fn(i)
        tgt_va    = (image_base+tvaddr+i) + li
        if caller_va and tgt_va in sym_map:
            callgraph[caller_va].add(tgt_va)
            
    # 2. Track lis/addi for indirect calls
    rd = (w >> 21) & 0x1F
    ra = (w >> 16) & 0x1F
    imm = w & 0xFFFF
    if op == 15 and ra == 0: # lis
        last_lis[rd] = (imm << 16, i)
    elif op == 14 and ra in last_lis: # addi
        # Only if addi follows lis relatively closely (within 16 bytes)
        prev_imm, prev_off = last_lis[ra]
        if i - prev_off <= 16:
            target = (prev_imm + (imm if imm < 0x8000 else (imm - 0x10000)))
            last_lis[rd] = (target, i)
            
    # 3. mtctr + bctr (indirect call pattern)
    if op == 31 and ((w >> 1) & 0x3FF) == 467: # mtctr
        rs = (w >> 21) & 0x1F
        if rs in last_lis:
            tgt_va, prev_off = last_lis[rs]
            if i - prev_off <= 16:
                # Look ahead for bctr (0x4E800420)
                if i + 4 < tvsize:
                    w_next = int.from_bytes(data[traw+i+4:traw+i+8], 'big')
                    if w_next == 0x4E800420:
                        caller_va = owning_fn(i)
                        if caller_va and tgt_va in sym_map:
                            callgraph[caller_va].add(tgt_va)

print(f"  Call edges: {sum(len(v) for v in callgraph.values())}")

# ── PASS 1: string-reference naming ──────────────────────────────────────────
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

# Pre-index .text by lis hi value
lis_index = defaultdict(list)
for i in range(0, tvsize, 4):
    w   = int.from_bytes(data[traw+i:traw+i+4], 'big')
    op  = (w >> 26) & 0x3F
    rd  = (w >> 21) & 0x1F
    ra  = (w >> 16) & 0x1F
    imm = w & 0xFFFF
    if op == 15 and ra == 0:
        lis_index[imm].append((i, rd))

KNOWN_CLASSES = [
    'pongCreature','pongPlayer','pongLiveManager','pongMover','pongTrainingDrill',
    'pongDrillCounterSpin','pongDrawBucket','pongShell','pongShadowMap',
    'pongRemotePlayer','pongPresence','PongNetGameModeCoordinator','pongLadder',
    'pongGameContext','pongFrontendState','pongDrillSpin','pongDrillSoftShot',
    'pongDrillSmash','pongDrawBucketManager','pongCutSceneManager','pongCSActionCamAnim',
    'pongBallInstance','PongBallHitDataStack',
    'pongBlendShapeController','pongSkelLookAtDriver','pongBlendLookAtDriver',
    'pongLookAtDriver','pongLerpQueue','pgStreamer','pgPackfile',
    'phPolytree','phSimulator','phLevelNew','phLevel','phBoundGeometry',
    'phBoundOctree','phBoundBox','phBound','phArchetype',
    'gmLogicSinglesMatch','gmLogicSwingPractice','gmLogic','gmLogicSpectator','gmBallNode',
    'grmModelFactory','grmEdgeModel','grcTextureFactory','grcRenderTarget',
    'grcDevice','grcRenderer','grcDisplay',
    'fxCloth','fxHair','fxEffect',
    'audVoiceStream','audVoiceSfx','audSystem','audBank','audChannel','audManager',
    'fiDevice','fiStream','fiPackfile',
    'gdTrainingDrillData','gdCrAttachmentData','gdBallPredictData',
    'gdBallData','gdMatchData','gdTier','gdDifficultyLadder','gdCSActionIfData','gdVibEvent',
    'SinglesNetworkClient','NetworkClient','NetDll','NetTimer','netSystem','netHardware',
    'NetBallHitManagerFreeList','SpectatorBallHitMessage','SessionMessageRequester',
    'geomBoxes','geomSphere',
    'crSkeletonData','crSkeleton','crFrame',
    'LocomotionStateAnim','LocomotionStateMf','LocomotionState',
    'CCalChannel','CCalMoviePlayer','xmlNodeStructAnimHashEntryArray','xmlNodeStruct',
    'pcrFaceAnimBlender','pcrSwingBlender','pcr',
    'sysMemSimpleAllocator','sysMemAllocator','Timer','SYMTAB','swfOBJECT','swfCMD',
    'hudTrainingPopUp','hudLeaderboard','HitMessage',
    'nt','xam','xe','rage','grc','aud','ke','ph','pg','gm','cr','gd','fx','fi',
    'sys','net','io','gfx','pcr','pong','gameLoop','ioInput',
]

def get_class(fn_va):
    """Return exact class name for a function, or None."""
    # First: string-derived exact class (best source)
    if fn_va in fn_class_map:
        return fn_class_map[fn_va][0]
    # Second: if we just gave it a new name, extract class from that
    name = new_names.get(fn_va, sym_map.get(fn_va, ''))
    if not name or name.startswith('fn_8'): return None
    # If name is like "pongCreature_StartEmote", class is "pongCreature"
    # Try to extract a known class prefix from the name
    name_lower = name.lower()
    for cls in KNOWN_CLASSES:
        if name_lower.startswith(cls.lower()):
            return cls
    return None

# fn_va -> (class, method, full_text) for ALL class-identified functions
fn_class_map = {}   # fn_va -> (class_str, method_str, full_debug_string)

# SEED with existing names from symbols.txt
for addr, name in sym_map.items():
    if not name or name.startswith('fn_8'): continue
    cls = get_class(addr)
    if cls:
        fn_class_map[addr] = (cls, name.replace(cls, '').strip('_'), name)

for str_va, (cls, meth, full) in all_strings.items():
    lo = str_va & 0xFFFF
    hi = ((str_va + 0x8000) >> 16) & 0xFFFF   # correct formula
    if hi not in lis_index:
        continue
    for lis_off, lis_rd in lis_index[hi]:
        for j in range(1, 20):
            idx = lis_off + j * 4
            if idx + 4 > tvsize: break
            w2   = int.from_bytes(data[traw+idx:traw+idx+4], 'big')
            op2  = (w2>>26)&0x3F
            rd2  = (w2>>21)&0x1F
            ra2  = (w2>>16)&0x1F
            imm2 = w2 & 0xFFFF
            if op2 == 14 and ra2 == lis_rd and imm2 == lo:
                fn_va = owning_fn(lis_off)  # owning_fn returns full VA
                if fn_va is None: break
                if fn_va not in fn_class_map:
                    fn_class_map[fn_va] = (cls, meth, full)
                break
            if op2 == 15 and rd2 == lis_rd: break

# *** KEY FIX: rename ALL functions with string matches (not just fn_8...) ***
# This turns game_7530 -> pongCreature_StartEmote, pg_8318 -> pgStreamer_Read, etc.
pass1_unnamed = 0
pass1_partial = 0
for fn_va, (cls, meth, full) in fn_class_map.items():
    cur = sym_map.get(fn_va, '')
    # Build a good name from the class + method
    proposed = safe_name(f"{cls}_{meth}")
    if fn_va in unnamed:
        if propose(fn_va, proposed, f"string ref: {full[:60]!r}", "P1"):
            pass1_unnamed += 1
    elif not cur.startswith('fn_8'):
        # It's a partially-named function like game_7530 - rename it too
        if propose(fn_va, proposed, f"string ref (was {cur}): {full[:60]!r}", "P1r"):
            pass1_partial += 1

print(f"  Functions identified by class: {len(fn_class_map)}")
print(f"  Unnamed (fn_8...) renamed:     {pass1_unnamed}")
print(f"  Partial names renamed:         {pass1_partial}")

# ── PASS 2: thunks ────────────────────────────────────────────────────────────
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
            # Use new name if we just renamed the target
            tgt_name = new_names.get(tgt, sym_map[tgt])
            proposed = safe_name('thunk_' + tgt_name)
            if propose(fn_va, proposed, f"thunk -> {tgt_name} (0x{tgt:08X})", "P2"):
                pass2_count += 1
print(f"  Named {pass2_count} thunks")

# ── PASS 3: call-graph class propagation (iterative) ─────────────────────────
print("\n=== PASS 3: Call-graph class propagation (iterative) ===")

pass3_total = 0
for iteration in range(10):  # up to 10 propagation rounds
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
        sz = fn_meta.get(fn_va, 0)

        # Tiered Confidence Strategy
        proposed = None
        reason   = ""
        tag      = "P3"

        # Tier 1: High confidence (>= 2 calls OR >= 40% ratio with multiple calls)
        if cnt >= 2:
            proposed = safe_name(f"{dominant}_{fn_va & 0xFFFF:04X}")
            reason   = f"calls {cnt}x into '{dominant}' ({cnt}/{named_callee_count} named callees)"
        
        # Tier 2: Medium confidence (1 call, small function <= 48 bytes) -> _wrap
        elif cnt == 1 and sz <= 48:
            proposed = safe_name(f"{dominant}_{fn_va & 0xFFFF:04X}_wrap")
            reason   = f"single-call wrapper (sz={sz}) to '{dominant}'"
            tag      = "P3w"

        # Tier 3: Transitive (only calls ONE function, and it was named this pass or earlier)
        elif len(callees) == 1 and cnt == 1:
            proposed = safe_name(f"{dominant}_{fn_va & 0xFFFF:04X}")
            reason   = f"transitively calling '{dominant}'"
            tag      = "P3t"

        if proposed and propose(fn_va, proposed, reason, tag):
            round_count += 1
            # Update fn_class_map so this function can seed further rounds
            fn_class_map[fn_va] = (dominant, proposed.split('_', 1)[1] if '_' in proposed else proposed, "")

    pass3_total += round_count
    print(f"  Round {iteration+1}: named {round_count}")
    if round_count == 0:
        break

print(f"  Pass 3 total: {pass3_total}")

# ── PASS 6: Constructor Linking ──────────────────────────────────────────────
print("\n=== PASS 6: Constructor Linking ===")

text_va_lo = image_base + tvaddr
text_va_hi = text_va_lo + tvsize

# 1. Identify vtable candidates again (using robust logic)
vtables_p6 = []
rdata_chunk = data[rraw:rraw+rvsize]
i = 0
while i < rvsize - 12:
    run = []
    j = i
    null_count = 0
    while j + 4 <= rvsize:
        ptr = int.from_bytes(rdata_chunk[j:j+4], 'big')
        if text_va_lo <= ptr < text_va_hi:
            run.append(ptr)
            null_count = 0
            j += 4
        elif ptr == 0:
            if null_count < 2:
                null_count += 1
                run.append(0)
                j += 4
            else:
                break
        else:
            break
    while run and run[-1] == 0: run.pop()
    if len(run) >= 3:
        vt_va = image_base + rvaddr + i
        vtables_p6.append((vt_va, run))
        i = j
    else:
        i += 4

print(f"  Found {len(vtables_p6)} vtable candidates for linking")

# 2. Find constructors using lis_index
pass6_count = 0
vt_to_class = {} # vt_va -> class_name

for vt_va, slots in vtables_p6:
    hi = ((vt_va + 0x8000) >> 16) & 0xFFFF
    lo = vt_va & 0xFFFF
    
    if hi not in lis_index: continue
    
    ctor_candidates = []
    for lis_off, lis_rd in lis_index[hi]:
        # Scan forward for addi + stw
        current_reg = lis_rd
        has_addi = False
        
        for k in range(1, 40): # Look ahead 40 insns
            idx = lis_off + k * 4
            if idx + 4 > tvsize: break
            w   = int.from_bytes(data[traw+idx:traw+idx+4], 'big')
            op  = (w >> 26) & 0x3F
            rd  = (w >> 21) & 0x1F
            ra  = (w >> 16) & 0x1F
            imm = w & 0xFFFF
            
            if op == 14 and ra == current_reg:
                if imm == lo:
                    current_reg = rd 
                    has_addi = True
            
            elif has_addi and op == 36: # stw
                d = imm if imm < 0x8000 else imm - 0x10000
                if d == 0:
                    rs = (w >> 21) & 0x1F
                    target = ra 
                    if rs == current_reg and target == 3: # Store to r3!
                        fn = owning_fn(lis_off)
                        if fn:
                            ctor_candidates.append(fn)
                        break

    if ctor_candidates:
        ctor_fn = ctor_candidates[0]
        cls = get_class(ctor_fn)
        if cls:
            vt_to_class[vt_va] = cls
        else:
            slot_classes = Counter()
            for s_fn in slots:
                if s_fn == 0: continue
                s_cls = get_class(s_fn)
                if s_cls: slot_classes[s_cls] += 1
            
            if slot_classes:
                dom, cnt = slot_classes.most_common(1)[0]
                if cnt >= 2:
                    proposed = safe_name(f"{dom}_ctor_{ctor_fn & 0xFFFF:04X}")
                    if propose(ctor_fn, proposed, f"constructor for vtable 0x{vt_va:08X} (class {dom})", "P6"):
                        pass6_count += 1
                        fn_class_map[ctor_fn] = (dom, "ctor", "")
                        vt_to_class[vt_va] = dom

print(f"  Named {pass6_count} constructors from vtables")

# ── PASS 4: Vtable slot inheritance ──────────────────────────────────────────
print("\n=== PASS 4: Vtable slot inheritance ===")

text_va_lo = image_base + tvaddr
text_va_hi = text_va_lo + tvsize

vtables = [] # Clear old list, reusing p6
pass4_count = 0
for vt_va, slots in vtables_p6:
    dominant = vt_to_class.get(vt_va)
    cnt = 999 if dominant else 0
    
    if not dominant:
        slot_classes = Counter()
        for fn_va in slots:
            if fn_va == 0: continue
            cls = get_class(fn_va)
            if cls: slot_classes[cls] += 1
        if slot_classes:
            dominant, cnt = slot_classes.most_common(1)[0]
    
    if not dominant or cnt < 2: continue

    for slot_idx, fn_va in enumerate(slots):
        if fn_va == 0: continue 
        if fn_va not in unnamed: continue
        if fn_va in new_names: continue
        proposed = safe_name(f"{dominant}_vt{vt_va & 0xFFFF:04X}_{slot_idx}")
        reason   = f"vtable 0x{vt_va:08X} slot {slot_idx}, class '{dominant}'"
        if propose(fn_va, proposed, reason, "P4"):
            pass4_count += 1
            fn_class_map[fn_va] = (dominant, f"vt_{slot_idx}", "")

print(f"  Named {pass4_count} vtable slots")

# ── PASS 7: Broad String Reference Naming ───────────────────────────────────
print("\n=== PASS 7: Broad String Reference Naming ===")

# Strings to look for (common structural names, subsystems, RTTI-like)
STRUCT_STRINGS = [
    'PostLoadProperties', 'cmOperatorCtor', 'pongNetMessageHolder', 'rlNotifier',
    'atSingleton', 'VNotifyHandler', 'cmSampleCamActions', 'cmSimpleActionCtor',
    'YAXPAV', 'PAVcm', 'StatsReaderNotifyHandler', 'cmRefreshableCtor',
    'cmNaryOperator', 'xact2System', 'cmWorldRefreshableCtor', 'atSafeContainer',
    'snSession', 'DLListSimple', 'atSafeDLListSimple', 'mcMemcardControl',
    'cmReporterCtor', 'PLAYERMGR', 'WriteStats', 'atFunctor3', 'atSingleton',
    'pgStreamer', 'gdDrillMovementData', 'gdDrillSoftShotData', 'atArray',
    'rage', 'grc', 'ph', 'aud', 'ke', 'fi', 'sys', 'net', 'io'
]

pass7_count = 0
for s_text in STRUCT_STRINGS:
    # Find ALL occurrences of this string in .rdata
    # (Simple approach: search bytes)
    s_bytes = s_text.encode()
    start = 0
    while True:
        idx = data.find(s_bytes, rraw + start, rraw + rvsize)
        if idx == -1: break
        str_va = image_base + rvaddr + (idx - rraw)
        start = (idx - rraw) + 1
        
        # Now find functions referencing this str_va
        hi = ((str_va + 0x8000) >> 16) & 0xFFFF
        lo = str_va & 0xFFFF
        
        if hi in lis_index:
            for lis_off, lis_rd in lis_index[hi]:
                # Scan for addi/lwz referencing this string
                for k in range(1, 40):
                    idx_t = lis_off + k * 4
                    if idx_t + 4 > tvsize: break
                    w = int.from_bytes(data[traw+idx_t : traw+idx_t+4], 'big')
                    op = (w >> 26) & 0x3F
                    ra = (w >> 16) & 0x1F
                    imm = w & 0xFFFF
                    
                    if (op == 14 or op == 32) and ra == lis_rd and imm == lo:
                        fn_va = owning_fn(lis_off)
                        if fn_va:
                            # Propose a name based on the string
                            proposed = safe_name(f"ref_{s_text}_{fn_va & 0xFFFF:04X}")
                            if propose(fn_va, proposed, f"references string '{s_text}'", "P7"):
                                pass7_count += 1
                                # Also seed class if it looks like one
                                if s_text in KNOWN_CLASSES:
                                    fn_class_map[fn_va] = (s_text, "ref", "")
                        break

print(f"  Named {pass7_count} functions referencing structural strings")

# ── PASS 5: NOP / pure-virtual stubs ─────────────────────────────────────────
print("\n=== PASS 5: NOP / pure-virtual stubs ===")
BLR  = 0x4E800020
TRAP = 0x7FE00008
NOP  = 0x60000000

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
        proposed = safe_name(f"stub_{fn_va & 0xFFFF:04X}")
        if propose(fn_va, proposed, "blr-only stub", "P5"):
            pass5_count += 1
    elif TRAP in words:
        proposed = safe_name(f"pure_{fn_va & 0xFFFF:04X}")
        if propose(fn_va, proposed, "trap=pure virtual", "P5"):
            pass5_count += 1

print(f"  Named {pass5_count} stubs/pure-virtuals")

# ── summary ───────────────────────────────────────────────────────────────────
total_unnamed_renamed = sum(1 for va in new_names if va in unnamed)
total_partial_renamed = sum(1 for va in new_names if va not in unnamed)
total = len(new_names)

pass_counts = Counter(pass_tag.values())

# ── write output ──────────────────────────────────────────────────────────────
print(f"\nWriting {OUT} ...")

# Build lookup: address -> new name for ANY renamed address
out_lines = []
for line in sym_lines:
    s = line.strip()
    matched = False
    for sect in ['.text', '.rdata', '.data']:
        if '= '+sect+':0x' in s:
            parts = s.split('=', 1)
            name  = parts[0].strip()
            try:
                addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                if addr in new_names:
                    out_lines.append(line.replace(name, new_names[addr], 1))
                    matched = True
                    break
            except: pass
    if not matched:
        out_lines.append(line)

with open(OUT, 'w') as f:
    f.writelines(out_lines)

print(f"Writing {REPORT} ...")
with open(REPORT, 'w') as f:
    f.write("auto_namer_v2.py Results (corrected)\n")
    f.write("=" * 70 + "\n\n")
    f.write(f"Pass 1  string refs - fn_8 renames:    {pass_counts.get('P1',0):5d}\n")
    f.write(f"Pass 1r string refs - partial renames: {pass_counts.get('P1r',0):5d}\n")
    f.write(f"Pass 2  thunks:                        {pass_counts.get('P2',0):5d}\n")
    f.write(f"Pass 3  call-graph propagation:        {pass_counts.get('P3',0) + pass_counts.get('P3w',0) + pass_counts.get('P3t',0):5d}\n")
    f.write(f"  - P3  (standard):                    {pass_counts.get('P3',0):5d}\n")
    f.write(f"  - P3w (wrapper):                     {pass_counts.get('P3w',0):5d}\n")
    f.write(f"  - P3t (transitive):                  {pass_counts.get('P3t',0):5d}\n")
    f.write(f"Pass 4  vtable slots:                  {pass_counts.get('P4',0):5d}\n")
    f.write(f"Pass 5  stubs/pure-virtuals:           {pass_counts.get('P5',0):5d}\n")
    f.write(f"Pass 6  constructors:                  {pass_counts.get('P6',0):5d}\n")
    f.write(f"Pass 7  structural strings:            {pass_counts.get('P7',0):5d}\n")
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
print(f"  Pass 3  (call-graph):              {pass_counts.get('P3',0)}")
print(f"  Pass 4  (vtable slots):            {pass_counts.get('P4',0)}")
print(f"  Pass 5  (stubs):                   {pass_counts.get('P5',0)}")
print(f"  Pass 6  (constructors):            {pass_counts.get('P6',0)}")
print(f"  Pass 7  (structural strings):      {pass_counts.get('P7',0)}")
print(f"  {'─'*40}")
print(f"  TOTAL renames:                     {total}")
print(f"  Unnamed (fn_8...) fixed:           {total_unnamed_renamed}")
print(f"  Partial names fixed:               {total_partial_renamed}")
print(f"  Coverage (fn_8 → named):           {total_unnamed_renamed/max(len(unnamed),1)*100:.1f}%")
print(f"\n  Output:   {OUT}")
print(f"  Report:   {REPORT}")
print(f"{'='*60}")
print("\nReview auto_namer_v2_report.txt, then to apply:")
print("  cp config/434C4803/symbols_autonamed.txt config/434C4803/symbols.txt")
