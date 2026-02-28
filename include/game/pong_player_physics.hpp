/**
 * pong_player_physics.hpp
 * 
 * Player locomotion and recovery state for Rockstar Table Tennis.
 * 
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#pragma once

#include <cstdint>

struct vec3 {
    float x, y, z;
};

struct pongAnimSystem;
struct pongTimingState;

/**
 * LocomotionState
 * 
 * Manages player locomotion during recovery from swings/serves.
 * Handles animation selection, timing, and movement direction.
 */
struct LocomotionState {
    pongAnimSystem* m_pAnimSystem;
    void* m_pPrimaryAnims;
    uint16_t m_primaryAnimIndex;
    void* m_pAlternateAnims;
    uint16_t m_alternateAnimIndex;
    
    uint32_t m_currentActionId;
    int32_t m_stateFlags;
    bool m_bActive;
    bool m_bForceBlock;
    float m_threshold;
    
    vec3 m_targetDirection;
    vec3 m_defaultDirection;
    vec3 m_velocity;
    
    float m_recoveryTimer;
    
    int32_t m_eventCounter1;
    int32_t m_eventCounter2;
    int32_t m_eventCounter3;
};

/**
 * BodyPartController
 * 
 * Controls IK targets for player body parts during animations.
 */
struct BodyPartController {
    void* m_pTorso;
    void* m_pLeftArm;
    void* m_pRightArm;
    void* m_pLeftLeg;
    void* m_pRightLeg;
    void* m_pHead;
    
    pongTimingState* m_pTimingState;
};

// @ 0x820DCD20
void ClearLocomotionState(LocomotionState* state);

// @ 0x820CD468
void SetBodyPartTargets(BodyPartController* controller, const vec3* target);

// @ 0x820DD060
void BeginRecoveryAnimation(LocomotionState* state, const vec3* direction, 
                           uint8_t flags, bool useAlternate);
