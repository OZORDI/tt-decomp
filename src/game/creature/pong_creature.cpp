

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

/**
 * LocomotionStateAnim::ProcessAnimationList @ 0x82380540 | size: 0xD0
 * 
 * Processes a list of animations by applying them to the locomotion state.
 * This function iterates through all animations in the provided list and
 * applies either a simple or complex animation processing function based
 * on the animation's flags.
 * 
 * The function first sets up the animation state, then determines which
 * processing method to use (simple vs complex based on bit 0 of flags),
 * and finally applies the selected method to each animation in the array.
 * 
 * @param animationList Pointer to animation list structure containing animations to process
 */
void LocomotionStateAnim::ProcessAnimationList(void* animationList) {
    if (!animationList) {
        return;
    }
    
    // Set up animation state with current parameters
    void* animContainer = *(void**)((char*)this + 36);  // Animation container at +36
    void* containerData = *(void**)((char*)animContainer + 0);
    void* containerParams = *(void**)((char*)animContainer + 4);
    float currentTime = *(float*)((char*)this + 8);
    
    // Call setup function to prepare animations
    LocomotionStateAnim_88E0_g(containerData, containerParams, currentTime, 1, 0);
    
    // Determine which processing function to use based on animation flags
    void* animListData = *(void**)((char*)animationList + 4);
    uint32_t animFlags = *(uint32_t*)((char*)animListData + 20);
    
    // Function pointer for animation processing
    typedef void (*AnimProcessFunc)(void*, void*, float);
    AnimProcessFunc processFunc;
    
    // Check bit 0 of flags to determine processing method
    bool useSimpleProcessing = (animFlags & 0x1) == 0;
    
    if (useSimpleProcessing) {
        processFunc = (AnimProcessFunc)0x8224C128;  // LocomotionStateAnim_C128_g - simple processing
    } else {
        processFunc = (AnimProcessFunc)0x8224C288;  // LocomotionStateAnim_C288_g - complex processing
    }
    
    // Get animation array info
    uint16_t animCount = *(uint16_t*)((char*)animListData + 12);
    
    if (animCount > 0) {
        void** animArray = *(void***)((char*)animListData + 8);
        
        // Load blend weight constant (typically 1.0f for full blend)
        extern const float g_animBlendWeight;  // @ 0x8202D110
        float blendWeight = g_animBlendWeight;
        
        // Process each animation in the array
        for (uint16_t i = 0; i < animCount; i++) {
            void* animation = animArray[i];
            processFunc(animation, animationList, blendWeight);
        }
    }
    
    // Finalize animation processing
    LocomotionStateAnim_C8F8_g(animationList);
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim — Animation State Management (Batch Implementation)
// ─────────────────────────────────────────────────────────────────────────────

// External references
extern void LocomotionState_vfn_5(void* state);
extern void rage_free_00C0(void* ptr);
extern void rage_FF70(void* obj);
extern void ph_59C8(void* loader, const char* name, int flag);
extern int util_5A70(void* loader, void* outBuffer, int maxLen, int param1, int param2);
extern void nop_8240E6D0(const char* fmt, ...);
extern void phBoundCapsule_5138_g(void* capsule, void* params, void* data);
extern void LocomotionStateAnim_8278_g(void* state, void* animData);

// Global constants
extern const float g_floatZero;      // @ 0x8202D110
extern const float g_floatOne;       // @ 0x8202D108

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::~LocomotionStateAnim()  [vtable slot 0 @ 0x820DFF20]
// Destructor - cleans up animation state resources
// ─────────────────────────────────────────────────────────────────────────────
void LocomotionStateAnim::~LocomotionStateAnim() {
    // Call parent destructor
    rage_FF70(this);
    
    // Note: The 'flags' parameter (r4) determines if memory should be freed
    // This is handled by the scalar destructor (vfn_1), not here
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::Reset()  [vtable slot 2 @ 0x820E0178]
// Resets animation state to initial values
// ─────────────────────────────────────────────────────────────────────────────
void LocomotionStateAnim::Reset() {
    // Clear animation state fields
    *(uint32_t*)((char*)this + 60) = 0;   // Clear state flags
    *(uint32_t*)((char*)this + 320) = 0;  // Clear animation pointer
    *(uint32_t*)((char*)this + 324) = 0;  // Clear animation data
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::GetAnimationProgress()  [vtable slot 10 @ 0x820E0FB0]
// Returns normalized animation progress (0.0 to 1.0)
// ─────────────────────────────────────────────────────────────────────────────
float LocomotionStateAnim::GetAnimationProgress() {
    // Get current animation data
    void* animData = *(void**)((char*)this + 320);
    
    // Get animation timing values
    float currentTime = *(float*)((char*)this + 184);
    float duration = *(float*)((char*)this + 40);
    float animEndTime = *(float*)((char*)animData + 12);
    
    // Calculate progress: (endTime - currentTime) / duration
    float remaining = animEndTime - currentTime;
    float progress = remaining / duration;
    
    return progress;
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::GetAnimationDuration()  [vtable slot 11 @ 0x820E0FD0]
// Returns total animation duration in seconds
// ─────────────────────────────────────────────────────────────────────────────
float LocomotionStateAnim::GetAnimationDuration() {
    // Get current animation data
    void* animData = *(void**)((char*)this + 320);
    
    // Get timing values
    float duration = *(float*)((char*)this + 40);
    float animEndTime = *(float*)((char*)animData + 12);
    
    // Calculate total duration: endTime / duration
    float totalDuration = animEndTime / duration;
    
    return totalDuration;
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::IsAnimationComplete()  [vtable slot 15 @ 0x820DF208]
// Returns true if animation has finished playing
// ─────────────────────────────────────────────────────────────────────────────
bool LocomotionStateAnim::IsAnimationComplete() {
    // Check completion flag at offset +328
    uint8_t isComplete = *(uint8_t*)((char*)this + 328);
    
    // Return inverted: 0 means complete, non-zero means still playing
    // This is a cntlzw + rlwinm pattern: count leading zeros and extract bit
    return (isComplete == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::Initialize()  [vtable slot 5 @ 0x820E0EE8]
// Initializes animation state with default blend vectors
// ─────────────────────────────────────────────────────────────────────────────
void LocomotionStateAnim::Initialize() {
    // Call parent initialization
    LocomotionState_vfn_5(this);
    
    // Load global constants
    const float zero = g_floatZero;   // 0.0f
    const float one = g_floatOne;     // 1.0f
    
    // Initialize first blend vector at offset +192 (12 floats)
    float* blendVec1 = (float*)((char*)this + 192);
    blendVec1[0] = one;   // X scale
    blendVec1[1] = zero;  // Y offset
    blendVec1[2] = zero;  // Z offset
    blendVec1[4] = zero;  // Rotation X
    blendVec1[5] = one;   // Rotation Y scale
    blendVec1[6] = zero;  // Rotation Z
    blendVec1[8] = zero;  // Translation X
    blendVec1[9] = zero;  // Translation Y
    blendVec1[10] = one;  // Translation Z scale
    
    // Clear 16-byte aligned buffer at +240
    uint8_t* buffer1 = (uint8_t*)((char*)this + 240);
    for (int i = 0; i < 16; i++) {
        buffer1[i] = 0;
    }
    
    // Initialize second blend vector at offset +256 (12 floats)
    float* blendVec2 = (float*)((char*)this + 256);
    blendVec2[0] = one;   // X scale
    blendVec2[1] = zero;  // Y offset
    blendVec2[2] = zero;  // Z offset
    blendVec2[4] = zero;  // Rotation X
    blendVec2[5] = one;   // Rotation Y scale
    blendVec2[6] = zero;  // Rotation Z
    blendVec2[8] = zero;  // Translation X
    blendVec2[9] = zero;  // Translation Y
    blendVec2[10] = one;  // Translation Z scale
    
    // Clear 16-byte aligned buffer at +304
    uint8_t* buffer2 = (uint8_t*)((char*)this + 304);
    for (int i = 0; i < 16; i++) {
        buffer2[i] = 0;
    }
    
    // Initialize state fields
    *(float*)((char*)this + 184) = zero;   // Current time
    *(uint32_t*)((char*)this + 324) = 0;   // Animation data pointer
    *(uint32_t*)((char*)this + 320) = 0;   // Animation object pointer
    *(float*)((char*)this + 332) = zero;   // Blend weight
    *(uint8_t*)((char*)this + 328) = 1;    // Mark as not complete
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::CalculateBlendFactor()  @ 0x820C1910
// Calculates animation blend factor based on timing parameters
// ─────────────────────────────────────────────────────────────────────────────
float LocomotionStateAnim::CalculateBlendFactor(void* timingData) {
    // Load global constants
    const float zero = g_floatZero;
    const float one = g_floatOne;
    
    // Get timing parameters from input data
    float targetTime = *(float*)((char*)timingData + 20);
    float blendInTime = *(float*)((char*)timingData + 12);
    float blendOutTime = *(float*)((char*)timingData + 8);
    
    // If at target time, return zero blend
    if (targetTime == one) {
        targetTime = zero;
    }
    
    // Calculate time until blend completes
    float timeUntilBlend = one - blendInTime;
    
    // Check if we're in the blend-out phase
    if (targetTime >= timeUntilBlend) {
        // In blend-out phase: use blend-out time
        blendOutTime = blendInTime;
        targetTime = one - targetTime;
    }
    
    // If past blend-out time, return full blend (1.0)
    if (targetTime >= blendOutTime) {
        return one;
    }
    
    // If before blend-in time, return zero blend
    if (targetTime <= one) {
        return targetTime;
    }
    
    // Calculate normalized blend factor
    float blendFactor = targetTime / blendOutTime;
    return blendFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::Load()  @ 0x820E0190
// Loads animation data from file
// ─────────────────────────────────────────────────────────────────────────────
bool LocomotionStateAnim::Load(void* parentState, void* fileLoader) {
    // Get animation duration from parent state
    float duration = 0.0f;
    {
        void** vtable = *(void***)fileLoader;
        typedef float (*GetDurationFunc)(void*);
        GetDurationFunc getDuration = (GetDurationFunc)vtable[5];
        duration = getDuration(fileLoader);
    }
    
    // Store duration at offset +12
    *(float*)((char*)this + 12) = duration;
    
    // Initialize animation set indices
    *(uint32_t*)((char*)this + 8) = 0;
    
    // Try to load primary animation set
    const char** animNames1 = (const char**)0x825C9B90;  // Global animation name table
    int animIndex1 = 0;
    
    for (int i = 0; i < 4; i++) {
        const char* animName = animNames1[i];
        uint8_t found = 0;
        
        // Call loader to check if animation exists
        ph_59C8(fileLoader, animName, 1);
        // Result in r3 (0 = not found, 1 = found)
        
        if (found == 0) {
            animIndex1++;
        } else {
            break;
        }
    }
    
    *(uint32_t*)((char*)this + 8) = animIndex1;
    
    // Initialize alternate animation set indices
    *(uint32_t*)((char*)this + 4) = 0;
    
    // Try to load alternate animation set
    const char** animNames2 = (const char**)0x825C9BA0;  // Alternate animation name table
    int animIndex2 = 0;
    
    for (int i = 0; i < 4; i++) {
        const char* animName = animNames2[i];
        uint8_t found = 0;
        
        ph_59C8(fileLoader, animName, 1);
        
        if (found == 0) {
            animIndex2++;
        } else {
            break;
        }
    }
    
    *(uint32_t*)((char*)this + 4) = animIndex2;
    
    // Load animation file name
    char fileNameBuffer[256];
    int result = util_5A70(fileLoader, fileNameBuffer, 255, 10, 1);
    
    if (result == 0) {
        // Failed to read file name
        return false;
    }
    
    // Get animation manager from parent state
    void* stateData = *(void**)((char*)parentState + 24);
    void* animManager = *(void**)((char*)stateData + 96);
    
    // Load animation from file
    void** animMgrVtable = *(void***)animManager;
    typedef void* (*LoadAnimFunc)(void*, const char*);
    LoadAnimFunc loadAnim = (LoadAnimFunc)animMgrVtable[4];
    void* loadedAnim = loadAnim(animManager, fileNameBuffer);
    
    if (loadedAnim == nullptr) {
        // Failed to load animation
        nop_8240E6D0("LocomotionStateAnim::Load - failed to load anim file '%s'", fileNameBuffer);
        return false;
    }
    
    // Store loaded animation pointer
    *(void**)((char*)this + 0) = loadedAnim;
    
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim::UpdateAnimationState()  @ 0x82380268
// Updates animation state and handles state transitions
// ─────────────────────────────────────────────────────────────────────────────
void LocomotionStateAnim::UpdateAnimationState(void* outputState) {
    const float zero = g_floatZero;
    
    // Get animation data pointer
    void* animData = *(void**)((char*)this + 36);
    
    if (animData == nullptr) {
        // No animation data - check if we should idle
        uint8_t flag1 = *(uint8_t*)((char*)this + 0);
        uint8_t flag2 = *(uint8_t*)((char*)this + 1);
        
        if (flag1 != 0 || flag2 != 0) {
            // Clear state and return
            *(float*)((char*)this + 8) = zero;
            *(uint32_t*)((char*)this + 20) = 0;
            return;
        }
        
        // Check frame counter
        uint32_t frameCount = *(uint32_t*)((char*)this + 28);
        if (frameCount != 0) {
            uint32_t currentFrame = *(uint32_t*)((char*)this + 24);
            currentFrame++;
            *(uint32_t*)((char*)this + 24) = currentFrame;
            
            if (currentFrame < frameCount) {
                // Still counting frames
                goto cleanup;
            }
            
            // Frame count reached - trigger state change
            uint32_t currentState = *(uint32_t*)((char*)this + 4);
            if (currentState == 2) {
                // Transition to state 0
                *(uint32_t*)((char*)this + 4) = 0;
            }
        }
        
    cleanup:
        *(float*)((char*)this + 8) = zero;
        *(uint32_t*)((char*)this + 20) = 0;
        return;
    }
    
    // Animation data exists - process it
    void* animSubData = (void*)((char*)animData + 16);
    
    // Check if we should randomize timing
    uint8_t shouldRandomize = *(uint8_t*)((char*)animSubData + 12);
    
    if (shouldRandomize != 0) {
        // Randomize animation timing using RNG
        // This uses a linear congruential generator (LCG)
        uint32_t* rngState = (uint32_t*)0x825C9A48;  // Global RNG state
        
        float minTime = *(float*)((char*)animSubData + 80);
        float maxTime = *(float*)((char*)animSubData + 84);
        float timeRange = maxTime - minTime;
        
        // Generate random value: (state * 0x5CEFA7) & 0x7FFFFF
        uint64_t seed = rngState[0];
        uint64_t next = seed * 0x5CEFA7 + rngState[1];
        uint32_t randomBits = (uint32_t)(next & 0x7FFFFF);
        
        rngState[0] = (uint32_t)next;
        rngState[1] = (uint32_t)(next >> 32);
        
        // Convert to float and scale
        float randomFloat = (float)randomBits;
        float scaledRandom = timeRange * randomFloat * (1.0f / 8388608.0f);  // Divide by 2^23
        float randomTime = scaledRandom + minTime;
        
        *(float*)((char*)this + 16) = randomTime;
        
        // Generate second random value for blend weight
        seed = rngState[0];
        next = seed * 0x5CEFA7 + rngState[1];
        randomBits = (uint32_t)(next & 0x7FFFFF);
        
        rngState[0] = (uint32_t)next;
        rngState[1] = (uint32_t)(next >> 32);
        
        float blendRange = *(float*)((char*)animSubData + 16);
        randomFloat = (float)randomBits;
        float randomBlend = randomFloat * blendRange * (1.0f / 8388608.0f);
        
        *(float*)((char*)this + 12) = randomBlend;
        
    } else {
        // Use fixed timing
        float fixedTime = *(float*)((char*)animSubData + 16);
        *(float*)((char*)this + 12) = fixedTime;
    }
    
    // Update state based on timing
    float currentTime = *(float*)((char*)this + 12);
    
    if (currentTime < zero) {
        // Negative time - transition to state 2
        uint32_t state = *(uint32_t*)((char*)this + 4);
        if (state != 2) {
            *(uint32_t*)((char*)this + 4) = 2;
        }
    } else if (currentTime == zero) {
        // Zero time - transition to state 1
        uint32_t state = *(uint32_t*)((char*)this + 4);
        if (state != 1) {
            *(uint32_t*)((char*)this + 4) = 1;
        }
    } else {
        // Positive time - transition to state 0
        uint32_t state = *(uint32_t*)((char*)this + 4);
        if (state != 0) {
            *(uint32_t*)((char*)this + 4) = 0;
        }
    }
    
    // Check if we should use capsule collision
    uint8_t useCapsule = *(uint8_t*)((char*)animSubData + 48);
    uint8_t capsuleEnabled = *(uint8_t*)((char*)animSubData + 49);
    
    if (useCapsule != 0 && capsuleEnabled != 0) {
        // Copy capsule data and calculate collision
        float capsuleData[4];
        float* srcCapsule = (float*)((char*)animSubData + 64);
        float* dstCapsule = (float*)((char*)outputState + 48);
        
        // Copy 16 bytes (4 floats)
        for (int i = 0; i < 4; i++) {
            dstCapsule[i] = srcCapsule[i];
        }
        
        // Calculate capsule collision parameters
        float* capsuleEnd = (float*)((char*)animSubData + 80);
        float capsuleDiff[4];
        for (int i = 0; i < 4; i++) {
            capsuleDiff[i] = capsuleEnd[i] - dstCapsule[i];
        }
        
        // Call capsule collision function
        phBoundCapsule_5138_g(outputState, capsuleDiff, (void*)0x825C9B60);
        
    } else {
        // Use simple animation data
        void* simpleAnimData = (void*)((char*)animSubData + 32);
        LocomotionStateAnim_8278_g(outputState, simpleAnimData);
    }
    
    // Copy final capsule data to output
    float* finalCapsule = (float*)((char*)animSubData + 64);
    float* outputCapsule = (float*)((char*)outputState + 48);
    for (int i = 0; i < 4; i++) {
        outputCapsule[i] = finalCapsule[i];
    }
    
    // Clear timing fields
    *(float*)((char*)this + 8) = zero;
    *(uint32_t*)((char*)this + 20) = 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// pcrSwingData / pcrJunkSwingData — Swing Animation Data
// ═════════════════════════════════════════════════════════════════════════════

// External vtable pointers
extern void* g_pcrSwingDataVtable;      // @ 0x8202E9AC
extern void* g_pcrJunkSwingDataVtable;  // @ 0x8202EAF4

/**
 * pcrSwingData_ctor_B888 @ 0x820EB888 | size: 0x60
 *
 * Constructor for pcrJunkSwingData (derived from pcrSwingData).
 * Initializes swing animation data with optional frame adjustment.
 *
 * The constructor:
 * 1. Sets base class vtable (pcrSwingData)
 * 2. Zeros out state fields (+4, +8, +12)
 * 3. If frame pointer (+16) is set, adjusts it based on segment data
 * 4. Sets derived class vtable (pcrJunkSwingData)
 *
 * Structure layout (inferred):
 *   +0   vtable pointer
 *   +4   state field 1 (zeroed)
 *   +8   state field 2 (zeroed)
 *   +12  state field 3 (zeroed)
 *   +16  frame pointer (adjusted if non-null)
 *
 * @param this - Pointer to pcrJunkSwingData object being constructed
 * @param segmentData - Pointer to segment/array data structure for frame calculation
 */
void pcrSwingData_ctor_B888(void* thisPtr, void* segmentData) {
    uint8_t* obj = (uint8_t*)thisPtr;
    uint8_t* segments = (uint8_t*)segmentData;
    
    // Set base class vtable (pcrSwingData)
    *(void**)(obj + 0) = (void*)0x8202E9AC;  // g_pcrSwingDataVtable
    
    // Zero out state fields
    *(uint32_t*)(obj + 4) = 0;
    *(uint32_t*)(obj + 8) = 0;
    *(uint32_t*)(obj + 12) = 0;
    
    // Check if frame pointer is set
    uint32_t framePtr = *(uint32_t*)(obj + 16);
    
    if (framePtr != 0) {
        // Load segment metadata
        uint32_t segmentStart = *(uint32_t*)(segments + 4);
        uint32_t segmentStride = *(uint32_t*)(segments + 76);
        
        // Calculate frame offset from segment start
        uint32_t frameOffset = framePtr - segmentStart;
        
        // Compute segment index (frameOffset / segmentStride)
        // Note: Division by zero trap is present in original code
        uint32_t segmentIndex = frameOffset / segmentStride;
        
        // Compute array index: segmentIndex + 2
        uint32_t arrayIndex = segmentIndex + 2;
        
        // Load segment base offset from array
        uint32_t segmentBaseOffset = *(uint32_t*)(segments + (arrayIndex * 4));
        
        // Adjust frame pointer by adding segment base
        uint32_t adjustedFramePtr = segmentBaseOffset + framePtr;
        
        // Store adjusted frame pointer
        *(uint32_t*)(obj + 16) = adjustedFramePtr;
    }
    
    // Set derived class vtable (pcrJunkSwingData)
    *(void**)(obj + 0) = (void*)0x8202EAF4;  // g_pcrJunkSwingDataVtable
}
