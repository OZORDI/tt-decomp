/*
 * RAGE Render System Header
 * Professional RAGE engine organization
 */

#ifndef RAGE_RENDER_HPP
#define RAGE_RENDER_HPP

#include <cstdint>
#include "../game/game_logic.hpp"
#include "../graphics/graphics_system.hpp"

// RAGE render constants - organized by category
namespace RageConstants {
    // Render flags
    constexpr uint32_t RENDER_FLAG_COLOR = 0x1;
    constexpr uint32_t RENDER_FLAG_DEPTH = 0x2;
    constexpr uint32_t RENDER_FLAG_STENCIL = 0x4;
    constexpr uint32_t RENDER_FLAG_ALL = RENDER_FLAG_COLOR | RENDER_FLAG_DEPTH | RENDER_FLAG_STENCIL;
    
    // Scene flags
    constexpr uint32_t SCENE_FLAG_ACTIVE = 0x1;
    constexpr uint32_t SCENE_FLAG_VISIBLE = 0x2;
    constexpr uint32_t SCENE_FLAG_DIRTY = 0x4;
    
    // Object flags
    constexpr uint32_t OBJECT_FLAG_RENDERABLE = 0x1;
    constexpr uint32_t OBJECT_FLAG_VISIBLE = 0x2;
    constexpr uint32_t OBJECT_FLAG_TRANSPARENT = 0x4;
}

// Render object structure
struct RenderObject {
    void* vtable;
    uint32_t object_type;
    uint32_t render_flags;
    void* next;
    void* geometry;
    void* material;
    void* transform;
    float bounding_sphere_radius;
    uint32_t draw_order;
};

// Scene structure
struct Scene {
    bool initialized;
    uint32_t flags;
    RenderObject* object_list_head;
    uint32_t object_count;
    void* camera;
    void* lighting;
    void* environment;
};

// Render vtable structure
struct RenderVTable {
    int (*render_0x80)(void* obj);
    int (*render_0x84)(void* obj);
    int (*render_0x88)(void* obj);
    int (*render_0x8C)(void* obj);
    int (*render_0x90)(void* obj);
    int (*render_0x94)(void* obj);
    int (*render_0x98)(void* obj);
    int (*render_0x9C)(void* obj);
};

// RAGE render interface - clean API design
namespace Rage {
    // Core render functions
    int initialize_render_system();
    int shutdown_render_system();
    
    // Scene management
    int create_scene();
    int destroy_scene();
    int add_render_object(RenderObject* obj);
    int remove_render_object(RenderObject* obj);
    int update_scene();
    
    // Rendering functions
    int render_default();
    int render_scene();
    int render_object(RenderObject* obj);
    
    // Scene cleanup
    int cleanup_scene();
    int validate_scene();
    
    // Object management
    int create_render_object(uint32_t type, void* geometry, void* material);
    int destroy_render_object(RenderObject* obj);
    int set_object_transform(RenderObject* obj, void* transform);
    int set_object_visibility(RenderObject* obj, bool visible);
    
    // Performance monitoring
    int get_render_object_count();
    int get_render_call_count();
    float get_average_render_time();
}

// Global RAGE render state
extern Scene g_scene;
extern uint32_t g_render_flags;

#endif // RAGE_RENDER_HPP
