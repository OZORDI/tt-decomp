/**
 * cloth.cpp — Cloth Physics System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements 8 functions across 4 cloth physics classes:
 *   phClothVerletInst     — 1 function  (destructor)
 *   phClothVerletBehavior — 4 functions (destructor, Reset, UpdateSimulation, AttachToCreatureInst)
 *   environmentCloth      — 2 functions (destructor + internal destroy body)
 *   clothController       — 1 function  (destructor)
 */

#include "include/physics/cloth.hpp"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// RAGE heap deallocator — frees memory allocated by rage::sysMemAllocator.
// @ 0x820C00C0
extern void rage_free(void* ptr);

// phClothVerletInst internal cleanup — tears down the Verlet mesh data.
// Called by ~phClothVerletInst before optional free.
// @ 0x822C8FD0
extern void phClothVerletInst_InternalDestroy(void* inst);

// phClothVerletBehavior internal cleanup — tears down bound/state data.
// Called by ~phClothVerletBehavior before optional free.
// @ 0x82421F70
extern void phClothVerletBehavior_InternalDestroy(void* behavior);

// clothController internal cleanup — tears down simulation state.
// Called by ~clothController before optional free.
// @ 0x823D8710
extern void clothController_InternalDestroy(void* controller);

// Level-volume table query — binds a cloth bound to the scene's collision volumes.
// Used by UpdateSimulation to initialise the simulation bounds.
// @ 0x82341A50
extern void lvlTable_BindBounds(void* bound, void* stateA, void* stateB);

// Physics bound query — extracts transform/capsule data from a phBound.
// @ 0x823405A8
extern void phBound_ExtractTransform(void* bound);

// Creature instance skin update — pushes cloth transforms to the renderer.
// @ 0x822C5D78
extern void pongCreatureInst_UpdateSkin(void* creatureMgr, float radius, uint16_t boneIndex);

// Debug trace — compiled to a single `blr` in release builds.
// @ 0x8240E6D0
extern void rage_debugLog(const char* msg);

// ─────────────────────────────────────────────────────────────────────────────
// Global data
// ─────────────────────────────────────────────────────────────────────────────

// Creature manager singleton (SDA-relative).
// @ 0x826065E4 (.data, 4 bytes) — SDA offset 26084
extern void* g_creatureMgr;

// Float constant — 0.0f, used for zero-initialisation in SIMD paths.
// @ 0x8202D110 (.rdata, 4 bytes)
extern const float kZeroFloat;

// Float constant — used as initial radius value for phBound setup.
// @ 0x8202D108 (.rdata, 4 bytes)
extern const float kInitialRadius;


namespace rage {

// ═══════════════════════════════════════════════════════════════════════════════
//  phClothVerletInst
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * phClothVerletInst::~phClothVerletInst  [vtable slot 0]
 * @ 0x823D7C90 | size: 0x50
 *
 * Standard RAGE deleting destructor.
 * Calls the internal cleanup function, then optionally frees memory
 * if the scalar-delete flag (r4 & 1) is set.
 */
phClothVerletInst::~phClothVerletInst()
{
    phClothVerletInst_InternalDestroy(this);
    // Note: `delete this` / rage_free is handled by the compiler-generated
    // scalar deleting destructor based on the flags parameter.
}


// ═══════════════════════════════════════════════════════════════════════════════
//  phClothVerletBehavior
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * phClothVerletBehavior::~phClothVerletBehavior  [vtable slot 0]
 * @ 0x82421F20 | size: 0x50
 *
 * Standard RAGE deleting destructor.
 */
phClothVerletBehavior::~phClothVerletBehavior()
{
    phClothVerletBehavior_InternalDestroy(this);
}

/**
 * phClothVerletBehavior::Reset  [vtable slot 1]
 * @ 0x823D7C50 | size: 0x40
 *
 * Resets both cloth tracking state blocks to their inactive state.
 * For each state block:
 *   - Always clears m_entryCount to 0
 *   - If m_refCount is already 0, also clears m_pDataArray and m_refCount
 *     (full reset when the state was never used)
 *
 * This allows Reset to be called safely on active states (m_refCount > 0)
 * without destroying the data array — only marking entries as consumed.
 */
void phClothVerletBehavior::Reset()
{
    // Reset state block A at +0x10
    if (m_stateA.m_refCount == 0) {
        m_stateA.m_refCount   = 0;
        m_stateA.m_pDataArray = 0;
    }
    m_stateA.m_entryCount = 0;

    // Reset state block B at +0x18
    if (m_stateB.m_refCount == 0) {
        m_stateB.m_refCount   = 0;
        m_stateB.m_pDataArray = 0;
    }
    m_stateB.m_entryCount = 0;
}

/**
 * phClothVerletBehavior::UpdateSimulation  [vtable slot 5]
 * @ 0x82421FD8 | size: 0x188
 *
 * Per-frame cloth simulation update.  Performs these steps:
 *   1. Copy the cloth bound's world transform (vec4 at +304) into a local
 *   2. Bind the cloth bound to the scene's collision volumes via lvlTable
 *   3. Extract the phBound transform for this frame
 *   4. Reset both state blocks (same logic as Reset)
 *   5. Read the bound's position vector from +288 and +304
 *   6. Build 4 transform vectors using SIMD for the cloth mesh:
 *      - Identity basis with kZeroFloat/kInitialRadius fill pattern
 *      - Copy into the creature physics instance (m_pPhysicsInst) at +16..+64
 *   7. Write the bound radius to the physics instance at +8
 *   8. Call the physics instance's vtable[37] to apply the transform
 *   9. Call vtable[6] on the physics instance to finalize
 *  10. Push the updated skin to the creature renderer via pongCreatureInst_UpdateSkin
 *
 * This function heavily uses VMX/AltiVec SIMD for 16-byte vector operations.
 */
void phClothVerletBehavior::UpdateSimulation()
{
    void* bound = m_pClothBound;

    // Copy world transform vec4 from bound+304 to local buffer
    float worldTransform[4];
    memcpy(worldTransform, (uint8_t*)bound + 304, 16);

    // Bind the bound to scene collision volumes
    lvlTable_BindBounds(bound, &m_stateA, &m_stateB);

    // Extract the current frame's transform from the bound
    phBound_ExtractTransform(bound);

    // Reset both state blocks (inline Reset logic)
    if (m_stateA.m_refCount == 0) {
        m_stateA.m_refCount   = 0;
        m_stateA.m_pDataArray = 0;
    }
    m_stateA.m_entryCount = 0;

    if (m_stateB.m_refCount == 0) {
        m_stateB.m_refCount   = 0;
        m_stateB.m_pDataArray = 0;
    }
    m_stateB.m_entryCount = 0;

    // Read bound position vectors
    void* boundBase = m_pClothBound;
    float* boundPos304 = (float*)((uint8_t*)boundBase + 304);  // world position
    float* boundPos288 = (float*)((uint8_t*)boundBase + 288);  // local position

    // Build transform vectors for the physics instance
    // Pattern: [kInitialRadius, kZeroFloat, kZeroFloat, ...] basis vectors
    void* physInst = m_pPhysicsInst;
    uint8_t* instBase = (uint8_t*)physInst;

    // Copy the world position vec4 into instance +16 (basis + offset)
    float basisVec[4] = { kInitialRadius, kZeroFloat, kZeroFloat, 0.0f };
    memcpy(instBase + 16, &basisVec, 16);

    float basisVec2[4] = { kZeroFloat, kInitialRadius, kZeroFloat, 0.0f };
    memcpy(instBase + 32, &basisVec2, 16);

    float basisVec3[4] = { kZeroFloat, kZeroFloat, kInitialRadius, 0.0f };
    memcpy(instBase + 48, &basisVec3, 16);

    // Copy the bound's local position to instance +64
    memcpy(instBase + 64, boundPos288, 16);

    // Write the bound radius from worldTransform to instance +8
    float radius = worldTransform[0];
    *(float*)(instBase + 8) = radius;

    // Splat radius to fill instance +112 (16 bytes, all same value)
    float radiusSplat[4] = { radius, radius, radius, radius };
    memcpy(instBase + 112, &radiusSplat, 16);

    // Call physics instance vtable[37] (apply transform)
    {
        void** vtbl = *(void***)physInst;
        typedef void (*ApplyFn)(void*);
        ((ApplyFn)vtbl[37])(physInst);
    }

    // Call vtable[6] on physics instance (finalize transform) with mirror plane
    {
        void** vtbl = *(void***)physInst;
        typedef void (*FinalizeFn)(void*, void*);
        extern const float g_playerMirrorPlane[4];  // @ 0x82606750
        ((FinalizeFn)vtbl[6])(physInst, (void*)g_playerMirrorPlane);
    }

    // Push updated cloth skin to the creature renderer
    uint16_t boneIndex = *((uint16_t*)((uint8_t*)m_pPhysicsInst + 8));
    pongCreatureInst_UpdateSkin(g_creatureMgr, worldTransform[0], boneIndex);
}

/**
 * phClothVerletBehavior::AttachToCreatureInst  [vtable slot 6]
 * @ 0x82422160 | size: 0x15C
 *
 * Attaches this cloth behavior to a creature instance by navigating the
 * creature's physics frame table to find matching cloth instances.
 *
 * The frame table (r4) has this layout:
 *   +0x0000  m_frameCount         — uint32_t: number of physics frames
 *   +0x42B0  m_pCurrentFrame      — void*: current active frame (may be null)
 *   +0x42B4  m_pNextFrame         — void*: next active frame (may be null)
 *   +0x42B8  m_pBoneMapping       — void*: bone->frame mapping table
 *
 * Each frame entry is 176 bytes, stored at frameTable + (index * 176).
 * Frame entry +0xB4 (180) stores the frame data pointer.
 *
 * The function checks if the current/next frame matches m_pPhysicsInst,
 * then navigates to the frame's sub-objects to find bone data.
 * If found, it calls vtable[1] and vtable[3] on the bone data to activate
 * the cloth, then records both the frame pointer and the bone data pointer
 * into m_stateA and m_stateB respectively.
 *
 * If the frame count is >= 255 (0xFF), the function logs a debug warning
 * and returns without attaching.
 */
void phClothVerletBehavior::AttachToCreatureInst(void* physFrameTable)
{
    // Guard: cloth must be enabled
    if (!m_bEnabled) {
        return;
    }

    uint8_t* table = (uint8_t*)physFrameTable;

    // Resolve the current frame — either from the cached pointer (+17072)
    // or by computing it from the frame count
    uint32_t cachedFrame = *(uint32_t*)(table + 17072);
    uint32_t currentFrame;
    if (cachedFrame != 0) {
        currentFrame = cachedFrame;
    } else {
        uint32_t frameCount = *(uint32_t*)(table + 0);
        uint32_t lastIdx = frameCount - 1;
        currentFrame = *(uint32_t*)(table + (lastIdx + 1) * 176);
    }

    uint32_t nextFrame;

    // Check if current frame matches our physics instance
    if (currentFrame == (uint32_t)(uintptr_t)m_pPhysicsInst) {
        // Match — resolve the next frame
        uint32_t cachedNext = *(uint32_t*)(table + 17076);
        if (cachedNext != 0) {
            nextFrame = cachedNext;
        } else {
            uint32_t frameCount = *(uint32_t*)(table + 0);
            uint32_t lastIdx = frameCount - 1;
            uint32_t offset = lastIdx * 176;
            nextFrame = *(uint32_t*)(table + offset + 180);
        }
    } else {
        // No match on current — try cached next or compute
        if (cachedFrame != 0) {
            nextFrame = cachedFrame;
        } else {
            uint32_t frameCount = *(uint32_t*)(table + 0);
            uint32_t lastIdx = frameCount - 1;
            nextFrame = *(uint32_t*)(table + (lastIdx + 1) * 176);
        }
    }

    // Resolve the bone mapping based on which frame matches
    uint32_t boneData;
    uint32_t cachedNext2 = *(uint32_t*)(table + 17076);
    if (nextFrame == cachedFrame) {
        boneData = *(uint32_t*)(table + 17064);
    } else if (nextFrame == cachedNext2) {
        boneData = *(uint32_t*)(table + 17068);
    } else {
        boneData = 0;
    }

    // Check frame count limit
    uint16_t frameHalfword = *(uint16_t*)((uint8_t*)this + 20);
    if (frameHalfword >= 255) {
        // Too many frames — log warning and bail
        rage_debugLog("phClothVerletBehavior::AttachToCreatureInst - too many frames");
        return;
    }

    // Activate cloth on the bone data object
    // Navigate: boneData -> +16 -> +4 -> +12 -> vtable[1](obj, 1) — activate
    uint32_t* boneObj = (uint32_t*)(uintptr_t)boneData;
    void* subObj1    = (void*)(uintptr_t)(*(uint32_t*)((uint8_t*)boneObj + 16));
    void* subObj2    = (void*)(uintptr_t)(*(uint32_t*)((uint8_t*)subObj1 + 4));
    void* targetObj  = (void*)(uintptr_t)(*(uint32_t*)((uint8_t*)subObj2 + 12));

    // Call vtable[1] on target — activate cloth
    {
        void** vtbl = *(void***)targetObj;
        typedef void (*ActivateFn)(void*, int);
        ((ActivateFn)vtbl[1])(targetObj, 1);
    }

    // Navigate again for vtable[3] — bind cloth to frame
    subObj1   = (void*)(uintptr_t)(*(uint32_t*)((uint8_t*)boneObj + 16));
    subObj2   = (void*)(uintptr_t)(*(uint32_t*)((uint8_t*)subObj1 + 4));
    targetObj = (void*)(uintptr_t)(*(uint32_t*)((uint8_t*)subObj2 + 12));

    // Call vtable[3] on target with boneData+160 as parameter — bind
    {
        void** vtbl = *(void***)targetObj;
        typedef void (*BindFn)(void*, void*);
        ((BindFn)vtbl[3])(targetObj, (void*)((uint8_t*)boneData + 160));
    }

    // Record the frame and bone data into state arrays
    // State A: record nextFrame at current entry index
    uint32_t* dataArrayA = (uint32_t*)(uintptr_t)m_stateA.m_pDataArray;
    uint16_t idxA = m_stateA.m_entryCount;
    dataArrayA[idxA] = nextFrame;
    m_stateA.m_entryCount = idxA + 1;

    // State B: record boneData at current entry index
    uint32_t* dataArrayB = (uint32_t*)(uintptr_t)m_stateB.m_pDataArray;
    uint16_t idxB = m_stateB.m_entryCount;
    dataArrayB[idxB] = boneData;
    m_stateB.m_entryCount = idxB + 1;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  environmentCloth
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * environmentCloth internal destroy body
 * @ 0x823D7D30 | size: 0x100
 *
 * Called by the destructor.  Sets the derived-class vtable to environmentCloth
 * (0x82071C34), then walks the 6 child object pointers at +4 through +24.
 * For each non-null child, calls virtual delete (vtable[0] with flags=1).
 * Finally resets the vtable to the base class rage::datBase (0x820276C4).
 */
static void environmentCloth_Destroy(environmentCloth* self)
{
    // Install environmentCloth vtable
    // (In the original binary this is: stw vtable_82071C34, 0(self))
    // This is implicit in C++ — we're already in the destructor chain.

    // Destroy all 6 child cloth objects
    for (int i = 0; i < 6; i++) {
        void* child = self->m_pClothObjects[i];
        if (child != nullptr) {
            // Virtual delete: call child->vtable[0](child, 1)
            void** childVtbl = *(void***)child;
            typedef void (*DtorFn)(void*, int);
            ((DtorFn)childVtbl[0])(child, 1);
        }
    }

    // Reset vtable to rage::datBase — the base destructor will run next.
    // (In C++ this happens automatically as the dtor chain unwinds.)
}

/**
 * environmentCloth::~environmentCloth  [vtable slot 0]
 * @ 0x823D7CE0 | size: 0x50
 *
 * Standard RAGE deleting destructor.
 * Calls the internal destroy body to clean up child objects,
 * then optionally frees memory.
 */
environmentCloth::~environmentCloth()
{
    environmentCloth_Destroy(this);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  clothController
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * clothController::~clothController  [vtable slot 0]
 * @ 0x823D86C0 | size: 0x50
 *
 * Standard RAGE deleting destructor.
 */
clothController::~clothController()
{
    clothController_InternalDestroy(this);
}

} // namespace rage
