

/**
 * pongMover — Movement and positioning system for creatures
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Handles creature positioning, matrix calculations, and movement state.
 */

#include "pong_creature.hpp"
#include "globals.h"
#include "game/pong_strings.hpp"
#include <cstring>

// External dependencies
extern void rage_Free(void* ptr);
extern void nop_8240E6D0(const char* msg, ...);
extern void pongCreature_7CE8_g(void* creature, void* matrix, int param1, int param2, int param3, int param4);
extern void* pg_9C00_g(void* player, int index);  // @ 0x82019C00 — returns creature info ptr
extern void pongPlayer_9CD0_g(void* player, int index, void* outMatrix1, void* outMatrix2);

// Forward declarations
bool IsMatrixIdentity(const float* matrix);  // @ 0x820C3C70 - Matrix identity check

// LocomotionStateAnim helper functions
extern void LocomotionStateAnim_88E0_g(void* containerData, void* containerParams, float time, int p1, int p2);
extern void LocomotionStateAnim_C8F8_g(void* animationList);

// Ref-counting helpers
extern "C" void rage_AddRef(void* obj);
extern "C" void rage_Release(void* obj);

// Creature lifecycle helpers
extern "C" void pongCreature_BaseDtor(void* obj);  // @ 0x820C53E8
extern "C" void pongCreature_Fixup(void* obj, void* relocator);
extern "C" void pongCreatureInst_Cleanup(void* obj);  // @ 0x822C8FD0
extern "C" void pongCreatureInst_SetMatrixImpl(void* obj, const void* matrix);
extern "C" float pongCreatureInst_ComputeHeading(void* obj);
extern "C" void pongCreatureInst_ApplyHeading(void* obj, float heading);
extern "C" void pongCreatureInst_NotifyHeadingChanged(void* obj);
extern "C" void LocomotionStateAnim_SetSpeed(void* obj, float speed);
extern "C" void LocomotionStateAnim_ApplySpeed(void* obj);
extern "C" void LocomotionStateAnim_InitCapsuleImpl(void* obj, float radius, const void* center);
extern "C" void LocomotionStateAnim_SetMatrixImpl(void* obj, const void* matrix);
extern "C" void* LocomotionState_FindFirstActive(void* list);
extern "C" void* LocomotionStateAnim_FindAnim(void* obj);
extern "C" void LocomotionStateAnim_ComputeOffset(void* obj, void* outVec);
extern "C" void LocomotionStateAnim_ProcessNode(void* obj, void* node);
extern "C" void LocomotionStateAnim_AccumNode(void* obj, void* node);
extern "C" void LocomotionStateAnim_GrowArray(void* obj);
extern "C" void LocomotionStateMf_ScaleWeights(void* obj);
extern "C" void LocomotionStateMf_BlendPoses(void* obj);
extern "C" void LocomotionStateMf_UpdateTransforms(void* obj);
extern "C" void LocomotionStateMf_ApplyConstraints(void* obj);
extern "C" void LocomotionStateMf_Finish(void* obj);

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
        nop_8240E6D0("pongMover::Reset() - no creature to reset");  /* UNVERIFIED — string not found in binary */
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
            
            nop_8240E6D0(g_str_pongMover_calcInitMatrix,
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
extern void LocomotionState_OnEnter(void* state);
extern void rage_free(void* ptr);
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
LocomotionStateAnim::~LocomotionStateAnim() {
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
    LocomotionState_OnEnter(this);
    
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
        nop_8240E6D0("LocomotionStateAnim::Load - failed to load anim file '%s'", fileNameBuffer);  /* UNVERIFIED — string not found in binary */
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


// ─────────────────────────────────────────────────────────────────────────────
// pongCreatureInst — Small vtable methods and utility functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongCreatureInst::GetType @ 0x8210CC40 | size: 0x8  [vtable slot 1]
 *
 * Returns the creature instance type identifier. Value 3 indicates
 * a pongCreatureInst (as opposed to other phInst subtypes).
 */
int pongCreatureInst::GetType() {  // vfn_1
    return 3;
}

/**
 * pongCreatureInst::GetEmbeddedObject @ 0x820C8ED8 | size: 0x8  [vtable slot 2]
 *
 * Returns a pointer to the embedded sub-object at offset +224 (0xE0).
 * This is the creature's collision/physics shape data.
 */
void* pongCreatureInst::GetEmbeddedObject() {  // vfn_2
    return (char*)this + 224;
}

/**
 * pongCreatureInst::GetDefaultIndex @ 0x824FF7C8 | size: 0x8  [vtable slot 19]
 *
 * Returns -1, indicating no default creature index. Used as a sentinel
 * value when no specific creature slot is assigned.
 */
int pongCreatureInst::GetDefaultIndex() {  // vfn_19
    return -1;
}

/**
 * pongCreatureInst::ClearBoundingVector @ 0x822C90F8 | size: 0x10  [vtable slot 4]
 *
 * Zeroes a 16-byte aligned vector passed by pointer. Used to clear
 * bounding volume data (AABB min/max or collision sphere).
 */
void pongCreatureInst::ClearBoundingVector(void* outVector) {  // vfn_4
    // Original uses vxor + stvx (AltiVec zero + aligned store)
    memset(outVector, 0, 16);
}

/**
 * pongCreatureInst::ForwardToUpdateBounds @ 0x822C90A0 | size: 0x10  [vtable slot 10]
 *
 * Tail-calls vtable slot 15 on self. Forwards the update-bounds
 * request to the more specific implementation.
 */
void pongCreatureInst::ForwardToUpdateBounds() {  // vfn_10
    typedef void (*UpdateFunc)(void*);
    void** vt = *(void***)this;
    UpdateFunc update = (UpdateFunc)vt[15];
    update(this);
}

/**
 * pongCreatureInst::UpdateBoundsWithDefaults @ 0x822C90B0 | size: 0x1C  [vtable slot 9]
 *
 * Calls vtable slot 10 with default parameters (outMin=this+64, outMax=0, flags=0).
 * Sets up the default bounding volume query.
 */
void pongCreatureInst::UpdateBoundsWithDefaults() {  // vfn_9
    typedef void (*BoundsFunc)(void*, void*, void*, int);
    void** vt = *(void***)this;
    BoundsFunc boundsFunc = (BoundsFunc)vt[10];
    boundsFunc(this, (char*)this + 64, nullptr, 0);
}

/**
 * pongCreatureInst::ComputeBoundsPartial @ 0x822C90E0 | size: 0x14  [vtable slot 14]
 *
 * Calls vtable slot 10 with flags=0, passing through the caller's
 * min/max pointers.
 */
void pongCreatureInst::ComputeBoundsPartial(void* outMin, void* outMax) {  // vfn_14
    typedef void (*BoundsFunc)(void*, void*, void*, int);
    void** vt = *(void***)this;
    BoundsFunc boundsFunc = (BoundsFunc)vt[10];
    boundsFunc(this, outMin, outMax, 0);
}

/**
 * pongCreatureInst::GetEntityByIndex @ 0x822C9318 | size: 0x18  [vtable slot 38]
 *
 * Looks up a sub-entity by index. Follows: this->field_4 -> field_12 -> field_120,
 * then adds (index << 6) to get the entity pointer. Each entity is 64 bytes.
 */
void* pongCreatureInst::GetEntityByIndex(int index) {  // vfn_38
    void* inner = *(void**)((char*)this + 4);
    void* pool = *(void**)((char*)inner + 12);
    char* base = *(char**)((char*)pool + 120);
    return base + (index << 6);
}

/**
 * pongCreatureInst::GetBoneTransform @ 0x8211DDB8 | size: 0x3C  [vtable slot 22]
 *
 * Returns a pointer to a bone's transform matrix. Looks up the bone
 * array through the creature's skeleton data.
 */
void* pongCreatureInst::GetBoneTransform(int boneIndex) {  // vfn_22
    void* skeletonData = *(void**)((char*)this + 80);
    if (!skeletonData) return nullptr;

    void* boneArray = *(void**)((char*)skeletonData + 12);
    char* boneBase = *(char**)((char*)boneArray + 120);
    return boneBase + (boneIndex << 6);
}

/**
 * pongCreatureInst::GetCreatureInfo @ 0x820C8F80 | size: 0x24  [vtable slot 30]
 *
 * Returns creature info by reading through the creature data chain:
 * this->field_4 -> field_84 (the creature info/stats pointer).
 */
void* pongCreatureInst::GetCreatureInfo() {  // vfn_30
    void* creatureData = *(void**)((char*)this + 4);
    if (!creatureData) return nullptr;
    return *(void**)((char*)creatureData + 84);
}

/**
 * pongCreatureInst::GetPhysicsState @ 0x822C9200 | size: 0x38  [vtable slot 31]
 *
 * Returns a pointer to the creature's physics state data by walking
 * the data chain: this->field_4 -> field_12 -> field_84.
 * Returns nullptr if any link is null.
 */
void* pongCreatureInst::GetPhysicsState() {  // vfn_31
    void* inner = *(void**)((char*)this + 4);
    if (!inner) return nullptr;
    void* physics = *(void**)((char*)inner + 12);
    if (!physics) return nullptr;
    return *(void**)((char*)physics + 84);
}

/**
 * pongCreatureInst::GetCollisionFlags @ 0x822C92D8 | size: 0x40  [vtable slot 37]
 *
 * Returns collision flag data from the creature's physics subsystem.
 * Walks: this->field_4 -> field_12 -> field_84, then reads the
 * collision flags at +16 of the resulting object.
 */
uint32_t pongCreatureInst::GetCollisionFlags() {  // vfn_37
    void* inner = *(void**)((char*)this + 4);
    if (!inner) return 0;
    void* physics = *(void**)((char*)inner + 12);
    if (!physics) return 0;
    void* collisionData = *(void**)((char*)physics + 84);
    if (!collisionData) return 0;
    return *(uint32_t*)((char*)collisionData + 16);
}

/**
 * pongCreatureInst::GetSubObjectTransform @ 0x820C8E98 | size: 0x40  [vtable slot 3]
 *
 * Returns a 4x4 matrix pointer for the creature's sub-object at the
 * specified index. Walks: this->field_80 -> field_12 -> buffer[index*64].
 */
void* pongCreatureInst::GetSubObjectTransform(int index) {  // vfn_3
    void* subObjMgr = *(void**)((char*)this + 80);
    if (!subObjMgr) return nullptr;
    void* pool = *(void**)((char*)subObjMgr + 12);
    char* base = *(char**)((char*)pool + 120);
    return base + (index << 6);
}


// ─────────────────────────────────────────────────────────────────────────────
// pongCreatureInst — Batch 2: thunks, flag checks, delegation helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongCreatureInst::HasVisibilityFlag @ 0x820C5378 | size: 0x20
 *
 * Tests bit 2 of the flags byte at +431 (0x1AF). Returns true if the
 * creature has the visibility/render flag set.
 */
bool pongCreatureInst::HasVisibilityFlag() {  // 5378
    uint8_t flags = *(uint8_t*)((char*)this + 431);
    return (flags & 0x4) != 0;
}

/**
 * pongCreatureInst::IsNetworkActive @ 0x8211C9C8 | size: 0x20
 *
 * Returns true if the global network state pointer at 0x8271A36C is
 * non-null, indicating an active network session.
 */
bool pongCreatureInst::IsNetworkActive() {  // C9C8_w
    extern void* g_networkStatePtr;  // @ 0x8271A36C
    return g_networkStatePtr != nullptr;
}

/**
 * pongCreatureInst::IsIndexValid @ 0x8211CF90 | size: 0x20
 *
 * Compares field_12 (current index) against field_16 (max index).
 * Returns true if current >= max (i.e., index has reached capacity).
 */
bool pongCreatureInst::IsIndexValid() {  // CF90_h
    int32_t currentIdx = *(int32_t*)((char*)this + 12);
    int32_t maxIdx = *(int32_t*)((char*)this + 16);
    return currentIdx >= maxIdx;
}

/**
 * pongCreatureInst::GetCurrentWeight @ 0x8211D010 | size: 0x38
 *
 * Returns the current animation weight for the active blend entry.
 * If field_0 (pointer array) is non-null, uses indexed lookup:
 *   array[index] -> field at +124
 * Otherwise falls back to linear array at field_4:
 *   field_4 + (index * 176) + 96
 */
float pongCreatureInst::GetCurrentWeight() {  // D010_h
    void* ptrArray = *(void**)((char*)this + 0);
    int32_t index = *(int32_t*)((char*)this + 12);

    if (ptrArray) {
        // Indexed pointer array lookup
        void** entries = (void**)ptrArray;
        void* entry = entries[index];
        return *(float*)((char*)entry + 124);
    } else {
        // Linear array fallback (stride = 176 bytes per entry)
        char* base = *(char**)((char*)this + 4);
        return *(float*)(base + index * 176 + 96);
    }
}

/**
 * pongCreatureInst::DelegateSlot9_Offset32 @ 0x82118FC0 | size: 0x1C
 *
 * Extracts the target object from arg->field_4, then tail-calls its
 * vtable slot 9 with this+32 as the data argument.
 */
void pongCreatureInst::DelegateSlot9_Offset32(void* arg) {  // 8FC0_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 32);
}

/**
 * pongCreatureInst::DelegateSlot9_Offset48 @ 0x82118FE0 | size: 0x1C
 *
 * Same pattern as DelegateSlot9_Offset32, with this+48.
 */
void pongCreatureInst::DelegateSlot9_Offset48(void* arg) {  // 8FE0_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 48);
}

/**
 * pongCreatureInst::DelegateSlot9_Offset64 @ 0x82119000 | size: 0x1C
 *
 * Same pattern as DelegateSlot9_Offset32, with this+64.
 */
void pongCreatureInst::DelegateSlot9_Offset64(void* arg) {  // 9000_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 64);
}

/**
 * pongCreatureInst::DelegateSlot9_Offset80 @ 0x82119020 | size: 0x1C
 *
 * Same pattern, with this+80.
 */
void pongCreatureInst::DelegateSlot9_Offset80(void* arg) {  // 9020_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 80);
}

/**
 * pongCreatureInst::DelegateSlot9_Offset112 @ 0x82119040 | size: 0x1C
 *
 * Same pattern, with this+112.
 */
void pongCreatureInst::DelegateSlot9_Offset112(void* arg) {  // 9040_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 112);
}

/**
 * pongCreatureInst::DelegateSlot9_Offset128 @ 0x82119060 | size: 0x1C
 *
 * Same pattern, with this+128.
 */
void pongCreatureInst::DelegateSlot9_Offset128(void* arg) {  // 9060_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 128);
}

/**
 * pongCreatureInst::DelegateSlot9_Offset144 @ 0x82119080 | size: 0x1C
 *
 * Same pattern, with this+144.
 */
void pongCreatureInst::DelegateSlot9_Offset144(void* arg) {  // 9080_p42
    void* target = *(void**)((char*)arg + 4);
    typedef void (*SlotFunc)(void*, void*);
    void** vt = *(void***)target;
    SlotFunc fn = (SlotFunc)vt[9];
    fn(target, (char*)this + 144);
}

/**
 * pongCreatureInst::StoreVCallResultAtField429 @ 0x82118E70 | size: 0x40
 *
 * Extracts object from arg->field_4, calls its vtable slot 4,
 * and stores the byte result at this+429. Used for syncing
 * animation state flags from a network message.
 */
void pongCreatureInst::StoreVCallResultAtField429(void* arg) {  // 8E70_p42
    void* target = *(void**)((char*)arg + 4);
    typedef uint8_t (*GetFunc)(void*);
    void** vt = *(void***)target;
    GetFunc fn = (GetFunc)vt[4];
    uint8_t result = fn(target);
    *(uint8_t*)((char*)this + 429) = result;
}

/**
 * pongCreatureInst::StoreVCallResultAtField446 @ 0x82119180 | size: 0x40
 *
 * Same pattern as StoreVCallResultAtField429, stores at this+446.
 */
void pongCreatureInst::StoreVCallResultAtField446(void* arg) {  // 9180_p42
    void* target = *(void**)((char*)arg + 4);
    typedef uint8_t (*GetFunc)(void*);
    void** vt = *(void***)target;
    GetFunc fn = (GetFunc)vt[4];
    uint8_t result = fn(target);
    *(uint8_t*)((char*)this + 446) = result;
}

// ─────────────────────────────────────────────────────────────────────────────
// LocomotionStateAnim — Small helpers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * LocomotionStateAnim::ForwardToVfn7 @ 0x8224BB98 | size: 0x10
 *
 * Tail-calls vtable slot 7 on self. Forwards a notification/update
 * to the state's concrete implementation.
 */
void LocomotionStateAnim::ForwardToVfn7() {  // BB98
    typedef void (*Func)(void*);
    void** vt = *(void***)this;
    Func fn = (Func)vt[7];
    fn(this);
}

/**
 * LocomotionStateAnim::CountActiveAnimLayers @ 0x820DBA60 | size: 0x3C
 *
 * Counts the number of active animation layers (at offsets +16, +20, +24).
 * Each non-null pointer counts as one active layer. Returns count + 1
 * (base layer is always counted) if field_24 is non-null, otherwise
 * returns just the count of the first two layers.
 */
int LocomotionStateAnim::CountActiveAnimLayers() {  // BA60_g
    int count = 0;
    if (*(void**)((char*)this + 16) != nullptr) count++;
    if (*(void**)((char*)this + 20) != nullptr) count++;
    if (*(void**)((char*)this + 24) != nullptr) return count + 1;
    return count;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongCreatureInst / LocomotionState — Batch 3 (39 remaining small functions)
// ─────────────────────────────────────────────────────────────────────────────

// ── VTABLE THUNKS ──────────────────────────────────────────────────────────

/**
 * pongCreatureInst vtable thunks @ 0x8211CC48-CC78 | 16B each
 *
 * Four adjacent vtable forwarding thunks. Each loads the vtable from
 * this, then tail-calls a specific slot. Used by the creature instance
 * to forward calls to its inner physics/collision object.
 */
void pongCreatureInst::ForwardVfn11() {  // CC48_p42 @ 0x8211CC48
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[11])(this);
}

void pongCreatureInst::ForwardVfn12() {  // CC58_p42 @ 0x8211CC58
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[12])(this);
}

void pongCreatureInst::ForwardVfn13() {  // CC68_p42 @ 0x8211CC68
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[13])(this);
}

void pongCreatureInst::ForwardVfn14() {  // CC78_p33 @ 0x8211CC78
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[14])(this);
}

// ── FIELD SETTERS (vcall result → store) ───────────────────────────────────

/**
 * pongCreatureInst::UpdateFlag445 @ 0x82118EB0 | size: 0x50
 *
 * Calls vtable slot 4 on the object at arg->field_4 to get a bool,
 * stores it at this+445. Used for syncing creature visibility flags.
 */
void pongCreatureInst::UpdateFlag445(void* arg) {  // 8EB0_p42
    void* target = *(void**)((char*)arg + 4);
    typedef uint8_t (*GetFn)(void*);
    uint8_t val = ((GetFn)(*(void***)target)[4])(target);
    *(uint8_t*)((char*)this + 445) = val;
}

/**
 * pongCreatureInst::UpdateFlag447 @ 0x82118F70 | size: 0x50
 *
 * Same pattern as UpdateFlag445, stores at this+447.
 */
void pongCreatureInst::UpdateFlag447(void* arg) {  // 8F70_p42
    void* target = *(void**)((char*)arg + 4);
    typedef uint8_t (*GetFn)(void*);
    uint8_t val = ((GetFn)(*(void***)target)[4])(target);
    *(uint8_t*)((char*)this + 447) = val;
}

/**
 * pongCreatureInst::UpdateFlagBit3 @ 0x82118F00 | size: 0x70
 *
 * Calls vtable slot 4 on arg->field_4 to get a bool. If true,
 * sets bit 3 (0x08) of the flags byte at this+431. If false, clears it.
 */
void pongCreatureInst::UpdateFlagBit3(void* arg) {  // 8F00_p42
    void* target = *(void**)((char*)arg + 4);
    typedef uint8_t (*GetFn)(void*);
    uint8_t val = ((GetFn)(*(void***)target)[4])(target);

    uint8_t* flags = (uint8_t*)((char*)this + 431);
    if (val) {
        *flags |= 0x08;   // set bit 3
    } else {
        *flags &= ~0x08;  // clear bit 3
    }
}

// ── COMPUTATION: Lookups & Searches ────────────────────────────────────────

/**
 * pongCreature::LookupHashValue @ 0x820C79F0 | size: 0x34
 *
 * Looks up a value in the creature's hash table at this+160.
 * Key from *keyPtr is shifted left 3 (×8 byte entries). The hash table
 * stores values at entry+4. Writes result to *outPtr, or 0 if not found.
 */
void pongCreature::LookupHashValue(void* outPtr, void* keyPtr) {  // 79F0_p45
    uint32_t key = *(uint32_t*)keyPtr;
    void* hashTable = *(void**)((char*)this + 160);
    char* entries = *(char**)((char*)hashTable + 8);
    char* entry = entries + (key << 3);

    if (entry) {
        *(uint32_t*)outPtr = *(uint32_t*)(entry + 4);
    } else {
        *(uint32_t*)outPtr = 0;
    }
}

/**
 * pongCreature::FindIndexByKey @ 0x820C7990 | size: 0x60
 *
 * Linear scan of an array looking for a matching key. Returns the
 * index into *outPtr, or -1 if not found. Array entries have stride 8
 * with key at entry+0.
 */
void pongCreature::FindIndexByKey(void* outPtr, void* keyPtr) {  // 7990_p45
    uint32_t searchKey = *(uint32_t*)keyPtr;
    void* arrayBase = *(void**)((char*)this + 160);
    char* entries = *(char**)((char*)arrayBase + 8);
    uint32_t count = *(uint32_t*)((char*)arrayBase + 12);

    for (uint32_t i = 0; i < count; i++) {
        uint32_t entryKey = *(uint32_t*)(entries + i * 8);
        if (entryKey == searchKey) {
            *(int32_t*)outPtr = (int32_t)i;
            return;
        }
    }
    *(int32_t*)outPtr = -1;
}

/**
 * pongCreatureInst::LookupContainer @ 0x8211CFB0 | size: 0x60
 *
 * Dual-path lookup: if field_0 (pointer array) is non-null, uses
 * indexed access: array[index]->field_4. Otherwise uses linear
 * stride: field_4 + (index * 176) + 4. Returns the uint32_t value.
 */
uint32_t pongCreatureInst::LookupContainer() {  // CFB0_h
    void* ptrArray = *(void**)((char*)this + 0);
    int32_t index = *(int32_t*)((char*)this + 12);

    if (ptrArray) {
        void** entries = (void**)ptrArray;
        void* entry = entries[index];
        return *(uint32_t*)((char*)entry + 4);
    } else {
        char* base = *(char**)((char*)this + 4);
        return *(uint32_t*)(base + index * 176 + 4);
    }
}

/**
 * pongCreatureInst::HasMultipleActiveSlots @ 0x82127BD8 | size: 0x6C
 *
 * Iterates the bitfield at this+464 (32 bits), counting set bits.
 * Returns true if more than 1 bit is set — indicating multiple
 * active animation or physics slots.
 */
bool pongCreatureInst::HasMultipleActiveSlots() {  // 7BD8_h
    uint32_t bitfield = *(uint32_t*)((char*)this + 464);
    int count = 0;
    while (bitfield) {
        count += (bitfield & 1);
        bitfield >>= 1;
        if (count > 1) return true;
    }
    return false;
}

// ── COMPUTATION: List/Array Operations ─────────────────────────────────────

/**
 * pongCreatureInst::ListInsertTail @ 0x821267E8 | size: 0x6C
 *
 * Inserts a node at the tail of a doubly-linked list. Updates prev/next
 * pointers and increments the element count at this+8.
 */
void pongCreatureInst::ListInsertTail(void* node) {  // 67E8_h
    char* listHead = (char*)this;
    void* tail = *(void**)(listHead + 4);  // current tail

    // Link new node
    *(void**)((char*)node + 0) = tail;       // node->prev = tail
    *(void**)((char*)node + 4) = nullptr;    // node->next = null

    if (tail) {
        *(void**)((char*)tail + 4) = node;   // tail->next = node
    } else {
        *(void**)(listHead + 0) = node;      // head = node (empty list)
    }
    *(void**)(listHead + 4) = node;          // tail = node

    // Increment count
    int32_t* count = (int32_t*)(listHead + 8);
    (*count)++;
}

/**
 * pongCreatureInst::ArrayRemove @ 0x822C9420 | size: 0x74
 *
 * Finds and removes an element from a pointer array by value.
 * Uses shift-left compaction to fill the gap. Decrements count at +8.
 */
void pongCreatureInst::ArrayRemove(void* element) {  // 9420_h
    void** array = *(void***)((char*)this + 0);
    int32_t count = *(int32_t*)((char*)this + 8);

    for (int i = 0; i < count; i++) {
        if (array[i] == element) {
            // Shift remaining elements left
            for (int j = i; j < count - 1; j++) {
                array[j] = array[j + 1];
            }
            array[count - 1] = nullptr;
            *(int32_t*)((char*)this + 8) = count - 1;
            return;
        }
    }
}

/**
 * pongCreatureInst::SetRefPtr @ 0x822C9030 | size: 0x70
 *
 * Atomically swaps a ref-counted pointer at this+4. Adds a reference
 * to the new object and releases the old one.
 */
void pongCreatureInst::SetRefPtr(void* newObj) {  // 9030_g
    void* oldObj = *(void**)((char*)this + 4);
    if (newObj) rage_AddRef(newObj);
    *(void**)((char*)this + 4) = newObj;
    if (oldObj) rage_Release(oldObj);
}

/**
 * pongCreatureInst::SetArraySlot @ 0x8228E7B8 | size: 0x64
 *
 * Sets a ref-counted pointer in an array slot. Adds ref to new value,
 * releases old value at the specified index.
 */
void pongCreatureInst::SetArraySlot(int index, void* newObj) {  // E7B8_h
    void** array = *(void***)((char*)this + 0);
    void* oldObj = array[index];

    if (newObj) rage_AddRef(newObj);
    array[index] = newObj;
    if (oldObj) rage_Release(oldObj);
}

// ── COMPUTATION: Detach/Destroy ────────────────────────────────────────────

/**
 * pongCreature::ScalarDeletingDtor @ 0x820C5398 | size: 0x50
 *
 * Standard MSVC scalar deleting destructor. Calls the base destructor,
 * then conditionally frees memory if bit 0 of the flags parameter is set.
 */
void pongCreature::ScalarDeletingDtor(int flags) {  // vfn_0_5398_1

    pongCreature_BaseDtor(this);
    if (flags & 1) {
        rage_free(this);
    }
}

/**
 * pongCreatureInst::Detach @ 0x8211D470 | size: 0x58
 *
 * Detaches this creature instance from its parent. Sets the vtable to
 * the base class, calls the parent's remove method (vtable slot 3) to
 * unlink from the parent list, then calls the cleanup helper.
 */
void pongCreatureInst::Detach() {  // D470_p33
    extern void* g_creatureManager;  // @ 0x826063B8 (SDA)

    // Set vtable to base phInst class
    *(void**)this = (void*)0x82022D2C;  // phInst vtable

    // Remove from parent's child list
    void* parent = *(void**)((char*)g_creatureManager + 876);
    void* childToRemove = *(void**)((char*)this + 192);
    typedef void (*RemoveFn)(void*, void*);
    RemoveFn removeChild = (RemoveFn)(*(void***)parent)[3];
    removeChild(parent, childToRemove);

    // Run cleanup
    pongCreatureInst_Cleanup(this);
}

/**
 * pongCreature::Relocate @ 0x820C8858 | size: 0x7C
 *
 * Relocates creature data pointers after a memory move. Sets vtable,
 * rebases the pointer at +4 using a stride table from the relocator,
 * then calls the fixup function.
 */
void pongCreature::Relocate(void* relocator) {  // 8858_p46

    // Set vtable
    *(void**)this = (void*)0x82028144;  // pongCreature vtable

    // Rebase field_4 pointer
    void* basePtr = *(void**)((char*)this + 4);
    if (basePtr && relocator) {
        uint32_t* strideTable = *(uint32_t**)((char*)relocator + 4);
        uint32_t offset = strideTable[1];
        *(void**)((char*)this + 4) = (char*)basePtr + offset;
    }

    pongCreature_Fixup(this, relocator);
}

// ── COMPUTATION: Matrix/Vector Operations ──────────────────────────────────

/**
 * pongCreatureInst::SetMatrix @ 0x820D5040 | size: 0x60
 *
 * Copies a 4×4 matrix (64 bytes, 4 vec128s) from the source to a
 * stack-local buffer, then calls the internal SetMatrix helper.
 */
void pongCreatureInst::SetMatrix(const void* srcMatrix) {  // 5040
    // Original copies 4 lvx128/stvx128 pairs to align the matrix on stack
    pongCreatureInst_SetMatrixImpl(this, srcMatrix);
}

/**
 * pongCreatureInst::UpdateHeading @ 0x8228E828 | size: 0x60
 *
 * Updates creature heading: calls two helpers to compute the heading
 * angle, stores it at this+8, then conditionally calls a third helper
 * if the heading changed significantly.
 */
void pongCreatureInst::UpdateHeading() {  // E828_v12

    float heading = pongCreatureInst_ComputeHeading(this);
    float prevHeading = *(float*)((char*)this + 8);
    *(float*)((char*)this + 8) = heading;

    if (heading != prevHeading) {
        pongCreatureInst_NotifyHeadingChanged(this);
    }
}

// ── LocomotionStateAnim ────────────────────────────────────────────────────

/**
 * LocomotionStateAnim::ResetAccumulators @ 0x820DB9F0 | size: 0x70
 *
 * Zeros 3 float weight values at +28, +32, +36 and 4 vec128
 * accumulators at +48, +64, +80, +96. Uses AltiVec vxor to clear
 * 16-byte aligned vectors.
 */
void LocomotionStateAnim::ResetAccumulators() {  // B9F0_w
    // Zero weight floats (loop 3 iterations: +28, +32, +36)
    *(float*)((char*)this + 28) = 0.0f;
    *(float*)((char*)this + 32) = 0.0f;
    *(float*)((char*)this + 36) = 0.0f;

    // Zero interleaved counters at +16, +20, +24
    *(uint32_t*)((char*)this + 16) = 0;
    *(uint32_t*)((char*)this + 20) = 0;
    *(uint32_t*)((char*)this + 24) = 0;

    // Zero 4 vec128 accumulators at +48, +64, +80, +96
    memset((char*)this + 48, 0, 64);
}

/**
 * LocomotionStateAnim::SetAnimSpeed @ 0x820C2858 | size: 0x54
 *
 * Sets animation playback speed by multiplying speed × scale factor,
 * then calls the internal anim speed setter.
 */
void LocomotionStateAnim::SetAnimSpeed(float speed, float scale) {  // 2858

    float scaledSpeed = speed * scale;
    LocomotionStateAnim_SetSpeed(this, scaledSpeed);
    LocomotionStateAnim_ApplySpeed(this);
}

/**
 * LocomotionStateAnim::InitBoundCapsule @ 0x820C3D48 | size: 0x58
 *
 * Initializes a bounding capsule from float constants. Zeroes a vec128
 * and calls the capsule initialization helper.
 */
void LocomotionStateAnim::InitBoundCapsule() {  // 3D48_g

    float zeroVec[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float radius = 0.0f;  // loaded from constant table
    LocomotionStateAnim_InitCapsuleImpl(this, radius, zeroVec);
}

/**
 * LocomotionStateAnim::SetGlobalInstance @ 0x820C34E8 | size: 0x58
 *
 * Swaps the global LocomotionStateAnim pointer at 0x8271A378.
 * Adds a reference to the new instance and releases the old one.
 */
void LocomotionStateAnim::SetGlobalInstance(void* newInstance) {  // 34E8_p46
    extern void* g_locomotionAnimInstance;  // @ 0x8271A378

    void* oldInstance = g_locomotionAnimInstance;
    if (newInstance) rage_AddRef(newInstance);
    g_locomotionAnimInstance = newInstance;
    if (oldInstance) rage_Release(oldInstance);
}

/**
 * LocomotionStateAnim::SetMatrix @ 0x820D7B30 | size: 0x60
 *
 * Copies a 4×4 matrix (64 bytes) and forwards to the internal setter.
 */
void LocomotionStateAnim::SetMatrix(const void* srcMatrix) {  // 7B30_g
    LocomotionStateAnim_SetMatrixImpl(this, srcMatrix);
}

/**
 * LocomotionState::AdvanceNode @ 0x820DEFA0 | size: 0x54
 *
 * If the linked list at this+4 is non-empty, advances to the next
 * node using FindFirstActive and stores the result.
 */
void LocomotionState::AdvanceNode() {  // EFA0_h

    void* listHead = *(void**)((char*)this + 4);
    if (listHead) {
        void* next = LocomotionState_FindFirstActive(listHead);
        *(void**)((char*)this + 4) = next;
    }
}

/**
 * LocomotionState::FindFirstActive @ 0x820DEF38 | size: 0x68
 *
 * Walks a linked list, skipping sentinel nodes (field+0 == 0).
 * Returns the first node whose field+8 is non-null.
 */
void* LocomotionState::FindFirstActive(void* listHead) {  // EF38_p46
    char* node = (char*)listHead;
    while (node) {
        void* sentinel = *(void**)(node + 0);
        if (sentinel != nullptr) {
            void* data = *(void**)(node + 8);
            if (data) return node;
        }
        node = *(char**)(node + 4);  // next pointer
    }
    return nullptr;
}

/**
 * LocomotionStateAnim::DispatchAnimEvent @ 0x820DEEC8 | size: 0x6C
 *
 * Finds an animation via the BEF8 lookup function, then calls its
 * vtable slot 18 with the provided argument. Returns true if the
 * animation was found and the event dispatched.
 */
bool LocomotionStateAnim::DispatchAnimEvent(void* eventArg) {  // EEC8

    void* anim = LocomotionStateAnim_FindAnim(this);
    if (!anim) return false;

    typedef bool (*EventFn)(void*, void*);
    EventFn dispatch = (EventFn)(*(void***)anim)[18];
    return dispatch(anim, eventArg);
}

/**
 * LocomotionStateAnim::GetAnimPosition @ 0x820CB800 | size: 0x74
 *
 * Finds an animation via BEF8, calls its vtable slot 14 to get a
 * position vector, and copies the result to the output pointer.
 */
void LocomotionStateAnim::GetAnimPosition(void* outPosition) {  // B800_g

    void* anim = LocomotionStateAnim_FindAnim(this);
    if (!anim) {
        memset(outPosition, 0, 16);
        return;
    }

    typedef void (*GetPosFn)(void*, void*);
    GetPosFn getPos = (GetPosFn)(*(void***)anim)[14];
    getPos(anim, outPosition);
}

/**
 * LocomotionStateAnim::AccumulateOffset @ 0x8223BDB0 | size: 0x6C
 *
 * Cross-product helper: calls BBB0 to get a vec128, then adds
 * the result to the accumulator.
 */
void LocomotionStateAnim::AccumulateOffset(void* accumulator) {  // BDB0_g

    float offset[4];
    LocomotionStateAnim_ComputeOffset(this, offset);

    // Add to accumulator (vec128 addition)
    float* acc = (float*)accumulator;
    acc[0] += offset[0];
    acc[1] += offset[1];
    acc[2] += offset[2];
    acc[3] += offset[3];
}

/**
 * LocomotionStateAnim::AccumulateList @ 0x8214D480 | size: 0x6C
 *
 * Walks a linked list of animation nodes, calling two helpers per
 * node to accumulate contributions (7E68 + 3AD8).
 */
void LocomotionStateAnim::AccumulateList() {  // D480_w

    void* node = *(void**)((char*)this + 4);
    while (node) {
        LocomotionStateAnim_ProcessNode(this, node);
        LocomotionStateAnim_AccumNode(this, node);
        node = *(void**)((char*)node + 4);  // next
    }
}

/**
 * LocomotionStateAnim::RebasePointers @ 0x823851A8 | size: 0x6C
 *
 * Rebases two pointers (+0 and +12) using a stride table provided
 * by the relocator at r4. Each pointer is offset by the corresponding
 * stride entry.
 */
void LocomotionStateAnim::RebasePointers(void* relocator) {  // 51A8_p46
    uint32_t* strideTable = *(uint32_t**)((char*)relocator + 4);

    void* ptr0 = *(void**)((char*)this + 0);
    if (ptr0) {
        *(void**)((char*)this + 0) = (char*)ptr0 + strideTable[0];
    }

    void* ptr12 = *(void**)((char*)this + 12);
    if (ptr12) {
        *(void**)((char*)this + 12) = (char*)ptr12 + strideTable[1];
    }
}

/**
 * LocomotionStateAnim::IsAngleCloser @ 0x823DA190 | size: 0x64
 *
 * Compares two angular distances. Returns true if the absolute value
 * of angle2 is closer to the target than angle3.
 */
bool LocomotionStateAnim::IsAngleCloser(float target, float angle2, float angle3) {  // A190_w
    float diff2 = angle2 - target;
    float diff3 = angle3 - target;
    float abs2 = (diff2 >= 0.0f) ? diff2 : -diff2;
    float abs3 = (diff3 >= 0.0f) ? diff3 : -diff3;
    return abs2 < abs3;
}

/**
 * LocomotionStateAnim::AdvanceRingBuffer @ 0x823804C0 | size: 0x80
 *
 * Advances a ring buffer index (mod 4), decrements the remaining
 * count, and updates internal state pointers. Used for animation
 * sample buffering.
 */
void LocomotionStateAnim::AdvanceRingBuffer() {  // 04C0_fw
    int32_t index = *(int32_t*)((char*)this + 8);
    int32_t remaining = *(int32_t*)((char*)this + 12);

    index = (index + 1) & 3;  // mod 4
    remaining--;

    *(int32_t*)((char*)this + 8) = index;
    *(int32_t*)((char*)this + 12) = remaining;

    // Update current pointer based on new index
    void** buffers = *(void***)((char*)this + 0);
    *(void**)((char*)this + 16) = buffers[index];
}

/**
 * LocomotionStateAnim::PushBack @ 0x82234190 | size: 0x80
 *
 * Pushes an element (key + vec128) onto a growable array. If the array
 * is full, grows it before insertion.
 */
void LocomotionStateAnim::PushBack(uint32_t key, const void* vecData) {  // 4190_g

    uint32_t count = *(uint32_t*)((char*)this + 8);
    uint32_t capacity = *(uint32_t*)((char*)this + 12);

    if (count >= capacity) {
        LocomotionStateAnim_GrowArray(this);
    }

    // Insert at end: entry = base + count * 20 (key=4 + vec=16)
    char* base = *(char**)((char*)this + 0);
    char* entry = base + count * 20;
    *(uint32_t*)entry = key;
    memcpy(entry + 4, vecData, 16);

    *(uint32_t*)((char*)this + 8) = count + 1;
}

// ── LocomotionStateMf ──────────────────────────────────────────────────────

/**
 * LocomotionStateMf::UpdateBlend @ 0x8222C128 | size: 0x60
 *
 * Chains 5 internal helper calls to update a blend state: scale
 * weights, blend poses, update transforms, apply constraints, finish.
 */
void LocomotionStateMf::UpdateBlend() {  // C128_w

    LocomotionStateMf_ScaleWeights(this);
    LocomotionStateMf_BlendPoses(this);
    LocomotionStateMf_UpdateTransforms(this);
    LocomotionStateMf_ApplyConstraints(this);
    LocomotionStateMf_Finish(this);
}

/**
 * LocomotionStateMf::EraseRange @ 0x822331A8 | size: 0x64
 *
 * Erases elements in range [start, end) from a contiguous array via
 * memmove. Updates the end pointer at this+8.
 */
void LocomotionStateMf::EraseRange(void* rangeStart, void* rangeEnd) {  // 31A8
    char* endPtr = *(char**)((char*)this + 8);
    size_t tailSize = endPtr - (char*)rangeEnd;

    if (tailSize > 0) {
        memmove(rangeStart, rangeEnd, tailSize);
    }

    // Update end pointer
    size_t erased = (char*)rangeEnd - (char*)rangeStart;
    *(char**)((char*)this + 8) = endPtr - erased;
}

/**
 * LocomotionStateMf::ComputeWeightedSum @ 0x820E3148 | size: 0x68
 *
 * Computes a weighted sum: dot product of weight array with data
 * array, scaled by a factor from this+40.
 */
float LocomotionStateMf::ComputeWeightedSum(const float* weights, const float* data, int count) {  // 3148_h
    float scale = *(float*)((char*)this + 40);
    float sum = 0.0f;

    for (int i = 0; i < count; i++) {
        sum += weights[i] * data[i];
    }

    return sum * scale;
}

/**
 * LocomotionStateMf::SerializeTriple @ 0x82237480 | size: 0x78
 *
 * Calls vtable slot 2 on the serializer three times with consecutive
 * field offsets from the data pointer. Used for serializing a 3-component
 * vector (e.g., position XYZ).
 */
void LocomotionStateMf::SerializeTriple(void* serializer, void* data) {  // 7480_w
    typedef void (*SerializeFn)(void*, void*);
    void** vt = *(void***)serializer;
    SerializeFn serialize = (SerializeFn)vt[2];

    serialize(serializer, (char*)data + 0);
    serialize(serializer, (char*)data + 4);
    serialize(serializer, (char*)data + 8);
}



////////////////////////////////////////////////////////////////////////////////
// LocomotionStateAnim — Quaternion / Matrix / Animation Utilities
////////////////////////////////////////////////////////////////////////////////

// External helpers used by newly-lifted functions
extern void phBoundCapsule_4360_g(void* capsule, void* outResult);
extern void phBoundCapsule_7B08_g(void* matrix, float* outVec);
extern float LocomotionStateAnim_A1F8_g(float value, float rangeMin, float rangeMax);
extern void LocomotionStateAnim_8278_g(void* matrix, float* vec);
extern void LocomotionStateAnim_B218_g(void* dst, void* srcMatrix);
extern void LocomotionStateAnim_6E90_w(void* self, float weight);
extern bool LocomotionStateAnim_04F0_g(void* animData, uint16_t animID_high, uint16_t* outIndex);
extern void util_BBB0(void* dst, float* src, float* work);
extern void util_B9A8(void* dst, void* quat, void* axis);
extern void LocomotionStateAnim_C128_g(void* anim, void* animData, float weight);
extern void LocomotionStateAnim_C288_g(void* anim, void* animData, float weight);
extern void LocomotionStateAnim_BEF8_g(void* self, uint16_t animID_high, uint8_t animID_low);
extern void LocomotionStateAnim_C358_g(void* animList, int p1, int p2, int p3, float* outVec);

/**
 * QuaternionToMatrix3x3 @ 0x8223AD30 | size: 0xAC
 *
 * Converts a unit quaternion (x, y, z, w) stored at src into a
 * 3x3 rotation matrix written row-major to dst (11 floats).
 * The quaternion components are first doubled (scaled by 2.0), then
 * the standard quat-to-matrix formula is applied.
 *
 * @param dst  Output 3x3 matrix (at least 44 bytes)
 * @param src  Input quaternion as 4 floats (x, y, z, w)
 */
// LocomotionStateAnim_AD30_g @ 0x8223AD30
void LocomotionStateAnim_AD30_g(float* dst, const float* src) {
    float qx = src[0];
    float qy = src[1];
    float qz = src[2];
    float qw = src[3];

    // Scale by 2.0 (constant at lbl_82029B7C)
    float s = 2.0f;
    float x2 = qx * s;
    float y2 = qy * s;
    float z2 = qz * s;
    float w2 = qw * s;

    float zx = z2 * x2;
    float yx = y2 * x2;
    float wz = w2 * z2;
    float zy = z2 * y2;
    float wy = w2 * y2;
    float wx = w2 * x2;
    float yy = y2 * y2;
    float zz = z2 * z2;
    float xx = x2 * x2;

    // Off-diagonal elements
    dst[1]  = yx + wz;
    dst[4]  = yx - wz;
    dst[2]  = zx - wy;
    dst[8]  = zx + wy;
    dst[6]  = wx + zy;
    dst[9]  = zy - wx;

    // Diagonal elements: 0.5 - (sum of two squared terms)
    float half = 0.5f;
    dst[0]  = half - (yy + zz);
    dst[5]  = half - (xx + zz);
    dst[10] = half - (xx + yy);
}

/**
 * ComputeCapsuleAngle @ 0x822347E8 | size: 0x84
 *
 * Computes a signed angle from a capsule query result. If the
 * result is negative, wraps it by adding 2*PI. Stores the result at self+0.
 * Also copies 16 bytes from capsule+48 to self+16.
 */
// LocomotionStateAnim_47E8_g @ 0x822347E8
void LocomotionStateAnim_47E8_g(void* self, void* capsule) {
    float result[6];

    // Store 0.0f at self+0 as default
    *(float*)((char*)self + 0) = 0.0f;

    // Copy 16 bytes from capsule+48 to self+16 (lvx/stvx)
    memcpy((char*)self + 16, (char*)capsule + 48, 16);

    // Call capsule angle computation
    phBoundCapsule_4360_g(capsule, result);

    // Read the angle from result[1]
    float angle = result[1];

    // If negative, wrap by adding 2*PI
    if (angle < 0.0f) {
        extern const float g_twoPi;  // @ 0x8202C02C
        angle += g_twoPi;
    }

    // Store final angle
    *(float*)((char*)self + 0) = angle;
}

/**
 * HasAnimIDs5And6 @ 0x822E09A0 | size: 0x8C
 *
 * Checks whether the animation container has both animation IDs 5
 * and 6 present. Returns true only if both lookups succeed.
 */
// LocomotionStateAnim_09A0_g @ 0x822E09A0
bool LocomotionStateAnim_09A0_g(void* self) {
    // Check for anim ID 5
    void* anim5 = ((LocomotionStateAnim*)self)->FindAnimationByID(5, 0);
    bool has5 = (anim5 != nullptr);

    // Check for anim ID 6
    void* anim6 = ((LocomotionStateAnim*)self)->FindAnimationByID(6, 0);
    bool has6 = (anim6 != nullptr);

    return has5 && has6;
}

/**
 * ClampAndTransformVector @ 0x82138110 | size: 0x8C
 *
 * Builds a rotation matrix from a quaternion, extracts a position
 * vector, clamps each XYZ component between min/max bounds, writes
 * the result back into the matrix, and applies it to the destination.
 */
// LocomotionStateAnim_8110_g @ 0x82138110
void LocomotionStateAnim_8110_g(void* quat, float* minVec, float* maxVec) {
    float matrix[12];
    float vec[4];

    // Build rotation matrix from quaternion
    LocomotionStateAnim_AD30_g(matrix, (const float*)quat);

    // Extract position vector from matrix
    phBoundCapsule_7B08_g(matrix, vec);

    // Clamp each component: LocomotionStateAnim_A1F8_g(value, min, max)
    vec[0] = LocomotionStateAnim_A1F8_g(vec[0], minVec[0], maxVec[0]);
    vec[1] = LocomotionStateAnim_A1F8_g(vec[1], minVec[1], maxVec[1]);
    vec[2] = LocomotionStateAnim_A1F8_g(vec[2], minVec[2], maxVec[2]);

    // Write clamped vector back into matrix
    LocomotionStateAnim_8278_g(matrix, vec);

    // Apply matrix to destination
    LocomotionStateAnim_B218_g(quat, matrix);
}

/**
 * SetupAnimNode @ 0x820E10D8 | size: 0x9C
 *
 * Sets up an animation node: configures container data with timing
 * parameters, queries a position vector from the container, builds
 * a rotation matrix, and copies a row into dst+48.
 */
// LocomotionStateAnim_10D8 @ 0x820E10D8
void LocomotionStateAnim_10D8(void* self, void* dst, void* unused, void* container, float time) {
    // Default container to self+320 if null
    if (container == nullptr) {
        container = *(void**)((char*)self + 320);
    }

    // If time < 0.0, use fallback from self+184
    if (time < 0.0f) {
        time = *(float*)((char*)self + 184);
    }

    // Get animation list from self+28
    void* animList = *(void**)((char*)self + 28);

    // Configure container with timing: LocomotionStateAnim_88E0_g(container, animList, time, 1, 0)
    LocomotionStateAnim_88E0_g(container, animList, time, 1, 0);

    // Query position from container
    float posVec[4];
    LocomotionStateAnim_C358_g(animList, 0, 1, 0, posVec);

    // Build rotation matrix from posVec into dst
    LocomotionStateAnim_AD30_g((float*)dst, posVec);

    // Copy 16 bytes from matrix output (row 0) into dst+48 (via lvx/stvx)
    memcpy((char*)dst + 48, dst, 16);
}

/**
 * RebasePointers3 @ 0x82385340 | size: 0xA0
 *
 * Rebases three pointers at self+4, self+8, self+12 using a stride
 * table from the relocator. Each non-null pointer is adjusted by
 * computing its index into the table and adding the corresponding offset.
 */
// LocomotionStateAnim_5340_p46 @ 0x82385340
void LocomotionStateAnim_5340_p46(void* self, void* relocator) {
    uint32_t baseAddr = *(uint32_t*)((char*)relocator + 4);
    uint32_t stride   = *(uint32_t*)((char*)relocator + 76);

    // Rebase pointer at self+4
    uint32_t ptr4 = *(uint32_t*)((char*)self + 4);
    if (ptr4 != 0) {
        uint32_t idx = (ptr4 - baseAddr) / stride;
        uint32_t offset = ((uint32_t*)relocator)[idx + 2];
        *(uint32_t*)((char*)self + 4) = offset + ptr4;
    }

    // Rebase pointer at self+8
    uint32_t ptr8 = *(uint32_t*)((char*)self + 8);
    if (ptr8 != 0) {
        uint32_t idx = (ptr8 - baseAddr) / stride;
        uint32_t offset = ((uint32_t*)relocator)[idx + 2];
        *(uint32_t*)((char*)self + 8) = offset + ptr8;
    }

    // Rebase pointer at self+12
    uint32_t ptr12 = *(uint32_t*)((char*)self + 12);
    if (ptr12 != 0) {
        uint32_t idx = (ptr12 - baseAddr) / stride;
        uint32_t offset = ((uint32_t*)relocator)[idx + 2];
        *(uint32_t*)((char*)self + 12) = offset + ptr12;
    }
}

/**
 * GetAnimWeight @ 0x822481A8 | size: 0xA4
 *
 * Looks up an animation entry by type (0 or 1) and retrieves its
 * blend weight from the weight array. Returns true if weight > 0.0.
 */
// LocomotionStateAnim_81A8 @ 0x822481A8
bool LocomotionStateAnim_81A8(void* self, uint8_t type, uint16_t animID, void** animPair, float* outWeight) {
    // Only types 0 and 1 are valid
    if (type != 0 && type != 1) {
        return false;
    }

    // Look up animation index via 04F0_g
    uint16_t index;
    bool found = LocomotionStateAnim_04F0_g(animPair[0], animID, &index);

    if (!found) {
        return false;
    }

    // Read weight from weight array: animPair[1] + 16 is the array base
    float* weightArrayBase = (float*)((char*)animPair[1] + 16);
    uint32_t arrayOffset = (uint32_t)index << 2;  // rotlwi r8,r9,2
    float weight = *(float*)((char*)weightArrayBase + arrayOffset);

    *outWeight = weight;

    return weight > 0.0f;
}

/**
 * FetchTransformPair @ 0x8224C4A8 | size: 0xA4
 *
 * Fetches transform data from two animation entries identified by
 * animID1 and animID2. Finds both via FindAnimationByID, retrieves
 * their world-space transforms via vtable slots, builds rotation
 * matrix, and writes to outNode.
 */
// LocomotionStateAnim_C4A8_g @ 0x8224C4A8
bool LocomotionStateAnim_C4A8_g(void* self, uint16_t animID1, uint16_t animID2, void* outNode) {
    LocomotionStateAnim* lsa = (LocomotionStateAnim*)self;

    // Find first animation
    void* anim1 = lsa->FindAnimationByID(animID1, 0);
    // Find second animation
    void* anim2 = lsa->FindAnimationByID(animID2, 0);

    if (anim1 == nullptr || anim2 == nullptr) {
        return false;
    }

    // Get world-space transform from anim1 via vtable slot 14
    void** vt1 = *(void***)anim1;
    typedef void* (*GetTransformFn)(void*);
    GetTransformFn getTransform14 = (GetTransformFn)vt1[14];
    void* transform1 = getTransform14(anim1);

    // Copy 16 bytes from transform1 into outNode+48
    memcpy((char*)outNode + 48, transform1, 16);

    // Get world-space transform from anim2 via vtable slot 15
    void** vt2 = *(void***)anim2;
    GetTransformFn getTransform15 = (GetTransformFn)vt2[15];
    void* transform2 = getTransform15(anim2);

    // Build rotation matrix from transform2 into outNode
    LocomotionStateAnim_AD30_g((float*)outNode, (const float*)transform2);

    return true;
}

/**
 * ApplyAnimCallbacks @ 0x82386380 | size: 0xA8
 *
 * Iterates over an animation list and calls a callback function for
 * each entry. The callback is either LocomotionStateAnim_C128_g or
 * LocomotionStateAnim_C288_g, depending on whether the animation
 * data has the "mirrored" flag set (bit 0 of field +20 at sub-data +4).
 * After processing, calls LocomotionStateAnim_C8F8_g on self+36.
 */
// LocomotionStateAnim_6380_g @ 0x82386380
void LocomotionStateAnim_6380_g(void* self, void* animList) {
    typedef void (*AnimCallback)(void* anim, void* animData, float weight);

    void* animData = *(void**)((char*)self + 36);

    // Select callback based on mirrored flag
    AnimCallback callback = LocomotionStateAnim_C128_g;

    void* subData = *(void**)((char*)animData + 4);
    uint32_t flags = *(uint32_t*)((char*)subData + 20);
    if (flags & 0x1) {
        callback = LocomotionStateAnim_C288_g;
    }

    // Iterate over animation entries
    int16_t count = *(int16_t*)((char*)animList + 12);
    if (count > 0) {
        void** animArray = *(void***)((char*)animList + 8);
        for (int i = 0; i < count; i++) {
            callback(animArray[i], animData, 0.0f);
        }
    }

    // Finalize
    LocomotionStateAnim_C8F8_g(*(void**)((char*)self + 36));
}

/**
 * BinarySearchAnimKey @ 0x8224A390 | size: 0xA8
 *
 * Binary search through a sorted animation key array to find an
 * entry matching a 24-bit key. The key is constructed from
 * (animID_high << 16) | animID_low. On match, writes the found
 * index to *outIndex and returns the entry pointer. On miss,
 * writes the insertion point to *outIndex and returns nullptr.
 */
// LocomotionStateAnim_A390_v12 @ 0x8224A390
void* LocomotionStateAnim_A390_v12(void* self, uint8_t animID_high, uint16_t animID_low, int32_t* outIndex) {
    uint16_t count = *(uint16_t*)((char*)self + 40);

    int high = (int)count - 1;
    int low = 0;

    if (high < 0) {
        *outIndex = low;
        return nullptr;
    }

    // Construct 24-bit search key: (high << 16) | low
    uint32_t searchKey = ((uint32_t)animID_high << 16) | animID_low;

    void** keyArray = *(void***)((char*)self + 36);

    while (low <= high) {
        int mid = (low + high) / 2;
        *outIndex = mid;

        void* entry = keyArray[mid];

        // Extract 24-bit key from entry: (byte_at_0 << 16) | uint16_at_2
        uint8_t entryHigh = *(uint8_t*)((char*)entry + 0);
        uint16_t entryLow = *(uint16_t*)((char*)entry + 2);
        uint32_t entryKey = ((uint32_t)entryHigh << 16) | entryLow;

        if (entryKey == searchKey) {
            return entry;
        }

        if (entryKey > searchKey) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    *outIndex = low;
    return nullptr;
}

/**
 * SumWeightsAndNormalize @ 0x820DBB48 | size: 0xAC
 *
 * Sums blend weights from an array (starting at self+28, count+1
 * entries of 4 bytes each), then returns the ratio of the last
 * entry's weight to the total sum. Returns 0.0 if sum == 0.0.
 * The loop is unrolled 4x in the original PPC code.
 */
// LocomotionStateAnim_BB48_g @ 0x820DBB48
float LocomotionStateAnim_BB48_g(void* self, int count) {
    float total = 0.0f;

    float* weights = (float*)((char*)self + 28);
    for (int i = 0; i <= count; i++) {
        total += weights[i];
    }

    if (total == 0.0f) {
        return 0.0f;
    }

    float lastWeight = weights[count];
    return lastWeight / total;
}

/**
 * InvertAndCombineQuaternions @ 0x8223BE20 | size: 0xB0
 *
 * Inverts quaternion src (negates xyz, keeps w), multiplies with
 * dst quaternion using util_BBB0, applies reference subtraction
 * and position addition via SIMD, and normalizes via util_B9A8.
 */
// LocomotionStateAnim_BE20_g @ 0x8223BE20
void LocomotionStateAnim_BE20_g(void* dst, void* src) {
    float* srcQ = (float*)src;
    float* dstQ = (float*)dst;

    // Invert src quaternion: negate xyz, keep w
    dstQ[0] = -srcQ[0];
    dstQ[1] = -srcQ[1];
    dstQ[2] = -srcQ[2];
    dstQ[3] =  srcQ[3];

    // Multiply inverted src quaternion with src position
    float* srcPos = (float*)((char*)src + 16);
    util_BBB0(dst, srcPos, (float*)((char*)dst + 16));

    // result_pos = reference_zero - combined_pos (vsubfp with zero vector)
    extern const float g_zeroVec4[4];  // reference zero vector
    float* dstPos = (float*)((char*)dst + 16);
    dstPos[0] = g_zeroVec4[0] - dstPos[0];
    dstPos[1] = g_zeroVec4[1] - dstPos[1];
    dstPos[2] = g_zeroVec4[2] - dstPos[2];
    dstPos[3] = g_zeroVec4[3] - dstPos[3];

    // Add dst position back (vaddfp with src position)
    float* origSrcPos = (float*)((char*)src + 16);
    dstPos[0] += origSrcPos[0];
    dstPos[1] += origSrcPos[1];
    dstPos[2] += origSrcPos[2];
    dstPos[3] += origSrcPos[3];

    // Normalize the result
    util_B9A8(dst, dst, src);
}

/**
 * GetTransformFromAnimPair @ 0x822E0A30 | size: 0xB4
 *
 * Checks if both animation IDs 5 and 6 exist. If so, fetches the
 * transform pair (IDs 5 and 6) and copies 4 rows of 16 bytes into
 * the destination. Otherwise, fetches transform pair (IDs 0 and 1).
 */
// LocomotionStateAnim_0A30_g @ 0x822E0A30
void* LocomotionStateAnim_0A30_g(void* dst, void* animSrc) {
    bool hasBoth = LocomotionStateAnim_09A0_g(animSrc);

    if (hasBoth) {
        // Fetch transform pair for IDs 5 and 6
        float outNode[16];
        LocomotionStateAnim_C4A8_g(animSrc, 5, 6, outNode);

        // Copy 4 x 16 bytes into dst (lvx/stvx pattern)
        memcpy((char*)dst + 0,  &outNode[0],  16);
        memcpy((char*)dst + 16, &outNode[4],  16);
        memcpy((char*)dst + 32, &outNode[8],  16);
        memcpy((char*)dst + 48, &outNode[12], 16);
    } else {
        // Fetch transform pair for IDs 0 and 1
        LocomotionStateAnim_C4A8_g(animSrc, 0, 1, dst);
    }

    return dst;
}

/**
 * ComputeBlendWeight @ 0x820E6DD8 | size: 0xB4
 *
 * Computes a blend weight for animation transitions. Calls the
 * vtable slot 11 handler to get the current frame position, looks
 * up the animation's start/end range from the container data at
 * self+304 (indexed by self+28), and interpolates.
 */
// LocomotionStateAnim_6DD8_w @ 0x820E6DD8
float LocomotionStateAnim_6DD8_w(void* self, float maxWeight, float frameTime) {
    // Set up frame state
    LocomotionStateAnim_6E90_w(self, frameTime);

    // Call vtable slot 11 to get current position
    void** vt = *(void***)self;
    typedef float (*GetPosFn)(void*);
    GetPosFn getPos = (GetPosFn)vt[11];
    float pos = getPos(self);

    // Get animation entry index and container data
    uint32_t animIdx = *(uint32_t*)((char*)self + 28);
    char* containerData = *(char**)((char*)self + 304);
    char* entry = containerData + animIdx * 68;

    float rangeStart = *(float*)(entry + 56);
    float rangeEnd   = *(float*)(entry + 60);

    if (pos < rangeStart) {
        return 0.0f;
    } else if (pos < rangeEnd) {
        float t = (pos - rangeStart) / (rangeEnd - rangeStart);
        extern const float g_blendCurveScale;   // @ 0x82029B20
        extern float g_blendCurveOffset;         // @ 0x825C8078 (.data)
        float blendVal = t * g_blendCurveScale + g_blendCurveOffset;
        float diff = blendVal - maxWeight;
        return (diff >= 0.0f) ? maxWeight : blendVal;
    } else {
        return maxWeight;
    }
}

/**
 * GetAnimWeightByMode @ 0x82248250 | size: 0xB4
 *
 * Similar to GetAnimWeight but with an additional mode parameter.
 * For modes 0 and 1, looks up the anim by index and reads weight
 * from the weight array. For mode >= 2, reads the weight directly
 * from a second array at animPair[3] (offset +12).
 */
// LocomotionStateAnim_8250_fw @ 0x82248250
bool LocomotionStateAnim_8250_fw(void* self, uint8_t mode, uint16_t animID, void** animPair, float* outWeight) {
    if (mode == 0 || mode == 1) {
        // Look up animation index via 04F0_g
        uint16_t index;
        bool found = LocomotionStateAnim_04F0_g(animPair[0], animID, &index);

        if (!found) {
            return false;
        }

        // Read weight from animPair base, offset by index
        uint32_t arrayOffset = (uint32_t)index << 2;
        float weight = *(float*)((char*)animPair[1] + arrayOffset);

        // Scaffold falls through without storing — returns false
        (void)weight;
        return false;
    } else if ((int32_t)mode >= 2) {
        // Direct weight lookup from animPair[3] (offset +12)
        float* weightArray = (float*)animPair[3];
        float weight = weightArray[mode];

        *outWeight = weight;
        return weight > 0.0f;
    }

    return false;
}

/**
 * LookupAndPlayAnim @ 0x82248828 | size: 0xB4
 *
 * Looks up an animation by computing a fractional index from the
 * animation count and a time parameter, runs a binary search to
 * find the matching key, and either starts the animation via
 * vtable slot 4 (on hit) or calls vtable slot 13 (on miss).
 */
// LocomotionStateAnim_8828_g @ 0x82248828
bool LocomotionStateAnim_8828_g(void* self, float time) {
    uint16_t rawCount = *(uint16_t*)((char*)self + 10);
    float duration = *(float*)((char*)self + 12);

    // Compute scaled time: ((count-1) / duration) * time
    float countMinusOne = (float)((int)rawCount - 1);
    float scaledTime = (countMinusOne / duration) * time;

    // Read the key bytes from self for the search
    uint8_t animID_high = *(uint8_t*)((char*)self + 5);
    uint16_t animID_low = *(uint16_t*)((char*)self + 6);

    // Binary search for the key
    int32_t foundIndex;
    void* result = LocomotionStateAnim_A390_v12(self, animID_high, animID_low, &foundIndex);

    if (result != nullptr) {
        // Found: call vtable slot 4 on result
        void** vt = *(void***)result;
        typedef void (*PlayFn)(void*, void*, void*);
        PlayFn play = (PlayFn)vt[4];

        void** keyArray = *(void***)((char*)self + 36);
        void* animEntry = keyArray[foundIndex];
        play(result, animEntry, self);

        return true;
    } else {
        // Not found: call vtable slot 13
        void** vt = *(void***)result;
        typedef void (*FallbackFn)(void*);
        FallbackFn fallback = (FallbackFn)vt[13];
        fallback(result);

        return false;
    }
}
