/**
 * rage_allocator.h — RAGE sysMemAllocator interface
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Defines the vtable layout and struct for the engine's allocator system.
 * The active allocator is stored in the SDA (Small Data Area) at r13+0,
 * with the allocator pointer at offset +4 from that base.
 *
 * Vtable layout (confirmed from recomp scaffold):
 *   slot  0 (+0x00) : destructor / bind
 *   slot  1 (+0x04) : Allocate(self, ptr, size)
 *   slot  2 (+0x08) : Free(self, ptr)
 *   slots 3-16      : reserved / other methods
 *   slot 17 (+0x44) : IsAddressOwned(self) -> bool
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * sysMemAllocator Vtable
 * ============================================================================ */

typedef struct rageAllocatorVtable {
    void  (*Destroy)(void* self);                                      /* +0x00 slot 0  */
    void* (*Allocate)(void* self, void* ptr, size_t size);             /* +0x04 slot 1  */
    void  (*Free)(void* self, void* ptr);                              /* +0x08 slot 2  */
    void* reserved3;                                                   /* +0x0C slot 3  */
    void* reserved4;                                                   /* +0x10 slot 4  */
    void* reserved5;                                                   /* +0x14 slot 5  */
    void* reserved6;                                                   /* +0x18 slot 6  */
    void* reserved7;                                                   /* +0x1C slot 7  */
    void* reserved8;                                                   /* +0x20 slot 8  */
    void* reserved9;                                                   /* +0x24 slot 9  */
    void* reserved10;                                                  /* +0x28 slot 10 */
    void* reserved11;                                                  /* +0x2C slot 11 */
    void* reserved12;                                                  /* +0x30 slot 12 */
    void* reserved13;                                                  /* +0x34 slot 13 */
    void* reserved14;                                                  /* +0x38 slot 14 */
    void* reserved15;                                                  /* +0x3C slot 15 */
    void* reserved16;                                                  /* +0x40 slot 16 */
    uint8_t (*IsAddressOwned)(void* self);                             /* +0x44 slot 17 */
} rageAllocatorVtable;

/* ============================================================================
 * sysMemAllocator Instance
 * ============================================================================ */

typedef struct rageAllocator {
    rageAllocatorVtable* vtable;    /* +0x00 */
} rageAllocator;

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

/** rage_free @ 0x820C00C0 | size: 0x60 */
void rage_free(void* ptr);

/** thunk_rage_free @ 0x820C0120 | size: 0x8 — tail-call to rage_free */
void rage_free_alias(void* ptr);

/** sysMemAllocator_Allocate @ 0x821861A0 | size: 0x84 */
void* sysMemAllocator_Allocate(void* ptr, size_t size);

/** sysMemAllocator_Free @ 0x82186228 | size: 0x74 */
void sysMemAllocator_Free(void* ptr);

/** atSingleton_Find_90D0 @ 0x820F90D0 | size: 0xA4
 *  Returns nonzero if pointer is owned by a registered singleton. */
uint8_t atSingleton_Find_90D0(void* ptr);

#ifdef __cplusplus
}
#endif
