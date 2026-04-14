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

// Forward declarations for audio effect classes (pong_audio.hpp needs rage_audio.hpp)
struct CDelayEffect {
    virtual void CalcInputFrames();
    virtual void GetRegistrationProperties();
    virtual void IsInputFormatSupported();
    virtual void LockForProcess();
    virtual void Process();
};
struct CPeakMeterEffect {
    virtual void CalcInputFrames();
    virtual void GetRegistrationProperties();
    virtual void IsInputFormatSupported();
    virtual void LockForProcess();
    virtual void Process();
    virtual void UnlockForProcess();
};
struct CShelvingFilterEffect {
    virtual void CalcInputFrames();
    virtual void LockForProcess();
};

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

void HalReturnToFirmware(int mode) {
    std::printf("HalReturnToFirmware(%d) - stub\n", mode);
}

void KeBugCheck(uint32_t code) {
    std::printf("KeBugCheck(0x%08X) - stub\n", code);
}

void KeSetAffinityThread(void* thread, uint32_t affinity) {
    (void)thread; (void)affinity;
}

void ObDereferenceObject(void* obj) {
    (void)obj;
}

int ObReferenceObjectByHandle(void* handle, void* type, void** obj) {
    (void)handle; (void)type; (void)obj;
    return 0;
}

void RtlEnterCriticalSection(void* cs) {
    (void)cs;
}

void RtlLeaveCriticalSection(void* cs) {
    (void)cs;
}

int _KeTlsAlloc_thunk(void* destructorThunk) {
    (void)destructorThunk;
    return 0;
}

int ke_KeTlsFree_624C(uint32_t tlsIndex) {
    (void)tlsIndex;
    return 0;
}

int ke_KeTlsSetValue_622C(uint32_t tlsIndex, void* value) {
    (void)tlsIndex; (void)value;
    return 0;
}

// ── Network Functions (Xbox 360 specific) ───────────────────────────────────

int NetDll_WSACleanup(void) {
    return 0;
}

int NetDll_WSAStartup(uint16_t version, void* data) {
    (void)version; (void)data;
    return 0;
}

int NetDll_XNetStartup(void* params) {
    (void)params;
    return 0;
}

// ── Video Functions ─────────────────────────────────────────────────────────

void XGetVideoMode(void* mode) {
    (void)mode;
}

// ── CRT Functions ───────────────────────────────────────────────────────────

int __cinit_impl(void) {
    return 1;
}

void* __imp_ExLoadedCommandLine = nullptr;
void* __imp_ExThreadObjectType = nullptr;

void _crt_spinlock_acquire(int id) {
    (void)id;
}

void _crt_spinlock_release(int id) {
    (void)id;
}

void _crt_tls_fiber_setup(void) {}

void _xe_strcpyn_10(char* dest, const char* src, size_t n) {
    (void)dest; (void)src; (void)n;
}

// ── Main Entry Point ────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    extern int rage_main_6970(int argc, char** argv);
    return rage_main_6970(argc, argv);
}

// ── Singleton Functions ─────────────────────────────────────────────────────

void atHashMap_Clear(void* map) {
    (void)map;
}

void rage_GameLoopThreadEntry(void* singleton) {
    (void)singleton;
}

void atSingleton_Find(void* ptr) {
    (void)ptr;
}

void atSingleton_Remove(void* singleton) {
    (void)singleton;
}

// ── Audio System Functions ──────────────────────────────────────────────────

void audSystem_Configure(void* config) {
    (void)config;
}

void audSystem_Shutdown(void) {}

// ── Data Reference Functions ────────────────────────────────────────────────

void datRef_Release(void* ref) {
    (void)ref;
}

// ── File I/O Functions ──────────────────────────────────────────────────────

void* _realloc_crt(size_t newSz, size_t minSz) {
    (void)newSz; (void)minSz;
    return nullptr;
}

void* _calloc_crt(int count, int size) {
    (void)count; (void)size;
    return nullptr;
}

void _crt_fiber_destroy(void) {}

void fiAsciiTokenizer_InitializeTokenizer(void* tokenizer) {
    (void)tokenizer;
}

void _tls_dtor_cleanup(void) {}

void _crt_tls_callback(int initFlag) {
    (void)initFlag;
}

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

void parStreamOutXml_3E40(void* stream, const char* format, ...) {
    (void)stream; (void)format;
}

// ── Graphics Device Functions ───────────────────────────────────────────────

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
void grcDevice_beginScene(void) {}

void grcDevice_clear(uint32_t flags, uint32_t color, float depth, uint8_t stencil) {
    (void)flags; (void)color; (void)depth; (void)stencil;
}

void grcDevice_shutdown(void) {}
void grcDevice_shutdownAlt(void) {}

void* grcDisplay_Create(void) { return nullptr; }

// ── HSM Functions ───────────────────────────────────────────────────────────

void hsmContext_SetNextState(void* context, int state) {
    (void)context; (void)state;
}

// ── Input Functions ─────────────────────────────────────────────────────────

void io_Input_poll(void) {}

// ── Network System Functions ────────────────────────────────────────────────

void netSystem_Shutdown(void) {}

// ── Debug/Logging Functions ─────────────────────────────────────────────────

void rage_debugLog_c(const char* fmt, ...) {
    (void)fmt;
}

// ── Physics Functions ───────────────────────────────────────────────────────

void phMaterialMgrImpl_UpdateActive(void* mgr) { (void)mgr; }
void* phWorld_Construct(void) { return nullptr; }
void rage_strchr(void) {}

// ── Game Functions ──────────────────────────────────────────────────────────

void pongPostEffects_Create(void* effects) { (void)effects; }
void contentManager_LoadUserContent(void) {}
void pg_EDE0_gen(void) {}

// ── RAGE Engine Functions ───────────────────────────────────────────────────

void rage_AcquireReference(void* obj) { (void)obj; }

void* rage_BlockAlloc(size_t size) {
    (void)size;
    return nullptr;
}

void hsmContext_InitTimers(void) {}

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

void hsmContext_Init(void* obj) { (void)obj; }

int rage_main_6970(int argc, char** argv) {
    (void)argc; (void)argv;
    std::printf("rage_main_6970 - stub\n");
    return 0;
}

void* rage_malloc(size_t size) {
    (void)size;
    return nullptr;
}

// ── SWF (Scaleform) Functions ───────────────────────────────────────────────

void swfContext_Cleanup(void* context) { (void)context; }
void swfFile_DestroyResources(void* file) { (void)file; }
void swfInstance_Cleanup(void* instance) { (void)instance; }

// ── Memory Allocator Functions ──────────────────────────────────────────────

void* sysMemAllocator_Alloc(size_t size) {
    (void)size;
    return nullptr;
}

// ── Utility Functions ───────────────────────────────────────────────────────

void thunk_game_C330(void) {}

void util_7AE8(void* pXtf, void* stackBase, uint32_t stackSize) {
    (void)pXtf; (void)stackBase; (void)stackSize;
}

void util_CE30(void) {}

// ── XAM Functions ───────────────────────────────────────────────────────────

void xam_CreateEvent_c(void) {}

// ── XE Functions ────────────────────────────────────────────────────────────

void grmShaderPreset_AllocArray_c(void) {}

void* rage_Alloc_c(uint32_t size) {
    (void)size;
    return nullptr;
}

void xe_main_thread_init(void) {}

void* xe_phys_alloc_6AC8(uint32_t sizeBytes, int32_t protectFlags,
                          uint32_t alignment, uint32_t allocFlags) {
    (void)sizeBytes; (void)protectFlags; (void)alignment; (void)allocFlags;
    return nullptr;
}

void xe_thread_ctx_init_6D40(void* ctx) { (void)ctx; }

// ── XML Functions ───────────────────────────────────────────────────────────

void xmlNodeStruct_vfn_2(void* node) { (void)node; }

// ── Render Target Functions ─────────────────────────────────────────────────

void CleanupRenderTargets(void) {}
void ConfigureRenderTargets(void) {}
void InitializeRenderConfig(void) {}
void SetupRenderFiber(void) {}

// ── Page Group Functions ────────────────────────────────────────────────────

void DestroyPageGroup(void* group) { (void)group; }
int GetPageGroupState(void* group) { (void)group; return 0; }
void ProcessPageGroupInput(void* group) { (void)group; }
void UpdatePageGroup(void* group) { (void)group; }

// ── UI Event Functions ──────────────────────────────────────────────────────

void NotifyUIEvent(int event) { (void)event; }

// ── Training Functions ──────────────────────────────────────────────────────

void SetTrainingState(int state) { (void)state; }

// ── Network Client Functions ────────────────────────────────────────────────

void SinglesNetworkClient_TickAll(void) {}

// ── Game Logic Functions ────────────────────────────────────────────────────

void gmLogic_StepFrame(void) {}

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

extern "C" void* CreatePhysicsInstance(void* a, void* b, void* c) {
    (void)a; (void)b; (void)c; return nullptr;
}
extern "C" void* CreateSimplePhysicsInstance(void* a, void* b) {
    (void)a; (void)b; return nullptr;
}
extern "C" void LinkPhysicsResource(void* a, void* b) { (void)a; (void)b; }
extern "C" void ReleaseSceneObject(void* a) { (void)a; }
extern "C" void ResetBoundObject(void* a, int b) { (void)a; (void)b; }
extern "C" void ResetViewBound(void* a, int b) { (void)a; (void)b; }
extern "C" void pongDrawBucket_AddEntry(void* a, void* b, void* c) {
    (void)a; (void)b; (void)c;
}
extern "C" void phInst_8F10(void* a, void* b, void* c, void* d) {
    (void)a; (void)b; (void)c; (void)d;
}
extern "C" void* phInst_BFB8_2hr(void* a) { (void)a; return nullptr; }
extern "C" void phInst_Cleanup(void* a) { (void)a; }
extern "C" float ph_Atan2(float y, float x) { (void)y; (void)x; return 0.0f; }
extern "C" float ph_Normalize(float x) { (void)x; return 0.0f; }
extern "C" void ke_DispatchPhysics(void* a) { (void)a; }
extern "C" void grc_SetupResource(void* a, void* b) { (void)a; (void)b; }
extern "C" void msgMsgSink_Broadcast(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
extern "C" void fragDrawable_0AA0_2h(void* a, void* b, uint32_t c, void* d) {
    (void)a; (void)b; (void)c; (void)d;
}

// ── Debug / Logging ─────────────────────────────────────────────────────────

extern "C" void DbgPrint(const char* fmt, ...) { (void)fmt; }

// ── UI / Page Groups ────────────────────────────────────────────────────────

extern "C" void FadePageGroup(void* a, float b, uint32_t c, uint32_t d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
extern "C" void FadePageGroupOut(void* a, float b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
extern "C" void PostPageGroupMessage(int a, int b, int c, int d) {
    (void)a; (void)b; (void)c; (void)d;
}
extern "C" void SetupCharViewDisplay(void* a) { (void)a; }

// ── Game Data ───────────────────────────────────────────────────────────────

extern "C" int32_t FindCharacterByName(void* a, const char* b) { (void)a; (void)b; return 0; }
extern "C" void* GetStateContextName(void* a) { (void)a; return nullptr; }
extern "C" uint16_t LookupEffectId(const char* a) { (void)a; return 0; }
extern "C" int32_t util_2458_FindCharacterIndex(void* a, const char* b) { (void)a; (void)b; return 0; }

// ── Xbox 360 Kernel / XAM ───────────────────────────────────────────────────

extern "C" int KeResetEvent(void* a) { (void)a; return 0; }
extern "C" int KeSetEvent(void* a, int b, int c) { (void)a; (void)b; (void)c; return 0; }
extern "C" int KeWaitForSingleObject(void* a, int b, int c, int d, void* e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return 0;
}
extern "C" int XamInputGetState(int a, int b, void* c) { (void)a; (void)b; (void)c; return 0; }
extern "C" int XamInputSetState(int a, void* b) { (void)a; (void)b; return 0; }
extern "C" int XamShowGamerCardUIForXUID(int a, uint64_t b) { (void)a; (void)b; return 0; }
extern "C" void XamLoaderTerminateTitle(void) {}
extern "C" uint32_t xam_CreateEvent(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    (void)a; (void)b; (void)c; (void)d; return 0;
}
extern "C" void ke_EnterCriticalSection(uint32_t a) { (void)a; }
extern "C" void ke_LeaveCriticalSection(uint32_t a) { (void)a; }
extern "C" int RtlMultiByteToUnicodeN_6FA8_w(void* a, uint32_t b, uint32_t* c, const char* d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return 0;
}
extern "C" int _check_xdk_version(void) { return 0; }
extern "C" int _heap_init_impl(void) { return 0; }
extern "C" int _stricmp(const char* a, const char* b) { (void)a; (void)b; return 0; }

// ── Network ─────────────────────────────────────────────────────────────────
// NetDll_recvfrom and NetDll_sendto are now implemented in xam/network.c
extern "C" void rage_atStringCopy(const char* src, char* dest, int maxSize) {
    if (dest && maxSize > 0) dest[0] = '\0';
    (void)src;
}
extern "C" void SinglesNetworkClient_SetMode(void* a, int b) { (void)a; (void)b; }

// ── Locomotion State / Animation ────────────────────────────────────────────

extern "C" void LocomotionStateAnim_AccumNode(void* a, void* b) { (void)a; (void)b; }
extern "C" void LocomotionStateAnim_ApplySpeed(void* a) { (void)a; }
extern "C" void LocomotionStateAnim_ComputeOffset(void* a, void* b) { (void)a; (void)b; }
extern "C" void* LocomotionStateAnim_FindAnim(void* a) { (void)a; return nullptr; }
extern "C" void LocomotionStateAnim_GrowArray(void* a) { (void)a; }
extern "C" void LocomotionStateAnim_InitCapsuleImpl(void* a, float b, const void* c) { (void)a; (void)b; (void)c; }
extern "C" void LocomotionStateAnim_ProcessNode(void* a, void* b) { (void)a; (void)b; }
extern "C" void LocomotionStateAnim_SetMatrixImpl(void* a, const void* b) { (void)a; (void)b; }
extern "C" void LocomotionStateAnim_SetSpeed(void* a, float b) { (void)a; (void)b; }
extern "C" void LocomotionStateMf_ApplyConstraints(void* a) { (void)a; }
extern "C" void LocomotionStateMf_BlendPoses(void* a) { (void)a; }
extern "C" void LocomotionStateMf_Finish(void* a) { (void)a; }
extern "C" void LocomotionStateMf_ScaleWeights(void* a) { (void)a; }
extern "C" void LocomotionStateMf_UpdateTransforms(void* a) { (void)a; }
extern "C" void* LocomotionState_FindFirstActive(void* a) { (void)a; return nullptr; }

// ── Serialization / XML ─────────────────────────────────────────────────────

extern "C" void RegisterSerializationField(void* a, const char* b, void* c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
extern "C" void RegisterSerializedField(void* a, const char* b, void* c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
extern "C" void xmlNodeStruct_BaseInitialize(void* a) { (void)a; }
extern "C" void xmlNodeStruct_SerializeField(void* a, const char* b, void* c, void* d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// ── atArray / atSingleton ───────────────────────────────────────────────────

extern "C" void* atArray_Destructor(void* a) { (void)a; return nullptr; }
extern "C" void atFactory_GetFactory(void* a) { (void)a; }
extern "C" void* atSingleton_CAD0_g(void* a) { (void)a; return nullptr; }
extern "C" void parStruct_SerializeMembers(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }

// ── Audio ───────────────────────────────────────────────────────────────────

extern "C" void xapo_UnpackBuffer(void* a, void* b) { (void)a; (void)b; }

// ── CM (Control Machine) System ─────────────────────────────────────────────

extern "C" float cmAngle_Normalize(float a) { (void)a; return 0.0f; }
extern "C" float cmApproach2_ComputeFactor(void* a) { (void)a; return 0.0f; }
extern "C" uint8_t cmNode_GetBoolValue(void* a) { (void)a; return 0; }
extern "C" uint32_t cmNode_GetDimValue(void* a) { (void)a; return 0; }
extern "C" bool cmNode_TryConnect3(void* a, void* b) { (void)a; (void)b; return false; }
extern "C" bool cmNode_TryConnectSingle(void* a, void* b) { (void)a; (void)b; return false; }
extern "C" void cmNormalizedTimer_Allocate(void* a) { (void)a; }

extern "C" void cmPort_CopyToBuffer(void* a, void* b) { (void)a; (void)b; }
extern "C" void cmPort_SyncValue(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
extern "C" void cmPowerApproach_Step(float* out, float a, float b, float c) {
    if (out) *out = 0.0f; (void)a; (void)b; (void)c;
}
extern "C" void cmReporter_Init(void* a) { (void)a; }
extern "C" int cmSwitch_4B60(void* a) { (void)a; return 0; }
extern "C" float cmVec4_Atan2(float* a, float* b) { (void)a; (void)b; return 0.0f; }

// ── File I/O / Tokenizer ────────────────────────────────────────────────────

extern "C" void fiAsciiTokenizer_SetString(void* a, const char* b, const char* c) { (void)a; (void)b; (void)c; }
extern "C" void fiAsciiTokenizer_Process(void* a, int b) { (void)a; (void)b; }

// ── parStream ───────────────────────────────────────────────────────────────

extern "C" float parStreamInXml_ReadFloat(void* a) { (void)a; return 0.0f; }

// ── Game Logic ──────────────────────────────────────────────────────────────

extern "C" void game_7868(void* a, int b) { (void)a; (void)b; }
extern "C" void gmLogic_StepFrame_impl(void* a) { (void)a; }
extern "C" void grmShaderPreset_AllocArray(uint32_t a) { (void)a; }

// ── Jump Tables / Dispatch ──────────────────────────────────────────────────

extern "C" void cmOperator_SetLabel(void* a, const char* b, const char* c, uint32_t d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}


// ── Page Group Internals ────────────────────────────────────────────────────

extern "C" void pg_6C40_g(void* a) { (void)a; }
extern "C" void pgBase_AcquireRef(void* a, int b) { (void)a; (void)b; }

// ── Camera Manager ──────────────────────────────────────────────────────────

extern "C" bool pongCameraMgr_FloatThresholdA(void* a, float b) { (void)a; (void)b; return false; }
extern "C" bool pongCameraMgr_FloatThresholdB(void* a) { (void)a; return false; }
extern "C" bool pongCameraMgr_QueryA(void* a, int b) { (void)a; (void)b; return false; }
extern "C" bool pongCameraMgr_QueryB(void* a, int b) { (void)a; (void)b; return false; }
extern "C" bool pongCameraMgr_QueryC(void* a, int b) { (void)a; (void)b; return false; }
extern "C" void pongCameraMgr_SetCameraByIndex(void* a, int b) { (void)a; (void)b; }

// ── Creature System ─────────────────────────────────────────────────────────

extern "C" void pongCreature_BaseDtor(void* a) { (void)a; }
extern "C" void pongCreature_Fixup(void* a, void* b) { (void)a; (void)b; }
extern "C" void pongCreature_UpdateReplay(void* a) { (void)a; }
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
extern "C" void pongPlayer_StateHandler_9188(void* a) { (void)a; }
extern "C" void pongPlayer_StateHandler_9198(void* a) { (void)a; }
extern "C" void pongPlayer_StateHandler_91A8(void* a) { (void)a; }
extern "C" void pongPlayer_StateHandler_91B8(void* a) { (void)a; }
extern "C" void pongPlayer_StateHandler_91C8(void* a) { (void)a; }
extern "C" void pongPlayer_StateHandler_91D8(void* a) { (void)a; }
extern "C" void pongPlayer_StateHandler_91E8(void* a) { (void)a; }
extern "C" void pongPlayer_SyncFloat(void* a, void* b) { (void)a; (void)b; }
extern "C" void pongPlayer_SyncWord(void* a, void* b) { (void)a; (void)b; }

// ── RAGE Engine Core ────────────────────────────────────────────────────────

extern "C" void rage_AddRef(void* a) { (void)a; }
extern "C" void rage_Release(void* a) { (void)a; }
extern "C" void rage_ReleaseSingleton(void* a) { (void)a; }
extern "C" void* parStreamInXml_FindChildByName(const void* a, const char* b) { (void)a; (void)b; return nullptr; }
extern "C" void parMember_Destructor(void) {}
extern "C" const void* rage_cmIntegrate_vtable = nullptr;
extern "C" void rage_obj_bind_3828(void* a, void* b, uint32_t c) { (void)a; (void)b; (void)c; }
extern "C" void* rage_obj_factory_create_3040(void* a, const char* b, int c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return nullptr;
}
extern "C" void rage_obj_finalize_3B38(void* a) { (void)a; }

// ── SWF / Scaleform ─────────────────────────────────────────────────────────

extern "C" float swfComputeFramePosition(void* a, void* b, float* c, float* d) {
    (void)a; (void)b; if (c) *c = 0.0f; if (d) *d = 0.0f; return 0.0f;
}
extern "C" const char* swfInternString(const char* a, void* b, int c) {
    (void)b; (void)c; return a;
}
extern "C" void swfPopResult(void* a) { (void)a; }
extern "C" void swfSymtab_Delete(void* a, const char* b) { (void)a; (void)b; }
extern "C" void swfSymtab_Enumerate(void* a) { (void)a; }
extern "C" void swfSymtab_Insert(void* a, const char* b, void* c) { (void)a; (void)b; (void)c; }
extern "C" void* swfSymtab_Lookup(void* a, const char* b) { (void)a; (void)b; return nullptr; }

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

// ── Utility Functions ───────────────────────────────────────────────────────

extern "C" float cmOperator_EvalFloat(void* a) { (void)a; return 0.0f; }
extern "C" int cmNode_GetInt(void* a) { (void)a; return 0; }
extern "C" void cmNode_GetVector(void* a, void* b) { (void)a; (void)b; }

// CRT snprintf overload (C++ mangled)
void ph_snprintf(char* buf, int size, const char* fmt, const char* str, int val) {
    if (buf && size > 0) std::snprintf(buf, (size_t)size, fmt, str, val);
}


// ============================================================================
// SECTION 3: C++ stubs from stubs_linker_0.cpp (main branch)
// ============================================================================

// ── BallHitMessage ──────────────────────────────────────────────────────────

void BallHitMessage_ctor_69C8(void* memory) { (void)memory; }

// ── CDelayEffect / CPeakMeterEffect / CShelvingFilterEffect ─────────────────

void CDelayEffect::CalcInputFrames() {}
void CDelayEffect::GetRegistrationProperties() {}
void CDelayEffect::IsInputFormatSupported() {}
void CDelayEffect::LockForProcess() {}
void CDelayEffect::Process() {}

void CPeakMeterEffect::CalcInputFrames() {}
void CPeakMeterEffect::GetRegistrationProperties() {}
void CPeakMeterEffect::IsInputFormatSupported() {}
void CPeakMeterEffect::LockForProcess() {}
void CPeakMeterEffect::Process() {}
void CPeakMeterEffect::UnlockForProcess() {}

void CShelvingFilterEffect::CalcInputFrames() {}
void CShelvingFilterEffect::LockForProcess() {}

// ── Misc free functions ─────────────────────────────────────────────────────

bool CheckButtonPressed(void* a) { (void)a; return false; }
void ComputeNetworkHash(void* a, int b) { (void)a; (void)b; }
void CopyVectorThreadSafe(void* a, void* b) { (void)a; (void)b; }
void DeserializeNetworkData(void* a, void* b, int c) { (void)a; (void)b; (void)c; }
uint8_t Dialog_IsComplete(void* a) { (void)a; return 0; }
void DismissPageGroup(void* a) { (void)a; }

void FadePageGroup(void* a, int b, int c, float d, float e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

void GetPlayerID(void* a, unsigned int b) { (void)a; (void)b; }
void HSM_QueueNotification(void* a) { (void)a; }
void InitializePageGroup(void* a) { (void)a; }

// ── LocomotionStateAnim members ─────────────────────────────────────────────

void LocomotionStateAnim::ScalarDtor(int flags) { (void)flags; }
void LocomotionStateAnim::UpdateAnimation(float dt, void* out, float w) { (void)dt; (void)out; (void)w; }
bool LocomotionStateAnim::EvaluateAnimation(void* out) { (void)out; return false; }
bool LocomotionStateAnim::GetAnimTransform(void* out) { (void)out; return false; }

void LocomotionStateAnim_8278_g(void* a, void* b) { (void)a; (void)b; }
void LocomotionStateAnim_88E0_g(void* a, void* b, float c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void LocomotionStateAnim_BlendLocomotionAnims(uint32_t a, void* b, float c) { (void)a; (void)b; (void)c; }
void LocomotionStateAnim_C8F8_g(void* a) { (void)a; }
void LocomotionStateAnim_TransitionLocomotionState(uint32_t a, void* b, float c) { (void)a; (void)b; (void)c; }
void LocomotionState_OnEnter(void* a) { (void)a; }

// ── Net / Page / Message ────────────────────────────────────────────────────

void NetDataQuery_ctor_A458(void* a) { (void)a; }
void* PageGroup_GetTextEntry(void* a, const char* b) { (void)a; (void)b; return nullptr; }
void* PageGroup_LookupText(uint32_t a, const char* b) { (void)a; (void)b; return nullptr; }
void PageGroup_Register(void* a) { (void)a; }
void* PageGroup_SetState(void* a) { (void)a; return nullptr; }

void PlayerMovementMessage_54B0_h(void* a, float* b, float* c, bool d) {
    (void)a; (void)b; (void)c; (void)d;
}

void PostPageGroupMessage(int a, int b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void PostPageGroupMessage(uint32_t a, uint8_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}
void PostPageGroupMessage(void* a, int b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void PostStateTransitionRequest(void* a, int b) { (void)a; (void)b; }

void QueueGameMessage(int a, int b, void* c, int d, void* e, void* f) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}

void ReadBitsFromStream(void* a, void* b, int c) { (void)a; (void)b; (void)c; }

void RegisterSerializedField(void* a, const char* b, void* c, const void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

void ResetShotTimingState(void* a) { (void)a; }

void* RtlEnterCriticalSection_D6F0_fw(void* a) { (void)a; return nullptr; }
void SendContextMessage(int a, int b, int c, int d) { (void)a; (void)b; (void)c; (void)d; }
void ServeStartedMessage_5728(void* a, void* b, void* c, float d) { (void)a; (void)b; (void)c; (void)d; }
void SetPageGroupVisible(void* a) { (void)a; }

// ── SinglesNetworkClient free-function stubs ────────────────────────────────

void snBitStream_Reset(void* a) { (void)a; }
void snBitStream_WriteBits(void* a, uint32_t b, int c) { (void)a; (void)b; (void)c; }
uint32_t SinglesNetworkClient_0738_g(void* a, const char* b, uint32_t c) { (void)a; (void)b; (void)c; return 0; }
void* rlEvent_Init(void* a, int b, void* c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return nullptr;
}
void snBitStream_ReadSigned(void* a, uint32_t* b, int c) { (void)a; (void)b; (void)c; }
void SinglesNetworkClient_0F80_g(void* a) { (void)a; }
uint8_t SinglesNetworkClient_1178_g(void* a) { (void)a; return 0; }
void SinglesNetworkClient_2F28_g(void* a) { (void)a; }
void SinglesNetworkClient_2FD8_g(void* a) { (void)a; }
void SinglesNetworkClient_4128_g(void* a, int b, void* c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
int SinglesNetworkClient_4928_g(void* a) { (void)a; return 0; }
void SinglesNetworkClient_4FB0_g(void* a) { (void)a; }
void* SinglesNetworkClient_58E8_g(uint8_t a) { (void)a; return nullptr; }
void SinglesNetworkClient_5998_g(void* a) { (void)a; }
void SinglesNetworkClient_5A40_g(void* a) { (void)a; }
bool SinglesNetworkClient_5EA0_g(void* a, uint32_t* b) { (void)a; (void)b; return false; }
void SinglesNetworkClient_6918_g(void* a, bool b) { (void)a; (void)b; }
void SinglesNetworkClient_70A0_g(void* a, uint8_t* b) { (void)a; (void)b; }
void snBitStream_ValidateWrite(void* a) { (void)a; }
int SinglesNetworkClient_8CC0_w(void* a) { (void)a; return 0; }
void snBitStream_ReadBits(void* a, void* b, int c) { (void)a; (void)b; (void)c; }
void* snSession_FindProperty(void* a, const char* b) { (void)a; (void)b; return nullptr; }
void* SinglesNetworkClient_9720_g(void* a, void* b) { (void)a; (void)b; return nullptr; }
bool SinglesNetworkClient_A250_g(void* a, void* b) { (void)a; (void)b; return false; }
void snSession_BeginOperation(void* a) { (void)a; }
uint8_t snSession_AcquireLock(void* a) { (void)a; return 0; }
void snSession_ReleaseLock(void* a) { (void)a; }
void SinglesNetworkClient_BE30_g(void* a, int b) { (void)a; (void)b; }
void* SinglesNetworkClient_C838_g(void* a) { (void)a; return nullptr; }

// ── SpectatorNetworkClient members ──────────────────────────────────────────

void SpectatorNetworkClient::PostLoadChildren() {}
void SpectatorNetworkClient::vfn_2() {}
void SpectatorNetworkClient::vfn_3() {}
void SpectatorNetworkClient::vfn_4() {}
void SpectatorNetworkClient::vfn_5() {}
void SpectatorNetworkClient::vfn_6() {}
void SpectatorNetworkClient::vfn_7() {}

// ── Misc ────────────────────────────────────────────────────────────────────

int32_t TextEntry_GetValue(void* a) { (void)a; return 0; }
void WriteBallHitDataToNetwork(void* a, void* b) { (void)a; (void)b; }
void WriteFloatToNetworkStream(void* a, float b) { (void)a; (void)b; }


// ============================================================================
// SECTION 4: C++ stubs from stubs_linker_1.cpp (function stubs only, globals moved)
// ============================================================================

void WriteInt8Bits(void* client, int16_t value, int bits) {
    (void)client; (void)value; (void)bits;
}

// ── atArray / atSingleton helpers ───────────────────────────────────────────

void atArray_Clear(void* obj) { (void)obj; }
void atSingleton_0128_wrh(void* a, uint32_t b) { (void)a; (void)b; }
float math_SafeReciprocal(float a, double b) { (void)a; (void)b; return 0.0f; }
void atSingleton_2038(void* a, uint32_t b) { (void)a; (void)b; }
void atSingleton_22B0(void* a, uint32_t b) { (void)a; (void)b; }
void atSingleton_23C0(void* a) { (void)a; }
void atSingleton_29E8_p25(void* a) { (void)a; }
void atSingleton_5CD0_fw(void* a, uint32_t b) { (void)a; (void)b; }
void atSingleton_8E88_p42(void* a, void* b, uint32_t c, uint32_t d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void* atSingletonPool_AllocEntry(uint32_t a) { (void)a; return nullptr; }
void atSingleton_QueryEventData(void* a, void* b) { (void)a; (void)b; }

// ── Audio ───────────────────────────────────────────────────────────────────

void audControl_Destructor(void* obj) { (void)obj; }

// ── charViewCS virtual methods (local class definition to avoid char_view.hpp) ──

class charViewCS {
public:
    virtual ~charViewCS() = default;
    virtual void vfn_3();
    virtual void vfn_4();
    virtual void vfn_6();
    virtual void vfn_7();
    virtual void vfn_9();
    virtual void vfn_10();
    virtual void vfn_11();
};

void charViewCS::vfn_3()  {}
void charViewCS::vfn_4()  {}
void charViewCS::vfn_6()  {}
void charViewCS::vfn_7()  {}
void charViewCS::vfn_9()  {}
void charViewCS::vfn_10() {}
void charViewCS::vfn_11() {}

// ── CM (Control Machine) helpers ────────────────────────────────────────────

void cmMetafileTuningSet_vfn_8(void* a, uint32_t b, uint32_t c) { (void)a; (void)b; (void)c; }
void cmOperator_5FC8_g(void* a, uint32_t* b, uint32_t* c) { (void)a; (void)b; (void)c; }

// ── Animation ───────────────────────────────────────────────────────────────

void crAnimBlenderState_Init(void* a) { (void)a; }

// ── File I/O / Tokenizer ────────────────────────────────────────────────────

float fiAsciiTokenizer_2628_g(float a) { (void)a; return 0.0f; }

// ── Game logic functions ────────────────────────────────────────────────────

void* DialogPageGroup_Construct(void* a) { (void)a; return nullptr; }
void game_2E80(void* a) { (void)a; }
void game_2EE0(void* a) { (void)a; }
void game_36E8(void* a, const float* b) { (void)a; (void)b; }
void game_3860(void* a, int b, const char* c) { (void)a; (void)b; (void)c; }
void game_8EE8(void* a) { (void)a; }
void* LegalsPageGroup_Construct(void* a) { (void)a; return nullptr; }
void game_9CF8_h(void* a, uint32_t b) { (void)a; (void)b; }
void game_9D10_h(void* a, uint32_t b) { (void)a; (void)b; }
void game_9D28_h(void* a, uint32_t b) { (void)a; (void)b; }
void game_AA88(void* a) { (void)a; }
void CreditsRoll_Deactivate(void* a, int b, int c) { (void)a; (void)b; (void)c; }
void* CreditsRoll_NotifyEntry(void* a, uint32_t b) { (void)a; (void)b; return nullptr; }
void game_CD20(void* a) { (void)a; }
void game_D060(void* a) { (void)a; }
void game_D3B0_h(void* a) { (void)a; }
void game_D468(void* a, vec3* b) { (void)a; (void)b; }
void game_D500(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void game_DA60() {}

// ── Graphics ────────────────────────────────────────────────────────────────

void grc_2CC8(void* a, uint32_t b, uint32_t c) { (void)a; (void)b; (void)c; }
void grc_EB10(void* a) { (void)a; }


// ============================================================================
// SECTION 5: C++ stubs from stubs_linker_2.cpp
// ============================================================================

// ── HSM Context free functions ──────────────────────────────────────────────

void hsmContext_5B40_w(void* a) { (void)a; }
void hsmContext_5BC8_fw(void* a) { (void)a; }

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

// ── HUD Flash free functions ────────────────────────────────────────────────

void hudFlashBase_0F08_g(void* a, int b, float c, float d, int e, int f, int g) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g;
}
void hudFlashBase_DrawFlashOverlay(void* a, void* b, void* c, float d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// ── IO free functions ───────────────────────────────────────────────────────

uint8_t io_9E30(io* self) { (void)self; return 0; }
int io_Input_poll_9D68(void* a) { (void)a; return 0; }

// ── Jump table / kernel free functions ──────────────────────────────────────

void jumptable_3A48(void* a, unsigned int b, unsigned int c, unsigned int d) {
    (void)a; (void)b; (void)c; (void)d;
}
void jumptable_5C20(void* a) { (void)a; }
void snListNode_Init(void* a) { (void)a; }

// ── Motion clip free function ───────────────────────────────────────────────

void* mfMotionClipRAGE_B8D8_g(void* a, unsigned int b, unsigned int c, unsigned int d) {
    (void)a; (void)b; (void)c; (void)d; return nullptr;
}

// ── msgMsgSink member functions ─────────────────────────────────────────────

void msgMsgSink::GetName() {}
void msgMsgSink::OnEnter() {}
uint32_t msgMsgSink::CheckAndProcess() { return 0; }
void msgMsgSink::PostLoadProperties() {}
void msgMsgSink::ScalarDtor(int flags) { (void)flags; }
void msgMsgSink::Validate() {}

void msgMsgSink::vfn_3() {}
void msgMsgSink::vfn_8() {} void msgMsgSink::vfn_9() {}
uint16_t msgMsgSink::GetPeerDataSize() { return 0; }
void msgMsgSink::NotifySessionEvent() {}
void msgMsgSink::vfn_17() {} void msgMsgSink::vfn_18() {} void msgMsgSink::vfn_19() {}
uint32_t msgMsgSink::ProcessMessage(uint32_t messageFlags) { (void)messageFlags; return 0; }
int32_t msgMsgSink::GetStateFlagsLocked(uint32_t* outFlags) { (void)outFlags; return 0; }
int32_t msgMsgSink::GenerateAndCleanup() { return 0; }
int32_t msgMsgSink::GetConnectionInfo(void* bufferOut, uint32_t sizeParam, uint32_t* totalSizeOut) { (void)bufferOut; (void)sizeParam; (void)totalSizeOut; return 0; }
void msgMsgSink::vfn_27() {}
uint32_t msgMsgSink::UpdatePrioritiesLocked(void* priorityData) { (void)priorityData; return 0; }
uint32_t msgMsgSink::ApplyPrioritiesLocked(void* priorityData) { (void)priorityData; return 0; }
int32_t msgMsgSink::FindValidMessageSlot(uint32_t searchKey) { (void)searchKey; return 0; }
uint32_t msgMsgSink::SetPropertyLocked(uint32_t key, float value) { (void)key; (void)value; return 0; }
uint32_t msgMsgSink::GetPropertyLocked(uint32_t key, float* outValue) { (void)key; (void)outValue; return 0; }
uint32_t msgMsgSink::ProcessWithLock(uint32_t param) { (void)param; return 0; }
uint32_t msgMsgSink::SetMessageBuffer(void* param) { (void)param; return 0; }
void msgMsgSink::vfn_35() {} void msgMsgSink::vfn_36() {} void msgMsgSink::vfn_37() {}
uint32_t msgMsgSink::GetStateFlags() { return 0; }
void msgMsgSink::ProcessPendingMessages() {}
void msgMsgSink::CleanupIfReady() {}
void msgMsgSink::ForwardToMatchHandler() {}
uint32_t msgMsgSink::DispatchEventDefault() { return 0; }
void msgMsgSink::ClearPointers() {}
void msgMsgSink::vfn_44() {} void msgMsgSink::vfn_45() {}
uint32_t msgMsgSink::GetNestedObjectValue() { return 0; }
void msgMsgSink::FlushAndDisconnect() {}
void msgMsgSink::BeginDisconnect() {}
void msgMsgSink::SendEvent() {}
void* msgMsgSink::GetSessionPointer() { return nullptr; }
void msgMsgSink::ForwardProcessMessage() {}
void msgMsgSink::vfn_56() {} void msgMsgSink::vfn_57() {}
void msgMsgSink::DispatchDefaultHandler() {}
void msgMsgSink::vfn_60() {} void msgMsgSink::vfn_61() {}
void msgMsgSink::vfn_62() {} void msgMsgSink::vfn_63() {}
void msgMsgSink::vfn_65() {} void msgMsgSink::vfn_66() {} void msgMsgSink::vfn_67() {}
void msgMsgSink::ForwardMessageToSession(uint32_t param) { (void)param; }
void msgMsgSink::vfn_75() {}
void msgMsgSink::ForwardToLeaderboard() {}
void msgMsgSink::ForwardToRageHandler() {}
void msgMsgSink::vfn_79() {}
void msgMsgSink::ForwardDispatchMessage() {}
void msgMsgSink::ForwardToStatsHandler() {}
void msgMsgSink::vfn_105() {} void msgMsgSink::vfn_108() {}
void msgMsgSink::vfn_109() {} void msgMsgSink::vfn_113() {} void msgMsgSink::vfn_114() {}
uint32_t msgMsgSink::GetNameLength() { return 0; }
void msgMsgSink::ForwardToSessionSync() {}
uint32_t msgMsgSink::Release() { return 0; }
void* msgMsgSink::SendPulseToConnection() { return nullptr; }
void msgMsgSink::DispatchEventWithSessionInfo() {}
void msgMsgSink::vfn_121() {} void msgMsgSink::vfn_122() {}
void msgMsgSink::DisconnectMatchingHandlers(void* key) { (void)key; }
void msgMsgSink::vfn_125() {} void msgMsgSink::vfn_126() {}
void msgMsgSink::vfn_128() {}
int32_t msgMsgSink::RegisterMessageHandler(void* handler, uint32_t flags) { (void)handler; (void)flags; return 0; }
uint32_t msgMsgSink::FlushPendingMessagesLocked() { return 0; }
int32_t msgMsgSink::QueryConnectionState(uint32_t* outConnected) { (void)outConnected; return 0; }
void msgMsgSink::ForwardCleanupMessage() {}
void msgMsgSink::ForwardFinalizeMessage() {}

uint32_t msgMsgSink_3C88_g(void* a, unsigned int b) { (void)a; (void)b; return 0; }
uint32_t msgMsgSink_3D70_p39(void* a) { (void)a; return 0; }
void msgMsgSink_4550_g(void* a) { (void)a; }
uint32_t msgMsgSink_4EB8_g(void* a) { (void)a; return 0; }

void sysCallback_Invoke(void* a, int b) { (void)a; (void)b; }
void xmlNodeStruct_Initialize(void* a) { (void)a; }


// ============================================================================
// SECTION 6: C++ stubs from stubs_linker_3.cpp
// ============================================================================

uint32_t msgMsgSink_5098_g(void* a, bool b) { (void)a; (void)b; return 0; }
void msgMsgSink_6138_2hr(void* a) { (void)a; }
void msgMsgSink_6220_w(void* a) { (void)a; }
int32_t msgMsgSink_84C0_gen(void* a) { (void)a; return 0; }
void msgMsgSink_A970_2h(void* a) { (void)a; }
void msgMsgSink_D168_sp(void* a) { (void)a; }
void msgMsgSink_D1D8_sp(void* a) { (void)a; }
void msgMsgSink_DB70_g(void* a, uint32_t b) { (void)a; (void)b; }
void msgMsgSink_E860_g(void* a, uint32_t b, uint32_t c, uint32_t d) { (void)a; (void)b; (void)c; (void)d; }
void* msgMsgSink_F518_wrh(uint32_t a) { (void)a; return nullptr; }

void net_6BA0_fw(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }

// ── rage_debugLog overloads (C++ mangled) ────────────────────────────────────

void rage_debugLog() {}
void rage_debugLog(const char* fmt, ...) { (void)fmt; }
void rage_debugLog(const void* ptr, ...) { (void)ptr; }

// ── Animation blending ──────────────────────────────────────────────────────

void pcrAnimBlender_Initialize(void* a) { (void)a; }
int32_t pcrPostPointBlender_GetActiveClipIndex(pcrPostPointBlender* a) { (void)a; return 0; }

// ── Page Group helpers ──────────────────────────────────────────────────────

bool pg_6F68(void* a, void* b, int c, unsigned int* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return false;
}
void* pg_9C00_g(void* a) { (void)a; return nullptr; }
void* pg_9C00_g(void* a, int b) { (void)a; (void)b; return nullptr; }
uint8_t pg_ApplyTransition(void* a, TransitionParams* b, int c, TransitionFlags* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return 0;
}
void pg_E6E0(int a, int b, int c, int d) { (void)a; (void)b; (void)c; (void)d; }

void* phArchetype_Load(const char* a, void* b) { (void)a; (void)b; return nullptr; }

// ── phBoundCapsule ──────────────────────────────────────────────────────────

float phBoundCapsule_01D0_g(float a) { (void)a; return 0.0f; }
float phBoundCapsule_01D8_g(float a) { (void)a; return 0.0f; }
float phBoundCapsule_02B0_g(float a) { (void)a; return 0.0f; }
float phBoundCapsule_0FE0_g(float a, float b) { (void)a; (void)b; return 0.0f; }
void phBoundCapsule_5138_g(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void phBoundCapsule_A080_g(void* a) { (void)a; }

// ── phInst / phObject ───────────────────────────────────────────────────────

void phInst_A3A0_p33(void* a) { (void)a; }
void* atHashMap_Find(void* a, const void* b) { (void)a; (void)b; return nullptr; }

void* phObject::vfn_17(void* a) { (void)a; return nullptr; }
void phObject::vfn_20() {}
void* phObject::vfn_1(void* a) { (void)a; return nullptr; }
void phObject::vfn_2() {} void phObject::vfn_3() {} void phObject::vfn_4() {}
void* phObject::vfn_5() { return nullptr; } void* phObject::vfn_6() { return nullptr; }
void* phObject::vfn_7() { return nullptr; } void* phObject::vfn_8() { return nullptr; }
void* phObject::vfn_9() { return nullptr; } void* phObject::vfn_10() { return nullptr; }
void* phObject::vfn_21() { return nullptr; } void* phObject::vfn_22() { return nullptr; }
void* phObject::vfn_23() { return nullptr; } void* phObject::vfn_24() { return nullptr; }
void* phObject::vfn_25() { return nullptr; } void* phObject::vfn_26() { return nullptr; }
void* phObject::vfn_27() { return nullptr; } void* phObject::vfn_28() { return nullptr; }
void* phObject::vfn_29() { return nullptr; } void* phObject::vfn_30() { return nullptr; }
void* phObject::vfn_31() { return nullptr; }

// ── Physics utility functions ───────────────────────────────────────────────

void ph_59C8(void* a, const char* b, int c) { (void)a; (void)b; (void)c; }
void* ph_6FC8(void* a, const char* b) { (void)a; (void)b; return nullptr; }
void ph_9E50(void* a, void* b) { (void)a; (void)b; }
void* ph_9EC0_1(void* a) { (void)a; return nullptr; }
void ph_CEE0(void* a, int b) { (void)a; (void)b; }
void* ph_E010(void* a, void* b, const char* c) { (void)a; (void)b; (void)c; return nullptr; }
void ph_E088(void* a, void* b, void* c, float d, int e) { (void)a; (void)b; (void)c; (void)d; (void)e; }
void ph_EF40(void* a, void* b) { (void)a; (void)b; }
void ph_ForwardTarget(void* a) { (void)a; }

// ── plrPlayerMgr ────────────────────────────────────────────────────────────

void plrPlayerMgr::PostLoadChildren() {}
void plrPlayerMgr::PostLoadProperties() {}
void plrPlayerMgr::Validate() {}
void plrPlayerMgr::vfn_23() {} void plrPlayerMgr::vfn_24() {}
void plrPlayerMgr::vfn_25() {} void plrPlayerMgr::vfn_27() {}
void plrPlayerMgr::vfn_28() {} void plrPlayerMgr::vfn_29() {}
void plrPlayerMgr::vfn_30() {} void plrPlayerMgr::vfn_31() {}
void plrPlayerMgr::vfn_32() {}

// ── pongAttractState ────────────────────────────────────────────────────────

void pongAttractState::GetName() {}
void pongAttractState::ProcessInput() {}
void pongAttractState::Reset() {}
void pongAttractState::Shutdown() {}
void pongAttractState_Shutdown(void* a) { (void)a; }

// ── pongBallInstance ────────────────────────────────────────────────────────

void pongBallInstance::vfn_2() {} void pongBallInstance::vfn_24() {}
void pongBallInstance::vfn_3() {} void pongBallInstance::vfn_30() {}
void pongBallInstance::vfn_4() {} void pongBallInstance::vfn_5() {}
void pongBallInstance_4980_g(void* a, int b, int c, int d, int e) { (void)a; (void)b; (void)c; (void)d; (void)e; }

// ── pongCameraMgr helpers ───────────────────────────────────────────────────

bool pongCameraMgr_3500_fw(pongCameraMgr* a, void* b, int c) { (void)a; (void)b; (void)c; return false; }
bool pongCameraMgr_35A0_fw(pongCameraMgr* a, void* b, int c) { (void)a; (void)b; (void)c; return false; }
bool pongCameraMgr_35A0_fw(void* a, void* b, int c) { (void)a; (void)b; (void)c; return false; }
bool pongCameraMgr_3650_fw(pongCameraMgr* a, void* b, int c) { (void)a; (void)b; (void)c; return false; }
uint8_t pongCameraMgr_6E08(void* a, TransitionParams* b, int c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return 0;
}
void* pongCameraMgr_B9B0_g(void* a, int b) { (void)a; (void)b; return nullptr; }
uint8_t pongCameraMgr_ValidateTransition(void* a, TransitionParams* b, int c, TransitionFlags* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e; return 0;
}


// ============================================================================
// SECTION 7: C++ stubs from stubs_linker_3b.cpp
// ============================================================================

// pongCharViewContext stubs — char_view.hpp forward-declares `class atSingleton`
// without rage:: namespace. Provide minimal definition before including.
// Also, char_view.hpp redefines pongAttractState, charViewCS — we handle conflicts.
class atSingleton {
public:
    virtual ~atSingleton() = default;
};

// char_view.hpp redefines charViewCS and pongAttractState which are already
// defined via pong_states.hpp. We can't include it directly. Forward-declare
// pongCharViewContext minimally instead.
class pongCharViewContext {
public:
    virtual ~pongCharViewContext();
    virtual void vfn_11();
    virtual void vfn_12();
    virtual void vfn_16();
    virtual void vfn_17();
    virtual void vfn_18();
    virtual void vfn_23();
};

void pongCharViewContext::vfn_11() {}
void pongCharViewContext::vfn_12() {}
void pongCharViewContext::vfn_16() {}



// ============================================================================
// SECTION 8: C++ stubs from stubs_linker_4.cpp
// ============================================================================

// pongCharViewContext vfn_17/18/23 defined above in Section 7

void pongCharViewContext::vfn_17() {}
void pongCharViewContext::vfn_18() {}
void pongCharViewContext::vfn_23() {}

// ── pongCreature / pongCreatureInst helpers ─────────────────────────────────

void pongCreatureInst_RegisterPhysics(void* a, void* b) { (void)a; (void)b; }
void pongCreatureInst_EDC0_g(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void pongCreature_7CE8_g(void* a, void* b, int c, int d, int e, int f) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}

// ── pongDrawBucket ──────────────────────────────────────────────────────────

void pongDrawBucket::InitStatics() {}
void pongDrawBucket::Load(const char* a, int b, void* c) { (void)a; (void)b; (void)c; }

// ── pongLerpQueue ───────────────────────────────────────────────────────────

void pongLerpQueue_3410_g(void* a) { (void)a; }

// ── pongNetMessageHolder ────────────────────────────────────────────────────

void pongNetMessageHolder::ScalarDtor(int flags) { (void)flags; }
void pongNetMessageHolder_5038_w() {}
void pongNetMessageHolder_6778_wrh(void* a) { (void)a; }
void pongNetMessageHolder_68D0_wrh(void* a) { (void)a; }
void pongNetMessageHolder_6B48_wrh(void* a) { (void)a; }
void pongNetMessageHolder_6C98_wrh(void* a) { (void)a; }
void pongNetMessageHolder_6D68_wrh(void* a) { (void)a; }
void pongNetMessageHolder_6E30_wrh(void* a) { (void)a; }
void pongNetMessageHolder_6FF8_wrh(void* a) { (void)a; }
void pongNetMessageHolder_7700_wrh(void* a) { (void)a; }
pongNetMessageHolder* pongNetMessageHolder_FAE0_isl() { return nullptr; }
void pongNetMessageHolder_vfn_2_0868_1(pongNetMessageHolder* a) { (void)a; }
void pongNetMessageHolder_vfn_2_1628_1(pongNetMessageHolder* a) { (void)a; }
void pongNetMessageHolder_vfn_2_1770_1(pongNetMessageHolder* a) { (void)a; }
void pongNetMessageHolder_vfn_2_18D0_1(pongNetMessageHolder* a) { (void)a; }
void pongNetMessageHolder_vfn_2_24B8_1(void* a) { (void)a; }
void pongNetMessageHolder_vfn_2_3878_1(pongNetMessageHolder* a) { (void)a; }
void pongNetMessageHolder_vfn_2_FD70_1(pongNetMessageHolder* a) { (void)a; }

// ── pongPlayer ──────────────────────────────────────────────────────────────

// pongPlayer::Process(float) -- skipped, pong_misc.hpp declares Process() with no args
int pongPlayer_0270_g(void* a) { (void)a; return 0; }
void pongPlayer_0508_g(void* a, int b, float* c) { (void)a; (void)b; if (c) *c = 0.0f; }
void pongPlayer_1460_g(void* a, int b) { (void)a; (void)b; }
bool pongPlayer_5B60_gen(pongPlayer* a) { (void)a; return false; }
void pongPlayer_6470_g(float* a, void* b, int c) { (void)a; (void)b; (void)c; }
void pongPlayer_6470_g(vec3* a, void* b) { (void)a; (void)b; }
void pongPlayer_ApplyNetStateImpl(void* a) { (void)a; }
void pongPlayer_B208_g(void* a, vec3* b, unsigned char c) { (void)a; (void)b; (void)c; }
int pongPlayer_BF18_g(void* a, int b, vec3* c, vec3* d, int e) { (void)a; (void)b; (void)c; (void)d; (void)e; return 0; }
void pongPlayer_C678_g(pongPlayer* a) { (void)a; }
// pongPlayer_D238_g: moved to src/game/player/pong_player.cpp (returns float)
bool pongPlayer_DE98_g(void* a) { (void)a; return false; }
// pongPlayer_E590_g — moved to src/game/player/pong_player.cpp (full implementation)
void pongPlayer_E7B0_g(vec3* a, void* b) { (void)a; (void)b; }
void pongPlayer_FD20_g(void* a, int b, void* c, void* d, int e, void* f, void* g, unsigned char h) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
}
void pongPlayer_GetPlayerPosition(void* a, void* b) { (void)a; (void)b; }
void pongPlayer_InitShotImpl(void* a) { (void)a; }
void pongPlayer_Interpolate(void* a, void* b) { (void)a; (void)b; }
void pongPlayer_ResetMoverImpl(void* a) { (void)a; }

// ── pongScrnTransFadeIn ─────────────────────────────────────────────────────

void pongScrnTransFadeIn_EndTransition(void* a) { (void)a; }

// ── namespace rage globals and stubs ────────────────────────────────────────

namespace rage {

void* Allocate(unsigned long size, unsigned long alignment) {
    (void)size; (void)alignment; return nullptr;
}

void _locale_register(void* a, void* b) { (void)a; (void)b; }
void atSingleton_8068_h(void* a) { (void)a; }
void atSingleton_8A48_p42(void* a) { (void)a; }
void aud_1498(void* a) { (void)a; }
void aud_6A20_wrap_6A20(void* a) { (void)a; }
void fiAsciiTokenizer_ReadToken(void* a, int b) { (void)a; (void)b; }
uint8_t fiAsciiTokenizer_ReadBool(void* a, void* b, void* c) { (void)a; (void)b; (void)c; return 0; }
void fiAsciiTokenizer_Destroy(void* a) { (void)a; }

// namespace rage globals
extern const float g_capsuleVolK1 = 1.5f;
extern const float g_capsuleVolK2 = 4.18879f;
uint8_t  g_contextStack[128] = {};
void*    g_currentGeometry = nullptr;
void*    g_currentSwfContext = nullptr;
uint32_t* g_iccProfilePtr = nullptr;
uint32_t  g_mmioStoredValue = 0;
void*    g_phAllocator = nullptr;
uint32_t g_phCollisionFlags = 0;
uint16_t g_phDefaultStatusId = 0;
void*    g_phGlobalState = nullptr;
uint32_t g_swfCallDepth = 0;
float    g_swfFrameScale = 1.0f;
void*    g_swfGlobalObject = nullptr;
void*    g_swfGlobalScope = nullptr;
void*    g_swfStringBuffer = nullptr;
void*    g_vtable_cmApproach2 = nullptr;
void*    g_vtable_swfSCRIPTOBJECT = nullptr;

// grcTextureReferenceBase virtual stubs
void grcTextureReferenceBase::vfn_1() {}
void grcTextureReferenceBase::vfn_10() {}
void grcTextureReferenceBase::vfn_19() {}
void grcTextureReferenceBase::vfn_2() {}
grcTexture* grcTextureReferenceBase::vfn_21(void* a, void* b, void* c) { (void)a; (void)b; (void)c; return nullptr; }
void grcTextureReferenceBase::vfn_22(void* a) { (void)a; }
void grcTextureReferenceBase::vfn_23() {}
void grcTextureReferenceBase::vfn_24() {}

} // namespace rage


// ============================================================================
// SECTION 9: C++ stubs from stubs_linker_5.cpp
// ============================================================================

namespace rage {

void grcTextureReferenceBase::vfn_25() {}
void grcTextureReferenceBase::vfn_3() {}

void swfINSTANCE::vfn_7() {}

int phBoundOTGrid::Collide3D(float a, float b, float c, float d, float e) { (void)a; (void)b; (void)c; (void)d; (void)e; return 0; }
int phBoundOTGrid::Collide3DFull(float a, float b, float c, float d, float e, float f) { (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; return 0; }
int phBoundOTGrid::Collide3DYExtent(float a, float b, float c, float d) { (void)a; (void)b; (void)c; (void)d; return 0; }
int phBoundOTGrid::CollideCapsule(float a, float b, float c, float d, float e, float f, float g, float h) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h; return 0;
}
int phBoundOTGrid::CollideLineX(float a, float b, float c) { (void)a; (void)b; (void)c; return 0; }
int phBoundOTGrid::CollidePlaneXY(float a, float b, float c) { (void)a; (void)b; (void)c; return 0; }
int phBoundOTGrid::CollidePlaneXZ(float a, float b, float c, float d) { (void)a; (void)b; (void)c; (void)d; return 0; }

void hudFlashBase_9CA8_h(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }

void ke_ConstructObject(void* a) { (void)a; }
void ke_DestroyObjectA(void* a) { (void)a; }
void ke_DestroyObjectB(void* a) { (void)a; }

void phArticulatedCollider_UpdateJointTransforms(void* a) { (void)a; }
void phArticulatedCollider_ProcessJoints(void* a) { (void)a; }
void phArticulatedCollider_ProcessColliderState(void* a) { (void)a; }

void phBoundCapsule_01D0_g(phJoint3Dof* j, float v) { (void)j; (void)v; }
void phBoundCapsule_6C28_fw(void* a) { (void)a; }

void phCollider_vfn_4(phArticulatedCollider* a) { (void)a; }
void phInst_5910_p39(void* a) { (void)a; }

float phJoint1Dof_AE38(void* a) { (void)a; return 0.0f; }
void phJoint1Dof_AFF8_p42(phJoint3Dof* a, int b, float* c) { (void)a; (void)b; (void)c; }
void phJoint3Dof_0170_g(void* a, int b, int c, void* d) { (void)a; (void)b; (void)c; (void)d; }
void phJoint_1388(phJoint3Dof* a, int b, float c, float* d, float* e) { (void)a; (void)b; (void)c; (void)d; (void)e; }
void phJoint_PreSyncState(phJoint3Dof* a) { (void)a; }

void ph_1B78(void* a, float b, float c, void* d) { (void)a; (void)b; (void)c; (void)d; }
void ph_A330(void* a) { (void)a; }
void ph_E1E8(void* a) { (void)a; }
void ph_FE48(void* a, void* b) { (void)a; (void)b; }
void ph_vt3DB0_12_8DB8(void* a) { (void)a; }
void ph_vt3DB0_13_8E10(void* a) { (void)a; }
void ph_vt57D8_20_0718(void* a) { (void)a; }
void ph_vt57D8_28_FD08(void* a) { (void)a; }
void ph_vt57D8_29_FDD0(void* a) { (void)a; }
void ph_vt57D8_2_6378(void* a) { (void)a; }
void ph_vt57D8_3_61E0(void* a) { (void)a; }
void ph_vt5A60_57_6858(void* a) { (void)a; }
void ph_vt5A60_58_6EE8(void* a) { (void)a; }
void ph_vt5A60_60_7870(void* a) { (void)a; }
void ph_vt5A60_61_7A38(void* a) { (void)a; }
void ph_vt5A60_62_8F80(void* a) { (void)a; }
void ph_vt5A7C_63_6A98(void* a) { (void)a; }
void ph_vt5A84_63_6B90(void* a) { (void)a; }
void ph_vt5A8C_63_6A50(void* a) { (void)a; }
void ph_vt5B98_40_8D50(void* a) { (void)a; }
void ph_vt5B98_41_8E50(void* a) { (void)a; }

void rage_1058(void* a) { (void)a; }
void rage_Alloc(int a, void* b) { (void)a; (void)b; }

void util_4628(phJoint3Dof* a, int b) { (void)a; (void)b; }
void* util_B188(void* a, int b) { (void)a; (void)b; return nullptr; }
void snSession_AssociateConnections(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void snSession_ProcessPendingConnections(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void hsmState_AttachChild(void* a, void* b) { (void)a; (void)b; }

} // namespace rage

// ── C-linkage / global scope free functions ─────────────────────────────────

void rage_27C0(void* a) { (void)a; }
void* rage_Allocate(unsigned int a) { (void)a; return nullptr; }
void rage_C1A8(void* a) { (void)a; }
void datResource_Load(void* a, const char* b) { (void)a; (void)b; }
void rage_FF70(void* a) { (void)a; }
void rage_free(void* p) { free(p); }
void rage_threadpool_cleanup_6878() {}
void sgSceneGraph_FreeLists(void* a) { (void)a; }
void snHsmAcceptingJoinRequest_9A70(void* a, void* b) { (void)a; (void)b; }
void snSession_Detach(void* a) { (void)a; }
void snSession_AddNode(void* a, void* b) { (void)a; (void)b; }

void sub_821A8F58(void* a, const char* b, void* c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// typeinfo for hsmState
namespace { struct FakeTypeInfo { void* vtable; const char* name; }; }
extern "C" const FakeTypeInfo _ZTI8hsmState = { nullptr, "hsmState" };

void util_03C0(void* a, uint32_t* b) { (void)a; (void)b; }
void util_0AF0(void* a, void* b) { (void)a; (void)b; }
void util_0AF0(void* a, void* b, short c) { (void)a; (void)b; (void)c; }
int util_5A70(void* a, void* b, int c, int d, int e) { (void)a; (void)b; (void)c; (void)d; (void)e; return 0; }
void sysMemAllocator_PlatformFree(void* a, unsigned int b) { (void)a; (void)b; }
void util_7970(void* a, void* b, int c) { (void)a; (void)b; (void)c; }
// ReadBallHitDataFromNetwork dependency (see src/game/network/pong_network_io.cpp).
// util_1668 — sign-magnitude signed-8-bit bit-stream reader, not yet lifted.
uint8_t netStream_ReadS8SignMagnitude(void* client, void* outByte) { (void)client; (void)outByte; return 0; }
void util_8FD0(void* a) { (void)a; }
void util_AA38(NetDataQuery* a) { (void)a; }
void util_B8A0(void* a) { (void)a; }
void util_C930(void* a) { (void)a; }
void util_CDF0(void* a, const void* b, unsigned int c) { (void)a; (void)b; (void)c; }
void util_D150(void* a, void* b) { (void)a; (void)b; }
void util_D150(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void hsmEvent_Init(void* a) { (void)a; }
void util_PackColorRGBA(uint32_t* a, const float* b) { (void)a; (void)b; }
void* xam_singleton_init_8D60(void* a) { (void)a; return nullptr; }

void* rage_Alloc(int a) { (void)a; return nullptr; }
void* rage_Alloc(unsigned int a) { (void)a; return nullptr; }
void xe_GetLoadContext() {}
void xe_GetLoadContext(void* a) { (void)a; }
void rage_AssertMainThread() {}
void xmlNodeStruct_Init(void* a) { (void)a; }
int xml_ReadInt(const char* a) { (void)a; return 0; }
void xml_ReadString(const char* a, char* b, int c) { (void)a; if (b && c > 0) b[0] = '\0'; }

void rage_RegisterUIContext(void* a, unsigned b, const char* c) { (void)a; (void)b; (void)c; }
void fsmMachine_Destroy(void* a) { (void)a; }

// ── C-linkage wrappers for dual-linkage symbols ─────────────────────────────

extern "C" {
void _c_PostStateTransitionRequest(void* m, int e) __asm__("_PostStateTransitionRequest");
void _c_PostStateTransitionRequest(void* m, int e) { PostStateTransitionRequest(m, e); }
// RtlEnterCriticalSection / RtlLeaveCriticalSection — C-linkage already
// provided by extern "C" definitions in Section 1
void _c_atArray_Clear(void* a) __asm__("_atArray_Clear");
void _c_atArray_Clear(void* a) { atArray_Clear(a); }
void _c_audControl_Destructor(void* o) __asm__("_audControl_Destructor");
void _c_audControl_Destructor(void* o) { audControl_Destructor(o); }
void _c_hsmContext_SetNextState(void* c, int s) __asm__("_hsmContext_SetNextState_2800");
void _c_hsmContext_SetNextState(void* c, int s) { hsmContext_SetNextState(c, s); }
void _c_pgPageGroup_DispatchEvent(void* a, void* b, int c, unsigned* d, int e) __asm__("_pg_6F68");
void _c_pgPageGroup_DispatchEvent(void* a, void* b, int c, unsigned* d, int e) { pg_6F68(a,b,c,d,e); }
void _c_atHashMap_Find(void* a, const void* b) __asm__("_atHashMap_Find");
void _c_atHashMap_Find(void* a, const void* b) { atHashMap_Find(a,b); }
void _c_sysCallback_Invoke(void* c, int code) __asm__("_sysCallback_Invoke");
void _c_sysCallback_Invoke(void* c, int code) { sysCallback_Invoke(c, code); }
void _c_xmlNodeStruct_Initialize(void* o) __asm__("_xmlNodeStruct_Initialize");
void _c_xmlNodeStruct_Initialize(void* o) { xmlNodeStruct_Initialize(o); }
}

// ── dcamPolarCam vtable stubs (asm labels from globals_linker.cpp) ───────────

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

// ── pgPageGroup_DispatchEvent ───────────────────────────────────────────────

void pgPageGroup_DispatchEvent(void* a, void* b, int c, unsigned int* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// ── pongPlayer swing stubs (from stubs_final.cpp) ───────────────────────────

void pongPlayer_42E0_g(void* a, vec3* b, vec3* c, float d) { (void)a; (void)b; (void)c; (void)d; }
void pongPlayer_73E8_g(pongPlayer* a, float b) { (void)a; (void)b; }
void pongPlayer_AB48_g(vec3* a, void* b) { (void)a; (void)b; }
void pongPlayer_9CD0_g(void* a) { (void)a; }

// ── pcrAnimBlender / pcrAnimState (from stubs_final.cpp) ────────────────────

void pcrAnimBlender_ApplyClipWeights(void* a, unsigned b, float c, float d) { (void)a; (void)b; (void)c; (void)d; }
void pcrAnimState_ComputePosition(vec3* a, pongAnimState* b) { (void)a; (void)b; }
