/**
 * memory.hpp — RAGE memory management system
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * atSingleton template class and memory allocation utilities.
 */

#pragma once

#include <cstdint>
#include <cstring>

// Forward declarations
extern void rage_free(void* ptr);
extern void* rage_01B8(uint32_t size, uint32_t flags);
extern void atSingleton_dtor_7628(void* obj);
extern void memset(void* ptr, int value, size_t size);

/**
 * atSingleton — RAGE singleton template base class
 * 
 * Base class for all singleton objects in the RAGE engine.
 * Provides reference counting, lifecycle management, and registry tracking.
 * 
 * This is a massive class with 1760+ functions, representing the core
 * singleton pattern used throughout the engine for managers, systems,
 * and data objects.
 */
struct atSingleton {
    void* vtable;           // +0x00 - Virtual function table
    uint32_t flags;         // +0x04 - Object state flags
    uint16_t refCount;      // +0x06 - Reference count
    
    // Additional fields discovered through analysis
    // (Full struct layout to be refined as more functions are implemented)
};

// Global data
extern uint32_t g_grcDevicePtr;  // @ 0x8260637C (SDA +25468)

