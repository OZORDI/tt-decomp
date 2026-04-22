/**
 * rage_core_utils.cpp — RAGE engine core utility function implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the RAGE memory allocation wrappers, debug logging, string
 * utilities, thread management stubs, and miscellaneous core helpers.
 * These are synthetic link-time symbols that bridge the recompiled binary
 * to the host platform's standard library.
 *
 * On the original Xbox 360, these functions dispatched through the RAGE
 * sysMemAllocator vtable and TLS-based thread heaps. On the host platform,
 * we route them through standard malloc/free with 16-byte alignment to
 * match PowerPC ABI requirements.
 */

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
// Forward declarations for functions defined elsewhere
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_free(void* ptr);

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 1: Memory Allocation
//
// On Xbox 360, RAGE used a custom heap (sysMemSimpleAllocator) with TLS-based
// per-thread allocators accessed via r13 (SDA base). On the host platform,
// we use standard malloc/free with posix_memalign for alignment guarantees.
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_malloc — General-purpose RAGE memory allocation
 *
 * On Xbox 360, this loaded the current thread's sysMemAllocator from the
 * SDA (r13+0) and called its Allocate virtual method. On the host, we
 * use standard malloc. All RAGE allocations are at least 16-byte aligned
 * on PowerPC, so we use aligned_alloc where possible.
 */
extern "C" void* rage_malloc(size_t size) {
    if (size == 0) return nullptr;
    // Round up to 16-byte alignment to match PPC ABI
    size_t aligned = (size + 15) & ~(size_t)15;
    void* ptr = malloc(aligned);
    if (ptr) memset(ptr, 0, aligned);
    return ptr;
}

/**
 * rage_BlockAlloc — Block allocator for fixed-size pool allocations
 *
 * Used by RAGE's pool allocator system for small, fixed-size objects
 * (singleton entries, message nodes, etc.). On the host, equivalent
 * to rage_malloc since we don't need pool optimization.
 */
extern "C" void* rage_BlockAlloc(size_t size) {
    return rage_malloc(size);
}

/**
 * sysMemAllocator_Alloc — Allocate from the system memory allocator
 *
 * Thin wrapper that routes to rage_malloc. On Xbox 360, this went through
 * the TLS allocator's vtable slot 1 (Allocate).
 */
extern "C" void* sysMemAllocator_Alloc(size_t size) {
    return rage_malloc(size);
}

/**
 * sysMemAllocator_Allocate — Allocate with alignment hint
 *
 * Parameters:
 *   a — alignment hint (cast from pointer, used as alignment value)
 *   b — size in bytes
 *
 * On Xbox 360, this checked if the current thread owned the allocator
 * (vtable slot 17), then either called the allocator's Allocate (slot 1)
 * or fell back to a manual alignment wrapper around xe_EC88 (malloc).
 * See sysMemAllocator_Allocate_61A0 @ 0x821861A0.
 */
extern "C" void* sysMemAllocator_Allocate(void* a, size_t b) {
    size_t alignment = (size_t)(uintptr_t)a;
    if (alignment < 16) alignment = 16;

    // Allocate with space for alignment and bookkeeping
    size_t total = b + alignment + sizeof(void*);
    void* raw = malloc(total);
    if (!raw) return nullptr;
    memset(raw, 0, total);

    // Align the returned pointer
    uintptr_t rawAddr = (uintptr_t)raw;
    uintptr_t aligned = (rawAddr + alignment + sizeof(void*)) & ~(alignment - 1);

    // Store the raw pointer just before the aligned pointer for free()
    ((void**)aligned)[-1] = raw;
    return (void*)aligned;
}

/**
 * sysMemAllocator_PlatformFree — Free with platform-specific flags
 *
 * On Xbox 360 @ 0x82566C20, this checked bit 31 of the flags parameter.
 * If set, it called atSingleton_6B58_p39 (a ref-counted release).
 * If clear, it called ke_89F8 (kernel free).
 * On the host, we just free the memory.
 */
extern "C" void sysMemAllocator_PlatformFree(void* a, unsigned int b) {
    (void)b;
    if (a) {
        free(a);
    }
}

/**
 * rage_free — Free memory allocated by RAGE allocators
 *
 * On Xbox 360 @ 0x820C00C0 (rage_free_00C0), this checked if the pointer
 * was tracked by the singleton pool (atSingleton_Find_90D0), and if not,
 * freed it via the global allocator's vtable slot 2 (Free).
 * On the host, we use standard free().
 *
 * NOTE: This is already defined in stubs.cpp as: void rage_free(void* p) { free(p); }
 * We do NOT redefine it here to avoid duplicate symbols.
 */

/**
 * rage_Alloc_c — C-linkage allocation wrapper
 *
 * Simple wrapper used by C-linkage callers that need RAGE heap memory.
 */
extern "C" void* rage_Alloc_c(uint32_t size) {
    return rage_malloc((size_t)size);
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 2: Debug Logging
//
// On Xbox 360, rage_DebugLog and rage_debugLog_c were compiled-out debug
// printf functions (just a blr/return in retail). We implement them as
// printf wrappers for development visibility.
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_DebugLog — RAGE engine debug log (C linkage)
 *
 * In the retail Xbox 360 binary, this was compiled to a single blr (return).
 * We implement it as a printf wrapper for development debugging.
 */
extern "C" void rage_DebugLog(const char* fmt, ...) {
    if (!fmt) return;
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\n");
}

/**
 * rage_debugLog_c — C-linkage debug log variant
 *
 * Same as rage_DebugLog but with a different mangled name.
 * Used by C-compiled translation units.
 */
extern "C" void rage_debugLog_c(const char* fmt, ...) {
    if (!fmt) return;
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\n");
}

/**
 * DbgPrint — Low-level debug print (Xbox 360 kernel debug output)
 *
 * On Xbox 360, this wrote to the kernel debug output channel.
 * On the host, we route to printf.
 */
extern "C" void DbgPrint(const char* fmt, ...) {
    if (!fmt) return;
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\n");
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 3: String Utilities
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_StringCompareNoCase — Case-insensitive string comparison
 *
 * Equivalent to _stricmp / strcasecmp. Returns <0, 0, or >0.
 */
extern "C" int rage_StringCompareNoCase(const char* a, const char* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;

    while (*a && *b) {
        int ca = std::tolower((unsigned char)*a);
        int cb = std::tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++;
        b++;
    }
    return std::tolower((unsigned char)*a) - std::tolower((unsigned char)*b);
}

/**
 * rage_StringFindChar — Find first occurrence of character in string
 *
 * Wrapper around strchr. Returns pointer to first occurrence of c in str,
 * or nullptr if not found.
 */
extern "C" const char* rage_StringFindChar(const char* str, char c) {
    if (!str) return nullptr;
    return std::strchr(str, c);
}

/**
 * rage_atStringCopy — Safe string copy with size limit
 *
 * Copies src to dest, ensuring null termination within maxSize bytes.
 * Equivalent to strncpy + forced null terminator.
 */
extern "C" void rage_atStringCopy(const char* src, char* dest, int maxSize) {
    if (!dest || maxSize <= 0) return;
    if (!src) {
        dest[0] = '\0';
        return;
    }
    int i = 0;
    for (; i < maxSize - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 4: Reference Counting & Singleton Management
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_AcquireReference — Increment reference count on a RAGE object
 *
 * On Xbox 360, RAGE objects have a uint16_t reference count at offset +0
 * (or +6 depending on the object type). This increments it.
 * For the host platform stub, this is a no-op since we don't track
 * reference counts in the same way.
 */
extern "C" void rage_AcquireReference(void* obj) {
    if (!obj) return;
    // On Xbox 360, this incremented a uint16_t ref count.
    // Host platform: no-op (we rely on standard C++ lifetime management)
}

/**
 * rage_AddRef — Add a reference to a RAGE-managed object
 *
 * Similar to rage_AcquireReference but used in different call sites.
 */
extern "C" void rage_AddRef(void* a) {
    if (!a) return;
    // No-op on host — reference counting not needed for host allocations
}

/**
 * rage_Release — Release a reference to a RAGE-managed object
 *
 * Decrements reference count. If it reaches zero, the object is freed.
 * On the host, this is a no-op since we don't track ref counts.
 */
extern "C" void rage_Release(void* a) {
    (void)a;
    // No-op on host — ref counting handled by standard C++ lifetime
}

/**
 * rage_ReleaseSingleton — Release a singleton instance
 *
 * Called during shutdown to release singleton objects from the global
 * singleton registry.
 */
extern "C" void rage_ReleaseSingleton(void* a) {
    (void)a;
    // No-op on host — singletons are managed differently
}

/**
 * rage_FindSingleton — Check if a pointer is tracked in the singleton pool
 *
 * On Xbox 360 @ 0x820F90D0 (atSingleton_Find_90D0), this walked the
 * global singleton hash table to check if a pointer was registered.
 * Returns 1 if found, 0 if not.
 *
 * On the host, we always return 0 (not found) since we don't maintain
 * the singleton registry.
 */
extern "C" uint8_t rage_FindSingleton(void* ptr) {
    (void)ptr;
    return 0;
}

/**
 * atSingletonPool_AllocEntry — Allocate an entry from the singleton pool
 *
 * On Xbox 360, this allocated from a pre-allocated pool of fixed-size
 * singleton entries. On the host, we use standard malloc.
 */
extern "C" void* atSingletonPool_AllocEntry(uint32_t a) {
    (void)a;
    // Allocate a generic singleton entry (typically 12 bytes: vtable + data + next)
    void* entry = rage_malloc(64);
    return entry;
}

/**
 * sysCallback_Invoke — Invoke a system callback
 *
 * On Xbox 360, this loaded a function pointer from the callback object
 * and called it with the given code. The callback object layout:
 *   +0: parameter (void*)
 *   +4: function pointer
 */
extern "C" void sysCallback_Invoke(void* a, int b) {
    if (!a) return;
    // Callback object: +0 = param, +4 = function pointer
    void* param = *(void**)a;
    typedef void (*CallbackFn)(void*);
    CallbackFn fn = *(CallbackFn*)((uint8_t*)a + sizeof(void*));
    if (fn) {
        fn(param);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 5: System Initialization & Thread Management
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_GetExecutableName — Return the game executable name
 *
 * Used by the RAGE engine for logging and crash reports.
 */
extern "C" const char* rage_GetExecutableName(void) {
    return "RockstarTableTennis";
}

/**
 * rage_InitMainThreadHeap — Initialize the main thread's heap allocator
 *
 * On Xbox 360, this set up the TLS-based per-thread allocator context
 * for the main thread. On the host, malloc is already thread-safe,
 * so this is a no-op.
 */
extern "C" void rage_InitMainThreadHeap(void) {
    // Host platform: standard malloc is already initialized
}

/**
 * rage_InitializeNetSystem — Initialize the RAGE networking subsystem
 *
 * On Xbox 360, this called XNetStartup and WSAStartup.
 * On the host, networking is stubbed out.
 */
extern "C" void rage_InitializeNetSystem(void) {
    // Networking stubbed for host platform
}

/**
 * rage_RegisterThread — Register a thread with the RAGE thread manager
 *
 * On Xbox 360, this registered the calling thread's name and ID with
 * the global thread registry for debugging and TLS management.
 */
extern "C" void rage_RegisterThread(const char* name) {
    if (name) {
        std::printf("[RAGE] Thread registered: %s\n", name);
    }
}

/**
 * rage_AssertMainThread — Assert that the current code is running on the main thread
 *
 * On Xbox 360, this compared the current thread ID against the stored
 * main thread ID and triggered a debug break if they didn't match.
 * On the host, this is a no-op since we're single-threaded for now.
 */
extern "C" void rage_AssertMainThread() {
    // No-op on host — single-threaded execution
}

/**
 * rage_GameLoopThreadEntry — Entry point for the game loop thread
 *
 * On Xbox 360, this was the thread function for the main game loop
 * thread, receiving the game singleton as its parameter.
 * On the host, the game loop runs on the main thread.
 */
extern "C" void rage_GameLoopThreadEntry(void* singleton) {
    (void)singleton;
    // Host platform: game loop runs on main thread via SDL event loop
}

/**
 * rage_SetRenderMode — Set the current rendering mode
 *
 * Controls whether the renderer is in normal, wireframe, debug, etc. mode.
 */
extern "C" void rage_SetRenderMode(int mode) {
    (void)mode;
    // TODO: implement when renderer is more complete
}

/**
 * rage_RenderDebugOverlay — Render debug overlay information
 *
 * Draws FPS counter, memory stats, and other debug info on screen.
 * No-op until the renderer supports text drawing.
 */
extern "C" void rage_RenderDebugOverlay(void) {
    // No-op until debug text rendering is implemented
}

/**
 * rage_ThreadPool_Cleanup — Clean up the global thread pool
 *
 * On Xbox 360, this walked the thread pool linked list and terminated
 * all worker threads. On the host, we're single-threaded.
 */
extern "C" void rage_ThreadPool_Cleanup(void) {
    // No-op on host — no thread pool to clean up
}

/**
 * rage_threadpool_cleanup_6878 — Per-node thread pool cleanup
 *
 * On Xbox 360 @ 0x82176878, this was a complex function that:
 *   1. Checked thread ownership via SDA globals
 *   2. Freed hash table entries and their values
 *   3. Decremented global allocation counters
 *
 * On the host, this is a no-op since we don't maintain the thread pool.
 */
extern "C" void rage_threadpool_cleanup_6878() {
    // No-op on host
}

/**
 * rage_RegisterUIContext — Register a UI context with the RAGE UI system
 *
 * Associates a UI context pointer with a name and flags for the
 * Scaleform/SWF UI system.
 */
extern "C" void rage_RegisterUIContext(void* a, unsigned b, const char* c) {
    (void)a; (void)b;
    if (c) {
        std::printf("[RAGE] UI context registered: %s\n", c);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 6: Math Utilities
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * math_SafeReciprocal — Compute 1/a with fallback
 *
 * Returns 1.0/a if a is non-zero, otherwise returns (float)b as fallback.
 * Used in physics and animation to avoid division by zero.
 */
extern "C" float math_SafeReciprocal(float a, double b) {
    if (a == 0.0f) return (float)b;
    return 1.0f / a;
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 7: Namespace rage functions
// ═══════════════════════════════════════════════════════════════════════════════

namespace rage {

/**
 * rage::Allocate — Allocate memory with alignment
 *
 * Primary allocation function in the rage namespace. Routes through
 * the global sysMemAllocator with the specified alignment.
 */
void* Allocate(unsigned long size, unsigned long alignment) {
    if (size == 0) return nullptr;
    if (alignment < 16) alignment = 16;

    // Use the same aligned allocation strategy as sysMemAllocator_Allocate
    size_t total = size + alignment + sizeof(void*);
    void* raw = malloc(total);
    if (!raw) return nullptr;
    memset(raw, 0, total);

    uintptr_t rawAddr = (uintptr_t)raw;
    uintptr_t aligned = (rawAddr + alignment + sizeof(void*)) & ~(alignment - 1);
    ((void**)aligned)[-1] = raw;
    return (void*)aligned;
}

/**
 * rage::_locale_register — Register a locale with the RAGE locale system
 *
 * Part of the localization subsystem. Registers a locale descriptor
 * for text lookup.
 */
void _locale_register(void* a, void* b) {
    (void)a; (void)b;
    // Localization stub — not needed for menu boot
}

/**
 * rage::ReleaseSingleton — Release a typed singleton instance
 */
class atSingleton_rage;
void ReleaseSingleton(atSingleton_rage* a) {
    (void)a;
    // No-op on host
}

} // namespace rage

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 8: Destructor / Cleanup Functions (address-backed)
//
// These functions have real addresses in the binary and perform object
// destruction. They are complex enough that full decompilation is deferred,
// but we provide functional stubs that free memory correctly.
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_1058 @ 0x82291058 | size: 0x74
 *
 * Destructor for a RAGE object with multiple sub-allocations.
 * Sets vtable, then frees pointers at offsets +112, +120, +160, +116, +124.
 * Finally resets vtable to base class.
 *
 * On the host, we free the sub-allocations if they're non-null.
 */
namespace rage {
void rage_1058(void* a) {
    if (!a) return;
    uint8_t* obj = (uint8_t*)a;

    // Free sub-allocations at known offsets
    void* p120 = *(void**)(obj + 120);
    void* p112 = *(void**)(obj + 112);
    void* p160 = *(void**)(obj + 160);
    void* p116 = *(void**)(obj + 116);
    void* p124 = *(void**)(obj + 124);

    if (p120) rage_free(p120);
    if (p112) rage_free(p112);
    if (p160) rage_free(p160);
    if (p116) rage_free(p116);
    if (p124) rage_free(p124);
}
} // namespace rage

/**
 * rage_27C0 @ 0x823C27C0 | size: 0x78
 *
 * Destructor for a message holder object. Frees the sub-object at +8
 * (after resetting its vtable), then decrements a global reference counter
 * at 0x820766A4.
 */
extern "C" void rage_27C0(void* a) {
    if (!a) return;
    uint8_t* obj = (uint8_t*)a;

    // Free sub-object at offset +8 if present
    void* subObj = *(void**)(obj + 8);
    if (subObj) {
        rage_free(subObj);
        *(void**)(obj + 8) = nullptr;
    }
}

/**
 * rage_C1A8 @ 0x8244C1A8 | size: 0x7C
 *
 * Adds a node to a doubly-linked list rooted at offsets +204/+208 of the
 * parent object. The node's vtable is set from parameter r4, and it's
 * inserted at the tail of the list. The list count at +212 is incremented.
 *
 * This is a linked-list insertion used by the message sink system.
 */
extern "C" void rage_C1A8(void* a) {
    (void)a;
    // Complex linked-list insertion — no-op stub for now
    // TODO: implement when message sink system is fully lifted
}

/**
 * rage_FF70 @ 0x820DFF70 | size: 0xD4
 *
 * Destructor for a LocomotionStateAnim object. Resets fields, frees
 * dynamic arrays at offsets +160 and +168 (with -4 offset for array
 * metadata prefix), clears size fields, then calls util_F870 for
 * final cleanup.
 */
extern "C" void rage_FF70(void* a) {
    if (!a) return;
    uint8_t* obj = (uint8_t*)a;

    // Clear state fields
    *(uint32_t*)(obj + 60) = 0;
    *(uint32_t*)(obj + 320) = 0;
    *(uint32_t*)(obj + 324) = 0;

    // Free dynamic array at +160 (data pointer, metadata at -4)
    void* arr1 = *(void**)(obj + 160);
    if (arr1) {
        void* rawArr1 = (uint8_t*)arr1 - 4;
        rage_free(rawArr1);
    }
    *(void**)(obj + 160) = nullptr;
    *(uint16_t*)(obj + 164) = 0;
    *(uint16_t*)(obj + 166) = 0;

    // Free dynamic array at +168
    void* arr2 = *(void**)(obj + 168);
    if (arr2) {
        void* rawArr2 = (uint8_t*)arr2 - 4;
        rage_free(rawArr2);
    }
    *(void**)(obj + 168) = nullptr;
    *(uint16_t*)(obj + 172) = 0;
    *(uint16_t*)(obj + 174) = 0;
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 9: C++ debug log overloads (mangled names)
//
// These are the C++ mangled versions of rage_debugLog that exist in the
// binary as compiled-out debug functions (just blr). We provide printf
// wrappers for development.
// ═══════════════════════════════════════════════════════════════════════════════

// NOTE: These are defined in stubs.cpp currently. We will remove them from
// stubs.cpp and keep them here.

// The no-arg overload
void rage_debugLog() {
    // No-op (matches retail binary)
}

// The const char* overload
void rage_debugLog(const char* fmt, ...) {
    if (!fmt) return;
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
    std::printf("\n");
}

// The void* overload (used for binary data logging)
void rage_debugLog(const void* ptr, ...) {
    (void)ptr;
    // No-op — binary data logging not needed on host
}

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 10: Namespace rage singleton/destructor helpers
// ═══════════════════════════════════════════════════════════════════════════════

namespace rage {

/**
 * rage::atSingleton_8068_h @ 0x82408068 | size: 0xF8
 *
 * Destructor for a complex singleton object. Walks an array at +40,
 * decrementing reference counts on each element and freeing those that
 * reach zero. Then frees the array itself (+40), the object at +44,
 * and arrays at +24 and +28.
 */
void atSingleton_8068_h(void* a) {
    if (!a) return;
    uint8_t* obj = (uint8_t*)a;

    // Free array at +40 if present
    void* arr = *(void**)(obj + 40);
    if (arr) {
        rage_free((uint8_t*)arr - 4);  // Array metadata at -4
    }

    // Free object at +44
    void* p44 = *(void**)(obj + 44);
    if (p44) rage_free(p44);

    // Free arrays at +24 and +28
    void* p24 = *(void**)(obj + 24);
    if (p24) rage_free(p24);

    void* p28 = *(void**)(obj + 28);
    if (p28) rage_free(p28);
}

/**
 * rage::atSingleton_8A48_p42 @ 0x82468A48 | size: 0x58
 *
 * Constructor/initializer for a multi-vtable singleton object.
 * Sets three vtable pointers at offsets +0, +4, and +16, then
 * calls atSingleton_0148_p39 on the sub-object at +4.
 */
void atSingleton_8A48_p42(void* a) {
    if (!a) return;
    // Initialize vtable pointers (on host, these are no-ops since
    // we don't use the original vtable dispatch)
    // The sub-object initialization at +4 would set up internal state
}

} // namespace rage

// ═══════════════════════════════════════════════════════════════════════════════
// SECTION 11: Vtable data
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * rage_cmIntegrate_vtable — Vtable pointer for rage::cmIntegrate
 *
 * On Xbox 360, this pointed to the vtable at 0x82054934.
 * On the host, we set it to nullptr since virtual dispatch is handled
 * by the C++ compiler.
 */
extern "C" const void* rage_cmIntegrate_vtable = nullptr;


// ============================================================================
// SECTION: Utility function implementations (lifted from stubs.cpp)
// ============================================================================

/**
 * util_PackColorRGBA — Pack float RGBA color into a uint32_t
 *
 * Converts a 4-component float color (RGBA, each in [0.0, 1.0]) into
 * a packed 32-bit RGBA value (8 bits per channel).
 *
 * Layout: 0xRRGGBBAA (R in high byte, A in low byte)
 *
 * This is used by the rendering system to convert shader-friendly float
 * colors into hardware-friendly packed formats for vertex buffers and
 * render state.
 */
extern "C" void util_PackColorRGBA(uint32_t* out, const float* rgba) {
    if (!out || !rgba)
        return;

    // Clamp each channel to [0, 1] and scale to [0, 255]
    auto clampByte = [](float v) -> uint8_t {
        if (v <= 0.0f) return 0;
        if (v >= 1.0f) return 255;
        return static_cast<uint8_t>(v * 255.0f + 0.5f);
    };

    uint8_t r = clampByte(rgba[0]);
    uint8_t g = clampByte(rgba[1]);
    uint8_t b = clampByte(rgba[2]);
    uint8_t a = clampByte(rgba[3]);

    // Pack as RGBA (R in high byte)
    *out = (static_cast<uint32_t>(r) << 24) |
           (static_cast<uint32_t>(g) << 16) |
           (static_cast<uint32_t>(b) << 8)  |
           (static_cast<uint32_t>(a));
}

/**
 * math_SafeReciprocal — Safe reciprocal with fallback
 *
 * Returns 1.0f / a if a is non-zero, otherwise returns (float)b as fallback.
 * Used in physics and animation to avoid division by zero.
 */
extern "C" float math_SafeReciprocal(float a, double b) {
    if (a == 0.0f)
        return static_cast<float>(b);
    return 1.0f / a;
}
