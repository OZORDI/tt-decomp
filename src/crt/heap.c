/**
 * heap.c — CRT Heap Management Stubs
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Platform heap allocation wrappers. On Xbox 360, these delegate to
 * the kernel's MmAllocatePhysicalMemory / MmFreePhysicalMemory.
 * For the decomp, we delegate to the platform's standard malloc/free.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * rage_01B8 — RAGE engine allocation function @ 0x820C01B8
 * Allocates memory with the given size and flags.
 * Flags are Xbox 360 specific allocation attributes (alignment, caching).
 */
void* rage_01B8(uint32_t size, uint32_t flags) {
    (void)flags;  /* Flags are Xbox 360 specific — ignored on host */
    return malloc(size);
}

/**
 * rage_free — RAGE engine free function @ 0x820C00C0
 * Frees previously allocated memory.
 */
void rage_free(void* ptr) {
    free(ptr);
}

/* rage_free_00C0 was an alias — merged with rage_free above */

/**
 * rage_alloc — Allocate zeroed memory @ 0x820CEC88
 */
void* rage_alloc(uint32_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}
