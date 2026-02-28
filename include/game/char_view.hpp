/**
 * char_view.hpp — Character View System Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Character viewing/selection UI data structures and state management.
 * These classes handle character model display, camera control, and
 * UI state for character selection screens.
 * 
 * Architecture:
 *   - pongAttractState: Base state machine state for all attract mode screens
 *     (logos, menus, character selection). Manages UI screen object lifecycle.
 *   
 *   - pongCharViewState: Derived state for character viewing/selection.
 *     Handles input, transitions, and rendering for character selection UI.
 *   
 *   - charViewData: Main data container with serialized XML fields.
 *     Manages array of 2D positions for character placement in UI.
 *   
 *   - charViewCharData: Per-character view data (positions, animations).
 *     Serialized from XML configuration files.
 *   
 *   - charViewCS: Camera shot controller for character view screens.
 *     Manages camera positioning and transitions.
 *   
 *   - pongCharViewContext: Context/controller with multiple inheritance.
 *     Coordinates the overall character viewing experience.
 * 
 * Classes:
 *   - charViewCharData: Per-character view data (positions, animations)
 *   - charViewData: Main character view data container
 *   - charViewCS: Character view camera shot
 *   - pongCharViewState: Character view UI state (inherits from pongAttractState)
 *   - pongCharViewContext: Character view context/controller
 *   - pongAttractState: Base state machine state for attract mode screens
 */

#pragma once

#include <stdint.h>
#include "rage/hsmState.hpp"

// Forward declarations
class atSingleton;

// ────────────────────────────────────────────────────────────────────────────
// pongAttractState — Base State Machine State for Attract Mode
// @ vtable 0x8205E0AC
// Inherits from: hsmState
// ────────────────────────────────────────────────────────────────────────────

/**
 * Base state class for attract mode screens (logos, menus, character selection).
 * Implements hierarchical state machine pattern with UI object management.
 * 
 * Derived states: pongCharViewState, pongPauseState, pongCreditsState, etc.
 * 
 * Structure:
 *   Inherits hsmState fields (+0x00 to +0x14)
 *   +0x18: m_pScreenObject - UI screen object (32 bytes, allocated/freed)
 * 
 * Size: 0x1C (28 bytes) = hsmState (0x18) + m_pScreenObject (0x4)
 */
class pongAttractState : public hsmState {
public:
    // Virtual methods
    
    /**
     * GetContext @ 0x823057E8 | size: 0x18
     * Returns the UI context object, forwarding to screen object's vtable slot 4
     */
    virtual void* GetContext();
    
    /**
     * OnExit @ 0x82306EA8 | size: 0x6C
     * Called when leaving this state - cleans up UI screen object
     */
    virtual void OnExit() override;
    
    /**
     * OnEvent @ 0x823058E0 | size: 0xC8
     * Handles input and system events
     * Event 6: Creates UI state
     * Events 9, 12: No-op
     * Other events: Forwarded to manager
     */
    virtual void OnEvent(int32_t eventType) override;
    
    /**
     * Update @ 0x823059A8 | size: 0xB4
     * Frame update - called every frame
     */
    virtual void Update(float deltaTime) override;
    
    /**
     * GetStateId @ 0x82305A60 | size: 0xC
     * Returns state identifier
     */
    virtual int32_t GetStateId() const;
    
    /**
     * OnEnter @ 0x82305800 | size: 0xE0
     * Called when entering this state - allocates and initializes UI screen object
     */
    virtual void OnEnter() override;

protected:
    // Inherits from hsmState:
    //   +0x00: vtable
    //   +0x04: m_pManager
    //   +0x08: m_pParentState
    //   +0x0C: m_pChildState
    //   +0x10: m_field_10
    //   +0x14: m_field_14
    
    void* m_pScreenObject;        // +0x18 - UI screen object (32 bytes)
};

// Verify struct size matches PowerPC layout
static_assert(sizeof(pongAttractState) == 0x1C, "pongAttractState size mismatch");

// ────────────────────────────────────────────────────────────────────────────
// charViewCharData — Per-Character View Data
// @ vtable 0x820762F4
// ────────────────────────────────────────────────────────────────────────────

/**
 * Stores view-specific data for a single character in the character
 * selection screen. Contains position, animation, and display parameters.
 * 
 * Serialized fields (XML):
 *   - field_10, field_14, field_18 (exact names TBD from XML)
 * 
 * Size: 0x1C (28 bytes) = vtable (4) + padding (12) + 3 fields (12)
 */
class charViewCharData {
public:
    // Virtual methods
    virtual ~charViewCharData() = default;
    
    /**
     * IsCharacterType @ 0x8240B238 | size: 0x48
     * Type checking function - compares input ID against known character types
     */
    virtual bool IsCharacterType(uint32_t typeId);
    
    /**
     * Serialize @ 0x8240B280 | size: 0x8C
     * Serialize character view data to XML/binary format
     */
    virtual void Serialize(void* serializer);
    
    /**
     * GetDataInfo @ 0x8230A4F8 | size: 0xC
     * Get character view data size or type info
     */
    virtual uint32_t GetDataInfo();

protected:
    void** m_vtable;              // +0x00
    uint32_t m_padding[3];        // +0x04-0x0F
    
    // Serialized fields (exact types TBD from usage)
    uint32_t m_field_10;          // +0x10 - XML field name TBD
    uint32_t m_field_14;          // +0x14 - XML field name TBD
    uint32_t m_field_18;          // +0x18 - XML field name TBD
};

static_assert(sizeof(charViewCharData) == 0x1C, "charViewCharData size mismatch");

// ────────────────────────────────────────────────────────────────────────────
// charViewData — Main Character View Data Container
// @ vtable 0x8207635C
// ────────────────────────────────────────────────────────────────────────────

/**
 * Main data container for the character view system. Manages multiple
 * character view entries, camera positions, and UI layout data.
 * 
 * Contains a large number of serialized fields (22+ fields) and manages
 * a dynamically allocated array of view elements (2D positions per character).
 * 
 * Data Flow:
 *   1. LoadViewData() allocates array based on character count from game data manager
 *   2. Initializes each element to (0.0f, 0.0f)
 *   3. Processes linked list of character data (m_pLinkedListHead)
 *   4. For each character matching type filter, looks up index and sets position
 * 
 * Serialized fields (XML):
 *   - field_10, field_14, field_18, field_1C, field_20, field_24
 *   - field_28, field_2C, field_40, field_44, field_4C, field_50
 *   - field_58, field_5C, field_60, field_64
 *   - field_F0, field_F4, field_120, field_124, field_150, field_154
 * 
 * Size: 0x184 (388 bytes)
 */
class charViewData : public atSingleton {
public:
    // Virtual methods
    
    /**
     * ~charViewData @ 0x8240B530 | size: 0x68
     * Destructor - frees allocated view data array
     */
    virtual ~charViewData();
    
    /**
     * Initialize @ 0x8240B598 | size: 0x24
     * Initialize or reset view data
     */
    virtual void Initialize();
    
    /**
     * LoadViewData @ 0x8240B5C0 | size: 0x1AC
     * Load and populate character view data from game data
     * Allocates array, initializes elements, processes linked list
     */
    virtual void LoadViewData();
    
    /**
     * ValidateData @ 0x8240B310 | size: 0x48
     * Type checking or validation function
     */
    virtual bool ValidateData(uint32_t param);
    
    /**
     * Serialize @ 0x8240B770 | size: 0x47C
     * Serialize all character view data (22+ fields)
     */
    virtual void Serialize(void* serializer);
    
    /**
     * GetDataInfo @ 0x8231F408 | size: 0xC
     * Get data info or size
     */
    virtual uint32_t GetDataInfo();

protected:
    void** m_vtable;              // +0x00
    uint32_t m_padding[2];        // +0x04-0x0B
    void* m_pLinkedListHead;      // +0x0C - linked list of view elements
    
    // Serialized data fields (types inferred from serialization)
    // XML field names TBD - need to extract from string table
    uint32_t m_field_10;          // +0x10
    uint32_t m_field_14;          // +0x14
    uint32_t m_field_18;          // +0x18
    uint32_t m_field_1C;          // +0x1C
    uint32_t m_field_20;          // +0x20
    uint32_t m_field_24;          // +0x24
    uint32_t m_field_28;          // +0x28
    uint32_t m_field_2C;          // +0x2C
    uint32_t m_field_40;          // +0x40
    uint32_t m_field_44;          // +0x44
    uint32_t m_field_4C;          // +0x4C
    uint32_t m_field_50;          // +0x50
    uint32_t m_field_58;          // +0x58
    uint32_t m_field_5C;          // +0x5C
    uint32_t m_field_60;          // +0x60
    uint32_t m_field_64;          // +0x64
    
    uint8_t m_padding2[140];      // +0x68-0xEF
    
    uint32_t m_field_F0;          // +0xF0 (240)
    uint32_t m_field_F4;          // +0xF4 (244)
    
    uint8_t m_padding3[44];       // +0xF8-0x11F
    
    uint32_t m_field_120;         // +0x120 (288)
    uint32_t m_field_124;         // +0x124 (292)
    
    uint8_t m_padding4[44];       // +0x128-0x14F
    
    uint32_t m_field_150;         // +0x150 (336)
    uint32_t m_field_154;         // +0x154 (340)
    
    uint8_t m_padding5[36];       // +0x158-0x17B
    
    // Dynamic array management
    void* m_pAllocatedData;       // +0x17C (380) - array of 2D position elements
    uint16_t m_currentCount;      // +0x180 (384) - current element count
    uint16_t m_allocatedSize;     // +0x182 (386) - allocated array size
};

static_assert(sizeof(charViewData) == 0x184, "charViewData size mismatch");

// ────────────────────────────────────────────────────────────────────────────
// charViewCS — Character View Camera Shot
// @ vtable 0x820362C0
// ────────────────────────────────────────────────────────────────────────────

/**
 * Camera shot controller for character view screens.
 * Manages camera positioning, transitions, and rendering for character display.
 * 
 * Virtual methods need better names - research ongoing.
 */
class charViewCS {
public:
    // Virtual methods
    
    /**
     * ~charViewCS @ 0x8216BD20 | size: 0x68
     * Destructor - cleans up embedded object at +292
     */
    virtual ~charViewCS();
    
    // TODO: Research and rename these methods
    virtual void vfn_3();         // @ 0x8216DBB8
    virtual void vfn_4();         // @ 0x8216DCB0
    virtual void vfn_6();         // @ 0x8216DB00
    virtual void vfn_7();         // @ 0x82177840
    virtual void vfn_9();         // @ 0x8216C200
    virtual void vfn_10();        // @ 0x8216BED0
    virtual void vfn_11();        // @ 0x8216DB88

protected:
    void** m_vtable;              // +0x00
    uint8_t m_data[288];          // +0x04-0x123
    uint32_t m_embeddedObject;    // +0x124 (292) - cleaned up in destructor
};

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewState — Character View UI State
// @ vtable 0x8205F174
// Inherits from: pongAttractState
// ────────────────────────────────────────────────────────────────────────────

/**
 * UI state machine state for character viewing/selection screens.
 * Handles input, transitions, and rendering for character selection UI.
 */
class pongCharViewState : public pongAttractState {
public:
    // Virtual methods
    
    /**
     * ~pongCharViewState @ 0x8230C490 | size: 0x68
     * Destructor
     */
    virtual ~pongCharViewState();
    
    /**
     * Update @ 0x8230C5F8 | size: 0xB8
     * Frame update override
     */
    virtual void Update(float deltaTime) override;
    
    /**
     * Render @ 0x8230C6B0 | size: 0xD8
     * Render character view UI
     */
    virtual void Render();
    
    /**
     * OnEnterState @ 0x8230A4E8 | size: 0xC
     * State entry callback
     */
    virtual void OnEnterState();
    
    /**
     * OnExitState @ 0x8230C4F8 | size: 0x100
     * State exit callback
     */
    virtual void OnExitState();

protected:
    // Inherits all fields from pongAttractState
    // Minimal additional data members
};

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewContext — Character View Context/Controller
// @ vtables 0x8205F1BC, 0x8205F1D4 (multiple inheritance)
// ────────────────────────────────────────────────────────────────────────────

/**
 * Context/controller for character view system. Manages the overall
 * character viewing experience including camera, UI, and state transitions.
 * 
 * Uses multiple inheritance (2 vtables) for different interfaces.
 * 
 * Virtual methods need better names - research ongoing.
 */
class pongCharViewContext {
public:
    // Virtual methods
    
    /**
     * ~pongCharViewContext @ 0x8230A600 | size: 0xAC
     * Destructor - cleans up managed object at +44 and embedded object at +80
     */
    virtual ~pongCharViewContext();
    
    // TODO: Research and rename these methods
    virtual void vfn_11();        // @ 0x8230A898
    virtual void vfn_12();        // @ 0x8230AB50
    virtual void vfn_16();        // @ 0x8230AC90
    virtual void vfn_17();        // @ 0x8230AF50
    virtual void vfn_18();        // @ 0x8230C0A8
    virtual void vfn_23();        // @ 0x8230A8F8

protected:
    void** m_vtable;              // +0x00 - primary vtable
    uint8_t m_data1[16];          // +0x04-0x13
    void** m_vtable2;             // +0x14 (20) - secondary vtable
    uint8_t m_data2[20];          // +0x18-0x2B
    void* m_pManagedObject;       // +0x2C (44) - deleted in destructor
    uint8_t m_data3[32];          // +0x30-0x4F
    uint32_t m_embeddedObject;    // +0x50 (80) - atArray_Clear called on it
};

// ────────────────────────────────────────────────────────────────────────────
// hudCharView — HUD Character View Component
// @ vtable 0x8206275C
// ────────────────────────────────────────────────────────────────────────────

/**
 * HUD component for displaying character information during gameplay.
 * Separate from the character selection screen views.
 * 
 * Virtual methods need better names - research ongoing.
 */
class hudCharView {
public:
    virtual ~hudCharView() = default;
    
    // TODO: Research and rename these methods
    virtual void vfn_2();         // @ 0x82320D68
    virtual void vfn_5();         // @ 0x82320BE0

protected:
    void** m_vtable;              // +0x00
};

