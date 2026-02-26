# Audio Control Wrappers Implementation — February 26, 2026

## Overview

Implemented three RAGE audio control wrapper classes that provide RAII-style management of audio control objects:

- `rage::audControlWrapper` (base class)
- `rage::audControl3dWrapper` (3D positional audio)
- `rage::audControl2dWrapper` (2D non-positional audio)

## Classes Implemented

### rage::audControlWrapper @ vtable 0x82074B88

Base wrapper class for audio control objects. Manages dynamically allocated audio controls.

**Structure:**
```cpp
struct audControlWrapper {
    void** vtable;              // +0x00 @ 0x82074B88
    audControl* m_pControl;     // +0x04 - Pointer to wrapped control(s)
};
```

**Methods:**
- Destructor @ 0x823F7C88 | size: 0x50

**Behavior:**
1. Sets vtable to base class vtable (for proper destruction order)
2. If m_pControl is non-null:
   - Calls its virtual destructor with flags=1 (delete)
   - Sets m_pControl to null
3. Wrapper object itself is freed by caller based on flags parameter

### rage::audControl3dWrapper @ vtable 0x82074B98

Wrapper for 3D positional audio controls. Inherits from audControlWrapper.

**Methods:**
- Destructor @ 0x823F8138 | size: 0xD0

**Behavior:**
More complex than base class because it can manage arrays of audControl3d objects:

1. Checks flags bit 1 (array delete flag)
2. If array delete:
   - Reads array count from offset -4 (before the array)
   - Loops through array backwards, destroying each element
   - Frees the array memory (including the count prefix)
3. Else (single object):
   - Calls base class destructor
   - Frees wrapper if delete flag set

**Array Memory Layout:**
```
[count:4 bytes][element0][element1]...[elementN]
^              ^
allocation     m_pControl points here
starts here
```

### rage::audControl2dWrapper @ vtable 0x82074BA8

Wrapper for 2D (non-positional) audio controls. Inherits from audControlWrapper.

Uses base class destructor (no override needed).

## Assembly Analysis

### audControlWrapper::~audControlWrapper @ 0x823F7C88

**Original Assembly Pattern:**
```assembly
mr r31,r3                    ; Save 'this' pointer
bl 0x823f7c28                ; Call base cleanup function
clrlwi r11,r30,31            ; Extract bit 0 of flags (delete flag)
mr r3,r31                    ; Restore 'this'
cmplwi cr6,r11,0             ; Check if delete flag set
beq cr6,0x823f7cc0           ; Skip if not set
bl 0x820c00c0                ; Call rage_free_00C0
mr r3,r31                    ; Return 'this'
```

**Key Observations:**
- Function at 0x823F7C28 handles the wrapped control cleanup
- Bit 0 of flags parameter (r4) controls whether to free the wrapper itself
- Uses rage_free_00C0 for memory deallocation

### audControl3dWrapper::~audControl3dWrapper @ 0x823F8138

**Original Assembly Pattern (Array Delete Path):**
```assembly
rlwinm r11,r26,0,30,30       ; Extract bit 1 (array delete flag)
cmplwi cr6,r11,0
beq cr6,0x823f81e0           ; Jump to single-object path if not array

; Array delete path:
addi r28,r31,-4              ; Point to count (4 bytes before array)
lwz r11,0(r28)               ; Load count
rlwinm r10,r11,1,0,30        ; count * 2
addi r30,r11,-1              ; count - 1 (for loop)
add r10,r11,r10              ; count * 3
rlwinm r11,r10,2,0,29        ; (count * 3) * 4 = count * 12
add r31,r11,r31              ; Calculate end of array

loc_823F8188:                ; Loop through array backwards
addi r31,r31,-12             ; Move to previous element
lwz r3,4(r31)                ; Load element's m_pControl
stw r29,0(r31)               ; Set vtable
cmplwi cr6,r3,0
beq cr6,0x823f81b4
lwz r9,0(r3)                 ; Load vtable
li r4,1                      ; flags = 1 (delete)
lwz r8,0(r9)                 ; Load destructor
bctrl                        ; Call destructor
stw r27,4(r31)               ; Clear m_pControl

loc_823F81B4:
addi r30,r30,-1              ; Decrement counter
cmpwi cr6,r30,0
bge cr6,0x823f8188           ; Loop if more elements

; Free array memory:
clrlwi r7,r26,31             ; Extract bit 0 (delete flag)
cmplwi cr6,r7,0
beq cr6,0x823f81d4
mr r3,r28                    ; Point to count (start of allocation)
bl 0x820c00c0                ; rage_free_00C0
```

**Key Observations:**
- Bit 1 of flags (r4) indicates array delete
- Array count stored at offset -4 from m_pControl
- Each element is 12 bytes (0xC)
- Elements destroyed in reverse order
- Memory freed starting from count location (not m_pControl)

## Implementation Notes

### C++ Translation Approach

The implementation uses standard C++ destructors rather than explicit flags parameters because:

1. **Compiler-Generated Code**: Modern C++ compilers handle the scalar/array delete distinction automatically
2. **RAII Semantics**: C++ destructors provide the same cleanup guarantees as the original assembly
3. **Simplified Logic**: The complex flag checking is handled by the compiler's delete operator

### Array Delete Complexity

The full array delete implementation is documented but not fully implemented because:

1. Requires compiler-specific array delete support
2. Modern C++ uses `delete[]` operator which handles this automatically
3. The documented assembly pattern shows the original implementation for reference

### Memory Management

- Uses `rage_free_00C0` for deallocation (RAGE engine's custom allocator)
- Maintains proper destruction order (wrapped objects before wrapper)
- Handles null pointer checks before calling destructors

## Files Created

1. `include/rage/audio_control_wrappers.hpp` - Header with class definitions
2. `src/rage/audio_control_wrappers.cpp` - Implementation
3. Updated `src/rage/rage_audio.hpp` - Forward declarations
4. Updated `CMakeLists.txt` - Added source file to build

## Build Status

✅ Compiles successfully with:
```bash
/usr/bin/c++ -I./include -Wall -Wextra -std=c++17 -c src/rage/audio_control_wrappers.cpp
```

## Related Systems

- **Audio Control Base Classes**: `audControl`, `audControl3d`, `audControl2d` (in rage_audio.hpp)
- **Memory Management**: `rage_free_00C0` @ 0x820C00C0
- **RTTI System**: Vtable addresses from rtti_vtable_map.json

## Future Work

1. Implement full array delete support when needed
2. Add constructor implementations
3. Implement wrapped audio control classes (audControl, audControl3d, audControl2d)
4. Add unit tests for wrapper lifecycle

## References

- RTTI Vtable Map: config/434C4803/rtti_vtable_map.json
- RTTI Class Hierarchy: config/434C4803/rtti_class_hierarchy.json
- Symbol Table: config/434C4803/symbols.txt
- ReXGlue Output: recomp/structured_pass5_final/tt-decomp_recomp.23.cpp
