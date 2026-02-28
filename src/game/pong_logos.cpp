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

// Forward declarations
extern "C" void PostStateTransitionRequest(void* obj);
extern "C" void nop_8240E6D0(const char* debugStr, void* obj, uint32_t stateIdx);
extern "C" void FadePageGroup(void* grcDevice, float fadeValue, uint32_t p3, uint32_t p4, uint32_t p5);
extern "C" void grcDevice_beginScene(void* grcDevice);
extern "C" void DismissPageGroup(void* grcDevice);
extern "C" void ShowPageGroup(void* grcDevice);
extern "C" void xe_main_thread_init_0038();
extern "C" void game_AC88(void* obj);
extern "C" void hsmContext_SetNextState_2800(void* hsmContext, uint32_t nextStateIdx);

// Global pointers
extern void* g_grcDevice_ptr;      // @ 0x8271A81C (graphics device)
extern void* g_game_obj_ptr;       // @ 0x8271A81C (game state)
extern void* g_ui_manager_ptr;     // @ 0x8271A358 (UI manager)

// SDA (Small Data Area) globals
extern void* g_allocator_ptr;      // r13+4 = allocator singleton

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
    void*       m_pUIContext;     // +0x18  UI context
    uint8_t     m_bActive;        // +0x1C  active flag
};

// Vtable addresses (from RTTI)
// pongLogosState vtable:   0x8205E51C
// pongLogosContext vtable: 0x8205E564

/**
 * pongLogosState::GetName @ 0x82306D38 | size: 0xC
 * 
 * Returns the state name string for debugging/logging.
 * This is likely slot 13 in the hsmState vtable.
 * 
 * Address calculated: lis r11,-32250; addi r3,r11,-7048
 * = 0x8205E478 (likely "pongLogosState" or "LogosState")
 */
extern "C" const char* pongLogosState_GetName(pongLogosState* self) {
    return (const char*)0x8205E478;
}

/**
 * pongLogosContext::GetType @ 0x82306D48 | size: 0xC
 * 
 * Returns the context type ID. This is likely slot 14 in the
 * hsmContext vtable, used for RTTI-style type checking.
 * 
 * Returns 1 (likely CONTEXT_TYPE_LOGOS or similar enum value).
 */
extern "C" uint32_t pongLogosContext_ProcessInput(pongLogosContext* self) {
    return 1;
}

/**
 * pongLogosContext::Update @ 0x82306D58 | size: 0x50
 * 
 * Updates the logos context each frame. Checks if the graphics
 * device has finished displaying the logos, and if so, transitions
 * to the next state (state 4 = likely main menu or next boot screen).
 * 
 * This is slot 16 in the hsmContext vtable.
 */
extern "C" void pongLogosContext_OnUpdate(pongLogosContext* self) {
    // Check if graphics device has finished displaying logos
    void* grcDevice = g_grcDevice_ptr;
    uint8_t isComplete = ShowPageGroup(grcDevice);
    
    if (!isComplete) {
        // Logos still displaying, transition to next state
        pongLogosState* ownerState = (pongLogosState*)self->m_pOwnerState;
        void* hsmContext = ownerState->m_pHSMContext;
        
        // Transition to state 4 (next boot screen or main menu)
        hsmContext_SetNextState_2800(hsmContext, 4);
    }
}

/**
 * pongLogosContext::Render @ 0x82306DA8 | size: 0xC
 * 
 * Renders the logos screen. Delegates to the graphics device's
 * render function (DismissPageGroup).
 * 
 * This is slot 18 in the hsmContext vtable.
 */
extern "C" void pongLogosContext_OnShutdown(pongLogosContext* self) {
    void* grcDevice = g_grcDevice_ptr;
    DismissPageGroup(grcDevice);
}

/**
 * pongLogosState::OnEnter @ 0x82306F18 | size: 0xC0
 * 
 * Called when entering the logos state. Handles two cases:
 * 1. If prevStateIdx == 2: Normal entry, log the transition
 * 2. Otherwise: Special case - render the logos and set up the scene
 * 
 * This is slot 11 in the hsmState vtable.
 */
extern "C" void pongLogosState_OnEnter(pongLogosState* self, uint32_t prevStateIdx) {
    if (prevStateIdx == 2) {
        // Normal transition from previous state
        void* hsmContext = self->m_pHSMContext;
        const char* contextName = (const char*)PostStateTransitionRequest(hsmContext);
        
        // Log the state transition
        // String at 0x8205E4B4: likely "Entering logos state from %s, prev=%d"
        const char* logFormat = (const char*)0x8205E4B4;
        nop_8240E6D0(logFormat, contextName, prevStateIdx);
    } else {
        // Special case: render the logos immediately
        void* logosContext = self->m_pLogosContext;
        
        // Call virtual method slot 6 on the logos context
        // (likely PrepareRender or similar)
        void** vtable = *(void***)logosContext;
        typedef void (*RenderPrepFunc)(void*);
        RenderPrepFunc prepFunc = (RenderPrepFunc)vtable[6];
        prepFunc(logosContext);
        
        // Set up fade-in effect
        // lfs f1,-12016(r11) loads a float constant (likely 1.0f for full opacity)
        float fadeValue = 1.0f;
        void* grcDevice = g_grcDevice_ptr;
        FadePageGroup(grcDevice, fadeValue, 0, 0, 0);
        
        // Begin rendering scene
        grcDevice_beginScene(grcDevice);
        
        // Set game state flags
        struct GameState {
            uint8_t _pad[576];
            uint8_t renderingLogos;  // +0x240
        };
        GameState* gameState = (GameState*)g_game_obj_ptr;
        gameState->renderingLogos = 1;
        
        // Set UI manager state
        struct UIManager {
            uint8_t _pad[52];
            uint32_t currentScreen;  // +0x34
            void*    activeContext;  // +0x38
        };
        UIManager* uiMgr = (UIManager*)g_ui_manager_ptr;
        uiMgr->currentScreen = 2;  // SCREEN_LOGOS
        uiMgr->activeContext = logosContext;
    }
}

/**
 * pongLogosState::OnExit @ 0x82306FD8 | size: 0x94
 * 
 * Called when exiting the logos state. Handles two cases:
 * 1. If nextStateIdx == 4: Transition to next screen, fade out
 * 2. Otherwise: Normal exit, log the transition
 * 
 * This is slot 12 in the hsmState vtable.
 */
extern "C" void pongLogosState_OnExit(pongLogosState* self, uint32_t nextStateIdx) {
    if (nextStateIdx == 4) {
        // Transitioning to next screen (main menu or loading)
        // Fade out the logos
        float fadeValue = 1.0f;
        void* grcDevice = g_grcDevice_ptr;
        FadePageGroup(grcDevice, fadeValue, 0xFFFFFFFF, 0, 0);  // -1 = fade out
        
        // Clear UI manager state
        struct UIManager {
            uint8_t _pad[52];
            uint32_t currentScreen;
            void*    activeContext;
        };
        UIManager* uiMgr = (UIManager*)g_ui_manager_ptr;
        uiMgr->currentScreen = 0;
        uiMgr->activeContext = nullptr;
    } else {
        // Normal exit, log the transition
        void* hsmContext = self->m_pHSMContext;
        const char* contextName = (const char*)PostStateTransitionRequest(hsmContext);
        
        // String at 0x8205E4E4: likely "Exiting logos state to %s, next=%d"
        const char* logFormat = (const char*)0x8205E4E4;
        nop_8240E6D0(logFormat, contextName, nextStateIdx);
    }
}

/**
 * pongLogosState::Init @ 0x82306DB8 | size: 0xEC
 * 
 * Initializes the logos state. Allocates and constructs the
 * pongLogosContext object, then logs the initialization.
 * 
 * This is slot 14 in the hsmState vtable.
 */
extern "C" void pongLogosState_ProcessInput(pongLogosState* self) {
    // Get allocator from SDA (Small Data Area)
    xe_main_thread_init_0038();  // Ensure TLS is initialized
    
    void** sdaBase = (void**)0x82600000;  // r13 base
    void* allocator = sdaBase[1];  // r13+4
    
    // Allocate pongLogosContext (32 bytes, 16-byte aligned)
    void** allocVtable = *(void***)allocator;
    typedef void* (*AllocFunc)(void*, uint32_t, uint32_t);
    AllocFunc allocFunc = (AllocFunc)allocVtable[1];
    void* logosContext = allocFunc(allocator, 32, 16);
    
    if (logosContext) {
        // Initialize the context structure
        pongLogosContext* ctx = (pongLogosContext*)logosContext;
        ctx->vtable = (void*)0x8205E564;  // pongLogosContext vtable
        ctx->m_field04 = 0;
        ctx->m_field08 = 0;
        ctx->m_field0C = 0;
        ctx->m_field10 = 0;
        ctx->m_pOwnerState = self;
        ctx->m_pUIContext = nullptr;
        ctx->m_bActive = 1;
    }
    
    // Store the context in the state
    self->m_pLogosContext = logosContext;
    
    // Log initialization
    // String at 0x8205E488: likely "pongLogosState initialized"
    const char* logMsg = (const char*)0x8205E488;
    nop_8240E6D0(logMsg, nullptr, 0);
    
    // Allocate UI context (96 bytes, 16-byte aligned)
    xe_main_thread_init_0038();
    void* uiContext = allocFunc(allocator, 96, 16);
    
    if (uiContext) {
        // Initialize UI context with game_AC88 (likely constructor)
        game_AC88(uiContext);
        
        // Set vtables for multiple inheritance
        // Vtable addresses calculated from assembly:
        // +3064 offset: 0x82060BF8 (primary vtable)
        // +3104 offset: 0x82060C20 (secondary vtable)
        *(void**)uiContext = (void*)0x82060BF8;
        *(void**)((char*)uiContext + 4) = (void*)0x82060C20;
    }
    
    // Store UI context in logos context
    if (logosContext) {
        ((pongLogosContext*)logosContext)->m_pUIContext = uiContext;
    }
    
    // Log completion
    // String at 0x8205E4A0: likely "pongLogosState UI context created"
    const char* completeMsg = (const char*)0x8205E4A0;
    nop_8240E6D0(completeMsg, nullptr, 0);
}
