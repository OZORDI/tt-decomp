/**
 * pong_audio.cpp — Audio effect classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Audio DSP effect wrappers for Xbox 360 XAudio2 system.
 * These classes wrap Xbox 360 audio effects with reference counting.
 *
 * Common pattern:
 *   +0x00: vtable pointer
 *   +0x04: reference count (int32_t)
 *   +0x1C: effect data pointer 1
 *   +0x20: effect data pointer 2
 */

#include "pong_audio.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// CPeakMeterEffect  [vtable @ 0x8203B260]
//
// Peak meter audio effect for monitoring audio levels.
// Used for VU meters and audio visualization.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * CPeakMeterEffect::~CPeakMeterEffect() @ 0x821ADD88 | size: 0x14
 *
 * Destructor - increments reference count.
 * This is the non-deleting destructor (vtable slot 0).
 */
CPeakMeterEffect::~CPeakMeterEffect() {
    // Increment reference count at +4
    // Pattern: lwz r10, 4(r11); addi r3, r10, 1; stw r3, 4(r11)
    int32_t* refCount = (int32_t*)((char*)this + 4);
    (*refCount)++;
}

/**
 * CPeakMeterEffect::ScalarDtor() @ 0x821ADDA0 | size: 0x28
 *
 * Scalar deleting destructor - decrements reference count.
 * When refcount reaches 0, clears effect data pointers at +28 and +32.
 */
void CPeakMeterEffect::ScalarDtor(int flags) {
    // Decrement reference count at +4
    int32_t* refCount = (int32_t*)((char*)this + 4);
    (*refCount)--;
    
    // If refcount reaches 0, clear effect data
    if (*refCount == 0) {
        // Clear effect data pointers at +28 (+0x1C) and +32 (+0x20)
        *(void**)((char*)this + 28) = nullptr;
        *(void**)((char*)this + 32) = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CShelvingFilterEffect  [vtable @ 0x8203B288]
//
// Shelving filter audio effect (low-shelf/high-shelf EQ).
// Used for bass/treble adjustment.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * CShelvingFilterEffect::~CShelvingFilterEffect() @ 0x821B26A8 | size: 0x14
 *
 * Destructor - increments reference count.
 */
CShelvingFilterEffect::~CShelvingFilterEffect() {
    int32_t* refCount = (int32_t*)((char*)this + 4);
    (*refCount)++;
}

/**
 * CShelvingFilterEffect::ScalarDtor() @ 0x821AE618 | size: 0x14
 *
 * Scalar deleting destructor - decrements reference count.
 */
void CShelvingFilterEffect::ScalarDtor(int flags) {
    int32_t* refCount = (int32_t*)((char*)this + 4);
    (*refCount)--;
    
    if (*refCount == 0) {
        *(void**)((char*)this + 28) = nullptr;
        *(void**)((char*)this + 32) = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CDelayEffect  [vtable @ 0x8203B570]
//
// Delay/echo audio effect.
// Used for reverb and echo effects.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * CDelayEffect::ScalarDtor() @ 0x821B1E30 | size: 0x28
 *
 * Scalar deleting destructor - decrements reference count.
 */
void CDelayEffect::ScalarDtor(int flags) {
    int32_t* refCount = (int32_t*)((char*)this + 4);
    (*refCount)--;
    
    if (*refCount == 0) {
        *(void**)((char*)this + 28) = nullptr;
        *(void**)((char*)this + 32) = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// NOTE: CCompEffect and CBiquadFilterEffect follow the same pattern
// but their specific functions are not yet traced.
// TODO: Implement remaining audio effect methods as needed.
// ─────────────────────────────────────────────────────────────────────────────
