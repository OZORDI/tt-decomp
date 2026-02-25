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
 * atSingleton_Find_90D0 @ 0x820F90D0 | size: 0xA4
 *
 * Checks if a pointer is owned by a registered singleton.
 * Returns nonzero if the pointer belongs to a singleton (do NOT free).
 * Returns zero if the pointer is not singleton-owned (safe to free).
 */
uint8_t atSingleton_Find_90D0(void* ptr);

#ifdef __cplusplus
}
#endif
