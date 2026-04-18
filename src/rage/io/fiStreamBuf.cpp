/**
 * fiStreamBuf.cpp — proto-RAGE stream-buffer bridging surface
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * fiStreamBuf is the platform-agnostic read/write/close wrapper that sits
 * on top of fiDevice handles. In this tree the surface is consumed by:
 *   - src/game/save/mc_memcard.cpp     (read & close while loading saves)
 *   - src/rage/memory/sysMemSimpleAllocator.cpp  (close on alloc log dump)
 *   - src/rage/io/parStream.cpp        (commented-out RBF read hook)
 *
 * Individual per-method decomps will drop in here as they land.
 *
 * xapo_UnpackBuffer lives alongside fiStreamBuf since it too reads a
 * platform buffer descriptor: pong_audio_effects.cpp Process paths call
 * it with the XAPO input-buffer parameter pair to extract frame counts.
 */

#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// fiStreamBuf core surface
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void fiStreamBuf_Read_stub(void* stream, void* dst, uint32_t size) {
    (void)stream; (void)dst; (void)size;
}

extern "C" void fiStreamBuf_Close_stub(void* stream) {
    (void)stream;
}

// ─────────────────────────────────────────────────────────────────────────────
// XAPO — Xbox Audio Processing Objects helper.
// Extracts frame-count / sample-buffer metadata from a platform buffer
// descriptor. Consumed by CDelayEffect::Process and CPeakMeterEffect::Process
// in pong_audio_effects.cpp.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void xapo_UnpackBuffer(void* buffer, void* outInfo) {
    (void)buffer; (void)outInfo;
}
