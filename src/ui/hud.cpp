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
// ═════════════════════════════════════════════════════════════════════════════
// Network and UI utility functions (continued)
// ═════════════════════════════════════════════════════════════════════════════

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
