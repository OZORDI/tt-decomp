/**
 * msgEventHandler - Message Event Handler System
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Handles message events from animation system and dispatches them to game systems.
 */

#pragma once

#include <cstdint>

// Forward declarations
class pcrAnimEvent;

/**
 * msgEventHandler - Message event handler
 * @ 0x8225E8C0 (construction function)
 * 
 * Manages a list of message events and dispatches them to appropriate handlers.
 * Events are filtered by type and queued for processing.
 */
class msgEventHandler {
public:
    // Virtual destructor @ 0x8225EB50
    virtual ~msgEventHandler();
    
    // Stub virtual function @ 0x8225EC58 (slot 22)
    virtual void Stub22();
    
    // Match event type @ 0x8225ED08 (slot 20)
    virtual bool MatchesType(uint32_t eventType);
    
    // Process events @ 0x8225ED50 (slot 3)
    virtual void ProcessEvents();
    
    // Constructor @ 0x8225E8C0
    msgEventHandler();
    
    // Initialize handler @ 0x8225E8C0
    void Initialize();
    
protected:
    void* m_vtable;           // +0x00: Virtual function table
    uint32_t m_field_04;      // +0x04
    uint32_t m_field_08;      // +0x08
    pcrAnimEvent* m_eventList;  // +0x0C: Linked list of events
    uint32_t m_field_10;      // +0x10
    uint32_t m_eventQueue[4]; // +0x14-0x20: Event queue (grows dynamically)
    uint32_t m_queueSize;     // +0x24: Current queue size
};

// External dependencies
extern "C" void cmSampleCamMachineBank_65C0_g(void* queue, uint32_t size);

// Global event type
extern uint32_t g_messageEventType;  // @ 0x8271B800 (glob:lbl_8271B800)

// Global event queue state
extern uint32_t g_eventQueueIndex;   // @ 0x8271A864 (glob:lbl_8271A864)
extern uint8_t g_eventQueueFlags[256];  // @ 0x8271A968 (glob:lbl_8271A968)
extern void* g_eventQueuePtrs[256];     // @ 0x8271A868 (glob:lbl_8271A868)
extern uint32_t g_eventQueueMasks[256]; // @ 0x8271AAE8 (glob:lbl_8271AAE8)
