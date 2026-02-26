/**
 * pong_states.cpp — Credits screen state/context + data classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Covers 4 classes:
 *   creditsData        — serialisable data container for credits roll assets
 *   creditsSettings    — serialisable settings for credits display params
 *   pongCreditsContext — HSM context managing the credits UI page group
 *   pongCreditsState   — HSM leaf state that drives the credits sequence
 *
 * Architecture:
 *   creditsData / creditsSettings are pure data objects loaded from disk
 *   (fiAscii / rage::dataSingleton style).  They live as singletons and
 *   expose IsSupported(assetId) and RegisterFields() virtuals common to
 *   all Rockstar data assets.
 *
 *   pongCreditsContext is a rage::hsmContext subclass (multiple inheritance —
 *   it embeds a secondary base vtable at +0x14).  It owns the UI page-group
 *   pointer and an "active" flag.
 *
 *   pongCreditsState inherits from pongAttractState and holds a back-pointer
 *   to its context.  The state machine calls Enter/Exit/GetContext in the
 *   usual HSM pattern.
 */

#include "pong_states.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// External globals / singletons (resolved from SDA / .data)
// ─────────────────────────────────────────────────────────────────────────────

// g_hsmMgr        — the game's central HSM manager (resolved @ 0x8271A81C)
// g_creditsRoll   — credits-roll singleton (resolved @ 0x8271A358)
// g_gameSettings  — global game settings object (resolved @ 0x8271A320)

// Byte flag: credits roll is currently visible (@ 0x8260660E, SDA-26126)
extern uint8_t g_creditsRollVisible;   // @ 0x8260660E

// ─────────────────────────────────────────────────────────────────────────────
// creditsData  [vtable @ 0x820766EC]
// ─────────────────────────────────────────────────────────────────────────────

/**
 * creditsData::~creditsData  @ 0x8240C0B8  |  size: 0x68
 *
 * Destructor.  If this object has allocated a bone/entry array
 * (tracked via the count at +0x26 and pointer at +0x20) it frees it,
 * then calls up to the atSingleton destructor and conditionally
 * frees the object itself.
 */
creditsData::~creditsData() {
    // If we allocated an entry array, free it
    if (m_entryCount != 0) {
        rage_free(m_pEntries);
    }
    // Let the singleton base do its teardown
    atSingleton_9420(this);

    // Note: conditional self-free (shouldFree flag from caller) is
    // handled by the generated thunk; this body covers the non-freeing path.
}

/**
 * creditsData::IsSupported  @ 0x8240C070  |  size: 0x48
 *
 * Returns true if assetId matches any of the three asset-type IDs
 * registered for creditsData (checked against two global singletons).
 * Slot 20 in the vtable — used by the asset manager to route loads.
 */
bool creditsData::IsSupported(uint32_t assetId) const {
    // First global singleton ID check
    extern uint32_t g_creditsAssetId_A;   // @ lbl resolved from -32164+11188
    if (assetId == g_creditsAssetId_A) {
        return true;
    }
    // Second global ID check
    extern uint32_t g_creditsAssetId_B;   // @ lbl resolved from -32163-32708
    if (assetId == g_creditsAssetId_B) {
        return true;
    }
    // Third global ID check
    extern uint32_t g_creditsAssetId_C;   // @ lbl resolved from -32163-32712
    return (assetId == g_creditsAssetId_C);
}

/**
 * creditsData::RegisterFields  @ 0x8240C120  |  size: 0x64
 *
 * Registers the two serialisable fields of this data object with
 * the rage data system.  Slot 21 — called during asset load/bind.
 * Registers: m_pEntries (array at +24) and m_strName (string at +28).
 */
void creditsData::RegisterFields() {
    // Register m_pEntries array field
    extern const void* g_creditsEntriesFieldDesc;   // @ lbl_825CAF00 area
    extern const void* g_creditsEntriesSchema;      // @ lbl_82079E18 area
    game_8F58(this, &g_creditsEntriesSchema, &m_pEntries, &g_creditsEntriesFieldDesc, 0);

    // Register m_strName string field
    extern const void* g_creditsNameFieldDesc;
    extern const void* g_creditsNameSchema;
    game_8F58(this, &g_creditsNameSchema, &m_strName, &g_creditsNameFieldDesc, 0);
}

/**
 * creditsData::BuildActiveList  @ 0x8240C188  |  size: 0x134
 *
 * Slot 3.  Iterates the linked list rooted at m_pNodeList (+12),
 * counts nodes whose IsSupported check passes (slot 20 on each node),
 * allocates a pointer array for them, then fills it in a second pass.
 * The result is stored in m_pActiveEntries (+32) / m_activeCount (+38).
 *
 * Nodes that fail the filter are logged via nop_8240E6D0 with the
 * "BoneName" key @ 0x8203F2E0.
 */
void creditsData::BuildActiveList() {
    uint32_t validCount = 0;
    extern uint32_t g_creditsAssetId_A;   // used as the filter arg

    // First pass: count valid nodes
    void* node = m_pNodeList;
    while (node != nullptr) {
        bool supported = VCALL_slot20(node, g_creditsAssetId_A);
        if (supported) {
            ++validCount;
        } else {
            // Log BoneName mismatch
            const char* key = "BoneName";   // @ 0x8203F2E0
            const char* self_name = VCALL_slot19(node);
            const char* our_name  = VCALL_slot19(this);
            nop_8240E6D0(key, our_name, self_name);
        }
        node = *(void**)((uint8_t*)node + 8);   // node->m_pNext
    }

    // Allocate active-entry array (or nullptr if empty)
    void** activeArray = nullptr;
    if (validCount > 0) {
        uint32_t allocSize = validCount * sizeof(void*);
        activeArray = (void**)xe_EC88(allocSize);
    }
    m_pActiveEntries = activeArray;
    m_activeCount    = (uint16_t)validCount;

    // Second pass: fill active-entry array
    uint16_t insertIdx = 0;
    node = m_pNodeList;
    while (node != nullptr) {
        bool supported = VCALL_slot20(node, g_creditsAssetId_A);
        if (supported) {
            activeArray[insertIdx] = node;
            ++insertIdx;
        }
        node = *(void**)((uint8_t*)node + 8);
    }
}

/**
 * creditsData::GetKey  @ 0x8231F358  |  size: 0xC
 *
 * Slot 22.  Returns the static key string used to identify this
 * data class in the asset registry.  (Parallel to GetName/GetKey
 * pattern used across all data singleton classes.)
 */
const char* creditsData::GetKey() const {
    // Returns pointer to static string in .rdata @ 0x82062378
    static const char* k_key = (const char*)0x82062378;
    return k_key;
}

// ─────────────────────────────────────────────────────────────────────────────
// creditsSettings  [vtable @ 0x82076754]
// ─────────────────────────────────────────────────────────────────────────────

/**
 * creditsSettings::IsSupported  @ 0x8240C2C0  |  size: 0x48
 *
 * Slot 20.  Same three-ID check pattern as creditsData::IsSupported,
 * using a different primary ID pulled from the settings singleton table.
 */
bool creditsSettings::IsSupported(uint32_t assetId) const {
    extern uint32_t g_settingsAssetId_A;   // @ lbl resolved -32164+11192
    if (assetId == g_settingsAssetId_A) {
        return true;
    }
    extern uint32_t g_settingsAssetId_B;   // @ lbl resolved -32163-32708
    if (assetId == g_settingsAssetId_B) {
        return true;
    }
    extern uint32_t g_settingsAssetId_C;   // @ lbl resolved -32163-32712
    return (assetId == g_settingsAssetId_C);
}

/**
 * creditsSettings::RegisterFields  @ 0x8240C308  |  size: 0x118
 *
 * Slot 21.  Registers 8 serialisable float/value fields of the credits
 * display settings (scroll speed, timing params, colour values, etc.)
 * plus one string field — all laid out sequentially from +16 to +44.
 *
 * The schema descriptor is a shared one (@ lbl_825CAF94).
 */
void creditsSettings::RegisterFields() {
    extern const void* g_creditsSettingsSchema;   // @ lbl_825CAF94

    // Fields are registered at offsets +16 through +44 (8 float fields + 1 string)
    // game_8F58 signature: (obj, schema, fieldPtr, desc, flags)
    extern const char* k_scrollSpeed;   // 25936
    extern const char* k_fadeInTime;    // 25948
    extern const char* k_fadeOutTime;   // 25964
    extern const char* k_holdTime;      // 25980
    extern const char* k_textScale;     // 25996
    extern const char* k_lineSpacing;   // 26008
    extern const char* k_pageSpacing;   // 26016
    extern const char* k_colourField;   // 26032 (with different schema @ -20592)

    game_8F58(this, k_scrollSpeed,  (uint8_t*)this + 16, &g_creditsSettingsSchema, 0);
    game_8F58(this, k_fadeInTime,   (uint8_t*)this + 20, &g_creditsSettingsSchema, 0);
    game_8F58(this, k_fadeOutTime,  (uint8_t*)this + 24, &g_creditsSettingsSchema, 0);
    game_8F58(this, k_holdTime,     (uint8_t*)this + 28, &g_creditsSettingsSchema, 0);
    game_8F58(this, k_textScale,    (uint8_t*)this + 32, &g_creditsSettingsSchema, 0);
    game_8F58(this, k_lineSpacing,  (uint8_t*)this + 36, &g_creditsSettingsSchema, 0);
    game_8F58(this, k_pageSpacing,  (uint8_t*)this + 40, &g_creditsSettingsSchema, 0);

    extern const void* g_creditsColourSchema;   // @ lbl_825CAF9C (-20592)
    game_8F58(this, k_colourField,  (uint8_t*)this + 44, &g_creditsColourSchema,   0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCreditsContext  [vtable @ 0x8205EE04 / secondary @ 0x8205EE6C]
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongCreditsContext::~pongCreditsContext  @ 0x82309C38  |  size: 0x68
 *
 * Destructor.  Resets both vtable pointers (primary and the embedded
 * secondary-base vtable at +0x14) to their base-class values, clears
 * the page-group pointer, then conditionally frees via rage_free.
 *
 * The secondary vtable at this+0x14 belongs to the embedded MI base
 * (the inner hsmContext object); its pointer is reset to the base
 * class's thunk table @ lbl_8202.... before the primary vtable is
 * written.
 */
pongCreditsContext::~pongCreditsContext(bool shouldFree) {
    // Restore secondary (MI-base) vtable @ this+0x14
    // vtable ptr = lbl computed from lis -32254 + 31540 (0x82027B34 area)
    *(void**)((uint8_t*)this + 20) = (void*)g_pongCreditsContext_secondaryVtable;
    // Clear page-group pointer @ this+0x18
    *(uint32_t*)((uint8_t*)this + 24) = 0;
    // Restore primary vtable @ this+0x00
    // vtable ptr = lbl computed from lis -32254 + 30404 (0x820276C4 area)
    *(void**)this = (void*)g_pongCreditsContext_baseVtable;

    if (shouldFree) {
        rage_free(this);
    }
}

/**
 * pongCreditsContext_rtti_EE6C_0  @ 0x82309CA0
 *
 * MI-base thunk destructor.  The secondary vtable slot 0 adjusts `this`
 * by -20 bytes to recover the true `this` pointer before delegating to
 * the primary destructor.  This is the standard Itanium ABI "in-charge"
 * destructor thunk for a non-primary base.
 */
static void pongCreditsContext_secondaryBase_dtor(pongCreditsContext_SecondaryBase* base, bool shouldFree) {
    // Adjust back to primary this pointer (base is at this+20)
    pongCreditsContext* self = (pongCreditsContext*)((uint8_t*)base - 20);
    self->~pongCreditsContext(shouldFree);
}

/**
 * pongCreditsContext::CanTransition  @ 0x82309D38  |  size: 0x28
 *
 * Slot 11.  Checks whether the credits context is ready to allow
 * an HSM state transition.  If the page group exists, notifies
 * atSingleton (likely clearing a pending request flag), then
 * always returns true (credits context never blocks transitions).
 */
bool pongCreditsContext::CanTransition() {
    if (m_pPageGroup != nullptr) {
        atSingleton_F728_g(m_pPageGroup);
    }
    return true;
}

/**
 * pongCreditsContext::OnEnterCredits  @ 0x82309E18  |  size: 0x120
 *
 * Slot 16.  Called when the HSM transitions into the credits state.
 * Checks whether we are NOT on the main thread (via HSM manager flag
 * at +495 and game settings flag at +24), then if so:
 *   1. Notifies the page group to start its presentation (slot 2 call).
 *   2. Decides whether to play a background movie or just transition
 *      to state 6 (the credits roll) via hsmContext_SetNextState.
 *   3. Queues a util_94B8 call with the current context data.
 *
 * If no page group or not active, falls through silently.
 */
void pongCreditsContext::OnEnterCredits() {
    // Determine if we are off the main thread
    extern uint8_t g_hsmMgr_flags[];   // @ lbl_8271A81C area, byte +495
    extern void*   g_gameSettings;     // @ lbl_8271A740 area, byte +24
    bool offMainThread = (g_hsmMgr_flags[495] != 0);
    if (!offMainThread) {
        bool gameSettingsActive = (*(uint8_t*)((uint8_t*)g_gameSettings + 24) != 0);
        offMainThread = gameSettingsActive;
    }

    if (!offMainThread) {
        // Main-thread path: nothing to do
        return;
    }

    if (m_pPageGroup == nullptr) {
        return;
    }

    // Notify page group to open/present
    pg_0890_g(m_pPageGroup);
    m_pPageGroup->Open();   // vtable slot 2

    // Check page group active state (+152) and m_bActive (+28)
    bool pageActive = (*(uint8_t*)((uint8_t*)m_pPageGroup + 152) != 0);
    bool selfActive = (m_bActive != 0);
    if (!pageActive && !selfActive) {
        return;
    }

    // Transition to state 6 (credits roll)
    extern void* g_hsmContextPtr;   // @ lbl_8271A81C
    hsmContext_SetNextState_2800(g_hsmContextPtr, 6 /*STATE_CREDITS_ROLL*/);

    // Check if the global movie record has valid frame range
    extern void* g_movieRecord;   // @ lbl_8271A320
    bool hasValidRange = !((*(int32_t*)((uint8_t*)g_movieRecord + 16) == -1) &&
                           (*(int32_t*)((uint8_t*)g_movieRecord + 20) == -1));
    if (!hasValidRange) {
        return;
    }

    // Queue credits context data
    extern void* g_creditsContextData;   // @ lbl_8258C000 area (-24384)
    util_94B8(/*r4=*/2, /*r5=*/1, /*r6=*/g_creditsContextData, /*r7=*/0,
              g_creditsContextData, g_creditsContextData);
}

/**
 * pongCreditsContext::OnExitCredits  @ 0x82309F38  |  size: 0x88
 *
 * Slot 18.  Called when leaving the credits HSM context.
 * Checks the same main-thread gate, then if off-thread and page group
 * is valid, calls pg_6000_g to dismiss the UI and calls slot 6 on
 * the page group (Close/Hide).
 */
void pongCreditsContext::OnExitCredits() {
    extern uint8_t g_hsmMgr_flags[];
    extern void*   g_gameSettings;
    bool offMainThread = (g_hsmMgr_flags[495] != 0);
    if (!offMainThread) {
        bool gameSettingsActive = (*(uint8_t*)((uint8_t*)g_gameSettings + 24) != 0);
        offMainThread = gameSettingsActive;
    }

    if (!offMainThread) {
        return;
    }

    // Dismiss the UI via the global UI manager
    extern void* g_uiManagerPtr;   // @ lbl_8271A81C
    pg_6000_g(g_uiManagerPtr);

    if (m_pPageGroup == nullptr) {
        return;
    }

    m_pPageGroup->Close();   // vtable slot 6
}

/**
 * pongCreditsContext::RegisterWithCreditsRoll  @ 0x82309CA0  |  size: 0x94
 *
 * Slot 23.  Allocates and initialises the UI page group for the credits roll,
 * then registers this context with the credits roll manager.
 *
 * Steps:
 *   1. Calls xe_main_thread_init_0038 to assert we're on the main thread.
 *   2. Allocates a 220-byte page group object from the main thread allocator
 *      (via vtable slot 1 of the SDA-resident allocator table).
 *   3. If allocation succeeds, calls game_F4C0 to construct the page group.
 *   4. Stores the result in m_pPageGroup (this+24).
 *   5. Calls rage_ADF8 to bind this context to the credits roll system,
 *      passing the credits roll name string and the category tag 202.
 *
 * Debug note: the strings "cfail" / "yreadyfail" are debug log tags
 * passed to the no-op logger (nop_8240E6D0) as bookends; they are
 * harmless in the shipping build.
 */
void pongCreditsContext::RegisterWithCreditsRoll() {
    // Entry debug bookend: "cfail" @ 0x8205ED64
    // nop_8240E6D0("cfail");   // stripped in release

    // Assert main thread is ready
    xe_main_thread_init_0038();

    // Allocate a UI page group (220 bytes, 16-byte aligned).
    // The main allocator table is at SDA offset 0 (base 0x82600000);
    // index [1] (byte offset 4) holds the active allocator pointer.
    // vtable slot 1 on that allocator is Alloc(size, align).
    extern uint32_t g_mainAllocTable;      // @ SDA offset 0 = 0x82600000
    uint32_t* allocTableBase = (uint32_t*)g_mainAllocTable;
    void*     allocator      = (void*)allocTableBase[1];  // [4 bytes in]
    void* pageGroupMem = VCALL_ALLOC(allocator, /*size=*/220, /*align=*/16);

    if (pageGroupMem != nullptr) {
        // Construct the page group object
        pageGroupMem = game_F4C0(pageGroupMem);
    } else {
        pageGroupMem = nullptr;
    }
    m_pPageGroup = (pongPageGroup*)pageGroupMem;

    // Register with the credits-roll manager.
    // g_pCreditsRollObj is a global pointer (SDA offset 25888 @ lbl_82606520)
    // that points to the live credits-roll singleton object; the name string
    // for this context lives at offset +50 within that object.
    extern void* g_pCreditsRollObj;   // @ lbl_82606520 [.data sz:4, SDA +25888]
    const char* nameStr = (const char*)((uint8_t*)g_pCreditsRollObj + 50);
    rage_ADF8(this, /*category=*/202, nameStr);

    // Exit debug bookend: "yreadyfail" @ 0x8205ED80
    // nop_8240E6D0("yreadyfail");   // stripped in release
}




// ─────────────────────────────────────────────────────────────────────────────
// pongCreditsState  [vtable @ 0x8205EE7C]
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongCreditsState::~pongCreditsState  @ 0x8230A000  |  size: 0x60
 *
 * Destructor.  Resets vtable, delegates to pongAttractState base-class
 * cleanup (vfn_8 = the shared attract state teardown), then restores
 * the pongCreditsState vtable before the conditional free.
 */
pongCreditsState::~pongCreditsState(bool shouldFree) {
    // Reset to pongCreditsState vtable (then base restores its own)
    *(void**)this = (void*)g_pongCreditsState_vtable;   // @ 0x8205EE7C

    // Delegate attract-state common cleanup
    pongAttractState_vfn_8(this);

    // Re-install pongCreditsState vtable for destructor finish
    // (base class restored theirs; we re-apply ours, then pongAttractState base)
    *(void**)this = (void*)g_pongAttractState_baseVtable;  // @ 0x820276C4 area

    if (shouldFree) {
        rage_free(this);
    }
}

/**
 * pongCreditsState::Init  @ 0x8230A068  |  size: 0x108
 *
 * Slot 14.  Initialises the credits state:
 *   1. Calls the main-thread init stub (xe_main_thread_init_0038).
 *   2. Allocates a new pongCreditsContext via the thread's allocator
 *      (vtable slot 1, requesting 32 bytes at alignment 16).
 *   3. Populates the new context object: sets vtable pointers, zeroes
 *      fields, stores it in m_pContext.
 *   4. Registers the context with the credits-roll manager (slot 23).
 */
void pongCreditsState::Init() {
    xe_main_thread_init_0038();

    // Allocate pongCreditsContext through the thread allocator
    // (SDA[0] is the main allocator table; slot 1 = Alloc(size, align))
    extern void* g_mainAllocTable;   // SDA @ 0x82600000 offset 0
    void* (*allocFn)(void*, uint32_t, uint32_t) = VTABLE_SLOT1(g_mainAllocTable);
    pongCreditsContext* ctx = (pongCreditsContext*)allocFn(g_mainAllocTable, 32, 16);

    if (ctx != nullptr) {
        // Initialise context fields
        *(void**)ctx                               = (void*)g_pongCreditsContext_primaryVtable;   // @ lbl_8205EE04
        *(uint32_t*)((uint8_t*)ctx + 4)            = 0;
        *(uint32_t*)((uint8_t*)ctx + 8)            = 0;
        *(uint32_t*)((uint8_t*)ctx + 12)           = 0;
        *(uint32_t*)((uint8_t*)ctx + 16)           = 0;
        *(void**)((uint8_t*)ctx + 20)              = (void*)g_pongCreditsContext_secondaryVtable; // @ lbl_8205EE6C
        *(uint32_t*)((uint8_t*)ctx + 24)           = 0;  // m_pPageGroup = nullptr
        *(uint8_t*)((uint8_t*)ctx + 28)            = 0;  // m_bActive = false

        // Build the back-pointer entry and register in the attract state arrays
        // (the scaffold's complex r9/r30 code sets up indexed slot storage)
        extern uint32_t g_attractStateCount;         // @ lbl_8261+14436
        uint32_t slotIdx = g_attractStateCount;
        extern uint8_t* g_attractStateFlags;         // @ lbl_8261+14952
        extern uint32_t* g_attractStateSizes;        // @ lbl_8261+14696
        extern pongCreditsContext** g_attractStateCtxPtrs; // @ lbl_8261+14440

        g_attractStateFlags[slotIdx]       = 128;
        g_attractStateSizes[slotIdx]       = 16;
        g_attractStateCtxPtrs[slotIdx]     = ctx;
        g_attractStateCount                = slotIdx + 1;
    } else {
        ctx = nullptr;
    }

    m_pContext = ctx;

    // Register context with credits-roll manager (slot 23 on ctx)
    if (ctx != nullptr) {
        ctx->RegisterWithCreditsRoll();   // vtable slot 23
    }
}

/**
 * pongCreditsState::GetContext  @ 0x82309C28  |  size: 0xC
 *
 * Slot 13.  Returns the static state-descriptor/context-ID for this
 * state type.  Used by the HSM to identify the state kind.
 * Equivalent to the RTTI-based GetStateClass() pattern.
 */
void* pongCreditsState::GetContext() {
    // Returns address of the static state-descriptor @ 0x8205EDB0
    // (embedded within the rdata block starting at lbl_8205ED98)
    return (void*)0x8205EDB0;
}

/**
 * pongCreditsState::OnEnter  @ 0x8230A150  |  size: 0x118
 *
 * Slot 11.  State entry handler.  Called by the HSM with the previous
 * state index.  Actions depend on which state we came from:
 *
 *   prevState == 6  → Coming from attract: clear the roll-visible flag,
 *                     set credits-roll mode to 2, store context ptr.
 *   prevState == 9  → Coming from network screen: set roll-visible flag,
 *                     enable the HSM manager's overlay (byte +576), bump
 *                     the credits-roll entry counter, notify the manager.
 *   prevState == 12 → Immediate return (transient pass-through state).
 *   other           → Post a transition request via game_28B8 / nop.
 *
 * Also enables the page group's auto-advance (+85) if present.
 */
void pongCreditsState::OnEnter(int prevStateIdx) {
    pg_E6E0(2053 /*CREDITS_ENTER_MSG*/, 64, 0, 0);

    // Enable auto-advance on the page group if it exists
    if (m_pContext != nullptr && m_pContext->m_pPageGroup != nullptr) {
        *(uint8_t*)((uint8_t*)m_pContext->m_pPageGroup + 85) = 1;
    }

    if (prevStateIdx == 12) {
        // Pass-through: nothing to do
        return;
    }

    if (prevStateIdx == 9) {
        // Coming from network screen — start visible credits roll
        extern void* g_creditsRoll;           // @ lbl_8271A358
        g_creditsRollVisible = 1;

        extern void* g_hsmMgr;               // @ lbl resolved from SDA
        *(uint8_t*)((uint8_t*)g_hsmMgr + 576) = 1;   // enable overlay

        // Add this context to the roll's entry list
        uint32_t newCount = *(uint32_t*)((uint8_t*)g_creditsRoll + 48) + 1;
        *(uint32_t*)((uint8_t*)g_creditsRoll + 48) = newCount;
        uint32_t* slotArray = *(uint32_t**)((uint8_t*)g_creditsRoll + 44);
        slotArray[newCount - 1] = (uint32_t)(uintptr_t)m_pContext;

        // Notify roll of the new count
        game_AD40(g_creditsRoll, newCount);

        pongCreditsState_9D68_h(m_pContext, 0 /*not visible*/);
        return;
    }

    if (prevStateIdx == 6) {
        // Coming from attract: deactivate roll, set mode 2, store context
        g_creditsRollVisible = 0;
        extern void* g_creditsRoll;
        *(uint32_t*)((uint8_t*)g_creditsRoll + 52) = 2;    // mode = 2
        *(uint32_t*)((uint8_t*)g_creditsRoll + 56) = (uint32_t)(uintptr_t)m_pContext;

        pongCreditsState_9D68_h(m_pContext, 0);
        return;
    }

    // Generic previous-state: post a transition request
    void* transReq = game_28B8(m_pHSMContext, prevStateIdx);
    // Notify credits-roll of transition (address of static table entry)
    extern void* g_creditsTransTable;   // @ lbl_8205F2D4 - 4712 area
    nop_8240E6D0(g_creditsTransTable, transReq, prevStateIdx);
}

/**
 * pongCreditsState::OnExit  @ 0x8230A268  |  size: 0x90
 *
 * Slot 12.  State exit handler.  Called with the next state index.
 *
 *   nextState == 6  → Set credits-roll mode to 3 (leaving via attract).
 *   nextState == 12 → Immediate return.
 *   other           → Post transition via game_28B8 / nop.
 *
 * Disables page group auto-advance on exit.
 */
void pongCreditsState::OnExit(int nextStateIdx) {
    pg_E6E0(2054 /*CREDITS_EXIT_MSG*/, 64, 0, 0);

    // Disable auto-advance on the page group
    if (m_pContext != nullptr && m_pContext->m_pPageGroup != nullptr) {
        *(uint8_t*)((uint8_t*)m_pContext->m_pPageGroup + 85) = 0;
    }

    if (nextStateIdx == 6) {
        // Leaving to attract: mode 3
        extern void* g_creditsRoll;
        *(uint32_t*)((uint8_t*)g_creditsRoll + 52) = 3;
        return;
    }

    if (nextStateIdx == 12) {
        return;
    }

    // Generic next-state: post a transition request
    void* transReq = game_28B8(m_pHSMContext, nextStateIdx);
    extern void* g_creditsExitTable;   // @ lbl_8205F2D4 - 4660 area
    nop_8240E6D0(g_creditsExitTable, transReq, nextStateIdx);
}

/**
 * pongCreditsState_9D68_h  @ 0x82309D68  |  size: ~0x80
 *
 * Helper shared between pongCreditsContext::CanTransition and
 * pongCreditsState::OnEnter.
 *
 * Sets the credits-context display mode:
 *   If visible == 0: calls pg_61E8_g on the UI manager with fade params,
 *                    fading from 2.0f over 2 seconds.
 * In both cases, if the page group is valid, writes visible to its
 * state byte (+155) and calls slot 5 (SetVisible/Refresh), then
 * clears the m_bActive flag on the credits state.
 */
void pongCreditsState_9D68_h(pongCreditsContext* ctx, uint8_t visible) {
    if (visible == 0) {
        extern void* g_uiManagerPtr;   // @ lbl_8271A81C
        extern float g_fadeTime;       // @ lbl_8253C000 - 12016 (e.g. 2.0f)
        pg_61E8_g(g_uiManagerPtr, 2 /*type*/, 1 /*mode*/, g_fadeTime, 0.0f);
    }

    if (ctx->m_pPageGroup != nullptr) {
        *(uint8_t*)((uint8_t*)ctx->m_pPageGroup + 155) = visible;
        ctx->m_pPageGroup->SetVisible();   // vtable slot 5

        // Clear m_bActive on the outer state (the caller, accessed via ctx+28
        // in the scaffold — this is the context's own m_bActive flag)
        ctx->m_bActive = 0;
    } else {
        ctx->m_bActive = 0;
    }
}
