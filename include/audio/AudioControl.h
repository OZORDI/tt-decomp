/**
 * Audio Control System - Header
 * Rockstar Table Tennis (Xbox 360)
 * 
 * Base classes for audio control and 3D positioning.
 * Part of the RAGE audio subsystem.
 */

#pragma once

namespace rage {

/**
 * audControl - Base audio control class
 * 
 * Manages basic audio parameters like volume and pitch.
 * Used for controlling individual sound sources.
 */
class audControl {
public:
    virtual ~audControl() = default;
    
    // Get current volume (0.0 - 1.0)
    // @ 0x82160768
    virtual float GetVolume() const;
    
    // Get current pitch multiplier (typically 0.5 - 2.0)
    // @ 0x82160770
    virtual float GetPitch() const;
    
    // Check if audio control is active (has valid data)
    // @ 0x82160780
    virtual bool IsActive() const;
    
protected:
    char pad_0x00[0x10];    // Base class data
    void* audioData;        // Pointer to audio data/resource
    char pad_0x14[0x0C];    // More base data
    float volume;           // Volume level (0.0 - 1.0)
    char pad_0x24[0x08];    // Padding/other fields
    float pitch;            // Pitch multiplier
};

/**
 * audControl3d - 3D positioned audio control
 * 
 * Extends audControl with 3D spatial positioning.
 * Used for sounds that need to be positioned in 3D space
 * (e.g., ball hits, player sounds).
 */
class audControl3d : public audControl {
public:
    // Get the 3D position data
    // @ 0x82160760
    virtual void* GetPositionData() const;
    
    // Get distance attenuation factor
    // @ 0x82160778
    virtual float GetAttenuation() const;
    
    // Get effective volume (base volume * attenuation)
    // @ 0x82160EB0
    virtual float GetEffectiveVolume() const;
    
    // Get pitch with optional modification
    // @ 0x82160EC0
    virtual float GetModifiedPitch() const;
    
    // Check if audio is audible (not muted globally or locally)
    // @ 0x821609A8
    virtual bool IsAudible() const;
    
protected:
    void* positionData;     // Pointer to 3D position/transform
    char pad_0x0C[0x1C];    // Additional 3D audio data
    float attenuation;      // Distance attenuation factor
    char pad_0x2C[0x5C];    // More 3D audio data
    float pitchModifier;    // Pitch modification factor (offset 0x88)
    char pad_0x8C[0x04];    // Padding
    bool localMuteFlag;     // Local mute flag (offset 0x90)
};

/**
 * audControlGroup - Audio control group
 * 
 * Manages a group of audio controls together.
 * Used for categories like SFX, Music, Voice, etc.
 */
class audControlGroup {
public:
    virtual ~audControlGroup() = default;
    
    // Get the group type name (for debugging/RTTI)
    // @ 0x82162548
    virtual const char* GetGroupTypeName() const;
    
    // Get the group category name
    // @ 0x82162538
    virtual const char* GetCategoryName() const;
    
    // Get fade/blend value based on conditions
    // @ 0x82162888
    virtual float GetFadeValue() const;
    
    // Get group volume (0.0 - 1.0)
    // @ 0x82162528
    virtual float GetGroupVolume() const;
    
    // Get group priority (higher = more important)
    // @ 0x82162530
    virtual float GetPriority() const;
    
protected:
    char pad_0x00[0x1C];    // Group management data
    float groupVolume;      // Master volume for this group
    char pad_0x20[0x04];    // Padding
    float priority;         // Priority for voice stealing
    char pad_0x28[0x04];    // Padding
    bool fadeFlag1;         // Fade condition flag 1 (offset 0x2C)
    char pad_0x2D[0x03];    // Padding
    bool fadeFlag2;         // Fade condition flag 2 (offset 0x30)
};

/**
 * AudioHandle - Handle to audio hardware resource
 * 
 * Used to reference audio voices in the hardware layer.
 * The handle_id is checked for validity (-1 = invalid).
 */
struct AudioHandle {
    void* resourcePtr;      // +0x00: Pointer to audio resource
    int handle_id;          // +0x04: Hardware handle ID (-1 = invalid)
    // Additional fields...
};

/**
 * audVoice - Abstract audio voice base class
 * 
 * Represents an active audio voice (playing sound).
 * This is an abstract base class - concrete implementations
 * are audVoiceSfx (sound effects) and audVoiceStream (streaming audio).
 */
class audVoice {
public:
    virtual ~audVoice() = default;
    
    // Pure virtual functions - must be implemented by derived classes
    // These will call error handlers if invoked on base class
    
    // @ 0x82163190
    virtual void Play() = 0;
    
    // @ 0x821631A0
    virtual void Stop() = 0;
    
    // @ 0x821631B0
    virtual void Pause() = 0;
    
    // @ 0x821631C0
    virtual void Resume() = 0;
    
    // @ 0x821631D0
    virtual void Update() = 0;
    
protected:
    char pad_0x00[0x0C];
    void* hardwareVoice;    // Pointer to hardware voice structure
};

/**
 * Stream state structure
 * Tracks the current state of an audio stream
 */
struct StreamState {
    char pad_0x00[4];
    int currentState;  // 14 = playing, 15 = paused
};

/**
 * audVoiceSfx - Sound effect voice
 * 
 * Handles short sound effects that are fully loaded in memory.
 * Used for gameplay sounds like ball hits, UI sounds, etc.
 */
class audVoiceSfx : public audVoice {
public:
    // Set reverb volume (0.0 - 1.0)
    // @ 0x821639A0
    virtual void SetReverbVolume(float volume, const AudioHandle* handle);
    
    // Set effect volume (0.0 - 1.0)
    // @ 0x82163A28
    virtual void SetEffectVolume(float volume, const AudioHandle* handle);
};

/**
 * audVoiceStream - Streaming audio voice
 * 
 * Handles streaming audio like music and long sound effects.
 * Streams data from disk/memory in chunks rather than loading entirely.
 */
class audVoiceStream : public audVoice {
public:
    // Check if stream is currently playing
    // @ 0x821645B8
    virtual bool IsPlaying() const;
    
    // Check if stream is currently paused
    // @ 0x821645D8
    virtual bool IsPaused() const;
    
    // Set reverb volume (0.0 - 1.0)
    // @ 0x821643F0
    virtual void SetReverbVolume(float volume, const AudioHandle* handle);
    
    // Set effect volume (0.0 - 1.0)
    // @ 0x82164478
    virtual void SetEffectVolume(float volume, const AudioHandle* handle);
    
protected:
    char pad_0x00[0x0C];
    StreamState* state;  // Pointer to stream state
};

} // namespace rage
