/**
 * ph_update_object.cpp — phUpdateObject destructor implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.12.cpp
 * Functions: phUpdateObject_vfn_0 @ 0x8227DA08 (size: 0x30)
 *            phUpdateObject_vfn_1 @ 0x8227D960 (size: 0xA8) — ScalarDtor
 *
 * phUpdateObject is the base class for physics objects that receive
 * per-frame transform updates via a double-buffered frame array.
 * The frame array is indexed by g_phFrameIndex, a global counter that
 * alternates each tick so the CPU and GPU can access different frames
 * simultaneously without stalls.
 */

#include "ph_physics.hpp"

// Global frame-index counter used to index the double-buffered frame array.
// Located at 0x825C4898 (.data, 16 bytes — first word is the active index).
// Accessed in assembly as: lis r11,-32164 ; lwz rN,18584(r11)
extern uint32_t g_phFrameIndex;  // @ 0x825C4898

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
        // Caller requests heap free of this object
        // rage_free is the canonical RAGE allocator free (@ 0x820C00C0)
        extern void rage_free(void* ptr);
        rage_free(this);
    }
}
