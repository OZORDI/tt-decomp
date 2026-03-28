/**
 * rage_audio.cpp — RAGE Audio System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Core audio voice and bank management for the RAGE engine.
 */

#include "rage_audio.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace rage {

// External engine entry points.
extern "C" void rage_DebugLog(const char* fmt, ...);
extern "C" float aud_2458(void* context);

extern void audVoiceStream_B328_fw(
    void* streamRef,
    int32_t bankSlot,
    void* bankEntryData,
    float volume,
    float pitch,
    float pan,
    float playVariance,
    uint32_t userParamA,
    uint32_t userParamB,
    uint8_t userParamC
);

// Binary globals resolved from pass5_final.
extern const float lbl_8202D110;      // randomization trigger
extern const float lbl_8202D998[3];   // randomization constants table
extern uint32_t lbl_825EE220;         // audio command write index
extern uint32_t* lbl_825EBD2C;        // audio command ring buffer

namespace {

constexpr int32_t kInvalidBankSlot = -1;
constexpr uint8_t kAudioCommandClass = 2;
constexpr uint16_t kSfxPlayCommandId = 16387;

const float& g_playVarianceThreshold = lbl_8202D110;
const float& g_playVarianceScale = lbl_8202D998[1];

uint32_t& g_audioCommandWriteIndex = lbl_825EE220;
uint32_t*& g_pAudioCommandRing = lbl_825EBD2C;

struct audCommandHeader {
    uint8_t m_class;
    uint8_t m_channel;
    uint16_t m_commandId;
};

class audSfxEntryView {
public:
    explicit audSfxEntryView(const void* rawEntry)
        : m_pRaw(static_cast<const uint8_t*>(rawEntry)) {}

    bool IsNull() const {
        return m_pRaw == nullptr;
    }

    int32_t GetBankSlot() const {
        return Read<int32_t>(kBankSlotOffset);
    }

    void* GetBankEntryData() const {
        return Read<void*>(kBankEntryOffset);
    }

    bool ShouldEmitPlayEvent() const {
        return Read<uint8_t>(kEmitPlayEventOffset) != 0;
    }

private:
    template <typename T>
    T Read(std::size_t offset) const {
        T value{};
        std::memcpy(&value, m_pRaw + offset, sizeof(value));
        return value;
    }

    static constexpr std::size_t kBankSlotOffset = 0x04;
    static constexpr std::size_t kBankEntryOffset = 0x08;
    static constexpr std::size_t kEmitPlayEventOffset = 0x0D;

    const uint8_t* m_pRaw;
};

void PushSfxPlayCommand(uint32_t streamHandle) {
    audCommandHeader* const header =
        reinterpret_cast<audCommandHeader*>(&g_pAudioCommandRing[g_audioCommandWriteIndex]);

    header->m_class = kAudioCommandClass;
    header->m_channel = kAudioCommandClass;
    header->m_commandId = kSfxPlayCommandId;

    g_pAudioCommandRing[g_audioCommandWriteIndex + 1] = streamHandle;
    g_audioCommandWriteIndex += 2;
}

} // namespace

void audVoiceSfx::PlayByEntry(
    void* entry,
    float volume,
    float pitch,
    float pan,
    float playVariance,
    uint32_t userParamA,
    uint32_t userParamB,
    uint8_t userParamC)
{
    const audSfxEntryView bankEntry(entry);
    if (bankEntry.IsNull() || bankEntry.GetBankSlot() == kInvalidBankSlot) {
        rage_DebugLog("audVoiceSfx::PlayByEntry - invalid entry (%p)", entry);  /* UNVERIFIED — string not found in binary */
        return;
    }

    if (playVariance > g_playVarianceThreshold) {
        const float randomSample = aud_2458(this);
        playVariance = randomSample * g_playVarianceScale;
    }

    if (bankEntry.ShouldEmitPlayEvent()) {
        uint32_t streamHandle = 0;
        std::memcpy(&streamHandle, m_pSfxRef, sizeof(streamHandle));
        PushSfxPlayCommand(streamHandle);
    }

    audVoiceStream_B328_fw(
        m_pSfxRef,
        bankEntry.GetBankSlot(),
        bankEntry.GetBankEntryData(),
        volume,
        pitch,
        pan,
        playVariance,
        userParamA,
        userParamB,
        userParamC
    );
}

// External memory free function
extern "C" void rage_free(void* ptr);

// ═════════════════════════════════════════════════════════════════════════════
// rage::audVoice — Audio Voice Base Class
// ═════════════════════════════════════════════════════════════════════════════




// ─────────────────────────────────────────────────────────────────────────────
// audVoice::~audVoice (vfn_0) @ 0x82163358 | size: 0x74
//
// Destructor for audVoice base class. Unlinks this voice from a doubly-linked
// list (likely a global voice manager list) and optionally frees the memory
// if the destructor flag indicates scalar deletion.
//
// The voice maintains prev/next pointers at offsets +4 and +8 for intrusive
// list management.
// ─────────────────────────────────────────────────────────────────────────────
audVoice::~audVoice() {
    // Update vtable to base audVoice vtable (for proper cleanup)
    // vtable = &audVoice_vtable; // Handled by compiler
    
    // Unlink from doubly-linked list
    // Structure: [vtable][prev*][next*]...
    
    void** prevNode = reinterpret_cast<void**>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + 4)
    );
    void** nextNode = reinterpret_cast<void**>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(this) + 8)
    );
    
    // If we have a previous node, update its next pointer
    if (prevNode) {
        prevNode[2] = nextNode;  // prev->next = this->next
    }
    
    // If we have a next node, update its prev pointer
    if (nextNode) {
        nextNode[1] = prevNode;  // next->prev = this->prev
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice pure-virtual stubs — base class logs error if called directly
// All confirmed by debug strings: "virtual audVoice::<Name> used"
// ─────────────────────────────────────────────────────────────────────────────

// @ 0x82037770 — "virtual audVoice::PlayByEntry used"
extern const char g_str_audVoice_PlayByEntry[];       // @ 0x82037770
// @ 0x82037794 — "virtual audVoice::Play (no args) used"
extern const char g_str_audVoice_Play[];              // @ 0x82037794
// @ 0x820377BC — "virtual audVoice::Stop used"
extern const char g_str_audVoice_Stop[];              // @ 0x820377BC
// @ 0x820377D8 — "virtual audVoice::Release used"
extern const char g_str_audVoice_Release[];           // @ 0x820377D8
// @ 0x820377F8 — "virtual audVoice::SetEffect used"
extern const char g_str_audVoice_SetEffect[];         // @ 0x820377F8
// @ 0x8203781C — "virtual audVoice::IsPlaying used"
extern const char g_str_audVoice_IsPlaying[];         // @ 0x8203781C

/**
 * audVoice::PlayByEntry @ 0x82163190 | size: 0xC
 * Pure virtual stub — logs error if base class version is called.
 */
void audVoice::PlayByEntry() {
    rage_DebugLog(g_str_audVoice_PlayByEntry);
}

/**
 * audVoice::Play @ 0x821631A0 | size: 0xC
 * Pure virtual stub — plays audio with no arguments.
 */
void audVoice::Play() {
    rage_DebugLog(g_str_audVoice_Play);
}

/**
 * audVoice::Stop @ 0x821631B0 | size: 0xC
 * Pure virtual stub — stops audio playback.
 */
void audVoice::Stop() {
    rage_DebugLog(g_str_audVoice_Stop);
}

/**
 * audVoice::Release @ 0x821631C0 | size: 0xC
 * Pure virtual stub — releases audio resources.
 */
void audVoice::Release() {
    rage_DebugLog(g_str_audVoice_Release);
}

/**
 * audVoice::SetEffect @ 0x821631D0 | size: 0xC
 * Pure virtual stub — applies an audio effect to the voice.
 */
void audVoice::SetEffect() {
    rage_DebugLog(g_str_audVoice_SetEffect);
}

/**
 * audVoice::IsPlaying @ 0x821631E0 | size: 0x2C
 * Pure virtual stub — returns 0 (not playing) if base version called.
 */
bool audVoice::IsPlaying() {
    rage_DebugLog(g_str_audVoice_IsPlaying);
    return false;
}





// ═════════════════════════════════════════════════════════════════════════════
// Batch 2 — Small audio accessor and delegation functions
// ═════════════════════════════════════════════════════════════════════════════

// External forward declarations for voice stream delegation functions.
extern void audVoiceStream_B430_fw(void* streamRef, int32_t flag);
extern void audVoiceStream_B298_fw(void* streamRef, int32_t flag);

// Binary globals for audio system state.
extern uint32_t* lbl_82066410;        // audSystem singleton pointer
extern uint8_t   lbl_82066418;        // audSystem mute flag

// Static .rdata references for audControlGroup type identifiers.
extern const char lbl_820356B0[];     // type name A
extern const char lbl_820356B4[];     // type name B




// ─────────────────────────────────────────────────────────────────────────────
// rage::audControl — Base audio control accessors
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audControl::GetVolume (vfn_11) @ 0x82160768 | size: 0x8
 * Returns the base volume value at field offset +0x20 (32).
 */
float audControl::GetVolume() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 32, sizeof(float));
    return value;
}

/**
 * audControl::GetPitch (vfn_12) @ 0x82160770 | size: 0x8
 * Returns the base pitch value at field offset +0x2C (44).
 */
float audControl::GetPitch() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 44, sizeof(float));
    return value;
}

/**
 * audControl::IsActive (vfn_19) @ 0x82160780 | size: 0x1C
 * Returns true if the control has an active reference (non-null pointer at +0x10).
 */
bool audControl::IsActive() {
    uint32_t ref;
    std::memcpy(&ref, reinterpret_cast<const char*>(this) + 16, sizeof(uint32_t));
    return ref != 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audControl3d — 3D spatialized audio control accessors
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audControl3d::GetControlRef (vfn_10) @ 0x82160760 | size: 0x8
 * Returns the control reference pointer stored at field offset +0x08 (8).
 */
void* audControl3d::GetControlRef() {
    void* ref;
    std::memcpy(&ref, reinterpret_cast<const char*>(this) + 8, sizeof(void*));
    return ref;
}

/**
 * audControl3d::GetPan (vfn_13) @ 0x82160778 | size: 0x8
 * Returns the pan value at field offset +0x28 (40).
 */
float audControl3d::GetPan() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 40, sizeof(float));
    return value;
}

/**
 * audControl3d::GetScaledVolume (vfn_11) @ 0x82160EB0 | size: 0x10
 * Returns volume scaled by the 3D attenuation factor.
 * Multiplies the attenuation at +0x7C (124) by the base volume at +0x20 (32).
 */
float audControl3d::GetScaledVolume() {
    float attenuation, baseVolume;
    std::memcpy(&attenuation, reinterpret_cast<const char*>(this) + 124, sizeof(float));
    std::memcpy(&baseVolume, reinterpret_cast<const char*>(this) + 32, sizeof(float));
    return attenuation * baseVolume;
}

/**
 * audControl3d::GetScaledPitch (vfn_12) @ 0x82160EC0 | size: 0x28
 * Returns pitch, optionally scaled by 3D Doppler factor.
 * If the global Doppler bypass flag (lbl_825EBCA0) is set, returns the raw
 * base pitch at +0x2C (44). Otherwise multiplies the Doppler scale at +0x88
 * (136) by the base pitch.
 */
float audControl3d::GetScaledPitch() {
    extern uint8_t lbl_825EBCA0;  // global Doppler bypass flag

    if (lbl_825EBCA0 != 0) {
        float basePitch;
        std::memcpy(&basePitch, reinterpret_cast<const char*>(this) + 44, sizeof(float));
        return basePitch;
    }

    float dopplerScale, basePitch;
    std::memcpy(&dopplerScale, reinterpret_cast<const char*>(this) + 136, sizeof(float));
    std::memcpy(&basePitch, reinterpret_cast<const char*>(this) + 44, sizeof(float));
    return dopplerScale * basePitch;
}


/**
 * audControl3d::IsAudible (vfn_19) @ 0x821609A8 | size: 0x34
 * Returns whether this 3D control is currently audible.
 * Checks the audio system's active flag via global singleton, then tests
 * the inaudible flag at +0x90 (144). Returns true only if the system is
 * active and the control is not flagged as inaudible.
 */
bool audControl3d::IsAudible() {
    uint32_t* pSystem = lbl_82066410;
    if (pSystem == nullptr) {
        return false;
    }

    uint8_t systemActive;
    std::memcpy(&systemActive, reinterpret_cast<const char*>(pSystem) + 4, sizeof(uint8_t));
    if (systemActive == 0) {
        return false;
    }

    uint8_t inaudibleFlag;
    std::memcpy(&inaudibleFlag, reinterpret_cast<const char*>(this) + 144, sizeof(uint8_t));
    return (inaudibleFlag == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audControlGroup — Audio control group accessors
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audControlGroup::GetVolume (vfn_9) @ 0x82162528 | size: 0x8
 * Returns the group volume at field offset +0x1C (28).
 */
float audControlGroup::GetVolume() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 28, sizeof(float));
    return value;
}

/**
 * audControlGroup::GetPitch (vfn_11) @ 0x82162530 | size: 0x8
 * Returns the group pitch at field offset +0x24 (36).
 */
float audControlGroup::GetPitch() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 36, sizeof(float));
    return value;
}

/**
 * audControlGroup::GetTypeNameA (vfn_7) @ 0x82162538 | size: 0xC
 * Returns a pointer to a static type identifier string in .rdata.
 */
const char* audControlGroup::GetTypeNameA() {
    return lbl_820356B0;
}

/**
 * audControlGroup::GetTypeNameB (vfn_6) @ 0x82162548 | size: 0xC
 * Returns a pointer to a second static type identifier string in .rdata.
 */
const char* audControlGroup::GetTypeNameB() {
    return lbl_820356B4;
}

/**
 * audControlGroup::GetEffectiveVolume (vfn_8) @ 0x82162888 | size: 0x3C
 * Returns the effective volume for the group, considering mute state.
 * If the global mute flag is clear, and the group has an active mute
 * override (+0x30, byte 48) with a ducking flag (+0x2C, byte 44),
 * returns the play-variance threshold (effectively 0.0) as muted volume.
 * Otherwise returns the stored group volume at +0x14 (20).
 */
float audControlGroup::GetEffectiveVolume() {
    if (lbl_82066418 != 0) {
        float volume;
        std::memcpy(&volume, reinterpret_cast<const char*>(this) + 20, sizeof(float));
        return volume;
    }

    uint8_t muteOverride;
    std::memcpy(&muteOverride, reinterpret_cast<const char*>(this) + 48, sizeof(uint8_t));
    if (muteOverride == 0) {
        float volume;
        std::memcpy(&volume, reinterpret_cast<const char*>(this) + 20, sizeof(float));
        return volume;
    }

    uint8_t duckingFlag;
    std::memcpy(&duckingFlag, reinterpret_cast<const char*>(this) + 44, sizeof(uint8_t));
    if (duckingFlag == 0) {
        float volume;
        std::memcpy(&volume, reinterpret_cast<const char*>(this) + 20, sizeof(float));
        return volume;
    }

    // Return the muted volume level (lbl_8202D110 is 0.0 threshold)
    return lbl_8202D110;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audVoiceSfx — SFX voice delegation functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audVoiceSfx::SetReverbSend (vfn_12) @ 0x82163918 | size: 0xC
 * Delegates to the stream engine's reverb send function with flag=0 (disable).
 * Passes the SFX reference pointer (m_pSfxRef at +0x0C) to the stream layer.
 */
void audVoiceSfx::SetReverbSend() {
    audVoiceStream_B430_fw(m_pSfxRef, 0);
}

/**
 * audVoiceSfx::EnableReverbSend (vfn_13) @ 0x82163928 | size: 0xC
 * Delegates to the stream engine's reverb send function with flag=1 (enable).
 */
void audVoiceSfx::EnableReverbSend() {
    audVoiceStream_B430_fw(m_pSfxRef, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audVoiceStream — Stream voice delegation and state query functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audVoiceStream::SetReverbSend (vfn_12) @ 0x821643D0 | size: 0xC
 * Delegates reverb send control to the stream engine with flag=0 (disable).
 * Uses the stream reference at +0x0C (field_0x000c).
 */
void audVoiceStream::SetReverbSend() {
    void* streamRef;
    std::memcpy(&streamRef, reinterpret_cast<const char*>(this) + 12, sizeof(void*));
    audVoiceStream_B298_fw(streamRef, 0);
}

/**
 * audVoiceStream::EnableReverbSend (vfn_13) @ 0x821643E0 | size: 0xC
 * Delegates reverb send control to the stream engine with flag=1 (enable).
 */
void audVoiceStream::EnableReverbSend() {
    void* streamRef;
    std::memcpy(&streamRef, reinterpret_cast<const char*>(this) + 12, sizeof(void*));
    audVoiceStream_B298_fw(streamRef, 1);
}

/**
 * audVoiceStream::IsStopping (vfn_19) @ 0x821645B8 | size: 0x20
 * Returns true if the stream is in the "stopping" state (state == 14).
 * Reads the state enum from the stream reference's field at +0x04.
 */
bool audVoiceStream::IsStopping() {
    void* streamRef;
    std::memcpy(&streamRef, reinterpret_cast<const char*>(this) + 12, sizeof(void*));

    int32_t state;
    std::memcpy(&state, reinterpret_cast<const char*>(streamRef) + 4, sizeof(int32_t));
    return state == 14;
}

/**
 * audVoiceStream::IsStopped (vfn_20) @ 0x821645D8 | size: 0x20
 * Returns true if the stream is in the "stopped" state (state == 15).
 * Reads the state enum from the stream reference's field at +0x04.
 */
bool audVoiceStream::IsStopped() {
    void* streamRef;
    std::memcpy(&streamRef, reinterpret_cast<const char*>(this) + 12, sizeof(void*));

    int32_t state;
    std::memcpy(&state, reinterpret_cast<const char*>(streamRef) + 4, sizeof(int32_t));
    return state == 15;
}


// ═════════════════════════════════════════════════════════════════════════════
// Batch 3 — audVoiceSfx command submission and state query functions
// ═════════════════════════════════════════════════════════════════════════════

// External forward declaration for the audio command submission helper.
// Writes a command header + payload into the global audio command ring buffer.
// Parameters: r3=commandClass, r4=commandId, r5=payloadPtr, r6=payloadWordCount
extern void audVoiceStream_AEE8_g(
    uint8_t commandClass,
    uint16_t commandId,
    const uint32_t* payloadData,
    uint8_t payloadWordCount
);

// .rdata float constants used for volume/level normalization.
extern const float lbl_8202D108;      // volume offset constant
extern const float lbl_8202D344;      // volume scale constant
extern const float lbl_8202D16C;      // speaker mix scale constant

// Global linked list head for free/available voice pool.
extern uint32_t lbl_825CA920[3];      // audVoiceSfx free list sentinel node

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::Pause (vfn_5) @ 0x821635B8 | size: 0x50
//
// Submits a Pause command (0x4004) to the audio command ring buffer with the
// stream handle from m_pSfxRef. Returns 1 (true) to indicate success.
// ─────────────────────────────────────────────────────────────────────────────
bool audVoiceSfx::Pause() {
    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    audCommandHeader* header =
        reinterpret_cast<audCommandHeader*>(&g_pAudioCommandRing[g_audioCommandWriteIndex]);

    header->m_class = kAudioCommandClass;
    header->m_channel = kAudioCommandClass;
    header->m_commandId = 0x4004;  // Pause

    g_pAudioCommandRing[g_audioCommandWriteIndex + 1] = streamHandle;
    g_audioCommandWriteIndex += 2;

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::Resume (vfn_6) @ 0x82163608 | size: 0x4C
//
// Submits a Resume command (0x4005) to the audio command ring buffer with
// the stream handle from m_pSfxRef.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::Resume() {
    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    audCommandHeader* header =
        reinterpret_cast<audCommandHeader*>(&g_pAudioCommandRing[g_audioCommandWriteIndex]);

    header->m_class = kAudioCommandClass;
    header->m_channel = kAudioCommandClass;
    header->m_commandId = 0x4005;  // Resume

    g_pAudioCommandRing[g_audioCommandWriteIndex + 1] = streamHandle;
    g_audioCommandWriteIndex += 2;
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::Stop (vfn_7) @ 0x82163658 | size: 0x4C
//
// Submits a Stop/Play command (0x4003) to the audio command ring buffer
// with the stream handle from m_pSfxRef.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::Stop() {
    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    audCommandHeader* header =
        reinterpret_cast<audCommandHeader*>(&g_pAudioCommandRing[g_audioCommandWriteIndex]);

    header->m_class = kAudioCommandClass;
    header->m_channel = kAudioCommandClass;
    header->m_commandId = kSfxPlayCommandId;  // 0x4003

    g_pAudioCommandRing[g_audioCommandWriteIndex + 1] = streamHandle;
    g_audioCommandWriteIndex += 2;
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetVolume (vfn_9) @ 0x82163778 | size: 0x60
//
// Converts a floating-point volume level to an integer representation using
// offset/scale constants and submits a SetVolume command (0x4006) via the
// audio command ring buffer.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetVolume(float volume) {
    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    float normalized = (volume - lbl_8202D108) * lbl_8202D344;
    int32_t intVolume = static_cast<int32_t>(normalized);

    uint32_t payload[2];
    payload[0] = streamHandle;
    payload[1] = static_cast<uint32_t>(intVolume);

    audVoiceStream_AEE8_g(kAudioCommandClass, 0x4006, payload, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetSpeakerMix (vfn_14) @ 0x82163938 | size: 0x64
//
// Converts two floating-point speaker mix parameters to integers using a
// shared scale constant and submits a SetSpeakerMix command (0x400C) with
// three payload words: stream handle + two scaled integer levels.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetSpeakerMix(float leftLevel, float rightLevel) {
    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    int32_t intLeft = static_cast<int32_t>(leftLevel * lbl_8202D16C);
    int32_t intRight = static_cast<int32_t>(rightLevel * lbl_8202D16C);

    uint32_t payload[3];
    payload[0] = streamHandle;
    payload[1] = static_cast<uint32_t>(intLeft);
    payload[2] = static_cast<uint32_t>(intRight);

    audVoiceStream_AEE8_g(kAudioCommandClass, 0x400C, payload, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetReverbVolume @ 0x821639A0 | size: 0x88
//
// Validates the bank entry's slot, then converts a float reverb volume to an
// integer and submits a SetReverbVolume command (0x400E). If the bank slot is
// invalid (-1), returns early (debug log stripped in release build).
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetReverbVolume(void* bankEntry, float volume) {
    int32_t bankSlot;
    std::memcpy(&bankSlot, reinterpret_cast<const char*>(bankEntry) + 4, sizeof(int32_t));

    if (bankSlot == -1) {
        // Debug log: "audVoiceSfx::SetReverbVolume - bank not resident for SFX entry (%p)"
        // Stripped to no-op in release build.
        return;
    }

    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    float normalized = (volume - lbl_8202D108) * lbl_8202D344;
    int32_t intVolume = static_cast<int32_t>(normalized);

    uint32_t payload[2];
    payload[0] = streamHandle;
    payload[1] = static_cast<uint32_t>(intVolume);

    audVoiceStream_AEE8_g(kAudioCommandClass, 0x400E, payload, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetEffectVolume @ 0x82163A28 | size: 0x88
//
// Validates the bank entry's slot, then converts a float effect volume to an
// integer and submits a SetEffectVolume command (0x4011). If the bank slot is
// invalid (-1), returns early (debug log stripped in release build).
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetEffectVolume(void* bankEntry, float volume) {
    int32_t bankSlot;
    std::memcpy(&bankSlot, reinterpret_cast<const char*>(bankEntry) + 4, sizeof(int32_t));

    if (bankSlot == -1) {
        // Debug log: "audVoiceSfx::SetEffectVolume - bank not resident for SFX entry (%p)"
        // Stripped to no-op in release build.
        return;
    }

    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    float normalized = (volume - lbl_8202D108) * lbl_8202D344;
    int32_t intVolume = static_cast<int32_t>(normalized);

    uint32_t payload[2];
    payload[0] = streamHandle;
    payload[1] = static_cast<uint32_t>(intVolume);

    audVoiceStream_AEE8_g(kAudioCommandClass, 0x4011, payload, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetEffect (vfn_17) @ 0x82163AB0 | size: 0x60
//
// Submits a SetEffect command (0x4010) with the stream handle, an effect ID,
// and an enable/disable flag as a three-word payload.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetEffect(uint32_t effectId, uint8_t enable) {
    uint32_t streamHandle;
    std::memcpy(&streamHandle, m_pSfxRef, sizeof(uint32_t));

    uint32_t payload[3];
    payload[0] = streamHandle;
    payload[1] = effectId;
    payload[2] = (enable != 0) ? 1u : 0u;

    audVoiceStream_AEE8_g(kAudioCommandClass, 0x4010, payload, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::IsPlaying (vfn_18) @ 0x82163B10 | size: 0x44
//
// Returns true if the underlying stream is in an active playback state.
// Reads the stream state from the SFX reference's field at +0x04 and checks
// against known playing states: 17, 18, 20, 21, 22.
// ─────────────────────────────────────────────────────────────────────────────
bool audVoiceSfx::IsPlaying() {
    void* sfxRef;
    std::memcpy(&sfxRef, reinterpret_cast<const char*>(this) + 12, sizeof(void*));

    int32_t state;
    std::memcpy(&state, reinterpret_cast<const char*>(sfxRef) + 4, sizeof(int32_t));

    return (state == 17 || state == 18 || state == 22 ||
            state == 20 || state == 21);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::DequeueFromList @ 0x821632F8 | size: 0x5C
//
// Removes the next voice node from this voice's intrusive linked list (at +0x08)
// and transfers it to the global free voice list (lbl_825CA920). If the list is
// empty (next points to self) or null, returns nullptr.
// Uses the linked list structure: [vtable +0x00] [prev +0x04] [next +0x08].
// ─────────────────────────────────────────────────────────────────────────────
void* audVoiceSfx::DequeueFromList() {
    // Read next pointer at +0x08 from this node
    uint32_t nextAddr;
    std::memcpy(&nextAddr, reinterpret_cast<const char*>(this) + 8, sizeof(uint32_t));

    // If next == this, list is empty
    if (nextAddr == reinterpret_cast<uintptr_t>(this)) {
        return nullptr;
    }

    void* nextNode = reinterpret_cast<void*>(nextAddr);
    if (nextNode == nullptr) {
        return nullptr;
    }

    // Unlink nextNode from this list:
    // this->next = nextNode->next
    uint32_t nextNextAddr;
    std::memcpy(&nextNextAddr, reinterpret_cast<const char*>(nextNode) + 8, sizeof(uint32_t));
    std::memcpy(reinterpret_cast<char*>(this) + 8, &nextNextAddr, sizeof(uint32_t));

    // nextNode->next->prev = this
    void* nextNext = reinterpret_cast<void*>(nextNextAddr);
    uint32_t thisAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this));
    std::memcpy(reinterpret_cast<char*>(nextNext) + 4, &thisAddr, sizeof(uint32_t));

    // Insert nextNode into the global free list (lbl_825CA920)
    uint32_t* freeListHead = lbl_825CA920;
    uint32_t freeListTail;
    std::memcpy(&freeListTail, reinterpret_cast<const char*>(freeListHead) + 4, sizeof(uint32_t));

    if (freeListTail != 0) {
        // tailNode->next = nextNode
        void* tailNode = reinterpret_cast<void*>(freeListTail);
        uint32_t nextNodeAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(nextNode));
        std::memcpy(reinterpret_cast<char*>(tailNode) + 8, &nextNodeAddr, sizeof(uint32_t));

        // Re-read tail in case it changed
        std::memcpy(&freeListTail, reinterpret_cast<const char*>(freeListHead) + 4, sizeof(uint32_t));
    }

    // nextNode->prev = freeListTail (may be 0)
    std::memcpy(reinterpret_cast<char*>(nextNode) + 4, &freeListTail, sizeof(uint32_t));

    // nextNode->next = freeListHead
    uint32_t headAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(freeListHead));
    std::memcpy(reinterpret_cast<char*>(nextNode) + 8, &headAddr, sizeof(uint32_t));

    // freeListHead->tail = nextNode
    uint32_t nextNodeAddr2 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(nextNode));
    std::memcpy(reinterpret_cast<char*>(freeListHead) + 4, &nextNodeAddr2, sizeof(uint32_t));

    return nextNode;
}

} // namespace rage
