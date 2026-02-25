/**
 * gm_ball_robot.hpp — Ball-serving robot for practice mode
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * gmBallRobot manages ball serving in practice/training modes.
 * It searches for a character named "BallRobot" and attaches to
 * a bone named "BallRobotBone" for positioning.
 * 
 * Multiple inheritance: Two vtables at +0 and +4 (12-byte offset)
 * Contains atSingleton base at +16 and audControl3d member at +384
 */

#pragma once
#include <stdint.h>

// Forward declarations
class atSingleton;
namespace rage {
    class audControl3d;
}

/**
 * gmBallRobot @ RTTI 0x82032338, 0x82032344
 * 
 * Ball-serving robot controller for practice modes.
 * Responds to game events to initialize/activate ball serving.
 * 
 * Size: ~576 bytes (0x240)
 */
class gmBallRobot {
public:
    // Destructor @ 0x821120A8 | size: 0xB8
    ~gmBallRobot();
    
    // Event handler (vtable slot 1) @ 0x82112160 | size: 0x20C
    // Handles initialization, activation, and game state events
    void HandleEvent(uint16_t eventType);
    
    // Thunk for secondary base @ 0x821129C8 | size: 0x8
    static void DestructorThunk(gmBallRobot* ptr);

private:
    // Multiple inheritance layout
    void* m_vtable1;                    // +0x000 - Primary vtable @ 0x82032338
    void* m_vtable2;                    // +0x004 - Secondary vtable @ 0x82032344
    uint8_t m_padding08[8];             // +0x008
    
    // atSingleton base @ +16
    uint8_t m_singletonData[368];       // +0x010 - atSingleton base
    
    // audControl3d member @ +384
    uint8_t m_audControl3d[176];        // +0x180 - 3D audio control (vtable 0x82035594)
    
    // Owned object @ +560
    void* m_pOwnedObject;               // +0x230 - Deleted in destructor
    
    // Robot state @ +540
    uint8_t m_isActive;                 // +0x21C - Active flag
    uint8_t m_padding21D[3];            // +0x21D
    void* m_pCharacter;                 // +0x220 - "BallRobot" character pointer
    void* m_pAnimationData;             // +0x224 - Animation data
    void* m_pMotionClip;                // +0x228 - Motion clip
    void* m_pAllocatedObject;           // +0x22C - 16-byte allocated object
    
    // Animation buffers @ +568
    void* m_pAnimBuffer1;               // +0x238 - Animation buffer 1
    void* m_pAnimBuffer2;               // +0x23C - Animation buffer 2
};
