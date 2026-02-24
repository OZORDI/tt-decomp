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
    for vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

def rbe(va):
    off = va_to_file(va)
    if off is None or off + 4 > len(data): return None
    return int.from_bytes(data[off:off+4], 'big')

def disasm_fn(start_va, label="", max_insns=150):
    lines = []
    visited = set()
    queue = [start_va]
    while queue:
        va = queue.pop(0)
        if va in visited: continue
        visited.add(va)
        for i in range(max_insns):
            cur = va + i*4
            if cur in visited and i > 0: break
            w = rbe(cur)
            if w is None: break
            op = (w >> 26) & 0x3F
            note = '.long 0x' + format(w,'08X')
            terminate = False
            follow = None

            if op == 18:
                li = w & 0x3FFFFFC
                if li & 0x2000000: li |= ~0x3FFFFFF
                lk = w & 1; aa = (w>>1)&1
                tgt = li if aa else cur + li
                tn = sym_map.get(tgt, '0x'+format(tgt,'08X'))
                note = 'b' + ('l' if lk else '') + ' ' + tn
                if not lk: follow = tgt; terminate = True
            elif op == 15:
                rd=(w>>21)&0x1F; imm=w&0xFFFF
                note = 'lis r' + str(rd) + ', 0x' + format(imm,'04X')
            elif op == 14:
                rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; imm=w&0xFFFF
                note = 'addi r' + str(rd) + ', r' + str(ra) + ', 0x' + format(imm,'04X')
            elif op == 32:
                rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; d=w&0xFFFF
                note = 'lwz r' + str(rd) + ', 0x' + format(d,'X') + '(r' + str(ra) + ')'
            elif op == 36:
                rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; d=w&0xFFFF
                note = 'stw r' + str(rs) + ', 0x' + format(d,'X') + '(r' + str(ra) + ')'
            elif op == 38:
                rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; d=w&0xFFFF
                note = 'stb r' + str(rs) + ', 0x' + format(d,'X') + '(r' + str(ra) + ')'
            elif op == 34:
                rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; d=w&0xFFFF
                note = 'lbz r' + str(rd) + ', 0x' + format(d,'X') + '(r' + str(ra) + ')'
            elif w == 0x4E800020:
                note = 'blr'; terminate = True
            elif w in (0x4E800420, 0x4E800421):
                note = 'bctr  ; vtable/indirect call'; terminate = True
            elif op == 16:
                bd = w & 0xFFFC
                if bd & 0x8000: bd |= ~0xFFFF
                tgt = cur + bd
                tn = sym_map.get(tgt, '0x'+format(tgt,'08X'))
                bo=(w>>21)&0x1F
                conds = {4:'blt',12:'bge',8:'bgt',20:'b',16:'bdnz'}
                cname = conds.get(bo, 'bc ' + str(bo))
                note = cname + ' ' + tn
                if bo == 20: follow = tgt; terminate = True
                else: queue.append(tgt)

            lname = sym_map.get(cur,'')
            lines.append((cur, lname, note))
            if terminate: break
            if follow: queue.append(follow); break

    lines.sort(key=lambda x: x[0])
    # deduplicate
    seen = set()
    out = []
    for item in lines:
        if item[0] not in seen:
            seen.add(item[0])
            out.append(item)
    return out

def print_fn(name_or_va, max_insns=150):
    if isinstance(name_or_va, str):
        va = sym_by_name.get(name_or_va, 0)
        label = name_or_va
    else:
        va = name_or_va
        label = sym_map.get(va, 'fn_' + format(va,'08X'))
    if not va:
        print('[not found: ' + str(name_or_va) + ']')
        return
    insns = disasm_fn(va, label, max_insns)
    print('\n' + '='*60)
    print('  ' + label + ' @ 0x' + format(va,'08X') + '  (' + str(len(insns)) + ' insns)')
    print('='*60)
    for cur, lname, note in insns:
        lstr = (lname + ':') if lname else ''
        print('  ' + lstr.ljust(28) + '  0x' + format(cur,'08X') + ':  ' + note)

# Find VdSwap / present callers
print("=== Searching for VdSwap / swap references ===")
with open('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols.txt') as f:
    for line in f:
        if any(k in line.lower() for k in ['vdswap','swap','present','flip']):
            print('  ' + line.strip())

print_fn('gameLoop_Update')
print_fn('game_vt74B4_8')   # physics A
print_fn('game_vt74B4_9')   # physics B
