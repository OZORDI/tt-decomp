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
 *   fiAsciiTokenizer_2628_g @ 0x82432628 — floating-point token emitter
 *     (Printf-style "%g"-equivalent that classifies the value: NaN, inf,
 *     whole-number, small, or general, dispatching to one of several
 *     formatters). The 0x24C-byte body lives in stubs.cpp; the thin
 *     passthrough is kept here until the full FP-classification state
 *     machine is lifted.
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
// fiAsciiTokenizer_2628_g @ 0x82432628 — float formatter entry.
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
// Virtual table layout derived from observed byte-offset dispatch in slot
// bodies (slot N lives at vtable byte +4N):
//
//   slot  0 : ~fiAsciiTokenizer()                 (no-op, r3=0)
//   slot  1 : GetToken(char* out, int outSize)    — raw token reader
//   slot  2 : GetInt()                            — trampoline to slot 4
//   slot  3 : (unused — not emitted)
//   slot  4 : GetInt()  [real]                    — GetToken + strtol
//   slot  5 : GetFloat()                          — GetToken + strtof
//   slot  6 : GetVector4()                        — four GetFloat, staged
//   slot  7 : GetVector3()                        — three GetFloat, staged
//   slot  8 : GetVector4NoStage()                 — four GetFloat, direct
//   slot  9 : GetVector3NoStage()                 — three GetFloat, direct
//   slot 10 : GetVector2()                        — two GetFloat, direct
//   slot 11 : SkipToken()                         — GetToken into 64-byte bit-bucket
//   slot 12 : MatchIntField()                     — SkipToken + slot 4
//   slot 13 : MatchFloatField()                   — SkipToken + slot 5
//   slot 14 : MatchVector4Field(out)              — SkipToken + slot 6
//   slot 15 : MatchVector3Field(out)              — SkipToken + slot 7
//   slot 16 : MatchVector4NoStageField(out)       — SkipToken + slot 8
//   slot 17 : MatchVector3NoStageField(out)       — SkipToken + slot 9
//   slot 18 : MatchVector2Field(out)              — SkipToken + slot 10
//   slot 19 : SkipWhitespaceThenInt()             — ph_58A8 + slot 4
//   slot 20 : SkipWhitespaceThenFloat()           — ph_58A8 + slot 5
//   slot 21 : SkipWhitespaceThenVector4(out)      — ph_58A8 + slot 6
//   slot 22 : SkipWhitespaceThenVector3(out)      — ph_58A8 + slot 7
//   slot 23 : SkipWhitespaceThenVector4NoStage(out) — ph_58A8 + slot 8
//   slot 24 : SkipWhitespaceThenVector3NoStage(out) — ph_58A8 + slot 9
//   slot 25 : SkipWhitespaceThenVector2(out)      — ph_58A8 + slot 10
//   slot 26 : WriteBlockOpen()                    — emit '\t'*depth + '{' + CRLF, depth++
//   slot 30 : SetIndentDepth(int delta)           — +=delta to m_indentDepth (+0xA0)
//   slot 36 : (tail target of slot 37, takes 1 arg)
//   slot 37 : CloseAndFlush()                     — tail-calls slot 36 with arg=0
//
// Instance layout (non-exhaustive):
//   +0x00  vtable*
//   +0x04  flags / mode bits
//   +0x08  (42 accesses — likely source buffer base pointer)
//   +0x0C  FILE* / output stream (fiAsciiTokenizer_51F0 writes a single byte)
//   +0xA0  int32_t m_indentDepth — structured-output nesting depth
//
// The .hpp for rage::fiAsciiTokenizer does not yet exist in this project
// — we provide the method definitions inline here without touching headers
// (per the partition rules). A future pass will promote these to a real
// header once more of the surface is characterized.

namespace rage {

extern "C" {
    // Leading-whitespace skip primitive used by the "SkipWhitespaceThen*"
    // slot family. Defined in src/game/pong_render.cpp (ph_58A8 @ 0x822E58A8).
    void ph_58A8(void* tokenizer);

    // strtol-equivalent integer parser. FLIRT-matched to `atol` family.
    // @ 0x82432120 (12 bytes). Returns signed int.
    int _xe_strcpyn_10(const char* src);

    // strtof-equivalent float parser. @ 0x82432B80 (8 bytes, jump thunk).
    float jumptable_2B80(const char* src);

    // Single-byte writer for the structured-output path. Reads a FILE* from
    // this->[+0x0C] and writes r4 (byte value). @ 0x822E51F0 (136 bytes).
    void fiAsciiTokenizer_51F0(void* stream, int byteValue);
}

// .rdata float at 0x8202D110 — the "no-match" default value for GetFloat.
// Matches binary layout; we honor the indirection rather than folding the
// constant here because the build may patch it.
extern "C" const float lbl_8202D110;

class fiAsciiTokenizer {
public:
    virtual ~fiAsciiTokenizer();
    virtual int   GetToken(char* out, int outSize);                  // slot 1
    virtual int   GetIntTrampoline();                                // slot 2
    virtual void  vfn_3();                                           // slot 3 (unused)
    virtual int   GetInt();                                          // slot 4
    virtual float GetFloat();                                        // slot 5
    virtual void  GetVector4(float outV4[4]);                        // slot 6
    virtual void  GetVector3(float outV3[3]);                        // slot 7
    virtual void  GetVector4NoStage(float outV4[4]);                 // slot 8
    virtual void  GetVector3NoStage(float outV3[3]);                 // slot 9
    virtual void  GetVector2(float outV2[2]);                        // slot 10
    virtual void  SkipToken();                                       // slot 11
    virtual int   MatchIntField();                                   // slot 12
    virtual float MatchFloatField();                                 // slot 13
    virtual void  MatchVector4Field(float outV4[4]);                 // slot 14
    virtual void  MatchVector3Field(float outV3[3]);                 // slot 15
    virtual void  MatchVector4NoStageField(float outV4[4]);          // slot 16
    virtual void  MatchVector3NoStageField(float outV3[3]);          // slot 17
    virtual void  MatchVector2Field(float outV2[2]);                 // slot 18
    virtual int   SkipWhitespaceThenInt();                           // slot 19
    virtual float SkipWhitespaceThenFloat();                         // slot 20
    virtual void  SkipWhitespaceThenVector4(float outV4[4]);         // slot 21
    virtual void  SkipWhitespaceThenVector3(float outV3[3]);         // slot 22
    virtual void  SkipWhitespaceThenVector4NoStage(float outV4[4]);  // slot 23
    virtual void  SkipWhitespaceThenVector3NoStage(float outV3[3]);  // slot 24
    virtual void  SkipWhitespaceThenVector2(float outV2[2]);         // slot 25
    virtual void  WriteBlockOpen();                                  // slot 26
    virtual void  SetIndentDepth(int delta);                         // slot 30
    virtual void  CloseAndFlush(const char* closingStr);             // slot 37
};

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::~fiAsciiTokenizer()   — vtable slot 0
// @ 0x8210CAF8 | size: 0x8
//
// Emitted as a one-instruction body (`li r3,0; blr`). The base destructor
// in this family is a no-op that simply zeroes r3. True teardown lives in
// slot 1 of the scalar-deleting dtor pair.
// ─────────────────────────────────────────────────────────────────────────────
fiAsciiTokenizer::~fiAsciiTokenizer() {
    // intentionally empty — base destructor returns 0 in r3.
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetIntTrampoline()    — vtable slot 2
// @ 0x822E5E78 | size: 0x10
//
// Pure tail-call to slot 4 via vtable[+16] (= slot 4 at 4*4 bytes). This
// is a convenience entry where no leading-whitespace skip is required.
// ─────────────────────────────────────────────────────────────────────────────
int fiAsciiTokenizer::GetIntTrampoline() {
    return this->GetInt();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetInt()              — vtable slot 4
// @ 0x822E5E88 | size: 0x7C
//
// Allocates a 32-byte stack buffer, calls slot 1 (GetToken) to fill it,
// then classifies on the first char:
//   - token empty (slot-1 returned 0)  -> return 0
//   - first char is '-'                -> strtol on buffer
//   - first char is '0'..'9'           -> strtol on buffer
//   - otherwise                        -> return 0
// ─────────────────────────────────────────────────────────────────────────────
int fiAsciiTokenizer::GetInt() {
    char buf[32];
    buf[0] = '\0';
    int len = this->GetToken(buf, static_cast<int>(sizeof(buf)));
    if (len == 0) {
        return 0;
    }
    char first = buf[0];
    if (first == '-') {
        return _xe_strcpyn_10(buf);
    }
    if (first >= '0' && first <= '9') {
        return _xe_strcpyn_10(buf);
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetFloat()            — vtable slot 5
// @ 0x822E5F08 | size: 0x8C
//
// Same shape as GetInt but for floats. Accepts '-', '.' or '0'..'9' as
// the leading char; otherwise returns the default float at lbl_8202D110.
// ─────────────────────────────────────────────────────────────────────────────
float fiAsciiTokenizer::GetFloat() {
    char buf[32];
    buf[0] = '\0';
    int len = this->GetToken(buf, static_cast<int>(sizeof(buf)));
    if (len == 0) {
        return lbl_8202D110;
    }
    char first = buf[0];
    if (first == '-' || first == '.') {
        return jumptable_2B80(buf);
    }
    if (first >= '0' && first <= '9') {
        return jumptable_2B80(buf);
    }
    return lbl_8202D110;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetVector2()          — vtable slot 10
// @ 0x822E5F98 | size: 0x60
//
// Reads two consecutive floats via slot 5 and stores them directly into
// outV2[0] / outV2[1].
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::GetVector2(float outV2[2]) {
    outV2[0] = this->GetFloat();
    outV2[1] = this->GetFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetVector3NoStage()   — vtable slot 9
// @ 0x822E5FF8 | size: 0x78
//
// Reads three consecutive floats via slot 5 and stores them directly. The
// compiler emitted the stores interleaved with each call (no staging).
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::GetVector3NoStage(float outV3[3]) {
    outV3[0] = this->GetFloat();
    outV3[1] = this->GetFloat();
    outV3[2] = this->GetFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetVector4NoStage()   — vtable slot 8
// @ 0x822E6070 | size: 0x90
//
// Reads four consecutive floats via slot 5 and stores them directly.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::GetVector4NoStage(float outV4[4]) {
    outV4[0] = this->GetFloat();
    outV4[1] = this->GetFloat();
    outV4[2] = this->GetFloat();
    outV4[3] = this->GetFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetVector3()          — vtable slot 7
// @ 0x822E6100 | size: 0x90
//
// Stages the first two reads in saved FP registers (f31/f30), then writes
// all three out at the end. Semantically identical to GetVector3NoStage
// but with the write-atomicity property of the "staged" variant family.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::GetVector3(float outV3[3]) {
    float x = this->GetFloat();
    float y = this->GetFloat();
    float z = this->GetFloat();
    outV3[0] = x;
    outV3[1] = y;
    outV3[2] = z;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::GetVector4()          — vtable slot 6
// @ 0x822E6190 | size: 0xCC
//
// Stages all four floats into a stack-local vec4 at sp+80, then bulk-copies
// 16 bytes out to outV4. Used when the caller requires write-atomicity —
// e.g. when the out buffer may alias mid-parse state.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::GetVector4(float outV4[4]) {
    float staged[4];
    staged[0] = this->GetFloat();
    staged[1] = this->GetFloat();
    staged[2] = this->GetFloat();
    staged[3] = this->GetFloat();
    // Bulk copy — compiler emitted 4x32-bit load/store from sp+80.
    outV4[0] = staged[0];
    outV4[1] = staged[1];
    outV4[2] = staged[2];
    outV4[3] = staged[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::SkipToken()           — vtable slot 11
// @ 0x822E6260 | size: 0x3C
//
// Calls slot 1 (GetToken) into a 64-byte stack scratch with leading NUL.
// Used as a "consume and discard next token" primitive by the Match*Field
// family — identifier before the typed payload.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::SkipToken() {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchIntField()       — vtable slot 12
// @ 0x822E62A0 | size: 0x5C
//
// Consume the field-name token, then invoke slot 4 (GetInt). Used by
// parStream to read a "name value" pair where the name is discarded.
// ─────────────────────────────────────────────────────────────────────────────
int fiAsciiTokenizer::MatchIntField() {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    return this->GetInt();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchFloatField()     — vtable slot 13
// @ 0x822E6300 | size: 0x5C
//
// Mirror of MatchIntField — consume field name, then slot 5 (GetFloat).
// ─────────────────────────────────────────────────────────────────────────────
float fiAsciiTokenizer::MatchFloatField() {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    return this->GetFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchVector2Field()   — vtable slot 18
// @ 0x822E6360 | size: 0x6C
//
// Consume the field-name token, then call slot 10 (GetVector2) forwarding
// the caller's out-buffer as r4. This is the parStream "name v2" pair.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::MatchVector2Field(float outV2[2]) {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    this->GetVector2(outV2);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::SkipWhitespaceThenInt()   — vtable slot 19
// @ 0x822E6590 | size: 0x40
//
// Skip leading whitespace via ph_58A8 (shared fi-layer primitive), then
// dispatch to slot 4 (GetInt). Entry point used when the caller knows a
// leading separator may or may not be present.
// ─────────────────────────────────────────────────────────────────────────────
int fiAsciiTokenizer::SkipWhitespaceThenInt() {
    ph_58A8(this);
    return this->GetInt();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::SkipWhitespaceThenFloat() — vtable slot 20
// @ 0x822E65D0 | size: 0x40
//
// Skip leading whitespace then slot 5 (GetFloat).
// ─────────────────────────────────────────────────────────────────────────────
float fiAsciiTokenizer::SkipWhitespaceThenFloat() {
    ph_58A8(this);
    return this->GetFloat();
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::WriteBlockOpen()      — vtable slot 26
// @ 0x822E67A0 | size: 0x84
//
// Structured-output helper: emits `m_indentDepth` tab bytes, followed by
// '{', CR, LF, then post-increments m_indentDepth. The write primitive is
// fiAsciiTokenizer_51F0, which pulls this->[+0x0C] as its FILE* and
// writes a single byte.
//
// m_indentDepth lives at +0xA0 on the instance. This is the same field
// that SetIndentDepth (slot 30) mutates.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::WriteBlockOpen() {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    int32_t*  pDepth = reinterpret_cast<int32_t*>(self + 0xA0);
    void*     stream = *reinterpret_cast<void**>(self + 0x0C);

    // Emit one tab per indent level.
    for (int32_t n = *pDepth; n > 0; --n) {
        fiAsciiTokenizer_51F0(stream, '\t');
    }
    // Block-open glyph + CRLF.
    fiAsciiTokenizer_51F0(stream, '{');
    fiAsciiTokenizer_51F0(stream, '\r');
    fiAsciiTokenizer_51F0(stream, '\n');

    // Nest one level deeper for subsequent writes.
    *pDepth = *pDepth + 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::SetIndentDepth()      — vtable slot 30
// @ 0x820C2E08 | size: 0x10
//
// Adds `delta` to the 32-bit nesting-depth counter at +0xA0. Despite the
// simplicity, the semantics is NOT a cursor seek — WriteBlockOpen (slot
// 26) treats the same field as a structured-output indent level. Callers
// use this to manually unwind after a WriteBlockClose-style operation.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::SetIndentDepth(int delta) {
    int32_t* pDepth = reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0xA0);
    *pDepth = *pDepth + delta;
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::CloseAndFlush()       — vtable slot 37
// @ 0x822E6D00 | size: 0x14
//
// Tail-calls vtable[+144] (= slot 36, byte offset 4*36) with r5 forced to 0,
// r4 forwarded through unchanged. Slot 36 is WriteStringWithPadding
// @ 0x822E6998 | size: 0xA4 — signature (this, const char* str, int padCount):
// writes `str` to the FILE* at this->[+12], then emits `padCount` tab bytes
// (0x9) via fiAsciiTokenizer_51F0. Returns bool (bytes written matched).
// Passing padCount=0 means "flush string, no tab padding", which matches the
// "close current element" idiom. CloseAndFlush itself takes the string as
// its caller-supplied argument.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::CloseAndFlush(const char* closingStr) {
    using Slot36Fn = bool (*)(fiAsciiTokenizer*, const char*, int);
    void** vt = *reinterpret_cast<void***>(this);
    Slot36Fn writeWithPadding = reinterpret_cast<Slot36Fn>(vt[36]);
    (void)writeWithPadding(this, closingStr, 0);
}

} // namespace rage

// Note: slot 3 body is not emitted in the binary — the vtable entry points
//   at a pure-virtual handler. No lift required.
// Note: slots 14–17, 21–25, 27–29, 31–35 follow the same vector/whitespace
//   dispatch templates documented above; they can be batched in a follow-up.
//   Slot 36 is WriteStringWithPadding @ 0x822E6998 (see CloseAndFlush note
//   for the resolved signature).
// Note: fiAsciiTokenizer_2628_g @ 0x82432628 (0x24C bytes) is the FP
//   formatter state machine — classifies NaN/inf/whole/small/general and
//   dispatches to the matching %g-variant. Migration from stubs.cpp is
//   deferred until a dedicated session.


// ═════════════════════════════════════════════════════════════════════════════
// Batch lift: 10 functions from rage::fiAsciiTokenizer
// vtable slots 14–17, 26, 30 + allocator helpers D588, 76D8, 8598, CC50
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchVector4Field()   — vtable slot 14
// @ 0x822E6520 | size: 0x6C
//
// Consume the field-name token via GetToken, then forward the output buffer
// to GetVector4 (slot 6). Used by parStream for "name x y z w" quads.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::MatchVector4Field(float outV4[4]) {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    this->GetVector4(outV4);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchVector3Field()   — vtable slot 15
// @ 0x822E64B0 | size: 0x6C
//
// Consume the field-name token, then forward the output buffer to
// GetVector3 (slot 7). Used by parStream for "name x y z" triples.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::MatchVector3Field(float outV3[3]) {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    this->GetVector3(outV3);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchVector4NoStageField() — vtable slot 16
// @ 0x822E6440 | size: 0x6C
//
// Consume the field-name token, then call GetVector4NoStage (slot 8).
// Writes each component directly to the output as it is parsed.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::MatchVector4NoStageField(float outV4[4]) {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    this->GetVector4NoStage(outV4);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::MatchVector3NoStageField() — vtable slot 17
// @ 0x822E63D0 | size: 0x6C
//
// Consume the field-name token, then call GetVector3NoStage (slot 9).
// Writes each component directly to the output as it is parsed.
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::MatchVector3NoStageField(float outV3[3]) {
    char scratch[64];
    scratch[0] = '\0';
    (void)this->GetToken(scratch, static_cast<int>(sizeof(scratch)));
    this->GetVector3NoStage(outV3);
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// Standalone allocator functions (same compilation unit, not class methods)
// ═════════════════════════════════════════════════════════════════════════════

extern "C" {

// Forward declarations for shared helpers
void fiAsciiTokenizer_1F08_g(const char* errorMsg);
[[noreturn]] void fiAsciiTokenizer_FB40_g(int exitCode);
void* xe_EC88(uint32_t size);

// .rdata string at 0x8202E2B8 — "out of memory"
static const char* const g_pOutOfMemoryStr = "out of memory";

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer_D588_g @ 0x8222D588 | size: 0x60
//
// Byte allocator — allocates the requested number of raw bytes via xe_EC88.
// Returns nullptr if size is zero. The overflow check against 0xFFFFFFFF
// is dead code in 32-bit context (always passes).
// ─────────────────────────────────────────────────────────────────────────────
void* fiAsciiTokenizer_D588_g(uint32_t size) {
    if (size == 0) {
        return nullptr;
    }
    return xe_EC88(size);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer_76D8_g @ 0x820E76D8 | size: 0x64
//
// Array allocator for 4-byte elements — allocates count * 4 bytes. Aborts
// with "out of memory" if count exceeds 0x3FFFFFFF (~1 billion entries).
// Returns nullptr if count is zero.
// ─────────────────────────────────────────────────────────────────────────────
void* fiAsciiTokenizer_76D8_g(uint32_t count) {
    if (count > 0x3FFFFFFFu) {
        fiAsciiTokenizer_1F08_g(g_pOutOfMemoryStr);
        fiAsciiTokenizer_FB40_g(1);
    }
    if (count == 0) {
        return nullptr;
    }
    return xe_EC88(count * 4);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer_8598_g @ 0x82238598 | size: 0x64
//
// Array allocator for 16-byte elements — allocates count * 16 bytes. Aborts
// with "out of memory" if count exceeds 0x0FFFFFFF (~268 million entries).
// Returns nullptr if count is zero.
// ─────────────────────────────────────────────────────────────────────────────
void* fiAsciiTokenizer_8598_g(uint32_t count) {
    if (count > 0x0FFFFFFFu) {
        fiAsciiTokenizer_1F08_g(g_pOutOfMemoryStr);
        fiAsciiTokenizer_FB40_g(1);
    }
    if (count == 0) {
        return nullptr;
    }
    return xe_EC88(count * 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer_CC50_g @ 0x8222CC50 | size: 0xA8
//
// Initializes a 12-byte dynamic array struct (rage vector-like container)
// and fills it with a single repeated uint32_t value.
//
// Layout of the struct at 'this':
//   +0x00: uint32_t* m_pBegin      — pointer to first element
//   +0x04: uint32_t* m_pEnd        — pointer past last written element
//   +0x08: uint32_t* m_pCapacityEnd — pointer past allocated region
//
// Arguments:
//   r3 = this (12-byte array struct)
//   r4 = capacity (number of uint32_t slots)
//   r5 = pointer to fill value (single uint32_t read repeatedly)
// ─────────────────────────────────────────────────────────────────────────────
struct DynArrayU32 {
    uint32_t* m_pBegin;
    uint32_t* m_pEnd;
    uint32_t* m_pCapacityEnd;
};

void fiAsciiTokenizer_CC50_g(DynArrayU32* arr, uint32_t capacity,
                              const uint32_t* pFillValue) {
    arr->m_pBegin = nullptr;
    arr->m_pEnd = nullptr;
    arr->m_pCapacityEnd = nullptr;

    if (capacity > 0x3FFFFFFFu) {
        fiAsciiTokenizer_1F08_g(g_pOutOfMemoryStr);
        fiAsciiTokenizer_FB40_g(1);
    }

    uint32_t* data = nullptr;
    if (capacity != 0) {
        data = static_cast<uint32_t*>(xe_EC88(capacity * 4));
    }

    arr->m_pBegin = data;
    arr->m_pEnd = data;
    arr->m_pCapacityEnd = data + capacity;

    // Fill all slots with the value pointed to by pFillValue
    uint32_t* dst = data;
    for (uint32_t i = capacity; i > 0; i--) {
        *dst = *pFillValue;
        dst++;
    }
    arr->m_pEnd = dst;
}

} // extern "C"
