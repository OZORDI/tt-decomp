/**
 * msg_msg_sink.cpp — msgMsgSink constructor & destructor
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.14.cpp:73786
 *                                           tt-decomp_recomp.12.cpp:28709
 */

#include "../misc/pong_misc.hpp"
#include <cstring>

// External dependencies
extern void rage_free(void* ptr);
extern void msgMsgSink_A970_2h(void*);
extern void* msgMsgSink_F518_wrh(uint32_t);
extern void msgMsgSink_E860_g(void*, uint32_t, uint32_t, uint32_t);

// Vtable pointers (from .rdata)
static void* const kMsgMsgSink_Vtable      = (void*)0x8205B0D0;  // derived vtable
static void* const kMsgMsgSink_SubVtable   = (void*)0x8205B0DC;  // stored at +4
static void* const kMsgMsgSink_BaseVtable  = (void*)0x82027B34;  // base vtable (used in dtor)

// Constant data addresses used by constructor
extern const float  g_msgSinkInitFloat;   // @ 0x825C5938  initial float per slot
extern const uint32_t g_msgSinkTemplate[4]; // @ 0x8241A0C0  16-byte template per slot

/**
 * msgMsgSink::msgMsgSink() @ 0x822EC7F8 | size: 0x12C
 *
 * Initializes the message sink:
 *   1. Sets vtable and sub-vtable pointer
 *   2. Zeroes control byte at +8, fields at +12 and +20
 *   3. Fills 32 message-slot entries (1152-byte stride) starting at offset +0x360
 *   4. Zeroes the 840-byte region at +0x18 through +0x35F (210 uint32s)
 *   5. Zeroes sentinel fields at +0x9360 and +0x93A8
 */
msgMsgSink::msgMsgSink()
{
    // Set vtable and secondary pointer
    vtable = (void**)kMsgMsgSink_Vtable;
    field_0x0004 = (uint32_t)(uintptr_t)kMsgMsgSink_SubVtable;

    // Zero initial fields
    field_0x0008 = 0;
    field_0x000c = 0;
    field_0x0014 = 0;

    // Initialize 32 message-slot entries at offset 864 (0x360)
    // Each entry: 1152-byte stride (0x480)
    uint8_t* slotBase = (uint8_t*)this + 864;

    for (int i = 0; i < 32; i++) {
        uint8_t* slot = slotBase + (i * 1152);

        // +0x00: float constant
        *(float*)(slot + 0) = g_msgSinkInitFloat;
        // +0x04: zero
        *(uint32_t*)(slot + 4) = 0;
        // +0x08: zero
        *(uint32_t*)(slot + 8) = 0;
        // +0x0C: zero
        *(uint32_t*)(slot + 12) = 0;
        // +0x10: zero
        *(uint32_t*)(slot + 16) = 0;
        // +0x14: 1 (active flag)
        *(uint32_t*)(slot + 20) = 1;
        // +0x18: copy 16-byte template
        *(uint32_t*)(slot + 24) = g_msgSinkTemplate[0];
        *(uint32_t*)(slot + 28) = g_msgSinkTemplate[1];
        *(uint32_t*)(slot + 32) = g_msgSinkTemplate[2];
        *(uint32_t*)(slot + 36) = g_msgSinkTemplate[3];
        // +0x28: zero
        *(uint32_t*)(slot + 40) = 0;
        // +0x2C: -1 (sentinel)
        *(int32_t*)(slot + 44) = -1;
        // +0x30: zero
        *(uint32_t*)(slot + 48) = 0;
        // +0x34: zero (byte)
        *(uint8_t*)(slot + 52) = 0;
        // +0x38: zero
        *(uint32_t*)(slot + 56) = 0;
        // +0x3C: zero
        *(uint32_t*)(slot + 60) = 0;
    }

    // Zero sentinel at offset 864 + 36864 = 37728 (0x9360)
    *(uint32_t*)((uint8_t*)this + 864 + 36864) = 0;
    // Zero sentinel at offset 37784 (0x93A8)
    *(uint32_t*)((uint8_t*)this + 37784) = 0;

    // Zero 210 uint32s (840 bytes) starting at offset +24 (0x18)
    // This covers offsets +0x18 through +0x35F
    uint32_t* zeroStart = (uint32_t*)((uint8_t*)this + 24);
    for (int i = 0; i < 210; i++) {
        zeroStart[i] = 0;
    }
}

/**
 * msgMsgSink::~msgMsgSink() @ 0x822808A0 | size: 0x2C
 *
 * Restores base vtable.  If the delete-self flag (bit 0 of r4) is set,
 * frees the object via rage_free.
 */
msgMsgSink::~msgMsgSink()
{
    // Restore base vtable
    vtable = (void**)kMsgMsgSink_BaseVtable;

    // Note: if (flags & 1) rage_free(this);
    // Handled by the standard RAGE deleting-destructor ABI.
}

/**
 * msgMsgSink::InitializeExtended() @ 0x8245C078 | size: 0x90
 *
 * Extended initialization for msgMsgSink after base construction.
 * Called after the primary constructor to set up additional state.
 *
 * Operations:
 *   1. Calls base initialization (msgMsgSink_A970_2h)
 *   2. Sets primary vtable at +0 to 0x820054E8
 *   3. Sets secondary vtable at +12 to 0x82003E28
 *   4. Clears upper 5 bits of flag byte at +212
 *   5. Zeros fields at +68, +72, +76, +80, +84, +88, +92, +96
 *   6. Zeros 22 uint32 array starting at +112
 */
void msgMsgSink::InitializeExtended() {
    // Call base initialization
    extern void msgMsgSink_A970_2h(void*);
    msgMsgSink_A970_2h(this);
    
    // Set vtables
    *(void**)((char*)this + 0) = (void*)0x820054E8;   // Primary vtable
    *(void**)((char*)this + 12) = (void*)0x82003E28;  // Secondary vtable
    
    // Clear upper 5 bits of flag byte at +212 (keep lower 5 bits)
    uint8_t flags = *((uint8_t*)this + 212);
    flags &= 0x1F;  // clrlwi r7, r8, 27 - clear left 27 bits, keep right 5
    *((uint8_t*)this + 212) = flags;
    
    // Zero fields at +68 through +96 (8 uint32s)
    uint32_t* fields = (uint32_t*)((char*)this + 68);
    for (int i = 0; i < 8; i++) {
        fields[i] = 0;
    }
    
    // Zero 22 uint32 array starting at +112
    uint32_t* array = (uint32_t*)((char*)this + 112);
    for (int i = 0; i < 22; i++) {
        array[i] = 0;
    }
}

/**
 * msgMsgSink::ProcessMessageWithIndex() @ 0x8244E978 | size: 0x84
 *
 * Processes a message with index validation. Retrieves a message object
 * from the sink's internal storage, validates its type and index range,
 * then dispatches it for processing.
 *
 * Parameters:
 *   r3 (this) - msgMsgSink instance
 *   r4 - message parameter 1
 *   r6 - message index (uint16_t)
 *   r9 - message parameter 2
 *
 * Returns: void (calls msgMsgSink_E860_g for actual processing)
 */
void msgMsgSink::ProcessMessageWithIndex(uint32_t param1, uint16_t msgIndex, uint32_t param2) {
    // Get message object from internal storage at offset +284 (0x11C)
    void* msgObj = msgMsgSink_F518_wrh(this->field_0x011c);
    
    uint32_t calculatedOffset = 0;
    
    if (msgObj != nullptr) {
        // Load vtable/flags from message object
        uint32_t objHeader = *(uint32_t*)msgObj;
        
        // Extract message type from lower 5 bits
        uint32_t msgType = objHeader & 0x1F;  // clrlwi r10,r10,27
        
        // Extract some field by rotating and masking
        // rlwinm r6,r10,27,8,31 - rotate left 27, keep bits 8-31
        uint32_t rotatedField = ((objHeader << 27) | (objHeader >> 5)) & 0x00FFFFFF;
        
        // Extract lower 16 bits of index
        uint16_t indexLower = msgIndex & 0xFFFF;  // clrlwi r11,r31,16
        
        // Check if index is non-zero and message type is 18
        if (indexLower != 0 && msgType == 18) {
            // Load a 32-bit value from offset +11 in the message object
            uint32_t msgData = *(uint32_t*)((uint8_t*)msgObj + 11);
            
            // Extract upper 16 bits (count/limit)
            uint16_t msgCount = (uint16_t)(msgData >> 16);
            
            // Check if index is within valid range
            if (msgCount > indexLower) {
                // Extract lower 16 bits (stride/multiplier)
                uint16_t msgStride = (uint16_t)(msgData & 0xFFFF);
                
                // Calculate offset: stride * index
                calculatedOffset = msgStride * indexLower;
            }
        }
    }
    
    // Call the actual message processing function
    msgMsgSink_E860_g(this, param1, param2, calculatedOffset);
}

/**
 * msgMsgSink::DispatchVirtualMethod() @ 0x824542D0 | size: 0x30
 * [vtable slot 12]
 *
 * Dispatches a virtual method call through a chain of object pointers.
 * This is a common pattern in the RAGE engine for delegating operations
 * through a hierarchy of objects.
 *
 * Call chain:
 *   this->field_0x0028 (offset +40)
 *     ->field_0x0018 (offset +24)
 *       ->field_0x0024 (offset +36)
 *         ->vtable[9] (offset +36 in vtable)
 *
 * Returns early if any pointer in the chain is null.
 */
void msgMsgSink::DispatchVirtualMethod() {
    // Get first object pointer at offset +40
    void* obj1 = *(void**)((char*)this + 40);
    if (obj1 == nullptr) {
        return;
    }
    
    // Get second object pointer at offset +24 from first object
    void* obj2 = *(void**)((char*)obj1 + 24);
    
    // Get third object pointer at offset +36 from second object
    void* obj3 = *(void**)((char*)obj2 + 36);
    if (obj3 == nullptr) {
        return;
    }
    
    // Get vtable from third object
    void** vtable = *(void***)obj3;
    
    // Call virtual method at slot 9 (offset +36 in vtable)
    typedef void (*VirtualMethod)(void*);
    VirtualMethod method = (VirtualMethod)vtable[9];
    method(obj3);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * msgMsgSink_8DE0_sp @ 0x82458DE0 | size: 0x20 (32 bytes)
 *
 * Thunk function that forwards a call to vtable slot 29 of a nested object.
 *
 * This function loads a pointer from offset +36 of the msgMsgSink instance,
 * checks if it's non-null, and if so, calls vtable slot 29 on that object.
 *
 * msgMsgSink layout (partial):
 *   +0x24 (36)   m_pNestedObject  - Pointer to object with vtable
 *
 * The nested object's vtable slot 29 (offset +116) is invoked with the
 * nested object as the 'this' pointer.
 * ═══════════════════════════════════════════════════════════════════════════ */
void msgMsgSink_8DE0_sp(msgMsgSink* pThis)
{
    /* Load nested object pointer from offset +36 */
    void* pNestedObject = *(void**)((uint8_t*)pThis + 36);
    
    /* Check if nested object exists */
    if (pNestedObject == NULL) {
        return;
    }
    
    /* Call vtable slot 29 on the nested object */
    typedef void (*VtableSlot29Fn)(void*);
    void** vtable = *(void***)pNestedObject;
    VtableSlot29Fn slot29 = (VtableSlot29Fn)vtable[29];
    slot29(pNestedObject);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetNestedValue() [vtable slot 39 @ 0x8244F490]
// Complex bit-flag based navigation through nested data structures
// ─────────────────────────────────────────────────────────────────────────────
uint16_t msgMsgSink::GetNestedValue(void* dataPtr) {
    if (dataPtr == nullptr) {
        return 0;
    }
    
    // Load control flags from data structure
    uint8_t flags = *(uint8_t*)dataPtr;
    
    // Check if bit 4 is set (0x10)
    if ((flags & 0x10) == 0) {
        return 0;
    }
    
    // Check if bit 0 is set (0x01)
    if (flags & 0x01) {
        // Check if bit 1 is NOT set (0x02)
        if ((flags & 0x02) == 0) {
            // Check if bit 2 is NOT set (0x04)
            if ((flags & 0x04) == 0) {
                // Navigate: dataPtr + offset at +10
                uint16_t offset = *(uint16_t*)((uint8_t*)dataPtr + 10);
                uint8_t* target = (uint8_t*)dataPtr + offset;
                return *(uint16_t*)(target + 10);
            }
        }
        // Direct read from offset +10
        uint16_t offset = *(uint16_t*)((uint8_t*)dataPtr + 10);
        uint8_t* target = (uint8_t*)dataPtr + offset;
        return *(uint16_t*)(target + 10);
    }
    
    // Bit 0 not set - check alternate path
    // Check if bit 1 is NOT set (0x02)
    if ((flags & 0x02) == 0) {
        // Check if bit 2 is NOT set (0x04)
        if ((flags & 0x04) == 0) {
            // Navigate: dataPtr + offset at +12
            uint16_t offset = *(uint16_t*)((uint8_t*)dataPtr + 12);
            uint8_t* target = (uint8_t*)dataPtr + offset;
            return *(uint16_t*)(target + 12);
        }
    }
    
    // Direct read from offset +12
    uint16_t offset = *(uint16_t*)((uint8_t*)dataPtr + 12);
    uint8_t* target = (uint8_t*)dataPtr + offset;
    return *(uint16_t*)(target + 12);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetStateFlags() [vtable slot 38 @ 0x8244DF58]
// Returns state flags based on nested object state
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::GetStateFlags() {
    void* nestedObj = *(void**)((uint8_t*)this + 52);
    
    if (nestedObj == nullptr) {
        return 32;  // Default state when no nested object
    }
    
    // Load state value from nested object at offset +80
    uint32_t state = *(uint32_t*)((uint8_t*)nestedObj + 80);
    
    // Check if state is in range 1-6
    if (state >= 1 && state <= 6) {
        // Map state to flag value
        static const uint32_t stateFlags[] = { 1, 2, 4, 8, 16, 32 };
        uint32_t flags = stateFlags[state - 1];
        
        // Check additional flag at offset +52, bit 5 (0x20)
        uint8_t extraFlags = *(uint8_t*)((uint8_t*)nestedObj + 52);
        if (extraFlags & 0x20) {
            flags |= 64;  // Set bit 6
        }
        
        return flags;
    }
    
    // State out of range - check flag directly
    uint8_t extraFlags = *(uint8_t*)((uint8_t*)nestedObj + 52);
    uint32_t result = 0;
    
    if (extraFlags & 0x20) {
        result |= 64;
    }
    
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ProcessWithLock() [vtable slot 33 @ 0x8244DEC0]
// Thread-safe message processing with critical section
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::ProcessWithLock(uint32_t param) {
    // Get critical section from nested object at offset +56
    void* nestedObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)nestedObj + 144;
    
    // Enter critical section
    extern void RtlEnterCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);
    
    uint32_t result = 0;
    void* msgObj = *(void**)((uint8_t*)this + 52);
    
    if (msgObj != nullptr) {
        // Call vtable slot 16 to get state flags
        typedef uint32_t (*GetStateFn)(void*);
        void** vtable = *(void***)this;
        GetStateFn getState = (GetStateFn)vtable[16];
        uint32_t stateFlags = getState(this);
        
        // Check if bit 5 is set (0x20)
        if (stateFlags & 0x20) {
            // Return error code
            result = 0x8A650006;  // -1966669818 as unsigned
        } else {
            // Process message
            extern uint32_t msgMsgSink_3C88_g(void*, uint32_t);
            result = msgMsgSink_3C88_g(msgObj, param);
        }
    }
    
    // Leave critical section
    extern void RtlLeaveCriticalSection(void*);
    RtlLeaveCriticalSection(criticalSection);
    
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::CleanupIfReady() [vtable slot 40 @ 0x8244E0C0]
// Cleans up nested object if ready state is reached
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::CleanupIfReady() {
    void* nestedObj = *(void**)((uint8_t*)this + 52);
    
    if (nestedObj != nullptr) {
        // Call helper to check/cleanup nested object
        extern void msgMsgSink_4550_g(void*);
        msgMsgSink_4550_g(nestedObj);
    }
    
    // Check counter at offset +324
    int32_t counter = *(int32_t*)((uint8_t*)this + 324);
    
    if (counter != 0) {
        // Call vtable slot 16 to get state
        typedef uint32_t (*GetStateFn)(void*);
        void** vtable = *(void***)this;
        GetStateFn getState = (GetStateFn)vtable[16];
        uint32_t state = getState(this);
        
        // If state is 32, destroy nested object at offset +12
        if (state == 32) {
            void* objToDestroy = (uint8_t*)this + 12;
            
            // Call destructor (vtable slot 0) with delete flag
            typedef void (*DtorFn)(void*, int);
            void** objVtable = *(void***)objToDestroy;
            DtorFn dtor = (DtorFn)objVtable[0];
            dtor(objToDestroy, 1);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ProcessMessage() [vtable slot 23 @ 0x8244DDF8]
// Main message processing with state validation
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::ProcessMessage(uint32_t messageFlags) {
    // Get critical section from nested object
    void* nestedObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)nestedObj + 144;
    
    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);
    
    uint32_t result = 0;
    
    // Get state flags via vtable slot 16
    typedef uint32_t (*GetStateFn)(void*);
    void** vtable = *(void***)this;
    GetStateFn getState = (GetStateFn)vtable[16];
    uint32_t stateFlags = getState(this);
    
    // Check if bit 5 is NOT set (0x20)
    if ((stateFlags & 0x20) == 0) {
        // Check if bit 4 is set (0x10)
        stateFlags = getState(this);
        if (stateFlags & 0x10) {
            // Check mode at offset +60
            int32_t mode = *(int32_t*)((uint8_t*)this + 60);
            
            if (mode == 3) {
                RtlLeaveCriticalSection(criticalSection);
                return 0;
            }
            
            // Check if bit 0 of messageFlags is NOT set
            if ((messageFlags & 0x01) == 0) {
                RtlLeaveCriticalSection(criticalSection);
                return 0;
            }
        }
        
        // Process the message
        extern void msgMsgSink_DB70_g(void*, uint32_t);
        msgMsgSink_DB70_g(this, messageFlags);
        
        void* msgObj = *(void**)((uint8_t*)this + 52);
        if (msgObj != nullptr) {
            int32_t mode = *(int32_t*)((uint8_t*)this + 60);
            bool shouldDelete = (mode == 3);
            
            extern uint32_t msgMsgSink_5098_g(void*, bool);
            result = msgMsgSink_5098_g(msgObj, shouldDelete);
        }
    }
    
    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GenerateAndCleanup() [vtable slot 25 @ 0x8244DCF0]
// Generates something and cleans up nested object
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::GenerateAndCleanup() {
    // Get critical section
    void* nestedObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)nestedObj + 144;
    
    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);
    
    // Generate via helper
    extern int32_t msgMsgSink_84C0_gen(void*);
    int32_t result = msgMsgSink_84C0_gen(nestedObj);
    
    // If generation succeeded (result >= 0), destroy nested object at +12
    if (result >= 0) {
        void* objToDestroy = (uint8_t*)this + 12;
        
        typedef void (*DtorFn)(void*, int);
        void** objVtable = *(void***)objToDestroy;
        DtorFn dtor = (DtorFn)objVtable[0];
        dtor(objToDestroy, 1);
    }
    
    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::CheckAndProcess() [vtable slot 22 @ 0x8244DD50]
// Checks state flags and processes if conditions are met
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::CheckAndProcess() {
    // Get critical section
    void* nestedObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)nestedObj + 144;
    
    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);
    
    // Get state flags via vtable slot 16
    typedef uint32_t (*GetStateFn)(void*);
    void** vtable = *(void***)this;
    GetStateFn getState = (GetStateFn)vtable[16];
    uint32_t stateFlags = getState(this);
    
    uint32_t result;
    
    // Check if bit 1 is NOT set (0x02)
    if ((stateFlags & 0x02) == 0) {
        // Check if bit 2 is NOT set (0x04)
        stateFlags = getState(this);
        if ((stateFlags & 0x04) == 0) {
            // Return error code
            result = 0x8A650006;
        } else {
            // Process message
            void* msgObj = *(void**)((uint8_t*)this + 52);
            extern uint32_t msgMsgSink_4EB8_g(void*);
            result = msgMsgSink_4EB8_g(msgObj);
        }
    } else {
        // Process message
        void* msgObj = *(void**)((uint8_t*)this + 52);
        extern uint32_t msgMsgSink_4EB8_g(void*);
        result = msgMsgSink_4EB8_g(msgObj);
    }
    
    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::SendEvent() [vtable slot 52 @ 0x8244DB58]
// Sends an event with type 2 to nested object at offset +24
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::SendEvent() {
    // Call jumptable function with parameters
    // r3 = this + 24, r4 = 2, r5 = 0, r6 = 0
    void* target = (uint8_t*)this + 24;
    extern void jumptable_3A48(void*, uint32_t, uint32_t, uint32_t);
    jumptable_3A48(target, 2, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ClearPointers() [vtable slot 43 @ 0x8244E138]
// Clears two object pointers
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ClearPointers() {
    *(void**)((uint8_t*)this + 52) = nullptr;
    *(void**)((uint8_t*)this + 40) = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetNestedObjectValue() [vtable slot 46 @ 0x8244E148]
// Returns value from nested object or 0 if null
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::GetNestedObjectValue() {
    void* nestedObj = *(void**)((uint8_t*)this + 52);
    
    if (nestedObj == nullptr) {
        return 0;
    }
    
    // Call helper function on nested object
    extern uint32_t msgMsgSink_3D70_p39(void*);
    return msgMsgSink_3D70_p39(nestedObj);
}
