/**
 * Xbox 360 XAM Utility Functions
 * Rockstar Presents Table Tennis
 * 
 * Various XAM helper functions for state management, list operations,
 * and flag checking.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// Forward declarations of external functions
extern void xam_C328_fw(void* ptr, void* unused1, void* unused2, void* unused3);
extern void xam_FB20_g(void* obj, void* data, uint32_t flags, uint32_t param);
extern uint32_t xam_00A0_g(void* obj);
extern bool xam_F2D8_g(void* obj, const char* str);
extern bool SinglesNetworkClient_F358_g(void* obj, const char* str);
extern void xam_52F8_g(void* obj);
extern bool xam_B1A8(void* obj, const char* str, void* outBuffer);
extern void nop_8240E6D0(void* buffer, const char* format, const char* str);
extern bool sysMemSimpleAllocator_7EA8_p44(void* allocator);

//=============================================================================
// xam_4780 @ 0x82574780 | size: 0xBC
// 
// Cleans up and resets multiple linked lists in a manager structure.
// Iterates through an array of lists, calling cleanup on each entry,
// then unlinking all nodes.
//=============================================================================
void xam_4780(void* manager) {
    if (!manager) {
        return;
    }
    
    // Structure layout:
    // +80  (0x50)  - Array of list data pointers
    // +100 (0x64)  - Array of list head/tail pairs (8 bytes each)
    // +116 (0x74)  - Single list head/tail pair
    // +136 (0x88)  - Number of lists in the array
    
    uint32_t* listDataArray = (uint32_t*)((char*)manager + 80);
    uint32_t* listHeadArray = (uint32_t*)((char*)manager + 100);
    uint32_t* singleListHead = (uint32_t*)((char*)manager + 116);
    uint32_t listCount = *(uint32_t*)((char*)manager + 136);
    
    // Process each list in the array
    for (uint32_t i = 0; i < listCount; i++) {
        // Call cleanup function on the list data
        void* listData = (void*)listDataArray[i];
        xam_C328_fw(listData, NULL, NULL, NULL);
        
        // Unlink all nodes in this list
        uint32_t* listHead = &listHeadArray[i * 2];      // head at +0
        uint32_t* listTail = &listHeadArray[i * 2 + 1];  // tail at +4
        
        while (*listHead != 0) {
            uint32_t* node = (uint32_t*)*listHead;
            
            // If this is the last node, clear the tail pointer
            if (*listHead == *listTail) {
                *listTail = 0;
            }
            
            // Unlink: head = node->next
            *listHead = node[1];  // node->next at offset +4
            
            // Clear the node's next pointer
            node[1] = 0;
        }
    }
    
    // Process the single list at +116
    while (*singleListHead != 0) {
        uint32_t* node = (uint32_t*)*singleListHead;
        
        // If this is the last node, clear the tail pointer
        if (*singleListHead == singleListHead[1]) {
            singleListHead[1] = 0;
        }
        
        // Unlink: head = node->next
        *singleListHead = node[1];
        
        // Clear the node's next pointer
        node[1] = 0;
    }
}

//=============================================================================
// xam_9608_g @ 0x82369608 | size: 0x60
// 
// Checks multiple flag conditions to determine if an operation is allowed.
// Returns true if any of three conditions are met.
//=============================================================================
bool xam_9608_g(void* obj) {
    if (!obj) {
        return false;
    }
    
    // Check condition 1: field at +36 is non-negative
    int32_t field36 = *(int32_t*)((char*)obj + 36);
    if (field36 >= 0) {
        return true;
    }
    
    // Check condition 2: byte flag at +76 is set
    uint8_t flag76 = *(uint8_t*)((char*)obj + 76);
    if (flag76 != 0) {
        return false;
    }
    
    // Check condition 3: bit 6 of byte at +436 is set
    uint8_t flags436 = *(uint8_t*)((char*)obj + 436);
    return (flags436 & 0x40) != 0;
}

//=============================================================================
// xam_F460_g @ 0x8236F460 | size: 0x20
// 
// Checks if bit 16 of the field at offset +36 is set.
// Simple flag test function.
//=============================================================================
bool xam_F460_g(void* obj) {
    if (!obj) {
        return false;
    }
    
    uint32_t field36 = *(uint32_t*)((char*)obj + 36);
    return (field36 & 0x10000) != 0;
}

//=============================================================================
// xam_F2D8_g @ 0x8236F2D8 | size: 0x80
// 
// Searches for a matching string in an array of string entries.
// Each entry is 40 bytes. Returns true if a match is found.
//=============================================================================
bool xam_F2D8_g(void* obj, const char* searchStr) {
    if (!obj || !searchStr) {
        return false;
    }
    
    // Field at +448: number of entries
    int32_t entryCount = *(int32_t*)((char*)obj + 448);
    if (entryCount <= 0) {
        return false;
    }
    
    // Field at +1736: pointer to array of 40-byte entries
    const char* entryArray = (const char*)*(uint32_t*)((char*)obj + 1736);
    
    // Search through entries
    for (int32_t i = 0; i < entryCount; i++) {
        const char* entry = entryArray + (i * 40);
        
        // Compare strings
        if (strcmp(searchStr, entry) == 0) {
            return true;
        }
    }
    
    return false;
}

//=============================================================================
// xam_FC58 @ 0x8236FC58 | size: 0x74
// 
// Combines multiple flag bits into a result value.
// Calls another function and ORs the result with extracted flag bits.
//=============================================================================
uint32_t xam_FC58(void* obj) {
    if (!obj) {
        return 0;
    }
    
    // Get data pointer at +180
    void* data = (void*)((char*)obj + 180);
    
    // Load flags from +436 and +3512
    uint32_t flags436 = *(uint32_t*)((char*)obj + 436);
    uint8_t flags3512 = *(uint8_t*)((char*)obj + 3512);
    
    // Extract bit 5 from flags3512 (rotate left 27, mask bit 0)
    uint32_t bit5 = (flags3512 >> 5) & 0x1;
    
    // Get object pointer at +168
    void* objPtr = (void*)*(uint32_t*)((char*)obj + 168);
    
    // Call helper function
    xam_FB20_g(objPtr, data, flags436, bit5);
    
    // Reload flags3512 (may have been modified)
    flags3512 = *(uint8_t*)((char*)obj + 3512);
    
    // Check if any of the high bits (bits 1-7) are set
    uint32_t result = 0;
    if ((flags3512 & 0xFE) != 0) {
        result |= 1;
    }
    
    // Check bit 3 (mask 0x8)
    if ((flags3512 & 0x8) != 0) {
        result |= 16;
    }
    
    return result;
}

//=============================================================================
// xam_01A8_g @ 0x823701A8 | size: 0x98
// 
// Calculates a pointer into one of two arrays based on string matching.
// Returns NULL if no match is found.
//=============================================================================
void* xam_01A8_g(void* obj, const char* str) {
    if (!obj) {
        return NULL;
    }
    
    // Check first array using xam_F2D8_g
    if (xam_F2D8_g(obj, str)) {
        // Get index
        uint32_t index = xam_00A0_g(obj);
        
        // Calculate offset: (index + index * 4) * 8 = index * 40
        uint32_t offset = index * 40;
        
        // Get base pointer at +1736
        void* basePtr = (void*)*(uint32_t*)((char*)obj + 1736);
        
        return (char*)basePtr + offset;
    }
    
    // Check second array using SinglesNetworkClient_F358_g
    if (SinglesNetworkClient_F358_g(obj, str)) {
        // Get index
        uint32_t index = xam_00A0_g(obj);
        
        // Calculate offset: (index + index * 4) * 8 = index * 40
        uint32_t offset = index * 40;
        
        // Get base pointer at +1740
        void* basePtr = (void*)*(uint32_t*)((char*)obj + 1740);
        
        return (char*)basePtr + offset;
    }
    
    return NULL;
}

//=============================================================================
// xam_18A0_fw @ 0x823718A0 | size: 0x5C
// 
// Constructor/initializer for a worker object.
// Sets up vtable pointer and initializes fields.
//=============================================================================
void xam_18A0_fw(void* obj) {
    if (!obj) {
        return;
    }
    
    // Call parent/base initializer
    xam_52F8_g(obj);
    
    // Set vtable pointer at +0
    // Address calculation: lis r11,-32249 (0x8177) + addi r11,r11,-32524 (0x8174)
    // = 0x81770000 + 0xFFFF8174 = 0x81778174
    uint32_t vtableAddr = 0x81778174;
    *(uint32_t*)obj = vtableAddr;
    
    // Initialize fields
    *(int32_t*)((char*)obj + 24) = -1;  // field at +24 = -1
    *(uint32_t*)((char*)obj + 28) = 0;  // field at +28 = 0
    *(uint32_t*)((char*)obj + 32) = 0;  // field at +32 = 0
    
    // Clear bits 0-5 of byte at +36 (keep bits 6-7)
    uint8_t* flags36 = (uint8_t*)((char*)obj + 36);
    *flags36 = *flags36 & 0xC0;  // mask 0x3F, keep upper 2 bits
}

//=============================================================================
// xam_1978_wrh @ 0x82371978 | size: 0x68
// 
// Decrements a reference count and performs cleanup if it reaches zero.
// Returns the calculated value from flags.
//=============================================================================
uint32_t xam_1978_wrh(void* obj) {
    if (!obj) {
        return 0;
    }
    
    // Pointer to allocator at +24
    void* allocator = (void*)((char*)obj + 24);
    
    // Try to acquire lock/allocator
    if (!sysMemSimpleAllocator_7EA8_p44(allocator)) {
        return 0;
    }
    
    // Decrement reference count at +28 (allocator+4)
    int32_t* refCount = (int32_t*)((char*)allocator + 4);
    (*refCount)--;
    
    // Get flags from +36
    uint8_t flags36 = *(uint8_t*)((char*)obj + 36);
    
    // Calculate result: (flags36 >> 7) & 0x1FFFFFF
    // This extracts bits and shifts them
    uint32_t result = (flags36 >> 7) & 0x1FFFFFF;
    
    // If reference count reached zero, perform cleanup
    if (*refCount == 0) {
        // Set field at +24 (allocator+0) to -1
        *(int32_t*)allocator = -1;
        
        // Memory barrier (lwsync on PowerPC)
        __sync_synchronize();
        
        // Clear field at +32 (allocator+8)
        *(uint32_t*)((char*)allocator + 8) = 0;
    }
    
    return result;
}

//=============================================================================
// xam_1F78_g @ 0x82371F78 | size: 0x60
// 
// Tests if a specific bit is set in a 64-bit bitfield.
// Returns true if the bit at the given index is set.
//=============================================================================
bool xam_1F78_g(void* obj, int32_t bitIndex) {
    // Validate bit index is non-negative
    if (bitIndex < 0) {
        return false;
    }
    
    // Get count field at +0
    int32_t count = 0;
    if (obj != NULL) {
        count = *(int32_t*)obj;
    }
    
    // Check if bit index is within range
    if (bitIndex >= count) {
        return false;
    }
    
    // Get 64-bit bitfield at +4
    uint64_t bitfield = 0;
    if (obj != NULL) {
        bitfield = *(uint64_t*)((char*)obj + 4);
    }
    
    // Test the bit: (1 << bitIndex) & bitfield
    uint64_t mask = 1ULL << bitIndex;
    return (bitfield & mask) != 0;
}

//=============================================================================
// xam_9758_g @ 0x82369758 | size: 0x9C
// 
// Validates a string parameter and checks if it's valid.
// Returns true if validation succeeds.
//=============================================================================
bool xam_9758_g(void* obj, const char* str) {
    if (!obj || !str) {
        return false;
    }
    
    // Check field at +36 (must be non-negative)
    int32_t field36 = *(int32_t*)((char*)obj + 36);
    if (field36 < 0) {
        return false;
    }
    
    // Stack buffer for validation result
    uint32_t validationResult = 0;
    
    // Call validation function
    uint32_t error = xam_B1A8(obj, str, &validationResult);
    
    if (error == 0) {
        // Success - check if result is non-zero
        return (validationResult != 0);
    }
    
    // Error occurred - log it (buffer on stack at r1+96)
    // This would normally format an error message
    // For cross-platform, we skip the formatting
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Xbox Live Session State Helpers
// Small boolean accessors for network session state machines.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * IsMatchInProgress @ 0x8236F440 | size: 0x1C
 *
 * Returns true if the session's match state field (+168) equals 1,
 * indicating a match is actively in progress.
 * Called by 7 network coordinator functions.
 */
bool IsMatchInProgress(void* session) {  // xam_F440_g
    return *(int32_t*)((char*)session + 168) == 1;
}

/**
 * HasSessionHandle @ 0x8236F480 | size: 0x1C
 *
 * Returns true if the session has a non-null Xbox Live session
 * handle at offset +1760. Used to check if a session was created.
 */
bool HasSessionHandle(void* session) {  // xam_F480
    return *(void**)((char*)session + 1760) != NULL;
}

/**
 * IsSessionConnected @ 0x8236F4A0 | size: 0x1C
 *
 * Returns true if the session's connection state (+40) equals 3,
 * indicating the session is fully connected and ready for gameplay.
 */
bool IsSessionConnected(void* session) {  // xam_F4A0_g
    return *(int32_t*)((char*)session + 40) == 3;
}

/**
 * IsSessionConnecting @ 0x8236F4C0 | size: 0x1C
 *
 * Returns true if the session's connection state (+40) equals 2,
 * indicating the session is in the process of connecting.
 */
bool IsSessionConnecting(void* session) {  // xam_F4C0_g
    return *(int32_t*)((char*)session + 40) == 2;
}

/**
 * IsQueryComplete @ 0x8239A8B8 | size: 0x1C
 *
 * Returns true if a network data query's state (+28) equals 1,
 * indicating the query has completed. Used by NetDataQuery callers
 * and PongNetRoundRobinCoordinator.
 */
bool IsQueryComplete(void* query) {  // xam_A8B8_g
    return *(int32_t*)((char*)query + 28) == 1;
}

/**
 * IsAsyncOperationComplete @ 0x82426000 | size: 0x1C
 *
 * Returns true if an async Xbox operation's state (+36) equals 1.
 * Generic completion check used across multiple XAM subsystems.
 */
bool IsAsyncOperationComplete(void* asyncOp) {  // xam_5FF8_w
    return *(int32_t*)((char*)asyncOp + 36) == 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// XAM API Wrappers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * XamEnumerateSync @ 0x82587948 | size: 0x18
 *
 * Synchronous wrapper around XamEnumerate. Passes 0 as the first
 * parameter (no async callback handle), shifting all other args
 * up by one position.
 */
extern uint32_t XamEnumerate(uint32_t callbackHandle, void* buffer,
                             uint32_t bufferSize, uint32_t* itemCount,
                             void* overlapped);

uint32_t XamEnumerateSync(void* buffer, uint32_t bufferSize,
                          uint32_t* itemCount, void* overlapped) {  // xam_7948_g
    return XamEnumerate(0, buffer, bufferSize, itemCount, overlapped);
}

/**
 * DispatchSessionAction @ 0x8256AFA0 | size: 0x14
 *
 * Routes a session object to one of two handlers based on its
 * state field (+36). State 1 → active handler, otherwise → default.
 */
extern void SessionAction_Active(void* session);   // xam_ADB0_g
extern void SessionAction_Default(void* session);  // phInst_AC20_p39

void DispatchSessionAction(void* session) {  // xam_AFA0_sp
    if (*(int32_t*)((char*)session + 36) == 1) {
        SessionAction_Active(session);
    } else {
        SessionAction_Default(session);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CRT Static Initializer Registration
// MSVC generates these for global C++ object constructors. Each function
// prepends a 20-byte StaticInitNode to a singly-linked list headed at
// g_staticInitListHead (0x825EBCF4). The nodes are in .data at 0x825C9Dxx.
// ─────────────────────────────────────────────────────────────────────────────

typedef struct StaticInitNode {
    uint8_t data[12];       // +0x00: init function pointer + flags
    struct StaticInitNode* next;  // +0x0C: next node in list
    uint8_t padding[4];     // +0x10
} StaticInitNode;

extern StaticInitNode* g_staticInitListHead;  // @ 0x825EBCF4

static void RegisterStaticInit(StaticInitNode* node) {
    node->next = g_staticInitListHead;
    g_staticInitListHead = node;
}

// Each of these registers a different static initializer descriptor.
// They're called once during CRT startup before main().

extern StaticInitNode g_staticInit_0;  // @ 0x825C9DD4
extern StaticInitNode g_staticInit_1;  // @ 0x825C9DE8
extern StaticInitNode g_staticInit_2;  // @ 0x825C9DFC
extern StaticInitNode g_staticInit_3;  // @ 0x825C9E10
extern StaticInitNode g_staticInit_4;  // @ 0x825C9E24
extern StaticInitNode g_staticInit_5;  // @ 0x825C9E38
extern StaticInitNode g_staticInit_6;  // @ 0x825C9E4C
extern StaticInitNode g_staticInit_7;  // @ 0x825C9E60
extern StaticInitNode g_staticInit_8;  // @ 0x825C9E74

/** RegisterStaticInit_0 @ 0x8256B940 | size: 0x1C */
void RegisterStaticInit_0(void) { RegisterStaticInit(&g_staticInit_0); }  // xam_B940_sp

/** RegisterStaticInit_1 @ 0x8256B960 | size: 0x1C */
void RegisterStaticInit_1(void) { RegisterStaticInit(&g_staticInit_1); }  // xam_B960_sp

/** RegisterStaticInit_2 @ 0x8256B980 | size: 0x1C */
void RegisterStaticInit_2(void) { RegisterStaticInit(&g_staticInit_2); }  // xam_B980_sp

/** RegisterStaticInit_3 @ 0x8256B9A0 | size: 0x1C */
void RegisterStaticInit_3(void) { RegisterStaticInit(&g_staticInit_3); }  // xam_B9A0_sp

/** RegisterStaticInit_4 @ 0x8256B9C0 | size: 0x1C */
void RegisterStaticInit_4(void) { RegisterStaticInit(&g_staticInit_4); }  // xam_B9C0_sp

/** RegisterStaticInit_5 @ 0x8256B9E0 | size: 0x1C */
void RegisterStaticInit_5(void) { RegisterStaticInit(&g_staticInit_5); }  // xam_B9E0_sp

/** RegisterStaticInit_6 @ 0x8256BA00 | size: 0x1C */
void RegisterStaticInit_6(void) { RegisterStaticInit(&g_staticInit_6); }  // xam_BA00_sp

/** RegisterStaticInit_7 @ 0x8256BA20 | size: 0x1C */
void RegisterStaticInit_7(void) { RegisterStaticInit(&g_staticInit_7); }  // xam_BA20_sp

/** RegisterStaticInit_8 @ 0x8256BA40 | size: 0x1C */
void RegisterStaticInit_8(void) { RegisterStaticInit(&g_staticInit_8); }  // xam_BA40_sp

// ─────────────────────────────────────────────────────────────────────────────
// Misc XAM Utility Functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * GetLinkedListCount @ 0x8256C5E8 | size: 0x18
 *
 * Counts nodes in a singly-linked list. The list head is at this->field_0,
 * and each node's next pointer is at +4. Returns the count.
 */
int GetLinkedListCount(void* listObj) {  // xam_C5E8_gen
    int count = 0;
    void* node = *(void**)listObj;
    while (node) {
        node = *(void**)((char*)node + 4);
        count++;
    }
    return count;
}

/**
 * GetPropertyByteFromSubObj @ 0x8256F948 | size: 0x14
 *
 * Reads a uint16 from *(this->subObj+16)+92, truncates to byte,
 * and writes it to *outValue. Returns 0 (S_OK).
 */
int GetPropertyByteFromSubObj(void* obj, uint8_t* outValue) {  // xam_F948_2hr
    void* subObj = *(void**)((char*)obj + 16);
    uint16_t val = *(uint16_t*)((char*)subObj + 92);
    *outValue = (uint8_t)val;
    return 0;
}

/**
 * GetMaxBufferSize @ 0x8256D760 | size: 0x18
 *
 * Writes the constant 6172 to *outSize if the output pointer is
 * non-null. Returns 0 (S_OK). This is the max network buffer size.
 */
int GetMaxBufferSize(void* obj, uint32_t* outSize) {  // xam_D760_sp
    if (outSize) {
        *outSize = 6172;
    }
    return 0;
}

/**
 * GetSessionFlags @ 0x8256D818 | size: 0x18
 *
 * Copies this->flags (+8) to *outFlags if the output pointer is
 * non-null. Returns 0 (S_OK).
 */
int GetSessionFlags(void* obj, uint32_t* outFlags) {  // xam_D818_sp
    if (outFlags) {
        *outFlags = *(uint32_t*)((char*)obj + 8);
    }
    return 0;
}

/**
 * ShowDirtyDiscAndReset @ 0x825870E0 | size: 0x1C
 *
 * Shows the Xbox "dirty disc" error dialog, then calls the system
 * reset function with (0, 0) to return to dashboard.
 */
extern void XamShowDirtyDiscErrorUI(void);
extern void SystemReset(int param1, int param2);  // xam_9010_h

void ShowDirtyDiscAndReset(void) {  // xam_70E0
    XamShowDirtyDiscErrorUI();
    SystemReset(0, 0);
}

/**
 * InitNetworkDescriptor @ 0x8256AB18 | size: 0x1C
 *
 * Initializes a 5-field network descriptor struct:
 *   +0:  callback pointer (arg1)
 *   +4:  context pointer (arg2)
 *   +8:  flags (arg3)
 *   +12: reserved (zeroed)
 *   +16: user data (arg4)
 */
void InitNetworkDescriptor(void* desc, void* callback, void* context,
                           uint32_t flags, void* userData) {  // xam_AB18_2hr
    *(void**)((char*)desc + 0) = callback;
    *(void**)((char*)desc + 4) = context;
    *(uint32_t*)((char*)desc + 8) = flags;
    *(uint32_t*)((char*)desc + 12) = 0;
    *(void**)((char*)desc + 16) = userData;
}
