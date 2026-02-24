# tools/analysis/ — Binary Analysis Scripts

One-off analysis tools used during the decompilation research process.

| Script | Purpose |
|---|---|
| `analyze2.py` | Early vtable + callgraph analysis |
| `analyze3.py` | Symbol cross-reference analysis |
| `analyze4.py` | RTTI class hierarchy extraction |
| `analyze5.py` | Coverage statistics |
| `analyze5b.py` | Improved coverage + class distribution |
| `analyze_vtables_usage.py` | Vtable slot usage mapping |
| `dump_vtables.py` | Dump raw vtable data from binary |
| `find_vtables.py` | Locate vtable structures in .rdata |
| `parse_pdata.py` | Parse .pdata section for function metadata |

These scripts are research tools, not part of the main build pipeline.
The `naming/auto_namer_v8.py` is the current production naming script.
