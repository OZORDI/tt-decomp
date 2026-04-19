/**
 * gd_data.cpp — Game data management classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Player manager and game data structures.
 */
// Binary string constants
#include "game/pong_strings.hpp"

#include "gd_data.hpp"
#include "globals.h"

// External function declarations
extern void rage_free(void* ptr);
extern void ReleaseObjectWithFlags(void* obj, uint32_t flags);    // @ 0x82566C20 - Free/release with flags
extern void sysMemAllocator_PlatformFree(void* obj, uint32_t flags);    // @ 0x82566C20 - Free/release with flags
extern bool atSingleton_IsTracked(void* obj);        // @ 0x820F90D0 - Check if singleton exists

// Forward declaration — canonical definition lives later in this file alongside
// the TemplateRegistry struct and the game_94F0 binary-search helper.
void datResourceMgr_RemoveEntry(void* templateKey);

// Global state
extern uint32_t g_plrPlayerMgr_state;  // @ 0x82066430

/**
 * plrPlayerMgr_DestroySubObjects @ 0x8218B2E0 | size: 0x90
 * Original symbol: plrPlayerMgr_ReleaseResourcePairs
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
void plrPlayerMgr_DestroySubObjects(void* subObjectArrayBase) {
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
            if (!atSingleton_IsTracked(subObj->m_pObject2)) {
                // Not in registry - free via allocator.
                // KNOWN LIMITATION: same TLS allocator dependency as
                // src/game/data/gd_data.cpp — see the comment there.
                // Keep both files in sync until rage_mem TLS lifts.
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
 *   3. Destroy sub-objects via plrPlayerMgr_DestroySubObjects (processes in reverse order)
 *   4. Restore base class vtables
 *   5. Call singleton cleanup
 *   6. Conditional free handled by caller based on flags
 */
plrPlayerMgr::~plrPlayerMgr() {
    // Set initial vtables for destruction phase
    m_vtable1 = (void**)0x82039A24;   // Primary vtable
    m_vtable2 = (void**)0x82039AB0;   // Secondary vtable (at offset +16)
    
    // Clear global player manager state
    g_plrPlayerMgr_state = 0;  // @ 0x82066430
    
    // Destroy sub-objects in reverse order
    // Pass pointer to sub-object array base at offset +20
    plrPlayerMgr_DestroySubObjects(&m_subObjects[0]);
    
    // Restore base class vtables after sub-object destruction
    m_vtable2 = (void**)0x82027B34;   // Base class vtable (secondary)
    m_vtable1 = (void**)0x8203338C;   // Base class vtable (primary)
    
    // Call singleton cleanup
    atSingleton_9420(this);
    
    // Note: Conditional free handled by caller
    // If (flags & 1), caller will invoke rage_free(this)
}


////////////////////////////////////////////////////////////////////////////////
// Statistics Classes — gdStatsOffline*, gdStatsOnline*, gdUnlockGroupData
////////////////////////////////////////////////////////////////////////////////

/**
 * Statistics tracking for offline and online gameplay.
 * These classes manage player statistics, achievements, and unlock conditions.
 */

// External utility function
extern void rage_FillMemory32(void* obj, uint32_t param1, uint32_t param2);

/**
 * gdStatsOfflineGeneral::~gdStatsOfflineGeneral
 * @ 0x8221CD40 | size: 0xB4
 * 
 * Destructor for offline general statistics.
 * Resets all stat fields to their default values (0 or 0.0f).
 * 
 * Structure contains 17 stat entries (16 bytes each):
 *   +0x00: vtable
 *   +0x04: stat array (17 entries × 16 bytes = 272 bytes)
 *   Each entry:
 *     +0x00: value (int or float)
 *     +0x04: type (1=int, 2=float)
 *     +0x08: additional data
 *     +0x0C: flags
 *   +0xF4 (244): cleanup data 1
 *   +0xFC (252): cleanup data 2
 *   +0x104 (260): cleanup data 3
 */
gdStatsOfflineGeneral::~gdStatsOfflineGeneral() {
    // Reset all 17 stat entries to default values
    for (int i = 0; i < 17; i++) {
        uint32_t* entry = (uint32_t*)((char*)this + 4 + (i * 16));
        uint32_t type = entry[1];  // Type field at +4
        
        if (type == 1) {
            // Integer type - set to 0
            entry[0] = 0;
        } else if (type == 2) {
            // Float type - set to 0.0f
            *(float*)&entry[0] = 0.0f;
        }
    }
    
    // Cleanup additional data structures
    rage_FillMemory32((char*)this + 244, 0xFFFFFFFF, 4);
    rage_FillMemory32((char*)this + 252, 0xFFFFFFFF, 4);
    rage_FillMemory32((char*)this + 260, 0xFFFFFFFF, 4);
}

/**
 * gdStatsOfflineHumanCPU::~gdStatsOfflineHumanCPU
 * @ 0x8221CFC0 | size: 0x148
 * 
 * Destructor for offline human vs CPU statistics.
 * Similar structure to gdStatsOfflineGeneral but with different stat count.
 */
/**
 * gdStatsOfflineHumanCPU::~gdStatsOfflineHumanCPU @ 0x8221CFC0 | size: 0x148
 *
 * Resets 8 stat field pairs at offsets +4/+8 through +52/+56, plus a
 * trailing loop of 3 pairs at +68/+72 through +84/+88. Each pair has
 * a value field and a type field (1=int, 2=float). Type 1 resets to 0,
 * type 2 resets to 0.0f.
 */
gdStatsOfflineHumanCPU::~gdStatsOfflineHumanCPU() {
    // Reset 8 stat pairs at fixed offsets (stride 8, value at N, type at N+4)
    struct StatPair { uint32_t value; int32_t type; };
    StatPair* stats = (StatPair*)((char*)this + 4);

    for (int i = 0; i < 8; i++) {
        if (stats[i].type == 1) {
            stats[i].value = 0;
        } else if (stats[i].type == 2) {
            *(float*)&stats[i].value = 0.0f;
        }
    }

    // Reset 3 trailing stat pairs starting at +68
    StatPair* trailing = (StatPair*)((char*)this + 68);
    for (int i = 0; i < 3; i++) {
        if (trailing[i].type == 1) {
            trailing[i].value = 0;
        } else if (trailing[i].type == 2) {
            *(float*)&trailing[i].value = 0.0f;
        }
    }
}

/**
 * gdStatsOfflineSession::~gdStatsOfflineSession @ 0x8221D2D0 | size: 0x90
 *
 * Resets 4 stat field pairs at offsets +4/+8 through +28/+32.
 */
gdStatsOfflineSession::~gdStatsOfflineSession() {
    struct StatPair { uint32_t value; int32_t type; };
    StatPair* stats = (StatPair*)((char*)this + 4);

    for (int i = 0; i < 4; i++) {
        if (stats[i].type == 1) {
            stats[i].value = 0;
        } else if (stats[i].type == 2) {
            *(float*)&stats[i].value = 0.0f;
        }
    }
}

/**
 * gdStatsOnlineGeneral::~gdStatsOnlineGeneral @ 0x8221D178 | size: 0xF0
 *
 * Resets 7 stat field pairs at offsets +4/+8 through +52/+56.
 */
gdStatsOnlineGeneral::~gdStatsOnlineGeneral() {
    struct StatPair { uint32_t value; int32_t type; };
    StatPair* stats = (StatPair*)((char*)this + 4);

    for (int i = 0; i < 7; i++) {
        if (stats[i].type == 1) {
            stats[i].value = 0;
        } else if (stats[i].type == 2) {
            *(float*)&stats[i].value = 0.0f;
        }
    }
}

/**
 * gdUnlockGroupData::~gdUnlockGroupData @ 0x82213748 | size: 0x78
 *
 * Destructor — clears the embedded array at +40, frees the allocated
 * condition array at +32 (if capacity > 0), frees the name string at +24,
 * then calls the base atSingleton destructor.
 */
gdUnlockGroupData::~gdUnlockGroupData() {
    extern "C" void rage_ClearArray(void* array);    // @ 0x8234C0E0
    extern "C" void rage_free(void* ptr);             // @ 0x820C00C0
    extern "C" void rage_ReleaseSingleton(void* obj); // @ 0x821A9420

    // Clear embedded array at +40
    rage_ClearArray((char*)this + 40);

    // Free condition array if allocated
    uint16_t capacity = *(uint16_t*)((char*)this + 38);
    if (capacity > 0) {
        void* condArray = *(void**)((char*)this + 32);
        rage_free(condArray);
    }

    // Free name string at +24
    void* nameStr = *(void**)((char*)this + 24);
    rage_free(nameStr);

    // Call base destructor
    rage_ReleaseSingleton(this);
}

/**
 * gdUnlockGroupData::IsType @ 0x8221C6D0 | size: 0x48
 */
bool gdUnlockGroupData::IsType(uint32_t typeId) {
    extern uint32_t g_gdUnlockGroupData_typeId;  // @ 0x825C5A60
    extern uint32_t g_xmlNodeStruct_typeId;       // @ 0x825C803C
    extern uint32_t g_xmlNodeStruct_typeId2;      // @ 0x825C8038

    if (typeId == g_gdUnlockGroupData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdUnlockGroupData::RegisterFields @ 0x8221C718 | size: 0xA4
 *
 * Registers 4 XML fields: GroupName (+16), Description (+20),
 * UnlockData (+24), and UnlockEnabled (+28).
 */
void gdUnlockGroupData::RegisterFields() {
    extern "C" void RegisterSerializedField(void*, const char*, void*, void*, int);
    extern void* g_hashFieldType;    // @ 0x825CB688
    extern void* g_enumFieldType;    // @ 0x825CAF90
    extern void* g_stringFieldType;  // @ 0x825CAF88
    extern void* g_boolFieldType;    // @ 0x825CAF80

    RegisterSerializedField(this, "GroupName",     (char*)this + 16, g_hashFieldType, 0);
    RegisterSerializedField(this, "Description",   (char*)this + 20, g_enumFieldType, 0);
    RegisterSerializedField(this, "UnlockData",    (char*)this + 24, g_stringFieldType, 0);
    RegisterSerializedField(this, "UnlockEnabled", (char*)this + 28, g_boolFieldType, 0);
}

/**
 * Scalar destructors (called by delete operator with flags)
////////////////////////////////////////////////////////////////////////////////
// HSM State Machine Helpers
////////////////////////////////////////////////////////////////////////////////

/**
 * PostStateTransitionRequest @ 0x822228B8 | size: 0x78
 * Original symbol: game_28B8
 * 
 * Posts a state transition request to the HSM (Hierarchical State Machine).
 * This is the core transition mechanism used by all game states.
 */
void* PostStateTransitionRequest(void* hsmContext, int stateIndex) {
    // Check if transition is allowed by calling vtable slot 6 (CanTransition)
    typedef bool (*CanTransitionFn)(void*);
    void** vtable = *(void***)hsmContext;
    CanTransitionFn canTransition = (CanTransitionFn)vtable[6];
    
    bool allowed = canTransition(hsmContext);
    
    if (!allowed) {
        // Transition blocked - return error string
        // Static string @ 0x8203DC08: "Transition not allowed"
        return (void*)0x8203DC08;
    }
    
    // Transition allowed - get the target state object
    // State array is at context+8, indexed by stateIndex * 4
    void** stateArray = *(void***)((uint8_t*)hsmContext + 8);
    void* targetState = stateArray[stateIndex];
    
    // Call vtable slot 13 on the target state (CreateTransitionRequest)
    typedef void* (*CreateTransitionRequestFn)(void*);
    void** stateVTable = *(void***)targetState;
    CreateTransitionRequestFn createRequest = (CreateTransitionRequestFn)stateVTable[13];
    
    return createRequest(targetState);
}

/**
 * InitializePageGroup @ 0x8231F4C0 | size: 0x16C
 * Original symbol: game_F4C0
 * 
 * Initializes a UI page group object for the frontend menu system.
 * Constructs a 220-byte object with dual vtables and embedded sub-objects.
 */
void InitializePageGroup(void* pageGroup) {
    // Call thread initialization check
    extern void sysMemAllocator_InitMainThread();
    sysMemAllocator_InitMainThread();
    
    // Cast to byte pointer for offset calculations
    uint8_t* pg = (uint8_t*)pageGroup;
    
    // Set up dual vtable pointers (multiple inheritance)
    *(void**)(pg + 0) = (void*)0x821A2498;   // Primary vtable
    *(void**)(pg + 4) = (void*)0x821A24D0;   // Secondary vtable (MI base)
    
    // Initialize embedded xmlTree objects at +96 and +112
    // Both use the rage::xmlTree vtable @ 0x8203A928
    void* xmlTreeVTable = (void*)0x8203A928;
    
    // Embedded object 1 at offset +96
    *(void**)(pg + 96) = xmlTreeVTable;
    *(uint32_t*)(pg + 100) = 0;
    *(uint32_t*)(pg + 104) = 0;
    *(uint16_t*)(pg + 108) = 0;
    *(uint16_t*)(pg + 110) = 0;
    
    // Embedded object 2 at offset +112
    *(void**)(pg + 112) = xmlTreeVTable;
    *(uint32_t*)(pg + 116) = 0;
    *(uint32_t*)(pg + 120) = 0;
    *(uint16_t*)(pg + 124) = 0;
    *(uint16_t*)(pg + 126) = 0;
    
    // Zero-initialize state fields
    *(uint32_t*)(pg + 128) = 0;
    *(uint16_t*)(pg + 132) = 0;
    *(uint16_t*)(pg + 134) = 0;
    *(uint32_t*)(pg + 136) = 0;
    *(uint32_t*)(pg + 140) = 0;
    *(uint16_t*)(pg + 144) = 0;
    *(uint16_t*)(pg + 146) = 0;
    *(uint32_t*)(pg + 148) = 0;
    
    // Initialize float fields to 0.0f
    // Float constant loaded from 0x8202D110
    float zeroFloat = 0.0f;
    *(float*)(pg + 164) = zeroFloat;
    *(float*)(pg + 168) = zeroFloat;
    *(float*)(pg + 172) = zeroFloat;
    
    // Initialize flags
    *(uint8_t*)(pg + 153) = 0;
    *(uint8_t*)(pg + 154) = 1;  // Enable flag
    *(uint8_t*)(pg + 155) = 0;
    *(uint8_t*)(pg + 156) = 0;
    
    // Allocate page manager sub-object (32 bytes, 16-byte aligned)
    // Uses main thread allocator from SDA[0][1]
    extern uint32_t g_mainAllocTable[];
    uint32_t* allocTableBase = (uint32_t*)g_mainAllocTable;
    void* allocator = (void*)allocTableBase[1];
    
    // Call vtable slot 1 (Allocate) on the allocator
    typedef void* (*AllocateFn)(void*, uint32_t, uint32_t);
    void** allocVTable = *(void***)allocator;
    AllocateFn allocate = (AllocateFn)allocVTable[1];
    
    void* pageManager = allocate(allocator, 32, 16);
    
    if (pageManager != nullptr) {
        // Initialize the allocated sub-object
        *(uint32_t*)pageManager = 0;
        *(uint16_t*)((uint8_t*)pageManager + 4) = 0;
        *(uint16_t*)((uint8_t*)pageManager + 6) = 0;
    }
    
    // Store page manager pointer at offset +64
    *(void**)(pg + 64) = pageManager;
    
    // Initialize audio-related fields
    // Calls aud_D370 @ 0x8224D370 with audio config string
    extern void aud_D370(void* obj, const char* config);
    const char* audioConfig = (const char*)0x821A2370;  // "page_group_audio"
    aud_D370(pageGroup, audioConfig);
    
    // Set up page manager parameters if allocation succeeded
    if (pageManager != nullptr) {
        *(uint32_t*)((uint8_t*)pageManager + 8) = 2000;   // Timeout 1
        *(uint32_t*)((uint8_t*)pageManager + 12) = 2000;  // Timeout 2
        *(uint32_t*)((uint8_t*)pageManager + 16) = 2000;  // Timeout 3
        *(uint32_t*)((uint8_t*)pageManager + 20) = 2000;  // Timeout 4
        *(uint32_t*)((uint8_t*)pageManager + 24) = 5120;  // Buffer size
        *(uint8_t*)((uint8_t*)pageManager + 28) = 0;      // Active flag
    }
    
    // Zero-initialize remaining fields
    *(uint32_t*)(pg + 176) = 0;
    *(uint32_t*)(pg + 180) = 0;
    *(uint32_t*)(pg + 184) = 0;
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

void gdStatsOfflineSession::ScalarDtor(int flags) {
    this->~gdStatsOfflineSession();
    if (flags & 1) {
        rage_free(this);
    }
}

void gdStatsOnlineGeneral::ScalarDtor(int flags) {
    this->~gdStatsOnlineGeneral();
    if (flags & 1) {
        rage_free(this);
    }
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
extern void* rage_strDuplicate(const void* key);      // @ 0x820C29E0 - Hash field key
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
    void* fieldHash = rage_strDuplicate(fieldKey);
    
    // Store new field metadata in the registry
    ((void**)fieldRegistry)[0] = fieldHash;      // +0x00: field hash/ID
    ((void**)fieldRegistry)[1] = fieldPtr;       // +0x04: field pointer
    ((void**)fieldRegistry)[2] = (void*)schemaDesc;  // +0x08: schema descriptor
    ((uint32_t*)fieldRegistry)[3] = 0;           // +0x0C: clear in-progress flag
}


////////////////////////////////////////////////////////////////////////////////
// Object Lifecycle Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * Template Registry Management
 * 
 * The game maintains a global registry of template objects (8-byte key pairs)
 * used for asset management and object instantiation. The registry is a sorted
 * array with binary search for fast lookup.
 */

// Global template registry @ 0x82607C40 (65616 bytes)
// Structure: 8192 entries of 8 bytes each + 4-byte count at offset 0x10000
struct TemplateRegistry {
    struct Entry {
        uint32_t key1;      // +0x00 - Primary sort key
        uint32_t key2;      // +0x04 - Secondary sort key
    };
    
    Entry entries[8192];    // 0x10000 bytes (65536)
    uint32_t count;         // +0x10000 - Active entry count
    uint32_t padding[19];   // +0x10004..+0x1004F (total size 0x10050)
};

extern TemplateRegistry g_templateRegistry;  // @ 0x82607C40

/**
 * game_94F0 @ 0x820F94F0 | size: 0xA4
 * 
 * Binary search in sorted template registry.
 * Searches for an entry matching the given key pair.
 * 
 * @param registry Pointer to template registry
 * @param searchKey Pointer to 8-byte key pair to search for
 * @return Index of matching entry, or -1 if not found
 */
int32_t game_94F0(TemplateRegistry* registry, const uint32_t* searchKey) {
    int32_t count = registry->count;
    
    if (count <= 0) {
        return -1;
    }
    
    int32_t low = 0;
    int32_t high = count - 1;
    uint32_t key1 = searchKey[0];
    uint32_t key2 = searchKey[1];
    
    // Binary search
    while (low <= high) {
        int32_t mid = (low + high) / 2;
        TemplateRegistry::Entry* entry = &registry->entries[mid];
        
        // Compare primary key
        if (key1 < entry->key1) {
            high = mid - 1;
            continue;
        }
        
        // Primary key matches or is greater - check secondary key
        if (key1 == entry->key1 && key2 <= entry->key2) {
            return mid;  // Found exact match
        }
        
        // Key is greater - search upper half
        low = mid + 1;
    }
    
    return -1;  // Not found
}

/**
 * datResourceMgr_RemoveEntry @ 0x820F8EE8 | size: 0xC8
 * 
 * Removes a template entry from the global registry by index.
 * 
 * This function is called during object cleanup to unregister templates
 * from the global template registry. It performs:
 * 1. Binary search to find the entry index
 * 2. Shift remaining entries down to fill the gap
 * 3. Decrement the active count
 * 
 * The registry maintains a sorted array of template entries (8 bytes each)
 * with a count at offset 0x10000. Maximum capacity is 8192 entries.
 * 
 * Called by:
 * - pg_8C38_g, atSingleton_IsTracked, lvlLevelMgr_vfn_24
 * - fxAmbientMgr_vfn_24, plrPropMgr_vfn_24, plrPlayerMgr_ReleaseResourcePairs, pg_8918_gen
 * 
 * @param templateKey Pointer to 8-byte template key to remove
 */
void datResourceMgr_RemoveEntry(void* templateKey) {
    TemplateRegistry* registry = &g_templateRegistry;
    uint32_t* key = (uint32_t*)templateKey;
    
    // Find the entry index via binary search
    int32_t index = game_94F0(registry, key);
    
    // Error if template not found
    if (index == -1) {
        rage_DebugLog(g_str_needToPreloadTemplate);
        return;
    }
    
    int32_t count = registry->count;
    
    // If removing from middle of array, shift remaining entries down
    if (index < count - 1) {
        // Shift all entries after the removed one down by one slot
        for (int32_t i = index; i < count - 1; i++) {
            registry->entries[i] = registry->entries[i + 1];
        }
    }
    
    // Decrement count
    registry->count = count - 1;
}


// ===========================================================================
// plrPlayerMgr::DrawPlayerConditional @ 0x8218A288 | size: 0x60
//
// Conditionally draws a player based on multiple state checks.
//
// This function performs a series of checks before calling pongPlayer_Draw:
//   1. Looks up a base index from array at offset (index+33)*4
//   2. Gets player pointer from array at offset (baseIndex+29)*4
//   3. Returns early if player pointer is null
//   4. Checks value at offset (baseIndex+13)*12 - must be negative
//   5. Checks byte flag at (this+baseIndex+180) - must be zero
//   6. If all checks pass, calls pongPlayer_Draw(player, 1)
//
// The complex indexing suggests this is navigating through a multi-level
// data structure, possibly a player slot table with state flags.
//
// Python-verified:
//   - (index+33)*4 = array lookup for base index
//   - (baseIndex+29)*4 = player pointer lookup
//   - (baseIndex+13)*3*4 = (baseIndex+13)*12 = state value lookup
//   - baseIndex+180 = visibility/draw flag
// ===========================================================================
void plrPlayerMgr_DrawPlayerConditional(plrPlayerMgr* mgr, uint32_t index)
{
    // Step 1: Look up base index from manager's array
    uint32_t baseIndex = mgr->m_indexArray[index + 33];
    
    // Step 2: Get player pointer using base index
    pongPlayer* player = (pongPlayer*)mgr->m_indexArray[baseIndex + 29];
    
    // Early return if player doesn't exist
    if (player == nullptr) {
        return;
    }
    
    // Step 3: Check state value - must be negative to proceed
    // The *12 stride suggests this is accessing a 12-byte structure array
    int32_t stateValue = *(int32_t*)&mgr->m_indexArray[(baseIndex + 13) * 3];
    if (stateValue >= 0) {
        return;
    }
    
    // Step 4: Check visibility/draw flag at offset 180 from base
    // This uses the base index as a byte offset into the manager structure
    uint8_t* flagPtr = (uint8_t*)mgr + baseIndex;
    uint8_t drawFlag = flagPtr[180];
    
    // Only draw if flag is clear (0)
    if (drawFlag != 0) {
        return;
    }
    
    // All checks passed - draw the player with flag=1
    pongPlayer_Draw(player, 1);
}
