#pragma once
/**
 * sg_node.hpp — sgNode base scene graph node
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * vtable @ 0x8203338C
 * Inherits from: atSingleton (via rage framework)
 * Size: 16 bytes
 */

#include <stdint.h>

// ---------------------------------------------------------------------------
// sgNode  —  base node in the scene graph tree
//
// Layout (confirmed from recomp of 0x8212B350):
//   +0x00  void**   vtable
//   +0x04  sgNode*  m_pParent
//   +0x08  sgNode*  m_pNextSibling
//   +0x0C  sgNode*  m_pFirstChild
// ---------------------------------------------------------------------------
struct sgNode {
    void**   vtable;           // +0x00
    sgNode*  m_pParent;        // +0x04
    sgNode*  m_pNextSibling;   // +0x08
    sgNode*  m_pFirstChild;    // +0x0C

    // slot  0 — destructor @ 0x8212B350 (size 0x5C)
    //   Resets vtable, calls atSingleton base dtor (@ 0x821A9420 - ReleaseSingleton), conditional free.
    virtual ~sgNode();

    // slot 20 — IsType(uint32_t typeId) @ 0x8212B330 (size 0x20)
    //   Compares typeId against a global type-ID; returns 1 if match.
    virtual int IsType(uint32_t typeId);

    // slot 22 — GetTypeName() @ 0x8212B2A0 (size 0x0C)
    //   Returns pointer to static string "sgNode".
    virtual const char* GetTypeName() const;
};
