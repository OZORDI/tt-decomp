/**
 * pong_misc.cpp — Miscellaneous game classes (fsmMachine, misc helpers)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.9.cpp
 */

#include "pong_misc.hpp"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// Forward declarations for helpers referenced in this file
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    void  rage_free(void* ptr);                  // @ 0x820C00C0
    void* rage_Alloc(uint32_t size);                // @ 0x820DEC88
    void  parStructure_ReleaseInstances(void* slot) __asm__("_util_CE30");                 // @ 0x8234CE30 - init parStructure
}

namespace rage {
    void ReleaseSingleton(void* obj);            // @ 0x821A9420
    void NotifyObservers(void* self, void* type, uint32_t* out);  // observer broadcast
}

// Logging stub (calls internal debug printf, no-op in release builds)
extern void rage_debugLog(const char* fmt, ...); // @ 0x8240E6D0

// Field registration helper (rage serialization system)
// RegisterSerializationField: registers a field of 'obj' with the data system
// Signature: (obj, schemaKey, fieldPtr, fieldDesc, flags)
extern void RegisterSerializedField(void* obj, const void* key, void* fieldPtr, const void* desc, uint32_t flags);

// Generic vtable dispatch typedefs for external COM-like interfaces
// (used by CCalMoviePlayer methods that dispatch on media/enumerator/element objects)
typedef void     (*VFuncV)(void*);
typedef void     (*VFuncVI)(void*, int32_t);
typedef void     (*VFuncVP)(void*, void*);
typedef int32_t  (*VFuncI)(void*);
typedef int32_t  (*VFuncII)(void*, int32_t);
typedef void*    (*VFuncP)(void*);
typedef void     (*VFunc4)(void*, void*, void*, int32_t);
typedef int32_t  (*VFunc3P)(void*, void*, void*);
typedef void     (*VFuncRender)(void*, int32_t);

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
    // NOTE (open): fsmMachine itself may carry additional members beyond
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
extern void parStructure_ReleaseInstances(void* obj) __asm__("_util_CE30");  // @ 0x8234CE30 - Initialize rage::parStructure
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
 * Each parStructure is initialized by parStructure_ReleaseInstances @ 0x8234CE30 which:
 *   - Sets vtable to 0x82065A9C (rage::parStructure vtable)
 *   - Initializes member array pointer at +0x10
 *   - Sets count and flags at +0x14/+0x16
 */
pongSaveFile::pongSaveFile() {
    // Set initial vtables
    // vtable managed by C++ runtime
    // vtable managed by C++ runtime
    
    // Initialize 4 save slots (rage::parStructure instances)
    // Starting from offset 0x10008 (65544) and going backwards by SAVE_SLOT_SIZE each iteration
    uint32_t offset = SAVE_SLOT_INIT_END;
    for (int i = 0; i < SAVE_SLOT_COUNT; i++) {
        offset -= SAVE_SLOT_SIZE;
        void* saveSlot = reinterpret_cast<char*>(this) + offset;
        parStructure_ReleaseInstances(saveSlot);  // Initialize parStructure @ 0x8234CE30
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
    
    // Get raw pointer to the selected save slot
    char* saveSlot = getSaveSlot(slotIndex);
    
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
// The field-registration function (sub_821A8F58 / RegisterSerializationField) stores a triplet
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
 * NOTE (open): cross-reference with the original asset-type enum to map
 *       field indices to meaningful asset-category names (characters,
 *       animations, maps, …).  The enum is not preserved in the binary.
 */
void assetVersions::RegisterFields()
{
    // The serialisation context is a runtime-initialised global pointer
    void* ctx = g_serCtx;

    // Field-name string constants (from .rdata; addresses shown for reference)
    static const char* const kFieldNames[23] = {
        /* +0x10 @ 0x82042C90 */ "PostLoadProperties() - 'ProbSameSpin' cannot be greater than 1.0",
        /* +0x14 @ 0x8204E30C */ "Index",
        /* +0x18 @ 0x8204E318 */ "temIndex",       // compiler-shared tail of "SubItemIndex"
        /* +0x1C @ 0x8204E324 */ "Text%d",
        /* +0x20 @ 0x8204E330 */ "%d",             // compiler-shared tail of "Type%d"
        /* +0x24 @ 0x8204E33C */ "column type found[ %d ], using default[ %d ]",
        /* +0x28 @ 0x8203B888 */ "[%d]",
        /* +0x2C @ 0x8204E348 */ "found[ %d ], using default[ %d ]",
        /* +0x30 @ 0x8204E358 */ "ng default[ %d ]",
        /* +0x34 @ 0x8204E368 */ "",
        /* +0x38 @ 0x8204E374 */ "lDelay",         // compiler-shared tail of "MinScrollDelay"
        /* +0x3C @ 0x8204E380 */ "crollDelay",     // compiler-shared tail of "MaxScrollDelay"
        /* +0x40 @ 0x8204E38C */ "ScrollAcceleration",
        /* +0x44 @ 0x8204E398 */ "ration",         // compiler-shared tail of "ScrollAcceleration"
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
 * NOTE (open): the exact RAGE type of the descriptor at 0x8204E424 (parStructure*,
 *       atTypeInfo*, …) is not yet resolved — the target .rdata block is a
 *       vtable-shaped descriptor rather than a plain string.
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
 * NOTE (open): the field+20 and field+24 name pointers resolve to binary
 *       .rdata data rather than null-terminated strings.  They are most
 *       likely RAGE atHashString* or parMemberType* descriptors rather
 *       than plain char* (see also the GetTypeDescriptor descriptor note).
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
                 reinterpret_cast<const char*>(0x82033444),   // rdata descriptor, non-ASCII key
                 reinterpret_cast<char*>(this) + 0x14,
                 ctx1, 0);

    // Field 2: +0x18 — name key @ 0x8204E454 (binary/descriptor), primary ctx
    sub_821A8F58(this,
                 reinterpret_cast<const char*>(0x8204E454),   // rdata descriptor, non-ASCII key
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
// hsmContext_BeginTimeSyncState @ 0x823961F0 | size: 0x114
// (was: util_61F0 — renamed to reflect HSM state + time-sync behavior)
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
    void hsmContext_SetNextState(void* hsmContext, int stateIndex);
}

/**
 * hsmContext_BeginTimeSyncState @ 0x823961F0 | size: 0x114
 *
 * Initializes state machine context with logging and dual data structures.
 * Only executes if the initialization flag at +1092 is not set.
 */
void hsmContext_BeginTimeSyncState(void* pContext, float timeValue, uint32_t param) {
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
    rage_debugLog(logFormat, 0, 0, 0, 4, 0, 4);
    
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
    hsmContext_SetNextState(hsmContext, 2);
    
    // Store time value at offset +1088
    *(float*)((char*)pContext + 1088) = timeValue;
}

// ─────────────────────────────────────────────────────────────────────────────
// io_ProcessStateTransition @ 0x822F9B88 | size: 0x9C
// (was: io_9B88_w)
//
// State transition handler for io system.
// Checks if state flag 2 is set, and if so:
//   1. Sets state flag 1, clears state flag 2
//   2. Calls io_ExecuteStateAction to perform state-specific operations
//   3. Decrements global loop counter if operation succeeded
//   4. Checks pause/active flags to determine if credits roll should be triggered
//
// This appears to be part of the game's state machine that manages transitions
// between gameplay and frontend/attract modes.
// ─────────────────────────────────────────────────────────────────────────────
void io_ProcessStateTransition(io* self) {
    // Check if state flag 2 is set
    if (self->m_stateFlag2 == 0) {
        return;
    }
    
    // Set state flag 1, clear state flag 2
    self->m_stateFlag1 = 1;
    self->m_stateFlag2 = 0;
    
    // Perform state-specific operations
    uint8_t success = io_ExecuteStateAction(self);
    
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
    CreditsRoll_Deactivate(g_creditsRoll, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// bkGroup_DispatchInputWithUiFlag @ 0x821F4A58 | size: 0x7C (was pg_4A58_fw)
//
// Page group input handler with conditional flag check.
// Builds a parameter array and delegates to bkGroup_DispatchEvent for processing.
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
extern bool bkGroup_DispatchEvent(void* pInputValue, void* pPageGroup, int eventType,
                    uint32_t* params, int paramCount) __asm__("_pg_6F68");

/**
 * bkGroup_DispatchInputWithUiFlag @ 0x821F4A58 | size: 0x7C
 *
 * Processes page group input with conditional flag checking.
 * Builds a 4-element parameter array and calls bkGroup_DispatchEvent for processing.
 */
bool bkGroup_DispatchInputWithUiFlag(void* pPageGroup, float* pInputValue) {
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
    bool result = bkGroup_DispatchEvent(pInputValue, pPageGroup, 19, params, 2);
    
    // Return true if processing succeeded (non-zero result)
    return result;
}

/**
 * bkGroup_DispatchSelectEvent @ 0x821F4900 | size: 0x5C (was pg_4900_fw)
 * 
 * Page group input handler wrapper.
 * Swaps parameters and calls bkGroup_DispatchEvent with fixed event type and parameters.
 * 
 * Parameters:
 *   - pPageGroup: Page group context object
 *   - pInputValue: Pointer to input value
 * 
 * Returns:
 *   - true if input was processed successfully (non-zero result)
 *   - false if processing failed (zero result)
 */
bool bkGroup_DispatchSelectEvent(void* pPageGroup, void* pInputValue) {
    // Build parameter array on stack
    // Array layout: [3, 1]
    uint32_t params[2];
    params[0] = 3;
    params[1] = 1;
    
    // Call page group processor with swapped parameters
    // Note: bkGroup_DispatchEvent expects (inputValue, pageGroup, ...) but we receive (pageGroup, inputValue)
    // Parameters: (inputValue, pageGroup, eventType=16, params, paramCount=1)
    uint8_t result = bkGroup_DispatchEvent(pInputValue, pPageGroup, 16, params, 1);
    
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

// Forward declaration for bkGroup_DispatchEvent (not yet lifted)
extern "C" uint8_t bkGroup_DispatchEvent(void* pageGroup, void* context, int eventType,
                           uint32_t* eventData, int enableFlag, int reserved) __asm__("_pg_6F68");

/**
 * bkGroup_DispatchCancelEvent @ 0x821F6770 | size: 0x5C (was pg_6770_fw)
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
bool bkGroup_DispatchCancelEvent(void* context, void* pageGroup) {
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
    uint8_t handled = bkGroup_DispatchEvent(pageGroup, context, 9, eventData, 1, 0);
    
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
void CCalMoviePlayer::WaitForEvent0() { KeWaitForSingleObject(&m_events[0],  3, 1, 0, nullptr); }   // _40
void CCalMoviePlayer::WaitForEvent1() { KeWaitForSingleObject(&m_events[16], 3, 1, 0, nullptr); }   // _41
void CCalMoviePlayer::WaitForEvent2() { KeWaitForSingleObject(&m_events[32], 3, 1, 0, nullptr); }   // _42
void CCalMoviePlayer::WaitForEvent3() { KeWaitForSingleObject(&m_events[48], 3, 1, 0, nullptr); }   // _43
void CCalMoviePlayer::WaitForEvent4() { KeWaitForSingleObject(&m_events[64], 3, 1, 0, nullptr); }   // _44
void CCalMoviePlayer::WaitForEvent5() { KeWaitForSingleObject(&m_events[80], 3, 1, 0, nullptr); }   // _45
void CCalMoviePlayer::WaitForEvent6() { KeWaitForSingleObject(&m_events[96], 3, 1, 0, nullptr); }   // _46
void CCalMoviePlayer::WaitForEvent7() { KeWaitForSingleObject(&m_events[112], 3, 1, 0, nullptr); }   // _47

// ── Pattern B: SignalEvent[0..7] (8 functions, 16B each) ──────────────────

/**
 * CCalMoviePlayer::SignalEvent[N] — 8 functions @ 16B each
 * Signals one of the 8 KEVENTs to wake a waiting thread.
 */
void CCalMoviePlayer::SignalEvent0() { KeSetEvent(&m_events[0],  1, 0); }   // _48
void CCalMoviePlayer::SignalEvent1() { KeSetEvent(&m_events[16], 1, 0); }   // _49
void CCalMoviePlayer::SignalEvent2() { KeSetEvent(&m_events[32], 1, 0); }   // _50
void CCalMoviePlayer::SignalEvent3() { KeSetEvent(&m_events[48], 1, 0); }   // _51
void CCalMoviePlayer::SignalEvent4() { KeSetEvent(&m_events[64], 1, 0); }   // _52
void CCalMoviePlayer::SignalEvent5() { KeSetEvent(&m_events[80], 1, 0); }   // _53
void CCalMoviePlayer::SignalEvent6() { KeSetEvent(&m_events[96], 1, 0); }   // _54
void CCalMoviePlayer::SignalEvent7() { KeSetEvent(&m_events[112], 1, 0); }   // _55

// ── Pattern C: ResetEvent[4..7] (8 functions, 8B each) ────────────────────

/**
 * CCalMoviePlayer::ResetEvent[N] — resets KEVENT to non-signaled state.
 */
void CCalMoviePlayer::ResetEvent4() { KeResetEvent(&m_events[64]); }   // _56
void CCalMoviePlayer::ResetEvent5() { KeResetEvent(&m_events[80]); }   // _57
void CCalMoviePlayer::ResetEvent6() { KeResetEvent(&m_events[96]); }   // _58
void CCalMoviePlayer::ResetEvent7() { KeResetEvent(&m_events[112]); }   // _59

// ── Pattern D: Field getters (5 functions, 8B each) ──────────────────────

/**
 * CCalMoviePlayer field getters — return int32 fields at +212..+228.
 * These expose video properties: width, height, frame count, etc.
 */
int CCalMoviePlayer::GetField212() { return m_statusA; }   // _60
int CCalMoviePlayer::GetField216() { return m_statusB; }   // _61
int CCalMoviePlayer::GetField220() { return m_isPlaying; }   // _62
int CCalMoviePlayer::GetField224() { return m_statusFieldB; }   // _63_a
int CCalMoviePlayer::GetField228() { return m_statusFieldC; }   // _63_b

// ── Pattern E: Vtable dispatch thunks (5 functions, 16-48B each) ─────────

/**
 * CCalMoviePlayer vtable dispatch thunks — call specific vtable slots
 * on self and return 0. Used by the COM-like IMediaControl interface.
 */
int CCalMoviePlayer::DispatchSlot32() {  // EC68_p33
    MediaControl_Run();
    return 0;
}

int CCalMoviePlayer::DispatchSlot34() {  // EC88_p33
    MediaControl_Pause();
    return 0;
}

int CCalMoviePlayer::DispatchSlot35() {  // ECB8_p33
    MediaControl_Stop();
    return 0;
}

int CCalMoviePlayer::DispatchSlot36() {  // ECE8_p33
    MediaControl_GetState();
    return 0;
}

int CCalMoviePlayer::DispatchSlot37() {  // ED18_p33
    MediaControl_StopWhenReady();
    return 0;
}

// ── Pattern F: Unimplemented stubs (2 functions, 56B each) ───────────────

/**
 * CCalMoviePlayer::Rewind / Seek — unimplemented in the final binary.
 * Both log a debug message and return STATUS_NOT_IMPLEMENTED.
 */
int CCalMoviePlayer::Rewind() {  // E758
    rage_debugLog("CCalMoviePlayer::Rewind() - not implemented");
    return -1;  // STATUS_NOT_IMPLEMENTED
}

int CCalMoviePlayer::Seek() {  // Seek stub
    rage_debugLog("CCalMoviePlayer::Seek() - not implemented");
    return -1;
}

// ── Pattern G: Field setters with change notification (2 functions) ──────

/**
 * CCalMoviePlayer::SetField212 / SetField216 — stores value, if changed
 * and a callback is registered at +64 (with userdata at +80), calls it.
 */
void CCalMoviePlayer::SetField212(int value) {  // sub_63_1
    int oldVal = m_statusA;
    m_statusA = value;
    if (value != oldVal) {
        typedef void (*CallbackFn)(void*, int);
        CallbackFn callback = (CallbackFn)m_pCallback;
        if (callback) {
            callback(m_pCallbackUserData, value);
        }
    }
}

void CCalMoviePlayer::SetField216(int value) {  // sub_63_2
    int oldVal = m_statusB;
    m_statusB = value;
    if (value != oldVal) {
        typedef void (*CallbackFn)(void*, int);
        CallbackFn callback = (CallbackFn)m_pCallback;
        if (callback) {
            callback(m_pCallbackUserData, value);
        }
    }
}

// ── Pattern H: Misc unique functions ─────────────────────────────────────

/**
 * CCalMoviePlayer::ClearFiberFlag @ ~16B
 * Clears the fiber state flag at this+10376 with an eieio barrier.
 */
void CCalMoviePlayer::ClearFiberFlag() {  // EB70
    m_fiberContext = 0;
    // Original has eieio (enforce in-order I/O) — memory barrier
}

/**
 * CCalMoviePlayer::GetRemainingFrames @ ~16B
 * Returns field_52 - field_68 (total frames minus processed frames).
 */
int CCalMoviePlayer::GetRemainingFrames() {  // DBD0
    return m_bufferCounterA - (int32_t)m_pendingCounter;
}

/**
 * CCalMoviePlayer::GetFrameBufferElement @ ~76B
 * Returns pointer to element at front index in ring buffer.
 * Element = base_48 + (frontIndex * 60).
 */
void* CCalMoviePlayer::GetFrontElement() {  // DBE0
    char* base = (char*)m_pBufferObject;
    int frontIdx = m_readIndex;
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
    char* base = (char*)m_pBufferObject;
    int backIdx = m_writeIndex;
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
    char* base = (char*)m_pBufferObject;
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
    // NOTE: offsets +124/+128 overlap with m_events[] — different variant layout
    int width = *(int*)((char*)&m_events[44]);   // +128 within events region
    int height = *(int*)((char*)&m_events[40]);  // +124 within events region
    return (width * height * 3) >> 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Batch 2: Intermediate Functions (72–176B)
// ─────────────────────────────────────────────────────────────────────────────

// External helpers used by batch 2+ functions
extern "C" void pong_EmptyVirtualDestructor(void* obj) __asm__("_util_85C8");            // @ 0x824885C8 - base destructor
extern "C" void phInst_Release(void* obj);            // @ 0x82460850 - physics instance release
extern "C" void* _crt_tls_fiber_setup();         // @ 0x82566B78 - fiber context setup
extern "C" void _locale_register(void* ptr);     // @ 0x820C02D0 - memory dealloc
extern "C" void pg_SleepYield(int frames);           // @ 0x82566C80 - wait N frames
extern "C" void sysIpcHandle_Release(void* ptr) __asm__("_pg_6F48");              // @ 0x82566F48 - release page ref
extern "C" uint32_t sysThread_WaitTimeout(void* handle, int timeout) __asm__("_pg_C3B8_g"); // @ 0x8242C3B8 - wait with timeout
extern "C" void sysIpcHandle_Close(void* handle) __asm__("_pg_6F10_g");                     // @ 0x82566F10 - close handle
extern "C" void* sysThread_Create(void* r3, uint32_t stackSize, void* entryPoint,
                            void* context, uint32_t priority, void* r8) __asm__("_pg_6F88_w"); // @ 0x82566F88 - create thread
extern "C" int xe_SetThreadProcessor(void* threadHandle, uint32_t proc); // @ 0x8242B8A8 - defined in src/rage/core/app_init.c (accepts thread handle pointer)
extern "C" void rmcDrawable_Release(void* ptr) __asm__("_rage_EAD8");            // @ 0x8235EAD8 - release callback object

// Thread entry points for CCalMoviePlayer worker threads
extern "C" void CCalMoviePlayer_EC88_p33(void* ctx); // @ 0x8248EC88 - FillReadBufferA entry
extern "C" void CCalMoviePlayer_ECB8_p33(void* ctx); // @ 0x8248ECB8 - FillReadBufferB entry
extern "C" void CCalMoviePlayer_ECE8_p33(void* ctx); // @ 0x8248ECE8 - WriteThreadProcA entry
extern "C" void CCalMoviePlayer_ED18_p33(void* ctx); // @ 0x8248ED18 - WriteThreadProcB entry

// Helper functions used by write threads
extern "C" uint32_t atSingleton_vfn_28_DBC8_1(void* obj); // @ 0x8248DBC8 - check has element
extern "C" void* CCalRingBuffer_PeekWriteSlot(void* obj) __asm__("_CCalMoviePlayer_DBE0");         // @ 0x8248DBE0 - get next element
extern "C" void CCalRingBuffer_AdvanceWriteIndex(void* obj) __asm__("_CCalMoviePlayer_DCC8");           // @ 0x8248DCC8 - advance read index atomic
extern "C" void CCalMoviePlayer_AcquireFiberContext(void* obj) __asm__("_CCalMoviePlayer_EB30");           // @ 0x8235EB30 - save/replace fiber context
extern "C" void CCalMoviePlayer_ReleaseFiberContext(void* obj) __asm__("_CCalMoviePlayer_EB70");           // @ 0x8235EB70 - clear fiber flag
extern "C" void grcBuffer_Release(void* obj) __asm__("_grc_BDC0");  // @ 0x8235BDC0 - release grc resource

/**
 * CCalMoviePlayer::DtorIntermediate @ 0x824915C8 | size: 0x40
 *
 * Intermediate destructor — resets vtable to base CCalMoviePlayer vtable
 * (0x820092C0), runs event cleanup, then destroys the base object.
 * Called by derived class destructors before their own cleanup.
 */
void CCalMoviePlayer::DtorIntermediate() {
    *(void**)this = (void*)0x820092C0;  // Restore CCalMoviePlayer vtable
    CleanupResources();                  // Clean up threads and resources
    pong_EmptyVirtualDestructor(this);                     // Destroy base class
}

/**
 * CCalMoviePlayer::SaveAndReplaceFiberContext @ 0x8235EB30 | size: 0x40
 *
 * If a fiber context is already stored at +10376, releases it first.
 * Then creates a new fiber context via _crt_tls_fiber_setup and stores
 * it at +10376. Used to switch video decode onto a dedicated fiber.
 */
void CCalMoviePlayer::SaveAndReplaceFiberContext() {
    if (m_fiberContext != 0) {
        _crt_tls_fiber_setup();  // Release existing context
    }
    void* newCtx = _crt_tls_fiber_setup();
    m_fiberContext = (uint32_t)(uintptr_t)newCtx;
}

/**
 * CCalMoviePlayer::QueryPhysicsInstance @ 0x82483AD8 | size: 0x48
 *
 * Queries the physics instance at field +56 via phInst_Release (release/query),
 * then writes the current value of field +56 into *outResult.
 * Returns 0.
 */
int CCalMoviePlayer::QueryPhysicsInstance(void* outResult) {
    phInst_Release((void*)m_readIndex);
    *(void**)outResult = (void*)m_readIndex;
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
    int32_t readIdx = m_readIndex;
    readIdx++;
    m_readIndex = readIdx;

    if ((uint32_t)readIdx >= (uint32_t)m_bufferCounterA) {
        m_readIndex = 0;
    }

    // Atomic decrement of pending-buffer count (consumer side)
    volatile int32_t* pPending = reinterpret_cast<volatile int32_t*>(&m_pendingCounter);
    int32_t oldVal, newVal;
    do {
        oldVal = *pPending;
        newVal = oldVal - 1;
    } while (!__sync_bool_compare_and_swap(pPending, oldVal, newVal));
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
    int32_t writeIdx = m_writeIndex;
    writeIdx++;
    m_writeIndex = writeIdx;

    if ((uint32_t)writeIdx >= (uint32_t)m_bufferCounterA) {
        m_writeIndex = 0;
    }

    // Atomic increment of pending-buffer count (producer side)
    volatile int32_t* pPending = reinterpret_cast<volatile int32_t*>(&m_pendingCounter);
    int32_t oldVal, newVal;
    do {
        oldVal = *pPending;
        newVal = oldVal + 1;
    } while (!__sync_bool_compare_and_swap(pPending, oldVal, newVal));
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
    ProcessCommand(params);
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
    ProcessCommand(params);
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
    ProcessCommand(params);
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
    Lock();

    // NOTE: fields +0xFC/+0x100/+0x104 are overlaid — they serve as
    //       m_pendingCounter/m_writePosition/m_hThreadFillA in the ring-buffer
    //       code paths, but are reused here as the channel-pair buffer slots.
    switch (channel - 1) {
    case 3:  // channel 4
        m_readIndex = (int32_t)bufA;
        m_writePosition = bufB;
        break;
    case 0:  // channel 1
        m_writeIndex = (int32_t)bufA;
        m_hThreadFillA = reinterpret_cast<void*>(static_cast<uintptr_t>(bufB));
        break;
    case 1:  // channel 2
        *(uint32_t*)&m_pCallback = bufA;
        *(uint32_t*)&m_pCallbackUserData = bufB;
        break;
    default: // channel 3 or out of range
        m_bufferCounterA = (int32_t)bufA;
        m_pendingCounter = bufB;
        break;
    }

    // Unlock via vtable slot 5
    Unlock();

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
    void* existing = m_pMediaSource;
    if (existing != nullptr) {
        typedef void (*ReleaseFn)(void*);
        void** vt = *(void***)existing;
        ((ReleaseFn)vt[2])(existing);
        m_pMediaSource = nullptr;
    }

    // Store new interface
    m_pMediaSource = audioIface;
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
    Lock();

    // Query width via vtable slot 60
    if (outWidth != nullptr) {
        uint32_t w = GetStatusFlagsA();
        *(uint32_t*)outWidth = w;
    }

    // Query height via vtable slot 61
    if (outHeight != nullptr) {
        uint32_t h = GetStatusFlagsB();
        *(uint32_t*)outHeight = h;
    }

    // Unlock
    Unlock();

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
    void* audioIface = m_pMediaSource;
    if (audioIface != nullptr) {
        typedef void (*ActivateFn)(void*);
        void** vt = *(void***)audioIface;
        ((ActivateFn)vt[1])(audioIface);
    }
    void* result = m_pMediaSource;
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
    void* provider = m_pMediaSource;
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
 * (finalize). If field +240 is non-zero, calls sysIpcHandle_Release on field +268
 * to release a page reference, and clears field +240.
 */
void CCalMoviePlayer::StopPlaybackSimple(uint32_t mode, void* param) {
    if (param != nullptr) {
        void* provider = m_pMediaSource;
        typedef void (*StopFn)(void*, uint32_t, void*);
        void** provVt = *(void***)provider;
        ((StopFn)provVt[19])(provider, mode, param);
    }

    // Finalize via provider vtable slot 21
    void* provider = m_pMediaSource;
    typedef void (*FinalizeFn)(void*);
    void** provVt = *(void***)provider;
    ((FinalizeFn)provVt[21])(provider);

    // Release page reference if pending
    if (m_activeFlag != 0) {
        sysIpcHandle_Release(m_hThreadWriteA);
        m_activeFlag = 0;
    }
}

/**
 * CCalMoviePlayer::StopPlaybackFull @ 0x8248EB40 | size: 0xB0
 *
 * Full stop with buffering delay: calls vtable slot 62 (IsPlaying check).
 * If not playing (bit 0 == 0), calls audio provider vtable slot 19 with
 * mode. If the reported buffer count > 5, waits (bufCount - 5) frames
 * via pg_SleepYield to allow buffers to drain. Then calls vtable slot 21
 * (finalize) and clears any pending page reference at +240.
 */
void CCalMoviePlayer::StopPlaybackFull(uint32_t mode) {
    // Check if still playing via vtable slot 62
    uint32_t playing = IsPlaying();

    if ((playing & 1) == 0) {
        // Call stop on audio provider
        void* provider = m_pMediaSource;
        typedef void (*StopFn)(void*, uint32_t, void*);
        uint32_t bufCount = 0;
        void** provVt = *(void***)provider;
        ((StopFn)provVt[19])(provider, mode, &bufCount);

        // If buffer count > 5, wait for buffers to drain
        if ((int32_t)bufCount > 5) {
            pg_SleepYield((int32_t)bufCount - 5);
        }
    }

    // Finalize via provider vtable slot 21
    void* provider2 = m_pMediaSource;
    typedef void (*FinalizeFn)(void*);
    void** provVt2 = *(void***)provider2;
    ((FinalizeFn)provVt2[21])(provider2);

    // Release page reference if pending
    if (m_activeFlag != 0) {
        sysIpcHandle_Release(m_hThreadWriteA);
        m_activeFlag = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Batch 3: Flag Management, State Control, Destructors
// 10 functions (64-132B each)
// ─────────────────────────────────────────────────────────────────────────────

// External helpers
extern void rmcDrawable_AddRef(void* ptr) __asm__("_rage_EAC0");   // @ 0x8235EAC0 - AddRef on video callback
extern void rmcDrawable_Release(void* ptr) __asm__("_rage_EAD8");   // @ 0x8235EAD8 - Release video callback

/**
 * CCalMoviePlayer::SetStatusFlagsA @ 0x8248EF70 | size: 0x64
 *
 * Atomically ORs the given bitmask into the status flags at offset +220.
 * Acquires the lock (vtable slot 3) before modifying, then notifies
 * (vtable slot 5) after the write.
 */
void CCalMoviePlayer::SetStatusFlagsA(uint32_t flagMask) {

    Lock();

    m_isPlaying |= flagMask;

    Unlock();
}

/**
 * CCalMoviePlayer::SetStatusFlagsB @ 0x8248EFD8 | size: 0x64
 *
 * Atomically ORs the given bitmask into the status flags at offset +224.
 */
void CCalMoviePlayer::SetStatusFlagsB(uint32_t flagMask) {

    Lock();

    m_statusFieldB |= flagMask;

    Unlock();
}

/**
 * CCalMoviePlayer::SetStatusFlagsC @ 0x8248F040 | size: 0x64
 *
 * Atomically ORs the given bitmask into the status flags at offset +228.
 */
void CCalMoviePlayer::SetStatusFlagsC(uint32_t flagMask) {

    Lock();

    m_statusFieldC |= flagMask;

    Unlock();
}

/**
 * CCalMoviePlayer::ClearStatusFlagsA @ 0x8248F0A8 | size: 0x64
 *
 * Atomically clears (AND-NOT) the given bitmask from offset +220.
 */
void CCalMoviePlayer::ClearStatusFlagsA(uint32_t flagMask) {

    Lock();

    m_isPlaying &= ~flagMask;

    Unlock();
}

/**
 * CCalMoviePlayer::ClearStatusFlagsB @ 0x8248F110 | size: 0x64
 *
 * Atomically clears (AND-NOT) the given bitmask from offset +224.
 */
void CCalMoviePlayer::ClearStatusFlagsB(uint32_t flagMask) {

    Lock();

    m_statusFieldB &= ~flagMask;

    Unlock();
}

/**
 * CCalMoviePlayer::ClearStatusFlagsC @ 0x8248F178 | size: 0x64
 *
 * Atomically clears (AND-NOT) the given bitmask from offset +228.
 */
void CCalMoviePlayer::ClearStatusFlagsC(uint32_t flagMask) {

    Lock();

    m_statusFieldC &= ~flagMask;

    Unlock();
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

    // Atomically decrement the semaphore (buffer counter B)
    volatile int32_t* pSemaphore = (volatile int32_t*)&m_bufferCounterB;
    int32_t oldVal;
    do {
        oldVal = *pSemaphore;
    } while (!__sync_bool_compare_and_swap(pSemaphore, oldVal, oldVal - 1));

    // Flush the audio source via vtable slot 23
    void* pAudio = m_pMediaSource;
    {
        typedef void (*FlushFn)(void*);
        void** audioVtable = *(void***)pAudio;
        ((FlushFn)audioVtable[23])(pAudio);
    }

    // Notify via vtable slot 49 (signal frame consumed)
    {
        SignalEvent1();
    }

    // Notify via vtable slot 51 (signal completion)
    {
        SignalEvent3();
    }
}

/**
 * CCalMoviePlayer::SetVideoCallback @ 0x8248FCF0 | size: 0x64
 *
 * Replaces the video callback object at offset +48. If a new callback is
 * provided, calls rmcDrawable_AddRef (AddRef) on it. If an existing callback is
 * stored, calls rmcDrawable_Release (Release) before replacing. Returns 0.
 *
 * This mirrors SetAudioInterface but for the video path, using direct
 * helper calls rather than vtable dispatch for ref-counting.
 */
int CCalMoviePlayer::SetVideoCallback(void* pCallback) {
    // AddRef on the new callback if non-null
    if (pCallback != nullptr) {
        rmcDrawable_AddRef(pCallback);
    }

    // Release the old callback if present
    void* pOldCallback = m_pBufferObject;
    if (pOldCallback != nullptr) {
        rmcDrawable_Release(pOldCallback);
        m_pBufferObject = nullptr;
    }

    // Store the new callback
    m_pBufferObject = pCallback;
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

    // Lock
    Lock();

    int result;

    // Check width via vtable slot 60
    int width = GetStatusFlagsA();
    if (width == 3) {
        // Check height via vtable slot 61
        int height = GetStatusFlagsB();
        if (height == 3) {
            // Both dimensions are the "ready" sentinel — activate outputs
            SetStatusMaskA(1);
            SetStatusMaskB(1);
            result = 0;
        } else {
            result = (int)0x80004005;  // E_FAIL
        }
    } else {
        result = (int)0x80004005;  // E_FAIL
    }

    // Unlock
    Unlock();

    return result;
}

/**
 * CCalMoviePlayer::ScalarDeletingDtorBase @ 0x82491608 | size: 0x6C
 *
 * Scalar deleting destructor for the base CCalMoviePlayer class.
 * Stamps the base vtable (0x820092C0), runs the event cleanup helper
 * (CCalMoviePlayer_13), destroys the base object (pong_EmptyVirtualDestructor), and
 * optionally frees memory if bit 0 of flags is set.
 */
void CCalMoviePlayer::ScalarDeletingDtorBase(int flags) {
    // Stamp base CCalMoviePlayer vtable
    *(void**)this = (void*)0x820092C0;

    // Clean up threads and resources
    CleanupResources();

    // Destroy base object
    pong_EmptyVirtualDestructor(this);

    // Free if ownership flag set
    if (flags & 1) {
        rage_free(this);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Batch 4: State Management & Playback Control (244–484B)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * CCalMoviePlayer::InitializeDefaults @ 0x8248E0E8 | size: 0xF4
 *
 * Resets all media player state fields to default values (vtable slot 38).
 * Zeroes all operational fields (ring buffer, KEVENTs, status) and sets
 * status fields 212, 216 to 1 (initial state) and field 240 to 1 (active).
 *
 * Called when preparing the player for a new media session.
 */
void CCalMoviePlayer::InitializeDefaults() {

    // Zero all operational fields from m_pMediaSource (+44) through m_hThreadWriteB (+272)
    // Covers: source ptr, buffer object, callbacks, events, status fields, counters, thread handles
    memset(&m_pMediaSource, 0, (272 + 4) - 44);

    // Set initial state values
    m_statusA = 1;      // Status A = initial
    m_statusB = 1;      // Status B = initial
    m_activeFlag = 1;   // Active flag = 1
}

/**
 * CCalMoviePlayer::QueryPlaybackReady @ 0x8248E520 | size: 0x134
 *
 * Checks if the player is ready for playback by querying status slots
 * 60 and 61 (vtable). Both must return 5 ("ready" state). If ready,
 * queries the sub-object at +44 via vtable slot 25 for the result code,
 * then sets status flags (vslots 71/72 with value 1) and signals events
 * 48-51 to wake waiting threads.
 *
 * Acquires lock (vslot 3) before checks, releases (vslot 5) after.
 *
 * @return 0 or positive on success, 0x80004005 (E_FAIL) if not ready.
 */
int32_t CCalMoviePlayer::QueryPlaybackReady() {

    int32_t result;

    // Lock
    Lock();

    // Check status A via vtable slot 60
    if (GetStatusFlagsA() == 5) {
        // Check status B via vtable slot 61
        if (GetStatusFlagsB() == 5) {
            // Both ready — query sub-object
            void* subObj = m_pMediaSource;
            void** subVt = *(void***)subObj;
            result = ((VFuncI)subVt[25])(subObj);

            // Signal status and events
            SetStatusValueA(1);
            SetStatusValueB(1);
            SignalEvent0();
            SignalEvent1();
            SignalEvent2();
            SignalEvent3();
        } else {
            result = (int32_t)0x80004005;  // E_FAIL
        }
    } else {
        result = (int32_t)0x80004005;  // E_FAIL
    }

    // Unlock
    Unlock();

    return result;
}

/**
 * CCalMoviePlayer::ResetPlaybackState @ 0x8248E658 | size: 0x100
 *
 * Resets playback state by setting status flags and signaling events.
 * Sets status slots 71/72 to 1, slots 68/69 to 2 (reset markers),
 * then signals events 48-51 to wake all waiting threads.
 *
 * Acquires lock (vslot 3) before modification, releases (vslot 5) after.
 * Returns 0.
 */
int32_t CCalMoviePlayer::ResetPlaybackState() {

    // Lock
    Lock();

    // Set status flags
    SetStatusValueA(1);
    SetStatusValueB(1);
    SetStatusMaskA(2);
    SetStatusMaskB(2);

    // Signal events
    SignalEvent0();
    SignalEvent1();
    SignalEvent2();
    SignalEvent3();

    // Unlock
    Unlock();

    return 0;
}

/**
 * CCalMoviePlayer::PreparePlayback @ 0x8248E968 | size: 0x1D4
 *
 * Prepares the player for playback (vtable slot 39). If both status slots
 * (vslots 60/61) return 1 ("initialized"), skips directly to state setup.
 * Otherwise, resets playback (vslot 24), waits for events 4-7 (vslots 44-47),
 * and queries the sub-object at +44 via vslot 20 to start playback.
 *
 * On successful start: zeros playback counters (232-256), sets active flag
 * (240 = 1), resets events 4-7 (vslots 56-59), sets channel flags
 * (vslots 65/66 with 1), and status markers (vslots 71/72 with -1).
 *
 * @return 0 or positive on success, negative on failure.
 */
int32_t CCalMoviePlayer::PreparePlayback() {

    int32_t playbackResult = 0;
    bool skipToInit = false;
    void** vt;

    // Check if already in initialized state (both status == 1)
    if (GetStatusFlagsA() == 1) {
        if (GetStatusFlagsB() == 1) {
            skipToInit = true;
        }
    }

    if (!skipToInit) {
        // Not already initialized — reset and start fresh
        WaitForAllThreads();    // ResetPlaybackState
        WaitForEvent4();    // WaitForEvent4
        WaitForEvent5();    // WaitForEvent5
        WaitForEvent6();    // WaitForEvent6
        WaitForEvent7();    // WaitForEvent7

        // Start playback via sub-object vslot 20
        void* subObj = m_pMediaSource;
        void** subVt = *(void***)subObj;
        playbackResult = ((VFuncI)subVt[20])(subObj);

        if (playbackResult < 0) {
            return playbackResult;  // Failed to start
        }
    }

    // Initialize playback state
    Lock();         // Lock

    SetStatusValueA(-1);   // Set status A = -1
    SetStatusValueB(-1);   // Set status B = -1
    SetChannelStatusA(1);    // Set channel A = 1
    SetChannelStatusB(1);    // Set channel B = 1

    // Zero playback counters
    m_frameCounterA = 0;
    m_frameCounterB = 0;
    m_activeFlag = 1;
    m_bufferCounterA = 0;
    m_bufferCounterB = 0;
    m_pendingCounter = 0;
    m_writePosition = 0;

    // Reset events 4-7
    ResetEvent4();        // ResetEvent4
    ResetEvent5();        // ResetEvent5
    ResetEvent6();        // ResetEvent6
    ResetEvent7();        // ResetEvent7

    Unlock();         // Unlock

    return playbackResult;
}

/**
 * CCalMoviePlayer::ProcessWriteBuffer @ 0x82490A18 | size: 0x12C
 *
 * Processes the next write buffer in the ring (vtable slot 32). Queries
 * the sub-object at +44 via vslot 17 for a media sample set, accesses
 * the element at the current write index (+256), activates it (element
 * vslot 14 with arg 1), reads data via element vslot 20, releases the
 * element, then atomically decrements the buffer counter at +244 and
 * advances the write index with wraparound.
 *
 * Finally dispatches the read data to the sub-object via vslot 22 and
 * signals events 48 and 50 (SignalEvent0/2).
 */
void CCalMoviePlayer::ProcessWriteBuffer() {

    // Query sub-object at +44 for media sample set
    void* subObj = m_pMediaSource;
    void** subVt = *(void***)subObj;

    void* queryResult = nullptr;
    ((VFunc4)subVt[17])(subObj, &queryResult, 0, 0);

    // Get total buffer count from query result
    uint32_t totalCount = *(uint32_t*)((char*)queryResult + 52);

    // Get current write index
    uint32_t writeIdx = m_writePosition;

    // Get element by index from query result's ring buffer
    char* base = *(char**)((char*)queryResult + 48);
    char* element = base + writeIdx * 60;
    {
        void** elVt = *(void***)element;
        if (elVt) ((VFuncV)elVt[1])(element);  // Activate
    }

    // Set element flag via vslot 14(element, 1)
    {
        void** elVt = *(void***)element;
        ((VFuncVI)elVt[14])(element, 1);
    }

    // Read data from element via vslot 20
    uint32_t dataValue;
    {
        void** elVt = *(void***)element;
        void* dataPtr = ((VFuncP)elVt[20])(element);
        dataValue = *(uint32_t*)((char*)dataPtr + 4);  // ld low-word (BE offset +4)
    }

    // Release element via vslot 2
    {
        void** elVt = *(void***)element;
        ((VFuncV)elVt[2])(element);
    }

    // Release query result if non-null
    if (queryResult != nullptr) {
        void** qVt = *(void***)queryResult;
        ((VFuncV)qVt[2])(queryResult);
        queryResult = nullptr;
    }

    // Atomically decrement buffer counter at +244
    volatile int32_t* pCounter = (volatile int32_t*)&m_bufferCounterA;
    int32_t oldVal;
    do {
        oldVal = *pCounter;
    } while (!__sync_bool_compare_and_swap(pCounter, oldVal, oldVal - 1));

    // Advance write index with wraparound
    uint32_t nextIdx = writeIdx + 1;
    if (nextIdx >= totalCount) {
        nextIdx = 0;
    }
    m_writePosition = nextIdx;

    // Dispatch read data to sub-object via vslot 22
    subObj = m_pMediaSource;
    subVt = *(void***)subObj;
    ((VFuncVP)subVt[22])(subObj, (void*)(uintptr_t)dataValue);

    // Signal events 0 and 2
    SignalEvent0();   // SignalEvent0
    SignalEvent2();   // SignalEvent2
}

// External helpers for buffer fill functions
extern "C" uint32_t CCalRingBuffer_GetFillCount(void* obj) __asm__("_CCalMoviePlayer_DBD0");  // @ 0x8248DBD0 - get remaining sample count
extern "C" void* CCalRingBuffer_PeekReadSlot(void* obj) __asm__("_CCalMoviePlayer_DC30");      // @ 0x8248DC30 - get/reset next element
extern "C" void CCalRingBuffer_AdvanceReadIndex(void* obj) __asm__("_CCalMoviePlayer_DD30");       // @ 0x8248DD30 - advance iterator index
extern "C" void sysIpcThread_Suspend(void* threadHandle) __asm__("_pg_B850");            // @ 0x8242B850 - suspend thread

/**
 * CCalMoviePlayer::FillReadBufferA @ 0x8248F268 | size: 0x278
 *
 * Media sample fill thread procedure for channel A (channel 0). Symmetric
 * pair with CCalMoviePlayer_35 (FillReadBufferB) which handles channel 1.
 *
 * Outer loop: queries the sub-object at +44 for an enumerator (vslot 15,
 * channel 0 via arg2) and iterator (vslot 17, channel 0 via arg2).
 * Inner loop: checks remaining samples via CCalRingBuffer_GetFillCount against
 * the threshold at +244. When enough are available, extracts each element
 * via CCalRingBuffer_PeekReadSlot, activates it (element vslot 14), adds it to
 * the enumerator (vslot 18), inserts by sort key (element vslot 21 →
 * enumerator vslot 30), then advances the iterator (CCalRingBuffer_AdvanceReadIndex)
 * and signals event 6 (vslot 50). If not enough samples, waits for
 * event 0 (vslot 40).
 *
 * After draining, finalizes the enumerator (vslot 20), releases both
 * enumerator and iterator (vslot 2), signals event 4 (vslot 52), and
 * suspends the thread at +260 via sysIpcThread_Suspend. Loops until status flags B
 * (vslot 63) bit 2 is set.
 */
void CCalMoviePlayer::FillReadBufferA() {

    void** vt;

    // Check status flags B — if bit 2 set, signal and return immediately
    int32_t flagsB = GetStatusFieldB();
    if (flagsB & 0x4) {
        SignalEvent4();  // SignalEvent4
        return;
    }

    for (;;) {
        void* enumerator = nullptr;
        void* iterator = nullptr;

        // Get enumerator for channel 0 from sub-object at +44
        void* subObj = m_pMediaSource;
        void** subVt = *(void***)subObj;
        ((VFunc4)subVt[15])(subObj, nullptr, &enumerator, 0);

        if (enumerator != nullptr) {
            // Get iterator for channel 0
            subObj = m_pMediaSource;
            subVt = *(void***)subObj;
            ((VFunc4)subVt[17])(subObj, nullptr, &iterator, 0);

            // Check flags B — bits 1&2 must be clear to process
            int32_t flags = GetStatusFieldB();

            while ((flags & 0x6) == 0) {
                // Get remaining sample count from iterator
                uint32_t remaining = CCalRingBuffer_GetFillCount(iterator);
                uint32_t threshold = m_bufferCounterA;

                if (remaining > threshold) {
                    // Get/reset next element from iterator
                    void* element = CCalRingBuffer_PeekReadSlot(iterator);

                    // Activate element (vslot 14 with arg 1)
                    void** elVt = *(void***)element;
                    ((VFuncVI)elVt[14])(element, 1);

                    // Add element to enumerator collection (vslot 18)
                    void** enumVt = *(void***)enumerator;
                    ((VFuncVP)enumVt[18])(enumerator, element);

                    // Get sort key from element (vslot 21)
                    elVt = *(void***)element;
                    void* sortKey = ((VFuncP)elVt[21])(element);

                    // Insert into sorted position (vslot 30), output next cursor
                    void* nextCursor = nullptr;
                    enumVt = *(void***)enumerator;
                    int32_t insertResult = ((VFunc3P)enumVt[30])(enumerator, (char*)sortKey - 8, &nextCursor);

                    if (insertResult < 0) {
                        // Error/overflow — reset playback state (vslot 24)
                        WaitForAllThreads();
                    }

                    // Release element (vslot 2)
                    elVt = *(void***)element;
                    ((VFuncV)elVt[2])(element);

                    // Advance iterator to next sample
                    CCalRingBuffer_AdvanceReadIndex(iterator);

                    // Signal event 6 (vslot 50)
                    SignalEvent2();

                    // If no next cursor, break to finalize
                    if (nextCursor == nullptr) break;
                } else {
                    // Not enough samples — wait for event 0 (vslot 40)
                    NotifyVideoReady();
                }

                // Re-check flags B
                flags = GetStatusFieldB();
            }

            // Finalize enumerator (vslot 20)
            {
                void** enumVt = *(void***)enumerator;
                ((VFuncV)enumVt[20])(enumerator);
            }

            // Release enumerator
            if (enumerator != nullptr) {
                void** eVt = *(void***)enumerator;
                ((VFuncV)eVt[2])(enumerator);
                enumerator = nullptr;
            }

            // Release iterator
            if (iterator != nullptr) {
                void** iVt = *(void***)iterator;
                ((VFuncV)iVt[2])(iterator);
                iterator = nullptr;
            }
        }

        // Re-check flags B — bit 2
        flagsB = GetStatusFieldB();
        if (flagsB & 0x4) {
            SignalEvent4();  // SignalEvent4
            return;
        }

        // Signal event 4 and suspend this thread
        SignalEvent4();  // SignalEvent4

        sysIpcThread_Suspend(m_hThreadFillA);  // Suspend thread handle at +260

        // Re-check flags B after resume
        flagsB = GetStatusFieldB();
        if (flagsB & 0x4) break;
    }

    // Final signal event 4
    SignalEvent4();
}

/**
 * CCalMoviePlayer::FillReadBufferB @ 0x8248F4E0 | size: 0x278
 *
 * Media sample fill thread procedure for channel B (channel 1). Symmetric
 * pair with CCalMoviePlayer_34 (FillReadBufferA) which handles channel 0.
 *
 * Outer loop: queries the sub-object at +44 for an enumerator (vslot 15,
 * channel 1 via arg2) and iterator (vslot 17, channel 1 via arg2).
 * Inner loop: checks remaining samples via CCalRingBuffer_GetFillCount against
 * the threshold at +248. When enough are available, extracts each element
 * via CCalRingBuffer_PeekReadSlot, activates it (element vslot 14), adds it to
 * the enumerator (vslot 18), inserts by sort key (element vslot 21 →
 * enumerator vslot 30), then advances the iterator (CCalRingBuffer_AdvanceReadIndex)
 * and signals event 3 (vslot 51). If not enough samples, waits for
 * event 1 (vslot 41).
 *
 * After draining, finalizes the enumerator (vslot 20), releases both
 * enumerator and iterator (vslot 2), signals event 5 (vslot 53), and
 * suspends the thread at +264 via sysIpcThread_Suspend. Loops until status flags C
 * (vslot 64) bit 2 is set.
 */
void CCalMoviePlayer::FillReadBufferB() {

    void** vt;

    // Check status flags C — if bit 2 set, signal and return immediately
    int32_t flagsC = GetStatusFieldC();
    if (flagsC & 0x4) {
        SignalEvent5();  // SignalEvent5
        return;
    }

    for (;;) {
        void* enumerator = nullptr;
        void* iterator = nullptr;

        // Get enumerator for channel 1 from sub-object at +44
        void* subObj = m_pMediaSource;
        void** subVt = *(void***)subObj;
        ((VFunc4)subVt[15])(subObj, nullptr, &enumerator, 0);

        if (enumerator != nullptr) {
            // Get iterator for channel 1
            subObj = m_pMediaSource;
            subVt = *(void***)subObj;
            ((VFunc4)subVt[17])(subObj, nullptr, &iterator, 0);

            // Check flags C — bits 1&2 must be clear to process
            int32_t flags = GetStatusFieldC();

            while ((flags & 0x6) == 0) {
                // Get remaining sample count from iterator
                uint32_t remaining = CCalRingBuffer_GetFillCount(iterator);
                uint32_t threshold = m_bufferCounterB;

                if (remaining > threshold) {
                    // Get/reset next element from iterator
                    void* element = CCalRingBuffer_PeekReadSlot(iterator);

                    // Activate element (vslot 14 with arg 1)
                    void** elVt = *(void***)element;
                    ((VFuncVI)elVt[14])(element, 1);

                    // Add element to enumerator collection (vslot 18)
                    void** enumVt = *(void***)enumerator;
                    ((VFuncVP)enumVt[18])(enumerator, element);

                    // Get sort key from element (vslot 21)
                    elVt = *(void***)element;
                    void* sortKey = ((VFuncP)elVt[21])(element);

                    // Insert into sorted position (vslot 30), output next cursor
                    void* nextCursor = nullptr;
                    enumVt = *(void***)enumerator;
                    int32_t insertResult = ((VFunc3P)enumVt[30])(enumerator, (char*)sortKey - 8, &nextCursor);

                    if (insertResult < 0) {
                        // Error/overflow — call vslot 24
                        WaitForAllThreads();
                    }

                    // Release element (vslot 2)
                    elVt = *(void***)element;
                    ((VFuncV)elVt[2])(element);

                    // Advance iterator to next sample
                    CCalRingBuffer_AdvanceReadIndex(iterator);

                    // Signal event 3 (vslot 51)
                    SignalEvent3();

                    // If no next cursor, break to finalize
                    if (nextCursor == nullptr) break;
                } else {
                    // Not enough samples — wait for event 1 (vslot 41)
                    NotifyAudioReady();
                }

                // Re-check flags C
                flags = GetStatusFieldC();
            }

            // Finalize enumerator (vslot 20)
            {
                void** enumVt = *(void***)enumerator;
                ((VFuncV)enumVt[20])(enumerator);
            }

            // Release enumerator
            if (enumerator != nullptr) {
                void** eVt = *(void***)enumerator;
                ((VFuncV)eVt[2])(enumerator);
                enumerator = nullptr;
            }

            // Release iterator
            if (iterator != nullptr) {
                void** iVt = *(void***)iterator;
                ((VFuncV)iVt[2])(iterator);
                iterator = nullptr;
            }
        }

        // Re-check flags C — bit 2
        flagsC = GetStatusFieldC();
        if (flagsC & 0x4) {
            SignalEvent5();  // SignalEvent5
            return;
        }

        // Signal event 5 and suspend this thread
        SignalEvent5();  // SignalEvent5

        sysIpcThread_Suspend(m_hThreadFillB);  // Suspend thread handle at +264

        // Re-check flags C after resume
        flagsC = GetStatusFieldC();
        if (flagsC & 0x4) break;
    }

    // Final signal event 5
    SignalEvent5();
}

/**
 * CCalMoviePlayer::DrainPlaybackBuffer @ 0x8248FD58 | size: 0x1E4
 *
 * Drains the playback buffer by looping through available frames
 * (vtable slot 21). Each iteration: clears the fiber flag on the buffer
 * object at +48 (EB70), locks (vslot 3), sets channels to 2 (vslots 65/66),
 * releases page refs at +260/+264/+272 via sysIpcHandle_Release, unlocks (vslot 5),
 * waits for events 4-7 (vslots 44-47), replaces fiber context on buffer
 * object at +48 (EB30), then checks status flag bits to continue.
 *
 * Loop continues while:
 *   - vslot 62 bit 1 is SET (flag A active)
 *   - vslot 63 bit 1 is CLEAR (flag B clear)
 *   - vslot 64 bit 1 is CLEAR (flag C clear)
 *   - vslot 39 returns >= 0 (frames available)
 *
 * @param statusC  Value written to status field C (+220) before processing
 * @return         Last frame index from vslot 39 (negative = exhausted)
 */
int32_t CCalMoviePlayer::DrainPlaybackBuffer(uint32_t statusC) {

    void** vt;

    // Store statusC into field +220
    m_isPlaying = statusC;

    // Get initial frame result via vslot 39 (PreparePlayback)
    int32_t frameResult = GetFrameResult();

    while (frameResult >= 0) {
        // Clear fiber flag on buffer object (inline EB70)
        {
            char* bufObj = (char*)m_pBufferObject;
            *(uint32_t*)(bufObj + 10376) = 0;
        }

        // Lock
        Lock();

        // Set channels to 2 (pause/reset mode)
        SetChannelStatusA(2);
        SetChannelStatusB(2);

        // Release page references
        sysIpcHandle_Release(m_hThreadFillA);
        sysIpcHandle_Release(m_hThreadFillB);
        sysIpcHandle_Release(m_hThreadWriteB);

        // Unlock
        Unlock();

        // Wait for events 4-7
        WaitForEvent4();
        WaitForEvent5();
        WaitForEvent6();
        WaitForEvent7();

        // Save and replace fiber context on buffer object (inline EB30)
        {
            char* bufObj = (char*)m_pBufferObject;
            uint32_t existingCtx = *(uint32_t*)(bufObj + 10376);
            if (existingCtx != 0) {
                _crt_tls_fiber_setup();
            }
            void* newCtx = _crt_tls_fiber_setup();
            *(void**)(bufObj + 10376) = newCtx;
        }

        // Lock
        Lock();

        // Check flag A via vslot 62 — bit 1 must be set to continue
        int32_t flagA = IsPlaying();
        if (!(flagA & 0x2)) {
            // Flag A not active — stop draining
            Unlock();  // Unlock
            break;
        }

        // Check flag B via vslot 63 — bit 1 must be clear to continue
        int32_t flagB = GetStatusFieldB();
        if (flagB & 0x2) {
            // Flag B set — stop draining
            Unlock();  // Unlock
            break;
        }

        // Check flag C via vslot 64 — bit 1 must be clear to continue
        int32_t flagC = GetStatusFieldC();
        if (flagC & 0x2) {
            // Flag C set — stop draining
            Unlock();  // Unlock
            break;
        }

        // Unlock and continue loop
        Unlock();

        // Check next frame
        frameResult = GetFrameResult();
    }

    return frameResult;
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Batch 5: Thread management & remaining functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * CCalMoviePlayer::CleanupResources @ 0x8248FA48 | size: 0x284
 *
 * Tears down worker threads and releases all resources. Called by
 * DtorIntermediate and ScalarDeletingDtorBase during destruction.
 *
 * 1. Signals termination to channels A/B via vslots 68/69 with value 4
 * 2. If not in default state (status A/B != 1), waits for all threads
 *    via vslot 24, then releases page refs and waits for events 4-7
 * 3. Terminates each worker thread at +260/+264/+268/+272 by:
 *    - signaling its event (vslots 48-51)
 *    - waiting with timeout (pg_C3B8_g, retrying on WAIT_TIMEOUT=258)
 *    - closing the handle (pg_6F10_g)
 * 4. Releases the callback object at +48 (rmcDrawable_Release)
 * 5. Releases the media source at +44 (vslot 2)
 * 6. Reinitializes defaults via vslot 38
 *
 * @return Always 0
 */
int32_t CCalMoviePlayer::CleanupResources() {

    void** vt;

    // Signal termination to both channels (set flag 4)
    SetStatusMaskA(4);    // SetStatusFlagsC_A(4) — terminate channel A
    SetStatusMaskB(4);    // SetStatusFlagsC_B(4) — terminate channel B

    // Check if system is in default state
    int32_t statusA = GetStatusFlagsA();   // GetStatusA
    if (statusA == 1) {
        int32_t statusB = GetStatusFlagsB();   // GetStatusB
        if (statusB == 1) {
            goto thread_cleanup;   // Already in default state, skip to thread termination
        }
    }

    // Not in default state — wait for all threads to complete
    WaitForAllThreads();        // WaitForAllThreads

    // Release page refs and wait for events for each active handle
    if (m_hThreadFillA != nullptr) {
        sysIpcHandle_Release(m_hThreadFillA);
        WaitForEvent4();    // WaitEvent4
    }
    if (m_hThreadFillB != nullptr) {
        sysIpcHandle_Release(m_hThreadFillB);
        WaitForEvent5();    // WaitEvent5
    }
    if (m_hThreadWriteA != nullptr) {
        sysIpcHandle_Release(m_hThreadWriteA);
        WaitForEvent6();    // WaitEvent6
    }
    if (m_hThreadWriteB != nullptr) {
        sysIpcHandle_Release(m_hThreadWriteB);
        WaitForEvent7();    // WaitEvent7
    }

thread_cleanup:
    // Terminate worker thread at +260 (FillReadBufferA)
    if (m_hThreadFillA != nullptr) {
        do {
            sysIpcHandle_Release(m_hThreadFillA);
            SignalEvent0();    // SignalEvent0
        } while (sysThread_WaitTimeout(m_hThreadFillA, 1) == 258);  // WAIT_TIMEOUT
        sysIpcHandle_Close(m_hThreadFillA);
        m_hThreadFillA = nullptr;
    }

    // Terminate worker thread at +264 (FillReadBufferB)
    if (m_hThreadFillB != nullptr) {
        do {
            sysIpcHandle_Release(m_hThreadFillB);
            SignalEvent1();    // SignalEvent1
        } while (sysThread_WaitTimeout(m_hThreadFillB, 1) == 258);
        sysIpcHandle_Close(m_hThreadFillB);
        m_hThreadFillB = nullptr;
    }

    // Terminate worker thread at +268 (WriteThreadProcA)
    if (m_hThreadWriteA != nullptr) {
        do {
            sysIpcHandle_Release(m_hThreadWriteA);
            SignalEvent2();    // SignalEvent2
        } while (sysThread_WaitTimeout(m_hThreadWriteA, 1) == 258);
        sysIpcHandle_Close(m_hThreadWriteA);
        m_hThreadWriteA = nullptr;
    }

    // Terminate worker thread at +272 (WriteThreadProcB)
    if (m_hThreadWriteB != nullptr) {
        do {
            sysIpcHandle_Release(m_hThreadWriteB);
            SignalEvent3();    // SignalEvent3
        } while (sysThread_WaitTimeout(m_hThreadWriteB, 1) == 258);
        sysIpcHandle_Close(m_hThreadWriteB);
        m_hThreadWriteB = nullptr;
    }

    // Release callback object at +48
    if (m_pBufferObject != nullptr) {
        rmcDrawable_Release(m_pBufferObject);
        m_pBufferObject = nullptr;
    }

    // Release media source at +44
    if (m_pMediaSource != nullptr) {
        void** srcVt = *(void***)m_pMediaSource;
        ((VFuncV)srcVt[2])(m_pMediaSource);    // Release
        m_pMediaSource = nullptr;
    }

    // Reinitialize default state
    ResetDefaults();        // InitializeDefaults

    return 0;
}

/**
 * CCalMoviePlayer::InitializeWorkerThreads @ 0x8248F758 | size: 0x2F0
 *
 * Creates worker threads and initializes event objects. Called during
 * playback setup to spin up the fill/write processing pipeline.
 *
 * 1. Validates via vslot 13 (returns error if < 0)
 * 2. Initializes 8 KEVENT structures:
 *    - 4 sync events at +84/+100/+116/+132 (ManualReset=1, Signaled=0)
 *    - 4 notification events at +148/+164/+180/+196 (NotSet=0, Value=1)
 * 3. Creates 4 worker threads via pg_6F88_w at +260/+264/+268/+272
 * 4. Sets processor affinity for each thread via xe_sleep_B8A8
 * 5. Configures buffer pairs via vslots 16/17
 * 6. On any failure, calls CleanupResources (vslot 13) and returns error
 *
 * @param params  Pointer to initialization parameters structure
 * @return        0 on success, negative error code on failure
 */
int32_t CCalMoviePlayer::InitializeWorkerThreads(void* params) {

    void** vt;
    int32_t result;

    // Step 1: Validate via vslot 13
    result = CleanupInternal();
    if (result < 0) {
        return result;
    }

    // Step 2: Initialize 8 KEVENT structures in m_events[128]
    // Each KEVENT is 16 bytes: [type(1) pad(3) signaled(4) flink(4) blink(4)]
    // Sync events 0-3 (ManualReset=1, Signaled=0)
    for (int i = 0; i < 4; i++) {
        uint8_t* ev = &m_events[i * 16];
        ev[0] = 1;                                            // ManualReset = true
        *(uint32_t*)(ev + 4) = 0;                             // Not signaled
        uint32_t selfAddr = (uint32_t)(uintptr_t)(ev + 8);
        *(uint32_t*)(ev + 8) = selfAddr;                       // flink = self
        *(uint32_t*)(ev + 12) = selfAddr;                      // blink = self
    }

    // Notification events 4-7 (NotSet=0, Value=1)
    for (int i = 4; i < 8; i++) {
        uint8_t* ev = &m_events[i * 16];
        ev[0] = 0;                                            // NotSet
        *(uint32_t*)(ev + 4) = 1;                             // Value = 1
        uint32_t selfAddr = (uint32_t)(uintptr_t)(ev + 8);
        *(uint32_t*)(ev + 8) = selfAddr;                       // flink = self
        *(uint32_t*)(ev + 12) = selfAddr;                      // blink = self
    }

    // Step 3: Create 4 worker threads
    // Thread 1: FillReadBufferA → stored at +260
    {
        void* thread1 = sysThread_Create(nullptr, 0x10000,
            (void*)CCalMoviePlayer_EC88_p33, this, 4, nullptr);
        m_hThreadFillA = thread1;
        if (thread1 == nullptr) {
            result = GetError();    // GetError
        }
    }
    if (result < 0) goto cleanup;

    // Thread 2: FillReadBufferB → stored at +264
    {
        void* thread2 = sysThread_Create(nullptr, 0x10000,
            (void*)CCalMoviePlayer_ECB8_p33, this, 4, nullptr);
        m_hThreadFillB = thread2;
        if (thread2 == nullptr) {
            result = GetError();
        }
    }
    if (result < 0) goto cleanup;

    // Thread 3: WriteThreadProcA → stored at +268
    {
        void* thread3 = sysThread_Create(nullptr, 0x10000,
            (void*)CCalMoviePlayer_ECE8_p33, this, 4, nullptr);
        m_hThreadWriteA = thread3;
        if (thread3 == nullptr) {
            result = GetError();
        }
    }
    if (result < 0) goto cleanup;

    // Thread 4: WriteThreadProcB → stored at +272
    {
        void* thread4 = sysThread_Create(nullptr, 0x10000,
            (void*)CCalMoviePlayer_ED18_p33, this, 4, nullptr);
        m_hThreadWriteB = thread4;
        if (thread4 == nullptr) {
            result = GetError();
        }
    }
    if (result < 0) goto cleanup;

    // Step 4: Set thread processor affinity
    {
        int32_t aff1, aff2, aff3, aff4;
        uint32_t* paramsU = (uint32_t*)params;
        if (*(int32_t*)(paramsU + 2) != 0) {      // params[2] (offset +8)
            aff1 = *(int32_t*)(paramsU + 3);       // params[3] (offset +12)
            aff2 = *(int32_t*)(paramsU + 4);       // params[4] (offset +16)
            aff3 = *(int32_t*)(paramsU + 5);       // params[5] (offset +20)
            aff4 = *(int32_t*)(paramsU + 6);       // params[6] (offset +24)
        } else {
            aff1 = 3;    // default affinity for FillReadBufferB
            aff2 = 1;    // default for thread at +264
            aff3 = 3;    // default for WriteThreadProcA
            aff4 = 2;    // default for WriteThreadProcB
        }

        xe_SetThreadProcessor(m_hThreadFillB, aff2);   // FillReadBufferB
        xe_SetThreadProcessor(m_hThreadWriteB, aff4);   // WriteThreadProcB
        xe_SetThreadProcessor(m_hThreadFillA, aff1);   // FillReadBufferA
        xe_SetThreadProcessor(m_hThreadWriteA, aff3);   // WriteThreadProcA
    }

    // Step 5: Configure buffer pairs
    {
        uint32_t* paramsU = (uint32_t*)params;
        result = SetBufferPairA((int32_t)paramsU[0]);   // SetBufferPairA
        if (result < 0) goto cleanup;

        result = SetBufferPairB((int32_t)paramsU[1]);   // SetBufferPairB
        if (result >= 0) {
            return result;
        }
    }

cleanup:
    CleanupInternal();    // CleanupResources
    return result;
}

/**
 * CCalMoviePlayer::WriteThreadProcA @ 0x82490B48 | size: 0x460
 *
 * Write thread procedure for channel A (channel 0). Processes media
 * samples from the sub-object at +44 and manages the write pipeline.
 *
 * Outer loop: queries media source (vslot 16/17 channel 0) for
 * enumerator and iterator. Registers callback with entry point.
 * Inner loop: checks status flags B (vslot 63), processes elements
 * via atomic counter operations at +244, manages sorting, and
 * handles state transitions.
 *
 * Suspends via sysIpcThread_Suspend at thread handle +268 when idle.
 * Exits when status flags B bit 2 is set (terminate signal).
 */
void CCalMoviePlayer::WriteThreadProcA() {

    void** vt;

    // Check initial status — if bit 2 of statusB set, exit immediately
    int32_t statusB = GetStatusFieldB();   // GetStatusFlagsB
    if (statusB & 0x4) {
        goto exit_final;
    }

    // Main outer loop
    for (;;) {
        void* enumerator = nullptr;
        void* iterator = nullptr;

        // Query media source for enumerator (channel 0)
        void* mediaSrc = m_pMediaSource;
        {
            void** mVt = *(void***)mediaSrc;
            typedef int32_t (*VFuncMediaEnum)(void*, void*, int32_t, int32_t);
            ((VFuncMediaEnum)mVt[16])(mediaSrc, &enumerator, 0, 0);
        }

        if (enumerator == nullptr) {
            // No samples — release thread page and clear active flag
            sysIpcHandle_Release(m_hThreadWriteA);
            m_activeFlag = 0;
            goto suspend;
        }

        // Get iterator from media source (channel 0)
        {
            void* mediaSrc2 = m_pMediaSource;
            void** mVt = *(void***)mediaSrc2;
            typedef int32_t (*VFuncMediaIter)(void*, void*, int32_t, int32_t);
            ((VFuncMediaIter)mVt[17])(mediaSrc2, &iterator, 0, 0);
        }

        // Register callback with enumerator
        {
            void** eVt = *(void***)enumerator;
            typedef void (*VFuncCB)(void*, int32_t, void*, void*);
            ((VFuncCB)eVt[12])(enumerator, 1, (void*)CCalMoviePlayer_ECE8_p33, this);
        }

        // Inner processing loop
        for (;;) {
            // Check status flags B
            statusB = GetStatusFieldB();

            if (statusB == 0) {
                goto process_element;
            }

            // Check bits 1-2 (value & 0x6)
            if (statusB & 0x6) {
                // Terminate or error in status
                // Set channel A to 6
                SetChannelStatusA(6);

                // Sort enumerator with key 4
                {
                    void** eVt = *(void***)enumerator;
                    ((VFuncVI)eVt[21])(enumerator, 4);
                }
                goto cleanup_iteration;
            }

            // Check bit 0 (status active)
            if (!(statusB & 0x1)) {
                goto process_element;
            }

            // Status bit 0 set — check channel A state
            {
                int32_t chA = GetStatusFlagsA();   // GetStatusFlagsA
                if (chA != 5) {
                    // Set channel A to 4
                    SetChannelStatusA(4);

                    // Sort enumerator with key 3
                    {
                        void** eVt = *(void***)enumerator;
                        ((VFuncVI)eVt[21])(enumerator, 3);
                    }

                    // Set channel A to 5
                    SetChannelStatusA(5);
                }
            }

            // Wait for event 0
            WaitForEvent0();
            continue;   // back to inner loop

        process_element:
            // Set channel A to 3
            SetChannelStatusA(3);

            // Check if iterator has elements
            {
                uint32_t hasElem = atSingleton_vfn_28_DBC8_1(iterator);
                if (hasElem == 0) {
                    // No more elements — loop waiting
                    WaitForEvent0();    // WaitEvent0
                    continue;
                }
            }

            // Get next element
            {
                void* element = CCalRingBuffer_PeekWriteSlot(iterator);

            // Get data from element via vslot 22
            void* dataPtr;
            {
                void** elVt = *(void***)element;
                dataPtr = ((VFuncP)elVt[22])(element);
            }

            if (dataPtr != nullptr) {
                void* adjustedPtr = (void*)((char*)dataPtr - 8);

                // Activate element (vslot 14, arg=1)
                {
                    void** elVt = *(void***)element;
                    ((VFuncVI)elVt[14])(element, 1);
                }

                // Atomic increment counter at +244
                volatile int32_t* pCounter = (volatile int32_t*)&m_bufferCounterA;
                int32_t oldVal;
                do {
                    oldVal = *pCounter;
                } while (!__sync_bool_compare_and_swap(pCounter, oldVal, oldVal + 1));

                // Add element to enumerator (vslot 14)
                {
                    void** eVt = *(void***)enumerator;
                    ((VFuncVI)eVt[14])(enumerator, (int32_t)(intptr_t)element);
                }

                // Set sort key on element (vslot 18, key=8)
                {
                    void** elVt = *(void***)element;
                    ((VFuncVI)elVt[18])(element, 8);
                }

                // Insert sorted (vslot 20 with adjusted dataPtr)
                int32_t insertResult;
                {
                    void** eVt = *(void***)enumerator;
                    insertResult = ((VFuncII)eVt[20])(enumerator, (int32_t)(intptr_t)adjustedPtr);
                }

                if (insertResult >= 0) {
                    // Success — advance read index
                    CCalRingBuffer_AdvanceWriteIndex(iterator);
                } else if (insertResult == (int32_t)0x8000000A) {
                    // Timeout/retry — undo
                    {
                        void** elVt = *(void***)element;
                        ((VFuncVI)elVt[18])(element, (int32_t)(intptr_t)adjustedPtr);
                    }

                    // Atomic decrement counter at +244
                    do {
                        oldVal = *pCounter;
                    } while (!__sync_bool_compare_and_swap(pCounter, oldVal, oldVal - 1));

                    // Wait for event 0
                    WaitForEvent0();
                }

                // Release element (vslot 2)
                {
                    void** elVt = *(void***)element;
                    ((VFuncV)elVt[2])(element);
                }
                continue;   // back to inner loop
            } else {
                // Data was null — release element
                {
                    void** elVt = *(void***)element;
                    ((VFuncV)elVt[2])(element);
                }
            }
            } // end element scope

        cleanup_iteration:
            // Check status A & clean up
            int32_t chA2 = GetStatusFlagsA();
            if (chA2 != 6) {
                SetChannelStatusA(6);

                void** eVt = *(void***)enumerator;
                ((VFuncVI)eVt[21])(enumerator, 2);
            }

            // Finalize enumerator (vslot 15)
            {
                void** eVt = *(void***)enumerator;
                ((VFuncV)eVt[15])(enumerator);
            }

            // Release enumerator
            if (enumerator != nullptr) {
                void** eVt = *(void***)enumerator;
                ((VFuncV)eVt[2])(enumerator);
                enumerator = nullptr;
            }

            // Release iterator
            if (iterator != nullptr) {
                void** iVt = *(void***)iterator;
                ((VFuncV)iVt[2])(iterator);
                iterator = nullptr;
            }

            break;   // exit inner loop
        }

    suspend:
        // Wait for event 4
        WaitForEvent4();

        // Check status B for termination
        statusB = GetStatusFieldB();
        if (statusB & 0x4) {
            goto exit_final;
        }

        // Set channel A to 7
        SetChannelStatusA(7);

        // Signal completion (vslot 54)
        SignalEvent6();

        // Suspend this thread
        sysIpcThread_Suspend(m_hThreadWriteA);

        // Check status B again after resume
        statusB = GetStatusFieldB();
        if (statusB & 0x4) {
            goto exit_final;
        }
        // Continue outer loop
    }

exit_final:
    // Set channel A to 8
    SetChannelStatusA(8);

    // Final signal
    SignalEvent6();
}

/**
 * CCalMoviePlayer::WriteThreadProcB @ 0x82490FA8 | size: 0x5F0
 *
 * Write thread procedure for channel B (channel 1). Symmetric with
 * WriteThreadProcA but operates on channel B state fields.
 *
 * Key differences from WriteThreadProcA:
 *   - Uses vslot 64 (GetStatusFlagsC) instead of 63
 *   - Uses vslot 66 (SetChannelB) instead of 65
 *   - Uses vslot 43 (WaitEvent1) instead of 42
 *   - Uses vslot 45 (WaitEvent5) instead of 44
 *   - Uses vslot 55 instead of 54 for completion signal
 *   - Uses +248 counter instead of +244
 *   - Uses +252 counter2 instead of +248 (for callback drain)
 *   - Thread handle at +272 instead of +268
 *   - Callback entry point is CCalMoviePlayer_ED18_p33 (vs ECE8)
 *   - Has additional callback drain processing with audio callback (+60/+76)
 */
void CCalMoviePlayer::WriteThreadProcB() {

    void** vt;

    // Check initial status — if bit 2 of statusC set, exit immediately
    int32_t statusC = GetStatusFieldC();   // GetStatusFlagsC
    if (statusC & 0x4) {
        goto exit_final;
    }

    // Main outer loop
    for (;;) {
        void* enumerator = nullptr;
        void* iterator = nullptr;

        // Query media source for enumerator (channel 0)
        void* mediaSrc = m_pMediaSource;
        {
            void** mVt = *(void***)mediaSrc;
            typedef int32_t (*VFuncMediaEnum)(void*, void*, int32_t, int32_t);
            ((VFuncMediaEnum)mVt[16])(mediaSrc, &enumerator, 0, 0);
        }

        if (enumerator == nullptr) {
            // No samples — release thread page and clear active flag
            sysIpcHandle_Release(m_hThreadWriteA);
            m_activeFlag = 0;
            goto suspend;
        }

        // Get iterator (channel 0)
        {
            void* mediaSrc2 = m_pMediaSource;
            void** mVt = *(void***)mediaSrc2;
            typedef int32_t (*VFuncMediaIter)(void*, void*, int32_t, int32_t);
            ((VFuncMediaIter)mVt[17])(mediaSrc2, &iterator, 0, 0);
        }

        // Register callback
        {
            void** eVt = *(void***)enumerator;
            typedef void (*VFuncCB)(void*, int32_t, void*, void*);
            ((VFuncCB)eVt[12])(enumerator, 1, (void*)CCalMoviePlayer_ED18_p33, this);
        }

        // Setup additional audio callbacks if present
        if (m_pAudioCallbackA != nullptr) {
            void** eVt = *(void***)enumerator;
            typedef void (*VFuncCB4)(void*, int32_t, void*, uint32_t);
            ((VFuncCB4)eVt[12])(enumerator, 3, m_pAudioCallbackA, m_audioParamA);
        }
        if (m_pAudioCallbackB != nullptr) {
            void** eVt = *(void***)enumerator;
            typedef void (*VFuncCB4)(void*, int32_t, void*, uint32_t);
            ((VFuncCB4)eVt[12])(enumerator, 4, m_pAudioCallbackB, m_audioParamB);
        }

        // Save/replace fiber context on buffer object
        {
            if (m_pBufferObject != nullptr) {
                CCalMoviePlayer_AcquireFiberContext(m_pBufferObject);
            }
        }

        // Inner processing loop
        for (;;) {
            // Drain callback queue if present
            void* callbackFunc = m_pCallbackFunc;
            if (callbackFunc != nullptr) {
                volatile uint32_t* pPending = (volatile uint32_t*)&m_pendingCounter;
                uint32_t pending = *pPending;
                uint32_t threshold = m_bufferCounterB;
                while (pending > threshold) {
                    // Call callback
                    typedef void (*CallbackFn)(void*);
                    ((CallbackFn)callbackFunc)(m_pCallbackUserData);

                    // Atomic decrement pending counter at +252
                    int32_t oldVal;
                    do {
                        oldVal = *(volatile int32_t*)pPending;
                    } while (!__sync_bool_compare_and_swap((volatile int32_t*)pPending, oldVal, oldVal - 1));

                    pending = *pPending;
                }
            }

            // Check status flags C
            statusC = GetStatusFieldC();

            if (statusC == 0) {
                goto process_element_b;
            }

            // Check bits 1-2 (value & 0x6)
            if (statusC & 0x6) {
                // Terminate — set channel B to 6
                SetChannelStatusB(6);

                // Sort enumerator with key 4
                void** eVt = *(void***)enumerator;
                ((VFuncVI)eVt[21])(enumerator, 4);
                goto cleanup_iteration_b;
            }

            // Check bit 0 (active)
            if (!(statusC & 0x1)) {
                goto process_element_b;
            }

            // Check channel B state
            {
                int32_t chB = GetStatusFlagsA();   // GetStatusFlagsA (shared state)
                if (chB != 5) {
                    // Set channel B to 4
                    SetChannelStatusB(4);

                    // Sort with key 3
                    void** eVt = *(void***)enumerator;
                    ((VFuncVI)eVt[21])(enumerator, 3);

                    // Set channel B to 5
                    SetChannelStatusB(5);
                }
            }

            // Wait for event 1
            WaitForEvent1();
            continue;

        process_element_b:
            // Set channel B to 3
            SetChannelStatusB(3);

            // Check if iterator has elements
            {
                uint32_t hasElem = atSingleton_vfn_28_DBC8_1(iterator);
                if (hasElem == 0) {
                    // No elements — wait
                    WaitForEvent1();
                    continue;
                }
            }

            // Get next element
            {
                void* element = CCalRingBuffer_PeekWriteSlot(iterator);

            // Get data
            void* dataPtr;
            {
                void** elVt = *(void***)element;
                dataPtr = ((VFuncP)elVt[22])(element);
            }

            if (dataPtr != nullptr) {
                void* adjustedPtr = (void*)((char*)dataPtr - 8);

                // Activate element
                {
                    void** elVt = *(void***)element;
                    ((VFuncVI)elVt[14])(element, 1);
                }

                // Read data value
                {
                    void** elVt = *(void***)element;
                    void* dataResult = ((VFuncP)elVt[20])(element);
                    (void)dataResult;   // data is used implicitly
                }

                // Atomic increment counter at +248
                volatile int32_t* pCounter = (volatile int32_t*)&m_bufferCounterB;
                int32_t oldVal;
                do {
                    oldVal = *pCounter;
                } while (!__sync_bool_compare_and_swap(pCounter, oldVal, oldVal + 1));

                // Add element to enumerator
                {
                    void** eVt = *(void***)enumerator;
                    ((VFuncVI)eVt[14])(enumerator, (int32_t)(intptr_t)element);
                }

                // Set sort key (vslot 18, key=8)
                {
                    void** elVt = *(void***)element;
                    ((VFuncVI)elVt[18])(element, 8);
                }

                // Insert sorted (vslot 20)
                int32_t insertResult;
                {
                    void** eVt = *(void***)enumerator;
                    insertResult = ((VFuncII)eVt[20])(enumerator, (int32_t)(intptr_t)adjustedPtr);
                }

                if (insertResult >= 0) {
                    // Check status B for post-processing
                    int32_t chBStatus = GetStatusFlagsB();
                    if (chBStatus == 5) {
                        // In pause mode — set sort key on enumerator and yield
                        {
                            void** eVt = *(void***)enumerator;
                            ((VFuncVI)eVt[18])(enumerator, (int32_t)(intptr_t)adjustedPtr);
                        }
                        pg_SleepYield(33);
                        grcBuffer_Release(m_pBufferObject);
                    } else {
                        // Advance read index
                        CCalRingBuffer_AdvanceWriteIndex(iterator);
                        // Signal event for render dispatch
                        RenderFrame(0);    // vslot 31
                        grcBuffer_Release(m_pBufferObject);
                    }
                } else if (insertResult == (int32_t)0x8000000A) {
                    // Timeout — undo
                    {
                        void** elVt = *(void***)element;
                        ((VFuncVI)elVt[18])(element, (int32_t)(intptr_t)adjustedPtr);
                    }
                    // Atomic decrement
                    do {
                        oldVal = *pCounter;
                    } while (!__sync_bool_compare_and_swap(pCounter, oldVal, oldVal - 1));

                    // Wait for event 1
                    WaitForEvent1();
                }

                // Release element
                {
                    void** elVt = *(void***)element;
                    ((VFuncV)elVt[2])(element);
                }
                continue;
            } else {
                // Data was null
                {
                    void** elVt = *(void***)element;
                    ((VFuncV)elVt[2])(element);
                }
            }
            } // end element scope

        cleanup_iteration_b:
            // Check status and clean up
            int32_t chB2 = GetStatusFlagsA();
            if (chB2 != 6) {
                SetChannelStatusB(6);

                void** eVt = *(void***)enumerator;
                ((VFuncVI)eVt[21])(enumerator, 2);
            }

            // Check and clear active flag
            if (m_activeFlag != 0) {
                sysIpcHandle_Release(m_hThreadWriteA);
                m_activeFlag = 0;
            }

            // Drain remaining callback queue
            if (m_pCallbackFunc != nullptr) {
                volatile uint32_t* pPending = (volatile uint32_t*)&m_pendingCounter;
                while (*pPending != 0) {
                    typedef void (*CallbackFn)(void*);
                    ((CallbackFn)m_pCallbackFunc)(m_pCallbackUserData);

                    int32_t oldVal;
                    do {
                        oldVal = *(volatile int32_t*)pPending;
                    } while (!__sync_bool_compare_and_swap((volatile int32_t*)pPending, oldVal, oldVal - 1));
                }
            }

            // Clear fiber flag on buffer object
            {
                CCalMoviePlayer_ReleaseFiberContext(m_pBufferObject);
            }

            // Finalize and release enumerator
            if (enumerator != nullptr) {
                void** eVt = *(void***)enumerator;
                ((VFuncV)eVt[15])(enumerator);

                eVt = *(void***)enumerator;
                ((VFuncV)eVt[2])(enumerator);
                enumerator = nullptr;
            }

            // Release iterator
            if (iterator != nullptr) {
                void** iVt = *(void***)iterator;
                ((VFuncV)iVt[2])(iterator);
                iterator = nullptr;
            }

            break;   // exit inner loop
        }

    suspend:
        // Wait for event 5
        WaitForEvent5();

        // Check status C for termination
        statusC = GetStatusFieldC();
        if (statusC & 0x4) {
            goto exit_final;
        }

        // Set channel B to 7
        SetChannelStatusB(7);

        // Signal completion (vslot 55)
        SignalEvent7();

        // Suspend this thread
        sysIpcThread_Suspend(m_hThreadWriteB);

        // Check status C again after resume
        statusC = GetStatusFieldC();
        if (statusC & 0x4) {
            goto exit_final;
        }
        // Continue outer loop
    }

exit_final:
    // Set channel B to 8
    SetChannelStatusB(8);

    // Final signal
    SignalEvent7();
}

// ─────────────────────────────────────────────────────────────────────────────
// grc helper functions — Graphics subsystem functions used by CCalMoviePlayer
// ─────────────────────────────────────────────────────────────────────────────

// External graphics helpers
extern "C" void* rage_A980(void* pDevice);  // @ 0x8235A980 - get/refresh device pointer
extern "C" void* grc_A088(void* pDevice, int paramA, int paramB); // @ 0x8235A088 - alloc shader constant block
extern "C" void game_37B0(void* pDevice, int32_t x, int32_t y, int32_t w, int32_t h); // @ 0x823537B0 - set viewport
extern "C" void grc_CFD8(void* pDevice, float f1, uint32_t r5, uint32_t r6,
                          void* r7, uint32_t r8, uint32_t r9);  // @ 0x8235CFD8 - dispatch shader pass

// .rdata float constants referenced by shader setup
static const float kShaderConst_Zero = 0.0f;        // @ 0x8202D10C (lbl_8202D10C)
static const float kShaderConst_One = 1.0f;          // @ 0x8202C518 (lbl_8202C518)

/**
 * SetupShaderConstants (CCalMoviePlayer_SetupShaderConstants) @ 0x8235CD88 | size: 0x19C
 *
 * Allocates and configures a shader constant block for video playback
 * rendering. Sets up transformation matrix constants and GPU register
 * values for the pixel shader pipeline.
 *
 * Operates on a grcDevice object (NOT CCalMoviePlayer's this pointer).
 * Called during MainPlaybackLoop frame rendering.
 *
 * @param pDevice  Pointer to the grcDevice object (large, +11000 byte structure)
 */
extern "C" void CCalMoviePlayer_SetupShaderConstants(void* pDevice) __asm__("_CCalMoviePlayer_CD88");
extern "C" void CCalMoviePlayer_SetupShaderConstants(void* pDevice) {
    char* dev = (char*)pDevice;

    // Check if current write pointer exceeds limit
    uint32_t writePtr = *(uint32_t*)(dev + 0);
    uint32_t limit = *(uint32_t*)(dev + 8);
    if (writePtr > limit) {
        // Refresh the device pointer
        pDevice = rage_A980(pDevice);
        dev = (char*)pDevice;
    }

    // Allocate shader constant block (21 float4s, type 4)
    void* constBlock = grc_A088(pDevice, 21, 4);
    if (constBlock == nullptr) {
        // Store fallback write ptr and return
        *(uint32_t*)(dev + 0) = *(uint32_t*)(dev + 0);
        return;
    }

    // Determine base value based on swap flag
    uint32_t swapFlag = *(uint32_t*)(dev + 11776) & 0x1;
    float baseVal = kShaderConst_Zero;
    float negBase;
    if (swapFlag == 0) {
        negBase = -baseVal;
    } else {
        negBase = -baseVal;  // same in both cases for 0.0
    }

    float oneMinusBase = kShaderConst_One - baseVal;
    float* cb = (float*)constBlock;

    // Fill constant buffer: 21 float4s (84 floats)
    // Constants 0-1: negBase values
    cb[0] = negBase;     // c0.x
    cb[1] = negBase;     // c0.y
    // Constants 2-6: base values
    cb[2] = baseVal;     // c0.z
    cb[3] = baseVal;     // c0.w
    cb[4] = baseVal;     // c1.x
    cb[5] = baseVal;     // c1.y
    cb[6] = baseVal;     // c1.z
    // Constant 7: oneMinusBase
    cb[7] = oneMinusBase;   // c1.w
    // Constant 8: negBase
    cb[8] = negBase;        // c2.x
    // Constants 9-13: base values
    cb[9] = baseVal;        // c2.y
    cb[10] = baseVal;       // c2.z
    cb[11] = baseVal;       // c2.w
    cb[12] = baseVal;       // c3.x
    cb[13] = baseVal;       // c3.y
    // Constants 14-15: oneMinusBase
    cb[14] = oneMinusBase;  // c3.z
    cb[15] = oneMinusBase;  // c3.w
    // Constants 16-20: base values
    cb[16] = baseVal;       // c4.x
    cb[17] = baseVal;       // c4.y
    cb[18] = baseVal;       // c4.z
    cb[19] = baseVal;       // c4.w
    cb[20] = baseVal;       // c5.x

    // Calculate address/mode bits from the constant block pointer
    uint32_t blockAddr = (uint32_t)(uintptr_t)constBlock;
    uint32_t addrBits = ((blockAddr >> 20) & 0xFFF) + 512;
    addrBits = addrBits & 0x1000;
    uint32_t baseBits = blockAddr & 0x1FFFFFFF;
    uint32_t combinedAddr = addrBits + baseBits;
    combinedAddr |= 0x3;  // set low bits

    // Build GPU register commands
    uint32_t* cmdPtr = (uint32_t*)(dev + 0);
    uint32_t writePos = *cmdPtr;

    // Write 13 register values
    uint32_t regVals[13];
    regVals[0] = (5 << 16) | 18432;     // header
    regVals[1] = combinedAddr;           // address
    regVals[2] = 0x10000056;             // mode register
    regVals[3] = 0;
    regVals[4] = 0;
    regVals[5] = 0;
    regVals[6] = 0;
    regVals[7] = 0x25000;               // config A
    regVals[8] = 0;
    regVals[9] = 0;
    regVals[10] = 0;
    regVals[11] = 0xC0003600;           // config B
    regVals[12] = 0x30088;              // config C

    uint32_t* dst = (uint32_t*)(uintptr_t)(writePos + 4);
    for (int i = 0; i < 13; i++) {
        *dst = regVals[i];
        dst++;
    }

    *(uint32_t*)(dev + 0) = (uint32_t)(uintptr_t)dst;

    // Extract viewport dimensions from device state
    uint32_t dimA = *(uint32_t*)(dev + 11524);
    uint32_t dimB = *(uint32_t*)(dev + 11528);
    int32_t x = (int32_t)((dimA * 2) & 0xFFFFFFFE) >> 17;
    int32_t y = (int32_t)(((dimA >> 15) & 0xFFFE0000) >> 17);
    int32_t w = (int32_t)((dimB * 2) & 0xFFFFFFFE) >> 17;
    int32_t h = (int32_t)(((dimB >> 15) & 0xFFFE0000) >> 17);

    game_37B0(pDevice, y, x, h, w);
}

/**
 * DispatchColorConstants (CCalMoviePlayer_CF28_wrh) @ 0x8235CF28 | size: 0xB0
 *
 * Dispatches color constant data through the shader pipeline. Converts
 * a color value from integer RGBA to normalized float4, then calls
 * grc_CFD8 for each vertex/pixel shader pass.
 *
 * @param pDevice   grcDevice pointer
 * @param f1        Blend factor (float)
 * @param r4        Loop count (0 = single pass)
 * @param r5        Vertex buffer pointer (0 = single pass)
 * @param r6        Color constant (RGBA packed uint32)
 * @param r7        Offset per iteration (used when r4 > 0)
 * @param r8        unused
 * @param r9        GPU flags
 */
extern "C" void CCalMoviePlayer_CF28_wrh(void* pDevice, float f1,
    uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r7, uint32_t r8, uint32_t r9) {
    // Convert packed color to normalized float4 via VMX128 unpack
    // The original uses vupkd3d128 to convert bytes to floats
    uint8_t colorBytes[4];
    colorBytes[0] = (uint8_t)(r7 & 0xFF);
    colorBytes[1] = (uint8_t)((r7 >> 8) & 0xFF);
    colorBytes[2] = (uint8_t)((r7 >> 16) & 0xFF);
    colorBytes[3] = (uint8_t)((r7 >> 24) & 0xFF);

    // Build color constant: convert from byte to 1.0-based float
    // vupkd3d128 with mode 0: byte | 0x3F800000 (produces 1.0 + byte/mantissa)
    // vnmsubfp then subtracts to get normalized color
    float colorConst[4];
    for (int i = 0; i < 4; i++) {
        uint32_t packed = colorBytes[i] | 0x3F800000;
        float fPacked;
        memcpy(&fPacked, &packed, sizeof(float));
        // vnmsubfp: result = -(a*b - c) = -(fPacked*orig - orig) = orig*(1-fPacked)
        // This is an approximation; the exact behavior depends on VMX128 semantics
        colorConst[i] = fPacked;
    }

    if (r5 == 0) {
        // Single-pass mode
        grc_CFD8(pDevice, f1, 0, r6, colorConst, r8, r9);
    } else {
        if (r4 == 0) {
            return;
        }
        // Multi-pass: iterate r4 times, advancing by r5 each iteration
        uint32_t offset = r5;
        for (uint32_t i = r4; i > 0; i--) {
            grc_CFD8(pDevice, f1, offset, r6, colorConst, r8, r9);
            offset += 16;
        }
    }
}

/**
 * UpdatePixelState (CCalMoviePlayer_D700_p39) @ 0x8235D700 | size: 0xCC
 *
 * Updates the pixel shader state on the grcDevice based on a floating-point
 * intensity value. Converts the float to a fixed-point pixel value and
 * writes GPU state registers.
 *
 * @param pDevice  grcDevice pointer
 * @param f1       Intensity value (float)
 * @param r4-r5    unused
 * @param r6       Alpha/mode value
 */
extern "C" void CCalMoviePlayer_D700_p39(void* pDevice, float f1,
    uint32_t r4, uint32_t r5, uint32_t r6) {
    (void)r4; (void)r5;
    char* dev = (char*)pDevice;

    // Check pixel format flag at +11464
    uint16_t pixelFormat = *(uint16_t*)(dev + 11464);
    uint32_t pixelValue;
    uint32_t modeValue;

    if (!(pixelFormat & 0x1)) {
        // Standard format: multiply by scale constants
        // Two .rdata constants for scale/offset
        float scaleA = 255.0f;    // approximate @ lbl_82079F6C
        float scaleB = 127.5f;    // approximate @ lbl_82079F68

        float scaledA = f1 * scaleA;
        float scaledB = f1 * scaleB;

        int32_t intA = (int32_t)scaledA;
        int32_t intB = (int32_t)scaledB;

        // Pack: (intA << 8) | r6
        pixelValue = ((uint32_t)intA << 8) | r6;
        modeValue = ((uint32_t)intB >> 21) & 0x7FF;
    } else {
        // HDR/float format: extract from float bit representation
        uint32_t floatBits;
        memcpy(&floatBits, &f1, sizeof(float));

        // Check sign bit
        if (floatBits & 0x80000000) {
            pixelValue = 0;
        } else {
            // Extract exponent
            uint32_t exponent = (floatBits >> 23) & 0x1FF;
            if (exponent >= 113) {
                // Normal range: extract mantissa bits
                uint32_t mantissaBits = floatBits & 0x7800000;
                uint32_t adjusted = floatBits + 4;
                uint32_t combined = mantissaBits | adjusted;
                pixelValue = (combined >> 3) & 0x1FFFFFFF;
            } else {
                // Subnormal: shift based on exponent
                uint32_t expo8 = exponent & 0xFF;
                uint32_t mantissa = floatBits & 0x7FFFFF;
                uint32_t shift = 113 - expo8;
                uint32_t roundBit = 1U << (shift - 1);
                pixelValue = ((roundBit + mantissa) | 0x800000) >> shift;
            }
        }

        // Pack: (pixelValue << 8) | r6 with insert
        uint32_t packed = (pixelValue << 8) | (r6 & 0xFF);
        pixelValue = packed;
        modeValue = (packed >> 21) & 0x7FF;
    }

    // Write to GPU state registers on the device
    uint8_t alphaRef = *(uint8_t*)(dev + 11650);
    uint32_t blendState = *(uint32_t*)(dev + 11648);
    uint32_t alphaDiff = r6 - alphaRef;

    // Store pixel state
    *(uint32_t*)(dev + 11884) = pixelValue;

    // Compute blend mode bits
    uint32_t notBlend = ~blendState;
    uint32_t clzAlpha = alphaDiff == 0 ? 32 : __builtin_clz(alphaDiff);
    uint32_t blendBit = (notBlend << 6) & 0x800;
    uint32_t alphaBit = (clzAlpha << 6) & 0x800;
    uint32_t xorBit = alphaBit ^ 0x800;
    uint32_t modeBits = (xorBit ^ blendBit) | modeValue;

    *(uint32_t*)(dev + 11880) = modeBits;

    // Set 64-bit GPU command flag at device+48
    uint64_t* cmdPtr = (uint64_t*)(dev + 48);
    *cmdPtr |= (3ULL << 34);
}
