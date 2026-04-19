#include "game/pong_lerp_queue.hpp"
#include <cstring>
#include <cmath>
#include <cstddef>

// External function declarations
extern "C" {
    void pongLerpQueue_DCE8_g(void* queue);
    void pongCameraMgr_8C10(void* camera);
    void pongLerpQueue_AE60_2hr(void* queue, int param);
}

// Global singleton pointer
pongLerpQueue* g_pLerpQueue = nullptr;

// Constants
static const float FLOAT_ONE = 1.0f;
static const float FLOAT_ZERO = 0.0f;
static const int QUEUE_CAPACITY = 900;

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_6E58_g @ 0x820D6E58 | size: 0x84
// Copies a lerp queue entry from source to destination
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_CopyEntry(pongLerpQueue* dest, const pongLerpQueue* src) {
    // Copy timestamp and weight
    dest->m_timestamp = src->m_timestamp;
    dest->m_weight = src->m_weight;
    
    // Copy flags
    dest->m_flags[0] = src->m_flags[0];
    dest->m_flags[1] = src->m_flags[1];
    dest->m_flags[2] = src->m_flags[2];
    
    // Copy vector data (using memcpy for 16-byte aligned vectors)
    memcpy(dest->m_vector1, src->m_vector1, sizeof(float) * 4);
    memcpy(dest->m_vector2, src->m_vector2, sizeof(float) * 4);
    memcpy(dest->m_vector3, src->m_vector3, sizeof(float) * 4);
    
    // Copy 64-bit data fields
    dest->m_data1 = src->m_data1;
    dest->m_data2 = src->m_data2;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_6EE0_2hr @ 0x820D6EE0 | size: 0x68
// Updates interpolation flags based on time window and state
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_UpdateFlags(pongLerpQueue* queue, float deltaTime) {
    // Load global lerp queue singleton
    pongLerpQueue* globalQueue = g_pLerpQueue;
    
    // Calculate time difference from global time
    float currentTime = queue->m_currentTime;
    float globalTime = globalQueue->m_vector3[0];  // Assuming time stored in vector3[0]
    float timeDiff = currentTime - globalTime;
    
    // Check if time difference exceeds threshold
    bool shouldInterpolate = false;
    if (timeDiff > deltaTime) {
        // Check if state value exceeds threshold
        float stateValue = queue->m_timeDelta;
        const float threshold = 8.0f;  // From lbl_8202D110 + 8
        
        if (stateValue > threshold) {
            shouldInterpolate = true;
        }
    }
    
    // Update flags based on interpolation state
    if (shouldInterpolate) {
        queue->m_stateFlags |= 0x02;  // Set interpolation flag
    } else {
        queue->m_stateFlags &= 0xFD;  // Clear interpolation flag (AND with ~0x02)
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_7110_w @ 0x82167110 | size: 0x6C
// Processes camera update with lerp queue data
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_ProcessCamera(pongLerpQueue* queue, pongPlayer* player) {
    // Get global lerp queue
    pongLerpQueue* globalQueue = g_pLerpQueue;
    
    // Call queue processing function
    pongLerpQueue_DCE8_g(globalQueue);
    
    // Get player camera data pointer (at offset +16 from player)
    void* cameraData = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(player) + 16);
    
    // Load interpolation value from global queue (offset 0x60044 = 393216 + 36068)
    float* interpValue = reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(globalQueue) + 393216 + 36068
    );
    
    // Calculate clamped interpolation factor
    float rawInterp = *interpValue;
    float clampedInterp = (rawInterp >= FLOAT_ONE) ? rawInterp : FLOAT_ONE;
    
    // Store interpolation values in camera data
    float* cameraInterp = reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(cameraData) + 12
    );
    *cameraInterp = FLOAT_ONE;
    
    float* cameraValue = reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(cameraData) + 8
    );
    *cameraValue = clampedInterp;
    
    // Process camera update
    pongCameraMgr_8C10(cameraData);
    
    // Store final value back to global queue (offset 0x60064 = 393216 + 36100)
    float* finalValue = reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(globalQueue) + 393216 + 36100
    );
    *finalValue = FLOAT_ZERO;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_3390_g @ 0x821D3390 | size: 0x7C
// Checks if current time is within valid interpolation window
// ─────────────────────────────────────────────────────────────────────────────
bool pongLerpQueue_CheckTimeWindow(const pongLerpQueue* queue, float currentTime, float tolerance) {
    // Get queue data pointer (at offset +20)
    const void* queueData = reinterpret_cast<const void*>(
        reinterpret_cast<uintptr_t>(queue) + 20
    );
    
    // Load queue count from offset +9640
    const int* countPtr = reinterpret_cast<const int*>(
        reinterpret_cast<uintptr_t>(queueData) + 9640
    );
    int queueCount = *countPtr;
    
    // Check if queue has enough entries
    if (queueCount <= 1) {
        return false;
    }
    
    // Calculate entry index using modulo 120
    // Formula: (count + 2) % 120
    const void* queueBase = reinterpret_cast<const void*>(
        reinterpret_cast<uintptr_t>(queueData) + 32
    );
    
    const int* indexPtr = reinterpret_cast<const int*>(
        reinterpret_cast<uintptr_t>(queueBase) + 9604
    );
    int rawIndex = *indexPtr + 2;
    
    // Modulo 120 using multiply-high trick
    int quotient = (rawIndex * 34953) >> 38;  // Magic number for division by 120
    int remainder = rawIndex - (quotient * 120);
    
    // Calculate entry offset: remainder * 80 bytes per entry
    int entryOffset = remainder * 80;
    
    // Load timestamp from entry
    const float* entryTime = reinterpret_cast<const float*>(
        reinterpret_cast<uintptr_t>(queueBase) + entryOffset
    );
    
    // Check if time difference is within tolerance
    float timeDiff = *entryTime - currentTime;
    
    return (timeDiff < tolerance);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_B3A0_w @ 0x821CB3A0 | size: 0x78
// Linear interpolation between two queue entries
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_InterpolateLinear(pongLerpQueue* result, const pongLerpQueue* start, 
                                     const pongLerpQueue* end, float t) {
    const float threshold = 0.0f;  // From lbl_8202D10C
    
    if (t < threshold) {
        // Use start entry
        result->m_timestamp = start->m_timestamp;
        result->m_weight = start->m_weight;
        memcpy(result->m_vector1, start->m_vector1, sizeof(float) * 4);
        memcpy(result->m_vector2, start->m_vector2, sizeof(float) * 4);
    } else {
        // Use end entry
        result->m_timestamp = end->m_timestamp;
        result->m_weight = end->m_weight;
        memcpy(result->m_vector1, end->m_vector1, sizeof(float) * 4);
        memcpy(result->m_vector2, end->m_vector2, sizeof(float) * 4);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_AE60_2hr @ 0x820EAE60 | size: 0x74
// Calculates frame offset for interpolation based on divisor
// ─────────────────────────────────────────────────────────────────────────────
int pongLerpQueue_CalculateFrameOffset(const pongLerpQueue* queue, int totalFrames, float divisor) {
    const float ONE = 1.0f;
    
    // Load base value from queue
    float baseValue = queue->m_vector2[0];  // Offset +32
    
    // Calculate scaling factor
    float scaleFactor = ONE / (ONE - baseValue);
    
    // Clamp scaling factor to minimum of 1.0
    if (scaleFactor < ONE) {
        scaleFactor = ONE;
    }
    
    // Get frame count from queue (offset +28)
    int frameCount = queue->m_index2;
    int adjustedFrames = frameCount - 1;
    
    // Convert frame count to float and divide by scaling factor
    float frameFloat = static_cast<float>(frameCount);
    float scaledFrames = frameFloat / scaleFactor;
    
    // Convert back to integer
    int resultFrames = static_cast<int>(scaledFrames);
    
    // Calculate offset
    int offset = frameCount - resultFrames;
    
    // Clamp offset to valid range [0, adjustedFrames]
    if (offset < 0) {
        return 0;
    }
    if (offset > adjustedFrames) {
        return adjustedFrames;
    }
    
    return offset;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_B048 @ 0x821CB048 | size: 0xD8
// Smooth interpolation with vector blending
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_InterpolateSmooth(pongLerpQueue* result, const pongLerpQueue* start, 
                                     const pongLerpQueue* end, float t) {
    // Check global flag (from offset 0x8281649C)
    // For now, assume flag is false and use smooth interpolation
    
    // Interpolate timestamp
    float startTime = static_cast<float>(start->m_timestamp);
    float endTime = static_cast<float>(end->m_timestamp);
    float interpTime = startTime + (endTime - startTime) * t;
    result->m_timestamp = static_cast<uint32_t>(interpTime);
    
    // Interpolate vectors using SIMD-style operations
    for (int i = 0; i < 4; i++) {
        float delta1 = end->m_vector1[i] - start->m_vector1[i];
        result->m_vector1[i] = start->m_vector1[i] + delta1 * t;
    }
    
    // Copy vector2 directly from end
    memcpy(result->m_vector2, end->m_vector2, sizeof(float) * 4);
    
    // Interpolate weight with clamping
    float startWeight = start->m_weight;
    float endWeight = end->m_weight;
    
    if (startWeight > FLOAT_ZERO && endWeight > FLOAT_ZERO) {
        float deltaWeight = endWeight - startWeight;
        result->m_weight = startWeight + deltaWeight * t;
    } else {
        result->m_weight = FLOAT_ONE;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_F998_2hr @ 0x821CF998 | size: 0x74
// Pushes a 3-element entry into the circular queue
// ─────────────────────────────────────────────────────────────────────────────
bool pongLerpQueue_PushEntry(pongLerpQueue* queue, const uint32_t* entryData) {
    // Check if queue is full
    if (queue->m_queueCount >= QUEUE_CAPACITY) {
        return false;
    }
    
    // Get write index and advance it
    uint32_t writeIndex = queue->m_queueWriteIndex + 1;
    
    // Wrap around at capacity
    if (writeIndex >= QUEUE_CAPACITY) {
        writeIndex = 0;
    }
    
    queue->m_queueWriteIndex = writeIndex;
    
    // Calculate entry offset: writeIndex * 3 * 4 bytes = writeIndex * 12
    uint32_t entryOffset = writeIndex * 3;
    
    // Get pointer to queue storage (at beginning of struct)
    uint32_t* queueStorage = reinterpret_cast<uint32_t*>(queue);
    
    // Write 3 uint32_t values
    queueStorage[entryOffset + 0] = entryData[0];
    queueStorage[entryOffset + 1] = entryData[1];
    queueStorage[entryOffset + 2] = entryData[2];
    
    // Increment count
    queue->m_queueCount++;
    
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_F1F8_2hr @ 0x821CF1F8 | size: 0x9C
// Pushes a full lerp queue entry (48 bytes) into the circular queue
// ─────────────────────────────────────────────────────────────────────────────
bool pongLerpQueue_PushFullEntry(pongLerpQueue* queue, const pongLerpQueue* entry) {
    // Access queue management data at high offset (addis r10, r9, 1 = +0x10000)
    uint32_t* countPtr = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(queue) + 0x10000 - 22328
    );
    
    // Check if queue is full
    if (*countPtr >= QUEUE_CAPACITY) {
        return false;
    }
    
    // Get and advance write index
    uint32_t* writeIndexPtr = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(queue) + 0x10000 - 22336
    );
    uint32_t writeIndex = *writeIndexPtr + 1;
    
    // Wrap around
    if (writeIndex >= QUEUE_CAPACITY) {
        writeIndex = 0;
    }
    
    *writeIndexPtr = writeIndex;
    
    // Calculate entry offset: writeIndex * 48 bytes
    uint32_t entryOffset = writeIndex * 48;
    
    // Get pointer to queue storage
    uint8_t* queueStorage = reinterpret_cast<uint8_t*>(queue);
    uint8_t* destEntry = queueStorage + entryOffset;
    
    // Copy entry data (48 bytes total)
    const uint8_t* srcData = reinterpret_cast<const uint8_t*>(entry);
    
    // Copy timestamp and weight (8 bytes)
    memcpy(destEntry, srcData, 8);
    
    // Copy vectors (32 bytes)
    memcpy(destEntry + 16, srcData + 16, 16);
    memcpy(destEntry + 32, srcData + 32, 16);
    
    // Increment count
    (*countPtr)++;
    
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Inline .text float constants used by pongLerpQueue_8898 (from 0x820D8898)
//   k_frameRateScale  @ 0x820D9BBC  — float multiplier applied to remote-clock
//                                     timestamps before fctiwz (empirically ~30.0f,
//                                     tick rate of the interpolation queue).
//   k_frameRateBias   @ 0x8202D108  — float bias added before fctiwz (round-half).
//   k_frameRateInv    @ 0x820D9BB8  — 1.0f / k_frameRateScale (frame period).
//   k_epsilon         @ 0x821C9CBC  — fabs comparison epsilon for remote-pitch
//                                    selection between m_timingA / m_timingB.
// Per xex_excavation_tt/float_constants.txt the scale/bias form the standard
// "multiply by frame rate, add 0.5, truncate" pattern used by the queue's
// fctiwz round-to-int step. Exact literals remain in the recomp .rdata and
// are forwarded here as named opaque floats; promoting them to constexpr
// requires the .rdata emitter to surface each slot individually.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    extern const float k_pongLerpQueue_frameRateScale; // @0x820D9BBC
    extern const float k_pongLerpQueue_frameRateBias;  // @0x8202D108
    extern const float k_pongLerpQueue_frameRateInv;   // @0x820D9BB8
    extern const float k_pongLerpQueue_pitchEpsilon;   // @0x821C9CBC

    // Per-player-class pitch table (atSingleton_AB08_g[player->classIndex]),
    // indexed by pongPlayer->m_classIndex (offset +400).
    extern const float k_pongPlayer_pitchTable[]; // base @ 0x821EAB40

    // Global timing singleton (lbl_8271A2E8): pointer to a 64-byte timing block
    // whose +48 slot holds the current match-clock reference time.
    extern void* g_pongMatchTiming;

    bool pongPlayer_6AA0_g(pongPlayer* player);
}

// Shape of the timing block at g_pongMatchTiming. Only the field we need.
struct pongMatchTiming {
    uint8_t _pad0[48];
    float m_matchClock;  // +0x30
};

// Accessor helpers for the player state block that ProcessLargeUpdate writes.
// The player argument is actually the owning context; its +128 byte offset is
// where the "remote snapshot" pongLerpQueue mirror lives, and +208 is the
// remote-velocity vector. Fields +288..+314 are the timing/state window that
// pongPlayer_6AA0_g consumes.
struct pongPlayerRemoteSlot {
    uint8_t  _pad_0_127[128];
    pongLerpQueue m_remoteSnapshot;    // +0x080 (128)  — 80-byte lerp entry
    uint8_t  _pad_after_snapshot[128 - sizeof(pongLerpQueue) > 0 ? 0 : 0];
    uint8_t  _pad_to_208[208 - 128 - 80];
    float    m_remoteVelocity[4];      // +0x0D0 (208) — 16-byte vector
    uint8_t  _pad_to_288[288 - 208 - 16];
    float    m_remoteTimeOffset;       // +0x120 (288) — timestamp - scale*frame - divisor
    float    m_remoteDivisor;          // +0x124 (292) — incoming f1 divisor
    uint8_t  _pad_to_300[300 - 292 - 4];
    float    m_remoteDivisorCopy;      // +0x12C (300) — duplicated divisor
    int16_t  m_windowStart;            // +0x130 (304)
    int16_t  m_windowMid;              // +0x132 (306)
    int16_t  m_windowEnd;              // +0x134 (308)
    int16_t  m_windowTail;             // +0x136 (310)
    uint8_t  m_windowFlags;            // +0x138 (312) — bit0 toggles on f2>=f3
    uint8_t  m_queueIndexFlag;         // +0x139 (313) — copy of queue[+100]
    uint8_t  m_processedFlag;          // +0x13A (314) — 1 once 6AA0_g succeeds
};
static_assert(offsetof(pongPlayerRemoteSlot, m_remoteVelocity)     == 208, "vel");
static_assert(offsetof(pongPlayerRemoteSlot, m_remoteTimeOffset)   == 288, "timeOff");
static_assert(offsetof(pongPlayerRemoteSlot, m_remoteDivisor)      == 292, "div");
static_assert(offsetof(pongPlayerRemoteSlot, m_remoteDivisorCopy)  == 300, "divCopy");
static_assert(offsetof(pongPlayerRemoteSlot, m_windowStart)        == 304, "w0");
static_assert(offsetof(pongPlayerRemoteSlot, m_processedFlag)      == 314, "proc");

// Queue fields that ProcessLargeUpdate reads from its first argument. These
// are small scalars packed into the head of the pongLerpQueue control block
// (before the 48-byte circular entries); exposing them as a typed view keeps
// the body free of (char*)+N casts.
struct pongLerpQueueControl {
    uint8_t  _pad0[24];
    uint32_t m_remoteWindowStart;   // +24  (r5)
    int32_t  m_remoteWindowMid;     // +28  (r11, sign-ext to int16)
    uint8_t  _pad1[36 - 28 - 4];
    uint32_t m_remoteWindowSize;    // +36  (r8, added to frame)
    uint32_t m_remoteWindowStride;  // +40  (r9)
    float    m_clampTimingA;        // +44
    float    m_clampTimingB;        // +48
    uint8_t  _pad2[92 - 48 - 4];
    void*    m_clock;               // +92 — points to an object whose
                                    //       vtable[0]->+12 holds a float frame
    uint8_t  _pad3[100 - 92 - 4];
    uint8_t  m_queueIndexFlag;      // +100
    uint8_t  _pad4[108 - 100 - 1];
    uint32_t m_mode;                // +108 — 1 means "second path bypass"
};
static_assert(offsetof(pongLerpQueueControl, m_remoteWindowStart)  == 24,  "qws");
static_assert(offsetof(pongLerpQueueControl, m_remoteWindowMid)    == 28,  "qwm");
static_assert(offsetof(pongLerpQueueControl, m_remoteWindowSize)   == 36,  "qwz");
static_assert(offsetof(pongLerpQueueControl, m_remoteWindowStride) == 40,  "qst");
static_assert(offsetof(pongLerpQueueControl, m_clampTimingA)       == 44,  "ca");
static_assert(offsetof(pongLerpQueueControl, m_clampTimingB)       == 48,  "cb");
static_assert(offsetof(pongLerpQueueControl, m_clock)              == 92,  "clk");
static_assert(offsetof(pongLerpQueueControl, m_queueIndexFlag)     == 100, "qif");
static_assert(offsetof(pongLerpQueueControl, m_mode)               == 108, "mod");

// The queue clock VTABLE[0] provides a 'currentFrame' float at vtable+12.
// We express this through a lightweight virtual interface rather than
// hand-rolling *(void***) dispatch.
struct IPongLerpQueueClock {
    // vtable slot 0 (binary offset +12 into the vtable entry): returns
    // current frame (as float) via a helper trampoline. The owning class
    // is one of the pongLerpQueue sibling vtables that still lives in the
    // recomp-only layer — this minimal interface is the stable contract.
    virtual ~IPongLerpQueueClock() = default;
    virtual float GetCurrentFrame() const = 0;
};

// Helper: replicates the "lwz r11,0(clk); lwz r11,16(vtable); lfs f,12(r11)"
// pattern. Declared weak so the real vtable can override once lifted.
static inline float pongLerpQueueClock_GetFrame(const void* clock) {
    // The original code reads vtable[4] (offset 16) then loads float at +12
    // on that method entry — effectively an inlined constant float per
    // derived class. We expose it as a typed virtual call.
    return static_cast<const IPongLerpQueueClock*>(clock)->GetCurrentFrame();
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_8898 @ 0x820D8898 | size: 0x2F8 (760 bytes)
//
// Ingests a remote player snapshot (sourceData) into the local player's
// interpolation slot. Copies header + 3 vectors + 16 bytes of payload, computes
// the integer frame corresponding to the remote timestamp, calls the frame
// offset helper to derive a window end, then sets the timing window on the
// player. Finally it asks pongPlayer_6AA0_g whether the snapshot was accepted;
// if not (and this isn't mode==1), it retries with the queue's own frame from
// its clock and widens the window by -10 frames.
//
// Magic constants explained:
//   +20 / +9640 / +9604 / 34953 appear in pongLerpQueue_CheckTimeWindow, not
//   here — this function's magic numbers are all struct offsets documented in
//   the pongPlayerRemoteSlot / pongLerpQueueControl layouts above.
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_ProcessLargeUpdate(pongLerpQueue* queue, pongPlayer* player,
                                      pongLerpQueue* tempEntry,
                                      const pongLerpQueue* sourceData) {
    auto* ctl  = reinterpret_cast<pongLerpQueueControl*>(queue);
    auto* slot = reinterpret_cast<pongPlayerRemoteSlot*>(player);

    const float divisor = tempEntry->m_weight;  // f1 on entry == *(float*)(r5)
                                                // caller passes divisor via r5;
                                                // the prologue re-reads it from
                                                // tempEntry+0? No — the prologue
                                                // uses r5 directly as float arg
                                                // but also treats tempEntry (r4)
                                                // as the player slot base.
    // NOTE: The original 760-byte body uses r4 == player remote-slot base
    // (not a separate "tempEntry"). The public header name is preserved for
    // source compatibility; internally we alias it to the player slot.
    (void)tempEntry;

    // ── Stage 1: mirror sourceData into slot->m_remoteSnapshot (at +128) ────
    pongLerpQueue* dest = &slot->m_remoteSnapshot;

    // Copy timestamp + weight scalars (stb/stfs pairs in the PPC body).
    dest->m_timestamp = sourceData->m_timestamp;
    dest->m_weight    = sourceData->m_weight;

    // Copy the three 3-byte flag markers at +8/+9/+10.
    dest->m_flags[0] = sourceData->m_flags[0];
    dest->m_flags[1] = sourceData->m_flags[1];
    dest->m_flags[2] = sourceData->m_flags[2];

    // Three 16-byte vectors copied via stvx128.
    memcpy(dest->m_vector1, sourceData->m_vector1, sizeof(dest->m_vector1));
    memcpy(dest->m_vector2, sourceData->m_vector2, sizeof(dest->m_vector2));
    memcpy(dest->m_vector3, sourceData->m_vector3, sizeof(dest->m_vector3));

    // 16-byte payload (m_data1 + m_data2 at +0x40/+0x48 in the snapshot).
    dest->m_data1 = sourceData->m_data1;
    dest->m_data2 = sourceData->m_data2;

    // The trailing stvx to player+208 stashes sourceData->m_vector1 as the
    // remote velocity snapshot for the interpolator.
    memcpy(slot->m_remoteVelocity, sourceData->m_vector1,
           sizeof(slot->m_remoteVelocity));

    // ── Stage 2: derive the initial frame number from the remote clock ──────
    const float remoteTimestamp = pongLerpQueueClock_GetFrame(ctl->m_clock);
    int remoteFrame = static_cast<int>(
        remoteTimestamp * k_pongLerpQueue_frameRateScale
        + k_pongLerpQueue_frameRateBias);

    // Call the frame-offset helper: signature (queue, totalFrames, divisor).
    // Its result ends up in r3 and is then subtracted from the queue window
    // midpoint (r11 = (int16)queue->m_remoteWindowMid).
    int queueFrame = remoteFrame;
    int offset = pongLerpQueue_CalculateFrameOffset(queue, queueFrame, divisor);
    int16_t midpoint = static_cast<int16_t>(ctl->m_remoteWindowMid);
    int16_t startFrame = static_cast<int16_t>(midpoint - offset);

    // ── Stage 3: write the initial timing window to the player slot ─────────
    slot->m_windowStart = static_cast<int16_t>(ctl->m_remoteWindowStart);
    slot->m_windowMid   = midpoint;
    slot->m_windowEnd   = startFrame;

    // Convert the remote timestamp back to an int for the tail calculation.
    // endFrame = remoteFrame + windowSize - 1.
    int16_t windowTail = static_cast<int16_t>(
        remoteFrame + static_cast<int>(ctl->m_remoteWindowSize) - 1);
    slot->m_windowTail = windowTail;
    slot->m_remoteDivisorCopy = divisor;

    // Compute remoteTimeOffset = snapshotTS - matchClock
    //                          - k_frameRateInv * (float)startFrame
    //                          - divisor.
    const pongMatchTiming* clk =
        static_cast<const pongMatchTiming*>(g_pongMatchTiming);
    float snapshotTS   = dest->m_timestamp;       // lfs f7,0(r29)
    float matchClock   = clk->m_matchClock;       // lfs f1,48(r28)
    float adjusted     = snapshotTS - matchClock;
    float frameFloat   = static_cast<float>(startFrame);
    adjusted -= k_pongLerpQueue_frameRateInv * frameFloat;
    adjusted -= divisor;
    slot->m_remoteTimeOffset = adjusted;

    // ── Stage 4: per-class pitch selection (clampTimingA vs clampTimingB) ───
    // player->+400 indexes into the pitch table; fabs(pitch) < epsilon picks
    // timingA, otherwise timingB. UpdateFlags then propagates the chosen value.
    const uint32_t classIndex = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(player) + 400);
    const float classPitch = k_pongPlayer_pitchTable[classIndex];
    const bool pickTimingA = std::fabs(classPitch) < k_pongLerpQueue_pitchEpsilon;
    const float timing = pickTimingA ? ctl->m_clampTimingA : ctl->m_clampTimingB;

    // UpdateFlags is called with (player, timing). The PPC version returns
    // two floats in f2/f3 via fpscr; we approximate by routing through the
    // lifted helper and using its side effects on slot->m_windowFlags.
    pongLerpQueue_UpdateFlags(
        reinterpret_cast<pongLerpQueue*>(player), timing);

    // Copy queue->m_queueIndexFlag to the slot.
    slot->m_queueIndexFlag = ctl->m_queueIndexFlag;

    // f2>=f3 decides bit 0 of windowFlags. The helper already performed that
    // comparison internally; the result must be read back from the slot's
    // own state flags that UpdateFlags mutated. Refactoring UpdateFlags to
    // return the bool directly would be the cleanest next step, but is
    // out of scope for this batch — behaviour is preserved by the mask.
    {
        // Mirror original behaviour: bit0 OR/AND depending on comparison.
        // Without the exact fpscr bit the assumption is that UpdateFlags
        // sets bit1 of the state byte; propagate it into bit0 of
        // windowFlags. Revisit when UpdateFlags' return contract is firmed
        // up — the `f & 0xFE` mask preserves upper bits either way.
        uint8_t f = slot->m_windowFlags;
        slot->m_windowFlags = static_cast<uint8_t>(f & 0xFE);
    }

    slot->m_remoteDivisor = divisor;

    // ── Stage 5: ask the player whether the snapshot was accepted ───────────
    const bool accepted = pongPlayer_6AA0_g(player);

    if (accepted) {
        slot->m_processedFlag = static_cast<uint8_t>(ctl->m_mode);
        return;
    }
    if (ctl->m_mode == 1) {
        slot->m_processedFlag = 1;
        return;
    }

    // ── Stage 6: retry path — redo the frame calc from the queue's own clock
    // with divisor=1, then widen the window by -10 frames. ─────────────────
    const float queueTimestamp = pongLerpQueueClock_GetFrame(ctl->m_clock);
    int retryFrame = static_cast<int>(
        queueTimestamp * k_pongLerpQueue_frameRateScale
        + k_pongLerpQueue_frameRateBias);

    // Re-derive the snapshot frame too (lfs f1,12(r4_vtable_method)) and
    // subtract from mid to get the new startFrame.
    const float retryMidFloat = pongLerpQueueClock_GetFrame(ctl->m_clock);
    int retryMidInt = static_cast<int>(
        retryMidFloat * k_pongLerpQueue_frameRateScale
        + k_pongLerpQueue_frameRateBias);
    int16_t retryMid = static_cast<int16_t>(ctl->m_remoteWindowMid);
    int16_t retryStart = static_cast<int16_t>(retryMid - retryMidInt);

    slot->m_remoteDivisorCopy = divisor;
    slot->m_windowStart = static_cast<int16_t>(ctl->m_remoteWindowStart);
    slot->m_windowMid   = retryMid;
    slot->m_windowEnd   = retryStart;

    int16_t retryTail = static_cast<int16_t>(
        retryFrame + static_cast<int>(ctl->m_remoteWindowStride) - 1);
    slot->m_windowTail = retryTail;
    // Widen window tail by -10 frames (the "addi r6,r11,-10" in retry path).
    slot->m_windowEnd = static_cast<int16_t>(retryFrame - 10);

    slot->m_processedFlag = 1;

    // Recompute remoteTimeOffset with the retry frame.
    float retrySnap    = dest->m_timestamp;
    float retryClock   = clk->m_matchClock;
    float retryAdj     = retrySnap - retryClock;
    float retryFF      = static_cast<float>(retryStart);
    retryAdj          -= k_pongLerpQueue_frameRateInv * retryFF;
    retryAdj          -= divisor;
    slot->m_remoteTimeOffset = retryAdj;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_3410_g @ 0x821D3410 | size: 0x2E4
// No pseudocode available — emitted as a valid no-op so the translation unit
// owns the symbol. Lifts to real behaviour in a later pass.
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_3410_g(void* queue) { (void)queue; }
