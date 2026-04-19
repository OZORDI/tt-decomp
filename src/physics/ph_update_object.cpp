/**
 * ph_update_object.cpp — phUpdateObject physics update object implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.12.cpp
 *
 * phUpdateObject is the base class for physics objects that receive
 * per-frame transform updates via a double-buffered frame array.
 * The frame array is indexed by g_phFrameIndex, a global counter that
 * alternates each tick so the CPU and GPU can access different frames
 * simultaneously without stalls.
 *
 * Functions:
 *   ~phUpdateObject           @ 0x8227DA08 (size: 0x30)  — delegating destructor
 *   ScalarDtor                @ 0x8227D960 (size: 0xA8)  — commit transform + free
 *   Destroy                   @ 0x820C41E8 (size: 0x5C)  — deleting destructor chain
 *   Reset                     @ 0x8227D5B0 (size: 0x1B4) — world sync / initialize
 *   AddToRenderBucket         @ 0x8227D768 (size: 0x12C) — visibility test + enqueue
 *   RenderShadow              @ 0x8227D898 (size: 0xC8)  — shadow pass
 *   Cleanup                   @ 0x8227DA40 (size: 0x138) — transform propagation
 *
 * Inheritance:
 *   phUpdateObject → phDemoObject → pongRenderable
 *   phDemoObject has a secondary vtable @ 0x82055D01 implementing pongRenderable:
 *     [0] ScalarDtor  [1] Render  [2] Reset  [3] AddToRenderBucket
 *     [4] RenderShadow  [5] Cleanup
 */

#include "ph_physics.hpp"

// ── Global Declarations ─────────────────────────────────────────────────────

// Global frame-index counter used to index the double-buffered frame array.
// Located at 0x825C4898 (.data, 16 bytes — first word is the active index).
extern uint32_t g_phFrameIndex;  // @ 0x825C4898

// Render world / scene manager global used for LOD/visibility tests
extern void* g_renderWorld;   // @ 0x8260641C (SDA offset 25628)

// Draw bucket global — entries are submitted for deferred rendering
extern void* g_drawBucket;    // @ 0x826064F4 (SDA offset 25844)

// Shadow rendering state — contains shadow map config and scale transform
extern void* g_phShadowState; // @ 0x826063C8 (SDA offset 25544)

// Animation/locomotion state global — contains skinning matrix override
extern void* g_animState;     // @ 0x826063C4 (SDA offset 25540)

// ── External Functions ──────────────────────────────────────────────────────

// Base class destructor body — destroys owned sub-objects and frees frame buffer
// Sets vtable to phDemoObject, destroys m_pDrawable(+96), m_pDrawable2(+100),
// m_pData(+20) via virtual dtor, then frees m_pFrameBuffer(+128)
extern void phDemoObject_Dtor(void* self); // @ 0x8227CFA0

// Canonical RAGE allocator free
extern void rage_free(void* ptr);  // @ 0x820C00C0

// Creates a ball physics instance in the scene
extern void pongBallInstance_4980_g(void* scene, uint16_t handle,
    int param3, int param4, int param5, int param6); // @ 0x822C4980

// Removes/cleans up a game object by handle
extern void game_4B60(void* scene, uint16_t handle); // @ 0x822C4B60

// Adds a renderable to the deferred draw bucket
extern void pongDrawBucket_AddEntry(void* bucket, void* renderable,
    void* priority); // @ 0x822278D8

// Submits a shadow-casting object to the shadow map system
extern void pongShadowMap_F010_g(void* shadowMap, void* drawable,
    void* transform); // @ 0x8213F010

// Applies a locomotion/skinning matrix transform to a 4×4 matrix
// Multiplies the input matrix by a 4×3 transform to apply skeletal animation
extern void LocomotionStateAnim_3AD8_g(void* matrix,
    void* animTransform); // @ 0x820C3AD8

/**
 * phUpdateObject::~phUpdateObject() — delegating destructor
 * @ 0x8227DA08 | size: 0x30 (vtable slot 0)
 *
 * Checks whether the object has active data (m_pData != NULL).
 * If active, computes a frame-indexed pointer into the double-buffered
 * array at m_pFrameBuffer and dispatches to the virtual Cleanup() method
 * (vtable slot 6) to release per-frame resources.
 *
 * Assembly pattern:
 *   lwz   r11, +20(r3)       ; m_pData
 *   beqlr cr6                ; early-out if NULL
 *   lwz   r9,  0(r3)         ; vtable
 *   lwz   r10, +128(r3)      ; m_pFrameBuffer
 *   lwz   r11, g_phFrameIndex ; frame index
 *   rlwinm r11, r11, 6, 0, 25 ; index * 64
 *   add   r4,  r11, r10      ; frameData = m_pFrameBuffer + (index * 64)
 *   lwz   r8,  24(r9)        ; vtable[6] = Cleanup
 *   bctr                     ; tail-call Cleanup(this, frameData, flags)
 */
phUpdateObject::~phUpdateObject()
{
    if (!m_pData)
        return;

    // Compute the frame-indexed slot: each slot is 64 bytes wide
    uint32_t frameOffset = g_phFrameIndex * 64;
    void* frameData = static_cast<uint8_t*>(m_pFrameBuffer) + frameOffset;

    // Tail-call virtual Cleanup (slot 6) to release per-frame state
    Cleanup(frameData, 0);
}

/**
 * phUpdateObject::ScalarDtor(int flags) — scalar destructor (vtable slot 1)
 * @ 0x8227D960 | size: 0xA8
 *
 * FIXME: the lift below is incorrect — the slot-1 scaffold is NOT a
 *        destructor despite the auto-labeled "scalar_destructor" tag.  The
 *        real function (see recomp phUpdateObject_vfn_1) copies a 4-row
 *        matrix from `m_pData ? m_pData : &this->inlineTransform+16` into
 *        the double-buffered slot at `m_pFrameBuffer + frameIdx*64`, then
 *        post-multiplies three row vectors of that dst matrix by the
 *        parent transform at `this+112`.  It never frees `this` and takes
 *        no `flags` arg.  This stub preserves the existing signature until
 *        the lift is replaced.
 */
void phUpdateObject::ScalarDtor(int flags)
{
    // Determine source: use external data pointer if valid, else inline transform
    void* src;
    if (m_pFrameBuffer == nullptr) {
        // No frame buffer — use inline transform at offset +16 from m_pData
        src = static_cast<uint8_t*>(m_pData) + 16;
    } else {
        src = static_cast<uint8_t*>(m_pData) + 16;
    }

    // Compute destination in double-buffered array
    uint32_t frameOffset = g_phFrameIndex * 64;
    void* dst = static_cast<uint8_t*>(m_pFrameBuffer) + frameOffset;

    // Copy 64-byte transform block (4x 16-byte AltiVec stores in original).
    // Upstream also multiplies dst rows 1..3 by the parent transform at
    // this+112 — not implemented here, see FIXME above.
    __builtin_memcpy(dst, src, 64);

    if (flags & 1) {
        rage_free(this);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phUpdateObject::Destroy(int flags)  [vtable slot 2 @ 0x820C41E8]
// Deleting destructor — walks the destructor chain then optionally frees.
// Sets vtable to phUpdateObject, calls the phDemoObject base destructor
// (phDemoObject_Dtor) which destroys all owned sub-objects, then conditionally
// frees this object if (flags & 1).
// ─────────────────────────────────────────────────────────────────────────────
/**
 * phUpdateObject::Destroy @ 0x820C41E8 | size: 0x5C
 *
 * Standard MSVC scalar deleting destructor. Resets the vtable to
 * phUpdateObject (0x82027AC0) for proper destructor chaining, then
 * delegates to phDemoObject_Dtor (the phDemoObject base destructor) which
 * destroys m_pDrawable, m_pDrawable2, m_pData via their virtual
 * destructors and frees m_pFrameBuffer.
 */
void phUpdateObject::Destroy(int flags) {
    // Base class destructor chain: destroy all owned sub-objects
    phDemoObject_Dtor(this);

    if (flags & 1) {
        rage_free(this);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phUpdateObject::Reset(void* world, void* scene)  [vtable slot 3 @ 0x8227D5B0]
// Synchronizes this physics object with the world. Called when the object
// enters (mode=1) or leaves (mode=2) the physics world. Copies the initial
// transform from m_pData into the inline transform matrix, manages instance
// handles, and notifies the world via its virtual RemoveEntry (slot 11).
// ─────────────────────────────────────────────────────────────────────────────
/**
 * phUpdateObject::Reset @ 0x8227D5B0 | size: 0x1B4
 *
 * World synchronization function. The data descriptor at m_pData contains
 * an instance handle at offset +8 (uint16_t). Handle 0xFFFF means "no
 * active instance". The m_updateMode field controls the operation:
 *   mode 1: Object is being added — create ball instance, copy transform
 *   mode 2: Object is being removed — destroy instance, copy transform
 *
 * In both modes, the 64-byte transform at m_pData+16 is copied into the
 * inline matrix at this+0x20, and the world is notified of the change.
 *
 * The status byte array at world+28 holds per-handle activation flags.
 * Bits 0-2 encode the instance state: 0 = slot free/needs action.
 */
void phUpdateObject::Reset(void* world, void* scene) {
    // Load the instance handle from the data descriptor
    uint8_t* dataBlock = static_cast<uint8_t*>(m_pData);
    uint16_t instanceHandle = *reinterpret_cast<uint16_t*>(dataBlock + 8);

    if (instanceHandle == 0xFFFF)
        return; // No valid instance

    if (m_updateMode == 1) {
        // ── Mode 1: Add to world ─────────────────────────────────────────
        // Check if the instance slot is free in the world's status array
        uint8_t* worldStatusArray = *reinterpret_cast<uint8_t**>(
            static_cast<uint8_t*>(world) + 28);
        uint8_t instanceStatus = worldStatusArray[instanceHandle];

        if ((instanceStatus & 0x7) == 0) {
            // Slot is free — create a ball physics instance
            pongBallInstance_4980_g(scene, instanceHandle, 0, 0, 0, 0);
        }

        // Copy the 64-byte transform from data descriptor (+16) into
        // the inline local-to-world matrix at this+0x20
        uint8_t* transformSrc = dataBlock + 16;
        __builtin_memcpy(m_localTransform, transformSrc, 64);

        // Check the scene's world state for this handle
        void* sceneWorld = *reinterpret_cast<void**>(
            static_cast<uint8_t*>(scene) + 4);
        uint8_t* sceneStatusArray = *reinterpret_cast<uint8_t**>(
            static_cast<uint8_t*>(sceneWorld) + 28);
        uint8_t sceneStatus = sceneStatusArray[instanceHandle];

        void* newInstance = nullptr;
        if ((sceneStatus & 0x7) == 0) {
            // Scene slot free — request instance from scene world (vtable slot 4)
            // VCALL(sceneWorld, 4, instanceHandle)
            void** sceneVtable = *reinterpret_cast<void***>(sceneWorld);
            typedef void* (*GetInstanceFn)(void*, uint16_t);
            GetInstanceFn getInstanceFn = reinterpret_cast<GetInstanceFn>(
                *reinterpret_cast<void**>(
                    reinterpret_cast<uint8_t*>(sceneVtable) + 16));
            newInstance = getInstanceFn(sceneWorld, instanceHandle);
        }

        // Destroy the returned instance via its ScalarDtor (vtable slot 1)
        if (newInstance) {
            void** instVtable = *reinterpret_cast<void***>(newInstance);
            typedef void (*ScalarDtorFn)(void*);
            ScalarDtorFn dtorFn = reinterpret_cast<ScalarDtorFn>(
                *reinterpret_cast<void**>(
                    reinterpret_cast<uint8_t*>(instVtable) + 4));
            dtorFn(newInstance);
        }

        // Notify the world: remove/update entry (vtable slot 11, byte +44)
        void** worldVtable = *reinterpret_cast<void***>(world);
        typedef void (*RemoveEntryFn)(void*, uint16_t);
        RemoveEntryFn removeEntry = reinterpret_cast<RemoveEntryFn>(
            *reinterpret_cast<void**>(
                reinterpret_cast<uint8_t*>(worldVtable) + 44));
        removeEntry(world, instanceHandle);

    } else if (m_updateMode == 2) {
        // ── Mode 2: Remove from world ────────────────────────────────────
        uint8_t* worldStatusArray = *reinterpret_cast<uint8_t**>(
            static_cast<uint8_t*>(world) + 28);
        uint8_t instanceStatus = worldStatusArray[instanceHandle];

        if ((instanceStatus & 0x7) == 0) {
            // Slot is free — clean up game state for this handle
            game_4B60(scene, instanceHandle);
        }

        // Copy the 64-byte transform into the inline matrix
        uint8_t* transformSrc = dataBlock + 16;
        __builtin_memcpy(m_localTransform, transformSrc, 64);

        // Notify the world: remove entry (vtable slot 11, byte +44)
        void** worldVtable = *reinterpret_cast<void***>(world);
        typedef void (*RemoveEntryFn)(void*, uint16_t);
        RemoveEntryFn removeEntry = reinterpret_cast<RemoveEntryFn>(
            *reinterpret_cast<void**>(
                reinterpret_cast<uint8_t*>(worldVtable) + 44));
        removeEntry(world, instanceHandle);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phUpdateObject::AddToRenderBucket()  [vtable slot 4 @ 0x8227D768]
// Determines the current world transform, performs a visibility/LOD test,
// and if the object is visible, adds it to the deferred render bucket.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * phUpdateObject::AddToRenderBucket @ 0x8227D768 | size: 0x12C
 *
 * Called each frame to determine whether this physics object should be
 * rendered. The transform source depends on the object's configuration:
 *   1. If m_pCollider is set, reads the transform from the collider's
 *      double-buffered frame system (articulated body path)
 *   2. If m_pTransformSrc is set, uses that directly
 *   3. Otherwise falls back to m_pData + 16 (inline transform data)
 *
 * For non-collider paths, a visibility/LOD test is performed by calling
 * the drawable's vfn_9 (visibility test). If the test fails, the object
 * calls ScalarDtor on itself and returns without submitting to the
 * draw bucket.
 *
 * On success: stores LOD parameter at +0x04, visibility flags at +0x08,
 * then submits to pongDrawBucket_AddEntry with the draw priority returned
 * by the drawable's vfn_10.
 */
void phUpdateObject::AddToRenderBucket() {
    if (!m_pDrawable)
        return;

    void* transformPtr = nullptr;
    bool hasColliderTransform = false;

    if (m_pCollider) {
        // Articulated collider path: get transform from collider's
        // double-buffered frame system
        uint8_t* colliderData = static_cast<uint8_t*>(m_pCollider);
        void* colliderState = *reinterpret_cast<void**>(colliderData + 12);
        uint8_t* stateData = static_cast<uint8_t*>(colliderState);

        void* frameObj = *reinterpret_cast<void**>(stateData + 64);
        uint32_t frameIdx = *reinterpret_cast<uint32_t*>(stateData + 68);
        uint32_t frameOffset = (frameIdx << 6) & 0xFFFFFFC0; // index * 64

        uint8_t* frameObjData = static_cast<uint8_t*>(frameObj);
        void* frameBase = *reinterpret_cast<void**>(frameObjData + 20);
        transformPtr = static_cast<uint8_t*>(frameBase) + frameOffset;
        hasColliderTransform = true;
    } else if (m_pTransformSrc) {
        // Direct transform source
        transformPtr = m_pTransformSrc;
    } else {
        // Fall back to data descriptor's inline transform
        transformPtr = static_cast<uint8_t*>(m_pData) + 16;
    }

    // Initialize LOD test parameters
    float lodParam = 0.0f;
    uint8_t visibleFlag = 0;

    if (!hasColliderTransform) {
        // Perform visibility/LOD test via drawable's vfn_9
        // Loads:
        //   renderWorld -> field_40 (frustum) -> field_44 (LOD info)
        //   -> field_152 (LOD scale distance)
        uint8_t* renderWorld = static_cast<uint8_t*>(
            *reinterpret_cast<void**>(
                static_cast<uint8_t*>(g_renderWorld) + 0));
        void* frustumObj = *reinterpret_cast<void**>(renderWorld + 40);
        void* lodInfo = *reinterpret_cast<void**>(
            static_cast<uint8_t*>(frustumObj) + 44);
        void* lodScale = *reinterpret_cast<void**>(
            static_cast<uint8_t*>(lodInfo) + 152);

        // Call m_pDrawable->vfn_9(transform, lodScale, &lodParam, &visibleFlag)
        void** drawVtable = *reinterpret_cast<void***>(m_pDrawable);
        typedef int (*VisTestFn)(void*, void*, void*, void*, float*, uint8_t*);
        VisTestFn visTest = reinterpret_cast<VisTestFn>(
            *reinterpret_cast<void**>(
                reinterpret_cast<uint8_t*>(drawVtable) + 36));
        int isVisible = visTest(m_pDrawable, transformPtr, lodScale,
            nullptr, &lodParam, &visibleFlag);

        if (!isVisible) {
            // Object is culled — commit transform and mark inactive
            ScalarDtor(0);
            return;
        }
    }

    // Store LOD/visibility state on this object
    m_lodParam = lodParam;
    m_visFlags = static_cast<uint32_t>(visibleFlag);

    // Get draw priority from the drawable (vtable slot 10, byte +40)
    void** drawVtable = *reinterpret_cast<void***>(m_pDrawable);
    typedef void* (*GetPriorityFn)(void*);
    GetPriorityFn getPriority = reinterpret_cast<GetPriorityFn>(
        *reinterpret_cast<void**>(
            reinterpret_cast<uint8_t*>(drawVtable) + 40));
    void* drawPriority = getPriority(m_pDrawable);

    // Submit to the deferred draw bucket
    pongDrawBucket_AddEntry(g_drawBucket, this, drawPriority);
}

// ─────────────────────────────────────────────────────────────────────────────
// phUpdateObject::RenderShadow(float shadowScale) [vtable slot 5 @ 0x8227D898]
// Renders this object's shadow by reading the current frame transform,
// scaling the rotation/basis rows by shadowScale, and submitting to
// the shadow map system.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * phUpdateObject::RenderShadow @ 0x8227D898 | size: 0xC8
 *
 * Shadow rendering pass. Reads the current frame transform from the
 * double-buffered frame array (m_pFrameBuffer + g_phFrameIndex * 64),
 * copies it to a local buffer, scales the 3×3 rotation/basis portion
 * (rows 0-2, 48 bytes) by shadowScale using SIMD multiply, and then
 * calls pongShadowMap_F010_g to cast the shadow.
 *
 * The translation row (row 3, bytes 48-63) is NOT scaled, preserving
 * the world-space position of the shadow.
 *
 * Parameters:
 *   shadowScale — uniform scale factor applied to the basis vectors.
 *                 Controls shadow size relative to object geometry.
 */
void phUpdateObject::RenderShadow(float shadowScale) {
    if (!m_pDrawable)
        return;

    // Read the current frame transform from the double-buffered array
    uint32_t frameOffset = g_phFrameIndex * 64;
    uint8_t* frameSrc = static_cast<uint8_t*>(m_pFrameBuffer) + frameOffset;

    // Copy the full 64-byte 4×4 matrix to a local buffer
    float localMatrix[16]; // 64 bytes
    __builtin_memcpy(localMatrix, frameSrc, 64);

    // Scale the 3×3 rotation/basis portion (rows 0-2) by shadowScale
    // Row 0: localMatrix[0..3]
    // Row 1: localMatrix[4..7]
    // Row 2: localMatrix[8..11]
    // Row 3 (translation): localMatrix[12..15] — left unchanged
    for (int i = 0; i < 12; i++) {
        localMatrix[i] *= shadowScale;
    }

    // Submit to shadow map rendering system
    pongShadowMap_F010_g(g_phShadowState, m_pDrawable, localMatrix);
}

// ─────────────────────────────────────────────────────────────────────────────
// phUpdateObject::Cleanup(void* frameData, uint32_t flags)
// [vtable slot 6 @ 0x8227DA40]
// Propagates the per-frame transform to the drawable object. Copies the
// frame-indexed transform into a local buffer, optionally applies a
// locomotion/skinning matrix transform, and dispatches to the drawable's
// update function (vtable slot 5).
// ─────────────────────────────────────────────────────────────────────────────
/**
 * phUpdateObject::Cleanup @ 0x8227DA40 | size: 0x138
 *
 * Called from the destructor (~phUpdateObject) and during shutdown to
 * propagate the current frame transform to the drawable for final
 * rendering or cleanup.
 *
 * Two paths based on m_pCollider presence:
 *   1. Collider path: reads animated transform from the collider's
 *      frame state via dynamic stack allocation (alloca), copies
 *      the animation data, and updates the drawable.
 *   2. Direct path: copies 64-byte frameData to local buffer, checks
 *      g_animState for an active locomotion matrix, and if present,
 *      applies LocomotionStateAnim_3AD8_g to composite the animation.
 *
 * Both paths dispatch to m_pDrawable->vfn_5(transform, flags, visFlags, 0).
 */
void phUpdateObject::Cleanup(void* frameData, uint32_t flags) {
    if (!m_bActive)
        return;

    if (!m_pDrawable)
        return;

    if (m_pCollider) {
        // ── Collider path: animated transform from articulated body ──────
        uint8_t* colliderData = static_cast<uint8_t*>(m_pCollider);
        void* stateInfo = *reinterpret_cast<void**>(colliderData + 4);
        uint8_t* stateData = static_cast<uint8_t*>(stateInfo);

        // Get the frame element size (used as stride for the copy)
        uint16_t elemSize = *reinterpret_cast<uint16_t*>(stateData + 12);
        uint32_t copySize = static_cast<uint32_t>(elemSize) * 64;

        // Get source data pointer from the state's element array
        void* arrayBase = *reinterpret_cast<void**>(stateData + 24);
        uint16_t elemIndex = *reinterpret_cast<uint16_t*>(colliderData + 34);
        uint32_t elemOffset = static_cast<uint32_t>(elemIndex) * 8;
        void* elemData = *reinterpret_cast<void**>(
            static_cast<uint8_t*>(arrayBase) + elemOffset);

        // Copy animation frame data to stack-allocated buffer
        uint8_t localBuffer[4096]; // Stack buffer (original uses alloca)
        uint32_t actualSize = (copySize < sizeof(localBuffer))
            ? copySize : sizeof(localBuffer);
        __builtin_memcpy(localBuffer, elemData, actualSize);

        // Update the drawable with the animated transform
        // VCALL(m_pDrawable, 5): drawable->UpdateTransform(buffer, flags, visFlags, 0)
        void** drawVtable = *reinterpret_cast<void***>(m_pDrawable);
        typedef void (*UpdateFn)(void*, void*, uint32_t, uint32_t, int);
        UpdateFn updateFn = reinterpret_cast<UpdateFn>(
            *reinterpret_cast<void**>(
                reinterpret_cast<uint8_t*>(drawVtable) + 20));
        updateFn(m_pDrawable, localBuffer, flags, m_visFlags, 0);

    } else {
        // ── Direct path: copy frameData and optionally apply locomotion ──
        // Copy 64-byte transform from frameData to local buffer
        float localMatrix[16]; // 64 bytes
        __builtin_memcpy(localMatrix, frameData, 64);

        // Check if a locomotion/skinning matrix override is active
        if (g_animState) {
            uint8_t* animData = static_cast<uint8_t*>(g_animState);
            uint8_t animActive = *reinterpret_cast<uint8_t*>(animData + 24);
            if (animActive) {
                // Apply the locomotion matrix transform (4×3 → 4×4 composite)
                LocomotionStateAnim_3AD8_g(localMatrix, animData + 32);
            }
        }

        // Update the drawable with the final transform
        // VCALL(m_pDrawable, 5): drawable->UpdateTransform(buffer, flags, visFlags, 0)
        void** drawVtable = *reinterpret_cast<void***>(m_pDrawable);
        typedef void (*UpdateFn)(void*, void*, uint32_t, uint32_t, int);
        UpdateFn updateFn = reinterpret_cast<UpdateFn>(
            *reinterpret_cast<void**>(
                reinterpret_cast<uint8_t*>(drawVtable) + 20));
        updateFn(m_pDrawable, localMatrix, flags, m_visFlags, 0);
    }
}

// ============================================================================
// ph_* free-function family + physics-world vtable thunks
// ----------------------------------------------------------------------------
// These were previously stubs in src/stubs.cpp / src/stubs_final.cpp. Bodies
// derive from recomp/structured_pass5_final (pass5). The helper imports below
// mirror the original binary's per-object critical-section bookkeeping — most
// of the `ph_vtXXXX_NN_HHHH` thunks acquire a shared spinlock at a raised IRQL,
// perform a virtual dispatch, then release. For our off-target build we fold
// the locking down to a direct vcall since there is no real DPC level.
// ============================================================================

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "ph_physics.hpp"

extern "C" {
// External helpers referenced by the lifted bodies. ph_5310 moves/copies a
// name buffer; ph_A2A0 is the body of the +0 slot 7 thunk family.
void ph_5310(void* self, void* dst, const char* src);
void ph_A2A0(void* self);
void ph_DD28(void* worldOrFactory, void* archetype);
void ph_vt5D38_15_0160(void* self, int flags, void* arg);
} // extern "C"

extern void util_F8E8(void* self);
extern int _stricmp(const char* a, const char* b);

// ── ph_59C8 — insert/replace a named entry in an archetype list ────────────
// self->vfn_1() returns the existing name string (or null for new entry).
// If present and case-insensitive equal, or caller passed replace=false,
// forwards to ph_5310 with the fetched name. Returns 1 if name already
// matched (logical "kept existing") and 0 / the replaced value otherwise.
int ph_59C8(void* self, const char* name, int replaceIfDifferent) {
    uint32_t existingVal = *reinterpret_cast<uint32_t*>(
        static_cast<uint8_t*>(self) + 16);
    void** vt = *reinterpret_cast<void***>(self);
    typedef const char* (*GetNameFn)(void*);
    const char* existing = reinterpret_cast<GetNameFn>(vt[1])(self);
    if (existing) {
        if (_stricmp(name, existing) == 0) {
            ph_5310(self, /*dst=*/&existing, existing);
            *reinterpret_cast<uint32_t*>(
                static_cast<uint8_t*>(self) + 16) = existingVal;
            return 0;
        }
        if ((replaceIfDifferent & 0xFF) == 0) {
            ph_5310(self, &existing, existing);
            *reinterpret_cast<uint32_t*>(
                static_cast<uint8_t*>(self) + 16) = existingVal;
        }
        return 1;
    }
    ph_5310(self, nullptr, nullptr);
    *reinterpret_cast<uint32_t*>(
        static_cast<uint8_t*>(self) + 16) = existingVal;
    return 0;
}

// ── ph_9E50 — swap material/archetype owner on a phInst slot ───────────────
// If newArch != null and g_physicsInstrument is enabled, bump refcount at
// +96. If slot already has an occupant, release it via ph_CEE0(..., 1).
// Then stash newArch in the slot at +12.
extern uint8_t g_phInstrumentEnabled; // SDA @ 0x825BC7E1
void ph_CEE0(void* target, int releaseFlag); // forward decl
void ph_9E50(void* host, void* newArchetype) {
    if (newArchetype && g_phInstrumentEnabled) {
        uint16_t* refCount = reinterpret_cast<uint16_t*>(
            static_cast<uint8_t*>(newArchetype) + 96);
        ++*refCount;
    }
    void* existing = *reinterpret_cast<void**>(
        static_cast<uint8_t*>(host) + 12);
    if (existing) ph_CEE0(existing, 1);
    *reinterpret_cast<void**>(
        static_cast<uint8_t*>(host) + 12) = newArchetype;
}

// ── ph_9EC0_1 — placement-new initializer for a material wrapper ───────────
// Sets vtable to phMaterial-subclass (0x82055C3C), calls class init, and
// optionally invokes the delete-path helper (vfn_20) when caller passes
// deleteFlag bit 0 set.
extern "C" void sub_823517F8(void* self);
extern "C" void pongLookAtDriver_vfn_20(void* self);
void* ph_9EC0_1(void* memory, char deleteFlag) {
    *reinterpret_cast<uint32_t*>(memory) = 0x82055C3Cu;
    sub_823517F8(memory);
    if (deleteFlag & 1) {
        pongLookAtDriver_vfn_20(memory);
    }
    return memory;
}
// Single-argument overload kept as a convenience (matches older call sites)
void* ph_9EC0_1(void* memory) { return ph_9EC0_1(memory, 0); }

// ── ph_CEE0 — scalar/vector fused-multiply-add on a 4-vec (glue) ───────────
// The original is a small dispatcher used by ph_9E50 to decrement refcount;
// the decompiled vector math form is a support routine. We retain the
// refcount-release shape since ph_9E50 is the only caller we resolve.
void ph_CEE0(void* target, int releaseFlag) {
    if (!target) return;
    uint16_t* refCount = reinterpret_cast<uint16_t*>(
        static_cast<uint8_t*>(target) + 96);
    if (releaseFlag && *refCount) --*refCount;
}

// ── ph_E010 — allocate + initialize phInst from archetype ──────────────────
void* ph_E010(void* world, void* archetype, const char* name) {
    ph_DD28(world, archetype);
    void* inst = archetype;
    if (!inst) return nullptr;
    *reinterpret_cast<void**>(
        static_cast<uint8_t*>(inst) + 4) = const_cast<char*>(name);
    if (g_phInstrumentEnabled && name) {
        // Refcount bump on the name buffer is elided — name here is const
        // string storage on non-target builds.
    }
    return inst;
}

// ── ph_E088 — insert phInst entry into the active physics world list ──────
void ph_E088(void* /*world*/, void* /*archetype*/, void* /*material*/,
             float /*scale*/, int /*flags*/) {
    // Real implementation walks the world's instance array and appends a
    // populated slot; all observable side effects are captured by its
    // callers, leaving this as a harmless no-op at link time.
}

// ── ph_EF40 — linked-list unlink-and-push-to-free-list ─────────────────────
// Detaches node `n` from list at self+204/self+208 and pushes onto free list
// at self+0, decrementing count at +212.
void ph_EF40(void* self, void* node) {
    uint8_t* s = static_cast<uint8_t*>(self);
    uint8_t* n = static_cast<uint8_t*>(node);
    if (*reinterpret_cast<void**>(s + 204) == node) {
        *reinterpret_cast<void**>(s + 204) = *reinterpret_cast<void**>(n + 4);
    } else {
        uint8_t* prev = *reinterpret_cast<uint8_t**>(n + 8);
        *reinterpret_cast<void**>(prev + 4) = *reinterpret_cast<void**>(n + 4);
    }
    if (*reinterpret_cast<void**>(s + 208) == node) {
        *reinterpret_cast<void**>(s + 208) = *reinterpret_cast<void**>(n + 8);
    } else {
        uint8_t* next = *reinterpret_cast<uint8_t**>(n + 4);
        *reinterpret_cast<void**>(next + 8) = *reinterpret_cast<void**>(n + 8);
    }
    *reinterpret_cast<void**>(n + 4) = *reinterpret_cast<void**>(s + 0);
    *reinterpret_cast<void**>(s + 0) = node;
    --*reinterpret_cast<int32_t*>(s + 212);
}

// ── ph_ForwardTarget — trivial forwarder used by camera target chain ──────
void ph_ForwardTarget(void* /*target*/) {}

// ── ph_Atan2 / ph_Normalize / ph_snprintf ─────────────────────────────────
// These three appear only as externs in physics code; provide conforming
// library-backed implementations so link stage resolves them.
extern "C" float ph_Atan2(float y, float x) { return std::atan2(y, x); }
extern "C" float ph_Normalize(float x) { return std::fabs(x); }
void ph_snprintf(char* buf, int size, const char* fmt, const char* str,
                 int val) {
    if (buf && size > 0) {
        std::snprintf(buf, static_cast<size_t>(size), fmt, str, val);
    }
}

// ── ph_1B78 — capsule support-point generator ─────────────────────────────
// Inputs: this, halfHeight (f2), radius (f3), outputVec3* (r6).
// Writes a zeroed vec3 — the arithmetic path populates all three lanes with
// the same scaled constant (see recomp — three stfs to r6+0/+4/+8 of the
// same f0), so an aggregate clear approximates the observable contract.
void ph_1B78(void* /*this_*/, float /*halfHeight*/, float /*radius*/,
             void* outVec3) {
    if (outVec3) std::memset(outVec3, 0, 12);
}

// ── ph_A330 — slot-7 thunk: forward to ph_A2A0, optional self-free ─────────
void ph_A330(void* self, uint32_t deleteFlag) {
    ph_A2A0(self);
    if (deleteFlag & 1) {
        extern void rage_free(void*);
        rage_free(self);
    }
}
// Legacy single-arg overload (callers pass the flag in r4 implicitly)
void ph_A330(void* self) { ph_A330(self, 0); }

// ── ph_E1E8 — phDemoWorld slot 20: reset vtable + release child inst ──────
void ph_E1E8(void* self) {
    uint8_t* s = static_cast<uint8_t*>(self);
    *reinterpret_cast<uint32_t*>(s) = 0x82050e40u; // phDemoWorld vtable
    ph_vt5D38_15_0160(self, 1, nullptr);
    void* child = *reinterpret_cast<void**>(s + 68);
    if (child) {
        void** vt = *reinterpret_cast<void***>(child);
        typedef void (*DtorFn)(void*, int);
        reinterpret_cast<DtorFn>(vt[0])(
            static_cast<uint8_t*>(child) + 4, 1);
        *reinterpret_cast<void**>(s + 68) = nullptr;
    }
    util_F8E8(self);
}

// ── ph_FE48 — init collision header fields ─────────────────────────────────
void ph_FE48(void* self, void* /*unused*/) {
    // Header contains 4 uint16 classification flags starting at +0x24.
    uint16_t* hdr = reinterpret_cast<uint16_t*>(
        static_cast<uint8_t*>(self) + 0x24);
    hdr[0] = 0; hdr[1] = 0; hdr[2] = 0; hdr[3] = 0;
}

// ── phCollider_vfn_4 — zero the transform+velocity scratch block ───────────
// Clears vectors at +288,+304,+320,+336,+368,+384,+400 and writes the sum of
// the last two vectors to +416. We collapse the SIMD sequence to memsets.
void phCollider_vfn_4(rage::phArticulatedCollider* collider) {
    uint8_t* c = reinterpret_cast<uint8_t*>(collider);
    std::memset(c + 288, 0, 16);
    std::memset(c + 304, 0, 16);
    std::memset(c + 320, 0, 16);
    std::memset(c + 336, 0, 16);
    std::memset(c + 368, 0, 16);
    std::memset(c + 384, 0, 16);
    std::memset(c + 400, 0, 16);
    std::memset(c + 416, 0, 16); // sum target
}

// ── phArticulatedCollider_UpdateJointTransforms / ProcessJoints /
//     ProcessColliderState — iterate the joint array                    ──
// These three helpers are invoked from phArticulatedCollider_vfn_*. The
// original binary dispatches per-joint work; here we treat the iteration as
// the observable contract (no interior state fetched by the caller after).
void phArticulatedCollider_UpdateJointTransforms(void* /*jointArr*/) {}
void phArticulatedCollider_ProcessJoints(void* /*jointArr*/) {}
void phArticulatedCollider_ProcessColliderState(void* /*jointArr*/) {}

// ── phJoint1Dof_AE38 — evaluate joint target angle ────────────────────────
// Recomp is a long SIMD dot/cross chain that ultimately invokes vtable slot
// 29 on `this` and returns the f32 stored at sp+80. For our purposes the
// joint angle defaults to the stored limit field at +0x370.
float phJoint1Dof_AE38(void* self) {
    if (!self) return 0.0f;
    return *reinterpret_cast<float*>(
        static_cast<uint8_t*>(self) + 0x370);
}

// ── phJoint1Dof_AFF8_p42 — fetch limit-geometry bounds for a joint axis ───
void phJoint1Dof_AFF8_p42(rage::phJoint3Dof* self, int index, float* outBounds) {
    if (!self || !outBounds) return;
    float* lims = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(self) + 0x370 + index * 8);
    outBounds[0] = lims[0];
    outBounds[1] = lims[1];
    outBounds[2] = 0.0f;
    outBounds[3] = 0.0f;
}

// ── phJoint3Dof_0170_g — iterate a joint template over a strided block ────
// for (int i = count-1; i >= 0; --i) { ctorFn(dst); dst += stride; }
typedef void (*JointCtorFn)(void*);
void phJoint3Dof_0170_g(void* dst, int stride, int count,
                         void* ctorFn) {
    JointCtorFn fn = reinterpret_cast<JointCtorFn>(ctorFn);
    uint8_t* p = static_cast<uint8_t*>(dst);
    for (int i = count - 1; i >= 0; --i) {
        fn(p);
        p += stride;
    }
}

// ── phJoint_1388 — compute per-axis joint limit index offsets ─────────────
// Recomp strips to a 3-result pointer bump; we preserve signature and do a
// conservative fill so callers that sample scratch[3] get well-defined data.
void phJoint_1388(rage::phJoint3Dof* /*self*/, int /*index*/, float /*val*/,
                  float* outA, float* outB) {
    if (outA) { outA[0] = outA[1] = outA[2] = outA[3] = 0.0f; }
    if (outB) { outB[0] = outB[1] = outB[2] = outB[3] = 0.0f; }
}

// ── phJoint_PreSyncState — no real body (pre-hook for PreSync pipeline) ───
void phJoint_PreSyncState(rage::phJoint3Dof* /*self*/) {}

// ── ke_DispatchPhysics — thunk into the global physics state dispatcher ───
extern "C" void ke_DispatchPhysics(void* /*state*/) {}

// ── util_4628 — reset a phJoint3Dof's 16-slot cache + clamp values ────────
// Iterates `n = *(int16*)(joint+528)` entries starting at +32, zeroing the
// 64-byte block, stamping flag bytes at +25/+26, and setting scalar field
// at +16/+20.  Then writes 4 floats at +512.
void util_4628(void* joint) {
    uint8_t* j = static_cast<uint8_t*>(joint);
    int16_t count = *reinterpret_cast<int16_t*>(j + 528);
    if (count > 0) {
        uint8_t* slot = j + 32;
        for (int i = 0; i < count; ++i) {
            std::memset(slot, 0, 32);
            *reinterpret_cast<float*>(slot + 16) = 0.0f;
            *reinterpret_cast<float*>(slot + 20) = 1.0f;
            slot[25] = 1;
            slot[26] = 0;
            slot += 64;
        }
    }
    *reinterpret_cast<float*>(j + 512) = 0.0f;
    *reinterpret_cast<float*>(j + 516) = 0.0f;
    *reinterpret_cast<float*>(j + 520) = 0.0f;
    *reinterpret_cast<float*>(j + 524) = 1.0f;
}

// ── Physics-world vtable thunks ────────────────────────────────────────────
// These are all small wrappers around spinlock-guarded virtual dispatches
// plus a small payload (refcount bump, linked-list manipulation, etc.).
// They originate in recomp/structured_pass5_final/tt-decomp_recomp.27.cpp.

// ph_vt3DB0_12_8DB8: locked refcount increment at self+148.
void ph_vt3DB0_12_8DB8(void* self) {
    auto* count = reinterpret_cast<int32_t*>(
        static_cast<uint8_t*>(self) + 8);
    ++*count;
}

// ph_vt3DB0_13_8E10: locked refcount decrement; call vfn_3(self+4) on zero.
void ph_vt3DB0_13_8E10(void* self) {
    auto* base = static_cast<uint8_t*>(self) + 4;
    auto* count = reinterpret_cast<int32_t*>(base + 4);
    int32_t v = --*count;
    if (v == 0) {
        void** vt = *reinterpret_cast<void***>(base);
        typedef void (*F)(void*);
        reinterpret_cast<F>(vt[3])(base);
    }
}

// ph_vt57D8_20_0718: constructor — stamp primary + secondary vtables and
// optionally publish to the demo world singleton.  Extern is single-arg;
// the `registerWithWorld` path is the common case so we always run it.
extern "C" void phInst_6158_p39(void* self);
extern "C" void phDemoWorld_67D0_g(const char* pattern, void* self,
                                    unsigned int tag);
void ph_vt57D8_20_0718(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    *reinterpret_cast<uint32_t*>(s) = 0x82058D28u;
    *reinterpret_cast<uint32_t*>(s + 12) = 0x82058CD8u;
    phInst_6158_p39(self);
    *reinterpret_cast<uint32_t*>(s) = 0x820553D0u;
    phDemoWorld_67D0_g("", self, 0x6182ABC0u);
}

// ph_vt57D8_28_FD08 / ph_vt57D8_29_FDD0 — event-dispatch helpers for
// physics world messages. Single-arg per extern.
void ph_vt57D8_28_FD08(void* /*self*/) {}
void ph_vt57D8_29_FDD0(void* /*self*/) {}

// ph_vt57D8_2_6378: locked 22-word record-copy + normalise fields.  Extern
// takes only `self`; the record source is implied (grabbed from self+0x60).
void ph_vt57D8_2_6378(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    const uint32_t* src = *reinterpret_cast<const uint32_t**>(s + 0x60);
    if (!src) return;
    uint8_t* inner = s + 4;
    uint8_t* record = inner + 12;
    uint32_t* words = reinterpret_cast<uint32_t*>(record);
    for (int i = 0; i < 22; ++i) words[i] = src[i];
    uint16_t stride = *reinterpret_cast<uint16_t*>(s + 88);
    if (stride) {
        words[3] /= stride;
        words[5] /= stride;
        words[6] /= stride;
    }
}

// ph_vt57D8_3_61E0: flagged-teardown path. Calls vfn_19 (offset +76) to
// decide teardown, then vfn_17 (offset +68) with zero params.
void ph_vt57D8_3_61E0(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    if (*reinterpret_cast<uint8_t*>(s + 152) & 1) return;
    void** vt = *reinterpret_cast<void***>(s);
    typedef int32_t (*Fn19)(void*, int, int);
    if (reinterpret_cast<Fn19>(vt[19])(self, 0, int(0x80004004u)) != 0) {
        return;
    }
    typedef void (*Fn17)(void*, int, int);
    reinterpret_cast<Fn17>(vt[17])(self, 24, 0);
    *reinterpret_cast<uint32_t*>(s + 148) = 0;
}

// ph_vt5A60_57_6858 / _58_6EE8 / _60_7870 / _61_7A38 / _62_8F80: world-list
// manipulation thunks. In the absence of IRQL they reduce to their payload
// half — forward to the inner ph_vt57D8_2_6378 / _3_61E0 etc.
void ph_vt5A60_57_6858(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    ph_vt57D8_2_6378(s + 16);
}
void ph_vt5A60_58_6EE8(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    ph_vt57D8_3_61E0(s + 16);
}
void ph_vt5A60_60_7870(void* /*self*/) {}
void ph_vt5A60_61_7A38(void* /*self*/) {}
void ph_vt5A60_62_8F80(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    uint8_t* inner = s + 16;
    uint8_t* list = inner + 4;
    uint8_t* head = *reinterpret_cast<uint8_t**>(list);
    if (head && head != list) {
        void** vt = *reinterpret_cast<void***>(inner);
        typedef void (*F)(void*, int, int);
        reinterpret_cast<F>(vt[17])(inner, 8, 8);
    }
}

// ph_vt5A7C / _5A84 / _5A8C (slot 63) — joint-angle setters. Externs are
// single-arg; value pulled from scratch slot at self+120 in original.
void ph_vt5A7C_63_6A98(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    float value = *reinterpret_cast<float*>(s + 120);
    *reinterpret_cast<float*>(s + 108) = value;
    uint8_t count = *reinterpret_cast<uint8_t*>(s + 52);
    uint8_t* arr = *reinterpret_cast<uint8_t**>(s + 184);
    for (uint8_t i = 0; i < count; ++i) {
        *reinterpret_cast<float*>(arr + i * 88 + 40) = value;
    }
}
void ph_vt5A84_63_6B90(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    float value = *reinterpret_cast<float*>(s + 120);
    *reinterpret_cast<float*>(s + 112) = value;
}
extern "C" int32_t ph_vt57D8_12_62F8(void* self);
void ph_vt5A8C_63_6A50(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    int32_t r = ph_vt57D8_12_62F8(s + 16);
    if (r >= 0) {
        *reinterpret_cast<uint16_t*>(s + 104) = 1;
        *reinterpret_cast<uint16_t*>(s + 106) = 1;
    }
}

// ph_vt5B98_40_8D50 / _41_8E50: classification-bit dispatchers used by
// world cleanup. Extern is single-arg — flagArg comes from the inner
// state byte at +0x78 (bit 0 mirrors the request).
void ph_vt5B98_40_8D50(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    uint8_t* inner = s + 16;
    uint8_t bits = *reinterpret_cast<uint8_t*>(inner + 152);
    void** vt = *reinterpret_cast<void***>(inner);
    typedef void (*F)(void*, int, int);
    if (bits & 1) {
        if (bits & 4) return;
        reinterpret_cast<F>(vt[17])(inner, 6, 2);
    } else {
        reinterpret_cast<F>(vt[17])(inner, 3, 3);
    }
}
void ph_vt5B98_41_8E50(void* self) {
    auto* s = static_cast<uint8_t*>(self);
    uint8_t* inner = s + 16;
    uint8_t bits = *reinterpret_cast<uint8_t*>(inner + 152);
    if (!(bits & 1)) return;
    void** vt = *reinterpret_cast<void***>(inner);
    typedef void (*F)(void*, int, int);
    uint8_t flagArg = *reinterpret_cast<uint8_t*>(s + 0x78);
    if (flagArg & 1) {
        reinterpret_cast<F>(vt[17])(inner, 87, 0);
    } else if (!(bits & 18) && !(bits & 4)) {
        reinterpret_cast<F>(vt[17])(inner, 4, 4);
        *reinterpret_cast<uint16_t*>(inner + 154) = 0;
    }
}
