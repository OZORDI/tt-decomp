/**
 * anim_blender_init.cpp — Animation Blender State Initialization
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Initializes/resets the animation blender sub-structure used by:
 * - pcrEmoteBlender (emote/reaction animations)
 * - pcrPostPointBlender (post-point sequences)
 * - LocomotionStateAnim (locomotion blending)
 */

#include <stdint.h>
#include <string.h>

// External function pointers for vtable-like initialization
extern void nop_8240E6D0(const void*, const void*, uint32_t);  // @ 0x8240E6D0 - no-op stub
extern void atSingleton_7BC0();                                 // @ 0x821E7BC0 - singleton helper

// Global constant data
extern const float g_animBlenderInitVec[4];      // @ 0x8261A0C0 - initial vector data
extern const float g_animBlenderConstants[3];    // @ 0x8202D108 - blend constants

/**
 * crAnimBlenderState — Animation blender sub-structure
 * 
 * Embedded in pcrEmoteBlender and pcrPostPointBlender at offset +0x10.
 * Size: 160 bytes (0xA0)
 */
struct crAnimBlenderState {
    // +0x00..+0x0F: base fields (not initialized by this function)
    uint8_t      _pad00[16];
    
    // +0x10: Initial state vector (copied from global)
    float        m_initVec[4];              // +0x10
    
    // +0x20: Function pointer pair 1
    void*        m_fnPtr1;                  // +0x20
    void*        m_fnPtr2;                  // +0x24
    void*        m_fnPtr3;                  // +0x28
    void*        m_fnPtr4;                  // +0x2C
    
    // +0x30: Function pointer pair 2
    void*        m_fnPtr5;                  // +0x30
    void*        m_fnPtr6;                  // +0x34
    void*        m_fnPtr7;                  // +0x38
    void*        m_fnPtr8;                  // +0x3C
    
    // +0x40: State counters
    uint32_t     m_counter1;                // +0x40
    uint32_t     m_counter2;                // +0x44
    uint16_t     m_stateFlags1;             // +0x48
    uint16_t     m_stateFlags2;             // +0x4A
    
    // +0x50: Blend weights
    float        m_blendWeightA;            // +0x50 (80)
    float        m_blendWeightB;            // +0x54 (84)
    
    // +0x58..+0x77: intermediate fields
    uint8_t      _pad58[32];
    
    // +0x78: Phase/timer values
    float        m_phaseTimer1;             // +0x78 (120)
    float        m_phaseValue1;             // +0x7C (124)
    float        m_phaseTimer2;             // +0x80 (128)
    float        m_phaseValue2;             // +0x84 (132)
    float        m_phaseTimer3;             // +0x88 (136)
    
    // +0x8C: Clip indices
    uint16_t     m_clipIndexFlags;          // +0x8C (140)
    uint16_t     m_activeClipIdx1;          // +0x8E (142) - -1 = none
    uint16_t     m_activeClipIdx2;          // +0x90 (144) - -1 = none
    uint16_t     m_activeClipIdx3;          // +0x92 (146) - -1 = none
    
    // +0x94..+0x97: pad
    uint8_t      _pad94[4];
    
    // +0x98: Final phase values
    float        m_finalPhase1;             // +0x98 (152)
    float        m_finalPhase2;             // +0x9C (156)
};

/**
 * crAnimBlenderState_Init
 * @ 0x8224C810 | size: 0x120
 *
 * Initializes an animation blender sub-structure to its default state.
 * Clears counters, sets blend weights to initial constants, resets all
 * clip indices to -1 (inactive), and installs function pointer pairs.
 *
 * Called by:
 * - pcrEmoteBlender::Reset() @ 0x820DB518
 * - pcrPostPointBlender::ResetState() @ 0x820DDE28
 * - LocomotionStateAnim initialization paths
 */
void crAnimBlenderState_Init(crAnimBlenderState* state)
{
    // Copy initial vector from global constant data
    memcpy(&state->m_initVec, g_animBlenderInitVec, 16);
    
    // Set up function pointer pairs (vtable-like dispatch)
    // These point to singleton helpers and no-op stubs
    state->m_fnPtr1 = (void*)nop_8240E6D0;
    state->m_fnPtr2 = (void*)atSingleton_7BC0;
    state->m_fnPtr3 = nullptr;
    state->m_fnPtr4 = (void*)atSingleton_7BC0;
    
    state->m_fnPtr5 = (void*)nop_8240E6D0;
    state->m_fnPtr6 = (void*)atSingleton_7BC0;
    state->m_fnPtr7 = nullptr;
    state->m_fnPtr8 = (void*)atSingleton_7BC0;
    
    // Clear state counters and flags
    state->m_counter1 = 0;
    state->m_counter2 = 0;
    state->m_stateFlags1 = 0;
    state->m_stateFlags2 = 0;
    
    // Initialize blend weights from constants
    // g_animBlenderConstants[0] = base value (~0.0f or 1.0f)
    // g_animBlenderConstants[2] = blend constant
    state->m_blendWeightA = g_animBlenderConstants[2];
    state->m_blendWeightB = g_animBlenderConstants[2];
    
    // Initialize phase/timer values
    state->m_phaseTimer1 = g_animBlenderConstants[0];
    state->m_phaseValue1 = g_animBlenderConstants[2];
    state->m_phaseTimer2 = g_animBlenderConstants[0];
    state->m_phaseValue2 = g_animBlenderConstants[2];
    state->m_phaseTimer3 = g_animBlenderConstants[0];
    
    // Reset clip indices to -1 (no active clip)
    state->m_clipIndexFlags = 0;
    state->m_activeClipIdx1 = 0xFFFF;  // -1
    state->m_activeClipIdx2 = 0xFFFF;  // -1
    state->m_activeClipIdx3 = 0xFFFF;  // -1
    
    // Initialize final phase values
    state->m_finalPhase1 = g_animBlenderConstants[2];
    state->m_finalPhase2 = g_animBlenderConstants[2];
}



/**
 * pcrAnimBlender_ApplyClipWeights
 * @ 0x820CF420 | size: 0x178
 *
 * Applies blend weights to animation clips in the player's animation system.
 * Iterates through the player's clip array, finds matching clips by name hash,
 * and applies the specified blend weights to each matching clip's blender.
 *
 * Parameters:
 * - pPlayer: Pointer to pongPlayer object
 * - clipNameHash: Hash of the clip name to match
 * - blendWeightA: First blend weight value
 * - blendWeightB: Second blend weight value
 *
 * Called by:
 * - pcrEmoteBlender::Update() @ 0x820DB520
 * - LocomotionStateAnim functions
 * - pongLocomotionController
 */
void pcrAnimBlender_ApplyClipWeights(void* pPlayer, uint32_t clipNameHash, 
                                      float blendWeightA, float blendWeightB)
{
    // Player animation system structure at pPlayer+12 contains:
    // +12: clip array pointer
    // +16: clip count (uint16_t)
    
    struct PlayerAnimSystem {
        uint8_t  _pad00[12];
        void**   m_pClipArray;      // +12
        uint16_t m_clipCount;       // +16
    };
    
    struct AnimClipEntry {
        void*    m_pClipData;       // +0
        void*    m_pNameString;     // +4
        void*    m_pHashTable;      // +8
        uint16_t m_hashTableSize;   // +12
    };
    
    struct AnimClipBlender {
        void*    m_pClipRef;        // +0
        uint8_t  _pad04[20];
        void**   m_pBlenderArray;   // +24
        uint16_t m_blenderCount;    // +28
        uint8_t  m_flags;           // +16
    };
    
    PlayerAnimSystem* animSys = static_cast<PlayerAnimSystem*>(pPlayer);
    
    if (animSys->m_clipCount <= 0) {
        return;
    }
    
    // Iterate through all clips in the player's animation system
    for (int clipIdx = 0; clipIdx < animSys->m_clipCount; clipIdx++) {
        AnimClipEntry* clipEntry = static_cast<AnimClipEntry*>(animSys->m_pClipArray[clipIdx]);
        
        if (clipEntry->m_hashTableSize == 0) {
            continue;
        }
        
        // Hash lookup: find clip by name hash
        // atSingleton_A818_g computes hash bucket index
        extern uint32_t atSingleton_A818_g(uint32_t hash);
        uint32_t bucketIdx = atSingleton_A818_g(clipNameHash) % clipEntry->m_hashTableSize;
        
        // Walk hash chain to find matching clip
        void** hashTable = static_cast<void**>(clipEntry->m_pHashTable);
        void** hashEntry = static_cast<void**>(hashTable[bucketIdx]);
        
        while (hashEntry != nullptr) {
            // Hash entry structure:
            // +0: key pointer (clip name string)
            // +4: value pointer (AnimClipBlender*)
            // +8: next entry in chain
            
            const char* entryName = static_cast<const char*>(hashEntry[0]);
            const char* searchName = reinterpret_cast<const char*>(clipNameHash);
            
            // String comparison
            if (strcmp(entryName, searchName) == 0) {
                // Found matching clip - apply blend weights to all blenders
                AnimClipBlender* blender = static_cast<AnimClipBlender*>(hashEntry[1]);
                
                if (blender != nullptr && blender->m_blenderCount > 0) {
                    for (int blenderIdx = 0; blenderIdx < blender->m_blenderCount; blenderIdx++) {
                        void* blenderObj = blender->m_pBlenderArray[blenderIdx];
                        
                        // Call the blender's apply-weights function
                        // pcrPostPointBlender_E6F8_p46 @ 0x820DE6F8
                        extern uint8_t pcrPostPointBlender_E6F8_p46(void* blenderObj, void* player,
                                                                      float weightA, float weightB);
                        uint8_t result = pcrPostPointBlender_E6F8_p46(blenderObj, pPlayer, 
                                                                        blendWeightA, blendWeightB);
                        
                        // If any blender returns non-zero and the clip has the "stop on complete" flag,
                        // stop processing
                        if (result != 0 && blender->m_flags != 0) {
                            return;
                        }
                    }
                }
                
                return;  // Found and processed the clip
            }
            
            // Move to next entry in hash chain
            hashEntry = static_cast<void**>(hashEntry[2]);
        }
    }
}

/**
 * pcrAnimState_ComputePosition
 * @ 0x820DC938 | size: 0x104
 *
 * Computes the current animation position vector based on the animation state.
 * Handles blend calculations, locomotion state, and player-side mirroring.
 *
 * Parameters:
 * - out: Output vec3 to receive the computed position
 * - animState: Pointer to pongAnimState structure
 *
 * Called by:
 * - pongPlayer_D7B0_g (player position update)
 */
void pcrAnimState_ComputePosition(float* out, void* animState)
{
    struct pongAnimState {
        uint8_t  _pad00[84];
        void*    m_pLocomotionClip;  // +84
        uint16_t m_locomotionState;  // +88
        uint8_t  _pad5A[6];
        float    m_currentPhase;     // +96
        float    m_targetPhase;      // +100
        uint8_t  _pad68[308];
        void*    m_pPlayerSide;      // +408
        float    m_phaseTimer;       // +412
    };
    
    pongAnimState* state = static_cast<pongAnimState*>(animState);
    
    // Load zero constant
    extern const float g_animBlenderConstants[3];  // @ 0x8202D108
    float zero = g_animBlenderConstants[0];
    
    // Initialize output to zero
    out[0] = zero;
    out[1] = zero;
    out[2] = zero;
    
    // Check if phase timer is positive (animation is active)
    if (state->m_phaseTimer > zero) {
        return;  // Early exit - no position update needed
    }
    
    // Initialize temp vector to zero
    float tempVec[4] = {zero, zero, zero, 0.0f};
    
    // Check locomotion state and compute position if needed
    if (state->m_locomotionState == 1 && state->m_pLocomotionClip != nullptr) {
        // Compute blend delta
        float phaseDelta = state->m_currentPhase - state->m_targetPhase;
        
        // Get clip duration
        struct AnimClip {
            uint8_t _pad00[12];
            float   m_duration;  // +12
        };
        AnimClip* clip = static_cast<AnimClip*>(state->m_pLocomotionClip);
        float duration = clip->m_duration;
        
        // Compute weighted phase values
        float phase1 = duration * state->m_currentPhase;
        float phase2 = phaseDelta * duration;
        
        // Call locomotion position calculator
        // LocomotionStateAnim_8C58_g @ 0x82248C58
        extern void LocomotionStateAnim_8C58_g(void* clip, float phase1, float phase2, 
                                                float* outVec);
        LocomotionStateAnim_8C58_g(state->m_pLocomotionClip, phase1, phase2, tempVec);
    }
    
    // Copy computed vector to output
    out[0] = tempVec[0];
    out[1] = tempVec[1];
    out[2] = tempVec[2];
    
    // Check player-side mirroring flag
    extern uint32_t g_playerSideMirrorFlags;  // SDA @ -21720
    uint32_t sideOffset = reinterpret_cast<uint32_t>(state->m_pPlayerSide) + g_playerSideMirrorFlags;
    uint8_t mirrorFlag = *reinterpret_cast<uint8_t*>(sideOffset + 64);
    
    if (mirrorFlag != 0) {
        return;  // No mirroring needed
    }
    
    // Apply mirroring: subtract from mirror plane
    extern const float g_animMirrorPlane[4];  // @ 0x821F67B0
    out[0] = g_animMirrorPlane[0] - tempVec[0];
    out[1] = g_animMirrorPlane[1] - tempVec[1];
    out[2] = g_animMirrorPlane[2] - tempVec[2];
}
