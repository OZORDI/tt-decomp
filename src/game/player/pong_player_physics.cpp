/**
 * pong_player_physics.cpp
 * 
 * Player locomotion and recovery implementation for Rockstar Table Tennis.
 * 
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "game/pong_player_physics.hpp"

// External dependencies
extern bool CheckPlayerState(LocomotionState* state);
extern void ApplyLocomotionState(LocomotionState* state);
extern void TriggerGameEvent(uint32_t eventId, uint32_t flags, uint32_t p1, uint32_t p2);
extern void TriggerAudioEvent(void* audioSystem);

// Globals
extern float g_recoveryThreshold;
extern int32_t g_audioEnabled;
extern void* g_audioSystem;

/**
 * ClearLocomotionState @ 0x820DCD20
 * 
 * Resets locomotion state to neutral. Called when canceling a swing
 * or returning player to idle state.
 */
void ClearLocomotionState(LocomotionState* state) {
    // Clear timers and velocity
    state->m_recoveryTimer = 0.0f;
    state->m_velocity = {0.0f, 0.0f, 0.0f};
    
    // Reset state flags
    state->m_bActive = false;
    state->m_bForceBlock = false;
    state->m_threshold = 1.0f;
    
    // Clear target direction
    state->m_targetDirection = {0.0f, 0.0f, 0.0f};
    
    // Reset action state
    state->m_currentActionId = 0;
    state->m_stateFlags = -1;  // Inactive
    state->m_eventCounter1 = -1;
    state->m_eventCounter2 = 0;
    state->m_eventCounter3 = 0;
    
    // Restore default direction
    state->m_defaultDirection = {0.0f, 0.0f, 0.0f};
}

/**
 * SetBodyPartTargets @ 0x820CD468
 * 
 * Sets IK targets for body parts during swing animation.
 * Only updates when timing conditions are met.
 */
void SetBodyPartTargets(BodyPartController* controller, const vec3* target) {
    // Check timing condition
    if (controller->m_pTimingState) {
        struct TimingData {
            float data[13];  // Simplified access
        };
        TimingData* timing = (TimingData*)controller->m_pTimingState;
        
        // Check if swing has reached apex (currentTime >= targetTime)
        if (timing->data[7] >= timing->data[12]) {
            // Update head only at apex
            if (controller->m_pHead) {
                vec3* headTarget = (vec3*)((char*)controller->m_pHead + 384);
                *headTarget = *target;
            }
            return;
        }
    }
    
    // Update all body parts
    void* parts[] = {
        controller->m_pTorso,
        controller->m_pLeftArm,
        controller->m_pRightArm,
        controller->m_pLeftLeg,
        controller->m_pRightLeg
    };
    
    for (void* part : parts) {
        if (part) {
            vec3* partTarget = (vec3*)((char*)part + 384);
            *partTarget = *target;
        }
    }
}

/**
 * BeginRecoveryAnimation @ 0x820DD060
 * 
 * Starts recovery animation after serve/swing. Selects appropriate
 * animation and triggers game events.
 */
void BeginRecoveryAnimation(LocomotionState* state, const vec3* direction,
                           uint8_t flags, bool useAlternate) {
    if (!state->m_bActive) {
        // Check if recovery timer has elapsed
        if (state->m_recoveryTimer < g_recoveryThreshold) {
            return;
        }
        
        if (!state->m_pAnimSystem) {
            return;
        }
        
        // Get player ID
        uint32_t playerId = *(uint32_t*)((char*)state->m_pAnimSystem + 204);
        
        // Validate player state
        if (CheckPlayerState(state)) {
            return;
        }
        
        // Update threshold from config
        state->m_threshold = g_recoveryThreshold;
        
        // Select animation
        uint32_t animId;
        if (useAlternate) {
            uint32_t* anims = (uint32_t*)state->m_pAlternateAnims;
            animId = anims[state->m_alternateAnimIndex];
        } else {
            uint32_t* anims = (uint32_t*)state->m_pPrimaryAnims;
            animId = anims[state->m_primaryAnimIndex];
        }
        
        state->m_currentActionId = animId;
        state->m_targetDirection = *direction;
        
        // Apply state
        ApplyLocomotionState(state);
        
        // Reset timer
        state->m_recoveryTimer = 0.0f;
        
        // Trigger events
        uint32_t eventFlags = (1 << playerId) | 0x40;
        TriggerGameEvent(4100, eventFlags, 0, 0);
        
        if (g_audioEnabled == 1) {
            TriggerAudioEvent(g_audioSystem);
        }
        
        uint32_t stateId = *(uint32_t*)((char*)state + 4);
        TriggerGameEvent(4098, (1 << stateId) | 0x40, 0, 0);
    }
    
    state->m_bActive = true;
}
