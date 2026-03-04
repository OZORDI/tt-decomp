#include "game/pong_lerp_queue.hpp"
#include <cstring>
#include <cmath>

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
// pongLerpQueue_8898 @ 0x820D8898 | size: 0x2F8
// Large complex function - processes player state update with interpolation
// This is a simplified version focusing on the core logic
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_ProcessLargeUpdate(pongLerpQueue* dest, pongPlayer* player, 
                                      pongLerpQueue* tempEntry, const pongLerpQueue* sourceData) {
    // This function is very complex (760 bytes of assembly)
    // It performs:
    // 1. Copies source data to temp entry
    // 2. Calculates frame timing and interpolation weights
    // 3. Updates player state indices
    // 4. Calls pongLerpQueue_AE60_2hr for frame offset calculation
    // 5. Processes camera and timing updates
    // 6. Updates various flags based on state
    
    // For now, provide a stub that maintains the function signature
    // Full implementation would require understanding the complete player state machine
    
    // Copy basic data from source to temp
    tempEntry->m_timestamp = sourceData->m_timestamp;
    tempEntry->m_weight = sourceData->m_weight;
    tempEntry->m_flags[0] = sourceData->m_flags[0];
    tempEntry->m_flags[1] = sourceData->m_flags[1];
    tempEntry->m_flags[2] = sourceData->m_flags[2];
    
    // Copy vector data
    memcpy(tempEntry->m_vector1, sourceData->m_vector1, sizeof(float) * 4);
    memcpy(tempEntry->m_vector2, sourceData->m_vector2, sizeof(float) * 4);
    memcpy(tempEntry->m_vector3, sourceData->m_vector3, sizeof(float) * 4);
    
    // TODO: Implement full state machine logic
    // This requires understanding player state structure and timing system
}
