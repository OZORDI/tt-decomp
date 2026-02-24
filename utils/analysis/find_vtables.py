# Find vtables that contain the known gameLoop_* functions
# by scanning .rdata for pointers to them

with open('/Users/Ozordi/Downloads/tt-decomp/orig/434C4803/pong_xenon_final.pe', 'rb') as f:
    data = f.read()

e_lfanew     = int.from_bytes(data[0x3C:0x40], 'little')
coff_off     = e_lfanew + 4
opt_off      = coff_off + 20
image_base   = int.from_bytes(data[opt_off+28:opt_off+32], 'little')
num_sects    = int.from_bytes(data[coff_off+2:coff_off+4], 'little')
opt_hdr_size = int.from_bytes(data[coff_off+16:coff_off+18], 'little')
sect_tbl = opt_off + opt_hdr_size
sections = []
for i in range(num_sects):
    s = data[sect_tbl + i*40 : sect_tbl + i*40 + 40]
    sname   = s[:8].rstrip(b'\x00').decode('ascii', errors='replace')
    vsize   = int.from_bytes(s[8:12],  'little')
    vaddr   = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((sname, vaddr, vsize, raw_off))

sym_map = {}
sym_by_name = {}
with open('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols.txt') as f:
    for line in f:
        line = line.strip()
        for sect in ['.text', '.rdata', '.data']:
            marker = '= ' + sect + ':0x'
            if marker in line:
                parts = line.split('=', 1)
                name = parts[0].strip()
                try:
                    addr = int(parts[1].split(';')[0].strip().split(':')[1], 16)
                    sym_map[addr] = name
                    sym_by_name[name] = addr
                except:
                    pass

def va_to_file(va):
    rel = va - image_base
    for sname, vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

def read_be(va):
    off = va_to_file(va)
    if off is None or off + 4 > len(data): return None
    return int.from_bytes(data[off:off+4], 'big')

# Target functions whose addresses should appear in a vtable
targets = {
    sym_by_name.get('gameLoop_PreRender',  0),
    sym_by_name.get('gameLoop_BeginRender',0),
    sym_by_name.get('gameLoop_Render',     0),
    sym_by_name.get('gameLoop_EndRender',  0),
    sym_by_name.get('gameLoop_PostRender', 0),
    sym_by_name.get('gameLoop_Present',    0),
}
targets.discard(0)
print('Looking for vtable entries containing:')
for t in sorted(targets):
    print('  0x' + format(t,'08X') + '  ' + sym_map.get(t,'?'))

# Scan .rdata for these pointers
rdata = next((s for s in sections if s[0] == '.rdata'), None)
if rdata:
    sname, vaddr_rd, vsize_rd, raw_off_rd = rdata
    print('\nScanning .rdata (VA=0x' + format(image_base+vaddr_rd,'08X') + ' size=0x' + format(vsize_rd,'X') + ') ...')
    for i in range(0, vsize_rd - 4, 4):
        off = raw_off_rd + i
        val = int.from_bytes(data[off:off+4], 'big')
        if val in targets:
            ptr_va = image_base + vaddr_rd + i
            fname = sym_map.get(val, '0x' + format(val,'08X'))
            print('  Found ' + fname + ' at rdata VA 0x' + format(ptr_va,'08X'))
            # Dump context: what's around it (show 16 entries before and after)
            print('  Context:')
            start = max(0, i - 64)
            for j in range(start, min(vsize_rd-4, i+80), 4):
                o = raw_off_rd + j
                v = int.from_bytes(data[o:o+4], 'big')
                va = image_base + vaddr_rd + j
                valid = 0x820C0000 <= v <= 0x825868FC
                n = sym_map.get(v, 'fn_' + format(v,'08X')) if valid else '.'
                marker = ' <== TARGET' if v in targets else ''
                print('    0x' + format(va,'08X') + ': 0x' + format(v,'08X') + '  ' + n + marker)
            print()
