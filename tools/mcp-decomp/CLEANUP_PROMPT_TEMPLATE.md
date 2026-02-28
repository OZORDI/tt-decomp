# Code Cleanup & Semantic Reconstruction Prompt Template

Use this prompt to clean up existing decompiled code and align it with our semantic reconstruction goals.

---

## PROMPT TEMPLATE

```
I need you to analyze and refactor the code in `[FILE_PATH]` to align with our semantic reconstruction goals for the Rockstar Table Tennis decompilation project.

**PRIMARY GOAL**: Transform this code from mechanical assembly translation into clean, idiomatic C++ that reconstructs what the original Rockstar developers likely wrote in 2006.

**CRITICAL**: This project is at 99.5% completion. Every struct layout, field name, type convention, and coding pattern is already established in the surrounding codebase. Your refactored code MUST integrate seamlessly with existing code. Inconsistency at this stage is a bug, not a style preference.

## Pre-Analysis: Understanding the Existing Codebase

**MANDATORY FIRST STEP**: Before analyzing the target file, understand what already exists.

### Why This Matters

At 99.5% completion:
- 1,081 classes are already implemented
- Field names and types are established across the codebase
- Calling conventions are set by existing callers
- Coding patterns are visible in 630+ implemented functions

Your refactored code will be read alongside existing implementations. A reader should not be able to tell which code was written last week vs. last month. Consistency is paramount.

### What to Read First

1. **Reference implementation**: `src/game/pong_player.cpp`
   - This is the gold standard for code quality
   - Study its struct definitions, naming, and style
   - Use it as a template for your refactoring

2. **Related classes in the same subsystem**:
   - If refactoring `rage::snSession`, read other `rage::sn*` classes
   - If refactoring `pongBall`, read `pongPlayer`, `pongCamera`
   - Look for shared patterns and conventions

3. **Parent and sibling classes**:
   - Classes that inherit from the same base
   - Classes in the same file
   - Classes that share memory layout patterns

4. **Callers and callees**:
   - How is this code used?
   - What does it depend on?
   - What types do existing call sites expect?

### Tools for Discovery

```bash
# Find related classes
suggest_file_placement("ClassName")

# See what calls this code
find_callers("FunctionName")

# See what this code calls
find_callees("FunctionName")

# Read existing implementations
get_existing_source("src/path/to/file.cpp")
get_existing_source("include/path/to/file.hpp")
```

## Analysis Phase

### Step 1: Read Existing Related Code (MANDATORY)

**Before touching the target file**, you MUST read related existing code to understand established patterns. This is not optional - at 99.5% project completion, consistency is critical.

**Read these files in order:**

1. **The target file itself**: `get_existing_source("[FILE_PATH]")`
   - What's already there?
   - What patterns are established?
   - What includes/types are already defined?

2. **Parent class implementations** (if applicable):
   - Use `get_class_context()` to identify parent classes
   - Call `get_existing_source()` on parent class files
   - Note: field layouts, naming conventions, method signatures

3. **Sibling classes in the same file**:
   - Use `suggest_file_placement()` to find related classes
   - Read their implementations to match style
   - Look for shared field names at common offsets

4. **Callers and callees**:
   - Use `find_callers("[FUNCTION_NAME]")` to see how this code is used
   - Use `find_callees("[FUNCTION_NAME]")` to see what it depends on
   - Call `get_existing_source()` on any that are already implemented
   - Note: parameter types, return types, calling patterns

5. **Related subsystem files**:
   - If working on `rage::snSession`, read other `rage::sn*` classes
   - If working on `pongPlayer`, read `pongBall`, `pongCamera`, etc.
   - Look for common patterns, shared types, consistent naming

**Document your findings:**
- What field names are used for similar concepts? (e.g., `m_pSession` vs `m_session`)
- What types are used? (e.g., `uint32_t` vs `int` vs `bool`)
- What naming style for locals? (e.g., `pPlayer` vs `player` vs `p_player`)
- What include patterns? (e.g., `#include "rage/session.hpp"` vs `<rage/session.hpp>`)
- What comment style? (e.g., `// @ 0xADDRESS` vs `/* @ 0xADDRESS */`)

### Step 2: Identify Issues in Target File

Now analyze the target file for these anti-patterns:
   - Manual padding bytes in structs (e.g., `uint8_t _pad[N]`)
   - Forced 32-bit pointer types (e.g., `uint32_t ptr` instead of actual pointers)
   - Raw numeric offsets in code (e.g., `*(int*)(obj + 0x1C)`)
   - Assembly artifacts (e.g., `ctx.rN`, `PPC_LOAD_*`, register names as variables)
   - Goto-heavy control flow that could be structured
   - Generic types where semantic types would be clearer (e.g., `uint32_t` for booleans)
   - Meaningless variable names (e.g., `r3`, `var_10`, `temp`)

### Step 3: Deep Research on Target Code

For each class/function in the file:
   - Call `get_class_context()` to understand the class structure
   - Call `get_function_info()` to get metadata
   - Call `get_function_pseudocode()` if available for better understanding
   - Use `resolve_address()` on any remaining magic addresses

### Step 4: Cross-Reference with Existing Code

**Critical**: Before writing ANY refactored code, verify consistency:

1. **Field names**: If offset +0x10 is accessed, check what neighboring classes call that field
   ```
   # Example: Check what pongPlayer calls offset +0x10
   get_existing_source("src/game/pong_player.cpp")
   # Look for: m_pPhysics, m_physics, physics, etc.
   ```

2. **Type choices**: If you see a pointer, check what type related classes use
   ```
   # Example: Is it PhysicsSystem* or void* or uint32_t?
   get_existing_source("include/physics/physics.hpp")
   ```

3. **Method signatures**: If refactoring a function, check how callers invoke it
   ```
   find_callers("MyClass::Update")
   # Then read those caller implementations
   get_existing_source("src/game/caller_file.cpp")
   ```

4. **Naming patterns**: Match the established conventions
   ```
   # If existing code uses m_pSession, don't use m_session or session_ptr
   # If existing code uses isActive(), don't use IsActive() or is_active()
   ```

## Refactoring Phase

**BEFORE WRITING**: Create a consistency checklist from your analysis:

```
Consistency Checklist for [FILE_PATH]:
- [ ] Field naming: Use m_pSession (matches rage::snCreateMachine)
- [ ] Pointer types: Use Session* not uint32_t (matches existing code)
- [ ] Bool types: Use bool not uint8_t (matches pongPlayer pattern)
- [ ] Method naming: Use PascalCase Update() (matches RAGE convention)
- [ ] Local naming: Use camelCase player (matches existing functions)
- [ ] Include style: Use "rage/session.hpp" (matches file's existing includes)
- [ ] Comment style: Use // @ 0xADDRESS (matches project standard)
```

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

## Integration Verification

After refactoring, verify integration with existing codebase:

### Consistency Checks

1. **Read the refactored code alongside related files**:
   ```
   # Compare your changes with established patterns
   get_existing_source("src/game/pong_player.cpp")  # Reference implementation
   get_existing_source("[YOUR_FILE]")                # Your refactored code
   ```

2. **Verify naming matches project conventions**:
   - Classes: PascalCase (e.g., `pongPlayer`, `rage::snSession`)
   - Methods: PascalCase (e.g., `Update()`, `GetState()`)
   - Members: `m_` prefix, camelCase (e.g., `m_pSession`, `m_flags`)
   - Pointers: `m_p` prefix (e.g., `m_pPhysics`)
   - Arrays: `m_a` prefix (e.g., `m_aPlayers`)
   - Locals: camelCase (e.g., `player`, `counter`)
   - Globals: `g_` prefix, snake_case (e.g., `g_game_obj_ptr`)

3. **Verify type consistency**:
   - If `pongPlayer` uses `bool m_isActive`, don't use `uint8_t m_isActive`
   - If `rage::snSession` uses `Session*`, don't use `void*` or `uint32_t`
   - Match integer types: if related code uses `int`, don't use `uint32_t` without reason

4. **Verify include patterns**:
   - Match existing include style in the file
   - Use forward declarations where existing code does
   - Don't duplicate includes that are already present

5. **Verify comment style**:
   - Use `// @ 0xADDRESS` for function addresses (project standard)
   - Explain WHY, not WHAT (semantic comments)
   - Match the tone/style of existing comments in related files

### Build Verification

After writing refactored code:

```bash
# Verify it compiles
cd build && ninja

# Check for warnings/errors
# (Use getDiagnostics tool if available)
```

### Integration Test

Ask yourself:
- If a developer reads this file alongside `pong_player.cpp`, would they think it's from the same codebase?
- Are field names at shared offsets consistent across classes?
- Do method signatures match what callers expect?
- Would the original Rockstar developers recognize this as their code?

If any answer is "no", revise before submitting.

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

**Workflow**:
1. Read reference: `get_existing_source("src/game/pong_player.cpp")`
2. Read related: `get_existing_source("src/rage/session_manager.cpp")`
3. Read parent: `get_existing_source("include/rage/snSession.hpp")`
4. Analyze target: `get_existing_source("src/rage/session_events.cpp")`
5. Find callers: `find_callers("rage::snSessionEvent::Process")`
6. Read caller code to understand expected types
7. Refactor with consistency checklist
8. Verify integration

### Example 2: Clean up a function with complex control flow

```
I need you to analyze and refactor the code in `src/game/pong_player.cpp` to align with our semantic reconstruction goals...
[rest of template]

Focus especially on the `ProcessInput()` function which has heavy goto usage.
```

**Workflow**:
1. Read entire file: `get_existing_source("src/game/pong_player.cpp")`
2. Note existing patterns in other methods
3. Read related: `get_existing_source("src/game/pong_ball.cpp")`
4. Check callers: `find_callers("pongPlayer::ProcessInput")`
5. Check callees: `find_callees("pongPlayer::ProcessInput")`
6. Read callee implementations if already lifted
7. Refactor `ProcessInput()` to match style of other methods in same file
8. Verify consistency with rest of `pong_player.cpp`

### Example 3: Clean up multiple related files

```
I need you to analyze and refactor the code in these files to align with our semantic reconstruction goals:
- `src/rage/ui_context.c`
- `include/rage/ui_context.h`

[rest of template]

Ensure the header and implementation are consistent with each other.
```

**Workflow**:
1. Read reference: `get_existing_source("src/game/pong_player.cpp")`
2. Read related UI code: `get_existing_source("src/rage/ui_manager.cpp")`
3. Read both target files:
   - `get_existing_source("src/rage/ui_context.c")`
   - `get_existing_source("include/rage/ui_context.h")`
4. Find what uses this: `find_callers("uiContext_Init")`
5. Read caller implementations to understand usage
6. Create consistency checklist from related UI code
7. Refactor both files maintaining header/impl consistency
8. Verify against related UI implementations

### Example 4: Refactor with explicit consistency requirements

```
I need you to analyze and refactor the code in `src/physics/collision.cpp` to align with our semantic reconstruction goals...
[rest of template]

IMPORTANT: This file must be consistent with:
- `src/physics/physics.cpp` (same subsystem)
- `src/game/pong_ball.cpp` (primary caller)
- `include/physics/phBound.hpp` (uses these types)

Read ALL of these files before refactoring to ensure:
1. Field names match at shared offsets
2. Type choices are consistent (e.g., Vector3 vs float[3])
3. Method naming follows established patterns
4. Include style matches existing physics code
```

**Workflow**:
1. Read all specified files first
2. Create detailed consistency checklist
3. Document field name mappings from existing code
4. Document type choices from existing code
5. Refactor target file
6. Cross-check every change against consistency checklist
7. Verify integration with each specified file

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


### Integration & Consistency (CRITICAL AT 99.5% COMPLETION)

**These checks are MANDATORY - not optional:**

- [ ] **Read reference implementation** (`src/game/pong_player.cpp`) BEFORE writing
- [ ] **Read related classes** in same subsystem BEFORE writing
- [ ] **Read parent/sibling classes** BEFORE writing
- [ ] **Read callers** to understand expected types/signatures
- [ ] **Read callees** to match their established signatures
- [ ] Field names match at shared offsets across classes
- [ ] Type choices match related implementations (bool vs uint8_t, etc.)
- [ ] Naming conventions match RAGE standards:
  - Members: `m_` prefix, camelCase (e.g., `m_flags`, `m_counter`)
  - Pointers: `m_p` prefix (e.g., `m_pSession`, `m_pPhysics`)
  - Arrays: `m_a` prefix (e.g., `m_aPlayers`)
  - Methods: PascalCase (e.g., `Update()`, `GetState()`)
  - Locals: camelCase (e.g., `player`, `counter`)
  - Globals: `g_` prefix, snake_case (e.g., `g_game_obj_ptr`)
- [ ] Include style matches existing file patterns
- [ ] Comment style matches project standard (`// @ 0xADDRESS`)
- [ ] Code reads naturally alongside existing implementations
- [ ] A developer couldn't tell this was refactored recently

### The Final Integration Test

**Open your refactored file side-by-side with `src/game/pong_player.cpp`** (the reference implementation).

Ask yourself these questions:

1. **Visual consistency**: Do they look like they're from the same codebase?
2. **Naming consistency**: Do they follow the same naming conventions?
3. **Type consistency**: Do they use the same type choices for similar concepts?
4. **Style consistency**: Do they have similar comment styles and formatting?
5. **Authenticity**: Would a Rockstar developer recognize both as their code?

**If you answer "no" to ANY question, STOP and revise.**

### Why This Matters

At 99.5% completion:
- Your code will be read alongside 630+ existing implementations
- Inconsistency is immediately visible and breaks immersion
- Field name mismatches cause integration bugs
- Type mismatches cause compiler errors or warnings
- Style mismatches make the codebase look unprofessional

**Remember**: You're not writing new code. You're completing an existing codebase. Make it seamless.

---

## FINAL REMINDER

**READ FIRST, WRITE SECOND**

The most common mistake is jumping straight to refactoring without understanding the existing codebase. At 99.5% completion, the patterns are already established. Your job is to match them, not invent new ones.

**Minimum files to read before writing:**
1. `src/game/pong_player.cpp` (reference implementation)
2. Target file itself
3. Related classes in same subsystem
4. Parent class (if applicable)
5. At least 2-3 caller implementations

**If you skip this step, your refactored code will be inconsistent and require rework.**
