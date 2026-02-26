/**
 * pong_game.hpp — Game state and context management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Core game state machine classes:
 * - pongGameState: Base state class with virtual interface
 * - pongGameContext: Context manager with message processing
 * - pongGame: Root game object
 * 
 * These form a hierarchical state machine that manages game flow,
 * UI transitions, and message dispatch throughout the game.
 */

#pragma once
#include <stdint.h>

// ────────────────────────────────────────────────────────────────────────────
// pongGameState — Base game state class
// @ vtable 0x82071AA4
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGameState
 * 
 * Base class for game states. Provides virtual interface for state-specific
 * behavior. States are managed by pongGameContext and represent different
 * phases of gameplay (menu, loading, playing, paused, etc.)
 * 
 * Virtual methods:
 * - vfn_0:  Destructor
 * - vfn_10: State update/tick
 * - vfn_11: State enter
 * - vfn_12: State exit
 * - vfn_13: Get state name (returns const char*)
 * - vfn_14: State-specific processing
 */
struct pongGameState {
    void** vtable;  // +0x00
    
    // Virtual interface
    virtual ~pongGameState() = default;
    virtual void vfn_10() = 0;  // Update
    virtual void vfn_11() = 0;  // Enter
    virtual void vfn_12() = 0;  // Exit
    virtual void vfn_13() = 0;  // GetName
    virtual void vfn_14() = 0;  // Process
};

// ────────────────────────────────────────────────────────────────────────────
// pongGameContext — Game context and message processor
// @ vtable 0x82071AEC (primary), 0x82071B50 (secondary)
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGameContext
 * 
 * Manages the current game context and processes messages from UI and input
 * systems. Uses multiple inheritance (2 vtables) for integration with RAGE
 * engine's message system.
 * 
 * Key responsibilities:
 * - Message dispatch (Process method)
 * - State management (holds current pongGameState)
 * - Coordination between UI, input, and game logic
 * - Character selection and variation handling
 * 
 * Message types handled:
 * - 2060: Character variation change
 * - 2138: Game state transition
 * - 2139: Game event
 * - 2140: Input/menu navigation
 * - ... and many more (Process is 2892 bytes!)
 */
struct pongGameContext {
    void** vtable;           // +0x00 - Primary vtable
    void*  m_pGameState;     // +0x04 - Current game state
    // +0x08 onwards: Additional fields (TBD from full analysis)
    
    // Virtual interface
    virtual ~pongGameContext();
    virtual void vfn_12() = 0;
    virtual void vfn_14() = 0;
    virtual void vfn_16() = 0;
    virtual void vfn_17() = 0;
    virtual void vfn_18() = 0;
    virtual void vfn_22() = 0;
    
    // Main message processor
    void Process(void* message);
};

// ────────────────────────────────────────────────────────────────────────────
// pongGame — Root game object
// @ vtable 0x82071B9C
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGame
 * 
 * Root game object. Likely holds the pongGameContext and coordinates
 * high-level game systems. Minimal vtable suggests this is primarily
 * a container/coordinator rather than having complex virtual behavior.
 */
struct pongGame {
    void** vtable;  // +0x00
    // Additional fields TBD
};

// ────────────────────────────────────────────────────────────────────────────
// Related classes (from same subsystem)
// ────────────────────────────────────────────────────────────────────────────

// These are referenced in the header but defined elsewhere:
// - lvlLevel, lvlLevelMgr, lvlLevelPiece, lvlTable, lvlTableTop
// - pongControlGroup, pongRandomizer, pongShell
// - shInit, shEnd, shShell

// ────────────────────────────────────────────────────────────────────────────
// ARCHITECTURE NOTES
// ────────────────────────────────────────────────────────────────────────────
//
// State Machine Hierarchy:
//
//   pongGame (root)
//     └─> pongGameContext (context manager)
//           ├─> pongGameState (current state)
//           └─> Process() message dispatcher
//
// Message Flow:
//   UI/Input System → Message → pongGameContext::Process() → State Handlers
//
// The Process() method is the central message dispatcher, routing messages
// to appropriate handlers based on message type. It's a massive function
// (2892 bytes) that needs to be refactored into smaller message-specific
// handlers for maintainability.
//
// ────────────────────────────────────────────────────────────────────────────
