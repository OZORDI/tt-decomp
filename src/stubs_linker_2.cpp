/**
 * stubs_linker_2.cpp — Stub implementations for 97 undefined linker symbols (chunk 2)
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 *
 * Note: hsmContext_SetNextState_2800 is already stubbed in stubs_linker.cpp.
 */

#include <cstddef>
#include <cstdint>

// Include struct definitions for member function stubs
#include "game/misc/pong_misc.hpp"
#include "game/match/pong_states.hpp"

// ============================================================================
// HSM Context free functions
// ============================================================================

void hsmContext_5B40_w(void* obj) {
    (void)obj;
}

void hsmContext_5BC8_fw(void* obj) {
    (void)obj;
}

// ============================================================================
// hsmState member functions
// ============================================================================

void hsmState::GetFullStatePath(char* buf, unsigned int bufSize) const {
    (void)buf; (void)bufSize;
    if (buf && bufSize > 0) buf[0] = '\0';
}

void hsmState::Reset() {
}

// ============================================================================
// HUD Flash free functions
// ============================================================================

void hudFlashBase_0F08_g(void* hudObj, int modeIndex, float param1, float param2,
                         int param3, int param4, int param5) {
    (void)hudObj; (void)modeIndex; (void)param1; (void)param2;
    (void)param3; (void)param4; (void)param5;
}

void hudFlashBase_DrawFlashOverlay(void* hudObj, void* corner1, void* corner2,
                                   float alpha, int reverseFlag) {
    (void)hudObj; (void)corner1; (void)corner2; (void)alpha; (void)reverseFlag;
}

// ============================================================================
// IO free functions
// ============================================================================

uint8_t io_9E30(io* self) {
    (void)self;
    return 0;
}

int io_Input_poll_9D68(void* inputObj) {
    (void)inputObj;
    return 0;
}

// ============================================================================
// Jump table / kernel free functions
// ============================================================================

void jumptable_3A48(void* target, unsigned int a, unsigned int b, unsigned int c) {
    (void)target; (void)a; (void)b; (void)c;
}

void jumptable_5C20(void* obj) {
    (void)obj;
}

void ke_1B00(void* msg) {
    (void)msg;
}

// ============================================================================
// Motion clip free function
// ============================================================================

void* mfMotionClipRAGE_B8D8_g(void* clip, unsigned int param2,
                               unsigned int param3, unsigned int param4) {
    (void)clip; (void)param2; (void)param3; (void)param4;
    return nullptr;
}

// ============================================================================
// msgMsgSink member functions — named virtuals
// ============================================================================

void msgMsgSink::GetName() {
}

void msgMsgSink::OnEnter() {
}

void msgMsgSink::PostLoadChildren() {
}

void msgMsgSink::PostLoadProperties() {
}

void msgMsgSink::ScalarDtor(int flags) {
    (void)flags;
}

void msgMsgSink::Validate() {
}

// ============================================================================
// msgMsgSink member functions — vfn stubs
// ============================================================================

void msgMsgSink::vfn_105() {}
void msgMsgSink::vfn_108() {}
void msgMsgSink::vfn_109() {}
void msgMsgSink::vfn_113() {}
void msgMsgSink::vfn_114() {}
void msgMsgSink::vfn_115() {}
void msgMsgSink::vfn_116() {}
void msgMsgSink::vfn_118() {}
void msgMsgSink::vfn_119() {}
void msgMsgSink::vfn_12() {}
void msgMsgSink::vfn_120() {}
void msgMsgSink::vfn_121() {}
void msgMsgSink::vfn_122() {}
void msgMsgSink::vfn_123() {}
void msgMsgSink::vfn_125() {}
void msgMsgSink::vfn_126() {}
void msgMsgSink::vfn_128() {}
void msgMsgSink::vfn_129() {}
void msgMsgSink::vfn_130() {}
void msgMsgSink::vfn_131() {}
void msgMsgSink::vfn_14() {}
void msgMsgSink::vfn_147() {}
void msgMsgSink::vfn_148() {}
void msgMsgSink::vfn_17() {}
void msgMsgSink::vfn_18() {}
void msgMsgSink::vfn_19() {}
void msgMsgSink::vfn_23() {}
void msgMsgSink::vfn_24() {}
void msgMsgSink::vfn_25() {}
void msgMsgSink::vfn_26() {}
void msgMsgSink::vfn_27() {}
void msgMsgSink::vfn_28() {}
void msgMsgSink::vfn_29() {}
void msgMsgSink::vfn_3() {}
void msgMsgSink::vfn_30() {}
void msgMsgSink::vfn_31() {}
void msgMsgSink::vfn_32() {}
void msgMsgSink::vfn_33() {}
void msgMsgSink::vfn_34() {}
void msgMsgSink::vfn_35() {}
void msgMsgSink::vfn_36() {}
void msgMsgSink::vfn_37() {}
void msgMsgSink::vfn_38() {}
void msgMsgSink::vfn_39() {}
void msgMsgSink::vfn_40() {}
void msgMsgSink::vfn_41() {}
void msgMsgSink::vfn_42() {}
void msgMsgSink::vfn_43() {}
void msgMsgSink::vfn_44() {}
void msgMsgSink::vfn_45() {}
void msgMsgSink::vfn_46() {}
void msgMsgSink::vfn_49() {}
void msgMsgSink::vfn_50() {}
void msgMsgSink::vfn_52() {}
void msgMsgSink::vfn_54() {}
void msgMsgSink::vfn_55() {}
void msgMsgSink::vfn_56() {}
void msgMsgSink::vfn_57() {}
void msgMsgSink::vfn_59() {}
void msgMsgSink::vfn_60() {}
void msgMsgSink::vfn_61() {}
void msgMsgSink::vfn_62() {}
void msgMsgSink::vfn_63() {}
void msgMsgSink::vfn_65() {}
void msgMsgSink::vfn_66() {}
void msgMsgSink::vfn_67() {}
void msgMsgSink::vfn_68() {}
void msgMsgSink::vfn_75() {}
void msgMsgSink::vfn_77() {}
void msgMsgSink::vfn_78() {}
void msgMsgSink::vfn_79() {}
void msgMsgSink::vfn_8() {}
void msgMsgSink::vfn_89() {}
void msgMsgSink::vfn_9() {}
void msgMsgSink::vfn_95() {}

// ============================================================================
// msgMsgSink-related free functions
// ============================================================================

uint32_t msgMsgSink_3C88_g(void* obj, unsigned int param) {
    (void)obj; (void)param;
    return 0;
}

uint32_t msgMsgSink_3D70_p39(void* obj) {
    (void)obj;
    return 0;
}

void msgMsgSink_4550_g(void* obj) {
    (void)obj;
}

uint32_t msgMsgSink_4EB8_g(void* obj) {
    (void)obj;
    return 0;
}
