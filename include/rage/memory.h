#pragma once

/**
 * memory.h — RAGE Engine Memory Management (convenience header)
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Includes the canonical allocator struct and function declarations
 * from rage_allocator.h, and adds the C++ rage::Free wrapper.
 */

#include "rage_allocator.h"

#ifdef __cplusplus
namespace rage {
    /** Convenience wrapper for rage_free with C++ namespace. */
    inline void Free(void* ptr) {
        rage_free(ptr);
    }
}
#endif
