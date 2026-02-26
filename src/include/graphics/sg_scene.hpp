/**
 * sg_scene.hpp — Scene Graph System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Scene graph traversal and rendering management.
 * Implements visitor pattern for scene operations.
 */
#pragma once

#include <cstdint>
#include <cstring>

// Forward declarations
class sgNode;

// ────────────────────────────────────────────────────────────────────────────
// Scene Graph Node Iterator Context
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGameContext - Scene graph traversal context
 * 
 * Manages iteration state during scene graph traversal.
 * Tracks current node and provides navigation methods.
 */
struct pongGameContext {
    sgNode* m_pRootNode;      // +0x00 - Root of the scene graph
    sgNode* m_pCurrentNode;   // +0x04 - Current node being processed
    
    // Navigation methods
    void Reset();                              // @ 0x823DA070
    void MoveToNext();                         // @ 0x823D9FD0
    void MoveToChild();                        // @ 0x823DA0F8
};

// ────────────────────────────────────────────────────────────────────────────
// sgNode — Base scene graph node
// ────────────────────────────────────────────────────────────────────────────

/**
 * sgNode - Base scene graph node
 * 
 * Foundation class for all scene graph objects in the RAGE engine.
 * Provides tree structure with parent/child/sibling links.
 * 
 * Vtable @ 0x8203338C
 */
class sgNode {
public:
    virtual ~sgNode();                                  // @ 0x8212B350 | slot 0
    
    // Core virtual interface
    virtual void Initialize(pongGameContext* ctx) = 0;  // slot 1
    virtual void Update(float deltaTime) = 0;           // slot 2
    virtual void Draw() = 0;                            // slot 3
    
    // ... additional virtual methods (slots 4-18)
    
    virtual const char* GetTypeName() const = 0;        // slot 19
    virtual bool IsType(uint32_t typeId) const = 0;     // slot 20
    virtual void Clone() = 0;                           // slot 21
    virtual void Render() = 0;                          // slot 22
    
protected:
    // Tree structure (inferred from pongGameContext navigation)
    sgNode* m_pParent;      // +0x04 - Parent node
    sgNode* m_pFirstChild;  // +0x08 - First child node
    sgNode* m_pNextSibling; // +0x0C - Next sibling node
};

// ────────────────────────────────────────────────────────────────────────────
// sgSceneGraph — Root scene graph container
// ────────────────────────────────────────────────────────────────────────────

/**
 * sgSceneGraph - Root scene graph container/manager
 * 
 * Manages the root of the scene graph hierarchy.
 * Coordinates traversal operations and node management.
 * 
 * Vtable @ 0x82071D0C
 */
class sgSceneGraph {
public:
    virtual ~sgSceneGraph();                            // @ 0x823D9AA8 | slot 0
    
    // TODO: Add management methods as discovered
    
protected:
    sgNode* m_pRootNode;    // Root of the scene graph
};

// ────────────────────────────────────────────────────────────────────────────
// sgRMDrawable — Renderable scene graph node
// ────────────────────────────────────────────────────────────────────────────

/**
 * sgRMDrawable - Renderable mesh scene graph node
 * 
 * Extends sgNode with rendering capabilities for mesh objects.
 * "RM" likely stands for "Render Mesh" or "Render Model".
 * 
 * Vtable @ 0x82071C6C
 */
class sgRMDrawable : public sgNode {
public:
    virtual ~sgRMDrawable();                            // @ 0x823D8E30 | slot 0
    
    // Override base class virtuals
    const char* GetTypeName() const override;           // @ 0x823D8F80 | slot 19
    bool IsType(uint32_t typeId) const override;        // @ 0x823D8DF8 | slot 20
    void OnResourceChanged();                           // @ 0x823D8F18 | slot 21
    void Render() override;                             // @ 0x823D8D18 | slot 22
    void UpdateBounds();                                // @ 0x823D8EA8 | slot 29
    void SetTransform(const float matrix[4][4]);        // @ 0x823D8CD8 | slot 32
    
protected:
    float m_transform[4][4];    // +0x10 (16) - 4x4 transformation matrix
    // ... additional fields
    void* m_pResource;          // +0x50 (80) - Mesh/material resource pointer
};

// ────────────────────────────────────────────────────────────────────────────
// Scene Graph Traversal Visitors (Visitor Pattern)
// ────────────────────────────────────────────────────────────────────────────

/**
 * Base class for scene graph traversal operations.
 * Implements the visitor pattern for different traversal types.
 */
class sgTraverseBase {
public:
    virtual ~sgTraverseBase() = default;
    virtual void Execute(pongGameContext* ctx) = 0;
    
protected:
    sgNode* m_pRootNode;        // +0x04 - Root node to traverse from
    sgNode* m_pCurrentNode;     // +0x08 - Current node being processed
};

/**
 * sgTraverseInitialize - Initialization traversal
 * 
 * Traverses the scene graph and calls Initialize() on each node.
 * Used during scene setup phase.
 * 
 * Vtable @ 0x82071D4C
 */
class sgTraverseInitialize : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9E90
};

/**
 * sgTraverseReset - Reset traversal
 * Vtable @ 0x82071D5C
 */
class sgTraverseReset : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9F28
};

/**
 * sgTraverseRestart - Restart traversal
 * Vtable @ 0x82071D6C
 */
class sgTraverseRestart : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9F40
};

/**
 * sgTraverseUpdate - Update traversal
 * Vtable @ 0x82071D7C
 */
class sgTraverseUpdate : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9F58
};

/**
 * sgTraversePostUpdate - Post-update traversal
 * Vtable @ 0x82071D8C
 */
class sgTraversePostUpdate : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9F70
};

/**
 * sgTraverseDraw - Draw traversal
 * Vtable @ 0x82071D9C
 */
class sgTraverseDraw : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9F88
};

/**
 * sgTraverseDrawDebug - Debug draw traversal
 * Vtable @ 0x82071DAC
 */
class sgTraverseDrawDebug : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9FA0
};

/**
 * sgTraverseTerminate - Termination traversal
 * Vtable @ 0x82071DBC
 */
class sgTraverseTerminate : public sgTraverseBase {
public:
    void Execute(pongGameContext* ctx) override;        // @ 0x823D9FB8
};

// ────────────────────────────────────────────────────────────────────────────
// Global type IDs (referenced in IsType checks)
// ────────────────────────────────────────────────────────────────────────────

extern uint32_t g_sgRMDrawable_typeId1;  // @ 0x825C2D78
extern uint32_t g_sgRMDrawable_typeId2;  // @ 0x825C93D0
