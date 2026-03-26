/**
 * pong_misc.cpp — Miscellaneous game classes (fsmMachine, misc helpers)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.9.cpp
 */

#include "pong_misc.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Forward declarations for helpers referenced in this file
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    void  rage_free(void* ptr);                  // @ 0x820C00C0
    void* xe_EC88(uint32_t size);                // @ 0x820DEC88
    void  util_CE30(void* slot);                 // @ 0x8234CE30 - init parStructure
}

namespace rage {
    void ReleaseSingleton(void* obj);            // @ 0x821A9420
    void NotifyObservers(void* self, void* type, uint32_t* out);  // observer broadcast
}

// Logging stub (calls internal debug printf, no-op in release builds)
extern void nop_8240E6D0(const char* fmt, ...); // @ 0x8240E6D0

// Field registration helper (rage serialization system)
// game_8F58: registers a field of 'obj' with the data system
// Signature: (obj, schemaKey, fieldPtr, fieldDesc, flags)
extern void RegisterSerializedField(void* obj, const void* key, void* fieldPtr, const void* desc, uint32_t flags);

// Virtual-call helpers (typed for readability)
#define VCALL_slot20(obj, arg)     (((bool(*)(void*, uint32_t))(*(void***)(obj))[20])(obj, arg))
#define VCALL_slot19(obj)     (((const char*(*)(void*))(*(void***)(obj))[19])(obj))


// ─────────────────────────────────────────────────────────────────────────────
// fsmMachine  [vtable @ 0x8204DD14]
//
// Base class for all finite state machines in the game.  Inherits from
// rage::datBase.  The object holds a heap-allocated array of state pointers
// at +0x08 (m_pStates).
//
// The destructor pattern:
//   1. Calls the internal state-teardown helper (fsmMachine_Destructor_27A8),
//      which first resets the vtable to the base-class (rage::datBase) vtable
//      and frees m_pStates if non-null.
//   2. If the 'delete self' flag (bit 0 of the flags parameter) is set,
//      the raw object memory is freed via rage::MemFree.
//
// This is identical in shape to rage::hsmContext's destructor — both follow
// the standard RAGE deleting-destructor ABI:
//   virtual ~T(int flags)   flags & 1 → free the object after cleanup
// ─────────────────────────────────────────────────────────────────────────────

/**
 * fsmMachine::~fsmMachine() @ 0x82222758 | size: 0x50
 *
 * Deleting destructor for fsmMachine.
 * Tears down internal state (via helper at 0x822227A8), then optionally frees
 * the object itself depending on the `flags` argument (bit 0 = delete self).
 */
fsmMachine::~fsmMachine()
{
    // Reset vtable to rage::datBase and free m_pStates if non-null.
    // Internally equivalent to:
    //   this->vtable    = &rage::datBase::vtable;   // 0x820276C4
    //   if (m_pStates) { rage::MemFree(m_pStates); m_pStates = nullptr; }
    //   this->vtable    = &fsmMachine::vtable;       // 0x8204DD14 (restored)
    //
    // The helper (fsmMachine_Destructor_27A8 @ 0x822227A8) handles this
    // sequence.  After it returns the caller checks bit 0 of 'flags'; if set,
    // rage::MemFree is called on `this`.
    //
    // TODO: confirm whether fsmMachine itself has additional members beyond
    //       +0x08 (m_pStates) — only one function is present in the vtable,
    //       so the remaining 15 slots are pure-virtual or belong to subclasses.
}

// ─────────────────────────────────────────────────────────────────────────────
// pongSaveFile  [2 vtables @ +0 and +12, MI with 24-byte offset]
//
// Save file manager with 4 save slots. Each slot is a rage::parStructure
// (15044 bytes) that manages serialized player data (progress, settings,
// unlocks, statistics).
//
// Xbox 360 Save System Integration:
//   - Uses XamContent* APIs through atSingleton system
//   - XamContentCreate @ 0x825861AC creates save file content package
//   - XamContentClose @ 0x825861CC closes save file handle
//   - rage::parStructure handles platform-independent serialization
//
// Slot Layout (from pongSaveFile base):
//   Slot 0: offset 0x3CE4 (15588 bytes) - initialized last
//   Slot 1: offset 0x77A8 (30632 bytes)
//   Slot 2: offset 0xB26C (45676 bytes)
//   Slot 3: offset 0xED30 (60720 bytes) - initialized first
// ─────────────────────────────────────────────────────────────────────────────

#include "rage/memory.h"

// External function declarations
extern void util_CE30(void* obj);  // @ 0x8234CE30 - Initialize rage::parStructure
// rage::NotifyObservers declared in namespace block above

/**
 * pongSaveFile::pongSaveFile() @ 0x821C5260 | size: 0x90
 *
 * Constructor for save file manager.
 * Initializes vtables and 4 save slot parStructure objects.
 *
 * Algorithm: Start at offset 0x10008 (65544), subtract 0x3AC4 (15044) per
 * iteration. This creates slots at offsets: 0xED30, 0xB26C, 0x77A8, 0x3CE4
 * (reverse order).
 *
 * Each parStructure is initialized by util_CE30 @ 0x8234CE30 which:
 *   - Sets vtable to 0x82065A9C (rage::parStructure vtable)
 *   - Initializes member array pointer at +0x10
 *   - Sets count and flags at +0x14/+0x16
 */
pongSaveFile::pongSaveFile() {
    // Set initial vtables
    // vtable managed by C++ runtime
    // vtable managed by C++ runtime
    
    // Initialize 4 save slots (rage::parStructure instances)
    // Starting from offset 0x10008 (65544) and going backwards by 15044 each iteration
    uint32_t offset = 0x10008;
    for (int i = 0; i < 4; i++) {
        offset -= 15044;  // 0x3AC4 - size of rage::parStructure
        void* saveSlot = (char*)this + offset;
        util_CE30(saveSlot);  // Initialize parStructure @ 0x8234CE30
    }
    
    // Set final vtables
    // vtable managed by C++ runtime
    // vtable managed by C++ runtime
    
    // Free data if flags indicate ownership
    if (m_entryCount != 0 && m_pData) {
        rage_free(m_pData);
    }
    
    // Set base class vtable
    // vtable managed by C++ runtime
}

/**
 * pongSaveFile::~pongSaveFile() @ 0x821C5210 | size: 0x50
 *
 * Destructor for save file manager.
 */
pongSaveFile::~pongSaveFile() {
    // Call cleanup function
    /* cleanup performed by pongSaveFile_5260; omitted in C++ dtor */
    
    // Free memory if destructor flag is set
    // (flag passed in r4, checked with & 0x1)
}

/**
 * pongSaveFile::HandleEvent() @ 0x821C7680 | size: 0x9C
 *
 * Event handler for save file operations.
 *
 * Event types (see GameEventID enum):
 *   EVENT_SAVE_WRITE (0x200F / 8207): Save - serialize current game state to save slot
 *   EVENT_SAVE_LOAD  (0x200E / 8206): Load - deserialize game state from save slot
 *
 * Process:
 *   1. Check if event is save or load
 *   2. Query atSingleton system to determine active save slot index (0-3)
 *   3. Calculate slot offset: slotIndex * 15044 (0x3AC4)
 *   4. Clear event state flags at slot+524, slot+528, slot+529
 *
 * The rage::parStructure at each slot handles actual serialization/deserialization.
 * The atSingleton system calls XamContentCreate @ 0x825861AC to create Xbox 360
 * save file content package.
 */
void pongSaveFile::HandleEvent(uint16_t eventType) {
    bool shouldProcess = false;
    
    // Check event type
    if (eventType == EVENT_SAVE_WRITE) {  // 8207 (0x200F)
        shouldProcess = true;  // Save
    } else if (eventType == EVENT_SAVE_LOAD) {  // 8206 (0x200E)
        shouldProcess = true;  // Load
    }
    
    if (!shouldProcess) {
        return;
    }
    
    // Get save slot index from singleton system
    uint32_t slotIndex = 0;
    rage::NotifyObservers(this, (void*)0x82017888, &slotIndex);
    
    // Calculate save slot offset (each slot is 15044 bytes)
    uint32_t slotOffset = slotIndex * 15044;
    char* saveSlot = (char*)this + slotOffset;
    
    // Clear event state for this slot
    *(uint32_t*)(saveSlot + 524) = 0;
    *(uint8_t*)(saveSlot + 528) = 0;
    *(uint8_t*)(saveSlot + 529) = 0;
}

/**
 * pongSaveFile::DestructorThunk() @ 0x821C7720 | size: 0x8
 *
 * Thunk for secondary base class destructor.
 * Adjusts this pointer by -12 bytes for multiple inheritance.
 */
void pongSaveFile::DestructorThunk(pongSaveFile* ptr) {
    pongSaveFile* adjusted = (pongSaveFile*)((char*)ptr - 12);
    adjusted->~pongSaveFile();
}

// ─────────────────────────────────────────────────────────────────────────────
// assetVersions  [vtable @ 0x8204E6FC]
// assetVersionsChar  [vtable @ 0x8204E764]
// assetVersionsCharSpecific  [vtable @ 0x8204E7CC]
//
// These three classes form a hierarchy that tracks asset format version numbers
// for save-game compatibility inside RAGE's serialization system.
//
// assetVersions (base)
//   — 23 uint32 version slots at offsets +0x10 … +0x68 (fields 16–104, step 4)
//   — Registers all 23 via the field-registration helper (sub_821A8F58)
//   — IsSupported() checks against three known historical version constants
//
// assetVersionsChar (character-asset variant)
//   — 1 version slot at +0x10
//
// assetVersionsCharSpecific (per-character, specific asset variant)
//   — 3 version slots at +0x10, +0x14, +0x18
//   — Extra owned-pointer cleanup at +0x10 in its destructor
//
// The field-registration function (sub_821A8F58 / game_8F58) stores a triplet
// {fieldNameString, fieldPtr, contextPtr} into a SDA-resident array.  The
// "field name" strings passed in are string constants from .rdata and serve as
// key identifiers; many appear to be suffix-optimised substrings sharing storage
// with other string constants elsewhere in the binary (compiler string-pooling).
//
// Serialize context global: lbl_825CAF90 @ 0x825CAF90 (.data, runtime init)
// IsSupported version globals:
//   assetVersions     version A: [0x825C5864]   (runtime init)
//   assetVersionsChar version A: [0x825C5868]
//   assetVersionsCharSpecific A: [0x825C586C]
//   shared            version B: [0x825C803C]
//   shared            version C: [0x825C8038]
// ─────────────────────────────────────────────────────────────────────────────

// Forward declarations for helpers used by all three classes
extern void sub_821A8F58(void* obj, const char* fieldName,
                         void* fieldPtr, void* serCtx, int flags);   // @ 0x821A8F58
extern void sub_821A9420(void* obj);                                   // @ 0x821A9420 - ReleaseSingleton  atSingleton cleanup
extern void rage_free(void* ptr);                                 // @ 0x820C00C0  RAGE heap free

// Serialisation context pointer stored in the SDA (r13-relative), runtime-init
static void** g_serCtx = nullptr;   // @ 0x825CAF90

// Per-class supported-version globals (runtime-init)
static uint32_t g_assetVersions_verA     = 0;  // @ 0x825C5864
static uint32_t g_assetVersionsChar_verA = 0;  // @ 0x825C5868
static uint32_t g_assetVersionsCS_verA   = 0;  // @ 0x825C586C
static uint32_t g_assetVersions_verB     = 0;  // @ 0x825C803C  shared across all three
static uint32_t g_assetVersions_verC     = 0;  // @ 0x825C8038  shared across all three

// ── assetVersions ────────────────────────────────────────────────────────────

/**
 * assetVersions::~assetVersions() @ 0x82227FE8 | size: 0x58
 *
 * Standard RAGE deleting destructor.
 * Restores the vtable pointer, calls atSingleton base-class cleanup,
 * then conditionally frees the object itself (bit 0 of the flags arg).
 */
assetVersions::~assetVersions()
{
    // Restore vtable to this class (vtable @ 0x8204E6FC)
    // (The base-class cleanup may have clobbered it.)
    // vtable managed by C++ runtime

    rage::ReleaseSingleton(this);                 // atSingleton::Release() destructor chain

    // If the delete-self flag (bit 0) is set, free the object allocation.
    // The flag is passed via r4 at call time; the scaffold captures it as
    // 'var_r30 & 1'.  We represent it via the standard C++ deleteing-dtor
    // convention; callers that pass flags=1 pass ownership.
    //   if (flags & 1) rage_free(this);
}

/**
 * assetVersions::IsSupported() @ 0x82228048 | size: 0x4C
 *
 * Returns true if 'version' matches any of the three version constants this
 * class was compiled to understand.  Used during save-game load to validate
 * that a stored asset-version record can be deserialised by the current build.
 *
 * @param version  Candidate version number read from save data.
 * @return         Non-zero (true) if the version is known/supported.
 */
bool assetVersions::IsSupported(uint32_t version) const
{
    // Fast path: match the primary (most-recent) version for this class
    if (version == g_assetVersions_verA)
        return true;

    // Check the two shared legacy versions accepted by all three classes
    if (version == g_assetVersions_verB)
        return true;

    return (version == g_assetVersions_verC);
}

/**
 * assetVersions::RegisterFields() @ 0x82227D30 | size: 0x134
 *
 * Registers all 23 serialisable version-number fields with the RAGE
 * field-registration system (sub_821A8F58).  Called once during startup to
 * build the runtime metadata table that drives serialisation of this class.
 *
 * Each call passes:
 *   sub_821A8F58(this, fieldNameStr, &this->fieldN, serCtx, 0)
 *
 * Field layout:
 *   +0x10  [field 0]   — "PostLoadProperties() - 'ProbSameSpin' cannot be greater than 1.0"
 *   +0x14  [field 1]   — "Index"
 *   +0x18  [field 2]   — "ItemIndex"  (compiler suffix-share of "SubItemIndex")
 *   +0x1C  [field 3]   — "Text%d"
 *   +0x20  [field 4]   — "Type%d"     (compiler suffix-share of "Type%d")
 *   +0x24  [field 5]   — "Invalid column type found[ %d ], using default[ %d ]"
 *   +0x28  [field 6]   — "[%d]"
 *   +0x2C  [field 7]   — "found[ %d ], using default[ %d ]"
 *   +0x30  [field 8]   — "ng default[ %d ]"     (tail of above string)
 *   +0x34  [field 9]   — ""                      (empty / null-terminated at that offset)
 *   +0x38  [field 10]  — "MinScrollDelay"         (suffix-share → "lDelay")
 *   +0x3C  [field 11]  — "MaxScrollDelay"         (suffix-share → "crollDelay")
 *   +0x40  [field 12]  — "ScrollAcceleration"
 *   +0x44  [field 13]  — "ScrollAcceleration"     (suffix-share → "ration")
 *   +0x48  [field 14]  — "None"
 *   +0x4C  [field 15]  — "None"
 *   +0x50  [field 16]  — "None"
 *   +0x54  [field 17]  — "None"
 *   +0x58  [field 18]  — "None"
 *   +0x5C  [field 19]  — "None"
 *   +0x60  [field 20]  — "None"
 *   +0x64  [field 21]  — "None"
 *   +0x68  [field 22]  — "None"
 *
 * NOTE: many apparent field-name strings are compiler-optimised suffixes of
 *       longer strings elsewhere in .rdata.  The string VALUES shown above are
 *       the actual C-string content starting at each pointer address.
 *
 * TODO: cross-reference with the original asset-type enum to map field indices
 *       to meaningful asset-category names (characters, animations, maps, …).
 */
void assetVersions::RegisterFields()
{
    // The serialisation context is a runtime-initialised global pointer
    void* ctx = g_serCtx;

    // Field-name string constants (from .rdata; addresses shown for reference)
    static const char* const kFieldNames[23] = {
        /* +0x10 @ 0x82042C90 */ "PostLoadProperties() - 'ProbSameSpin' cannot be greater than 1.0",
        /* +0x14 @ 0x8204E30C */ "Index",
        /* +0x18 @ 0x8204E318 */ "temIndex",       // TODO: tail of "SubItemIndex"
        /* +0x1C @ 0x8204E324 */ "Text%d",
        /* +0x20 @ 0x8204E330 */ "%d",             // TODO: tail of "Type%d"
        /* +0x24 @ 0x8204E33C */ "column type found[ %d ], using default[ %d ]",
        /* +0x28 @ 0x8203B888 */ "[%d]",
        /* +0x2C @ 0x8204E348 */ "found[ %d ], using default[ %d ]",
        /* +0x30 @ 0x8204E358 */ "ng default[ %d ]",
        /* +0x34 @ 0x8204E368 */ "",
        /* +0x38 @ 0x8204E374 */ "lDelay",         // TODO: tail of "MinScrollDelay"
        /* +0x3C @ 0x8204E380 */ "crollDelay",     // TODO: tail of "MaxScrollDelay"
        /* +0x40 @ 0x8204E38C */ "ScrollAcceleration",
        /* +0x44 @ 0x8204E398 */ "ration",         // TODO: tail of "ScrollAcceleration"
        /* +0x48 @ 0x8204E3A8 */ "None",
        /* +0x4C @ 0x8204E3BC */ "None",
        /* +0x50 @ 0x8204E3CC */ "None",
        /* +0x54 @ 0x8204E3DC */ "None",
        /* +0x58 @ 0x8204E3E8 */ "None",
        /* +0x5C @ 0x8204E3F4 */ "None",
        /* +0x60 @ 0x8204E3FC */ "None",
        /* +0x64 @ 0x8204E408 */ "None",
        /* +0x68 @ 0x8204E418 */ "None",
    };

    for (int i = 0; i < 23; i++) {
        sub_821A8F58(this, kFieldNames[i],
                     reinterpret_cast<char*>(this) + 0x10 + i * 4,
                     ctx, 0);
    }
}

/**
 * assetVersions::GetTypeDescriptor() @ 0x82228090 | size: 0xC
 *
 * Returns a pointer to the RAGE type-descriptor (parStructure / atHashString
 * or similar) for this class.  The returned pointer (0x8204E424) lies in the
 * .rdata section and holds a vtable-shaped data block; it is NOT a plain
 * const char* name string.
 *
 * TODO: determine the exact RAGE type returned (parStructure*, atTypeInfo*, …).
 */
const void* assetVersions::GetTypeDescriptor() const
{
    return reinterpret_cast<const void*>(0x8204E424);
}


// ── assetVersionsChar ─────────────────────────────────────────────────────────

/**
 * assetVersionsChar::~assetVersionsChar() @ 0x82228198 | size: 0x4C
 *
 * Identical pattern to assetVersions destructor, but restores to
 * assetVersionsChar vtable (0x8204E764).
 */
assetVersionsChar::~assetVersionsChar()
{
    // vtable managed by C++ runtime
    rage::ReleaseSingleton(this);
    // if (flags & 1) rage_free(this);
}

/**
 * assetVersionsChar::IsSupported() @ 0x822281F8 | size: 0x4C
 *
 * Same version-check logic as assetVersions::IsSupported but uses the
 * character-specific version-A global (0x825C5868 / g_assetVersionsChar_verA).
 */
bool assetVersionsChar::IsSupported(uint32_t version) const
{
    if (version == g_assetVersionsChar_verA)
        return true;
    if (version == g_assetVersions_verB)
        return true;
    return (version == g_assetVersions_verC);
}

/**
 * assetVersionsChar::RegisterFields() @ 0x82228178 | size: 0x28
 *
 * Registers the single character-asset version field at +0x10.
 * Field name key: "None" (@ 0x8204E434 in .rdata).
 */
void assetVersionsChar::RegisterFields()
{
    void* ctx = g_serCtx;
    // Only one field — the character-asset version number at +0x10
    static const char kFieldName[] = "None";   // @ 0x8204E434
    sub_821A8F58(this, kFieldName,
                 reinterpret_cast<char*>(this) + 0x10,
                 ctx, 0);
}

/**
 * assetVersionsChar::GetTypeDescriptor() @ 0x82228240 | size: 0xC
 *
 * Returns type descriptor pointer 0x8204E440 (binary .rdata block, not a
 * plain string).
 */
const void* assetVersionsChar::GetTypeDescriptor() const
{
    return reinterpret_cast<const void*>(0x8204E440);
}


// ── assetVersionsCharSpecific ─────────────────────────────────────────────────

/**
 * assetVersionsCharSpecific::~assetVersionsCharSpecific() @ 0x82228360 | size: 0x58
 *
 * Extended destructor: also frees the heap object pointed to by the first
 * version field (m_fields[0] at +0x10) before handing off to the atSingleton
 * cleanup chain.  This implies +0x10 is an owned heap pointer in this subclass
 * rather than a plain version integer.
 */
assetVersionsCharSpecific::~assetVersionsCharSpecific()
{
    // Free the owned pointer at +0x10 (unlike the base classes, this field
    // holds a heap allocation in assetVersionsCharSpecific)
    void* ownedPtr = *reinterpret_cast<void**>(
                         reinterpret_cast<char*>(this) + 0x10);
    rage_free(ownedPtr);

    // Restore vtable and run atSingleton base-class teardown
    // vtable managed by C++ runtime
    rage::ReleaseSingleton(this);
    // if (flags & 1) rage_free(this);
}

/**
 * assetVersionsCharSpecific::IsSupported() @ 0x822283C8 | size: 0x4C
 *
 * Same three-version check; uses per-character-specific version A at
 * 0x825C586C (g_assetVersionsCS_verA).
 */
bool assetVersionsCharSpecific::IsSupported(uint32_t version) const
{
    if (version == g_assetVersionsCS_verA)
        return true;
    if (version == g_assetVersions_verB)
        return true;
    return (version == g_assetVersions_verC);
}

/**
 * assetVersionsCharSpecific::RegisterFields() @ 0x822282D0 | size: 0x94
 *
 * Registers three fields for the per-character-specific asset-version record.
 * Uses two different serialize-context pointers:
 *   First field  uses lbl_825CAF88 (@ 0x825CAF88) as context — a secondary ctx
 *   Fields 2–3  use lbl_825CAF90 (@ 0x825CAF90) as context  — primary ctx
 *
 * Field name keys (all .rdata):
 *   +0x10 @ 0x8202E5A8 : "i"     (1-character key; tail of a longer string)
 *   +0x14 @ 0x82033444 : binary  (pointer data — key is a non-ASCII address
 *                                  likely pointing into a RAGE type-descriptor)
 *   +0x18 @ 0x8204E454 : binary  (same — points into vtable region)
 *
 * TODO: the field+20 and field+24 name pointers resolve to binary .rdata data
 *       rather than null-terminated strings.  Their values may be RAGE
 *       atHashString* or parMemberType* descriptors rather than plain char*.
 */
void assetVersionsCharSpecific::RegisterFields()
{
    // lbl_825CAF88 is used as the context for the FIRST field registration
    void* ctx2 = *reinterpret_cast<void**>(0x825CAF88);  // secondary context
    void* ctx1 = g_serCtx;                                // primary  context

    // Field 0: +0x10 — name key "i" (@ 0x8202E5A8), secondary context
    sub_821A8F58(this,
                 reinterpret_cast<const char*>(0x8202E5A8),   // "i"
                 reinterpret_cast<char*>(this) + 0x10,
                 ctx2, 0);

    // Field 1: +0x14 — name key @ 0x82033444 (binary/descriptor), primary ctx
    sub_821A8F58(this,
                 reinterpret_cast<const char*>(0x82033444),   // TODO: non-ASCII key
                 reinterpret_cast<char*>(this) + 0x14,
                 ctx1, 0);

    // Field 2: +0x18 — name key @ 0x8204E454 (binary/descriptor), primary ctx
    sub_821A8F58(this,
                 reinterpret_cast<const char*>(0x8204E454),   // TODO: non-ASCII key
                 reinterpret_cast<char*>(this) + 0x18,
                 ctx1, 0);
}

/**
 * assetVersionsCharSpecific::GetTypeDescriptor() @ 0x82228410 | size: 0xC
 *
 * Returns type descriptor pointer 0x8204E460 (.rdata, binary block).
 */
const void* assetVersionsCharSpecific::GetTypeDescriptor() const
{
    return reinterpret_cast<const void*>(0x8204E460);
}

// ─────────────────────────────────────────────────────────────────────────────
// util_61F0 @ 0x823961F0 | size: 0x114
//
// Utility function for initializing state machine context with debug logging.
// Sets up dual 16-byte data structures and transitions to state 2.
//
// Parameters:
//   - pContext: Pointer to context object (contains HSM at +1032)
//   - timeValue: Float time/duration value (stored at +1088)
//   - param: Additional parameter (stored at +52 of HSM context)
// ─────────────────────────────────────────────────────────────────────────────

extern "C" {
    // External function declarations
    void hsmContext_SetNextState_2800(void* hsmContext, int stateIndex);
}

/**
 * util_61F0 @ 0x823961F0 | size: 0x114
 * 
 * Initializes state machine context with logging and dual data structures.
 * Only executes if the initialization flag at +1092 is not set.
 */
void util_61F0(void* pContext, float timeValue, uint32_t param) {
    // Check initialization flag at offset +1092
    uint32_t initFlag = *(uint32_t*)((char*)pContext + 1092);
    
    if (initFlag != 0) {
        return;  // Already initialized
    }
    
    // Get HSM context at offset +1032
    void* hsmContext = (char*)pContext + 1032;
    
    // Debug logging with format string and parameters
    // Format: "eSessionTimeSyncMessage %s"
    // This appears to be session synchronization logging
    const char* logFormat = (const char*)0x820712CC;  // "eSessionTimeSyncMessage %s"
    nop_8240E6D0(logFormat, 0, 0, 0, 4, 0, 4);
    
    // Call virtual method slot 3 on HSM context
    // This is likely an initialization or reset method
    void** vtable = *(void***)hsmContext;
    typedef void (*VirtualMethod)(void*);
    VirtualMethod initMethod = (VirtualMethod)vtable[3];
    initMethod(hsmContext);
    
    // Store parameter at offset +52
    *(uint32_t*)((char*)hsmContext + 52) = param;
    
    // Set flag at offset +60 to 1 (mark as initialized)
    *(uint32_t*)((char*)hsmContext + 60) = 1;
    
    // Set up dual 16-byte data structures
    // These appear to be copied from stack-allocated structures
    // Structure 1: at offset +20 (16 bytes)
    // Structure 2: at offset +36 (16 bytes)
    
    // Initialize structure 1 (at +20)
    uint32_t* struct1 = (uint32_t*)((char*)hsmContext + 20);
    struct1[0] = reinterpret_cast<uintptr_t>(pContext);  // Base context pointer
    struct1[1] = 0x82396690;          // Function pointer: SinglesNetworkClient_6690_p39
    struct1[2] = 0;                   // Zero
    struct1[3] = 4;                   // Count/size
    
    // Initialize structure 2 (at +36)
    uint32_t* struct2 = (uint32_t*)((char*)hsmContext + 36);
    struct2[0] = reinterpret_cast<uintptr_t>(pContext);  // Base context pointer
    struct2[1] = 0x820766A0;          // String pointer (partial): "tered for arbitration"
    struct2[2] = 0;                   // Zero
    struct2[3] = 4;                   // Count/size
    
    // Transition to state 2
    hsmContext_SetNextState_2800(hsmContext, 2);
    
    // Store time value at offset +1088
    *(float*)((char*)pContext + 1088) = timeValue;
}

// ─────────────────────────────────────────────────────────────────────────────
// io_9B88_w @ 0x822F9B88 | size: 0x9C
//
// State transition handler for io system.
// Checks if state flag 2 is set, and if so:
//   1. Sets state flag 1, clears state flag 2
//   2. Calls io_9E30 to perform state-specific operations
//   3. Decrements global loop counter if operation succeeded
//   4. Checks pause/active flags to determine if credits roll should be triggered
//
// This appears to be part of the game's state machine that manages transitions
// between gameplay and frontend/attract modes.
// ─────────────────────────────────────────────────────────────────────────────
void io_9B88_w(io* self) {
    // Check if state flag 2 is set
    if (self->m_stateFlag2 == 0) {
        return;
    }
    
    // Set state flag 1, clear state flag 2
    self->m_stateFlag1 = 1;
    self->m_stateFlag2 = 0;
    
    // Perform state-specific operations
    uint8_t success = io_9E30(self);
    
    // Get global loop object
    LoopObject* loopObj = g_loop_obj_ptr;
    
    // If operation succeeded (returned non-zero), decrement counter
    if (success != 0) {
        loopObj->m_counter--;
    }
    
    // Check if we should trigger credits roll
    // Logic: if NOT paused AND NOT active, return early (don't trigger)
    // Otherwise, trigger credits roll
    if (loopObj->m_pauseFlag == 0 && loopObj->m_activeFlag == 0) {
        return;  // Early exit - don't trigger credits
    }
    
    // Trigger credits roll
    game_AAF8(g_creditsRoll, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pg_4A58_fw @ 0x821F4A58 | size: 0x7C
//
// Page group input handler with conditional flag check.
// Builds a parameter array and delegates to pg_6F68 for processing.
//
// Parameters:
//   - pPageGroup: Page group context object
//   - pInputValue: Pointer to input value (float)
//
// Returns:
//   - true if input was processed successfully
//   - false if processing failed
// ─────────────────────────────────────────────────────────────────────────────

// External declarations
extern uint8_t g_uiInputFlag;  // @ SDA+25804 (0x826064CC) - UI input enable flag
extern bool pg_6F68(void* pInputValue, void* pPageGroup, int eventType, 
                    uint32_t* params, int paramCount);

/**
 * pg_4A58_fw @ 0x821F4A58 | size: 0x7C
 * 
 * Processes page group input with conditional flag checking.
 * Builds a 4-element parameter array and calls pg_6F68 for processing.
 */
bool pg_4A58_fw(void* pPageGroup, float* pInputValue) {
    // Build parameter array on stack
    // Array layout: [1, 0, flag, 2]
    uint32_t params[4];
    params[0] = 1;
    params[1] = 0;
    
    // Check global UI input flag at SDA+25804
    // If flag is set, use 1; otherwise use 0
    params[2] = (g_uiInputFlag != 0) ? 1 : 0;
    
    params[3] = 2;
    
    // Call page group processor
    // Parameters: (inputValue, pageGroup, eventType=19, params, paramCount=2)
    bool result = pg_6F68(pInputValue, pPageGroup, 19, params, 2);
    
    // Return true if processing succeeded (non-zero result)
    return result;
}

/**
 * pg_4900_fw @ 0x821F4900 | size: 0x5C
 * 
 * Page group input handler wrapper.
 * Swaps parameters and calls pg_6F68 with fixed event type and parameters.
 * 
 * Parameters:
 *   - pPageGroup: Page group context object
 *   - pInputValue: Pointer to input value
 * 
 * Returns:
 *   - true if input was processed successfully (non-zero result)
 *   - false if processing failed (zero result)
 */
bool pg_4900_fw(void* pPageGroup, void* pInputValue) {
    // Build parameter array on stack
    // Array layout: [3, 1]
    uint32_t params[2];
    params[0] = 3;
    params[1] = 1;
    
    // Call page group processor with swapped parameters
    // Note: pg_6F68 expects (inputValue, pageGroup, ...) but we receive (pageGroup, inputValue)
    // Parameters: (inputValue, pageGroup, eventType=16, params, paramCount=1)
    uint8_t result = pg_6F68(pInputValue, pPageGroup, 16, params, 1);
    
    // Return true if processing succeeded (non-zero result)
    return (result != 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pg (Page Group) — UI/Menu System Functions
//
// The "pg" prefix indicates page group functions, which are part of the game's
// UI and menu management system. These functions handle UI state, input
// processing, and page transitions.
// ─────────────────────────────────────────────────────────────────────────────

// Forward declaration for pg_6F68 (not yet lifted)
extern "C" uint8_t pg_6F68(void* pageGroup, void* context, int eventType, 
                           uint32_t* eventData, int enableFlag, int reserved);

/**
 * pg_6770_fw @ 0x821F6770 | size: 0x5C
 *
 * Page group event handler wrapper with context/page group argument swap.
 * 
 * This function appears to be a forwarding wrapper that swaps the context
 * and page group arguments before dispatching to the main event handler.
 * The "_fw" suffix likely indicates "forward" or "wrapper".
 *
 * @param context Page group context object
 * @param pageGroup Page group UI object
 * @return true if event was handled successfully, false otherwise
 */
bool pg_6770_fw(void* context, void* pageGroup) {
    // Event data structure: { type: 4, value: 0 }
    // Type 4 may indicate a specific UI event category
    uint32_t eventData[2] = { 4, 0 };
    
    // Call main handler with swapped arguments:
    //   - pageGroup becomes first parameter (the target object)
    //   - context becomes second parameter (the calling context)
    //   - 9 is the event type constant
    //   - eventData contains event-specific data
    //   - 1 enables processing
    //   - 0 is reserved/unused
    uint8_t handled = pg_6F68(pageGroup, context, 9, eventData, 1, 0);
    
    // Return true if event was handled, false otherwise
    return (handled != 0);
}


// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Bink Video Player (52 small functions)
// ─────────────────────────────────────────────────────────────────────────────

// Xbox kernel event functions
extern "C" int KeWaitForSingleObject(void* event, int waitReason, int waitMode, int alertable, void* timeout);
extern "C" int KeSetEvent(void* event, int increment, int wait);
extern "C" int KeResetEvent(void* event);

// ── Pattern A: WaitForEvent[0..7] (8 functions, 24B each) ─────────────────

/**
 * CCalMoviePlayer::WaitForEvent[N] — 8 functions @ 24B each
 *
 * Each waits on one of the 8 KEVENT objects in the player's event
 * array at offsets +84 through +196 (stride 16 bytes per KEVENT).
 * Called to synchronize video decode/render threads.
 */
void CCalMoviePlayer::WaitForEvent0() { KeWaitForSingleObject((char*)this + 84,  3, 1, 0, nullptr); }   // _40
void CCalMoviePlayer::WaitForEvent1() { KeWaitForSingleObject((char*)this + 100, 3, 1, 0, nullptr); }   // _41
void CCalMoviePlayer::WaitForEvent2() { KeWaitForSingleObject((char*)this + 116, 3, 1, 0, nullptr); }   // _42
void CCalMoviePlayer::WaitForEvent3() { KeWaitForSingleObject((char*)this + 132, 3, 1, 0, nullptr); }   // _43
void CCalMoviePlayer::WaitForEvent4() { KeWaitForSingleObject((char*)this + 148, 3, 1, 0, nullptr); }   // _44
void CCalMoviePlayer::WaitForEvent5() { KeWaitForSingleObject((char*)this + 164, 3, 1, 0, nullptr); }   // _45
void CCalMoviePlayer::WaitForEvent6() { KeWaitForSingleObject((char*)this + 180, 3, 1, 0, nullptr); }   // _46
void CCalMoviePlayer::WaitForEvent7() { KeWaitForSingleObject((char*)this + 196, 3, 1, 0, nullptr); }   // _47

// ── Pattern B: SignalEvent[0..7] (8 functions, 16B each) ──────────────────

/**
 * CCalMoviePlayer::SignalEvent[N] — 8 functions @ 16B each
 * Signals one of the 8 KEVENTs to wake a waiting thread.
 */
void CCalMoviePlayer::SignalEvent0() { KeSetEvent((char*)this + 84,  1, 0); }   // _48
void CCalMoviePlayer::SignalEvent1() { KeSetEvent((char*)this + 100, 1, 0); }   // _49
void CCalMoviePlayer::SignalEvent2() { KeSetEvent((char*)this + 116, 1, 0); }   // _50
void CCalMoviePlayer::SignalEvent3() { KeSetEvent((char*)this + 132, 1, 0); }   // _51
void CCalMoviePlayer::SignalEvent4() { KeSetEvent((char*)this + 148, 1, 0); }   // _52
void CCalMoviePlayer::SignalEvent5() { KeSetEvent((char*)this + 164, 1, 0); }   // _53
void CCalMoviePlayer::SignalEvent6() { KeSetEvent((char*)this + 180, 1, 0); }   // _54
void CCalMoviePlayer::SignalEvent7() { KeSetEvent((char*)this + 196, 1, 0); }   // _55

// ── Pattern C: ResetEvent[4..7] (8 functions, 8B each) ────────────────────

/**
 * CCalMoviePlayer::ResetEvent[N] — resets KEVENT to non-signaled state.
 */
void CCalMoviePlayer::ResetEvent4() { KeResetEvent((char*)this + 148); }   // _56
void CCalMoviePlayer::ResetEvent5() { KeResetEvent((char*)this + 164); }   // _57
void CCalMoviePlayer::ResetEvent6() { KeResetEvent((char*)this + 180); }   // _58
void CCalMoviePlayer::ResetEvent7() { KeResetEvent((char*)this + 196); }   // _59

// ── Pattern D: Field getters (5 functions, 8B each) ──────────────────────

/**
 * CCalMoviePlayer field getters — return int32 fields at +212..+228.
 * These expose video properties: width, height, frame count, etc.
 */
int CCalMoviePlayer::GetField212() { return *(int*)((char*)this + 212); }   // _60
int CCalMoviePlayer::GetField216() { return *(int*)((char*)this + 216); }   // _61
int CCalMoviePlayer::GetField220() { return *(int*)((char*)this + 220); }   // _62
int CCalMoviePlayer::GetField224() { return *(int*)((char*)this + 224); }   // _63_a
int CCalMoviePlayer::GetField228() { return *(int*)((char*)this + 228); }   // _63_b

// ── Pattern E: Vtable dispatch thunks (5 functions, 16-48B each) ─────────

/**
 * CCalMoviePlayer vtable dispatch thunks — call specific vtable slots
 * on self and return 0. Used by the COM-like IMediaControl interface.
 */
int CCalMoviePlayer::DispatchSlot32() {  // EC68_p33
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[32])(this);
    return 0;
}

int CCalMoviePlayer::DispatchSlot34() {  // EC88_p33
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[34])(this);
    return 0;
}

int CCalMoviePlayer::DispatchSlot35() {  // ECB8_p33
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[35])(this);
    return 0;
}

int CCalMoviePlayer::DispatchSlot36() {  // ECE8_p33
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[36])(this);
    return 0;
}

int CCalMoviePlayer::DispatchSlot37() {  // ED18_p33
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[37])(this);
    return 0;
}

// ── Pattern F: Unimplemented stubs (2 functions, 56B each) ───────────────

/**
 * CCalMoviePlayer::Rewind / Seek — unimplemented in the final binary.
 * Both log a debug message and return STATUS_NOT_IMPLEMENTED.
 */
int CCalMoviePlayer::Rewind() {  // E758
    nop_8240E6D0("CCalMoviePlayer::Rewind() - not implemented");
    return -1;  // STATUS_NOT_IMPLEMENTED
}

int CCalMoviePlayer::Seek() {  // Seek stub
    nop_8240E6D0("CCalMoviePlayer::Seek() - not implemented");
    return -1;
}

// ── Pattern G: Field setters with change notification (2 functions) ──────

/**
 * CCalMoviePlayer::SetField212 / SetField216 — stores value, if changed
 * and a callback is registered at +64 (with userdata at +80), calls it.
 */
void CCalMoviePlayer::SetField212(int value) {  // sub_63_1
    int oldVal = *(int*)((char*)this + 212);
    *(int*)((char*)this + 212) = value;
    if (value != oldVal) {
        typedef void (*CallbackFn)(void*, int);
        CallbackFn callback = *(CallbackFn*)((char*)this + 64);
        if (callback) {
            void* userData = *(void**)((char*)this + 80);
            callback(userData, value);
        }
    }
}

void CCalMoviePlayer::SetField216(int value) {  // sub_63_2
    int oldVal = *(int*)((char*)this + 216);
    *(int*)((char*)this + 216) = value;
    if (value != oldVal) {
        typedef void (*CallbackFn)(void*, int);
        CallbackFn callback = *(CallbackFn*)((char*)this + 64);
        if (callback) {
            void* userData = *(void**)((char*)this + 80);
            callback(userData, value);
        }
    }
}

// ── Pattern H: Misc unique functions ─────────────────────────────────────

/**
 * CCalMoviePlayer::ClearFiberFlag @ ~16B
 * Clears the fiber state flag at this+10376 with an eieio barrier.
 */
void CCalMoviePlayer::ClearFiberFlag() {  // EB70
    *(uint32_t*)((char*)this + 10376) = 0;
    // Original has eieio (enforce in-order I/O) — memory barrier
}

/**
 * CCalMoviePlayer::GetRemainingFrames @ ~16B
 * Returns field_52 - field_68 (total frames minus processed frames).
 */
int CCalMoviePlayer::GetRemainingFrames() {  // DBD0
    return *(int*)((char*)this + 52) - *(int*)((char*)this + 68);
}

/**
 * CCalMoviePlayer::GetFrameBufferElement @ ~76B
 * Returns pointer to element at front index in ring buffer.
 * Element = base_48 + (frontIndex * 60).
 */
void* CCalMoviePlayer::GetFrontElement() {  // DBE0
    char* base = *(char**)((char*)this + 48);
    int frontIdx = *(int*)((char*)this + 56);
    char* element = base + frontIdx * 60;

    // Activate element via vtable slot 1
    typedef void (*ActivateFn)(void*);
    void** vt = *(void***)element;
    if (vt) ((ActivateFn)vt[1])(element);

    return element;
}

/**
 * CCalMoviePlayer::GetBackElement @ ~76B
 * Same as GetFrontElement but uses the back index at +60.
 */
void* CCalMoviePlayer::GetBackElement() {  // DC30
    char* base = *(char**)((char*)this + 48);
    int backIdx = *(int*)((char*)this + 60);
    char* element = base + backIdx * 60;

    typedef void (*ActivateFn)(void*);
    void** vt = *(void***)element;
    if (vt) ((ActivateFn)vt[1])(element);

    return element;
}

/**
 * CCalMoviePlayer::GetElementByIndex @ ~72B
 * Returns element at arbitrary index in ring buffer.
 */
void* CCalMoviePlayer::GetElementByIndex(int index) {  // DC80_h
    char* base = *(char**)((char*)this + 48);
    char* element = base + index * 60;

    typedef void (*ActivateFn)(void*);
    void** vt = *(void***)element;
    if (vt) ((ActivateFn)vt[1])(element);

    return element;
}

/**
 * CCalMoviePlayer::ComputeFrameSize @ ~40B
 * Returns (field_128 * field_124 * 3) >> 1 — frame buffer size in bytes
 * for YUV 4:2:0 format (width × height × 1.5).
 */
int CCalMoviePlayer::ComputeFrameSize() {  // 3F00_h
    int width = *(int*)((char*)this + 128);
    int height = *(int*)((char*)this + 124);
    return (width * height * 3) >> 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Batch 2: Intermediate Functions (72–176B)
// ─────────────────────────────────────────────────────────────────────────────

// External helpers used by batch 2 functions
extern "C" void CCalMoviePlayer_13(void* obj);  // @ 0x8248FA48 - event cleanup
extern "C" void util_85C8(void* obj);            // @ 0x824885C8 - base destructor
extern "C" void util_0850(void* obj);            // @ 0x82460850 - physics instance release
extern "C" void* _crt_tls_fiber_setup();         // @ 0x82566B78 - fiber context setup
extern "C" void _locale_register(void* ptr);     // @ 0x820C02D0 - memory dealloc
extern "C" void pg_6C80_g(int frames);           // @ 0x82566C80 - wait N frames
extern "C" void pg_6F48(void* ptr);              // @ 0x82566F48 - release page ref

/**
 * CCalMoviePlayer::DtorIntermediate @ 0x824915C8 | size: 0x40
 *
 * Intermediate destructor — resets vtable to base CCalMoviePlayer vtable
 * (0x820092C0), runs event cleanup, then destroys the base object.
 * Called by derived class destructors before their own cleanup.
 */
void CCalMoviePlayer::DtorIntermediate() {
    *(void**)this = (void*)0x820092C0;  // Restore CCalMoviePlayer vtable
    CCalMoviePlayer_13(this);            // Clean up event objects
    util_85C8(this);                     // Destroy base class
}

/**
 * CCalMoviePlayer::SaveAndReplaceFiberContext @ 0x8235EB30 | size: 0x40
 *
 * If a fiber context is already stored at +10376, releases it first.
 * Then creates a new fiber context via _crt_tls_fiber_setup and stores
 * it at +10376. Used to switch video decode onto a dedicated fiber.
 */
void CCalMoviePlayer::SaveAndReplaceFiberContext() {
    uint32_t existingCtx = *(uint32_t*)((char*)this + 10376);
    if (existingCtx != 0) {
        _crt_tls_fiber_setup();  // Release existing context
    }
    void* newCtx = _crt_tls_fiber_setup();
    *(void**)((char*)this + 10376) = newCtx;
}

/**
 * CCalMoviePlayer::QueryPhysicsInstance @ 0x82483AD8 | size: 0x48
 *
 * Queries the physics instance at field +56 via util_0850 (release/query),
 * then writes the current value of field +56 into *outResult.
 * Returns 0.
 */
int CCalMoviePlayer::QueryPhysicsInstance(void* outResult) {
    void* physInst = *(void**)((char*)this + 56);
    util_0850(physInst);
    void* result = *(void**)((char*)this + 56);
    *(void**)outResult = result;
    return 0;
}

/**
 * CCalMoviePlayer::DeletingDestructor @ 0x82487240 | size: 0x64
 *
 * Virtual deleting destructor for the derived CCalMoviePlayer variant.
 * Sets vtable to derived vtable, calls DtorIntermediate (15C8_w),
 * then conditionally frees `this` if flags bit 0 is set.
 *
 * Vtable set: 0x82008A00 (derived CCalMoviePlayer vtable)
 */
void CCalMoviePlayer::DeletingDestructor(int flags) {
    *(void**)this = (void*)0x82008A00;  // Set derived vtable
    DtorIntermediate();                  // Run intermediate destructor chain
    if (flags & 1) {
        _locale_register(this);          // Free object memory
    }
}

/**
 * CCalMoviePlayer::AdvanceReadIndexAtomic @ 0x8248DCC8 | size: 0x68
 *
 * Increments the read index at +56. If it reaches the buffer count at +52,
 * wraps to 0. Then atomically decrements the available-buffer counter at +68
 * using lwarx/stwcx. (PPC atomic CAS loop). This is the consumer side of
 * the lock-free ring buffer — signals one buffer consumed.
 */
void CCalMoviePlayer::AdvanceReadIndexAtomic() {
    int32_t readIdx = *(int32_t*)((char*)this + 56);
    readIdx++;
    *(int32_t*)((char*)this + 56) = readIdx;

    int32_t bufCount = *(int32_t*)((char*)this + 52);
    if ((uint32_t)readIdx >= (uint32_t)bufCount) {
        *(int32_t*)((char*)this + 56) = 0;
    }

    // Atomic decrement of available-buffer count at +68
    volatile int32_t* pAvail = (volatile int32_t*)((char*)this + 68);
    int32_t oldVal, newVal;
    do {
        oldVal = *pAvail;
        newVal = oldVal - 1;
    } while (!__sync_bool_compare_and_swap(pAvail, oldVal, newVal));
}

/**
 * CCalMoviePlayer::AdvanceWriteIndexAtomic @ 0x8248DD30 | size: 0x68
 *
 * Increments the write index at +60. If it reaches the buffer count at +52,
 * wraps to 0. Then atomically increments the available-buffer counter at +68.
 * This is the producer side of the lock-free ring buffer — signals one buffer
 * filled and ready for consumption.
 */
void CCalMoviePlayer::AdvanceWriteIndexAtomic() {
    int32_t writeIdx = *(int32_t*)((char*)this + 60);
    writeIdx++;
    *(int32_t*)((char*)this + 60) = writeIdx;

    int32_t bufCount = *(int32_t*)((char*)this + 52);
    if ((uint32_t)writeIdx >= (uint32_t)bufCount) {
        *(int32_t*)((char*)this + 60) = 0;
    }

    // Atomic increment of available-buffer count at +68
    volatile int32_t* pAvail = (volatile int32_t*)((char*)this + 68);
    int32_t oldVal, newVal;
    do {
        oldVal = *pAvail;
        newVal = oldVal + 1;
    } while (!__sync_bool_compare_and_swap(pAvail, oldVal, newVal));
}

/**
 * CCalMoviePlayer::DispatchVSlot9ZeroArgs @ 0x8248E1E0 | size: 0x48
 *
 * Prepares a 28-byte zero-filled parameter block on the stack and dispatches
 * vtable slot 9 (byte offset +36) with it as the second argument.
 * Slot 9 is the "process command" entry point — zero args = no-op/status query.
 */
void CCalMoviePlayer::DispatchVSlot9ZeroArgs() {
    uint8_t params[28] = {0};
    typedef void (*VSlot9Fn)(void*, void*);
    VSlot9Fn fn = (VSlot9Fn)(*(void***)this)[9];
    fn(this, params);
}

/**
 * CCalMoviePlayer::DispatchVSlot9OneArg @ 0x8248E228 | size: 0x50
 *
 * Same as DispatchVSlot9ZeroArgs but writes arg0 into the first 4 bytes
 * of the parameter block before dispatching.
 */
void CCalMoviePlayer::DispatchVSlot9OneArg(uint32_t arg0) {
    uint8_t params[28] = {0};
    *(uint32_t*)&params[0] = arg0;
    typedef void (*VSlot9Fn)(void*, void*);
    VSlot9Fn fn = (VSlot9Fn)(*(void***)this)[9];
    fn(this, params);
}

/**
 * CCalMoviePlayer::DispatchVSlot9TwoArgs @ 0x8248E278 | size: 0x54
 *
 * Same as DispatchVSlot9OneArg but writes a second argument at offset +4
 * in the parameter block before dispatching vtable slot 9.
 */
void CCalMoviePlayer::DispatchVSlot9TwoArgs(uint32_t a0, uint32_t a1) {
    uint8_t params[28] = {0};
    *(uint32_t*)&params[0] = a0;
    *(uint32_t*)&params[4] = a1;
    typedef void (*VSlot9Fn)(void*, void*);
    VSlot9Fn fn = (VSlot9Fn)(*(void***)this)[9];
    fn(this, params);
}

/**
 * CCalMoviePlayer::SetBufferPairByChannel @ 0x8248E2E8 | size: 0xAC
 *
 * Sets a pair of buffer pointers (bufA at +52/56/60/64, bufB at +68/72/76/80)
 * based on channel index. Calls vtable slot 3 first (lock/prepare), then
 * vtable slot 5 (unlock/commit) after writing. Channel mapping (1-indexed):
 *   channel 1 → offsets +60, +76   (channel A left)
 *   channel 2 → offsets +64, +80   (channel A right)
 *   channel 3 → offsets +52, +68   (channel B / default)
 *   channel 4 → offsets +56, +72   (channel B alternate)
 * Returns 0.
 */
int CCalMoviePlayer::SetBufferPairByChannel(uint32_t channel, uint32_t bufA, uint32_t bufB) {
    // Lock via vtable slot 3
    typedef void (*LockFn)(void*);
    ((LockFn)(*(void***)this)[3])(this);

    switch (channel - 1) {
    case 3:  // channel 4
        *(uint32_t*)((char*)this + 56) = bufA;
        *(uint32_t*)((char*)this + 72) = bufB;
        break;
    case 0:  // channel 1
        *(uint32_t*)((char*)this + 60) = bufA;
        *(uint32_t*)((char*)this + 76) = bufB;
        break;
    case 1:  // channel 2
        *(uint32_t*)((char*)this + 64) = bufA;
        *(uint32_t*)((char*)this + 80) = bufB;
        break;
    default: // channel 3 or out of range
        *(uint32_t*)((char*)this + 52) = bufA;
        *(uint32_t*)((char*)this + 68) = bufB;
        break;
    }

    // Unlock via vtable slot 5
    typedef void (*UnlockFn)(void*);
    ((UnlockFn)(*(void***)this)[5])(this);

    return 0;
}

/**
 * CCalMoviePlayer::SetAudioInterface @ 0x8248E398 | size: 0x7C
 *
 * Replaces the audio interface stored at +44. If the new interface is non-null,
 * activates it via vtable slot 1. If an existing interface was stored, releases
 * it via vtable slot 2 and clears the pointer before storing the new one.
 * Returns 0.
 */
int CCalMoviePlayer::SetAudioInterface(void* audioIface) {
    // Activate new interface if non-null
    if (audioIface != nullptr) {
        typedef void (*ActivateFn)(void*);
        void** vt = *(void***)audioIface;
        ((ActivateFn)vt[1])(audioIface);
    }

    // Release existing interface if present
    void* existing = *(void**)((char*)this + 44);
    if (existing != nullptr) {
        typedef void (*ReleaseFn)(void*);
        void** vt = *(void***)existing;
        ((ReleaseFn)vt[2])(existing);
        *(void**)((char*)this + 44) = nullptr;
    }

    // Store new interface
    *(void**)((char*)this + 44) = audioIface;
    return 0;
}

/**
 * CCalMoviePlayer::QueryVideoProperties @ 0x8248E7C8 | size: 0x9C
 *
 * Queries video dimensions and optionally an extra property.
 * Calls vtable slot 3 (lock), then:
 *   - If outWidth non-null, calls vtable slot 60 to get width, stores result.
 *   - If outHeight non-null, calls vtable slot 61 to get height, stores result.
 * Calls vtable slot 5 (unlock).
 * If outUnused non-null, stores 0 into it.
 * Returns 0.
 */
void CCalMoviePlayer::QueryVideoProperties(void* outWidth, void* outHeight, void* outUnused) {
    // Lock
    typedef void (*LockFn)(void*);
    ((LockFn)(*(void***)this)[3])(this);

    // Query width via vtable slot 60
    if (outWidth != nullptr) {
        typedef uint32_t (*GetWidthFn)(void*);
        uint32_t w = ((GetWidthFn)(*(void***)this)[60])(this);
        *(uint32_t*)outWidth = w;
    }

    // Query height via vtable slot 61
    if (outHeight != nullptr) {
        typedef uint32_t (*GetHeightFn)(void*);
        uint32_t h = ((GetHeightFn)(*(void***)this)[61])(this);
        *(uint32_t*)outHeight = h;
    }

    // Unlock
    typedef void (*UnlockFn)(void*);
    ((UnlockFn)(*(void***)this)[5])(this);

    // Clear optional output
    if (outUnused != nullptr) {
        *(uint32_t*)outUnused = 0;
    }
}

/**
 * CCalMoviePlayer::GetAudioInterfaceRef @ 0x8248E868 | size: 0x60
 *
 * Returns the audio interface pointer at +44 via outAudioIface. If the
 * interface is non-null, activates it first via vtable slot 1.
 * Returns 0.
 */
int CCalMoviePlayer::GetAudioInterfaceRef(void* outAudioIface) {
    void* audioIface = *(void**)((char*)this + 44);
    if (audioIface != nullptr) {
        typedef void (*ActivateFn)(void*);
        void** vt = *(void***)audioIface;
        ((ActivateFn)vt[1])(audioIface);
    }
    void* result = *(void**)((char*)this + 44);
    *(void**)outAudioIface = result;
    return 0;
}

/**
 * CCalMoviePlayer::QueryAudioInterfaceViaProvider @ 0x8248E8C8 | size: 0x9C
 *
 * Queries the audio provider at +44 via its vtable slot 16 (create/get
 * audio interface). If the provider returns a non-null interface, calls
 * CCalMoviePlayer_3AD8_h on it to extract the physics instance, releases
 * the temporary interface via vtable slot 2, and returns the result.
 * If the provider returns null, stores 0 into *outResult.
 * Returns 0.
 */
int CCalMoviePlayer::QueryAudioInterfaceViaProvider(void* outResult) {
    void* provider = *(void**)((char*)this + 44);
    uint32_t tempIface = 0;

    // Call provider vtable slot 16 to get temporary interface
    typedef void (*QueryFn)(void*, void*, uint32_t, uint32_t);
    void** provVt = *(void***)provider;
    ((QueryFn)provVt[16])(provider, &tempIface, 0, 0);

    if (tempIface != 0) {
        // Extract physics instance from temporary interface
        int ret = QueryPhysicsInstance(outResult);

        // Release temporary interface via vtable slot 2
        void** tempVt = *(void***)(uintptr_t)tempIface;
        if (tempVt != nullptr) {
            typedef void (*ReleaseFn)(void*);
            ((ReleaseFn)tempVt[2])((void*)(uintptr_t)tempIface);
        }
        return ret;
    }

    // No interface available
    *(uint32_t*)outResult = 0;
    return 0;
}

/**
 * CCalMoviePlayer::StopPlaybackSimple @ 0x8248EBF0 | size: 0x74
 *
 * Simplified stop: if param is non-null, calls the audio provider's
 * vtable slot 19 (stop) with mode and param. Then calls vtable slot 21
 * (finalize). If field +240 is non-zero, calls pg_6F48 on field +268
 * to release a page reference, and clears field +240.
 */
void CCalMoviePlayer::StopPlaybackSimple(uint32_t mode, void* param) {
    if (param != nullptr) {
        void* provider = *(void**)((char*)this + 44);
        typedef void (*StopFn)(void*, uint32_t, void*);
        void** provVt = *(void***)provider;
        ((StopFn)provVt[19])(provider, mode, param);
    }

    // Finalize via provider vtable slot 21
    void* provider = *(void**)((char*)this + 44);
    typedef void (*FinalizeFn)(void*);
    void** provVt = *(void***)provider;
    ((FinalizeFn)provVt[21])(provider);

    // Release page reference if pending
    int32_t pendingFlag = *(int32_t*)((char*)this + 240);
    if (pendingFlag != 0) {
        void* pageRef = *(void**)((char*)this + 268);
        pg_6F48(pageRef);
        *(int32_t*)((char*)this + 240) = 0;
    }
}

/**
 * CCalMoviePlayer::StopPlaybackFull @ 0x8248EB40 | size: 0xB0
 *
 * Full stop with buffering delay: calls vtable slot 62 (IsPlaying check).
 * If not playing (bit 0 == 0), calls audio provider vtable slot 19 with
 * mode. If the reported buffer count > 5, waits (bufCount - 5) frames
 * via pg_6C80_g to allow buffers to drain. Then calls vtable slot 21
 * (finalize) and clears any pending page reference at +240.
 */
void CCalMoviePlayer::StopPlaybackFull(uint32_t mode) {
    // Check if still playing via vtable slot 62
    typedef uint32_t (*IsPlayingFn)(void*);
    uint32_t playing = ((IsPlayingFn)(*(void***)this)[62])(this);

    if ((playing & 1) == 0) {
        // Call stop on audio provider
        void* provider = *(void**)((char*)this + 44);
        typedef void (*StopFn)(void*, uint32_t, void*);
        uint32_t bufCount = 0;
        void** provVt = *(void***)provider;
        ((StopFn)provVt[19])(provider, mode, &bufCount);

        // If buffer count > 5, wait for buffers to drain
        if ((int32_t)bufCount > 5) {
            pg_6C80_g((int32_t)bufCount - 5);
        }
    }

    // Finalize via provider vtable slot 21
    void* provider2 = *(void**)((char*)this + 44);
    typedef void (*FinalizeFn)(void*);
    void** provVt2 = *(void***)provider2;
    ((FinalizeFn)provVt2[21])(provider2);

    // Release page reference if pending
    int32_t pendingFlag2 = *(int32_t*)((char*)this + 240);
    if (pendingFlag2 != 0) {
        void* pageRef2 = *(void**)((char*)this + 268);
        pg_6F48(pageRef2);
        *(int32_t*)((char*)this + 240) = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Batch 3: Flag Management, State Control, Destructors
// 10 functions (64-132B each)
// ─────────────────────────────────────────────────────────────────────────────

// External helpers
extern void rage_EAC0(void* ptr);   // @ 0x8235EAC0 - AddRef on video callback
extern void rage_EAD8(void* ptr);   // @ 0x8235EAD8 - Release video callback

/**
 * CCalMoviePlayer::SetStatusFlagsA @ 0x8248EF70 | size: 0x64
 *
 * Atomically ORs the given bitmask into the status flags at offset +220.
 * Acquires the lock (vtable slot 3) before modifying, then notifies
 * (vtable slot 5) after the write.
 */
void CCalMoviePlayer::SetStatusFlagsA(uint32_t flagMask) {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    void** vtable = *(void***)this;

    ((LockFn)vtable[3])(this);

    char* self = (char*)this;
    uint32_t current = *(uint32_t*)(self + 220);
    *(uint32_t*)(self + 220) = current | flagMask;

    ((UnlockFn)vtable[5])(this);
}

/**
 * CCalMoviePlayer::SetStatusFlagsB @ 0x8248EFD8 | size: 0x64
 *
 * Atomically ORs the given bitmask into the status flags at offset +224.
 */
void CCalMoviePlayer::SetStatusFlagsB(uint32_t flagMask) {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    void** vtable = *(void***)this;

    ((LockFn)vtable[3])(this);

    char* self = (char*)this;
    uint32_t current = *(uint32_t*)(self + 224);
    *(uint32_t*)(self + 224) = current | flagMask;

    ((UnlockFn)vtable[5])(this);
}

/**
 * CCalMoviePlayer::SetStatusFlagsC @ 0x8248F040 | size: 0x64
 *
 * Atomically ORs the given bitmask into the status flags at offset +228.
 */
void CCalMoviePlayer::SetStatusFlagsC(uint32_t flagMask) {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    void** vtable = *(void***)this;

    ((LockFn)vtable[3])(this);

    char* self = (char*)this;
    uint32_t current = *(uint32_t*)(self + 228);
    *(uint32_t*)(self + 228) = current | flagMask;

    ((UnlockFn)vtable[5])(this);
}

/**
 * CCalMoviePlayer::ClearStatusFlagsA @ 0x8248F0A8 | size: 0x64
 *
 * Atomically clears (AND-NOT) the given bitmask from offset +220.
 */
void CCalMoviePlayer::ClearStatusFlagsA(uint32_t flagMask) {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    void** vtable = *(void***)this;

    ((LockFn)vtable[3])(this);

    char* self = (char*)this;
    uint32_t current = *(uint32_t*)(self + 220);
    *(uint32_t*)(self + 220) = current & ~flagMask;

    ((UnlockFn)vtable[5])(this);
}

/**
 * CCalMoviePlayer::ClearStatusFlagsB @ 0x8248F110 | size: 0x64
 *
 * Atomically clears (AND-NOT) the given bitmask from offset +224.
 */
void CCalMoviePlayer::ClearStatusFlagsB(uint32_t flagMask) {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    void** vtable = *(void***)this;

    ((LockFn)vtable[3])(this);

    char* self = (char*)this;
    uint32_t current = *(uint32_t*)(self + 224);
    *(uint32_t*)(self + 224) = current & ~flagMask;

    ((UnlockFn)vtable[5])(this);
}

/**
 * CCalMoviePlayer::ClearStatusFlagsC @ 0x8248F178 | size: 0x64
 *
 * Atomically clears (AND-NOT) the given bitmask from offset +228.
 */
void CCalMoviePlayer::ClearStatusFlagsC(uint32_t flagMask) {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    void** vtable = *(void***)this;

    ((LockFn)vtable[3])(this);

    char* self = (char*)this;
    uint32_t current = *(uint32_t*)(self + 228);
    *(uint32_t*)(self + 228) = current & ~flagMask;

    ((UnlockFn)vtable[5])(this);
}

/**
 * CCalMoviePlayer::FlushAndNotify @ 0x8248F1E0 | size: 0x84
 *
 * Atomically decrements the semaphore at offset +248 (using compare-and-swap),
 * then calls vtable slot 23 on the audio source to flush pending data.
 * Finally dispatches vtable slots 49 and 51 on this object to notify
 * downstream consumers that a frame has been consumed.
 */
void CCalMoviePlayer::FlushAndNotify() {
    char* self = (char*)this;

    // Atomically decrement the semaphore at +248
    volatile int32_t* pSemaphore = (volatile int32_t*)(self + 248);
    int32_t oldVal;
    do {
        oldVal = *pSemaphore;
    } while (!__sync_bool_compare_and_swap(pSemaphore, oldVal, oldVal - 1));

    // Flush the audio source via vtable slot 23
    void* pAudio = *(void**)(self + 44);
    {
        typedef void (*FlushFn)(void*);
        void** audioVtable = *(void***)pAudio;
        ((FlushFn)audioVtable[23])(pAudio);
    }

    // Notify via vtable slot 49 (signal frame consumed)
    {
        typedef void (*NotifyFn)(void*);
        void** vtable = *(void***)this;
        ((NotifyFn)vtable[49])(this);
    }

    // Notify via vtable slot 51 (signal completion)
    {
        typedef void (*CompleteFn)(void*);
        void** vtable = *(void***)this;
        ((CompleteFn)vtable[51])(this);
    }
}

/**
 * CCalMoviePlayer::SetVideoCallback @ 0x8248FCF0 | size: 0x64
 *
 * Replaces the video callback object at offset +48. If a new callback is
 * provided, calls rage_EAC0 (AddRef) on it. If an existing callback is
 * stored, calls rage_EAD8 (Release) before replacing. Returns 0.
 *
 * This mirrors SetAudioInterface but for the video path, using direct
 * helper calls rather than vtable dispatch for ref-counting.
 */
int CCalMoviePlayer::SetVideoCallback(void* pCallback) {
    // AddRef on the new callback if non-null
    if (pCallback != nullptr) {
        rage_EAC0(pCallback);
    }

    // Release the old callback if present
    char* self = (char*)this;
    void* pOldCallback = *(void**)(self + 48);
    if (pOldCallback != nullptr) {
        rage_EAD8(pOldCallback);
        *(void**)(self + 48) = nullptr;
    }

    // Store the new callback
    *(void**)(self + 48) = pCallback;
    return 0;
}

/**
 * CCalMoviePlayer::ValidateOutputFormat @ 0x8248E450 | size: 0xD0
 *
 * Validates the current output format by checking if both width (vtable
 * slot 60) and height (vtable slot 61) equal 3 (a special "ready" sentinel).
 * If both match, calls vtable slots 68 and 69 with argument 1 to activate
 * the output and returns 0 (success). Otherwise returns E_FAIL (0x80004005).
 *
 * Acquires lock (slot 3) before checking and releases (slot 5) after.
 */
int CCalMoviePlayer::ValidateOutputFormat() {
    typedef void (*LockFn)(void*);
    typedef void (*UnlockFn)(void*);
    typedef int (*GetDimFn)(void*);
    typedef void (*ActivateFn)(void*, int);
    void** vtable = *(void***)this;

    // Lock
    ((LockFn)vtable[3])(this);

    int result;

    // Check width via vtable slot 60
    int width = ((GetDimFn)vtable[60])(this);
    if (width == 3) {
        // Check height via vtable slot 61
        int height = ((GetDimFn)vtable[61])(this);
        if (height == 3) {
            // Both dimensions are the "ready" sentinel — activate outputs
            ((ActivateFn)vtable[68])(this, 1);
            ((ActivateFn)vtable[69])(this, 1);
            result = 0;
        } else {
            result = (int)0x80004005;  // E_FAIL
        }
    } else {
        result = (int)0x80004005;  // E_FAIL
    }

    // Unlock
    ((UnlockFn)vtable[5])(this);

    return result;
}

/**
 * CCalMoviePlayer::ScalarDeletingDtorBase @ 0x82491608 | size: 0x6C
 *
 * Scalar deleting destructor for the base CCalMoviePlayer class.
 * Stamps the base vtable (0x820092C0), runs the event cleanup helper
 * (CCalMoviePlayer_13), destroys the base object (util_85C8), and
 * optionally frees memory if bit 0 of flags is set.
 */
void CCalMoviePlayer::ScalarDeletingDtorBase(int flags) {
    // Stamp base CCalMoviePlayer vtable
    *(void**)this = (void*)0x820092C0;

    // Clean up events
    CCalMoviePlayer_13(this);

    // Destroy base object
    util_85C8(this);

    // Free if ownership flag set
    if (flags & 1) {
        rage_Free(this);
    }
}
