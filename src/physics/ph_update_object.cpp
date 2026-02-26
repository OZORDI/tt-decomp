/**
 * ph_update_object.cpp — phUpdateObject destructor
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.12.cpp:22462
 */

#include "ph_physics.hpp"

// Global frame index used to compute array offset
extern uint32_t g_phFrameIndex;  // @ 0x825C48A8

/**
 * phUpdateObject::~phUpdateObject() @ 0x8227DA08 | size: 0x30
 *
 * Delegating destructor — checks if the object has active data at +20.
 * If so, computes an indexed pointer from the frame-indexed array at +128
 * and tail-calls vtable slot 6 (cleanup method).
 *
 * Pattern:
 *   if (field_0x14 == 0) return;
 *   index = g_phFrameIndex << 6;        // multiply by 64
 *   ptr   = (uint8_t*)field_0x80 + index;
 *   this->vfn_6(ptr, original_arg);     // vtable slot 6
 */
phUpdateObject::~phUpdateObject()
{
    // Early-out if no active data
    if (field_0x0014 == 0)
        return;

    // Compute frame-indexed pointer into array at +0x80
    uint32_t index = g_phFrameIndex << 6;  // * 64
    uint8_t* arrayBase = (uint8_t*)(uintptr_t)field_0x0080;
    void* framePtr = arrayBase + index;

    // Tail-call vtable slot 6 (cleanup)
    typedef void (*Vfn6)(void*, void*, uint32_t);
    void** vt = *(void***)this;
    Vfn6 fn = (Vfn6)vt[6];
    fn(this, framePtr, 0);  // r5 passed through from original caller
}
