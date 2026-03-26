/**
 * stubs_linker_5.cpp — Stub implementations for linker chunk 5 symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 */

#include "graphics/texture_reference.hpp"
#include "rage/rage_swf.hpp"
#include "physics/ph_physics.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

// Forward declarations for types not in headers
struct NetDataQuery;

// ============================================================================
// namespace rage — member functions & namespaced free functions
// ============================================================================

namespace rage {

// ---------- grcTextureReferenceBase virtual stubs ----------

void grcTextureReferenceBase::vfn_25() {}
void grcTextureReferenceBase::vfn_3() {}

// ---------- swfINSTANCE virtual stub ----------

void swfINSTANCE::vfn_7() {}

// ---------- phBoundOTGrid — collision detection methods ----------

int phBoundOTGrid::Collide3D(float a, float b, float c, float d, float e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
    return 0;
}

int phBoundOTGrid::Collide3DFull(float a, float b, float c, float d, float e, float f) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    return 0;
}

int phBoundOTGrid::Collide3DYExtent(float a, float b, float c, float d) {
    (void)a; (void)b; (void)c; (void)d;
    return 0;
}

int phBoundOTGrid::CollideCapsule(float a, float b, float c, float d,
                                   float e, float f, float g, float h) {
    (void)a; (void)b; (void)c; (void)d;
    (void)e; (void)f; (void)g; (void)h;
    return 0;
}

int phBoundOTGrid::CollideLineX(float a, float b, float c) {
    (void)a; (void)b; (void)c;
    return 0;
}

int phBoundOTGrid::CollidePlaneXY(float a, float b, float c) {
    (void)a; (void)b; (void)c;
    return 0;
}

int phBoundOTGrid::CollidePlaneXZ(float a, float b, float c, float d) {
    (void)a; (void)b; (void)c; (void)d;
    return 0;
}

// ---------- HUD / Flash ----------

void hudFlashBase_9CA8_h(void* a, void* b, void* c) {
    (void)a; (void)b; (void)c;
}

// ---------- Kernel engine helpers ----------

void ke_A428(void* obj) { (void)obj; }
void ke_C750(void* obj) { (void)obj; }
void ke_D440(void* obj) { (void)obj; }

// msgMsgSink_8DA0_sp — already implemented in msg_msg_sink.cpp

// ---------- phArticulatedCollider helpers ----------

void phArticulatedCollider_5B50_wrh(void* a) { (void)a; }
void phArticulatedCollider_5C60_sp(void* a) { (void)a; }
void phArticulatedCollider_6D30_h(void* a) { (void)a; }

// ---------- phBoundCapsule helpers ----------

// REMOVED (conflict): void phBoundCapsule_01D0_g(phJoint3Dof* j, float v) { (void)j; (void)v; }
void phBoundCapsule_6C28_fw(void* a) { (void)a; }

// ---------- phCollider / phInst / phJoint ----------

void phCollider_vfn_4(phArticulatedCollider* c) { (void)c; }
void phInst_5910_p39(void* a) { (void)a; }

float phJoint1Dof_AE38(void* a) { (void)a; return 0.0f; }
void phJoint1Dof_AFF8_p42(phJoint3Dof* j, int i, float* o) { (void)j; (void)i; (void)o; }
void phJoint3Dof_0170_g(void* a, int b, int c, void* d) { (void)a; (void)b; (void)c; (void)d; }
void phJoint_1388(phJoint3Dof* j, int i, float v, float* a, float* b) {
    (void)j; (void)i; (void)v; (void)a; (void)b;
}
void phJoint_1618_g(phJoint3Dof* j) { (void)j; }

// ---------- ph_* vtable thunks / physics free functions ----------

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

// ---------- rage core / allocation ----------

void rage_1058(void* a) { (void)a; }

// rage_Alloc 2-param overload (size + allocator)
void rage_Alloc(int size, void* allocator) { (void)size; (void)allocator; }

// ---------- utility functions in rage:: namespace ----------

void util_4628(phJoint3Dof* j, int p) { (void)j; (void)p; }
void* util_B188(void* o, int p) { (void)o; (void)p; return nullptr; }
void util_D988(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void util_DA90(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
void util_F850(void* a, void* b) { (void)a; (void)b; }

} // namespace rage

// ============================================================================
// C-linkage / global scope free functions
// ============================================================================

void rage_27C0(void* a) { (void)a; }

void* rage_Allocate(unsigned int size) { (void)size; return nullptr; }

void rage_C1A8(void* a) { (void)a; }

void rage_D220(void* a, const char* b) { (void)a; (void)b; }

void rage_FF70(void* a) { (void)a; }

// rage_RegisterUIContext — already implemented in ui_context.c
// rage_free — already implemented in crt/heap.c

void rage_threadpool_cleanup_6878() {}

void sgSceneGraph_FreeLists(void* a) { (void)a; }

void snHsmAcceptingJoinRequest_9A70(void* a, void* b) { (void)a; (void)b; }

void snSession_6C98_h(void* a) { (void)a; }

void snSession_AddNode_C068(void* a, void* b) { (void)a; (void)b; }

// strncpy — 3rd param is int in the mangled signature
char* strncpy(char* dest, const char* src, int n) {
    if (!dest) return dest;
    int i = 0;
    for (; i < n && src[i]; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = '\0';
    return dest;
}

void sub_821A8F58(void* a, const char* b, void* c, void* d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// sysCallback_Invoke — already stubbed in stubs_linker.cpp

// typeinfo for hsmState — RTTI data symbol
namespace {
    struct FakeTypeInfo { void* vtable; const char* name; };
}
extern "C" const FakeTypeInfo _ZTI8hsmState = { nullptr, "hsmState" };

void util_03C0(void* a, uint32_t* b) { (void)a; (void)b; }

// util_0AF0 has two overloads
void util_0AF0(void* a, void* b) { (void)a; (void)b; }
void util_0AF0(void* a, void* b, short c) { (void)a; (void)b; (void)c; }

int util_5A70(void* a, void* b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
    return 0;
}

void util_6C20(void* a, unsigned int b) { (void)a; (void)b; }

void util_7970(void* a, void* b, int c) { (void)a; (void)b; (void)c; }

void util_8FD0(void* a) { (void)a; }

void util_AA38(NetDataQuery* a) { (void)a; }

void util_B8A0(void* a) { (void)a; }

void util_C930(void* a) { (void)a; }

void util_CDF0(void* a, const void* b, unsigned int c) { (void)a; (void)b; (void)c; }

// util_D150 has two overloads
void util_D150(void* a, void* b) { (void)a; (void)b; }
void util_D150(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }

void util_DA08(void* a) { (void)a; }

void util_PackColorRGBA(uint32_t* a, const float* b) { (void)a; (void)b; }

void* xam_singleton_init_8D60(void* a) { (void)a; return nullptr; }

// xe_EC88 has two overloads (int / unsigned int)
// REMOVED: xe_EC88 (defined in heap.c)
// REMOVED: xe_EC88 overload

// xe_F4C0 has two overloads (void / void*)
void xe_F4C0() {}
void xe_F4C0(void* a) { (void)a; }

void xe_main_thread_init_0038() {}

void xmlNodeStruct_Init(void* a) { (void)a; }

// xmlNodeStruct_Initialize — already stubbed in stubs_linker.cpp

int xml_ReadInt(const char* a) { (void)a; return 0; }

void xml_ReadString(const char* a, char* b, int c) {
    (void)a;
    if (b && c > 0) b[0] = '\0';
}
