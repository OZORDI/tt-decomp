#pragma once

/**
 * Input System - Rockstar Table Tennis (Xbox 360)
 * 
 * Professional AAA game input pipeline with:
 * - Hardware abstraction (Xbox 360 XInput → SDL2)
 * - Configuration management (dead zones, sensitivity)
 * - Event processing (button press/release/hold)
 * - Analog filtering (stick/trigger smoothing)
 * - Gesture recognition (combos, timing)
 */

#include <stdint.h>

namespace rage {

// Maximum number of controllers supported (Xbox 360 standard)
constexpr int MAX_CONTROLLERS = 4;

/**
 * Input configuration - tuning parameters for feel
 * Size: 80 bytes (16-byte aligned)
 * 
 * Contains float constants loaded from game data for:
 * - Dead zone thresholds
 * - Sensitivity curves
 * - Smoothing factors
 * - Acceleration curves
 */
struct InputConfig {
    int32_t stateFlag;                    // +0x00: Initialization state
    float   primarySensitivity;           // +0x04: Base sensitivity
    float   reserved1[2];                 // +0x08: Padding/reserved
    float   leftStickDeadZone;            // +0x10: Left stick dead zone
    float   leftStickSensitivity;         // +0x14: Left stick sensitivity
    float   leftStickAcceleration;        // +0x18: Left stick acceleration curve
    float   reserved2;                    // +0x1C: Padding/reserved
    float   rightStickDeadZone;           // +0x20: Right stick dead zone
    float   rightStickSensitivity;        // +0x24: Right stick sensitivity
    float   rightStickAcceleration;       // +0x28: Right stick acceleration curve
    float   triggerDeadZone;              // +0x2C: Trigger dead zone (reused value)
    float   triggerSensitivity;           // +0x30: Trigger sensitivity
    float   smoothingFactor;              // +0x34: Input smoothing factor
    float   responseTime;                 // +0x38: Response time constant
    float   vibrationIntensity;           // +0x3C: Vibration multiplier
    float   gestureThreshold;             // +0x40: Gesture detection threshold
    float   reserved3[3];                 // +0x44: Padding to 80 bytes
};

/**
 * Raw gamepad state - hardware interface
 * Size: 52 bytes (16-byte aligned)
 * 
 * Manages connection to Xbox 360 controllers:
 * - Controller handles (-1 = disconnected)
 * - Connection state flags
 * - Polymorphic device interface (vtable)
 */
struct GamepadRawState {
    void*   vtable;                       // +0x00: Device interface vtable
    uint8_t connectionFlags[2];           // +0x04: Connection state flags
    uint8_t reserved1[2];                 // +0x06: Padding
    int32_t controllerHandles[MAX_CONTROLLERS]; // +0x08: XInput handles (-1 = invalid)
    uint8_t deviceFlags[20];              // +0x18: Per-device state flags
    uint8_t reserved2[8];                 // +0x2C: Padding to 52 bytes
};

/**
 * Processed gamepad state - game-ready input with events
 * Size: 52 bytes (16-byte aligned)
 * 
 * Filtered input with dead zones applied and button events generated.
 * Uses dual vtables for multiple inheritance (InputDevice + EventSource).
 */
struct GamepadProcessedState {
    void*    vtablePrimary;               // +0x00: InputDevice interface
    void*    vtableEvents;                // +0x04: EventSource interface (button events)
    uint8_t  processedData[44];           // +0x08-0x33: Processed input data (zero-initialized)
};

/**
 * Complete gamepad state - main input structure
 * Size: ~120 bytes (16-byte aligned)
 * 
 * This is the COMPLETE structure used by UpdateGamepadState @ 0x8221FA50.
 * Contains vtables, calibration, connection status, and normalized analog inputs.
 * 
 * Memory layout discovered through deep PowerPC assembly analysis.
 */
struct GamepadState {
    // Vtable pointers (12 bytes)
    void*    vtable1;                     // +0x00: Primary interface
    void*    vtable2;                     // +0x04: Secondary interface
    void*    vtable2Copy;                 // +0x08: Copy of vtable2 (updated each frame)
    
    // Trigger calibration (4 bytes)
    uint8_t  triggerCenterValues[4];      // +0x0C-0x0F: All set to 128 for symmetric range
    
    // Unknown region 1 (12 bytes) - likely button state, packet number, etc.
    uint8_t  reserved1[12];               // +0x10-0x1B
    
    // Connection status (1 byte)
    bool     connected;                   // +0x1C: Set to false when XamInputGetState fails
    
    // Unknown region 2 (71 bytes) - button history, event state, timing, etc.
    uint8_t  reserved2[71];               // +0x1D-0x63
    
    // Normalized analog inputs (11 bytes with padding)
    // These are the final processed values after normalization (÷256)
    int8_t   leftStickX;                  // +0x64 (100): -128 to +127
    uint8_t  pad1;                        // +0x65 (101): Padding
    int8_t   leftStickY;                  // +0x66 (102): -128 to +127
    int8_t   rightStickX;                 // +0x67 (103): -128 to +127
    int8_t   rightStickY;                 // +0x68 (104): -128 to +127
    uint8_t  pad2;                        // +0x69 (105): Padding
    uint8_t  leftTrigger;                 // +0x6A (106): 0 to 255
    uint8_t  pad3;                        // +0x6B (107): Padding
    uint8_t  rightTrigger;                // +0x6C (108): 0 to 255
    uint8_t  pad4;                        // +0x6D (109): Padding
    uint8_t  dpadOrButtons;               // +0x6E (110): Additional input state
    
    // Padding to alignment
    uint8_t  reserved3[5];                // +0x6F-0x73: Padding to 16-byte boundary
};

/**
 * Analog input state - stick/trigger tracking with history
 * Size: 84 bytes (16-byte aligned)
 * 
 * Tracks analog input over time for smoothing:
 * - Current stick/trigger positions
 * - Previous frame values (for delta/velocity)
 * - Smoothing buffers
 */
struct AnalogInputState {
    void*   vtable;                       // +0x00: Analog processor interface
    void*   vtableSecondary;              // +0x04: Secondary interface
    int32_t stateValues[2];               // +0x08: State tracking
    uint16_t analogHistory[4];            // +0x10: History buffer
    int32_t processingState[4];           // +0x18: Processing state
    uint16_t smoothingBuffer[8];          // +0x28: Smoothing buffer
    float   currentPositions[4];          // +0x38: Current analog positions
    float   previousPositions[2];         // +0x48: Previous frame positions
    float   velocities[2];                // +0x50: Calculated velocities
    uint8_t reserved[4];                  // +0x58: Padding to 84 bytes
};

/**
 * Button history state - combo/gesture detection
 * Size: 60 bytes (16-byte aligned)
 * 
 * Tracks button timing for combos:
 * - Button state history (6-bit masks)
 * - Press timestamps (for duration)
 * - Combo detection state
 */
struct ButtonHistoryState {
    float   timestamps[3];                // +0x00: Press timestamps
    int32_t historyDepth[3];              // +0x0C: History depth counters
    uint8_t buttonHistory[3];             // +0x18: Button state history (6-bit masks)
    uint8_t reserved1;                    // +0x1B: Padding
    int32_t comboState;                   // +0x1C: Combo detection state
    uint8_t comboFlags;                   // +0x20: Combo flags
    uint8_t reserved2[3];                 // +0x21: Padding
    float   holdDurations[4];             // +0x24: Button hold durations
    int32_t gestureState;                 // +0x34: Gesture recognition state
    uint8_t reserved3[4];                 // +0x38: Padding to 60 bytes
};

/**
 * Input Manager - singleton that manages the complete input pipeline
 * 
 * Initialized by atSingleton_vfn_1 @ 0x821C7948
 * Updated every frame (60 FPS) to process controller input
 */
class InputManager {
public:
    // Master initialization function (called once at game startup)
    // atSingleton_vfn_1 @ 0x821C7948 | size: 3208 bytes (0xC88)
    static void* Initialize();
    
    // Initialization functions (called once at startup)
    static InputConfig*            CreateConfig();           // xe_9DC8 @ 0x821C9DC8
    static GamepadRawState*        CreateRawState();         // xe_9ED8 @ 0x821C9ED8
    static GamepadProcessedState*  CreateProcessedState();   // xe_9FC0 @ 0x821C9FC0
    static AnalogInputState*       CreateAnalogState();      // xe_A090 @ 0x821CA090
    static ButtonHistoryState*     CreateButtonHistory();    // xe_A188 @ 0x821CA188
    
    // Main update function (called every frame at 60 FPS)
    // SinglesNetworkClient_FA50 @ 0x8221FA50 | size: 772 bytes
    static void UpdateGamepadState(GamepadState* state);
};

} // namespace rage
