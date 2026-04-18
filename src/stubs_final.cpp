// stubs_final.cpp — Final linker stubs with exact demangled signatures
#include <cstdlib>
#include <cstdint>
// hsmState methods — use weak linkage to avoid ODR conflicts
// The actual class is defined in include/rage/hsmState.hpp

// Forward declarations for typed params
struct vec3 { float x, y, z; };
struct pongAnimState;
struct pcrPostPointBlender;
struct TransitionParams;
struct TransitionFlags;
struct pongCameraMgr;
struct pongPlayer;
namespace rage { class atSingleton; }

// These are called with C++ linkage (no extern "C" at call sites)
// PostStateTransitionRequest — lifted in src/rage/core/hsm.cpp
void RtlEnterCriticalSection(void*) {}
void RtlLeaveCriticalSection(void*) {}
// atArray_Clear — moved to src/rage/data/at_types.cpp.
void audControl_Destructor(void*) {}
void fsmMachine_Destroy(void*) {}
void hsmContext_SetNextState(void*, int) {}
// msgMsgSink_5098_g .. msgMsgSink_F518_wrh + net_6BA0_fw lifted to
// src/game/network/pong_network_io.cpp
void pcrAnimBlender_ApplyClipWeights(void*, unsigned, float, float) {}
void pcrAnimBlender_Initialize(void*) {}
void pcrAnimState_ComputePosition(vec3*, pongAnimState*) {}
void pcrPostPointBlender_GetActiveClipIndex(pcrPostPointBlender*) {}
// pgPageGroup_DispatchEvent / pg_9C00_g / pg_ApplyTransition / pg_E6E0 → src/rage/swf.cpp
void* phArchetype_Load(const char*, void*) { return nullptr; }
float phBoundCapsule_01D0_g(float v) { return v; }
float phBoundCapsule_01D8_g(float v) { return v; }
float phBoundCapsule_02B0_g(float v) { return v; }
float phBoundCapsule_0FE0_g(float a, float b) { (void)b; return a; }
void phBoundCapsule_5138_g(void*, void*, void*) {}
void phBoundCapsule_A080_g(void*) {}
void phInst_A3A0_p33(void*) {}
// atHashMap_Find — moved to src/rage/data/at_types.cpp.
void ph_59C8(void*, const char*, int) {}
void* phArchetype_Find(void*, const char*) { return nullptr; }
void ph_9E50(void*, void*) {}
void* ph_9EC0_1(void*) { return nullptr; }
void ph_CEE0(void*, int) {}
void* ph_E010(void*, void*, const char*) { return nullptr; }
void ph_E088(void*, void*, void*, float, int) {}
void ph_EF40(void*, void*) {}
void ph_ForwardTarget(void*) {}
// pongAttractState_Shutdown — moved to src/game/char_view/pong_attract_state_shims.cpp
void pongCameraMgr_3500_fw(pongCameraMgr*, void*, int) {}
void pongCameraMgr_3650_fw(pongCameraMgr*, void*, int) {}
void pongCameraMgr_6E08(void*, TransitionParams*, int, void*, int) {}
void pongCameraMgr_B9B0_g(void*, int) {}
void pongCameraMgr_ValidateTransition(void*, TransitionParams*, int, TransitionFlags*, int) {}
// pongPlayer_{42E0_g,73E8_g,AB48_g} — defined in src/game/player/pong_player_swing.cpp
void rage_free(void* p) { free(p); }
void sysCallback_Invoke(void*, int) {}
// xmlNodeStruct_Initialize — moved to src/rage/data/par_xml_types.cpp.

// hsmState methods are in stubs_linker_2.cpp
// rage::ReleaseSingleton is in stubs_linker_3.cpp

// ── C-linkage wrappers for C callers ────────────────────────────────────────
extern "C" {
// _c_PostStateTransitionRequest — lifted in src/rage/core/hsm.cpp
void _c_RtlEnterCriticalSection(void* cs) __asm__("_RtlEnterCriticalSection");
void _c_RtlEnterCriticalSection(void* cs) { RtlEnterCriticalSection(cs); }
void _c_RtlLeaveCriticalSection(void* cs) __asm__("_RtlLeaveCriticalSection");
void _c_RtlLeaveCriticalSection(void* cs) { RtlLeaveCriticalSection(cs); }
// _atArray_Clear / _atHashMap_Find / _xmlNodeStruct_Initialize wrappers —
//   moved alongside their implementations in src/rage/data/at_types.cpp
//   and src/rage/data/par_xml_types.cpp.
void _c_audControl_Destructor(void* o) __asm__("_audControl_Destructor");
void _c_audControl_Destructor(void* o) { audControl_Destructor(o); }
void _c_hsmContext_SetNextState(void* c, int s) __asm__("_hsmContext_SetNextState_2800");
void _c_hsmContext_SetNextState(void* c, int s) { hsmContext_SetNextState(c, s); }
// _c_pgPageGroup_DispatchEvent (_pg_6F68 alias) → src/rage/swf.cpp
void _c_atHashMap_Find(void* a, const void* b) __asm__("_atHashMap_Find");
void _c_atHashMap_Find(void* a, const void* b) { atHashMap_Find(a,b); }
void _c_sysCallback_Invoke(void* c, int code) __asm__("_sysCallback_Invoke");
void _c_sysCallback_Invoke(void* c, int code) { sysCallback_Invoke(c, code); }
}

// ── C++ signatures that differ from stubs_final ─────────────────────────────
// pg_9C00_g(void*, int) → src/rage/swf.cpp
void pongBallInstance_4980_g(void* a, int b, int c, int d, int e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
void rage_RegisterUIContext(void* a, unsigned b, const char* c) { (void)a;(void)b;(void)c; }
namespace rage { struct phJoint3Dof; void phBoundCapsule_01D0_g(phJoint3Dof* j, float f) { (void)j;(void)f; } }

namespace rage { class atSingleton; void ReleaseSingleton(atSingleton*) {} }

// pongAttractState base methods (Reset/Shutdown/GetName/ProcessInput) —
// moved to src/game/char_view/pong_attract_state_shims.cpp.

