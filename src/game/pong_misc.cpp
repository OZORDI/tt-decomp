/**
 * pong_misc.cpp — Miscellaneous game classes (fsmMachine, misc helpers)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.9.cpp
 */

#include "pong_misc.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// fsmMachine  [vtable @ 0x8204DD14]
//
// Base class for all finite state machines in the game.  Inherits from
// rage::datBase.  The object holds a heap-allocated array of state pointers
// at +0x08 (m_pStates).
//
// The destructor pattern:
//   1. Calls the internal state-teardown helper (gameLoop_DestroyAudio_27A8),
//      which first resets the vtable to the base-class (rage::datBase) vtable
//      and frees m_pStates if non-null.
//   2. If the 'delete self' flag (bit 0 of the flags parameter) is set,
//      the raw object memory is freed via rage::MemFree.
//
// This is identical in shape to rage::hsmContext's destructor — both follow
// the standard RAGE deleting-destructor ABI:
//   virtual ~T(int flags)   flags & 1 → free the object after cleanup
// ─────────────────────────────────────────────────────────────────────────────

/**
 * fsmMachine::~fsmMachine() @ 0x82222758 | size: 0x50
 *
 * Deleting destructor for fsmMachine.
 * Tears down internal state (via helper at 0x822227A8), then optionally frees
 * the object itself depending on the `flags` argument (bit 0 = delete self).
 */
fsmMachine::~fsmMachine()
{
    // Reset vtable to rage::datBase and free m_pStates if non-null.
    // Internally equivalent to:
    //   this->vtable    = &rage::datBase::vtable;   // 0x820276C4
    //   if (m_pStates) { rage::MemFree(m_pStates); m_pStates = nullptr; }
    //   this->vtable    = &fsmMachine::vtable;       // 0x8204DD14 (restored)
    //
    // The helper (gameLoop_DestroyAudio_27A8 @ 0x822227A8) handles this
    // sequence.  After it returns the caller checks bit 0 of 'flags'; if set,
    // rage::MemFree is called on `this`.
    //
    // TODO: confirm whether fsmMachine itself has additional members beyond
    //       +0x08 (m_pStates) — only one function is present in the vtable,
    //       so the remaining 15 slots are pure-virtual or belong to subclasses.
}

// ─────────────────────────────────────────────────────────────────────────────
// pongSaveFile  [2 vtables @ +0 and +12, MI with 24-byte offset]
//
// Save file manager with 4 save slots. Each slot is a rage::parStructure
// (15044 bytes) that manages serialized player data (progress, settings,
// unlocks, statistics).
//
// Xbox 360 Save System Integration:
//   - Uses XamContent* APIs through atSingleton system
//   - XamContentCreate @ 0x825861AC creates save file content package
//   - XamContentClose @ 0x825861CC closes save file handle
//   - rage::parStructure handles platform-independent serialization
//
// Slot Layout (from pongSaveFile base):
//   Slot 0: offset 0x3CE4 (15588 bytes) - initialized last
//   Slot 1: offset 0x77A8 (30632 bytes)
//   Slot 2: offset 0xB26C (45676 bytes)
//   Slot 3: offset 0xED30 (60720 bytes) - initialized first
// ─────────────────────────────────────────────────────────────────────────────

#include "rage/memory.h"

// External function declarations
extern void util_CE30(void* obj);  // @ 0x8234CE30 - Initialize rage::parStructure
extern void atSingleton_E998_g(void* obj, void* param2, void* outIndex);

/**
 * pongSaveFile::pongSaveFile() @ 0x821C5260 | size: 0x90
 *
 * Constructor for save file manager.
 * Initializes vtables and 4 save slot parStructure objects.
 *
 * Algorithm: Start at offset 0x10008 (65544), subtract 0x3AC4 (15044) per
 * iteration. This creates slots at offsets: 0xED30, 0xB26C, 0x77A8, 0x3CE4
 * (reverse order).
 *
 * Each parStructure is initialized by util_CE30 @ 0x8234CE30 which:
 *   - Sets vtable to 0x82065A9C (rage::parStructure vtable)
 *   - Initializes member array pointer at +0x10
 *   - Sets count and flags at +0x14/+0x16
 */
pongSaveFile::pongSaveFile() {
    // Set initial vtables
    m_vtable1 = (void**)0x8200CB18;
    m_vtable2 = (void**)0x8200CB30;
    
    // Initialize 4 save slots (rage::parStructure instances)
    // Starting from offset 0x10008 (65544) and going backwards by 15044 each iteration
    uint32_t offset = 0x10008;
    for (int i = 0; i < 4; i++) {
        offset -= 15044;  // 0x3AC4 - size of rage::parStructure
        void* saveSlot = (char*)this + offset;
        util_CE30(saveSlot);  // Initialize parStructure @ 0x8234CE30
    }
    
    // Set final vtables
    m_vtable2 = (void**)0x82017B34;
    m_vtable1 = (void**)0x8204D9B0;
    
    // Free data if flags indicate ownership
    if (m_flags != 0 && m_pData) {
        rage_free_00C0(m_pData);
    }
    
    // Set base class vtable
    m_vtable1 = (void**)0x820176C4;
}

/**
 * pongSaveFile::~pongSaveFile() @ 0x821C5210 | size: 0x50
 *
 * Destructor for save file manager.
 */
pongSaveFile::~pongSaveFile() {
    // Call cleanup function
    pongSaveFile::pongSaveFile();  // Calls constructor logic for cleanup
    
    // Free memory if destructor flag is set
    // (flag passed in r4, checked with & 0x1)
}

/**
 * pongSaveFile::HandleEvent() @ 0x821C7680 | size: 0x9C
 *
 * Event handler for save file operations.
 *
 * Event types (see GameEventID enum):
 *   EVENT_SAVE_WRITE (0x200F / 8207): Save - serialize current game state to save slot
 *   EVENT_SAVE_LOAD  (0x200E / 8206): Load - deserialize game state from save slot
 *
 * Process:
 *   1. Check if event is save or load
 *   2. Query atSingleton system to determine active save slot index (0-3)
 *   3. Calculate slot offset: slotIndex * 15044 (0x3AC4)
 *   4. Clear event state flags at slot+524, slot+528, slot+529
 *
 * The rage::parStructure at each slot handles actual serialization/deserialization.
 * The atSingleton system calls XamContentCreate @ 0x825861AC to create Xbox 360
 * save file content package.
 */
void pongSaveFile::HandleEvent(uint16_t eventType) {
    bool shouldProcess = false;
    
    // Check event type
    if (eventType == EVENT_SAVE_WRITE) {  // 8207 (0x200F)
        shouldProcess = true;  // Save
    } else if (eventType == EVENT_SAVE_LOAD) {  // 8206 (0x200E)
        shouldProcess = true;  // Load
    }
    
    if (!shouldProcess) {
        return;
    }
    
    // Get save slot index from singleton system
    uint32_t slotIndex = 0;
    atSingleton_E998_g(this, (void*)0x82017888, &slotIndex);
    
    // Calculate save slot offset (each slot is 15044 bytes)
    uint32_t slotOffset = slotIndex * 15044;
    char* saveSlot = (char*)this + slotOffset;
    
    // Clear event state for this slot
    *(uint32_t*)(saveSlot + 524) = 0;
    *(uint8_t*)(saveSlot + 528) = 0;
    *(uint8_t*)(saveSlot + 529) = 0;
}

/**
 * pongSaveFile::DestructorThunk() @ 0x821C7720 | size: 0x8
 *
 * Thunk for secondary base class destructor.
 * Adjusts this pointer by -12 bytes for multiple inheritance.
 */
void pongSaveFile::DestructorThunk(pongSaveFile* ptr) {
    pongSaveFile* adjusted = (pongSaveFile*)((char*)ptr - 12);
    adjusted->~pongSaveFile();
}
