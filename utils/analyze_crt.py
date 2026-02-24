#!/usr/bin/env python3
"""
Strict CRT signature analyzer for Table Tennis (Xbox 360).

This script reads:
  - build/434C4803/obj/auto_03_820C0000_text.obj
  - config/434C4803/symbols.txt

It validates strict instruction signatures for selected CRT candidates and,
optionally, applies safe symbol renames for confirmed matches.
"""

from __future__ import annotations

import argparse
import bisect
import dataclasses
import re
import struct
import sys
from collections import Counter
from pathlib import Path
from typing import Callable, Dict, Iterable, List, Optional, Sequence, Tuple


TEXT_BASE = 0x820C0000
REL24_TYPE = 0x0006


@dataclasses.dataclass(frozen=True)
class FunctionSymbol:
    name: str
    address: int
    size: int
    line_index: int
    line_text: str


@dataclasses.dataclass(frozen=True)
class CoffSymbol:
    name: str
    value: int
    section_number: int
    sym_type: int
    storage_class: int
    aux_count: int


@dataclasses.dataclass(frozen=True)
class CoffSection:
    name: str
    raw_size: int
    raw_ptr: int
    rel_ptr: int
    rel_count_field: int


@dataclasses.dataclass(frozen=True)
class Relocation:
    virtual_address: int
    symbol_index: int
    rel_type: int


@dataclasses.dataclass(frozen=True)
class CandidateRule:
    source_name: str
    target_name: str
    target_address: int
    target_size: int
    validator: Callable[[Sequence[int]], Tuple[bool, str]]


@dataclasses.dataclass(frozen=True)
class CandidateResult:
    source_name: str
    target_name: str
    target_address: int
    matched: bool
    reason: str


FUNCTION_LINE_RE = re.compile(
    r"^(?P<name>\S+) = \.text:0x(?P<addr>[0-9A-Fa-f]+); // type:function size:0x(?P<size>[0-9A-Fa-f]+)"
)


def read_u16(data: bytes, offset: int) -> int:
    return struct.unpack_from("<H", data, offset)[0]


def read_i16(data: bytes, offset: int) -> int:
    return struct.unpack_from("<h", data, offset)[0]


def read_u32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<L", data, offset)[0]


def parse_symbols(symbols_path: Path) -> Tuple[List[str], List[FunctionSymbol]]:
    lines = symbols_path.read_text(encoding="utf-8", errors="ignore").splitlines()
    functions: List[FunctionSymbol] = []
    seen_names: set[str] = set()
    duplicates: List[str] = []

    for idx, line in enumerate(lines):
        match = FUNCTION_LINE_RE.match(line)
        if not match:
            continue
        name = match.group("name")
        address = int(match.group("addr"), 16)
        size = int(match.group("size"), 16)
        if name in seen_names:
            duplicates.append(name)
        seen_names.add(name)
        functions.append(
            FunctionSymbol(
                name=name,
                address=address,
                size=size,
                line_index=idx,
                line_text=line,
            )
        )

    if duplicates:
        dup_str = ", ".join(sorted(set(duplicates)))
        raise ValueError(f"Duplicate symbol names in symbols.txt: {dup_str}")

    return lines, functions


def decode_name(name8: bytes, string_table: bytes) -> str:
    zeroes, str_off = struct.unpack_from("<LL", name8, 0)
    if zeroes == 0 and str_off != 0:
        # String table offsets include the 4-byte table length field.
        start = str_off - 4
        if start < 0 or start >= len(string_table):
            return ""
        end = string_table.find(b"\0", start)
        if end == -1:
            return ""
        return string_table[start:end].decode("ascii", errors="replace")
    return name8.split(b"\0", 1)[0].decode("ascii", errors="replace")


def parse_coff_symbols(obj_data: bytes) -> List[CoffSymbol]:
    pointer_to_symbol_table = read_u32(obj_data, 8)
    number_of_symbols = read_u32(obj_data, 12)
    string_table_offset = pointer_to_symbol_table + number_of_symbols * 18
    string_table_length = read_u32(obj_data, string_table_offset)
    string_table = obj_data[string_table_offset + 4 : string_table_offset + string_table_length]

    symbols: List[CoffSymbol] = []
    i = 0
    while i < number_of_symbols:
        entry_off = pointer_to_symbol_table + i * 18
        name = decode_name(obj_data[entry_off : entry_off + 8], string_table)
        value = read_u32(obj_data, entry_off + 8)
        section_number = read_i16(obj_data, entry_off + 12)
        sym_type = read_u16(obj_data, entry_off + 14)
        storage_class = obj_data[entry_off + 16]
        aux_count = obj_data[entry_off + 17]
        symbols.append(
            CoffSymbol(
                name=name,
                value=value,
                section_number=section_number,
                sym_type=sym_type,
                storage_class=storage_class,
                aux_count=aux_count,
            )
        )
        i += 1 + aux_count
    return symbols


def parse_sections(obj_data: bytes) -> List[CoffSection]:
    number_of_sections = read_u16(obj_data, 2)
    optional_header_size = read_u16(obj_data, 16)
    section_table_offset = 20 + optional_header_size

    sections: List[CoffSection] = []
    for i in range(number_of_sections):
        off = section_table_offset + i * 40
        name = obj_data[off : off + 8].split(b"\0", 1)[0].decode("ascii", errors="replace")
        raw_size = read_u32(obj_data, off + 16)
        raw_ptr = read_u32(obj_data, off + 20)
        rel_ptr = read_u32(obj_data, off + 24)
        rel_count_field = read_u16(obj_data, off + 32)
        sections.append(
            CoffSection(
                name=name,
                raw_size=raw_size,
                raw_ptr=raw_ptr,
                rel_ptr=rel_ptr,
                rel_count_field=rel_count_field,
            )
        )
    return sections


def parse_relocations(obj_data: bytes, section: CoffSection) -> List[Relocation]:
    rel_ptr = section.rel_ptr
    rel_count_field = section.rel_count_field

    # Overflow rule: 0xFFFF in section header, then first relocation entry
    # stores the actual relocation count in its VirtualAddress field.
    if rel_count_field == 0xFFFF:
        actual_count = read_u32(obj_data, rel_ptr)
        rel_start = rel_ptr + 10
        rel_count = actual_count - 1
    else:
        rel_start = rel_ptr
        rel_count = rel_count_field

    rels: List[Relocation] = []
    for i in range(rel_count):
        off = rel_start + i * 10
        virtual_address = read_u32(obj_data, off)
        symbol_index = read_u32(obj_data, off + 4)
        rel_type = read_u16(obj_data, off + 8)
        rels.append(
            Relocation(
                virtual_address=virtual_address,
                symbol_index=symbol_index,
                rel_type=rel_type,
            )
        )
    return rels


def words_from_func(
    obj_data: bytes,
    text_section: CoffSection,
    func_addr: int,
    func_size: int,
) -> List[int]:
    start = text_section.raw_ptr + (func_addr - TEXT_BASE)
    end = start + func_size
    if start < text_section.raw_ptr or end > text_section.raw_ptr + text_section.raw_size:
        raise ValueError(
            f"Function 0x{func_addr:08X} range 0x{func_size:X} lies outside .text raw data"
        )
    out: List[int] = []
    for off in range(start, end, 4):
        out.append(struct.unpack_from(">I", obj_data, off)[0])
    return out


def has_all(words: Sequence[int], required: Iterable[int]) -> bool:
    word_set = set(words)
    for item in required:
        if item not in word_set:
            return False
    return True


def validate_exact(words: Sequence[int], expected: Sequence[int], label: str) -> Tuple[bool, str]:
    if list(words) != list(expected):
        return False, f"{label}: exact instruction sequence mismatch"
    return True, f"{label}: exact instruction sequence matched"


def validate_strncpy(words: Sequence[int]) -> Tuple[bool, str]:
    expected = [
        0x7C6B1B78, 0x2B050000, 0x4D9A0020, 0x89440000, 0x38840001, 0x280A0000,
        0x994B0000, 0x396B0001, 0x4182000C, 0x34A5FFFF, 0x4082FFE4, 0x2B050000,
        0x4D9A0020, 0x3545FFFF, 0x4D820020, 0x39200000, 0x280A0000, 0x4D820020,
        0x7D4903A6, 0x992B0000, 0x396B0001, 0x4200FFF8, 0x4E800020,
    ]
    ok, reason = validate_exact(words, expected, "strncpy")
    if not ok:
        return ok, reason
    # Scenario check: null-pad loop via mtctr + bdnz with explicit zero store.
    if not has_all(words, [0x7D4903A6, 0x4200FFF8, 0x992B0000]):
        return False, "strncpy: null-pad loop pattern missing"
    return True, reason + " + null-pad loop verified"


def validate_strncmp(words: Sequence[int]) -> Tuple[bool, str]:
    expected = [
        0x7CA02B79, 0x7CA903A6, 0x4081003C, 0x89030000, 0x88E40000, 0x7C6A1B78,
        0x2C880000, 0x7C674011, 0x40060008, 0x4E800020, 0x4C820020, 0x8D0A0001,
        0x8CE40001, 0x2C880000, 0x7C674011, 0x4006FFEC, 0x4E800020, 0x38600000,
        0x4E800020,
    ]
    ok, reason = validate_exact(words, expected, "strncmp")
    if not ok:
        return ok, reason
    # Scenario check: count-limited compare via mtctr setup and loop branch.
    if not has_all(words, [0x7CA903A6, 0x4006FFEC, 0x38600000]):
        return False, "strncmp: count-limited loop pattern missing"
    return True, reason + " + count-limited compare loop verified"


def validate_strcmp(words: Sequence[int]) -> Tuple[bool, str]:
    expected = [
        0x88A30000, 0x88C40000, 0x7C691B78, 0x2F860000, 0x7C662851, 0x419E0014,
        0x40820010, 0x8CA90001, 0x8CC40001, 0x4BFFFFE8, 0x4D820020, 0x3860FFFF,
        0x4D800020, 0x38600001, 0x4E800020,
    ]
    ok, reason = validate_exact(words, expected, "strcmp")
    if not ok:
        return ok, reason
    # Scenario check: tri-state return (+1 / -1 / 0 via condition returns).
    if not has_all(words, [0x3860FFFF, 0x38600001, 0x4D820020, 0x4D800020]):
        return False, "strcmp: tri-state return pattern missing"
    return True, reason + " + tri-state compare verified"


def validate_stricmp(words: Sequence[int]) -> Tuple[bool, str]:
    expected = [
        0x3923FFFF, 0x3884FFFF, 0x8CC40001, 0x8CA90001, 0x2F860000, 0x7C662851,
        0x419E0038, 0x4182FFEC, 0x2E860041, 0x2F06005A, 0x4194000C, 0x41990008,
        0x60C60020, 0x2C050041, 0x2C85005A, 0x4180000C, 0x41850008, 0x60A50020,
        0x7C662851, 0x4182FFBC, 0x4E800020,
    ]
    ok, reason = validate_exact(words, expected, "_stricmp")
    if not ok:
        return ok, reason
    # Scenario check: uppercase A..Z normalization on both chars with ORI 0x20.
    if not has_all(words, [0x2E860041, 0x2F06005A, 0x60C60020, 0x2C050041, 0x2C85005A, 0x60A50020]):
        return False, "_stricmp: ASCII case-fold pattern missing"
    return True, reason + " + ASCII normalization verified"


def get_candidate_rules() -> List[CandidateRule]:
    return [
        CandidateRule(
            source_name="fn_82430120",
            target_name="strncpy",
            target_address=0x82430120,
            target_size=0x5C,
            validator=validate_strncpy,
        ),
        CandidateRule(
            source_name="fn_824320D0",
            target_name="strncmp",
            target_address=0x824320D0,
            target_size=0x4C,
            validator=validate_strncmp,
        ),
        CandidateRule(
            source_name="fn_82432970",
            target_name="strcmp",
            target_address=0x82432970,
            target_size=0x3C,
            validator=validate_strcmp,
        ),
        CandidateRule(
            source_name="fn_82433A40",
            target_name="_stricmp",
            target_address=0x82433A40,
            target_size=0x54,
            validator=validate_stricmp,
        ),
    ]


def build_branch_maps(
    functions: Sequence[FunctionSymbol],
    relocs: Sequence[Relocation],
    coff_symbols: Sequence[CoffSymbol],
) -> Tuple[Counter[str], int]:
    functions_sorted = sorted(functions, key=lambda f: f.address)
    starts = [f.address for f in functions_sorted]
    inbound = Counter()
    mapped_sources = 0

    for rel in relocs:
        if rel.rel_type != REL24_TYPE:
            continue
        if rel.symbol_index >= len(coff_symbols):
            continue
        target_name = coff_symbols[rel.symbol_index].name
        if target_name:
            inbound[target_name] += 1

        src_addr = TEXT_BASE + rel.virtual_address
        idx = bisect.bisect_right(starts, src_addr) - 1
        if idx < 0:
            continue
        src = functions_sorted[idx]
        if src_addr < src.address + src.size:
            mapped_sources += 1

    return inbound, mapped_sources


def analyze(
    obj_path: Path,
    symbols_path: Path,
) -> Tuple[List[str], List[FunctionSymbol], List[CandidateResult], Counter[str], int, int]:
    lines, functions = parse_symbols(symbols_path)
    func_by_name = {f.name: f for f in functions}
    func_by_addr = {f.address: f for f in functions}

    obj_data = obj_path.read_bytes()
    sections = parse_sections(obj_data)
    text_section = next((s for s in sections if s.name == ".text"), None)
    if text_section is None:
        raise ValueError("No .text section found in COFF object")

    coff_symbols = parse_coff_symbols(obj_data)
    relocs = parse_relocations(obj_data, text_section)
    rel24_total = sum(1 for rel in relocs if rel.rel_type == REL24_TYPE)
    inbound_calls, mapped_source_calls = build_branch_maps(functions, relocs, coff_symbols)

    results: List[CandidateResult] = []
    for rule in get_candidate_rules():
        src = func_by_name.get(rule.source_name)
        resolved_name = rule.source_name
        if src is None:
            # Allow post-rename validation: same address now bound to target CRT name.
            src = func_by_name.get(rule.target_name)
            resolved_name = rule.target_name
        if src is None:
            # Final fallback by address only.
            src = func_by_addr.get(rule.target_address)
            resolved_name = src.name if src is not None else rule.source_name
        if src is None:
            results.append(
                CandidateResult(
                    source_name=rule.source_name,
                    target_name=rule.target_name,
                    target_address=rule.target_address,
                    matched=False,
                    reason=(
                        f"{rule.source_name}/{rule.target_name}: missing from symbols.txt "
                        f"at 0x{rule.target_address:08X}"
                    ),
                )
            )
            continue
        if src.address != rule.target_address:
            results.append(
                CandidateResult(
                    source_name=rule.source_name,
                    target_name=rule.target_name,
                    target_address=rule.target_address,
                    matched=False,
                    reason=(
                        f"{rule.source_name}: address mismatch; expected 0x{rule.target_address:08X}, "
                        f"found 0x{src.address:08X}"
                    ),
                )
            )
            continue
        if src.size != rule.target_size:
            results.append(
                CandidateResult(
                    source_name=rule.source_name,
                    target_name=rule.target_name,
                    target_address=rule.target_address,
                    matched=False,
                    reason=(
                        f"{rule.source_name}: size mismatch; expected 0x{rule.target_size:X}, "
                        f"found 0x{src.size:X}"
                    ),
                )
            )
            continue

        words = words_from_func(obj_data, text_section, src.address, src.size)
        matched, reason = rule.validator(words)
        if matched and resolved_name == rule.target_name:
            reason = reason + " (post-rename symbol verified)"
        results.append(
            CandidateResult(
                source_name=rule.source_name,
                target_name=rule.target_name,
                target_address=rule.target_address,
                matched=matched,
                reason=reason,
            )
        )

    # Ensure candidates can still be looked up by address.
    for rule in get_candidate_rules():
        if rule.target_address not in func_by_addr:
            raise ValueError(f"Address 0x{rule.target_address:08X} not found in symbols.txt")

    return lines, functions, results, inbound_calls, rel24_total, mapped_source_calls


def apply_renames(
    symbols_lines: List[str],
    functions: Sequence[FunctionSymbol],
    results: Sequence[CandidateResult],
    symbols_path: Path,
) -> int:
    rename_map: Dict[str, str] = {}
    by_name = {f.name: f for f in functions}
    by_addr = {f.address: f for f in functions}
    for result in results:
        if result.matched:
            # If already renamed at the target address, treat as idempotent no-op.
            at_addr = by_addr.get(result.target_address)
            if at_addr is not None and at_addr.name == result.target_name:
                continue
            if result.source_name in by_name:
                rename_map[result.source_name] = result.target_name

    if not rename_map:
        return 0

    existing_names = {f.name for f in functions}
    for old, new in rename_map.items():
        # If target name exists at the same address, skip as idempotent.
        existing_target = by_name.get(new)
        existing_old = by_name.get(old)
        if (
            existing_target is not None
            and existing_old is not None
            and existing_target.address == existing_old.address
        ):
            continue
        if new in existing_names and new != old:
            raise ValueError(f"Refusing rename {old} -> {new}: target name already exists")

    updated = list(symbols_lines)
    changed = 0
    for f in functions:
        if f.name not in rename_map:
            continue
        old = f.name
        new = rename_map[old]
        updated_line = f.line_text.replace(old, new, 1)
        if updated_line != updated[f.line_index]:
            updated[f.line_index] = updated_line
            changed += 1

    if changed > 0:
        symbols_path.write_text("\n".join(updated) + "\n", encoding="utf-8")
    return changed


def main() -> int:
    parser = argparse.ArgumentParser(description="Strict Table Tennis CRT analyzer")
    parser.add_argument(
        "--obj",
        type=Path,
        default=Path("build/434C4803/obj/auto_03_820C0000_text.obj"),
        help="Path to split .text object",
    )
    parser.add_argument(
        "--symbols",
        type=Path,
        default=Path("config/434C4803/symbols.txt"),
        help="Path to symbols.txt",
    )
    parser.add_argument(
        "--apply",
        action="store_true",
        help="Apply confirmed renames to symbols.txt",
    )
    args = parser.parse_args()

    try:
        symbols_lines, functions, results, inbound_calls, rel24_total, mapped_source_calls = analyze(
            args.obj,
            args.symbols,
        )
    except Exception as exc:
        print(f"[error] {exc}", file=sys.stderr)
        return 1

    print("CRT strict analysis report")
    print(f"  object:  {args.obj}")
    print(f"  symbols: {args.symbols}")
    print(f"  rel24 relocations: {rel24_total}")
    print(f"  rel24 source mappings resolved: {mapped_source_calls}")
    print("")

    all_matched = True
    for result in results:
        status = "MATCH" if result.matched else "SKIP"
        if not result.matched:
            all_matched = False
        inbound = inbound_calls.get(result.source_name, 0)
        print(
            f"[{status}] 0x{result.target_address:08X} {result.source_name} -> {result.target_name} "
            f"(inbound_calls={inbound})"
        )
        print(f"  {result.reason}")

    print("")
    print("Unresolved by design (unchanged): strcpy, wcslen, _purecall")

    if args.apply:
        try:
            changed = apply_renames(symbols_lines, functions, results, args.symbols)
        except Exception as exc:
            print(f"[error] apply failed: {exc}", file=sys.stderr)
            return 1
        print("")
        print(f"Applied renames: {changed}")
        if changed == 0:
            print("No changes written (no strict matches or already renamed).")

    return 0 if all_matched else 2


if __name__ == "__main__":
    raise SystemExit(main())
