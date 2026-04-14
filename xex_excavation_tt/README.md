# XEX Excavation — Authoritative Reference Database

## ✅ This is the canonical source for VA → content lookups

`tt.bin` in this directory is the **post-load mapped image** of the Xbox 360 binary. File offset `N` corresponds to virtual address `0x82000000 + N` — making it the correct file to read when you need to know what byte/string lives at a given virtual address.

## Verification

This was confirmed by cross-checking 5 known-correct debug strings from `debug_strings.txt` against both `tt.bin` and `orig/434C4803/default_base.bin`:

| Source | Matches |
|-|-|
| **`tt.bin` (this dir)** | **5/5 ✅** |
| `orig/434C4803/default_base.bin` | 0/5 ❌ (raw file layout, not mapped image) |

Cross-confirmed against recomp output: function code at `0x82038538` references the string `"*** Unknown option '%s' on command line."` (argv error handler) — which is what `tt.bin` contains at that VA, but `default_base.bin` contains unrelated audio cue data.

## File inventory

| File | Size | Purpose |
|-|-|-|
| **`tt.bin`** | **7.8 MB** | **Mapped image — use this for VA→content lookup** |
| `tt_decrypted.xex` | 6.4 MB | Decrypted XEX (same content as `orig/434C4803/default_base.RAW_FILE_NOT_FOR_VA_LOOKUP.bin`) |
| `tt_decrypted.xex.asm` | 86 MB | Full IDA Pro disassembly listing |
| `tt_decrypted.xex.i64` | — | IDA Pro database |
| `debug_strings.txt` | 231 lines | Extracted debug/log strings with VAs |
| `assert_strings.txt` | 414 lines | Assert/error messages with VAs |
| `format_strings.txt` | 1,375 lines | printf-style format strings with VAs |
| `all_strings_with_addrs.txt` | 22,614 lines | Complete string table |
| `vtables_with_addrs.txt` | 19,907 lines | All vtable addresses + slot functions |
| `rtti_classes.txt` | 1,777 lines | All RTTI type descriptors |
| ... | | (see SYSTEM_PROMPT_BATCH.md for full catalog) |

## How to read tt.bin from Python

```python
addr = 0x8202762C  # virtual address
load_base = 0x82000000
with open('xex_excavation_tt/tt.bin', 'rb') as f:
    f.seek(addr - load_base)
    data = f.read(64)
end = data.find(b'\x00')
string = data[:end].decode('ascii', errors='replace')
print(string)  # → "debugface"
```

## What NOT to use

- `orig/434C4803/default_base.bin` — raw file, NOT mapped (see warning README in that dir)
- `orig/434C4803/default.xex` — encrypted, only useful as recomp tool input
