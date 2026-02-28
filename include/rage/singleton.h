#pragma once

/**
 * singleton.h — RAGE atSingleton Registry
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * The atSingleton registry tracks singleton-owned memory regions.
 * The heap free path (rage_free, sysMemAllocator_Free) checks this
 * registry to avoid freeing memory that belongs to a singleton subsystem.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * rage::FindSingleton @ 0x820F90D0 | size: 0xA4
 *
 * Checks if a pointer is owned by a registered singleton.
 * Returns nonzero if the pointer belongs to a singleton (do NOT free).
 * Returns zero if the pointer is not singleton-owned (safe to free).
 *
 * C-linkage alias: rage_FindSingleton
 */
uint8_t rage_FindSingleton(void* ptr);

#ifdef __cplusplus
}  /* extern "C" */

/* C++ namespace alias */
namespace rage {
    inline uint8_t FindSingleton(void* ptr) { return rage_FindSingleton(ptr); }
}
#endif
