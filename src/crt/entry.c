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

#include <stdint.h>
#include <stddef.h>

#if defined(TT_ENABLE_NATIVE_CRT_HOOKS)
int crt_hooks_register(void);
#endif

/* ── XeTlsBlock — Xbox 360 Thread Frame descriptor ──────────────────
 * Minimal struct definition matching the fields accessed by entry.c.
 * Full layout TBD as more thread-management functions are decompiled. */
struct XeTlsBlock {
    void*    m_pVtable;      /* +0x00 */
    void*    m_pStackAlign16; /* +0x04 */
    void*    m_pStackRaw;    /* +0x08 */
    uint8_t  m_padding0[68]; /* +0x0C */
    uint32_t m_stackSz;      /* +0x50 */
    uint8_t  m_padding1[103];/* +0x54 */
    uint8_t  m_flags;        /* +0xB8 */
    uint8_t  m_pad3[7];      /* +0xB9 */
    uint8_t  m_physFlag;     /* +0xC0 */
};

/* ── Forward declarations ──────────────────────────────────────────── */
int   rage_main_6970(void* parms, void* base);
int   atexit(void (*fn)(void));
void* xe_phys_alloc_6AC8(uint32_t sizeBytes, int32_t protectFlags, uint32_t alignment, uint32_t allocFlags);
void  util_7AE8(struct XeTlsBlock* pXtf, void* stackBase, uint32_t stackSize);
void  xe_thread_ctx_init_6D40(struct XeTlsBlock* pXtf);
void  nop_8240E6D0(const char* fmt, ...);

/* ── Globals ───────────────────────────────────────────────────────── */

/* SDA-relative allocator context base — offset +4 holds the
 * active sysMemAllocator vtable pointer. @ 0x82600000 (SDA base) */
extern uint32_t* g_sda_base;                    /* r13-relative */

/* Main thread XTF descriptor @ 0x8271B114 */
extern struct XeTlsBlock g_mainThreadXtf;

/* Allocator initialization flag — bit 0 indicates already-initialized. */
extern uint32_t g_allocatorInitFlag;            /* @ 0x8271B1D8 */

static const uintptr_t kThreadCtxCtorVtable = 0x82038C4Cu;
static const uintptr_t kThreadAllocFailFmt = 0x82038610u;
static const uint32_t kThreadPhysAllocFlags = 0x20000004u;


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
    /* PPC sequence: addis size,1; addi -1; rlwinm 0,0,15 */
    uint32_t alignedSize = (stackSize + 0xFFFFu) & 0xFFFF0000u;

    pXtf->m_flags = (uint8_t)flags;
    pXtf->m_pad3[0] = 0;
    pXtf->m_stackSz = stackSize;

    pXtf->m_physFlag = pXtf->m_physFlag | 0x80;
    pXtf->m_pVtable = (void*)kThreadCtxCtorVtable;

    void* stackBase = xe_phys_alloc_6AC8(alignedSize, -1, 0, kThreadPhysAllocFlags);
    if (stackBase == NULL) {
        nop_8240E6D0((const char*)kThreadAllocFailFmt, (int32_t)alignedSize >> 10);
    }

    /* The original always runs util_7AE8 even when allocation fails. */
    util_7AE8(pXtf, stackBase, alignedSize);
}

/**
 * xe_get_thread_ctx_36E8 @ 0x825836E8 | size: 0x0C
 *
 * atexit callback registered by xe_main_thread_init_0038. It forwards the
 * process-global main-thread context object to xe_thread_ctx_init_6D40.
 */
void xe_get_thread_ctx_36E8(void)
{
    xe_thread_ctx_init_6D40(&g_mainThreadXtf);
}


/**
 * xe_main_thread_init_0038 @ 0x820C0038 | size: 0x88
 *
 * Initializes the main thread's XTF (Xbox Thread Frame) if it hasn't been
 * set up yet. Checks an init flag to guard against double-init. Registers
 * xe_get_thread_ctx_36E8() with atexit() so the context is finalized on exit.
 *
 * The SDA offset +4 holds a pointer to the active TLS block slot; this
 * function writes the main thread XTF address into slots +4 and +8.
 */
void xe_main_thread_init_0038(void)
{
    uint32_t* allocCtx = (uint32_t*)(uintptr_t)g_sda_base[0];
    if (allocCtx[1] == 0u) {
        if ((g_allocatorInitFlag & 1u) == 0u) {
            g_allocatorInitFlag |= 1u;
            xe_alloc_thread_ctx_6CA8(&g_mainThreadXtf, 0x002C0000u, 0u);
            atexit(xe_get_thread_ctx_36E8);
        }

        allocCtx[1] = (uint32_t)(uintptr_t)&g_mainThreadXtf;
        allocCtx[2] = (uint32_t)(uintptr_t)&g_mainThreadXtf;
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
#if defined(TT_ENABLE_NATIVE_CRT_HOOKS)
    static int s_crtHooksInstalled = 0;
    if (!s_crtHooksInstalled) {
        crt_hooks_register();
        s_crtHooksInstalled = 1;
    }
#endif

    xe_main_thread_init_0038();
    (void)rage_main_6970(pStartupParms, pBase);
}
