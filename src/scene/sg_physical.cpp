#include "sg_physical.hpp"
#include "../physics/ph_physics.hpp"

// Draw bucket entry point — adds a rendered object to the current frame's
// draw bucket for deferred submission.
extern "C" void pongDrawBucket_AddEntry(void* bucket, void* renderEntry, void* boundsData);

// Physics system functions
extern "C" void* CreatePhysicsInstance(void* physSystem, void* physType, void* resource);  // @ 0x8227F6A8
extern "C" void* CreateSimplePhysicsInstance(void* physSystem, void* physType);             // @ 0x8227F810
extern "C" void  LinkPhysicsResource(void* physInstance, void* resource);                   // @ 0x8227D220

// Global physics system singleton
extern void* g_physicsSystem;           // @ 0x82606568
// Global renderer state (contains camera/LOD data)
extern void* g_rendererState;           // @ 0x8260641C
// Global draw bucket for deferred rendering
extern void* g_drawBucket;              // @ 0x826064F4

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
    {
        // Compute articulated transform from bone data
        void* articulationData = reinterpret_cast<void*>(ph->field_0x006c);
        uint32_t* artPtr = (uint32_t*)((char*)articulationData + 12);
        uint32_t boneTable = *(uint32_t*)((char*)*artPtr + 64);
        uint32_t boneIndex = *(uint32_t*)((char*)*artPtr + 68);
        uint32_t baseTransforms = *(uint32_t*)((char*)boneTable + 20);
        renderPos = (void*)(((boneIndex << 6) & 0xFFFFFFC0) + baseTransforms);
    }
    else if (ph->field_0x0068)
        renderPos = reinterpret_cast<void*>(ph->field_0x0068);
    else
        renderPos = reinterpret_cast<void*>(reinterpret_cast<char*>(ph->field_0x0014) + 16);

    if (!renderPos)
        return;

    // Build render data on the stack for visibility check
    struct RenderData {
        uint8_t  isVisible;
        uint8_t  pad[3];
        float    renderDistance;
    };
    RenderData renderData = { 0, {0}, 0.0f };

    // Get the collision bound
    void* pBound = reinterpret_cast<void*>(ph->field_0x0060);

    // Get LOD/camera data from renderer state
    uint32_t* pRendererState = (uint32_t*)g_rendererState;
    void* pCameraData = (void*)pRendererState[10];  // +40
    void* pLodData = (void*)*(uint32_t*)((char*)pCameraData + 44);
    void* pLodThreshold = (void*)*(uint32_t*)((char*)pLodData + 152);

    // Call visibility/culling check (vtable slot 9 on the bound)
    typedef int (*CullCheckFunc)(void*, void*, void*, uint8_t*, float*, void*);
    void** boundVtable = *(void***)pBound;
    CullCheckFunc cullCheck = (CullCheckFunc)boundVtable[9];
    int isVisible = cullCheck(pBound, renderPos, pLodThreshold, &renderData.isVisible,
                              &renderData.renderDistance, nullptr);

    if (isVisible) {
        // Store render state
        *(float*)((char*)this + 0x80) = renderData.renderDistance;
        *(uint32_t*)((char*)this + 0x84) = renderData.isVisible;

        // Get AABB from bound (vtable slot 10)
        typedef void* (*GetAABBFunc)(void*);
        GetAABBFunc getAABB = (GetAABBFunc)boundVtable[10];
        void* aabbData = getAABB(pBound);

        // Submit to draw bucket
        void* renderDataPtr = (char*)this + 0x88;
        pongDrawBucket_AddEntry(g_drawBucket, renderDataPtr, aabbData);
    } else if (!*(uint8_t*)((char*)this + 0x8C)) {
        // Object not visible and not flagged as hidden — cleanup
        typedef void (*CleanupFunc)(void*);
        void* renderDataPtr = (char*)this + 0x88;
        void** renderVtable = *(void***)renderDataPtr;
        CleanupFunc cleanup = (CleanupFunc)renderVtable[1];
        cleanup(renderDataPtr);
    }
}

/**
 * sgPhysicalObject::PhysicsUpdate
 * @ 0x823D9368 | size: 0x14
 */
void sgPhysicalObject::PhysicsUpdate() {
    // Tail call to physics instance's Update method (vtable slot 5)
    typedef void (*UpdateFunc)(void*);
    void** vtable = *(void***)m_pPhysicsInstance;
    UpdateFunc update = (UpdateFunc)vtable[5];
    update(m_pPhysicsInstance);
}

/**
 * sgPhysicalObject::AdvancedPhysics
 * @ 0x823D98C0 | size: 0x7C
 *
 * Handles ragdoll, articulation, and other complex physics operations.
 */
void sgPhysicalObject::AdvancedPhysics() {
    // Check articulation flag at +0x72
    uint8_t isArticulated = *(uint8_t*)((char*)this + 0x72);
    // Physics type descriptor at +0x74
    void* pPhysicsType = *(void**)((char*)this + 0x74);

    if (isArticulated) {
        // Create articulated physics instance (ragdoll/skeleton)
        void* pNewInstance = CreatePhysicsInstance(g_physicsSystem, pPhysicsType, m_pResource);
        m_pPhysicsInstance = (phInst*)pNewInstance;

        // Copy secondary data from physics type (+4) to this (+0x78)
        uint32_t secondaryData = *(uint32_t*)((char*)pPhysicsType + 4);
        *(uint32_t*)((char*)this + 0x78) = secondaryData;
    } else {
        // Create simple physics instance (rigid body)
        void* pNewInstance = CreateSimplePhysicsInstance(g_physicsSystem, pPhysicsType);
        m_pPhysicsInstance = (phInst*)pNewInstance;

        // Link the resource/mesh to the physics instance
        LinkPhysicsResource(pNewInstance, m_pResource);
    }
}
