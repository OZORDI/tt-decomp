/**
 * rage/fsmMachine.c — Finite State Machine base class implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "rage/fsmMachine.h"
#include <stdint.h>

/* Heap free @ 0x820C00C0 */
extern void rage_free_00C0(void* ptr);

/* fsmMachine base vtable @ 0x8204DD14 */
extern void* fsmMachine_vtable;  /* lbl_8204DD14 */

/* rage::datBase vtable @ 0x820276C4 */
extern void* rage_datBase_vtable;  /* lbl_820276C4 */

/**
 * fsmMachine_Destructor_27A8 @ 0x822227A8 | size: 0x58
 * 
 * Original symbol: gameLoop_DestroyAudio_27A8 (auto-named, misleading)
 * Actual function: fsmMachine::~fsmMachine()
 * 
 * Destructor for fsmMachine base class. This is the standard two-phase
 * C++ destructor pattern seen in PowerPC ABI:
 *   1. Set vtable to derived class vtable (fsmMachine @ 0x8204DD14)
 *   2. Free owned resources (m_pStateData at +0x08)
 *   3. Set vtable to base class vtable (rage::datBase @ 0x820276C4)
 * 
 * The function is called from subclass destructors before they free
 * their own memory.
 */
void fsmMachine_Destructor_27A8(fsmMachine* pMachine)
{
    /* Set vtable to fsmMachine (derived class cleanup phase) */
    pMachine->m_pVtable = &fsmMachine_vtable;
    
    /* Free state data if allocated */
    if (pMachine->m_pStateData != NULL) {
        rage_free_00C0(pMachine->m_pStateData);
        pMachine->m_pStateData = NULL;
    }
    
    /* Set vtable to rage::datBase (base class cleanup phase) */
    pMachine->m_pVtable = &rage_datBase_vtable;
}
