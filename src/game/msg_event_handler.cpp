/**
 * msg_event_handler.cpp — msgEventHandler methods
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.11.cpp:4857-5534
 */

#include "pong_misc.hpp"
#include <cstring>

// External dependencies
extern void rage_free(void* ptr);
extern void* sysMemAllocator_Allocate(void* ptr, size_t size);
extern void  xe_main_thread_init_0038();
extern void  atSingleton_9420(void* obj);

// Vtable pointers
static void* const kMsgEvtHandler_PrimaryVtable  = (void*)0x82050144;
static void* const kMsgEvtHandler_SecondaryVtable = (void*)0x820501A8;
static void* const kMsgEvtHandler_BaseVtable      = (void*)0x82027B34; // base class

// Global handler registry (SDA-relative)
// g_handlerCount  @ 0x825F3864  — count of registered handlers
// g_handlerPtrs   @ 0x825F3868  — array of handler pointers (stride 4)
// g_handlerData1  @ 0x825F3998  — parallel array (stride 4)
// g_handlerData2  @ 0x825F3868  — parallel array (stride 4)
extern uint32_t  g_handlerCount;   // @ lbl_825F3864
extern void*     g_handlerPtrs[];  // @ lbl_825F3868  (indexed by handler slot)

// Global message type IDs for AcceptsMessage
extern uint32_t  g_msgType1;       // @ 0x825C495C
extern uint32_t  g_msgType2;       // @ 0x825E801C
extern uint32_t  g_msgType3;       // @ 0x825E8018

// Static name string returned by GetName
static const char* const kHandlerName = (const char*)0x82049FA0;

/**
 * msgEventHandler::Create() @ 0x8225EC68 | size: 0x90
 *
 * Factory function — allocates a 28-byte msgEventHandler via the
 * thread-local allocator and initialises all fields to zero.
 *
 * @return Pointer to new handler, or nullptr on allocation failure.
 */
msgEventHandler* msgEventHandler::Create()
{
    xe_main_thread_init_0038();

    // Allocate 28 bytes (size=28, alignment=16) via allocator vtable slot 1
    void* mem = sysMemAllocator_Allocate(nullptr, 28);
    if (!mem)
        return nullptr;

    msgEventHandler* handler = (msgEventHandler*)mem;

    // Zero all fields
    handler->m_ppHandlers   = nullptr;              // +0x04
    handler->m_nHandlers    = 0;                    // +0x08
    handler->m_pMsgChain    = nullptr;              // +0x0C

    // Set vtables
    handler->vtable  = (void**)kMsgEvtHandler_PrimaryVtable;
    handler->vtable2 = (void**)kMsgEvtHandler_SecondaryVtable;

    // Zero owned data fields
    handler->m_pOwnedData   = nullptr;              // +0x14
    handler->m_field_18     = 0;                    // +0x18
    handler->m_ownedFlag    = 0;                    // +0x1A

    return handler;
}

/**
 * msgEventHandler::Unregister() @ 0x8225E8C0 | size: 0xD0
 *
 * Removes this handler's subobject (at +16) from the global handler
 * registry.  Searches g_handlerPtrs[] for a matching pointer, then
 * shifts remaining entries down (using memmove-equivalent) to keep
 * the array packed.
 */
void msgEventHandler::Unregister()
{
    if (g_handlerCount == 0)
        return;

    // Search for this handler's secondary-vtable subobject in the global array
    void* self = (void*)((uint8_t*)this);
    uint32_t idx = 0;

    for (idx = 0; idx < g_handlerCount; idx++) {
        if (g_handlerPtrs[idx] == self)
            break;
    }

    if (idx >= g_handlerCount)
        return;  // not found

    // Decrement count
    g_handlerCount--;

    if (idx < g_handlerCount) {
        // Shift remaining entries down in all three parallel arrays
        uint32_t remaining = g_handlerCount - idx;
        // memmove for handler ptrs, data1, data2 arrays
        memmove(&g_handlerPtrs[idx], &g_handlerPtrs[idx + 1],
                remaining * sizeof(void*));
    }
}

/**
 * msgEventHandler::DispatchMessage() — rtti_01A8_1 @ 0x8225EBC8(+offset)
 *
 * Iterates the handler array at +4 (count at +8).  For each handler
 * whose message-type field (+20, uint16) matches the incoming message
 * type (uint16 at msg+0), calls the handler's vtable slot 23.
 *
 * @param msg  Pointer to message object (uint16 type at offset 0)
 */
void msgEventHandler::DispatchMessage(void* msg)
{
    uint16_t incomingType = *(uint16_t*)msg;

    for (int i = 0; i < (int)m_nHandlers; i++) {
        void* handler = m_ppHandlers[i];
        uint16_t handlerType = *(uint16_t*)((uint8_t*)handler + 20);

        if (incomingType == handlerType) {
            // Call handler vtable slot 23 (OnMessage)
            void** handlerVtable = *(void***)handler;
            typedef void (*OnMessageFn)(void*, void*);
            OnMessageFn onMsg = (OnMessageFn)handlerVtable[23];
            onMsg(handler, msg);
        }
    }
}

/**
 * msgEventHandler::GetName() — vfn_22 @ 0x8225EC58 | size: 0x10
 *
 * Returns a static string constant identifying this handler type.
 */
const char* msgEventHandler::GetName()
{
    return kHandlerName;
}

/**
 * msgEventHandler::AcceptsMessage() — vfn_20 @ 0x8225ED08 | size: 0x48
 *
 * Checks whether a message type is accepted by this handler.
 * Compares against three global message-type IDs.
 *
 * @param msgType  The message type to check
 */
void msgEventHandler::AcceptsMessage(uint32_t msgType)
{
    // Check against known message type globals
    if (msgType == g_msgType1)
        *(uint32_t*)((uint8_t*)this) = 1;  // return true via r3
    else if (msgType == g_msgType2)
        *(uint32_t*)((uint8_t*)this) = 1;
    else
        *(uint32_t*)((uint8_t*)this) = (msgType == g_msgType3) ? 1 : 0;
}

/**
 * msgEventHandler::~msgEventHandler() @ 0x8225EB50 | size: 0x78
 *
 * Destructor:
 *   1. Sets derived vtables (base class vtables for cleanup)
 *   2. Calls Unregister() on the secondary subobject (+16)
 *   3. If m_ownedFlag != 0: frees m_pOwnedData
 *   4. Sets base vtable on secondary subobject
 *   5. Calls atSingleton_9420 for cleanup
 *   6. If (flags & 1): frees this
 */
msgEventHandler::~msgEventHandler()
{
    // Set derived-class vtables for cleanup dispatch
    vtable  = (void**)kMsgEvtHandler_PrimaryVtable;
    vtable2 = (void**)kMsgEvtHandler_SecondaryVtable;

    // Unregister the secondary subobject from the global handler array
    msgEventHandler* subObj = (msgEventHandler*)((uint8_t*)this + 16);
    subObj->Unregister();

    // Free owned data if flagged
    if (m_ownedFlag != 0) {
        rage_free(m_pOwnedData);
    }

    // Restore base vtable on secondary subobject
    vtable2 = (void**)kMsgEvtHandler_BaseVtable;

    // Base class cleanup
    atSingleton_9420(this);

    // Note: if (flags & 1) rage_free(this);
    // Handled by the standard RAGE deleting-destructor ABI.
}
