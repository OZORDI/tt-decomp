# Remaining Recovery Opportunities - Table Tennis Decompilation

**Current Status**: 92.15% coverage (15,664/16,998 functions named)  
**Remaining**: 1,334 unnamed functions (7.85%)  
**Date**: February 21, 2026

---

## Executive Summary

With 92.15% coverage achieved, we're approaching the practical limit of automated naming. The remaining 1,334 functions fall into categories that require increasingly sophisticated analysis or manual intervention.

**Key Insight**: We've exhausted most graph-based and structural approaches. The remaining gains require:
1. **Content-based analysis** (strings, constants, code patterns)
2. **Cross-binary matching** (other RAGE games)
3. **Dynamic analysis** (runtime tracing)
4. **Manual reverse engineering** (the final 2-3%)

---

## What Can Still Be Recovered from the Binary

### 1. String Literal References (High Value) 🔥

**Potential**: 200-400 functions  
**Confidence**: 85-90%  
**Effort**: Medium

**What to extract**:
```
Debug strings:
  "Failed to initialize %s"
  "Loading texture: %s"
  "Physics constraint violated"
  
Error messages:
  "Out of memory"
  "Invalid parameter"
  "File not found: %s"
  
Format strings:
  "Player %d scored"
  "Frame time: %.2fms"
  
Asset paths:
  "models/ball.dff"
  "textures/court_diffuse.txd"
```

**Strategy**:
- Extract all string literals from `.rdata` section
- Find xrefs to each string (which functions reference them)
- Match unique strings to function purpose
- Propagate via call graph

**Example**:
```
String: "grcDevice::Create failed"
→ Function 0x82160A40 references it
→ Name: grcDevice_Create or grcDevice_init_error_handler
```

**Implementation**: Extend P13 (currently returns 0) with proper string extraction from PE binary.

---

### 2. Constant Analysis (Medium Value) 🔥

**Potential**: 150-300 functions  
**Confidence**: 70-80%  
**Effort**: Medium-High

**What to look for**:

#### Magic Numbers
```cpp
0x47435244  // "GRCD" - Graphics device signature
0x50485953  // "PHYS" - Physics system signature
0x41554449  // "AUDI" - Audio system signature
```

#### Graphics Constants
```cpp
D3DFMT_A8R8G8B8 = 21      // Texture format
D3DPOOL_DEFAULT = 0        // Memory pool
D3DCLEAR_TARGET = 0x1      // Clear flags
```

#### Physics Constants
```cpp
9.81f        // Gravity (m/s²)
0.9f         // Ball restitution coefficient
1.2f         // Air resistance coefficient
```

#### Math Constants
```cpp
3.14159265f  // PI
1.57079633f  // PI/2
0.01745329f  // Degrees to radians (PI/180)
```

**Strategy**:
- Disassemble all unnamed functions
- Extract immediate values from `li`, `lis`, `addi` instructions
- Build constant fingerprints
- Match rare constant combinations

**Example**:
```
Function uses: [9.81, 0.9, 1.2]
→ Likely physics simulation
→ Name: ph_ball_update_velocity or ph_apply_gravity
```

---

### 3. Code Pattern Matching (Medium Value)

**Potential**: 100-200 functions  
**Confidence**: 65-75%  
**Effort**: High

**Patterns to detect**:

#### Getters/Setters
```cpp
// Getter pattern: load from offset, return
lwz r3, 0x10(r3)  // Load member at offset 0x10
blr               // Return

→ Name: ClassName_get_member_at_0x10
```

#### Constructors
```cpp
// Constructor pattern: store vtable pointer, initialize members
lis r4, vtable@ha
addi r4, r4, vtable@l
stw r4, 0(r3)     // Store vtable pointer
li r4, 0
stw r4, 4(r3)     // Initialize member to 0
...
→ Name: ClassName_ctor
```

#### Destructors
```cpp
// Destructor pattern: vtable lookup, cleanup, free
lwz r4, 0(r3)     // Load vtable
lwz r4, 4(r4)     // Load destructor from vtable
mtctr r4
bctrl             // Call destructor
→ Name: ClassName_dtor
```

#### Null Checks
```cpp
// Null check pattern
cmpwi r3, 0
beqlr             // Return if null
→ Name: function_name_null_safe
```

---

### 4. Cross-Binary Matching - Alternative Sources (High Value) 🔥

**Potential**: 300-600 functions  
**Confidence**: 80-90%  
**Effort**: Medium (if sources available)

#### Better Source Games (than GTA IV)

**Priority 1: Bully (2006)** ⭐⭐⭐⭐⭐
- Same year as Table Tennis
- Similar scope (smaller than GTA)
- Same RAGE engine generation
- **Expected match rate**: 5-10% (vs 0.07% with GTA IV)
- **Potential gain**: 500-800 functions

**Priority 2: Midnight Club 3 (2005)** ⭐⭐⭐⭐
- Pre-RAGE but may share libraries
- Racing game (simpler than GTA)
- Close in time
- **Expected match rate**: 2-5%
- **Potential gain**: 200-400 functions

**Priority 3: Max Payne 3 (2012)** ⭐⭐⭐
- Later RAGE but linear gameplay
- More similar scope to Table Tennis
- Better documented
- **Expected match rate**: 3-7%
- **Potential gain**: 300-500 functions

**Strategy**:
- Reuse `cross_match_gta4_v4_fast.py` script
- Adjust normalization for different compiler versions
- Focus on engine subsystems (grc, rage, ph)
- Use relaxed vtable matching (2-3 candidates)

---

### 5. Dynamic Analysis (Medium-High Value) 🔥

**Potential**: 200-400 functions  
**Confidence**: 90-95% (for executed functions)  
**Effort**: High

**Approach**: Run game in emulator with tracing

#### Xenia Emulator Tracing
```bash
# Enable function call tracing
xenia-canary --trace_function_calls

# Capture execution trace
# Play through game scenarios:
# - Main menu
# - Single match
# - Tournament mode
# - Options/settings
```

**What you get**:
```
0x82160A40 called from 0x82160B20  # grcDevice initialization
0x82161D40 called from 0x82160A40  # grcSetup_vfn_2 (confirmed!)
0x82345678 called 1,247 times      # Likely game loop or render function
0x82456789 called only in menu     # Menu-specific function
```

**Strategy**:
- Correlate call frequency with function purpose
- High-frequency calls: game loop, rendering, physics
- Low-frequency calls: initialization, cleanup
- Context-specific calls: menu, gameplay, loading

**Example**:
```
Function 0x82345678:
- Called 60 times per second
- Always called from game_main_loop
- Calls ph_world_step and grc_render_frame
→ Name: game_update_and_render
```

---

### 6. Struct Layout Recovery (High Value for Implementation) 🔥

**Potential**: Not direct naming, but critical for C++ reconstruction  
**Confidence**: 70-85%  
**Effort**: High

**What to recover**:

#### Class Member Offsets
```cpp
// From assembly patterns:
lwz r4, 0x10(r3)  // Member at offset 0x10
lwz r5, 0x14(r3)  // Member at offset 0x14
lwz r6, 0x18(r3)  // Member at offset 0x18

→ Reconstruct:
class ClassName {
    void* vtable;        // 0x00
    int unknown_0x04;    // 0x04
    int unknown_0x08;    // 0x08
    int unknown_0x0C;    // 0x0C
    float member_0x10;   // 0x10 ← accessed
    float member_0x14;   // 0x14 ← accessed
    float member_0x18;   // 0x18 ← accessed
};
```

#### Vtable Layouts
```cpp
// From vtable analysis:
vtable 5594 (rage::grcSetup):
  0x00: grcSetup_vfn_0   (unknown)
  0x04: grcSetup_vfn_1   (unknown)
  0x08: grcSetup_vfn_2   ← GTA4 seed
  0x0C: grcSetup_vfn_3   ← GTA4 seed
  ...
```

**Strategy**:
- Analyze all load/store patterns
- Build offset usage maps
- Infer member types from usage
- Cross-reference with RTTI

---

### 7. Import/Export Analysis (Low Value)

**Potential**: 50-100 functions  
**Confidence**: 95%  
**Effort**: Low

**What to extract**:

#### Xbox 360 Kernel Imports
```
XAM (Xbox Application Model):
  XamInputGetState
  XamUserGetSigninState
  XamShowMessageBoxUI
  
XAudio:
  XAudio2Create
  IXAudio2::CreateSourceVoice
  
D3D (Xenos GPU):
  D3DDevice_CreateTexture
  D3DDevice_DrawIndexedPrimitive
```

**Strategy**:
- Parse import table from XEX
- Find xrefs to each import
- Name wrapper functions

**Example**:
```
Import: XamInputGetState
→ Wrapper at 0x82123456
→ Name: xe_input_get_state_wrapper
```

---

### 8. Exception Handler Analysis (Low Value)

**Potential**: 20-50 functions  
**Confidence**: 90%  
**Effort**: Low

**What to find**:
```cpp
__try {
    // Protected code
} __except (filter_function(exception_info)) {
    // Handler code
}
```

**Strategy**:
- Parse exception directory in PE
- Identify filter and handler functions
- Name based on protected code

---

### 9. Inline Assembly Patterns (Low Value)

**Potential**: 30-60 functions  
**Confidence**: 80%  
**Effort**: Medium

**Patterns**:

#### SIMD/AltiVec Operations
```asm
lvx v0, r3, r4      // Load vector
vaddfp v0, v0, v1   // Vector add (floating point)
stvx v0, r3, r4     // Store vector

→ Name: vector_add_float4 or simd_add_vectors
```

#### Atomic Operations
```asm
lwarx r4, 0, r3     // Load and reserve
stwcx. r5, 0, r3    // Store conditional
bne- retry          // Retry if failed

→ Name: atomic_compare_exchange or interlocked_increment
```

---

### 10. Compiler Intrinsics (Low Value)

**Potential**: 40-80 functions  
**Confidence**: 95%  
**Effort**: Low

**Common intrinsics**:
```cpp
memcpy, memset, memmove
strlen, strcmp, strcpy
abs, fabs, sqrt, sin, cos
_rotl, _rotr (bit rotation)
__builtin_clz (count leading zeros)
```

**Strategy**:
- Recognize standard library patterns
- Match against known compiler implementations
- Name with standard names

---

## Restoration Opportunities (Beyond Naming)

### 1. Source Code Comments (from Debug Strings)

**What to recover**:
```cpp
// From string: "FIXME: This is a hack"
// From string: "TODO: Optimize this"
// From string: "WARNING: Not thread-safe"
```

**Value**: Provides developer intent and known issues

---

### 2. Original Variable Names (from Debug Info)

**Potential**: If debug symbols exist (unlikely for retail)

**What to look for**:
- PDB file (Windows debug symbols)
- DWARF info (if any dev builds leaked)
- String references to variable names

**Example**:
```cpp
// String: "m_pDevice is NULL"
→ Variable name: m_pDevice
→ Type: pointer to device
```

---

### 3. Function Signatures (from RTTI + Usage)

**What to reconstruct**:
```cpp
// From RTTI: class rage::grcDevice
// From vtable: virtual function at slot 5
// From usage: takes 2 int params, returns bool

→ Signature:
virtual bool grcDevice::vfn_5(int width, int height);
```

**Strategy**:
- Analyze parameter passing (r3-r10, f1-f13)
- Analyze return values (r3, f1)
- Infer types from usage patterns

---

### 4. Enum Values (from Switch Tables)

**What to recover**:
```cpp
// From switch table analysis:
enum GameState {
    STATE_MENU = 0,
    STATE_LOADING = 1,
    STATE_PLAYING = 2,
    STATE_PAUSED = 3,
    STATE_GAME_OVER = 4
};
```

**Strategy**:
- Find switch statements (branch tables)
- Extract case values
- Infer enum names from context

---

### 5. Asset File Formats (from Parsing Code)

**What to document**:
```cpp
// From texture loading code:
struct TextureHeader {
    uint32_t magic;      // "GTEX"
    uint16_t width;
    uint16_t height;
    uint32_t format;     // D3DFMT_*
    uint32_t mipLevels;
    // ... inferred from load sequence
};
```

**Value**: Enables asset extraction and modding

---

### 6. Network Protocol (from Packet Handlers)

**What to reverse**:
```cpp
// From network code:
enum PacketType {
    PACKET_CONNECT = 0x01,
    PACKET_DISCONNECT = 0x02,
    PACKET_GAME_STATE = 0x10,
    PACKET_PLAYER_INPUT = 0x11,
    // ... from packet handler switch
};
```

**Value**: Enables multiplayer reconstruction

---

## Practical Limits

### What We'll Never Recover (Without Source)

1. **Original variable names** (unless in debug strings)
2. **Original comments** (unless in debug strings)
3. **Macro definitions** (expanded at compile time)
4. **Template instantiations** (mangled beyond recognition)
5. **Inline function boundaries** (merged by compiler)
6. **Dead code** (stripped by linker)
7. **Compile-time constants** (folded by optimizer)

### Realistic Final Coverage

| Approach | Expected Final Coverage |
|----------|------------------------|
| **Current (v11)** | 92.15% |
| **+ String analysis** | 93.5-94.5% |
| **+ Constant analysis** | 94.5-95.5% |
| **+ Cross-binary (Bully)** | 95.5-97.0% |
| **+ Dynamic tracing** | 96.5-97.5% |
| **+ Manual RE** | 98.0-99.0% |
| **Theoretical maximum** | ~99.5% |

**The final 0.5-1%**: Likely dead code, compiler-generated stubs, or functions so generic they're indistinguishable.

---

## Recommended Next Steps (Priority Order)

### Phase 1: Low-Hanging Fruit (1-2 weeks)
1. ✅ **String literal extraction** - Extend P13 with proper implementation
2. ✅ **Import/export analysis** - Parse XEX import table
3. ✅ **Compiler intrinsics** - Pattern matching for standard library

**Expected gain**: +1.5-2.5% coverage (93.5-94.5%)

### Phase 2: Medium Effort (2-4 weeks)
4. ✅ **Constant analysis** - Magic numbers and rare combinations
5. ✅ **Code pattern matching** - Getters, setters, constructors
6. ✅ **Cross-binary (Bully)** - If sources available

**Expected gain**: +2.0-3.5% coverage (95.5-97.0%)

### Phase 3: High Effort (1-2 months)
7. ✅ **Dynamic tracing** - Xenia emulator with function tracing
8. ✅ **Struct layout recovery** - For C++ implementation
9. ✅ **Manual reverse engineering** - Final 2-3%

**Expected gain**: +1.5-2.5% coverage (97.0-99.0%)

---

## Implementation Priorities for C++ Reconstruction

Beyond naming, focus on:

### 1. Critical Engine Systems (for playability)
- **Graphics**: grcDevice, grcTexture, grcEffect (rendering)
- **Physics**: phWorld, phRigidBody, phConstraint (ball physics)
- **Input**: xe_input_* (controller handling)
- **Audio**: aud_* (sound effects, music)

### 2. Game Logic (for gameplay)
- **Game loop**: game_update, game_render
- **Ball physics**: ph_ball_* (trajectory, spin, collision)
- **AI**: ai_opponent_* (computer player)
- **Scoring**: game_score_* (points, sets, matches)

### 3. Asset Loading (for content)
- **File I/O**: fiDevice, fiPackfile
- **Texture loading**: grcTexture_load_*
- **Model loading**: grcModel_load_*
- **Audio loading**: aud_load_*

---

## Conclusion

With 92.15% coverage, you've achieved excellent automated naming. The remaining 7.85% requires:

**Quick wins** (string/constant analysis): +2-3% coverage  
**Medium effort** (cross-binary, patterns): +2-3% coverage  
**High effort** (dynamic tracing, manual RE): +1-2% coverage  

**Realistic final target**: 97-99% coverage

**Focus shift**: From naming to C++ implementation. With 92% of functions identified, you have enough context to start reconstructing critical systems (graphics, physics, game loop) and make the game playable.

The remaining unnamed functions are likely:
- Utility functions (can be inferred from usage)
- Compiler-generated code (can be stubbed)
- Dead code (can be ignored)

**Recommendation**: Declare naming "complete enough" and shift focus to implementation. The remaining 8% can be named incrementally as you implement and test each subsystem.
