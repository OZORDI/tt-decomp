/**
 * message.hpp — Game Message Class Hierarchy
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Original source: .\message.cpp (relative path embedded in binary)
 *
 * Defines the network message classes used for game events:
 *   HitMessage                  — Base: a player hit the ball
 *   BallHitMessage              — Extended: ball hit with frame/timing data
 *   SpectatorBallHitMessage     — Spectator view variant with position
 *   SignalReadyToServeMessage   — Server ready signal
 *
 * All message classes share a common vtable layout:
 *   [1] Deserialize  — read message data from a network stream
 *   [2] Serialize    — write message data to a network stream
 *   [5] Allocate     — return this message to a free-list pool
 *   [6] GetPool      — return the global free-list pool for this type
 *   [7] GetClassName — return the RTTI class name string
 *   [8] SetData      — populate the message fields before sending
 *
 * Additional slots vary per class:
 *   HitMessage:                    [3] Use (dispatch, 692 bytes)
 *   BallHitMessage:                [3] Use
 *   SpectatorBallHitMessage:       [3] Use
 *   SignalReadyToServeMessage:     [4] Use (different slot!)
 */

#pragma once

#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// HitMessage  [vtable @ 0x8206F528]
//
// Base message for a player hit event.
// Layout (12 bytes):
//   +0x00  vtable pointer
//   +0x04  m_timestamp       (float)  — game time when the hit occurred
//   +0x08  m_playerIndex     (uint8_t) — index of the hitting player (0 or 1)
// ─────────────────────────────────────────────────────────────────────────────
struct HitMessage {
    void*    vtable;           // +0x00
    float    m_timestamp;      // +0x04: game time of the hit
    uint8_t  m_playerIndex;    // +0x08: which player hit (0/1)

    // ── Virtual methods ──
    // [1] Deserialize(stream)     @ 0x823B5FE0 | reads float+bytes from stream
    // [2] Serialize(stream)       @ 0x823BA5F0 | writes float+bytes to stream
    // [3] Use()                   @ 0x823B6128 | main dispatch (692 bytes)
    // [5] Allocate()              @ 0x823B6040 | return to pool free-list
    // [6] GetPool()               @ 0x823B6108 | return global pool ptr
    // [7] GetClassName()          @ 0x823B6118 | return "HitMessage"
    // [8] SetData(float, byte)    @ 0x823B5FD0 | populate fields
};

// ─────────────────────────────────────────────────────────────────────────────
// BallHitMessage  [vtable @ 0x8206FA9C]
//
// Extended hit message with additional ball/frame timing data.
// Inherits from HitMessage.
// Layout (20 bytes):
//   +0x00  vtable pointer
//   +0x04  m_timestamp       (float)  — inherited from HitMessage
//   +0x08  m_playerIndex     (uint8_t) — inherited from HitMessage
//   +0x0C  m_hitCount        (uint16_t) — rally hit counter
//   +0x0E  m_frameOffset     (uint8_t)  — frame offset (timing delta)
//   +0x10  m_matchTime       (int16_t)  — match time (signed, from render state)
// ─────────────────────────────────────────────────────────────────────────────
struct BallHitMessage : public HitMessage {
    uint8_t  _pad09[3];        // +0x09..+0x0B alignment
    uint16_t m_hitCount;       // +0x0C: rally hit counter
    uint8_t  m_frameOffset;    // +0x0E: frame timing offset
    uint8_t  _pad0F;           // +0x0F: alignment
    int16_t  m_matchTime;      // +0x10: match time from render state

    // ── Virtual methods ──
    // [1] Deserialize(stream)     @ 0x823B65F8 | calls base, reads extra fields
    // [2] Serialize(stream)       @ 0x823B6660 | writes all fields to stream
    // [3] Use()                   @ 0x823B6588 | dispatch with PostMessage
    // [5] Allocate()              @ 0x823B63E0 | return to pool (24-byte entries)
    // [6] GetPool()               @ 0x823B64E8 | return global pool ptr
    // [7] GetClassName()          @ 0x823B64F8 | return "BallHitMessage"
    // [8] SetData(float, byte)    @ 0x823B6508 | populate all fields from game state
};

// ─────────────────────────────────────────────────────────────────────────────
// SpectatorBallHitMessage  [vtable @ 0x8206FAC8]
//
// Spectator view variant of BallHitMessage with 3D position data.
// Inherits from HitMessage (same base fields).
// Layout (28+ bytes):
//   +0x00  vtable pointer
//   +0x04  m_timestamp       (float)
//   +0x08  m_playerIndex     (uint8_t)
//   +0x10  m_position        (float[3]) — ball position at hit time
// ─────────────────────────────────────────────────────────────────────────────
struct SpectatorBallHitMessage : public HitMessage {
    uint8_t  _pad09[7];        // +0x09..+0x0F alignment
    float    m_position[3];    // +0x10: ball position XYZ

    // ── Virtual methods ──
    // [1] Deserialize(stream)     @ 0x823B6B00
    // [2] Serialize(stream)       @ 0x823B6B48
    // [3] Use()                   @ 0x823B6950 | dispatch (428 bytes)
    // [5] Allocate()              @ 0x823B67E8
    // [6] GetPool()               @ 0x823B68D0
    // [7] GetClassName()          @ 0x823B68E0
    // [8] SetData(float, byte)    @ 0x823B68F0
};

// ─────────────────────────────────────────────────────────────────────────────
// SignalReadyToServeMessage  [vtable @ 0x8206F6BC]
//
// Signal from a player that they are ready to serve.
// Uses the same HitMessage base layout (timestamp + player index).
// The Use function is at vtable slot 4 (not slot 3 like the hit messages).
// Layout (12 bytes, same as HitMessage):
//   +0x00  vtable pointer
//   +0x04  m_timestamp       (float) — computed from game timer
//   +0x08  m_playerIndex     (uint8_t)
// ─────────────────────────────────────────────────────────────────────────────
struct SignalReadyToServeMessage : public HitMessage {
    // No additional fields — same layout as HitMessage

    // ── Virtual methods ──
    // [4] Use()                   @ 0x823B8808 | serve-ready dispatch (208 bytes)
    // [5] Allocate()              @ 0x823B8720
    // [6] GetPool()               @ 0x823B87E8
    // [7] GetClassName()          @ 0x823B87F8
    // [8] SetData(byte)           @ 0x823B86E8 | sets playerIndex + computes timestamp
};
