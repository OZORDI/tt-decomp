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

// ---------------------------------------------------------------------------
// sgTypedIterator — stateful pre-order walker with a cmSimple type filter
//
// Prime and Advance are mutually recursive through the iterator state:
//   Advance() moves m_pCurrent to the next pre-order node, then calls Prime()
//   Prime()   checks the type filter; on miss it logs and calls Advance()
//
// Together they land m_pCurrent on the next cmSimple-compatible node or
// leave it nullptr when the walk is exhausted.
// ---------------------------------------------------------------------------

namespace {

// cmSimple's vtable — used as the opaque type ID by sgNode::IsType.
// Loaded as a global rdata address (`lis -32163; lwz -27696`), i.e.
// 0xFFFF82050000 - 27696 sign-extended = 0x820562D0.
extern void* const kCmSimpleFilterVtable;

// Assertion / trace format stub (4-byte `blr`-only stripped helper).
// Declared here rather than pulled in from pong_player.cpp to keep the
// scene-graph TU self-contained for lookups during link.
extern "C" void nop_8240E6D0(const char* fmt, const char* arg);   // @ 0x8240E6D0

// Rodata string passed to nop_8240E6D0 as the fmt arg. 34 bytes at rdata
// 0x82071DC8 — content not reconstructable from the recomp scaffold, so it
// is described rather than reproduced here.
extern const char kTypeMismatchFmt[];   // @ 0x82071DC8

}  // namespace

/**
 * sgTypedIterator::Prime
 * @ 0x823DA070 | size: 0x88
 *
 * If m_pCurrent is non-null, asks it IsType(cmSimple_vtable) (slot 20).
 *   - returns true → iterator sits on a matching node, nothing to do.
 *   - returns false → log the mismatch via the node's slot-19 getter +
 *                     the shared nop/log stub, then Advance() past it.
 *
 * No-op when m_pCurrent is already nullptr (walk already finished).
 */
void sgTypedIterator::Prime() {
    sgNode* pCurrent = m_pCurrent;
    if (pCurrent == nullptr) {
        return;
    }

    // Slot 20 — sgNode::IsType(typeId). typeId is the cmSimple vtable
    // address; the base sgNode implementation simply compares it against
    // a static per-derived-class pointer (see sgNode_vfn_20).
    typedef int (*IsTypeFn)(sgNode*, void*);
    IsTypeFn isType = reinterpret_cast<IsTypeFn>(pCurrent->vtable[20]);
    if (isType(pCurrent, kCmSimpleFilterVtable)) {
        return;  // match: leave m_pCurrent where it is
    }

    // Mismatch path: fetch the node's slot-19 accessor (likely
    // GetDebugName / GetTypeName) and hand its result, plus a fixed
    // rodata fmt string, to the stripped logger stub.
    typedef const char* (*SlotFn)(sgNode*);
    SlotFn slot19 = reinterpret_cast<SlotFn>(m_pCurrent->vtable[19]);
    const char* pInfo = slot19(m_pCurrent);
    nop_8240E6D0(kTypeMismatchFmt, pInfo);

    // Advance() will re-Prime(), so the loop continues until we land on
    // a match or exhaust the walk.
    Advance();
}

/**
 * sgTypedIterator::Advance
 * @ 0x823D9FD0 | size: 0x9C
 *
 * Pre-order successor walk over the scene-graph tree rooted at m_pRoot.
 * Successor search order for the current node C:
 *   1. C->m_pFirstChild   (descend)
 *   2. if C is the root, terminate (m_pCurrent := nullptr)
 *   3. C->m_pNextSibling  (step right)
 *   4. C->m_pParent->m_pNextSibling  (one climb; further climbs would
 *      be done by the recursive Prime()/Advance() pair but the lifted
 *      scaffold only models the single-level uncle step)
 *
 * After updating m_pCurrent, re-Prime() so the filter is re-checked.
 */
void sgTypedIterator::Advance() {
    sgNode* pCurrent = m_pCurrent;
    sgNode* pNext    = nullptr;

    if (pCurrent != nullptr) {
        // Step 1: descend to first child if any.
        sgNode* pChild = pCurrent->m_pFirstChild;
        if (pChild != nullptr) {
            pNext = pChild;
        } else if (pCurrent == m_pRoot) {
            // Step 2: root with no children — walk over.
            pNext = nullptr;
        } else {
            // Step 3: try next sibling.
            sgNode* pSibling = pCurrent->m_pNextSibling;
            if (pSibling != nullptr) {
                pNext = pSibling;
            } else {
                // Step 4: climb once, then try parent's sibling.
                sgNode* pParent = pCurrent->m_pParent;
                if (pParent != nullptr && pParent != m_pRoot &&
                    pParent->m_pNextSibling == nullptr) {
                    Prime();  // tail-recursive call into Prime (matches asm)
                    return;
                }
                pNext = (pParent != nullptr) ? pParent->m_pNextSibling : nullptr;
            }
        }
    }

    m_pCurrent = pNext;
    Prime();  // re-filter the new position
}

/**
 * sgTraverseInitialize::Traverse
 * @ 0x823D9E90 | size: 0x7C
 *
 * Iterator-driven visitor loop: snap the iterator's current pointer to its
 * root, Prime() to land on the first type-matched node, then repeatedly
 * invoke this->Apply(current) and advance until the iterator is exhausted.
 *
 * Matches the PPC body:
 *   r11 = pIter->m_pRoot;           // +0x00
 *   pIter->m_pCurrent = r11;        // +0x04 (reset)
 *   pIter->Prime();
 *   while (pIter->m_pCurrent != nullptr) {
 *       this->Apply(pIter->m_pCurrent);   // VCALL slot 1 on `this`
 *       pIter->Advance();
 *   }
 */
void sgTraverseInitialize::Traverse(sgTypedIterator* pIter) {
    // Reset walk cursor to root. The lifted asm reads +0x00 and writes
    // +0x04, so m_pCurrent starts equal to m_pRoot.
    pIter->m_pCurrent = pIter->m_pRoot;
    pIter->Prime();

    while (pIter->m_pCurrent != nullptr) {
        // VCALL slot 1 — this->Apply(current). Virtual so derived visitor
        // types (TraverseUpdate/TraverseDraw/…) would hook in via this
        // same driver, though only the Initialize specialization was
        // emitted per the symbol table.
        this->Apply(pIter->m_pCurrent);
        pIter->Advance();
    }
}
