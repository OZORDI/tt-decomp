import sys

with open('auto_namer_v12.py', 'r') as f:
    code = f.read()

old_code = """
        for off in range(0, size, 4):
            if raw + off + 12 > len(data): break
            ins_w = int.from_bytes(data[raw+off : raw+off+4], 'big')
            op = (ins_w >> 26) & 0x3F; rd = (ins_w >> 21) & 0x1F; ra = (ins_w >> 16) & 0x1F; imm = ins_w & 0xFFFF
            
            if op == 15 and rd == 3 and ra == 0:  # lis r3, HI
                hi = imm if imm < 0x8000 else imm - 0x10000
                ins2 = int.from_bytes(data[raw+off+4 : raw+off+8], 'big')
                op2 = (ins2 >> 26) & 0x3F; rd2 = (ins2 >> 21) & 0x1F; ra2 = (ins2 >> 16) & 0x1F; imm2 = ins2 & 0xFFFF
                if op2 == 14 and rd2 == 3 and ra2 == 3:  # addi r3, r3, LO
"""
new_code = """
        for off in range(0, size, 4):
            if raw + off + 12 > len(data): break
            ins_w = int.from_bytes(data[raw+off : raw+off+4], 'big')
            op = (ins_w >> 26) & 0x3F; rd = (ins_w >> 21) & 0x1F; ra = (ins_w >> 16) & 0x1F; imm = ins_w & 0xFFFF
            
            if op == 15 and ra == 0:  # lis rX, HI
                hi = imm if imm < 0x8000 else imm - 0x10000
                ins2 = int.from_bytes(data[raw+off+4 : raw+off+8], 'big')
                op2 = (ins2 >> 26) & 0x3F; rd2 = (ins2 >> 21) & 0x1F; ra2 = (ins2 >> 16) & 0x1F; imm2 = ins2 & 0xFFFF
                if op2 == 14 and rd2 == 3 and ra2 == rd:  # addi r3, rX, LO
"""
if old_code in code:
    code = code.replace(old_code, new_code)
    with open('auto_namer_v12.py', 'w') as f:
        f.write(code)
    print("Patched P19 successfully")
else:
    print("Could not find code to patch")
