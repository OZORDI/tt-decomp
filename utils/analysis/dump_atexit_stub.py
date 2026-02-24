import os, struct
BASE = '/Users/Ozordi/Downloads/tt-decomp'
XEX = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v11.txt')

data = open(XEX, 'rb').read()
e_lfanew = int.from_bytes(data[0x3C:0x40], 'little')
coff_off = e_lfanew + 4
opt_off = coff_off + 20
image_base = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18], 'little')
num_sects = int.from_bytes(data[coff_off+2:coff_off+4], 'little')
sect_tbl = opt_off + opt_hdr_size
sections = []
for i in range(num_sects):
    s = data[sect_tbl+i*40 : sect_tbl+i*40+40]
    sname = s[:8].rstrip(b'\x00').decode('ascii', 'replace')
    vsize = int.from_bytes(s[8:12], 'little')
    vaddr = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((sname, image_base + vaddr, vsize, raw_off))

def va_to_file(va):
    for _, abs_va, vsize, raw_off in sections:
        if abs_va <= va < abs_va + vsize:
            return raw_off + (va - abs_va)
    return None

fns = []
with open(SYMS) as f:
    for line in f:
        if 'fn_8257' in line and 'size:0xC' in line:
            parts = line.split('=')
            name = parts[0].strip()
            addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
            fns.append(addr)

print("Found", len(fns), "tiny functions in 0x8257BBE0+ range")

for va in fns[:3]:
    raw = va_to_file(va)
    print(f"\\nFunction at 0x{va:X}:")
    for i in range(3):
        w = int.from_bytes(data[raw+i*4:raw+i*4+4], 'big')
        op = (w >> 26) & 0x3F; rd = (w >> 21) & 0x1F; ra = (w >> 16) & 0x1F; imm = w & 0xFFFF
        print(f"  ins{i}: {w:08X} | op={op} rd={rd} ra={ra} imm={imm:04X}")
