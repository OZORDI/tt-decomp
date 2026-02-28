/**
 * msg_msg_sink.cpp — msgMsgSink constructor & destructor
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.14.cpp:73786
 *                                           tt-decomp_recomp.12.cpp:28709
 */

#include "pong_misc.hpp"
#include <cstring>

// External dependencies
extern void rage_free(void* ptr);

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
