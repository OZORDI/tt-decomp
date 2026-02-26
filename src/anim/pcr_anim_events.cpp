/**
 * pcr_anim_events.cpp — Creature Animation Event Hierarchy
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the pcrAnimEvent class family.  Animation events are serialised
 * from the creature data pack and post-load–fixed-up via FixupPointers.
 * When an event fires (Process), it registers itself once via atexit and
 * then dispatches through the RAGE crAnimEvent subsystem.
 *
 * Class hierarchy (all share the base pcrAnimEvent layout):
 *
 *   pcrAnimEvent          — base; matches Default, FootAudio, Message types
 *     pcrAnimEventFootAud  — intermediate for foot events
 *       pcrAnimEventFootContact  — foot contact  (GetTypeName = "FootContact")
 *       pcrAnimEventFootScuff    — foot scuff    (GetTypeName = "FootScuff")
 *       pcrAnimEventFootStep     — foot step     (GetTypeName = "FootStep")
 *       pcrAnimEventFootStomp    — foot stomp    (GetTypeName = "FootStomp")
 *     pcrAnimEventMsg      — message events      (GetTypeName = "Msg")
 *       pcrAnimEventMsg_s  — string-message      (GetTypeName = "Msg_s")
 *
 * pcrAnimEvent struct layout (all confirmed from recomp offsets):
 *
 *   +0x00  void**      vtable
 *   +0x04  uint32_t    m_field_04    (zeroed on fixup; purpose unclear)
 *   +0x08  uint32_t    m_field_08
 *   +0x0C  uint32_t    m_field_0C
 *   +0x10  uint32_t    m_field_10
 *   +0x14  const char* m_name        ← atString ptr; fixed up from pack offset
 *   +0x18  int16_t     m_nameLen
 *   +0x1A  uint16_t    m_nameCap     ← if non-zero, m_name is heap-owned
 *   +0x1C  const char* m_value       ← second atString (e.g. bone name)
 *   +0x20  int16_t     m_valueLen
 *   +0x22  uint16_t    m_valueCap
 *   +0x24  const char* m_extra       ← raw ptr (not heap-owned by pcrAnimEvent)
 *
 * pcrAnimEventMsg_s adds:
 *   +0x30  const char* m_stringParam ← extra string; fixed up if non-null
 */

#include "../game/pong_creature.hpp"
#include <cstdint>
#include <cstdlib>  // atexit

// ─────────────────────────────────────────────────────────────────────────────
// Externals
// ─────────────────────────────────────────────────────────────────────────────

// RAGE base class destructor called at the end of pcrAnimEvent's own dtor.
// atSingleton_9420 tears down the atSingleton base in the vtable chain.
extern void atSingleton_9420(void* obj);

// Heap allocator / deallocator (RAGE custom).
extern void rage_free_00C0(void* ptr);

// RAGE animation event dispatcher.
//   Fires this event through the crAnimEvent subsystem.
//   arg1 = this, arg2 = type-name string, arg3 = type-registry block ptr,
//   arg4 = &m_name, arg5 = 0, arg6 = 0.
// @ 0x821A8F58
extern void game_8F58(void* self,
                      const char* typeName,
                      void* typeRegistry,
                      const char** pName,
                      uint32_t unused5,
                      uint32_t unused6);

// atexit-handler and cleanup registration for the type system.
// The atexit handler (@ 0x82583780) is registered once per type;
// the cleanup fn ptr (@ 0x82028D4C) is stored in the registry block.
extern void (*pcrAnimEvent_AtExitHandler)();  // @ 0x82583780
// Static one-time-registration guard block embedded in .data @ 0x8271B0D0.
//   [+0] uint32_t  flags   — bit 0 = "already registered"
//   [+4] void*     cleanup — pointer to the cleanup function
extern uint32_t g_pcrAnimEvent_typeRegistry;   // @ 0x8271B0D0

// Type-name string literals compiled into the binary's .rdata section.
// These are returned by GetTypeName() on each subclass.
// @ 0x82028CD4
extern const char kTypeName_pcrAnimEvent[];
// @ 0x82028D5C
extern const char kTypeName_FootContact[];
// @ 0x82028D74
extern const char kTypeName_FootScuff[];
// @ 0x82028D8C
extern const char kTypeName_FootStep[];
// @ 0x82028DA4
extern const char kTypeName_FootStomp[];
// @ 0x82028DBC
extern const char kTypeName_Msg[];
// @ 0x82028DCC
extern const char kTypeName_Msg_s[];

// ─────────────────────────────────────────────────────────────────────────────
// Event type ID globals (populated at first process time via game_8F58).
// @ 0x825C9C60, 0x825C803C, 0x825C8038
// ─────────────────────────────────────────────────────────────────────────────
uint32_t g_eventType_Default   = 0;   // @ 0x825C9C60
uint32_t g_eventType_FootAudio = 0;   // @ 0x825C803C
uint32_t g_eventType_Message   = 0;   // @ 0x825C8038

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEvent — non-virtual destructor body
// @ 0x820DE690 | size: 0x50
//
// Called by the virtual destructor wrapper below.  Resets the vtable to the
// base pcrAnimEvent vtable, frees any heap-owned atString buffers (m_name,
// m_value), then calls the atSingleton base destructor.
// ─────────────────────────────────────────────────────────────────────────────
static void pcrAnimEvent_DtorBody(pcrAnimEvent* self)
{
    // Reset vtable to base pcrAnimEvent (erases any sub-class vtable that may
    // have been stamped by CreateEvent or a derived class ctor).
    // Vtable address: 0x82028CE4
    extern void* g_pcrAnimEvent_vtable;
    self->vtable = (void**)&g_pcrAnimEvent_vtable;

    // Free the m_value atString buffer if it is heap-owned (m_valueCap != 0).
    if (self->m_valueCap != 0)
        rage_free_00C0(const_cast<char*>(self->m_value));

    // Free the m_name atString buffer if it is heap-owned (m_nameCap != 0).
    if (self->m_nameCap != 0)
        rage_free_00C0(const_cast<char*>(self->m_name));

    // Tear down the atSingleton base.
    atSingleton_9420(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEvent virtual destructor  [vtable slot 0]
// @ 0x820DE640 | size: 0x50
//
// Standard RAGE two-phase destructor.  The hidden flags parameter controls
// whether the backing allocation is freed after the object is torn down.
// ─────────────────────────────────────────────────────────────────────────────
pcrAnimEvent::~pcrAnimEvent()
{
    pcrAnimEvent_DtorBody(this);
    // The caller passes bit 0 of the original r4 to decide whether to free
    // the allocation.  In lifted C++ this is handled by delete vs. explicit dtor.
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEvent::MatchesType  [vtable slot 20]
// @ 0x820DE7D8 | size: 0x48
//
// Returns true if this event class accepts the given event type ID.
// pcrAnimEvent is the base, so it handles Default, FootAudio, and Message.
// ─────────────────────────────────────────────────────────────────────────────
bool pcrAnimEvent::MatchesType(uint32_t eventType) const
{
    // Default and foot-audio types are always accepted by the base class.
    if (eventType == g_eventType_Default || eventType == g_eventType_FootAudio)
        return true;

    // Message type is also accepted.
    return (eventType == g_eventType_Message);
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEvent::Process  [vtable slot 21]
// @ 0x820DE820 | size: 0x88
//
// Fires this animation event through the RAGE crAnimEvent subsystem.
// On the very first call, it registers a cleanup handler via atexit (the
// guard flag lives in the static type-registry block at 0x8271B0D0).
// ─────────────────────────────────────────────────────────────────────────────
void pcrAnimEvent::Process()
{
    // g_pcrAnimEvent_typeRegistry: bit 0 = "atexit already registered".
    if (!(g_pcrAnimEvent_typeRegistry & 1))
    {
        // Mark as registered and store the cleanup function pointer into the
        // registry block before calling atexit.
        g_pcrAnimEvent_typeRegistry |= 1;
        // NOTE: raw fn-ptr store into registry+4 omitted here; the atexit
        // call below uses the address directly.
        atexit(pcrAnimEvent_AtExitHandler);
    }

    // Dispatch through the engine event system.
    // The type-name string identifies the class for the dispatcher;
    // &m_name gives the dispatcher the event's per-instance name.
    game_8F58(this,
              kTypeName_pcrAnimEvent,
              &g_pcrAnimEvent_typeRegistry,
              &m_name,
              0,
              0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEvent::FixupPointers
// @ 0x820DE8A8 | size: 0xBC
//
// Called after deserializing an event from a resource pack.  The pack stores
// string data as byte-offsets from the pack's base pointer; this function
// converts those offsets into absolute runtime pointers.
//
// loadCtx layout (r4):
//   +0x04  void*    base        — pack data base pointer
//   +0x4C  uint32_t entrySize   — stride of the string-data table
//   The pointer table starts at loadCtx+8; entry at index i is loadCtx[i+2].
//
// Affected fields: m_name (+0x14), m_value (+0x1C), m_extra (+0x24).
// ─────────────────────────────────────────────────────────────────────────────
static inline const char* FixupStringPtr(const char* rawPtr, const void* loadCtx)
{
    // loadCtx offsets (words, i.e. sizeof(uint32_t) steps):
    const uint32_t* ctx       = static_cast<const uint32_t*>(loadCtx);
    uint32_t        base      = ctx[1];          // +0x04
    uint32_t        entrySize = ctx[0x13];       // +0x4C  (19th word)

    uint32_t addr  = reinterpret_cast<uintptr_t>(rawPtr);
    uint32_t index = (addr - base) / entrySize;

    // Entry pointer at loadCtx[(index+2)*4].
    const uint32_t* entry = reinterpret_cast<const uint32_t*>(ctx[index + 2]);
    return reinterpret_cast<const char*>(entry + addr);
}

void pcrAnimEvent::FixupPointers(const void* loadCtx)
{
    // Reset header fields (vtable, reserved words).
    // This is the base-class init called before sub-class ctors.
    extern void* g_pcrAnimEvent_vtable;
    vtable       = (void**)&g_pcrAnimEvent_vtable;
    m_field_04   = 0;
    m_field_08   = 0;
    m_field_0C   = 0;

    // Fix up each string field if it is non-null (i.e. present in the pack).
    if (m_name)  m_name  = FixupStringPtr(m_name,  loadCtx);
    if (m_value) m_value = FixupStringPtr(m_value, loadCtx);
    if (m_extra) m_extra = FixupStringPtr(m_extra, loadCtx);
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEvent::CreateEvent  (factory / placement constructor)
// @ 0x820DEB38 | size: varies
//
// Placement-constructs the right event subclass into `dst` based on a type
// index stored in the serialised data block at loadCtx+16.  After calling the
// appropriate base FixupPointers, the vtable is overridden to the correct
// sub-class vtable.
//
// Type index → class:
//   0  pcrAnimEventFootContact  (vtable 0x8202EC4C)
//   1  pcrAnimEventFootScuff    (vtable 0x8202ECBC)
//   2  pcrAnimEventFootStep     (vtable 0x8202ED2C)
//   3  pcrAnimEventFootStomp    (vtable 0x8202ED9C)
//   4  pcrAnimEventMsg          (constructed via msg ctor @ 0x820EC0D8)
//   5  pcrAnimEventMsg_s        (msg ctor + vtable override to 0x8202EEAC)
// ─────────────────────────────────────────────────────────────────────────────
extern void pcrAnimEvent_MsgCtor(pcrAnimEvent* dst, const void* loadCtx);  // @ 0x820EC0D8

extern void* g_vtable_pcrAnimEventFootContact;  // 0x8202EC4C
extern void* g_vtable_pcrAnimEventFootScuff;    // 0x8202ECBC
extern void* g_vtable_pcrAnimEventFootStep;     // 0x8202ED2C
extern void* g_vtable_pcrAnimEventFootStomp;    // 0x8202ED9C
extern void* g_vtable_pcrAnimEventMsg_s;        // 0x8202EEAC

void pcrAnimEvent::CreateEvent(pcrAnimEvent* dst, const void* loadCtx)
{
    const uint32_t* data = static_cast<const uint32_t*>(loadCtx);
    uint32_t typeIndex   = data[4];  // type code is at loadCtx+16

    switch (typeIndex)
    {
    case 0:
        dst->FixupPointers(loadCtx);
        dst->vtable = (void**)&g_vtable_pcrAnimEventFootContact;
        break;

    case 1:
        dst->FixupPointers(loadCtx);
        dst->vtable = (void**)&g_vtable_pcrAnimEventFootScuff;
        break;

    case 2:
        dst->FixupPointers(loadCtx);
        dst->vtable = (void**)&g_vtable_pcrAnimEventFootStep;
        break;

    case 3:
        dst->FixupPointers(loadCtx);
        dst->vtable = (void**)&g_vtable_pcrAnimEventFootStomp;
        break;

    case 4:
        // pcrAnimEventMsg uses its own ctor which sets the vtable.
        pcrAnimEvent_MsgCtor(dst, loadCtx);
        break;

    case 5:
        // pcrAnimEventMsg_s shares the Msg ctor but overrides the vtable and
        // fixes up the extra string pointer at +0x30 if present.
        pcrAnimEvent_MsgCtor(dst, loadCtx);
        dst->vtable = (void**)&g_vtable_pcrAnimEventMsg_s;
        {
            // m_stringParam for Msg_s is at +0x30.
            pcrAnimEventMsg_s* ms = static_cast<pcrAnimEventMsg_s*>(
                static_cast<void*>(dst));
            if (ms->m_stringParam)
                ms->m_stringParam = FixupStringPtr(ms->m_stringParam, loadCtx);
        }
        break;

    default:
        // Type index out of range — should never happen in shipped data.
        break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEventFootContact::GetTypeName  [vtable slot 22]
// @ 0x820DEAD8 | size: 0x10
// ─────────────────────────────────────────────────────────────────────────────
const char* pcrAnimEventFootContact::GetTypeName() const
{
    return kTypeName_FootContact;   // "FootContact" @ 0x82028D5C
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEventFootScuff::GetTypeName  [vtable slot 22]
// @ 0x820DEAE8 | size: 0x10
// ─────────────────────────────────────────────────────────────────────────────
const char* pcrAnimEventFootScuff::GetTypeName() const
{
    return kTypeName_FootScuff;     // "FootScuff" @ 0x82028D74
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEventFootStep::GetTypeName  [vtable slot 22]
// @ 0x820DEAF8 | size: 0x10
// ─────────────────────────────────────────────────────────────────────────────
const char* pcrAnimEventFootStep::GetTypeName() const
{
    return kTypeName_FootStep;      // "FootStep" @ 0x82028D8C
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEventFootStomp::GetTypeName  [vtable slot 22]
// @ 0x820DEB08 | size: 0x10
// ─────────────────────────────────────────────────────────────────────────────
const char* pcrAnimEventFootStomp::GetTypeName() const
{
    return kTypeName_FootStomp;     // "FootStomp" @ 0x82028DA4
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEventMsg::GetTypeName  [vtable slot 22]
// @ 0x820DEB18 | size: 0x10
// ─────────────────────────────────────────────────────────────────────────────
const char* pcrAnimEventMsg::GetTypeName() const
{
    return kTypeName_Msg;           // "Msg" @ 0x82028DBC
}

// ─────────────────────────────────────────────────────────────────────────────
// pcrAnimEventMsg_s::GetTypeName  [vtable slot 22]
// @ 0x820DEB28 | size: 0x10
// ─────────────────────────────────────────────────────────────────────────────
const char* pcrAnimEventMsg_s::GetTypeName() const
{
    return kTypeName_Msg_s;         // "Msg_s" @ 0x82028DCC
}
