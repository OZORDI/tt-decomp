/**
 * msgEventHandler - Message Event Handler Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "game/msg_event_handler.hpp"
#include "anim/pcr_anim_events.hpp"

// External dependencies
extern "C" void cmSampleCamMachineBank_65C0_g(void* queue, uint32_t size);

// Global event state
uint32_t g_messageEventType = 0;
uint32_t g_eventQueueIndex = 0;
uint8_t g_eventQueueFlags[256] = {0};
void* g_eventQueuePtrs[256] = {nullptr};
uint32_t g_eventQueueMasks[256] = {0};

/**
 * msgEventHandler::msgEventHandler (constructor)
 * @ 0x8225E8C0 | size: 0xD8
 * 
 * Initialize message event handler.
 */
msgEventHandler::msgEventHandler() {
    // Vtable set by compiler
    m_field_04 = 0;
    m_field_08 = 0;
    m_eventList = nullptr;
    m_field_10 = 0;
    m_queueSize = 0;
    
    // Initialize event queue
    for (int i = 0; i < 4; i++) {
        m_eventQueue[i] = 0;
    }
}

/**
 * msgEventHandler::Initialize
 * @ 0x8225E8C0 | size: 0xD8
 * 
 * Initialize the event handler system.
 */
void msgEventHandler::Initialize() {
    // Initialization handled in constructor
}

/**
 * msgEventHandler::~msgEventHandler
 * @ 0x8225EB50 | size: 0x80
 * 
 * Virtual destructor for message event handler.
 */
msgEventHandler::~msgEventHandler() {
    // Cleanup handled by base class
}

/**
 * msgEventHandler::Stub22
 * @ 0x8225EC58 | size: 0xC
 * 
 * Stub virtual function (slot 22).
 */
void msgEventHandler::Stub22() {
    // Empty stub
}

/**
 * msgEventHandler::MatchesType
 * @ 0x8225ED08 | size: 0x48
 * 
 * Check if this handler matches the given event type.
 */
bool msgEventHandler::MatchesType(uint32_t eventType) {
    return eventType == g_messageEventType;
}

/**
 * msgEventHandler::ProcessEvents
 * @ 0x8225ED50 | size: 0x100
 * 
 * Process all queued message events.
 * Iterates through event list, checks types, and queues matching events.
 */
void msgEventHandler::ProcessEvents() {
    // Get event list head
    pcrAnimEvent* currentEvent = m_eventList;
    
    // Clear event mask accumulator
    uint32_t eventMask = 0;
    
    // Iterate through event list
    while (currentEvent != nullptr) {
        // Check if event matches message type
        // Call virtual function at slot 20 (offset +80)
        bool matches = currentEvent->MatchesType(g_messageEventType);
        
        if (matches) {
            // Add event to queue
            uint32_t* queueBase = &m_eventQueue[0];
            
            // Grow queue if needed
            cmSampleCamMachineBank_65C0_g(queueBase, 16);  // 16 bytes per entry
            
            // Store event pointer in queue
            void** queueEntry = reinterpret_cast<void**>(queueBase);
            *queueEntry = currentEvent;
            
            // Get event priority/flags from event
            // Load from event+20 (m_field_14 in pcrAnimEvent)
            uint32_t* eventData = reinterpret_cast<uint32_t*>(currentEvent);
            uint32_t eventFlags = eventData[5];  // +20 bytes / 4
            
            // Calculate event bit mask
            if (eventFlags != 0) {
                // Extract bit position from flags
                uint32_t bitPos = (eventFlags >> 11) & 0x1FFFFF;  // rlwinm r6,r11,21,11,31
                uint32_t bitMask = 1 << bitPos;  // slw r9,r26,r6
                eventMask |= bitMask;
            }
        }
        
        // Move to next event in list
        // Events are linked at offset +8
        uint32_t* eventPtr = reinterpret_cast<uint32_t*>(currentEvent);
        currentEvent = reinterpret_cast<pcrAnimEvent*>(eventPtr[2]);  // +8 bytes / 4
    }
    
    // If any events were queued, add to global event queue
    if (eventMask != 0) {
        // Get current queue index
        uint32_t queueIndex = g_eventQueueIndex;
        
        // Store event queue pointer
        uint32_t queueOffset = queueIndex * 4;
        g_eventQueuePtrs[queueIndex] = &m_eventQueue[0];
        
        // Store event mask
        g_eventQueueMasks[queueIndex] = eventMask;
        
        // Set queue flag (0x80 = 128)
        g_eventQueueFlags[queueIndex] = 0x80;
        
        // Increment queue index
        g_eventQueueIndex = queueIndex + 1;
    }
}
