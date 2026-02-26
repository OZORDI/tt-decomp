# tt-decomp Phase 5 MCP Server

MCP server that provides an LLM agent all the tools needed to lift Rockstar Table Tennis functions from a static-recomp scaffold into clean, idiomatic C++.

## Setup

```bash
cd tools/mcp-decomp
npm install
```

## Add to Claude Desktop

Add to `~/Library/Application Support/Claude/claude_desktop_config.json`:

```json
{
  "mcpServers": {
    "tt-decomp-phase5": {
      "command": "node",
      "args": ["/Users/Ozordi/Downloads/tt-decomp/tools/mcp-decomp/server.js"]
    }
  }
}
```

Restart Claude Desktop after editing the config.

## Tools (11 total)

| Tool | Purpose |
|------|---------|
| `suggest_unimplemented_class` | Call at session start. Picks a random unimplemented class family from the RTTI map, cross-references against src/, and returns the full hierarchy to implement together. Optional `prefix` parameter filters by subsystem (e.g. `"rage::sn"`, `"pong"`). |
| `get_function_info` | Address, size, source file, class, vtable slot, lift status, RTTI-verified class name. |
| `get_class_context` | RTTI name and inheritance chain, vtable layout, struct field clusters, debug string hints, known methods. Call once per class — covers the entire family. |
| `get_function_recomp` | Raw pass5_final scaffold with lbl_ references auto-annotated. |
| `get_function_pseudocode` | IDA Hex-Rays pseudocode when available. |
| `resolve_address` | Decodes any address: symbol name, section, size, RTTI vtable class, string content, SDA global. Required before naming any lbl_ reference. |
| `find_callers` | All functions in pass5_final that directly call the given function. |
| `find_callees` | All functions called by the given function. |
| `search_symbols` | Substring search across 52,691 symbols and RTTI class names. |
| `write_source_file` | Surgical editor for src/ files. Modes: create, append, insert_at_line, replace_lines, delete_lines. No full-overwrite. |
| `get_existing_source` | Read existing lifted code by file path or function name. |
| `suggest_file_placement` | Returns the correct .hpp and .cpp paths for a class, with sibling classes listed. Call before write_source_file. |

## Session Workflow

### Starting a session

The agent calls `suggest_unimplemented_class()` at the start of every session without prompting. It picks a random class family and begins immediately.

To filter by subsystem, pass a prefix:

```
suggest_unimplemented_class({ prefix: "rage::sn" })
suggest_unimplemented_class({ prefix: "pong" })
```

### Per-function loop

1. `get_class_context` on the root class — once for the whole family
2. `get_function_info` — orientation for each function
3. `get_function_recomp` + `get_function_pseudocode` — understand the scaffold
4. `resolve_address` on every unfamiliar `lbl_XXXXXXXX`
5. `find_callers` / `find_callees` / `search_symbols` as needed
6. `get_existing_source` — check current state of target file
7. `suggest_file_placement` — confirm correct output paths
8. `write_source_file` — write the clean C++
9. `git add -A && git commit && git push` — commit at end of session

### Why implement whole families at once

The RTTI map contains nested hierarchies where sub-classes share struct layout, vtable entries, and state machine context. Lifting an entire family in one session avoids reloading context and produces more coherent output.

Example: selecting `rage::snJoinMachine` also surfaces:
```
rage::snJoinMachine::snHsmLocalJoinPending
rage::snJoinMachine::snHsmRequestingJoin
rage::snJoinMachine::snHsmRequestingJoin::snHsmWaitingForReply
rage::snJoinMachine::snHsmRequestingJoin::snHsmJoinPending
```

## Data Sources

| Path | Contents |
|------|----------|
| `config/434C4803/master_symbol_map.json` | 52,691 symbols with addresses |
| `config/434C4803/splits_addr_map.json` | Address ranges mapped to source files |
| `config/434C4803/vtable_layout_map.json` | 1,115 class vtable layouts |
| `config/434C4803/offset_cluster_map.json` | Struct field frequency clusters |
| `config/434C4803/debug_string_field_map.json` | Field names inferred from debug strings |
| `config/434C4803/rtti_vtable_map.json` | 1,266 RTTI class names mapped to vtable addresses |
| `config/434C4803/rtti_class_hierarchy.json` | Inheritance chains |
| `config/434C4803/sda_resolution_map.json` | r2/r13 SDA global names |
| `orig/434C4803/default_base.bin` | Full binary for string reading |
| `recomp/structured_pass5_final/` | 16,934 functions — the scaffold |
| `orig/pseudocode/` | 13,125 IDA pseudocode files |

## System Prompt

See `SYSTEM_PROMPT.md` for the complete agent instructions. Paste into Claude Desktop's Project Instructions field.
