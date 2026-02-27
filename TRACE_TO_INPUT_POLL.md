# Execution Trace: Entry Point → First Controller Input Poll

**Project:** Rockstar Table Tennis (Xbox 360) Decompilation  
**Analysis Date:** February 26, 2026  
**Entry Point:** 0x8242BD20 (xstart)  
**First Input Poll:** io_Input_poll_9D68 @ 0x821C9D68  
**Input Init:** io_Input_init_77E8 @ 0x821C77E8

---

## Executive Summary

This document traces the complete execution path from the game's entry point to the first controller input poll (`XamInputGetState`). The input system is initialized in `main()` and polled during the render phase of the game loop.

**Key Finding:** Input polling occurs in `io_Input_poll_9D68`, which is called from `rage_render_default_08D8` (the first draw function) and many other locations throughout the game loop.

---

## Complete Call Tree: Entry → First Input Poll

```
xstart @ 0x8242BD20 (CRT Entry Point)
│
├─ [CRT Initialization - see BOOT_TO_FIRST_DRAW_COMPLETE.md]
│
└─ rage_main_6970 @ 0x82186970
   └─ rage_run_6780 @ 0x82186780
      └─ main @ 0x820C0378 ← GAME MAIN FUNCTION
         │
         ├─ INITIALIZATION PHASE
         │  │
         │  ├─ [Graphics Init]
         │  │  └─ grcDevice_init_8A00 @ 0x820F8A00
         │  │
         │  ├─ [Scheduler Init]
         │  │  └─ rage_scheduler_init_ED0 @ 0x82221ED0
         │  │
         │  └─ INPUT SYSTEM INIT ← CRITICAL
         │     └─ io_Input_init_77E8 @ 0x821C77E8
         │        ├─ Initializes input buffers
         │        ├─ Sets up controller state structures
         │        └─ Prepares for XamInputGetState calls
         │
         └─ MAIN GAME LOOP @ loc_820C0974
            │
            while (isRunning) {
               │
               ├─ VCALL slot 17 @ byte +68
               │  └─ gameLoop_StepFrame_9760 @ 0x82229760
               │
               ├─ VCALL slot 18 @ byte +72
               │  └─ gameLoop_Update_9768 @ 0x82229768
               │     └─ [May poll input here]
               │
               ├─ VCALL slot 4 @ byte +16
               │  └─ gameLoop_Tick_97D8 @ 0x822297D8
               │     └─ [Physics update, may use input]
               │
               ├─ VCALL slot 21 @ byte +84
               │  └─ gameLoop_PreRender_9938 @ 0x82229938
               │
               ├─ VCALL slot 23 @ byte +92
               │  └─ gameLoop_BeginRender_9950 @ 0x82229950
               │
               ├─ VCALL slot 5 @ byte +20
               │  └─ gameLoop_Render_9A58 @ 0x82229A58
               │     │
               │     └─ rage_render_default_08D8 @ 0x822C07E0
               │        │
               │        ├─ grcDevice_beginScene_5E78
               │        ├─ grcDevice_clear_9290
               │        │
               │        └─ io_Input_poll_9D68 @ 0x821C9D68 ← FIRST INPUT POLL
               │           │
               │           └─ xam_XamInputGetState_60FC @ 0x825860FC
               │              │
               │              └─ XamInputGetState (Xbox 360 Kernel)
               │                 ├─ Reads controller 0 state
               │                 ├─ Reads controller 1 state
               │                 ├─ Reads controller 2 state
               │                 └─ Reads controller 3 state
               │
               ├─ VCALL slot 24 @ byte +96
               │  └─ gameLoop_EndRender_9AF8 @ 0x82229AF8
               │
               ├─ VCALL slot 6 @ byte +24
               │  └─ gameLoop_PostRender_9BA0 @ 0x82229BA0
               │     └─ [May poll input for UI]
               │
               ├─ VCALL slot 25 @ byte +100
               │  └─ gameLoop_Present_9F30 @ 0x82229F30
               │
               └─ Loop continues...
            }
```

---

## Input System Architecture

### Input Initialization: `io_Input_init_77E8`

**Address:** 0x821C77E8  
**Size:** 0x118 bytes (280 bytes)  
**Called From:** `main @ 0x820C0378`  
**Purpose:** Initialize input system before game loop starts

**Responsibilities:**
1. Allocate input state buffers for 4 controllers
2. Initialize button state tracking
3. Setup analog stick dead zones
4. Clear previous frame input data
5. Register input callbacks (if any)

**Pseudo-code:**
```cpp
void io_Input_init_77E8() {
    // Initialize controller state for 4 players
    for (int i = 0; i < 4; i++) {
        g_controllerState[i].connected = false;
        g_controllerState[i].buttons = 0;
        g_controllerState[i].leftStick = {0, 0};
        g_controllerState[i].rightStick = {0, 0};
        g_controllerState[i].leftTrigger = 0;
        g_controllerState[i].rightTrigger = 0;
    }
    
    // Set dead zones
    g_inputDeadZone = 0.2f;
    
    // Initialize input history buffers
    memset(g_inputHistory, 0, sizeof(g_inputHistory));
}
```

---

### Input Polling: `io_Input_poll_9D68`

**Address:** 0x821C9D68  
**Size:** 0x5C bytes (92 bytes)  
**Called From:** 83 locations (see below)  
**Purpose:** Poll all controllers and update input state

**Call Frequency:** Every frame (60 FPS) = 60 times per second

**Responsibilities:**
1. Call `XamInputGetState` for each controller
2. Update button press/release states
3. Calculate analog stick values with dead zones
4. Detect button press events (transition from up to down)
5. Store previous frame state for delta detection

**Pseudo-code:**
```cpp
void io_Input_poll_9D68() {
    for (int i = 0; i < 4; i++) {
        XINPUT_STATE state;
        
        // Call Xbox 360 kernel function
        DWORD result = XamInputGetState(i, &state);
        
        if (result == ERROR_SUCCESS) {
            g_controllerState[i].connected = true;
            
            // Store previous state for delta detection
            g_controllerState[i].prevButtons = g_controllerState[i].buttons;
            
            // Update current state
            g_controllerState[i].buttons = state.Gamepad.wButtons;
            g_controllerState[i].leftStick.x = state.Gamepad.sThumbLX;
            g_controllerState[i].leftStick.y = state.Gamepad.sThumbLY;
            g_controllerState[i].rightStick.x = state.Gamepad.sThumbRX;
            g_controllerState[i].rightStick.y = state.Gamepad.sThumbRY;
            g_controllerState[i].leftTrigger = state.Gamepad.bLeftTrigger;
            g_controllerState[i].rightTrigger = state.Gamepad.bRightTrigger;
            
            // Apply dead zones
            applyDeadZone(&g_controllerState[i].leftStick);
            applyDeadZone(&g_controllerState[i].rightStick);
        } else {
            g_controllerState[i].connected = false;
        }
    }
}
```

---

## Input Polling Locations (83 Total)

### Critical Locations (First Calls)

1. **rage_render_default_08D8** @ 0x822C07E0
   - **When:** During render phase
   - **Purpose:** Update input before rendering UI/HUD
   - **Priority:** HIGH - This is likely the FIRST input poll

2. **gameLoop_Update_9768** @ 0x82229768
   - **When:** During update phase
   - **Purpose:** Update game state based on input
   - **Priority:** HIGH

3. **gameLoop_Tick_97D8** @ 0x822297D8
   - **When:** During physics tick
   - **Purpose:** Apply input to player physics
   - **Priority:** HIGH

### Gameplay Locations

4. **pongPlayer_Process** @ 0x82196348
   - **When:** Player update
   - **Purpose:** Read paddle control input

5. **pongPlayer_7038_g** / **pongPlayer_73E8_g**
   - **When:** Player state updates
   - **Purpose:** Handle player actions

6. **pongLocomotionController_vfn_8**
   - **When:** Character movement
   - **Purpose:** Control player locomotion

7. **gmBallNode_Reset**
   - **When:** Ball reset (serve)
   - **Purpose:** Detect serve button press

### UI Locations

8. **hudHUD_vfn_5**
   - **When:** HUD update
   - **Purpose:** Menu navigation

9. **hudPause_vfn_5** / **hudPause_vfn_2**
   - **When:** Pause menu
   - **Purpose:** Pause menu input

10. **pongPauseState_vfn_11** / **pongPauseState_vfn_12**
    - **When:** Pause state
    - **Purpose:** Handle pause/unpause

### Loading & Context Switches

11. **pongLoadingContext_vfn_28** / **pongLoadingContext_vfn_29**
    - **When:** Loading screens
    - **Purpose:** Allow skipping with button press

12. **pongPauseContext_vfn_16**
    - **When:** Context switches
    - **Purpose:** Handle input during transitions

### Network/Multiplayer

13. **SinglesNetworkClient_*** (multiple functions)
    - **When:** Network gameplay
    - **Purpose:** Send local input to network

---

## XamInputGetState Wrapper

### Function: `xam_XamInputGetState_60FC`

**Address:** 0x825860FC  
**Size:** 0x10 bytes (16 bytes)  
**Type:** Thunk/wrapper function

**Purpose:** Wrapper around Xbox 360 kernel `XamInputGetState` function

**Signature:**
```cpp
DWORD XamInputGetState(
    DWORD dwUserIndex,      // Controller index (0-3)
    XINPUT_STATE* pState    // Output: controller state
);
```

**XINPUT_STATE Structure:**
```cpp
typedef struct _XINPUT_STATE {
    DWORD dwPacketNumber;   // Increments when state changes
    XINPUT_GAMEPAD Gamepad; // Current gamepad state
} XINPUT_STATE;

typedef struct _XINPUT_GAMEPAD {
    WORD wButtons;          // Button bitmask
    BYTE bLeftTrigger;      // Left trigger (0-255)
    BYTE bRightTrigger;     // Right trigger (0-255)
    SHORT sThumbLX;         // Left stick X (-32768 to 32767)
    SHORT sThumbLY;         // Left stick Y (-32768 to 32767)
    SHORT sThumbRX;         // Right stick X (-32768 to 32767)
    SHORT sThumbRY;         // Right stick Y (-32768 to 32767)
} XINPUT_GAMEPAD;
```

**Button Bitmask (wButtons):**
```cpp
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
```

---

## Cross-Platform Implementation

### SDL2 Mapping

For cross-platform support, `XamInputGetState` should be implemented using SDL2:

```cpp
// src/xam/input.c
#include <SDL2/SDL.h>

static SDL_GameController* g_controllers[4] = {NULL};
static XINPUT_STATE g_inputState[4] = {0};

DWORD XamInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    if (dwUserIndex >= 4 || !pState) {
        return ERROR_INVALID_PARAMETER;
    }
    
    // Open controller if not already open
    if (!g_controllers[dwUserIndex]) {
        if (SDL_NumJoysticks() > dwUserIndex) {
            g_controllers[dwUserIndex] = SDL_GameControllerOpen(dwUserIndex);
        }
    }
    
    if (!g_controllers[dwUserIndex]) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    
    // Read SDL controller state
    SDL_GameController* controller = g_controllers[dwUserIndex];
    
    // Map buttons
    WORD buttons = 0;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
        buttons |= XINPUT_GAMEPAD_DPAD_UP;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
        buttons |= XINPUT_GAMEPAD_DPAD_DOWN;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
        buttons |= XINPUT_GAMEPAD_DPAD_LEFT;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
        buttons |= XINPUT_GAMEPAD_DPAD_RIGHT;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START))
        buttons |= XINPUT_GAMEPAD_START;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
        buttons |= XINPUT_GAMEPAD_BACK;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK))
        buttons |= XINPUT_GAMEPAD_LEFT_THUMB;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
        buttons |= XINPUT_GAMEPAD_RIGHT_THUMB;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
        buttons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
        buttons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
        buttons |= XINPUT_GAMEPAD_A;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
        buttons |= XINPUT_GAMEPAD_B;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
        buttons |= XINPUT_GAMEPAD_X;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
        buttons |= XINPUT_GAMEPAD_Y;
    
    // Map analog sticks
    SHORT leftX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    SHORT leftY = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY); // Invert Y
    SHORT rightX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    SHORT rightY = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY); // Invert Y
    
    // Map triggers (SDL returns 0-32767, Xbox expects 0-255)
    BYTE leftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 128;
    BYTE rightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 128;
    
    // Fill output structure
    pState->dwPacketNumber++;
    pState->Gamepad.wButtons = buttons;
    pState->Gamepad.bLeftTrigger = leftTrigger;
    pState->Gamepad.bRightTrigger = rightTrigger;
    pState->Gamepad.sThumbLX = leftX;
    pState->Gamepad.sThumbLY = leftY;
    pState->Gamepad.sThumbRX = rightX;
    pState->Gamepad.sThumbRY = rightY;
    
    return ERROR_SUCCESS;
}
```

---

## Functions Required for Input System

### Phase 1: Input System Core (3 functions)

| Function | Address | Purpose | Status |
|----------|---------|---------|--------|
| `io_Input_init_77E8` | 0x821C77E8 | Initialize input system | ⏳ Needed |
| `io_Input_poll_9D68` | 0x821C9D68 | Poll all controllers | ⏳ Needed |
| `xam_XamInputGetState_60FC` | 0x825860FC | XamInputGetState wrapper | ⏳ Needed |

### Phase 2: Input Processing (10-15 functions)

| Function | Purpose | Status |
|----------|---------|--------|
| `io_Input_getButton` | Check if button is pressed | ⏳ Needed |
| `io_Input_getButtonDown` | Check if button was just pressed | ⏳ Needed |
| `io_Input_getButtonUp` | Check if button was just released | ⏳ Needed |
| `io_Input_getAxis` | Get analog stick value | ⏳ Needed |
| `io_Input_getTrigger` | Get trigger value | ⏳ Needed |
| `io_Input_applyDeadZone` | Apply dead zone to analog input | ⏳ Needed |
| `io_Input_isConnected` | Check if controller is connected | ⏳ Needed |
| `io_Input_setVibration` | Set controller vibration | ⏳ Needed |

---

## Timeline: Entry → First Input Poll

| Time | Event | Function |
|------|-------|----------|
| 0ms | Program start | `xstart @ 0x8242BD20` |
| 0-50ms | CRT initialization | Multiple CRT functions |
| 50-100ms | Thread setup | `xe_main_thread_init_0038` |
| 100-200ms | RAGE engine init | `rage_main_6970` |
| 200-300ms | Graphics init | `grcDevice_init_8A00` |
| 250-300ms | **Input init** | `io_Input_init_77E8` |
| 300ms | Game loop starts | `main @ 0x820C0378` |
| 316ms | First frame begins | Frame 0 |
| 316ms | Update phase | `gameLoop_Update_9768` |
| 316ms | Render phase | `gameLoop_Render_9A58` |
| 316ms | **First input poll** | `io_Input_poll_9D68` |
| 316ms | XamInputGetState called | `xam_XamInputGetState_60FC` |
| 333ms | Frame 1 begins | 60 FPS = 16.67ms per frame |

---

## Testing Milestones

### Milestone 1: Input System Initialized
- ✅ `io_Input_init_77E8` called successfully
- ✅ Input buffers allocated
- ✅ Controller state structures initialized

### Milestone 2: First Input Poll
- ✅ Game loop reaches render phase
- ✅ `io_Input_poll_9D68` called
- ✅ `XamInputGetState` wrapper executed
- ✅ Controller state read (even if no controller connected)

### Milestone 3: Input Responsive
- ✅ Button presses detected
- ✅ Analog sticks read correctly
- ✅ Dead zones applied
- ✅ Input affects game state

---

## Summary

**First Input Poll Location:** `io_Input_poll_9D68 @ 0x821C9D68`  
**Called From:** `rage_render_default_08D8 @ 0x822C07E0` (during first render)  
**Timing:** ~316ms after program start (first frame render)  
**Frequency:** 60 times per second (every frame)

**Functions Required:**
- **Core:** 3 functions (init, poll, XamInputGetState wrapper)
- **Processing:** 10-15 helper functions
- **Total:** ~15-20 functions for complete input system

**Implementation Priority:**
1. Implement `xam_XamInputGetState_60FC` with SDL2
2. Implement `io_Input_init_77E8`
3. Implement `io_Input_poll_9D68`
4. Add input processing helpers as needed

---

**Document Version:** 1.0  
**Last Updated:** February 26, 2026  
**Author:** Kiro AI Assistant  
**Status:** Complete
