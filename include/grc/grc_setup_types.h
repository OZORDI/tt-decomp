/**
 * grc/grc_setup_types.h — C-compatible grcSetup struct and helpers
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * app_init.c is compiled as C99/C11; it needs a plain-C definition of
 * grcSetup with all data fields. The C++ side in grc_render.hpp holds
 * the virtual-method declarations. This header guards itself so only
 * one definition is visible in any given translation unit.
 */
#pragma once
#include <stdint.h>
#include <stddef.h>

#ifndef __cplusplus   /* C-only: full struct definitions */

/* ── grcNameRecord ──────────────────────────────────────────────────────── */
struct grcNameRecord {
    uint32_t field0;
    uint32_t field1;
    uint32_t field2;
    uint32_t field3;
};

/* ── grcFlagsObj ────────────────────────────────────────────────────────── */
struct grcFlagsObj {
    void*    pFlags;
    uint32_t flagBits;
};

/* ── grcSingletonSlot ───────────────────────────────────────────────────── */
struct grcSingletonSlot {
    void*    pInstance;   /* non-NULL if slot taken */
    uint32_t refCount;
};

/* ── grcSetup (C layout — data fields only, no vtable machinery) ─────────── */
struct grcSetup {
    void**      vtable;
    uint32_t    flags;
    uint64_t    m_renderTimestamp;
    uint64_t    m_frameTimestamp2;
    uint64_t    m_frameTimestamp3;
    float       m_smoothRenderTime;
    uint64_t    m_frameTimestamp1;
    float       m_smoothFrameTime;
    uint8_t     m_bActive;
    uint8_t     m_bVisible;
    uint8_t     m_bLodFlag;
    uint8_t     m_bHasFlags;
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
    uint8_t     _pad[0x168];
    uint8_t     m_renderState;
    uint64_t    m_presentTimestamp;
};

#else  /* C++ — provide only the helper types that grc_render.hpp doesn't define */

struct grcNameRecord {
    uint32_t field0;
    uint32_t field1;
    uint32_t field2;
    uint32_t field3;
};
struct grcFlagsObj {
    void*    pFlags;
    uint32_t flagBits;
};
struct grcSingletonSlot {
    void*    pInstance;
    uint32_t refCount;
};

#endif /* !__cplusplus */

/* mftb stub for non-PPC platforms */
#ifndef __mftb
#  include <stdint.h>
#  include <time.h>
static inline uint64_t __mftb(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}
#endif
