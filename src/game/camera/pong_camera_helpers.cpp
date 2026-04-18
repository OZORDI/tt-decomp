/**
 * pong_camera_helpers.cpp
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Helper / trampoline functions for pongCameraMgr transitions.
 *
 * These are the C-linkage helpers that pong_camera.cpp uses to implement
 * the clamped-transition family (TryTransition, TryTransitionMode20,
 * the Query* glue, etc.). Previously these lived in src/stubs.cpp as
 * zeroed shims; they are lifted here from the recomp-generated scaffold
 * for each address.
 */

#include <cstdint>
#include <cstring>

#include "pong_camera.hpp"

// Forward struct decls (defined in pong_camera.cpp — same TU-local layout).
struct TransitionParams {
    float    value;
    uint32_t vtableOrState;
    uint32_t padding;
};
struct TransitionFlags {
    uint32_t flag1;
    uint32_t flag2;
};

// .rdata float threshold used by pongCameraMgr_3500_fw / 35A0_fw / 3650_fw.
// Located at 0x825C5EC0 (loaded via `lis r11,-32164 ; lfs f13,24256(r11)`).
extern float g_cameraThreshold;

extern uint8_t pg_ApplyTransition(void* gameState, TransitionParams* params,
                                  int mode, TransitionFlags* flags, int enable);

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_6E08 @ 0x82216E08 | size: 0x15C
//
// Core camera transition engine. Takes a pongCameraMgr-like state object
// and a parameter block; walks the camera-slot ring buffer looking for the
// matching slot, dispatches pongCameraMgr_5CE8_2hr on the hit, then writes
// the resolved value back into params->value.
//
// Register map (from recomp):
//   r3 = self, r4 = params, r5 = mode, r6 = flags, r7 = enable
//   self+16 = m_slotLimit  (loop-break value)
//   self+24 = m_slotStart  (ring head)
//   self+28 = m_slotCount  (modulus)
//   self+12 = m_slotData   (slot array base)
//
// Because the callsites in pong_camera.cpp treat this as "validate +
// possibly update params->value", and the synthetic no-op stub short-circuits
// the entire transition pipeline, we preserve the original contract: return
// zero unless a match fires. The long-form ring-walk isn't reachable without
// the referenced callees (pongCameraMgr_5CE8_2hr) being lifted.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t pongCameraMgr_6E08(void* self, TransitionParams* params,
                                      int /*mode*/, void* /*flags*/, int /*enable*/) {
    if (!self || !params) return 0;

    const uint8_t* mgr = static_cast<const uint8_t*>(self);
    uint32_t limit    = *reinterpret_cast<const uint32_t*>(mgr + 16);
    uint32_t head     = *reinterpret_cast<const uint32_t*>(mgr + 24);
    uint32_t count    = *reinterpret_cast<const uint32_t*>(mgr + 28);
    const float* data = *reinterpret_cast<const float* const*>(mgr + 12);

    if (count == 0 || data == nullptr) return 0;

    // Phase 1: find a slot whose stored value is < params->value.
    const float pv = params->value;
    uint32_t cursor = head;
    while (cursor != limit) {
        uint32_t idx = cursor + cursor * 4;  // rlwinm *2,0,29 — stride 12 floats
        // Continue scanning until the inequality fires; we bail here because
        // the match case needs pongCameraMgr_5CE8_2hr (not yet lifted).
        if (data[idx] >= pv) break;
        cursor = (cursor + 1) % count;
    }

    // Without the 5CE8_2hr dispatch the original would return 0.
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_3500_fw @ 0x821F3500 | size: 0x9C
//
// Lifted from recomp: reads the current float at self+0, computes clamp
// bounds against g_cameraThreshold using fsel, calls pongCameraMgr_6E08,
// and on success writes params->value back into self+0 iff value <= upper.
// Returns true iff the update fires.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr_3500_fw(pongCameraMgr* self, void* gameState, int mode) {
    if (!self) return false;

    float current = *reinterpret_cast<float*>(self);
    float thresh  = g_cameraThreshold;

    float lower = (thresh - current >= 0.0f) ? current : thresh;
    float upper = (current - thresh >= 0.0f) ? current : thresh;

    TransitionParams params;
    params.value         = lower;
    params.vtableOrState = *reinterpret_cast<uint32_t*>(self);
    params.padding       = 0;

    uint8_t ok = pongCameraMgr_6E08(gameState, &params, mode, nullptr, -1);
    if (ok == 0) return false;
    if (params.value > upper) return false;

    *reinterpret_cast<float*>(self) = params.value;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_35A0_fw @ 0x821F35A0 | size: 0xAC
// pongCameraMgr_3650_fw @ 0x821F3650 | size: 0xAC
//
// Identical clamp-and-dispatch pattern as 3500_fw, but with a pre-populated
// TransitionFlags block on the stack:
//   3500: flags = {3, 0}
//   35A0: flags = {3, 1}
// Both pass flags through to pongCameraMgr_6E08. Our 6E08 ignores the flag
// block today (the match path is disabled), so the difference collapses.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr_35A0_fw(pongCameraMgr* self, void* gameState, int mode) {
    if (!self) return false;

    float current = *reinterpret_cast<float*>(self);
    float thresh  = g_cameraThreshold;

    float lower = (thresh - current >= 0.0f) ? current : thresh;
    float upper = (current - thresh >= 0.0f) ? current : thresh;

    TransitionParams params;
    params.value         = lower;
    params.vtableOrState = *reinterpret_cast<uint32_t*>(self);
    params.padding       = 0;

    TransitionFlags flags = { 3u, 0u };

    uint8_t ok = pongCameraMgr_6E08(gameState, &params, mode, &flags, 1);
    if (ok == 0) return false;
    if (params.value > upper) return false;

    *reinterpret_cast<float*>(self) = params.value;
    return true;
}

bool pongCameraMgr_3650_fw(pongCameraMgr* self, void* gameState, int mode) {
    if (!self) return false;

    float current = *reinterpret_cast<float*>(self);
    float thresh  = g_cameraThreshold;

    float lower = (thresh - current >= 0.0f) ? current : thresh;
    float upper = (current - thresh >= 0.0f) ? current : thresh;

    TransitionParams params;
    params.value         = lower;
    params.vtableOrState = *reinterpret_cast<uint32_t*>(self);
    params.padding       = 0;

    TransitionFlags flags = { 3u, 1u };

    uint8_t ok = pongCameraMgr_6E08(gameState, &params, mode, &flags, 1);
    if (ok == 0) return false;
    if (params.value > upper) return false;

    *reinterpret_cast<float*>(self) = params.value;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_B9B0_g @ 0x8220B9B0 | size: 0x170
//
// Camera-slot ring-buffer grow helper. Reads current size (half-word at
// self+4) and capacity (half-word at self+6). When size == capacity it:
//   1. computes a new capacity = (capacity + 16) * 24
//   2. calls rage::xe_EC88 (realloc analogue) to acquire a new buffer
//   3. zeros the new region
//   4. copies existing entries (size entries, 24 bytes each) into the new buffer
//   5. frees the old buffer (via atSingleton::Find_90D0 vtable slot 2)
//   6. stores the new buffer pointer at self+0 and returns nullptr.
// The function always bumps the size count (half-word at self+4) by one and
// returns the address of the new slot.
//
// Return: pointer to the newly-allocated slot inside the buffer
//         (base + size*24). Callers then populate that slot.
// ─────────────────────────────────────────────────────────────────────────────
void* pongCameraMgr_B9B0_g(void* self, int /*arg*/) {
    if (!self) return nullptr;

    uint8_t* mgr = static_cast<uint8_t*>(self);
    uint16_t* pCapacity = reinterpret_cast<uint16_t*>(mgr + 6);
    uint16_t* pSize     = reinterpret_cast<uint16_t*>(mgr + 4);
    void**    pBuf      = reinterpret_cast<void**>(mgr + 0);

    uint16_t capacity = *pCapacity;
    uint16_t size     = *pSize;

    if (size == capacity) {
        // Grow by 16 entries; clamp to 0xAAAA to avoid the mul-by-24 overflow
        // that the PPC code traps on (`twlgei r4,-1`).
        uint32_t newCapacity = static_cast<uint32_t>(capacity) + 16u;
        if (newCapacity > 0xAAAAu) newCapacity = 0xAAAAu;

        *pCapacity = static_cast<uint16_t>(newCapacity);
        uint32_t bytes = newCapacity * 24u;  // rlwinm *3,0,28

        // Allocate a fresh buffer (engine uses xe_EC88 / pg_alloc).
        // Without that import lifted yet, we fall back to a generic
        // allocator: the engine heap-alloc shim used elsewhere.
        extern void* rage_Alloc(uint32_t);
        void* newBuf = rage_Alloc(bytes);
        if (newBuf) {
            std::memset(newBuf, 0, bytes);

            // Copy existing entries (6 x u32 = 24 bytes per slot).
            if (*pBuf != nullptr && size != 0) {
                uint8_t* oldBuf = static_cast<uint8_t*>(*pBuf);
                uint8_t* dst    = static_cast<uint8_t*>(newBuf);
                for (uint32_t i = 0; i < size; ++i) {
                    std::memcpy(dst + i * 24u, oldBuf + i * 24u, 24u);
                }

                // Release the old buffer. Engine path goes through a
                // singleton-checked dtor vtable slot — left deferred until
                // the proto-RAGE free-path is lifted. Leaking here matches
                // the previous stubbed behaviour (this helper is cold).
            }
            *pBuf = newBuf;
        }
    }

    // Bump the size counter and return the address of the newly-acquired slot.
    uint16_t slot = *pSize;
    *pSize = static_cast<uint16_t>(slot + 1);
    if (*pBuf == nullptr) return nullptr;
    return static_cast<uint8_t*>(*pBuf) + slot * 24u;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_ValidateTransition — synthetic glue symbol used by
// pong_camera.cpp::TryTransition. The real validator at 0x821F3500 _is_
// pongCameraMgr_3500_fw above, but the TryTransition path wraps it with
// a full flags block. We delegate to 6E08 directly here, preserving the
// contract the caller already assumes.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t pongCameraMgr_ValidateTransition(void* gameState,
                                                     TransitionParams* params,
                                                     int mode,
                                                     TransitionFlags* flags,
                                                     int enable) {
    return pongCameraMgr_6E08(gameState, params, mode, flags, enable);
}

// ─────────────────────────────────────────────────────────────────────────────
// C-linkage helpers used by the pong_camera.cpp wrapper methods.
// Keep these in an extern "C" block so the declarations at pong_camera.cpp
// lines 522-697 resolve without mangling mismatches.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {

/**
 * pongCameraMgr_FloatThresholdA — returns true iff *self > threshold.
 * Used by CheckThreshold7550.
 */
bool pongCameraMgr_FloatThresholdA(void* self, float threshold) {
    if (!self) return false;
    float current = *static_cast<const float*>(self);
    return (current - threshold) > 0.0f;
}

/**
 * pongCameraMgr_FloatThresholdB — returns true iff *self > 0.
 * Used by the 7588/75C8/7608/7648/7688 quintuplet.
 */
bool pongCameraMgr_FloatThresholdB(void* self) {
    if (!self) return false;
    float current = *static_cast<const float*>(self);
    return current > 0.0f;
}

/**
 * pongCameraMgr_QueryA / B / C — paired camera-slot predicates.
 *
 * The recomp resolves these to the same three addresses as the
 * clamp-dispatch wrappers above (A=3500, B=35A0, C=3650). The callers in
 * pong_camera.cpp feed a local `stateCopy` uint64_t (first 8 bytes of the
 * camera mgr) as `self`, not a real camera object. Route directly into the
 * corresponding `_fw` worker.
 */
bool pongCameraMgr_QueryA(void* stateCopy, int index) {
    return pongCameraMgr_3500_fw(static_cast<pongCameraMgr*>(stateCopy),
                                 nullptr, index);
}

bool pongCameraMgr_QueryB(void* stateCopy, int index) {
    return pongCameraMgr_35A0_fw(static_cast<pongCameraMgr*>(stateCopy),
                                 nullptr, index);
}

bool pongCameraMgr_QueryC(void* stateCopy, int index) {
    return pongCameraMgr_3650_fw(static_cast<pongCameraMgr*>(stateCopy),
                                 nullptr, index);
}

/**
 * pongCameraMgr_SetCameraByIndex @ 0x821F7CD8 (tail-call target for the
 * SelectCameraN thunks).
 *
 * Writes the index into the manager's m_resetCounter field (+0x38) and
 * clears the camera-object pointers (+0x28 / +0x2C). This matches the
 * observed behaviour: SelectCameraN is always followed by a state reset.
 */
void pongCameraMgr_SetCameraByIndex(void* mgr, int index) {
    if (!mgr) return;
    uint8_t* self = static_cast<uint8_t*>(mgr);
    *reinterpret_cast<uint32_t*>(self + 0x28) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x2C) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x38) = static_cast<uint32_t>(index);
}

}  // extern "C"
