/*
 * GRC Graphics System Header
 * Professional graphics engine organization
 */

#ifndef GRAPHICS_SYSTEM_HPP
#define GRAPHICS_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// Graphics constants - organized by category
namespace GraphicsConstants {
    // Display settings
    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    
    // Clear colors
    constexpr float CLEAR_COLOR_R = 0.0f;
    constexpr float CLEAR_COLOR_G = 0.0f;
    constexpr float CLEAR_COLOR_B = 0.0f;
    constexpr float CLEAR_COLOR_A = 1.0f;
    
    // Depth settings
    constexpr float CLEAR_DEPTH = 1.0f;
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FAR_PLANE = 1000.0f;
    
    // Viewport settings
    constexpr int VIEWPORT_X = 0;
    constexpr int VIEWPORT_Y = 0;
    
    // Command buffer
    constexpr int COMMAND_BUFFER_SIZE = 256;
}

// Graphics device structure
struct GraphicsDevice {
    bool initialized;
    int width;
    int height;
    void* context;  // OpenGL context
    uint32_t frame_buffer;
    
    // Command buffer simulation
    struct {
        uint32_t commands[GraphicsConstants::COMMAND_BUFFER_SIZE];
        int write_index;
        int read_index;
    } cmd_buffer;
};

// Renderer state structure
struct RendererState {
    bool initialized;
    
    // Viewport settings
    struct {
        int x, y, width, height;
    } viewport;
    
    // Scissor settings
    struct {
        int x, y, width, height;
        bool enabled;
    } scissor;
    
    // Render state
    struct {
        bool depth_test_enabled;
        bool cull_face_enabled;
        uint32_t clear_flags;
        float clear_color[4];
        float clear_depth;
    } render_state;
};

// Clear descriptor structure
struct ClearDescriptor {
    uint32_t viewport_x, viewport_y, viewport_w, viewport_h;
    uint32_t pad1[2];
    float clear_color[4];
    float clear_depth;
    uint32_t clear_stencil;
    uint32_t clear_flags;
};

// Graphics interface - clean API design
namespace Graphics {
    // Core device functions
    int initialize_device();
    int shutdown_device();
    int begin_scene();
    int clear_framebuffer(const ClearDescriptor* clear_desc);
    int present_frame();
    
    // Renderer functions
    int initialize_renderer();
    int set_viewport(int x, int y, int width, int height);
    int set_scissor(int x, int y, int width, int height, bool enabled);
    int set_render_state(bool depth_test, bool cull_face);
    
    // Command buffer functions
    int write_command(uint32_t command);
    int flush_commands();
    int reset_command_buffer();
    
    // State management
    int get_device_info();
    int get_renderer_info();
    int validate_graphics_state();
    
    // Performance monitoring
    int get_frame_count();
    float get_average_frame_time();
    int get_command_count();
}

// Global graphics state
extern GraphicsDevice g_graphics_device;
extern RendererState g_renderer_state;

#endif // GRAPHICS_SYSTEM_HPP
