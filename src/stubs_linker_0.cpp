/**
 * stubs_linker_0.cpp — Stub implementations for linker chunk 0
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 */

#include <cstddef>
#include <cstdint>

// ── Headers for class types ──
#include "game/audio/pong_audio.hpp"
#include "game/creature/pong_creature.hpp"
#include "game/network/pong_network.hpp"

// ============================================================================
// 1. (anonymous namespace)::g_miniheap — handled in rage/misc/rage_misc.cpp
// ============================================================================

// ============================================================================
// 2. BallHitMessage_ctor_69C8
// ============================================================================
void BallHitMessage_ctor_69C8(void* memory) {
    (void)memory;
}

// ============================================================================
// 3–7. CDelayEffect member stubs
// ============================================================================
void CDelayEffect::CalcInputFrames() {}

void CDelayEffect::GetRegistrationProperties() {}

void CDelayEffect::IsInputFormatSupported() {}

void CDelayEffect::LockForProcess() {}

void CDelayEffect::Process() {}

// ============================================================================
// 8–13. CPeakMeterEffect member stubs
// ============================================================================
void CPeakMeterEffect::CalcInputFrames() {}

void CPeakMeterEffect::GetRegistrationProperties() {}

void CPeakMeterEffect::IsInputFormatSupported() {}

void CPeakMeterEffect::LockForProcess() {}

void CPeakMeterEffect::Process() {}

void CPeakMeterEffect::UnlockForProcess() {}

// ============================================================================
// 14–15. CShelvingFilterEffect member stubs
// ============================================================================
void CShelvingFilterEffect::CalcInputFrames() {}

void CShelvingFilterEffect::LockForProcess() {}

// ============================================================================
// 16. CheckButtonPressed
// ============================================================================
bool CheckButtonPressed(void* record) {
    (void)record;
    return false;
}

// ============================================================================
// 17. ComputeNetworkHash
// ============================================================================
void ComputeNetworkHash(void* networkClient, int numSuccesses) {
    (void)networkClient; (void)numSuccesses;
}

// ============================================================================
// 18. CopyVectorThreadSafe
// ============================================================================
void CopyVectorThreadSafe(void* dst, void* src) {
    (void)dst; (void)src;
}

// ============================================================================
// 19. DeserializeNetworkData
// ============================================================================
void DeserializeNetworkData(void* client, void* dst, int size) {
    (void)client; (void)dst; (void)size;
}

// ============================================================================
// 20. Dialog_IsComplete
// ============================================================================
uint8_t Dialog_IsComplete(void* pg) {
    (void)pg;
    return 0;
}

// ============================================================================
// 21. DismissPageGroup
// ============================================================================
void DismissPageGroup(void* obj) {
    (void)obj;
}

// ============================================================================
// 22. FadePageGroup (overload: void*, int, int, float, float)
// ============================================================================
void FadePageGroup(void* mgr, int type, int mode, float fadeTime, float param2) {
    (void)mgr; (void)type; (void)mode; (void)fadeTime; (void)param2;
}

// ============================================================================
// 23. GetPlayerID
// ============================================================================
void GetPlayerID(void* playerContext, unsigned int param) {
    (void)playerContext; (void)param;
}

// ============================================================================
// 24. HSM_QueueNotification
// ============================================================================
void HSM_QueueNotification(void* ctx) {
    (void)ctx;
}

// ============================================================================
// 25. InitializePageGroup
// ============================================================================
void InitializePageGroup(void* pageGroup) {
    (void)pageGroup;
}

// ============================================================================
// 26–29. LocomotionStateAnim member stubs
// ============================================================================
void LocomotionStateAnim::ScalarDtor(int flags) {
    (void)flags;
}

void LocomotionStateAnim::vfn_6() {}

void LocomotionStateAnim::vfn_7() {}

void LocomotionStateAnim::vfn_9() {}

// ============================================================================
// 30. LocomotionStateAnim_8278_g
// ============================================================================
void LocomotionStateAnim_8278_g(void* state, void* animData) {
    (void)state; (void)animData;
}

// ============================================================================
// 31. LocomotionStateAnim_88E0_g
// ============================================================================
void LocomotionStateAnim_88E0_g(void* containerData, void* containerParams, float time, int p1, int p2) {
    (void)containerData; (void)containerParams; (void)time; (void)p1; (void)p2;
}

// ============================================================================
// 32. LocomotionStateAnim_BlendLocomotionAnims
// ============================================================================
void LocomotionStateAnim_BlendLocomotionAnims(uint32_t entry, void* parent, float dt) {
    (void)entry; (void)parent; (void)dt;
}

// ============================================================================
// 33. LocomotionStateAnim_C8F8_g
// ============================================================================
void LocomotionStateAnim_C8F8_g(void* animationList) {
    (void)animationList;
}

// ============================================================================
// 34. LocomotionStateAnim_TransitionLocomotionState
// ============================================================================
void LocomotionStateAnim_TransitionLocomotionState(uint32_t entry, void* parent, float dt) {
    (void)entry; (void)parent; (void)dt;
}

// ============================================================================
// 35. LocomotionState_OnEnter
// ============================================================================
void LocomotionState_OnEnter(void* state) {
    (void)state;
}

// ============================================================================
// 36. NetDataQuery_ctor_A458
// ============================================================================
void NetDataQuery_ctor_A458(void* self) {
    (void)self;
}

// ============================================================================
// 37. PageGroup_GetTextEntry
// ============================================================================
void* PageGroup_GetTextEntry(void* pg, const char* key) {
    (void)pg; (void)key;
    return nullptr;
}

// ============================================================================
// 38. PageGroup_LookupText
// ============================================================================
void* PageGroup_LookupText(uint32_t table, const char* key) {
    (void)table; (void)key;
    return nullptr;
}

// ============================================================================
// 39. PageGroup_Register
// ============================================================================
void PageGroup_Register(void* pg) {
    (void)pg;
}

// ============================================================================
// 40. PageGroup_SetState
// ============================================================================
void* PageGroup_SetState(void* pageGroup) {
    (void)pageGroup;
    return nullptr;
}

// ============================================================================
// 41. PlayerMovementMessage_54B0_h
// ============================================================================
void PlayerMovementMessage_54B0_h(void* player, float* velocity, float* acceleration, bool isMoving) {
    (void)player; (void)velocity; (void)acceleration; (void)isMoving;
}

// ============================================================================
// 42. PostPageGroupMessage (overload: int, int, int, int, int)
// ============================================================================
void PostPageGroupMessage(int eventCode, int arg1, int arg2, int arg3, int arg4) {
    (void)eventCode; (void)arg1; (void)arg2; (void)arg3; (void)arg4;
}

// ============================================================================
// 43. PostPageGroupMessage (overload: uint, uint8, uint, uint, uint, uint)
// ============================================================================
void PostPageGroupMessage(uint32_t msg, uint8_t flags, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6) {
    (void)msg; (void)flags; (void)p3; (void)p4; (void)p5; (void)p6;
}

// ============================================================================
// 44. PostPageGroupMessage (overload: void*, int, int, int, int)
// ============================================================================
void PostPageGroupMessage(void* record, int code, int mask, int a3, int a4) {
    (void)record; (void)code; (void)mask; (void)a3; (void)a4;
}

// ============================================================================
// 45. PostStateTransitionRequest — ALREADY in stubs_linker.cpp, skip
// ============================================================================

// ============================================================================
// 46. QueueGameMessage
// ============================================================================
void QueueGameMessage(int a, int b, void* c, int d, void* e, void* f) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}

// ============================================================================
// 47. ReadBitsFromStream
// ============================================================================
void ReadBitsFromStream(void* client, void* field, int size) {
    (void)client; (void)field; (void)size;
}

// ============================================================================
// 48. RegisterSerializedField (overload: void*, const char*, void*, const void*, int)
// ============================================================================
void RegisterSerializedField(void* obj, const char* name, void* fieldPtr, const void* desc, int flags) {
    (void)obj; (void)name; (void)fieldPtr; (void)desc; (void)flags;
}

// ============================================================================
// 49. RegisterSerializedField (void*, const char*, void*, void*, int) — C++ linkage
//     Note: stubs_linker.cpp has an extern "C" version (different mangling)
// ============================================================================
void RegisterSerializedField(void* obj, const char* name, void* fieldPtr, void* desc, int flags) {
    (void)obj; (void)name; (void)fieldPtr; (void)desc; (void)flags;
}

// ============================================================================
// 50. ResetShotTimingState
// ============================================================================
void ResetShotTimingState(void* shotTimer) {
    (void)shotTimer;
}

// ============================================================================
// 51. RtlEnterCriticalSection — ALREADY in stubs_remaining.c + xam/kernel.c
// ============================================================================

// ============================================================================
// 52. RtlEnterCriticalSection_D6F0_fw
// ============================================================================
void* RtlEnterCriticalSection_D6F0_fw(void* cs) {
    (void)cs;
    return nullptr;
}

// ============================================================================
// 53. RtlLeaveCriticalSection — ALREADY in stubs_remaining.c + xam/kernel.c
// ============================================================================

// ============================================================================
// 54. SendContextMessage
// ============================================================================
void SendContextMessage(int code, int mask, int a3, int a4) {
    (void)code; (void)mask; (void)a3; (void)a4;
}

// ============================================================================
// 55. ServeStartedMessage_5728
// ============================================================================
void ServeStartedMessage_5728(void* matchObj, void* slotA, void* slotB, float timingRef) {
    (void)matchObj; (void)slotA; (void)slotB; (void)timingRef;
}

// ============================================================================
// 56. SetPageGroupVisible
// ============================================================================
void SetPageGroupVisible(void* obj) {
    (void)obj;
}

// ============================================================================
// 57–88. SinglesNetworkClient free-function stubs
// ============================================================================

void SinglesNetworkClient_0268_g(void* self) {
    (void)self;
}

void SinglesNetworkClient_0448_g(void* self, uint32_t value, int bits) {
    (void)self; (void)value; (void)bits;
}

uint32_t SinglesNetworkClient_0738_g(void* client, const char* str, uint32_t length) {
    (void)client; (void)str; (void)length;
    return 0;
}

void* SinglesNetworkClient_0978_g(void* msg, int param1, void* rttiStr1, void* rttiStr2, int param2) {
    (void)msg; (void)param1; (void)rttiStr1; (void)rttiStr2; (void)param2;
    return nullptr;
}

void SinglesNetworkClient_0E18_g(void* client, uint32_t* queue, int param) {
    (void)client; (void)queue; (void)param;
}

void SinglesNetworkClient_0F80_g(void* self) {
    (void)self;
}

uint8_t SinglesNetworkClient_1178_g(void* self) {
    (void)self;
    return 0;
}

void SinglesNetworkClient_2F28_g(void* state) {
    (void)state;
}

void SinglesNetworkClient_2FD8_g(void* client) {
    (void)client;
}

void SinglesNetworkClient_4128_g(void* dispatcher, int msgId, void* context, int param1, int param2) {
    (void)dispatcher; (void)msgId; (void)context; (void)param1; (void)param2;
}

int SinglesNetworkClient_4928_g(void* msgHandler) {
    (void)msgHandler;
    return 0;
}

void SinglesNetworkClient_4FB0_g(void* obj) {
    (void)obj;
}

void* SinglesNetworkClient_58E8_g(uint8_t playerIdx) {
    (void)playerIdx;
    return nullptr;
}

void SinglesNetworkClient_5998_g(void* flags) {
    (void)flags;
}

void SinglesNetworkClient_5A40_g(void* flags) {
    (void)flags;
}

bool SinglesNetworkClient_5EA0_g(void* flags, uint32_t* queue) {
    (void)flags; (void)queue;
    return false;
}

void SinglesNetworkClient_67C8_g(void* client, int16_t index, void* data) {
    (void)client; (void)index; (void)data;
}

void SinglesNetworkClient_6838_g(void* client, uint8_t val, int size) {
    (void)client; (void)val; (void)size;
}

void SinglesNetworkClient_68A8_g(void* client, int16_t value, int bits) {
    (void)client; (void)value; (void)bits;
}

void SinglesNetworkClient_6918_g(void* client, bool flag) {
    (void)client; (void)flag;
}

void SinglesNetworkClient_70A0_g(void* msgPtr, uint8_t* sourcePtr) {
    (void)msgPtr; (void)sourcePtr;
}

void SinglesNetworkClient_8AE0_g(void* self) {
    (void)self;
}

int SinglesNetworkClient_8CC0_w(void* self) {
    (void)self;
    return 0;
}

void SinglesNetworkClient_8DF8_g(void* client, void* buf, int size) {
    (void)client; (void)buf; (void)size;
}

void* SinglesNetworkClient_9318_g(void* clientState, const char* msgType) {
    (void)clientState; (void)msgType;
    return nullptr;
}

void* SinglesNetworkClient_9720_g(void* stackBuf, void* client) {
    (void)stackBuf; (void)client;
    return nullptr;
}

bool SinglesNetworkClient_A250_g(void* queueBase, void* msgBuffer) {
    (void)queueBase; (void)msgBuffer;
    return false;
}

void SinglesNetworkClient_B1E8_g(void* pg) {
    (void)pg;
}

uint8_t SinglesNetworkClient_B2A8_g(void* client) {
    (void)client;
    return 0;
}

void SinglesNetworkClient_B320_g(void* client) {
    (void)client;
}

void SinglesNetworkClient_BE30_g(void* netObj, int frames) {
    (void)netObj; (void)frames;
}

void* SinglesNetworkClient_C838_g(void* currentMsg) {
    (void)currentMsg;
    return nullptr;
}

// ============================================================================
// 89–95. SpectatorNetworkClient member stubs
// ============================================================================

void SpectatorNetworkClient::PostLoadChildren() {}

void SpectatorNetworkClient::vfn_2() {}

void SpectatorNetworkClient::vfn_3() {}

void SpectatorNetworkClient::vfn_4() {}

void SpectatorNetworkClient::vfn_5() {}

void SpectatorNetworkClient::vfn_6() {}

void SpectatorNetworkClient::vfn_7() {}

// ============================================================================
// 96. TextEntry_GetValue
// ============================================================================
int32_t TextEntry_GetValue(void* entry) {
    (void)entry;
    return 0;
}

// ============================================================================
// 97. WriteBallHitDataToNetwork
// ============================================================================
void WriteBallHitDataToNetwork(void* ballHitData, void* client) {
    (void)ballHitData; (void)client;
}

// ============================================================================
// 98. WriteFloatToNetworkStream
// ============================================================================
void WriteFloatToNetworkStream(void* client, float value) {
    (void)client; (void)value;
}
