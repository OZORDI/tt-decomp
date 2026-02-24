import os
import sys

try:
    from capstone import *
    from capstone.ppc import *
except ImportError:
    sys.exit(1)

BASE = '/Users/Ozordi/Downloads/tt-decomp'
XEX = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v12.txt')

targets = {
    'util_8D60': 0x82238D60,
    'memcpy': None,
    'memset': None,
    'atSingleton_Find_90D0': 0x820F90D0,
    'PongNetGameModeCoordinator_vfn_6': None,
    '__savegprlr_14': None,
    'RtlEnterCriticalSection': None,
    'RtlLeaveCriticalSection': None
}

with open(SYMS, 'r') as f:
    for line in f:
        if '= .text:0x' in line:
            name = line.split('=')[0].strip()
            addr_str = line.split('=')[1].split(';')[0].strip().split(':')[1]
            addr = int(addr_str, 16)
            
            for k in targets.keys():
                if name == k:
                    targets[k] = addr
            
            if name.startswith('rage_free_') and 'rage_free' not in targets:
                targets['rage_free'] = addr
            if name.startswith('memcpy') and 'memcpy' not in targets:
                targets['memcpy'] = addr
            if name.startswith('memset') and 'memset' not in targets:
                targets['memset'] = addr

data = open(XEX, 'rb').read()
tvaddr = 0x820C0000; tvsize = 0x82586A00 - 0x820C0000; traw = 0x4000
rvaddr = 0x82000600; rraw = 0x600; rvsize = 0x8209B000 - 0x82000600

md = Cs(CS_ARCH_PPC, CS_MODE_64 | CS_MODE_BIG_ENDIAN)
md.detail = True

def read_be32(va):
    if tvaddr <= va < tvaddr + tvsize:
        off = traw + (va - tvaddr)
        return int.from_bytes(data[off:off+4], 'big')
    if rvaddr <= va < rvaddr + rvsize:
        off = rraw + (va - rvaddr)
        return int.from_bytes(data[off:off+4], 'big')
    return None

def disasm_func(addr, max_size=0x200):
    offset = traw + (addr - tvaddr)
    # Stop at blr (0x4e800020)
    for end_off in range(offset, offset + max_size, 4):
        if data[end_off:end_off+4] == b'N\\x80\\x00\\x20': # blr
            code = data[offset : end_off + 4]
            break
    else:
        code = data[offset : offset + max_size]
        
    return list(md.disasm(code, addr))

with open('hotspots_analysis.txt', 'w') as out:
    for name, addr in targets.items():
        if not addr: continue
        out.write(f"=== {name} @ 0x{addr:08X} ===\\n")
        insns = disasm_func(addr)
        for i in insns:
            out.write(f"0x{i.address:x}:\\t{i.mnemonic}\\t{i.op_str}\\n")
        
        # Address tracking logic
        out.write(" --- Math Analysis ---\\n")
        regs = {}
        for i in insns:
            if i.mnemonic == 'lis':
                r = int(i.op_str.split(',')[0].strip().replace('r',''))
                imm = int(i.op_str.split(',')[1].strip(), 16)
                if imm >= 0x8000: imm -= 0x10000
                regs[r] = (imm << 16)
            elif i.mnemonic in ('addi', 'addis'):
                parts = [p.strip() for p in i.op_str.split(',')]
                r_dest = int(parts[0].replace('r',''))
                r_src = int(parts[1].replace('r',''))
                if parts[2].startswith('-0x'):
                    imm = -int(parts[2][3:], 16)
                elif parts[2].startswith('0x'):
                    imm = int(parts[2][2:], 16)
                else:
                    imm = int(parts[2])
                
                if r_src in regs:
                    val = regs[r_src] + (imm if i.mnemonic == 'addi' else (imm << 16))
                    val &= 0xFFFFFFFF
                    regs[r_dest] = val
                    if 0x82000000 <= val < 0x83000000:
                        out.write(f"  [Math] r{r_dest} computed as 0x{val:08X}\\n")
                        # Try reading from it if it's in rdata
                        read_val = read_be32(val)
                        if read_val:
                            out.write(f"         > Points to value 0x{read_val:08X}\\n")
            elif i.mnemonic == 'lwz':
                pass # can trace more if needed
        out.write("\\n")

print("Analysis written to hotspots_analysis.txt")
