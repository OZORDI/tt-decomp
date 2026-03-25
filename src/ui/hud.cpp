/**
 * hud.cpp — HUD and Flash-based UI implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "hud.hpp"
#include <cstdint>

// External references
extern void SetHudParameter(void* client, int param, float value);
extern uint8_t* g_pGlobalGameState;  // @ 0x82606430 (SDA r13+25648)

// Float constants
static const float g_floatOne = 1.0f;      // @ 0x8202D108+8
static const float g_floatZero = 0.0f;     // @ 0x8202D108


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingHUD::OnEvent @ 0x822FA518 | size: 0xE0
//
// Handles training HUD events by checking event codes and dispatching to
// the network client with appropriate parameters.
//
// Event codes:
//   2146 (0x862) - Training event with param=1, value=1.0f
//                  Also clears a flag at global state offset +6208+32
//   2147 (0x863) - Training event with param=0, value=1.0f  
//   2080 (0x820) - Training event with param=0, value=0.0f
//
// Parameters:
//   eventData - Pointer to event data structure (first uint16_t is event code)
// ─────────────────────────────────────────────────────────────────────────────
void hudTrainingHUD::OnEvent(void* eventData)
{
    // Read event code from first uint16_t of event data
    uint16_t eventCode = *(uint16_t*)eventData;
    
    // Get pointer to this object's network client interface (at offset -96)
    void* networkClient = (char*)this - 96;
    
    // Handle event code 2146 (0x862)
    if (eventCode == 2146) {
        // Access global game state
        uint8_t* globalState = *(uint8_t**)g_pGlobalGameState;
        
        // Get player slot array at offset +132
        uint32_t slotIndex = *(uint32_t*)(globalState + 132);
        
        // Calculate array index: (slotIndex + 29) * 4
        uint32_t arrayIndex = (slotIndex + 29) * 4;
        
        // Get player object from array
        void* playerObj = *(void**)(globalState + arrayIndex);
        
        // Check if training state object exists at offset +6208
        void* trainingState = *(void**)((char*)playerObj + 6208);
        
        if (trainingState != nullptr) {
            // Clear flag at offset +32 in training state
            *(uint32_t*)((char*)trainingState + 32) = 0;
        }
        
        // Call network client with param=1, value=1.0f
        SetHudParameter(networkClient, 1, g_floatOne);
        return;
    }
    
    // Handle event code 2147 (0x863)
    if (eventCode == 2147) {
        // Call network client with param=0, value=1.0f
        SetHudParameter(networkClient, 0, g_floatOne);
        return;
    }
    
    // Handle event code 2080 (0x820)
    if (eventCode == 2080) {
        // Call network client with param=0, value=0.0f
        SetHudParameter(networkClient, 0, g_floatZero);
        return;
    }
    
    // No matching event code - return without action
}


// External data structures
extern uint32_t g_hudObjectArray[];      // @ 0x825C8ED8 (124 bytes, 31 entries)
extern void*    g_pHudBasePointer;       // @ 0x825EB988
extern uint32_t g_hudLookupTable[][2];   // @ 0x825B1F90 (696 bytes, 87 entries of 8 bytes)


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase::SetPropertyById @ 0x8215B060 | size: 0x84
//
// Sets a property value for a HUD object identified by an ID.
// Looks up the object ID in a global lookup table and stores the value
// at the corresponding offset in the object array.
//
// The function:
// 1. Calculates array index from objectId (objectId * 4)
// 2. Gets the object offset from g_hudObjectArray
// 3. Adds base pointer to get actual object address
// 4. Calls a virtual function at offset +96 on the object
// 5. Searches g_hudLookupTable (87 entries) for matching object offset
// 6. When found, stores the value at the corresponding slot (offset +4)
//
// Parameters:
//   objectId - ID of the HUD object (used as array index)
//   value    - Value to store for this object
// ─────────────────────────────────────────────────────────────────────────────
void hudFlashBase::SetPropertyById(int objectId, int value)
{
    // Calculate array index: objectId * 4 (rlwinm r30,r3,2,0,29)
    uint32_t arrayIndex = objectId * 4;
    
    // Get object offset from array
    uint32_t objectOffset = g_hudObjectArray[objectId];
    
    // Add base pointer to get actual object address
    void* hudObject = (char*)g_pHudBasePointer + objectOffset;
    
    // Call virtual function at offset +96
    typedef void (*VirtualFunc)(void*);
    VirtualFunc vfunc = *(VirtualFunc*)((char*)hudObject + 96);
    vfunc(hudObject);
    
    // Search lookup table for matching object offset
    // Table has 87 entries, each 8 bytes: [objectOffset, valueSlot]
    for (int i = 0; i < 87; i++) {
        if (g_hudLookupTable[i][0] == objectOffset) {
            // Found match - store value at offset +4 in the entry
            // rlwinm r6,r10,3,0,28 = i * 8 (entry size)
            g_hudLookupTable[i][1] = value;
            return;
        }
    }
    
    // No match found - return without storing
}
/**
 * ui/hud.cpp — Network and UI utility functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "ui/hud.hpp"
#include <stdint.h>

/* ── External dependencies for network cleanup ────────────────────────────── */

/* RAGE cleanup function @ 0x8214C788 */
extern void rage_C788(void* pObject);

/* Network cleanup function @ 0x82115B40 */
extern void net_5B40(void* pObject, int mode);

/* Vtable for rage::mfAnimationController @ 0x8205A054 */
extern void* g_vtable_mfAnimationController;  /* @ 0x8205A054 */


/* ═══════════════════════════════════════════════════════════════════════════
 * net_F5D8 @ 0x822DF5D8 | size: 0x5c (92 bytes)
 *
 * Network object cleanup/destructor function.
 *
 * This function performs cleanup operations on a network object, setting its
 * vtable and calling cleanup functions on embedded objects.
 *
 * Network object layout (partial):
 *   +0x00 (0)    m_vtable      - Vtable pointer
 *   +0x04 (4)    m_embedded    - Embedded object requiring cleanup
 *   +0x1C (28)   m_pResource   - Resource pointer (at embedded+24)
 *   +0x22 (34)   m_flags       - Status flags (at embedded+30)
 *
 * Algorithm:
 *   1. Set vtable to mfAnimationController
 *   2. Call rage_C788 on embedded object at offset +4
 *   3. If flags at embedded+30 are non-zero:
 *      a. Check if resource pointer at embedded+24 is non-null
 *      b. If so, call net_5B40 with mode 3 to clean up resource
 * ═══════════════════════════════════════════════════════════════════════════ */
void net_F5D8(void* pThis)
{
    uint8_t* netObj = (uint8_t*)pThis;
    
    /* Set vtable pointer */
    *(void**)netObj = &g_vtable_mfAnimationController;
    
    /* Get pointer to embedded object at offset +4 */
    void* pEmbedded = netObj + 4;
    
    /* Call RAGE cleanup on embedded object */
    rage_C788(pEmbedded);
    
    /* Check flags at embedded+30 */
    uint16_t flags = *(uint16_t*)((uint8_t*)pEmbedded + 30);
    
    if (flags != 0) {
        /* Load resource pointer from embedded+24 */
        void* pResource = *(void**)((uint8_t*)pEmbedded + 24);
        
        if (pResource != NULL) {
            /* Clean up resource with mode 3 */
            net_5B40(pResource, 3);
        }
    }
}
