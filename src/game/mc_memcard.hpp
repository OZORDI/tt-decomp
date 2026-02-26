/**
 * mc_memcard.hpp — Xbox 360 memory card / save-data system
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Memory card state machine and related classes for save/load operations.
 */

#pragma once
#include <stdint.h>

// Forward declarations
class IHeap;
struct mcMemcardStatus;

// ─────────────────────────────────────────────────────────────────────────────
// mcFileSegment  [vtable @ 0x8203CE14]
//
// RTTI-declared class with no observable implementation.
// Single vtable (24 bytes) but no virtual methods, field accesses,
// or instantiation sites found in the binary.
//
// The vtable address (0x8203CE14) contains the string " mastering effect(s)..."
// which suggests this may have been related to audio mastering or effects
// processing during save/load operations, but was never implemented.
//
// This appears to be an unused class - likely a planned feature for segmented
// file I/O or streaming that was never completed, or a development artifact
// that remained in the RTTI data.
//
// Status: UNUSED - No implementation required (no code references this class)
// ─────────────────────────────────────────────────────────────────────────────
struct mcFileSegment {
    // Vtable @ 0x8203CE14 (24 bytes)
    // No virtual methods beyond base class
    // No field accesses observed
    // No global instances
    // No references in recomp output
    // Vtable contains string: " mastering effect(s)..."
};

// ─────────────────────────────────────────────────────────────────────────────
// mcMemcardControl — Xbox 360 memory card state machine
// ─────────────────────────────────────────────────────────────────────────────

struct mcSaveOp {
    void*    m_pVtable;
    int32_t  m_deviceResult;
    int32_t  m_opStatus;
    int32_t  m_pendingOp;
    uint8_t  m_contentData[4936];
    char     m_szContentFileName[43];
};

struct fsmMachine {
    void*              m_pVtable;
    int32_t            m_stateCount;
    mcMemcardStatus**  m_pStates;
};

struct mcMemcardControl : fsmMachine {
    int32_t  m_currentState;
    uint32_t m_unk10;
    int32_t  m_result1;
    int32_t  m_result2;
    uint8_t  m_pad1C[0x10];
    mcSaveOp m_saveOp;

    virtual ~mcMemcardControl(bool shouldFree = false);  // @ 0x822BFFF8
    void Init();                                          // @ 0x8221E628
    void Shutdown();                                      // @ 0x8221EAB0
    void Reset();                                         // @ 0x8221EC80
    bool IsTransitionAllowed(int targetState);            // @ 0x8221ECC8
};
