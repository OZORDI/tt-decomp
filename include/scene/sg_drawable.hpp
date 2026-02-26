#pragma once

#include "scene/sg_node.hpp"

/**
 * sgRMDrawable - Renderable mesh scene graph node
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Extends sgNode with rendering capabilities for mesh objects.
 * "RM" likely stands for "Render Mesh" or "Render Model".
 * 
 * Vtable @ 0x82071C6C (34 slots, 136 bytes)
 */
class sgRMDrawable : public sgNode {
public:
    virtual ~sgRMDrawable();                        // @ 0x823D8E30 | slot 0
    
    // Override base class virtuals
    void* GetType() override;                       // @ 0x823D8F80 | slot 19
    void Clone() override;                          // @ 0x823D8DF8 | slot 20
    void Update();                                  // @ 0x823D8F18 | slot 21
    void Render() override;                         // @ 0x823D8D18 | slot 22
    void Transform();                               // @ 0x823D8EA8 | slot 29
    void GetBounds();                               // @ 0x823D8CD8 | slot 32
    
    // Additional virtual methods (slots 33)
    virtual void vfn_33() = 0;
    
protected:
    // +0x50 (80): Mesh/material data pointer (freed in destructor)
    void* m_pMeshData;
};

static_assert(sizeof(sgRMDrawable) >= 84, "sgRMDrawable struct too small");
