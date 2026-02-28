/**
 * pong_player_swing.cpp — Player swing mechanics and input processing
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * This file contains the core swing mechanics functions that were previously
 * referenced by hex identifiers. These functions handle:
 * - Swing readiness checks
 * - Swing strength and target calculation
 * - Player position prediction
 * - Swing input processing
 * - Creature bone matrix retrieval
 *
 * All functions have been decompiled from PowerPC assembly and reconstructed
 * to match the original Rockstar implementation intent.
 */

#include "pong_player.hpp"
#include <cmath>

// Forward declarations for external dependencies
extern void* g_input_obj_ptr;
extern float g_kZero;
extern float g_kOne;
extern const float g_swingStrengthMin;  // @ 0x8202D110
extern const float g_swingStrengthMax;  // @ 0x8202D10C
extern const float g_heightScaleFactor; // @ 0x8204A3B8
extern const float g_depthScaleFactor;  // @ 0x8204A3BC

// External function dependencies
extern int io_Input_poll_9D68(void* inputObj);
extern bool pongPlayer_C3C8_g(pongPlayerState* state);  // CanAcceptSwingInput
extern void pg_C7A0_g(float value);
extern void game_AF18(void* output, void* physBound, vec3* velocity, vec3* position);

/**
 * IsPlayerReadyToSwing @ 0x820D6AA0 | size: 0xEC
 *
 * Checks if the player is in a valid state to execute a swing action.
 * This function performs multiple condition checks including animation progress,
 * state flags, position validation, and locomotion readiness.
 *
 * @param locoState Pointer to player's locomotion state structure
 * @return true if all swing readiness conditions are met, false otherwise
 *
 * Conditions checked:
 * 1. Animation progress (float at +128) must exceed threshold
 * 2. State flag bit 0 at +312 must be set
 * 3. Position value (float at +288) must be non-negative
 * 4. Both state bytes at +138 and +136 must be non-zero
 * 5. State flag at +312 must have either bit 2 or bit 3 clear
 */
bool IsPlayerReadyToSwing(void* locoState) {
    if (!locoState) return false;
    
    uint8_t* stateBytes = (uint8_t*)locoState;
    float* stateFloats = (float*)locoState;
    
    // Check animation progress threshold
    float animProgress = stateFloats[128 / 4];  // +128 bytes = +32 floats
    if (animProgress <= g_swingStrengthMin) {
        return false;
    }
    
    // Check primary state flag (bit 0)
    uint8_t stateFlags = stateBytes[312];
    if ((stateFlags & 0x01) == 0) {
        return false;
    }
    
    // Check position validity
    float positionValue = stateFloats[288 / 4];  // +288 bytes = +72 floats
    if (positionValue < 0.0f) {
        return false;
    }
    
    // Check both state bytes are active
    uint8_t stateByte138 = stateBytes[138];
    uint8_t stateByte136 = stateBytes[136];
    if (stateByte138 == 0 || stateByte136 == 0) {
        return false;
    }
    
    // Check that either bit 2 or bit 3 is clear (not both set)
    bool bit2Set = (stateFlags & 0x04) != 0;
    bool bit3Set = (stateFlags & 0x08) != 0;
    
    // If both bits are set, player is not ready
    if (bit2Set && bit3Set) {
        return false;
    }
    
    return true;
}

/**
 * CalculateSwingStrengthAndTarget @ 0x820D42E0 | size: 0x14C
 *
 * Calculates the swing strength and target direction vector for a player's swing action.
 * Uses interpolation based on swing type and applies creature-specific transformations.
 *
 * @param swingData Pointer to pongSwingData structure containing swing parameters
 * @param outStrength Output pointer for calculated swing strength (single float)
 * @param outTarget Output pointer for calculated target direction vector (vec3)
 * @param alpha Interpolation factor (0.0 to 1.0)
 *
 * Swing types:
 * - Type 0: Uses default minimum strength
 * - Type 9: Calls virtual function and clamps result
 * - Other: Uses alpha directly (or 1.0 - alpha based on sign)
 */
void CalculateSwingStrengthAndTarget(void* swingData, float* outStrength, vec3* outTarget, float alpha) {
    if (!swingData || !outStrength || !outTarget) return;
    
    uint32_t* swingDataWords = (uint32_t*)swingData;
    float* swingDataFloats = (float*)swingData;
    vec3* swingDataVecs = (vec3*)swingData;
    
    // Read swing type from +36
    int32_t swingType = (int32_t)swingDataWords[36 / 4];
    
    // Calculate strength based on swing type
    float strength;
    float finalAlpha = (alpha >= 0.0f) ? alpha : (1.0f - alpha);
    
    if (swingType == 0) {
        // Default case: use minimum strength
        strength = g_swingStrengthMax;
    } else if (swingType == 9) {
        // Special case: call virtual function slot 10 and clamp
        void* creatureData = (void*)swingDataWords[192 / 4];
        if (creatureData) {
            void** vtable = *(void***)creatureData;
            float (*vfunc)(void*) = (float(*)(void*))vtable[10];
            float rawStrength = vfunc(creatureData);
            
            // Clamp between finalAlpha and rawStrength
            if (rawStrength >= 0.0f) {
                strength = (rawStrength > finalAlpha) ? finalAlpha : rawStrength;
            } else {
                strength = finalAlpha;
            }
        } else {
            strength = finalAlpha;
        }
    } else {
        // Standard case: use alpha directly
        strength = finalAlpha;
    }
    
    *outStrength = strength;
    
    // Calculate target direction vector
    // Get base direction vector from +384
    vec3 baseDirection = swingDataVecs[384 / 16];
    
    // Get creature data from +336
    void* creatureData = (void*)swingDataWords[336 / 4];
    if (creatureData) {
        uint32_t* creatureWords = (uint32_t*)creatureData;
        
        // Get bone matrix data from creature +164
        void* boneData = (void*)creatureWords[164 / 4];
        if (boneData) {
            float* boneFloats = (float*)boneData;
            
            // Get bone position offset (+64 from bone data)
            vec3 boneOffset;
            boneOffset.x = boneFloats[64 / 4 + 0];
            boneOffset.y = boneFloats[64 / 4 + 1];
            boneOffset.z = boneFloats[64 / 4 + 2];
            
            // Apply strength scaling to base direction
            vec3 scaledDirection;
            scaledDirection.x = baseDirection.x * strength;
            scaledDirection.y = baseDirection.y * strength;
            scaledDirection.z = baseDirection.z * strength;
            
            // Add bone offset to get final target
            outTarget->x = boneOffset.x + scaledDirection.x;
            outTarget->y = boneOffset.y + scaledDirection.y;
            outTarget->z = boneOffset.z + scaledDirection.z;
            
            // Clamp target to valid range
            float minBound = g_swingStrengthMin;
            float maxBound = g_swingStrengthMax;
            
            outTarget->x = (outTarget->x < minBound) ? minBound : 
                          (outTarget->x > maxBound) ? maxBound : outTarget->x;
            outTarget->z = (outTarget->z < minBound) ? minBound :
                          (outTarget->z > maxBound) ? maxBound : outTarget->z;
        }
    }
}

/**
 * UpdatePlayerSwingInput @ 0x820C73E8 | size: 0x144
 *
 * Processes player swing input and updates swing state based on input polling results.
 * This function is called during the swing input polling loop to handle player actions.
 *
 * @param player Pointer to pongPlayer structure
 * @param inputValue Float value representing input magnitude or timing
 */
void UpdatePlayerSwingInput(pongPlayer* player, float inputValue) {
    if (!player) return;
    
    pongPlayerState* playerState = player->m_pPlayerState;
    if (!playerState) return;
    
    // Check if player can accept swing input
    if (!pongPlayer_C3C8_g(playerState)) {
        return;
    }
    
    // Poll input system
    int inputResult = io_Input_poll_9D68(g_input_obj_ptr);
    
    if (inputResult == 0) {
        // Active input detected - calculate swing parameters
        pongSwingData* swingData = playerState->m_pSwingData;
        if (swingData) {
            float swingStrength;
            vec3 swingTarget;
            
            // Calculate swing with interpolation factor
            const float interpolationFactor = 0.5f;  // From 0x8202CFE8
            CalculateSwingStrengthAndTarget(swingData, &swingStrength, &swingTarget, interpolationFactor);
            
            // Update swing data
            swingData->m_swingStrength = swingStrength;
            swingData->m_bTriggered = 1;
            
            // Copy target vector
            swingData->m_swingVec[0] = swingTarget.x;
            swingData->m_swingVec[1] = swingTarget.y;
            swingData->m_swingVec[2] = swingTarget.z;
        }
    } else {
        // No active input - check for state updates
        void* gameState = player->m_pGameState;
        if (gameState) {
            uint32_t* gameWords = (uint32_t*)gameState;
            void* innerState = (void*)gameWords[112 / 4];
            if (innerState) {
                uint32_t* innerWords = (uint32_t*)innerState;
                void* animState = (void*)innerWords[188 / 4];
                if (animState) {
                    uint32_t* animWords = (uint32_t*)animState;
                    pongSwingData* swingData = (pongSwingData*)animWords[148 / 4];
                    if (swingData) {
                        swingData->_pad341 = 1;  // Set alternate flag
                    }
                }
            }
        }
    }
    
    // Final state check and animation update
    if (pongPlayer_C3C8_g(playerState)) {
        pongAnimState* animState = playerState->m_pAnimState;
        if (animState && swingStrength > g_kZero) {
            animState->m_swingPhase = swingStrength;
        } else {
            pg_C7A0_g(inputValue);
        }
    }
}

// Alias for backward compatibility
extern "C" void pongPlayer_73E8_g(pongPlayer* p, float f) {
    UpdatePlayerSwingInput(p, f);
}

// Alias for backward compatibility  
extern "C" void pongPlayer_42E0_g(void* swingData, vec3* outStrength, vec3* outTarget, float alpha) {
    float strength;
    CalculateSwingStrengthAndTarget(swingData, &strength, outTarget, alpha);
    outStrength->x = strength;
}

// Alias for backward compatibility
extern "C" bool pongPlayer_6AA0_g(void* locoState) {
    return IsPlayerReadyToSwing(locoState);
}

/**
 * GetCreatureBoneMatrix @ 0x821C9CD0 | size: 0x94
 *
 * Retrieves transformation matrices for a creature's bone at a specific index.
 * Calculates two output matrices based on creature height, position, and side.
 *
 * @param creatureData Pointer to creature data structure
 * @param boneIndex Index of the bone to retrieve (added to base creature pointer)
 * @param outMatrix1 First output transformation matrix (vec3)
 * @param outMatrix2 Second output transformation matrix (vec3)
 */
void GetCreatureBoneMatrix(void* creatureData, int boneIndex, vec3* outMatrix1, vec3* outMatrix2) {
    if (!creatureData || !outMatrix1 || !outMatrix2) return;
    
    float* creatureFloats = (float*)creatureData;
    uint8_t* creatureBytes = (uint8_t*)creatureData;
    
    // Set Y components to default value
    outMatrix1->y = g_swingStrengthMax;
    outMatrix2->y = g_swingStrengthMax;
    
    // Get creature height from +128
    float creatureHeight = creatureFloats[128 / 4];
    
    // Calculate X components with height scaling
    outMatrix1->x = creatureHeight * g_heightScaleFactor;
    outMatrix2->x = creatureHeight * g_depthScaleFactor;
    
    // Get global table height offset
    extern void* g_pGlobalTableData;
    float* tableData = (float*)g_pGlobalTableData;
    float tableHeight = tableData[24 / 4];
    float heightOffset = tableHeight * g_depthScaleFactor;
    
    // Check creature side flag at +64 (relative to boneIndex offset)
    uint8_t* boneData = creatureBytes + boneIndex;
    bool isRightSide = (boneData[64] != 0);
    
    // Get creature position offsets
    float posY1 = creatureFloats[136 / 4];
    float posY2 = creatureFloats[132 / 4];
    
    if (isRightSide) {
        // Right side: add height offset
        outMatrix1->z = posY1 + heightOffset;
        outMatrix2->z = posY2 + heightOffset;
    } else {
        // Left side: subtract height offset
        outMatrix2->z = -heightOffset - posY1;
        outMatrix1->z = outMatrix2->z - posY2;
    }
}

/**
 * PredictPlayerPosition @ 0x820CAB48 | size: 0x130
 *
 * Predicts the future position of a player based on current physics state,
 * velocity, and swing data. Used for AI targeting and swing calculations.
 *
 * @param outPosition Output vector for predicted position
 * @param physicsBound Pointer to physics bound structure (phBoundCapsule)
 */
void PredictPlayerPosition(vec3* outPosition, void* physicsBound) {
    if (!outPosition || !physicsBound) return;
    
    uint32_t* boundWords = (uint32_t*)physicsBound;
    float* boundFloats = (float*)physicsBound;
    
    // Get player state from physics bound
    void* playerState = (void*)boundWords[112 / 4];
    if (!playerState) {
        // No player state - use default position from bound
        uint32_t* stateWords = (uint32_t*)playerState;
        void* boneData = (void*)stateWords[164 / 4];
        if (boneData) {
            float* boneFloats = (float*)boneData;
            outPosition->x = boneFloats[64 / 4 + 0];
            outPosition->y = boneFloats[64 / 4 + 1];
            outPosition->z = boneFloats[64 / 4 + 2];
        }
        return;
    }
    
    uint32_t* playerWords = (uint32_t*)playerState;
    
    // Check if swing data exists
    void* animState = (void*)playerWords[188 / 4];
    if (!animState) {
        // No animation state - use creature bone position
        void* boneData = (void*)playerWords[164 / 4];
        if (boneData) {
            float* boneFloats = (float*)boneData;
            outPosition->x = boneFloats[64 / 4 + 0];
            outPosition->y = boneFloats[64 / 4 + 1];
            outPosition->z = boneFloats[64 / 4 + 2];
        }
        return;
    }
    
    uint32_t* animWords = (uint32_t*)animState;
    pongSwingData* swingData = (pongSwingData*)animWords[148 / 4];
    
    if (swingData) {
        // Has swing data - calculate predicted position with velocity
        void* boneData = (void*)playerWords[164 / 4];
        if (boneData) {
            float* boneFloats = (float*)boneData;
            
            // Start with bone position
            outPosition->x = boneFloats[64 / 4 + 0];
            outPosition->y = boneFloats[64 / 4 + 1];
            outPosition->z = boneFloats[64 / 4 + 2];
            
            // Get velocity from physics bound
            vec3 velocity;
            velocity.x = boundFloats[48 / 4];
            velocity.y = g_kZero;
            velocity.z = boundFloats[52 / 4];
            
            // Calculate position delta
            vec3 positionDelta;
            game_AF18(&positionDelta, physicsBound, &velocity, outPosition);
            
            // Add delta to current position
            outPosition->x += positionDelta.x;
            outPosition->y += positionDelta.y;
            outPosition->z += positionDelta.z;
        }
    } else {
        // No swing data - use bone position directly
        void* boneData = (void*)playerWords[164 / 4];
        if (boneData) {
            float* boneFloats = (float*)boneData;
            outPosition->x = boneFloats[64 / 4 + 0];
            outPosition->y = boneFloats[64 / 4 + 1];
            outPosition->z = boneFloats[64 / 4 + 2];
        }
    }
    
    // Get creature index and apply bone matrix transformation
    int creatureIndex = (int)boundWords[68 / 4];
    vec3 boneMatrix1, boneMatrix2;
    GetCreatureBoneMatrix(g_input_obj_ptr, creatureIndex, &boneMatrix1, &boneMatrix2);
    
    // Clamp position to valid bounds
    float minX = g_swingStrengthMin;
    float maxX = g_swingStrengthMax;
    float minZ = g_swingStrengthMin;
    float maxZ = g_swingStrengthMax;
    
    // Clamp X
    if (outPosition->x < boneMatrix1.x) {
        outPosition->x = (outPosition->x < minX) ? minX : outPosition->x;
    } else {
        outPosition->x = (outPosition->x > maxX) ? maxX : outPosition->x;
    }
    
    // Clamp Z
    if (outPosition->z < boneMatrix1.z) {
        outPosition->z = (outPosition->z < minZ) ? minZ : outPosition->z;
    } else {
        outPosition->z = (outPosition->z > maxZ) ? maxZ : outPosition->z;
    }
}

// Backward compatibility aliases
extern "C" void pongPlayer_9CD0_g(void* creatureData, int index, void* outMatrix1, void* outMatrix2) {
    GetCreatureBoneMatrix(creatureData, index, (vec3*)outMatrix1, (vec3*)outMatrix2);
}

extern "C" void pongPlayer_AB48_g(vec3* out, void* physBound) {
    PredictPlayerPosition(out, physBound);
}
