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
    vsize   = int.from_bytes(s[8:12],  'little')
    vaddr   = int.from_bytes(s[12:16], 'little')
    raw_off = int.from_bytes(s[20:24], 'little')
    sections.append((vaddr, vsize, raw_off))

sym_map = {}
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
                except:
                    pass

def va_to_file(va):
    rel = va - image_base
    for vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

def read_be(va):
    off = va_to_file(va)
    if off is None or off + 4 > len(data): return None
    return int.from_bytes(data[off:off+4], 'big')

def dump_vtable(va, label, num=40):
    print('\n=== ' + label + ' @ 0x' + format(va, '08X') + ' ===')
    for i in range(num):
        fn_ptr = read_be(va + i * 4)
        if fn_ptr is None:
            break
        valid = 0x820C0000 <= fn_ptr <= 0x825868FC
        if valid:
            name = sym_map.get(fn_ptr, 'fn_' + format(fn_ptr, '08X'))
            markers = ['render','begin','end','present','pre','post','update','draw','scene','clear','init']
            tag = ' <<<' if any(m in name.lower() for m in markers) else ''
            print('  [' + str(i).rjust(2) + '] +0x' + format(i*4, '02X') + ': 0x' + format(fn_ptr,'08X') + '  ' + name + tag)
        else:
            try:
                ascii_val = bytes.fromhex(format(fn_ptr, '08X')).decode('ascii')
            except:
                ascii_val = '????'
            print('  [' + str(i).rjust(2) + '] +0x' + format(i*4, '02X') + ': 0x' + format(fn_ptr,'08X') + '  (data) "' + ascii_val + '"')
            if i > 4:
                break

dump_vtable(0x82069154, 'gameLoop sub-vtable (on +0x18 subobj)')
dump_vtable(0x82034A68, 'rage_game_obj vtable (lis 0x8203 + addi 0x4A68)')
dump_vtable(0x82027408, 'vtable at lis 0x8202 + 0x7408')
dump_vtable(0x82027414, 'vtable at lis 0x8202 + 0x7414')
