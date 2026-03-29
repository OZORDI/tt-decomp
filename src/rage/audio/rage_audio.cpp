/**
 * rage_audio.cpp — RAGE Audio System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Core audio voice and bank management for the RAGE engine.
 */

#include "rage_audio.hpp"
#include "game/pong_strings.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace rage {

// ═════════════════════════════════════════════════════════════════════════════
// File-scope extern declarations (consolidated from body-scoped externs)
// ═════════════════════════════════════════════════════════════════════════════

// External engine entry points.
extern "C" void audDebugLog(const char* fmt, ...);
extern "C" float audRandom_Float(void* context);

// Audio voice delegation functions.
extern void audVoiceSfx_PlayInternal(
    void* streamRef, int32_t bankSlot, void* bankEntryData,
    float volume, float pitch, float pan, float playVariance,
    uint32_t userParamA, uint32_t userParamB, uint8_t userParamC);
extern void audVoiceNonStream_Play(void* streamRef, int32_t bankSlot, uint32_t bankEntryData,
                     float volume, float pitch, float pan, float playVariance,
                     uint32_t userParamA, uint32_t userParamB, uint8_t userParamC);
extern void audVoiceSfx_SetPaused(void* sfxRef, int flag);
extern void audVoiceStream_SetPaused(void* streamRef, int flag);

// Float constant tables.
extern const float g_audZeroConst;      // randomization trigger / zero constant
extern const float g_audRandomConstants[3];   // randomization constants table
extern const float g_audVolumeScaleFactor;      // volume scale factor
extern const float g_audVolumeOffset;      // volume offset (subtracted before scaling)

// Audio command ring buffer globals.
extern uint32_t g_audCommandWriteIndex;         // audio command write index
extern uint32_t* g_pAudCommandRingBuffer;        // audio command ring buffer

// Voice free list sentinels.
extern uint8_t g_audSfxVoiceFreeList[];        // SFX voice free list sentinel @ 0x825F5758
extern uint8_t g_audStreamVoiceFreeList[];        // Stream voice free list sentinel @ 0x825F57C0
extern uint32_t* g_pAudSystemActiveFlag;        // Audio system active flag pointer @ 0x825F5810

// Vtable pointers.
extern void* g_audVoice_vtable[];          // audVoice base vtable
extern void* g_audVoiceStream_vtable_intermediate[];          // audVoiceStream vtable (intermediate)
extern void* g_audVoiceStream_vtable[];          // audVoiceStream vtable (final)
extern void* g_audSystem_vtable_A[];          // audSystem intermediate vtable A
extern void* g_audSystem_vtable_B[];          // audSystem intermediate vtable B
extern void* g_msgMsgSink_vtable[];          // audSystem final (base) vtable
extern void* g_audBankMgr_vtable_intermediate[];          // audBankMgr vtable (intermediate)
extern void* g_audBankMgr_vtable[];          // audBankMgr vtable (final/base)

// audControlGroup type name strings.
extern const char g_audControlGroupTypeName_short[];
extern const char g_audControlGroupTypeName[];

// audSystem / audBankMgr helpers.
extern void audSystem_DestroyInternal(void* self);
extern void rage_free(void* ptr);
extern void audHashTable_Destroy(void* hashTable);

// audControl3d globals.
extern uint8_t g_audControl3dArray[];    // Global array of 4 audControl3d objects
extern void audControl3d_Initialize(void* control);

// Bank entry type check globals.
extern uint32_t* g_pAudBankEntryData;    // Audio bank entry data base @ 0x825EE224

// Audio scene globals.
extern void* g_pAudSceneSingleton;            // Audio scene singleton @ 0x825E7864
extern void audScene_Shutdown(void* self);
extern void atList_InsertItem(void* list, void* obj, uint32_t flags);
extern uint8_t g_audSceneList[];        // Audio scene list @ 0x825E7878

namespace {

constexpr int32_t kInvalidBankSlot = -1;
constexpr uint8_t kAudioCommandClass = 2;
constexpr uint16_t kSfxPlayCommandId = 16387;

const float& g_playVarianceThreshold = g_audZeroConst;
const float& g_playVarianceScale = g_audRandomConstants[1];

uint32_t& g_audioCommandWriteIndex = g_audCommandWriteIndex;
uint32_t*& g_pAudioCommandRing = g_pAudCommandRingBuffer;

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
        audDebugLog("audVoiceSfx::PlayByEntry - invalid entry (%p)", entry);
        return;
    }

    if (playVariance > g_playVarianceThreshold) {
        const float randomSample = audRandom_Float(this);
        playVariance = randomSample * g_playVarianceScale;
    }

    if (bankEntry.ShouldEmitPlayEvent()) {
        uint32_t streamHandle = 0;
        std::memcpy(&streamHandle, m_pSfxRef, sizeof(streamHandle));
        PushSfxPlayCommand(streamHandle);
    }

    audVoiceSfx_PlayInternal(
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
// audVoice::Stop @ 0x82163190 | size: 0xC
//
// Unimplemented stub - logs a message and returns.
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::Stop() {
    audDebugLog(reinterpret_cast<const char*>(0x82035770));
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::StopImmediate @ 0x821631A0 | size: 0xC
//
// Unimplemented stub - logs a message and returns.
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::StopImmediate() {
    audDebugLog(reinterpret_cast<const char*>(0x82035794));
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::Play @ 0x821631B0 | size: 0xC
//
// Unimplemented stub - logs format string "char_%l_%s".
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::Play() {
    audDebugLog(g_str_audVoiceSfx_formatChar);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::StopAndRelease @ 0x821631C0 | size: 0xC
//
// Unimplemented stub - logs format string "_%s".
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::StopAndRelease() {
    audDebugLog(g_str_audVoiceSfx_formatSuffix);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::SetLooping @ 0x821631D0 | size: 0xC
//
// Unimplemented stub - logs format string "_%l".
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::SetLooping(uint32_t handle, bool loop) {
    audDebugLog(g_str_audVoiceSfx_formatLevel);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::IsPlaying @ 0x821631E0 | size: 0x2C
//
// Unimplemented stub - logs a message and returns false.
// ─────────────────────────────────────────────────────────────────────────────
bool audVoice::IsPlaying() {
    audDebugLog(g_str_audVoiceSfx_ankle);
    return false;
}

// ═════════════════════════════════════════════════════════════════════════════
// Audio Command Ring Buffer System
// ═════════════════════════════════════════════════════════════════════════════

namespace {

uint32_t& g_cmdWriteIndex = g_audCommandWriteIndex;  // reuses file-scope extern
uint32_t*& g_pCmdRing     = g_pAudCommandRingBuffer;  // reuses file-scope extern

// Packed command header written into the ring buffer as a single uint32_t:
//   byte 0: command class (voice type)
//   byte 1: parameter count + 1
//   bytes 2-3: command ID
void PushAudioCommand(uint8_t cmdClass, uint16_t cmdId, uint8_t paramCount,
                      const uint32_t* params) {
    uint32_t idx = g_cmdWriteIndex;
    uint32_t* slot = &g_pCmdRing[idx];

    // Write header: class | (paramCount+1) | cmdId
    uint8_t* hdr = reinterpret_cast<uint8_t*>(slot);
    hdr[0] = cmdClass;
    hdr[1] = paramCount + 1;
    *reinterpret_cast<uint16_t*>(hdr + 2) = cmdId;

    idx += 1;

    // Copy parameter words
    if (paramCount > 0) {
        uint32_t* dst = &g_pCmdRing[idx];
        for (uint8_t i = 0; i < paramCount; i++) {
            dst[i] = params[i];
        }
        idx += paramCount;
    }

    g_cmdWriteIndex = idx;
}

} // namespace

// ═════════════════════════════════════════════════════════════════════════════
// audVoiceSfx — SFX Voice Functions
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceFreeList_Dequeue @ 0x821632F8 | size: 0x5C
//
// Dequeues the first voice node from an intrusive doubly-linked list (the
// voice free list), then re-links it into a second list (the active voice
// list sentinel at g_audSfxVoiceFreeList+4). Returns the dequeued node, or nullptr
// if the list is empty or points to itself (sentinel-only).
// ─────────────────────────────────────────────────────────────────────────────
void* audVoiceFreeList_Dequeue(void* listHead) // @ 0x821632F8
{
    uint32_t* head = static_cast<uint32_t*>(listHead);
    uint32_t* node = reinterpret_cast<uint32_t*>(head[2]); // head->next (+8)

    // If next points back to head, list is empty
    if (node == head) {
        return nullptr;
    }
    if (node == nullptr) {
        return nullptr;
    }

    // Unlink node from source list: head->next = node->next
    uint32_t* nodeNext = reinterpret_cast<uint32_t*>(node[2]);
    head[2] = reinterpret_cast<uintptr_t>(nodeNext);
    // nodeNext->prev = head
    nodeNext = reinterpret_cast<uint32_t*>(node[2]);
    nodeNext[1] = reinterpret_cast<uintptr_t>(head);

    // Insert into active list at g_audSfxVoiceFreeList+4 (sentinel)
    uint32_t* activeSentinel = reinterpret_cast<uint32_t*>(&g_audSfxVoiceFreeList[4]);
    uint32_t* activeTail = reinterpret_cast<uint32_t*>(activeSentinel[1]); // sentinel->prev

    if (activeTail != nullptr) {
        activeTail[2] = reinterpret_cast<uintptr_t>(node); // tail->next = node
        activeTail = reinterpret_cast<uint32_t*>(activeSentinel[1]);
    }

    node[1] = reinterpret_cast<uintptr_t>(activeTail);             // node->prev = tail
    node[2] = reinterpret_cast<uintptr_t>(activeSentinel);         // node->next = sentinel
    activeSentinel[1] = reinterpret_cast<uintptr_t>(node);         // sentinel->prev = node

    return node;
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream_GetStreamRef @ 0x82163D28 | size: 0x60
//
// Allocates a voice node from the stream free list by calling
// audVoiceFreeList_Dequeue. If allocation fails and the audio system is active,
// logs a warning. Returns the allocated node (or nullptr).
// ─────────────────────────────────────────────────────────────────────────────

void* audVoiceStream_GetStreamRef() // @ 0x82163D28
{
    void* node = audVoiceFreeList_Dequeue(&g_audStreamVoiceFreeList[0]);
    if (node == nullptr) {
        // Check if audio system is active (flag at offset +4 of the pointer)
        uint8_t* sysPtr = reinterpret_cast<uint8_t*>(g_pAudSystemActiveFlag);
        if (sysPtr != nullptr && sysPtr[4] != 0) {
            audDebugLog("audVoiceStream::GetStreamRef - no free stream voices");
        }
    }
    return node;
}

// ─────────────────────────────────────────────────────────────────────────────
// audCommand_Enqueue @ 0x821AAEE8 | size: 0x70
//
// Low-level audio command enqueue. Writes a command header (class, id,
// param count) and copies parameter words into the ring buffer.
// ─────────────────────────────────────────────────────────────────────────────
void audCommand_Enqueue(uint8_t cmdClass, uint16_t cmdId,  // @ 0x821AAEE8
                           const uint32_t* params, uint8_t paramCount)
{
    PushAudioCommand(cmdClass, cmdId, paramCount, params);
}

// ─────────────────────────────────────────────────────────────────────────────
// audCommand_SetEnabled @ 0x821AAF58 | size: 0x54
//
// Sends a boolean-valued audio command (cmdId 4124 = 0x101C).
// Packs {voiceHandle, boolValue} as two uint32_t params.
// Used to enable/disable a voice stream.
// ─────────────────────────────────────────────────────────────────────────────
void audCommand_SetEnabled(uint32_t voiceHandle, bool enabled) // @ 0x821AAF58
{
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = enabled ? 1u : 0u;

    audCommand_Enqueue(1, 4124, params, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audCommand_SetVolumePair @ 0x821AAFB0 | size: 0x6C
//
// Sends a volume-pair command (cmdId 4125 = 0x101D).
// Converts two float volumes (0.0..1.0 range) to integer scale and packs
// {voiceHandle, scaledVol1, scaledVol2} as three uint32_t params.
// ─────────────────────────────────────────────────────────────────────────────
void audCommand_SetVolumePair(uint32_t voiceHandle, float vol1, float vol2) // @ 0x821AAFB0
{
    uint32_t params[3];
    params[0] = voiceHandle;

    float offset = g_audVolumeOffset;
    float scale  = g_audVolumeScaleFactor;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((vol1 - offset) * scale));
    params[2] = static_cast<uint32_t>(static_cast<int32_t>((vol2 - offset) * scale));

    audCommand_Enqueue(1, 4125, params, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetReverbVolume @ 0x821639A0 | size: 0x88
//
// Sets reverb send volume on an SFX voice. Reads the bank entry from the
// voice's sfxRef, validates it, converts the float volume to integer scale,
// and enqueues a reverb volume command (cmdId 16398 = 0x400E).
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetReverbVolume(void* bankEntry, float volume) // @ 0x821639A0
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        audDebugLog("audVoiceSfx::SetReverbVolume - bank not resident for SFX entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* sfxData = static_cast<uint32_t*>(m_pSfxRef);
    uint32_t voiceHandle = sfxData[0];

    float offset = g_audVolumeOffset;
    float scale  = g_audVolumeScaleFactor;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audCommand_Enqueue(2, 16398, params, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx::SetEffectVolume @ 0x82163A28 | size: 0x88
//
// Sets effect send volume on an SFX voice. Same structure as SetReverbVolume
// but uses cmdId 16401 = 0x4011.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceSfx::SetEffectVolume(void* bankEntry, float volume) // @ 0x82163A28
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        audDebugLog("audVoiceSfx::SetEffectVolume - bank not resident for SFX entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* sfxData = static_cast<uint32_t*>(m_pSfxRef);
    uint32_t voiceHandle = sfxData[0];

    float offset = g_audVolumeOffset;
    float scale  = g_audVolumeScaleFactor;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audCommand_Enqueue(2, 16401, params, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream::SetReverbVolume @ 0x821643F0 | size: 0x88
//
// Sets reverb send volume on a stream voice. Same pattern as SFX version
// but uses cmdClass=3 and cmdId 8207 = 0x200F.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream::SetReverbVolume(void* bankEntry, float volume) // @ 0x821643F0
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        audDebugLog("audVoiceStream::SetReverbVolume - bank not resident for stream entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* streamData = reinterpret_cast<uint32_t*>(m_pStreamRef);
    uint32_t voiceHandle = streamData[0];

    float offset = g_audVolumeOffset;
    float scale  = g_audVolumeScaleFactor;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audCommand_Enqueue(3, 8207, params, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream::SetEffectVolume @ 0x82164478 | size: 0x88
//
// Sets effect send volume on a stream voice. Same pattern as SetReverbVolume
// but uses cmdId 8209 = 0x2011.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream::SetEffectVolume(void* bankEntry, float volume) // @ 0x82164478
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        audDebugLog("audVoiceStream::SetEffectVolume - bank not resident for stream entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* streamData = reinterpret_cast<uint32_t*>(m_pStreamRef);
    uint32_t voiceHandle = streamData[0];

    float offset = g_audVolumeOffset;
    float scale  = g_audVolumeScaleFactor;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audCommand_Enqueue(3, 8209, params, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream::Prime @ 0x82163E50 | size: 0x84
//
// Primes a stream voice for playback. Sends a prime command (cmdId 8194 =
// 0x2002) with the bank entry slot, data pointer, and voice handle.
// Then sets the stream state to 14 (primed).
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream::Prime(void* bankEntry) // @ 0x82163E50
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        audDebugLog("audVoiceStream::Prime - bank not resident for audBankEntry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* streamData = reinterpret_cast<uint32_t*>(m_pStreamRef);
    uint8_t* entryBytes = static_cast<uint8_t*>(bankEntry);
    uint32_t entryDataPtr = *reinterpret_cast<uint32_t*>(entryBytes + 8);

    uint32_t params[3];
    params[0] = streamData[0];   // voice handle
    params[1] = static_cast<uint32_t>(bankSlot);
    params[2] = entryDataPtr;

    audCommand_Enqueue(3, 8194, params, 3);

    // Set stream state to primed
    streamData[1] = 14;
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream::PlayByEntry @ 0x82163ED8 | size: 0xC8
//
// Plays a stream voice using a bank entry. Validates the bank slot, applies
// play variance randomization if needed, then delegates to the internal
// stream play function.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream::PlayByEntry(void* bankEntry, float volume, float pitch, // @ 0x82163ED8
                                  float pan, float playVariance,
                                  uint32_t userParamA, uint32_t userParamB,
                                  uint8_t userParamC)
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        audDebugLog("audVoiceStream::PlayByEntry - bank not resident for audBankEntry (%p)",
                      bankEntry);
        return;
    }

    if (playVariance > g_audZeroConst) {
        float randomSample = audRandom_Float(this);
        playVariance = randomSample * g_audRandomConstants[1];
    }

    uint32_t bankEntryData = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 8);

    audVoiceNonStream_Play(reinterpret_cast<void*>(m_pStreamRef), bankSlot, bankEntryData,
             volume, pitch, pan, playVariance,
             userParamA, userParamB, userParamC);
}

// ═════════════════════════════════════════════════════════════════════════════
// audVoiceStream Constructor / audSystem Destructor / audBankMgr Destructor
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream_Construct @ 0x82163BA8 | size: 0xA0
//
// Constructor for audVoiceStream. Sets the vtable, enqueues an audio command
// to register the voice (cmdId 8199 = 0x2007), then re-links the node in
// the intrusive list to use the stream vtable.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream_Construct(void* self) // @ 0x82163BA8
{
    uint32_t* voice = static_cast<uint32_t*>(self);

    // Read the ref pointer at +12
    uint32_t* refPtr = reinterpret_cast<uint32_t*>(voice[3]);

    // Write intermediate vtable
    voice[0] = reinterpret_cast<uintptr_t>(g_audVoiceStream_vtable_intermediate);

    // Enqueue voice registration command
    uint32_t voiceHandle = refPtr[0];
    uint32_t params[2];
    params[0] = voiceHandle;

    // cmdClass=3, cmdId=8199, 2 params
    audCommand_Enqueue(3, 8199, params, 2);

    // Set final vtable
    voice[0] = reinterpret_cast<uintptr_t>(g_audVoiceStream_vtable);

    // Re-link prev/next in intrusive list
    uint32_t* prev = reinterpret_cast<uint32_t*>(voice[1]);
    if (prev != nullptr) {
        uint32_t* next = reinterpret_cast<uint32_t*>(voice[2]);
        prev[2] = reinterpret_cast<uintptr_t>(next);
    }
    uint32_t* next = reinterpret_cast<uint32_t*>(voice[2]);
    if (next != nullptr) {
        uint32_t* prevAgain = reinterpret_cast<uint32_t*>(voice[1]);
        next[1] = reinterpret_cast<uintptr_t>(prevAgain);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// audSystem_Destroy @ 0x82166DC0 | size: 0x78
//
// Destructor for the audSystem class. Sets base class vtables, calls the
// gameLoop audio teardown, installs the final vtable, and optionally frees
// memory (scalar deleting destructor pattern).
// ─────────────────────────────────────────────────────────────────────────────
void audSystem_Destroy(void* self, uint32_t flags) // @ 0x82166DC0
{
    uint32_t* obj = static_cast<uint32_t*>(self);

    // Install intermediate vtables
    obj[0] = reinterpret_cast<uintptr_t>(g_audSystem_vtable_A);
    uint32_t* subObj = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(obj) + 4);
    subObj[0] = reinterpret_cast<uintptr_t>(g_audSystem_vtable_B);

    // Call base class teardown
    audSystem_DestroyInternal(subObj);

    // Install final vtable
    obj[0] = reinterpret_cast<uintptr_t>(g_msgMsgSink_vtable);

    // Scalar delete if flag bit 0 is set
    if (flags & 1) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// audBankMgr_Destroy @ 0x82162D48 | size: 0xA8
//
// Destructor for audBankMgr. Installs the audBankMgr vtable, iterates all
// hash buckets, calling the destructor (vfn_0) on each bank's resource
// pointer, then frees the hash table storage twice (two internal arrays),
// and installs the final base vtable.
// ─────────────────────────────────────────────────────────────────────────────
void audBankMgr_Destroy(void* self) // @ 0x82162D48
{
    uint32_t* mgr = static_cast<uint32_t*>(self);

    // Install intermediate vtable
    mgr[0] = reinterpret_cast<uintptr_t>(g_audBankMgr_vtable_intermediate);

    // Iterate hash buckets and destroy bank resources
    uint16_t numBuckets = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<uint8_t*>(mgr) + 24);

    if (numBuckets > 0) {
        uint32_t* bucketArray = reinterpret_cast<uint32_t*>(mgr[5]); // offset +20

        for (int32_t i = 0; i < numBuckets; i++) {
            uint32_t* node = reinterpret_cast<uint32_t*>(bucketArray[i]);

            while (node != nullptr) {
                void* resource = reinterpret_cast<void*>(node[2]); // offset +8
                if (resource != nullptr) {
                    // Call virtual destructor (slot 0) with flags=1
                    uint32_t** vtbl = reinterpret_cast<uint32_t**>(resource);
                    typedef void (*DtorFn)(void*, int);
                    DtorFn dtor = reinterpret_cast<DtorFn>((*vtbl)[0]);
                    dtor(resource, 1);
                }
                node = reinterpret_cast<uint32_t*>(node[3]); // next node at +12
            }
        }
    }

    // Free hash table storage (called twice for two internal arrays)
    void* hashTablePtr = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(mgr) + 20);
    audHashTable_Destroy(hashTablePtr);
    audHashTable_Destroy(hashTablePtr);

    // Install final base vtable
    mgr[0] = reinterpret_cast<uintptr_t>(g_audBankMgr_vtable);
}

// ═════════════════════════════════════════════════════════════════════════════
// audMsgSink — Audio Message Sink Functions
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audMsgSink_Reset @ 0x821B3A10 | size: 0x5C
//
// Resets an audio message sink. Calls vfn_4 (Stop) on the control at +20,
// then clears the state: sets volume to 0.0, clears the handle at +24,
// and zeroes the three flag bytes at +32, +33, +34.
// ─────────────────────────────────────────────────────────────────────────────
void audMsgSink_Reset(void* self) // @ 0x821B3A10
{
    uint32_t* sink = static_cast<uint32_t*>(self);

    // Call vfn_4 (Stop) on the control object at offset +20
    void* control = reinterpret_cast<void*>(sink[5]); // +20
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(control);
    typedef void (*StopFn)(void*);
    StopFn stop = reinterpret_cast<StopFn>((*vtbl)[4]);
    stop(control);

    // Clear volume at +28 to 0.0f
    float zero = 0.0f;
    uint32_t zeroU32;
    __builtin_memcpy(&zeroU32, &zero, 4);
    sink[7] = zeroU32;  // +28

    // Clear handle at +24
    sink[6] = 0;  // +24

    // Clear flag bytes at +32, +33, +34
    uint8_t* bytes = reinterpret_cast<uint8_t*>(sink);
    bytes[34] = 0;
    bytes[32] = 0;
    bytes[33] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// audMsgSink_Release @ 0x821B8598 | size: 0x68
//
// Releases/cleans up an audio message sink resource. If the object pointer
// at +0 is non-null, calls vfn_4 (Stop) on it, then sets +0 to null.
// If already null, just ensures it stays null.
// ─────────────────────────────────────────────────────────────────────────────
void audMsgSink_Release(void* self) // @ 0x821B8598
{
    uint32_t* sink = static_cast<uint32_t*>(self);
    void* resource = reinterpret_cast<void*>(sink[0]);

    if (resource != nullptr) {
        // Call vfn_4 (Stop) on the resource
        uint32_t** vtbl = reinterpret_cast<uint32_t**>(resource);
        typedef void (*StopFn)(void*);
        StopFn stop = reinterpret_cast<StopFn>((*vtbl)[4]);
        stop(resource);
    }

    sink[0] = 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// audControl3d — 3D Audio Control Functions
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audControl3d_InitAll @ 0x8257C678 | size: 0x44
//
// Iterates over a global array of 4 audControl3d objects (stride 176 bytes)
// and calls audControl3d_Initialize (a reset/init method) on each one.
// ─────────────────────────────────────────────────────────────────────────────
void audControl3d_InitAll() // @ 0x8257C678
{
    uint8_t* control = g_audControl3dArray;

    for (int32_t i = 3; i >= 0; i--) {
        audControl3d_Initialize(control);
        control += 176;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// Bank Entry Type Checks
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audBankEntry_IsStreaming @ 0x82164D48 | size: 0x44
//
// Checks if a bank entry is of streaming type. Reads the entry offset from
// the voice at +16, looks up the type byte at +16 of the entry data, and
// returns true if the type is 25 (streaming) or 28 (alternate streaming).
// ─────────────────────────────────────────────────────────────────────────────
bool audBankEntry_IsStreaming(void* voice) // @ 0x82164D48
{
    uint32_t* v = static_cast<uint32_t*>(voice);
    int32_t entryOffset = static_cast<int32_t>(v[4]); // +16

    if (entryOffset < 0) {
        return false;
    }

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(g_pAudBankEntryData);
    uint8_t entryType = entryBase[entryOffset + 16];
    int8_t typeSigned = static_cast<int8_t>(entryType);

    return (typeSigned == 25 || typeSigned == 28);
}

// ─────────────────────────────────────────────────────────────────────────────
// audBankEntry_IsXMA @ 0x82164D90 | size: 0x38
//
// Checks if a bank entry is of XMA (compressed audio) type. Reads the entry
// offset from the voice at +16, looks up the type byte, and returns true
// if the type is 27 (XMA).
// ─────────────────────────────────────────────────────────────────────────────
bool audBankEntry_IsXMA(void* voice) // @ 0x82164D90
{
    uint32_t* v = static_cast<uint32_t*>(voice);
    int32_t entryOffset = static_cast<int32_t>(v[4]); // +16

    if (entryOffset < 0) {
        return false;
    }

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(g_pAudBankEntryData);
    uint8_t entryType = entryBase[entryOffset + 16];

    return (entryType == 27);
}

// ═════════════════════════════════════════════════════════════════════════════
// Audio Scene Helper
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audScene_CleanupAndRegister @ 0x8246AE30 | size: 0x50
//
// Wraps audio scene cleanup. Calls audScene_Shutdown (which performs the main scene
// teardown), then if the input pointer is non-null, calls atList_InsertItem
// to register it in the audio scene's object list. Returns 0 (null).
// ─────────────────────────────────────────────────────────────────────────────
void* audScene_CleanupAndRegister(void* obj) // @ 0x8246AE30
{
    audScene_Shutdown(obj);

    if (obj != nullptr) {
        static constexpr uint32_t kAudioSceneRegFlags = 0x81020007;
        atList_InsertItem(g_audSceneList, obj, kAudioSceneRegFlags);
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// audScene_DispatchStop @ 0x82447B70 | size: 0x18
//
// Global thunk that loads the audio scene singleton pointer, reads its
// vtable, and tail-calls vfn_4 (vtable slot 4, byte offset 16).
// Effectively: return g_pAudioScene->vfn_4();
// ─────────────────────────────────────────────────────────────────────────────
void audScene_DispatchStop() // @ 0x82447B70
{
    void* scene = g_pAudSceneSingleton;
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(scene);
    typedef void (*VFn)(void*);
    VFn fn = reinterpret_cast<VFn>((*vtbl)[4]);
    fn(scene);
}

// ─────────────────────────────────────────────────────────────────────────────
// audScene_DispatchUpdate @ 0x82447A80 | size: 0x30
//
// Global thunk that loads the audio scene singleton, prepends it as the
// first argument, and tail-calls vfn_5 (vtable slot 5, byte offset 20).
// Effectively: g_pAudioScene->vfn_5(arg1, arg2, arg3, arg4);
// ─────────────────────────────────────────────────────────────────────────────
void audScene_DispatchUpdate(void* a1, void* a2, void* a3, void* a4) // @ 0x82447A80
{
    void* scene = g_pAudSceneSingleton;
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(scene);
    typedef void (*VFn)(void*, void*, void*, void*, void*);
    VFn fn = reinterpret_cast<VFn>((*vtbl)[5]);
    fn(scene, a1, a2, a3, a4);
}

// ─────────────────────────────────────────────────────────────────────────────
// audScene_DispatchRender @ 0x82447AB0 | size: 0x30
//
// Global thunk that loads the audio scene singleton, prepends it as the
// first argument, and tail-calls vfn_6 (vtable slot 6, byte offset 24).
// Effectively: g_pAudioScene->vfn_6(arg1, arg2, arg3, arg4);
// ─────────────────────────────────────────────────────────────────────────────
void audScene_DispatchRender(void* a1, void* a2, void* a3, void* a4) // @ 0x82447AB0
{
    void* scene = g_pAudSceneSingleton;
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(scene);
    typedef void (*VFn)(void*, void*, void*, void*, void*);
    VFn fn = reinterpret_cast<VFn>((*vtbl)[6]);
    fn(scene, a1, a2, a3, a4);
}


// ═════════════════════════════════════════════════════════════════════════════
// Batch 4 — audControlGroup accessors, audVoiceSfx/audVoiceStream pause and
//           state query functions (8-32B each)
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// rage::audControlGroup — Group-level audio control accessors
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audControlGroup::GetVolume @ 0x82162528 | size: 0x8
 * Returns the group volume at field offset +0x1C (28).
 */
float audControlGroup::GetVolume() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 28, sizeof(float));
    return value;
}

/**
 * audControlGroup::GetPitch @ 0x82162530 | size: 0x8
 * Returns the group pitch at field offset +0x24 (36).
 */
float audControlGroup::GetPitch() {
    float value;
    std::memcpy(&value, reinterpret_cast<const char*>(this) + 36, sizeof(float));
    return value;
}

/**
 * audControlGroup::GetTypeNameA @ 0x82162538 | size: 0xC
 * Returns a pointer to a static type name string (g_audControlGroupTypeName_short).
 */
const char* audControlGroup::GetTypeNameA() {
    return g_audControlGroupTypeName_short;
}

/**
 * audControlGroup::GetTypeNameB @ 0x82162548 | size: 0xC
 * Returns a pointer to a static type name string (g_audControlGroupTypeName).
 */
const char* audControlGroup::GetTypeNameB() {
    return g_audControlGroupTypeName;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audVoiceSfx — SFX voice pause/unpause delegation
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audVoiceSfx::Pause @ 0x82163918 | size: 0xC
 * Pauses the SFX voice by delegating to audVoiceSfx_SetPaused with flag=0.
 */
void audVoiceSfx::Pause() {
    audVoiceSfx_SetPaused(m_pSfxRef, 0);
}

/**
 * audVoiceSfx::Unpause @ 0x82163928 | size: 0xC
 * Unpauses the SFX voice by delegating to audVoiceSfx_SetPaused with flag=1.
 */
void audVoiceSfx::Unpause() {
    audVoiceSfx_SetPaused(m_pSfxRef, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audVoiceStream — Stream voice pause/unpause delegation
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audVoiceStream::Pause @ 0x821643D0 | size: 0xC
 * Pauses the stream voice by delegating to audVoiceStream_SetPaused with flag=0.
 */
void audVoiceStream::Pause() {
    audVoiceStream_SetPaused(reinterpret_cast<void*>(m_pStreamRef), 0);
}

/**
 * audVoiceStream::Unpause @ 0x821643E0 | size: 0xC
 * Unpauses the stream voice by delegating to audVoiceStream_SetPaused with flag=1.
 */
void audVoiceStream::Unpause() {
    audVoiceStream_SetPaused(reinterpret_cast<void*>(m_pStreamRef), 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::audVoiceStream — Stream state query functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * audVoiceStream::IsPrimed @ 0x821645B8 | size: 0x20
 * Returns true if the stream state (at streamRef+4) equals 14 (primed).
 */
bool audVoiceStream::IsPrimed() {
    uint32_t* streamData = reinterpret_cast<uint32_t*>(m_pStreamRef);
    return streamData[1] == 14;
}

/**
 * audVoiceStream::IsReady @ 0x821645D8 | size: 0x20
 * Returns true if the stream state (at streamRef+4) equals 15 (ready).
 */
bool audVoiceStream::IsReady() {
    uint32_t* streamData = reinterpret_cast<uint32_t*>(m_pStreamRef);
    return streamData[1] == 15;
}

// ═══════════════════════════════════════════════════════════════════════════════
// audControlWrapper — Audio Control Wrapper Destructors
//
// Thin wrapper classes that own an audControl* pointer and handle its
// lifecycle (destruction).  The 3D variant adds array-delete support for
// batches of 12-byte wrapper elements.
//
// Vtable: rage::audControlWrapper  @ 0x82074B88  [RTTI confirmed]
// ═══════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audControlWrapper::cleanupControl @ 0x823F7C28 | size: 0x5C
//
// Core cleanup helper (non-virtual).  Destroys the wrapped audControl
// object via its virtual destructor and nulls the pointer.
// ─────────────────────────────────────────────────────────────────────────────
void audControlWrapper::cleanupControl()
{
    if (m_pControl != nullptr) {
        delete m_pControl;
        m_pControl = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// audControlWrapper::~audControlWrapper @ 0x823F7C88 | size: 0x50
//
// Base wrapper destructor.  Calls cleanupControl() then the compiler
// handles the optional scalar-delete (flag bit 0 in the destructor's
// hidden parameter).
// ─────────────────────────────────────────────────────────────────────────────
audControlWrapper::~audControlWrapper()
{
    cleanupControl();
}

// ─────────────────────────────────────────────────────────────────────────────
// audControl3dWrapper::~audControl3dWrapper @ 0x823F8138 | size: 0xD0
//
// 3D wrapper destructor with array-delete support.
//
// The assembly checks bit 1 of the flags parameter to determine if this
// is an array delete.  In the array path, it:
//   1. Reads the element count from (this - 4)
//   2. Iterates count elements in reverse (stride 12 bytes each)
//   3. Resets each element's vtable to audControlWrapper base
//   4. Destroys each element's m_pControl via virtual destructor
//   5. Frees the array (including the count prefix)
//
// For standard C++ delete, the compiler handles this via delete[].
// ─────────────────────────────────────────────────────────────────────────────
audControl3dWrapper::~audControl3dWrapper()
{
    // Base class destructor (cleanupControl) called automatically by C++
}

} // namespace rage
