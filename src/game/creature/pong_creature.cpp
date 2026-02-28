

/**
 * pongMover — Movement and positioning system for creatures
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Handles creature positioning, matrix calculations, and movement state.
 */

#include "pong_creature.hpp"
#include "globals.h"
#include <cstring>

// External dependencies
extern void rage_Free(void* ptr);
extern void nop_8240E6D0(const char* msg, ...);
extern void pongCreature_7CE8_g(void* creature, void* matrix, int param1, int param2, int param3, int param4);
extern void* pg_9C00_g(void* player, int index);  // @ 0x82019C00 — returns creature info ptr
extern void pongPlayer_9CD0_g(void* player, int index, void* outMatrix1, void* outMatrix2);

// Forward declarations
bool IsMatrixIdentity(const float* matrix);  // @ 0x820C3C70 - Matrix identity check

// Global data @ 0x825CB800 and 0x82619BE0
extern const float g_identityMatrixRef[16];      // @ 0x825CB800 - Reference identity matrix (64 bytes)
extern const uint32_t g_vectorComparisonMask[4]; // @ 0x82619BE0 - SIMD comparison mask (16 bytes)

// Global zero constant @ 0x8202D110
static const float g_zero = 0.0f;

/**
 * pongMover::pongMover @ 0x820CB5F0 | size: 0x90
 * 
 * Constructor - initializes mover with default values.
 * Sets up position, velocity, and state flags.
 */
pongMover::pongMover(void* creaturePool) {
    // Set vtable pointer
    vtable = (void**)0x8202807C;  // pongMover vtable @ 0x820280DC - 0x60

    // Zero twelve motion floats at +0x10..+0x3C
    m_posX  = m_posY  = m_posZ  = 0.0f;
    m_unk1C = m_unk20 = m_unk24 = 0.0f;
    m_unk28 = m_unk2C = m_unk30 = 0.0f;
    m_unk34 = m_unk38 = m_unk3C = 0.0f;

    // Mark slot active (+0x40 = 1)
    m_flags = 1;

    // Zero tail fields
    m_unkE0 = 0;
    m_unkE4 = 0;

    // If creature pointer is already set (placement-new on pre-allocated memory),
    // adjust it relative to the pool base: m_pCreature += pool[idx+2]
    if (m_pCreature) {
        uint32_t* pool   = (uint32_t*)creaturePool;
        uint32_t basePtr = pool[1];         // pool+4
        uint32_t stride  = pool[76 / 4];    // pool+76
        // twllei: stride must be > 0 (binary traps on divide-by-zero)
        uint32_t idx = ((uint32_t)(uintptr_t)m_pCreature - basePtr) / stride;
        m_pCreature = (void*)(uintptr_t)(pool[idx + 2] + (uint32_t)(uintptr_t)m_pCreature);
    }
}

/**
 * pongMover::~pongMover @ 0x820C9EF8 | size: 0x48
 * 
 * Destructor - cleans up mover resources.
 */
pongMover::~pongMover() {
    // Restore vtable to base class
    vtable = (void**)0x8202807C;
    
    // Note: Actual cleanup handled by flags parameter
    // If flags & 1, the calling code will free(this)
}

/**
 * pongMover::Reset @ 0x820C9F40 | size: 0x13C
 * 
 * Resets mover state to initial position and orientation.
 * Calculates initial matrix from creature data and applies it.
 */
void pongMover::Reset(void* creatureData) {
    float initMatrix[16];
    
    // Calculate initial transformation matrix
    CalcInitMatrix(initMatrix, this, creatureData);
    
    // Get creature index and look up in global table
    uint32_t creatureIndex = m_creatureIndex;
    uint32_t* globalTable = *(uint32_t**)0x82606430;  // g_creatureTable
    uint32_t tableEntry = globalTable[(creatureIndex + 33) * 4];
    uint32_t subEntry = globalTable[(tableEntry + 29) * 4];
    
    // Apply matrix to creature if valid
    if (m_pCreature && subEntry != 0) {
        pongCreature_7CE8_g(m_pCreature, initMatrix, 0, 1, 0, 0);
    } else {
        nop_8240E6D0("pongMover::Reset() - no creature to reset");
    }
    
    // Extract position from matrix column 3 (translation vector)
    m_posX  = initMatrix[12];   // +0x10
    m_posY  = initMatrix[13];   // +0x14
    m_posZ  = 0.0f;             // +0x18

    // Zero remaining motion floats
    m_unk1C = m_unk20 = m_unk24 = 0.0f;
    m_unk28 = m_unk2C = m_unk30 = 0.0f;
    m_unk34 = m_unk38 = m_unk3C = 0.0f;

    // Clear physics bits 1 and 2 from flags (+0x40), preserve all others
    m_flags = (m_flags & 0xF9);

    // Zero physics state floats (+0x48..+0x54)
    m_unk48 = m_unk4C = m_unk50 = m_unk54 = 0.0f;

    // Zero 16-byte AltiVec-aligned movement buffer (+0x60)
    memset(m_unk60, 0, sizeof(m_unk60));

    // Reset movement state (+0x74..+0x86)
    m_bIsMoving    = 1;
    m_moveTimer    = 0.0f;    // +0x78
    m_targetTimer  = 0.0f;    // +0x7C
    m_bHasTarget   = 0;       // +0x84
    m_bTargetReached = 0;     // +0x85
    m_bPathBlocked = 0;       // +0x86

    // Zero path data (+0xD0, 16 bytes)
    memset(m_pathData, 0, sizeof(m_pathData));
}

/**
 * pongMover::CalcInitMatrix @ 0x820CAC78 | size: 0x29C
 * 
 * Calculates initial transformation matrix for the mover.
 * Uses creature position, orientation, and player data.
 */
void pongMover::CalcInitMatrix(float* outMatrix, pongMover* mover, void* creatureData) {
    // Start with identity matrix
    memcpy(outMatrix, g_identityMatrixRef, 64);
    
    // Check if we should use remote rest matrix
    uint8_t flags = *(uint8_t*)((char*)mover + 64);
    bool useRemoteMatrix = (flags & 0x10) != 0;
    
    if (useRemoteMatrix) {
        // Check if remote matrix is valid (identity check)
        float* remoteMatrixObj = (float*)((char*)mover + 144);
        bool isIdentity = IsMatrixIdentity(remoteMatrixObj);
        
        // If not identity, copy remote rest matrix
        if (!isIdentity) {
            float* remoteMatrix = (float*)((char*)mover + 192);
            
            nop_8240E6D0("pongMover::CalcInitMatrix() - setting rest mtx to remote rest mtx [%f,%f,%f]",
                        remoteMatrix[0], remoteMatrix[1], remoteMatrix[2]);
            
            // Copy 4x4 matrix (64 bytes)
            memcpy(outMatrix, (char*)mover + 144, 64);
            
            // Get player and creature data
            void* player = *(void**)0x825F1238;  // g_currentPlayer
            uint32_t creatureIndex = *(uint32_t*)((char*)mover + 68);
            
            void* playerCreature = (void*)((char*)player + creatureIndex);
            void* creatureInfo = pg_9C00_g(player, creatureIndex);
            void* creatureStats = *(void**)((char*)creatureInfo + 44);
            
            // Check if mirrored
            uint8_t playerMirror = *(uint8_t*)((char*)creatureStats + 260);
            uint8_t creatureMirror = *(uint8_t*)((char*)playerCreature + 64);
            
            if (playerMirror != creatureMirror) {
                // Apply mirror transformation
                // Negate X-axis components (flip horizontally)
                float* row0 = &outMatrix[0];
                float* row2 = &outMatrix[8];
                
                float* mirrorVec = (float*)0x82606750;  // g_mirrorVector
                
                row0[0] = mirrorVec[0] - row0[0];
                row0[1] = mirrorVec[1] - row0[1];
                row0[2] = mirrorVec[2] - row0[2];
                row0[3] = mirrorVec[3] - row0[3];
                
                row2[0] = mirrorVec[0] - row2[0];
                row2[1] = mirrorVec[1] - row2[1];
                row2[2] = mirrorVec[2] - row2[2];
                row2[3] = mirrorVec[3] - row2[3];
            }
            
            return;
        }
    }
    
    // Check if using alternate matrix source
    bool useAlternate = *(uint8_t*)((char*)creatureData + 64) != 0;
    
    if (useAlternate) {
        // Check global flag
        bool globalFlag = *(uint8_t*)0x82606327 != 0;
        
        if (globalFlag) {
            // Use global alternate matrix @ 0x82606990
            memcpy(outMatrix, (void*)0x82606990, 64);
            return;
        }
    }
    
    // Default: calculate from player position
    void* player = *(void**)0x825F1238;
    uint32_t creatureIndex = *(uint32_t*)((char*)mover + 68);
    
    float tempMatrix1[16];
    float tempMatrix2[16];
    
    pongPlayer_9CD0_g(player, creatureIndex, tempMatrix1, tempMatrix2);
    
    void* creatureInfo = pg_9C00_g(player, creatureIndex);
    void* creatureStats = *(void**)((char*)creatureInfo + 44);
    
    // Get height offset
    float heightOffset = *(float*)((char*)creatureStats + 252);
    outMatrix[13] = heightOffset;  // Y position
    
    // Check if creature is active
    uint32_t creaturePtr = *(uint32_t*)((char*)mover + 68);
    void* activeCreature = (void*)((char*)player + creaturePtr);
    bool isActive = *(uint8_t*)((char*)activeCreature + 64) != 0;
    
    if (isActive) {
        // Use calculated position from temp matrix
        float calcHeight = tempMatrix2[2];  // Z component
        outMatrix[14] = calcHeight;
        
        // Additional positioning logic based on creature type...
        // (Simplified - full logic involves more complex calculations)
    }
}


////////////////////////////////////////////////////////////////////////////////
// Matrix Comparison Utilities
////////////////////////////////////////////////////////////////////////////////

/**
 * IsMatrixIdentity @ 0x820C3C70 | size: 0xD4
 * 
 * Checks if a 4x4 transformation matrix is the identity matrix.
 * 
 * Uses SIMD-style comparison with masking to allow ignoring specific
 * components (e.g., translation values) during the check.
 * 
 * @param matrix Pointer to 64-byte (4x4) matrix to check
 * @return true if matrix matches identity (after masking), false otherwise
 */
bool IsMatrixIdentity(const float* matrix) {
    const uint32_t* matrixBits = (const uint32_t*)matrix;
    const uint32_t* refBits = (const uint32_t*)g_identityMatrixRef;
    const uint32_t* mask = g_vectorComparisonMask;
    
    // Compare all 16 floats (4 vectors of 4 floats each)
    // Apply mask to both matrices before comparison
    for (int i = 0; i < 16; i++) {
        uint32_t maskedMatrix = matrixBits[i] & mask[i % 4];
        uint32_t maskedRef = refBits[i] & mask[i % 4];
        
        if (maskedMatrix != maskedRef) {
            return false;  // Mismatch found
        }
    }
    
    return true;  // All components match
}

// Alias for compatibility with existing code
uint8_t game_3C70(const float* matrix) {
    return IsMatrixIdentity(matrix) ? 1 : 0;
}

/**
 * pongBlendLookAtDriver::CalculateBlendFactor @ 0x820D0CA0 | size: 0x2C
 * [vtable slot 18]
 *
 * Calculates a blend factor for look-at animation based on the difference
 * between the current look-at angle and a reference angle.
 *
 * Formula: abs(currentAngle - refAngle) * multiplier * blendWeight
 *
 * @return Blend factor (float in f1)
 */
float pongBlendLookAtDriver::CalculateBlendFactor() {
    // Load current look-at angle from +224
    float currentAngle = *(float*)((char*)this + 224);
    
    // Load blend weight from +16
    float blendWeight = *(float*)((char*)this + 16);
    
    // Load reference angle from global @ 0x8202D10C
    extern const float g_lookAtRefAngle;  // @ 0x8202D10C
    float refAngle = g_lookAtRefAngle;
    
    // Calculate angle difference
    float angleDiff = currentAngle - refAngle;
    
    // Take absolute value
    float absAngleDiff = (angleDiff < 0.0f) ? -angleDiff : angleDiff;
    
    // Load multiplier from global @ 0x8202D184
    extern const float g_lookAtMultiplier;  // @ 0x8202D184
    float multiplier = g_lookAtMultiplier;
    
    // Calculate final blend factor
    float blendFactor = absAngleDiff * multiplier * blendWeight;
    
    return blendFactor;
}


////////////////////////////////////////////////////////////////////////////////
// LocomotionStateAnim - Animation State Management
////////////////////////////////////////////////////////////////////////////////

/**
 * LocomotionStateAnim::FindAnimationByID @ 0x8224BEF8 | size: 0x9C
 * 
 * Binary search through the animation array to find an animation entry
 * matching the given 24-bit ID (constructed from high 16 bits + low 8 bits).
 * 
 * The animation array is sorted by ID, allowing efficient O(log n) lookup.
 * Each entry stores its ID as: (byte_at_offset_5 << 16) | uint16_at_offset_6
 * 
 * @param animID_high Upper 16 bits of animation ID
 * @param animID_low Lower 8 bits of animation ID (typically 0)
 * @return Pointer to animation entry if found, nullptr otherwise
 */
void* LocomotionStateAnim::FindAnimationByID(uint16_t animID_high, uint8_t animID_low) {
    // Get animation count from offset +12
    uint16_t animCount = *(uint16_t*)((char*)this + 12);
    
    if (animCount == 0) {
        return nullptr;
    }
    
    // Get animation array pointer from offset +8
    void** animArray = *(void***)((char*)this + 8);
    
    // Construct 24-bit search key: (animID_high << 16) | animID_low
    uint32_t searchKey = ((uint32_t)animID_high << 16) | animID_low;
    
    // Binary search
    int low = 0;
    int high = animCount - 1;
    
    while (low <= high) {
        int mid = (low + high) / 2;
        
        // Get animation entry at mid index
        void* entry = animArray[mid];
        
        // Extract 24-bit ID from entry: (byte_at_5 << 16) | uint16_at_6
        uint8_t idHigh = *(uint8_t*)((char*)entry + 5);
        uint16_t idLow = *(uint16_t*)((char*)entry + 6);
        uint32_t entryKey = ((uint32_t)idHigh << 16) | idLow;
        
        if (entryKey == searchKey) {
            // Found exact match
            return entry;
        } else if (entryKey > searchKey) {
            // Search lower half
            high = mid - 1;
        } else {
            // Search upper half
            low = mid + 1;
        }
    }
    
    // Not found
    return nullptr;
}

/**
 * LocomotionStateAnim::SetupAnimationPair @ 0x8224C418 | size: 0x90
 * 
 * Finds two animations by their IDs and calls virtual methods on them
 * to set up animation blending or transition. This is commonly used for
 * locomotion state transitions where two animations need to be blended.
 * 
 * The function searches for both animations, and if both are found,
 * calls their respective virtual methods with the provided parameter.
 * 
 * @param animID1 First animation ID (upper 16 bits)
 * @param animID2 Second animation ID (upper 16 bits)
 * @param param Parameter to pass to animation virtual methods
 * @return true if both animations found and configured, false otherwise
 */
bool LocomotionStateAnim::SetupAnimationPair(uint16_t animID1, uint16_t animID2, void* param) {
    // Find first animation (ID1, 0)
    void* anim1 = FindAnimationByID(animID1, 0);
    
    // Find second animation (ID2, 0)
    void* anim2 = FindAnimationByID(animID2, 0);
    
    // Both animations must exist
    if (!anim1 || !anim2) {
        return false;
    }
    
    // Call virtual method on first animation with offset parameter
    // vtable[17] - likely SetupBlendSource or similar
    void** vtable1 = *(void***)anim1;
    typedef void (*AnimSetupFunc)(void*, void*);
    AnimSetupFunc setupFunc1 = (AnimSetupFunc)vtable1[17];
    setupFunc1(anim1, (char*)param + 16);
    
    // Call virtual method on second animation with base parameter
    // vtable[18] - likely SetupBlendTarget or similar
    void** vtable2 = *(void***)anim2;
    typedef void (*AnimSetupFunc2)(void*, void*);
    AnimSetupFunc2 setupFunc2 = (AnimSetupFunc2)vtable2[18];
    setupFunc2(anim2, param);
    
    return true;
}

/**
 * LocomotionState::ValidateAnimationTiming @ 0x820DF220 | size: 0x6C
 * 
 * Validates that the animation timing for the given animation indices is valid (>= 0.0f).
 * If the timing is negative, it defaults both indices to 11 (idle/default animation).
 * 
 * This function ensures that the locomotion system doesn't try to use animations
 * with invalid timing data, which could cause rendering or blending issues.
 * 
 * @param animSetIndex Index into the animation set array (r4)
 * @param animIndex Index of the specific animation within the set (r5)
 * @param outTiming Output parameter to receive the validated animation timing (f1)
 */
void LocomotionState::ValidateAnimationTiming(uint32_t& animSetIndex, uint32_t& animIndex, float& outTiming) {
    // Threshold for valid timing (must be >= 0.0f)
    const float TIMING_THRESHOLD = 0.0f;
    const uint32_t DEFAULT_ANIM_INDEX = 11;  // Default/idle animation
    
    // Get animation set array pointer from offset 0
    void*** animSetArray = *(void****)this;
    
    // Loop until we find valid timing
    while (true) {
        // Index into animation set array (each pointer is 4 bytes, but indexed by 8)
        // This suggests the array might be storing pairs or have padding
        void** animSet = (void**)((char*)animSetArray + (animSetIndex * 8));
        
        // Get specific animation entry (28 bytes per entry)
        char* animEntry = (char*)(*animSet) + (animIndex * 28);
        
        // Get timing field at offset +24
        float timing = *(float*)(animEntry + 24);
        
        // Check if timing is valid
        if (timing >= TIMING_THRESHOLD) {
            // Valid timing found, return it
            outTiming = timing;
            return;
        }
        
        // Invalid timing - need to use default animation
        if (animIndex != DEFAULT_ANIM_INDEX) {
            // First try: set animIndex to default
            animIndex = DEFAULT_ANIM_INDEX;
        } else if (animSetIndex != DEFAULT_ANIM_INDEX) {
            // Second try: set animSetIndex to default
            animSetIndex = DEFAULT_ANIM_INDEX;
        } else {
            // Both already at default - this shouldn't happen, but exit to prevent infinite loop
            // Load the timing anyway (even if invalid)
            outTiming = timing;
            return;
        }
        
        // Loop continues with updated indices
    }
}

/**
 * LocomotionStateAnim::CopyAnimationData @ 0x8214C8F8 | size: 0xD0
 * 
 * Copies animation transformation data between two internal buffers.
 * This is used during locomotion state transitions to transfer animation
 * pose data from one state to another.
 * 
 * The function handles two cases:
 * 1. If a helper object exists, delegates to pongCreatureInst helper
 * 2. Otherwise, manually copies 64 bytes of transformation data using SIMD
 * 
 * After the initial copy, processes additional animations from an array.
 */
void LocomotionStateAnim::CopyAnimationData() {
    // Get source and destination buffer pointers
    void* srcBuffer = *(void**)((char*)this + 12);   // +0x0C
    void* dstBuffer = *(void**)((char*)this + 20);   // +0x14
    
    // Check if we have a helper object for the copy operation
    void* helperObj = *(void**)((char*)this + 8);    // +0x08
    
    if (helperObj != nullptr) {
        // Use pongCreatureInst helper to perform the copy
        // This likely handles more complex transformation logic
        extern void pongCreatureInst_EDC0_g(void* dst, void* src, void* helper);
        pongCreatureInst_EDC0_g(dstBuffer, srcBuffer, helperObj);
    } else {
        // Manual copy: 64 bytes (4 x 16-byte vectors) of transformation data
        // This is likely a 4x4 transformation matrix (16 floats = 64 bytes)
        
        // Copy using 16-byte aligned SIMD operations
        // Vector 0: bytes 0-15
        memcpy(dstBuffer, srcBuffer, 16);
        
        // Vector 1: bytes 16-31
        memcpy((char*)dstBuffer + 16, (char*)srcBuffer + 16, 16);
        
        // Vector 2: bytes 32-47
        memcpy((char*)dstBuffer + 32, (char*)srcBuffer + 32, 16);
        
        // Vector 3: bytes 48-63
        memcpy((char*)dstBuffer + 48, (char*)srcBuffer + 48, 16);
    }
    
    // Process additional animations from the animation array
    // Get animation info structure from offset +4
    void* animInfo = *(void**)((char*)this + 4);
    
    if (animInfo == nullptr) {
        return;
    }
    
    // Get animation count from offset +12 in animInfo
    uint16_t animCount = *(uint16_t*)((char*)animInfo + 12);
    
    // Get animation array pointer from offset +4 in animInfo
    uint32_t* animArray = *(uint32_t**)((char*)animInfo + 4);
    
    if (animCount <= 1) {
        return;  // No additional animations to process
    }
    
    // Process each additional animation (skip first one, already handled)
    for (int i = 0; i < animCount - 1; i++) {
        // Get animation index from array
        uint32_t animIndex = animArray[i];
        
        // Calculate buffer offsets for this animation
        // Each animation uses 64-byte destination blocks and 80-byte source blocks
        uint32_t dstOffset = 64 + (i * 64);   // Start at +64, increment by 64
        uint32_t srcOffset = 80 + (i * 80);   // Start at +80, increment by 80
        
        // Calculate indexed destination offset: animIndex * 64
        uint32_t indexedDstOffset = animIndex * 64;
        
        // Copy animation data using helper
        void* dstPtr = (char*)dstBuffer + dstOffset;
        void* srcPtr = (char*)srcBuffer + srcOffset;
        void* indexedDstPtr = (char*)dstBuffer + indexedDstOffset;
        
        extern void pongCreatureInst_EDC0_g(void* dst, void* src, void* indexed);
        pongCreatureInst_EDC0_g(dstPtr, srcPtr, indexedDstPtr);
    }
}
