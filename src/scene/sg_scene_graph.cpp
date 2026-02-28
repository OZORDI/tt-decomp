#include "sg_scene_graph.hpp"

// RAGE engine heap free — canonical name declared in rage/rage_cm_types.hpp
extern void rage_free(void* ptr);               // @ 0x820C00C0

// sgSceneGraph internal destructor body @ 0x823D9AF8
// Frees the seven traverse-list pointers at offsets +0x10..+0x2C
// (skipping +0x1C), then calls atArray_Clear (rage::gameObject base dtor).
// Named after its address in the translation unit that contains sgSceneGraph.
extern void sgSceneGraph_FreeLists(void* obj);  // @ 0x823D9AF8

/**
 * sgSceneGraph::~sgSceneGraph
 * @ 0x823D9AA8 | size: 0x50
 *
 * Resets the vtable to rage::gameObject's vtable, then calls
 * sgSceneGraph_FreeLists which frees all seven traversal-list
 * allocations and invokes the rage::gameObject base destructor.
 * The scalar-dtor wrapper handles the optional rage_free(this).
 */
sgSceneGraph::~sgSceneGraph() {
    sgSceneGraph_FreeLists(this);
}
