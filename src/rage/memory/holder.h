/**
 * holder.h — Holder Template Class Declaration
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * "Holder" is a template-instantiated class for managing game parameter and
 * fragment data.  It holds tuning parameters, transform caches, animation
 * state, and extended float parameters for game resources loaded from the
 * tuning system ($/tune/types/fragments).
 *
 * RTTI name: Holder
 * Vtable(s): 6 vtables (template specialization / virtual-base MI)
 *   Primary: 0x82032E34 (20 bytes = 5 vtable slots)
 *   Others:  0x82032E48, 0x82032E5C, 0x82032E70, 0x82032E84, (+ 1 more)
 *
 * 6 singleton instances stored at g_HolderSingletons @ 0x825D4560
 * (28 bytes each × 6 = 168 bytes total).
 *
 * Object layout (706+ bytes):
 *   +0x00   vtable pointer
 *   +0x04   m_tuningParam1          (float)  — set via SetParam1
 *   +0x08   m_tuningParam2          (float)  — set via SetParam2
 *   +0x0C   m_tuningParam3          (float)  — set via SetParam3
 *   +0x10   m_defaultVector         (float4) — identity vector (0,0,0,0)
 *   +0x20   (gap — other fields)
 *   +0x99   m_initFlags             (uint8)  — initialization flags
 *   +0xE0   m_constantVectors[64]   (float)  — constant transform data (4 × vec4)
 *   +0x130  m_tuningParams[8]       (float)  — tuning parameter array
 *   +0x150  m_tuningParam8          (float)  — set via SetParam8
 *   +0x154  m_tuningParam9          (float)  — set via SetParam9
 *   +0x158  m_tuningParam10         (float)  — set via SetParam10
 *   +0x1B0  m_transformCache[96]    (bytes)  — transform cache (6 × vec4)
 *   +0x210  m_animationState[128]   (bytes)  — animation state (8 × vec4)
 *   +0x290  m_extendedParams        (float)  — extended parameter block
 *   +0x2C0  m_stateFlags            (uint8)  — state flags
 *   +0x2C1  m_statusFlags           (uint8)  — status flags
 */
#pragma once
#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// Holder — Game Parameter / Fragment Data Manager
// ─────────────────────────────────────────────────────────────────────────────

class Holder {
public:
    // ── Virtual methods (vtable @ 0x82032E34, 5 slots) ──
    // slot 0: (unused / RTTI info pointer)
    virtual void* GetSingleton();                    // [1] @ 0x82121BF8 — returns template-specific singleton
    virtual void* AllocateAndInitialize();            // [2] @ 0x82121C08 — TLS-based 32-byte allocation + AltiVec copy

    // ── Constructor ──
    Holder();                                         // @ 0x82122798 | size: 0x11C

    // ── Parameter setters (extract float from source vtable slot 5) ──
    void SetParam1(void* source);                     // @ 0x82123488 — stores at +0x04
    void SetParam2(void* source);                     // @ 0x821234C8 — stores at +0x08
    void SetParam3(void* source);                     // @ 0x82123508 — stores at +0x0C
    void SetParam8(void* source);                     // @ 0x82123548 — stores at +0x150
    void SetParam9(void* source);                     // @ 0x82123588 — stores at +0x154
    void SetParam10(void* source);                    // @ 0x821235C8 — stores at +0x158

    // ── Flags / State ──
    void SetInitializedFlag();                        // @ 0x82123478 — sets bit 0 of m_initFlags
    void DispatchVtableSlot55();                       // @ 0x82122BF0 — tail-call to vtable slot 55

    // ── Fields (partial — full layout documented above) ──
    float    m_tuningParam1;         // +0x04
    float    m_tuningParam2;         // +0x08
    float    m_tuningParam3;         // +0x0C
    float    m_defaultVectorX;       // +0x10
    float    m_defaultVectorY;       // +0x14
    float    m_defaultVectorZ;       // +0x18
    float    m_defaultVectorW;       // +0x1C
    uint8_t  _pad_20[0x79];         // +0x20 .. +0x98
    uint8_t  m_initFlags;           // +0x99
    uint8_t  _pad_9A[0x46];        // +0x9A .. +0xDF
    uint8_t  m_constantVectors[64]; // +0xE0
    uint8_t  _pad_120[0x10];       // +0x120 .. +0x12F
    float    m_tuningParams[8];     // +0x130
    float    m_tuningParam8;        // +0x150
    float    m_tuningParam9;        // +0x154
    float    m_tuningParam10;       // +0x158
    uint8_t  _pad_15C[0x54];       // +0x15C .. +0x1AF
    uint8_t  m_transformCache[96];  // +0x1B0
    uint8_t  m_animationState[128]; // +0x210
    float    m_extendedParam0;      // +0x290
    float    m_extendedParam1;      // +0x294
    float    m_extendedParam2;      // +0x298
    uint32_t _pad_29C;             // +0x29C
    float    m_extendedParam3;      // +0x2A0
    float    m_extendedParam4;      // +0x2A4
    float    m_extendedParam5;      // +0x2A8
    uint32_t _pad_2AC;             // +0x2AC
    float    m_extendedParam6;      // +0x2B0
    uint8_t  _pad_2B4[0x0C];      // +0x2B4 .. +0x2BF
    uint8_t  m_stateFlags;         // +0x2C0
    uint8_t  m_statusFlags;        // +0x2C1
};

// Global singleton array — 6 template specializations, 28 bytes each.
// @ 0x825D4560
extern uint8_t g_HolderSingletons[168];
