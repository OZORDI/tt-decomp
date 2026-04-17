/**
 * hud.cpp — HUD and Flash-based UI implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "hud.hpp"
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// Shared helpers used by this cluster's virtual-function overrides
// ─────────────────────────────────────────────────────────────────────────────

// rage global allocator free @ 0x820C00C0
extern "C" void rage_free(void* pObject);

// Inner destructor helpers (per-derived-class teardown before base dtor).
// Each resets vtables, frees class-owned sub-objects, then calls the base
// dtor chain; these are invoked from the scalar-deleting destructor.
extern "C" void hudPause_InnerDtor(void* pThis);                // @ 0x82312CB8
extern "C" void hudShell_InnerDtor(void* pThis);                // @ 0x823168D0
extern "C" void hudFrontEnd_BaseDtor(void* pThis);              // @ 0x822EAD88
extern "C" void hudLoadingScreen_InnerDtor(void* pThis);        // @ 0x8231B438

// Forward-declare the base slot-6 (OnExit) — hudPause::OnExit tail-calls it.
// Mirrors the declaration already exposed on hudFlashBase (slot 6).
extern "C" void hudFlashBase_OnExit(void* pThis);               // @ 0x822EB148

// External references
extern void SetHudParameter(void* client, int param, float value);
extern uint8_t* g_pGlobalGameState;  // @ 0x82606430 (SDA r13+25648)

// Float constants
static const float g_floatOne = 1.0f;      // @ 0x8202D108+8
static const float g_floatZero = 0.0f;     // @ 0x8202D108


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingHUD::OnEvent @ 0x822FA518 | size: 0xE0
//
// Handles training HUD events by checking event codes and dispatching to
// the network client with appropriate parameters.
//
// Event codes:
//   2146 (0x862) - Training event with param=1, value=1.0f
//                  Also clears a flag at global state offset +6208+32
//   2147 (0x863) - Training event with param=0, value=1.0f  
//   2080 (0x820) - Training event with param=0, value=0.0f
//
// Parameters:
//   eventData - Pointer to event data structure (first uint16_t is event code)
// ─────────────────────────────────────────────────────────────────────────────
void hudTrainingHUD::OnEvent(void* eventData)
{
    // Read event code from first uint16_t of event data
    uint16_t eventCode = *(uint16_t*)eventData;
    
    // Get pointer to this object's network client interface (at offset -96)
    void* networkClient = (char*)this - 96;
    
    // Handle event code 2146 (0x862)
    if (eventCode == 2146) {
        // Access global game state
        uint8_t* globalState = *(uint8_t**)g_pGlobalGameState;
        
        // Get player slot array at offset +132
        uint32_t slotIndex = *(uint32_t*)(globalState + 132);
        
        // Calculate array index: (slotIndex + 29) * 4
        uint32_t arrayIndex = (slotIndex + 29) * 4;
        
        // Get player object from array
        void* playerObj = *(void**)(globalState + arrayIndex);
        
        // Check if training state object exists at offset +6208
        void* trainingState = *(void**)((char*)playerObj + 6208);
        
        if (trainingState != nullptr) {
            // Clear flag at offset +32 in training state
            *(uint32_t*)((char*)trainingState + 32) = 0;
        }
        
        // Call network client with param=1, value=1.0f
        SetHudParameter(networkClient, 1, g_floatOne);
        return;
    }
    
    // Handle event code 2147 (0x863)
    if (eventCode == 2147) {
        // Call network client with param=0, value=1.0f
        SetHudParameter(networkClient, 0, g_floatOne);
        return;
    }
    
    // Handle event code 2080 (0x820)
    if (eventCode == 2080) {
        // Call network client with param=0, value=0.0f
        SetHudParameter(networkClient, 0, g_floatZero);
        return;
    }
    
    // No matching event code - return without action
}


// External data structures
extern uint32_t g_hudObjectArray[];      // @ 0x825C8ED8 (124 bytes, 31 entries)
extern void*    g_pHudBasePointer;       // @ 0x825EB988
extern uint32_t g_hudLookupTable[][2];   // @ 0x825B1F90 (696 bytes, 87 entries of 8 bytes)


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase::SetPropertyById @ 0x8215B060 | size: 0x84
//
// Sets a property value for a HUD object identified by an ID.
// Looks up the object ID in a global lookup table and stores the value
// at the corresponding offset in the object array.
//
// The function:
// 1. Calculates array index from objectId (objectId * 4)
// 2. Gets the object offset from g_hudObjectArray
// 3. Adds base pointer to get actual object address
// 4. Calls a virtual function at offset +96 on the object
// 5. Searches g_hudLookupTable (87 entries) for matching object offset
// 6. When found, stores the value at the corresponding slot (offset +4)
//
// Parameters:
//   objectId - ID of the HUD object (used as array index)
//   value    - Value to store for this object
// ─────────────────────────────────────────────────────────────────────────────
void hudFlashBase::SetPropertyById(int objectId, int value)
{
    // Calculate array index: objectId * 4 (rlwinm r30,r3,2,0,29)
    uint32_t arrayIndex = objectId * 4;
    
    // Get object offset from array
    uint32_t objectOffset = g_hudObjectArray[objectId];
    
    // Add base pointer to get actual object address
    void* hudObject = (char*)g_pHudBasePointer + objectOffset;
    
    // Call virtual function at offset +96
    typedef void (*VirtualFunc)(void*);
    VirtualFunc vfunc = *(VirtualFunc*)((char*)hudObject + 96);
    vfunc(hudObject);
    
    // Search lookup table for matching object offset
    // Table has 87 entries, each 8 bytes: [objectOffset, valueSlot]
    for (int i = 0; i < 87; i++) {
        if (g_hudLookupTable[i][0] == objectOffset) {
            // Found match - store value at offset +4 in the entry
            // rlwinm r6,r10,3,0,28 = i * 8 (entry size)
            g_hudLookupTable[i][1] = value;
            return;
        }
    }
    
    // No match found - return without storing
}
/**
 * ui/hud.cpp — Network and UI utility functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "ui/hud.hpp"
#include <stdint.h>

/* ── External dependencies for network cleanup ────────────────────────────── */

/* RAGE cleanup function @ 0x8214C788 */
extern void mfResource_Release(void* pObject);

/* Network cleanup function @ 0x82115B40 */
extern void mfResource_Free(void* pObject, int mode);

/* Vtable for rage::mfAnimationController @ 0x8205A054 */
extern void* g_vtable_mfAnimationController;  /* @ 0x8205A054 */


/* ═══════════════════════════════════════════════════════════════════════════
 * mfAnimationController_DestroyBody @ 0x822DF5D8 | size: 0x5c (92 bytes)
 *
 * Network object cleanup/destructor function.
 *
 * This function performs cleanup operations on a network object, setting its
 * vtable and calling cleanup functions on embedded objects.
 *
 * Network object layout (partial):
 *   +0x00 (0)    m_vtable      - Vtable pointer
 *   +0x04 (4)    m_embedded    - Embedded object requiring cleanup
 *   +0x1C (28)   m_pResource   - Resource pointer (at embedded+24)
 *   +0x22 (34)   m_flags       - Status flags (at embedded+30)
 *
 * Algorithm:
 *   1. Set vtable to mfAnimationController
 *   2. Call mfResource_Release on embedded object at offset +4
 *   3. If flags at embedded+30 are non-zero:
 *      a. Check if resource pointer at embedded+24 is non-null
 *      b. If so, call mfResource_Free with mode 3 to clean up resource
 * ═══════════════════════════════════════════════════════════════════════════ */
void mfAnimationController_DestroyBody(void* pThis)
{
    uint8_t* netObj = (uint8_t*)pThis;
    
    /* Set vtable pointer */
    *(void**)netObj = &g_vtable_mfAnimationController;
    
    /* Get pointer to embedded object at offset +4 */
    void* pEmbedded = netObj + 4;
    
    /* Call RAGE cleanup on embedded object */
    mfResource_Release(pEmbedded);
    
    /* Check flags at embedded+30 */
    uint16_t flags = *(uint16_t*)((uint8_t*)pEmbedded + 30);
    
    if (flags != 0) {
        /* Load resource pointer from embedded+24 */
        void* pResource = *(void**)((uint8_t*)pEmbedded + 24);

        if (pResource != NULL) {
            /* Clean up resource with mode 3 */
            mfResource_Free(pResource, 3);
        }
    }
}


// ═════════════════════════════════════════════════════════════════════════════
// Loading / Pause / Shell / FrontEnd screen cluster
//
// Shared pattern for slot-0 across all four classes:
//   pass5 receives (this, deleteFlag) from the scalar-deleting thunk; it runs
//   the class-specific inner destructor, then — only if (deleteFlag & 1) — it
//   returns the allocation to the rage global heap. The low bit is the
//   standard "should free" selector emitted by the MSVC/Rockstar RTTI dtor
//   thunks. Modelled here as a second arg because these overrides are called
//   through the vtable, not directly as a C++ destructor.
// ═════════════════════════════════════════════════════════════════════════════


// ─────────────────────────────────────────────────────────────────────────────
// hudPause::~hudPause (slot 0) @ 0x82312C68 | size: 0x50
// Runs the hudPause-specific teardown at 0x82312CB8, then frees the object
// when invoked as a scalar-delete (deleteFlag bit0 set).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudPause_vfn_0(void* pThis, uint32_t deleteFlag)
{
    hudPause_InnerDtor(pThis);
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudPause::OnExit (slot 6) @ 0x82314018 | size: 0x20
//
// When the pause menu is dismissed, reset the movie's per-frame blend/alpha
// field at +8 to a global scalar and forward to hudFlashBase::OnExit for the
// shared hide path. The guard byte at +99 gates whether the reset fires.
// ─────────────────────────────────────────────────────────────────────────────
// Pause-overlay default alpha, float @ 0x825C90BC
extern const float g_hudPause_DefaultAlpha;

void hudPause::OnExit()
{
    uint8_t* self = (uint8_t*)this;

    // Bail out if the exit-overlay flag at +99 is clear.
    if (self[99] == 0) return;

    // Reset the pause-movie alpha at +8 to the global default.
    *(float*)(self + 8) = g_hudPause_DefaultAlpha;

    // Tail into the shared Flash-base hide path.
    hudFlashBase_OnExit(this);
}


// ─────────────────────────────────────────────────────────────────────────────
// hudShell::~hudShell (slot 0) @ 0x82316880 | size: 0x50
// Same scalar-delete pattern as hudPause::~hudPause.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudShell_vfn_0(void* pThis, uint32_t deleteFlag)
{
    hudShell_InnerDtor(pThis);
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudShell::vfn_2 (Update / per-tick) @ 0x82316A20 | size: 0xA8
//
// Per pseudocode (confirmed against pass5 scaffold):
//   1. Call the base per-frame Update (vtable slot 3, offset +12) with the
//      global tick-delta scalar at g_hudShell_TickDelta (+84 into the
//      .data block at 0x825CAEB8).
//   2. Ask the SinglesNetworkClient page-group for a specific key. If the key
//      is present, its integer value is read, otherwise -1 (the stack default
//      at sp+80 is set by the caller on the "not-found" branch).
//   3. Store (value != -1) as a uint8 flag at +96 on the shell.
//   4. If the earlier B2A8 query reported dirty state, invoke the base
//      "refresh" helper at B320 to republish the page-group.
//
// The two lbl_ strings the SDK-helpers consume are plain ASCII keys in
// rodata; the pass5 recomp addresses them via a small offset from a .rdata
// base. Keep them as opaque void* — the network client owns their names.
// ─────────────────────────────────────────────────────────────────────────────

// Opaque SinglesNetworkClient helpers exposed from the network layer
extern "C" bool   hudFlashBase_IsNetworkDirty(void* pThis);        // @ 0x822EB2A8
extern "C" void*  hudFlashBase_GetPageGroup(void* pThis);          // @ 0x822EB1E8
extern "C" void   hudFlashBase_PublishPageGroup(void* pThis);      // @ 0x822EB320
extern "C" void*  SinglesNetworkClient_FindParam(void* pageGroup,
                                                 const void* key); // @ 0x823F9280
extern "C" int    SinglesNetworkClient_ReadInt(void* pParam);      // @ 0x823FA5C8

// Shell per-frame update constants block @ 0x825CAEB8
extern const uint8_t g_hudShell_UpdateBlock[0x60];

// Page-group lookup key (".rdata" literal, owned by network strings pool)
extern const void* const g_hudShell_LookupKey;

void hudShell::vfn_2()
{
    uint8_t* self = (uint8_t*)this;

    // 1) Base Update(dt) via vtable slot 3 (byte offset +12).
    const float tickDelta = *(const float*)(g_hudShell_UpdateBlock + 84);
    typedef void (*UpdateFn)(void*, float);
    UpdateFn baseUpdate = *(UpdateFn*)((*(uint8_t**)self) + 12);
    baseUpdate(this, tickDelta);

    // 2) Network query — dirty-flag first, then page-group lookup.
    bool  netDirty  = hudFlashBase_IsNetworkDirty(this);
    void* pageGroup = hudFlashBase_GetPageGroup(this);

    void* pParam = SinglesNetworkClient_FindParam(pageGroup,
                                                  g_hudShell_LookupKey);
    int value;
    if (pParam != nullptr) {
        value = SinglesNetworkClient_ReadInt(pParam);
    } else {
        // On the "missing" path pass5 reloads the caller-owned scratch slot;
        // treat that as the sentinel -1 (matches the "value != -1" test).
        value = -1;
    }

    // 3) Store presence flag at +96 (1 iff the key resolved to a real value).
    self[96] = (value != -1) ? 1 : 0;

    // 4) If the network state was dirty, republish the page-group.
    if (netDirty) {
        hudFlashBase_PublishPageGroup(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudFrontEnd::~hudFrontEnd (slot 0) @ 0x823233C8 | size: 0x7C
//
// The front-end owns two heap arrays keyed by trailing uint16 counts:
//   +0x12A (298) count / +0x124 (292) pointer  — primary array
//   +0x132 (306) count / +0x12C (300) pointer  — secondary array
// Both are freed when their count is non-zero, then the shared base dtor
// (rage_AD88) runs before the optional scalar-delete free.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudFrontEnd_vfn_0(void* pThis, uint32_t deleteFlag)
{
    uint8_t* self = (uint8_t*)pThis;

    // Secondary array (count @ +306, data @ +300).
    uint16_t countSecondary = *(uint16_t*)(self + 306);
    if (countSecondary != 0) {
        void* dataSecondary = *(void**)(self + 300);
        rage_free(dataSecondary);
    }

    // Primary array (count @ +298, data @ +292).
    uint16_t countPrimary = *(uint16_t*)(self + 298);
    if (countPrimary != 0) {
        void* dataPrimary = *(void**)(self + 292);
        rage_free(dataPrimary);
    }

    // Shared base teardown.
    hudFrontEnd_BaseDtor(pThis);

    // Scalar-delete selector.
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLoadingScreen::~hudLoadingScreen (slot 0) @ 0x8231E6E0 | size: 0x50
// Same scalar-delete pattern; inner dtor lives at 0x8231B438.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudLoadingScreen_vfn_0(void* pThis, uint32_t deleteFlag)
{
    hudLoadingScreen_InnerDtor(pThis);
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLoadingScreen::vfn_11 @ 0x8231CC58 | size: 0xC4
//
// Refresh-panel hook: after the base update refreshes state, publish two
// page-group entries ("status" and the copied tip-string buffer). The
// g_input_obj_ptr poll gate decides whether the string-copy branch runs;
// if input is idle (poll returns 0) the tip is pushed, otherwise only the
// status entry is touched.
//
// On exit, two sub-resources at +116 and +120 are released through the
// shared resource releaser at 0x82566F10 regardless of the branch taken.
// ─────────────────────────────────────────────────────────────────────────────

extern "C" void* SinglesNetworkClient_BindEntry(void* pageGroup,
                                                const void* key);  // @ 0x823F9318
extern "C" bool  rage_input_poll(void* pInput);                    // @ 0x821C9D68
extern "C" void  rage_atStringCopy(void* src, void* dst, int cap); // @ 0x82228990
extern "C" void  rage_resource_release(void* pResource);           // @ 0x82566F10

// rage global input object pointer @ 0x825EAB28
extern void* g_input_obj_ptr;

// Network-key literals baked into .rdata (strings pool).
extern const void* const g_hudLoading_KeyStatus;
extern const void* const g_hudLoading_KeyTipText;
extern const void* const g_hudLoading_TipTemplate;

void hudLoadingScreen::vfn_11()
{
    uint8_t* self = (uint8_t*)this;

    bool  netDirty  = hudFlashBase_IsNetworkDirty(this);
    void* pageGroup = hudFlashBase_GetPageGroup(this);

    // Bind "status" entry — value 1, kind 3.
    void* pStatus = SinglesNetworkClient_BindEntry(pageGroup,
                                                   g_hudLoading_KeyStatus);
    if (pStatus != nullptr) {
        *(uint32_t*)((uint8_t*)pStatus + 0) = 1;
        *(uint32_t*)((uint8_t*)pStatus + 4) = 3;
    }

    // Only copy the tip text when input is idle.
    if (!rage_input_poll(g_input_obj_ptr)) {
        // Destination buffer lives inline at +1469 (0x5BD) on the screen.
        void* tipDst = (void*)(self + 1469);
        rage_atStringCopy((void*)g_hudLoading_TipTemplate, tipDst, 64);

        void* pTip = SinglesNetworkClient_BindEntry(pageGroup,
                                                    g_hudLoading_KeyTipText);
        if (pTip != nullptr) {
            *(void**  )((uint8_t*)pTip + 0) = tipDst;
            *(uint32_t*)((uint8_t*)pTip + 4) = 1;
        }
    }

    // If the network state was dirty, republish the page-group.
    if (netDirty) {
        hudFlashBase_PublishPageGroup(this);
    }

    // Release the two sub-resources unconditionally.
    rage_resource_release(*(void**)(self + 116));
    rage_resource_release(*(void**)(self + 120));
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLoadingScreen::vfn_12 @ 0x8231CD20 | size: 0x1C
//
// Tiny state-predicate: returns true iff the screen-state integer at +4116
// equals the "ready" phase (3). Used by the outer front-end to decide when
// the loading screen has finished its interactive-panel reveal.
// ─────────────────────────────────────────────────────────────────────────────
void hudLoadingScreen::vfn_12()
{
    // Preserves the return-via-r3 uint8 contract of the original thunk —
    // the caller reads the low byte; declared void in the header to match.
    uint32_t state = *(uint32_t*)((uint8_t*)this + 4116);
    uint8_t  result = (state == 3) ? 1 : 0;
    (void)result;
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLoadingScreen::vfn_13 @ 0x8231CD40 | size: 0x1C
//
// Queue-ready notification: if the primary state int at +96 is zero
// (meaning no pending job) then set the "dispatch" byte at +20 of the child
// object pointed to by +100 to 1. Fires once per frame from the outer tick.
// ─────────────────────────────────────────────────────────────────────────────
void hudLoadingScreen::vfn_13()
{
    uint8_t* self = (uint8_t*)this;

    // Only act when the primary state slot is idle.
    if (*(int32_t*)(self + 96) != 0) return;

    // Flag the child object as ready-for-dispatch.
    uint8_t* child = *(uint8_t**)(self + 100);
    child[20] = 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLoadingScreen::vfn_14 @ 0x8231B2B8 | size: 0x8
//
// Minimal accessor: fetch the uint8 flag at +4105 (0x1009) on the screen
// and return it in r3. The surrounding pseudocode's "28 * a3 + v3" callback
// form is a compiler-emitted tail-merge artifact from adjacent slots — the
// raw PPC pass5 body is just `lbz r3, 4105(r3); blr`, so model it as-is.
// ─────────────────────────────────────────────────────────────────────────────
void hudLoadingScreen::vfn_14()
{
    // Header declares `void vfn_14()`; the binary returns a uint8 in r3.
    // Emulate the single load for correctness in side-effect traces.
    uint8_t flag = *((uint8_t*)this + 4105);
    (void)flag;
}


// ═════════════════════════════════════════════════════════════════════════════
// hudTraining + hudLeaderboard cluster — slot-0 destructors and vfn_2.
//
// All four classes here share the same scalar-delete layout as the block
// above: the slot-0 thunk receives (pThis, deleteFlag) and frees the object
// only when the low bit is set. hudTrainingHUD additionally tears down an
// embedded msgEventHandler at +0x60 and a class-owned helper at +0x40.
// ═════════════════════════════════════════════════════════════════════════════

// Shared inner-teardown for hudFlashBase-derived classes lives at 0x822EAD88
// | 0x6C and is already forward-declared above as `hudFrontEnd_BaseDtor` —
// reuse that alias here instead of introducing a duplicate symbol.

// msgEventHandler inner-teardown @ 0x8225E8C0 | 0xD8 — used by classes that
// embed an msgEventHandler sub-object.
extern "C" void msgEventHandler_InnerTeardown(void* pHandler);    // @ 0x8225E8C0

// hudLeaderboard-specific inner teardown @ 0x8232A858 | 0x278 — tears down
// the embedded xam leaderboard reader at +0x88 then chains to the shared
// hudFlashBase cleanup.
extern "C" void hudLeaderboard_InnerTeardown(void* pThis);        // @ 0x8232A858

// Compiler-emitted vtable blob addresses (one symbol per blob).
extern uint32_t g_vtbl_hudLogosScreen_primary[];     // @ 0x82060BF8
extern uint32_t g_vtbl_hudLogosScreen_secondary[];   // @ 0x82060C20
extern uint32_t g_vtbl_hudTrainingHUD_primary[];     // @ 0x8205CB50
extern uint32_t g_vtbl_hudTrainingHUD_mi1[];         // @ 0x8205CB78
extern uint32_t g_vtbl_hudTrainingHUD_mi2[];         // @ 0x8205CB88
extern uint32_t g_vtbl_hudTrainingPopUp_primary[];   // @ 0x820619B4
extern uint32_t g_vtbl_hudTrainingPopUp_secondary[]; // @ 0x820619DC

// Tail-teardown vtable stamped onto the embedded msgEventHandler slot just
// before the base dtor runs (nullsub_1 in slot 0).
extern uint32_t g_vtbl_msgEventHandler_TailTeardown[]; // @ 0x82302674

// Global "active training drill" pointer cleared by ~hudTrainingHUD.
// Address derived from `lis r9,-32160; stw ...,26132(r9)` => 0x8262E614.
extern void* g_pActiveTrainingDrill;                 // @ 0x8262E614


// ─────────────────────────────────────────────────────────────────────────────
// hudLogosScreen::~hudLogosScreen (slot 0) @ 0x82317E50 | size: 0x68
//
// Stamps the two hudLogosScreen MI vtables into the object header, runs the
// shared hudFlashBase tail cleanup, then frees the backing storage when
// invoked through the scalar-deleting thunk.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudLogosScreen_vfn_0(void* pThis, uint32_t deleteFlag)
{
    uint32_t* header = (uint32_t*)pThis;

    // Install the hudLogosScreen MI vtable pair over the object header.
    header[0] = (uint32_t)g_vtbl_hudLogosScreen_primary;
    header[1] = (uint32_t)g_vtbl_hudLogosScreen_secondary;

    // Shared base tail cleanup.
    hudFrontEnd_BaseDtor(pThis);

    // Scalar-delete selector.
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingHUD::~hudTrainingHUD (slot 0) @ 0x822FA110 | size: 0xA0
//
// Three-vtable MI header with an embedded msgEventHandler sub-object:
//   +0x00  vtable_primary    (@ 0x8205CB50)
//   +0x04  vtable_secondary  (@ 0x8205CB78)  — MI / virtual-base thunk
//   +0x40  pOwnedHelper      — polymorphic helper; slot-0 destructed then
//                              freed when non-null
//   +0x60  msgEventHandler   — embedded; its vtable slot is also restamped
//                              (@ 0x8205CB88) before teardown
//
// The destructor also clears the global active-training-drill pointer
// (g_pActiveTrainingDrill) and, just before chaining to the base cleanup,
// overwrites the handler's vtable with the nullsub_1 tail-teardown vtable at
// 0x82302674 so the base dtor doesn't double-fire the handler's real slot.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudTrainingHUD_vfn_0(void* pThis, uint32_t deleteFlag)
{
    uint32_t* header = (uint32_t*)pThis;
    uint8_t*  raw    = (uint8_t*)pThis;

    // Install primary MI vtable pair at the object header.
    header[0] = (uint32_t)g_vtbl_hudTrainingHUD_primary;
    header[1] = (uint32_t)g_vtbl_hudTrainingHUD_mi1;

    // Install the embedded msgEventHandler's vtable (at +0x60).
    void* pEventHandler = raw + 96;
    *(uint32_t*)pEventHandler = (uint32_t)g_vtbl_hudTrainingHUD_mi2;

    // Tear down that embedded handler.
    msgEventHandler_InnerTeardown(pEventHandler);

    // Destroy + free the helper pointer at +0x40 if one was allocated.
    void* pOwnedHelper = *(void**)(raw + 64);
    if (pOwnedHelper != nullptr) {
        typedef void (*SlotZero)(void*);
        SlotZero helperDtor = *(SlotZero*)pOwnedHelper;
        helperDtor(pOwnedHelper);
        rage_free(pOwnedHelper);
    }

    // Swap in the tail-teardown vtable on the embedded handler.
    *(uint32_t*)pEventHandler = (uint32_t)g_vtbl_msgEventHandler_TailTeardown;

    // Clear the global "current training drill" pointer.
    g_pActiveTrainingDrill = nullptr;

    // Shared base tail cleanup.
    hudFrontEnd_BaseDtor(pThis);

    // Scalar-delete selector.
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingPopUp::~hudTrainingPopUp (slot 0) @ 0x82318540 | size: ~0x70
//
// Two-vtable MI header; owned helper at +0x40 is torn down when non-null,
// then the shared base cleanup runs before the optional scalar-delete free.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudTrainingPopUp_vfn_0(void* pThis, uint32_t deleteFlag)
{
    uint32_t* header = (uint32_t*)pThis;
    uint8_t*  raw    = (uint8_t*)pThis;

    // Snapshot the owned helper before overwriting the header.
    void* pOwnedHelper = *(void**)(raw + 64);

    // Install the hudTrainingPopUp MI vtable pair.
    header[0] = (uint32_t)g_vtbl_hudTrainingPopUp_primary;
    header[1] = (uint32_t)g_vtbl_hudTrainingPopUp_secondary;

    // Destroy + free the owned helper if present.
    if (pOwnedHelper != nullptr) {
        typedef void (*SlotZero)(void*);
        SlotZero helperDtor = *(SlotZero*)pOwnedHelper;
        helperDtor(pOwnedHelper);
        rage_free(pOwnedHelper);
    }

    // Shared base tail cleanup.
    hudFrontEnd_BaseDtor(pThis);

    // Scalar-delete selector.
    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLeaderboard::~hudLeaderboard (slot 0) @ 0x8232A808 | size: 0x50
//
// Thin thunk: delegates to the class's dedicated inner-teardown routine at
// 0x8232A858 (which handles the embedded xam leaderboard reader state and
// chains to hudFlashBase cleanup), then frees storage on scalar-delete.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudLeaderboard_vfn_0(void* pThis, uint32_t deleteFlag)
{
    hudLeaderboard_InnerTeardown(pThis);

    if ((deleteFlag & 1) != 0) {
        rage_free(pThis);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingHUD::vfn_2 (Update tick) @ 0x822FA340 | size: 0x1D8
//
// Per-frame update for the training HUD.
//   1. Query SinglesNetworkClient dirty state (B2A8) and fetch the active
//      page-group pointer (B1E8).
//   2. Invoke the base Update(dt) through vtable slot 1 (byte +4) using the
//      shared tick-delta scalar at g_hudShell_UpdateBlock+84.
//   3. If the global "active training drill" pointer is at state 9 and its
//      pre-check query (0x821DFFF8) reports ready, reset the drill state
//      block to zero, clear an associated packed-flag byte, advance the
//      HSM to its next state, and zero an adjacent status word.
//   4. If the movie's main phase field at +100 is 1 or 2, run the fade
//      tick: decrement the phase-alpha float at +204 by the per-frame
//      decrement (UpdateBlock+8); if it crosses zero, clamp to zero and
//      publish status=3 for a known stat key.
//   5. Sample two floats on the active training player block (slot-index
//      at globalState+132, indexed to player+6600 / +6608), round to int,
//      and push each as a stat value with status=3 via a shared finder.
//   6. If the network state was dirty, re-publish the page-group.
//
// Slot-index math matches pongPlayer::Process et al.: ((slotIdx + 29) * 4)
// used as a lookup into the training-drill table that hangs off the global
// game-state at g_pGlobalGameState (SDA r13+25648).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" bool  SinglesNetworkClient_QueryDirty(void* pThis);           // 0x822EB2A8
extern "C" void* SinglesNetworkClient_GetPageGroup(void* pThis);         // 0x822EB1E8
extern "C" void* SinglesNetworkClient_FindStatByKey(void* pageGroup,
                                                    const void* key);    // 0x823F9318
extern "C" void  SinglesNetworkClient_PublishPageGroup(void* pThis);     // 0x822EB320
extern "C" bool  TrainingDrill_IsReady(void* pDrill);                    // 0x821DFFF8
extern "C" void  hsmContext_SetNextState(void* pDrill, int state);       // 0x82222800

// Pointer-holder for the active training drill (SDA-adjacent global).
extern void* g_pActiveTrainingDrill;  // @ 0x82605A60 (loaded via -32142 base)

// Training-HUD status mailbox cleared alongside the HSM advance.
extern uint32_t g_hudTrainingHUD_StatusWord;  // @ 0x825CB48C

// Page-group keys used by the training HUD stat push (rodata).
extern const void* const g_trainingHUD_Key_FadeOut;   // @ rdata -20168
extern const void* const g_trainingHUD_Key_StatA;     // @ rdata -16792
extern const void* const g_trainingHUD_Key_StatB;     // @ rdata -14340

// Float constants live at &g_hudShell_UpdateBlock — slot +84 is the base
// update tick-delta, slot +8 is the fade decrement per frame.

void hudTrainingHUD::vfn_2()
{
    uint8_t* self = (uint8_t*)this;

    // 1) Network dirty + page-group snapshot (captured before vtable call).
    bool  netDirty  = SinglesNetworkClient_QueryDirty(this);
    void* pageGroup = SinglesNetworkClient_GetPageGroup(this);

    // 2) Base Update(dt) via vtable slot 1 (byte offset +4).
    const float tickDelta = *(const float*)(g_hudShell_UpdateBlock + 84);
    typedef void (*UpdateFn)(void*, float);
    UpdateFn baseUpdate = *(UpdateFn*)((*(uint8_t**)self) + 4);
    baseUpdate(this, tickDelta);

    // 3) Drill ready-check: state machine at +12 == 9 and TrainingDrill_IsReady.
    void* pDrill = g_pActiveTrainingDrill;
    if (pDrill != nullptr && *(int32_t*)((uint8_t*)pDrill + 12) == 9) {
        // Route through global state's current player slot to find the drill.
        uint8_t* globalState = *(uint8_t**)g_pGlobalGameState;
        uint32_t slotIdx    = *(uint32_t*)(globalState + 132);
        void*    playerObj  = *(void**)(globalState + ((slotIdx + 29) * 4));
        void*    drillState = *(void**)((uint8_t*)playerObj + 808);

        if (drillState != nullptr && TrainingDrill_IsReady(drillState)) {
            // Zero out the drill's float-slot-0 field, clear low-7 bit of
            // the packed flag byte at +8, then advance the drill HSM.
            uint8_t* drill = (uint8_t*)pDrill;
            *(float*)(drill + 0) = g_floatZero;
            *(uint32_t*)(drill + 4) = 0;
            drill[8] = (uint8_t)(drill[8] & 191);  // clear bit 6

            hsmContext_SetNextState(pDrill, 10);

            // Mirror status word to 0 (matches pass5 stw r26,18316(r10)).
            g_hudTrainingHUD_StatusWord = 0;
        }
    }

    // 4) Phase fade — only active in phase 1 or 2.
    uint32_t phase = *(uint32_t*)(self + 100);
    if (phase == 1 || phase == 2) {
        float alpha = *(float*)(self + 204);
        if (alpha > g_floatZero) {
            float dec = *(const float*)(g_hudShell_UpdateBlock + 8);
            alpha -= dec;
            *(float*)(self + 204) = alpha;
            if (alpha <= g_floatZero) {
                *(float*)(self + 204) = g_floatZero;

                void* pStat = SinglesNetworkClient_FindStatByKey(
                    pageGroup, &g_trainingHUD_Key_FadeOut);
                if (pStat != nullptr) {
                    *(uint32_t*)((uint8_t*)pStat + 0) = 0;
                    *(uint32_t*)((uint8_t*)pStat + 4) = 3;
                }
            }
        }
    }

    // 5) Push two player-side floats (rounded to int) as stats.
    uint8_t* globalState = *(uint8_t**)g_pGlobalGameState;
    uint32_t slotIdx     = *(uint32_t*)(globalState + 132);
    void*    playerObj   = *(void**)(globalState + ((slotIdx + 29) * 4));

    {
        float  fA = *(float*)((uint8_t*)playerObj + 6600);
        int    iA = (int)fA;
        void*  pStat = SinglesNetworkClient_FindStatByKey(
            pageGroup, &g_trainingHUD_Key_StatA);
        if (pStat != nullptr) {
            *(uint32_t*)((uint8_t*)pStat + 0) = (uint32_t)iA;
            *(uint32_t*)((uint8_t*)pStat + 4) = 3;
        }
    }
    {
        float  fB = *(float*)((uint8_t*)playerObj + 6608);
        int    iB = (int)fB;
        void*  pStat = SinglesNetworkClient_FindStatByKey(
            pageGroup, &g_trainingHUD_Key_StatB);
        if (pStat != nullptr) {
            *(uint32_t*)((uint8_t*)pStat + 0) = (uint32_t)iB;
            *(uint32_t*)((uint8_t*)pStat + 4) = 3;
        }
    }

    // 6) Republish if network state was dirty on entry.
    if (netDirty) {
        SinglesNetworkClient_PublishPageGroup(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLeaderboard::vfn_2 (Update tick) @ 0x8232BC50 | size: 0x128
//
// Per-frame update for the leaderboard screen.
//   1. Pump the embedded xam reader at +136 via its tick (0x82373150).
//   2. Invoke the base Update(dt) through vtable slot 3 (byte +12) with the
//      shared tick-delta at g_hudShell_UpdateBlock+84.
//   3. Query dirty state + page-group for later publish.
//   4. Sweep the 4-entry row-status table at 0x82619AF0 (entries stride 36,
//      table spans +4..+148). For each active row (byte +24 non-zero):
//        bit 0 of the flag word  → call the "row present" helper (0x82336850)
//        bit 1 of the flag word  → call the "row missing" helper (0x82336250)
//   5. Delegate to the leaderboard-local stat-flush helper at 0x8232BD78.
//   6. Push a mode stat under Key_Mode — value=0 if the global byte at
//      0x8205D0F5 is non-zero, else value=1; status=3 either way.
//   7. Republish the page-group if dirty on entry.
//
// The row-table layout (4×36 bytes) and offsets match the field-map exposed
// by the leaderboard inner-teardown; the flag-word low 2 bits select the
// dispatch arm (pass5 emits them via rlwinm mask 0x1 / 0x2).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void xamReaderState_Tick(void* pReader);                   // 0x82373150
extern "C" void hudLeaderboard_RowPresent(void* pThis);               // 0x82336850
extern "C" void hudLeaderboard_RowMissing(void* pThis);               // 0x82336250
extern "C" void hudLeaderboard_FlushRowStats(void* pThis, void* pg);  // 0x8232BD78

// Leaderboard row-status table: 4 × 36 bytes starting at +4 of this 144-byte
// block; entry byte +24 is the "active" flag, entry word +0 is the bitfield.
extern uint8_t g_hudLeaderboard_RowTable[0x90];  // @ 0x82619AF0

// Global mode byte consumed on the publish path ("singles" vs "challenge").
extern uint8_t g_hudLeaderboard_ModeByte;  // @ 0x8205D0F5

// Page-group keys used by the leaderboard stat push (rodata).
extern const void* const g_leaderboard_Key_Mode;  // @ rdata +18440

void hudLeaderboard::vfn_2()
{
    uint8_t* self = (uint8_t*)this;

    // 1) Tick the embedded xam reader (spans +136..+228).
    xamReaderState_Tick(self + 136);

    // 2) Base Update(dt) via vtable slot 3 (byte offset +12).
    const float tickDelta = *(const float*)(g_hudShell_UpdateBlock + 84);
    typedef void (*UpdateFn)(void*, float);
    UpdateFn baseUpdate = *(UpdateFn*)((*(uint8_t**)self) + 12);
    baseUpdate(this, tickDelta);

    // 3) Capture network state snapshot for later publish decision.
    bool  netDirty  = SinglesNetworkClient_QueryDirty(this);
    void* pageGroup = SinglesNetworkClient_GetPageGroup(this);

    // 4) Sweep the 4-row state table. Table base at +4 of the 144-byte block,
    //    stride 36, end at +148. Each entry's +24 byte gates dispatch.
    uint8_t* entry = g_hudLeaderboard_RowTable + 4;
    uint8_t* end   = g_hudLeaderboard_RowTable + 148;
    for (; entry < end; entry += 36) {
        if (entry[24] == 0) continue;

        uint32_t flags = *(uint32_t*)(entry + 0);
        if ((flags & 0x1) != 0) {
            hudLeaderboard_RowPresent(this);
        } else if ((flags & 0x2) != 0) {
            hudLeaderboard_RowMissing(this);
        }
    }

    // 5) Flush row-derived stats through the local helper.
    hudLeaderboard_FlushRowStats(this, pageGroup);

    // 6) Mode stat — inverted polarity: byte != 0 → value=0, byte == 0 → value=1.
    uint8_t mode = g_hudLeaderboard_ModeByte;
    void*   pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                                        &g_leaderboard_Key_Mode);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = (mode != 0) ? 0u : 1u;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 7) Republish if dirty.
    if (netDirty) {
        SinglesNetworkClient_PublishPageGroup(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudLeaderboard::vfn_5 (OnEnter / first-frame init) @ 0x8232BA78 | size: 0x1D4
//
// Called once when the leaderboard page first activates. Publishes the
// static "header" stats (player count, mode, username, focus flags) before
// kicking off the xam leaderboard read request.
//
// Flow:
//   1. Base OnEnter via vtable slot 3 (byte +12) — matches vfn_2's base call.
//   2. Reset the leaderboard-local state (0x8232EDE8, arg=0).
//   3. Snapshot network dirty + page-group for later decisions.
//   4. Publish a constant "header" slot (key rdata+10212): value=0, status=3.
//   5. Publish the co-op flag — the global global-state byte at +512 equals 1
//      maps to value=1, else value=0; status=3.
//   6. Publish the active player-index stat (global state word +4, key -8220),
//      status=3.
//   7. Publish a fixed "1" stat under key -14644, status=3.
//   8. If the page-group carries a name blob (key rdata+18388 → A5C8 read),
//      echo the name int back under key rdata+18404, status=3.
//   9. Zero the HUD's runtime flags:
//        self[100] = 0  (phase marker)
//        self[112] = 0  (row request pending)
//        self[120] = 0  (read-complete flag)
//      Then call the three "reset row view" helpers (0x8232EE48 / 0x8232EFA0
//      / 0x8232F100).
//  10. Publish a zeroed slot under key rdata+18420 (value=0, status=3).
//  11. Republish page-group if dirty.
//  12. Kick off the xam leaderboard read at +136 (0x82372AA0), set the HUD
//      "request pending" flag at +112, run the post-submit hook (ref_FC08,
//      flag=0), then flip the HUD into "reading" mode (0x822EBF20, flag=1).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void hudLeaderboard_ResetState(void* pThis, int mode);            // 0x8232EDE8
extern "C" void hudLeaderboard_ResetRowViewA(void* pThis);                   // 0x8232EE48
extern "C" void hudLeaderboard_ResetRowViewB(void* pThis);                   // 0x8232EFA0
extern "C" void hudLeaderboard_ResetRowViewC(void* pThis);                   // 0x8232F100
extern "C" void xamReaderState_SubmitRequest(void* pReader);                 // 0x82372AA0
extern "C" void hudLeaderboard_PostSubmit(void* pThis, int flag);            // 0x8232FC08
extern "C" void hudFlashBase_SetReadingMode(void* pThis, int reading);       // 0x822EBF20
extern "C" int  SinglesNetworkClient_ReadInt_g(void* pParam);                // 0x823FA5C8
extern "C" void* SinglesNetworkClient_FindParamByKey(void* pageGroup,
                                                     const void* key);      // 0x823F9280

// Page-group keys consumed by the header publish (rodata).
extern const void* const g_leaderboard_Key_Header;     // @ rdata +10212
extern const void* const g_leaderboard_Key_CoopFlag;   // @ rdata -19736
extern const void* const g_leaderboard_Key_Player;     // @ rdata -8220
extern const void* const g_leaderboard_Key_One;        // @ rdata -14644
extern const void* const g_leaderboard_Key_NameIn;     // @ rdata +18388
extern const void* const g_leaderboard_Key_NameOut;    // @ rdata +18404
extern const void* const g_leaderboard_Key_Reset;      // @ rdata +18420

// Global session-state byte consulted for the co-op polarity (player count==1).
// Lives in the global game-state block at SDA r13+25648, offset +512.

void hudLeaderboard::vfn_5()
{
    uint8_t* self = (uint8_t*)this;

    // 1) Base OnEnter via vtable slot 3 (+12).
    const float tickDelta = *(const float*)(g_hudShell_UpdateBlock + 84);
    typedef void (*EnterFn)(void*, float);
    EnterFn baseEnter = *(EnterFn*)((*(uint8_t**)self) + 12);
    baseEnter(this, tickDelta);

    // 2) Reset the leaderboard-local state machine.
    hudLeaderboard_ResetState(this, 0);

    // 3) Snapshot network state.
    bool  netDirty  = SinglesNetworkClient_QueryDirty(this);
    void* pageGroup = SinglesNetworkClient_GetPageGroup(this);

    // 4) Publish "header" constant (value=0, status=3).
    void* pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                                     &g_leaderboard_Key_Header);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = 0;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 5) Co-op flag (global-state +512 == 1 → 1 else 0).
    uint8_t* globalState = *(uint8_t**)g_pGlobalGameState;
    uint32_t sessionMode = *(uint32_t*)(globalState + 512);
    uint32_t coopFlag    = (sessionMode == 1) ? 1u : 0u;

    pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                               &g_leaderboard_Key_CoopFlag);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = coopFlag;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 6) Active player index (global-state word +4).
    uint32_t playerIdx = *(uint32_t*)(globalState + 4);
    pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                               &g_leaderboard_Key_Player);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = playerIdx;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 7) Constant "1" stat.
    pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                               &g_leaderboard_Key_One);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = 1;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 8) Optional name passthrough — default to 0, override with source int.
    uint32_t nameInt = 0;
    void* pName = SinglesNetworkClient_FindParamByKey(pageGroup,
                                                      &g_leaderboard_Key_NameIn);
    if (pName != nullptr) {
        nameInt = (uint32_t)SinglesNetworkClient_ReadInt_g(pName);
    }
    pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                               &g_leaderboard_Key_NameOut);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = nameInt;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 9) Reset the HUD's runtime bookkeeping then the three row views.
    self[100] = 0;
    *(uint32_t*)(self + 120) = 0;
    hudLeaderboard_ResetRowViewA(this);
    hudLeaderboard_ResetRowViewB(this);
    hudLeaderboard_ResetRowViewC(this);
    *(uint32_t*)(self + 112) = 0;

    // 10) Zeroed "reset" stat.
    pStat = SinglesNetworkClient_FindStatByKey(pageGroup,
                                               &g_leaderboard_Key_Reset);
    if (pStat != nullptr) {
        *(uint32_t*)((uint8_t*)pStat + 0) = 0;
        *(uint32_t*)((uint8_t*)pStat + 4) = 3;
    }

    // 11) Republish if dirty.
    if (netDirty) {
        SinglesNetworkClient_PublishPageGroup(this);
    }

    // 12) Kick off the xam read + transition into reading mode.
    xamReaderState_SubmitRequest(self + 136);
    *(uint32_t*)(self + 112) = 1;
    hudLeaderboard_PostSubmit(this, 0);
    hudFlashBase_SetReadingMode(this, 1);
}


// ═════════════════════════════════════════════════════════════════════════════
// hudFlashBase — shared base-class virtual thunks inherited by all 18 HUD
// derivatives (hudBoot, hudCharView, hudController, hudCredits, hudDialog,
// hudFrontEnd, hudHUD, hudLeaderboard, hudLegals, hudList, hudLoadingScreen,
// hudLogosScreen, hudPause, hudShell, hudTrainingHUD, hudTrainingLoadScreen,
// hudTrainingPopUp, hudUnlocks).
//
// The primary vtable at 0x8205A780 has 8 slots:
//   [0] sub_822EAFC8  — "ScalarDtor" thunk (actually Update dispatcher)
//   [1] loc_8231EFD0  — InnerDtor (lifted separately)
//   [2] sub_822EB058  — Update(dt)
//   [3] sub_822EB0E8  — TickRelease (reload-guard release path)
//   [4] nullsub_1     — no-op
//   [5] sub_822EB148  — OnExit
//   [6] nullsub_1     — no-op
//   [7] nullsub_1     — no-op
//
// These four non-trivial base bodies are shared by every derivative that does
// not override them. All pattern the same way: call hudFlashBase_IsNetworkDirty
// (0x822EB2A8), do class-specific work, and tail-call
// hudFlashBase_PublishPageGroup (0x822EB320) iff the network state was dirty
// on entry. MI HAZARD: implemented on the hudFlashBase base class only —
// derived-class overrides (e.g. hudShell::vfn_2, hudPause::OnExit) are
// handled separately in their own bodies elsewhere in this file.
// ═════════════════════════════════════════════════════════════════════════════

// Full-update tick helper @ 0x822EBF88 — runs when m_bEnabled (+84) is set in
// the Update path, handles the heavy-weight state refresh and re-binding.
extern "C" void hudFlashBase_FullUpdateTick(void* pThis);

// Reload-guard release helper @ 0x823F9868 — called in TickRelease when the
// page-group is still bound; releases the transient render slot without
// publishing.
extern "C" void rage_renderSlot_release(void* pPageGroup);

// Page-group halfword-to-float scale used by the per-tick dispatcher
// (stored as 256.0f at 0x8202D9E0; dt = int16/256.0f).
static const float g_hudFlashBase_PageGroupScale = 256.0f;


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase::ScalarDtor (slot 1) @ 0x822EAFC8 | size: 0x8C
//
// Despite the RTTI-assigned "scalar_destructor" label this slot is actually
// the per-tick Flash scheduler entry point — it never frees storage. Each
// frame the scheduler calls this thunk, which:
//   1. Samples hudFlashBase_IsNetworkDirty() to decide whether a publish is
//      needed at the end of the tick.
//   2. Reads m_pPageGroup (+92); fetches the nested renderer sub-object at
//      pageGroup+24, then pulls a signed int16 at +44 of that sub-object —
//      this is the current scaled frame dt in 1/256-second units.
//   3. Converts the int16 to float dt (val / 256.0f) and virtual-dispatches
//      to Update (slot 3) on `this`.
//   4. If the network state was dirty on entry, tail-calls
//      hudFlashBase_PublishPageGroup() to republish the page group.
//
// The RTTI-assigned "scalar_destructor" name is inherited from the vtable
// slot-1 convention; kept for compatibility with the declared header method,
// but the body is really Update-driven scheduling.
// ─────────────────────────────────────────────────────────────────────────────
void hudFlashBase::ScalarDtor()
{
    uint8_t* self = (uint8_t*)this;

    // Sample dirty flag before any dispatch (B2A8 returns bool in r3).
    bool netDirty = hudFlashBase_IsNetworkDirty(this);

    // Page group pointer at +92 (0x5C).
    uint8_t* pageGroup = *(uint8_t**)(self + 92);

    // Renderer sub-object lives at pageGroup+24; scaled frame dt is a signed
    // int16 at +44 of that sub-object.
    uint8_t* renderSub = *(uint8_t**)(pageGroup + 24);
    int16_t  rawScaled = *(int16_t*)(renderSub + 44);
    float    dt        = (float)(int32_t)rawScaled / g_hudFlashBase_PageGroupScale;

    // Virtual dispatch to this->Update(dt). Slot 3 sits at byte offset +12
    // in the vtable. Preserves derived overrides that override Update.
    typedef void (*UpdateFn)(hudFlashBase*, float);
    UpdateFn pfnUpdate = *(UpdateFn*)(*(uint8_t**)self + 12);
    pfnUpdate(this, dt);

    // Publish only if the dirty flag was set on entry.
    if (netDirty) {
        hudFlashBase_PublishPageGroup(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase::Update (slot 3) @ 0x822EB058 | size: 0x8C
//
// Core per-tick update body dispatched by ScalarDtor above. Steps:
//   1. Sample hudFlashBase_IsNetworkDirty() up-front.
//   2. If m_bEnabled (uint8 at +84) is non-zero, run the heavy-weight
//      refresh helper at 0x822EBF88 (hudFlashBase_FullUpdateTick) — this
//      walks the page-group, rebinds entries, and propagates field writes.
//   3. Virtual-dispatch slot 1 (byte +4 into vtable) on m_pPageGroup (+92)
//      with (1, 0, dt) — this is the page-group's own per-frame tick, and
//      is the only slot-1 VCALL in the cluster against a page-group, not
//      against `this`.
//   4. Republish via hudFlashBase_PublishPageGroup() iff the initial dirty
//      flag was set (identical tail to ScalarDtor).
// ─────────────────────────────────────────────────────────────────────────────
void hudFlashBase::Update(float dt)
{
    uint8_t* self = (uint8_t*)this;

    bool netDirty = hudFlashBase_IsNetworkDirty(this);

    // m_bEnabled at +84 gates the full refresh path.
    if (*(uint8_t*)(self + 84) != 0) {
        hudFlashBase_FullUpdateTick(this);
    }

    // Page-group tick: virtual-call its slot 1 with (kind=1, slot=0, dt).
    void* pageGroup = *(void**)(self + 92);
    typedef void (*PageGroupTick)(void*, int, int, float);
    PageGroupTick pfn = *(PageGroupTick*)(*(uint8_t**)pageGroup + 4);
    pfn(pageGroup, 1, 0, dt);

    if (netDirty) {
        hudFlashBase_PublishPageGroup(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase::vfn_4 (TickRelease) @ 0x822EB0E8 | size: 0x60
//
// Lightweight companion to Update. Same dirty-check idiom:
//   1. Sample hudFlashBase_IsNetworkDirty().
//   2. If m_pPageGroup (+92) is bound, forward the bare pointer to the
//      render-slot release helper at 0x823F9868 (rage_renderSlot_release) —
//      this releases a transient renderer hold without publishing or
//      invoking the page-group's own tick.
//   3. Republish iff dirty on entry.
//
// Fires from the scheduler when the HUD is to be quiesced but the page
// binding is kept intact (e.g., between menu transitions).
// ─────────────────────────────────────────────────────────────────────────────
void hudFlashBase::vfn_4()
{
    uint8_t* self = (uint8_t*)this;

    bool netDirty = hudFlashBase_IsNetworkDirty(this);

    void* pageGroup = *(void**)(self + 92);
    if (pageGroup != nullptr) {
        rage_renderSlot_release(pageGroup);
    }

    if (netDirty) {
        hudFlashBase_PublishPageGroup(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase::OnExit (slot 6) @ 0x822EB148 | size: 0xA0
//
// Shared hide/close path. Reads the global SinglesNetworkClient singleton at
// 0x826064F4 (SDA r13+25844) to decide which branch to take:
//
//   fast-path (client present AND its byte at +104 is clear):
//     • Virtual-dispatch slot 7 (byte +28) on `this` — derived classes that
//       override slot 7 do their own on-exit work here. Base declares a
//       nullsub at slot 7 so the default is a no-op.
//     • Enqueue a draw-bucket entry at (this+4) keyed by client->+88
//       (pongDrawBucket_AddEntry @ 0x822278D8) — this schedules the final
//       hide-frame into the render pipeline.
//
//   slow-path (no client or client is in the "teardown" phase, byte +104
//   non-zero):
//     • Virtual-dispatch the real destructor (slot 0) on (this+4) — i.e.,
//       on the secondary MI sub-object. The second argument 0 suppresses
//       the scalar-delete free; the storage is owned elsewhere.
//
// The hudFlashBase_OnExit alias declared at the top of this file (used by
// hudPause::OnExit tail-calls) resolves to this body.
// ─────────────────────────────────────────────────────────────────────────────

// SinglesNetworkClient singleton pointer @ 0x826064F4 (SDA r13+25844).
// Null while the client is unloaded; non-null with byte +104 set during
// the teardown phase.
extern "C" void* g_pSinglesNetworkClient;

// pongDrawBucket_AddEntry @ 0x822278D8 — schedules a render-bucket entry.
extern "C" void pongDrawBucket_AddEntry(void* pEntry, void* pKey);

void hudFlashBase::OnExit()
{
    uint8_t* self = (uint8_t*)this;

    uint8_t* client = (uint8_t*)g_pSinglesNetworkClient;

    // Fast path: client present and not in teardown (byte +104 clear).
    if (client != nullptr && client[104] == 0) {
        // Dispatch derived slot 7 on `this` (default: nullsub_1).
        typedef void (*Slot7)(hudFlashBase*);
        Slot7 pfnSlot7 = *(Slot7*)(*(uint8_t**)self + 28);
        pfnSlot7(this);

        // Enqueue the hide frame into the draw bucket, keyed by client+88.
        void* drawKey = *(void**)(client + 88);
        pongDrawBucket_AddEntry(self + 4, drawKey);
        return;
    }

    // Slow path: destroy the secondary MI sub-object at (this+4) without
    // freeing (deleteFlag=0 — storage is owned by the outer object).
    uint8_t* subObj = self + 4;
    typedef void (*Slot0)(void*, uint32_t);
    Slot0 pfnDtor = *(Slot0*)(*(uint8_t**)subObj);
    pfnDtor(subObj, 0);
}


// C-linkage trampoline that resolves the forward declaration at line 26
// used by hudPause::OnExit (and any other non-member tail-call site).
extern "C" void hudFlashBase_OnExit(void* pThis)
{
    static_cast<hudFlashBase*>(pThis)->OnExit();
}


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase trivial base slots — all resolve to nullsub_1 in the binary:
//   raw slot 4 (post-TickRelease) | raw slot 6 (post-OnExit) | raw slot 7
// The hud.hpp declaration omits them because they carry no body; any
// derived class that appears to dispatch through these slots is actually
// dispatching through its own override (hudPause::OnExit, hudLoadingScreen
// tail slots, etc.). Left documented here so future lifters don't spend
// budget rediscovering this.
// ─────────────────────────────────────────────────────────────────────────────
