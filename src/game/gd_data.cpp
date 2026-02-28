/**
 * gd_data.cpp — Game data management classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Player manager and game data structures.
 */

#include "gd_data.hpp"

// External function declarations
extern void rage_free(void* ptr);
extern void rage::ReleaseSingleton(void* obj);
extern void game_8EE8(void* obj);                    // @ 0x820F8EE8 - Object cleanup
extern void util_6C20(void* obj, uint32_t flags);    // @ 0x82566C20 - Free/release with flags
extern bool rage::FindSingleton(void* obj);        // @ 0x820F90D0 - Check if singleton exists

// Global state
extern uint32_t g_plrPlayerMgr_state;  // @ 0x82066430

/**
 * game_B2E0 @ 0x8218B2E0 | size: 0x90
 * 
 * Destroys sub-object array containing 2 sub-objects (24 bytes each).
 * Each sub-object has 2 pointers: primary object and singleton-managed object.
 * Processes in reverse order (sub-object 1, then sub-object 0).
 * 
 * Sub-object structure (24 bytes):
 *   +0x00: Primary object pointer
 *   +0x10: Singleton-managed object pointer
 * 
 * Algorithm:
 *   1. Start at base + 48 (end of array)
 *   2. Loop 2 times, moving back 24 bytes each iteration
 *   3. For each sub-object:
 *      - If primary object exists: call game_8EE8, then util_6C20 with flag 0xE001
 *      - If singleton object exists and not in registry: free via allocator vtable slot 2
 */
void game_B2E0(void* subObjectArrayBase) {
    struct SubObject {
        void* m_pObject1;       // +0x00
        uint32_t m_padding[3];  // +0x04
        void* m_pObject2;       // +0x10
        uint32_t m_padding2;    // +0x14
    };
    
    SubObject* subObjects = (SubObject*)subObjectArrayBase;
    
    // Process 2 sub-objects in reverse order
    for (int i = 1; i >= 0; i--) {
        SubObject* subObj = &subObjects[i];
        
        // Handle primary object
        if (subObj->m_pObject1) {
            game_8EE8(subObj->m_pObject1);
            util_6C20(subObj->m_pObject1, 0xE001);
        }
        
        // Handle singleton-managed object
        if (subObj->m_pObject2) {
            // Check if object is in singleton registry
            if (!rage::FindSingleton(subObj->m_pObject2)) {
                // Not in registry - free via allocator
                // Get allocator from TLS and call vtable slot 2 (Free/Release)
                // TODO: Implement proper TLS allocator access
                // For now, this is a placeholder showing the intent
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPlayerMgr  [2 vtables @ 0x82039A24 and 0x82039AB0]
//
// Player manager with multiple inheritance.
// Contains 2 sub-objects at offset +20 (48 bytes total).
// ─────────────────────────────────────────────────────────────────────────────

/**
 * plrPlayerMgr::~plrPlayerMgr() @ 0x82188D40 | size: 0x98
 *
 * Destructor for player manager with multiple inheritance.
 * 
 * Structure (verified via Python analysis):
 *   +0x00 (0):  Primary vtable @ 0x82039A24
 *   +0x10 (16): Secondary vtable @ 0x82039AB0
 *   +0x14 (20): Sub-object array (48 bytes, 2 objects × 24 bytes)
 *     Sub-object 0: +0x14 (20) - pointers at +20 and +36
 *     Sub-object 1: +0x2C (44) - pointers at +44 and +60
 *
 * Destructor sequence:
 *   1. Set initial vtables for destruction phase
 *   2. Clear global player manager state
 *   3. Destroy sub-objects via game_B2E0 (processes in reverse order)
 *   4. Restore base class vtables
 *   5. Call singleton cleanup
 *   6. Conditional free handled by caller based on flags
 */
plrPlayerMgr::~plrPlayerMgr() {
    // Set initial vtables for destruction phase
    vtable = (void**)0x82039A24;    // m_vtable1   // Primary vtable
    // m_vtable2 (secondary vtable at +16): 0x82039AB0   // Secondary vtable (at offset +16)
    
    // Clear global player manager state
    g_plrPlayerMgr_state = 0;  // @ 0x82066430
    
    // Destroy sub-objects in reverse order
    // Pass pointer to sub-object array base at offset +20
    game_B2E0(&m_subObjects[0]);
    
    // Restore base class vtables after sub-object destruction
    m_vtable2 = (void**)0x82027B34;   // Base class vtable (secondary)
    m_vtable1 = (void**)0x8203338C;   // Base class vtable (primary)
    
    // Call singleton cleanup
    rage::ReleaseSingleton(this);
    
    // Note: Conditional free handled by caller
    // If (flags & 1), caller will invoke rage_free(this)
}


////////////////////////////////////////////////////////////////////////////////
// Ladder / Rivalry System — merged from gd_ladder.cpp
////////////////////////////////////////////////////////////////////////////////

/**
 * Ladder/Rivalry System Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "gd_data.hpp"
#include <string.h>
#include <stdio.h>

// External function declarations
extern "C" {
    void nop_8240E6D0(const char* message);
    void xmlNodeStruct_vfn_2(void* node);
}

/**
 * gdTier::PostLoadProperties
 * @ 0x821F0EA0 | size: 0x6C
 * 
 * Validates that the tier has a name specified.
 * Called after XML properties are loaded.
 * 
 * Error message @ 0x8204280C:
 *   "gdTier::PostLoadProperties() - 'TierName' not specified"
 */
void gdTier::PostLoadProperties() {
    // Call base class post-load
    xmlNodeStruct_vfn_2(this);
    
    // Check if tier name is specified
    if (m_tierName == nullptr || m_tierName[0] == '\0') {
        nop_8240E6D0("gdTier::PostLoadProperties() - 'TierName' not specified");
    }
}

/**
 * gdTier::PostLoadChildren
 * @ 0x821F0F10 | size: 0x120
 * 
 * Processes child nodes after loading.
 * Initializes tier members and validates structure.
 */
void gdTier::PostLoadChildren() {
    // TODO: Implement tier member initialization
    // Original processes child XML nodes and builds member array
}

/**
 * gdTierMember::PostLoadProperties
 * @ 0x821F0C88 | size: 0xA0
 * 
 * Validates that the tier member has a character name specified
 * and resolves the character data pointer.
 * 
 * Error messages:
 *   "gdTierMember::PostLoadProperties() - 'CharacterName' not specified"
 *   "gdTierMember::PostLoadProperties() - unknown character '%s'"
 */
void gdTierMember::PostLoadProperties() {
    // Call base class post-load
    xmlNodeStruct_vfn_2(this);
    
    // Check if character name is specified
    if (m_characterName == nullptr || m_characterName[0] == '\0') {
        nop_8240E6D0("gdTierMember::PostLoadProperties() - 'CharacterName' not specified");
        return;
    }
    
    // TODO: Resolve character data pointer from name
    // Original looks up character in character database
    // If not found, prints error with character name
}

/**
 * gdLadder::PostLoadChildren
 * @ 0x821F1FE8 | size: 0x148
 * 
 * Processes child tier nodes after loading.
 * Builds the ladder structure from XML data.
 */
void gdLadder::PostLoadChildren() {
    // TODO: Implement ladder tier initialization
    // Original processes child XML nodes and builds tier array
}

/**
 * gdRivalry::PostLoadChildren
 * @ (address TBD) | size: (TBD)
 * 
 * Processes rivalry relationship data after loading.
 */
void gdRivalry::PostLoadChildren() {
    // TODO: Implement rivalry initialization
    // Original processes rivalry matchup data
}

////////////////////////////////////////////////////////////////////////////////
// RAGE Serialization System
////////////////////////////////////////////////////////////////////////////////

/**
 * RegisterSerializedField @ 0x821A8F58 | size: 0xC8
 *
 * Original symbol: game_8F58
 *
 * RAGE serialization system field registration helper.
 * Registers a single field of a data object with the serialization system,
 * tracking field metadata and managing registration state.
 *
 * This function is called during asset load/bind (vtable slot 21 - RegisterFields)
 * for all RAGE data singleton classes (creditsData, dialogData, frontendData, etc.)
 *
 * Parameters:
 *   obj       - The data object being registered (this pointer)
 *   fieldKey  - Field identifier (string or schema pointer)
 *   fieldPtr  - Pointer to the field within the object
 *   schemaDesc- Schema descriptor for the field type
 *   flags     - Registration flags (usually 0)
 *
 * The function maintains registration state in a global singleton accessed via
 * SDA offset 0 (g_serializationRegistry @ 0x82600000), tracking:
 *   +0x04: Current registration index
 *   +0x08: Maximum registration capacity
 *   +0x0C: Overflow counter (incremented when capacity exceeded)
 *   +0x10: Previous registration index (for rollback)
 *
 * When the current index equals capacity, the function:
 *   1. Increments the overflow counter
 *   2. Stores the previous index for potential rollback
 *   3. Resets current index to the stored previous value
 *
 * Otherwise, it:
 *   1. Decrements the overflow counter (if > 0)
 *   2. Stores the previous index
 *
 * Finally, it:
 *   1. Retrieves the field registry singleton (16-byte aligned)
 *   2. Frees any existing field metadata at the current slot
 *   3. Hashes the field key to generate a unique identifier
 *   4. Stores the new field metadata (hash, fieldPtr, schemaDesc)
 *   5. Clears the registration-in-progress flag
 */

// External dependencies
extern void* atSingleton_91E0_gen(uint32_t size);      // @ 0x821A91E0 - Get singleton
extern void* rage::UnregisterSingleton(const void* key);      // @ 0x820C29E0 - Hash field key
extern void rage_free(void* ptr);                 // @ 0x820C00C0 - Free memory

// Global serialization registry (SDA offset 0)
extern uint32_t g_serializationRegistry;  // @ 0x82600000

void RegisterSerializedField(void* obj,
                             const void* fieldKey,
                             void* fieldPtr,
                             const void* schemaDesc,
                             uint32_t flags)
{
    // Access the global serialization registry (SDA base + 0)
    uint32_t* registry = (uint32_t*)g_serializationRegistry;
    
    // Load registration state
    uint32_t currentIdx = registry[1];   // +0x04: current index
    uint32_t maxCapacity = registry[2];  // +0x08: max capacity
    
    // Check if we've hit capacity
    if (currentIdx == maxCapacity) {
        // Overflow: increment counter and store previous index
        uint32_t overflowCount = registry[3];  // +0x0C
        registry[3] = overflowCount + 1;
        
        uint32_t prevIdx = registry[4];  // +0x10: previous index
        registry[4] = currentIdx;
        registry[1] = prevIdx;  // Reset to previous
    } else {
        // Normal path: decrement overflow counter if needed
        uint32_t overflowCount = registry[3];
        if (overflowCount > 0) {
            registry[3] = overflowCount - 1;
        }
        
        // Store previous index
        uint32_t prevIdx = registry[4];
        registry[4] = currentIdx;
    }
    
    // Get the field registry singleton (16-byte aligned allocation)
    void* fieldRegistry = atSingleton_91E0_gen(16);
    
    // Free any existing field metadata at this slot
    void* existingMetadata = ((void**)fieldRegistry)[0];
    rage_free(existingMetadata);
    
    // Hash the field key to generate unique identifier
    void* fieldHash = rage::UnregisterSingleton(fieldKey);
    
    // Store new field metadata in the registry
    ((void**)fieldRegistry)[0] = fieldHash;      // +0x00: field hash/ID
    ((void**)fieldRegistry)[1] = fieldPtr;       // +0x04: field pointer
    ((void**)fieldRegistry)[2] = (void*)schemaDesc;  // +0x08: schema descriptor
    ((uint32_t*)fieldRegistry)[3] = 0;           // +0x0C: clear in-progress flag
}
