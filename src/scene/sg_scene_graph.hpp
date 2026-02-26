#pragma once
/**
 * sg_scene_graph.hpp — sgSceneGraph root container
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * vtable @ 0x82071D0C
 * Inherits from: rage::gameObject
 *
 * Traversal visitor types (all vtables confirmed):
 *   sgTraverseInitialize  @ 0x82071D4C
 *   sgTraverseReset       @ 0x82071D5C
 *   sgTraverseRestart     @ 0x82071D6C
 *   sgTraverseUpdate      @ 0x82071D7C
 *   sgTraversePostUpdate  @ 0x82071D8C
 *   sgTraverseDraw        @ 0x82071D9C
 *   sgTraverseDrawDebug   @ 0x82071DAC
 *   sgTraverseTerminate   @ 0x82071DBC
 */

#include <stdint.h>
#include "sg_node.hpp"

// ---------------------------------------------------------------------------
// Traversal context — passed through depth-first visitor walk
//
//   +0x00  sgNode*  m_pRootNode
//   +0x04  sgNode*  m_pCurrentNode
//   Walk order: child → sibling → parent's sibling
// ---------------------------------------------------------------------------
struct sgTraversalContext {
    sgNode*  m_pRootNode;     // +0x00
    sgNode*  m_pCurrentNode;  // +0x04
};

// ---------------------------------------------------------------------------
// Traversal visitor base — one concrete subclass per traversal type
// ---------------------------------------------------------------------------
struct sgTraverseBase {
    void** vtable;  // +0x00
};

struct sgTraverseInitialize  : sgTraverseBase {};  // vtable @ 0x82071D4C  slot 23
struct sgTraverseReset       : sgTraverseBase {};  // vtable @ 0x82071D5C  slot 24
struct sgTraverseRestart     : sgTraverseBase {};  // vtable @ 0x82071D6C  slot 25
struct sgTraverseUpdate      : sgTraverseBase {};  // vtable @ 0x82071D7C  slot 27
struct sgTraversePostUpdate  : sgTraverseBase {};  // vtable @ 0x82071D8C  slot 28
struct sgTraverseDraw        : sgTraverseBase {};  // vtable @ 0x82071D9C  slot 29
struct sgTraverseDrawDebug   : sgTraverseBase {};  // vtable @ 0x82071DAC  slot 30
struct sgTraverseTerminate   : sgTraverseBase {};  // vtable @ 0x82071DBC  slot 24 (alt)

// ---------------------------------------------------------------------------
// sgSceneGraph  —  root container; holds 7 traversal lists
//
// Layout (confirmed from destructor @ 0x823D9AA8 via sgSceneGraph_FreeLists @ 0x823D9AF8):
//   inherits rage::gameObject fields
//   +0x10  void*  m_pTraverseList1
//   +0x14  void*  m_pTraverseList2
//   +0x18  void*  m_pTraverseList3
//   +0x1C  (skipped in destructor)
//   +0x20  void*  m_pTraverseList4
//   +0x24  void*  m_pTraverseList5
//   +0x28  void*  m_pTraverseList6
//   +0x2C  void*  m_pTraverseList7
// ---------------------------------------------------------------------------
struct sgSceneGraph {
    void*  vtable;              // +0x00  (rage::gameObject base)
    void*  m_pGameObjectBase;   // +0x04..+0x0F  (gameObject fields, size TBD)

    void*  m_pTraverseList1;    // +0x10
    void*  m_pTraverseList2;    // +0x14
    void*  m_pTraverseList3;    // +0x18
    uint32_t _pad1C;            // +0x1C  (skipped by destructor)
    void*  m_pTraverseList4;    // +0x20
    void*  m_pTraverseList5;    // +0x24
    void*  m_pTraverseList6;    // +0x28
    void*  m_pTraverseList7;    // +0x2C

    // slot 0 — ~sgSceneGraph @ 0x823D9AA8 (size 0x50)
    //   Calls sgSceneGraph_FreeLists (@ 0x823D9AF8) to free all 7 traverse
    //   list pointers (+0x10..+0x2C, skipping +0x1C), then invokes the
    //   rage::gameObject base destructor. Conditional rage_free(this) follows.
    virtual ~sgSceneGraph();
};
