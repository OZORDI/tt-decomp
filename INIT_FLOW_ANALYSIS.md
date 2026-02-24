# Initialization Flow Analysis - Xbox 360 Entry to GPU Rendering

**Date**: February 23, 2026  
**Source**: ReXGlue Recompiled PowerPC Code (Ground Truth)  
**Objective**: Trace complete execution path from Xbox 360 entry point to GPU rendering calls

---

## Executive Summary

This document traces the complete initialization and rendering flow of Rockstar Presents Table Tennis (Xbox 360) from the CRT entry point through to GPU draw calls. All analysis is based on the ReXGlue recompiled PowerPC code, which serves as our source of truth.
 
**Key Finding**: The game follows a classic RAGE engine initialization pattern:
1. CRT/Thread initialization
2. RAGE subsystem setup
3. Graphics device creation
4. Game loop with conditional rendering (scene vs. default)

---

## Complete Call Chain

### Level 0: Xbox 360 Entry Point

```
__mainCRTStartup @ 0x8242BD20 (Xbox 360 CRT)
  ↓
__crt_main_entry @ 0x820C0128 (CRT initialization)
  ↓
rage_main @ 0x82186970 (RAGE engine entry)
  ↓
rage_init @ 0x82186418 (RAGE initialization)
  ↓
rage_run @ 0x82186780 (RAGE runner)
  ↓
game_main @ 0x820C0378 (Game initialization and main loop)
```

---

## Level 1: game_main @ 0x820C0378

**Size**: 0x7C8 bytes (1,992 bytes)  
**Purpose**: Main game initialization and rendering loop

### Function Calls (24 total)

| Address    | Function Name                  | Purpose                          |
|------------|--------------------------------|----------------------------------|
| 0x820C0038 | xe_main_thread_init_0038       | Thread/TLS initialization        |
| 0x8215CB60 | rage_game_obj_init_CB60        | Root game object setup           |
| 0x822E2430 | rage_subsystem_init_2430       | RAGE subsystems registration     |
| 0x820F8A00 | grcDevice_init_8A00            | Graphics device initialization   |
| 0x821C77E8 | io_Input_init_77E8             | Input system setup               |
| 0x82100B78 | grcRenderer_init_0B78          | Renderer initialization          |
| 0x82221ED0 | audSystem_init_1ED0            | Audio system setup               |
| 0x82222060 | audSystem_start_2060           | Start audio thread               |
| 0x822BC2D0 | gameLoop_init_C2D0             | Game loop object creation        |
| 0x822E3040 | rage_obj_factory_create_3040   | Create scene instance            |
| 0x822E3828 | rage_obj_bind_3828             | Bind scene to game object        |
| 0x822E3B38 | rage_obj_finalize_3B38         | Finalize scene object            |
| 0x822E17B0 | rage_scene_load_17B0           | Load scene data                  |
| 0x822C0B18 | rage_render_scene_0B18         | **Render scene (conditional)**   |
| 0x822C08D8 | rage_render_default_08D8       | **Render default (main loop)**   |
| 0x821861A0 | sysMemAllocator_Allocate_61A0  | Memory allocation                |
| 0x82186228 | sysMemAllocator_Free_6228      | Memory deallocation              |
| 0x82186628 | rage_get_exe_name_6628         | Get executable name              |
| 0x82186BE8 | rage_is_app_started_6BE8       | Check app started flag           |
| 0x82186CA8 | xe_alloc_thread_ctx_6CA8       | Allocate thread context          |
| 0x820C0B40 | _snprintf                      | String formatting                |
| 0x8240E6D0 | nop_8240E6D0                   | No-op function                   |
| 0x8242B8A8 | xe_sleep_B8A8                  | Sleep/delay function             |
| 0x8242F870 | __savegprlr_18                 | Register save/restore            |

### Initialization Sequence (Lines 548-900)

```c
// Phase 1: Thread and TLS setup
xe_main_thread_init();

// Phase 2: Core game object
rage_game_obj_init(game_obj_buffer);

// Phase 3: RAGE subsystems
rage_subsystem_init();

// Phase 4: Graphics device (Xenos GPU → OpenGL)
grcDevice_init(width, flags);

// Phase 5: Input system
io_Input_init(input_buffer);

// Phase 6: Renderer
grcRenderer_init(renderer_buffer);

// Phase 7: Audio system
audSystem_init();
audSystem_start();

// Phase 8: Game loop object
gameLoop_init();

// Phase 9: Scene creation
scene = rage_obj_factory_create(factory, name, type, unused, flags);
rage_obj_bind();
rage_obj_finalize(scene);

// Phase 10: Load scene data
rage_scene_load();
```

### Main Rendering Loop (Lines 900-950)

```c
// Main game loop
while (running) {
    // Update game logic (vtable call at offset 92)
    gameLoop->vtable->update(gameLoop);
    
    // Conditional rendering
    if (scene && scene->active) {
        // Render scene with objects
        rage_render_scene();  // @ 0x822C0B18
    } else {
        // Default rendering path
        rage_render_default();  // @ 0x822C08D8
    }
    
    // Additional game loop logic...
}
```

**Key Observation**: The game uses conditional rendering:
- `rage_render_scene` when a scene object is active (gameplay)
- `rage_render_default` for default/fallback rendering (menus, loading)

---

## Level 2: rage_render_default @ 0x822C08D8

**Size**: 0x240 bytes (576 bytes)  
**Purpose**: Main rendering function for default path

### Function Calls (5 total)

| Address    | Function Name              | Purpose                    |
|------------|----------------------------|----------------------------|
| 0x821C9D68 | io_Input_poll_9D68         | Poll input readiness       |
| 0x82305E78 | grcDevice_beginScene_5E78  | **Begin frame rendering**  |
| 0x82379290 | grcDevice_clear_9290       | **Clear framebuffer**      |
| 0x823CEDE0 | pg_EDE0_gen                | GPU command generation     |
| 0x8242F898 | __savegprlr_28             | Register save/restore      |

### Rendering Sequence

```c
void rage_render_default() {
    // Check input state
    if (io_Input_poll()) {
        // Input is ready
    }
    
    // Begin frame
    grcDevice_beginScene();  // @ 0x82305E78
    
    // Clear buffers
    grcDevice_clear();       // @ 0x82379290
    
    // Generate GPU commands
    pg_EDE0_gen();           // @ 0x823CEDE0
    
    // Additional rendering logic...
}
```

**Critical GPU Functions**:
- `grcDevice_beginScene` - Binds framebuffer, prepares for rendering
- `grcDevice_clear` - Clears color/depth/stencil buffers
- `pg_EDE0_gen` - Generates GPU command buffer (Xenos-specific)

---

## Level 2: rage_render_scene @ 0x822C0B18

**Size**: 0x130 bytes (304 bytes)  
**Purpose**: Render scene with game objects

### Function Calls (6 total)

| Address    | Function Name           | Purpose                    |
|------------|-------------------------|----------------------------|
| 0x822C08D8 | rage_render_default     | Call default render path   |
| 0x8240E6D0 | nop_8240E6D0            | No-op                      |
| 0x8242C3B8 | pg_C3B8_g               | GPU command generation     |
| 0x8242F868 | __savegprlr_16          | Register save/restore      |
| 0x8242F898 | __savegprlr_28          | Register save/restore      |
| 0x82566C40 | pg_6C40_g               | GPU command generation     |

### Rendering Sequence

```c
void rage_render_scene() {
    // Call default rendering first
    rage_render_default();  // @ 0x822C08D8
    
    // Generate scene-specific GPU commands
    pg_C3B8_g();            // @ 0x8242C3B8
    pg_6C40_g();            // @ 0x82566C40
    
    // Additional scene rendering...
}
```

**Key Observation**: `rage_render_scene` calls `rage_render_default` first, then adds scene-specific rendering on top.

---

## Level 3: grcDevice_beginScene @ 0x82305E78

**Size**: 0x188 bytes (392 bytes)  
**Purpose**: Begin frame rendering, bind framebuffer

### Function Calls (3 total)

| Address    | Function Name      | Purpose                    |
|------------|--------------------|----------------------------|
| 0x82305D50 | pg_5D50_g          | GPU state setup            |
| 0x8242C3B8 | pg_C3B8_g          | GPU command generation     |
| 0x8242F89C | __savegprlr_29     | Register save/restore      |

### GPU Setup Sequence

```c
void grcDevice_beginScene() {
    // Set up GPU state
    pg_5D50_g();  // @ 0x82305D50
    
    // Generate begin-frame commands
    pg_C3B8_g();  // @ 0x8242C3B8
    
    // Bind default framebuffer
    // Set viewport
    // Reset render state
}
```

---

## Level 3: grcDevice_clear @ 0x82379290

**Size**: 0x190 bytes (400 bytes)  
**Purpose**: Clear color, depth, and stencil buffers

### Function Calls (1 total)

| Address    | Function Name      | Purpose               |
|------------|--------------------|-----------------------|
| 0x8242F88C | __savegprlr_26     | Register save/restore |

### Clear Sequence

```c
void grcDevice_clear() {
    // Clear color buffer
    // Clear depth buffer
    // Clear stencil buffer
    // (All done via Xenos GPU commands)
}
```

**Note**: This function has minimal calls because it directly writes to GPU command buffers.

---

## GPU Command Generation Functions

These functions generate Xenos GPU command buffers:

| Function    | Address    | Size  | Purpose                          |
|-------------|------------|-------|----------------------------------|
| pg_5D50_g   | 0x82305D50 | 0x124 | GPU state setup (beginScene)     |
| pg_C3B8_g   | 0x8242C3B8 | 0x8   | GPU command generation (common)  |
| pg_6C40_g   | 0x82566C40 | 0x3C  | GPU command generation (scene)   |
| pg_EDE0_gen | 0x823CEDE0 | ???   | GPU command generation (default) |

**Note**: The `pg_*` prefix likely stands for "PowerPC GPU" or "Packet Generator" - these are low-level GPU command buffer builders for the Xbox 360 Xenos GPU.

---

## Memory Layout Observations

### game_main Stack Frame

**Size**: 0x7C8 bytes (1,992 bytes)

**Large stack frame indicates**:
- Local buffers for initialization (game_obj_buffer, renderer_buffer, input_buffer)
- String buffers for logging/debugging
- Temporary storage for object creation

### Key Offsets in Game Object

From the rendering loop analysis:
- `+0x000`: vtable pointer
- `+0x004`: flags/state
- `+0x22C` (556): scene object pointer
- `+0x230` (560): scene active flag
- `+0x210` (528): rendering mode flag

---

## Rendering Flow Summary

```
Entry Point
    ↓
game_main initialization
    ├─ xe_main_thread_init (TLS setup)
    ├─ rage_game_obj_init (root object)
    ├─ rage_subsystem_init (RAGE systems)
    ├─ grcDevice_init (GPU device)
    ├─ io_Input_init (input)
    ├─ grcRenderer_init (renderer)
    ├─ audSystem_init (audio)
    ├─ gameLoop_init (game loop)
    ├─ rage_obj_factory_create (scene)
    ├─ rage_obj_bind (bind scene)
    ├─ rage_obj_finalize (finalize)
    └─ rage_scene_load (load data)
    ↓
Main Game Loop
    ├─ gameLoop->update() (vtable call)
    ├─ if (scene && scene->active)
    │   └─ rage_render_scene
    │       ├─ rage_render_default
    │       │   ├─ io_Input_poll
    │       │   ├─ grcDevice_beginScene
    │       │   │   ├─ pg_5D50_g (GPU state)
    │       │   │   └─ pg_C3B8_g (GPU commands)
    │       │   ├─ grcDevice_clear
    │       │   └─ pg_EDE0_gen (GPU commands)
    │       ├─ pg_C3B8_g (scene GPU commands)
    │       └─ pg_6C40_g (scene GPU commands)
    └─ else
        └─ rage_render_default
            ├─ io_Input_poll
            ├─ grcDevice_beginScene
            │   ├─ pg_5D50_g
            │   └─ pg_C3B8_g
            ├─ grcDevice_clear
            └─ pg_EDE0_gen
```

---

## Implementation Roadmap

Based on this analysis, the decompilation should proceed in this order:

### Phase 1: Initialization (COMPLETE ✅)
- [x] `__crt_main_entry` @ 0x820C0128
- [x] `rage_main` @ 0x82186970
- [x] `rage_init` @ 0x82186418
- [x] `rage_run` @ 0x82186780
- [x] `game_main` @ 0x820C0378 (partial)

### Phase 2: Core Systems (IN PROGRESS ⏳)
- [x] `xe_main_thread_init` @ 0x820C0038
- [x] `rage_game_obj_init` @ 0x8215CB60
- [ ] `rage_subsystem_init` @ 0x822E2430 (stub)
- [x] `grcDevice_init` @ 0x820F8A00
- [ ] `io_Input_init` @ 0x821C77E8 (stub)
- [x] `grcRenderer_init` @ 0x82100B78
- [ ] `audSystem_init` @ 0x82221ED0 (stub)
- [ ] `gameLoop_init` @ 0x822BC2D0 (stub)

### Phase 3: Scene Management (PENDING 📋)
- [ ] `rage_obj_factory_create` @ 0x822E3040
- [ ] `rage_obj_bind` @ 0x822E3828
- [ ] `rage_obj_finalize` @ 0x822E3B38
- [ ] `rage_scene_load` @ 0x822E17B0

### Phase 4: Rendering Loop (PENDING 📋)
- [ ] `rage_render_default` @ 0x822C08D8
- [ ] `rage_render_scene` @ 0x822C0B18
- [x] `grcDevice_beginScene` @ 0x82305E78 (OpenGL impl)
- [x] `grcDevice_clear` @ 0x82379290 (OpenGL impl)
- [ ] `io_Input_poll` @ 0x821C9D68

### Phase 5: GPU Command Generation (FUTURE 🔮)
- [ ] `pg_5D50_g` @ 0x82305D50
- [ ] `pg_C3B8_g` @ 0x8242C3B8
- [ ] `pg_6C40_g` @ 0x82566C40
- [ ] `pg_EDE0_gen` @ 0x823CEDE0

---

## Key Insights

1. **Conditional Rendering**: The game uses two rendering paths:
   - `rage_render_scene` for active gameplay with scene objects
   - `rage_render_default` for menus, loading screens, fallback

2. **GPU Abstraction**: The `grc*` functions abstract Xenos GPU operations:
   - `grcDevice_*` - Device-level operations (init, begin, clear, present)
   - `pg_*` - Low-level GPU command buffer generation

3. **RAGE Architecture**: Classic RAGE engine pattern:
   - Root game object with vtable
   - Subsystem registration
   - Scene object factory
   - Vtable-driven game loop

4. **Memory Management**: Heavy use of TLS (Thread Local Storage):
   - `xe_main_thread_init` sets up TLS
   - `xe_alloc_thread_ctx` allocates per-thread contexts
   - `sysMemAllocator_*` uses TLS for memory allocation

5. **Stack Frame Size**: `game_main` has a 1,992-byte stack frame:
   - Indicates significant local buffer usage
   - Matches original PowerPC ABI requirements
   - Suggests complex initialization sequence

---

## Next Steps

1. **Complete game_main loop**: Implement the full rendering loop with conditional paths
2. **Implement rage_render_default**: Core rendering function with GPU calls
3. **Stub remaining init functions**: Create minimal stubs for audio, input, scene management
4. **Test rendering flow**: Verify the initialization → rendering path works end-to-end
5. **GPU command translation**: Begin translating Xenos GPU commands to OpenGL

---

## References

- ReXGlue recompiled code: `recomp/generated/tt-decomp_recomp.*.cpp`
- Symbol table: `config/434C4803/symbols.txt`
- Function analysis: `game_main @ 0x820C0378` (953 lines of ReXGlue code)
- Rendering functions: `rage_render_default @ 0x822C08D8`, `rage_render_scene @ 0x822C0B18`
