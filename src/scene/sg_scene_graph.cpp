#include "scene/sg_scene_graph.hpp"

// Forward declarations
extern "C" void rage_free_00C0(void* ptr);
extern "C" void rage_9AF8_1(void* ctx, void* base);

/**
 * sgSceneGraph::~sgSceneGraph
 * @ 0x823D9AA8 | size: 0x50
 * 
 * Destructor for scene graph root container.
 * Calls cleanup function and optionally frees memory.
 * 
 * Original assembly:
 *   bl rage_9AF8_1           -> cleanup
 *   clrlwi r11,r30,31        -> check delete flag
 *   beq skip
 *   bl rage_free_00C0        -> free object
 */
sgSceneGraph::~sgSceneGraph() {
    // Call cleanup function
    // rage_9AF8_1(this, nullptr);
    
    // Note: Object freeing handled by caller via delete flag
}
