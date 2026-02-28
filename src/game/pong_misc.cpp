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
