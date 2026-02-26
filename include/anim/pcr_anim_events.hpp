/**
 * pcrAnimEvent - Animation Event System
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Animation event system for triggering gameplay events from animation playback.
 * Events include footsteps, audio cues, messages, and vibration feedback.
 */

#pragma once

#include <cstdint>

// Forward declarations
class pcrAnimEventMsg;
class pcrAnimEventFootAud;

/**
 * pcrAnimEvent - Base animation event class
 * @ 0x820DE640 (vtable)
 * 
 * Base class for all animation events. Events are triggered during animation
 * playback and can trigger audio, haptics, messages, or other game effects.
 */
class pcrAnimEvent {
public:
    // Virtual destructor @ 0x820DE640
    virtual ~pcrAnimEvent();
    
    // Check if event matches type @ 0x820DE7D8 (slot 20)
    virtual bool MatchesType(uint32_t eventType);
    
    // Process event @ 0x820DE820 (slot 21)
    virtual void Process();
    
    // Constructor @ 0x820DE8A8
    pcrAnimEvent();
    
    // Fixup pointers after load @ 0x820DE8A8
    void FixupPointers(void* loadContext);
    
protected:
    void* m_vtable;           // +0x00: Virtual function table
    uint32_t m_flags;         // +0x04: Event flags
    uint32_t m_eventType;     // +0x08: Event type identifier
    uint32_t m_userData;      // +0x0C: User data
    uint32_t m_field_10;      // +0x10
    const char* m_name;       // +0x14: Event name (offset +20)
    uint32_t m_field_18;      // +0x18
    const char* m_field_1C;   // +0x1C: String field (offset +28)
    uint32_t m_field_20;      // +0x20
    const char* m_field_24;   // +0x24: String field (offset +36)
};

/**
 * pcrAnimEventFootAud - Footstep audio event
 * @ 0x820EBBD8 (vtable)
 * 
 * Triggers footstep audio based on surface type and foot contact.
 */
class pcrAnimEventFootAud : public pcrAnimEvent {
public:
    virtual ~pcrAnimEventFootAud();  // @ 0x820EBD48
    virtual bool MatchesType(uint32_t eventType) override;  // @ 0x820EBBD8
    virtual void Process() override;  // @ 0x820EBC30
    virtual void TriggerAudio();  // @ 0x820EBC80 (slot 23)
    virtual uint32_t GetSurfaceType();  // @ 0x820EBDA8 (slot 24)
};

/**
 * pcrAnimEventFootContact - Foot contact event
 */
class pcrAnimEventFootContact : public pcrAnimEventFootAud {
public:
    virtual void TriggerAudio() override;  // @ 0x820DEAD8 (slot 22)
    virtual uint32_t GetSurfaceType() override;  // @ 0x820EBDA8
};

/**
 * pcrAnimEventFootScuff - Foot scuff event
 */
class pcrAnimEventFootScuff : public pcrAnimEventFootAud {
public:
    virtual void TriggerAudio() override;  // @ 0x820DEAE8 (slot 22)
    virtual uint32_t GetSurfaceType() override;  // @ 0x820EBDB0
};

/**
 * pcrAnimEventFootStep - Foot step event
 */
class pcrAnimEventFootStep : public pcrAnimEventFootAud {
public:
    virtual void TriggerAudio() override;  // @ 0x820DEAF8 (slot 22)
    virtual uint32_t GetSurfaceType() override;  // @ 0x820EBDB8
};

/**
 * pcrAnimEventFootStomp - Foot stomp event
 */
class pcrAnimEventFootStomp : public pcrAnimEventFootAud {
public:
    virtual void TriggerAudio() override;  // @ 0x820DEB08 (slot 22)
    virtual uint32_t GetSurfaceType() override;  // @ 0x820EBDC0
};

/**
 * pcrAnimEventMsg - Message event
 * @ 0x820EBDC8 (vtable)
 * 
 * Sends a message to game systems when triggered.
 */
class pcrAnimEventMsg : public pcrAnimEvent {
public:
    virtual ~pcrAnimEventMsg();  // @ 0x820EBDC8
    virtual bool MatchesType(uint32_t eventType) override;  // @ 0x820EBE30
    virtual void Process() override;  // @ 0x820EBE88
    virtual void SendMessage();  // @ 0x820EBED8 (slot 2)
    virtual void OnTrigger();  // @ 0x820EBF30 (slot 23)
    virtual void PostTrigger();  // @ 0x820EBF50 (slot 24)
};

/**
 * pcrAnimEventMsg_s - String message event
 * @ 0x820EBF70 (vtable)
 */
class pcrAnimEventMsg_s : public pcrAnimEvent {
public:
    virtual ~pcrAnimEventMsg_s();  // @ 0x820EBF70
    virtual bool MatchesType(uint32_t eventType) override;  // @ 0x820EBFF0
    virtual void Process() override;  // @ 0x820EC010
    virtual void SendStringMessage();  // @ 0x820EC088 (slot 24)
};

// Event type constants (inferred from MatchesType checks)
extern uint32_t g_eventType_Default;      // @ 0x8271B0D4 (glob:lbl_8271B0D4)
extern uint32_t g_eventType_FootAudio;    // @ 0x8271B0D8 (glob:lbl_8271B0D8)
extern uint32_t g_eventType_Message;      // @ 0x8271B0DC (glob:lbl_8271B0DC)

// Utility functions
void util_E690();  // @ 0x820DE690 - Event cleanup utility
void game_8F58(pcrAnimEvent* event, uint32_t param1, void* param2, 
               const char* name, uint32_t flags, uint32_t userData);  // @ 0x821A8F58
