#!/usr/bin/env python3
import os, struct, bisect
from collections import defaultdict

BASE = os.path.dirname(os.path.abspath(__file__))
XEX  = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols.txt')

def main():
    print("Loading binary...")
    with open(XEX, 'rb') as f:
        data = f.read()
        
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

    text_sec  = next(s for s in sections if s[0]=='.text')
    rdata_sec = next(s for s in sections if s[0]=='.rdata')
    _, tvaddr, tvsize, traw = text_sec
    _, rvaddr, rvsize, rraw = rdata_sec
    
    text_va_lo = image_base + tvaddr
    text_va_hi = text_va_lo + tvsize

    # Load symbols
    sym_map = {}
    with open(SYMS) as f:
        for line in f:
            if '= .text:0x' in line:
                try:
                    parts = line.split('=')
                    name = parts[0].strip()
                    addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                    sym_map[addr] = name
                except: pass
    
    fn_addrs = sorted(sym_map.keys())
    
    def get_owning_fn(va):
        idx = bisect.bisect_right(fn_addrs, va) - 1
        if idx >= 0:
            return fn_addrs[idx]
        return None

    # Find Vtables
    print("Scanning for vtables...")
    vtables = [] # (va, length)
    rdata_chunk = data[rraw:rraw+rvsize]
    i = 0
    while i < rvsize - 12:
        run_len = 0
        j = i
        null_count = 0
        while j + 4 <= rvsize:
            ptr = int.from_bytes(rdata_chunk[j:j+4], 'big')
            if text_va_lo <= ptr < text_va_hi:
                run_len += 1
                null_count = 0
                j += 4
            elif ptr == 0:
                if null_count < 2:
                    null_count += 1
                    run_len += 1
                    j += 4
                else:
                    break
            else:
                break
        
        while run_len > 0 and int.from_bytes(rdata_chunk[i+(run_len-1)*4:i+run_len*4], 'big') == 0:
            run_len -= 1
            
        if run_len >= 3:
            vt_va = image_base + rvaddr + i
            vtables.append((vt_va, run_len))
            i = j
        else:
            i += 4
            
    print(f"Found {len(vtables)} vtable candidates.")
    vt_set = set(v[0] for v in vtables)

    print("Analyzing instructions for vtable stores...")
    regs = {} 
    ctor_candidates = [] # (fn_va, vt_va, reg_stored_to)
    
    for i in range(0, tvsize, 4):
        w = int.from_bytes(data[traw+i:traw+i+4], 'big')
        op = (w >> 26) & 0x3F
        rd = (w >> 21) & 0x1F
        ra = (w >> 16) & 0x1F
        imm = w & 0xFFFF
        
        # LIS rD, imm
        if op == 15 and ra == 0: 
            regs[rd] = (imm << 16, i)
            
        # ADDI rD, rA, imm
        elif op == 14: 
            if ra in regs:
                base_val, base_off = regs[ra]
                if i - base_off < 64: # Must be close
                    val = base_val + (imm if imm < 0x8000 else imm - 0x10000)
                    regs[rd] = (val, i)
                    
        # STW rS, d(rA)
        elif op == 36: 
            rs = (w >> 21) & 0x1F
            if rs in regs:
                val, set_off = regs[rs]
                if i - set_off < 64:
                    if val in vt_set:
                        # Found a store of a vtable pointer!
                        d = imm if imm < 0x8000 else imm - 0x10000
                        target_reg = ra
                        
                        # Only care about stores to offset 0 (vtable ptr is at 0x0)
                        if d == 0:
                            curr_fn = get_owning_fn(image_base + tvaddr + i)
                            if curr_fn:
                                ctor_candidates.append((curr_fn, val, target_reg))

    print(f"Found {len(ctor_candidates)} potential vtable stores.")
    
    # Analyze results
    vt_to_fn = defaultdict(list)
    for fn_va, vt_va, reg in ctor_candidates:
        vt_to_fn[vt_va].append((fn_va, reg))
        
    print("\n=== High Confidence Constructors (store to 0(r3)) ===")
    count = 0
    # Sort by vtable address
    for vt_va in sorted(vt_to_fn.keys()):
        refs = vt_to_fn[vt_va]
        # Look for r3 stores (this pointer)
        r3_refs = [fn for fn, reg in refs if reg == 3]
        if r3_refs:
            # First function is likely the constructor
            fn = r3_refs[0]
            fn_name = sym_map.get(fn, f"fn_{fn:08X}")
            print(f"  Vtable 0x{vt_va:08X} -> Constructor {fn_name} (0x{fn:08X})")
            count += 1
            
    print(f"\nTotal high-confidence constructors: {count}")

if __name__ == '__main__':
    main()
