/**
 * xe_tls.c — Xbox 360 Thread-Local Storage and physical memory shims
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Implements the XE (Xbox Executive) TLS block management and physical
 * memory allocation functions. On Xbox 360, these manage per-thread
 * execution contexts (XeTlsBlock) and physical memory pages.
 * On the host, we use standard malloc and POSIX TLS.
 *
 * Functions implemented:
 *   XeTlsBlock_InitStack       — initialize thread stack descriptor
 *   XeTlsBlock_BindMainThread  — bind TLS block to main thread
 *   xe_main_thread_init        — main thread context setup (alias for sysMemAllocator_InitMainThread)
 *   XePhysicalAlloc_stub       — physical memory allocation
 *   xe_GetLoadContext           — get load context (2 overloads)
 *   game_CrtFatalExit_thunk    — CRT fatal exit handler
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*=========================================================================
 * XeTlsBlock Management
 *
 * On Xbox 360, each thread has an XeTlsBlock that describes its stack,
 * TLS area, and execution context. The structure is accessed via the
 * SDA base register (r13).
 *
 * On the host, we don't need real TLS block management since the
 * platform's threading system handles stack and TLS automatically.
 * These functions maintain minimal state for compatibility.
 *=========================================================================*/

/**
 * XeTlsBlock_InitStack
 *
 * Initializes the stack descriptor fields of an XeTlsBlock.
 * On Xbox 360, this records the stack base address and size so the
 * kernel can manage stack overflow detection and guard pages.
 *
 * On the host, we just record the values in the structure for
 * compatibility with code that reads them.
 *
 * @param pXtf       Pointer to XeTlsBlock structure
 * @param stackBase  Base address of the allocated stack
 * @param stackSize  Size of the stack in bytes
 */
void XeTlsBlock_InitStack(void* pXtf, void* stackBase, uint32_t stackSize) {
    if (!pXtf) return;

    /* XeTlsBlock layout (from entry.c):
     *   +0x04  m_pStackAlign16
     *   +0x08  m_pStackRaw
     *   +0x50  m_stackSz
     */
    uint8_t* block = (uint8_t*)pXtf;

    /* Store stack base at +0x04 (aligned) and +0x08 (raw) */
    *(void**)(block + 0x04) = stackBase;
    *(void**)(block + 0x08) = stackBase;

    /* Store stack size at +0x50 */
    *(uint32_t*)(block + 0x50) = stackSize;
}

/**
 * XeTlsBlock_BindMainThread
 *
 * Binds an XeTlsBlock to the main thread's execution context.
 * On Xbox 360, this writes the block pointer into the kernel's
 * per-thread data area so the scheduler can find it.
 *
 * On the host, this is a no-op — the main thread's context is
 * managed by the OS.
 *
 * @param ctx  Pointer to XeTlsBlock to bind
 */
void XeTlsBlock_BindMainThread(void* ctx) {
    (void)ctx;
    /* No-op on host — thread context is managed by the OS */
}

/**
 * xe_main_thread_init @ 0x820C0038 | size: 0x88
 *
 * Initializes the main thread's memory allocator context.
 * This is an alias for sysMemAllocator_InitMainThread, which is
 * already implemented in crt/entry.c.
 *
 * On the host, the standard C library's malloc/free are always
 * available, so this is a no-op.
 */
void xe_main_thread_init(void) {
    /* The host's memory allocator is always ready.
     * sysMemAllocator_InitMainThread in entry.c handles the
     * Xbox 360-specific allocator context setup. */
}

/*=========================================================================
 * Physical Memory Allocation
 *=========================================================================*/

/**
 * XePhysicalAlloc_stub
 *
 * Allocates physical memory with specified protection and alignment.
 * On Xbox 360, this calls MmAllocatePhysicalMemoryEx to get
 * contiguous physical pages for DMA, GPU, or thread stacks.
 *
 * On the host, we use aligned_alloc (or posix_memalign) to provide
 * aligned memory. The protection flags are ignored since the host
 * OS manages memory protection differently.
 *
 * @param sizeBytes     Number of bytes to allocate
 * @param protectFlags  Memory protection flags (ignored on host)
 * @param alignment     Required alignment (must be power of 2)
 * @param allocFlags    Allocation flags (ignored on host)
 * @return  Pointer to allocated memory, or NULL on failure
 */
void* XePhysicalAlloc_stub(uint32_t sizeBytes, int32_t protectFlags,
                           uint32_t alignment, uint32_t allocFlags) {
    (void)protectFlags;
    (void)allocFlags;

    if (sizeBytes == 0) return NULL;

    /* Ensure minimum alignment of 16 bytes (Xbox 360 standard) */
    if (alignment < 16) alignment = 16;

    /* Round up alignment to power of 2 */
    uint32_t align = alignment;
    if ((align & (align - 1)) != 0) {
        /* Not a power of 2 — round up */
        align = 1;
        while (align < alignment) align <<= 1;
    }

    /* Round up size to alignment boundary */
    size_t allocSize = (sizeBytes + align - 1) & ~(size_t)(align - 1);

    void* ptr = NULL;
#if defined(_WIN32)
    ptr = _aligned_malloc(allocSize, align);
#else
    if (posix_memalign(&ptr, align, allocSize) != 0) {
        ptr = NULL;
    }
#endif

    if (ptr) {
        memset(ptr, 0, allocSize);
    }

    return ptr;
}

/*=========================================================================
 * Load Context
 *=========================================================================*/

/**
 * xe_GetLoadContext (no-args overload)
 *
 * Returns the current module's load context. On Xbox 360, this
 * provides information about the XEX module (base address, size, etc.).
 * On the host, this is a no-op.
 */
void xe_GetLoadContext(void) {
    /* No-op on host */
}

/*=========================================================================
 * CRT Fatal Exit
 *=========================================================================*/

/**
 * game_CrtFatalExit_thunk
 *
 * Called when the CRT encounters a fatal error that cannot be recovered.
 * On Xbox 360, this triggers a system-level crash handler.
 * On the host, we print an error and abort.
 */
void game_CrtFatalExit_thunk(void) {
    fprintf(stderr, "[CRT] Fatal exit — aborting\n");
    abort();
}
