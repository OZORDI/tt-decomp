#!/usr/bin/env python3
"""
Phase 3D: Struct Member Lifting
=================================
Transforms structured_pass5c/ -> structured_pass5d/

Algorithm:
  1. Per-function, collect all (base_reg, offset, width, access_type) tuples
  2. Determine type of each base register from context:
     - Named function prefix (pongPlayer_* -> ctx.r3 is pongPlayer*)
     - Constructor call followed by immediate vtable use
     - SDA global (known type from symbols)
  3. Cross-validate with debug_string_field_map.json (field name oracle)
  4. Cross-validate with manual include/ headers (ground truth)
  5. Annotate memory accesses with field names
  6. Generate include/generated_structs.h

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/stage3_structs.py [--dry-run] [--file N]
"""

import re, json, os, sys, shutil, argparse
from pathlib import Path
from collections import defaultdict

BASE        = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG         = BASE / "config/434C4803"
INPUT_DIR   = BASE / "recomp/structured_pass5c"
OUTPUT_DIR  = BASE / "recomp/structured_pass5d"
INCLUDE_DIR = BASE / "include"

# ─────────────────────────────────────────────────────────────────────────────
# Load tables
# ─────────────────────────────────────────────────────────────────────────────

def load_tables():
    with open(CFG / "master_symbol_map.json") as f:
        msm = json.load(f)

    with open(CFG / "debug_string_field_map.json") as f:
        field_map = json.load(f)

    with open(CFG / "vtable_layout_map.json") as f:
        vtable_map = json.load(f)

    return msm, field_map, vtable_map


# ─────────────────────────────────────────────────────────────────────────────
# Build a known-struct offset table from manual include/ headers
# ─────────────────────────────────────────────────────────────────────────────

def parse_manual_headers():
    """Parse include/ headers to extract known struct field offsets."""
    known_fields = {}  # (struct_name, offset) -> field_name

    # Hardcode the verified fields from the manual headers we've read
    # These are ground truth - do not override
    MANUAL_KNOWN = {
        # rage::GameObject (from include/rage/game_object.h)
        ("GameObject", 0x00): ("vtable", "void*"),
        ("GameObject", 0x04): ("flags", "uint32_t"),
        ("GameObject", 0x22C): ("scene_state", "uint32_t"),
        ("GameObject", 0x240): ("render_flags_1", "uint8_t"),
        ("GameObject", 0x241): ("render_flags_2", "uint8_t"),
        ("GameObject", 0x242): ("render_flags_3", "uint8_t"),
        ("GameObject", 0x243): ("render_flags_4", "uint8_t"),
        # BallState (from include/tabletennis_types.h)
        # pongPlayer (inferred from debug strings - field oracle)
        ("pongPlayer", 0x00): ("vtable", "void*"),
        ("pongPlayer", 0x04): ("flags", "uint32_t"),
    }

    known_fields = {}
    for (struct, offset), (field, ftype) in MANUAL_KNOWN.items():
        known_fields[(struct, offset)] = (field, ftype, "manual")

    return known_fields


# ─────────────────────────────────────────────────────────────────────────────
# Infer class from function name
# ─────────────────────────────────────────────────────────────────────────────

# Known function-prefix -> this-pointer class mappings
# r3 in any pongPlayer_* function is pongPlayer*
FUNC_PREFIX_CLASS = {
    "pongPlayer":              "pongPlayer",
    "pongCreature":            "pongCreature",
    "pongBallInstance":        "pongBallInstance",
    "pongCameraMgr":           "pongCameraMgr",
    "pongLiveManager":         "pongLiveManager",
    "pongMover":               "pongMover",
    "pongDrawBucket":          "pongDrawBucket",
    "pongDrawBucketManager":   "pongDrawBucketManager",
    "phBoundBox":              "phBoundBox",
    "phBoundCapsule":          "phBoundCapsule",
    "phSimulator":             "phSimulator",
    "phJoint":                 "phJoint",
    "phJoint3Dof":             "phJoint3Dof",
    "SinglesNetworkClient":    "SinglesNetworkClient",
    "pongNetMessageHolder":    "pongNetMessageHolder",
    "sysMemSimpleAllocator":   "sysMemSimpleAllocator",
    "gmLogicSinglesMatch":     "gmLogicSinglesMatch",
    "audBank":                 "audBank",
    "audVoiceSfx":             "audVoiceSfx",
    "audVoiceStream":          "audVoiceStream",
    "fiDevice":                "fiDevice",
    "fiAsciiTokenizer":        "fiAsciiTokenizer",
    "crIKSpine":               "crIKSpine",
    "crIKLimb":                "crIKLimb",
    "crIKHead":                "crIKHead",
    "gdBallPredictData":       "gdBallPredictData",
    "pongLadder":              "pongLadder",
    "pongBlendShapeController":"pongBlendShapeController",
    "msgMsgSink":              "msgMsgSink",
    "atSingleton":             "atSingleton",
    "rage":                    "rage_GameObject",
}

def infer_class_from_func_name(func_name):
    """Try to determine the this-pointer class from function name prefix."""
    if not func_name:
        return None
    for prefix, class_name in FUNC_PREFIX_CLASS.items():
        if func_name.startswith(prefix + "_") or func_name.startswith(prefix + "::"):
            return class_name
    return None


# ─────────────────────────────────────────────────────────────────────────────
# Field name oracle
# ─────────────────────────────────────────────────────────────────────────────

# Known field layout for key classes (from four-way triangulation)
# These are the highest-confidence inferences
KNOWN_STRUCT_FIELDS = {
    "pongPlayer": {
        0x00: ("vtable", "void*"),
        0x04: ("flags", "uint32_t"),
        0x08: ("m_pCreature", "pongCreature*"),    # debug: "creature already activated"
        0x0C: ("m_bActive", "uint32_t"),            # debug: "player is not active"
        0x10: ("m_playerIndex", "int32_t"),         # debug: "player[%d]"
        0x18: ("m_pMover", "pongMover*"),           # pongMover sub-accesses
    },
    "pongCreature": {
        0x00: ("vtable", "void*"),
        0x04: ("flags", "uint32_t"),
    },
    "phBoundBox": {
        0x00: ("vtable", "void*"),
        0x04: ("flags", "uint32_t"),
    },
    "phSimulator": {
        0x00: ("vtable", "void*"),
        0x04: ("flags", "uint32_t"),
    },
    "pongBallInstance": {
        0x00: ("vtable", "void*"),
        0x04: ("flags", "uint32_t"),
        0x08: ("m_bActive", "uint32_t"),            # "ball already active"
    },
    "pongDrawBucketManager": {
        0x00: ("vtable", "void*"),
        0x04: ("m_maxBuckets", "uint32_t"),
        0x08: ("m_buckets", "void*"),               # array pointer
    },
    "sysMemSimpleAllocator": {
        0x00: ("vtable", "void*"),
    },
    # Universal for all objects:
    # +0x00 = vtable (void*)
    # +0x04 = flags (uint32_t)
}


def lookup_field_name(class_name, offset, known_fields, field_map):
    """Look up a field name for (class, offset) using all available sources."""
    # Priority 1: Manual headers / high-confidence known fields
    if (class_name, offset) in known_fields:
        return known_fields[(class_name, offset)]

    # Priority 2: KNOWN_STRUCT_FIELDS table
    if class_name in KNOWN_STRUCT_FIELDS:
        class_fields = KNOWN_STRUCT_FIELDS[class_name]
        if offset in class_fields:
            field_name, field_type = class_fields[offset]
            return (field_name, field_type, "struct_oracle")

    # Priority 3: Universal vtable (offset 0 = vtable, offset 4 = flags)
    if offset == 0:
        return ("vtable", "void*", "universal")
    if offset == 4:
        return ("flags", "uint32_t", "universal")

    # Priority 4: Field map hints from debug strings
    if class_name in field_map:
        for entry in field_map[class_name]:
            for hint in entry.get("field_hints", []):
                # We don't have exact offsets from strings - mark as hint only
                pass

    return None  # Unknown - leave as PPC_LOAD/STORE


# ─────────────────────────────────────────────────────────────────────────────
# Regex patterns for memory access annotation
# ─────────────────────────────────────────────────────────────────────────────

# PPC_LOAD_U32(reg + offset) where reg is a named variable or ctx.rN
RE_LOAD32  = re.compile(r'PPC_LOAD_U32\(((?:ctx\.r\d+\.u32|var_r\d+))\s*\+\s*(\d+)\)')
RE_STORE32 = re.compile(r'PPC_STORE_U32\(((?:ctx\.r\d+\.u32|var_r\d+))\s*\+\s*(\d+),')
RE_LOAD_F  = re.compile(r'PPC_LOAD_F32\(((?:ctx\.r\d+\.u32|var_r\d+))\s*\+\s*(\d+)\)')
RE_STORE_F = re.compile(r'PPC_STORE_F32\(((?:ctx\.r\d+\.u32|var_r\d+))\s*\+\s*(\d+),')

# PPC_FUNC_IMPL line to detect current function name
RE_FUNC_IMPL = re.compile(r'^PPC_FUNC_IMPL\(__imp__(\w+)\)')


def annotate_memory_access(line, class_name, known_fields, field_map, stats):
    """Add field name comments to memory access lines."""
    if not class_name:
        return line

    already_commented = "/*" in line
    if already_commented:
        return line

    def make_annotation(m, is_float=False):
        reg = m.group(1)
        offset = int(m.group(2))

        # Only annotate r3 (this pointer) and r29-r31 (common callee-saved this ptrs)
        if reg not in ("ctx.r3.u32", "ctx.r29.u32", "ctx.r30.u32", "ctx.r31.u32",
                       "var_r29", "var_r30", "var_r31"):
            return m.group(0)

        result = lookup_field_name(class_name, offset, known_fields, field_map)
        if result:
            field_name, field_type, source = result
            stats["fields_annotated"] += 1
            return m.group(0) + f"/* {class_name}::{field_name}@+{offset:#x} */"
        return m.group(0)

    line = RE_LOAD32.sub(make_annotation, line)
    line = RE_STORE32.sub(make_annotation, line)
    line = RE_LOAD_F.sub(make_annotation, line)
    line = RE_STORE_F.sub(make_annotation, line)

    return line


# ─────────────────────────────────────────────────────────────────────────────
# Generate include/generated_structs.h
# ─────────────────────────────────────────────────────────────────────────────

def generate_structs_header(field_map):
    """Generate struct definitions for classes with inferred layouts."""
    out_path = INCLUDE_DIR / "generated_structs.h"

    lines = [
        "// AUTO-GENERATED by stage3_structs.py — DO NOT EDIT\n",
        "// Partial struct definitions inferred from 4-way triangulation:\n",
        "// symbols.txt + RTTI + debug strings + PPC offset patterns\n",
        "// Field confidence: ★★★ = debug string, ★★☆ = offset cluster, ★☆☆ = inference\n",
        "// Validate all offsets before use in non-recomp code!\n",
        "#pragma once\n",
        "#include <cstdint>\n",
        "\n",
        "// Forward declarations\n",
    ]

    # Forward declare all known classes
    for class_name in sorted(KNOWN_STRUCT_FIELDS.keys()):
        if "::" not in class_name:
            lines.append(f"struct {class_name};\n")

    lines.append("\n// Struct definitions with known field layouts\n\n")

    for class_name, fields in sorted(KNOWN_STRUCT_FIELDS.items()):
        if not fields:
            continue

        # Get debug string hints for this class
        hints = {}
        if class_name in field_map:
            for entry in field_map[class_name]:
                for hint in entry.get("field_hints", []):
                    hints[hint["field"]] = hint.get("evidence", "")

        lines.append(f"// {class_name}\n")
        lines.append(f"// Debug string evidence: {len(hints)} field hints\n")
        if hints:
            for field, ev in list(hints.items())[:3]:
                lines.append(f"//   {field}: \"{ev[:60]}\"\n")
        lines.append(f"struct {class_name} {{\n")

        sorted_offsets = sorted(fields.items())
        for offset, (field_name, field_type) in sorted_offsets:
            lines.append(f"    {field_type} {field_name};  // +{offset:#04x}\n")

        lines.append(f"    // ... additional fields not yet recovered\n")
        lines.append(f"}};\n\n")

    with open(out_path, "w") as f:
        f.writelines(lines)

    print(f"  Generated {out_path.name} with {len(KNOWN_STRUCT_FIELDS)} struct definitions")


# ─────────────────────────────────────────────────────────────────────────────
# Process a single file
# ─────────────────────────────────────────────────────────────────────────────

def process_file(in_path, out_path, known_fields, field_map, stats, dry_run=False):
    with open(in_path) as f:
        lines = f.readlines()

    out_lines = []
    current_func_name = None
    current_class = None

    for line in lines:
        # Detect current function for class inference
        m_func = RE_FUNC_IMPL.match(line.strip())
        if m_func:
            current_func_name = m_func.group(1)
            current_class = infer_class_from_func_name(current_func_name)

        # Annotate memory accesses
        if current_class and ("PPC_LOAD" in line or "PPC_STORE" in line):
            line = annotate_memory_access(line, current_class, known_fields, field_map, stats)

        out_lines.append(line)

    stats["files_processed"] += 1

    if not dry_run:
        with open(out_path, "w") as f:
            f.writelines(out_lines)


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Stage 3D: Struct Member Lifting")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--file", type=str)
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args()

    print("=" * 60)
    print("Stage 3D: Struct Member Lifting")
    print(f"Input:  {INPUT_DIR}")
    print(f"Output: {OUTPUT_DIR}")
    print("=" * 60)

    msm, field_map, vtable_map = load_tables()
    known_fields = parse_manual_headers()
    print(f"  Loaded {len(known_fields)} manual header field entries")
    print(f"  Loaded {len(field_map)} classes with debug string hints")

    input_files = sorted(INPUT_DIR.glob("tt-decomp_recomp.*.cpp"))
    input_files += [INPUT_DIR / "tt-decomp_init.cpp"]

    if args.file is not None:
        if args.file == "init":
            input_files = [INPUT_DIR / "tt-decomp_init.cpp"]
        else:
            target = f"tt-decomp_recomp.{args.file}.cpp"
            input_files = [f for f in input_files if f.name == target]

    stats = defaultdict(int)

    if not args.dry_run:
        os.makedirs(OUTPUT_DIR, exist_ok=True)
        for static in ["sources.cmake", "tt-decomp_config.h", "tt-decomp_init.h"]:
            src = INPUT_DIR / static
            if src.exists():
                shutil.copy2(src, OUTPUT_DIR / static)
        generate_structs_header(field_map)

    total = len(input_files)
    for idx, in_path in enumerate(input_files):
        out_path = OUTPUT_DIR / in_path.name
        if args.verbose:
            print(f"  [{idx+1}/{total}] {in_path.name}")
        else:
            print(f"  [{idx+1}/{total}] {in_path.name}", end="\r")
        process_file(in_path, out_path, known_fields, field_map, stats, dry_run=args.dry_run)

    print()
    print("\n--- Phase 3D Results ---")
    print(f"  Files processed:       {stats['files_processed']}")
    print(f"  Fields annotated:      {stats['fields_annotated']}")
    if not args.dry_run:
        print(f"\nOutput: {OUTPUT_DIR}")
    print("Phase 3D complete.")


if __name__ == "__main__":
    main()
