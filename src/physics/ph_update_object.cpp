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
 * phUpdateObject::ScalarDtor(int flags) — scalar destructor
 * @ 0x8227D960 | size: 0xA8 (vtable slot 1)
 *
 * Copies the current transform from the per-frame source (m_pData or
 * the inline transform at +0x14) into the double-buffered destination
 * array using 4x AltiVec vector loads/stores (4 × 16 bytes = 64 bytes).
 * After the copy, if (flags & 1), frees this object via rage_free.
 *
 * The copy loop:
 *   src = (m_pData != NULL) ? m_pData + 16 : &this->inlineTransform + 16
 *   dst = m_pFrameBuffer + (g_phFrameIndex * 64)
 *   memcpy(dst, src, 64)  — using 4x lvx/stvx
 *
 * Note: g_phFrameIndex here is accessed via lis r10,-32160 + 26076 = 0x826065DC
 *       which is a separate SDA global (the copy path uses a different base).
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

    // Copy 64-byte transform block (4x 16-byte AltiVec stores in original)
    // TODO: verify exact source selection logic from scaffold — may use
    //       inline matrix at +0x14 when m_pFrameBuffer is NULL
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
