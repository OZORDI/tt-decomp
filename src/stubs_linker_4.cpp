/**
 * stubs_linker_4.cpp — Stub implementations for linker chunk 4 symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 */

#include <cstddef>
#include <cstdint>

// ── Headers for class method stubs ──────────────────────────────────────────
// NOTE: game/char_view.hpp is not included because its static_asserts fail
// on 64-bit hosts.  We re-declare the minimal class shape below.
#include "game/pong_render.hpp"          // pongDrawBucket
#include "game/network/pong_network.hpp" // pongNetMessageHolder
#include "game/player/pong_player.hpp"   // pongPlayer
#include "graphics/texture_reference.hpp" // grcTextureReferenceBase

// Forward declarations for types used only in free-function signatures
struct vec3;

// ── Minimal re-declaration of pongCharViewContext for method stubs ───────────
// Full declaration lives in include/game/char_view.hpp.
// We only need the class name and the virtual methods we're stubbing.
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

// ============================================================================
// pongCharViewContext — virtual method stubs (3)
// ============================================================================

void pongCharViewContext::vfn_17() {}
void pongCharViewContext::vfn_18() {}
void pongCharViewContext::vfn_23() {}

// ============================================================================
// pongCreature / pongCreatureInst helpers (3)
// ============================================================================

void pongCreatureInst_9030_g(void* a, void* b) {
    (void)a; (void)b;
}

void pongCreatureInst_EDC0_g(void* dst, void* src, void* helper) {
    (void)dst; (void)src; (void)helper;
}

void pongCreature_7CE8_g(void* creature, void* matrix, int p1, int p2, int p3, int p4) {
    (void)creature; (void)matrix; (void)p1; (void)p2; (void)p3; (void)p4;
}

// ============================================================================
// pongDrawBucket (2)
// ============================================================================

void pongDrawBucket::InitStatics() {}

void pongDrawBucket::Load(const char* name, int maxEntries, void* bucketData) {
    (void)name; (void)maxEntries; (void)bucketData;
}

// ============================================================================
// pongLerpQueue (1)
// ============================================================================

void pongLerpQueue_3410_g(void* queue) {
    (void)queue;
}

// ============================================================================
// pongNetMessageHolder (18)
// ============================================================================

void pongNetMessageHolder::ScalarDtor(int flags) {
    (void)flags;
}

void pongNetMessageHolder_5038_w() {}

void pongNetMessageHolder_6778_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_68D0_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_6B48_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_6C98_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_6D68_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_6E30_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_6FF8_wrh(void* memory) { (void)memory; }
void pongNetMessageHolder_7700_wrh(void* memory) { (void)memory; }

pongNetMessageHolder* pongNetMessageHolder_FAE0_isl() {
    return nullptr;
}

void pongNetMessageHolder_vfn_2_0868_1(pongNetMessageHolder* holder) { (void)holder; }
void pongNetMessageHolder_vfn_2_1628_1(pongNetMessageHolder* holder) { (void)holder; }
void pongNetMessageHolder_vfn_2_1770_1(pongNetMessageHolder* holder) { (void)holder; }
void pongNetMessageHolder_vfn_2_18D0_1(pongNetMessageHolder* holder) { (void)holder; }
void pongNetMessageHolder_vfn_2_24B8_1(pongNetMessageHolder* holder) { (void)holder; }
void pongNetMessageHolder_vfn_2_3878_1(pongNetMessageHolder* holder) { (void)holder; }
void pongNetMessageHolder_vfn_2_FD70_1(pongNetMessageHolder* holder) { (void)holder; }

// ============================================================================
// pongPlayer (24 — 4 already defined in pong_player_swing.cpp)
// ============================================================================

void pongPlayer::Process(float dt) { (void)dt; }

int pongPlayer_0270_g(void* player) {
    (void)player;
    return 0;
}

void pongPlayer_0508_g(void* obj, int maxSteps, float* outDelta) {
    (void)obj; (void)maxSteps;
    if (outDelta) *outDelta = 0.0f;
}

void pongPlayer_1460_g(void* actionState, int released) {
    (void)actionState; (void)released;
}

// pongPlayer_42E0_g — already defined in pong_player_swing.cpp

bool pongPlayer_5B60_gen(pongPlayer* p) {
    (void)p;
    return false;
}

// Two overloads with different C++ mangled names
void pongPlayer_6470_g(float* out, void* player, int flags) {
    (void)out; (void)player; (void)flags;
}

void pongPlayer_6470_g(vec3* outVec, void* targetStruct) {
    (void)outVec; (void)targetStruct;
}

// pongPlayer_73E8_g — already defined in pong_player_swing.cpp
// pongPlayer_9CD0_g — already defined in pong_player_swing.cpp
// pongPlayer_AB48_g — already defined in pong_player_swing.cpp

void pongPlayer_ApplyNetStateImpl(void* player) { (void)player; }

void pongPlayer_B208_g(void* playerState, vec3* swingVec, unsigned char suppressFlip) {
    (void)playerState; (void)swingVec; (void)suppressFlip;
}

int pongPlayer_BF18_g(void* playerState, int r4, vec3* swingStrength, vec3* targetDir, int r5) {
    (void)playerState; (void)r4; (void)swingStrength; (void)targetDir; (void)r5;
    return 0;
}

void pongPlayer_C678_g(pongPlayer* state) { (void)state; }
void pongPlayer_D238_g(pongPlayer* state) { (void)state; }

bool pongPlayer_DE98_g(void* player) {
    (void)player;
    return false;
}

void pongPlayer_E590_g(void* adjustedVec, void* gridBase, void* p3, void* p4, void* p5) {
    (void)adjustedVec; (void)gridBase; (void)p3; (void)p4; (void)p5;
}

void pongPlayer_E7B0_g(vec3* outVec, void* targetData) {
    (void)outVec; (void)targetData;
}

void pongPlayer_FD20_g(void* self, int r4, void* p3, void* p4, int p5, void* p6, void* p7, unsigned char metadataByte) {
    (void)self; (void)r4; (void)p3; (void)p4; (void)p5; (void)p6; (void)p7; (void)metadataByte;
}

void pongPlayer_GetPlayerPosition(void* player, void* outPos) {
    (void)player; (void)outPos;
}

void pongPlayer_InitShotImpl(void* player) { (void)player; }

void pongPlayer_Interpolate(void* player, void* posData) {
    (void)player; (void)posData;
}

void pongPlayer_ResetMoverImpl(void* player) { (void)player; }

// ============================================================================
// pongScrnTransFadeIn (1)
// ============================================================================

void pongScrnTransFadeIn_EndTransition(void* obj) { (void)obj; }

// ============================================================================
// rage:: namespace — memory / locale (2)
// ============================================================================

namespace rage {

void* Allocate(unsigned long size, unsigned long alignment) {
    (void)size; (void)alignment;
    return nullptr;
}

// ReleaseSingleton(atSingleton*) — already defined in rage/memory/atSingleton.cpp

void _locale_register(void* ptr, void* allocator) {
    (void)ptr; (void)allocator;
}

} // namespace rage

// ============================================================================
// rage:: namespace — atSingleton helpers (2)
// ============================================================================

namespace rage {

void atSingleton_8068_h(void* ptr) { (void)ptr; }
void atSingleton_8A48_p42(void* obj) { (void)obj; }

} // namespace rage

// ============================================================================
// rage:: namespace — audio (2)
// ============================================================================

namespace rage {

void aud_1498(void* obj) { (void)obj; }
void aud_6A20_wrap_6A20(void* obj) { (void)obj; }

} // namespace rage

// ============================================================================
// rage:: namespace — fiAsciiTokenizer helpers (3)
// ============================================================================

namespace rage {

void fiAsciiTokenizer_0BA8_g(void* tokenizer, int mode) {
    (void)tokenizer; (void)mode;
}

uint8_t fiAsciiTokenizer_2110_gen(void* a, void* b, void* c) {
    (void)a; (void)b; (void)c;
    return 0;
}

void fiAsciiTokenizer_3310_g(void* obj) { (void)obj; }

} // namespace rage

// ============================================================================
// Global variables (28 — skip those already defined elsewhere)
// All in namespace rage to match linker symbol expectations.
// ============================================================================

namespace rage {

extern const float g_capsuleVolK1 = 1.5f;      // @ .rdata (fmadds multiplier)
extern const float g_capsuleVolK2 = 4.18879f;  // @ .rdata (~4/3*PI)

uint8_t  g_contextStack[128] = {};              // @ 0x82604C68
void*    g_currentGeometry = nullptr;           // @ 0x82606350
void*    g_currentSwfContext = nullptr;         // @ 0x8260281C

// g_display_obj_ptr — already defined in globals_extended.c

uint32_t* g_iccProfilePtr = nullptr;           // @ 0x825EB1D0
uint32_t  g_mmioStoredValue = 0;               // @ 0x825E7890

void*    g_phAllocator = nullptr;              // @ 0x64890018
// g_phCallback0-5 — already defined in globals_linker.cpp
uint32_t g_phCollisionFlags = 0;               // @ 0x825C48E8
uint16_t g_phDefaultStatusId = 0;              // @ 0x82465A58
void*    g_phGlobalState = nullptr;

// g_str_swf_unsupportedMethod — already defined in pong_strings.cpp

uint32_t g_swfCallDepth = 0;                   // @ 0x826064EC
float    g_swfFrameScale = 1.0f;               // @ 0x82079B90
void*    g_swfGlobalObject = nullptr;          // @ 0x82602818
void*    g_swfGlobalScope = nullptr;           // @ 0x8260282C
void*    g_swfStringBuffer = nullptr;          // @ 0x82604850

// g_swfScriptObjectPool — already defined in globals_extended.c
// g_tls_base — already defined in globals_linker.cpp

void*    g_vtable_cmApproach2 = nullptr;       // @ 0x82055EFC
void*    g_vtable_swfSCRIPTOBJECT = nullptr;   // @ 0x82074D14

} // namespace rage

// ============================================================================
// rage::grcTextureReferenceBase — virtual method stubs (8)
// ============================================================================

namespace rage {

void grcTextureReferenceBase::vfn_1() {}
void grcTextureReferenceBase::vfn_10() {}
void grcTextureReferenceBase::vfn_19() {}
void grcTextureReferenceBase::vfn_2() {}

grcTexture* grcTextureReferenceBase::vfn_21(void* a, void* b, void* c) {
    (void)a; (void)b; (void)c;
    return nullptr;
}

void grcTextureReferenceBase::vfn_22(void* a) { (void)a; }
void grcTextureReferenceBase::vfn_23() {}
void grcTextureReferenceBase::vfn_24() {}

} // namespace rage
