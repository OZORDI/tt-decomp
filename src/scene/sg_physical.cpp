#include "sg_physical.hpp"
#include "../physics/ph_physics.hpp"

// Draw bucket entry point — adds a rendered object to the current frame's
// draw bucket for deferred submission.
extern "C" void pongDrawBucket_AddEntry(void* bucket, void* entry);

/**
 * sgPhysicalObject::Transform
 * @ 0x823D9208 | size: 0x130
 *
 * Integrates physics state with the rendering pipeline:
 *   1. Bail early if no physics instance or the instance has no bound.
 *   2. Pick the transform source:
 *        – articulation present  → use phInst::m_pTransform (ragdoll source)
 *        – field_0x0068 non-null → use that pointer (blended/constrained pose)
 *        – otherwise             → use phInst::m_pTransform (base transform)
 *   3. Call virtual slot 9 for visibility/culling determination.
 *   4. If visible: store render distance + flags, add to draw bucket.
 *   5. If not visible and m_isHidden is false: call virtual slot 1 cleanup.
 */
void sgPhysicalObject::Transform() {
    if (!m_pPhysicsInstance)
        return;

    phInst* ph = m_pPhysicsInstance;

    // Bail if no collision bound attached
    if (!ph->field_0x0060)   // m_pBound @ +0x60
        return;

    // Pick transform source — articulation > blended pose > base
    void* renderPos;
    if (ph->field_0x006c)    // m_pArticulation @ +0x6C
        renderPos = reinterpret_cast<void*>(ph->field_0x0014);  // m_pTransform @ +0x14
    else if (ph->field_0x0068)
        renderPos = reinterpret_cast<void*>(ph->field_0x0068);
    else
        renderPos = reinterpret_cast<void*>(ph->field_0x0014);

    if (!renderPos)
        return;

    // TODO: virtual visibility/culling check (slot 9)
    bool isVisible = true;

    if (isVisible) {
        // TODO: pongDrawBucket_AddEntry(g_drawBucket, &renderData);
    } else if (!m_isHidden) {
        // TODO: call slot-1 (ScalarDtor/cleanup) virtual
    }
}

/**
 * sgPhysicalObject::PhysicsUpdate
 * @ 0x823D9368 | size: 0x14
 */
void sgPhysicalObject::PhysicsUpdate() {
    // TODO: advance physics simulation step
}

/**
 * sgPhysicalObject::AdvancedPhysics
 * @ 0x823D98C0 | size: 0x7C
 *
 * Handles ragdoll, articulation, and other complex physics operations.
 */
void sgPhysicalObject::AdvancedPhysics() {
    // TODO: implement ragdoll / articulation update
}
