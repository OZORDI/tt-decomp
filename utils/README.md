# Utils

This directory contains analysis tools, naming scripts, and decompilation utilities.

## Subdirectories

- `analysis/` - Binary analysis scripts for function identification
- `naming/` - Automated function naming tools (v1-v17)
- `decompiled_wip/` - Work-in-progress decompiled functions

## Key Scripts

- `project.py` - Main project configuration and build system
- `auto_name.py` - Automated function naming
- `analyze_crt.py` - CRT function analysis
- `build_chat_pdf.py` - Documentation generation

## Decompilation Workflow

1. Run analysis scripts to identify functions
2. Use naming tools to assign meaningful names
3. Decompile functions in `decompiled_wip/`
4. Move completed functions to `src/`
5. Verify with objdiff and cross-platform builds
