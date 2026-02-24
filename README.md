# Rockstar Table Tennis Decompilation

AI-assisted decompilation of **Rockstar Presents Table Tennis** (Xbox 360, 2006) using Claude and the Model Context Protocol (MCP).

## Project Status

- **16,653 functions** identified and named (100% coverage)
- **631 functions** implemented (3.79% complete)
- **PowerPC → C++** translation using AI-assisted workflow
- **Cross-platform** target: Windows, macOS, Linux (SDL2 + OpenGL 3.3)

## How to Contribute

This project uses an **MCP server** to give AI models (Claude recommended) direct access to decompilation data and tools. Contributors work with Claude to lift functions from assembly into clean C++.

### Prerequisites

1. **Claude Desktop** (or any MCP-compatible IDE)
2. **Node.js** (for the MCP server)
3. **Your own copy** of Rockstar Table Tennis (Xbox 360)

### Setup

1. Clone this repository:
```bash
git clone https://github.com/yourusername/tt-decomp.git
cd tt-decomp
```

2. Install the MCP server:
```bash
cd tools/mcp-decomp
npm install
```

3. Configure Claude Desktop by adding to `~/Library/Application Support/Claude/claude_desktop_config.json`:
```json
{
  "mcpServers": {
    "tt-decomp-phase5": {
      "command": "node",
      "args": ["/absolute/path/to/tt-decomp/tools/mcp-decomp/server.js"]
    }
  }
}
```

4. Restart Claude Desktop

### The AI-Assisted Workflow

Once configured, Claude has access to 9 specialized tools:

| Tool | Purpose |
|------|---------|
| `get_function_info` | Get function metadata (address, size, source file, class) |
| `get_class_context` | Get class vtable, struct fields, and methods |
| `get_function_recomp` | Get raw static-recomp scaffold code |
| `get_function_pseudocode` | Get IDA Hex-Rays pseudocode (cleaner view) |
| `find_callers` | See what calls this function |
| `find_callees` | See what this function calls |
| `search_symbols` | Search 52,691 symbols by name |
| `get_existing_source` | Read current lifted code |
| `write_source_file` | Write finished C++ to src/ |

**Typical workflow:**
1. Ask Claude: "Lift function `pongPlayer_Update`"
2. Claude uses the tools to understand the function
3. Claude writes clean, idiomatic C++ to `src/`
4. You review, test, and commit

### What Makes a Good Contribution

- **Clean C++**: No raw offsets, no register names, structured control flow
- **Meaningful names**: `player->recoveryTimer` not `*(ptr + 0x1C)`
- **Proper types**: Use structs, not `void*` everywhere
- **Comments**: Explain what the code does, reference original address
- **ABI preservation**: Match original calling conventions and struct layouts

See `CODING_STYLE.md` for detailed guidelines.

## Project Structure

```
tt-decomp/
├── src/              # Decompiled C++ source code
├── include/          # Header files
├── config/434C4803/  # Symbol maps, vtables, struct data
├── tools/mcp-decomp/ # MCP server for AI assistance
├── CMakeLists.txt    # Build configuration
└── CODING_STYLE.md   # Contribution guidelines
```

## Building

```bash
mkdir build && cd build
cmake ..
ninja
```

## Resources

- **MCP Server**: `tools/mcp-decomp/` - AI decompilation assistant
- **Coding Style**: `CODING_STYLE.md` - Code quality standards
- **Reference Projects**: [re3](https://github.com/GTAmodding/re3), [reVC](https://github.com/GTAmodding/re-VC) - Similar decompilation efforts

## Community

- **Discord**: [GC/Wii Decompilation](https://discord.gg/hKx3FJJgrV) (PowerPC architecture discussion)
- **Tools**: [objdiff](https://github.com/encounter/objdiff), [decomp.me](https://decomp.me)

## Legal

This project is for educational and preservation purposes. You must own a legal copy of Rockstar Presents Table Tennis to use this software. No game assets are included in this repository.
