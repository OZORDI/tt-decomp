/**
 * mcMemcardControl — Xbox 360 memory card / save-data state machine
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * mcMemcardControl is a finite state machine (inherits fsmMachine) that
 * orchestrates all memory-card I/O operations: finding existing saves,
 * selecting a device, loading/saving game data, writing the save icon,
 * searching for content, removing stale saves, querying free space, and
 * formatting / unformatting the storage device.
 *
 * Each operation is modelled as an inner state class derived from
 * mcMemcardStatus.  The FSM enforces a narrow set of legal transitions:
 *   Idle  → any state          (start any operation)
 *   any   → Idle               (complete / cancel)
 *   Save  → SaveIcon or Remove (write icon, or clear space first)
 *   Remove → Save              (retry save after clearing space)
 *
 * A datBase-derived embedded sub-object (m_saveOp) tracks the pending
 * operation's result handle and carries a 4936-byte content data buffer
 * plus a 42-character filename field at the end.
 *
 * Vtable layout  (mcMemcardControl @ 0x8204D684, size 0x44 = 17 slots)
 * Inherits from  fsmMachine     (vtable @ 0x8204DD14)
 * Inherits from  rage::datBase  (vtable @ 0x820276C4, embedded @ +0x2C)
 */

#include "mc_memcard.hpp"
#include "rage/heap.hpp"       // rage_free
#include "rage/fsm_machine.hpp"

// ── Forward declarations ──────────────────────────────────────────────────────

class mcMemcardControl;


// ── mcMemcardStatus – base class for all inner state objects ─────────────────
//
// Vtable @ 0x8204D6C8, size 0x0C (3 slots).
// Each concrete state class expands this to 0x40 (16 slots).

struct mcMemcardStatus /* : virtual base */ {
    virtual ~mcMemcardStatus() = 0;  // slot 0 — scalar delete destructor
    // slot 1, slot 2: provided by concrete states
};


// ── mcMemcardControl struct ───────────────────────────────────────────────────
//
// Total size ≥ 0x13CF bytes (≈ 5071 bytes).
// The fsmMachine base occupies the first 0x0C bytes; mcMemcardControl
// appends its own fields starting at +0x0C.

struct mcSaveOp /* : rage::datBase */ {
    void*    m_pVtable;         // +0x00  rage::datBase vtable @ 0x820276C4
    int32_t  m_deviceResult;    // +0x04  device/handle result, init -1
    int32_t  m_opStatus;        // +0x08  operation status,   init  0
    int32_t  m_pendingOp;       // +0x0C  pending-op flag,    init  0; set 1 when idle
    // +0x10 .. +0x1337  — 4936-byte raw content data buffer (m_contentData)
    uint8_t  m_contentData[4936]; // +0x10
    // Xbox 360 content filename (max 42 chars + NUL)
    char     m_szContentFileName[43]; // +0x1348  (offset +4952 from mcSaveOp base)
};

struct fsmMachine {
    void*              m_pVtable;    // +0x00  (fsmMachine vtable or subclass override)
    int32_t            m_stateCount; // +0x04  number of registered states
    mcMemcardStatus**  m_pStates;    // +0x08  heap-allocated array of state pointers
};

struct mcMemcardControl : fsmMachine {
    // fsmMachine fields occupy +0x00..+0x0B
    int32_t  m_currentState;   // +0x0C  index of active state (0 = Idle/none)
    uint32_t m_unk10;          // +0x10  (reserved / padding)
    int32_t  m_result1;        // +0x14  first operation result,  init -1
    int32_t  m_result2;        // +0x18  second operation result, init -1
    uint8_t  m_pad1C[0x10];    // +0x1C  padding to sub-object alignment
    mcSaveOp m_saveOp;         // +0x2C  embedded save-data state / content buffer
};

// State count
static const int kMcStateCount = 11;

// State indices (match the RTTI inner-class vtable order)
enum McState {
    MC_STATE_IDLE         = 0,
    MC_STATE_FIND         = 1,
    MC_STATE_SELECT       = 2,
    MC_STATE_LOAD         = 3,
    MC_STATE_SAVE         = 4,
    MC_STATE_SAVE_ICON    = 5,
    MC_STATE_SEARCH       = 6,
    MC_STATE_REMOVE       = 7,
    MC_STATE_GET_FREE     = 8,
    MC_STATE_FORMAT       = 9,
    MC_STATE_UNFORMAT     = 10,
};


// ── mcMemcardControl inner state concrete classes ─────────────────────────────
//
// Each inner state has its own vtable (0x40 bytes = 16 slots).
// Concrete allocations sizes observed in the constructor:
//   Idle, Find, Select:            8 bytes  (vtable + 1 int)
//   Load, Save:                   12 bytes  (vtable + 2 ints)
//   SaveIcon, Search, Remove,
//   GetFreeSpace, Format, Unformat: 8 bytes (vtable + 1 int)
// All aligned to 16 bytes via the RAGE heap.

struct mcMemcardControl::Idle         : mcMemcardStatus { /* vtable @ 0x8204D6D4 */ };
struct mcMemcardControl::Find         : mcMemcardStatus { /* vtable @ 0x8204D714 */ };
struct mcMemcardControl::Select       : mcMemcardStatus { /* vtable @ 0x8204D754 */ };
struct mcMemcardControl::Load         : mcMemcardStatus { int32_t m_unk0; /* vtable @ 0x8204D794 */ };
struct mcMemcardControl::Save         : mcMemcardStatus { int32_t m_unk0; /* vtable @ 0x8204D7D4 */ };
struct mcMemcardControl::SaveIcon     : mcMemcardStatus { /* vtable @ 0x8204D814 */ };
struct mcMemcardControl::Search       : mcMemcardStatus { /* vtable @ 0x8204D854 */ };
struct mcMemcardControl::Remove       : mcMemcardStatus { /* vtable @ 0x8204D894 */ };
struct mcMemcardControl::GetFreeSpace : mcMemcardStatus { /* vtable @ 0x8204D8D4 */ };
struct mcMemcardControl::Format       : mcMemcardStatus { /* vtable @ 0x8204D914 */ };
struct mcMemcardControl::Unformat     : mcMemcardStatus { /* vtable @ 0x8204D954 */ };


// ── mcMemcardControl::~mcMemcardControl @ 0x822BFFF8 | size: 0x68 ─────────────
//
// Destructor (vfn_0 / vtable slot 0).
// Resets both vtable pointers (main + embedded mcSaveOp), then delegates to
// the fsmMachine destructor body to free the states array.  Optionally frees
// 'this' if r4 == 1 (heap-allocated instance).
//
// NOTE: The fsmMachine destructor body (gameLoop_DestroyAudio_27A8) is inlined
// here under /Ob2.  It resets the vtable, frees m_pStates if non-null, then
// resets the vtable to rage::datBase to mark the object dead.

void mcMemcardControl::~mcMemcardControl(bool shouldFree)
{
    // Restore mcMemcardControl vtable (in case we arrived via base-class ptr)
    // vtable @ 0x8204D684
    m_pVtable = (void*)0x8204D684;

    // Restore embedded sub-object vtable (reset to rage::datBase @ 0x820276C4)
    m_saveOp.m_pVtable = (void*)0x820276C4;

    // -- inlined fsmMachine destructor body --
    // Reset main vtable to fsmMachine @ 0x8204DD14
    m_pVtable = (void*)0x8204DD14;

    // Free the states array if allocated
    if (m_pStates != nullptr) {
        rage_free(m_pStates);
        m_pStates = nullptr;
    }

    // Mark object dead (vtable → rage::datBase)
    m_pVtable = (void*)0x820276C4;

    // Conditionally free this (heap-allocated instance)
    if (shouldFree) {
        rage_free(this);
    }
}


// ── mcMemcardControl::Init @ 0x8221E628 | size: 0x488 ─────────────────────────
//
// vfn_8 — Constructor / Init.
// Sets m_stateCount = 11, allocates the states array (44 bytes, 16-byte aligned),
// zeros it, then allocates and constructs each of the 11 concrete state objects
// in order (Idle → Unformat).  Each state object receives its concrete vtable
// and is stored into m_pStates[i].
//
// After the states are set up, the function continues (in the truncated region)
// to initialise the embedded mcSaveOp sub-object and any additional fields via
// the RAGE three-step factory (create / bind / finalize) for the datBase layer.
//
// All allocations go through the per-thread RAGE heap (retrieved from the SDA
// global at r13+0, then dereferenced at [heapBase + 4]).

void mcMemcardControl::Init()
{
    // Mark stateCount and allocate the states pointer array
    m_stateCount = kMcStateCount;

    // xe_main_thread_init_0038: ensures the per-thread heap is ready
    xe_main_thread_init();

    // Heap manager lives at SDA[0] → deref [heapBase+4] for the allocator vtable
    IHeap* pHeap = GetThreadHeap();

    // Allocate array of 11 state pointers (44 bytes, aligned 16)
    mcMemcardStatus** pStatesArr = (mcMemcardStatus**)pHeap->Allocate(44, 16);
    m_pStates = pStatesArr;

    // Zero the array (11 × 4 bytes)
    for (int i = 0; i < kMcStateCount; i++) {
        pStatesArr[i] = nullptr;
    }

    // Allocate and init Idle state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Idle* pIdle = (Idle*)pHeap->Allocate(8, 16);
        if (pIdle) {
            pIdle->m_unk0 = 0;
            pIdle->m_pVtable = (void*)0x8204D6D4;  // mcMemcardControl::Idle vtable
        }
        m_pStates[MC_STATE_IDLE] = pIdle;
    }

    // Allocate and init Find state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Find* pFind = (Find*)pHeap->Allocate(8, 16);
        if (pFind) {
            pFind->m_unk0 = 0;
            pFind->m_pVtable = (void*)0x8204D714;  // mcMemcardControl::Find vtable
        }
        m_pStates[MC_STATE_FIND] = pFind;
    }

    // Allocate and init Select state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Select* pSelect = (Select*)pHeap->Allocate(8, 16);
        if (pSelect) {
            pSelect->m_unk0 = 0;
            pSelect->m_pVtable = (void*)0x8204D754;  // mcMemcardControl::Select vtable
        }
        m_pStates[MC_STATE_SELECT] = pSelect;
    }

    // Allocate and init Load state (12 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Load* pLoad = (Load*)pHeap->Allocate(12, 16);
        if (pLoad) {
            pLoad->m_unk0 = 0;
            pLoad->m_pVtable = (void*)0x8204D794;  // mcMemcardControl::Load vtable
        }
        m_pStates[MC_STATE_LOAD] = pLoad;
    }

    // Allocate and init Save state (12 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Save* pSave = (Save*)pHeap->Allocate(12, 16);
        if (pSave) {
            pSave->m_unk0 = 0;
            pSave->m_pVtable = (void*)0x8204D7D4;  // mcMemcardControl::Save vtable
        }
        m_pStates[MC_STATE_SAVE] = pSave;
    }

    // Allocate and init SaveIcon state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        SaveIcon* pSaveIcon = (SaveIcon*)pHeap->Allocate(8, 16);
        if (pSaveIcon) {
            pSaveIcon->m_unk0 = 0;
            pSaveIcon->m_pVtable = (void*)0x8204D814;  // mcMemcardControl::SaveIcon vtable
        }
        m_pStates[MC_STATE_SAVE_ICON] = pSaveIcon;
    }

    // Allocate and init Search state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Search* pSearch = (Search*)pHeap->Allocate(8, 16);
        if (pSearch) {
            pSearch->m_unk0 = 0;
            pSearch->m_pVtable = (void*)0x8204D854;  // mcMemcardControl::Search vtable
        }
        m_pStates[MC_STATE_SEARCH] = pSearch;
    }

    // Allocate and init Remove state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Remove* pRemove = (Remove*)pHeap->Allocate(8, 16);
        if (pRemove) {
            pRemove->m_unk0 = 0;
            pRemove->m_pVtable = (void*)0x8204D894;  // mcMemcardControl::Remove vtable
        }
        m_pStates[MC_STATE_REMOVE] = pRemove;
    }

    // Allocate and init GetFreeSpace state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        GetFreeSpace* pGFS = (GetFreeSpace*)pHeap->Allocate(8, 16);
        if (pGFS) {
            pGFS->m_unk0 = 0;
            pGFS->m_pVtable = (void*)0x8204D8D4;  // mcMemcardControl::GetFreeSpace vtable
        }
        m_pStates[MC_STATE_GET_FREE] = pGFS;
    }

    // Allocate and init Format state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Format* pFormat = (Format*)pHeap->Allocate(8, 16);
        if (pFormat) {
            pFormat->m_unk0 = 0;
            pFormat->m_pVtable = (void*)0x8204D914;  // mcMemcardControl::Format vtable
        }
        m_pStates[MC_STATE_FORMAT] = pFormat;
    }

    // Allocate and init Unformat state (8 bytes, aligned 16)
    xe_main_thread_init();
    pHeap = GetThreadHeap();
    {
        Unformat* pUnformat = (Unformat*)pHeap->Allocate(8, 16);
        if (pUnformat) {
            pUnformat->m_unk0 = 0;
            pUnformat->m_pVtable = (void*)0x8204D954;  // mcMemcardControl::Unformat vtable
        }
        m_pStates[MC_STATE_UNFORMAT] = pUnformat;
    }

    // TODO: lines 300–522 (truncated) — initialise mcSaveOp sub-object at +0x2C
    // using rage_obj_factory_create_3040 / rage_obj_bind_3828 / rage_obj_finalize_3B38
    // and atSingleton_2E60_g; also resets m_currentState, m_result1, m_result2.
}


// ── mcMemcardControl::Shutdown @ 0x8221EAB0 | size: 0x1CC ────────────────────
//
// vfn_9 — Destructor helper / Shutdown.
// Walks the states array in reverse-dependency order, calling the virtual
// delete destructor (slot 0, r4=1) on each non-null state object.
// After all states are released, frees the states array itself and nulls
// the m_pStates pointer.
//
// Destruction order observed in assembly: 10, 9, 8, 7, 4, 5, 6, 3, 2, 1, 0
// (roughly reverse of Init, with states 4-6 re-ordered for dependency reasons).

void mcMemcardControl::Shutdown()
{
    if (m_pStates == nullptr)
        return;

    // Destroy in dependency order (reverse of construction, with reordering)
    static const int kDestroyOrder[] = { 10, 9, 8, 7, 4, 5, 6, 3, 2, 1, 0 };

    for (int i = 0; i < kMcStateCount; i++) {
        int idx = kDestroyOrder[i];
        mcMemcardStatus* pState = m_pStates[idx];
        if (pState != nullptr) {
            // Virtual delete destructor: slot 0, second arg = 1 (heap-allocated)
            pState->~mcMemcardStatus(/*shouldFree=*/true);
        }
    }

    // Free the states array and clear the pointer
    rage_free(m_pStates);
    m_pStates = nullptr;
}


// ── mcMemcardControl::Reset @ 0x8221EC80 | size: 0x44 ────────────────────────
//
// vfn_10 — Reset all operational state back to defaults.
// Clears both result fields, resets the embedded mcSaveOp metadata, and
// zeroes the content filename buffer.  Does NOT re-allocate or re-init
// the state objects — only data fields are affected.

void mcMemcardControl::Reset()
{
    // Reset operation results (both to "no result / invalid")
    m_result1 = -1;
    m_result2 = -1;

    // Reset embedded save-op metadata
    mcSaveOp* pOp = &m_saveOp;
    pOp->m_deviceResult = -1;
    pOp->m_opStatus     =  0;
    pOp->m_pendingOp    =  0;

    // Zero the content filename buffer (42 chars + NUL = 43 bytes)
    // Assembly: 10 × stw (40 bytes) + sth (2 bytes) + stb (1 byte) = 43 bytes
    memset(pOp->m_szContentFileName, 0, sizeof(pOp->m_szContentFileName));
}


// ── mcMemcardControl::IsTransitionAllowed @ 0x8221ECC8 | size: 0x54 ──────────
//
// vfn_13 — Query whether a state transition from m_currentState → targetState
// is legal.
//
// Transition table:
//   Idle (0)   → any           always allowed
//   any        → Idle (0)      always allowed
//   Save (4)   → SaveIcon (5)  allowed
//   Save (4)   → Remove  (7)   allowed (clear space before saving)
//   Remove (7) → Save    (4)   allowed (retry save after clearing space)
//   all other transitions      DENIED
//
// Returns: 1 if allowed, 0 if denied.

bool mcMemcardControl::IsTransitionAllowed(int32_t targetState) const
{
    if (m_currentState == 0) {
        // From Idle: any transition is valid
        return true;
    }

    // From any active state: always allow going back to Idle
    if (targetState == 0) {
        return true;
    }

    // Save can branch to SaveIcon or Remove
    if (m_currentState == MC_STATE_SAVE) {
        if (targetState == MC_STATE_SAVE_ICON) return true;
        if (targetState == MC_STATE_REMOVE)    return true;
    }

    // Remove can loop back to Save (after clearing space)
    if (m_currentState == MC_STATE_REMOVE && targetState == MC_STATE_SAVE) {
        return true;
    }

    // All other transitions are forbidden
    return false;
}


// ── mcMemcardControl::RequestOp @ 0x8221ED20 | size: 0x18 ────────────────────
//
// vfn_14 — Mark that a new operation should be initiated.
// Only effective when the machine is in the Idle state (m_currentState == 0);
// sets m_pendingOp = 1 to signal the update loop that work is waiting.

void mcMemcardControl::RequestOp()
{
    if (m_currentState != 0) {
        // Already busy — ignore the request
        return;
    }

    m_saveOp.m_pendingOp = 1;
}


// ── fsmMachine destructor body @ 0x822227A8 | size: 0x58 ─────────────────────
//
// (gameLoop_DestroyAudio_27A8 in the scaffold — misleading auto-name)
//
// Shared destructor body called by both fsmMachine::~fsmMachine() and
// (inlined into) mcMemcardControl::~mcMemcardControl().
// Resets the vtable to fsmMachine, frees m_pStates, resets to rage::datBase.

void fsmMachine_DestructorBody(fsmMachine* self)
{
    // Reset to fsmMachine vtable while we free resources
    self->m_pVtable = (void*)0x8204DD14;  // fsmMachine vtable

    // Release state array if present
    if (self->m_pStates != nullptr) {
        rage_free(self->m_pStates);
        self->m_pStates = nullptr;
    }

    // Object is now dead — downgrade vtable to base
    self->m_pVtable = (void*)0x820276C4;  // rage::datBase vtable
}


// ── fsmMachine::~fsmMachine @ 0x82222758 | size: 0x50 ────────────────────────
//
// vfn_0 of fsmMachine.  Delegates to fsmMachine_DestructorBody, then optionally
// frees 'this' when called as delete destructor (shouldFree = true).

void fsmMachine::~fsmMachine(bool shouldFree)
{
    fsmMachine_DestructorBody(this);

    if (shouldFree) {
        rage_free(this);
    }
}
