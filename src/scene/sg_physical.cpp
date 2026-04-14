#include "sg_physical.hpp"
#include "../physics/ph_physics.hpp"

// Draw bucket entry point -- adds a rendered object to the current frame's
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
 *        - articulation present  -> use articulated bone transform
 *        - field_0x0068 non-null -> use that pointer (blended/constrained pose)
 *        - otherwise             -> use phInst::m_pBoundData + 16 (base transform)
 *   3. Call phBound virtual slot 9 for visibility/culling determination.
 *   4. If visible: store render distance + flags, add to draw bucket.
 *   5. If not visible and m_bIsHidden is false: call render-entry cleanup (slot 1).
 */
void sgPhysicalObject::Transform() {
    if (!m_pPhysicsInstance)
        return;

    phInst* ph = m_pPhysicsInstance;

    // Bail if no collision bound attached
    if (!ph->m_fMassInv)   // phBound* m_pBound @ phInst+0x60
        return;

    // Pick transform source -- articulation > blended pose > base
    void* renderPos;
    if (ph->m_fDamping)    // m_pArticulation @ phInst+0x6C
    {
        // Compute articulated transform from bone data
        uint32_t* pArticulation = (uint32_t*)(uintptr_t)ph->m_fDamping;
        uint32_t* pSkeletonData = (uint32_t*)(uintptr_t)pArticulation[3];   // +12
        uint32_t* pBoneTable    = (uint32_t*)(uintptr_t)pSkeletonData[16];  // +64
        uint32_t  nBoneIndex    = pSkeletonData[17];                         // +68
        uint32_t  pBaseTransforms = pBoneTable[5];                           // +20
        // Each bone transform is a 4x4 matrix (64 bytes); compute offset
        renderPos = (void*)(uintptr_t)(pBaseTransforms + nBoneIndex * 64);
    }
    else if (ph->m_fRestitution)   // m_pTransformSrc @ phInst+0x68
    {
        renderPos = (void*)(uintptr_t)ph->m_fRestitution;
    }
    else
    {
        // Fall back to base transform: m_pBoundData + 16
        renderPos = (void*)(uintptr_t)(ph->m_pBoundData + 16);
    }

    if (!renderPos)
        return;

    // Build render data on the stack for visibility check
    struct RenderData {
        uint8_t  isVisible;
        uint8_t  pad[3];
        float    renderDistance;
    };
    RenderData renderData = { 0, {0}, 0.0f };

    // Get the collision bound (phBound*)
    void* pBound = (void*)(uintptr_t)ph->m_fMassInv;

    // Get LOD/camera data from renderer state
    uint32_t* pRendererState = (uint32_t*)g_rendererState;
    void* pCameraData    = (void*)(uintptr_t)pRendererState[10];            // +40
    void* pLodData       = (void*)(uintptr_t)*(uint32_t*)((char*)pCameraData + 44);
    void* pLodThreshold  = (void*)(uintptr_t)*(uint32_t*)((char*)pLodData + 152);

    // Call visibility/culling check -- phBound virtual slot 9
    typedef int (*BoundCullCheckFunc)(void*, void*, void*, uint8_t*, float*, void*);
    void** boundVtable = *(void***)pBound;
    BoundCullCheckFunc cullCheck = (BoundCullCheckFunc)boundVtable[9];
    int isVisible = cullCheck(pBound, renderPos, pLodThreshold, &renderData.isVisible,
                              &renderData.renderDistance, nullptr);

    if (isVisible) {
        // Store render state into sgPhysicalObject fields
        m_fRenderDistance = renderData.renderDistance;
        m_nVisibility    = renderData.isVisible;

        // Get AABB from bound -- phBound virtual slot 10
        typedef void* (*BoundGetAABBFunc)(void*);
        BoundGetAABBFunc getAABB = (BoundGetAABBFunc)boundVtable[10];
        void* pAABBData = getAABB(pBound);

        // Submit render entry (+0x60) to draw bucket
        void* pRenderEntry = (void*)&m_pRenderEntryVtable;
        pongDrawBucket_AddEntry(g_drawBucket, pRenderEntry, pAABBData);
    } else if (!m_bIsHidden) {
        // Object not visible and not flagged as hidden -- cleanup via render-entry vtable slot 1
        void* pRenderEntry = (void*)&m_pRenderEntryVtable;
        void** renderVtable = *(void***)pRenderEntry;
        typedef void (*RenderEntryCleanupFunc)(void*);
        RenderEntryCleanupFunc cleanup = (RenderEntryCleanupFunc)renderVtable[1];
        cleanup(pRenderEntry);
    }
}

/**
 * sgPhysicalObject::PhysicsUpdate
 * @ 0x823D9368 | size: 0x14
 *
 * Tail-calls phInst virtual slot 5 (Update method).
 */
void sgPhysicalObject::PhysicsUpdate() {
    // phInst virtual slot 5 = Update
    typedef void (*PhInstUpdateFunc)(void*);
    void** vtable = *(void***)m_pPhysicsInstance;
    PhInstUpdateFunc update = (PhInstUpdateFunc)vtable[5];
    update((void*)m_pPhysicsInstance);
}

/**
 * sgPhysicalObject::AdvancedPhysics
 * @ 0x823D98C0 | size: 0x7C
 *
 * Creates a physics instance: articulated (ragdoll/skeleton) or simple (rigid body).
 */
void sgPhysicalObject::AdvancedPhysics() {
    if (m_bIsArticulated) {
        // Create articulated physics instance (ragdoll/skeleton)
        void* pNewInstance = CreatePhysicsInstance(g_physicsSystem, m_pPhysicsType, m_pResource);
        m_pPhysicsInstance = (phInst*)pNewInstance;

        // Copy secondary data from physics type (+4)
        m_nSecondaryData = *(uint32_t*)((char*)m_pPhysicsType + 4);
    } else {
        // Create simple physics instance (rigid body)
        void* pNewInstance = CreateSimplePhysicsInstance(g_physicsSystem, m_pPhysicsType);
        m_pPhysicsInstance = (phInst*)pNewInstance;

        // Link the resource/mesh to the physics instance
        LinkPhysicsResource((void*)pNewInstance, m_pResource);
    }
}
