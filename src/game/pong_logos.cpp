/**
 * pong_logos.cpp — Boot Logos State & Context Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the boot logos sequence state machine:
 * - pongLogosState:   HSM leaf state that drives the logo display sequence
 * - pongLogosContext: HSM context that manages the logos UI page group
 *
 * This is the first screen shown after boot, displaying the Rockstar
 * and Take-Two logos before transitioning to the legals / main menu.
 *
 * Architecture:
 *   pongLogosState inherits from pongAttractState and holds an
 *   m_pHSMContext (rage::hsmContext*) at +0x04 and a pongLogosContext*
 *   at +0x08. The context owns a hudLogosScreen (multiple-inheritance,
 *   two vtables) that handles the actual page-group rendering.
 *
 * Source functions: 7 total (4 state + 3 context)
 */

#include "match/pong_states.hpp"
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// External function declarations
// ─────────────────────────────────────────────────────────────────────────────

// Thread / allocator init
extern void xe_main_thread_init_0038();                     // @ 0x820C0038 — ensure TLS initialised

// HSM helpers
extern const char* hsmContext_GetStateName(void* hsmCtx, uint32_t stateIdx);
                                                            // @ 0x822228B8 — looks up state[idx]->GetName()
extern void hsmContext_SetNextState(void* hsmCtx, uint32_t nextStateIdx);
                                                            // @ 0x82222800 — queue HSM transition

// Page-group / grcDevice helpers
extern uint8_t grcPageGroup_IsComplete(void* grcDevice);    // @ 0x823060D8 — returns non-zero while still showing
extern void    grcPageGroup_Render(void* grcDevice);        // @ 0x82306000 — render the current page group
extern void    grcPageGroup_SetFade(void* grcDevice, float fadeValue,
                                    uint32_t fadeMode, uint32_t p3, uint32_t p4);
                                                            // @ 0x823061E8 — fade in/out
extern void    grcDevice_BeginScene(void* grcDevice);       // @ 0x82305E78

// UI manager helpers
extern void    game_AAF8(void* uiMgr, uint32_t screenId, uint32_t contextPtr);
                                                            // @ 0x8222AAF8 — pop/clear active screen context

// hudLogosScreen constructor
extern void    hudLogosScreen_Init(void* mem);              // @ 0x822EAC88 — base init for logos screen object

// Logging (no-op in release builds)
extern void    nop_8240E6D0(const char* fmt, ...);          // @ 0x8240E6D0

// ─────────────────────────────────────────────────────────────────────────────
// Global pointers (resolved from .data / SDA)
// ─────────────────────────────────────────────────────────────────────────────

extern void*   g_grcDevice;        // @ 0x8271A81C — graphics device singleton
extern void*   g_uiManager;        // @ 0x8271A358 — UI manager singleton
extern void*   g_loop_obj_ptr;     // @ 0x825EAB30 — game loop object

// SDA (r13-relative) allocator table
extern void*   g_mainAllocTable[];  // @ r13+0 → allocator array

// ─────────────────────────────────────────────────────────────────────────────
// Constants
// ─────────────────────────────────────────────────────────────────────────────

// State IDs within the boot HSM
enum BootStateID {
    BOOT_STATE_SPLASH   = 2,    // initial boot / splash screen
    BOOT_STATE_LEGALS   = 4     // legals / EULA (transition target after logos complete)
};

// Screen IDs for the UI manager
enum ScreenID {
    SCREEN_NONE  = 0,
    SCREEN_LOGOS = 2
};

// String constants — addresses in .rdata, content determined by resolve_address
static const char* const STR_LOGOS_STATE_NAME   = (const char*)0x8205E478;  // GetName result
static const char* const STR_ON_ENTER_LOG       = (const char*)0x8205E4B4;  // OnEnter debug format
static const char* const STR_ON_EXIT_LOG        = (const char*)0x8205E4E4;  // OnExit debug format
static const char* const STR_INIT_LOG_1         = (const char*)0x8205E488;  // Init debug msg 1
static const char* const STR_INIT_LOG_2         = (const char*)0x8205E4A0;  // Init debug msg 2

// Vtable addresses
static const uint32_t VTABLE_PONG_LOGOS_CONTEXT    = 0x8205E564;
static const uint32_t VTABLE_HUD_LOGOS_PRIMARY     = 0x82060BF8;  // hudLogosScreen vtable #1
static const uint32_t VTABLE_HUD_LOGOS_SECONDARY   = 0x82060C20;  // hudLogosScreen vtable #2

// Float constant @ 0x8202D110 (.rdata, 4 bytes) — 1.0f for full-opacity fade
static const float FADE_FULL_OPACITY = 1.0f;

// ─────────────────────────────────────────────────────────────────────────────
// Helper: typed virtual call through raw vtable pointer
// ─────────────────────────────────────────────────────────────────────────────

// Allocator vtable slot 1: Alloc(allocator, size, alignment) → void*
static inline void* AllocatorAlloc(void* allocator, uint32_t size, uint32_t alignment) {
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    void** vtbl = *(void***)allocator;
    return ((AllocFn)vtbl[1])(allocator, size, alignment);
}

// Vtable slot 6 call on a context object (likely PrepareRender / Activate)
static inline void VCallSlot6(void* obj) {
    typedef void (*Fn)(void*);
    void** vtbl = *(void***)obj;
    ((Fn)vtbl[6])(obj);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  pongLogosContext — HSM Context (3 functions)
// ═══════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosContext::GetType  [vtable slot 14 @ 0x82306D48 | size: 0xC]
//
// Activates the context by setting m_bActive = 1.
// Despite the vtable slot name, the scaffold stores 1 into offset +0x1C
// (the m_bActive flag). This is called when the context becomes active
// in the HSM lifecycle.
// ─────────────────────────────────────────────────────────────────────────────
void pongLogosContext_Activate(pongLogosContext* self) {
    self->m_bActive = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosContext::Update  [vtable slot 16 @ 0x82306D58 | size: 0x50]
//
// Per-frame update: polls the graphics device to check if the logos
// page group has finished rendering. When complete (IsComplete returns 0),
// transitions the HSM to the legals state (BOOT_STATE_LEGALS = 4).
// ─────────────────────────────────────────────────────────────────────────────
void pongLogosContext_Update(pongLogosContext* self) {
    uint8_t stillShowing = grcPageGroup_IsComplete(g_grcDevice);

    if (stillShowing == 0) {
        // Logos have finished displaying — advance to legals
        pongLogosState* ownerState = (pongLogosState*)self->m_pOwnerState;
        hsmContext_SetNextState(ownerState->m_pHSMContext, BOOT_STATE_LEGALS);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosContext::Render  [vtable slot 18 @ 0x82306DA8 | size: 0xC]
//
// Delegates rendering to the grcDevice page-group renderer.
// Simply calls through to grcPageGroup_Render with the global device.
// ─────────────────────────────────────────────────────────────────────────────
void pongLogosContext_Render(pongLogosContext* self) {
    grcPageGroup_Render(g_grcDevice);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  pongLogosState — HSM State (4 functions)
// ═══════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosState::GetName  [vtable slot 13 @ 0x82306D38 | size: 0xC]
//
// Returns the debug name string for this HSM state.
// Used by the HSM framework for logging state transitions.
// ─────────────────────────────────────────────────────────────────────────────
const char* pongLogosState_GetName(pongLogosState* self) {
    return STR_LOGOS_STATE_NAME;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosState::Init  [vtable slot 14 @ 0x82306DB8 | size: 0xEC]
//
// Initialises the logos state by allocating and constructing:
//   1. A pongLogosContext (32 bytes, 16-byte aligned) — the HSM context
//   2. A hudLogosScreen  (96 bytes, 16-byte aligned) — the UI page object
//
// The hudLogosScreen uses multiple inheritance with two vtables:
//   +0x00 → primary vtable   @ 0x82060BF8
//   +0x04 → secondary vtable @ 0x82060C20
//
// Both objects are allocated through the SDA allocator (r13-relative).
// ─────────────────────────────────────────────────────────────────────────────
void pongLogosState_Init(pongLogosState* self) {
    // Ensure thread-local storage is set up for the allocator
    xe_main_thread_init_0038();

    // Get the allocator from the SDA global table (r13+0 → table, slot 4 → allocator ptr)
    void* allocator = *(void**)((uint8_t*)g_mainAllocTable + 4);

    // ── Allocate pongLogosContext (32 bytes, 16-byte aligned) ──
    void* contextMem = AllocatorAlloc(allocator, 32, 16);

    pongLogosContext* logosContext = nullptr;
    if (contextMem != nullptr) {
        // Construct the context in-place
        logosContext = (pongLogosContext*)contextMem;
        logosContext->m_field04     = 0;
        logosContext->m_field08     = 0;
        logosContext->m_field0C     = 0;
        logosContext->m_field10     = 0;
        logosContext->m_pOwnerState = self;
        logosContext->m_pUIContext  = nullptr;
        logosContext->m_bActive     = 1;
        // Set vtable last (standard RAGE pattern — allows base ctor to run first)
        logosContext->vtable = (void*)VTABLE_PONG_LOGOS_CONTEXT;
    }

    self->m_pLogosContext = logosContext;

    // Log initialisation
    nop_8240E6D0(STR_INIT_LOG_1);

    // ── Allocate hudLogosScreen (96 bytes, 16-byte aligned) ──
    xe_main_thread_init_0038();
    void* screenMem = AllocatorAlloc(allocator, 96, 16);

    void* logosScreen = nullptr;
    if (screenMem != nullptr) {
        // Run the base constructor
        hudLogosScreen_Init(screenMem);

        // Install the dual vtables (multiple inheritance)
        *(void**)((uint8_t*)screenMem + 0) = (void*)VTABLE_HUD_LOGOS_PRIMARY;
        *(void**)((uint8_t*)screenMem + 4) = (void*)VTABLE_HUD_LOGOS_SECONDARY;

        logosScreen = screenMem;
    }

    // Store the screen in the context's UI context slot (+0x18)
    if (logosContext != nullptr) {
        logosContext->m_pUIContext = logosScreen;
    }

    // Log completion
    nop_8240E6D0(STR_INIT_LOG_2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosState::OnEnter  [vtable slot 11 @ 0x82306F18 | size: 0xC0]
//
// Called when entering the logos state. Two paths:
//
// Path A (prevStateIdx != BOOT_STATE_SPLASH):
//   - Normal re-entry from another state. Logs the transition using
//     hsmContext_GetStateName to look up the previous state's name.
//
// Path B (prevStateIdx == BOOT_STATE_SPLASH):
//   - Initial entry from boot. Sets up the full rendering pipeline:
//     1. Activate the logos context via vtable slot 6 (PrepareRender)
//     2. Fade the page group to full opacity
//     3. Begin the graphics scene
//     4. Set the game loop rendering flag
//     5. Register the logos context with the UI manager
// ─────────────────────────────────────────────────────────────────────────────
void pongLogosState_OnEnter(pongLogosState* self, uint32_t prevStateIdx) {
    if (prevStateIdx != BOOT_STATE_SPLASH) {
        // Path A: log the transition
        const char* prevStateName = hsmContext_GetStateName(self->m_pHSMContext, prevStateIdx);
        nop_8240E6D0(STR_ON_ENTER_LOG, prevStateName, prevStateIdx);
        return;
    }

    // Path B: initial boot entry — set up rendering
    pongLogosContext* logosContext = (pongLogosContext*)self->m_pLogosContext;

    // Prepare the context for rendering (vtable slot 6)
    VCallSlot6(logosContext);

    // Fade to full opacity
    grcPageGroup_SetFade(g_grcDevice, FADE_FULL_OPACITY, 0, 0, 0);

    // Begin graphics scene
    grcDevice_BeginScene(g_grcDevice);

    // Set the game loop's "rendering logos" flag
    // g_loop_obj_ptr → +576 (0x240) is the rendering-logos byte flag
    uint8_t* gameLoopFlags = (uint8_t*)g_loop_obj_ptr;
    gameLoopFlags[576] = 1;

    // Register with the UI manager: screen = SCREEN_LOGOS, context = logosContext
    uint8_t* uiMgr = (uint8_t*)g_uiManager;
    *(uint32_t*)(uiMgr + 52) = SCREEN_LOGOS;                       // +0x34: m_currentScreen
    *(void**)(uiMgr + 56)    = (void*)self->m_pLogosContext;       // +0x38: m_pActiveContext
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLogosState::OnExit  [vtable slot 12 @ 0x82306FD8 | size: 0x94]
//
// Called when exiting the logos state. Two paths:
//
// Path A (nextStateIdx != BOOT_STATE_LEGALS):
//   - Non-standard exit. Logs the transition for debugging.
//
// Path B (nextStateIdx == BOOT_STATE_LEGALS):
//   - Normal forward transition to the legals screen:
//     1. Fade the page group with mode -1 (0xFFFFFFFF = fade-out)
//     2. Clear the UI manager's active context
// ─────────────────────────────────────────────────────────────────────────────
void pongLogosState_OnExit(pongLogosState* self, uint32_t nextStateIdx) {
    if (nextStateIdx != BOOT_STATE_LEGALS) {
        // Path A: non-standard exit — log the transition
        const char* nextStateName = hsmContext_GetStateName(self->m_pHSMContext, nextStateIdx);
        nop_8240E6D0(STR_ON_EXIT_LOG, nextStateName, nextStateIdx);
        return;
    }

    // Path B: standard exit to legals — fade out and clear UI
    grcPageGroup_SetFade(g_grcDevice, FADE_FULL_OPACITY, 0xFFFFFFFF, 0, 0);

    // Clear the UI manager's active screen
    game_AAF8(g_uiManager, 0, 0);
}
