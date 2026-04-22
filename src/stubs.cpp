/**
 * stubs.cpp — Minimal residual stubs for linker resolution
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * This file contains ONLY items that cannot be moved to proper source files:
 *   - Entry points (main, rage_main_stub)
 *   - Vtable arrays (link anchors for generated vtable emissions)
 *   - C-linkage wrappers (asm-label aliases for dual-linkage symbols)
 *   - Namespace rage globals (physics, context, geometry state)
 *   - Complex init functions (sysMemAllocator_InitThreadHeap)
 *   - Deferred lifts (RtlEnterCriticalSection_D6F0_fw)
 *   - Simple no-ops for systems not yet active
 *
 * All game logic, player, creature, ball, network, physics, audio,
 * animation, rendering, and RAGE core stubs have been moved to their
 * proper source files. See git log for the migration.
 */

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
using std::atexit;

// ── Headers for C-linkage wrapper targets ───────────────────────────────────
#include "game/network/pong_network.hpp"

// Forward declarations for wrapper targets
extern void atArray_Clear(void* arr);
extern void audControl_Destructor(void* pControl);
extern void hsmContext_SetNextState(void* ctx, int state);
extern "C" void* atHashMap_Find(void* table, const char* key);
extern void sysCallback_Invoke(void* callback, int code);


// ============================================================================
// SECTION 1: Entry points
// ============================================================================

extern "C" {

int main(int argc, char** argv) {
    extern int rage_main_stub(int argc, char** argv);
    return rage_main_stub(argc, argv);
}

int rage_main_stub(int argc, char** argv) {
    (void)argc; (void)argv;
    std::printf("rage_main_stub — stub\n");
    return 0;
}


// ============================================================================
// SECTION 2: Vtable arrays (link anchors for vtable emissions)
// ============================================================================

void* fsmMachine_vtable[16] = {0};
void* gameLoop_vtable_derived[16] = {0};
void* gameLoop_vtable_parent[16] = {0};
void* grcDisplay_vtable[16] = {0};
void* grcDisplay_vtable2[16] = {0};
void* hsmContext_vtable_init[16] = {0};
void* physicsWorld_vtable[16] = {0};
void* k_factoryVtable[16] = {0};
void* rage_datBase_vtable[16] = {0};


// ============================================================================
// SECTION 3: Simple no-ops for systems not yet active
// ============================================================================

/** Content manager — user content loading not needed for menu boot */
void contentManager_LoadUserContent(void) {}

/** Frame sync signal — no-op in single-threaded mode */
void game_FrameSyncSignal(void* event, int signal) {
    (void)event; (void)signal;
}

/** Shader preset array allocation — no-op until rendering is active */
void grmShaderPreset_AllocArray(uint32_t count) { (void)count; }

/** Network system shutdown — no-op until networking is active */
void netSystem_Shutdown(void) {}


// ============================================================================
// SECTION 4: Complex init / deferred lifts
// ============================================================================

/**
 * sysMemAllocator_InitThreadHeap — initializes the main thread's
 * memory allocator context from the XeTlsBlock storage.
 */
void sysMemAllocator_InitThreadHeap(void) {
    extern void* g_pAllocatorBase;
    extern char g_mainThreadXtfStorage[256];
    extern uint32_t g_allocInitFlag;
    extern void xe_alloc_thread_ctx_6CA8(void* ctx, uint32_t heapSize, uint32_t flags);
    extern void xe_get_thread_ctx_36E8(void);

    void** base = (void**)g_pAllocatorBase;
    if (!base) return;
    if (base[1] != nullptr) return;
    void* xtfBlock = (void*)g_mainThreadXtfStorage;
    if (!(g_allocInitFlag & 1)) {
        g_allocInitFlag |= 1;
        xe_alloc_thread_ctx_6CA8(xtfBlock, 0x002C0000u, 0);
        atexit(xe_get_thread_ctx_36E8);
    }
    base[1] = xtfBlock;
    base[2] = xtfBlock;
}

/**
 * RtlEnterCriticalSection_D6F0_fw @ 0x8244D6F0 | size: 0x844
 * Complex critical section handler with switch table + vtable dispatch.
 * TODO: Full lift deferred (844 bytes).
 */
void* RtlEnterCriticalSection_D6F0_fw(void* critSection) {
    (void)critSection;
    return nullptr;
}

/** xe_GetLoadContext — C++ overload (C version in xe_tls.c) */
void xe_GetLoadContext(void* context) { (void)context; }


// ============================================================================
// SECTION 5: Utility stubs not referenced from other source files
// These are link targets for generated code only.
// ============================================================================

/** util_B188 @ 0x823FB188 | size: 0x64 — SWF context lookup */
void* util_B188(void* swfContext, int index) {
    (void)swfContext; (void)index;
    return nullptr;
}

/** util_5A70 @ 0x822E5A70 | size: 0x190 — file device operation */
int util_5A70(void* device, void* buffer, int offset, int size, int flags) {
    (void)device; (void)buffer; (void)offset; (void)size; (void)flags;
    return 0;
}

/** util_8FD0 is now in src/game/ball/pong_ball.cpp */
/** util_03C0 is now in src/game/pong_render.cpp */
/** util_D150 is now in src/game/ball/pong_ball.cpp */
/** util_CDF0 is now in src/anim/pcr_anim_blenders.cpp */
/** util_C930 is now in src/anim/pcr_anim_blenders.cpp */
/** util_B8A0 is now in src/anim/pcr_anim_blenders.cpp */
/** sub_821A8F58 is now in src/rage/data/at_types.cpp */
/** game_8EE8 is now in src/rage/memory/atSingleton.cpp */


// ============================================================================
// SECTION 6: typeinfo for hsmState
// ============================================================================

namespace { struct FakeTypeInfo { void* vtable; const char* name; }; }
const FakeTypeInfo _ZTI8hsmState = { nullptr, "hsmState" };


} // extern "C"


// ============================================================================
// SECTION 7: C-linkage wrappers for dual-linkage symbols
// These use __asm__ labels to provide C-mangled names for functions
// that have C++ implementations.
// ============================================================================

extern "C" {

void _c_atArray_Clear(void* a) __asm__("_atArray_Clear");
void _c_atArray_Clear(void* a) { atArray_Clear(a); }

void _c_audControl_Destructor(void* o) __asm__("_audControl_Destructor");
void _c_audControl_Destructor(void* o) { audControl_Destructor(o); }

void _c_hsmContext_SetNextState(void* c, int s) __asm__("_hsmContext_SetNextState_2800");
void _c_hsmContext_SetNextState(void* c, int s) { hsmContext_SetNextState(c, s); }

void _c_atHashMap_Find(void* a, const void* b) __asm__("_atHashMap_Find");
void _c_atHashMap_Find(void* a, const void* b) {
    atHashMap_Find(a, static_cast<const char*>(b));
}

void _c_sysCallback_Invoke(void* c, int code) __asm__("_sysCallback_Invoke");
void _c_sysCallback_Invoke(void* c, int code) { sysCallback_Invoke(c, code); }

} // extern "C"


// ============================================================================
// SECTION 8: Namespace rage globals
// These are global state variables used by the physics, rendering,
// and animation subsystems. They need to live in a single translation
// unit to avoid multiple-definition errors.
// ============================================================================

namespace rage {

extern const float g_capsuleVolK1 = 1.5f;
extern const float g_capsuleVolK2 = 4.18879f;
uint8_t  g_contextStack[128] = {};
void*    g_currentGeometry = nullptr;
uint32_t* g_iccProfilePtr = nullptr;
uint32_t  g_mmioStoredValue = 0;
void*    g_phAllocator = nullptr;
uint32_t g_phCollisionFlags = 0;
uint16_t g_phDefaultStatusId = 0;
void*    g_phGlobalState = nullptr;
void*    g_vtable_cmApproach2 = nullptr;

} // namespace rage
