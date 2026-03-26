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
#include <cstdio>
#include <cstring>

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

/**
 * hsmState::GetFullStatePath @ 0x823ED290 | size: 0xF8
 *
 * Builds full hierarchical state path by walking the parent chain.
 * Layout used:
 *   +0x00 vtable, +0x08 parent (hsmState*),
 *   vtable slot 2 = GetStateName, vtable slot 3 = GetFullStatePath
 */
void hsmState::GetFullStatePath(char* buffer, unsigned int bufferSize) const {
    // +0x08 = parent state pointer
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    hsmState* parent = *reinterpret_cast<hsmState* const*>(self + 0x08);

    if (parent) {
        // Recurse: call parent's GetFullStatePath (vtable slot 3)
        parent->GetFullStatePath(buffer, bufferSize);

        // Inline strlen to find current length
        unsigned int currentLen = 0;
        {
            const char* p = buffer;
            while (*p++) {}
            currentLen = static_cast<unsigned int>(p - buffer - 1);
        }
        if (currentLen >= bufferSize) return;

        // vtable slot 2 = GetStateName — call via vtable
        typedef const char* (*GetNameFn)(const void*);
        GetNameFn getName = reinterpret_cast<GetNameFn>(vtable[2]);
        const char* name = getName(this);

        snprintf(buffer + currentLen, bufferSize - currentLen, "/%s", name);
        return;
    }

    // No parent — copy our state name directly
    typedef const char* (*GetNameFn)(const void*);
    GetNameFn getName = reinterpret_cast<GetNameFn>(vtable[2]);
    const char* name = getName(this);

    unsigned int remaining = bufferSize - 1;
    char* dest = buffer;
    while (remaining > 0 && *name) {
        *dest++ = *name++;
        --remaining;
    }
    *dest = '\0';
}

/**
 * hsmState::Reset @ 0x823E8D58 | size: 0x50
 *
 * Calls OnExit (vtable slot 6) then zeroes fields at +4, +8, +12, +16, +20.
 */
void hsmState::Reset() {
    // vtable slot 6 = OnExit cleanup hook
    typedef void (*OnExitFn)(void*);
    OnExitFn onExit = reinterpret_cast<OnExitFn>(vtable[6]);
    onExit(this);

    // Zero fields +0x04 through +0x14
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    *reinterpret_cast<uint32_t*>(self + 0x10) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x14) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x04) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x08) = 0;
    *reinterpret_cast<uint32_t*>(self + 0x0C) = 0;
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

// ============================================================================
// System callback and XML stubs (from chunk 3 linker symbols)
// ============================================================================

void sysCallback_Invoke(void* obj, int flags) {
    (void)obj; (void)flags;
}

void xmlNodeStruct_Initialize(void* obj) {
    (void)obj;
}
