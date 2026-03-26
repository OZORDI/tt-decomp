/**
 * stubs_linker_3.cpp — Stub implementations for linker chunk 3 symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 */

#include <cstddef>
#include <cstdint>
#include <cstdarg>

// Forward declarations for types used by stubs
struct pcrPostPointBlender;
struct pongCameraMgr;
struct TransitionParams;
struct TransitionFlags;

// ============================================================================
// msgMsgSink — Network message sink helpers
// ============================================================================

// REMOVED (conflict): extern "C" uint32_t msgMsgSink_5098_g(void* obj, bool flag) {

// REMOVED (conflict): extern "C" void msgMsgSink_6138_2hr(void* obj) {

// REMOVED (conflict): extern "C" void msgMsgSink_6220_w(void* obj) {

// REMOVED (conflict): extern "C" int32_t msgMsgSink_84C0_gen(void* obj) {

// REMOVED (conflict): extern "C" void msgMsgSink_A970_2h(void* obj) {

// REMOVED (conflict): extern "C" void msgMsgSink_D168_sp(void* obj) {

// REMOVED (conflict): extern "C" void msgMsgSink_D1D8_sp(void* obj) {

// REMOVED (conflict): extern "C" void msgMsgSink_DB70_g(void* obj, uint32_t flags) {

// REMOVED (conflict): extern "C" void msgMsgSink_E860_g(void* obj, uint32_t a, uint32_t b, uint32_t c) {

// REMOVED (conflict): extern "C" void* msgMsgSink_F518_wrh(uint32_t param) {

// ============================================================================
// net — Network helpers
// ============================================================================

// REMOVED (conflict): extern "C" void net_6BA0_fw(void* session, void* param1, void* param2) {

// ============================================================================
// nop_8240E6D0 — C++ overloads (variadic debug log no-ops)
// ============================================================================

// No-arg overload
void nop_8240E6D0() {
}

// char const* overload (C++ mangled — separate from extern "C" in stubs_linker.cpp)
void nop_8240E6D0(const char* fmt, ...) {
    (void)fmt;
}

// void const* overload
void nop_8240E6D0(const void* ptr, ...) {
    (void)ptr;
}

// ============================================================================
// pcrAnimBlender / pcrPostPointBlender — Animation blending
// ============================================================================

// pcrAnimBlender_ApplyClipWeights already implemented in anim_blender_init.cpp
// pcrAnimState_ComputePosition already implemented in anim_blender_init.cpp

// REMOVED (conflict): extern "C" void pcrAnimBlender_Initialize(void* obj) {

// REMOVED (conflict): extern "C" int32_t pcrPostPointBlender_GetActiveClipIndex(pcrPostPointBlender* self) {

// ============================================================================
// pg — Page Group helpers
// ============================================================================

// pg_6F68 already stubbed in stubs_linker.cpp

// REMOVED (conflict): extern "C" void* pg_9C00_g(void* singleton) {

// Overloaded pg_9C00_g(void*, int) — C++ mangled
// REMOVED (conflict): void* pg_9C00_g(void* player, int index) {

extern "C" uint8_t pg_ApplyTransition(void* gameState, TransitionParams* params,
    int mode, TransitionFlags* flags, int flagCount)
{
    (void)gameState; (void)params; (void)mode; (void)flags; (void)flagCount;
    return 0;
}

// REMOVED (conflict): extern "C" void pg_E6E0(int code, int param1, int param2, int param3) {

// ============================================================================
// phArchetype — Physics archetype loading
// ============================================================================

// REMOVED (conflict): extern "C" void* phArchetype_Load(const char* path, void* materialInfo) {

// ============================================================================
// phBoundCapsule — Capsule bound math
// ============================================================================

// REMOVED (conflict): extern "C" float phBoundCapsule_01D0_g(float value) {

// REMOVED (conflict): extern "C" float phBoundCapsule_01D8_g(float value) {

// REMOVED (conflict): extern "C" float phBoundCapsule_02B0_g(float value) {

// REMOVED (conflict): extern "C" float phBoundCapsule_0FE0_g(float f1, float f2) {

// REMOVED (conflict): extern "C" void phBoundCapsule_5138_g(void* capsule, void* params, void* data) {

// REMOVED (conflict): extern "C" void phBoundCapsule_A080_g(void* bound) {

// ============================================================================
// phInst — Physics instance helpers
// ============================================================================

// REMOVED (conflict): extern "C" void phInst_A3A0_p33(void* obj) {

// phMaterialMgrImpl_C208_g already stubbed in stubs_linker.cpp

// ============================================================================
// phObject — Physics object virtual methods
// ============================================================================

#include "physics/ph_physics.hpp"

void* phObject::CreateViews(void* param) {
    (void)param;
    return nullptr;
}

void phObject::ReleaseResources() {
}

void* phObject::vfn_1(void* a) {
    (void)a;
    return nullptr;
}

void phObject::vfn_2() {
}

void phObject::vfn_3() {
}

void phObject::vfn_4() {
}

void* phObject::vfn_5() {
    return nullptr;
}

void* phObject::vfn_6() {
    return nullptr;
}

void* phObject::vfn_7() {
    return nullptr;
}

void* phObject::vfn_8() {
    return nullptr;
}

void* phObject::vfn_9() {
    return nullptr;
}

void* phObject::vfn_10() {
    return nullptr;
}

void* phObject::vfn_21() {
    return nullptr;
}

void* phObject::vfn_22() {
    return nullptr;
}

void* phObject::vfn_23() {
    return nullptr;
}

void* phObject::vfn_24() {
    return nullptr;
}

void* phObject::vfn_25() {
    return nullptr;
}

void* phObject::vfn_26() {
    return nullptr;
}

void* phObject::vfn_27() {
    return nullptr;
}

void* phObject::vfn_28() {
    return nullptr;
}

void* phObject::vfn_29() {
    return nullptr;
}

void* phObject::vfn_30() {
    return nullptr;
}

void* phObject::vfn_31() {
    return nullptr;
}

// ============================================================================
// ph_ — Physics utility functions
// ============================================================================

// REMOVED (conflict): extern "C" void ph_59C8(void* loader, const char* name, int flag) {

// REMOVED (conflict): extern "C" void* ph_6FC8(void* system, const char* path) {

// REMOVED (conflict): extern "C" void ph_9E50(void* archetype, void* material) {

// REMOVED (conflict): extern "C" void* ph_9EC0_1(void* memory) {

// REMOVED (conflict): extern "C" void ph_CEE0(void* material, int param) {

// REMOVED (conflict): extern "C" void* ph_E010(void* world, void* archetype, const char* path) {

// REMOVED (conflict): extern "C" void ph_E088(void* world, void* archetype, void* material, float scale, int flags) {

// REMOVED (conflict): extern "C" void ph_EF40(void* list, void* node) {

// REMOVED (conflict): extern "C" void ph_ForwardTarget(void* target) {

// ============================================================================
// plrPlayerMgr — Player manager virtual methods
// ============================================================================

#include "game/data/gd_data.hpp"

void plrPlayerMgr::PostLoadChildren() {
}

void plrPlayerMgr::PostLoadProperties() {
}

void plrPlayerMgr::Validate() {
}

void plrPlayerMgr::vfn_23() {
}

void plrPlayerMgr::vfn_24() {
}

void plrPlayerMgr::vfn_25() {
}

void plrPlayerMgr::vfn_27() {
}

void plrPlayerMgr::vfn_28() {
}

void plrPlayerMgr::vfn_29() {
}

void plrPlayerMgr::vfn_30() {
}

void plrPlayerMgr::vfn_31() {
}

void plrPlayerMgr::vfn_32() {
}

// ============================================================================
// DISABLED: // pongAttractState — Attract state virtual methods
// ============================================================================

// DISABLED: // pong_states.hpp struct and char_view.hpp class both define pongAttractState.
// Use pong_states.hpp since it declares the exact method names the linker needs.
#include "game/match/pong_states.hpp"

// DISABLED: void pongAttractState::GetName() {
}

// DISABLED: void pongAttractState::ProcessInput() {
}

// DISABLED: void pongAttractState::Reset() {
}

// DISABLED: void pongAttractState::Shutdown() {
}

// DISABLED: // REMOVED (conflict): extern "C" void pongAttractState_Shutdown(void* state) {

// ============================================================================
// pongBallInstance — Ball instance virtual methods
// ============================================================================

#include "game/ball/pong_ball.hpp"

void pongBallInstance::vfn_2() {
}

void pongBallInstance::vfn_24() {
}

void pongBallInstance::vfn_3() {
}

void pongBallInstance::vfn_30() {
}

void pongBallInstance::vfn_4() {
}

void pongBallInstance::vfn_5() {
}

// REMOVED (conflict): extern "C" void pongBallInstance_4980_g(void* mgr, int a, int b, int c, int d) {

// ============================================================================
// pongCameraMgr — Camera manager helpers
// ============================================================================

// REMOVED (conflict): extern "C" bool pongCameraMgr_3500_fw(pongCameraMgr* mgr, void* gameState, int mode) {

// Two overloads: typed (pongCameraMgr*, void*, int) and untyped (void*, void*, int)
bool pongCameraMgr_35A0_fw(pongCameraMgr* mgr, void* gameState, int mode) {
    (void)mgr; (void)gameState; (void)mode;
    return false;
}

bool pongCameraMgr_35A0_fw(void* mgr, void* gameState, int mode) {
    (void)mgr; (void)gameState; (void)mode;
    return false;
}

// REMOVED (conflict): extern "C" bool pongCameraMgr_3650_fw(pongCameraMgr* mgr, void* gameState, int mode) {

extern "C" uint8_t pongCameraMgr_6E08(void* gameState, TransitionParams* params,
    int mode, void* extra, int extraCount)
{
    (void)gameState; (void)params; (void)mode; (void)extra; (void)extraCount;
    return 0;
}

// REMOVED (conflict): extern "C" void* pongCameraMgr_B9B0_g(void* mgr, int size) {

extern "C" uint8_t pongCameraMgr_ValidateTransition(void* gameState, TransitionParams* params,
    int mode, TransitionFlags* flags, int flagCount)
{
    (void)gameState; (void)params; (void)mode; (void)flags; (void)flagCount;
    return 0;
}

// pongCharViewContext virtual methods are in stubs_linker_3b.cpp
// (separate TU to avoid header conflict with pong_states.hpp)
