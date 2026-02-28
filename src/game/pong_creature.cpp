

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
