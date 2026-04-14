/**
 * msg_msg_sink.cpp — msgMsgSink constructor & destructor
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.14.cpp:73786
 *                                           tt-decomp_recomp.12.cpp:28709
 *
 * TODO: Duplicate function definitions exist for the same vtable addresses
 *       (different names for the same underlying function). Needs dedup:
 *       - 0x8244DCF0 (vslot 25): GenerateAndCleanup / GenerateWithLock
 *       - 0x8244DB58 (vslot 52): SendEvent / ResetEventDispatch
 *       - 0x8244E148 (vslot 46): GetNestedObjectValue / GetConnectionCount
 *       - 0x8244E628 (vslot 49): FlushAndDisconnect / OnDisconnect
 *       - 0x8244E680 (vslot 50): BeginDisconnect / OnReconnect
 *       - 0x8244E6D0 (vslot 30): FindValidMessageSlot / FindGamerByIndex
 *       - 0x8244FE80 (vslot 129): RegisterMessageHandler / CreateSessionLocked
 *       - 0x8244FEE8 (vslot 131): QueryConnectionState / QueryConnectionStatus
 *       - 0x8244EF38 (vslot 54): GetSessionPointer / GetSessionData
 *       - 0x8244F8A8 (vslot 55): ForwardProcessMessage / ForwardToBaseWrite
 *       - 0x8244F8B0 (vslot 89): ForwardDispatchMessage / ForwardToBaseUpdate
 *       - 0x824505C8 (vslot 147): ForwardCleanupMessage / ForwardToBaseNotify
 *       - 0x824508A8 (vslot 148): ForwardFinalizeMessage / ForwardToBaseProcess
 *       - 0x824514E8 (vslot 78): ForwardToRageHandler / ForwardToEmbeddedObject
 *       - 0x8244F7B8 (vslot 115): GetNameLength / GetMessageBufferPtr
 *       - 0x8244F768 (vslot 119): SendPulseToConnection / GetSessionLock
 *       - 0x824564A0 (vslot 120): DispatchEventWithSessionInfo / NotifyStateChange
 *       - 0x8244DCF0 / 0x8244E0C0 / 0x8244E138 / 0x8244E148 also appear as
 *         free-function msgMsgSink_XXXX_g alongside method versions.
 *       These should be consolidated; most represent the same function body
 *       renamed during incremental decomp passes.
 *
 * Vtable identity confirmed via RTTI:
 *   0x8205B0D0 — dialogManager vtable (msgMsgSink derived)
 *   0x82027B34 — msgMsgSink base vtable
 *   0x820054E8 / 0x82003E28 — anonymous .rdata vtables (InitializeExtended)
 */

#include "../misc/pong_misc.hpp"
#include <cstring>

// Xbox kernel critical-section stubs
extern void RtlEnterCriticalSection(void*);
extern void RtlLeaveCriticalSection(void*);

// External dependencies
extern void rage_free(void* ptr);
extern void msgMsgSink_A970_2h(void*);
extern void* msgMsgSink_F518_wrh(uint32_t);
extern void msgMsgSink_E860_g(void*, uint32_t, uint32_t, uint32_t);

// Vtable pointers (from .rdata)
static void* const kMsgMsgSink_Vtable      = (void*)0x8205B0D0;  // dialogManager vtable (derived)
static void* const kMsgMsgSink_SubVtable   = (void*)0x8205B0DC;  // dialogManager 2nd vtable (+4)
static void* const kMsgMsgSink_BaseVtable  = (void*)0x82027B34;  // msgMsgSink base vtable
static void* const kVtable_54E8            = (void*)0x820054E8;  // lbl_820054E8 (size 40)
static void* const kVtable_3E28            = (void*)0x82003E28;  // lbl_82003E28 (secondary)

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
    m_subVtable = (uint32_t)(uintptr_t)kMsgMsgSink_SubVtable;

    // Zero initial fields
    m_controlByte = 0;
    m_pSubVtable = 0;  // was field_0x000c
    m_field0x14 = 0;   // was field_0x0014

    // Initialize 32 message slots
    for (int i = 0; i < 32; i++) {
        MsgSlot* slot = &m_slots[i];

        slot->m_initFloat   = g_msgSinkInitFloat;
        slot->m_field04     = 0;
        slot->m_field08     = 0;
        slot->m_field0C     = 0;
        slot->m_field10     = 0;
        slot->m_activeFlag  = 1;
        slot->m_template[0] = g_msgSinkTemplate[0];
        slot->m_template[1] = g_msgSinkTemplate[1];
        slot->m_template[2] = g_msgSinkTemplate[2];
        slot->m_template[3] = g_msgSinkTemplate[3];
        slot->m_field28     = 0;
        slot->m_sentinel    = -1;
        slot->m_field30     = 0;
        slot->m_flagByte    = 0;
        slot->m_field38     = 0;
        slot->m_field3C     = 0;
    }

    // Zero sentinels after slot array
    m_slotSentinel = 0;
    m_tailSentinel = 0;

    // Zero 210 uint32s (840 bytes) from offset +0x18 through +0x35F
    // This covers the region between the header fields and the slot array
    uint32_t* zeroStart = (uint32_t*)(&m_eventSink);
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
    *(void**)((char*)this + 0) = kVtable_54E8;    // Primary vtable (0x820054E8)
    *(void**)((char*)this + 12) = kVtable_3E28;   // Secondary vtable (0x82003E28)
    
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
    void* msgObj = msgMsgSink_F518_wrh(this->m_connectionHandle);
    
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


/* ═══════════════════════════════════════════════════════════════════════════
 * Small msgMsgSink functions (<=64 bytes)
 * 34 stubs: vtable thunks, field accessors, forwarding wrappers
 * ═══════════════════════════════════════════════════════════════════════════ */

// Forward declarations for tail-call targets
extern void msgMsgSink_6220_w(void*);
extern void rage_C1A8(void*);
extern void atSingleton_5CD0_fw(void*, uint32_t);
extern void* RtlEnterCriticalSection_D6F0_fw(void*);
extern void msgMsgSink_D168_sp(void*);
extern void msgMsgSink_D1D8_sp(void*);
extern void msgMsgSink_6138_2hr(void*);
extern void grc_2CC8(void*, uint32_t, uint32_t);
extern void atSingleton_8E88_p42(void*, void*, uint32_t, uint32_t, uint32_t);
extern void ph_EF40(void*, void*);
extern void game_9CF8_h(void*, uint32_t);
extern void game_9D10_h(void*, uint32_t);
extern void game_9D28_h(void*, uint32_t);

// Vtable constants (from .rdata)
static void* const kVtable_4A94 = (void*)0x82004A94;  // lbl_82004A94
static void* const kVtable_4FF0 = (void*)0x82004FF0;  // lbl_82004FF0
static void* const kVtable_4FE8 = (void*)0x82004FE8;  // lbl_82004FE8
static void* const kVtable_4FE4 = (void*)0x82004FE4;  // lbl_82004FE4
static void* const kVtable_5030 = (void*)0x82005030;  // lbl_82005030
static void* const kVtable_502C = (void*)0x8200502C;  // lbl_8200502C
static void* const kVtable_3DC8 = (void*)0x82003DC8;  // lbl_82003DC8

// Global data
extern uint32_t g_msgSinkBitfield;  // @ 0x825B0C30 (.data)

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 1: Tail-call thunks with base-pointer adjustment (addi + branch)
 * Pattern: adjust 'this' pointer, then tail-call another function.
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_vfn_23_6130_1 @ 0x82456130 | size: 0x8
// Adjusts this by -8 and tail-calls msgMsgSink_6220_w
void msgMsgSink_vfn_23_6130_1(void* self) {
    msgMsgSink_6220_w((uint8_t*)self - 8);
}

// msgMsgSink_vfn_42_EFE8_1 @ 0x8244EFE8 | size: 0x8
// Adjusts this by +60 and tail-calls rage_C1A8
void msgMsgSink_vfn_42_EFE8_1(void* self) {
    rage_C1A8((uint8_t*)self + 60);
}

// msgMsgSink_3258_wrh @ 0x82353258 | size: 0xC
// Loads field at +12, passes it with zero to grc_2CC8
void msgMsgSink_3258_wrh(void* self) {
    uint32_t val = *(uint32_t*)((uint8_t*)self + 12);
    grc_2CC8((void*)(uintptr_t)val, 0, 0);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 2: Simple field getters (return value from this+offset)
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_1D20_g @ 0x82451D20 | size: 0xC
// Returns bits 1-3 of m_subVtable: (flags >> 1) & 7
uint32_t msgMsgSink_1D20_g(void* self) {
    uint32_t flags = *(uint32_t*)((uint8_t*)self + 4);
    return (flags >> 1) & 0x7;
}

// msgMsgSink_vfn_20_59E0_1 @ 0x824559E0 | size: 0xC
// Returns pointer to offset +12 within the object at +28
void* msgMsgSink_vfn_20_59E0_1(void* self) {
    void* obj = *(void**)((uint8_t*)self + 28);
    return (uint8_t*)obj + 12;
}

// msgMsgSink_vfn_21_5988_1 @ 0x82455988 | size: 0xC
// Returns uint16 at offset +12 within the object at +28
uint16_t msgMsgSink_vfn_21_5988_1(void* self) {
    void* obj = *(void**)((uint8_t*)self + 28);
    return *(uint16_t*)((uint8_t*)obj + 12);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 3: Vtable setters (write a vtable pointer to this+0)
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_3A38_g @ 0x82453A38 | size: 0x10
// Sets vtable at +0 to kVtable_4A94 (0x82004A94)
void msgMsgSink_3A38_g(void* self) {
    *(void**)self = kVtable_4A94;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 4: Store-and-forward stubs (set field, then tail-call)
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_6080_p33 @ 0x82456080 | size: 0x10
// Sets field at +252 to 1, then tail-calls atSingleton_5CD0_fw with param=14
void msgMsgSink_6080_p33(void* self) {
    *(uint32_t*)((uint8_t*)self + 252) = 1;
    atSingleton_5CD0_fw(self, 14);
}

// msgMsgSink_DA10_p46 @ 0x8256DA10 | size: 0x10
// Loads pointer at +0, then zeroes field at +1092 of that pointer
void msgMsgSink_DA10_p46(void* self) {
    void* ptr = *(void**)self;
    *(uint32_t*)((uint8_t*)ptr + 1092) = 0;
}

// msgMsgSink_vfn_43 @ 0x8244E138 | size: 0x10
// Stores r4 at offset +264 of self
// (already lifted — skipped in unlifted list; included for reference only)

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 5: Null-guard + tail-call through field_0x007C (+124)
 * Pattern: load ptr from +124, if null return, else tail-call target
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_8B78_g @ 0x82448B78 | size: 0x14
// Null-guard +124, tail-calls msgMsgSink_D168_sp
void msgMsgSink_8B78_g(void* self) {
    void* obj = *(void**)((uint8_t*)self + 124);
    if (obj == nullptr) return;
    msgMsgSink_D168_sp(obj);
}

// msgMsgSink_8B90_g @ 0x82448B90 | size: 0x14
// Null-guard +124, tail-calls msgMsgSink_D1D8_sp
void msgMsgSink_8B90_g(void* self) {
    void* obj = *(void**)((uint8_t*)self + 124);
    if (obj == nullptr) return;
    msgMsgSink_D1D8_sp(obj);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 6: Null-guard + tail-call through field_0x007C, returns 0 on null
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_8A60_sp @ 0x82448A60 | size: 0x18
// Null-guard +124, tail-calls RtlEnterCriticalSection_D6F0_fw; returns 0 if null
void* msgMsgSink_8A60_sp(void* self) {
    void* obj = *(void**)((uint8_t*)self + 124);
    if (obj != nullptr) {
        return RtlEnterCriticalSection_D6F0_fw(obj);
    }
    return nullptr;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 7: Virtual dispatch thunks through field_0x0024 (+36)
 * Pattern: load nested obj from +36, null-guard, call vtable[N]
 * These forward calls to the nested object's vtable.
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_DispatchNested @ 0x82458DA0 | size: 0x20
// Forwards to nested->vtable[17] (offset 68 in vtable)
void msgMsgSink_DispatchNested(void* self) {
    void* nested = *(void**)((uint8_t*)self + 36);
    if (nested == nullptr) return;
    typedef void (*Fn)(void*);
    void** vt = *(void***)nested;
    ((Fn)vt[17])(nested);
}

// msgMsgSink_8E00_sp @ 0x82458E00 | size: 0x20
// Forwards to nested->vtable[30] (offset 120 in vtable)
void msgMsgSink_8E00_sp(void* self) {
    void* nested = *(void**)((uint8_t*)self + 36);
    if (nested == nullptr) return;
    typedef void (*Fn)(void*);
    void** vt = *(void***)nested;
    ((Fn)vt[30])(nested);
}

// msgMsgSink_8E20_sp @ 0x82458E20 | size: 0x20
// Forwards to nested->vtable[19] (offset 76 in vtable)
void msgMsgSink_8E20_sp(void* self) {
    void* nested = *(void**)((uint8_t*)self + 36);
    if (nested == nullptr) return;
    typedef void (*Fn)(void*);
    void** vt = *(void***)nested;
    ((Fn)vt[19])(nested);
}

// msgMsgSink_8E40_sp @ 0x82458E40 | size: 0x20
// Forwards to nested->vtable[20] (offset 80 in vtable)
void msgMsgSink_8E40_sp(void* self) {
    void* nested = *(void**)((uint8_t*)self + 36);
    if (nested == nullptr) return;
    typedef void (*Fn)(void*);
    void** vt = *(void***)nested;
    ((Fn)vt[20])(nested);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 8: Virtual dispatch thunks through +36, return 0 if null
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_8E60_sp @ 0x82458E60 | size: 0x24
// Forwards to nested->vtable[11] (offset 44), returns 0 if nested is null
uint32_t msgMsgSink_8E60_sp(void* self) {
    void* nested = *(void**)((uint8_t*)self + 36);
    if (nested != nullptr) {
        typedef uint32_t (*Fn)(void*);
        void** vt = *(void***)nested;
        return ((Fn)vt[11])(nested);
    }
    return 0;
}

// msgMsgSink_8E88_sp @ 0x82458E88 | size: 0x24
// Forwards to nested->vtable[21] (offset 84), returns 0 if nested is null
uint32_t msgMsgSink_8E88_sp(void* self) {
    void* nested = *(void**)((uint8_t*)self + 36);
    if (nested != nullptr) {
        typedef uint32_t (*Fn)(void*);
        void** vt = *(void***)nested;
        return ((Fn)vt[21])(nested);
    }
    return 0;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 9: Adjusted-this virtual dispatch
 * Pattern: adjust this, load vtable, call through slot
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_vfn_39_EF08_1 @ 0x8244EF08 | size: 0x18
// Adjusts this by -24, calls vtable[1] with r4=1 on adjusted object
void msgMsgSink_vfn_39_EF08_1(void* self) {
    void* adjusted = (uint8_t*)self - 24;
    typedef void (*Fn)(void*, uint32_t);
    void** vt = *(void***)adjusted;
    ((Fn)vt[1])(adjusted, 1);
}

// msgMsgSink_vfn_60 @ 0x824564C0 | size: 0x18
// (already lifted — included in existing vtable entries)

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 10: Float-compare-and-set with dirty flag
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_vfn_38_6690_1 @ 0x82456690 | size: 0x1C
// Compares float at +16 with f1 param; if different, stores new value and
// sets dirty flag at +20
void msgMsgSink_vfn_38_6690_1(void* self, float newVal) {
    float* pFloat = (float*)((uint8_t*)self + 16);
    if (*pFloat == newVal) return;
    *pFloat = newVal;
    *(uint32_t*)((uint8_t*)self + 20) = 1;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 11: Memory-clear (byte loop)
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_1B98_g @ 0x82451B98 | size: 0x20
// Zeroes 40 bytes starting at self (byte-by-byte clear)
void msgMsgSink_1B98_g(void* self) {
    uint8_t* p = (uint8_t*)self;
    for (int i = 0; i < 40; i++) {
        p[i] = 0;
    }
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 12: Bitfield read-modify-write on global
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_32A0_h @ 0x824332A0 | size: 0x20
// Atomic-style masked merge: old = g_msgSinkBitfield;
//   g_msgSinkBitfield = (old & ~mask) | (value & mask); returns old
uint32_t msgMsgSink_32A0_h(uint32_t value, uint32_t mask) {
    uint32_t old = g_msgSinkBitfield;
    uint32_t merged = (old & ~mask) | (value & mask);
    g_msgSinkBitfield = merged;
    return old;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 13: Field copy / struct write
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_EF60_wrh @ 0x8235EF60 | size: 0x24
// Copies 4 fields from this (at various offsets) into output struct
void msgMsgSink_EF60_wrh(void* self, void* unused, uint32_t* out) {
    out[0] = *(uint32_t*)((uint8_t*)self + 13736);
    out[1] = *(uint32_t*)((uint8_t*)self + 13740);
    out[2] = *(uint32_t*)((uint8_t*)self + 13784);
    out[3] = *(uint32_t*)((uint8_t*)self + 13744);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 14: Flag-based return value
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_1BE0_g @ 0x82451BE0 | size: 0x28
// Returns: 0 if bit0 of flags is 0, 1 if bit0 is 1, 2 otherwise (never hit)
uint32_t msgMsgSink_1BE0_g(void* self) {
    uint32_t flags = *(uint32_t*)((uint8_t*)self + 4);
    uint32_t bit0 = flags & 0x1;
    if (bit0 == 1) return 1;
    if (bit0 == 0) return 0;
    return 2;
}

// msgMsgSink_1C08_g @ 0x82451C08 | size: 0x38
// Compares bit0 of flags against constant 2; returns 1 if field < threshold
uint32_t msgMsgSink_1C08_g(void* self) {
    uint32_t flags = *(uint32_t*)((uint8_t*)self + 4);
    uint32_t bit0 = flags & 0x1;
    uint32_t cmp;
    if (bit0 >= 1) {
        if (bit0 == 1) {
            cmp = 1;
        } else {
            // bit0 > 1 (impossible for & 0x1, but matches scaffold)
            return (2 >= 1) ? 0 : 1;
        }
    } else {
        cmp = 0;
    }
    return (cmp >= 1) ? 0 : 1;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 15: Multi-vtable init + tail-call
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_7DA8_p33 @ 0x82457DA8 | size: 0x28
// Sets 3 vtable pointers at +0, +4, +8, then tail-calls msgMsgSink_6138_2hr
void msgMsgSink_7DA8_p33(void* self) {
    *(void**)((uint8_t*)self + 0) = kVtable_4FF0;
    *(void**)((uint8_t*)self + 4) = kVtable_4FE8;
    *(void**)((uint8_t*)self + 8) = kVtable_4FE4;
    msgMsgSink_6138_2hr(self);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 16: Multi-field write + message setup
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_B008_w @ 0x8244B008 | size: 0x28
// Stores several parameters into message fields; converts int bits to float
void msgMsgSink_B008_w(void* self, uint32_t p1, uint32_t p2, uint16_t p3,
                       void* p4, uint32_t p5) {
    *(uint32_t*)((uint8_t*)self + 12) = p1;
    *(uint32_t*)((uint8_t*)self + 16) = (uint32_t)(uintptr_t)p4;
    *(uint32_t*)((uint8_t*)self + 240) = p5;
    *(uint16_t*)((uint8_t*)self + 244) = p3;
    *(uint32_t*)((uint8_t*)self + 248) = p2;
    // Load float from p4+1 (unaligned) and store at +20
    uint32_t rawBits = *(uint32_t*)((uint8_t*)p4 + 1);
    float fval;
    memcpy(&fval, &rawBits, sizeof(float));
    *(float*)((uint8_t*)self + 20) = fval;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 17: Decrement-and-forward (counter management)
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_03B8_g @ 0x824503B8 | size: 0x2C
// Decrements indexed counter at +248, then tail-calls atSingleton_8E88_p42
void msgMsgSink_03B8_g(void* self, uint32_t index) {
    uint32_t* counterArray = *(uint32_t**)((uint8_t*)self + 248);
    uint32_t offset = (index & 0x3FFF) << 2;  // rlwinm r11,r5,2,14,29
    uint32_t* pCounter = (uint32_t*)((uint8_t*)counterArray + offset);
    (*pCounter)--;
    void* handler = *(void**)((uint8_t*)self + 24);
    uint32_t newCount = *pCounter;
    atSingleton_8E88_p42(handler, self, index, newCount, 0);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 18: Linked-list search + remove
 * Pattern: walk linked list from head, find node by key, remove via ph_EF40
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_vfn_79 @ 0x824513D0 | size: 0x3C
// Walks list at +224, finds node where [+0] == key, removes via ph_EF40(+20)
void msgMsgSink_vfn_79(void* self, void* key) {
    uint32_t* node = *(uint32_t**)((uint8_t*)self + 224);
    if (node == nullptr) return;
    while (node != nullptr) {
        void* nodeKey = *(void**)node;
        if (nodeKey == key) {
            if (node == nullptr) return;
            ph_EF40((uint8_t*)self + 20, node);
            return;
        }
        node = *(uint32_t**)((uint8_t*)node + 4);
    }
}

// msgMsgSink_vfn_125 @ 0x824505D0 | size: 0x3C
// Walks list at +228, finds node where [+0] == key, removes via ph_EF40(+24)
void msgMsgSink_vfn_125(void* self, void* key) {
    uint32_t* node = *(uint32_t**)((uint8_t*)self + 228);
    if (node == nullptr) return;
    while (node != nullptr) {
        void* nodeKey = *(void**)node;
        if (nodeKey == key) {
            if (node == nullptr) return;
            ph_EF40((uint8_t*)self + 24, node);
            return;
        }
        node = *(uint32_t**)((uint8_t*)node + 4);
    }
}

// msgMsgSink_vfn_43_EFA8_1 @ 0x8244EFA8 | size: 0x3C
// Walks list at +264, finds node where [+0] == key, removes via ph_EF40(+60)
void msgMsgSink_vfn_43_EFA8_1(void* self, void* key) {
    uint32_t* node = *(uint32_t**)((uint8_t*)self + 264);
    if (node == nullptr) return;
    while (node != nullptr) {
        void* nodeKey = *(void**)node;
        if (nodeKey == key) {
            if (node == nullptr) return;
            ph_EF40((uint8_t*)self + 60, node);
            return;
        }
        node = *(uint32_t**)((uint8_t*)node + 4);
    }
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 19: Multi-vtable init with extra field + tail-call
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_7FB8_p33 @ 0x82457FB8 | size: 0x34
// Sets 3 vtable pointers at +0/+4/+8 and extra vtable at +292,
// then tail-calls msgMsgSink_6138_2hr
void msgMsgSink_7FB8_p33(void* self) {
    *(void**)((uint8_t*)self + 0)   = kVtable_5030;
    *(void**)((uint8_t*)self + 4)   = kVtable_4FE8;
    *(void**)((uint8_t*)self + 8)   = kVtable_502C;
    *(void**)((uint8_t*)self + 292) = kVtable_3DC8;
    msgMsgSink_6138_2hr(self);
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 20: Linked-list search by uint16 key
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_E758_p33 @ 0x8244E758 | size: 0x34
// Walks linked list from +316, sentinel at +312.
// Returns node where uint16 at node+244 matches param; else nullptr.
void* msgMsgSink_E758_p33(void* self, uint32_t param) {
    void* sentinel = (uint8_t*)self + 312;
    void* node = *(void**)((uint8_t*)self + 316);
    uint16_t key = (uint16_t)(param & 0xFFFF);
    while (node != sentinel) {
        uint16_t nodeKey = *(uint16_t*)((uint8_t*)node + 244);
        if (nodeKey == key) return node;
        node = *(void**)((uint8_t*)node + 4);
    }
    return nullptr;
}

/* ───────────────────────────────────────────────────────────────────────────
 * GROUP 21: Linked-list iteration with callback
 * Pattern: walk list from +40, sentinel at +36, call handler per node
 * ─────────────────────────────────────────────────────────────────────────── */

// msgMsgSink_3E68_g @ 0x82453E68 | size: 0x40
// Iterates list from +40, sentinel +36, calls game_9CF8_h on each node
void msgMsgSink_3E68_g(void* self, uint32_t param) {
    void* sentinel = (uint8_t*)self + 36;
    void* node = *(void**)((uint8_t*)self + 40);
    while (node != sentinel) {
        game_9CF8_h(node, param);
        node = *(void**)((uint8_t*)node + 4);
    }
}

// msgMsgSink_3EA8_g @ 0x82453EA8 | size: 0x40
// Iterates list from +40, sentinel +36, calls game_9D10_h on each node
void msgMsgSink_3EA8_g(void* self, uint32_t param) {
    void* sentinel = (uint8_t*)self + 36;
    void* node = *(void**)((uint8_t*)self + 40);
    while (node != sentinel) {
        game_9D10_h(node, param);
        node = *(void**)((uint8_t*)node + 4);
    }
}

// msgMsgSink_3EE8_g @ 0x82453EE8 | size: 0x40
// Iterates list from +40, sentinel +36, calls game_9D28_h on each node
void msgMsgSink_3EE8_g(void* self, uint32_t param) {
    void* sentinel = (uint8_t*)self + 36;
    void* node = *(void**)((uint8_t*)self + 40);
    while (node != sentinel) {
        game_9D28_h(node, param);
        node = *(void**)((uint8_t*)node + 4);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GenerateWithLock() [vtable slot 25 @ 0x8244DCF0]
// Enters critical section, calls the generate handler, destroys the sub-object
// at +12 on success, then leaves the critical section. Returns the generate result.
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::GenerateWithLock() {
    // Get critical section from session object at offset +56
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    RtlEnterCriticalSection(criticalSection);

    // Call the generate handler on the session object
    extern int32_t msgMsgSink_84C0_gen(void*);
    int32_t result = msgMsgSink_84C0_gen(sessionObj);

    if (result >= 0) {
        // Destroy the sub-object at offset +12 (message handler interface)
        void* subObj = (uint8_t*)this + 12;
        typedef void (*DtorFn)(void*, int);
        void** subVtable = *(void***)subObj;
        DtorFn dtor = (DtorFn)subVtable[0];
        dtor(subObj, 1);
    }

    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetStateFlagsLocked() [vtable slot 24 @ 0x8244DFF8]
// Thread-safe wrapper that enters critical section, calls vtable slot 16
// (GetStateFlags) to read current state, writes the result to the output
// parameter, then returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::GetStateFlagsLocked(uint32_t* outFlags) {
    // Get critical section from session object at offset +56
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    RtlEnterCriticalSection(criticalSection);

    // Call vtable slot 16 to get state flags
    typedef uint32_t (*GetStateFlagsFn)(void*);
    void** vt = *(void***)this;
    GetStateFlagsFn getFlags = (GetStateFlagsFn)vt[16];
    uint32_t stateFlags = getFlags(this);

    *outFlags = stateFlags;

    RtlLeaveCriticalSection(criticalSection);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ProcessPendingMessages() [vtable slot 39 @ 0x8244E050]
// Checks state flags for pending messages (bits 3-4), processes them if
// the queue has data, then flushes the message object at offset +52.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ProcessPendingMessages() {
    // Call vtable slot 16 to get state flags
    typedef uint32_t (*GetStateFlagsFn)(void*);
    void** vt = *(void***)this;
    GetStateFlagsFn getFlags = (GetStateFlagsFn)vt[16];
    uint32_t stateFlags = getFlags(this);

    // Check bits 3-4 (mask 0x18) for pending message indicators
    if (stateFlags & 0x18) {
        // Check if message queue at offset +24 has data
        extern int32_t msgMsgSink_3B48_g(void*);
        void* msgQueue = (uint8_t*)this + 24;
        int32_t hasData = msgMsgSink_3B48_g(msgQueue);

        if (hasData != 0) {
            // Process pending messages from the message object
            void* msgObj = *(void**)((uint8_t*)this + 52);
            extern void msgMsgSink_3DC8_g(void*);
            msgMsgSink_3DC8_g(msgObj);
        }
    }

    // Flush the message object if present
    void* msgObj = *(void**)((uint8_t*)this + 52);
    if (msgObj != nullptr) {
        extern void msgMsgSink_5148_g(void*);
        msgMsgSink_5148_g(msgObj);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::CleanupSession() [vtable slot 40 @ 0x8244E0C0]
// Cleans up the current session. If the message object at +52 exists, calls
// the cleanup handler. If field +324 (pending flag) is set, checks state;
// if state is idle (32), destroys the sub-object at +12.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::CleanupSession() {
    // Clean up message object if present
    void* msgObj = *(void**)((uint8_t*)this + 52);
    if (msgObj != nullptr) {
        extern void msgMsgSink_4550_g(void*);
        msgMsgSink_4550_g(msgObj);
    }

    // Check pending cleanup flag at offset +324
    int32_t pendingCleanup = *(int32_t*)((uint8_t*)this + 324);
    if (pendingCleanup != 0) {
        // Get state via vtable slot 16
        typedef uint32_t (*GetStateFlagsFn)(void*);
        void** vt = *(void***)this;
        GetStateFlagsFn getFlags = (GetStateFlagsFn)vt[16];
        uint32_t stateFlags = getFlags(this);

        // State 32 = idle/default, meaning cleanup can proceed
        if (stateFlags == 32) {
            // Destroy the sub-object at offset +12
            void* subObj = (uint8_t*)this + 12;
            typedef void (*DtorFn)(void*, int);
            void** subVtable = *(void***)subObj;
            DtorFn dtor = (DtorFn)subVtable[0];
            dtor(subObj, 1);
            return;
        }

        // State not idle yet, return without destroying
        return;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetConnectionInfo() [vtable slot 26 @ 0x8244E160]
// Thread-safe accessor that copies connection info from the sink. Reads the
// connection flags byte at +296, computes the data size as (flags << 3) + 8,
// copies the data from +300 into the output buffer, and optionally writes
// the total size.
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::GetConnectionInfo(void* bufferOut, uint32_t sizeParam, uint32_t* totalSizeOut) {
    // Get critical section from session object at offset +56
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    RtlEnterCriticalSection(criticalSection);

    // Read connection flags byte at offset +296
    uint8_t connectionFlags = *(uint8_t*)((uint8_t*)this + 296);

    // Compute data size: (flags << 3) + 8
    uint32_t dataSize = (connectionFlags << 3) + 8;

    // Copy connection data if output buffer and size parameter are valid
    if (sizeParam != 0 && bufferOut != nullptr) {
        // Store flags byte at start of output buffer
        *(uint8_t*)bufferOut = connectionFlags;

        // Copy connection data from offset +300 into buffer at +4
        void* srcData = (uint8_t*)this + 300;
        void* dstData = (uint8_t*)bufferOut + 4;
        memcpy(dstData, srcData, dataSize);
    }

    // Write total size if output pointer is valid
    if (totalSizeOut != nullptr) {
        *totalSizeOut = dataSize;
    }

    RtlLeaveCriticalSection(criticalSection);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::FlushAndDisconnect() [vtable slot 49 @ 0x8244E628]
// If there is a pending disconnect request (field +308 is set), clears the
// flag and dispatches the pending disconnect event. Then transitions the
// state machine to state 3 (disconnecting).
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::FlushAndDisconnect() {
    // Check if there is a pending disconnect request
    int32_t pendingRequest = *(int32_t*)((uint8_t*)this + 308);
    if (pendingRequest != 0) {
        // Clear the pending request flag
        *(int32_t*)((uint8_t*)this + 308) = 0;

        // Dispatch the pending disconnect event
        uint16_t channelId = *(uint16_t*)((uint8_t*)this + 292);
        uint32_t connectionHandle = *(uint32_t*)((uint8_t*)this + 284);
        extern void msgMsgSink_03B8_g(void*, uint16_t);
        msgMsgSink_03B8_g((void*)connectionHandle, channelId);
    }

    // Transition to state 3 (disconnecting)
    extern void msgMsgSink_DC40_g(void*, int32_t);
    msgMsgSink_DC40_g(this, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::BeginDisconnect() [vtable slot 50 @ 0x8244E680]
// Initiates a disconnect if the channel ID at +292 is valid (not 0xFFFF).
// Sets the pending-request flag at +308, sends the disconnect notification,
// and transitions the state machine to state 2 (disconnect pending).
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::BeginDisconnect() {
    // Check if channel ID is valid
    uint16_t channelId = *(uint16_t*)((uint8_t*)this + 292);
    if (channelId == 0xFFFF) {
        return;  // No active channel, nothing to disconnect
    }

    // Set pending-request flag
    *(int32_t*)((uint8_t*)this + 308) = 1;

    // Send disconnect notification
    uint32_t connectionHandle = *(uint32_t*)((uint8_t*)this + 284);
    extern void game_0388_h(void*, uint16_t);
    game_0388_h((void*)connectionHandle, channelId);

    // Transition to state 2 (disconnect pending)
    extern void msgMsgSink_DC40_g(void*, int32_t);
    msgMsgSink_DC40_g(this, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::FindValidMessageSlot() [vtable slot 30 @ 0x8244E6D0]
// Thread-safe lookup for a valid message slot. Enters critical section, calls
// the slot finder on the session object, validates the result, and checks
// that the slot has bit 0 set and bit 2 clear before returning it.
// Returns -1 if no valid slot is found.
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::FindValidMessageSlot(uint32_t searchKey) {
    // Get critical section from session object at offset +56
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    RtlEnterCriticalSection(criticalSection);

    // Search for a message slot matching the key
    extern uint32_t game_8CF8_h(void*, uint32_t);
    uint32_t slotId = game_8CF8_h(sessionObj, searchKey);

    // Validate the slot ID (lower 16 bits must not be 0xFFFF)
    uint16_t slotIndex = (uint16_t)(slotId & 0xFFFF);
    if (slotIndex != 0xFFFF) {
        // Get the slot data
        extern void* msgMsgSink_8D10_g(void*, uint32_t);
        void* slotData = msgMsgSink_8D10_g(sessionObj, slotId);

        if (slotData != nullptr) {
            uint8_t slotFlags = *(uint8_t*)slotData;

            // Check bit 0 is set (slot is active)
            if ((slotFlags & 0x01) != 0) {
                // Check bit 2 is NOT set (slot is not locked)
                if ((slotFlags & 0x04) != 0) {
                    // Slot is locked, return the ID anyway (special case)
                    RtlLeaveCriticalSection(criticalSection);
                    return (int32_t)slotId;
                }
            }
        }

        // Slot invalid or inactive, return -1
        slotId = (uint32_t)-1;
    }

    RtlLeaveCriticalSection(criticalSection);
    return (int32_t)slotId;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::RegisterMessageHandler() [vtable slot 129 @ 0x8244FE80]
// Thread-safe registration of a message handler. Enters critical section,
// extracts the registration mode from bit 0 of the flags parameter, then
// calls the internal registration function with the handler and mode.
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::RegisterMessageHandler(void* handler, uint32_t flags) {
    // Get critical section from the network object at offset +24
    void* networkObj = *(void**)((uint8_t*)this + 24);
    void* criticalSection = (uint8_t*)networkObj + 144;

    RtlEnterCriticalSection(criticalSection);

    // Extract registration mode from bit 0 of flags
    uint32_t registerMode = (flags & 0x1) ? 1 : 0;

    // Call internal registration function
    extern int32_t RtlEnterCriticalSection_A158_h(void*, void*, void*, uint32_t, int32_t);
    void* netObj = *(void**)((uint8_t*)this + 24);
    int32_t result = RtlEnterCriticalSection_A158_h(netObj, this, handler, registerMode, 0);

    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::QueryConnectionState() [vtable slot 131 @ 0x8244FEE8]
// Thread-safe query of the connection state. Enters critical section, reads
// the connection pointer at +240, writes whether a connection exists (1/0)
// to the output parameter, then returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t msgMsgSink::QueryConnectionState(uint32_t* outConnected) {
    // Get critical section from the network object at offset +24
    void* networkObj = *(void**)((uint8_t*)this + 24);
    void* criticalSection = (uint8_t*)networkObj + 144;

    RtlEnterCriticalSection(criticalSection);

    // Check if connection pointer at +240 is non-null
    uint32_t connectionPtr = *(uint32_t*)((uint8_t*)this + 240);
    bool isConnected = (connectionPtr != 0);

    *outConnected = isConnected ? 1 : 0;

    RtlLeaveCriticalSection(criticalSection);
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * msgMsgSink vtable methods & thunks (batch 4: 8-88 bytes)
 * 10 functions: field getters, this-adjustment thunks, event dispatchers
 * ═══════════════════════════════════════════════════════════════════════════ */

// Forward declarations for tail-call targets
extern void msgMsgSink_03E8_w(void*);
extern void msgMsgSink_0480_w(void*);
extern void msgMsgSink_07D8_w(void*);
extern void msgMsgSink_11B0_w(void*);

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetSessionPointer() [vtable slot 54 @ 0x8244EF38 | 8 bytes]
// Returns the session pointer stored at offset +324 (0x144).
// ─────────────────────────────────────────────────────────────────────────────
void* msgMsgSink::GetSessionPointer() {
    return *(void**)((uint8_t*)this + 324);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ForwardProcessMessage() [vtable slot 55 @ 0x8244F8A8 | 8 bytes]
// Adjusts this by -12 to the base sub-object, tail-calls msgMsgSink_03E8_w.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ForwardProcessMessage() {
    msgMsgSink_03E8_w((uint8_t*)this - 12);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ForwardToRageHandler() [vtable slot 78 @ 0x824514E8 | 8 bytes]
// Adjusts this by +20 to the embedded rage sub-object, tail-calls rage_C1A8.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ForwardToRageHandler() {
    rage_C1A8((uint8_t*)this + 20);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ForwardDispatchMessage() [vtable slot 89 @ 0x8244F8B0 | 8 bytes]
// Adjusts this by -12 to the base sub-object, tail-calls msgMsgSink_0480_w.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ForwardDispatchMessage() {
    msgMsgSink_0480_w((uint8_t*)this - 12);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetNameLength() [vtable slot 115 @ 0x8244F7B8 | 12 bytes]
// Loads the value at offset +48 (0x30) and returns it plus 9, computing a
// string length or offset past a fixed-size header.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::GetNameLength() {
    uint32_t baseValue = *(uint32_t*)((uint8_t*)this + 48);
    return baseValue + 9;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::SendPulseToConnection() [vtable slot 119 @ 0x8244F768 | 12 bytes]
// Loads the network connection from offset +56 (0x38), then forwards to
// msgMsgSink_8A60_sp (SendPulse) with that connection as the argument.
// ─────────────────────────────────────────────────────────────────────────────
void* msgMsgSink::SendPulseToConnection() {
    void* connection = *(void**)((uint8_t*)this + 56);
    return msgMsgSink_8A60_sp(connection);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ForwardCleanupMessage() [vtable slot 147 @ 0x824505C8 | 8 bytes]
// Adjusts this by -12 to the base sub-object, tail-calls msgMsgSink_07D8_w.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ForwardCleanupMessage() {
    msgMsgSink_07D8_w((uint8_t*)this - 12);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ForwardFinalizeMessage() [vtable slot 148 @ 0x824508A8 | 8 bytes]
// Adjusts this by -12 to the base sub-object, tail-calls msgMsgSink_11B0_w.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ForwardFinalizeMessage() {
    msgMsgSink_11B0_w((uint8_t*)this - 12);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::DispatchEventWithSessionInfo() [vtable slot 120 @ 0x824564A0 | 28 bytes]
// Loads session data from offset +48 (0x30), extracts the reliability mode
// (lower 2 bits of byte at +14) and the channel ID (uint16 at +10), then
// dispatches the event via jumptable_3A48 with type=2 on sub-object at +24.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::DispatchEventWithSessionInfo() {
    void* sessionObj = *(void**)((uint8_t*)this + 48);
    uint8_t flags = *(uint8_t*)((uint8_t*)sessionObj + 14);
    uint32_t reliabilityMode = flags & 0x3;
    uint16_t channelId = *(uint16_t*)((uint8_t*)sessionObj + 10);

    void* target = (uint8_t*)this + 24;
    extern void jumptable_3A48(void*, uint32_t, uint32_t, uint32_t);
    jumptable_3A48(target, 2, channelId, reliabilityMode);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::SetActiveAndReleaseObject() @ 0x8244A670 | 88 bytes
// Sets the active flag (bit 4) at offset +24, then if the object pointer
// at +40 is non-null, calls vtable[7] (Release) on it and clears the pointer.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::SetActiveAndReleaseObject() {
    // Set bit 4 (0x10) of the flag byte at offset +24
    uint8_t flags = *(uint8_t*)((uint8_t*)this + 24);
    flags |= 0x10;
    *(uint8_t*)((uint8_t*)this + 24) = flags;

    // Check if object pointer at +40 is valid
    void* obj = *(void**)((uint8_t*)this + 40);
    if (obj != nullptr) {
        // Call vtable slot 7 (Release) on the object
        typedef void (*ReleaseFn)(void*);
        void** vt = *(void***)obj;
        ((ReleaseFn)vt[7])(obj);

        // Clear the object pointer
        *(void**)((uint8_t*)this + 40) = nullptr;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * msgMsgSink vtable methods — batch 5 (message dispatch & event sink)
 *
 * 10 functions, all <=200 bytes.  Vtable slots from the msgMsgSink class
 * covering disconnect/reconnect handling, refcount management,
 * thread-safe session operations, and event dispatch helpers.
 * ═══════════════════════════════════════════════════════════════════════════ */

// External helpers used by batch 5
extern void msgMsgSink_DC40_g(void*, uint32_t);
extern void game_0388_h(void*, uint16_t);
extern uint32_t game_8CF8_h(void*, uint16_t);
extern void* msgMsgSink_8D10_g(void*, uint16_t);
extern void jumptable_3A48(void*, uint32_t, uint16_t, uint32_t);
extern uint32_t msgMsgSink_3D70_p39(void*);
extern void atSingleton_B0E8_sp(void*, uint32_t, uint32_t);
extern uint32_t RtlEnterCriticalSection_A158_h(void*, void*, uint32_t, uint32_t, uint32_t);

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::OnDisconnect() [vfn_49 @ 0x8244E628 | 84B]
//
// Handles a disconnect event.  If a pending message flag (+308) is set,
// clears it and cancels the pending message via msgMsgSink_03B8_g.
// Then dispatches a state-change notification with code 3 (disconnected).
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::OnDisconnect() {
    uint32_t pendingFlag = *(uint32_t*)((uint8_t*)this + 308);

    if (pendingFlag != 0) {
        uint16_t gamerIndex = *(uint16_t*)((uint8_t*)this + 292);
        void* sessionObj = *(void**)((uint8_t*)this + 284);

        *(uint32_t*)((uint8_t*)this + 308) = 0;

        extern void msgMsgSink_03B8_g(void*, uint16_t);
        msgMsgSink_03B8_g(sessionObj, gamerIndex);
    }

    msgMsgSink_DC40_g(this, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::OnReconnect() [vfn_50 @ 0x8244E680 | 80B]
//
// Handles a reconnect event.  If the gamer index (+292) is valid
// (not 0xFFFF), sets the pending flag, looks up the gamer, and
// dispatches a state-change notification with code 2 (reconnected).
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::OnReconnect() {
    uint16_t gamerIndex = *(uint16_t*)((uint8_t*)this + 292);

    if (gamerIndex != 0xFFFF) {
        void* sessionObj = *(void**)((uint8_t*)this + 284);

        *(uint32_t*)((uint8_t*)this + 308) = 1;

        game_0388_h(sessionObj, gamerIndex);

        msgMsgSink_DC40_g(this, 2);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ResetEventDispatch() [vfn_52 @ 0x8244DB58 | 20B]
//
// Dispatches a reset event through the internal event table.
// Adjusts 'this' to the embedded event-sink sub-object at +24, then
// tail-calls jumptable_3A48 with type=2 and zero payload.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ResetEventDispatch() {
    void* eventSink = (uint8_t*)this + 24;
    jumptable_3A48(eventSink, 2, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetConnectionCount() [vfn_46 @ 0x8244E148 | 24B]
//
// Returns the active connection count from the delegate object at +52.
// If the delegate pointer is null, returns 0.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::GetConnectionCount() {
    void* delegate = *(void**)((uint8_t*)this + 52);

    if (delegate == nullptr) {
        return 0;
    }

    return msgMsgSink_3D70_p39(delegate);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ForwardMessageToSession() [vfn_68 @ 0x82456F28 | 20B]
//
// Forwards a message to the session layer.  Loads the session ID from
// +288 and the session object from +284, then tail-calls
// atSingleton_B0E8_sp to deliver the message.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::ForwardMessageToSession(uint32_t param) {
    uint32_t sessionId = *(uint32_t*)((uint8_t*)this + 288);
    void* sessionObj = *(void**)((uint8_t*)this + 284);
    atSingleton_B0E8_sp(sessionObj, sessionId, param);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::Release() [vfn_118 @ 0x8246BC00 | 84B]
//
// Decrements the reference count at +4.  When it reaches zero, invokes
// the cleanup handler via vtable slot 3 (vfn_3) and returns 0.
// Otherwise returns the new refcount.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::Release() {
    uint32_t refCount = *(uint32_t*)((uint8_t*)this + 4);
    refCount--;
    *(uint32_t*)((uint8_t*)this + 4) = refCount;

    if (refCount == 0) {
        // Call virtual method at slot 3 (cleanup/destroy)
        typedef void (*VfnSlot3)(void*);
        void** vt = *(void***)this;
        VfnSlot3 cleanup = (VfnSlot3)vt[3];
        cleanup(this);
        return 0;
    }

    return refCount;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::NotifyStateChange() [vfn_120 @ 0x824564A0 | 28B]
//
// Extracts notification parameters from the nested state object at +48,
// then dispatches the event through jumptable_3A48 with type=2.
// Reads the payload ID from +10 and the flags (low 2 bits) from +14
// of the state object.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::NotifyStateChange() {
    void* stateObj = *(void**)((uint8_t*)this + 48);
    void* eventSink = (uint8_t*)this + 24;

    uint16_t payloadId = *(uint16_t*)((uint8_t*)stateObj + 10);
    uint8_t rawFlags = *(uint8_t*)((uint8_t*)stateObj + 14);
    uint32_t flags = rawFlags & 0x3;

    jumptable_3A48(eventSink, 2, payloadId, flags);
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::CreateSessionLocked() [vfn_129 @ 0x8244FE80 | 104B]
//
// Thread-safe session creation.  Enters the critical section on the
// network manager (+24, offset +144), delegates to the session creation
// helper (RtlEnterCriticalSection_A158_h), then leaves the critical section.
// The 'flags' parameter's low bit controls a boolean option.
// ─────────────────────────────────────────────────────────────────────────────
void* msgMsgSink::CreateSessionLocked(uint32_t sessionParam, uint32_t flags) {
    void* networkMgr = *(void**)((uint8_t*)this + 24);
    void* critSection = (uint8_t*)networkMgr + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);

    RtlEnterCriticalSection(critSection);

    uint32_t boolFlag = (flags & 0x1) ? 1 : 0;

    uint32_t result = RtlEnterCriticalSection_A158_h(
        *(void**)((uint8_t*)this + 24),  // network manager
        this,                             // this sink
        sessionParam,                     // session param
        boolFlag,                         // bool option
        0                                 // reserved
    );

    RtlLeaveCriticalSection(critSection);

    return (void*)(uintptr_t)result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::QueryConnectionStatus() [vfn_131 @ 0x8244FEE8 | 80B]
//
// Thread-safe query of connection status.  Enters the critical section
// on the network manager, checks whether the connection pointer at +240
// is non-null (connected = 1, disconnected = 0), writes the result to
// the output parameter, then leaves the critical section.  Always returns 0.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::QueryConnectionStatus(uint32_t* outStatus) {
    void* networkMgr = *(void**)((uint8_t*)this + 24);
    void* critSection = (uint8_t*)networkMgr + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);

    RtlEnterCriticalSection(critSection);

    uint32_t connPtr = *(uint32_t*)((uint8_t*)this + 240);
    // connPtr != 0  =>  connected (1),  connPtr == 0  =>  disconnected (0)
    uint32_t isConnected = (connPtr != 0) ? 1 : 0;
    *outStatus = isConnected;

    RtlLeaveCriticalSection(critSection);

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::FindGamerByIndex() [vfn_30 @ 0x8244E6D0 | 136B]
//
// Thread-safe gamer lookup by index.  Enters the critical section on the
// delegate object (+56, offset +144), looks up the gamer via game_8CF8_h,
// validates the result is not 0xFFFF, then checks the gamer's active flag
// (bit 0 set AND bit 2 set means valid).  Returns the gamer handle on
// success or -1 (0xFFFF) on failure.
// ─────────────────────────────────────────────────────────────────────────────
uint16_t msgMsgSink::FindGamerByIndex(uint16_t index) {
    void* delegateObj = *(void**)((uint8_t*)this + 56);
    void* critSection = (uint8_t*)delegateObj + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);

    RtlEnterCriticalSection(critSection);

    void* delegate = *(void**)((uint8_t*)this + 56);
    uint16_t gamerHandle = (uint16_t)game_8CF8_h(delegate, index);

    if ((gamerHandle & 0xFFFF) != 0xFFFF) {
        void* gamerObj = msgMsgSink_8D10_g(delegate, gamerHandle);

        if (gamerObj != nullptr) {
            uint8_t statusByte = *(uint8_t*)gamerObj;

            // Check bit 0 (active) is set
            if (statusByte & 0x1) {
                // Check bit 2 (confirmed) is set - if so, return immediately
                if (statusByte & 0x4) {
                    RtlLeaveCriticalSection(critSection);
                    return gamerHandle;
                }
            }
        }

        gamerHandle = (uint16_t)0xFFFF;
    }

    RtlLeaveCriticalSection(critSection);
    return gamerHandle;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * msgMsgSink vtable dispatch methods (8-80 bytes)
 * 10 functions: forwarding thunks, field accessors, thread-safe queries
 * ═══════════════════════════════════════════════════════════════════════════ */

// Forward declarations for tail-call targets
extern void msgMsgSink_03E8_w(void*);
extern void msgMsgSink_0480_w(void*);
extern void msgMsgSink_07D8_w(void*);
extern void msgMsgSink_11B0_w(void*);

/**
 * msgMsgSink::DispatchDefaultHandler() [vtable slot 59 @ 0x821EFD10]
 * size: 0x10 (16 bytes)
 *
 * Loads the vtable from this object and tail-calls vtable[0] (the
 * destructor / default handler slot) on self. This is a simple
 * virtual dispatch thunk that re-enters through the base vtable.
 */
void msgMsgSink::DispatchDefaultHandler() {
    void** vt = *(void***)this;
    typedef void (*Fn)(void*);
    ((Fn)vt[0])(this);
}

/**
 * msgMsgSink::GetSessionData() [vtable slot 54 @ 0x8244EF38]
 * size: 0x8 (8 bytes)
 *
 * Returns the session data pointer stored at offset +324 (0x144).
 */
void* msgMsgSink::GetSessionData() {
    return *(void**)((uint8_t*)this + 324);
}

/**
 * msgMsgSink::ForwardToBaseWrite() [vtable slot 55 @ 0x8244F8A8]
 * size: 0x8 (8 bytes)
 *
 * Adjusts this pointer by -12 to reach the base sub-object,
 * then tail-calls msgMsgSink_03E8_w for write processing.
 */
void msgMsgSink::ForwardToBaseWrite() {
    msgMsgSink_03E8_w((uint8_t*)this - 12);
}

/**
 * msgMsgSink::ForwardToBaseUpdate() [vtable slot 89 @ 0x8244F8B0]
 * size: 0x8 (8 bytes)
 *
 * Adjusts this pointer by -12 to reach the base sub-object,
 * then tail-calls msgMsgSink_0480_w for update processing.
 */
void msgMsgSink::ForwardToBaseUpdate() {
    msgMsgSink_0480_w((uint8_t*)this - 12);
}

/**
 * msgMsgSink::ForwardToBaseNotify() [vtable slot 147 @ 0x824505C8]
 * size: 0x8 (8 bytes)
 *
 * Adjusts this pointer by -12 to reach the base sub-object,
 * then tail-calls msgMsgSink_07D8_w for notification handling.
 */
void msgMsgSink::ForwardToBaseNotify() {
    msgMsgSink_07D8_w((uint8_t*)this - 12);
}

/**
 * msgMsgSink::ForwardToBaseProcess() [vtable slot 148 @ 0x824508A8]
 * size: 0x8 (8 bytes)
 *
 * Adjusts this pointer by -12 to reach the base sub-object,
 * then tail-calls msgMsgSink_11B0_w for message processing.
 */
void msgMsgSink::ForwardToBaseProcess() {
    msgMsgSink_11B0_w((uint8_t*)this - 12);
}

/**
 * msgMsgSink::ForwardToEmbeddedObject() [vtable slot 78 @ 0x824514E8]
 * size: 0x8 (8 bytes)
 *
 * Adjusts this pointer by +20 to reach an embedded sub-object,
 * then tail-calls rage_C1A8 for processing.
 */
void msgMsgSink::ForwardToEmbeddedObject() {
    rage_C1A8((uint8_t*)this + 20);
}

/**
 * msgMsgSink::GetSessionLock() [vtable slot 119 @ 0x8244F768]
 * size: 0xC (12 bytes)
 *
 * Loads the session manager from offset +56, then delegates to
 * msgMsgSink_8A60_sp to acquire a session lock. Returns the lock
 * handle or nullptr if the session manager has no lock object.
 */
void* msgMsgSink::GetSessionLock() {
    void* sessionMgr = *(void**)((uint8_t*)this + 56);
    return msgMsgSink_8A60_sp(sessionMgr);
}

/**
 * msgMsgSink::GetMessageBufferPtr() [vtable slot 115 @ 0x8244F7B8]
 * size: 0xC (12 bytes)
 *
 * Returns a pointer into the message buffer. Loads the buffer base
 * address from offset +48, then adds 9 to skip the message header,
 * returning a pointer to the message payload.
 */
void* msgMsgSink::GetMessageBufferPtr() {
    uint32_t bufferBase = *(uint32_t*)((uint8_t*)this + 48);
    return (void*)(bufferBase + 9);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * GROUP 23: msgMsgSink vtable dispatch thunks (8-196B)
 * Small vtable methods: this-adjustment thunks, field accessors,
 * and locked message buffer operations.
 * ═══════════════════════════════════════════════════════════════════════════ */

// External callees for group 23
extern void msgMsgSink_5098_g(void* self, uint32_t mode);
extern void msgMsgSink_EC28_g(void* self, uint32_t param1, uint32_t param2, uint32_t param3);
extern void msgMsgSink_7970_w(void* self);
extern void msgMsgSink_7DD8_w(void* self);
extern void msgMsgSink_8638_w(void* self);
extern void* rage_01B8(uint32_t size, uint32_t allocatorId);
extern void _locale_register(void* obj, uint32_t allocatorId);

// msgMsgSink::NotifySessionEvent() [vtable slot 14 @ 0x82455540 | 12B]
// Adjusts this by -4 to reach the parent object, then tail-calls
// msgMsgSink_5098_g with mode=1 to notify a session event.
// MATCH: 0x82455540
void msgMsgSink::NotifySessionEvent() {
    void* parent = (uint8_t*)this - 4;
    msgMsgSink_5098_g(parent, 1);
}

// msgMsgSink::GetPeerDataSize() [vtable slot 12 @ 0x82455630 | 12B]
// Returns the 16-bit peer data size field from the descriptor object
// at offset +28, reading the uint16 at descriptor+10.
// MATCH: 0x82455630
uint16_t msgMsgSink::GetPeerDataSize() {
    void* descriptor = *(void**)((uint8_t*)this + 28);
    return *(uint16_t*)((uint8_t*)descriptor + 10);
}

// msgMsgSink::GetPeerDataPtr() [vtable slot 11 @ 0x82455688 | 12B]
// Returns a pointer to the peer data region within the descriptor
// object at offset +28, computed as descriptor+10.
// MATCH: 0x82455688
void* msgMsgSink::GetPeerDataPtr() {
    uint8_t* descriptor = *(uint8_t**)((uint8_t*)this + 28);
    return descriptor + 10;
}

// msgMsgSink::DispatchEventDefault() [vtable slot 42 @ 0x8244ED38 | 48B]
// Calls msgMsgSink_EC28_g with default parameters (0, 0, 1) to dispatch
// an event with a single-shot flag, then returns 0 (success).
// MATCH: 0x8244ED38
uint32_t msgMsgSink::DispatchEventDefault() {
    msgMsgSink_EC28_g(this, 0, 0, 1);
    return 0;
}

// msgMsgSink::ReplaceMessageObject() [vtable slot 21 @ 0x8244EE90 | 120B]
// Replaces the current message object. If the new object (param) is
// non-null, calls vtable[1] (scalar deleting destructor) on it, passing
// this (or null if base offset is zero). Then destroys the sub-object
// at this-8 offset +12 via its destructor with delete flag=1.
// MATCH: 0x8244EE90
void msgMsgSink::ReplaceMessageObject(void* newObj) {
    if (newObj == nullptr) {
        return;
    }

    uint8_t* baseObj = (uint8_t*)this - 8;

    // Call vtable[1] (scalar deleting destructor) on newObj
    void* context = (baseObj != nullptr) ? (void*)this : nullptr;
    typedef void (*ScalarDtorFn)(void*, void*);
    void** vt = *(void***)newObj;
    ScalarDtorFn scalarDtor = (ScalarDtorFn)vt[1];
    scalarDtor(newObj, context);

    // If base object is null, skip cleanup
    if (baseObj == nullptr) {
        return;
    }

    // Destroy the nested object at baseObj+12 with delete flag
    void* nested = baseObj + 12;
    typedef void (*DtorFn)(void*, uint32_t);
    void** nestedVt = *(void***)nested;
    DtorFn dtor = (DtorFn)nestedVt[0];
    dtor(nested, 1);
}

// msgMsgSink::ForwardToMatchHandler() [vtable slot 41 @ 0x82456688 | 8B]
// Adjusts this by -12 to reach the containing network object, then
// tail-calls msgMsgSink_7970_w for match handling.
// MATCH: 0x82456688
void msgMsgSink::ForwardToMatchHandler() {
    void* outer = (uint8_t*)this - 12;
    msgMsgSink_7970_w(outer);
}

// msgMsgSink::ForwardToLeaderboard() [vtable slot 77 @ 0x82457DD0 | 8B]
// Adjusts this by -8 to reach the containing object, then tail-calls
// msgMsgSink_7DD8_w for leaderboard data processing.
// MATCH: 0x82457DD0
void msgMsgSink::ForwardToLeaderboard() {
    void* outer = (uint8_t*)this - 8;
    msgMsgSink_7DD8_w(outer);
}

// msgMsgSink::ForwardToStatsHandler() [vtable slot 95 @ 0x82457FF0 | 8B]
// Adjusts this by -8 to reach the containing object, then tail-calls
// msgMsgSink_8638_w for stats handling.
// MATCH: 0x82457FF0
void msgMsgSink::ForwardToStatsHandler() {
    void* outer = (uint8_t*)this - 8;
    msgMsgSink_8638_w(outer);
}

// msgMsgSink::ForwardToSessionSync() [vtable slot 116 @ 0x824584F0 | 8B]
// Adjusts this by -12 to reach the containing object, then tail-calls
// msgMsgSink_vfn_121 for session synchronization.
// MATCH: 0x824584F0
void msgMsgSink::ForwardToSessionSync() {
    void* outer = (uint8_t*)this - 12;
    extern void msgMsgSink_vfn_121(void*);
    msgMsgSink_vfn_121(outer);
}

// msgMsgSink::SetMessageBuffer() [vtable slot 34 @ 0x8244E1D8 | 196B]
// Thread-safe message buffer allocation and installation.
// Enters critical section from session manager at +56, allocates a
// buffer of the required size (derived from the message header byte
// at param+0, shifted left by 3), copies data from param+4 into it,
// registers the buffer with the locale system if a previous buffer
// existed, stores the new buffer at +332 and header byte at +328,
// notifies the session via msgMsgSink_3E68_g, sets the dirty flag
// at +336, and leaves the critical section. Returns 0 on success,
// or 0x8007000E on allocation failure.
// MATCH: 0x8244E1D8
uint32_t msgMsgSink::SetMessageBuffer(void* param) {
    // Get critical section from session manager at +56
    void* sessionMgr = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionMgr + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);

    // Compute allocation size: header byte << 3
    uint8_t headerByte = *(uint8_t*)param;
    uint32_t allocSize = (uint32_t)headerByte << 3;

    // Allocate buffer with allocator ID 0x20840001
    uint32_t result = 0;
    void* buffer = rage_01B8(allocSize, 0x20840001);

    if (buffer == nullptr) {
        result = (uint32_t)0x8007000E;
        RtlLeaveCriticalSection(criticalSection);
        return result;
    }

    // Copy data from param+4 into the allocated buffer
    void* srcData = *(void**)((uint8_t*)param + 4);
    memcpy(buffer, srcData, allocSize);

    // If a previous buffer existed at +332, register with locale system
    void* prevBuffer = *(void**)((uint8_t*)this + 332);
    if (prevBuffer != nullptr) {
        _locale_register(prevBuffer, 0x20840001);
    }

    // Store new buffer and header byte
    *(void**)((uint8_t*)this + 332) = buffer;
    uint8_t tag = *(uint8_t*)param;
    *(uint8_t*)((uint8_t*)this + 328) = tag;

    // Notify session
    void* session = *(void**)((uint8_t*)this + 52);
    msgMsgSink_3E68_g(session, *(uint32_t*)((uint8_t*)this + 328));

    // Set dirty flag
    *(uint32_t*)((uint8_t*)this + 336) = 1;

    RtlLeaveCriticalSection(criticalSection);
    return result;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * 10 msgMsgSink message dispatch functions (108-200B)
 * Ref-count walkers, slot lookup, session filtering, thread-safe property
 * accessors, and handler lifecycle management.
 * ═══════════════════════════════════════════════════════════════════════════ */

// Forward declarations for callees
extern void msgMsgSink_EC28_g(void* sink, uint32_t key, void* outSlot, uint32_t flags);
extern void msgSlot_SetFloat(void* slot, float value, uint32_t flags);
extern void game_3F28_h(void* session, void* priorityData);
extern int32_t _crt_tls_fiber_setup();

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink_D168_sp @ 0x8244D168 | 108 bytes
// Walks a message chain starting from r4 (slot index). For each valid slot
// (index != 0xFFFF), increments the reference count in the counter array
// at +104, then calls vtable slot 6 to get the next slot index.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink_D168_sp(void* self, uint16_t slotIndex) {
    uint16_t index = slotIndex;

    while (index != 0xFFFF) {
        // Increment reference count: counterArray[index]++
        uint32_t* counterArray = *(uint32_t**)((uint8_t*)self + 104);
        counterArray[index]++;

        // Call vtable slot 6 to get next slot index
        typedef uint16_t (*GetNextFn)(void*);
        void** vt = *(void***)self;
        GetNextFn getNext = (GetNextFn)vt[6];
        index = getNext(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink_D1D8_sp @ 0x8244D1D8 | 108 bytes
// Walks a message chain starting from r4 (slot index). For each valid slot
// (index != 0xFFFF), decrements the reference count in the counter array
// at +104, then calls vtable slot 6 to get the next slot index.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink_D1D8_sp(void* self, uint16_t slotIndex) {
    uint16_t index = slotIndex;

    while (index != 0xFFFF) {
        // Decrement reference count: counterArray[index]--
        uint32_t* counterArray = *(uint32_t**)((uint8_t*)self + 104);
        counterArray[index]--;

        // Call vtable slot 6 to get next slot index
        typedef uint16_t (*GetNextFn)(void*);
        void** vt = *(void***)self;
        GetNextFn getNext = (GetNextFn)vt[6];
        index = getNext(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink_8D10_g @ 0x82448D10 | 128 bytes
// Looks up slot data by index. Loads the message handler object from +124.
// If present, calls handler vtable slot 11 to get the entry count, validates
// that the index is within range, then calls handler vtable slot 2 to
// retrieve the slot data pointer. Returns nullptr on failure.
// ─────────────────────────────────────────────────────────────────────────────
void* msgMsgSink_8D10_g(void* self, uint32_t slotIndex) {
    void* handler = *(void**)((uint8_t*)self + 124);

    if (handler != nullptr) {
        // Call vtable slot 11 to get the slot table descriptor
        typedef void* (*GetDescFn)(void*);
        void** vt = *(void***)handler;
        GetDescFn getDesc = (GetDescFn)vt[11];
        void* desc = getDesc(handler);

        // Check index against entry count at desc+11 (uint16)
        uint16_t entryCount = *(uint16_t*)((uint8_t*)desc + 11);
        uint16_t idx = (uint16_t)(slotIndex & 0xFFFF);

        if (idx >= entryCount) {
            return nullptr;
        }

        // Call vtable slot 2 to get slot data by index
        typedef void* (*GetSlotFn)(void*, uint32_t);
        void** vt2 = *(void***)handler;
        GetSlotFn getSlot = (GetSlotFn)vt2[2];
        return getSlot(handler, slotIndex);
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink_8BA8_2hr @ 0x82448BA8 | 184 bytes
// Iterates the intrusive list anchored at +128/+132, filtering sessions by
// up to three optional criteria: a vtable-slot-24 match (r4), a field +284
// match (r5), and a uint16 +292 match (r6, 0xFFFF = skip). Destroys
// matching sessions by calling their destructor at +12. Continues until
// the list is exhausted.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink_8BA8_2hr(void* self, void* matchKey, void* matchSession,
                         uint16_t matchSlot) {
    void* anchor = (uint8_t*)self + 128;
    void* node = *(void**)((uint8_t*)self + 132);

    while (node != anchor) {
        // Save next pointer before potential destruction
        void* next = *(void**)((uint8_t*)node + 4);

        // Adjust node pointer: entry is at node - 12
        void* entry = (uint8_t*)node - 12;

        // Filter 1: matchKey (vtable slot 24 comparison)
        if (matchKey != nullptr) {
            typedef int (*MatchFn)(void*, void*);
            void** vt = *(void***)entry;
            MatchFn matchFn = (MatchFn)vt[24];
            if (matchFn(entry, matchKey) == 0) {
                goto skipEntry;
            }
        }

        // Filter 2: matchSession (field +284 comparison)
        if (matchSession != nullptr) {
            void* entrySession = *(void**)((uint8_t*)entry + 284);
            if (entrySession != matchSession) {
                goto skipEntry;
            }
        }

        // Filter 3: matchSlot (uint16 at +292 comparison)
        if ((matchSlot & 0xFFFF) != 0xFFFF) {
            uint16_t entrySlot = *(uint16_t*)((uint8_t*)entry + 292);
            if (entrySlot != (matchSlot & 0xFFFF)) {
                goto skipEntry;
            }
        }

        // All filters passed - destroy the entry
        if (entry != nullptr) {
            void* objToDestroy = (uint8_t*)entry + 12;
            typedef void (*DtorFn)(void*, int);
            void** objVt = *(void***)objToDestroy;
            DtorFn dtor = (DtorFn)objVt[0];
            dtor(objToDestroy, 1);
        }

    skipEntry:
        node = next;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::UpdatePrioritiesLocked() [vtable slot 28 @ 0x8244E4C0] | 168 bytes
// Thread-safe priority update: enters critical section, iterates over
// entries in the priority data structure. For each entry whose index is
// less than the local count at +296, copies the priority float from the
// data buffer at +300. Entries beyond the count get a default zero float.
// Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::UpdatePrioritiesLocked(void* priorityData) {
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);

    uint8_t entryCount = *(uint8_t*)priorityData;

    if (entryCount != 0) {
        float defaultPriority = 0.0f;
        uint8_t localCount = *(uint8_t*)((uint8_t*)this + 296);
        void* dataBuffer = *(void**)((uint8_t*)this + 300);

        for (uint8_t i = 0; i < entryCount; i++) {
            uint8_t* entry = (uint8_t*)priorityData + 4 + ((uint32_t)i << 3);
            uint8_t entryIndex = *(uint8_t*)entry;

            if (entryIndex < localCount) {
                float* srcPriority = (float*)((uint8_t*)dataBuffer +
                                              ((uint32_t)entryIndex << 3) + 4);
                *(float*)(entry + 4) = *srcPriority;
            } else {
                *(float*)(entry + 4) = defaultPriority;
            }
        }
    }

    RtlLeaveCriticalSection(criticalSection);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::ApplyPrioritiesLocked() [vtable slot 29 @ 0x8244E568] | 168 bytes
// Thread-safe priority apply: enters critical section, iterates entries in
// the priority data. For each entry whose index is in range, copies the
// float from the entry into the local data buffer at +300. After the loop,
// forwards the data to the session object at +52 via game_3F28_h.
// Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::ApplyPrioritiesLocked(void* priorityData) {
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);

    uint8_t entryCount = *(uint8_t*)priorityData;

    if (entryCount != 0) {
        for (uint8_t i = 0; i < entryCount; i++) {
            uint8_t* srcEntry = (uint8_t*)priorityData + 4 + ((uint32_t)i << 3);
            uint8_t entryIndex = *(uint8_t*)srcEntry;
            uint8_t localCount = *(uint8_t*)((uint8_t*)this + 296);

            if (entryIndex < localCount) {
                float srcFloat = *(float*)(srcEntry + 4);
                void* dataBuffer = *(void**)((uint8_t*)this + 300);
                float* dstPriority = (float*)((uint8_t*)dataBuffer +
                                              ((uint32_t)entryIndex << 3) + 4);
                *dstPriority = srcFloat;
            }
        }
    }

    // Forward to session if present
    void* session = *(void**)((uint8_t*)this + 52);
    if (session != nullptr) {
        game_3F28_h(session, priorityData);
    }

    RtlLeaveCriticalSection(criticalSection);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::SetPropertyLocked() [vtable slot 31 @ 0x8244F268] | 200 bytes
// Thread-safe property set: acquires generation lock, locates the message
// slot via msgMsgSink_EC28_g, validates the slot is active (bit 0) and not
// disconnecting (bit 1). If the new float value differs from the current
// one, applies it via msgSlot_SetFloat. Returns error 0x8A65000A if slot invalid.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::SetPropertyLocked(uint32_t key, float value) {
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);

    void* session = *(void**)((uint8_t*)this + 56);
    extern int32_t msgMsgSink_84C0_gen(void*);
    int32_t genResult = msgMsgSink_84C0_gen(session);

    uint32_t result = (uint32_t)genResult;
    void* slot = nullptr;

    if (genResult >= 0) {
        msgMsgSink_EC28_g(this, key, &slot, 0);
        result = (uint32_t)(uintptr_t)slot;

        if ((int32_t)result >= 0 && slot != nullptr) {
            void* desc = *(void**)((uint8_t*)slot + 16);
            uint8_t flags = *(uint8_t*)desc;

            if ((flags & 0x1) != 1) {
                result = 0x8A65000A;
            } else if (flags & 0x2) {
                result = 0x8A65000A;
            } else {
                float currentVal = *(float*)((uint8_t*)slot + 20);
                if (value != currentVal) {
                    msgSlot_SetFloat(slot, value, 0);
                }
            }
        }
    }

    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::GetPropertyLocked() [vtable slot 32 @ 0x8244F330] | 200 bytes
// Thread-safe property get: acquires generation lock, validates the key
// (rejects keys 1 and 2), locates the message slot, validates it is active
// (bit 0 set), reads the float at slot+20 into the output pointer.
// Returns error 0x8A65000A if slot is invalid.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::GetPropertyLocked(uint32_t key, float* outValue) {
    void* sessionObj = *(void**)((uint8_t*)this + 56);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);

    void* session = *(void**)((uint8_t*)this + 56);
    extern int32_t msgMsgSink_84C0_gen(void*);
    int32_t genResult = msgMsgSink_84C0_gen(session);

    uint32_t result = (uint32_t)genResult;

    if (genResult >= 0) {
        uint16_t keyLower = (uint16_t)(key & 0xFFFF);
        if (keyLower == 1 || keyLower == 2) {
            RtlLeaveCriticalSection(criticalSection);
            return result;
        }

        void* slot = nullptr;
        msgMsgSink_EC28_g(this, key, &slot, 0);
        result = (uint32_t)(uintptr_t)slot;

        if ((int32_t)result >= 0 && slot != nullptr) {
            void* desc = *(void**)((uint8_t*)slot + 16);
            uint8_t flags = *(uint8_t*)desc;

            if (flags & 0x1) {
                float propValue = *(float*)((uint8_t*)slot + 20);
                *outValue = propValue;

                RtlLeaveCriticalSection(criticalSection);
                return result;
            }

            result = 0x8A65000A;
        }
    }

    RtlLeaveCriticalSection(criticalSection);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::FlushPendingMessagesLocked() [vtable slot 130 @ 0x8244F9E8] | 188 bytes
// Thread-safe flush: acquires generation lock, then while the session at
// +240 is non-null, dequeues the head message from the pending list at
// +28/+204 and calls its destructor (vtable slot 0) with +4 as param.
// After the loop, destroys the sub-object at +12. Returns the generation
// result.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t msgMsgSink::FlushPendingMessagesLocked() {
    void* sessionObj = *(void**)((uint8_t*)this + 24);
    void* criticalSection = (uint8_t*)sessionObj + 144;

    extern void RtlEnterCriticalSection(void*);
    extern void RtlLeaveCriticalSection(void*);
    RtlEnterCriticalSection(criticalSection);

    void* session = *(void**)((uint8_t*)this + 24);
    extern int32_t msgMsgSink_84C0_gen(void*);
    int32_t genResult = msgMsgSink_84C0_gen(session);

    if (genResult >= 0) {
        uint32_t sessionField = *(uint32_t*)((uint8_t*)this + 240);
        bool hasSession = (sessionField != 0);

        while (hasSession) {
            void* pendingList = (uint8_t*)this + 28;
            void* pendingHead = *(void**)((uint8_t*)pendingList + 204);
            void* msg;

            if (pendingHead != nullptr) {
                msg = *(void**)pendingHead;
            } else {
                msg = nullptr;
            }

            typedef void (*DtorFn)(void*, void*);
            void** vt = *(void***)msg;
            DtorFn dtor = (DtorFn)vt[0];
            dtor(msg, (void*)((uint8_t*)this + 4));

            sessionField = *(uint32_t*)((uint8_t*)this + 240);
            hasSession = (sessionField != 0);
        }

        void* subObj = (uint8_t*)this + 12;
        typedef void (*SubDtorFn)(void*, int);
        void** subVt = *(void***)subObj;
        SubDtorFn subDtor = (SubDtorFn)subVt[0];
        subDtor(subObj, 1);
    }

    RtlLeaveCriticalSection(criticalSection);
    return (uint32_t)genResult;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink::DisconnectMatchingHandlers() [vtable slot 123 @ 0x82450610] | 192 bytes
// Walks the handler linked list from +224. For each node, extracts the
// handler object (node[0] - 8), calls vtable slot 24 to match against the
// provided key (adjusted by -4). If the match succeeds, destroys the
// handler's sub-object at +8. After the loop, calls vtable slot 1 on the
// key object for final cleanup.
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink::DisconnectMatchingHandlers(void* key) {
    void* adjustedKey = (key != nullptr) ? (uint8_t*)key - 4 : nullptr;

    void* node = *(void**)((uint8_t*)this + 224);

    while (node != nullptr) {
        void* rawHandler = *(void**)node;
        void* next = *(void**)((uint8_t*)node + 4);

        void* handler = (rawHandler != nullptr) ? (uint8_t*)rawHandler - 8 : nullptr;

        typedef int (*MatchFn)(void*, void*);
        void** vt = *(void***)handler;
        MatchFn matchFn = (MatchFn)vt[24];
        int matched = matchFn(handler, adjustedKey);

        if (matched != 0) {
            void* adjustedSelf = ((uintptr_t)this > 8) ?
                                 (uint8_t*)this - 4 : nullptr;

            void* subObj = (uint8_t*)handler + 8;
            typedef void (*DtorFn)(void*, void*);
            void** subVt = *(void***)subObj;
            DtorFn dtor = (DtorFn)subVt[0];
            dtor(subObj, adjustedSelf);
        }

        node = next;
    }

    if (key != nullptr) {
        typedef void (*CleanupFn)(void*, void*);
        void** keyVt = *(void***)key;
        CleanupFn cleanup = (CleanupFn)keyVt[1];
        cleanup(key, (void*)this);
    }
}
