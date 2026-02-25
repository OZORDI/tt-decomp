/**
 * Training Drill Classes
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Training mode drill system with base class and 11 specialized drill types.
 * Each drill teaches a specific table tennis skill (movement, serving, spin, etc.)
 */

#pragma once

#include <stdint.h>

// Forward declarations
class pongTrainingDrillConfig;

/**
 * pongTrainingDrill (Base Class)
 * @ RTTI: 0x82031C3C
 * 
 * Base class for all training drills. Manages success/failure tracking,
 * timing, and common drill state.
 */
class pongTrainingDrill {
public:
    // Virtual functions
    virtual ~pongTrainingDrill() {}                          // vfn_0
    virtual void ScalarDestructor() {}                       // vfn_1
    virtual void CallInit() {}                               // vfn_2 - thunk to vfn_3
    virtual void Init();                                     // vfn_3 @ 0x8210CDB8
    virtual void Update() {}                                 // vfn_4
    virtual void OnStart() {}                                // vfn_5
    virtual void OnEnd() {}                                  // vfn_6
    virtual void OnReset() {}                                // vfn_7
    virtual void Process() {}                                // vfn_9
    virtual void Render() {}                                 // vfn_10
    virtual void OnSuccess() {}                              // vfn_11
    virtual void OnFailure() {}                              // vfn_12
    virtual void GetName() {}                                // vfn_13
    virtual void GetDescription() {}                         // vfn_14
    virtual void IsComplete() {}                             // vfn_15
    virtual void GetProgress() {}                            // vfn_16
    virtual void CanAdvance() {}                             // vfn_23
    virtual void GetDifficulty() {}                          // vfn_24
    virtual void SetDifficulty() {}                          // vfn_25
    virtual void OnBallHit() {}                              // vfn_27
    virtual void OnBallMiss() {}                             // vfn_28
    virtual void OnRallyEnd() {}                             // vfn_29
    virtual void OnPointScored() {}                          // vfn_30

    // @ 0x8210D400 | size: 0x84
    void IncreaseNumSuccesses(int increment);

protected:
    // +0x00: vtable (implicit)
    uint32_t m_numAttempts;              // +0x04 - Total number of attempts
    uint32_t m_numSuccesses;             // +0x08 - Number of successful attempts
    uint32_t m_prevSuccesses;            // +0x0C - Previous success count (for change detection)
    uint32_t m_numFailures;              // +0x10 - Number of failed attempts
    float m_timeRemaining;               // +0x14 - Time remaining for drill
    uint8_t m_completionFlag;            // +0x18 - Completion event flag
    uint8_t m_padding19[3];              // +0x19 - Padding
    uint32_t m_targetIndex;              // +0x1C - Current target index (-1 = none)
    pongTrainingDrillConfig* m_pConfig;  // +0x20 - Pointer to drill configuration
};

/**
 * pongDrillMovement
 * @ RTTI: 0x82031CCC
 * 
 * Teaches player movement and positioning.
 */
class pongDrillMovement : public pongTrainingDrill {
public:
    virtual void Init() override;        // vfn_3 @ 0x8210DB48
    virtual void Update() override {}    // vfn_4
    virtual void OnStart() override {}   // vfn_5
    virtual void Process() override {}   // vfn_9

private:
    enum MovementState {
        MOVEMENT_STATE_0 = 0,
        MOVEMENT_STATE_1 = 1,
        MOVEMENT_STATE_2 = 2,
        MOVEMENT_STATE_3 = 3,
        MOVEMENT_STATE_4 = 4
    };

    uint8_t m_movementFlag;              // +0x24 - Movement state flag
    uint8_t m_padding25[3];              // +0x25 - Padding
    float m_targetDistance;              // +0x28 - Distance to target position
    float m_moveTimer;                   // +0x2C - Movement timer (counts down)
    MovementState m_movementState;       // +0x30 - Current movement state (0-4)
    uint8_t m_isComplete;                // +0x34 - Drill completion flag
    uint8_t m_padding35[3];              // +0x35 - Padding
};

/**
 * pongDrillServeMeter
 * @ RTTI: 0x82031D5C
 * 
 * Teaches serve power meter timing.
 */
class pongDrillServeMeter : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillServing
 * @ RTTI: 0x82031DEC
 * 
 * Teaches serving technique and placement.
 */
class pongDrillServing : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillReturn
 * @ RTTI: 0x82031E7C
 * 
 * Teaches returning serves.
 */
class pongDrillReturn : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillPlacement
 * @ RTTI: 0x82031F0C
 * 
 * Teaches shot placement accuracy.
 */
class pongDrillPlacement : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillSoftShot
 * @ RTTI: 0x82031F9C
 * 
 * Teaches soft shot technique (drop shots, touch shots).
 */
class pongDrillSoftShot : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillCharging
 * @ RTTI: 0x8203202C
 * 
 * Teaches charging shots for power.
 */
class pongDrillCharging : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillSpin
 * @ RTTI: 0x820320BC
 * 
 * Teaches applying spin to shots (topspin, backspin, sidespin).
 */
class pongDrillSpin : public pongTrainingDrill {
public:
    virtual void Init() override {}      // vfn_3 @ 0x821101E0
    virtual void Process() override {}   // vfn_8 @ 0x82110218
};

/**
 * pongDrillFocusShot
 * @ RTTI: 0x8203214C
 * 
 * Teaches focus shot mechanic (special powered shot).
 */
class pongDrillFocusShot : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillCounterSpin
 * @ RTTI: 0x820321DC
 * 
 * Teaches countering opponent's spin.
 */
class pongDrillCounterSpin : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

/**
 * pongDrillSmash
 * @ RTTI: 0x8203226C
 * 
 * Teaches smash technique (overhead power shot).
 */
class pongDrillSmash : public pongTrainingDrill {
public:
    virtual void Init() override {}
    virtual void Update() override {}
    virtual void Process() override {}
};

// Compile-time size verification
static_assert(sizeof(pongTrainingDrill) >= 36, "pongTrainingDrill minimum size check");
