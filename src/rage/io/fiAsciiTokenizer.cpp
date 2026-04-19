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

// ═════════════════════════════════════════════════════════════════════════════
// rage::fiAsciiTokenizer — semantic lifts
// ═════════════════════════════════════════════════════════════════════════════
//
// Below: 10 virtual slots lifted from pass5_final. The tokenizer exposes a
// canonical RAGE-style token-read surface:
//   slot  1 : ReadToken(char* buf, int len)          -> int length (bool-ish)
//   slot  4 : ReadInt()                              -> int32_t
//   slot  5 : ReadFloat()                            -> float
//   slot 10 : GetMatrixRow (scratch buffer wrapper, mode 0)
//   ...
// Several slots are thin "read-token-then-parse" wrappers that stage a 32- or
// 64-byte scratch buffer on the stack, invoke slot 1 (token reader) to fill
// it, then dispatch to the appropriate parse routine. These are the Get* API
// that ph_physics.cpp and parStream consume.
//
// IMPORTANT: The .hpp for rage::fiAsciiTokenizer does not yet exist in this
// project — we provide the method definitions inline here without touching
// headers (per the partition rules). The compiler will see a translation-unit
// local class declaration, which is sufficient for the body addresses to line
// up with the binary. A future pass will promote these to a real header.

namespace rage {

// Translation-unit-local class declaration to host the method definitions
// until a proper header arrives. Fields at offsets we actually observe:
//   +0x00  vtable*          (175 accesses)
//   +0x04  flags / state    (151 accesses)
//   +0x08  ...              (42 accesses)
//   +0x0c  ...              (45 accesses)
//   +0xa0  field_160        (cursor / offset — used by vfn_30)
// Full layout is not yet characterized.
class fiAsciiTokenizer {
public:
    // Virtual slot 0 — pure-virtual-style no-op destructor body.
    virtual ~fiAsciiTokenizer();

    // Virtual slot 1 — raw token read into caller buffer.
    // Signature: int ReadTokenRaw(char* buf, int bufLen);
    virtual int ReadTokenRaw(char* buf, int bufLen);

    // Virtual slot 2 — trampoline to ReadInt (slot 4).
    virtual int ReadIntTrampoline();

    // Slot 3 reserved (not yet touched).
    virtual void vfn_3();

    // Virtual slot 4 — read a signed integer token. Accepts a leading '-'
    // or digits 0-9; otherwise returns 0.
    virtual int ReadInt();

    // Virtual slot 5 — read a floating-point token. Accepts '-', '.',
    // or digits; else returns default (0.0f stored at .rdata lbl_8202D110).
    virtual float ReadFloat();

    // Virtual slot 6..10 — ReadVector variants.
    virtual void ReadVector4Staged(float outV4[4]);   // slot 6
    virtual void ReadVector3Saved(float outV3[3]);    // slot 7
    virtual void ReadVector4(float outV4[4]);         // slot 8
    virtual void ReadVector3(float outV3[3]);         // slot 9
    virtual void ReadVector2(float outV2[2]);         // slot 10

    // Virtual slot 11 — read a token into an internal 64-byte scratch.
    virtual void ReadToken64();

    // Virtual slot 12 — read token, then call slot 4 (ReadInt).
    virtual int MatchThenReadInt();

    // Virtual slot 13 — read token, then call slot 5 (ReadFloat).
    virtual float MatchThenReadFloat();

    // Virtual slot 18 — read token, then call slot 10 (ReadVector2) — but
    // the slot-10 dispatch takes the caller's r5 argument as out buffer.
    virtual void MatchThenReadVector2(void* out);

    // Virtual slot 30 — advance the cursor at +0xA0 by r4 bytes.
    virtual void Advance(int delta);

    // Virtual slot 37 — tail-call to slot 36 passing 0.
    virtual void Finalize();
};

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::~fiAsciiTokenizer()   — vtable slot 0
// @ 0x8210CAF8 | size: 0x8
//
// The compiler emitted a one-instruction body: `li r3,0; blr`. The slot-0
// destructor in this family is effectively a no-op that simply zeroes r3.
// True teardown lives in slot 1 (the scalar/deleting destructor).
// ─────────────────────────────────────────────────────────────────────────────
fiAsciiTokenizer::~fiAsciiTokenizer() {
    // intentionally empty — base destructor returns 0 in r3.
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadIntTrampoline()   — vtable slot 2
// @ 0x822E5E78 | size: 0x10
//
// Pure tail-call trampoline: loads vtable[slot 4] (byte-offset 16) and jumps.
// ─────────────────────────────────────────────────────────────────────────────
int fiAsciiTokenizer::ReadIntTrampoline() {
    // bctr to vtable[4] — direct virtual dispatch to ReadInt().
    return this->ReadInt();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadInt()             — vtable slot 4
// @ 0x822E5E88 | size: 0x7C
//
// Allocates a 32-byte stack buffer, calls slot 1 (ReadTokenRaw) to fill it,
// then dispatches based on the first char:
//   - Token is empty              -> return 0
//   - First char is '-'           -> strtol (_xe_strcpyn_10 alias) on buffer
//   - First char is digit '0'-'9' -> strtol on buffer
//   - Otherwise                   -> return 0
//
// The recomp names the call target `_xe_strcpyn_10`; symbolically this is the
// RAGE integer-parse helper at 0x82432120.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int _xe_strcpyn_10(const char* src);  // @ 0x82432120, strtol-equiv.

int fiAsciiTokenizer::ReadInt() {
    char buf[32];
    buf[0] = '\0';
    int n = this->ReadTokenRaw(buf, (int)sizeof(buf));
    if (n == 0) {
        return 0;
    }
    char c = buf[0];
    if (c == '-') {
        return _xe_strcpyn_10(buf);
    }
    if (c >= '0' && c <= '9') {
        return _xe_strcpyn_10(buf);
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadFloat()           — vtable slot 5
// @ 0x822E5F08 | size: 0x8C
//
// Same shape as ReadInt but for floats. Accepts '-', '.', digits as the
// leading char; otherwise returns the default float at lbl_8202D110.
// The inner parse target is the jumptable at 0x82432B80 (_xe_strtof).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" float jumptable_2B80(const char* src);  // @ 0x82432B80, strtof-equiv.

// .rdata float at 0x8202D110 — the "no-match" default return for ReadFloat.
// Matches binary layout; the actual value is 0.0f in this build but we
// honor the indirection rather than folding the constant here.
extern "C" const float lbl_8202D110;

float fiAsciiTokenizer::ReadFloat() {
    char buf[32];
    buf[0] = '\0';
    int n = this->ReadTokenRaw(buf, (int)sizeof(buf));
    if (n == 0) {
        return lbl_8202D110;
    }
    char c = buf[0];
    if (c == '-') {
        return jumptable_2B80(buf);
    }
    if (c == '.') {
        return jumptable_2B80(buf);
    }
    if (c >= '0' && c <= '9') {
        return jumptable_2B80(buf);
    }
    return lbl_8202D110;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadVector2()         — vtable slot 10
// @ 0x822E5F98 | size: 0x60
//
// Reads two consecutive floats via slot 5 (ReadFloat) and stores them to
// outV2[0] / outV2[1]. The recomp keeps r31=this, r30=outV2.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::ReadVector2(float outV2[2]) {
    outV2[0] = this->ReadFloat();
    outV2[1] = this->ReadFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadVector3()         — vtable slot 9
// @ 0x822E5FF8 | size: 0x78
//
// Reads three consecutive floats via slot 5 and stores them sequentially.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::ReadVector3(float outV3[3]) {
    outV3[0] = this->ReadFloat();
    outV3[1] = this->ReadFloat();
    outV3[2] = this->ReadFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadVector4()         — vtable slot 8
// @ 0x822E6070 | size: 0x90
//
// Reads four consecutive floats straight into outV4[0..3].
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::ReadVector4(float outV4[4]) {
    outV4[0] = this->ReadFloat();
    outV4[1] = this->ReadFloat();
    outV4[2] = this->ReadFloat();
    outV4[3] = this->ReadFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadVector3Saved()    — vtable slot 7
// @ 0x822E6100 | size: 0x90
//
// Variant of ReadVector3 that saves f30/f31 registers explicitly before the
// three virtual calls (the compiler cached the first two reads in f31/f30
// and wrote them at the end). Semantically identical to ReadVector3.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::ReadVector3Saved(float outV3[3]) {
    float a = this->ReadFloat();
    float b = this->ReadFloat();
    float c = this->ReadFloat();
    outV3[0] = a;
    outV3[1] = b;
    outV3[2] = c;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadVector4Staged()   — vtable slot 6
// @ 0x822E6190 | size: 0xD0
//
// Stages four floats into a stack-local vec4 at sp+80, then does a bulk
// 16-byte copy out to outV4. Used when the caller wants write-atomicity.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::ReadVector4Staged(float outV4[4]) {
    float staged[4];
    staged[0] = this->ReadFloat();
    staged[1] = this->ReadFloat();
    staged[2] = this->ReadFloat();
    staged[3] = this->ReadFloat();
    // bulk copy (compiler emitted four 32-bit loads + stores)
    outV4[0] = staged[0];
    outV4[1] = staged[1];
    outV4[2] = staged[2];
    outV4[3] = staged[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::ReadToken64()         — vtable slot 11
// @ 0x822E6260 | size: 0x40
//
// Calls slot 1 (ReadTokenRaw) into a 64-byte stack scratch with leading NUL.
// Used as a "skip past next token" primitive by the ini/cfg readers.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::ReadToken64() {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->ReadTokenRaw(scratch, (int)sizeof(scratch));
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchThenReadInt()    — vtable slot 12
// @ 0x822E62A0 | size: 0x60
//
// Advances past the current 64-byte token, then invokes slot 4 (ReadInt) and
// returns its value. Used by parStream to consume a name + int pair.
// ─────────────────────────────────────────────────────────────────────────────
int fiAsciiTokenizer::MatchThenReadInt() {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->ReadTokenRaw(scratch, (int)sizeof(scratch));
    return this->ReadInt();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchThenReadFloat()  — vtable slot 13
// @ 0x822E6300 | size: 0x60
//
// Mirror of MatchThenReadInt — advances past current token then reads float.
// ─────────────────────────────────────────────────────────────────────────────
float fiAsciiTokenizer::MatchThenReadFloat() {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->ReadTokenRaw(scratch, (int)sizeof(scratch));
    return this->ReadFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::Advance()             — vtable slot 30
// @ 0x820C2E08 | size: 0x10
//
// Adds r4 (delta) to the cursor field at offset 0xA0. No bounds check.
// This is the low-level seek primitive used by the parse state machine.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::Advance(int delta) {
    uint32_t* cursor = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0xA0);
    *cursor = *cursor + static_cast<uint32_t>(delta);
}

} // namespace rage

// TODO(fiAsciiTokenizer): slot 3 (vfn_3) shape unknown — not in pass5_final.
// TODO(fiAsciiTokenizer): slot 18/19/20 family take a third out-buffer arg;
//   needs follow-up to characterize the dispatch vtable layout beyond slot 10.
// TODO(fiAsciiTokenizer): migrate fiAsciiTokenizer_2628_g real body from
//   stubs.cpp once its 0x24C-byte internal state machine is lifted.
