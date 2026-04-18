/**
 * fiAsciiTokenizer.cpp — proto-RAGE ASCII tokenizer bridging surface
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The fiAsciiTokenizer family provides the text-parsing backbone behind
 * rage_par.cpp (XML/ini driver), the physics archetype loader, and the
 * HSM config-file ingest path (render_loop.c).
 *
 * 126 fiAsciiTokenizer_* symbols exist in the binary. Individual decomps
 * will replace the weak bodies below as each one is lifted.
 *
 * Already address-backed in the project:
 *   fiAsciiTokenizer_2628_g @ 0x82432628 — floating-point token dispatch;
 *     real body lives in stubs.cpp and will migrate here in a follow-up.
 *
 * This file intentionally keeps the surface minimal and self-contained.
 */

#include <cstdint>

extern "C" {

// ─────────────────────────────────────────────────────────────────────────────
// Initialization / lifecycle
// ─────────────────────────────────────────────────────────────────────────────
// Called by render_loop.c with extra state params for HSM ingest. The C
// declaration there uses three args; the general shape takes one `this`.
// Keep the single-arg form (matches the stubs.cpp declaration).
void fiAsciiTokenizer_InitializeTokenizer(void* tokenizer) {
    (void)tokenizer;
}

// ─────────────────────────────────────────────────────────────────────────────
// Source-buffer attachment + drive loop
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer_SetString(void* tokenizer,
                                const char* begin,
                                const char* end) {
    (void)tokenizer; (void)begin; (void)end;
}

void fiAsciiTokenizer_Process(void* tokenizer, int mode) {
    (void)tokenizer; (void)mode;
}

// ─────────────────────────────────────────────────────────────────────────────
// Token read API — called from ph_physics.cpp archetype parser.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer_ReadToken(void* tokenizer, int mode) {
    (void)tokenizer; (void)mode;
}

uint8_t fiAsciiTokenizer_ReadBool(void* tokenizer, void* dst, void* key) {
    (void)tokenizer; (void)dst; (void)key;
    return 0;
}

void fiAsciiTokenizer_Destroy(void* tokenizer) {
    (void)tokenizer;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer_2628_g @ 0x82432628 — passthrough float accessor.
// Left as a weak body matching the prior stubs.cpp definition until the
// 0x24C-byte body is decompiled.
// ─────────────────────────────────────────────────────────────────────────────
float fiAsciiTokenizer_2628_g(float a) {
    (void)a;
    return 0.0f;
}

} // extern "C"
