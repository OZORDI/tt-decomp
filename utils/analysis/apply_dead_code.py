import os

BASE = '/Users/Ozordi/Downloads/tt-decomp'
SYMS_IN = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v12.txt')
SYMS_OUT = os.path.join(BASE, 'config/434C4803/symbols_autonamed_v13_deadcode.txt')
DEAD_CODE_FILE = os.path.join(BASE, 'dead_code_analysis.txt')

dead_addrs = set()
with open(DEAD_CODE_FILE, 'r') as f:
    for line in f:
        if line.startswith('0x'):
            parts = line.split()
            addr = int(parts[0], 16)
            dead_addrs.add(addr)

counts = 0
with open(SYMS_IN, 'r') as fi, open(SYMS_OUT, 'w') as fo:
    for line in fi:
        if '= .text:0x' in line:
            addr_str = line.split('=')[1].split(';')[0].strip().split(':')[1]
            addr = int(addr_str, 16)
            
            if addr in dead_addrs:
                # Replace 'fn_8' with 'dead_code'
                name_part = line.split('=')[0].strip()
                if name_part.startswith('fn_8'):
                    new_name = f"dead_code_{addr:04X}"
                    # retain size
                    size_part = ""
                    if "size:0x" in line:
                        size_part = line[line.find("size:0x"):]
                    line = f"{new_name} = .text:0x{addr:08X}; // {size_part}\\n"
                    counts += 1
        fo.write(line)

print(f"Applied 'dead_code_' prefix to {counts} functions.")
print(f"Wrote {SYMS_OUT}")
