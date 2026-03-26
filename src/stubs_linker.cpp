/**
 * stubs_linker.cpp — Stub implementations for 163 undefined linker symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
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

// ============================================================================
// Physics / Scene
// ============================================================================

extern "C" void* CreatePhysicsInstance(void* physSystem, void* physType, void* resource) {
    (void)physSystem; (void)physType; (void)resource;
    return nullptr;
}

extern "C" void* CreateSimplePhysicsInstance(void* physSystem, void* physType) {
    (void)physSystem; (void)physType;
    return nullptr;
}

extern "C" void LinkPhysicsResource(void* physInstance, void* resource) {
    (void)physInstance; (void)resource;
}

extern "C" void ReleaseSceneObject(void* sceneObj) {
    (void)sceneObj;
}

extern "C" void ResetBoundObject(void* bound, int param) {
    (void)bound; (void)param;
}

extern "C" void ResetViewBound(void* screenObj, int param) {
    (void)screenObj; (void)param;
}

extern "C" void pongDrawBucket_AddEntry(void* bucket, void* renderEntry, void* boundsData) {
    (void)bucket; (void)renderEntry; (void)boundsData;
}

extern "C" void phInst_8F10(void* obj, void* a, void* b, void* c) {
    (void)obj; (void)a; (void)b; (void)c;
}

extern "C" void* phInst_BFB8_2hr(void* obj) {
    (void)obj;
    return nullptr;
}

extern "C" void phInst_Cleanup(void* obj) {
    (void)obj;
}

// REMOVED (conflict): extern "C" void* phMaterialMgrImpl_C208_g(void* table, const void* key) {

extern "C" float ph_Atan2(float y, float x) {
    (void)y; (void)x;
    return 0.0f;
}

extern "C" float ph_Normalize(float x) {
    (void)x;
    return 0.0f;
}

extern "C" void ke_DispatchPhysics(void* state) {
    (void)state;
}

extern "C" void grc_SetupResource(void* a, void* b) {
    (void)a; (void)b;
}

extern "C" void msgMsgSink_Broadcast(void* sink, void* msg, void* data) {
    (void)sink; (void)msg; (void)data;
}

extern "C" void fragDrawable_0AA0_2h(void* geom, void* drawable, uint32_t index, void* param) {
    (void)geom; (void)drawable; (void)index; (void)param;
}

// ============================================================================
// Debug / Logging
// ============================================================================

extern "C" void DbgPrint(const char* fmt, ...) {
    (void)fmt;
}

extern "C" void nop_8240E6D0(const char* fmt, ...) {
    (void)fmt;
}

// ============================================================================
// UI / Page Groups
// ============================================================================

extern "C" void FadePageGroup(void* grcDevice, float fadeValue, uint32_t p3, uint32_t p4, uint32_t p5) {
    (void)grcDevice; (void)fadeValue; (void)p3; (void)p4; (void)p5;
}

extern "C" void FadePageGroupOut(void* context, float fade, int flags, int p2, int p3) {
    (void)context; (void)fade; (void)flags; (void)p2; (void)p3;
}

extern "C" void PostPageGroupMessage(int code, int param1, int param2, int param3) {
    (void)code; (void)param1; (void)param2; (void)param3;
}

extern "C" void SetupCharViewDisplay(void* screenObj) {
    (void)screenObj;
}

// ============================================================================
// Game Data
// ============================================================================

extern "C" int32_t FindCharacterByName(void* gameDataMgr, const char* name) {
    (void)gameDataMgr; (void)name;
    return 0;
}

extern "C" void* GetStateContextName(void* manager) {
    (void)manager;
    return nullptr;
}

extern "C" uint16_t LookupEffectId(const char* name) {
    (void)name;
    return 0;
}

extern "C" int32_t util_2458_FindCharacterIndex(void* gameData, const char* name) {
    (void)gameData; (void)name;
    return 0;
}

// ============================================================================
// Xbox 360 Kernel / XAM
// ============================================================================

extern "C" int KeResetEvent(void* event) {
    (void)event;
    return 0;
}

extern "C" int KeSetEvent(void* event, int increment, int wait) {
    (void)event; (void)increment; (void)wait;
    return 0;
}

extern "C" int KeWaitForSingleObject(void* event, int waitReason, int waitMode, int alertable, void* timeout) {
    (void)event; (void)waitReason; (void)waitMode; (void)alertable; (void)timeout;
    return 0;
}

extern "C" int XamInputGetState(int userIndex, int flags, void* state) {
    (void)userIndex; (void)flags; (void)state;
    return 0;
}

extern "C" int XamInputSetState(int userIndex, void* vibration) {
    (void)userIndex; (void)vibration;
    return 0;
}

extern "C" int XamShowGamerCardUIForXUID(int userIndex, uint64_t xuid) {
    (void)userIndex; (void)xuid;
    return 0;
}

extern "C" void XamLoaderTerminateTitle(void) {
}

extern "C" uint32_t xam_CreateEvent(uint32_t type, uint32_t initCount, uint32_t p3, uint32_t p4) {
    (void)type; (void)initCount; (void)p3; (void)p4;
    return 0;
}

extern "C" void ke_EnterCriticalSection(uint32_t lockId) {
    (void)lockId;
}

extern "C" void ke_LeaveCriticalSection(uint32_t lockId) {
    (void)lockId;
}

extern "C" int RtlMultiByteToUnicodeN_6FA8_w(
    void* unicodeString, uint32_t maxBytes, uint32_t* resultBytes,
    const char* multiByteString, uint32_t multiByteLength)
{
    (void)unicodeString; (void)maxBytes; (void)resultBytes;
    (void)multiByteString; (void)multiByteLength;
    return 0;
}

extern "C" int _check_xdk_version(void) {
    return 0;
}

extern "C" int _heap_init_impl(void) {
    return 0;
}

extern "C" int _stricmp(const char* str1, const char* str2) {
    (void)str1; (void)str2;
    return 0;
}

// ============================================================================
// Network
// ============================================================================

extern "C" int NetDll_recvfrom(int handle, void* socket, void* buf, int len, int flags, void* from, int* fromlen) {
    (void)handle; (void)socket; (void)buf; (void)len; (void)flags; (void)from; (void)fromlen;
    return 0;
}

extern "C" int NetDll_sendto(int handle, void* socket, const void* buf, int len, int flags, const void* to, int tolen) {
    (void)handle; (void)socket; (void)buf; (void)len; (void)flags; (void)to; (void)tolen;
    return 0;
}

extern "C" void SinglesNetworkClient_8990_g(const char* src, char* dest, int maxSize) {
    if (dest && maxSize > 0) {
        dest[0] = '\0';
    }
    (void)src;
}

extern "C" void SinglesNetworkClient_SetMode(void* obj, int mode) {
    (void)obj; (void)mode;
}

extern "C" void SinglesNetworkClient_TickAll(void) {
}

// ============================================================================
// HSM / State Machine
// ============================================================================

// REMOVED (conflict): extern "C" void hsmContext_SetNextState_2800(void* hsmContext, uint32_t nextStateIdx) {

// REMOVED (conflict): extern "C" void* PostStateTransitionRequest(void* hsmCtx, int stateIdx) {

// ============================================================================
// Locomotion State / Animation
// ============================================================================

extern "C" void LocomotionStateAnim_AccumNode(void* obj, void* node) {
    (void)obj; (void)node;
}

extern "C" void LocomotionStateAnim_ApplySpeed(void* obj) {
    (void)obj;
}

extern "C" void LocomotionStateAnim_ComputeOffset(void* obj, void* outVec) {
    (void)obj; (void)outVec;
}

extern "C" void* LocomotionStateAnim_FindAnim(void* obj) {
    (void)obj;
    return nullptr;
}

extern "C" void LocomotionStateAnim_GrowArray(void* obj) {
    (void)obj;
}

extern "C" void LocomotionStateAnim_InitCapsuleImpl(void* obj, float radius, const void* center) {
    (void)obj; (void)radius; (void)center;
}

extern "C" void LocomotionStateAnim_ProcessNode(void* obj, void* node) {
    (void)obj; (void)node;
}

extern "C" void LocomotionStateAnim_SetMatrixImpl(void* obj, const void* matrix) {
    (void)obj; (void)matrix;
}

extern "C" void LocomotionStateAnim_SetSpeed(void* obj, float speed) {
    (void)obj; (void)speed;
}

extern "C" void LocomotionStateMf_ApplyConstraints(void* obj) {
    (void)obj;
}

extern "C" void LocomotionStateMf_BlendPoses(void* obj) {
    (void)obj;
}

extern "C" void LocomotionStateMf_Finish(void* obj) {
    (void)obj;
}

extern "C" void LocomotionStateMf_ScaleWeights(void* obj) {
    (void)obj;
}

extern "C" void LocomotionStateMf_UpdateTransforms(void* obj) {
    (void)obj;
}

extern "C" void* LocomotionState_FindFirstActive(void* list) {
    (void)list;
    return nullptr;
}

// ============================================================================
// Serialization / XML
// ============================================================================

extern "C" void RegisterSerializationField(void* obj, const char* name,
    void* fieldPtr, void* fieldType, int fieldFlags)
{
    (void)obj; (void)name; (void)fieldPtr; (void)fieldType; (void)fieldFlags;
}

extern "C" void RegisterSerializedField(void* obj, const char* name,
    void* fieldPtr, void* desc, int flags)
{
    (void)obj; (void)name; (void)fieldPtr; (void)desc; (void)flags;
}

extern "C" void xmlNodeStruct_BaseInitialize(void* obj) {
    (void)obj;
}

// REMOVED (conflict): extern "C" void xmlNodeStruct_Initialize(void* obj) {

extern "C" void xmlNodeStruct_SerializeField(void* obj, const char* name,
    void* target, void* defaultVal, uint32_t flags)
{
    (void)obj; (void)name; (void)target; (void)defaultVal; (void)flags;
}

// ============================================================================
// atArray / atSingleton
// ============================================================================

// REMOVED (conflict): extern "C" void atArray_Clear(void* obj) {

extern "C" void* atArray_Destructor(void* pArray) {
    (void)pArray;
    return nullptr;
}

extern "C" void atSingleton_2E60_g(void* singleton) {
    (void)singleton;
}

extern "C" void* atSingleton_CAD0_g(void* self) {
    (void)self;
    return nullptr;
}

extern "C" void atSingleton_D210_p46(void* pStructType, void* pStructInstance, void* pOperator) {
    (void)pStructType; (void)pStructInstance; (void)pOperator;
}

extern "C" void atSingleton_DA18_p46(void* pNode, void* pRootNode) {
    (void)pNode; (void)pRootNode;
}

// ============================================================================
// Audio
// ============================================================================

// REMOVED (conflict): extern "C" void audControl_Destructor(void* obj) {

extern "C" void xapo_UnpackBuffer(void* desc, void* outInfo) {
    (void)desc; (void)outInfo;
}

// ============================================================================
// CM (Control Machine) System
// ============================================================================

extern "C" float cmAngle_Normalize(float value) {
    (void)value;
    return 0.0f;
}

extern "C" float cmApproach2_ComputeFactor(void* node) {
    (void)node;
    return 0.0f;
}

extern "C" void cmCond_1038_g(void* condObj, void* dest) {
    (void)condObj; (void)dest;
}

extern "C" bool cmCond_21B0(void* condObj) {
    (void)condObj;
    return false;
}

extern "C" uint8_t cmNode_GetBoolValue(void* port) {
    (void)port;
    return 0;
}

extern "C" uint32_t cmNode_GetDimValue(void* port) {
    (void)port;
    return 0;
}

extern "C" bool cmNode_TryConnect3(void* node, void* desc) {
    (void)node; (void)desc;
    return false;
}

extern "C" bool cmNode_TryConnectSingle(void* node, void* desc) {
    (void)node; (void)desc;
    return false;
}

extern "C" void cmNormalizedTimer_Allocate(void* node) {
    (void)node;
}

extern "C" void cmOperatorCtor_D8C0_w(void* pOperator, const void* pValueData, uint32_t valueSize) {
    (void)pOperator; (void)pValueData; (void)valueSize;
}

extern "C" void cmOperatorCtor_DAE0_w(void* pOperator, uint32_t valueData, int flags) {
    (void)pOperator; (void)valueData; (void)flags;
}

extern "C" void cmOperatorCtor_DBC0_w(void* pOperator, int value, int flags) {
    (void)pOperator; (void)value; (void)flags;
}

extern "C" void* cmOperatorCtor_DC80_w(void* pOwner, const char* pText, int flags) {
    (void)pOwner; (void)pText; (void)flags;
    return nullptr;
}

extern "C" void cmPort_CopyToBuffer(void* reporter, void* port) {
    (void)reporter; (void)port;
}

extern "C" void cmPort_SyncValue(void* node, void* portDesc, void* portCtx) {
    (void)node; (void)portDesc; (void)portCtx;
}

extern "C" void cmPowerApproach_Step(float* outProgress, float diff, float speed, float dt) {
    if (outProgress) *outProgress = 0.0f;
    (void)diff; (void)speed; (void)dt;
}

extern "C" void cmReporter_Init(void* reporter) {
    (void)reporter;
}

extern "C" int cmSwitch_4B60(void* switchObj) {
    (void)switchObj;
    return 0;
}

extern "C" float cmVec4_Atan2(float* vecA, float* vecB) {
    (void)vecA; (void)vecB;
    return 0.0f;
}

// ============================================================================
// File I/O / Tokenizer
// ============================================================================

extern "C" void fiAsciiTokenizer_CFA8_w(void* pStringValue, const char* pBegin, const char* pEnd) {
    (void)pStringValue; (void)pBegin; (void)pEnd;
}

extern "C" void fiAsciiTokenizer_Process(void* tokenizer, int mode) {
    (void)tokenizer; (void)mode;
}

// ============================================================================
// parStream
// ============================================================================

extern "C" float parStreamInXml_A5D0(void* pOperator) {
    (void)pOperator;
    return 0.0f;
}

// ============================================================================
// Game Logic
// ============================================================================

extern "C" void game_7868(void* obj, int param) {
    (void)obj; (void)param;
}

extern "C" void gmLogic_StepFrame(void* pLogic) {
    (void)pLogic;
}

extern "C" void grmShaderPreset_AllocArray(uint32_t count) {
    (void)count;
}

// ============================================================================
// Jump Tables / Dispatch
// ============================================================================

extern "C" void jumptable_9498(
    void* pOperator,
    const char* pPrimaryName,
    const char* pSecondaryName,
    uint32_t copyPrimary,
    uint32_t copySecondary)
{
    (void)pOperator; (void)pPrimaryName; (void)pSecondaryName;
    (void)copyPrimary; (void)copySecondary;
}

extern "C" bool jumptable_A578_h(void* pOperator) {
    (void)pOperator;
    return false;
}

extern "C" uint8_t jumptable_E058_h(void* pMemberArray) {
    (void)pMemberArray;
    return 0;
}

// ============================================================================
// Page Group Internals
// ============================================================================

extern "C" void pg_6C40_g(void* streamer) {
    (void)streamer;
}

// REMOVED (conflict): extern "C" void pg_6F68(void* pageGroup, void* context, int eventType, ...) {

extern "C" void pg_C3B8_g(void* obj, int param) {
    (void)obj; (void)param;
}

// ============================================================================
// Camera Manager
// ============================================================================

extern "C" bool pongCameraMgr_FloatThresholdA(void* mgr, float threshold) {
    (void)mgr; (void)threshold;
    return false;
}

extern "C" bool pongCameraMgr_FloatThresholdB(void* mgr) {
    (void)mgr;
    return false;
}

extern "C" bool pongCameraMgr_QueryA(void* mgr, int index) {
    (void)mgr; (void)index;
    return false;
}

extern "C" bool pongCameraMgr_QueryB(void* mgr, int index) {
    (void)mgr; (void)index;
    return false;
}

extern "C" bool pongCameraMgr_QueryC(void* mgr, int index) {
    (void)mgr; (void)index;
    return false;
}

extern "C" void pongCameraMgr_SetCameraByIndex(void* mgr, int index) {
    (void)mgr; (void)index;
}

// ============================================================================
// Creature System
// ============================================================================

extern "C" void pongCreature_BaseDtor(void* obj) {
    (void)obj;
}

extern "C" void pongCreature_Fixup(void* obj, void* relocator) {
    (void)obj; (void)relocator;
}

extern "C" void pongCreature_UpdateReplay(void* creature) {
    (void)creature;
}

extern "C" void pongCreatureInst_Cleanup(void* obj) {
    (void)obj;
}

extern "C" float pongCreatureInst_ComputeHeading(void* obj) {
    (void)obj;
    return 0.0f;
}

extern "C" void pongCreatureInst_NotifyHeadingChanged(void* obj) {
    (void)obj;
}

extern "C" void pongCreatureInst_SetMatrixImpl(void* obj, const void* matrix) {
    (void)obj; (void)matrix;
}

// ============================================================================
// Player System
// ============================================================================

extern "C" bool pongPlayer_CheckBallSplash(void* player) {
    (void)player;
    return false;
}

extern "C" void pongPlayer_ClearShotState(void* player) {
    (void)player;
}

extern "C" int pongPlayer_GetAnimIndex(void* player) {
    (void)player;
    return 0;
}

extern "C" void pongPlayer_GetVelocity(void* player, float* outVec) {
    (void)player;
    if (outVec) { outVec[0] = 0.0f; outVec[1] = 0.0f; outVec[2] = 0.0f; }
}

extern "C" void pongPlayer_NetSync(void* player, int param) {
    (void)player; (void)param;
}

extern "C" void pongPlayer_ProcessReplay(void* player) {
    (void)player;
}

extern "C" void pongPlayer_ResetShotTiming(void* player, float val) {
    (void)player; (void)val;
}

extern "C" void pongPlayer_ResetSwingImpl(void* subObj) {
    (void)subObj;
}

extern "C" void pongPlayer_SetScoreSlot(void* player, int slot) {
    (void)player; (void)slot;
}

extern "C" void pongPlayer_StateHandler_9188(void* p) { (void)p; }
extern "C" void pongPlayer_StateHandler_9198(void* p) { (void)p; }
extern "C" void pongPlayer_StateHandler_91A8(void* p) { (void)p; }
extern "C" void pongPlayer_StateHandler_91B8(void* p) { (void)p; }
extern "C" void pongPlayer_StateHandler_91C8(void* p) { (void)p; }
extern "C" void pongPlayer_StateHandler_91D8(void* p) { (void)p; }
extern "C" void pongPlayer_StateHandler_91E8(void* p) { (void)p; }

extern "C" void pongPlayer_SyncFloat(void* player, void* desc) {
    (void)player; (void)desc;
}

extern "C" void pongPlayer_SyncWord(void* player, void* desc) {
    (void)player; (void)desc;
}

// ============================================================================
// RAGE Engine Core
// ============================================================================

extern "C" void rage_AddRef(void* obj) {
    (void)obj;
}

extern "C" void rage_Release(void* obj) {
    (void)obj;
}

extern "C" void rage_ReleaseSingleton(void* obj) {
    (void)obj;
}

extern "C" void* rage_97A8(const void* pOperator, const char* pNodeName) {
    (void)pOperator; (void)pNodeName;
    return nullptr;
}

extern "C" void rage_EC58(void* pStringValue, const char* pText) {
    (void)pStringValue; (void)pText;
}

extern "C" void rage_F6F0(void) {
}

// rage_cmIntegrate_vtable is a data symbol (vtable pointer)
extern "C" const void* rage_cmIntegrate_vtable = nullptr;

extern "C" void rage_obj_bind_3828(void* factoryContext, void* storage, uint32_t size) {
    (void)factoryContext; (void)storage; (void)size;
}

extern "C" void* rage_obj_factory_create_3040(void* singleton,
    const char* typeName, int typeId, void* allocator, int flags)
{
    (void)singleton; (void)typeName; (void)typeId; (void)allocator; (void)flags;
    return nullptr;
}

extern "C" void rage_obj_finalize_3B38(void* factoryContext) {
    (void)factoryContext;
}

// ============================================================================
// SWF / Scaleform
// ============================================================================

extern "C" float swfComputeFramePosition(void* ctx, void* entry, float* outMin, float* outMax) {
    (void)ctx; (void)entry;
    if (outMin) *outMin = 0.0f;
    if (outMax) *outMax = 0.0f;
    return 0.0f;
}

extern "C" const char* swfInternString(const char* name, void* buffer, int maxLen) {
    (void)buffer; (void)maxLen;
    return name;
}

extern "C" void swfPopResult(void* dst) {
    (void)dst;
}

extern "C" void swfSymtab_Delete(void* symtab, const char* key) {
    (void)symtab; (void)key;
}

extern "C" void swfSymtab_Enumerate(void* symtab) {
    (void)symtab;
}

extern "C" void swfSymtab_Insert(void* symtab, const char* key, void* value) {
    (void)symtab; (void)key; (void)value;
}

extern "C" void* swfSymtab_Lookup(void* symtab, const char* key) {
    (void)symtab; (void)key;
    return nullptr;
}

// ============================================================================
// System / Memory
// ============================================================================

// REMOVED (conflict): extern "C" void sysCallback_Invoke(void* obj, int flags) {

extern "C" void* sysMemAllocator_Allocate(void* ptr, size_t size) {
    (void)ptr; (void)size;
    return nullptr;
}

/**
 * sysMemAllocator_InitThreadHeap @ 0x820C0038 | size: 0x88
 *
 * Initializes the main thread's allocator context. Reads g_pAllocatorBase
 * (SDA r13+0), checks if slot [1] is populated, and if not, creates the
 * main thread's XeTlsBlock at a static .data address (0x8271B114).
 *
 * xe_alloc_thread_ctx_6CA8 (decompiled in src/crt/entry.c) sets the
 * rage::sysMemSimpleAllocator vtable, allocates a 44MB physical memory
 * block via xe_phys_alloc_6AC8, and initializes the free-list via util_7AE8.
 */
extern "C" void sysMemAllocator_InitThreadHeap(void) {
    extern void* g_pAllocatorBase;
    extern char g_mainThreadXtfStorage[256];
    extern uint32_t g_allocInitFlag;  // @ 0x8271B1D8 — initialization guard
    extern void xe_alloc_thread_ctx_6CA8(void* ctx, uint32_t heapSize, uint32_t flags);
    extern void xe_get_thread_ctx_36E8(void);

    void** base = (void**)g_pAllocatorBase;
    if (!base) return;

    // Check if slot [1] (offset +4 in original PPC layout) is already set
    if (base[1] != nullptr) return;

    void* xtfBlock = (void*)g_mainThreadXtfStorage;

    if (!(g_allocInitFlag & 1)) {
        g_allocInitFlag |= 1;
        // Allocates 44MB (0x2C0000) physical heap, sets sysMemSimpleAllocator vtable
        xe_alloc_thread_ctx_6CA8(xtfBlock, 0x002C0000u, 0);
        atexit(xe_get_thread_ctx_36E8);
    }

    // Store the main thread context into slots [1] and [2]
    base[1] = xtfBlock;
    base[2] = xtfBlock;
}

// ============================================================================
// Utility Functions
// ============================================================================

extern "C" float util_9350(void* entry) {
    (void)entry;
    return 0.0f;
}

extern "C" int util_4BD8(void* obj) {
    (void)obj;
    return 0;
}

extern "C" void util_92D8(void* dest, void* src) {
    (void)dest; (void)src;
}

extern "C" void util_9410(void* pOperator, void* pOwner, uint32_t duplicateOwner) {
    (void)pOperator; (void)pOwner; (void)duplicateOwner;
}

// ============================================================================
// CRT snprintf overload (C++ mangled — called from ph_physics.cpp)
// ============================================================================

// The codebase declares: void ph_snprintf(char*, int, const char*, const char*, int)
// inside C++ functions, producing a mangled symbol.  Provide the definition here.
void ph_snprintf(char* buf, int size, const char* fmt, const char* str, int val) {
    if (buf && size > 0) {
        std::snprintf(buf, (size_t)size, fmt, str, val);
    }
}
