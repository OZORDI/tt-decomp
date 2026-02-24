#!/usr/bin/env python3
"""
Phase 4B: Struct Field Offset Clustering
==========================================
Analysis script — does not transform source files.
Reads recomp/structured_pass5_ph4/ (or pass5/ if ph4 not present).

For each function named ClassName_*, tracks the this-pointer register
and collects all PPC_LOAD/STORE(this_reg + OFFSET) accesses.
Clusters by offset, counts access frequency and widths, producing a
field-frequency map per class.

Outputs:
  config/434C4803/offset_cluster_map.json  — full data
  include/generated_structs.h              — updated with clustered fields

Usage:
  cd /Users/Ozordi/Downloads/tt-decomp
  python3 scripts/phase4b_offset_cluster.py [--top N] [--class CLASSNAME]
"""

import re, json, argparse
from pathlib import Path
from collections import defaultdict

BASE        = Path("/Users/Ozordi/Downloads/tt-decomp")
CFG         = BASE / "config/434C4803"
PH4_DIR     = BASE / "recomp/structured_pass5_ph4"
PASS5_DIR   = BASE / "recomp/structured_pass5"
PSEUDO_DIR  = BASE / "orig/pseudocode"
INCLUDE_DIR = BASE / "include"

# Classes to cluster (extend as needed)
KNOWN_CLASSES = [
    "pongPlayer", "pongCreature", "pongCreatureInst", "pongBallInstance",
    "pongCameraMgr", "pongMover", "pongDrawBucket", "pongDrawBucketManager",
    "pongShadowMap", "pongTrainingDrill", "pongNetMessageHolder",
    "pongLiveManager", "pongLadder", "pongBlendShapeController",
    "gmLogicSinglesMatch", "phBoundBox", "phBoundCapsule", "phBoundComposite",
    "phBoundGeometry", "phSimulator", "phJoint3Dof", "phJoint1Dof",
    "phInst", "phArticulatedCollider", "phCollider", "phLevelNew",
    "SinglesNetworkClient", "sysMemSimpleAllocator", "msgMsgSink",
    "atSingleton", "fiAsciiTokenizer", "fiDevice", "audBank",
    "audVoiceSfx", "audVoiceStream", "crIKSpine", "crIKLimb", "crIKHead",
    "crAnimDofFloat", "crAnimDofQuaternion", "LocomotionStateAnim",
    "grcTextureReferenceBase", "hudFlashBase",
]

# Build sorted-longest-first for greedy prefix matching
KNOWN_CLASSES_SORTED = sorted(KNOWN_CLASSES, key=len, reverse=True)

# ─────────────────────────────────────────────────────────────────────────────
# Regex patterns
# ─────────────────────────────────────────────────────────────────────────────

RE_FUNC_IMPL = re.compile(r'^PPC_FUNC_IMPL\(__imp__([a-zA-Z_][a-zA-Z0-9_]*)\)\s*\{', re.MULTILINE)

# var_r31 = ctx.r3.u32;  — this-pointer save at function entry
RE_THIS_SAVE = re.compile(r'^\s*(var_r\d+)\s*=\s*ctx\.r3\.u32\s*;')

# PPC_LOAD/STORE_XX(BASE + OFFSET)
# Captures: op, width, full_base_expr, var_part(no prefix), ctx_part(no prefix), offset
RE_MEM_ACCESS = re.compile(
    r'PPC_(LOAD|STORE)_(U8|U16|U32|S32|F32|U64|S64)\('
    r'(var_(r\d+)|ctx\.(r\d+)\.u32)'  # groups 3,4,5
    r'\s*\+\s*(-?\d+)'                 # group 6: offset
)

# Pseudocode field: *(uint32_t*)(vN + offset)
RE_PSEUDO_FIELD = re.compile(
    r'\*\s*\(\s*(\w+)\s*\*\s*\)\s*\(\s*v\d+\s*\+\s*(\d+)\s*\)'
)


def extract_class(func_name):
    """Returns longest-matching class prefix or None."""
    for cls in KNOWN_CLASSES_SORTED:
        if func_name.startswith(cls + "_") or func_name.startswith(cls + "_vfn"):
            return cls
    return None


# ─────────────────────────────────────────────────────────────────────────────
# Mine pass5_ph4 cpp files for offset accesses
# ─────────────────────────────────────────────────────────────────────────────

def mine_cpp_files(source_dir):
    """
    Returns: {class_name: {offset_int: {"count", "widths", "ops"}}}
    """
    cluster = defaultdict(lambda: defaultdict(
        lambda: {"count": 0, "widths": defaultdict(int), "ops": defaultdict(int)}
    ))

    cpp_files = sorted(source_dir.glob("tt-decomp_recomp.*.cpp"))
    print(f"  Mining {len(cpp_files)} files in {source_dir.name}...")

    total_funcs = 0
    matched_funcs = 0

    for path in cpp_files:
        with open(path, errors='replace') as f:
            lines_all = f.readlines()

        # Split file into per-function blocks by scanning for PPC_FUNC_IMPL lines
        func_starts = []
        for i, line in enumerate(lines_all):
            if line.startswith("PPC_FUNC_IMPL("):
                func_starts.append(i)

        # Add sentinel
        func_starts.append(len(lines_all))

        for idx, start in enumerate(func_starts[:-1]):
            end = func_starts[idx + 1]
            func_lines = lines_all[start:end]

            total_funcs += 1

            # Extract function name from first line
            m = RE_FUNC_IMPL.match(func_lines[0])
            if not m:
                continue
            func_name = m.group(1)
            class_name = extract_class(func_name)
            if not class_name:
                continue

            matched_funcs += 1

            # Find this-pointer save (usually within first 20 lines)
            this_vars = set()
            for line in func_lines[:20]:
                ms = RE_THIS_SAVE.match(line)
                if ms:
                    this_vars.add(ms.group(1))  # e.g. "var_r31"
                    break

            # For tiny functions with no explicit save, track ctx.r3 directly
            if not this_vars:
                this_vars = {"__direct_r3__"}

            # Scan all lines for memory accesses on the this-pointer
            for line in func_lines:
                for m_acc in RE_MEM_ACCESS.finditer(line):
                    op     = m_acc.group(1)   # LOAD / STORE
                    width  = m_acc.group(2)   # U8/U16/U32/F32 etc.
                    var_r  = m_acc.group(4)   # "r31" or None
                    ctx_r  = m_acc.group(5)   # "r3" or None
                    offset = int(m_acc.group(6))

                    if offset < 0:
                        continue  # stack frame access, not struct field

                    # Construct canonical base name
                    if var_r:
                        base = f"var_{var_r}"      # e.g. "var_r31"
                    else:
                        base = f"ctx.{ctx_r}"      # e.g. "ctx.r3"

                    # Check against direct r3 tracking
                    if "__direct_r3__" in this_vars and base in ("ctx.r3", "ctx.r"):
                        pass  # match
                    elif base not in this_vars:
                        continue

                    entry = cluster[class_name][offset]
                    entry["count"] += 1
                    entry["widths"][width] += 1
                    entry["ops"][op] += 1

    print(f"  Processed {total_funcs} functions, {matched_funcs} matched known classes")
    return cluster


# ─────────────────────────────────────────────────────────────────────────────
# Mine pseudocode/ for type hints
# ─────────────────────────────────────────────────────────────────────────────

def mine_pseudocode(pseudo_dir):
    """Returns: {offset_int: [type_hint, ...]}"""
    if not pseudo_dir.exists():
        print("  No pseudocode/ directory, skipping.")
        return {}

    hints = defaultdict(list)
    pseudo_files = list(pseudo_dir.glob("*.c")) + list(pseudo_dir.glob("*.cpp"))
    print(f"  Mining {len(pseudo_files)} pseudocode files...")

    for path in pseudo_files:
        try:
            text = path.read_text(errors='replace')
        except Exception:
            continue
        for m in RE_PSEUDO_FIELD.finditer(text):
            type_hint = m.group(1)
            offset = int(m.group(2))
            if 0 < offset < 0x2000:
                hints[offset].append(type_hint)

    return hints


# ─────────────────────────────────────────────────────────────────────────────
# Report + header generation
# ─────────────────────────────────────────────────────────────────────────────

def infer_field_type(width_counts):
    """Pick C type from the dominant access width."""
    if not width_counts:
        return "uint32_t"
    dominant = max(width_counts, key=width_counts.get)
    return {"U8":"uint8_t","U16":"uint16_t","U32":"uint32_t","S32":"int32_t",
            "F32":"float","U64":"uint64_t","S64":"int64_t"}.get(dominant, "uint32_t")


def generate_report(cluster, top_n):
    totals = {cls: sum(e["count"] for e in offs.values()) for cls, offs in cluster.items()}
    ranked = sorted(totals.items(), key=lambda x: -x[1])
    print(f"\n  Top {top_n} classes by field-access count:")
    print(f"  {'Class':<38} {'Accesses':>10}  {'Offsets':>7}")
    print("  " + "-" * 60)
    for cls, total in ranked[:top_n]:
        print(f"  {cls:<38} {total:>10,}  {len(cluster[cls]):>7}")
    return ranked


def generate_struct_header(cluster, top_classes, pseudo_hints):
    lines = [
        "// AUTO-GENERATED by phase4b_offset_cluster.py — DO NOT EDIT",
        "// Field offsets inferred from PPC_LOAD/STORE frequency clustering across pass5_ph4",
        "// Stars: ★★★ = 50+ accesses, ★★☆ = 10-49, ★☆☆ = <10",
        "// Validate every offset before using in non-recomp code!",
        "#pragma once",
        "#include <cstdint>",
        "",
        "// Forward declarations",
    ]
    fwd = sorted(set(cls for cls, _ in top_classes[:30]))
    lines += [f"struct {cls};" for cls in fwd]
    lines += ["", "// " + "─" * 70, ""]

    for cls, total in top_classes[:30]:
        offsets = cluster[cls]
        if not offsets:
            continue
        # Top-12 most-accessed offsets, then sort by offset value
        top12 = sorted(sorted(offsets.items(), key=lambda x: -x[1]["count"])[:12],
                       key=lambda x: x[0])

        lines.append(f"// {cls}  (total accesses: {total:,}, {len(offsets)} unique offsets)")
        lines.append(f"struct {cls} {{")
        lines.append(f"    void*          vtable;           // +0x0000  (vtable pointer)")
        lines.append(f"    uint32_t       flags;            // +0x0004  (object flags)")

        for off, entry in top12:
            if off < 8:
                continue
            count = entry["count"]
            ftype = infer_field_type(entry["widths"])
            stars = "★★★" if count >= 50 else ("★★☆" if count >= 10 else "★☆☆")
            ph = ""
            if off in pseudo_hints:
                ph = "  /* " + "/".join(sorted(set(pseudo_hints[off]))[:2]) + " */"
            reads = entry["ops"].get("LOAD", 0)
            writes = entry["ops"].get("STORE", 0)
            lines.append(
                f"    {ftype:<14} field_{off:#06x};  "
                f"// +{off:#06x}  ({count} accesses {stars}, R:{reads}/W:{writes}){ph}"
            )

        remaining = len(offsets) - 2 - len([o for o, _ in top12 if o >= 8])
        if remaining > 0:
            lines.append(f"    // ... {remaining} more offsets in offset_cluster_map.json")
        lines.append("};")
        lines.append("")

    return "\n".join(lines) + "\n"


# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--top",   type=int, default=20)
    ap.add_argument("--class", dest="cls", default=None, help="Dump detail for one class")
    args = ap.parse_args()

    print("=" * 60)
    print("Phase 4B: Struct Offset Clustering")
    print("=" * 60)

    source_dir = PH4_DIR if PH4_DIR.exists() else PASS5_DIR
    print(f"  Source: {source_dir}")

    cluster = mine_cpp_files(source_dir)
    print(f"  Classes with data: {len(cluster)}")

    pseudo_hints = mine_pseudocode(PSEUDO_DIR)
    print(f"  Pseudocode type hints at {len(pseudo_hints)} unique offsets")

    # Serialize to JSON (convert defaultdicts)
    json_out = CFG / "offset_cluster_map.json"
    out_data = {}
    for cls, offs in cluster.items():
        out_data[cls] = {}
        for off, entry in sorted(offs.items()):
            out_data[cls][str(off)] = {
                "hex": hex(off),
                "count": entry["count"],
                "widths": dict(entry["widths"]),
                "ops":    dict(entry["ops"]),
            }
    json_out.write_text(json.dumps(out_data, indent=2))
    print(f"  JSON saved: {json_out}")

    top_classes = generate_report(cluster, args.top)

    # Detailed dump
    if args.cls:
        cls = args.cls
        if cls in cluster:
            print(f"\n  === Offset detail for {cls} ===")
            print(f"  {'Hex':>8}  {'Dec':>6}  {'Count':>6}  {'Type':>10}  R/W")
            for off, entry in sorted(cluster[cls].items(), key=lambda x: -x[1]["count"])[:30]:
                ftype = infer_field_type(entry["widths"])
                r = entry["ops"].get("LOAD", 0)
                w = entry["ops"].get("STORE", 0)
                print(f"  {off:#010x}  {off:>6}  {entry['count']:>6}  {ftype:>10}  R:{r} W:{w}")
        else:
            print(f"  Class '{cls}' not found (available: {sorted(cluster)[:5]}...)")

    # Update generated_structs.h
    struct_path = INCLUDE_DIR / "generated_structs.h"
    struct_path.write_text(generate_struct_header(cluster, top_classes, pseudo_hints))
    print(f"\n  Struct header updated: {struct_path}")

    print()
    print("=" * 60)
    print("Phase 4B Complete")
    print("=" * 60)


if __name__ == "__main__":
    main()
