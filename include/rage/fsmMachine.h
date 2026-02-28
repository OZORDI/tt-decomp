/**
 * rage/fsmMachine.h — Finite State Machine base class
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * fsmMachine — Finite State Machine base class
 * 
 * Base class for finite state machines used throughout the game.
 * Vtable @ 0x8204DD14
 * 
 * Layout:
 *   +0x00  void*     m_pVtable
 *   +0x04  uint32_t  m_flags
 *   +0x08  void*     m_pStateData  (freed in destructor if non-null)
 */
typedef struct fsmMachine {
    void*     m_pVtable;      /* +0x00 */
    uint32_t  m_flags;        /* +0x04 */
    void*     m_pStateData;   /* +0x08 — freed in destructor */
} fsmMachine;

/**
 * fsmMachine_Destructor_27A8 @ 0x822227A8 | size: 0x58
 * 
 * Original symbol: gameLoop_DestroyAudio_27A8 (auto-named, misleading)
 * Actual function: fsmMachine::~fsmMachine()
 * 
 * Destructor for fsmMachine base class:
 *   1. Sets vtable to fsmMachine base vtable (0x8204DD14)
 *   2. Frees m_pStateData if non-null
 *   3. Sets final vtable to rage::datBase (0x820276C4)
 */
void fsmMachine_Destructor_27A8(fsmMachine* pMachine);

#ifdef __cplusplus
}
#endif
