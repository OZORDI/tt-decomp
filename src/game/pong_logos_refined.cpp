/**
 * pong_logos.cpp — Logos State and Context Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the boot logos sequence state machine:
 * - pongLogosState: HSM state that drives the logo display sequence
 * - pongLogosContext: HSM context that manages the logos UI
 *
 * This is the first screen shown after boot, displaying publisher
 * and developer logos before transitioning to the main menu.
 */

#include <stdint.h>
#include "globals.h"

// Forward declarations
extern "C" {
    void hsmContext_GetStateName(void* hsmContext, char* outBuffer, uint32_t bufferSize);
    void debug_LogStateTransition(const char* format, const char* contextName, uint32_t stateIdx);
    void grcDevice_SetFade(void* device, float fadeValue, uint32_t fadeMode, uint32_t p3, uint32_t p4);
    void grcDevice_BeginScene(void* device);
    void grcDevice_Render(void* device);
    uint8_t grcDevice_IsRenderComplete(void* device);
    void xe_InitThreadLocalStorage();
    void* rage_Allocate(void* allocator, uint32_t size, uint32_t alignment);
    void pongLogosUIContext_Init(void* uiContext);
    void hsmContext_SetNextState(void* hsmContext, uint32_t nextStateIdx);
}

// Global allocator (SDA r13+4)
extern void* g_allocator;

/**
 * pongLogosState — HSM State for Logo Display
 * 
 * Structure layout:
 *   +0x00  vtable pointer
 *   +0x04  m_pHSMContext  — rage::hsmContext* for state transitions
 *   +0x08  m_pLogosContext — pongLogosContext* allocated in Init()
 */
struct pongLogosState {
    void*       vtable;           // +0x00
    void*       m_pHSMContext;    // +0x04  HSM context for transitions
    void*       m_pLogosContext;  // +0x08  Logos UI context
};

/**
 * pongLogosContext — HSM Context for Logos UI
 * 
 * Structure layout:
 *   +0x00  vtable pointer
 *   +0x04  m_field04  — unknown
 *   +0x08  m_field08  — unknown
 *   +0x0C  m_field0C  — unknown
 *   +0x10  m_field10  — unknown
 *   +0x14  m_pOwnerState — pongLogosState* that owns this context
 *   +0x18  m_pUIContext — UI context object
 *   +0x1C  m_bActive — whether logos are currently displaying
 */
struct pongLogosContext {
    void*       vtable;           // +0x00
    uint32_t    m_field04;        // +0x04
    uint32_t    m_field08;        // +0x08
    uint32_t    m_field0C;        // +0x0C
    uint32_t    m_field10;        // +0x10
    void*       m_pOwnerState;    // +0x14  pongLogosState* owner
    void*       m_pUIContext;     // +0x18  UI context object
    uint8_t     m_bActive;        // +0x1C  active flag
};

/**
 * UIManager — Global UI state manager
 * 
 * Structure layout (partial):
 *   +0x34  m_currentScreen — current screen ID
 *   +0x38  m_pActiveContext — active UI context pointer
 */
struct UIManager {
    uint8_t     _pad[52];         // +0x00..+0x33
    uint32_t    m_currentScreen;  // +0x34
    void*       m_pActiveContext; // +0x38
};

/**
 * GameState — Global game state object
 * 
 * Structure layout (partial):
 *   +0x240  m_bRenderingLogos — flag indicating logos are being rendered
 */
struct GameState {
    uint8_t     _pad[576];        // +0x00..+0x23F
    uint8_t     m_bRenderingLogos;// +0x240
};

// String constants (in .rdata section)
static const char* STR_LOGOS_STATE_NAME = (const char*)0x8205E478;  // "pongLogosState"
static const char* STR_ENTERING_LOGOS = (const char*)0x8205E4B4;    // "Entering logos state..."
static const char* STR_EXITING_LOGOS = (const char*)0x8205E4E4;     // "Exiting logos state..."
static const char* STR_LOGOS_INITIALIZED = (const char*)0x8205E488; // "Logos state initialized"
static const char* STR_UI_CONTEXT_CREATED = (const char*)0x8205E4A0;// "UI context created"

// Vtable addresses
static const void* VTABLE_PONG_LOGOS_CONTEXT = (void*)0x8205E564;
static const void* VTABLE_UI_CONTEXT_PRIMARY = (void*)0x82060BF8;
static const void* VTABLE_UI_CONTEXT_SECONDARY = (void*)0x82060C20;

// State IDs
enum StateID {
    STATE_BOOT = 2,
    STATE_MAIN_MENU = 4
};

// Screen IDs
enum ScreenID {
    SCREEN_NONE = 0,
    SCREEN_LOGOS = 2
};

/**
 * pongLogosState::GetName @ 0x82306D38 | size: 0xC
 * 
 * Returns the state name string for debugging/logging.
 * This is slot 13 in the hsmState vtable.
 */
extern "C" const char* pongLogosState_GetName(pongLogosState* self) {
    return STR_LOGOS_STATE_NAME;
}

/**
 * pongLogosContext::GetType @ 0x82306D48 | size: 0xC
 * 
 * Returns the context type ID. This is slot 14 in the
 * hsmContext vtable, used for RTTI-style type checking.
 */
extern "C" uint32_t pongLogosContext_ProcessInput(pongLogosContext* self) {
    return 1;  // CONTEXT_TYPE_LOGOS
}

/**
 * pongLogosContext::Update @ 0x82306D58 | size: 0x50
 * 
 * Updates the logos context each frame. Checks if the graphics
 * device has finished displaying the logos, and if so, transitions
 * to the next state (STATE_MAIN_MENU).
 * 
 * This is slot 16 in the hsmContext vtable.
 */
extern "C" void pongLogosContext_OnUpdate(pongLogosContext* self) {
    // Check if graphics device has finished displaying logos
    uint8_t isComplete = grcDevice_IsRenderComplete(g_grcDevice);
    
    if (!isComplete) {
        // Logos finished, transition to main menu
        pongLogosState* ownerState = (pongLogosState*)self->m_pOwnerState;
        hsmContext_SetNextState(ownerState->m_pHSMContext, STATE_MAIN_MENU);
    }
}

/**
 * pongLogosContext::Render @ 0x82306DA8 | size: 0xC
 * 
 * Renders the logos screen. Delegates to the graphics device's
 * render function.
 * 
 * This is slot 18 in the hsmContext vtable.
 */
extern "C" void pongLogosContext_OnShutdown(pongLogosContext* self) {
    grcDevice_Render(g_grcDevice);
}

/**
 * pongLogosState::OnEnter @ 0x82306F18 | size: 0xC0
 * 
 * Called when entering the logos state. Handles two cases:
 * 1. If prevStateIdx == STATE_BOOT: Normal entry, log the transition
 * 2. Otherwise: Special case - render the logos and set up the scene
 * 
 * This is slot 11 in the hsmState vtable.
 */
extern "C" void pongLogosState_OnEnter(pongLogosState* self, uint32_t prevStateIdx) {
    if (prevStateIdx == STATE_BOOT) {
        // Normal transition from boot state
        char contextName[128];
        hsmContext_GetStateName(self->m_pHSMContext, contextName, sizeof(contextName));
        debug_LogStateTransition(STR_ENTERING_LOGOS, contextName, prevStateIdx);
    } else {
        // Special case: render the logos immediately
        pongLogosContext* logosContext = (pongLogosContext*)self->m_pLogosContext;
        
        // Call virtual method slot 6 on the logos context (PrepareRender)
        void** vtable = *(void***)logosContext;
        typedef void (*RenderPrepFunc)(void*);
        RenderPrepFunc prepFunc = (RenderPrepFunc)vtable[6];
        prepFunc(logosContext);
        
        // Set up fade-in effect (full opacity)
        grcDevice_SetFade(g_grcDevice, 1.0f, 0, 0, 0);
        
        // Begin rendering scene
        grcDevice_BeginScene(g_grcDevice);
        
        // Set game state flags
        GameState* gameState = (GameState*)g_gameObject;
        gameState->m_bRenderingLogos = 1;
        
        // Set UI manager state
        UIManager* uiMgr = (UIManager*)g_uiManager;
        uiMgr->m_currentScreen = SCREEN_LOGOS;
        uiMgr->m_pActiveContext = logosContext;
    }
}

/**
 * pongLogosState::OnExit @ 0x82306FD8 | size: 0x94
 * 
 * Called when exiting the logos state. Handles two cases:
 * 1. If nextStateIdx == STATE_MAIN_MENU: Transition to main menu, fade out
 * 2. Otherwise: Normal exit, log the transition
 * 
 * This is slot 12 in the hsmState vtable.
 */
extern "C" void pongLogosState_OnExit(pongLogosState* self, uint32_t nextStateIdx) {
    if (nextStateIdx == STATE_MAIN_MENU) {
        // Transitioning to main menu - fade out the logos
        grcDevice_SetFade(g_grcDevice, 1.0f, 0xFFFFFFFF, 0, 0);  // Fade out mode
        
        // Clear UI manager state
        UIManager* uiMgr = (UIManager*)g_uiManager;
        uiMgr->m_currentScreen = SCREEN_NONE;
        uiMgr->m_pActiveContext = nullptr;
    } else {
        // Normal exit, log the transition
        char contextName[128];
        hsmContext_GetStateName(self->m_pHSMContext, contextName, sizeof(contextName));
        debug_LogStateTransition(STR_EXITING_LOGOS, contextName, nextStateIdx);
    }
}

/**
 * pongLogosState::Init @ 0x82306DB8 | size: 0xEC
 * 
 * Initializes the logos state. Allocates and constructs the
 * pongLogosContext object and its UI context, then logs initialization.
 * 
 * This is slot 14 in the hsmState vtable.
 */
extern "C" void pongLogosState_ProcessInput(pongLogosState* self) {
    // Ensure TLS is initialized
    xe_InitThreadLocalStorage();
    
    // Allocate pongLogosContext (32 bytes, 16-byte aligned)
    pongLogosContext* logosContext = (pongLogosContext*)rage_Allocate(
        g_allocator, 32, 16
    );
    
    if (logosContext) {
        // Initialize the context structure
        logosContext->vtable = (void*)VTABLE_PONG_LOGOS_CONTEXT;
        logosContext->m_field04 = 0;
        logosContext->m_field08 = 0;
        logosContext->m_field0C = 0;
        logosContext->m_field10 = 0;
        logosContext->m_pOwnerState = self;
        logosContext->m_pUIContext = nullptr;
        logosContext->m_bActive = 1;
    }
    
    // Store the context in the state
    self->m_pLogosContext = logosContext;
    
    // Log initialization
    debug_LogStateTransition(STR_LOGOS_INITIALIZED, nullptr, 0);
    
    // Allocate UI context (96 bytes, 16-byte aligned)
    xe_InitThreadLocalStorage();
    void* uiContext = rage_Allocate(g_allocator, 96, 16);
    
    if (uiContext) {
        // Initialize UI context
        pongLogosUIContext_Init(uiContext);
        
        // Set vtables for multiple inheritance
        *(void**)uiContext = (void*)VTABLE_UI_CONTEXT_PRIMARY;
        *(void**)((char*)uiContext + 4) = (void*)VTABLE_UI_CONTEXT_SECONDARY;
    }
    
    // Store UI context in logos context
    if (logosContext) {
        logosContext->m_pUIContext = uiContext;
    }
    
    // Log completion
    debug_LogStateTransition(STR_UI_CONTEXT_CREATED, nullptr, 0);
}
