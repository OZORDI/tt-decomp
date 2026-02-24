/**
 * serve_started_message.hpp — ServeStartedMessage class declaration
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Network message sent when the serving player begins a serve.
 * Carries timing, position, velocity and player-flag data.
 *
 * See serve_started_message.cpp for full field notes.
 */

#pragma once
#include <stdint.h>

// Forward declarations
struct vec3;
struct lfs_vec4 { float x, y, z, w; };

struct ServeStartedMessage {
    // ── vtable ─────────────────────────────────────────────────────────
    void**    vtable;               // +0x000

    // ── timing / position payload ───────────────────────────────────────
    float     m_timingRef;          // +0x004  serve start timing reference
    uint8_t   _pad0[8];
    lfs_vec4  m_targetPos;          // +0x010  target position (16-byte aligned)
    uint8_t   _pad1[0xBC];

    // ── velocity / spin payload ─────────────────────────────────────────
    float     m_velocityX;          // +0x0D0  (+208)
    float     m_velocityY;          // +0x0D4  (+212)
    float     m_velocityZ;          // +0x0D8  (+216)
    float     m_spin;               // +0x0DC  (+220) spin/power value

    // ── flag bytes ───────────────────────────────────────────────────────
    uint8_t   m_playerIdx;          // +0x0E0  (+224) player index: who is serving (0 or 1)
    uint8_t   m_bNotServer;         // +0x0E1  (+225) 0=this player is server, 1=not server
                                    //                (inverted when stored as m_bIsServer)
    uint8_t   m_playerIdx2;         // +0x0E2  (+226) secondary player / handedness flag

    // ── vtable methods ───────────────────────────────────────────────────
    virtual void       Deserialise(void* client);     // slot 1 — read fields from net stream
    virtual void       Serialise(void* client);       // slot 2 — write fields to net stream
    virtual void       Process(void* matchObj);       // slot 4 — apply to live game state
    virtual uint16_t   GetIndexInPool() const;        // slot 5 — index in message pool
    virtual void*      GetSingleton();                // slot 6 — pool/factory singleton
    virtual const char* GetTypeName();                // slot 7 — ASCII type name string

    // ── global singletons ────────────────────────────────────────────────
    // g_pNetMsgSingleton   @ 0x825D11D0  — pool singleton pointer
    // g_szServeStartedTypeName @ 0x8206E9D0
    // g_pPlayerLerpTable   @ lis(-32158)+-23600, stride=416 bytes/player
};

// ── Global refs needed by Process() ────────────────────────────────────────
extern void*    g_pNetMsgSingleton;         // @ 0x825D11D0
extern uint8_t* g_pPlayerLerpTable;        // @ lis(-32158)+-23600
