# Coding Style

This project follows the coding style of the original Rockstar Table Tennis codebase as closely as possible.

## General Rules

- Match the original assembly's behavior exactly
- Preserve ABI compatibility (calling conventions, stack frames, struct layout)
- Use C++17 standard features where appropriate
- Keep function boundaries as they appear in the original binary

## Naming Conventions

- Classes: PascalCase (e.g., `grcDevice`, `rage_render_default`)
- Functions: camelCase or snake_case matching original symbols
- Variables: camelCase for local, snake_case for globals
- Constants: UPPER_SNAKE_CASE
- Namespaces: lowercase (e.g., `rage::`, `grc::`)

## Code Organization

- One function per assembly function (no merging)
- Headers in `include/` directory
- Implementation in `src/` directory
- Group by subsystem (grc/, rage/, game/, physics/, crt/)

## Comments

- Document ABI considerations
- Note correspondence with assembly addresses
- Explain non-obvious arithmetic or bit manipulation
- Reference original Xbox 360 behavior where relevant

## Formatting

- 4 spaces for indentation (no tabs)
- Opening braces on same line for functions
- Use `#pragma once` for header guards
- Keep lines under 120 characters where reasonable

## Cross-Platform

- Use SDL2 for window/input/audio
- Use OpenGL 3.3 Core for graphics
- Avoid platform-specific code outside `crt/` directory
- Test on Windows, macOS, and Linux

## Verification

- Build with CMake + Ninja
- Run getDiagnostics for syntax/type checking
- Use objdiff for binary comparison where possible
- Test cross-platform compilation regularly
