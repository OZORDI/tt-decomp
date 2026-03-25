/**
 * stubs_linker_1.cpp - Stub implementations for 37 undefined linker symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 */

#include <cstdint>
#include <cstddef>

// Forward declarations used by extern signatures
struct vec3;

// ============================================================================
// WriteInt8Bits — network bit packing
// ============================================================================

void WriteInt8Bits(void* client, int16_t value, int bits) {
    (void)client; (void)value; (void)bits;
}

// ============================================================================
// atSingleton helpers — engine singleton / container utilities
// ============================================================================

void atSingleton_0128_wrh(void* pNode, uint32_t param) {
    (void)pNode; (void)param;
}

float atSingleton_1308_g(float value, double param) {
    (void)value; (void)param;
    return 0.0f;
}

void atSingleton_2038(void* array, uint32_t newCapacity) {
    (void)array; (void)newCapacity;
}

void atSingleton_22B0(void* array, uint32_t newCapacity) {
    (void)array; (void)newCapacity;
}

void atSingleton_23C0(void* pBitStream) {
    (void)pBitStream;
}

void atSingleton_29E8_p25(void* obj) {
    (void)obj;
}

void atSingleton_5CD0_fw(void* self, uint32_t param) {
    (void)self; (void)param;
}

void atSingleton_8E88_p42(void* handler, void* self, uint32_t index,
                           uint32_t newCount, uint32_t flags) {
    (void)handler; (void)self; (void)index; (void)newCount; (void)flags;
}

void* atSingleton_91E0_gen(uint32_t size) {
    (void)size;
    return nullptr;
}

void atSingleton_E998_g(void* ctx, void* base) {
    (void)ctx; (void)base;
}

// ============================================================================
// charViewCS virtual methods
// ============================================================================

// charViewCS is declared in include/game/char_view.hpp
#include "game/char_view.hpp"

void charViewCS::vfn_3()  {}
void charViewCS::vfn_4()  {}
void charViewCS::vfn_6()  {}
void charViewCS::vfn_7()  {}
void charViewCS::vfn_9()  {}
void charViewCS::vfn_10() {}
void charViewCS::vfn_11() {}

// ============================================================================
// CM (Control Machine) helpers
// ============================================================================

void cmMetafileTuningSet_vfn_8(void* pThis, uint32_t param1, uint32_t param2) {
    (void)pThis; (void)param1; (void)param2;
}

void cmOperator_5FC8_g(void* pOperator, uint32_t* pParam1, uint32_t* pParam2) {
    (void)pOperator; (void)pParam1; (void)pParam2;
}

// ============================================================================
// Animation
// ============================================================================

void crAnimBlenderState_Init(void* state) {
    (void)state;
}

// ============================================================================
// File I/O / Tokenizer
// ============================================================================

float fiAsciiTokenizer_2628_g(float input) {
    (void)input;
    return 0.0f;
}

// ============================================================================
// Game logic functions
// ============================================================================

void* game_1620(void* mem) {
    (void)mem;
    return nullptr;
}

void game_2E80(void* renderState) {
    (void)renderState;
}

void game_2EE0(void* renderState) {
    (void)renderState;
}

void game_36E8(void* ball, const float* velocity) {
    (void)ball; (void)velocity;
}

void game_3860(void* gameState, int param, const char* event) {
    (void)gameState; (void)param; (void)event;
}

void game_8EE8(void* obj) {
    (void)obj;
}

void* game_9358(void* mem) {
    (void)mem;
    return nullptr;
}

void game_9CF8_h(void* node, uint32_t param) {
    (void)node; (void)param;
}

void game_9D10_h(void* node, uint32_t param) {
    (void)node; (void)param;
}

void game_9D28_h(void* node, uint32_t param) {
    (void)node; (void)param;
}

void game_AA88(void* player) {
    (void)player;
}

void game_AAF8(void* roll, int a, int b) {
    (void)roll; (void)a; (void)b;
}

void* game_AD40(void* obj, uint32_t count) {
    (void)obj; (void)count;
    return nullptr;
}

void game_CD20(void* recoveryState) {
    (void)recoveryState;
}

void game_D060(void* recoveryState) {
    (void)recoveryState;
}

void game_D3B0_h(void* self) {
    (void)self;
}

void game_D468(void* obj, vec3* v) {
    (void)obj; (void)v;
}

void game_D500(void* activationContext, void* player, void* ball) {
    (void)activationContext; (void)player; (void)ball;
}

void game_DA60() {
}

// ============================================================================
// Graphics
// ============================================================================

void grc_2CC8(void* val, uint32_t a, uint32_t b) {
    (void)val; (void)a; (void)b;
}

void grc_EB10(void* deviceState) {
    (void)deviceState;
}
