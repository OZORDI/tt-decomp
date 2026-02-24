/*
 * Xbox 360 Input System Header
 * Professional input engine organization
 */

#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// Input constants - organized by category
namespace InputConstants {
    // Controller buttons
    constexpr int BUTTON_A = 0x1000;
    constexpr int BUTTON_B = 0x2000;
    constexpr int BUTTON_X = 0x4000;
    constexpr int BUTTON_Y = 0x8000;
    constexpr int BUTTON_LB = 0x0100;
    constexpr int BUTTON_RB = 0x0200;
    constexpr int BUTTON_LS = 0x0040;
    constexpr int BUTTON_RS = 0x0080;
    constexpr int BUTTON_BACK = 0x0020;
    constexpr int BUTTON_START = 0x0010;
    constexpr int BUTTON_GUIDE = 0x0800;
    
    // D-pad
    constexpr int DPAD_UP = 0x0001;
    constexpr int DPAD_DOWN = 0x0002;
    constexpr int DPAD_LEFT = 0x0004;
    constexpr int DPAD_RIGHT = 0x0008;
    
    // Triggers
    constexpr int TRIGGER_LEFT = 0x100;
    constexpr int TRIGGER_RIGHT = 0x200;
    
    // Analog stick ranges
    constexpr float STICK_MIN = -32768.0f;
    constexpr float STICK_MAX = 32767.0f;
    constexpr float STICK_CENTER = 0.0f;
    constexpr float STICK_DEADZONE = 0.2f;
    
    // Trigger ranges
    constexpr float TRIGGER_MIN = 0.0f;
    constexpr float TRIGGER_MAX = 255.0f;
    constexpr float TRIGGER_DEADZONE = 0.1f;
    
    // Input processing
    constexpr int MAX_CONTROLLERS = 4;
    constexpr int INPUT_BUFFER_SIZE = 64;
    constexpr int POLL_INTERVAL_MS = 16;  // 60 FPS
}

// Controller state structure
struct ControllerState {
    bool connected;
    uint32_t buttons;
    uint16_t dpad;
    uint8_t left_trigger;
    uint8_t right_trigger;
    float left_stick_x;
    float left_stick_y;
    float right_stick_x;
    float right_stick_y;
    uint32_t packet_number;
    
    // Vibration
    struct {
        uint8_t left_motor;
        uint8_t right_motor;
    } vibration;
};

// Input device structure
struct InputDevice {
    bool initialized;
    int num_controllers;
    ControllerState controllers[InputConstants::MAX_CONTROLLERS];
    void* context;  // SDL input context
    
    // Input processing
    struct {
        uint32_t input_queue[InputConstants::INPUT_BUFFER_SIZE];
        int queue_head;
        int queue_tail;
    } input_queue;
    
    // VTable for polymorphic input handling
    void* input_vtable;
};

// Input configuration structure
struct InputConfig {
    bool enable_vibration;
    float stick_deadzone;
    float trigger_deadzone;
    bool invert_y_axis;
    bool invert_x_axis;
    float sensitivity;
    int button_mapping[16];  // Button remapping
};

// Input vtable structure
struct InputVTable {
    int (*process_input_0)(void* device);
    int (*process_input_1)(void* device);
    int (*process_input_2)(void* device);
};

// Input interface - clean API design
namespace Input {
    // Core input functions
    int initialize_system();
    int shutdown_system();
    int start_input();
    int stop_input();
    
    // XAM input functions
    int get_controller_state(int controller_id, ControllerState* state);
    int set_controller_state(int controller_id, const ControllerState* state);
    
    // RAGE input functions
    int poll_input_devices();
    int process_input_frame();
    
    // Input vtable functions
    int vtable_process_input_0(void* device);
    int vtable_process_input_1(void* device);
    int vtable_process_input_2(void* device);
    
    // Input processing
    int map_buttons(uint32_t raw_buttons);
    int normalize_analog_sticks(float raw_x, float raw_y, float* norm_x, float* norm_y);
    int normalize_triggers(uint8_t raw_trigger, float* norm_trigger);
    int apply_deadzone(float value, float deadzone);
    
    // Input validation
    int validate_input_state();
    int validate_controller_state(const ControllerState* state);
    
    // Performance monitoring
    int get_input_latency();
    float get_input_frequency();
    int get_dropped_inputs();
}

// Global input state
extern InputDevice g_input_device;
extern InputConfig g_input_config;

#endif // INPUT_SYSTEM_HPP
