/**
 * heap.hpp - RAGE Heap Management
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Heap allocation functions
void* rage_malloc(size_t size);
void rage_free(void* ptr);

#ifdef __cplusplus
}
#endif
