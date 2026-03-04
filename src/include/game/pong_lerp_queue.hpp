#pragma once

#include <cstdint>

// Forward declarations
struct pongPlayer;
struct pongCameraMgr;

/**
 * pongLerpQueue — Network interpolation queue for smooth player movement
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Manages a circular buffer of player state snapshots for network interpolation.
 * Stores up to 900 timestamped entries with position, rotation, and state data.
 */
struct pongLerpQueue {
    uint32_t m_timestamp;           // +0x0000
    float m_weight;                 // +0x0004
    uint8_t m_flags[3];             // +0x0008-0x000A
    uint8_t _pad0[5];               // +0x000B-0x000F
    float m_vector1[4];             // +0x0010 (16-byte aligned vector)
    float m_vector2[4];             // +0x0020 (16-byte aligned vector)
    float m_vector3[4];             // +0x0030 (16-byte aligned vector)
    uint64_t m_data1;               // +0x0040
    uint64_t m_data2;               // +0x0048
    uint32_t m_frameIndex;          // +0x0050
    uint8_t _pad1[76];              // +0x0054-0x009F
    float m_currentTime;            // +0x0080 (overlaps padding - verify)
    uint8_t _pad2[200];             // +0x00A0-0x011F (adjust based on actual layout)
    float m_timeDelta;              // +0x0120
    uint8_t _pad3[12];              // +0x0124-0x012F
    float m_interpolationWeight;    // +0x012C (overlaps - verify)
    int16_t m_index1;               // +0x0130
    int16_t m_index2;               // +0x0132
    int16_t m_neighborIndex1;       // +0x0134
    int16_t m_neighborIndex2;       // +0x0136
    uint8_t m_stateFlags;           // +0x0138
    uint8_t m_playerState;          // +0x0139
    uint8_t _pad4[10458];           // +0x013A-0x2A2F
    
    // Circular queue management (at end of struct)
    uint32_t m_queueWriteIndex;     // +0x2A30 (10800) - write position (0-899)
    uint32_t _pad5;                 // +0x2A34
    uint32_t m_queueCount;          // +0x2A38 (10808) - number of entries (0-900)
    
    // Queue storage for 900 entries of 12 bytes each (3 x uint32_t per entry)
    // Located at offset 0 - 10799 (overlaps with above fields - this is a union-like structure)
};

// Global singleton
extern pongLerpQueue* g_pLerpQueue;

// Function declarations
void pongLerpQueue_CopyEntry(pongLerpQueue* dest, const pongLerpQueue* src);
void pongLerpQueue_UpdateFlags(pongLerpQueue* queue, float deltaTime);
void pongLerpQueue_ProcessCamera(pongLerpQueue* queue, pongPlayer* player);
bool pongLerpQueue_CheckTimeWindow(const pongLerpQueue* queue, float currentTime, float tolerance);
void pongLerpQueue_InterpolateLinear(pongLerpQueue* result, const pongLerpQueue* start, 
                                     const pongLerpQueue* end, float t);
void pongLerpQueue_InterpolateSmooth(pongLerpQueue* result, const pongLerpQueue* start, 
                                     const pongLerpQueue* end, float t);
int pongLerpQueue_CalculateFrameOffset(const pongLerpQueue* queue, int totalFrames, float divisor);
bool pongLerpQueue_PushEntry(pongLerpQueue* queue, const uint32_t* entryData);
bool pongLerpQueue_PushFullEntry(pongLerpQueue* queue, const pongLerpQueue* entry);
void pongLerpQueue_ProcessLargeUpdate(pongLerpQueue* dest, pongPlayer* player, 
                                      pongLerpQueue* tempEntry, const pongLerpQueue* sourceData);
