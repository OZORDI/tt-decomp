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
                except: pass

def va_to_file(va):
    rel = va - image_base
    for vaddr, vsize, raw_off in sections:
        if vaddr <= rel < vaddr + vsize:
            return raw_off + (rel - vaddr)
    return None

def rbe(va):
    off = va_to_file(va)
    if off is None or off+4 > len(data): return None
    return int.from_bytes(data[off:off+4], 'big')

def disasm(start_va, count=80):
    out = []
    va = start_va
    for _ in range(count):
        w = rbe(va)
        if w is None: break
        op = (w>>26)&0x3F
        note = '.long 0x'+format(w,'08X')
        stop = False
        if op==18:
            li=w&0x3FFFFFC
            if li&0x2000000: li|=~0x3FFFFFF
            lk=w&1
            tgt=va+li
            tn=sym_map.get(tgt,'0x'+format(tgt,'08X'))
            note='bl '+tn if lk else 'b '+tn
            if not lk: stop=True
        elif op==15:
            rd=(w>>21)&0x1F; note='lis r'+str(rd)+', 0x'+format(w&0xFFFF,'04X')
        elif op==14:
            rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='addi r'+str(rd)+', r'+str(ra)+', 0x'+format(w&0xFFFF,'04X')
        elif op==13:
            rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='addic. r'+str(rd)+', r'+str(ra)+', '+str(w&0xFFFF)
        elif op==32:
            rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='lwz r'+str(rd)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==36:
            rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='stw r'+str(rs)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==38:
            rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='stb r'+str(rs)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==34:
            rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='lbz r'+str(rd)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==40:
            rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; d=w&0xFFFF; note='lhz r'+str(rd)+', 0x'+format(d,'X')+'(r'+str(ra)+')'
        elif op==48:
            rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='lfs f'+str(rd)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==52:
            rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='stfs f'+str(rs)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif w==0x4E800020: note='blr'; stop=True
        elif w in(0x4E800420,0x4E800421): note='bctr'; stop=True
        elif op==16:
            bd=w&0xFFFC
            if bd&0x8000: bd|=~0xFFFF
            tgt=va+bd
            tn=sym_map.get(tgt,'0x'+format(tgt,'08X'))
            bo=(w>>21)&0x1F
            c={4:'blt',12:'bge',8:'bgt',20:'b',16:'bdnz',5:'blt',13:'ble'}.get(bo,'bc'+str(bo))
            note=c+' '+tn
            if bo==20: stop=True
        lname=sym_map.get(va,'')
        out.append((va,lname,note))
        va+=4
        if stop: break
    return out

def show(name, count=80):
    va = sym_by_name.get(name,0)
    if not va:
        print('[not found: '+name+']'); return
    insns = disasm(va, count)
    print('\n'+'='*60)
    print('  '+name+' @ 0x'+format(va,'08X'))
    print('='*60)
    for cur,lname,note in insns:
        lstr=(lname+':') if lname else ''
        print('  '+lstr.ljust(26)+'0x'+format(cur,'08X')+':  '+note)

# VdSwap wrapper - the actual present call
show('VdSwap')

# Find what calls VdSwap
vdswap_va = sym_by_name.get('VdSwap', 0)
__imp_vdswap = sym_by_name.get('__imp_VdSwap', 0)
print('\n=== Scanning .text for callers of VdSwap (0x'+format(vdswap_va,'08X')+') ===')
text_sec = None
for i in range(num_sects):
    s = data[sect_tbl + i*40 : sect_tbl + i*40 + 40]
    sname = s[:8].rstrip(b'\x00').decode('ascii','replace')
    if sname == '.text':
        vaddr   = int.from_bytes(s[12:16], 'little')
        vsize   = int.from_bytes(s[8:12],  'little')
        raw_off = int.from_bytes(s[20:24], 'little')
        text_sec = (vaddr, vsize, raw_off)
        break

if text_sec and vdswap_va:
    vaddr, vsize, raw_off = text_sec
    for i in range(0, vsize-4, 4):
        w = int.from_bytes(data[raw_off+i:raw_off+i+4], 'big')
        if (w>>26)&0x3F == 18 and (w&1):  # bl
            cur_va = image_base + vaddr + i
            li=w&0x3FFFFFC
            if li&0x2000000: li|=~0x3FFFFFF
            tgt = cur_va + li
            if tgt == vdswap_va:
                caller_name = sym_map.get(cur_va, '?')
                # find containing function by scanning backwards
                fn_va = cur_va
                for j in range(1, 300):
                    check = cur_va - j*4
                    if check in sym_map:
                        fn_va = check
                        break
                fn_name = sym_map.get(fn_va, 'fn_'+format(fn_va,'08X'))
                print('  Caller: '+fn_name+' @ 0x'+format(fn_va,'08X')+'  (bl at 0x'+format(cur_va,'08X')+')')

# Disasm rage_render_default more fully
show('rage_render_default', 120)
