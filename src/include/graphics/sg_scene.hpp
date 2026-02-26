/**
 * sg_scene.hpp — Scene Graph System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Scene graph traversal and rendering management.
 * Implements visitor pattern for scene operations.
 */
#pragma once
#include <stdint.h>

// ────────────────────────────────────────────────────────────────────────────
// sgNode — Base scene graph node
// ────────────────────────────────────────────────────────────────────────────

struct sgNode {
    void** vtable;  // +0x00
    
    virtual ~sgNode();                      // [0] @ 0x8212B350
    virtual void Initialize(void* context); // [1]
    virtual void Update(void* context);     // [2]
    virtual void Draw(void* context);       // [3]
    // ... more virtual methods
    virtual const char* GetTypeName() const; // [19]
    virtual bool IsType(uint32_t typeId) const; // [20]
    virtual void Reset();                   // [22]
};

// ────────────────────────────────────────────────────────────────────────────
// sgSceneGraph — Root scene graph container
// ────────────────────────────────────────────────────────────────────────────

struct sgSceneGraph {
    void** vtable;  // +0x00
    
    virtual ~sgSceneGraph();  // [0] @ 0x823D9AA8
    
    // TODO: Add member fields discovered from usage analysis
};

// ────────────────────────────────────────────────────────────────────────────
// sgRMDrawable — Renderable scene graph node
// ────────────────────────────────────────────────────────────────────────────

struct sgRMDrawable {
    void** vtable;              // +0x00
    // ... inherited fields from sgNode and atSingleton
    
    float m_transform[4][4];    // +0x10 (16) - 4x4 transformation matrix
    // ... more fields
    void* m_pResource;          // +0x50 (80) - Resource pointer
    
    virtual ~sgRMDrawable();                        // [0] @ 0x823D8E30
    virtual const char* GetTypeName() const;        // [19] @ 0x823D8F80
    virtual bool IsType(uint32_t typeId) const;     // [20] @ 0x823D8DF8
    virtual void OnResourceChanged();               // [21] @ 0x823D8F18
    virtual void UpdateBounds();                    // [22] @ 0x823D8D18
    virtual void Render(void* context);             // [29] @ 0x823D8EA8
    virtual void SetTransform(const float* matrix); // [32] @ 0x823D8CD8
};

// ────────────────────────────────────────────────────────────────────────────
// sgPhysicalObject — Physical scene object
// ────────────────────────────────────────────────────────────────────────────

struct sgPhysicalObject {
    void** vtable;  // +0x00
    
    virtual void UpdatePhysics(float deltaTime);  // [29] @ 0x823D9208
    virtual void ApplyForce(const float* force);  // [33] @ 0x823D9368
    virtual void OnCollision(void* other);        // [39] @ 0x823D98C0
};

// ────────────────────────────────────────────────────────────────────────────
// Scene Graph Traversal Visitors (Visitor Pattern)
// ────────────────────────────────────────────────────────────────────────────

struct sgTraverseInitialize {
    void** vtable;       // +0x00
    void* m_pRootNode;   // +0x04
    void* m_pCurrentNode; // +0x08
    
    void Execute(void* context);  // @ 0x823D9E90
};

struct sgTraverseReset {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

struct sgTraverseRestart {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

struct sgTraverseUpdate {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

struct sgTraversePostUpdate {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

struct sgTraverseDraw {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

struct sgTraverseDrawDebug {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

struct sgTraverseTerminate {
    void** vtable;  // +0x00
    
    void Execute(void* context);
};

// ────────────────────────────────────────────────────────────────────────────
// Global type IDs (referenced in IsType checks)
// ────────────────────────────────────────────────────────────────────────────

extern uint32_t g_sgRMDrawable_typeId1;  // @ 0x825C2D78
extern uint32_t g_sgRMDrawable_typeId2;  // @ 0x825D9450
