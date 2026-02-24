import sys

with open('auto_namer_v12.py', 'r') as f:
    code = f.read()

# 1. Update PASS_CONFIDENCE
old_conf = """    'P16': 72,
    'P17': 42,
}"""
new_conf = """    'P16': 72,
    'P17': 42,
    # New v12
    'P18': 45,
    'P19': 95, 'P19s': 85,
    'P20': 75,
}"""
code = code.replace(old_conf, new_conf)

# 2. Insert P19 after P14
p14_end = """print(f"  P14: {p14} vtable slots renamed ({p14_fn8} were fn_8)")\n"""
p19_code = """
# ── PASS 19: Atexit / Static Init Stubs ───────────────────────────────────────
print("\\n=== PASS 19: Atexit / Static Init Stubs ===")
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
            
            if op == 15 and rd == 3 and ra == 0:  # lis r3, HI
                hi = imm if imm < 0x8000 else imm - 0x10000
                ins2 = int.from_bytes(data[raw+off+4 : raw+off+8], 'big')
                op2 = (ins2 >> 26) & 0x3F; rd2 = (ins2 >> 21) & 0x1F; ra2 = (ins2 >> 16) & 0x1F; imm2 = ins2 & 0xFFFF
                if op2 == 14 and rd2 == 3 and ra2 == 3:  # addi r3, r3, LO
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
"""
code = code.replace(p14_end, p14_end + p19_code)

# 3. Insert P20 right after P6 re-run
p6_end = """print(f"  P6 re-run: {p6} strict, {p6r} relaxed")\n"""
p20_code = """
# ── PASS 20: Generic Rename + Cascade ─────────────────────────────────────────
print("\\n=== PASS 20: Generic rename + cascade ===")
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
"""
code = code.replace(p6_end, p6_end + p20_code)

# 4. Insert P18 right after P15
p15_end = """print(f"  P15: {p15}")\n"""
p18_code = """
# ── PASS 18: V12 BFS Re-convergence ───────────────────────────────────────────
print("\\n=== PASS 18: V12 BFS Re-convergence ===")
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
"""
code = code.replace(p15_end, p15_end + p18_code)


# 5. Update Report prints
old_print_tags = """for tag in ['SEED', 'P10', 'P10r', 'P11', 'P4f', 'P4fw', 'P4rh',
            'P6', 'P6r', 'P12', 'P12fw', 'P12rh', 'P13', 'P14', 'P15', 'P17']:"""
new_print_tags = """for tag in ['SEED', 'P10', 'P10r', 'P11', 'P4f', 'P4fw', 'P4rh',
            'P6', 'P6r', 'P12', 'P12fw', 'P12rh', 'P13', 'P14', 'P15', 'P17',
            'P18', 'P19', 'P19s', 'P20']:"""
code = code.replace(old_print_tags, new_print_tags)

old_report_header = """    f.write(f"auto_namer_v11 Results\\n{'='*70}\\n\\n")"""
new_report_header = """    f.write(f"auto_namer_v12 Results\\n{'='*70}\\n\\n")"""
code = code.replace(old_report_header, new_report_header)

old_report_lines = """    f.write(f"New passes summary:\\n")
    f.write(f"  P14 (RTTI vtable completor):   {pc.get('P14',0)} renames ({p14_fn8} were fn_8)\\n")
    f.write(f"  P12 (extended BFS):             {pc.get('P12',0)+pc.get('P12fw',0)+pc.get('P12rh',0)} fn_8\\n")
    f.write(f"  P15 (island BFS):               {pc.get('P15',0)} fn_8\\n")
    f.write(f"  P13 (string literal):           {pc.get('P13',0)} fn_8\\n")
    f.write(f"  P17 (spatial proximity):        {pc.get('P17',0)} fn_8\\n")"""
new_report_lines = """    f.write(f"New passes summary:\\n")
    p20_eval = pc.get('P20', 0)
    # Number of renamed generics might just be stored in a global variable p20 if scope permits
    f.write(f"  P18 (V12 Reconvergence):        {pc.get('P18',0)} fn_8\\n")
    f.write(f"  P19 (Atexit/Static Init):       {pc.get('P19',0)} semantic, {pc.get('P19s',0)} generic fn_8\\n")
    f.write(f"  P20 (Generic cascade):          {p20_eval} items tagged P20 (generics + fn_8 cascade)\\n")
    f.write(f"  P14 (RTTI vtable completor):    {pc.get('P14',0)} renames ({p14_fn8} were fn_8)\\n")
    f.write(f"  P12 (extended BFS):             {pc.get('P12',0)+pc.get('P12fw',0)+pc.get('P12rh',0)} fn_8\\n")
    f.write(f"  P15 (island BFS):               {pc.get('P15',0)} fn_8\\n")
    f.write(f"  P13 (string literal):           {pc.get('P13',0)} fn_8\\n")
    f.write(f"  P17 (spatial proximity):        {pc.get('P17',0)} fn_8\\n")"""
code = code.replace(old_report_lines, new_report_lines)

with open('auto_namer_v12.py', 'w') as f:
    f.write(code)
print("done! auto_namer_v12.py patched.")
