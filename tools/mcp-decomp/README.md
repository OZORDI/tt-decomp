# tt-decomp Phase 5 MCP Server

MCP server that gives an LLM agent all the tools it needs to lift Rockstar Table Tennis functions from the static-recomp scaffold into clean, idiomatic C++.

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

Then restart Claude Desktop.

## Tools (9 total)

| Tool | Purpose |
|------|---------|
| `get_function_info` | Address, size, source file, class, vtable slot, lift status |
| `get_class_context` | Vtable, struct field clusters, debug hints, methods |
| `get_function_recomp` | Raw pass5_final scaffold for a function |
| `get_function_pseudocode` | IDA Hex-Rays pseudocode (when available) |
| `find_callers` | Which functions call this one |
| `find_callees` | Which functions this one calls |
| `search_symbols` | Fuzzy search across 52,691 symbols |
| `write_source_file` | Write lifted C++ to src/ |
| `get_existing_source` | Read existing lifted code |

## The Agent Workflow

The model should:
1. `get_function_info` → orientation
2. `get_class_context` → class knowledge (once per class)
3. `get_function_recomp` + `get_function_pseudocode` → understand the code
4. Optionally `find_callers`/`find_callees`/`search_symbols` for context
5. `get_existing_source` → check current target file
6. `write_source_file` → write clean C++ output

## Data Sources

All data is read from the project's config directory:
- `config/434C4803/master_symbol_map.json` — 52,691 symbols with addresses
- `config/434C4803/splits_addr_map.json` — Address ranges → source files
- `config/434C4803/vtable_layout_map.json` — 1,115 class vtable layouts
- `config/434C4803/offset_cluster_map.json` — Struct field frequency clusters
- `config/434C4803/debug_string_field_map.json` — Field names from debug strings
- `recomp/structured_pass5_final/` — 16,934 functions (the scaffold)
- `orig/pseudocode/` — 13,125 IDA pseudocode files

## System Prompt

See `SYSTEM_PROMPT.md` for the complete agent instructions. Paste into Claude Desktop's "Project Instructions" or the system prompt field.
