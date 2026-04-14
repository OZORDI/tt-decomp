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

// ---------------------------------------------------------------------------
// Traversal visitor thunks
//
// Each visitor's Apply is a single indirect tail-call into a fixed slot of
// the target node's own vtable. The compiler emits them all as:
//     lwz  r11, 0(r4)        ; load pNode->vtable
//     mr   r3,  r4           ; this = pNode
//     lwz  r10, SLOT*4(r11)  ; vtable[slotN]
//     mtctr r10 / bctr       ; tail-call
//
// The per-visitor difference is exactly the slot index; treating them as a
// family keeps the intent visible and avoids duplicating the dispatch logic.
// ---------------------------------------------------------------------------
namespace {

typedef void (*sgNodeVisitFn)(sgNode*);

// Indices into sgNode's vtable (slot-index, not byte offset).
enum sgNodeVisitorSlot : unsigned {
    kSlotInitialize = 23,  // byte  92 — Initialize()
    kSlotTerminate  = 24,  // byte  96 — Terminate()
    kSlotReset      = 25,  // byte 100 — Reset()
    kSlotRestart    = 26,  // byte 104 — Restart()
    kSlotUpdate     = 27,  // byte 108 — Update()
    kSlotPostUpdate = 28,  // byte 112 — PostUpdate()
    kSlotDraw       = 29,  // byte 116 — Draw()
    kSlotDrawDebug  = 30,  // byte 120 — DrawDebug()
};

// Fetch the node's virtual at the given slot and invoke it on pNode.
// Matches the `lwz r11,0(r4) / lwz r10,SLOT*4(r11) / mtctr / bctr` pattern.
static inline void DispatchNodeVisit(sgNode* pNode, sgNodeVisitorSlot slot) {
    sgNodeVisitFn fn = reinterpret_cast<sgNodeVisitFn>(pNode->vtable[slot]);
    fn(pNode);
}

}  // namespace

/**
 * sgTraverseInitialize::Apply
 * @ 0x823D9F10 | size: 0x14
 *
 * Invokes the node's Initialize() virtual (sgNode vtable slot 23).
 */
void sgTraverseInitialize::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotInitialize);
}

/**
 * sgTraverseTerminate::Apply
 * @ 0x823D9FB8 | size: 0x14
 *
 * Invokes the node's Terminate() virtual (sgNode vtable slot 24).
 */
void sgTraverseTerminate::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotTerminate);
}

/**
 * sgTraverseReset::Apply
 * @ 0x823D9F28 | size: 0x14
 *
 * Invokes the node's Reset() virtual (sgNode vtable slot 25).
 */
void sgTraverseReset::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotReset);
}

/**
 * sgTraverseRestart::Apply
 * @ 0x823D9F40 | size: 0x14
 *
 * Invokes the node's Restart() virtual (sgNode vtable slot 26).
 */
void sgTraverseRestart::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotRestart);
}

/**
 * sgTraverseUpdate::Apply
 * @ 0x823D9F58 | size: 0x14
 *
 * Invokes the node's Update() virtual (sgNode vtable slot 27).
 */
void sgTraverseUpdate::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotUpdate);
}

/**
 * sgTraversePostUpdate::Apply
 * @ 0x823D9F70 | size: 0x14
 *
 * Invokes the node's PostUpdate() virtual (sgNode vtable slot 28).
 */
void sgTraversePostUpdate::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotPostUpdate);
}

/**
 * sgTraverseDraw::Apply
 * @ 0x823D9F88 | size: 0x14
 *
 * Invokes the node's Draw() virtual (sgNode vtable slot 29).
 */
void sgTraverseDraw::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotDraw);
}

/**
 * sgTraverseDrawDebug::Apply
 * @ 0x823D9FA0 | size: 0x14
 *
 * Invokes the node's DrawDebug() virtual (sgNode vtable slot 30).
 */
void sgTraverseDrawDebug::Apply(sgNode* pNode) {
    DispatchNodeVisit(pNode, kSlotDrawDebug);
}
