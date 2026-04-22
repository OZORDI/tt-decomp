/**
 * stubs.cpp -- Consolidated stub implementations for all undefined linker symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Merged from: stubs_remaining.c, stubs_linker_1..5.cpp, stubs_linker_3b.cpp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 */

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
using std::atexit;

// ── Headers for class method stubs ──────────────────────────────────────────
// NOTE: Headers are carefully chosen to avoid redefinition conflicts.
// pong_misc.hpp and pong_player.hpp both define pongPlayer -- only include pong_misc.
// pong_audio.hpp requires rage_audio.hpp which may not exist -- forward-declare instead.
#include "game/misc/pong_misc.hpp"
#include "game/match/pong_states.hpp"
#include "game/pong_render.hpp"
#include "game/network/pong_network.hpp"
#include "game/ball/pong_ball.hpp"
#include "game/data/gd_data.hpp"
#include "game/creature/pong_creature.hpp"
#include "graphics/texture_reference.hpp"
#include "rage/rage_swf.hpp"
#include "physics/ph_physics.hpp"

// Audio effect classes (CDelayEffect/CPeakMeterEffect/CShelvingFilterEffect)
// have real class defs in src/game/audio/pong_audio.hpp and real impls in
// src/game/audio/pong_audio_effects.cpp + pong_audio.cpp.

// Forward declarations for types used by stubs
struct vec3;
struct pcrPostPointBlender;
struct pongCameraMgr;
struct TransitionParams;
struct TransitionFlags;
struct NetDataQuery;
struct pongAnimState;

// ============================================================================
// SECTION 1: C-linkage stubs (from stubs_remaining.c)
// ============================================================================

extern "C" {

// ── Xbox 360 Kernel Functions ───────────────────────────────────────────────
// HalReturnToFirmware, KeBugCheck, KeSetAffinityThread, ObDereferenceObject,
// ObReferenceObjectByHandle, RtlEnterCriticalSection, RtlLeaveCriticalSection
// → moved to src/xam/kernel_shims.c
// _KeTlsAlloc_thunk, KeTlsFree_stub, KeTlsSetValue_stub
// → moved to src/xam/kernel_shims.c

// ── Network Functions (Xbox 360 specific) ───────────────────────────────────
// NetDll_WSACleanup, NetDll_WSAStartup, NetDll_XNetStartup
// → moved to src/xam/xam_shims.c

// ── Video Functions ─────────────────────────────────────────────────────────
// XGetVideoMode → moved to src/xam/xam_shims.c

// ── CRT Functions ───────────────────────────────────────────────────────────
// __cinit_impl, _crt_spinlock_acquire, _crt_spinlock_release,
// _crt_tls_fiber_setup, _xe_strcpyn_10
// → moved to src/crt/crt_init.c

// ── Main Entry Point ────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    extern int rage_main_stub(int argc, char** argv);
    return rage_main_stub(argc, argv);
}

// ── Singleton Functions ─────────────────────────────────────────────────────
// atHashMap_Clear / atSingleton_Find / atSingleton_Remove — moved to
// src/rage/data/at_types.cpp.

void rage_GameLoopThreadEntry(void* singleton) {
    (void)singleton;
}

// ── Audio System Functions ──────────────────────────────────────────────────
// audSystem_Configure / audSystem_Shutdown — real impls moved to
// src/rage/audio/rage_audio.cpp (Agent P15 lift).

// ── Data Reference Functions ────────────────────────────────────────────────
// datRef_Release — moved to src/rage/data/at_types.cpp.

// ── File I/O Functions ──────────────────────────────────────────────────────

// _realloc_crt, _calloc_crt, _crt_fiber_destroy, _tls_dtor_cleanup,
// _crt_tls_callback → moved to src/crt/crt_init.c

void* fiDeviceMemory_AllocateDeviceMemory(size_t size) {
    (void)size;
    return nullptr;
}

void* fiDevice_GetDevice(const char* name) {
    (void)name;
    return nullptr;
}

void* fiStream_Open(const char* path, int mode) {
    (void)path; (void)mode;
    return nullptr;
}

// parStreamOutXml_vprintf — moved to src/rage/data/par_xml_types.cpp.

// ── Graphics Device Functions ───────────────────────────────────────────────
// grcDevice_shutdown/shutdownAlt — real impl in src/graphics/grc_device.cpp
// grcDevice_beginScene/clear     — real impl in src/graphics/grc_render.cpp
// grcDevice_Present              — no-op retained for host-side presentation
// grcDevice_SubInit / WaitFence  — thin stubs; referenced from app_init.c

#if defined(TT_PLATFORM_PC)
extern void sdl_PollAndSwap(void);
#endif

void grcDevice_Present(void) {
#if defined(TT_PLATFORM_PC)
    sdl_PollAndSwap();
#endif
}

void grcDevice_SubInit(void* device) { (void)device; }
void grcDevice_WaitFence(void) {}

void* grcDisplay_Create(void) { return nullptr; }

// ── HSM Functions ───────────────────────────────────────────────────────────
// hsmContext_SetNextState — lifted in src/rage/core/hsm.cpp

// ── Input Functions ─────────────────────────────────────────────────────────
// io_Input_poll — lifted in src/rage/core/io_input.cpp

// ── Network System Functions ────────────────────────────────────────────────

void netSystem_Shutdown(void) {}

// ── Debug/Logging Functions ─────────────────────────────────────────────────

void rage_debugLog_c(const char* fmt, ...) {
    (void)fmt;
}

// ── Physics Functions ───────────────────────────────────────────────────────

// phMaterialMgrImpl_UpdateActive lifted to src/physics/ph_physics.cpp.
void* phWorld_Construct(void) { return nullptr; }
void rage_strchr(void) {}

// ── Game Functions ──────────────────────────────────────────────────────────

// pongPostEffects_Create — lifted to src/game/pong_render.cpp (returns void*)
void contentManager_LoadUserContent(void) {}
// pg_EDE0_gen → src/rage/swf.cpp

// ── RAGE Engine Functions ───────────────────────────────────────────────────

void rage_AcquireReference(void* obj) { (void)obj; }

void* rage_BlockAlloc(size_t size) {
    (void)size;
    return nullptr;
}

// hsmContext_InitTimers — lifted in src/rage/core/hsm.cpp

void rage_DebugLog(const char* fmt, ...) {
    (void)fmt;
}

uint8_t rage_FindSingleton(void* ptr) {
    (void)ptr;
    return 0;
}

const char* rage_GetExecutableName(void) {
    return "RockstarTableTennis";
}

void rage_InitMainThreadHeap(void) {}
void rage_InitializeNetSystem(void) {}

void rage_RegisterThread(const char* name) { (void)name; }
void rage_RenderDebugOverlay(void) {}
void rage_SetRenderMode(int mode) { (void)mode; }

int rage_StringCompareNoCase(const char* a, const char* b) {
    (void)a; (void)b;
    return 0;
}

const char* rage_StringFindChar(const char* str, char c) {
    (void)str; (void)c;
    return nullptr;
}

void rage_ThreadPool_Cleanup(void) {}

void sysMemAllocator_Free(void* ptr) {
    extern void rage_free(void* ptr);
    rage_free(ptr);
}

// hsmContext_Init — lifted in src/rage/core/hsm.cpp

int rage_main_stub(int argc, char** argv) {
    (void)argc; (void)argv;
    std::printf("rage_main_stub - stub\n");
    return 0;
}

void* rage_malloc(size_t size) {
    (void)size;
    return nullptr;
}

// ── SWF (Scaleform) Functions → src/rage/swf.cpp ────────────────────────────

// ── Memory Allocator Functions ──────────────────────────────────────────────

void* sysMemAllocator_Alloc(size_t size) {
    (void)size;
    return nullptr;
}

// ── Utility Functions ───────────────────────────────────────────────────────

// game_CrtFatalExit_thunk → moved to src/crt/xe_tls.c

// XeTlsBlock_InitStack → moved to src/crt/xe_tls.c

// rage_parStructure_Init_stub — moved to src/rage/data/par_xml_types.cpp.

// ── XAM Functions ───────────────────────────────────────────────────────────
// xam_CreateEvent_c → moved to src/xam/xam_shims.c

// ── XE Functions ────────────────────────────────────────────────────────────

void grmShaderPreset_AllocArray_c(void) {}

void* rage_Alloc_c(uint32_t size) {
    (void)size;
    return nullptr;
}

// xe_main_thread_init → moved to src/crt/xe_tls.c

// XePhysicalAlloc_stub → moved to src/crt/xe_tls.c

// XeTlsBlock_BindMainThread → moved to src/crt/xe_tls.c

// ── XML Functions ───────────────────────────────────────────────────────────
// xmlNodeStruct_vfn_2 — moved to src/rage/data/par_xml_types.cpp.

// ── Render Target Functions ─────────────────────────────────────────────────
// CleanupRenderTargets / ConfigureRenderTargets / InitializeRenderConfig /
// SetupRenderFiber — real impls in src/rage/render_lifecycle.c

// ── Page Group Functions → src/rage/swf.cpp ─────────────────────────────────

// ── UI Event Functions → src/rage/swf.cpp ───────────────────────────────────

// ── Training Functions ──────────────────────────────────────────────────────
// SetTrainingState — lifted in src/rage/core/hsm.cpp

// ── Network Client Functions ────────────────────────────────────────────────
// SinglesNetworkClient_TickAll — lifted in src/game/network/pong_network.cpp

// ── Game Logic Functions ────────────────────────────────────────────────────
// gmLogic_StepFrame — lifted in src/rage/core/hsm.cpp

// ── Vtables ─────────────────────────────────────────────────────────────────

void* fsmMachine_vtable[16] = {0};
void* gameLoop_vtable_derived[16] = {0};
void* gameLoop_vtable_parent[16] = {0};
void* grcDisplay_vtable[16] = {0};
void* grcDisplay_vtable2[16] = {0};
void* hsmContext_vtable_init[16] = {0};
void* physicsWorld_vtable[16] = {0};
void* k_factoryVtable[16] = {0};
void* rage_datBase_vtable[16] = {0};

} // extern "C"


// ============================================================================
// SECTION 2: C++ stubs from stubs_linker.cpp (main branch)
// ============================================================================

// ── Physics / Scene ─────────────────────────────────────────────────────────

// CreatePhysicsInstance, CreateSimplePhysicsInstance, LinkPhysicsResource,
// ReleaseSceneObject, ResetBoundObject, ResetViewBound, pongDrawBucket_AddEntry
// lifted to src/physics/ph_physics.cpp.
extern "C" void phInst_SetMatrix_Impl(void* a, void* b, void* c, void* d) {
    (void)a; (void)b; (void)c; (void)d;
}
extern "C" void* phInst_BFB8_2hr(void* a) { (void)a; return nullptr; }
extern "C" void phInst_Cleanup(void* a) { (void)a; }
extern "C" float ph_Atan2(float y, float x) { (void)y; (void)x; return 0.0f; }
extern "C" float ph_Normalize(float x) { (void)x; return 0.0f; }
extern "C" void ke_DispatchPhysics(void* a) { (void)a; }
// grc_SetupResource: synthetic glue, no real address in binary. Link anchor.
extern "C" void grc_SetupResource(void* a, void* b) { (void)a; (void)b; }
extern "C" void msgMsgSink_Broadcast(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
// fragDrawable_0AA0_2h @ 0x820F0AA0 size:0xAC — SIMD-heavy (sp_wrong flag).
// Real impl pending deferred lift; this anchor keeps callers in rage_grm.cpp linking.
extern "C" void fragDrawable_0AA0_2h(void* a, void* b, uint32_t c, void* d) {
    (void)a; (void)b; (void)c; (void)d;
}

// ── Debug / Logging ─────────────────────────────────────────────────────────

extern "C" void DbgPrint(const char* fmt, ...) { (void)fmt; }

// ── UI / Page Groups → src/rage/swf.cpp ─────────────────────────────────────

extern "C" void SetupCharViewDisplay(void* a) { (void)a; }

// ── Game Data ───────────────────────────────────────────────────────────────

extern "C" int32_t FindCharacterByName(void* a, const char* b) { (void)a; (void)b; return 0; }
// GetStateContextName → src/rage/swf.cpp
extern "C" uint16_t LookupEffectId(const char* a) { (void)a; return 0; }
extern "C" int32_t util_2458_FindCharacterIndex(void* a, const char* b) { (void)a; (void)b; return 0; }

// ── Xbox 360 Kernel / XAM ───────────────────────────────────────────────────

// ── Xbox 360 Kernel / XAM ───────────────────────────────────────────────────
// KeResetEvent, KeSetEvent, KeWaitForSingleObject → already in src/xam/kernel.c
// XamInputGetState, XamInputSetState, XamShowGamerCardUIForXUID → already in src/xam/input.c
// XamLoaderTerminateTitle → already in src/xam/xam_stubs.c
// xam_CreateEvent → moved to src/xam/xam_shims.c
// ke_EnterCriticalSection, ke_LeaveCriticalSection → moved to src/xam/kernel_shims.c
// RtlMultiByteToUnicodeN_6FA8_w → already in src/xam/xam_stubs.c
// _check_xdk_version → already in src/crt/version.c
// _heap_init_impl → moved to src/crt/crt_init.c
// _stricmp → moved to src/crt/crt_init.c

// ── Network ─────────────────────────────────────────────────────────────────
// NetDll_recvfrom and NetDll_sendto are now implemented in xam/network.c
extern "C" void rage_atStringCopy(const char* src, char* dest, int maxSize) {
    if (dest && maxSize > 0) dest[0] = '\0';
    (void)src;
}
// SinglesNetworkClient_SetMode — lifted in src/game/network/pong_network.cpp

// ── Locomotion State / Animation ────────────────────────────────────────────
// Lifted to src/anim/locomotion.cpp (P10).

// ── Serialization / XML ─────────────────────────────────────────────────────
// RegisterSerializationField / RegisterSerializedField /
// xmlNodeStruct_BaseInitialize / xmlNodeStruct_SerializeField —
//   moved to src/rage/data/par_xml_types.cpp.

// ── atArray / atSingleton ───────────────────────────────────────────────────
// atArray_Destructor is address-backed in src/rage/memory/atArray.cpp.
// atFactory_GetFactory / atSingleton_CAD0_g / parStruct_SerializeMembers —
//   moved to src/rage/data/at_types.cpp and src/rage/data/par_xml_types.cpp.

// ── Audio ───────────────────────────────────────────────────────────────────
// xapo_UnpackBuffer — moved to src/rage/io/fiStreamBuf.cpp.

// ── CM (Control Machine) System — lifted to src/rage/animation/cm_shims.cpp ─

// ── File I/O / Tokenizer ────────────────────────────────────────────────────
// fiAsciiTokenizer_SetString / fiAsciiTokenizer_Process —
//   moved to src/rage/io/fiAsciiTokenizer.cpp.

// ── parStream ───────────────────────────────────────────────────────────────
// parStreamInXml_ReadFloat — moved to src/rage/data/par_xml_types.cpp.

// ── Game Logic ──────────────────────────────────────────────────────────────

extern "C" void game_FrameSyncSignal(void* a, int b) { (void)a; (void)b; }
// gmLogic_StepFrame_impl — lifted in src/rage/core/hsm.cpp
extern "C" void grmShaderPreset_AllocArray(uint32_t a) { (void)a; }

// ── Jump Tables / Dispatch ──────────────────────────────────────────────────

// cmOperator_SetLabel — lifted to src/rage/animation/cm_shims.cpp

// ── Page Group Internals → src/rage/swf.cpp ─────────────────────────────────

// ── Camera Manager — lifted to src/game/camera/pong_camera_helpers.cpp ─────

// ── Creature System ─────────────────────────────────────────────────────────

// pongCreature_BaseDtor / Fixup / UpdateReplay — moved to src/game/creature/pong_creature.cpp
extern "C" void pongCreatureInst_Cleanup(void* a) { (void)a; }
extern "C" float pongCreatureInst_ComputeHeading(void* a) { (void)a; return 0.0f; }
extern "C" void pongCreatureInst_NotifyHeadingChanged(void* a) { (void)a; }
extern "C" void pongCreatureInst_SetMatrixImpl(void* a, const void* b) { (void)a; (void)b; }

// ── Player System ───────────────────────────────────────────────────────────

extern "C" bool pongPlayer_CheckBallSplash(void* a) { (void)a; return false; }
extern "C" void pongPlayer_ClearShotState(void* a) { (void)a; }
extern "C" int pongPlayer_GetAnimIndex(void* a) { (void)a; return 0; }
extern "C" void pongPlayer_GetVelocity(void* a, float* b) {
    (void)a; if (b) { b[0] = 0.0f; b[1] = 0.0f; b[2] = 0.0f; }
}
extern "C" void pongPlayer_NetSync(void* a, int b) { (void)a; (void)b; }
extern "C" void pongPlayer_ProcessReplay(void* a) { (void)a; }
extern "C" void pongPlayer_ResetShotTiming(void* a, float b) { (void)a; (void)b; }
extern "C" void pongPlayer_ResetSwingImpl(void* a) { (void)a; }
extern "C" void pongPlayer_SetScoreSlot(void* a, int b) { (void)a; (void)b; }
// pongPlayer_StateHandler_{9188..91E8} — lifted to src/game/player/pong_player.cpp
extern "C" void pongPlayer_SyncFloat(void* a, void* b) { (void)a; (void)b; }
extern "C" void pongPlayer_SyncWord(void* a, void* b) { (void)a; (void)b; }

// ── RAGE Engine Core ────────────────────────────────────────────────────────

extern "C" void rage_AddRef(void* a) { (void)a; }
extern "C" void rage_Release(void* a) { (void)a; }
extern "C" void rage_ReleaseSingleton(void* a) { (void)a; }
// parStreamInXml_FindChildByName / parMember_Destructor —
//   moved to src/rage/data/par_xml_types.cpp.
extern "C" const void* rage_cmIntegrate_vtable = nullptr;
// fiStreamBuf_Read_stub / fiStreamBuf_Close_stub — moved to src/rage/io/fiStreamBuf.cpp.
// rage_datTypeFactory_Create_stub — moved to src/rage/data/par_xml_types.cpp.

// ── SWF / Scaleform → src/rage/swf.cpp ──────────────────────────────────────

// ── System / Memory ─────────────────────────────────────────────────────────

extern "C" void* sysMemAllocator_Allocate(void* a, size_t b) { (void)a; (void)b; return nullptr; }

extern "C" void sysMemAllocator_InitThreadHeap(void) {
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

// ── Utility Functions — cm* lifted to src/rage/animation/cm_shims.cpp ──────

// ph_snprintf — lifted into src/physics/ph_update_object.cpp.


// ============================================================================
// SECTION 3: C++ stubs from stubs_linker_0.cpp (main branch)
// ============================================================================

// ── BallHitMessage ──────────────────────────────────────────────────────────
// BallHitMessage_ctor_69C8 — lifted in src/game/network/pong_network.cpp

// CDelayEffect/CPeakMeterEffect/CShelvingFilterEffect:
// Real class defs in src/game/audio/pong_audio.hpp
// Real impls in pong_audio_effects.cpp and pong_audio.cpp

// ── Misc free functions ─────────────────────────────────────────────────────

// CheckButtonPressed — moved to src/game/char_view/char_view.cpp
void ComputeNetworkHash(void* a, int b) { (void)a; (void)b; }
// CopyVectorThreadSafe — moved to src/game/char_view/char_view.cpp
void DeserializeNetworkData(void* a, void* b, int c) { (void)a; (void)b; (void)c; }
// Dialog_IsComplete / DismissPageGroup / FadePageGroup / InitializePageGroup → src/rage/swf.cpp

void GetPlayerID(void* a, unsigned int b) { (void)a; (void)b; }
void HSM_QueueNotification(void* a) { (void)a; }

// ── LocomotionStateAnim members ─────────────────────────────────────────────
// Lifted to src/anim/locomotion.cpp (P10).

// ── Net / Page / Message ────────────────────────────────────────────────────

void NetDataQuery_InitNested(void* a) { (void)a; }
// PageGroup_GetTextEntry/LookupText/Register/SetState → src/rage/swf.cpp

// PlayerMovementMessage_54B0_h — lifted in src/game/network/pong_network.cpp

// PostPageGroupMessage (3 overloads) → src/rage/swf.cpp
void PostStateTransitionRequest(void* a, int b) { (void)a; (void)b; }

// QueueGameMessage — lifted in src/game/network/pong_network.cpp

void ReadBitsFromStream(void* a, void* b, int c) { (void)a; (void)b; (void)c; }

// RegisterSerializedField (const-qualified overload) — moved to
//   src/rage/data/par_xml_types.cpp.

void ResetShotTimingState(void* a) { (void)a; }

void* RtlEnterCriticalSection_D6F0_fw(void* a) { (void)a; return nullptr; }
void SendContextMessage(int a, int b, int c, int d) { (void)a; (void)b; (void)c; (void)d; }
void Player_ApplyServeStarted(void* a, void* b, void* c, float d) { (void)a; (void)b; (void)c; (void)d; }
// SetPageGroupVisible → src/rage/swf.cpp

// ── SinglesNetworkClient free-function stubs ────────────────────────────────

// snBitStream_Reset / snBitStream_WriteBits / rlEvent_Init / snBitStream_ReadSigned
// lifted to src/game/network/pong_network_io.cpp.
uint32_t netStream_WriteString(void* a, const char* b, uint32_t c) { (void)a; (void)b; (void)c; return 0; }
void NetworkClient_BeginJoinRequest(void* a) { (void)a; }
uint8_t NetworkClient_PollJoinResponse(void* a) { (void)a; return 0; }
void NetworkClient_InitInternalState(void* a) { (void)a; }
void NetworkClient_ResetLocalState(void* a) { (void)a; }
void NetworkClient_DispatchMessage(void* a, int b, void* c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
int NetworkClient_GetMessageId(void* a) { (void)a; return 0; }
void SinglesNetworkClient_4FB0_g(void* a) { (void)a; }
void* NetworkClient_LookupPlayer(uint8_t a) { (void)a; return nullptr; }
void NetworkClient_AbortMessageProcessing(void* a) { (void)a; }
void NetworkClient_EndMessageProcessing(void* a) { (void)a; }
bool NetworkClient_TryDequeueMessage(void* a, uint32_t* b) { (void)a; (void)b; return false; }
void netStream_WriteBool(void* a, bool b) { (void)a; (void)b; }
void NetworkClient_ReadQueuedMessageSource(void* a, uint8_t* b) { (void)a; (void)b; }
int SinglesNetworkClient_8CC0_w(void* a) { (void)a; return 0; }
void* snSession_FindProperty(void* a, const char* b) { (void)a; (void)b; return nullptr; }
void snSession_BeginOperation(void* a) { (void)a; }
uint8_t snSession_AcquireLock(void* a) { (void)a; return 0; }
void snSession_ReleaseLock(void* a) { (void)a; }

// SpectatorNetworkClient members — lifted to src/game/network/pong_network_classes.cpp

// ── Misc ────────────────────────────────────────────────────────────────────

// TextEntry_GetValue → src/rage/swf.cpp
void WriteBallHitDataToNetwork(void* a, void* b) { (void)a; (void)b; }
void WriteFloatToNetworkStream(void* a, float b) { (void)a; (void)b; }


// ============================================================================
// SECTION 4: C++ stubs from stubs_linker_1.cpp (function stubs only, globals moved)
// ============================================================================

void WriteInt8Bits(void* client, int16_t value, int bits) {
    (void)client; (void)value; (void)bits;
}

// ── atArray / atSingleton helpers ───────────────────────────────────────────
// atArray_Clear, atSingleton_0128_wrh, atSingleton_2038, atSingleton_23C0,
// atSingleton_29E8_p25, atSingleton_5CD0_fw, atSingleton_8E88_p42,
// atSingleton_QueryEventData, datArray_Grow —
//   moved to src/rage/data/at_types.cpp.
float math_SafeReciprocal(float a, double b) { (void)a; (void)b; return 0.0f; }
void* atSingletonPool_AllocEntry(uint32_t a) { (void)a; return nullptr; }

// ── Audio ───────────────────────────────────────────────────────────────────
// audControl_Destructor — lifted to src/rage/audio/rage_audio.cpp

// charViewCS virtual methods vfn_3/4/6/7/9/10/11 — moved to
// src/game/char_view/char_view.cpp where they're lifted with semantic names
// (RegisterXmlFields, Validate, GetName, GetVariantName, Update,
// RecalcBounds, PurgeFilteredNodes).

// ── CM (Control Machine) helpers — lifted to src/rage/animation/cm_shims.cpp

// ── Animation ───────────────────────────────────────────────────────────────
// Lifted to src/anim/locomotion.cpp (P10).

// ── File I/O / Tokenizer ────────────────────────────────────────────────────
// fiAsciiTokenizer_2628_g — moved to src/rage/io/fiAsciiTokenizer.cpp.

// ── Game logic functions ────────────────────────────────────────────────────

// DialogPageGroup_Construct → src/rage/swf.cpp
void game_2E80(void* a) { (void)a; }
void game_2EE0(void* a) { (void)a; }
void game_36E8(void* a, const float* b) { (void)a; (void)b; }
void game_3860(void* a, int b, const char* c) { (void)a; (void)b; (void)c; }
void game_8EE8(void* a) { (void)a; }
// LegalsPageGroup_Construct → src/rage/swf.cpp
void game_9CF8_h(void* a, uint32_t b) { (void)a; (void)b; }
void game_9D10_h(void* a, uint32_t b) { (void)a; (void)b; }
void game_9D28_h(void* a, uint32_t b) { (void)a; (void)b; }
void game_AA88(void* a) { (void)a; }
// CreditsRoll_Deactivate / CreditsRoll_NotifyEntry → src/rage/swf.cpp
void game_CD20(void* a) { (void)a; }
void game_D060(void* a) { (void)a; }
void game_D3B0_h(void* a) { (void)a; }
void game_D468(void* a, vec3* b) { (void)a; (void)b; }
void game_D500(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void game_DA60() {}

// ── Graphics ────────────────────────────────────────────────────────────────

// grc_2CC8 @ 0x82352CC8 size:0xF0 — graphics resource setup, pending lift.
// grc_EB10 @ 0x8214EB10 size:0x12C — device state save/restore, pending lift.
// Both are real symbols; these are link anchors until deferred lift lands.
void grc_2CC8(void* a, uint32_t b, uint32_t c) { (void)a; (void)b; (void)c; }
void grc_EB10(void* a) { (void)a; }


// ============================================================================
// SECTION 5: C++ stubs from stubs_linker_2.cpp
// ============================================================================

// ── HSM Context free functions ──────────────────────────────────────────────
// hsmContext_5B40_w, hsmContext_5BC8_fw — lifted in src/rage/core/hsm.cpp

// ── hsmState member functions ───────────────────────────────────────────────

void hsmState::GetFullStatePath(char* buffer, unsigned int bufferSize) const {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    hsmState* parent = *reinterpret_cast<hsmState* const*>(self + 0x08);
    if (parent) {
        parent->GetFullStatePath(buffer, bufferSize);
        unsigned int currentLen = 0;
        { const char* p = buffer; while (*p++) {} currentLen = static_cast<unsigned int>(p - buffer - 1); }
        if (currentLen >= bufferSize) return;
        typedef const char* (*GetNameFn)(const void*);
        GetNameFn getName = reinterpret_cast<GetNameFn>(vtable[2]);
        const char* name = getName(this);
        std::snprintf(buffer + currentLen, bufferSize - currentLen, "/%s", name);
        return;
    }
    typedef const char* (*GetNameFn)(const void*);
    GetNameFn getName = reinterpret_cast<GetNameFn>(vtable[2]);
    const char* name = getName(this);
    unsigned int remaining = bufferSize - 1;
    char* dest = buffer;
    while (remaining > 0 && *name) { *dest++ = *name++; --remaining; }
    *dest = '\0';
}

void hsmState::Reset() {
    typedef void (*OnExitFn)(void*);
    OnExitFn onExit = reinterpret_cast<OnExitFn>(vtable[6]);
    onExit(this);
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    *reinterpret_cast<uint32_t*>(self + 0x10) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x14) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x04) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x08) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x0C) = 0;
}

// ── HUD Flash free functions → src/rage/swf.cpp ─────────────────────────────

// ── IO free functions ───────────────────────────────────────────────────────
// io_ExecuteStateAction, io_Input_poll_9D68 — lifted in src/rage/core/io_input.cpp

// ── Jump table / kernel free functions ──────────────────────────────────────

void jumptable_3A48(void* a, unsigned int b, unsigned int c, unsigned int d) {
    (void)a; (void)b; (void)c; (void)d;
}
void jumptable_5C20(void* a) { (void)a; }
// snListNode_Init lifted to src/game/network/pong_network_io.cpp

// ── Motion clip free function ───────────────────────────────────────────────
// Lifted to src/anim/locomotion.cpp (P10).

// ── msgMsgSink member functions ─────────────────────────────────────────────

// ── P1/15 batch 1 methods lifted to src/game/network/msg_msg_sink.cpp ─────
// OnEnter, CheckAndProcess, PostLoadProperties, ScalarDtor, Validate,
// GetName, GetPeerDataSize, NotifySessionEvent, ProcessMessage,
// GetStateFlagsLocked, vfn_3, vfn_8, vfn_9, vfn_17, vfn_18, vfn_19,
// vfn_27, vfn_35, vfn_36, vfn_37, vfn_44, vfn_45
// (GenerateAndCleanup / GetConnectionInfo / UpdatePrioritiesLocked /
//  ApplyPrioritiesLocked / FindValidMessageSlot / SetPropertyLocked /
//  GetPropertyLocked / ProcessWithLock / SetMessageBuffer / GetStateFlags /
//  ProcessPendingMessages / CleanupIfReady / ForwardToMatchHandler /
//  DispatchEventDefault / ClearPointers — already lifted earlier)

void msgMsgSink::vfn_3() {}
void msgMsgSink::vfn_8() {} void msgMsgSink::vfn_9() {}
uint16_t msgMsgSink::GetPeerDataSize() { return 0; }
void msgMsgSink::NotifySessionEvent() {}
void msgMsgSink::vfn_17() {} void msgMsgSink::vfn_18() {} void msgMsgSink::vfn_19() {}
uint32_t msgMsgSink::ProcessMessage(uint32_t messageFlags) { (void)messageFlags; return 0; }
int32_t msgMsgSink::GetStateFlagsLocked(uint32_t* outFlags) { (void)outFlags; return 0; }
void msgMsgSink::vfn_27() {}
void msgMsgSink::vfn_35() {} void msgMsgSink::vfn_36() {} void msgMsgSink::vfn_37() {}
void msgMsgSink::vfn_44() {} void msgMsgSink::vfn_45() {}
// P2 batch 2 stubs lifted → src/game/network/msg_msg_sink.cpp

// msgMsgSink_3C88_g / 3D70_p39 / 4550_g / 4EB8_g lifted to
// src/game/network/pong_network_io.cpp

void sysCallback_Invoke(void* a, int b) { (void)a; (void)b; }
// xmlNodeStruct_Initialize — moved to src/rage/data/par_xml_types.cpp.


// ============================================================================
// SECTION 6: C++ stubs from stubs_linker_3.cpp
// ============================================================================

// msgMsgSink_5098_g / 6138_2hr / 6220_w / 84C0_gen / A970_2h / D168_sp / D1D8_sp /
// DB70_g / E860_g / F518_wrh + net_6BA0_fw lifted to
// src/game/network/pong_network_io.cpp

// ── rage_debugLog overloads (C++ mangled) ────────────────────────────────────

void rage_debugLog() {}
void rage_debugLog(const char* fmt, ...) { (void)fmt; }
void rage_debugLog(const void* ptr, ...) { (void)ptr; }

// ── Animation blending ──────────────────────────────────────────────────────
// Lifted to src/anim/locomotion.cpp (P10).

// ── Page Group helpers → src/rage/swf.cpp ───────────────────────────────────

// phArchetype_Load, phBoundCapsule_01D0_g/01D8_g/02B0_g/0FE0_g/5138_g/A080_g,
// phInst_A3A0_p33, phObject::vfn_1..31 lifted to src/physics/ph_physics.cpp.
extern "C" void* atHashMap_Find(void* table, const char* key);

// ── Physics utility functions ───────────────────────────────────────────────
// phArchetype_Find lifted to src/physics/ph_physics.cpp.

// plrPlayerMgr members — lifted to src/game/data/gd_data.cpp

// ── pongAttractState ────────────────────────────────────────────────────────
// pongAttractState::{Reset,Shutdown,GetName,ProcessInput} and the free
// function pongAttractState_Shutdown — moved to
// src/game/char_view/pong_attract_state_shims.cpp.

// ── pongBallInstance ────────────────────────────────────────────────────────
// vfn_2..vfn_30 stubs removed — methods are now declared with semantic names
// (GetMatrixAt60, SetMatrixAt60, GetCurrentPosition, GetCurrentVelocity,
// ProcessCollisionAndActivate, ValidateCollisionParams3) and implemented in
// pong_ball.cpp.
// pongBallInstance_4980_g — moved to src/game/ball/pong_ball.cpp

// ── pongCameraMgr helpers — lifted to src/game/camera/pong_camera_helpers.cpp


// ============================================================================
// SECTION 7: C++ stubs from stubs_linker_3b.cpp
// ============================================================================

// pongCharViewContext vfn_11..vfn_23 — moved to
// src/game/char_view/char_view.cpp (uses the definition in char_view.hpp).



// ============================================================================
// SECTION 8: C++ stubs from stubs_linker_4.cpp
// ============================================================================

// pongCreature / pongCreatureInst helpers (pongCreatureInst_RegisterPhysics,
// pongCreatureInst_EDC0_g, pongCreature_7CE8_g) — moved to
// src/game/creature/pong_creature.cpp

// pongDrawBucket::InitStatics / Load — moved to src/game/pong_render.cpp

// pongLerpQueue_3410_g — moved to src/game/pong_lerp_queue.cpp

// pongNetMessageHolder family (ScalarDtor, _5038_w, all *_wrh, _FAE0_isl,
// vfn_2_*) — moved to src/game/network/pong_network_classes.cpp

// ── pongPlayer ──────────────────────────────────────────────────────────────

// pongPlayer::Process(float) -- skipped, pong_misc.hpp declares Process() with no args
// pongPlayer_* free functions lifted to src/game/player/pong_player.cpp
// (0270_g, 1460_g, 5B60_gen, 6470_g, B208_g, BF18_g, C678_g, DE98_g,
//  FD20_g, ApplyNetStateImpl, GetPlayerPosition, InitShotImpl,
//  Interpolate, ResetMoverImpl, StateHandler_{9188..91E8})
// Already-lifted elsewhere (removed duplicates): 0508_g, D238_g, E590_g, E7B0_g

// pongScrnTransFadeIn_EndTransition — moved to src/game/pong_render.cpp

// ── namespace rage globals and stubs ────────────────────────────────────────

namespace rage {

void* Allocate(unsigned long size, unsigned long alignment) {
    (void)size; (void)alignment; return nullptr;
}

void _locale_register(void* a, void* b) { (void)a; (void)b; }
void atSingleton_8068_h(void* a) { (void)a; }
void atSingleton_8A48_p42(void* a) { (void)a; }
// aud_1498 / aud_6A20_wrap_6A20 — lifted to src/rage/audio/rage_audio.cpp (global scope)
void fiAsciiTokenizer_ReadToken(void* a, int b) { (void)a; (void)b; }
uint8_t fiAsciiTokenizer_ReadBool(void* a, void* b, void* c) { (void)a; (void)b; (void)c; return 0; }
void fiAsciiTokenizer_Destroy(void* a) { (void)a; }

// namespace rage globals
extern const float g_capsuleVolK1 = 1.5f;
extern const float g_capsuleVolK2 = 4.18879f;
uint8_t  g_contextStack[128] = {};
void*    g_currentGeometry = nullptr;
// g_currentSwfContext → src/rage/swf.cpp
uint32_t* g_iccProfilePtr = nullptr;
uint32_t  g_mmioStoredValue = 0;
void*    g_phAllocator = nullptr;
uint32_t g_phCollisionFlags = 0;
uint16_t g_phDefaultStatusId = 0;
void*    g_phGlobalState = nullptr;
// g_swfCallDepth / g_swfFrameScale / g_swfGlobalObject / g_swfGlobalScope /
// g_swfStringBuffer → src/rage/swf.cpp
void*    g_vtable_cmApproach2 = nullptr;
// g_vtable_swfSCRIPTOBJECT → src/rage/swf.cpp

// grcTextureReferenceBase virtual stubs
// vfn_21 / vfn_22 — real impls in src/graphics/texture_reference.cpp
// vfn_1/2/3/10/19/23/24/25 — pure forward-to-inner-texture thunks, kept here
//   as link anchors for the vtable slots that no concrete subclass overrides.
void grcTextureReferenceBase::vfn_1() {}
void grcTextureReferenceBase::vfn_10() {}
void grcTextureReferenceBase::vfn_19() {}
void grcTextureReferenceBase::vfn_2() {}
void grcTextureReferenceBase::vfn_23() {}
void grcTextureReferenceBase::vfn_24() {}

} // namespace rage


// ============================================================================
// SECTION 9: C++ stubs from stubs_linker_5.cpp
// ============================================================================

namespace rage {

void grcTextureReferenceBase::vfn_25() {}
void grcTextureReferenceBase::vfn_3() {}

// swfINSTANCE::vfn_7 → src/rage/swf.cpp

// phBoundOTGrid::Collide3D/Collide3DFull/Collide3DYExtent/CollideCapsule/
// CollideLineX/CollidePlaneXY/CollidePlaneXZ lifted to src/physics/ph_physics.cpp.

// hudFlashBase_9CA8_h → src/rage/swf.cpp

// ke_ConstructObject, ke_DestroyObjectA, ke_DestroyObjectB
// → moved to src/xam/kernel_shims.c

// phArticulatedCollider_UpdateJointTransforms/ProcessJoints/ProcessColliderState,
// phCollider_vfn_4, phJoint1Dof_AE38, phJoint1Dof_AFF8_p42, phJoint3Dof_0170_g,
// phJoint_1388, phJoint_PreSyncState, ph_1B78, ph_A330, ph_E1E8, ph_FE48, and
// ph_vt{3DB0,57D8,5A60,5A7C,5A84,5A8C,5B98}_* vtable thunks — lifted into
// src/physics/ph_update_object.cpp (namespace rage).

// rage::phBoundCapsule_01D0_g(phJoint3Dof*,float) and phBoundCapsule_6C28_fw
// lifted to src/physics/ph_physics.cpp (01D0_g is reconciled to the single
// fsqrt-tail form taking a float).

void phCollider_vfn_4(phArticulatedCollider* a) { (void)a; }
// phInst_5910_p39 was already lifted to src/physics/ph_physics.cpp.

void rage_1058(void* a) { (void)a; }
void rage_Alloc(int a, void* b) { (void)a; (void)b; }

// util_4628 — lifted into src/physics/ph_update_object.cpp (namespace rage).
void* util_B188(void* a, int b) { (void)a; (void)b; return nullptr; }
// snSession_AssociateConnections / snSession_ProcessPendingConnections lifted
// to src/game/network/pong_network_io.cpp (at global scope — original callers
// reach them via extern declarations without namespace qualification).
void hsmState_AttachChild(void* a, void* b) { (void)a; (void)b; }

} // namespace rage

// ── C-linkage / global scope free functions ─────────────────────────────────

void rage_27C0(void* a) { (void)a; }
void* rage_Allocate(unsigned int a) { (void)a; return nullptr; }
void rage_C1A8(void* a) { (void)a; }
// datResource_Load — moved to src/rage/data/at_types.cpp.
void rage_FF70(void* a) { (void)a; }
void rage_free(void* p) { free(p); }
void rage_threadpool_cleanup_6878() {}
void sgSceneGraph_FreeLists(void* a) { (void)a; }
// snHsmAcceptingJoinRequest_9A70 / snSession_Detach / snSession_AddNode lifted
// to src/game/network/pong_network_io.cpp

void sub_821A8F58(void* a, const char* b, void* c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// typeinfo for hsmState
namespace { struct FakeTypeInfo { void* vtable; const char* name; }; }
extern "C" const FakeTypeInfo _ZTI8hsmState = { nullptr, "hsmState" };

void util_03C0(void* a, uint32_t* b) { (void)a; (void)b; }
// netStream_ReadStringRaw / netStream_ReadBlock lifted to
// src/game/network/pong_network_io.cpp
int util_5A70(void* a, void* b, int c, int d, int e) { (void)a; (void)b; (void)c; (void)d; (void)e; return 0; }
void sysMemAllocator_PlatformFree(void* a, unsigned int b) { (void)a; (void)b; }
// netStream_ReadS16 / netStream_ReadS8SignMagnitude lifted to
// src/game/network/pong_network_io.cpp
void util_8FD0(void* a) { (void)a; }
// util_AA38 — lifted in src/game/network/pong_network.cpp
void util_B8A0(void* a) { (void)a; }
void util_C930(void* a) { (void)a; }
void util_CDF0(void* a, const void* b, unsigned int c) { (void)a; (void)b; (void)c; }
void util_D150(void* a, void* b) { (void)a; (void)b; }
void util_D150(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
// hsmEvent_Init — lifted in src/rage/core/hsm.cpp
void util_PackColorRGBA(uint32_t* a, const float* b) { (void)a; (void)b; }
// xam_singleton_init_8D60 → already in src/xam/static_init.c (xam_GetInitSingleton)

void* rage_Alloc(int a) { (void)a; return nullptr; }
void* rage_Alloc(unsigned int a) { (void)a; return nullptr; }
// xe_GetLoadContext (2 overloads) → moved to src/crt/xe_tls.c
// Note: C++ overloads need to stay here since xe_tls.c is C
void xe_GetLoadContext(void* a) { (void)a; }
void rage_AssertMainThread() {}
// xmlNodeStruct_Init / xml_ReadInt / xml_ReadString —
//   moved to src/rage/data/par_xml_types.cpp.

void rage_RegisterUIContext(void* a, unsigned b, const char* c) { (void)a; (void)b; (void)c; }
// fsmMachine_Destroy — real impl in src/rage/core/fsmMachine.c

// ── C-linkage wrappers for dual-linkage symbols ─────────────────────────────

extern "C" {
// _c_PostStateTransitionRequest — lifted in src/rage/core/hsm.cpp
// RtlEnterCriticalSection / RtlLeaveCriticalSection — C-linkage already
// provided by extern "C" definitions in Section 1
void _c_atArray_Clear(void* a) __asm__("_atArray_Clear");
void _c_atArray_Clear(void* a) { atArray_Clear(a); }
// audControl_Destructor lifted to src/rage/audio/rage_audio.cpp — forward-declare here.
extern void audControl_Destructor(void* pControl);
void _c_audControl_Destructor(void* o) __asm__("_audControl_Destructor");
void _c_audControl_Destructor(void* o) { audControl_Destructor(o); }
void _c_hsmContext_SetNextState(void* c, int s) __asm__("_hsmContext_SetNextState_2800");
void _c_hsmContext_SetNextState(void* c, int s) { hsmContext_SetNextState(c, s); }
// _c_pgPageGroup_DispatchEvent (_pg_6F68 alias) → src/rage/swf.cpp
void _c_atHashMap_Find(void* a, const void* b) __asm__("_atHashMap_Find");
void _c_atHashMap_Find(void* a, const void* b) {
    atHashMap_Find(a, static_cast<const char*>(b));
}
void _c_sysCallback_Invoke(void* c, int code) __asm__("_sysCallback_Invoke");
void _c_sysCallback_Invoke(void* c, int code) { sysCallback_Invoke(c, code); }
}

// ── dcamPolarCam vtable stubs (asm labels from globals_linker.cpp) ───────────
// Real addresses (camera state update/query functions — SIMD-heavy):
//   dcamPolarCam_vfn_6 @ 0x8227E1A8 size:0x160
//   dcamPolarCam_vfn_7 @ 0x823F77D0 size:0xAC
//   dcamPolarCam_vfn_8 @ 0x823F7B08 size:0x11C
//   dcamPolarCam_vfn_9 @ 0x8227DD98 size:0x124
// Kept as link anchors for dcam vtable emission; real lifts pending.

void dcamPolarCam_vfn_6_impl() __asm__("dcamPolarCam_vfn_6");
void dcamPolarCam_vfn_6_impl() {}
void dcamPolarCam_vfn_7_impl() __asm__("dcamPolarCam_vfn_7");
void dcamPolarCam_vfn_7_impl() {}
void dcamPolarCam_vfn_8_impl() __asm__("dcamPolarCam_vfn_8");
void dcamPolarCam_vfn_8_impl() {}
void dcamPolarCam_vfn_9_impl() __asm__("dcamPolarCam_vfn_9");
void dcamPolarCam_vfn_9_impl() {}

// ── namespace rage ReleaseSingleton ─────────────────────────────────────────

namespace rage {
    class atSingleton_rage;
    void ReleaseSingleton(atSingleton_rage*) {}
}

// ── pgPageGroup_DispatchEvent → src/rage/swf.cpp ────────────────────────────

// pongPlayer swing free functions — lifted to src/game/player/pong_player_swing.cpp
// (42E0_g, 73E8_g, AB48_g, 9CD0_g — real bodies already in pong_player_swing.cpp)

// ── pcrAnimBlender / pcrAnimState (from stubs_final.cpp) ────────────────────
// Lifted to src/anim/locomotion.cpp (P10).
