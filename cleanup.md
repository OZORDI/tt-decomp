I need you to analyze and refactor the code in `[FILE_PATH]` to align with our semantic reconstruction goals for the Rockstar Table Tennis decompilation project.

**PRIMARY GOAL**: Transform this code from mechanical assembly translation into clean, idiomatic C++ that reconstructs what the original Rockstar developers likely wrote in 2006.

## Analysis Phase

First, read and analyze the current code:

1. **Read the file**: Use the MCP tools to read `[FILE_PATH]`
2. **Identify issues**: Look for these anti-patterns:
   - Manual padding bytes in structs (e.g., `uint8_t _pad[N]`)
   - Forced 32-bit pointer types (e.g., `uint32_t ptr` instead of actual pointers)
   - Raw numeric offsets in code (e.g., `*(int*)(obj + 0x1C)`)
   - Assembly artifacts (e.g., `ctx.rN`, `PPC_LOAD_*`, register names as variables)
   - Goto-heavy control flow that could be structured
   - Generic types where semantic types would be clearer (e.g., `uint32_t` for booleans)
   - Meaningless variable names (e.g., `r3`, `var_10`, `temp`)

3. **Research context**: For each class/function in the file:
   - Call `get_class_context()` to understand the class structure
   - Call `get_function_info()` to get metadata
   - Call `get_function_pseudocode()` if available for better understanding
   - Call `find_callers()` and `find_callees()` to understand usage patterns
   - Use `resolve_address()` on any remaining magic addresses

## Refactoring Phase

Transform the code following these principles:

### Struct Cleanup

**Before** (ABI-obsessed):
```cpp
struct MyClass {
    void* vtable;           // +0x00
    uint32_t flags;         // +0x04
    uint8_t _pad1[4];       // +0x08 manual padding
    uint32_t ptrAsInt;      // +0x0C stored as int
    uint8_t boolAsInt;      // +0x10 bool as byte
    uint8_t _pad2[15];      // +0x11 manual padding
};
```

**After** (Semantic):
```cpp
struct MyClass {
    uint32_t flags;
    SomeObject* object;     // Natural pointer type
    bool isActive;          // Natural bool type
    // Let compiler handle alignment
};
```

### Function Cleanup

**Before** (Assembly translation):
```cpp
void MyClass::Update(void* ctx, void* base) {
    uint32_t r3 = (uint32_t)this;
    uint32_t r4 = PPC_LOAD_U32(r3 + 0x1C);
    if (r4 == 0) goto label_exit;
    uint32_t r5 = PPC_LOAD_U32(r4 + 0x08);
    // ...
label_exit:
    return;
}
```

**After** (Semantic):
```cpp
void MyClass::Update() {
    if (!m_pSubsystem) {
        return;
    }
    
    int counter = m_pSubsystem->GetCounter();
    // ...
}
```

### Type Improvements

Replace:
- `uint8_t` used as bool → `bool`
- `uint32_t` used as pointer → actual pointer type
- `int` used as enum → proper enum type
- Generic `void*` → typed pointers where possible

### Control Flow

Replace:
- Goto chains → structured if/else/while/for
- Boolean inversion idioms (`cntlzw` patterns) → direct boolean logic
- Register-style temporary variables → meaningful names

### Naming

Replace:
- `r3`, `r30`, `var_f13` → semantic names based on usage
- `field_0x1C` → `m_pSubsystem` or similar
- `sub_82XXXXXX` → descriptive function names if purpose is clear

## Research Deeper Semantics

For each function, try to understand:

1. **What is the high-level purpose?** (Not "loads value from offset 0x1C", but "checks if subsystem is initialized")
2. **What are the preconditions?** (What must be true before calling?)
3. **What are the postconditions?** (What changes after calling?)
4. **What patterns does it follow?** (Constructor? Destructor? Update loop? State machine transition?)

Use Python to verify any arithmetic:
```python
# Verify offset calculations
python3 -c "print(f'0x{0x820C0000 + 0x1C:08X}')"

# Verify bit operations
python3 -c "
def cntlzw(v): 
    v = v & 0xFFFFFFFF
    return 32 if v == 0 else 31 - int(v).bit_length() + 1
print(cntlzw(0))  # Should be 32
print(cntlzw(1))  # Should be 31
"
```

## Output Requirements

Produce refactored code that:

1. **Looks human-written**: A Rockstar developer from 2006 would recognize this as their code
2. **Uses natural C++ types**: bool, int, float, proper pointers
3. **Has semantic field names**: Based on usage, not offsets
4. **Has structured control flow**: Minimal or no gotos
5. **Has meaningful comments**: Explain WHY, not WHAT
6. **Follows RAGE conventions**: 
   - `m_` prefix for members
   - `m_p` prefix for pointer members
   - `m_a` prefix for array members
   - PascalCase for methods
   - `g_` prefix for globals

7. **Is consistent with surrounding code**: Read related files first to match existing patterns

## Deliverables

1. **Analysis summary**: What issues did you find?
2. **Refactored code**: Clean, semantic C++ 
3. **Research notes**: What did you learn about the purpose/behavior?
4. **Confidence assessment**: What parts are you certain about vs. uncertain?

Write the refactored code using `write_source_file()` with appropriate mode (replace_lines, etc.).
```

---

## USAGE EXAMPLES

### Example 1: Clean up a struct-heavy file
```
I need you to analyze and refactor the code in `src/rage/session_events.cpp` to align with our semantic reconstruction goals...
[rest of template]
```

### Example 2: Clean up a function with complex control flow
```
I need you to analyze and refactor the code in `src/game/pong_player.cpp` to align with our semantic reconstruction goals...
[rest of template]

Focus especially on the `ProcessInput()` function which has heavy goto usage.
```

### Example 3: Clean up multiple related files
```
I need you to analyze and refactor the code in these files to align with our semantic reconstruction goals:
- `src/rage/ui_context.c`
- `include/rage/ui_context.h`

[rest of template]

Ensure the header and implementation are consistent with each other.
```

---

## QUICK REFERENCE: Common Transformations

| Assembly Artifact | Semantic Equivalent |
|------------------|---------------------|
| `uint8_t _pad[N]` | Remove - let compiler pad |
| `uint32_t ptrValue` | `SomeType* ptr` |
| `uint8_t boolValue` | `bool isActive` |
| `*(int*)(obj + 0x1C)` | `obj->m_counter` |
| `if (x == 0) goto skip;` | `if (x != 0) { ... }` |
| `r3`, `r30`, `var_10` | `player`, `subsystem`, `counter` |
| `PPC_LOAD_U32(ptr + N)` | `ptr->fieldName` |
| `ctx.rN.u32` | Remove entirely |
| `cntlzw` idiom | `!condition` or `== 0` |
| `void* ctx, void* base` params | Remove entirely |

---

## VALIDATION CHECKLIST

Before submitting refactored code, verify:

- [ ] No manual padding bytes in structs
- [ ] No `uint32_t` used as pointers
- [ ] No raw numeric offsets in code
- [ ] No assembly artifacts (`ctx`, `PPC_*`, register names)
- [ ] Control flow is structured (minimal gotos)
- [ ] Types are semantic (bool for booleans, etc.)
- [ ] Variable names are meaningful
- [ ] Comments explain intent, not mechanics
- [ ] Consistent with surrounding code in the same file
- [ ] Follows RAGE naming conventions
- [ ] Would look natural to a 2006 game developer

