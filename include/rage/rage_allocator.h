/**
 * RAGE Allocator Interface
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Memory allocation system for RAGE engine.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// RAGE Allocator Vtable
// ============================================================================

typedef struct rageAllocatorVtable {
    void* (*Allocate)(void* self, size_t size, size_t alignment);  // +0x00
    void* reserved1;                                                // +0x04
    void (*Free)(void* self, void* ptr);                           // +0x08
} rageAllocatorVtable;

// ============================================================================
// RAGE Allocator Base Class
// ============================================================================

typedef struct rageAllocator {
    rageAllocatorVtable* vtable;  // +0x00
    // Additional members TBD
} rageAllocator;

// ============================================================================
// Memory Management Functions
// ============================================================================

/**
 * rage_free
 * @ 0x820C00C0 | size: 0x60
 * 
 * Frees memory allocated by RAGE allocator.
 * Validates pointer before freeing.
 */
void rage_free(void* ptr);

/**
 * rage_free_alias
 * @ 0x820C0120 | size: 0x8
 * 
 * Alias for rage_free.
 */
void rage_free_alias(void* ptr);

// ============================================================================
// Helper Functions
// ============================================================================

/**
 * rage_IsValidPointer
 * @ 0x820F90D0
 * 
 * Validates if a pointer is within valid memory ranges.
 * Returns 0 if valid, non-zero if invalid.
 */
uint8_t rage_IsValidPointer(void* ptr);

/**
 * rage_GetAllocatorFromTLS
 * 
 * Gets the current thread's RAGE allocator from TLS.
 */
void* rage_GetAllocatorFromTLS(void);

/**
 * rage_SetAllocatorInTLS
 * 
 * Sets the current thread's RAGE allocator in TLS.
 */
void rage_SetAllocatorInTLS(void* allocator);

#ifdef __cplusplus
}
#endif
