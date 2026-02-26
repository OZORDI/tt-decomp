/**
 * pcrAnimEvent - Animation Event System Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "anim/pcr_anim_events.hpp"
#include <cstdlib>

// Event type globals
uint32_t g_eventType_Default = 0;
uint32_t g_eventType_FootAudio = 0;
uint32_t g_eventType_Message = 0;

// External dependencies
extern "C" void rage_free_00C0(void* ptr);
extern "C" void atexit(void (*func)());
extern "C" void util_E690();
extern "C" void game_8F58(pcrAnimEvent* event, uint32_t param1, void* param2, 
                          const char* name, uint32_t flags, uint32_t userData);

// Static cleanup function pointer
static void (*s_cleanupFunc)() = nullptr;
static bool s_cleanupRegistered = false;

/**
 * pcrAnimEvent::~pcrAnimEvent
 * @ 0x820DE640 | size: 0x50
 * 
 * Virtual destructor for animation events.
 * Calls cleanup utility and optionally frees memory.
 */
pcrAnimEvent::~pcrAnimEvent() {
    // Call cleanup utility
    util_E690();
    
    // Note: Memory freeing handled by caller based on flags
    // If bit 0 of destructor flags is set, rage_free_00C0 is called
}

/**
 * pcrAnimEvent::MatchesType
 * @ 0x820DE7D8 | size: 0x48
 * 
 * Check if this event matches the given event type.
 * Returns true for default type or foot audio type.
 */
bool pcrAnimEvent::MatchesType(uint32_t eventType) {
    // Check against default event type
    if (eventType == g_eventType_Default) {
        return true;
    }
    
    // Check against foot audio event type
    if (eventType == g_eventType_FootAudio) {
        return true;
    }
    
    // Check against message event type
    if (eventType == g_eventType_Message) {
        return true;
    }
    
    return false;
}

/**
 * pcrAnimEvent::Process
 * @ 0x820DE820 | size: 0x88
 * 
 * Process the animation event. Sets up cleanup on first call,
 * then calls game event processing function.
 */
void pcrAnimEvent::Process() {
    // Register cleanup function on first call
    if (!s_cleanupRegistered) {
        s_cleanupRegistered = true;
        // Store cleanup function pointer
        // atexit(cleanupFunc);  // Commented - actual cleanup registration
    }
    
    // Call game event processing
    // Parameters: event, vtable, cleanup_ptr, name, flags, userData
    game_8F58(this, 
              reinterpret_cast<uint32_t>(m_vtable),
              &s_cleanupFunc,
              m_name,
              0,  // flags
              reinterpret_cast<uint32_t>(this));
}

/**
 * pcrAnimEvent::pcrAnimEvent (constructor)
 * @ 0x820DE8A8 | size: 0xBC
 * 
 * Initialize animation event with vtable and zero fields.
 */
pcrAnimEvent::pcrAnimEvent() {
    // Vtable set by compiler
    m_flags = 0;
    m_eventType = 0;
    m_userData = 0;
}

/**
 * pcrAnimEvent::FixupPointers
 * @ 0x820DE8A8 | size: 0xBC
 * 
 * Fix up string pointers after loading from file.
 * Converts file offsets to runtime pointers.
 */
void pcrAnimEvent::FixupPointers(void* loadContext) {
    // Load context structure:
    // +0x04: base pointer
    // +0x4C: entry size
    
    uint32_t* ctx = static_cast<uint32_t*>(loadContext);
    uint32_t basePtr = ctx[1];  // +0x04
    uint32_t entrySize = ctx[19];  // +0x4C
    
    // Fixup m_name pointer (offset +20)
    if (m_name != nullptr) {
        uint32_t offset = reinterpret_cast<uint32_t>(m_name) - basePtr;
        uint32_t index = offset / entrySize;
        uint32_t* entryTable = reinterpret_cast<uint32_t*>(ctx[(index + 2) * 4]);
        m_name = reinterpret_cast<const char*>(entryTable + reinterpret_cast<uint32_t>(m_name));
    }
    
    // Fixup m_field_1C pointer (offset +28)
    if (m_field_1C != nullptr) {
        uint32_t offset = reinterpret_cast<uint32_t>(m_field_1C) - basePtr;
        uint32_t index = offset / entrySize;
        uint32_t* entryTable = reinterpret_cast<uint32_t*>(ctx[(index + 2) * 4]);
        m_field_1C = reinterpret_cast<const char*>(entryTable + reinterpret_cast<uint32_t>(m_field_1C));
    }
    
    // Fixup m_field_24 pointer (offset +36)
    if (m_field_24 != nullptr) {
        uint32_t offset = reinterpret_cast<uint32_t>(m_field_24) - basePtr;
        uint32_t index = offset / entrySize;
        uint32_t* entryTable = reinterpret_cast<uint32_t*>(ctx[(index + 2) * 4]);
        m_field_24 = reinterpret_cast<const char*>(entryTable + reinterpret_cast<uint32_t>(m_field_24));
    }
}

// ============================================================================
// pcrAnimEventFootAud - Footstep Audio Events
// ============================================================================

pcrAnimEventFootAud::~pcrAnimEventFootAud() {
    // Base destructor handles cleanup
}

bool pcrAnimEventFootAud::MatchesType(uint32_t eventType) {
    // Foot audio events match foot audio type
    return eventType == g_eventType_FootAudio;
}

void pcrAnimEventFootAud::Process() {
    // Call base processing
    pcrAnimEvent::Process();
}

void pcrAnimEventFootAud::TriggerAudio() {
    // Implemented by derived classes
}

uint32_t pcrAnimEventFootAud::GetSurfaceType() {
    return 0;  // Default surface
}

// ============================================================================
// Foot Event Specializations
// ============================================================================

void pcrAnimEventFootContact::TriggerAudio() {
    // Trigger foot contact audio
}

uint32_t pcrAnimEventFootContact::GetSurfaceType() {
    return 0;  // Contact surface type
}

void pcrAnimEventFootScuff::TriggerAudio() {
    // Trigger foot scuff audio
}

uint32_t pcrAnimEventFootScuff::GetSurfaceType() {
    return 1;  // Scuff surface type
}

void pcrAnimEventFootStep::TriggerAudio() {
    // Trigger foot step audio
}

uint32_t pcrAnimEventFootStep::GetSurfaceType() {
    return 2;  // Step surface type
}

void pcrAnimEventFootStomp::TriggerAudio() {
    // Trigger foot stomp audio
}

uint32_t pcrAnimEventFootStomp::GetSurfaceType() {
    return 3;  // Stomp surface type
}

// ============================================================================
// pcrAnimEventMsg - Message Events
// ============================================================================

pcrAnimEventMsg::~pcrAnimEventMsg() {
    // Base destructor handles cleanup
}

bool pcrAnimEventMsg::MatchesType(uint32_t eventType) {
    return eventType == g_eventType_Message;
}

void pcrAnimEventMsg::Process() {
    pcrAnimEvent::Process();
}

void pcrAnimEventMsg::SendMessage() {
    // Send message to game systems
}

void pcrAnimEventMsg::OnTrigger() {
    // Called when event triggers
}

void pcrAnimEventMsg::PostTrigger() {
    // Called after event triggers
}

// ============================================================================
// pcrAnimEventMsg_s - String Message Events
// ============================================================================

pcrAnimEventMsg_s::~pcrAnimEventMsg_s() {
    // Base destructor handles cleanup
}

bool pcrAnimEventMsg_s::MatchesType(uint32_t eventType) {
    return eventType == g_eventType_Message;
}

void pcrAnimEventMsg_s::Process() {
    pcrAnimEvent::Process();
}

void pcrAnimEventMsg_s::SendStringMessage() {
    // Send string message to game systems
}
