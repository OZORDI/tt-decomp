import pefile
import os

BASE = '/Users/Ozordi/Downloads/tt-decomp'
XEX = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v12.txt')

def get_raw_offset(pe, va):
    for sec in pe.sections:
        if sec.VirtualAddress <= va < sec.VirtualAddress + sec.Misc_VirtualSize:
            return sec.PointerToRawData + (va - sec.VirtualAddress)
    return None

def main():
    data = open(XEX, 'rb').read()
    pe = pefile.PE(XEX)
    pe_base = pe.OPTIONAL_HEADER.ImageBase
    
    fns = set()
    fn_names = {}
    with open(SYMS, 'r') as f:
        for line in f:
            if '= .text:0x' in line:
                name = line.split('=')[0].strip()
                addr = int(line.split('=')[1].split(';')[0].split(':')[1], 16)
                fns.add(addr)
                fn_names[addr] = name

    print(f"Total labeled text functions: {len(fns)}")
    
    # Text section info (we still need this for iterating instructions)
    for sec in pe.sections:
        if sec.Name.strip(b'\x00') == b'.text':
            traw = sec.PointerToRawData
            tvaddr = pe_base + sec.VirtualAddress
            tvsize = sec.Misc_VirtualSize
            break
            
    has_bl_in = set()
    has_b_tail_in = set()
    lis_addi_ptrs = set()
    
    # Pass 1: Code section scanning (BL, B, lis/addi)
    for i in range(0, tvsize, 4):
        w = int.from_bytes(data[traw+i:traw+i+4], 'big')
        op = (w >> 26) & 0x3F
        
        # Branch instruction (18 = b, ba, bl, bla)
        if op == 18:
            li = w & 0x3FFFFFC
            if li & 0x2000000: 
                li -= 0x4000000
            
            p_AA = (w >> 1) & 1
            p_LK = w & 1
            
            tgt = li if p_AA else (tvaddr + i + li)
            
            if tgt in fns:
                if p_LK:
                    has_bl_in.add(tgt)
                else:
                    has_b_tail_in.add(tgt)
                    
            if len(has_bl_in) + len(has_b_tail_in) < 10 and tgt > 0x82000000:
                print(f"DEBUG: pc=0x{tvaddr+i:08X} li={li:X} AA={p_AA} LK={p_LK} tgt=0x{tgt:08X} in_fns={tgt in fns}")
                    
        # Check for lis / addi pair targeting functions
        if op == 15: # lis rD, SIMM
            rD = (w >> 21) & 0x1F
            simm_hi = w & 0xFFFF
            
            # Check next instruction for addi or ori
            if i + 4 < tvsize:
                nw = int.from_bytes(data[traw+i+4:traw+i+8], 'big')
                if (nw >> 26) == 14: # addi
                    nrD = (nw >> 21) & 0x1F
                    nrA = (nw >> 16) & 0x1F
                    if nrA == rD:
                        hi = simm_hi
                        lo = nw & 0xFFFF
                        if lo & 0x8000: hi -= 1
                        val = ((hi & 0xFFFF) << 16) | lo
                        
                        if val in fns:
                            lis_addi_ptrs.add(val)
    
    data_ptrs = set()
    # Pass 2: Data sections (.rdata, .data)
    for sec in pe.sections:
        name = sec.Name.strip(b'\x00')
        if name in [b'.rdata', b'.data', b'.embsec_', b'BINKDATA', b'BINK']:
            s_raw = sec.PointerToRawData
            s_size = sec.Misc_VirtualSize
            for i in range(0, s_size - 3, 4):
                w = int.from_bytes(data[s_raw+i:s_raw+i+4], 'big')
                if w in fns:
                    data_ptrs.add(w)
                
    # Now analyze the functions WITH NO incoming BL
    no_bl = fns - has_bl_in
    print(f"Fns with NO incoming BL: {len(no_bl)}")
    
    categorized = {
        'tail_call': 0,
        'data_ptr': 0,      # vtable or globals
        'code_ptr': 0,      # lis/addi loaded
        'completely_isolated': 0
    }
    
    isolated_fns = []
    
    for f in no_bl:
        cat = False
        if f in has_b_tail_in:
            categorized['tail_call'] += 1
            cat = True
        if f in data_ptrs:
            categorized['data_ptr'] += 1
            cat = True
        if f in lis_addi_ptrs:
            categorized['code_ptr'] += 1
            cat = True
            
        if not cat:
            categorized['completely_isolated'] += 1
            isolated_fns.append(f)
            
    print("\nCategorization of the NO-BL Functions:")
    print(f"1. Reachable via Tail Call (B instead of BL): {categorized['tail_call']}")
    print(f"2. Present in Data Sections (vtables, struct fn-ptrs): {categorized['data_ptr']}")
    print(f"3. Loaded directly into registers (lis/addi): {categorized['code_ptr']}")
    print(f"4. Completely Isolated (No obvious references): {categorized['completely_isolated']}")
    
    print("\nSample completely isolated functions:")
    for f in isolated_fns[:10]:
        print(f" - 0x{f:08X}: {fn_names[f]}")

if __name__ == '__main__':
    main()
