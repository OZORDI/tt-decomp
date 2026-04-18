/**
 * par_xml_types.cpp — RAGE parametric XML-schema bridging stubs
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The proto-RAGE "parStructure / parMember / xmlNodeStruct" reflection
 * system is used by gd_data, gd_cutscene, pong_match, pong_ball, char_view,
 * pcr_anim_events, and scene/sg_drawable to describe serializable fields.
 *
 * Real, address-backed implementations of some of these functions live in
 * src/game/data/gd_data.cpp (RegisterSerializedField @ 0x821A8F58, actually
 * an alias for RegisterSerializationField) and src/rage/io/rage_par.cpp.
 * The remaining names below are the proto-RAGE linker surface — no-op
 * bodies that keep the tree linking until each member-reflection primitive
 * gets an individual decomp pass.
 *
 *   RegisterSerializationField / RegisterSerializedField
 *     Two spellings of the same field-registration primitive. Different
 *     translation units declare slightly different signatures (void* vs
 *     const void* for the default-value argument), hence the overload set.
 *
 *   parStruct_SerializeMembers  — iterates a parStruct's member list and
 *     applies a parStreamIn / parStreamOut operator to each.
 *
 *   parMember_Destructor — virtual dtor for the parMember base class.
 *
 *   parStreamInXml_ReadFloat     — scalar-float XML attribute read.
 *   parStreamInXml_FindChildByName — locates a named child element.
 *   parStreamOutXml_vprintf      — formatted-output helper used by
 *                                  sysMemSimpleAllocator diagnostics.
 *
 *   xmlNodeStruct_Init / _Initialize / _BaseInitialize / _SerializeField
 *     Initialization hooks called from gd_cutscene, gd_data, gd_vib_event,
 *     pong_ball::pongBallHitData::LoadProperties, and char_view.
 *
 *   xmlNodeStruct_vfn_2 — virtual slot 2 (PostLoadProperties base body);
 *                         invoked from pong_drill.cpp.
 *
 *   xml_ReadInt / xml_ReadString — token-read helpers called by
 *     pong_render.cpp's effect-bucket config parser.
 */

#include <cstdint>
#include <cstdarg>

extern "C" {

// ─────────────────────────────────────────────────────────────────────────────
// parStructure / parMember surface
// ─────────────────────────────────────────────────────────────────────────────
void parStruct_SerializeMembers(void* structType,
                                void* structInstance,
                                void* op) {
    (void)structType; (void)structInstance; (void)op;
}

void parMember_Destructor(void) {}

// ─────────────────────────────────────────────────────────────────────────────
// parStreamInXml reader surface
// ─────────────────────────────────────────────────────────────────────────────
float parStreamInXml_ReadFloat(void* stream) {
    (void)stream;
    return 0.0f;
}

void* parStreamInXml_FindChildByName(const void* node, const char* name) {
    (void)node; (void)name;
    return nullptr;
}

// parStreamOutXml_vprintf is consumed by sysMemSimpleAllocator for
// allocation-trace dumps. It follows printf semantics.
void parStreamOutXml_vprintf(void* stream, const char* format, ...) {
    (void)stream; (void)format;
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStruct initialization + field serialization
// ─────────────────────────────────────────────────────────────────────────────
void xmlNodeStruct_Init(void* node)            { (void)node; }
void xmlNodeStruct_Initialize(void* node)      { (void)node; }
void xmlNodeStruct_vfn_2(void* node)           { (void)node; }
void xmlNodeStruct_BaseInitialize(void* node)  { (void)node; }

void xmlNodeStruct_SerializeField(void* obj,
                                  const char* name,
                                  void* target,
                                  void* defaultVal,
                                  uint32_t flags) {
    (void)obj; (void)name; (void)target; (void)defaultVal; (void)flags;
}

// ─────────────────────────────────────────────────────────────────────────────
// xml_Read* — token-based XML config readers (pong_render effect buckets).
// ─────────────────────────────────────────────────────────────────────────────
int xml_ReadInt(const char* key) {
    (void)key;
    return 0;
}

void xml_ReadString(const char* key, char* buf, int maxLen) {
    (void)key;
    if (buf && maxLen > 0) buf[0] = '\0';
}

// ─────────────────────────────────────────────────────────────────────────────
// RegisterSerializationField / RegisterSerializedField
//
// Both names target the same @ 0x821A8F58 primitive. C callers use
// underscore-prefixed aliases; C++ callers hit the unmangled name.
// ─────────────────────────────────────────────────────────────────────────────
void RegisterSerializationField(void* obj, const char* name,
                                void* fieldPtr, void* desc,
                                int flags) {
    (void)obj; (void)name; (void)fieldPtr; (void)desc; (void)flags;
}

void RegisterSerializedField(void* obj, const char* name,
                             void* fieldPtr, void* desc,
                             int flags) {
    (void)obj; (void)name; (void)fieldPtr; (void)desc; (void)flags;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_datTypeFactory_Create / rage_parStructure_Init — early-init shims
// consumed from mc_memcard.cpp; leave as weak bodies until the datTypeFactory
// tree is decomped.
// ─────────────────────────────────────────────────────────────────────────────
void* rage_datTypeFactory_Create_stub(void* singleton,
                                      const char* typeName,
                                      int sizeBytes,
                                      void* bucketMemory,
                                      int bucketCount) {
    (void)singleton; (void)typeName; (void)sizeBytes;
    (void)bucketMemory; (void)bucketCount;
    return nullptr;
}

void rage_parStructure_Init_stub(void) {}

} // extern "C"

// Second overload for RegisterSerializedField — C++ callers in gd_data.cpp
// and sg_drawable.cpp pass a const-qualified descriptor pointer for `desc`.
// This is C++-linkage intentionally (mangled) so it does not collide with
// the extern "C" spelling above.
void RegisterSerializedField(void* obj, const char* name,
                             void* fieldPtr, const void* desc,
                             int flags) {
    (void)obj; (void)name; (void)fieldPtr; (void)desc; (void)flags;
}
