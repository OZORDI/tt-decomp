/**
 * cm_operators.cpp — Conditional Machine operators and constructors
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The "cm" subsystem implements a data-flow graph for game logic conditions,
 * reporters, and operators. These classes are template-instantiated constructors
 * that build operator nodes at runtime.
 */

#include "cm_operators.hpp"
#include <stdint.h>
#include <stddef.h>

// Forward declarations
extern "C" void xe_main_thread_init_0038(void);

// External vtable addresses
extern "C" const void* rage_cmIntegrate_vtable;  // @ 0x82054934

// SDA base - points to allocator context
extern uint32_t* g_sda_base;  // @ 0x82600000 (r13)


/**
 * cmWorldRefreshableCtor::vfn_0_8438_1 @ 0x82268438 | size: 0xa0
 *
 * Constructor variant that allocates and initializes a rage::cmIntegrate object.
 * This is one of 18 template instantiations of cmWorldRefreshableCtor for different
 * operator types.
 *
 * The function:
 * 1. Ensures thread context is initialized
 * 2. Allocates 40 bytes with 16-byte alignment from the RAGE allocator
 * 3. Zero-initializes the object
 * 4. Sets the vtable to rage::cmIntegrate
 * 5. Clears the "initialized" flag bit at offset +8
 *
 * @return Pointer to the newly constructed rage::cmIntegrate object, or nullptr on failure
 */
void* cmWorldRefreshableCtor_vfn_0_8438_1()
{
    // Initialize main thread context if not already done
    xe_main_thread_init_0038();
    
    // Get allocator context from SDA base
    // SDA offset 0 points to allocator context struct
    uint32_t* allocatorCtx = (uint32_t*)(uintptr_t)g_sda_base[0];
    
    // Get allocator object from context offset +4
    void** allocator = (void**)(uintptr_t)allocatorCtx[1];
    
    // Call allocator->Allocate(40, 16) via vtable slot 1
    typedef void* (*AllocateFn)(void* self, uint32_t size, uint32_t alignment);
    AllocateFn allocate = (AllocateFn)allocator[1];
    void* obj = allocate(allocator, 40, 16);
    
    if (obj != nullptr) {
        // Zero-initialize the object fields
        uint32_t* fields = (uint32_t*)obj;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28 - set to 2 (likely a type/state enum)
        fields[8] = 0;  // +32
        fields[9] = 0;  // +36
        
        // Set vtable pointer to rage::cmIntegrate
        fields[0] = (uint32_t)(uintptr_t)&rage_cmIntegrate_vtable;
        
        // Clear bit 0 of the field at offset +8 (clear "initialized" flag)
        fields[2] = fields[2] & ~1u;
    }
    
    return obj;
}
