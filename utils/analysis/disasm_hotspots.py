import os
import sys

try:
    from capstone import *
    from capstone.ppc import *
except ImportError:
    print("Please install capstone: pip install capstone")
    sys.exit(1)

BASE = '/Users/Ozordi/Downloads/tt-decomp'
XEX = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v12.txt')

hotspot_names = [
    'util_8D60',
    'memcpy',
    'memset',
    'atSingleton_Find_90D0',
    'PongNetGameModeCoordinator_vfn_6',
    'RtlEnterCriticalSection',
    'RtlLeaveCriticalSection'
]

targets = {}
with open(SYMS, 'r') as f:
    for line in f:
        if '= .text:0x' in line:
            name = line.split('=')[0].strip()
            # Match exact or prefixes like rage_free
            if name in hotspot_names or name.startswith('rage_free_'):
                addr_str = line.split('=')[1].split(';')[0].strip().split(':')[1]
                addr = int(addr_str, 16)
                
                size = 0
                if "size:0x" in line:
                    size_str = line[line.find("size:0x")+7:].strip().split()[0]
                    size = int(size_str, 16)
                
                # Only grab one rage_free as an example
                if name.startswith('rage_free_'):
                    if any(t.startswith('rage_free_') for t in targets.values()):
                        continue
                targets[addr] = (name, size)

print(f"Testing binary size and reading...")
data = open(XEX, 'rb').read()
tvaddr = 0x820C0000; tvsize = 0x82586A00 - 0x820C0000; traw = 0x4000

md = Cs(CS_ARCH_PPC, CS_MODE_64 | CS_MODE_BIG_ENDIAN)
md.detail = True

with open('hotspots_disasm.txt', 'w') as out:
    for addr, (name, size) in targets.items():
        out.write(f"=== {name} (0x{addr:08X}, size 0x{size:X}) ===\n")
        
        if size == 0:
            out.write("ERROR: Unknown size\n\n")
            continue
            
        offset = traw + (addr - tvaddr)
        code = data[offset : offset + size]
        
        coffset = 0
        while coffset < size:
            instrs = list(md.disasm(code[coffset:size], addr + coffset))
            if instrs:
                for i in instrs:
                    out.write(f"0x{i.address:x}:\t{i.mnemonic}\t{i.op_str}\n")
                last = instrs[-1]
                coffset = (last.address - addr) + last.size
            else:
                out.write(f"0x{(addr + coffset):x}:\t.long 0x{int.from_bytes(code[coffset:coffset+4], 'big'):08X}\n")
                coffset += 4
        out.write("\n")

print("Wrote hotspots_disasm.txt")
