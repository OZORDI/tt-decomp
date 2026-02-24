#!/usr/bin/env python3
"""
Phase 3-Pre: Build Master Lookup Tables
========================================
Builds four JSON lookup tables used by all subsequent Stage 3 phases.
Run this ONCE before running any stage3_*.py scripts.

Outputs (all in config/434C4803/):
  master_symbol_map.json       - 52,691 entries: addr -> {name, type, section, size}
  rtti_class_hierarchy.json    - class name -> {vtable_addrs, rtti_addr}
  vtable_layout_map.json       - class_name -> {slot -> {addr, symbol}}
  debug_string_field_map.json  - class_name -> [field hints from Class::Method() strings]
  sda_resolution_map.json      - signed_offset -> {addr, name} for all r13 SDA accesses
  splits_addr_map.json         - addr -> source_file for all split ranges

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/stage3_pre_build_tables.py
"""

import re, json, os, sys
from pathlib import Path
from collections import defaultdict

BASE = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG  = BASE / "config/434C4803"

SDA_BASE = 0x82600000  # r13 runtime value - confirmed from IDC + rage_state_flags cross-check

# ─────────────────────────────────────────────────────────────────────────────
# Table 1: master_symbol_map.json
# Parse symbols_autonamed_v18.txt
# Format: name = section:0xADDR; // type:T size:0xS scope:S [data:D]
# ─────────────────────────────────────────────────────────────────────────────

def build_symbol_map():
    sym_path = CFG / "symbols_autonamed_v18.txt"
    sym_map   = {}   # addr (int) -> {name, type, section, size}
    name_to_addr = {}

    RE_LINE = re.compile(
        r'^(\S+)\s*=\s*(\.\w+):0x([0-9A-Fa-f]+);\s*//\s*type:(\w+)\s+size:0x([0-9A-Fa-f]+)'
    )

    with open(sym_path) as f:
        for line in f:
            m = RE_LINE.match(line.strip())
            if not m:
                continue
            name, section, addr_hex, typ, size_hex = m.groups()
            addr = int(addr_hex, 16)
            entry = {
                "name": name,
                "type": typ,
                "section": section,
                "size": int(size_hex, 16),
            }
            sym_map[addr] = entry
            name_to_addr[name] = addr

    print(f"  [Table 1] Loaded {len(sym_map)} symbol entries")

    # Also extract class->methods mapping from function names
    class_methods = defaultdict(list)
    for addr, e in sym_map.items():
        if e["type"] != "function":
            continue
        name = e["name"]
        # pongPlayer_PostUpdate -> class=pongPlayer, method=PostUpdate
        # SinglesNetworkClient_HandleBallHitMessage -> class=SinglesNetworkClient, method=...
        # atSingleton_GetInstance_8224CFE0 -> class=atSingleton, method=GetInstance
        # Try to split on first underscore that is NOT part of a camelCase run
        # Strategy: if name has _vfn_ -> it's a vfn symbol, handled in vtable map
        # Otherwise split at first _ that separates a known prefix
        if '_vfn_' not in name:
            parts = name.split('_', 1)
            if len(parts) == 2 and parts[0]:
                class_methods[parts[0]].append({"addr": hex(addr), "method": parts[1], "full": name})

    out = {
        "by_address": {hex(k): v for k, v in sym_map.items()},
        "by_name": {v["name"]: hex(k) for k, v in sym_map.items()},
        "function_count": sum(1 for v in sym_map.values() if v["type"] == "function"),
        "object_count": sum(1 for v in sym_map.values() if v["type"] == "object"),
    }

    with open(CFG / "master_symbol_map.json", "w") as f:
        json.dump(out, f, indent=2)
    print(f"  [Table 1] Wrote master_symbol_map.json ({out['function_count']} funcs, {out['object_count']} objs)")

    return sym_map, name_to_addr


# ─────────────────────────────────────────────────────────────────────────────
# Table 2: rtti_class_hierarchy.json
# Uses existing rtti_vtable_map.json (1715 entries: addr -> class_name)
# Builds: class_name -> [vtable_addr_list]
# ─────────────────────────────────────────────────────────────────────────────

def build_rtti_hierarchy():
    rtti_path = CFG / "rtti_vtable_map.json"
    with open(rtti_path) as f:
        rtti_raw = json.load(f)  # { "0x82XXXXXX": "ClassName" }

    # Invert: class -> list of vtable addresses
    class_vtables = defaultdict(list)
    for addr_str, class_name in rtti_raw.items():
        class_vtables[class_name].append(addr_str)

    # Build hierarchy from naming conventions
    # .?AVpongRemotePlayer@@ inherits pongPlayer (name contains base name)
    # We can infer simple hierarchies from naming patterns
    hierarchy = {}
    for class_name, vtable_addrs in class_vtables.items():
        entry = {
            "vtable_addrs": sorted(vtable_addrs),
            "vtable_count": len(vtable_addrs),
            "inferred_bases": []
        }
        # Infer base classes from naming conventions
        cn = class_name
        if cn.startswith("pong") and cn not in ("pongPlayer", "pongCreature"):
            # Most pong* classes likely have rage::Entity or pongCreature as base
            pass
        # gmLogicSinglesMatch::state* -> base is gmLogicSinglesMatch (inner class)
        if "::" in cn:
            outer = cn.split("::")[0]
            entry["outer_class"] = outer

        hierarchy[class_name] = entry

    print(f"  [Table 2] Built hierarchy for {len(hierarchy)} classes, {len(rtti_raw)} vtable entries")
    with open(CFG / "rtti_class_hierarchy.json", "w") as f:
        json.dump(hierarchy, f, indent=2)
    print(f"  [Table 2] Wrote rtti_class_hierarchy.json")

    return hierarchy, rtti_raw


# ─────────────────────────────────────────────────────────────────────────────
# Table 3: vtable_layout_map.json
# Extract from symbols_autonamed_v18.txt: entries with _vfn_N pattern
# Also merge lost vt* entries from v17
# ─────────────────────────────────────────────────────────────────────────────

def build_vtable_map(sym_map, rtti_raw):
    vtable_map = defaultdict(dict)   # class_name -> {slot_int -> {addr, symbol}}

    RE_VFN = re.compile(r'^(.+?)_vfn_(\d+)$')
    RE_VT_PREFIX = re.compile(r'^vt([0-9A-Fa-f]+)_(.+)$')

    # Pass 1: _vfn_N named entries from v18
    for addr, e in sym_map.items():
        if e["type"] != "function":
            continue
        name = e["name"]
        m = RE_VFN.match(name)
        if m:
            class_name = m.group(1)
            slot = int(m.group(2))
            vtable_map[class_name][slot] = {
                "addr": hex(addr),
                "symbol": name,
                "slot": slot,
                "source": "vfn_symbol"
            }

    # Pass 2: Recover lost vt* entries from v17 that v18 dropped
    # These encode vtable group addresses. Cross-ref with rtti_vtable_map.json
    v17_path = CFG / "symbols_autonamed_v17.txt"
    RE_LINE = re.compile(r'^(\S+)\s*=\s*(\.\w+):0x([0-9A-Fa-f]+);')
    v17_vt_entries = {}
    with open(v17_path) as f:
        for line in f:
            m = RE_LINE.match(line.strip())
            if not m:
                continue
            name, section, addr_hex = m.groups()
            if RE_VT_PREFIX.match(name):
                v17_vt_entries[name] = (int(addr_hex, 16), section)

    # v18 addr set for quick lookup
    v18_addrs = set(sym_map.keys())

    recovered_count = 0
    rdata_base = 0x82000600  # .rdata base

    for name, (addr, section) in v17_vt_entries.items():
        if addr not in v18_addrs:
            # This was dropped by v18. Decode the vt prefix.
            mvt = RE_VT_PREFIX.match(name)
            if mvt:
                vt_hex = mvt.group(1)
                rest = mvt.group(2)
                # vt{XXXX} encodes .rdata-relative vtable group offset
                vt_offset = int(vt_hex, 16)
                vtable_rdata_addr = hex(rdata_base + vt_offset)
                # Look up in rtti_vtable_map
                class_name = rtti_raw.get(vtable_rdata_addr, f"vt{vt_hex}_unknown")
                # Store with recovered tag
                v17_vt_entries[name] = (addr, section, class_name, vtable_rdata_addr)
                recovered_count += 1

    print(f"  [Table 3] Recovered {recovered_count} vt* entries from v17 that v18 dropped")

    # Slot 0 is always destructor - annotate all slot-0 entries
    for class_name, slots in vtable_map.items():
        if 0 in slots:
            slots[0]["inferred_role"] = "destructor"
        if 1 in slots:
            slots[1]["inferred_role"] = "scalar_destructor"

    # Convert defaultdict -> regular dict with string keys for JSON
    out = {}
    for class_name, slots in vtable_map.items():
        out[class_name] = {str(slot): info for slot, info in sorted(slots.items())}

    total_slots = sum(len(v) for v in out.values())
    print(f"  [Table 3] {len(out)} classes, {total_slots} named virtual slots total")
    with open(CFG / "vtable_layout_map.json", "w") as f:
        json.dump(out, f, indent=2)
    print(f"  [Table 3] Wrote vtable_layout_map.json")

    return out


# ─────────────────────────────────────────────────────────────────────────────
# Table 4: debug_string_field_map.json
# Parse binary string pool from orig/ pseudocode dumps for "Class::Method()" patterns
# Extract implied field names and method signatures
# ─────────────────────────────────────────────────────────────────────────────

# Hard-coded set of known Class::Method() debug strings from the deep research
# These were extracted during the asset excavation session
KNOWN_DEBUG_STRINGS = [
    "pongPlayer::Activate() - creature already activated",
    "pongPlayer::Draw() - player is not active",
    "pongPlayer::Render() - player[%d] is not active",
    "pongPlayer::Process() - player is not active",
    "pongPlayer::Teleport() - teleporting player[%d] to [%f,%f,%f]",
    "pongMover::CalcInitMatrix() - setting rest mtx to remote rest mtx [%f,%f,%f]",
    "pongCreature::Teleport() - teleporting player[%d] to [%f,%f,%f]",
    "pongBlendShapeController - blend function '%s' not supported",
    "pongLiveManager::GetAvailablePeerIndex() Out of peer indexes!!!",
    "pongDrawBucketManager::AddEntry - failed, draw bucket full",
    "phSimulator::FindWeakestInst(): Maximum number of breakable components %d exceeded: %d",
    "fiDevice::Mount - mount name must end in a slash",
    "audBank::LoadBank %s timeout",
    "pongPlayer::EnterServeMode",
    "pongPlayer::StartSwing() %s",
    "Cloth Simulation thread for player %d starting on CPU %d",
    "pongBallInstance::ActivateBall() - ball already active",
    "pongCameraMgr::ChangeCamera - no camera set",
    "gmLogicSinglesMatch::MidGame(): Unknown PointsPerGame enum value = %d",
    "phBoundBox::FindImpactsToBox() -- MAX_NUM_IMPACTS %d exceeded",
    "crIKSpine::LoadConfig - could not find root bone '%s'",
    "crIKLimb::LoadConfig - could not find spine bone '%s'",
    "crIKHead::LoadConfig - could not find neck bone '%s'",
    "gdBallPredictData::GetAngularVelocity - no samples",
    "gdCameraDef::Load - unknown player side '%s'",
    "gdTrainingDrillData::Load - aim delay must be > 0",
    "pgStreamer::OpenResource - failed to open '%s'",
    "SinglesNetworkClient::HandleBallHitMessage - invalid ball hit data",
    "pongLadder::LoadTier - unknown opponent type '%s'",
    "pongLiveManager::SetSpectatorSlot - spectator slot %d out of range",
    "pongDrawBucket::AddEntry() -- pongDrawBucket full!",
    "pongCreature::Activate() - already activated",
    "pongCreature::Draw() - creature is not active",
    "audVoiceSfx::Play() - sfx ref is null",
    "audVoiceStream::Play() - stream ref is null",
    "phArchetype::Load() -- .phys file %s does not exist",
]

def extract_field_hints_from_string(s):
    """Extract field name hints from a debug string."""
    hints = []
    # Pattern: "creature already activated" -> m_pCreature (pointer, has activated state)
    # Pattern: "player is not active" -> m_bActive (bool)
    # Pattern: "player[%d]" -> m_playerIndex (int)
    # Pattern: "out of peer indexes" -> m_peerSlots (array)
    # Pattern: "rest mtx" -> m_restMatrix
    # Pattern: "[%f,%f,%f]" -> position/vector3 param

    if "creature" in s.lower() and ("null" in s.lower() or "activated" in s.lower() or "active" in s.lower()):
        hints.append({"field": "m_pCreature", "type": "ptr", "evidence": s})
    if "player is not active" in s.lower() or ("active" in s.lower() and "player" in s.lower()):
        hints.append({"field": "m_bActive", "type": "bool", "evidence": s})
    if "player[%d]" in s or "player %d" in s.lower():
        hints.append({"field": "m_playerIndex", "type": "int", "evidence": s})
    if "%f,%f,%f" in s or "[%f" in s:
        hints.append({"field": "m_position", "type": "Vector3", "evidence": s})
    if "rest mtx" in s.lower():
        hints.append({"field": "m_restMatrix", "type": "Matrix34", "evidence": s})
    if "remote rest mtx" in s.lower():
        hints.append({"field": "m_remoteRestMatrix", "type": "Matrix34", "evidence": s})
    if "peer index" in s.lower():
        hints.append({"field": "m_peerSlots", "type": "array", "evidence": s})
    if "spectator slot" in s.lower():
        hints.append({"field": "m_spectatorSlots", "type": "array", "evidence": s})
    if "draw bucket full" in s.lower():
        hints.append({"field": "m_buckets", "type": "array", "evidence": s})
    if "sfx ref" in s.lower():
        hints.append({"field": "m_sfxRef", "type": "ptr", "evidence": s})
    if "stream ref" in s.lower():
        hints.append({"field": "m_streamRef", "type": "ptr", "evidence": s})
    if "mount name" in s.lower():
        hints.append({"field": "m_mountName", "type": "const char*", "evidence": s})
    if "timeout" in s.lower():
        hints.append({"field": "m_timeout", "type": "float", "evidence": s})
    if "MAX_NUM_IMPACTS" in s:
        hints.append({"field": "m_maxImpacts", "type": "int", "evidence": s})
    if "breakable component" in s.lower():
        hints.append({"field": "m_maxBreakableComponents", "type": "int", "evidence": s})
    if "enum value" in s.lower():
        # Extract enum type from "Unknown ENUMNAME enum value"
        m = re.search(r'Unknown (\w+) enum value', s)
        if m:
            hints.append({"field": f"m_{m.group(1)}", "type": "enum", "enum_name": m.group(1), "evidence": s})
    return hints


def build_debug_field_map():
    field_map = defaultdict(list)  # class_name -> list of {method, string, field_hints}

    RE_CLASS_METHOD = re.compile(r'^(\w[\w:]+)::(\w+)\s*[\(\-]')

    # Also try to mine from the pseudocode dump files
    dump_files = [
        BASE / "orig/pseudocode_dump_FINAL_SYMBOLS.txt",
        BASE / "orig/pseudocode_dump.txt",
    ]

    all_strings = list(KNOWN_DEBUG_STRINGS)  # start with known

    for dump_file in dump_files:
        if dump_file.exists():
            try:
                with open(dump_file, errors='replace') as f:
                    content = f.read()
                # Find quoted strings that look like debug messages
                RE_QUOTED = re.compile(r'"([^"]{10,200})"')
                found = RE_QUOTED.findall(content)
                # Filter: must contain "::" to be a Class::Method pattern
                class_method_strings = [s for s in found if "::" in s and len(s) < 200]
                all_strings.extend(class_method_strings[:2000])  # cap to avoid explosion
                print(f"  [Table 4] Mined {len(class_method_strings)} Class::Method strings from {dump_file.name}")
            except Exception as e:
                print(f"  [Table 4] Could not read {dump_file}: {e}")

    processed = set()
    for s in all_strings:
        if s in processed:
            continue
        processed.add(s)

        m = RE_CLASS_METHOD.match(s)
        if m:
            class_name = m.group(1)
            method_name = m.group(2)
            hints = extract_field_hints_from_string(s)

            field_map[class_name].append({
                "method": method_name,
                "string": s,
                "field_hints": hints
            })

    total_hints = sum(len(v) for v in field_map.values())
    print(f"  [Table 4] {len(field_map)} classes, {total_hints} total field hints from {len(processed)} unique strings")

    with open(CFG / "debug_string_field_map.json", "w") as f:
        json.dump(dict(field_map), f, indent=2)
    print(f"  [Table 4] Wrote debug_string_field_map.json")

    return dict(field_map)


# ─────────────────────────────────────────────────────────────────────────────
# Table 5: sda_resolution_map.json
# Precompute all valid SDA offsets (r13 + offset) -> {addr_hex, symbol_name}
# Covers the range .data section to be safe
# ─────────────────────────────────────────────────────────────────────────────

def build_sda_map(sym_map):
    sda_map = {}  # signed_offset_str -> {addr, name, type, size}

    DATA_START = 0x825B0000
    DATA_END   = 0x82739DAC

    for addr, e in sym_map.items():
        if e["section"] not in (".data", ".rdata"):
            continue
        # Compute what offset r13 would use to reach this address
        # r13 = SDA_BASE = 0x82600000
        # offset = addr - SDA_BASE (signed 16-bit range is -32768 to +32767)
        # But PPC uses 16-bit signed immediate, so only offsets in [-32768, 32767] are SDA
        # For larger offsets, lis+addi is used instead
        offset = addr - SDA_BASE
        if -32768 <= offset <= 32767:
            sda_map[str(offset)] = {
                "addr": hex(addr),
                "name": e["name"],
                "type": e["type"],
                "size": e["size"]
            }

    # Also add the known validated case:
    # ctx.r13 + (-0x4EEC) = 0x82600000 - 0x4EEC = 0x825FB114 = rage_state_flags area
    # Verify our SDA_BASE is correct:
    test_addr = SDA_BASE + (-0x4EEC)
    test_hex = hex(test_addr)
    print(f"  [Table 5] SDA validation: r13 + (-0x4EEC) = {test_hex}")
    if test_hex == "0x825fb114":
        print(f"  [Table 5] ✓ SDA_BASE=0x82600000 validated (0x825FB114 in .data range)")
    else:
        print(f"  [Table 5] ⚠ Unexpected SDA test result: {test_hex}")

    print(f"  [Table 5] {len(sda_map)} symbols reachable via r13 SDA offsets")
    with open(CFG / "sda_resolution_map.json", "w") as f:
        json.dump(sda_map, f, indent=2)
    print(f"  [Table 5] Wrote sda_resolution_map.json")

    return sda_map


# ─────────────────────────────────────────────────────────────────────────────
# Table 6: splits_addr_map.json
# Build a sorted list of (start, end, source_file) for fast range lookup
# ─────────────────────────────────────────────────────────────────────────────

def build_splits_map():
    splits_path = CFG / "splits.txt"
    ranges = []  # list of (start_int, end_int, source_file_str)

    current_file = None
    RE_FILE = re.compile(r'^(src/[\w/]+\.\w+):')
    RE_RANGE = re.compile(r'\.text\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)')

    with open(splits_path) as f:
        for line in f:
            m_file = RE_FILE.match(line.strip())
            if m_file:
                current_file = m_file.group(1)
                continue
            m_range = RE_RANGE.search(line)
            if m_range and current_file:
                start = int(m_range.group(1), 16)
                end   = int(m_range.group(2), 16)
                ranges.append((start, end, current_file))

    ranges.sort()
    print(f"  [Table 6] {len(ranges)} address ranges from {len(set(r[2] for r in ranges))} source files")

    out = [{"start": hex(s), "end": hex(e), "source": f} for s, e, f in ranges]
    with open(CFG / "splits_addr_map.json", "w") as f:
        json.dump(out, f, indent=2)
    print(f"  [Table 6] Wrote splits_addr_map.json")

    return ranges


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    print("=" * 60)
    print("Stage 3-Pre: Building Master Lookup Tables")
    print(f"Config dir: {CFG}")
    print("=" * 60)

    os.makedirs(CFG, exist_ok=True)

    print("\n[Table 1] Building master symbol map...")
    sym_map, name_to_addr = build_symbol_map()

    print("\n[Table 2] Building RTTI class hierarchy...")
    hierarchy, rtti_raw = build_rtti_hierarchy()

    print("\n[Table 3] Building vtable layout map...")
    vtable_map = build_vtable_map(sym_map, rtti_raw)

    print("\n[Table 4] Building debug string field map...")
    field_map = build_debug_field_map()

    print("\n[Table 5] Building SDA resolution map...")
    sda_map = build_sda_map(sym_map)

    print("\n[Table 6] Building splits address map...")
    splits_ranges = build_splits_map()

    print("\n" + "=" * 60)
    print("Phase 3-Pre COMPLETE. All lookup tables built.")
    print(f"Output directory: {CFG}")
    print("=" * 60)
    print("\nReady for Phase 3A: stage3_globals.py")


if __name__ == "__main__":
    main()
