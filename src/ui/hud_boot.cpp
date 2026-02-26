/**
 * hud_boot.cpp — Boot-Sequence HUD Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * hudBoot manages the very first HUD shown on launch:
 *   1. Creates two Xbox system handles via the XAM layer.
 *   2. Calls the base-class virtual init (slot 1).
 *   3. Queries the game loop's current network mode.
 *   4. Configures several SinglesNetworkClient settings:
 *        - Online-mode flag key       (lbl_8205B2E8, 7 bytes)
 *        - "eyMsg" key                (lbl_8205E318, 6 bytes)
 *        - Short 4-char key           (lbl_8205DFB8, 5 bytes)
 *        - Tournament-name template   (lbl_8205E320, 12 bytes)
 *        - Tournament-name key        (lbl_8205E32C, 9 bytes)
 *   5. If the network client is already active, kicks off the connection.
 *
 * The string constants are raw .rdata pointers.  Their exact content is not
 * recoverable from the symbol table alone — the sizes are consistent with the
 * comments, and the functional context suggests Xbox Live lobby/session keys.
 *
 * Cross-references:
 *   hud.hpp          — class and struct layout
 *   pong_states.cpp  — SinglesNetworkClient calling conventions
 *   structured_pass5_final/recomp.15.cpp — hudBoot_vfn_0, hudBoot_vfn_5
 */

#include "hud.hpp"
#include <cstring>

// ── External declarations ─────────────────────────────────────────────────────

// rage base-class destructor called by hudBoot::~hudBoot.
// Sets vtable pointers back to hudBoot's vtables, frees m_pInnerObject,
// clears m_pNetworkClient, then calls rage_AD88 to restore the hudFlashBase
// vtable and finish base teardown.
// @ 0x823069D8
extern void hudBoot_BaseDtor(hudBoot* self);

// Rage memory allocator free.  @ 0x820C00C0
extern void rage_Free(void* ptr);

// XAM system call — creates an Xbox handle.
// Args: (0, 1, 0, 0) → returns the handle value.  @ 0x82566C88
extern uint32_t XAM_CreateHandle(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4);

// SinglesNetworkClient helpers — see pong_states.cpp for calling conventions.
extern uint8_t  SinglesNetworkClient_B2A8_g(void* pageGroup);   // poll button state
extern void*    SinglesNetworkClient_B1E8_g(hudBoot* self);      // get network client obj
extern void*    SinglesNetworkClient_9318_g(void* client,
                                             const char* key);   // look up setting by key
extern void     SinglesNetworkClient_8990_g(const char* src,
                                             char* dst,
                                             uint32_t size);     // string copy helper
extern void     SinglesNetworkClient_B320_g(hudBoot* self);      // kick off connection

// Global game-loop object.  @ 0x825EAB30 (r13-relative SDA global g_loop_obj_ptr)
extern void*    g_loop_obj_ptr;

// .rdata string keys used by SinglesNetworkClient_9318_g ─────────────────────
// Exact strings not recoverable from symbols.txt; sizes and context documented.
// @ 0x8205B2E8 (7 bytes = 6-char key + NUL)
// Likely an online/offline mode flag key (value set to 0 or 1 depending on
// whether g_loop_obj_ptr->field_0x200 == 1).
static const char* const k_bootOnlineModeKey =
    reinterpret_cast<const char*>(0x8205B2E8);

// @ 0x8205E318 (6 bytes = 5-char key + NUL).  resolve_address shows "eyMsg".
static const char* const k_bootEyMsgKey =
    reinterpret_cast<const char*>(0x8205E318);

// @ 0x8205DFB8 (5 bytes = 4-char key + NUL).  Matches k_SAVING in pong_states.cpp.
static const char* const k_bootSaveKey =
    reinterpret_cast<const char*>(0x8205DFB8);

// @ 0x8205E320 (12 bytes).  Source template for SinglesNetworkClient_8990_g
// copy into m_buffer; contents match a tournament-name pattern.
static const char* const k_bootNameTemplate =
    reinterpret_cast<const char*>(0x8205E320);

// @ 0x8205E32C (9 bytes = 8-char key + NUL).  Key used to register m_buffer
// with the network client as a string-typed setting (type = 1).
static const char* const k_bootNameKey =
    reinterpret_cast<const char*>(0x8205E32C);

// ── hudBoot::~hudBoot @ 0x82306988 | size: 0x50 ───────────────────────────────
/**
 * Destructor.  Delegates main cleanup to hudBoot_BaseDtor which:
 *   - Resets vtable pointers to hudBoot's own vtables (0x8205E444 / 0x8205E46C).
 *   - Frees m_pInnerObject and clears m_pNetworkClient.
 *   - Chains to rage_AD88 which restores hudFlashBase's vtable and releases
 *     the remaining two resource pointers at +0x38 / +0x3C.
 * If shouldFree is set the object's memory is then freed via rage_Free.
 */
hudBoot::~hudBoot() {
    hudBoot_BaseDtor(this);
    // If this is a scalar delete (bit 0 of the flags parameter is set),
    // the caller's thunk (ScalarDtor, slot 1) handles freeing via rage_Free.
    // The raw destructor (slot 0) does not free; that is left to the caller.
}

// ── hudBoot::OnEnter @ 0x82306A50 | size: 0x164 ───────────────────────────────
/**
 * OnEnter — called when the boot HUD is shown for the first time.
 *
 * 1. Allocates two Xbox handles via XAM_CreateHandle and stores them.
 * 2. Calls vtable slot 1 (ScalarDtor / base Init) on self — this is the
 *    rage base-class virtual Init, repurposed here for boot setup.
 * 3. Clears m_bInitialized.
 * 4. Polls the SinglesNetworkClient to see if it is already connected.
 * 5. Reads the network mode from g_loop_obj_ptr->field_0x200:
 *      == 1 → online (Xbox Live) mode
 *      != 1 → offline / system-link mode
 * 6. Looks up and configures five named settings in the network client.
 * 7. If the client was already active, triggers connection via B320.
 */
void hudBoot::OnEnter() {
    // Step 1: Acquire two Xbox system handles.
    m_xboxHandle1 = XAM_CreateHandle(0, 1, 0, 0);
    m_xboxHandle2 = XAM_CreateHandle(0, 1, 0, 0);

    // Step 2: Call base-class virtual Init (vtable slot 1) on self.
    // TODO: confirm exactly which base-class method slot 1 maps to.
    ScalarDtor();   // placeholder — see slot 1 in hud.hpp

    // Step 3: Clear initialised flag.
    m_bInitialized = 0;

    // Step 4: Poll SinglesNetworkClient active state.
    uint8_t isNetworkActive = SinglesNetworkClient_B2A8_g(this);

    // Step 5: Get the network-client object and check the current mode.
    void* networkClient = SinglesNetworkClient_B1E8_g(this);

    // g_loop_obj_ptr->field_0x200: 1 = online/Xbox Live, other = offline.
    struct GameLoop { uint8_t _pad[0x200]; uint32_t networkMode; };
    GameLoop* loop = static_cast<GameLoop*>(g_loop_obj_ptr);
    bool isOnline = (loop->networkMode == 1);

    // Step 6: Configure SinglesNetworkClient settings.

    // Setting A — online-mode flag (bool stored as 0/1).
    // cntlzw+rlwinm PPC idiom: isOnline → 0, offline → 1 (inverted).
    void* settingOnline = SinglesNetworkClient_9318_g(networkClient, k_bootOnlineModeKey);
    if (settingOnline != nullptr) {
        *reinterpret_cast<uint32_t*>(settingOnline)           = isOnline ? 0u : 1u;
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(settingOnline) + 4)    = 3u;  // type = bool
    }

    // Setting B — "eyMsg" key, initialised to 0 / type 3.
    void* settingEyMsg = SinglesNetworkClient_9318_g(networkClient, k_bootEyMsgKey);
    if (settingEyMsg != nullptr) {
        *reinterpret_cast<uint32_t*>(settingEyMsg)            = 0u;
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(settingEyMsg) + 4)     = 3u;
    }

    // Setting C — save/short key, initialised to 0 / type 3.
    void* settingSave = SinglesNetworkClient_9318_g(networkClient, k_bootSaveKey);
    if (settingSave != nullptr) {
        *reinterpret_cast<uint32_t*>(settingSave)             = 0u;
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(settingSave) + 4)      = 3u;
    }

    // Step 6d: Copy the name template into m_buffer.
    // m_buffer sits at +0x70 (112 bytes into the object).
    char* buf = m_buffer;
    SinglesNetworkClient_8990_g(k_bootNameTemplate, buf, sizeof(m_buffer));

    // Setting D — register m_buffer with the network client as a string (type 1).
    void* settingName = SinglesNetworkClient_9318_g(networkClient, k_bootNameKey);
    if (settingName != nullptr) {
        *reinterpret_cast<void**>(settingName)                 = buf;
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(settingName) + 4)      = 1u;  // type = string ptr
    }

    // Step 7: If already connected, kick off session negotiation.
    if (isNetworkActive != 0) {
        SinglesNetworkClient_B320_g(this);
    }
}

