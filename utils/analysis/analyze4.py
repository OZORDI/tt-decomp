with open('/Users/Ozordi/Downloads/tt-decomp/orig/434C4803/pong_xenon_final.pe', 'rb') as f:
    data = f.read()
e_lfanew=int.from_bytes(data[0x3C:0x40],'little')
coff_off=e_lfanew+4; opt_off=coff_off+20
image_base=int.from_bytes(data[opt_off+28:opt_off+32],'little')
num_sects=int.from_bytes(data[coff_off+2:coff_off+4],'little')
opt_hdr_size=int.from_bytes(data[coff_off+16:coff_off+18],'little')
sect_tbl=opt_off+opt_hdr_size
sections=[]
for i in range(num_sects):
    s=data[sect_tbl+i*40:sect_tbl+i*40+40]
    vsize=int.from_bytes(s[8:12],'little'); vaddr=int.from_bytes(s[12:16],'little'); raw_off=int.from_bytes(s[20:24],'little')
    sections.append((vaddr,vsize,raw_off))
sym_map={}; sym_by_name={}
with open('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols.txt') as f:
    for line in f:
        line=line.strip()
        for sect in ['.text','.rdata','.data']:
            marker='= '+sect+':0x'
            if marker in line:
                parts=line.split('=',1); name=parts[0].strip()
                try:
                    addr=int(parts[1].split(';')[0].strip().split(':')[1],16)
                    sym_map[addr]=name; sym_by_name[name]=addr
                except: pass
def vf(va):
    rel=va-image_base
    for vaddr,vsize,raw_off in sections:
        if vaddr<=rel<vaddr+vsize: return raw_off+(rel-vaddr)
def rbe(va):
    off=vf(va)
    if off is None or off+4>len(data): return None
    return int.from_bytes(data[off:off+4],'big')
def disasm(va,n=100):
    out=[]; cur=va
    for _ in range(n):
        w=rbe(cur)
        if w is None: break
        op=(w>>26)&0x3F; note='.long 0x'+format(w,'08X'); stop=False
        if op==18:
            li=w&0x3FFFFFC
            if li&0x2000000: li|=~0x3FFFFFF
            lk=w&1; tgt=cur+li; tn=sym_map.get(tgt,'0x'+format(tgt,'08X'))
            note=('bl ' if lk else 'b ')+tn
            if not lk: stop=True
        elif op==15: rd=(w>>21)&0x1F; note='lis r'+str(rd)+', 0x'+format(w&0xFFFF,'04X')
        elif op==14: rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='addi r'+str(rd)+', r'+str(ra)+', 0x'+format(w&0xFFFF,'04X')
        elif op==32: rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='lwz r'+str(rd)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==36: rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='stw r'+str(rs)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==38: rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='stb r'+str(rs)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==34: rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='lbz r'+str(rd)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==48: rd=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='lfs f'+str(rd)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif op==52: rs=(w>>21)&0x1F; ra=(w>>16)&0x1F; note='stfs f'+str(rs)+', 0x'+format(w&0xFFFF,'X')+'(r'+str(ra)+')'
        elif w==0x4E800020: note='blr'; stop=True
        elif w in(0x4E800420,0x4E800421): note='bctr'; stop=True
        elif op==16:
            bd=w&0xFFFC
            if bd&0x8000: bd|=~0xFFFF
            tgt=cur+bd; tn=sym_map.get(tgt,'0x'+format(tgt,'08X'))
            bo=(w>>21)&0x1F; c={4:'blt',12:'bge',8:'bgt',20:'b',16:'bdnz'}.get(bo,'bc'+str(bo))
            note=c+' '+tn
            if bo==20: stop=True
        lname=sym_map.get(cur,''); out.append((cur,lname,note)); cur+=4
        if stop: break
    return out
def show(name,n=100):
    va=sym_by_name.get(name,0)
    if not va: print('[not found: '+name+']'); return
    insns=disasm(va,n)
    print('\n'+'='*60); print('  '+name+' @ 0x'+format(va,'08X')); print('='*60)
    for c,ln,note in insns:
        ls=(ln+':') if ln else ''; print('  '+ls.ljust(26)+'0x'+format(c,'08X')+':  '+note)

# The sole Present/VdSwap caller
show('grc_B750', 120)
