/**
 * entry.c — Xbox 360 CRT entry point and thread context initialization
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * The game's startup sequence:
 *   __crt_main_entry  (XEX entry, called by the loader)
 *       └─ xe_main_thread_init_0038   (init main thread's XTF context)
 *           └─ xe_alloc_thread_ctx_6CA8 (allocate physical thread stack/TLS)
 *       └─ rage_main_6970             (RAGE main loop)
 *
 * The SDA base register (r13) points to a global struct that holds the
 * active memory allocator context at offset +4. xe_main_thread_init sets
 * up the main thread's XTF (Xbox Thread Frame) structure so that the heap
 * and TLS allocator are online before game code runs.
 */

#include "xe_tls_host.h"
#include <stdint.h>
#include <stddef.h>

/* Forward declarations */
void rage_main_6970(void* parms, void* base);
void atexit(void (*fn)(void));
static void xe_main_thread_cleanup(void);

/* Global SDA-relative allocator context base — offset +4 holds the
 * active sysMemAllocator vtable pointer. @ 0x82600000 (SDA base) */
extern uint32_t* g_sda_base;                    /* r13-relative */

/* Main thread XTF descriptor @ 0x8271B114 */
extern struct XeTlsBlock g_mainThreadXtf;

/* Allocator initialization flag — bit 0 indicates already-initialized.
 * Located at SDA-relative offset -20008 from g_sda_base upper half. */
extern uint32_t g_allocatorInitFlag;            /* @ ~0x8271XXXX */


/**
 * xe_alloc_thread_ctx_6CA8 @ 0x82186CA8 | size: 0x94
 *
 * Allocates a physical memory block for a thread's execution context
 * (stack + TLS scratch area) and initializes the XeTlsBlock descriptor.
 *
 * @param pXtf      Pointer to the XeTlsBlock to initialize
 * @param stackSize Requested stack size in bytes
 * @param flags     Thread flags (stored at XtfBlock+184)
 */
void xe_alloc_thread_ctx_6CA8(struct XeTlsBlock* pXtf, uint32_t stackSize, uint32_t flags)
{
    /* Round stack size up to next 64KB boundary: ((size + 65535) & ~65535) - 1 */
    uint32_t alignedSize = (stackSize + 0xFFFF) & 0xFFFF0000;

    /* Store thread flags and stack size into the XTF descriptor */
    pXtf->m_flags    = (uint8_t)flags;
    pXtf->m_stackSz  = stackSize;              /* +80 */

    /* Set bit 7 of byte at +192 (marks thread as having a physical alloc) */
    pXtf->m_physFlag = pXtf->m_physFlag | 0x80;

    /* Store vtable/type tag for the block  */
    pXtf->m_pVtable  = (void*)0x8241B00C;      /* crt allocator type tag */

    /* Allocate physically contiguous memory for the stack */
    void* pStack = xe_phys_alloc(alignedSize, /*align=*/0, /*flags=*/-1);
    if (!pStack) {
        /* Allocation failure — log and abort (nop_8240E6D0 = debug error log) */
        xe_log_alloc_failure(alignedSize >> 10);
        return;
    }

    /* Initialize the thread context with the allocated stack block */
    xe_thread_ctx_init(pXtf, pStack, alignedSize);
}


/**
 * xe_main_thread_init_0038 @ 0x820C0038 | size: 0x88
 *
 * Initializes the main thread's XTF (Xbox Thread Frame) if it hasn't been
 * set up yet. Checks an init flag to guard against double-init. Registers
 * xe_main_thread_cleanup() with atexit() so the TLS context is freed on exit.
 *
 * The SDA offset +4 holds a pointer to the active TLS block slot; this
 * function writes the main thread XTF address into slots +4 and +8.
 */
void xe_main_thread_init_0038(void)
{
    /* Load the allocator context base from SDA[0] */
    uint32_t* pAllocCtx = (uint32_t*)g_sda_base[0];

    /* Check if an XTF is already installed at slot +4 */
    bool alreadyInited = (pAllocCtx[1] != 0);   /* lwzx slot 4 bytes */

    if (!alreadyInited) {
        /* Check and set the global allocator init flag (bit 0) */
        if (!(g_allocatorInitFlag & 1)) {
            g_allocatorInitFlag |= 1;

            /* Allocate the main thread context: stack = 0x2C0000 (2.75 MB),
             * flags = 0 (main thread marker) */
            xe_alloc_thread_ctx_6CA8(&g_mainThreadXtf, 0x002C0000, 0);

            /* Register cleanup handler */
            atexit(xe_main_thread_cleanup);
        }

        /* Install the XTF descriptor into both TLS lookup slots */
        pAllocCtx[1] = (uint32_t)&g_mainThreadXtf;   /* slot +4  */
        pAllocCtx[2] = (uint32_t)&g_mainThreadXtf;   /* slot +8  */
    }
}


/**
 * __crt_main_entry @ 0x820C0128 | size: 0x44
 *
 * XEX entry point — called by the Xbox 360 kernel with the startup parms
 * block. Initializes the main thread context then hands off to rage_main.
 *
 * @param pStartupParms  Pointer to the XEX startup parameter block (r3)
 * @param pBase          Module base address passed through from kernel (r4)
 */
void __crt_main_entry(void* pStartupParms, void* pBase)
{
    xe_main_thread_init_0038();
    rage_main_6970(pStartupParms, pBase);
}
