import os, struct

BASE = os.path.dirname(os.path.abspath(__file__))
XEX  = os.path.join(BASE, 'orig/434C4803/pong_xenon_final.pe')
SYMS = os.path.join(BASE, 'config/434C4803/symbols.txt')

PDATA_OFF  = 0x09B000
PDATA_SIZE = 0x167F8

def main():
    if not os.path.exists(XEX): return
    with open(XEX, 'rb') as f:
        f.seek(PDATA_OFF)
        pdata_raw = f.read(PDATA_SIZE)

    sym_meta = {}
    with open(SYMS, 'r') as f:
        for line in f:
            if '= .text:0x' in line:
                try:
                    addr = int(line.split('=')[1].split(';')[0].strip().split(':')[1], 16)
                    size = 0
                    if 'size:0x' in line:
                        size = int(line.split('size:0x')[1].split()[0], 16)
                    sym_meta[addr] = size
                except: pass

    print("Parsing .pdata (12-byte entries, Big Endian VA)...")
    num_entries = PDATA_SIZE // 12
    missing = []
    
    # Let's try to interpret the 12 bytes differently if 4-byte doesn't work.
    # From od: 38000c82 03220040 c0000c82 ...
    # 0x820C0038 is a function start. 38000c82 is 0x820C0038 but little-endian.
    # So the entries ARE little-endian, but they are NOT (begin, end, unwind).
    # 0x820C0038 (begin), 0x40002203 (?), 0x820C00C0 (next begin?)
    
    for i in range(num_entries):
        entry = pdata_raw[i*12 : (i+1)*12]
        if len(entry) < 12: break
        # If it's little-endian:
        begin_va, val2, val3 = struct.unpack("<III", entry)
        
        # Check if begin_va looks like a valid text address
        if 0x820C0000 <= begin_va <= 0x82580000:
            if begin_va not in sym_meta:
                missing.append(begin_va)
                
    print(f"Found {len(missing)} VAs in .pdata not in symbols.txt")
    for va in sorted(missing)[:20]:
        print(f"  Missing: 0x{va:08X}")

if __name__ == "__main__":
    main()
