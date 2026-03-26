// stubs_final.cpp — Final linker stubs with exact demangled signatures
#include <cstdlib>
#include <cstdint>
// hsmState declared inline to avoid include path issues
class hsmState {
public:
    virtual ~hsmState() = default;
    virtual const char* GetStateName() const { return ""; }
    virtual char* GetFullStatePath(char* buf, uint32_t bufSize) const;
    virtual void Reset();
protected:
    void* m_pManager;
    void* m_pParentState;
    void* m_pChildState;
    uint32_t m_field_10;
    uint32_t m_field_14;
};

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
void PostStateTransitionRequest(void*, int) {}
void RtlEnterCriticalSection(void*) {}
void RtlLeaveCriticalSection(void*) {}
void atArray_Clear(void*) {}
void audControl_Destructor(void*) {}
void fsmMachine_Destructor_27A8(void*) {}
void hsmContext_SetNextState_2800(void*, int) {}
void msgMsgSink_5098_g(void*, bool) {}
void msgMsgSink_6138_2hr(void*) {}
void msgMsgSink_6220_w(void*) {}
void msgMsgSink_84C0_gen(void*) {}
void msgMsgSink_A970_2h(void*) {}
void msgMsgSink_D168_sp(void*) {}
void msgMsgSink_D1D8_sp(void*) {}
void msgMsgSink_DB70_g(void*, unsigned) {}
void msgMsgSink_E860_g(void*, unsigned, unsigned, unsigned) {}
unsigned msgMsgSink_F518_wrh(unsigned) { return 0; }
void net_6BA0_fw(void*, void*, void*) {}
void pcrAnimBlender_ApplyClipWeights(void*, unsigned, float, float) {}
void pcrAnimBlender_Initialize(void*) {}
void pcrAnimState_ComputePosition(vec3*, pongAnimState*) {}
void pcrPostPointBlender_GetActiveClipIndex(pcrPostPointBlender*) {}
void pg_6F68(void*, void*, int, unsigned*, int) {}
void* pg_9C00_g(void*) { return nullptr; }
void pg_ApplyTransition(void*, TransitionParams*, int, TransitionFlags*, int) {}
void pg_E6E0(int, int, int, int) {}
void* phArchetype_Load(const char*, void*) { return nullptr; }
float phBoundCapsule_01D0_g(float v) { return v; }
float phBoundCapsule_01D8_g(float v) { return v; }
float phBoundCapsule_02B0_g(float v) { return v; }
float phBoundCapsule_0FE0_g(float a, float b) { (void)b; return a; }
void phBoundCapsule_5138_g(void*, void*, void*) {}
void phBoundCapsule_A080_g(void*) {}
void phInst_A3A0_p33(void*) {}
void phMaterialMgrImpl_C208_g(void*, const void*) {}
void ph_59C8(void*, const char*, int) {}
void* ph_6FC8(void*, const char*) { return nullptr; }
void ph_9E50(void*, void*) {}
void* ph_9EC0_1(void*) { return nullptr; }
void ph_CEE0(void*, int) {}
void* ph_E010(void*, void*, const char*) { return nullptr; }
void ph_E088(void*, void*, void*, float, int) {}
void ph_EF40(void*, void*) {}
void ph_ForwardTarget(void*) {}
void pongAttractState_Shutdown(void*) {}
void pongCameraMgr_3500_fw(pongCameraMgr*, void*, int) {}
void pongCameraMgr_3650_fw(pongCameraMgr*, void*, int) {}
void pongCameraMgr_6E08(void*, TransitionParams*, int, void*, int) {}
void pongCameraMgr_B9B0_g(void*, int) {}
void pongCameraMgr_ValidateTransition(void*, TransitionParams*, int, TransitionFlags*, int) {}
void pongPlayer_42E0_g(void*, vec3*, vec3*, float) {}
void pongPlayer_73E8_g(pongPlayer*, float) {}
void pongPlayer_AB48_g(vec3*, void*) {}
void rage_free(void* p) { free(p); }
void sysCallback_Invoke(void*, int) {}
void xmlNodeStruct_Initialize(void*) {}

// C++ linkage methods
char* hsmState::GetFullStatePath(char* buf, uint32_t bufSize) const {
    if (buf && bufSize > 0) buf[0] = '\0';
    return buf;
}

void hsmState::Reset() {}

namespace rage {
void ReleaseSingleton(atSingleton*) {}
}

// ── C-linkage wrappers for C callers ────────────────────────────────────────
extern "C" {
void _c_PostStateTransitionRequest(void* m, int e) __asm__("_PostStateTransitionRequest");
void _c_PostStateTransitionRequest(void* m, int e) { PostStateTransitionRequest(m, e); }
void _c_RtlEnterCriticalSection(void* cs) __asm__("_RtlEnterCriticalSection");
void _c_RtlEnterCriticalSection(void* cs) { RtlEnterCriticalSection(cs); }
void _c_RtlLeaveCriticalSection(void* cs) __asm__("_RtlLeaveCriticalSection");
void _c_RtlLeaveCriticalSection(void* cs) { RtlLeaveCriticalSection(cs); }
void _c_atArray_Clear(void* a) __asm__("_atArray_Clear");
void _c_atArray_Clear(void* a) { atArray_Clear(a); }
void _c_audControl_Destructor(void* o) __asm__("_audControl_Destructor");
void _c_audControl_Destructor(void* o) { audControl_Destructor(o); }
void _c_hsmContext_SetNextState_2800(void* c, int s) __asm__("_hsmContext_SetNextState_2800");
void _c_hsmContext_SetNextState_2800(void* c, int s) { hsmContext_SetNextState_2800(c, s); }
void _c_pg_6F68(void* a, void* b, int c, unsigned* d, int e) __asm__("_pg_6F68");
void _c_pg_6F68(void* a, void* b, int c, unsigned* d, int e) { pg_6F68(a,b,c,d,e); }
void _c_phMaterialMgrImpl_C208_g(void* a, const void* b) __asm__("_phMaterialMgrImpl_C208_g");
void _c_phMaterialMgrImpl_C208_g(void* a, const void* b) { phMaterialMgrImpl_C208_g(a,b); }
void _c_sysCallback_Invoke(void* c, int code) __asm__("_sysCallback_Invoke");
void _c_sysCallback_Invoke(void* c, int code) { sysCallback_Invoke(c, code); }
void _c_xmlNodeStruct_Initialize(void* o) __asm__("_xmlNodeStruct_Initialize");
void _c_xmlNodeStruct_Initialize(void* o) { xmlNodeStruct_Initialize(o); }
}

// ── C++ signatures that differ from stubs_final ─────────────────────────────
void* pg_9C00_g(void* p, int i) { (void)p;(void)i; return nullptr; }
void pongBallInstance_4980_g(void* a, int b, int c, int d, int e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
void rage_RegisterUIContext(void* a, unsigned b, const char* c) { (void)a;(void)b;(void)c; }
namespace rage { struct phJoint3Dof; void phBoundCapsule_01D0_g(phJoint3Dof* j, float f) { (void)j;(void)f; } }

// xe_EC88 C++ overloads (C version in heap.c takes uint32_t)
extern "C" void* xe_EC88(uint32_t size);
void* xe_EC88(int size) { return xe_EC88((uint32_t)size); }
void* xe_EC88(unsigned int size) { return xe_EC88((uint32_t)size); }
