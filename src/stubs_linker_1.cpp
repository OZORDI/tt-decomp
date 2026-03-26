/**
 * stubs_linker_1.cpp - Stub implementations for 98 undefined linker symbols
 * Rockstar Presents Table Tennis (Xbox 360) decomp
 *
 * Each signature is derived from extern declarations found in the source tree.
 * These stubs allow the project to link while the real implementations are
 * being lifted from the original binary.
 *
 * Symbols already defined elsewhere in the build:
 *   rage_dat.cpp:  datParser_rtti_74A0_0, datParserRecord_vfn_0,
 *                  datSerialize_vfn_0, dcamPolarCam_vfn_0..5,10
 *   fsmMachine.c:  fsmMachine_Destructor_27A8
 */

#include <cstdint>
#include <cstddef>

// Forward declarations used by extern signatures
struct vec3;

// ============================================================================
// Global variables (43 symbols)
// C linkage required: .c files reference these with C-mangled names (_g_xxx)
// ============================================================================

extern "C" {

const float    g_alphaMultiplier             = 0.0f;  // @ 0x82079BE0
const float    g_animBlendWeight             = 0.0f;  // @ 0x8202D110
const float    g_contactZoneMaxA             = 0.0f;
const float    g_contactZoneMaxB             = 0.0f;
const float    g_contactZoneMinA             = 0.0f;
const float    g_contactZoneMinB             = 0.0f;
const float    g_defaultDuration             = 0.0f;  // @ 0x825C755C
const float    g_default_float_value         = 0.0f;  // @ 0x825CAF88
const float    g_default_float_value_2       = 0.0f;  // @ 0x825CAF90
const float    g_default_float_zero          = 0.0f;  // @ 0x825CAF94
const float    g_floatConstant1              = 0.0f;  // @ 0x82079AD4
const float    g_floatConstant2              = 0.0f;  // @ 0x8202D110
const float    g_floatConstant3              = 0.0f;  // @ 0x8202D110
const float    g_floatConstant4              = 0.0f;  // @ 0x825C7600
const float    g_floatEpsilon                = 0.0f;  // @ 0x8202D110
const float    g_floatNegOne                 = 0.0f;  // @ 0x8202D110
const float    g_floatOne                    = 0.0f;  // @ 0x8202D110
const float    g_floatZero                   = 0.0f;  // @ 0x8202D110
const float    g_kAdjustmentThreshold        = 0.0f;  // @ 0x8202D110
const float    g_kInputScale                 = 0.0f;  // @ 0x825C5938
const float    g_kInputThresholdHigh         = 0.0f;  // @ 0x8202D110
const float    g_kMaxAdjustment              = 0.0f;  // @ 0x82079CD8
const float    g_kQuantScale1                = 0.0f;  // @ 0x82079FFC
const float    g_kQuantScale2                = 0.0f;  // @ 0x82079FF8
const float    g_kSpecialThreshold           = 0.0f;  // @ 0x825C8A50
const float    g_kZeroThreshold              = 0.0f;  // @ 0x8202D108
const float    g_lookAtMultiplier            = 0.0f;  // @ 0x8202D184
const float    g_lookAtRefAngle              = 0.0f;  // @ 0x8202D10C
const float    g_msgSinkInitFloat            = 0.0f;  // @ 0x825C5938
const uint32_t g_msgSinkTemplate[4]          = {0};   // @ 0x8241A0C0
const float    g_recoveryScale               = 0.0f;
const char     g_szCreateMachineRTTI[]       = "";    // @ 0x825DEEB0
const char     g_szDataRequestTypeName[]     = "";    // @ 0x8206EA74
const char     g_szDataSendTypeName[]        = "";    // @ 0x8206EA88
const char     g_szDefaultError[]            = "";    // @ 0x82027423
const char     g_szInvalidError[]            = "";    // @ 0x8204EF04
const char     g_szMessageType[]             = "";    // @ 0x8205DFB0
const char     g_szNetworkDebugMsg[]         = "";    // @ 0x8205AE98
const char     g_szRageRTTI[]               = "";    // @ 0x825DEECC
const char     g_szTopSpin[]                 = "";    // @ 0x8206CB54
const char     g_szTournamentCompleteTypeName[] = ""; // @ 0x8206EC40
const float    g_zeroConstant                = 0.0f;  // @ 0x8202D110
const float    g_zeroFloat                   = 0.0f;  // @ 0x82079AD8

} // extern "C"

// ============================================================================
// WriteInt8Bits - network bit packing
// ============================================================================

void WriteInt8Bits(void* client, int16_t value, int bits) {
    (void)client; (void)value; (void)bits;
}

// ============================================================================
// atArray / atSingleton helpers
// ============================================================================

void atArray_Clear(void* obj) {
    (void)obj;
}

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
// Audio
// ============================================================================

void audControl_Destructor(void* obj) {
    (void)obj;
}

// ============================================================================
// charViewCS virtual methods
// Minimal self-contained class definition for the methods we need to stub.
// The full char_view.hpp header has compilation issues (incomplete atSingleton),
// so we declare the class with just the virtuals we need to define.
// ============================================================================

class charViewCS {
public:
    virtual ~charViewCS() = default;
    virtual void vfn_3();
    virtual void vfn_4();
    virtual void vfn_6();
    virtual void vfn_7();
    virtual void vfn_9();
    virtual void vfn_10();
    virtual void vfn_11();
};

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
