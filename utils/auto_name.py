#!/usr/bin/env python3
"""
Table Tennis Xbox 360 — Bulk Function Auto-Namer

Analyzes the binary and symbols.txt to automatically assign meaningful
names to thousands of unnamed functions by:

1. String reference analysis — functions that reference known strings
   get module prefixes (e.g., "audStream" → aud_, "grcDevice" → grc_)
2. Vtable membership — functions in vtables get ClassName_SlotN names
3. Thunk detection — tiny functions that just branch to another
4. Call pattern analysis — functions calling known SDK/API functions
5. Cluster analysis — functions in tight address groups share prefixes

Usage:
  python3 tools/auto_name.py [--dry-run] [--verbose]
"""

import re
import struct
import argparse
from collections import defaultdict
from pathlib import Path

# ─── PE Layout ──────────────────────────────────────────────────────
IMAGE_BASE = 0x82000000
RDATA_VA   = 0x82000600;  RDATA_RAW = 0x600;    RDATA_SIZE = 0x9AA00
TEXT_VA    = 0x820C0000;  TEXT_RAW  = 0xB6200;  TEXT_SIZE  = 0x4C6A00
DATA_VA    = 0x825B0000;  DATA_RAW  = 0x5A2400; DATA_SIZE  = 0x37000

# ─── Module prefixes based on RAGE engine naming conventions ────────
MODULE_STRINGS = {
    # String pattern → (prefix, module name)
    "grcDevice":       ("grc",    "Graphics Device"),
    "grcRender":       ("grc",    "Graphics Renderer"),
    "grcTexture":      ("grc",    "Graphics Texture"),
    "grcEffect":       ("grc",    "Graphics Effect"),
    "grcWindow":       ("grc",    "Graphics Window"),
    "audStream":       ("aud",    "Audio Stream"),
    "audSystem":       ("aud",    "Audio System"),
    "audSound":        ("aud",    "Audio Sound"),
    "audMixer":        ("aud",    "Audio Mixer"),
    "audBank":         ("aud",    "Audio Bank"),
    "RAGE audio":      ("aud",    "Audio"),
    "ioInput":         ("io",     "Input"),
    "ioMouse":         ("io",     "Input Mouse"),
    "ioKeyboard":      ("io",     "Input Keyboard"),
    "ioPad":           ("io",     "Input Pad"),
    "pgStreamer":      ("pg",     "Page Streamer"),
    "pgDictionary":    ("pg",     "Page Dictionary"),
    "pgBase":          ("pg",     "Page Base"),
    "sysMemAllocator": ("sysMem", "Memory Allocator"),
    "sysMemSimple":    ("sysMem", "Simple Allocator"),
    "phCollider":      ("ph",     "Physics Collider"),
    "phBound":         ("ph",     "Physics Bound"),
    "phInst":          ("ph",     "Physics Instance"),
    "phConstraint":    ("ph",     "Physics Constraint"),
    "phJoint":         ("ph",     "Physics Joint"),
    "phArch":          ("ph",     "Physics Archetype"),
    "rmcDrawable":     ("rmc",    "Drawable"),
    "rmcLod":          ("rmc",    "LOD"),
    "fwTimer":         ("fw",     "Framework Timer"),
    "fwAction":        ("fw",     "Framework Action"),
    "fwArchetype":     ("fw",     "Framework Archetype"),
    "fwEntity":        ("fw",     "Framework Entity"),
    "fwRefContainer":  ("fw",     "Framework RefContainer"),
    "netSession":      ("net",    "Network Session"),
    "netPlayer":       ("net",    "Network Player"),
    "netSocket":       ("net",    "Network Socket"),
    "netSize":         ("net",    "Network"),
    "netState":        ("net",    "Network"),
    "CCmd":            ("cmd",    "Command"),
    "rage_default":    ("rage",   "RAGE Default"),
    "rage_alpha":      ("rage",   "RAGE Alpha"),
    "rage_bink":       ("rage",   "RAGE Bink"),
    "rage_im":         ("rage",   "RAGE Immediate"),
    "rageLuminance":   ("rage",   "RAGE Luminance"),
    "MatchWindow":     ("game",   "Game Match"),
    "Tournament":      ("game",   "Game Tournament"),
    "PlayerSelect":    ("game",   "Game PlayerSelect"),
    "MenuScreen":      ("game",   "Game Menu"),
    "HudElement":      ("game",   "Game HUD"),
    "Loading":         ("game",   "Game Loading"),
    "Replay":          ("game",   "Game Replay"),
    "Training":        ("game",   "Game Training"),
    "pong":            ("game",   "Game Core"),
}

# SDK/XDK function prefixes (for call pattern analysis)
SDK_PATTERNS = {
    "XInput":   "io",
    "Xam":      "xam",
    "NetDll":   "net",
    "Nt":       "nt",
    "Ke":       "ke",
    "Vd":       "grc",
    "XMA":      "aud",
    "XAudio":   "aud",
    "XNet":     "net",
}


def va_to_raw(va):
    """Convert virtual address to file offset."""
    if RDATA_VA <= va < RDATA_VA + RDATA_SIZE:
        return RDATA_RAW + (va - RDATA_VA)
    if TEXT_VA <= va < TEXT_VA + TEXT_SIZE:
        return TEXT_RAW + (va - TEXT_VA)
    if DATA_VA <= va < DATA_VA + DATA_SIZE:
        return DATA_RAW + (va - DATA_VA)
    return None


def read_string(data, va, maxlen=120):
    """Read a null-terminated ASCII string at VA."""
    raw = va_to_raw(va)
    if raw is None or raw >= len(data):
        return None
    s = b""
    for i in range(maxlen):
        if raw + i >= len(data):
            break
        b = data[raw + i:raw + i + 1]
        if not b or b == b'\x00':
            break
        if b[0] < 0x20 or b[0] > 0x7E:
            return None  # not a printable string
        s += b
    if len(s) < 3:
        return None
    return s.decode('ascii', errors='replace')


def parse_symbols(sym_path):
    """Parse symbols.txt into function and data label lists."""
    functions = {}  # addr → (name, size)
    data_labels = {}  # addr → (name, size)

    with open(sym_path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('//') or line.startswith('#'):
                continue
            m = re.match(
                r'(\w+)\s*=\s*\.(\w+):0x([0-9A-Fa-f]+);\s*//\s*type:(\w+)\s*size:0x([0-9A-Fa-f]+)',
                line)
            if m:
                name, section, addr_s, typ, size_s = m.groups()
                addr = int(addr_s, 16)
                size = int(size_s, 16)
                if typ == 'function':
                    functions[addr] = (name, size)
                elif typ == 'object':
                    data_labels[addr] = (name, size)
    return functions, data_labels


def find_string_references(data, functions):
    """Find functions that reference known string patterns via lis+addi pairs."""
    func_strings = defaultdict(set)  # addr → set of referenced strings
    func_modules = defaultdict(set)  # addr → set of module prefixes

    for addr, (name, size) in functions.items():
        if not name.startswith('fn_') and not name.startswith('sub_'):
            continue  # skip already-named

        raw = va_to_raw(addr)
        if raw is None:
            continue

        # Scan for lis+addi pairs (string address loading)
        # lis rX, imm16  → opcode 0x3CXY, 0x3DXY  (addis)
        # addi rX, rX, imm16 → opcode 0x38XY
        lis_regs = {}  # reg → upper16
        for off in range(0, size, 4):
            if raw + off + 4 > len(data):
                break
            insn = struct.unpack_from('>I', data, raw + off)[0]
            opcode = (insn >> 26) & 0x3F

            # lis rD, imm (addis rD, 0, imm)
            if opcode == 15:  # addis
                rd = (insn >> 21) & 0x1F
                ra = (insn >> 16) & 0x1F
                imm = insn & 0xFFFF
                if imm & 0x8000:
                    imm -= 0x10000
                if ra == 0:
                    lis_regs[rd] = imm << 16

            # addi rD, rA, imm
            elif opcode == 14:  # addi
                rd = (insn >> 21) & 0x1F
                ra = (insn >> 16) & 0x1F
                imm = insn & 0xFFFF
                if imm & 0x8000:
                    imm -= 0x10000
                if ra in lis_regs:
                    full_addr = (lis_regs[ra] + imm) & 0xFFFFFFFF
                    # Check if it points to a string in .rdata
                    s = read_string(data, full_addr)
                    if s:
                        func_strings[addr].add(s)
                        # Check against module patterns
                        for pattern, (prefix, modname) in MODULE_STRINGS.items():
                            if pattern.lower() in s.lower():
                                func_modules[addr].add(prefix)

    return func_strings, func_modules


def find_vtables(data, functions, data_labels):
    """Find vtable structures in .rdata and map functions to classes."""
    vtable_map = {}  # func_addr → set of (vtable_addr, slot_index)

    # Scan .rdata for vtable-like structures (arrays of .text pointers)
    func_addrs = set(functions.keys())
    vtables_found = 0

    for offset in range(0, RDATA_SIZE - 16, 4):
        va = RDATA_VA + offset
        raw = RDATA_RAW + offset

        # Read consecutive function pointers
        entries = []
        for i in range(64):  # max 64 vtable entries
            if raw + i * 4 + 4 > len(data):
                break
            val = struct.unpack_from('>I', data, raw + i * 4)[0]
            if val in func_addrs:
                entries.append((i, val))
            elif val == 0:
                break
            elif TEXT_VA <= val < TEXT_VA + TEXT_SIZE:
                entries.append((i, val))  # might be an unlisted function
            else:
                break

        if len(entries) >= 3:  # at least 3 function pointers = vtable
            vtables_found += 1
            for slot_idx, func_addr in entries:
                if func_addr in func_addrs:
                    if func_addr not in vtable_map:
                        vtable_map[func_addr] = set()
                    vtable_map[func_addr].add((va, slot_idx))
            # Skip past this vtable to avoid double-counting
            offset += len(entries) * 4

    return vtable_map, vtables_found


def detect_thunks(data, functions):
    """Detect tiny functions that are just branch thunks."""
    thunks = {}  # addr → target_addr

    for addr, (name, size) in functions.items():
        if size > 16 or not name.startswith('fn_'):
            continue

        raw = va_to_raw(addr)
        if raw is None:
            continue

        # Check for: b target (unconditional branch)
        insn = struct.unpack_from('>I', data, raw)[0]
        opcode = (insn >> 26) & 0x3F
        if opcode == 18:  # b/bl
            li = insn & 0x03FFFFFC
            if li & 0x02000000:
                li -= 0x04000000
            aa = (insn >> 1) & 1
            target = li if aa else (addr + li)
            target &= 0xFFFFFFFF
            if target in functions:
                thunks[addr] = target

        # Check for: blr (single instruction return)
        if size == 4 and insn == 0x4E800020:
            thunks[addr] = 0  # nop/ret thunk

    return thunks


def find_call_targets(data, functions):
    """Map functions to their direct call targets (bl instructions)."""
    call_graph = defaultdict(set)  # caller → set of callee addrs

    for addr, (name, size) in functions.items():
        raw = va_to_raw(addr)
        if raw is None:
            continue

        for off in range(0, size, 4):
            if raw + off + 4 > len(data):
                break
            insn = struct.unpack_from('>I', data, raw + off)[0]
            opcode = (insn >> 26) & 0x3F

            if opcode == 18:  # b/bl
                lk = insn & 1
                if lk:  # bl (call)
                    li = insn & 0x03FFFFFC
                    if li & 0x02000000:
                        li -= 0x04000000
                    target = (addr + off + li) & 0xFFFFFFFF
                    if target in functions:
                        call_graph[addr].add(target)

    return call_graph


def propagate_modules(functions, call_graph, func_modules):
    """Propagate module assignments through the call graph."""
    # Functions called by known-module functions likely belong to same module
    for _ in range(3):  # 3 passes
        new_assignments = {}
        for caller, callees in call_graph.items():
            if caller in func_modules:
                for callee in callees:
                    if callee not in func_modules:
                        name = functions.get(callee, ("", 0))[0]
                        if name.startswith('fn_'):
                            new_assignments[callee] = func_modules[caller].copy()

        for addr, modules in new_assignments.items():
            func_modules[addr] = modules


def generate_names(functions, func_modules, func_strings, vtable_map,
                   thunks, call_graph):
    """Generate new names for unnamed functions."""
    renames = {}  # addr → new_name
    used_names = set(n for n, _ in functions.values())
    module_counters = defaultdict(int)

    for addr in sorted(functions.keys()):
        name, size = functions[addr]
        if not name.startswith('fn_') and not name.startswith('sub_'):
            continue  # already named

        new_name = None

        # 1. Thunk → thunk_targetName
        if addr in thunks:
            target = thunks[addr]
            if target == 0:
                new_name = f"nop_{addr:08X}"
            elif target in functions:
                target_name = functions[target][0]
                if not target_name.startswith('fn_'):
                    new_name = f"thunk_{target_name}"
                else:
                    continue  # skip, target also unnamed

        # 2. Module from string references
        if new_name is None and addr in func_modules:
            modules = func_modules[addr]
            if len(modules) == 1:
                prefix = list(modules)[0]
                module_counters[prefix] += 1
                new_name = f"{prefix}_{addr & 0xFFFF:04X}"

        # 3. Vtable membership with module context
        if new_name is None and addr in vtable_map:
            slots = vtable_map[addr]
            # Use the first vtable
            vt_addr, slot_idx = sorted(slots)[0]
            # Try to find a module for the vtable
            vt_module = None
            for fn_addr, mods in func_modules.items():
                if fn_addr in vtable_map:
                    for va, si in vtable_map[fn_addr]:
                        if va == vt_addr and len(mods) == 1:
                            vt_module = list(mods)[0]
                            break
            if vt_module:
                new_name = f"{vt_module}_vt{(vt_addr & 0xFFFF):04X}_{slot_idx}"
            else:
                new_name = f"vt{(vt_addr & 0xFFFF):04X}_{slot_idx}"

        # 4. Call-pattern based (calls known SDK functions)
        if new_name is None and addr in call_graph:
            for callee in call_graph[addr]:
                callee_name = functions.get(callee, ("", 0))[0]
                for pat, prefix in SDK_PATTERNS.items():
                    if callee_name.startswith(pat):
                        module_counters[prefix] += 1
                        new_name = f"{prefix}_{addr & 0xFFFF:04X}"
                        break
                if new_name:
                    break

        if new_name and new_name not in used_names:
            renames[addr] = new_name
            used_names.add(new_name)

    return renames


def apply_renames(sym_path, renames, dry_run=False):
    """Apply renames to symbols.txt."""
    with open(sym_path) as f:
        txt = f.read()

    applied = 0
    for addr, new_name in sorted(renames.items()):
        hex_addr = f"{addr:08X}"
        old_pattern = rf'^(fn_{hex_addr}|sub_{hex_addr})\s*='
        new_line = f'{new_name} ='
        new_txt = re.sub(old_pattern, new_line, txt, flags=re.MULTILINE)
        if new_txt != txt:
            txt = new_txt
            applied += 1

    if not dry_run:
        with open(sym_path, 'w') as f:
            f.write(txt)

    return applied


def main():
    parser = argparse.ArgumentParser(description="Bulk auto-name functions")
    parser.add_argument('--dry-run', action='store_true',
                        help="Don't write changes, just report")
    parser.add_argument('--verbose', '-v', action='store_true',
                        help="Print detailed output")
    args = parser.parse_args()

    pe_path = Path('orig/434C4803/pong_xenon_final.pe')
    sym_path = Path('config/434C4803/symbols.txt')

    print("Loading binary...")
    with open(pe_path, 'rb') as f:
        data = f.read()

    print("Parsing symbols...")
    functions, data_labels = parse_symbols(sym_path)
    unnamed = sum(1 for n, _ in functions.values()
                  if n.startswith('fn_') or n.startswith('sub_'))
    print(f"  {len(functions)} functions, {unnamed} unnamed")

    print("Scanning string references...")
    func_strings, func_modules = find_string_references(data, functions)
    print(f"  {len(func_strings)} functions reference strings")
    print(f"  {len(func_modules)} functions assigned to modules")

    print("Scanning vtables...")
    vtable_map, vtables_found = find_vtables(data, functions, data_labels)
    print(f"  {vtables_found} vtable structures found")
    print(f"  {len(vtable_map)} functions found in vtables")

    print("Detecting thunks...")
    thunks = detect_thunks(data, functions)
    print(f"  {len(thunks)} thunks detected")

    print("Building call graph...")
    call_graph = find_call_targets(data, functions)
    print(f"  {len(call_graph)} functions with outgoing calls")

    print("Propagating module assignments...")
    propagate_modules(functions, call_graph, func_modules)
    print(f"  {len(func_modules)} functions now have module assignments")

    print("Generating names...")
    renames = generate_names(functions, func_modules, func_strings,
                             vtable_map, thunks, call_graph)
    print(f"  {len(renames)} new names generated")

    if args.verbose:
        # Module breakdown
        module_counts = defaultdict(int)
        for addr, name in renames.items():
            prefix = name.split('_')[0] if '_' in name else name[:4]
            module_counts[prefix] += 1
        print("\n  Module breakdown:")
        for mod, count in sorted(module_counts.items(), key=lambda x: -x[1]):
            print(f"    {mod:15s} {count:5d}")

    if args.dry_run:
        print(f"\n[DRY RUN] Would rename {len(renames)} functions")
        # Show first 20
        for addr, name in sorted(renames.items())[:20]:
            old = functions[addr][0]
            print(f"  {old} → {name}")
        if len(renames) > 20:
            print(f"  ... and {len(renames) - 20} more")
    else:
        print(f"\nApplying {len(renames)} renames to symbols.txt...")
        applied = apply_renames(sym_path, renames)
        print(f"  {applied} renames applied successfully")

    # Summary stats
    total = len(functions)
    already_named = sum(1 for n, _ in functions.values()
                        if not n.startswith('fn_') and not n.startswith('sub_'))
    newly_named = len(renames)
    still_unnamed = total - already_named - newly_named
    print(f"\n=== Coverage Summary ===")
    print(f"  Previously named:  {already_named:6d} ({already_named*100/total:.1f}%)")
    print(f"  Newly named:       {newly_named:6d} ({newly_named*100/total:.1f}%)")
    print(f"  Still unnamed:     {still_unnamed:6d} ({still_unnamed*100/total:.1f}%)")
    print(f"  Total:             {total:6d}")


if __name__ == '__main__':
    main()
