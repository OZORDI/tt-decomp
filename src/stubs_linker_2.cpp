/**
 * stubs_linker_2.cpp — Stub implementations for 97 undefined linker symbols (chunk 2)
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 *
 * Note: hsmContext_SetNextState is already stubbed in stubs_linker.cpp.
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

// ── Missing C-linkage globals (macOS prefixes _ for C symbols) ──────────────
extern "C" {
    float g_alphaMultiplier_c = 1.0f;
    float g_animBlendWeight_c = 1.0f;
    // ... these are actually the same globals but with different linkage
    // The real fix: mark them extern "C" in globals_linker.cpp
}

// ── Missing free functions ──────────────────────────────────────────────────
extern "C" {
// REMOVED (conflict):     void PostStateTransitionRequest(void* mgr, int event) { (void)mgr; (void)event; }
// REMOVED (conflict):     void RtlEnterCriticalSection(void* cs) { (void)cs; }
// REMOVED (conflict):     void RtlLeaveCriticalSection(void* cs) { (void)cs; }
// REMOVED (conflict):     void atArray_Clear(void* arr) { (void)arr; }
// REMOVED (conflict):     void audControl_Destructor(void* obj) { (void)obj; }
// REMOVED (conflict):     void fsmMachine_Destroy(void* obj) { (void)obj; }
// REMOVED (conflict):     void hsmContext_SetNextState(void* ctx, int state) { (void)ctx; (void)state; }
// REMOVED (conflict):     void rage_free(void* ptr) { (void)ptr; }
// REMOVED (conflict):     void xmlNodeStruct_Initialize(void* obj) { (void)obj; }
// REMOVED (conflict):     void phInst_A3A0_p33(void* obj) { (void)obj; }
// REMOVED (conflict):     void pongAttractState_Shutdown(void* obj) { (void)obj; }
// REMOVED (conflict):     void sysCallback_Invoke(void* cb, int code) { (void)cb; (void)code; }
// REMOVED (conflict):     void* pg_9C00_g(void* p) { (void)p; return nullptr; }
// REMOVED (conflict):     void pg_E6E0(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; }
// REMOVED (conflict):     float phBoundCapsule_01D0_g(float v) { return v; }
// REMOVED (conflict):     float phBoundCapsule_01D8_g(float v) { return v; }
// REMOVED (conflict):     float phBoundCapsule_02B0_g(float v) { return v; }
// REMOVED (conflict):     float phBoundCapsule_0FE0_g(float a, float b) { (void)b; return a; }
// REMOVED (conflict):     void* ph_9EC0_1(void* m) { return m; }
// REMOVED (conflict):     void* ph_E010(void* w, void* a, const char* p) { (void)w;(void)a;(void)p; return nullptr; }
// REMOVED (conflict):     void ph_9E50(void* a, void* b) { (void)a;(void)b; }
// REMOVED (conflict):     void ph_CEE0(void* a, int b) { (void)a;(void)b; }
// REMOVED (conflict):     void ph_E088(void* a, void* b, void* c, float d, int e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
// REMOVED (conflict):     void* ph_6FC8(void* a, const char* b) { (void)a;(void)b; return nullptr; }
// REMOVED (conflict):     void* phArchetype_Load(const char* p, void* m) { (void)p;(void)m; return nullptr; }
// REMOVED (conflict):     void ph_59C8(void* a, const char* b, int c) { (void)a;(void)b;(void)c; }
// REMOVED (conflict):     void ph_EF40(void* a, void* b) { (void)a;(void)b; }
// REMOVED (conflict):     void ph_ForwardTarget(void* a) { (void)a; }
// REMOVED (conflict):     void phBoundCapsule_5138_g(void* a, void* b, void* c) { (void)a;(void)b;(void)c; }
// REMOVED (conflict):     void phBoundCapsule_A080_g(void* a) { (void)a; }
// REMOVED (conflict):     void phMaterialMgrImpl_C208_g(void* a, const void* b) { (void)a;(void)b; }
    // dcamPolarCam asm-label stubs
    void datParserRecord_vfn_0_impl() __asm__("datParserRecord_vfn_0"); void datParserRecord_vfn_0_impl() {}
    void datParser_rtti_74A0_0_impl() __asm__("datParser_rtti_74A0_0"); void datParser_rtti_74A0_0_impl() {}
    void datSerialize_vfn_0_impl() __asm__("datSerialize_vfn_0"); void datSerialize_vfn_0_impl() {}
    void dcamPolarCam_vfn_0_impl() __asm__("dcamPolarCam_vfn_0"); void dcamPolarCam_vfn_0_impl() {}
    void dcamPolarCam_vfn_1_impl() __asm__("dcamPolarCam_vfn_1"); void dcamPolarCam_vfn_1_impl() {}
    void dcamPolarCam_vfn_3_impl() __asm__("dcamPolarCam_vfn_3"); void dcamPolarCam_vfn_3_impl() {}
    void dcamPolarCam_vfn_4_impl() __asm__("dcamPolarCam_vfn_4"); void dcamPolarCam_vfn_4_impl() {}
    void dcamPolarCam_vfn_5_impl() __asm__("dcamPolarCam_vfn_5"); void dcamPolarCam_vfn_5_impl() {}
    void dcamPolarCam_vfn_10_impl() __asm__("dcamPolarCam_vfn_10"); void dcamPolarCam_vfn_10_impl() {}
}

// ── rage:: namespace stubs ──────────────────────────────────────────────────
namespace rage {
// REMOVED (conflict):     void ReleaseSingleton(void* obj) { (void)obj; }
    void* g_display_obj_ptr = nullptr;
    void* g_phCallback0 = nullptr;
    void* g_phCallback1 = nullptr;
    void* g_phCallback2 = nullptr;
    void* g_phCallback3 = nullptr;
    void* g_phCallback4 = nullptr;
    void* g_phCallback5 = nullptr;
    const char* g_str_swf_unsupportedMethod = "";
    void* g_swfScriptObjectPool = nullptr;
    void* g_tls_base = nullptr;
    void msgMsgSink_8DA0_sp(void* obj) { (void)obj; }
}

// ── Remaining game stubs ────────────────────────────────────────────────────
// REMOVED (conflict): void pongPlayer_42E0_g(void* p, void* a, void* b, float c) { (void)p;(void)a;(void)b;(void)c; }
// REMOVED (conflict): void pongPlayer_73E8_g(void* p, float v) { (void)p;(void)v; }
void pongPlayer_9CD0_g(void* p, int i, void* a, void* b) { (void)p;(void)i;(void)a;(void)b; }
// REMOVED (conflict): void pongPlayer_AB48_g(void* a, void* b) { (void)a;(void)b; }
// REMOVED (conflict): void pongBallInstance_4980_g(void* a, int b, int c, int d, int e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
// REMOVED (conflict): void rage_RegisterUIContext(void* a, unsigned int b, const char* c) { (void)a;(void)b;(void)c; }


