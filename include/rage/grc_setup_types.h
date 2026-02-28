/**
 * rage/grc_setup_types.h — grcSetup struct, grcNameRecord, grcFlagsObj
 * C-compatible layout for use from both .c and .cpp translation units
 */
#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Platform shim: __mftb() reads the PowerPC timebase.
 * On host x86-64 / arm64 we substitute RDTSC / monotonic counter. */
#if defined(__x86_64__) || defined(_M_X64)
#  include <x86intrin.h>
static inline uint64_t __mftb(void) { return __rdtsc(); }
#elif defined(__aarch64__) || defined(_M_ARM64)
static inline uint64_t __mftb(void) {
    uint64_t v; __asm__ volatile("mrs %0, cntvct_el0" : "=r"(v)); return v;
}
#else
static inline uint64_t __mftb(void) { return 0; }
#endif

/* ── grcNameRecord (4 fields, exact layout TBD) ───────────────────────── */
struct grcNameRecord {
    uint32_t field0;
    uint32_t field1;
    uint32_t field2;
    uint32_t field3;
};

/* ── grcFlagsObj (contains a pointer to optional flags) ──────────────── */
struct grcFlagsObj {
    void* pFlags;
};

/* ── grcSingletonSlot (availability guard for subsystems) ───────────── */
struct grcSingletonSlot {
    uint32_t flags;
    void*    pInstance;
};

/* ── grcSetup — full layout from app_init.c analysis ──────────────────── */
struct grcSetup {
    void**      vtable;               /* +0x00 */
    uint32_t    flags;                /* +0x04 */
    uint64_t    m_renderTimestamp;    /* +0x08 */
    uint64_t    m_presentTimestamp;   /* +0x10 */
    uint64_t    m_frameTimestamp2;    /* +0x18 */
    uint64_t    m_frameTimestamp3;    /* +0x20 — third mftb at ctor (see comment) */
    /* Note: the three ctor timestamps are stored in order 3,2,1 by the original
     * PPC code (mftb -> f3, mftb -> f2, mftb -> f1).  The layout here matches
     * the field offsets discovered from UpdateFrameTime/UpdateRenderTime. */
    float       m_smoothRenderTime;   /* +0x20 (+32) EMA render delta */
    uint64_t    m_frameTimestamp1;    /* +0x28 (+40) updated by UpdateFrameTime */
    float       m_smoothFrameTime;    /* +0x30 */
    uint8_t     _pad[0x198];          /* padding to m_renderState */
    uint8_t     m_renderState;        /* +0x1CC (460) */
    uint8_t     m_bActive;
    uint8_t     m_bVisible;
    uint8_t     m_bLodFlag;
    uint8_t     m_bHasFlags;
    uint8_t     _pad2[3];
    uint32_t    m_objectCount;
    void*       m_pChildLink;
    uint32_t    nameField0;
    uint32_t    nameField1;
    uint32_t    nameField2;
    uint32_t    nameField3;
    float       m_boundX;
    float       m_boundY;
    float       m_boundZ;
    uint32_t    m_instanceId;
    uint32_t    m_instanceId2;
};

#ifdef __cplusplus
} // extern "C"
#endif
