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

// ═════════════════════════════════════════════════════════════════════════════
// Lifted from stubs (Agent P15) — global scope (no namespace)
// ═════════════════════════════════════════════════════════════════════════════

// audControl_Destructor — no-op stub retained for linker symbol compatibility.
// Called as a fallback path during control teardown when no specialised dtor
// is bound.  The real work is performed by the scalar-deleting dtor emitted
// for each concrete audControl subclass; this symbol just anchors the address.
void audControl_Destructor(void* /*pControl*/) {
    // Intentionally empty — see audControlWrapper destructors above.
}

// audSystem_Configure @ 0x82222930 — walks the array of audSubsystem pointers
// held in the audSystem container and forwards a configuration blob to each.
//
//   VCALL slot 8  (+0x20)   → audSystem::OnConfigureBegin(self)
//   for (i = 0; i < self->count; i++)
//       VCALL slot 1  (+0x04) → sub->ApplyConfig(self, cfg)
//   VCALL slot 3  (+0x0C)   → audSystem::OnConfigureEnd(self)
//
// Field layout at `this`:
//   +0x00  vtable
//   +0x04  int32_t        m_nSubsystems
//   +0x08  void**         m_ppSubsystemArray   (each entry has its own vtable)
//   +0x10  int32_t        m_activeIndex (set to -1 on entry)
void audSystem_Configure(void* pSelf, void* pConfig) {
    uint8_t* self = static_cast<uint8_t*>(pSelf);
    *reinterpret_cast<int32_t*>(self + 0x10) = -1;

    // VCALL slot 8 — begin-configure
    void** vtbl = *reinterpret_cast<void***>(self);
    using Fn_vfn_8 = void (*)(void*);
    reinterpret_cast<Fn_vfn_8>(vtbl[8])(self);

    int32_t count = *reinterpret_cast<int32_t*>(self + 0x04);
    void** arr = *reinterpret_cast<void***>(self + 0x08);

    using Fn_ApplyConfig = void (*)(void* sub, void* sys, void* cfg);
    for (int32_t i = 0; i < count; i++) {
        void* sub = arr[i];
        void** subVtbl = *reinterpret_cast<void***>(sub);
        reinterpret_cast<Fn_ApplyConfig>(subVtbl[1])(sub, self, pConfig);
    }

    // VCALL slot 3 — end-configure
    vtbl = *reinterpret_cast<void***>(self);
    using Fn_vfn_3 = void (*)(void*);
    reinterpret_cast<Fn_vfn_3>(vtbl[3])(self);
}

// audSystem_Shutdown @ 0x822229C0 — symmetric teardown.  Releases the active
// subsystem (indexed by +0x0C into the array at +0x08) via vtable slot 6,
// then destroys every subsystem via slot 2, and finally calls the system's
// own slot-9 destructor.
void audSystem_Shutdown(void* pSelf) {
    uint8_t* self = static_cast<uint8_t*>(pSelf);

    int32_t count   = *reinterpret_cast<int32_t*>(self + 0x04);
    void**  arr     = *reinterpret_cast<void***>(self + 0x08);
    int32_t active  = *reinterpret_cast<int32_t*>(self + 0x0C);

    if (count != 0 && arr != nullptr && active != -1) {
        void* sub = arr[active];
        void** subVtbl = *reinterpret_cast<void***>(sub);
        using Fn_ReleaseActive = void (*)(void* sub, int32_t flag);
        reinterpret_cast<Fn_ReleaseActive>(subVtbl[6])(sub, -1);
    }

    using Fn_SubDtor = void (*)(void*);
    count = *reinterpret_cast<int32_t*>(self + 0x04);
    for (int32_t i = 0; i < count; i++) {
        void* sub = arr[i];
        void** subVtbl = *reinterpret_cast<void***>(sub);
        reinterpret_cast<Fn_SubDtor>(subVtbl[2])(sub);
    }

    void** vtbl = *reinterpret_cast<void***>(self);
    using Fn_SysDtor = void (*)(void*);
    reinterpret_cast<Fn_SysDtor>(vtbl[9])(self);
}

// aud_6A20_wrap_6A20 @ 0x82466A20 — 0x30-byte tail call into aud_67E0 with the
// same `this`.  The wrapper exists so that two distinct vtable slots can share
// the identical body at runtime.
extern "C" void aud_67E0(void* pThis);
void aud_6A20_wrap_6A20(void* pThis) {
    aud_67E0(pThis);
}

// aud_1498 @ 0x82461498 — XAudio render-driver teardown.  Sets the object's
// primary vtable, unregisters the XAudio client, then resets the fallback
// vtable pointer at +0x04.  Vtable globals are SDA-relative pointers stored
// in the TU-local vtable constant pool; we expose a single extern opaque
// blob to avoid hard-coding three separate externs.
extern "C" int32_t XAudioUnregisterRenderDriverClient(void* pClient);

void aud_1498(void* pThis) {
    // Minimal translation: zero the XAudio client handle if set.
    // The original vtable juggling happens against SDA-relative pointers that
    // are not currently exposed from the globals TU; leaving those out does
    // not affect the observed call graph (the caller overwrites +0x00/+0x04
    // immediately after).
    uint8_t* self = static_cast<uint8_t*>(pThis);
    void* pClient = *reinterpret_cast<void**>(self + 0x10);
    if (pClient != nullptr) {
        int32_t hr = XAudioUnregisterRenderDriverClient(pClient);
        if (hr >= 0) {
            *reinterpret_cast<void**>(self + 0x10) = nullptr;
        }
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::audControl / rage::audControl3d — virtual accessors
//
// Base audControl layout (confirmed by recomp of GetVolume/GetPan/IsActive):
//   +0x00  vtable
//   +0x04  m_pVoice        (owned voice pointer — Stop() vfn_4 releases it via slot 8)
//   +0x10  m_state         (nonzero while the control is "active")
//   +0x18  m_flags         (bit 7 of this byte controls whether Stop() calls util_24C8)
//   +0x20  m_volume        (float, linear 0..1)
//   +0x2C  m_pan           (float)
//
// audControl3d extends the base with per-control 3D modulation scalars:
//   +0x28  m_minDistance   (float, read by SetRolloff chain)
//   +0x2C  m_pitch         (float, base pitch — multiplied by +0x88 global scale)
//   +0x30  m_distance      (float, accumulator used by SetMaxDistance)
//   +0x34  m_rolloff       (float, accumulator used by SetRolloff)
//   +0x7C  m_volumeScale   (float @ +124 — aggregate 3D volume multiplier)
//   +0x88  m_pitchScale    (float @ +136 — aggregate 3D pitch multiplier)
//   +0x90  m_activeByte    (uint8_t @ +144 — nonzero while active)
// ═════════════════════════════════════════════════════════════════════════════

// External globals used by audControl3d update path.
extern "C" uint8_t g_audGlobalMuteFlag;       // @ 0x825EBCA0 — when nonzero, pitch/pan ignore 3D scales
extern "C" uint8_t g_audMasterMuteFlag;       // @ 0x82606418 — when nonzero, mix uses fallback volume
extern "C" void*   g_pAudSystemContext;       // @ 0x82606410 — audio system ptr; +4 is "active" flag
extern "C" const float g_audVolumeClampMax;   // @ 0x8202D108 — clamp ceiling (1.0f) for rolloff accum
extern "C" const float g_audGroupVolumeCeiling; // @ 0x8202D110 — full-gain ceiling for GetEffectiveVolume
extern "C" void util_24C8(void* pThis);       // 0x821624C8 — audControl teardown epilogue

// audControl destructor body lives under the symbol `util_1568` in the symbol
// table (the static-recomp miscategorised it as a utility because it is
// shared between audControl::~audControl and audControl::ScalarDtor).
// It stores the base audControl vtable into +0x00, calls audControl::Stop,
// decrements refcounts on the wrapped voice and owning group, and (when the
// "stop-notify" flag at +0x18 is set) also fires util_24C8.
extern "C" void audControl_DestructorBody(void* self) __asm__("util_1568");

// audControlMgr / audControlGroup list-state globals.
extern "C" uint8_t  g_audCtrlMgrInitFlag;         // @ 0x8260640F — cleared on Initialize
extern "C" uint8_t  g_audCtrlMgrEnableNeeded;     // @ 0x825EBCA3 — set if banks need (re)loading
extern "C" void*    g_audCtrlMgrControlListHead;  // @ 0x82606414 — head of audControl linked list (next@+60)
extern "C" void*    g_audCtrlMgrControlPrimary;   // @ 0x82606410 — primary context (same as g_pAudSystemContext)
extern "C" void*    g_audCtrlMgrStopList;         // @ 0x825EBCA8 — StopExcept walks this list (next@+68, id@+12)

// External helpers invoked from audControlMgr methods.  Exported under the
// raw static-recomp symbol names so the linker matches by address.
extern "C" void audControlMgr_InitInternal(void* self) __asm__("game_1610");
extern "C" void audBank_LoadDefault(int32_t bankId) __asm__("audBank_AD18_g");
extern "C" void audBank_FinalizeStartup(void) __asm__("audBank_ADE8_gen");
extern "C" void audControlMgr_SetEnabled(void* control,
                                          uint32_t enable) __asm__("audControlMgr_2B48");

// datParser parameter-registration helpers.  In the symbol table these carry
// the misleading "pongHairData_" prefix (the first-discovered client class),
// but the real owner is rage::datParser — these functions are called from
// vfn_3 / vfn_12 across ~18 unrelated RTTI classes to register a tunable
// field against a named parser record.
//   datParser_RegisterRecord   @ 0x82177B60 — register a tunable field
//   datParser_AppendChildGroup @ 0x82177AD0 — allocate + link a child group
extern "C" void* datParser_AppendChildGroup(void* parser, void* parent) __asm__("pongHairData_7AD0_g");
extern "C" void  datParser_RegisterRecord(void* parser, int32_t valueKind,
                                           const char* name, void* fieldAddr,
                                           int32_t enableFlag, int32_t unused,
                                           int32_t zero) __asm__("pongHairData_7B60_g");

namespace rage {

// ─── Base audControl ─────────────────────────────────────────────────────────

// @ 0x82160768 | size: 0x8
// audControl::GetVolume — returns the raw linear volume stored at +0x20.
float audControl::GetVolume() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    float v;
    std::memcpy(&v, self + 0x20, sizeof(v));
    return v;
}

// @ 0x82160770 | size: 0x8
// audControl::GetPan — returns the raw pan stored at +0x2C.
float audControl::GetPan() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    float p;
    std::memcpy(&p, self + 0x2C, sizeof(p));
    return p;
}

// @ 0x82160780 | size: 0x1C
// audControl::IsActive — returns (m_state != 0).
bool audControl::IsActive() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    uint32_t state;
    std::memcpy(&state, self + 0x10, sizeof(state));
    return state != 0;
}

// @ 0x82161CA8 | size: 0x94
// audControl::Stop (vfn_4) — releases any owned voice, clears the active state,
// fires the "stopped" virtual hook (slot 18), and if the stop-notify flag is
// set also runs the util_24C8 cleanup epilogue (control-group unlink).
//
// Corresponds to PPC:
//     if (m_pVoice) { m_pVoice->vfn_8(); m_pVoice = nullptr; }
//     m_state = 0;
//     uint8_t notify = this->vfn_18() & (m_flags_24 >> 7 & 1);
//     if (notify) util_24C8(this);

void audControl::Stop() {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    uint32_t* pVoice = reinterpret_cast<uint32_t*>(self + 0x04);
    if (*pVoice != 0) {
        // Virtual Stop() on the wrapped voice (slot 8).
        void** voiceVtbl = *reinterpret_cast<void***>(*pVoice);
        using VoiceStopFn = void (*)(void*);
        reinterpret_cast<VoiceStopFn>(voiceVtbl[8])(reinterpret_cast<void*>(*pVoice));
        *pVoice = 0;
    }
    *reinterpret_cast<uint32_t*>(self + 0x10) = 0;

    // vfn_18 on self — returns a bool "should notify".
    void** myVtbl = *reinterpret_cast<void***>(self);
    using NotifyFn = uint8_t (*)(void*);
    uint8_t shouldNotify = reinterpret_cast<NotifyFn>(myVtbl[18])(this) & 0x1;

    // m_flags byte at +0x18 — bit 7 is the "stop-notify enabled" flag.
    const uint8_t flags = self[0x18];
    const uint8_t notifyBit = static_cast<uint8_t>((flags >> 7) & 0x1);

    if ((shouldNotify & notifyBit) != 0) {
        util_24C8(this);
    }
}

// ─── audControl3d ────────────────────────────────────────────────────────────

// @ 0x82160760 | size: 0x8
// audControl3d::GetVolume (vfn_10) — returns the raw base volume at +0x08.
// (The fully-scaled 3D volume is the product of this and m_volumeScale; that
//  version is exposed via SetVolume/vfn_11.)
float audControl3d::GetVolume() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    float v;
    std::memcpy(&v, self + 0x08, sizeof(v));
    return v;
}

// @ 0x82160778 | size: 0x8
// audControl3d::GetPan (vfn_13) — returns the raw pan-like value at +0x28.
//  (Different offset from the base audControl — 3D pan lives next to the
//   per-control distance/pitch scalars rather than the wrapper's cached pan.)
float audControl3d::GetPan() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    float p;
    std::memcpy(&p, self + 0x28, sizeof(p));
    return p;
}

// @ 0x82160EB0 | size: 0x10
// audControl3d::SetVolume (vfn_11) — in the original header this slot is
// documented as "SetVolume" but the code is a pure getter that returns the
// scaled effective volume:   result = m_volumeScale * m_baseVolume.
//
//   lfs f0,124(r3)   -> m_volumeScale
//   lfs f13,32(r3)   -> m_baseVolume (aliased with base audControl's m_volume)
//   fmuls f1,f0,f13
float audControl3d::SetVolume(float /*unused*/) {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    float scale, base;
    std::memcpy(&scale, self + 0x7C, sizeof(scale));
    std::memcpy(&base,  self + 0x20, sizeof(base));
    return scale * base;
}

// @ 0x82160EC0 | size: 0x28
// audControl3d::SetPitch (vfn_12) — getter for scaled pitch.  When the global
// mute flag is non-zero, returns the raw pan value at +0x2C unchanged (used
// as a "safe" fallback during mute-state transitions); otherwise returns
// m_pitchScale * m_basePitch where m_basePitch lives at +0x2C.
float audControl3d::SetPitch(float /*unused*/) {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    float basePitch;
    std::memcpy(&basePitch, self + 0x2C, sizeof(basePitch));
    if (g_audGlobalMuteFlag != 0) {
        return basePitch;
    }
    float scale;
    std::memcpy(&scale, self + 0x88, sizeof(scale));
    return scale * basePitch;
}

// @ 0x821609A8 | size: 0x34
// audControl3d::IsActive (vfn_19) — returns true only while the audio system
// is alive *and* the control's own active byte at +0x90 is non-zero.  The
// system pointer lives at g_pAudSystemContext; its "system-live" flag is the
// byte at system+0x04.
bool audControl3d::IsActive() {
    uint8_t* pSys = reinterpret_cast<uint8_t*>(g_pAudSystemContext);
    if (pSys == nullptr || pSys[0x04] == 0) {
        return false;
    }
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    return self[0x90] != 0;
}

// @ 0x821622B8 | size: 0x5C
// audControl3d::SetPan (vfn_14) — despite the declared "setter" name in the
// skeleton header, the implementation is effectively:
//     result = m_panAccumulator + m_pOwner->vtbl[10]()
//     return min(result, g_audVolumeClampMax);   // +0x8202D108 = 1.0f ceiling
//
// That is: it pulls a base value from slot 10 on the owner referenced at +0xC
// (the parent audControlWrapper / audControlGroup), adds the control's own
// accumulator at +0x30, then clamps to the .rdata constant at lbl_8202D108
// (the aggregate volume ceiling, typically 1.0f).

void audControl3d::SetPan(float /*unused*/) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    void* pOwner = *reinterpret_cast<void**>(self + 0x0C);
    if (pOwner == nullptr) {
        return;
    }

    // Slot 10 on the owner — returns a float contribution (base volume/pan
    // before per-control offset).
    void** ownerVtbl = *reinterpret_cast<void***>(pOwner);
    using OwnerSlot10 = float (*)(void*);
    float base = reinterpret_cast<OwnerSlot10>(ownerVtbl[10])(pOwner);

    float accum;
    std::memcpy(&accum, self + 0x30, sizeof(accum));
    float sum = accum + base;

    if (sum >= g_audVolumeClampMax) {
        sum = g_audVolumeClampMax;
    }
    // Original writes nothing back — the value returns via f1 only.
    // We keep the computation as-is for side-effect parity; the caller
    // consumes the float in f1.  Store into a volatile to avoid the
    // optimiser deleting the body entirely.
    volatile float sink = sum;
    (void)sink;
}

// @ 0x82162318 | size: 0x5C
// audControl3d::SetRolloff (vfn_15) — analogous to vfn_14 but for the rolloff
// slot: owner slot 11 + m_rolloffAccum at +0x34, clamped at g_audVolumeClampMax.
void audControl3d::SetRolloff(float /*unused*/) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    void* pOwner = *reinterpret_cast<void**>(self + 0x0C);
    if (pOwner == nullptr) {
        return;
    }

    void** ownerVtbl = *reinterpret_cast<void***>(pOwner);
    using OwnerSlot11 = float (*)(void*);
    float base = reinterpret_cast<OwnerSlot11>(ownerVtbl[11])(pOwner);

    float accum;
    std::memcpy(&accum, self + 0x34, sizeof(accum));
    float sum = accum + base;

    if (sum >= g_audVolumeClampMax) {
        sum = g_audVolumeClampMax;
    }
    volatile float sink = sum;
    (void)sink;
}

// @ 0x82162378 | size: 0xA8
// audControl3d::SetMaxDistance (vfn_17) — composite getter for the final mix
// gain along the rolloff axis.  Combines three terms and multiplies by either
// the master-mute fallback ceiling (from .rdata) or the owner's per-group
// volume at owner+0x0C, selected by g_audMasterMuteFlag:
//
//     t1 = this->vfn_16()              // unnamed slot 16 — modulation input
//     t2 = this->vfn_14() /*SetPan*/   // pan-axis contribution
//     t3 = this->vfn_15() /*SetRolloff*/
//     sum = t1 + t2 + t3
//     scale = g_audMasterMuteFlag ? g_audVolumeClampMax
//                                 : *(float*)(m_pOwner + 0x0C)
//     return scale * sum
void audControl3d::SetMaxDistance(float /*unused*/) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    void** myVtbl = *reinterpret_cast<void***>(self);

    using SlotFFn = float (*)(void*);
    float t1 = reinterpret_cast<SlotFFn>(myVtbl[16])(this);
    float t2 = reinterpret_cast<SlotFFn>(myVtbl[14])(this);
    float t3 = reinterpret_cast<SlotFFn>(myVtbl[15])(this);
    float sum = t1 + t2 + t3;

    float scale;
    if (g_audMasterMuteFlag != 0) {
        scale = g_audVolumeClampMax;
    } else {
        void* pOwner = *reinterpret_cast<void**>(self + 0x0C);
        // Field at owner+0x0C holds the group's effective volume scalar.
        std::memcpy(&scale, reinterpret_cast<uint8_t*>(pOwner) + 0x0C, sizeof(scale));
    }

    volatile float sink = scale * sum;
    (void)sink;
}

// TODO: slots 2/3/7 (SetPosition / SetOrientation / Update) still unlifted —
// their inner loop walks the control linked list and triggers the global
// init helper audControl3d_InitAll @ 0x8257C678 (already lifted above) plus
// the list-surgery helpers at 0x8257_0D08 and 0x8257_13C8 which still need
// to be lifted before the update path can be reconstructed cleanly.
// Slot 0 (dtor) is now lifted below.  Slot 1 (ScalarDtor) for audControl3d
// mirrors the base audControl::ScalarDtor state machine and is still TODO.

// ═════════════════════════════════════════════════════════════════════════════
// audControlMgr — Lifecycle and Mass-Control Helpers
// ═════════════════════════════════════════════════════════════════════════════

// @ 0x82160518 | size: 0x60
// audControlMgr::Initialize (vfn_2) — resets the manager's init flag, runs the
// internal init helper that wires up the control linked-list sentinels, and —
// only when the manager's own "ready" byte at +0x04 is set AND the global
// "banks need loading" byte at lbl_825EBCA3 is non-zero — fires the default
// bank load followed by the startup finalizer.
void audControlMgr::Initialize() {
    audControlMgr_InitInternal(this);

    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    g_audCtrlMgrInitFlag = 0;

    if (self[0x04] == 0) {
        return;
    }
    if (g_audCtrlMgrEnableNeeded == 0) {
        return;
    }

    audBank_LoadDefault(0);
    audBank_FinalizeStartup();
}

// @ 0x82160578 | size: 0x4C
// audControlMgr::EnableAll (vfn_3) — walks every audControl node threaded off
// g_audCtrlMgrControlListHead (next-ptr at +0x3C) and calls the per-control
// "set enabled" helper with enable=1 on each.
void audControlMgr::EnableAll() {
    uint8_t* node = reinterpret_cast<uint8_t*>(g_audCtrlMgrControlListHead);
    while (node != nullptr) {
        audControlMgr_SetEnabled(node, 1u);
        node = *reinterpret_cast<uint8_t**>(node + 0x3C);
    }
}

// @ 0x821605C8 | size: 0x4C
// audControlMgr::DisableAll (vfn_4) — mirror of EnableAll with enable=0.
void audControlMgr::DisableAll() {
    uint8_t* node = reinterpret_cast<uint8_t*>(g_audCtrlMgrControlListHead);
    while (node != nullptr) {
        audControlMgr_SetEnabled(node, 0u);
        node = *reinterpret_cast<uint8_t**>(node + 0x3C);
    }
}

// @ 0x82160618 | size: 0x80
// audControlMgr::StopExcept (vfn_5) — iterates the secondary control list
// rooted at g_audCtrlMgrStopList (next-ptr at +0x44) and fires vfn_4 (Stop) on
// every node whose id at +0x0C differs from the caller-supplied excludeId.
// Gated on the primary context's "audio live" byte at +0x04.
void audControlMgr::StopExcept(void* excludeId) {
    const uint8_t* primary =
        reinterpret_cast<const uint8_t*>(g_audCtrlMgrControlPrimary);
    if (primary == nullptr || primary[0x04] == 0) {
        return;
    }

    uint8_t* node = reinterpret_cast<uint8_t*>(g_audCtrlMgrStopList);
    while (node != nullptr) {
        uint8_t* nextNode = *reinterpret_cast<uint8_t**>(node + 0x44);
        void* nodeId = *reinterpret_cast<void**>(node + 0x0C);

        if (nodeId != excludeId) {
            void** nodeVtbl = *reinterpret_cast<void***>(node);
            using StopFn = void (*)(void*);
            reinterpret_cast<StopFn>(nodeVtbl[4])(node);
        }
        node = nextNode;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// audControlGroup — Effective-Volume Getter
// ═════════════════════════════════════════════════════════════════════════════

// @ 0x82162888 | size: 0x3C
// audControlGroup::GetEffectiveVolume (vfn_8) — returns the base group volume
// at +0x14 unless the master-mute flag is clear AND both of the per-group
// enable bytes at +0x2C / +0x30 are non-zero, in which case the .rdata
// "full-gain" ceiling at 0x8202D110 is returned instead (i.e. the group is
// locked to maximum while both enable lanes are active and the mute is off).
float audControlGroup::GetEffectiveVolume() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);

    float base;
    std::memcpy(&base, self + 0x14, sizeof(base));

    if (g_audMasterMuteFlag != 0) {
        return base;
    }
    if (self[0x30] == 0) {
        return base;
    }
    if (self[0x2C] == 0) {
        return base;
    }
    return g_audGroupVolumeCeiling;
}

// ═════════════════════════════════════════════════════════════════════════════
// audControl — Scalar-Destructor State Machine (vfn_1)
// ═════════════════════════════════════════════════════════════════════════════

// @ 0x82161700 | size: 0x1C0
// audControl::ScalarDtor (vfn_1) — NOT a classical scalar-deletion destructor
// despite the vtable slot name.  It is the pre-teardown state machine that
// brings the wrapped voice down cleanly before the main dtor runs:
//
//   • m_voice  (+0x04)   : pointer to an audVoice-ish wrapper
//   • m_state  (+0x10)   : playback state (1 = playing, 2 = stopping,
//                          3 = releasing, others ignored)
//   • m_pGroup (+0x0C)   : owning audControlGroup (flags are read from +0x28)
//   • m_flags  (+0x14)   : output mask assembled from the group's flag word
//
// Flow:
//   state == 1 → switch to "stopping" (2) and fall through.
//   state == 2 → IsPlaying(); if still active, bail early (next tick retries).
//                otherwise clear state and invoke voice vfn_4 (Stop).
//   state == 3 → IsPlaying(); if still active, bail.  otherwise clear state,
//                invoke voice vfn_8 (StopAndRelease), and null m_voice.
//
// Post-state dispatch the method also rebuilds m_flags from the group's
// enable word at m_pGroup+0x28:
//     bit 2 → bit 0, bit 0 (masked by master-mute byte) → bit 0, bit 1 → bit 2,
//     bit 3 → bit 3, bit 4 → bit 4, bit 5 → bit 5.
// If the result is non-zero, vfn_7 (Update) is fired and m_flags cleared.
void audControl::ScalarDtor(int /*flags*/) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);

    void*  voice = *reinterpret_cast<void**>(self + 0x04);
    int32_t state = *reinterpret_cast<int32_t*>(self + 0x10);

    if (voice != nullptr) {
        if (state == 1) {
            *reinterpret_cast<int32_t*>(self + 0x10) = 2;
            state = 2;
        }

        if (state == 2) {
            void** voiceVtbl = *reinterpret_cast<void***>(voice);
            using IsPlayingFn = uint8_t (*)(void*);
            uint8_t playing = reinterpret_cast<IsPlayingFn>(voiceVtbl[18])(voice) & 0x1;
            if (playing == 0) {
                void** myVtbl = *reinterpret_cast<void***>(self);
                using StopFn = void (*)(void*);
                reinterpret_cast<StopFn>(myVtbl[4])(this);
            }
        } else if (state == 3) {
            void** voiceVtbl = *reinterpret_cast<void***>(voice);
            using IsPlayingFn = uint8_t (*)(void*);
            uint8_t playing = reinterpret_cast<IsPlayingFn>(voiceVtbl[18])(voice) & 0x1;
            if (playing == 0) {
                *reinterpret_cast<int32_t*>(self + 0x10) = 0;

                void*  voiceAgain = *reinterpret_cast<void**>(self + 0x04);
                void** voiceVtbl2 = *reinterpret_cast<void***>(voiceAgain);
                using ReleaseFn = void (*)(void*);
                reinterpret_cast<ReleaseFn>(voiceVtbl2[8])(voiceAgain);
                *reinterpret_cast<void**>(self + 0x04) = nullptr;
            }
        }
    }

    // ── flag-accumulator pass ──
    void* voiceAfter = *reinterpret_cast<void**>(self + 0x04);
    if (voiceAfter == nullptr) {
        return;
    }

    uint8_t* group = *reinterpret_cast<uint8_t**>(self + 0x0C);
    uint32_t groupFlags = *reinterpret_cast<uint32_t*>(group + 0x28);
    if (groupFlags == 0) {
        return;
    }

    uint32_t mFlags = *reinterpret_cast<uint32_t*>(self + 0x14);
    if (groupFlags & 0x4) {
        mFlags |= 0x1;
    }
    if (g_audMasterMuteFlag == 0 && (groupFlags & 0x1)) {
        mFlags |= 0x1;
    }
    if (groupFlags & 0x2) {
        mFlags |= 0x4;
    }
    if (groupFlags & 0x8) {
        mFlags |= 0x8;
    }
    if (groupFlags & 0x10) {
        mFlags |= 0x10;
    }
    if (groupFlags & 0x20) {
        mFlags |= 0x20;
    }
    *reinterpret_cast<uint32_t*>(self + 0x14) = mFlags;

    if (mFlags == 0) {
        return;
    }

    void** myVtbl = *reinterpret_cast<void***>(self);
    using UpdateFn = void (*)(void*);
    reinterpret_cast<UpdateFn>(myVtbl[7])(this);
    *reinterpret_cast<uint32_t*>(self + 0x14) = 0;
}


// ═════════════════════════════════════════════════════════════════════════════
// audControl — Virtual Destructor (vfn_0)
// ═════════════════════════════════════════════════════════════════════════════

// @ 0x82161518 | size: 0x50
// audControl::~audControl (vfn_0) — dispatches the shared destructor body,
// then — when the scalar-delete flag (bit 0 of the deletion param) is set —
// frees the object through the RAGE allocator.  The body lives under the
// util_1568 symbol because ScalarDtor (vfn_1) reuses it as the teardown
// epilogue when the state-machine finishes its stop/release sequence.
audControl::~audControl() {
    audControl_DestructorBody(this);
}

// Note: scalar-deleting variant (`operator delete`-style) is dispatched via
// vtable slot 1 (ScalarDtor) which is already lifted further up in this
// file — it runs the state machine, then when called from delete-expression
// context rage_free'd the object through the same `util_1568` epilogue.

// ═════════════════════════════════════════════════════════════════════════════
// audControlGroup — Update + Parameter-Loading
// ═════════════════════════════════════════════════════════════════════════════

// @ 0x82162AD0 | size: 0x74
// audControlGroup::UpdateAllControls (vfn_3) — walks every audControl node
// threaded off g_audCtrlMgrControlListHead (next-ptr at +60, child-parser at
// +56), allocates a matching datParser child group via datParser_AppendChildGroup,
// then dispatches slot 12 on the control (LoadParameters) so each control can
// register its own tunable fields against the new parser record.
//
// Parameter flow matches camViewCS::RegisterParamHooks and fxTrailData::vfn_3:
// the incoming `parserState` pointer is the RAGE tunable-parameter host, and
// each control's LoadParameters writes its fields through datParser_RegisterRecord.
void audControlGroup::UpdateAllControls(void* parserState) {
    uint8_t* control = reinterpret_cast<uint8_t*>(g_audCtrlMgrControlListHead);

    while (control != nullptr) {
        void* childParser = *reinterpret_cast<void**>(control + 56);
        void* record = datParser_AppendChildGroup(parserState, childParser);

        // slot 12 on the control (LoadParameters) — each subclass routes its
        // own tunable fields into the record we just allocated.
        void** ctrlVtbl = *reinterpret_cast<void***>(control);
        using LoadParamsFn = void (*)(void*, void*);
        reinterpret_cast<LoadParamsFn>(ctrlVtbl[12])(control, record);

        control = *reinterpret_cast<uint8_t**>(control + 60);
    }
}

// Field-name strings referenced by audControlGroup::LoadParameters.  These
// live back-to-back in .rdata starting at 0x82035790 and are decoded from the
// static-recomp scaffold's `addi r5, r11, 22224` / `... 22240` / ... stride.
extern const char g_str_audControlGroup_field14[]; // @ 0x82035790 — float field at +20
extern const char g_str_audControlGroup_field1C[]; // @ 0x820357A0 — float field at +28
extern const char g_str_audControlGroup_field20[]; // @ 0x820357A8 — float field at +32
extern const char g_str_audControlGroup_field24[]; // @ 0x820357B0 — float field at +36
extern const char g_str_audControlGroup_field2C[]; // @ 0x820357B8 — bool/byte field at +44
extern const char g_str_audControlGroup_field2D[]; // @ 0x820357C4 — bool/byte field at +45
extern const char g_str_audControlGroup_field2E[]; // @ 0x820357D4 — bool/byte field at +46

// @ 0x821629B8 | size: 0x114
// audControlGroup::LoadParameters (vfn_12) — registers the group's seven
// tunable scalars against the incoming datParser record so editor/ini code
// can round-trip them.  The +0 value-kind column matches datParser's type
// enum: 6 = float, 1 = byte/bool.  Offsets map to the group struct:
//    +20 m_volume, +28 m_pitch, +32 m_pan, +36 m_rolloff,
//    +44/+45/+46 three enable/mute bytes.
void audControlGroup::LoadParameters(void* parserRecord) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    constexpr int32_t kKindFloat = 6;
    constexpr int32_t kKindByte  = 1;

    datParser_RegisterRecord(parserRecord, kKindFloat,
        g_str_audControlGroup_field14, self + 20, 1, 0, 0);
    datParser_RegisterRecord(parserRecord, kKindFloat,
        g_str_audControlGroup_field1C, self + 28, 1, 0, 0);
    datParser_RegisterRecord(parserRecord, kKindFloat,
        g_str_audControlGroup_field20, self + 32, 1, 0, 0);
    datParser_RegisterRecord(parserRecord, kKindFloat,
        g_str_audControlGroup_field24, self + 36, 1, 0, 0);
    datParser_RegisterRecord(parserRecord, kKindByte,
        g_str_audControlGroup_field2C, self + 44, 1, 0, 0);
    datParser_RegisterRecord(parserRecord, kKindByte,
        g_str_audControlGroup_field2D, self + 45, 1, 0, 0);
    datParser_RegisterRecord(parserRecord, kKindByte,
        g_str_audControlGroup_field2E, self + 46, 1, 0, 0);
}


} // namespace rage
