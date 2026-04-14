/**
 * gd_data.cpp — Game data management classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Player manager and game data structures.
 */

#include "gd_data.hpp"
#include "game/gd_data_strings.hpp"
#include <cstddef>
#include <cstdint>

// External function declarations
extern void rage_free(void* ptr);
extern void datResourceMgr_RemoveEntry(void* obj);                    // @ 0x820F8EE8 - Object cleanup
extern void sysMemAllocator_PlatformFree(void* obj, uint32_t flags);    // @ 0x82566C20 - Free/release with flags
extern void sysMemAllocator_InitMainThread(void);          // @ 0x820C0038 - Main thread init

namespace rage {
    void  ReleaseSingleton(void* obj);               // @ 0x821A9420
    bool  FindSingleton(void* obj);                  // @ 0x820F90D0
    void* UnregisterSingleton(const void* key);      // @ 0x820C29E0
}

// RAGE allocator structure (minimal definition for this file)
struct rageAllocatorVTable {
    void (*Destructor)(void* self);
    void* (*Allocate)(void* self, void* hint, size_t size);
    void (*Free)(void* self, void* ptr);
};

struct rageAllocator {
    rageAllocatorVTable* vtable;
};

// Global state
extern uint32_t g_plrPlayerMgr_state;  // @ 0x82066430
extern uint32_t* g_sda_base;            // @ SDA:0x82600000 - Small Data Area base

// Serialization / XML support
extern "C" void xmlNodeStruct_Initialize(void* obj);                              // @ 0x821A8988
extern "C" void rage_DebugLog(const char* msg, ...);                          // @ 0x8240E6D0
extern "C" void RegisterSerializationField(void* obj, const char* name,
                                           void* fieldPtr, void* typeDesc,
                                           int flags);                        // @ 0x821A8F58
extern "C" int32_t FindCharacterByName(void* gameDataMgr, const char* name); // @ 0x821D2458

// Game data manager singleton
extern void* g_gameDataMgr;             // @ 0x8271A2E4

// Type IDs for IsType checks
extern uint32_t g_gdRivalryData_typeId;  // @ 0x825C5F68
extern uint32_t g_gdRivalry_typeId;      // @ 0x825C5F6C
extern uint32_t g_gdTierMember_typeId;   // @ 0x825C5F70
extern uint32_t g_xmlNodeStruct_typeId;  // @ 0x825C803C (shared base type)
extern uint32_t g_xmlNodeStruct_typeId2; // @ 0x825C8038 (shared base type)

// Serialization type descriptor for string fields
extern void* g_stringFieldType;          // @ 0x825CAF88

/**
 * plrPlayerMgr_ReleaseResourcePairs @ 0x8218B2E0 | size: 0x90
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
 *      - If primary object exists: call datResourceMgr_RemoveEntry, then sysMemAllocator_PlatformFree with flag 0xE001
 *      - If singleton object exists and not in registry: free via allocator vtable slot 2
 */
void plrPlayerMgr_ReleaseResourcePairs(void* subObjectArrayBase) {
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
            datResourceMgr_RemoveEntry(subObj->m_pObject1);
            sysMemAllocator_PlatformFree(subObj->m_pObject1, 0xE001);
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
 *   3. Destroy sub-objects via plrPlayerMgr_ReleaseResourcePairs (processes in reverse order)
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
    plrPlayerMgr_ReleaseResourcePairs(&m_subObjects[0]);
    
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

// (extern declarations already provided above)

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
    xmlNodeStruct_Initialize(this);
    
    // Check if tier name is specified
    if (m_tierName == nullptr || m_tierName[0] == '\0') {
        rage_DebugLog(g_str_gdTier_noTierName);
    }
}

/**
 * gdTier::PostLoadChildren
 * @ 0x821F0F10 | size: 0x120
 * 
 * Processes child nodes after loading.
 * Initializes tier members and validates structure.
 */
/**
 * gdTier::PostLoadChildren @ 0x821F0F10 | size: 0x120
 *
 * Walks the linked list of child XML nodes at +12. For each child,
 * calls IsType to check if it's a gdTierMember. Counts matching
 * children, allocates an array at +20, then walks the list again
 * to populate the array with pointers to matching children.
 * Non-matching children are logged via their GetTypeName.
 */
extern "C" void* rage_alloc(uint32_t size);

void gdTier::PostLoadChildren() {
    extern uint32_t g_gdTierMember_typeId;  // @ 0x825C5F70

    // Pass 1: count matching children (virtual IsType / GetTypeName).
    uint32_t matchCount = 0;
    for (xmlNodeStruct* pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling) {
        if (pChild->IsType(g_gdTierMember_typeId)) {
            ++matchCount;
        } else {
            rage_DebugLog(g_str_gdTier_unknownNodeType, pChild->GetTypeName());
        }
    }

    // Allocate member array once (capacity stored in m_memberCap).
    if (m_memberCap == 0) {
        m_memberCap = (uint16_t)matchCount;
        m_pMembers  = (matchCount > 0)
            ? (gdTierMember**)rage_alloc(matchCount * sizeof(gdTierMember*))
            : nullptr;
    }
    m_numMembers = (uint16_t)matchCount;

    // Pass 2: populate member array in source order.
    uint32_t writeIdx = 0;
    for (xmlNodeStruct* pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling) {
        if (pChild->IsType(g_gdTierMember_typeId)) {
            m_pMembers[writeIdx++] = static_cast<gdTierMember*>(pChild);
        }
    }
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
    xmlNodeStruct_Initialize(this);

    // Validate character name is specified and non-empty
    if (m_characterName == nullptr || m_characterName[0] == '\0') {
        rage_DebugLog(g_str_gdTierMember_noCharName);
        return;
    }

    // Look up character index in the game data manager
    int32_t charIndex = FindCharacterByName(g_gameDataMgr, m_characterName);
    m_pCharData = (gdCharData*)(intptr_t)charIndex;

    if (charIndex < 0) {
        rage_DebugLog(g_str_gdTierMember_unknownChar);
    }
}

/**
 * gdLadder::PostLoadChildren
 * @ 0x821F1FE8 | size: 0x148
 * 
 * Processes child tier nodes after loading.
 * Builds the ladder structure from XML data.
 */
/**
 * gdLadder::PostLoadChildren @ 0x821F1FE8 | size: 0x148
 *
 * Allocates a fixed array of 4 tier slots at +16, zeroes them, then
 * walks child nodes. For each child matching gdTier type, stores it
 * at the index specified by the child's tier level field (+16).
 * Logs errors for duplicate or non-matching entries.
 */
void gdLadder::PostLoadChildren() {
    extern uint32_t g_gdTier_typeId;  // @ 0x825C5F80 (offset 24448 from SDA)

    // Allocate a fixed 4-slot tier array on first call.
    constexpr uint16_t kTierSlots = 4;
    if (m_tierCap == 0) {
        m_tierCap = kTierSlots;
        m_pTiers  = (gdTier**)rage_alloc(kTierSlots * sizeof(gdTier*));
    }
    m_numTiers = kTierSlots;
    for (uint16_t i = 0; i < kTierSlots; ++i) {
        m_pTiers[i] = nullptr;
    }

    // Walk children; place each gdTier into the slot named by its tier level (+0x10).
    for (xmlNodeStruct* pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling) {
        if (pChild->IsType(g_gdTier_typeId)) {
            gdTier* pTier = static_cast<gdTier*>(pChild);
            uint32_t tierIdx = pTier->m_tierLevel;  // union view of +0x10
            if (m_pTiers[tierIdx] != nullptr) {
                rage_DebugLog(g_str_gdLadder_duplicateLevel);
            } else {
                m_pTiers[tierIdx] = pTier;
            }
        } else {
            rage_DebugLog(g_str_gdLadder_unknownNodeType, pChild->GetTypeName());
        }
    }

    // Validate every slot got populated.
    for (uint16_t i = 0; i < m_numTiers; ++i) {
        if (m_pTiers[i] == nullptr) {
            rage_DebugLog(g_str_gdLadder_missingLadder, i);
        }
    }
}

/**
 * gdRivalry::PostLoadChildren
 * @ (address TBD) | size: (TBD)
 * 
 * Processes rivalry relationship data after loading.
 */
/**
 * gdRivalry::PostLoadChildren @ 0x821F09D0 | size: 0x1B8
 *
 * Allocates an array sized to the character count from the game data manager.
 * Walks child nodes, checking each against gdRivalryData type. For matching
 * children, resolves the character name (+16) to an index and stores the
 * child pointer at that index in the array. Validates no duplicate entries
 * and all expected slots are filled.
 */
void gdRivalry::PostLoadChildren() {
    extern uint32_t g_gdRivalryData_typeId;  // @ 0x825C5F68
    extern void* g_gameDataMgr;              // @ 0x8271A2E4

    // Game-data manager view: +0x18 m_pCharArray, +0x1C m_numCharacters.
    auto* dataMgr = reinterpret_cast<gdGameDataMgrView*>(g_gameDataMgr);
    const uint32_t charCount = dataMgr->m_numCharacters;

    // Allocate per-character entry array on first call.
    if (m_capacity == 0) {
        m_capacity = (uint16_t)charCount;
        m_pEntries = (charCount > 0)
            ? (gdRivalryData**)rage_alloc(charCount * sizeof(gdRivalryData*))
            : nullptr;
    }
    m_numEntries = (uint16_t)charCount;
    for (uint32_t i = 0; i < charCount; ++i) {
        m_pEntries[i] = nullptr;
    }

    // Walk children and drop each gdRivalryData into its character's slot.
    for (xmlNodeStruct* pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling) {
        if (pChild->IsType(g_gdRivalryData_typeId)) {
            auto* pRivalry = static_cast<gdRivalryData*>(pChild);
            const int32_t charIdx = FindCharacterByName(static_cast<void*>(dataMgr),
                                                        pRivalry->m_characterName);
            if (charIdx < 0) {
                rage_DebugLog(g_str_gdRivalry_unknownChar);
            } else if (m_pEntries[charIdx] != nullptr) {
                rage_DebugLog(g_str_gdRivalry_duplicateChar, pRivalry->m_characterName);
            } else {
                m_pEntries[charIdx] = pRivalry;
            }
        } else {
            rage_DebugLog(g_str_gdRivalry_unknownNodeType, pChild->GetTypeName());
        }
    }

    // Every character must have a rivalry entry — report the missing ones.
    for (uint16_t i = 0; i < m_numEntries; ++i) {
        if (m_pEntries[i] == nullptr) {
            auto** charArray = reinterpret_cast<gdCharDataEntry**>(dataMgr->m_pCharArray);
            gdCharDataEntry* entry = charArray[i];
            rage_DebugLog(g_str_gdRivalry_missingRival, entry->m_entryName);
        }
    }
}

// ────────────────────────────────────────────────────────────────────────────
// gdRivalryData vtable methods
// ────────────────────────────────────────────────────────────────────────────

/**
 * gdRivalryData::IsType @ 0x821F0650 | size: 0x48
 *
 * Checks if the given type ID matches gdRivalryData or its base classes.
 */
bool gdRivalryData::IsType(uint32_t typeId) {
    if (typeId == g_gdRivalryData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdRivalryData::GetTypeName @ 0x821F0698 | size: 0xC
 *
 * Returns the class name string "gdRivalryData".
 */
const char* gdRivalryData::GetTypeName() const {
    return "gdRivalryData";
}

/**
 * gdRivalryData::RegisterFields @ 0x821F0738 | size: 0x6C
 *
 * Registers CharacterName (+16) and RivalName (+20) for XML serialization.
 */
void gdRivalryData::RegisterFields() {
    RegisterSerializationField(this, "CharacterName",
                               &m_characterName, g_stringFieldType, 0);
    RegisterSerializationField(this, "RivalName",
                               &m_rivalName, g_stringFieldType, 0);
}

/**
 * gdRivalryData::PostLoadProperties @ 0x821F07A8 | size: 0xE0
 *
 * Validates that both CharacterName and RivalName are specified and non-empty.
 * Resolves the rival character index from the game data manager.
 *
 * Debug strings:
 *   "gdRivalryData::PostLoadProperties() - 'CharacterName' not specified"
 *   "gdRivalryData::PostLoadProperties() - 'RivalName' not specified"
 *   "gdRivalryData::PostLoadProperties() - unknown rival character '%s'"
 */
void gdRivalryData::PostLoadProperties() {
    xmlNodeStruct_Initialize(this);

    if (m_characterName == nullptr || m_characterName[0] == '\0') {
        rage_DebugLog(g_str_gdRivalryData_noCharName);
    }
    if (m_rivalName == nullptr || m_rivalName[0] == '\0') {
        rage_DebugLog(g_str_gdRivalryData_noRivalName);
    }

    // Resolve rival character name into the cached index field.
    m_rivalCharIndex = FindCharacterByName(g_gameDataMgr, m_rivalName);
    if (m_rivalCharIndex < 0) {
        rage_DebugLog(g_str_gdRivalryData_unknownRival);
    }
}

// ────────────────────────────────────────────────────────────────────────────
// gdRivalry vtable methods
// ────────────────────────────────────────────────────────────────────────────

/**
 * gdRivalry::IsType @ 0x821F0888 | size: 0x48
 *
 * Checks if the given type ID matches gdRivalry or its base classes.
 */
bool gdRivalry::IsType(uint32_t typeId) {
    if (typeId == g_gdRivalry_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdRivalry::GetTypeName @ 0x821F08D0 | size: 0xC
 *
 * Returns the class name string "gdRivalry".
 */
const char* gdRivalry::GetTypeName() const {
    return "gdRivalry";
}

// ────────────────────────────────────────────────────────────────────────────
// gdTierMember vtable methods
// ────────────────────────────────────────────────────────────────────────────

/**
 * gdTierMember::IsType @ 0x821F0B88 | size: 0x48
 *
 * Checks if the given type ID matches gdTierMember or its base classes.
 */
bool gdTierMember::IsType(uint32_t typeId) {
    if (typeId == g_gdTierMember_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdTierMember::GetTypeName @ 0x821F0BD0 | size: 0xC
 *
 * Returns the class name string "gdTierMember".
 */
const char* gdTierMember::GetTypeName() const {
    return "gdTierMember";
}

/**
 * gdTierMember::RegisterFields @ 0x821F0C68 | size: 0x1C
 *
 * Registers CharacterName (+16) for XML serialization.
 * Tail-calls directly to the serialization helper.
 */
void gdTierMember::RegisterFields() {
    RegisterSerializationField(this, "CharacterName",
                               &m_characterName, g_stringFieldType, 0);
}

////////////////////////////////////////////////////////////////////////////////
// RAGE Serialization System
////////////////////////////////////////////////////////////////////////////////

/**
 * RegisterSerializedField @ 0x821A8F58 | size: 0xC8
 *
 * Original symbol: RegisterSerializationField
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
extern void* atSingletonPool_AllocEntry(uint32_t size);      // @ 0x821A91E0 - Get singleton
// rage::UnregisterSingleton declared in namespace block above
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
    void* fieldRegistry = atSingletonPool_AllocEntry(16);
    
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

/**
 * xe_13E8_1 @ 0x822113E8 | size: 0x90
 * 
 * Allocates and initializes a gdPropData object.
 * 
 * This is a factory function that:
 * 1. Ensures the main thread heap context is initialized
 * 2. Allocates 32 bytes with 16-byte alignment from the RAGE heap
 * 3. Initializes the gdPropData structure with its vtable and zero-filled fields
 * 4. Sets the last field to -1 (likely an invalid index sentinel)
 * 
 * @return Pointer to newly allocated gdPropData, or NULL if allocation failed
 */
gdPropData* xe_13E8_1(void) {
    // Ensure main thread heap is initialized
    sysMemAllocator_InitMainThread();
    
    // Get the active allocator from SDA context
    // The SDA base (r13) points to a context struct where offset +4 holds the allocator pointer
    uint32_t* sdaBase = (uint32_t*)(uintptr_t)g_sda_base[0];
    rageAllocator* allocator = (rageAllocator*)(uintptr_t)sdaBase[1];
    
    // Allocate 32 bytes with 16-byte alignment via allocator vtable slot 1
    gdPropData* propData = (gdPropData*)allocator->vtable->Allocate(
        allocator,
        (void*)16,  // alignment hint
        32          // size in bytes
    );
    
    if (propData != NULL) {
        // Initialize the gdPropData structure (32 bytes, vtable + 7 payload words).
        propData->vtable = (void**)0x8204A364;  // gdPropData vtable

        // Zero the 6 payload words after the vtable, then stamp an "invalid index"
        // sentinel into the last word. The inline struct view keeps the code
        // readable without bloating gd_data.hpp's public surface.
        struct gdPropDataInit {
            void**   vtable;     // +0x00
            uint32_t w04;        // +0x04
            uint32_t w08;        // +0x08
            uint32_t w0C;        // +0x0C
            uint32_t w10;        // +0x10
            uint32_t w14;        // +0x14
            uint32_t w18;        // +0x18
            int32_t  w1C;        // +0x1C — invalid-index sentinel
        };
        auto* init = reinterpret_cast<gdPropDataInit*>(propData);
        init->w04 = init->w08 = init->w0C = 0;
        init->w10 = init->w14 = init->w18 = 0;
        init->w1C = -1;
    }

    return propData;
}

////////////////////////////////////////////////////////////////////////////////
// AI Data System — gdai* classes
////////////////////////////////////////////////////////////////////////////////

// External global type identifiers for AI data validation
extern uint32_t* g_aiTypeId_1;  // @ 0x825C60B4
extern uint32_t* g_aiTypeId_2;  // @ 0x825C803C
extern uint32_t* g_aiTypeId_3;  // @ 0x825C8038

/**
 * gdaiMeterLogic::ValidateTypeId (vtable slot 20)
 * @ 0x821E91C0 | size: 0x48
 * 
 * Validates that the provided type identifier matches one of the expected
 * AI data type identifiers. This is part of the RAGE serialization system's
 * type safety mechanism.
 * 
 * The function checks against three global type identifiers:
 * - g_aiTypeId_1 @ 0x825C60B4
 * - g_aiTypeId_2 @ 0x825C803C  
 * - g_aiTypeId_3 @ 0x825C8038
 * 
 * @param typeId - Type identifier to validate
 * @return true if typeId matches any of the three expected identifiers, false otherwise
 * 
 * Pattern: All gdai* classes (gdaiData, gdaiDefPos, gdaiOffPos, gdaiMeterLogic,
 * gdaiShotSelection, gdaiSkillSet, gdaiDifficulty, gdaiSkillStats) share this
 * identical validation logic at vtable slot 20.
 */
bool gdaiMeterLogic::ValidateTypeId(uint32_t* typeId) {
    // Check against first type identifier
    if (typeId == g_aiTypeId_1) {
        return true;
    }
    
    // Check against second type identifier
    if (typeId == g_aiTypeId_2) {
        return true;
    }
    
    // Check against third type identifier
    if (typeId == g_aiTypeId_3) {
        return true;
    }
    
    return false;
}

/**
 * gdaiMeterLogic::GetTypeName (vtable slot 22)
 * @ 0x821E9208 | size: 0xC
 * 
 * Returns a pointer to the type name string for this AI data class.
 * Used for debugging and error reporting in the serialization system.
 * 
 * @return Pointer to type name string (appears to be part of a debug message)
 */
const char* gdaiMeterLogic::GetTypeName() {
    // Returns address 0x820418FC which contains a debug string
    // The actual string appears to be part of a larger validation message
    return reinterpret_cast<const char*>(0x820418FC);
}
