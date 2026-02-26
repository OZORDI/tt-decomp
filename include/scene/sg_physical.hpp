#pragma once

#include "scene/sg_drawable.hpp"
#include <cstdint>

// Forward declarations
struct phInst;

/**
 * sgPhysicalObject - Physical + renderable scene graph object
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Combines rendering with physics simulation.
 * Integrates with phBound (collision bounds) and phInst (physics instance).
 * Manages visibility culling and draw bucket submission.
 */
class sgPhysicalObject : public sgRMDrawable {
public:
    // Override virtual methods
    void Transform() override;                      // @ 0x823D9208 | slot 29
    void PhysicsUpdate();                           // @ 0x823D9368 | slot 33
    void AdvancedPhysics();                         // @ 0x823D98C0 | slot 39
    
protected:
    // +0x60 (96):  Render data/drawable info
    void* m_pRenderData;
    
    // +0x64 (100): Render distance for LOD
    float m_renderDistance;
    
    // +0x68 (104): Render state flags
    uint32_t m_renderFlags;
    
    // +0x6C-0x6F: Padding/unknown
    uint32_t m_padding1;
    
    // +0x70 (112): Visibility flag
    uint8_t m_isHidden;
    
    // +0x71-0x7B: Padding
    uint8_t m_padding2[11];
    
    // +0x7C (124): Physics instance pointer
    phInst* m_pPhysicsInstance;
};

static_assert(sizeof(sgPhysicalObject) >= 128, "sgPhysicalObject struct too small");
