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

// External engine entry points.
extern "C" void nop_8240E6D0(const char* fmt, ...);
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
        nop_8240E6D0("audVoiceSfx::PlayByEntry - invalid entry (%p)", entry);
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

} // namespace rage (first block: PlayByEntry)

// ═════════════════════════════════════════════════════════════════════════════
// rage::audVoice — Audio Voice Base Class
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

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
// audVoice::vfn_2 @ 0x82163190 | size: 0xC
//
// Unimplemented stub - logs a message and returns.
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::vfn_2() {
    nop_8240E6D0(reinterpret_cast<const char*>(0x82035770));
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::vfn_4 @ 0x821631A0 | size: 0xC
//
// Unimplemented stub - logs a message and returns.
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::vfn_4() {
    nop_8240E6D0(reinterpret_cast<const char*>(0x82035794));
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::vfn_7 @ 0x821631B0 | size: 0xC
//
// Unimplemented stub - logs format string "char_%l_%s".
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::vfn_7() {
    nop_8240E6D0(g_str_audVoiceSfx_formatChar);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::vfn_8 @ 0x821631C0 | size: 0xC
//
// Unimplemented stub - logs format string "_%s".
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::vfn_8() {
    nop_8240E6D0(g_str_audVoiceSfx_formatSuffix);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::vfn_17 @ 0x821631D0 | size: 0xC
//
// Unimplemented stub - logs format string "_%l".
// ─────────────────────────────────────────────────────────────────────────────
void audVoice::vfn_17() {
    nop_8240E6D0(g_str_audVoiceSfx_formatLevel);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoice::vfn_18 @ 0x821631E0 | size: 0x2C
//
// Unimplemented stub - logs a message and returns 0.
// Likely a query function that would return voice state or handle.
// ─────────────────────────────────────────────────────────────────────────────
int audVoice::vfn_18() {
    nop_8240E6D0(g_str_audVoiceSfx_ankle);
    return 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// Audio Command Ring Buffer System
// ═════════════════════════════════════════════════════════════════════════════

// Binary globals for the audio command ring buffer.
extern uint32_t  lbl_825EE220;   // audio command write index
extern uint32_t* lbl_825EBD2C;   // audio command ring buffer base

namespace {

uint32_t& g_cmdWriteIndex = lbl_825EE220;
uint32_t*& g_pCmdRing     = lbl_825EBD2C;

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

// Extern: global voice free list sentinel @ 0x825F5758
// The voice system uses an intrusive doubly-linked list with prev/next at +4/+8.
extern uint8_t lbl_825F5758[];

// Float constant table for volume conversion (maps 0.0..1.0 to integer range)
extern const float lbl_8202D3C4;   // volume scale factor
extern const float lbl_8202CFC4;   // volume offset (subtracted before scaling)

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceSfx_32F8_p45 @ 0x821632F8 | size: 0x5C
//
// Dequeues the first voice node from an intrusive doubly-linked list (the
// voice free list), then re-links it into a second list (the active voice
// list sentinel at lbl_825F5758+4). Returns the dequeued node, or nullptr
// if the list is empty or points to itself (sentinel-only).
// ─────────────────────────────────────────────────────────────────────────────
void* audVoiceSfx_32F8_p45(void* listHead) // @ 0x821632F8
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

    // Insert into active list at lbl_825F5758+4 (sentinel)
    uint32_t* activeSentinel = reinterpret_cast<uint32_t*>(&lbl_825F5758[4]);
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
// audVoiceSfx_32F8_p45. If allocation fails and the audio system is active,
// logs a warning. Returns the allocated node (or nullptr).
// ─────────────────────────────────────────────────────────────────────────────

// Free list sentinel for stream voices @ 0x825F57C0
extern uint8_t lbl_825F57C0[];
// Audio system active flag pointer @ 0x825F5810
extern uint32_t* lbl_825F5810;

void* audVoiceStream_GetStreamRef() // @ 0x82163D28
{
    void* node = audVoiceSfx_32F8_p45(&lbl_825F57C0[0]);
    if (node == nullptr) {
        // Check if audio system is active (flag at offset +4 of the pointer)
        uint8_t* sysPtr = reinterpret_cast<uint8_t*>(lbl_825F5810);
        if (sysPtr != nullptr && sysPtr[4] != 0) {
            nop_8240E6D0("audVoiceStream::GetStreamRef - no free stream voices");
        }
    }
    return node;
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream_AEE8_g @ 0x821AAEE8 | size: 0x70
//
// Low-level audio command enqueue. Writes a command header (class, id,
// param count) and copies parameter words into the ring buffer.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream_AEE8_g(uint8_t cmdClass, uint16_t cmdId,  // @ 0x821AAEE8
                           const uint32_t* params, uint8_t paramCount)
{
    PushAudioCommand(cmdClass, cmdId, paramCount, params);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream_AF58_g @ 0x821AAF58 | size: 0x54
//
// Sends a boolean-valued audio command (cmdId 4124 = 0x101C).
// Packs {voiceHandle, boolValue} as two uint32_t params.
// Used to enable/disable a voice stream.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream_AF58_g(uint32_t voiceHandle, bool enabled) // @ 0x821AAF58
{
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = enabled ? 1u : 0u;

    audVoiceStream_AEE8_g(1, 4124, params, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream_AFB0_g @ 0x821AAFB0 | size: 0x6C
//
// Sends a volume-pair command (cmdId 4125 = 0x101D).
// Converts two float volumes (0.0..1.0 range) to integer scale and packs
// {voiceHandle, scaledVol1, scaledVol2} as three uint32_t params.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream_AFB0_g(uint32_t voiceHandle, float vol1, float vol2) // @ 0x821AAFB0
{
    uint32_t params[3];
    params[0] = voiceHandle;

    float offset = lbl_8202CFC4;
    float scale  = lbl_8202D3C4;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((vol1 - offset) * scale));
    params[2] = static_cast<uint32_t>(static_cast<int32_t>((vol2 - offset) * scale));

    audVoiceStream_AEE8_g(1, 4125, params, 3);
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
        nop_8240E6D0("audVoiceSfx::SetReverbVolume - bank not resident for SFX entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* sfxData = static_cast<uint32_t*>(m_pSfxRef);
    uint32_t voiceHandle = sfxData[0];

    float offset = lbl_8202CFC4;
    float scale  = lbl_8202D3C4;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audVoiceStream_AEE8_g(2, 16398, params, 2);
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
        nop_8240E6D0("audVoiceSfx::SetEffectVolume - bank not resident for SFX entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* sfxData = static_cast<uint32_t*>(m_pSfxRef);
    uint32_t voiceHandle = sfxData[0];

    float offset = lbl_8202CFC4;
    float scale  = lbl_8202D3C4;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audVoiceStream_AEE8_g(2, 16401, params, 2);
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
        nop_8240E6D0("audVoiceStream::SetReverbVolume - bank not resident for stream entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* streamData = reinterpret_cast<uint32_t*>(field_0x000c);
    uint32_t voiceHandle = streamData[0];

    float offset = lbl_8202CFC4;
    float scale  = lbl_8202D3C4;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audVoiceStream_AEE8_g(3, 8207, params, 2);
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
        nop_8240E6D0("audVoiceStream::SetEffectVolume - bank not resident for stream entry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* streamData = reinterpret_cast<uint32_t*>(field_0x000c);
    uint32_t voiceHandle = streamData[0];

    float offset = lbl_8202CFC4;
    float scale  = lbl_8202D3C4;
    uint32_t params[2];
    params[0] = voiceHandle;
    params[1] = static_cast<uint32_t>(static_cast<int32_t>((volume - offset) * scale));

    audVoiceStream_AEE8_g(3, 8209, params, 2);
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
        nop_8240E6D0("audVoiceStream::Prime - bank not resident for audBankEntry (%p)",
                      bankEntry);
        return;
    }

    uint32_t* streamData = reinterpret_cast<uint32_t*>(field_0x000c);
    uint8_t* entryBytes = static_cast<uint8_t*>(bankEntry);
    uint32_t entryDataPtr = *reinterpret_cast<uint32_t*>(entryBytes + 8);

    uint32_t params[3];
    params[0] = streamData[0];   // voice handle
    params[1] = static_cast<uint32_t>(bankSlot);
    params[2] = entryDataPtr;

    audVoiceStream_AEE8_g(3, 8194, params, 3);

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
extern void aud_B148(void* streamRef, int32_t bankSlot, uint32_t bankEntryData,
                     float volume, float pitch, float pan, float playVariance,
                     uint32_t userParamA, uint32_t userParamB, uint8_t userParamC);
extern const float lbl_8202D110;     // play variance threshold
extern const float lbl_8202D998[];   // play variance constants

void audVoiceStream::PlayByEntry(void* bankEntry, float volume, float pitch, // @ 0x82163ED8
                                  float pan, float playVariance,
                                  uint32_t userParamA, uint32_t userParamB,
                                  uint8_t userParamC)
{
    int32_t bankSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 4);

    if (bankSlot == -1) {
        nop_8240E6D0("audVoiceStream::PlayByEntry - bank not resident for audBankEntry (%p)",
                      bankEntry);
        return;
    }

    if (playVariance > lbl_8202D110) {
        float randomSample = aud_2458(this);
        playVariance = randomSample * lbl_8202D998[1];
    }

    uint32_t bankEntryData = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(bankEntry) + 8);

    aud_B148(reinterpret_cast<void*>(field_0x000c), bankSlot, bankEntryData,
             volume, pitch, pan, playVariance,
             userParamA, userParamB, userParamC);
}

// ═════════════════════════════════════════════════════════════════════════════
// audVoiceStream Constructor / audSystem Destructor / audBankMgr Destructor
// ═════════════════════════════════════════════════════════════════════════════

// audVoice vtable (base class) and audVoiceStream vtable
extern void* lbl_82025AFC[];   // audVoice base vtable
extern void* lbl_82025ADC[];   // audVoiceStream vtable (intermediate)
extern void* lbl_82025A7C[];   // audVoiceStream vtable (final)

// ─────────────────────────────────────────────────────────────────────────────
// audVoiceStream_ctor_3BA8 @ 0x82163BA8 | size: 0xA0
//
// Constructor for audVoiceStream. Sets the vtable, enqueues an audio command
// to register the voice (cmdId 8199 = 0x2007), then re-links the node in
// the intrusive list to use the stream vtable.
// ─────────────────────────────────────────────────────────────────────────────
void audVoiceStream_ctor_3BA8(void* self) // @ 0x82163BA8
{
    uint32_t* voice = static_cast<uint32_t*>(self);

    // Read the ref pointer at +12
    uint32_t* refPtr = reinterpret_cast<uint32_t*>(voice[3]);

    // Write intermediate vtable
    voice[0] = reinterpret_cast<uintptr_t>(lbl_82025ADC);

    // Enqueue voice registration command
    uint32_t voiceHandle = refPtr[0];
    uint32_t params[2];
    params[0] = voiceHandle;

    // cmdClass=3, cmdId=8199, 2 params
    audVoiceStream_AEE8_g(3, 8199, params, 2);

    // Set final vtable
    voice[0] = reinterpret_cast<uintptr_t>(lbl_82025A7C);

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
// audSystem_ctor_6DC0 @ 0x82166DC0 | size: 0x78
//
// Destructor for the audSystem class. Sets base class vtables, calls the
// gameLoop audio teardown, installs the final vtable, and optionally frees
// memory (scalar deleting destructor pattern).
// ─────────────────────────────────────────────────────────────────────────────
extern void gameLoop_DestroyAudio_27A8(void* self);
extern void rage_free_00C0(void* ptr);
extern void* lbl_82025D2C[];  // audSystem intermediate vtable A
extern void* lbl_82025D3C[];  // audSystem intermediate vtable B
extern void* lbl_82027B34[];  // audSystem final (base) vtable

void audSystem_ctor_6DC0(void* self, uint32_t flags) // @ 0x82166DC0
{
    uint32_t* obj = static_cast<uint32_t*>(self);

    // Install intermediate vtables
    obj[0] = reinterpret_cast<uintptr_t>(lbl_82025D2C);
    uint32_t* subObj = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(obj) + 4);
    subObj[0] = reinterpret_cast<uintptr_t>(lbl_82025D3C);

    // Call base class teardown
    gameLoop_DestroyAudio_27A8(subObj);

    // Install final vtable
    obj[0] = reinterpret_cast<uintptr_t>(lbl_82027B34);

    // Scalar delete if flag bit 0 is set
    if (flags & 1) {
        rage_free_00C0(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// audBankMgr_2D48_wrh @ 0x82162D48 | size: 0xA8
//
// Destructor for audBankMgr. Installs the audBankMgr vtable, iterates all
// hash buckets, calling the destructor (vfn_0) on each bank's resource
// pointer, then frees the hash table storage twice (two internal arrays),
// and installs the final base vtable.
// ─────────────────────────────────────────────────────────────────────────────
extern void* lbl_82025768[];   // audBankMgr vtable (intermediate)
extern void* lbl_82027644[];   // audBankMgr vtable (final/base)
extern void  util_5140(void* hashTable);

void audBankMgr_2D48_wrh(void* self) // @ 0x82162D48
{
    uint32_t* mgr = static_cast<uint32_t*>(self);

    // Install intermediate vtable
    mgr[0] = reinterpret_cast<uintptr_t>(lbl_82025768);

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
    util_5140(hashTablePtr);
    util_5140(hashTablePtr);

    // Install final base vtable
    mgr[0] = reinterpret_cast<uintptr_t>(lbl_82027644);
}

// ═════════════════════════════════════════════════════════════════════════════
// audMsgSink — Audio Message Sink Functions
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// audMsgSink_3A10_g @ 0x821B3A10 | size: 0x5C
//
// Resets an audio message sink. Calls vfn_4 (Stop) on the control at +20,
// then clears the state: sets volume to 0.0, clears the handle at +24,
// and zeroes the three flag bytes at +32, +33, +34.
// ─────────────────────────────────────────────────────────────────────────────
extern const float lbl_8202D110;  // 0.0f constant (also used as variance threshold)

void audMsgSink_3A10_g(void* self) // @ 0x821B3A10
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
// audMsgSink_8598_g @ 0x821B8598 | size: 0x68
//
// Releases/cleans up an audio message sink resource. If the object pointer
// at +0 is non-null, calls vfn_4 (Stop) on it, then sets +0 to null.
// If already null, just ensures it stays null.
// ─────────────────────────────────────────────────────────────────────────────
void audMsgSink_8598_g(void* self) // @ 0x821B8598
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

// Global array of 4 audControl3d objects (each 176 bytes) @ lbl_825F5810
extern uint8_t lbl_825F5810_arr[];
extern void audControl3d_13C8_w(void* control);

// ─────────────────────────────────────────────────────────────────────────────
// audControl3d_C678_2hr @ 0x8257C678 | size: 0x44
//
// Iterates over a global array of 4 audControl3d objects (stride 176 bytes)
// and calls audControl3d_13C8_w (a reset/init method) on each one.
// ─────────────────────────────────────────────────────────────────────────────
void audControl3d_C678_2hr() // @ 0x8257C678
{
    uint8_t* control = lbl_825F5810_arr;

    for (int32_t i = 3; i >= 0; i--) {
        audControl3d_13C8_w(control);
        control += 176;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// Bank Entry Type Checks
// ═════════════════════════════════════════════════════════════════════════════

// Global pointer to audio bank entry data base @ 0x825EE224
extern uint32_t* lbl_825EE224_ptr;

// ─────────────────────────────────────────────────────────────────────────────
// aud_4D48 @ 0x82164D48 | size: 0x44
//
// Checks if a bank entry is of streaming type. Reads the entry offset from
// the voice at +16, looks up the type byte at +16 of the entry data, and
// returns true if the type is 25 (streaming) or 28 (alternate streaming).
// ─────────────────────────────────────────────────────────────────────────────
bool aud_4D48(void* voice) // @ 0x82164D48
{
    uint32_t* v = static_cast<uint32_t*>(voice);
    int32_t entryOffset = static_cast<int32_t>(v[4]); // +16

    if (entryOffset < 0) {
        return false;
    }

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(lbl_825EE224_ptr);
    uint8_t entryType = entryBase[entryOffset + 16];
    int8_t typeSigned = static_cast<int8_t>(entryType);

    return (typeSigned == 25 || typeSigned == 28);
}

// ─────────────────────────────────────────────────────────────────────────────
// aud_4D90 @ 0x82164D90 | size: 0x38
//
// Checks if a bank entry is of XMA (compressed audio) type. Reads the entry
// offset from the voice at +16, looks up the type byte, and returns true
// if the type is 27 (XMA).
// ─────────────────────────────────────────────────────────────────────────────
bool aud_4D90(void* voice) // @ 0x82164D90
{
    uint32_t* v = static_cast<uint32_t*>(voice);
    int32_t entryOffset = static_cast<int32_t>(v[4]); // +16

    if (entryOffset < 0) {
        return false;
    }

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(lbl_825EE224_ptr);
    uint8_t entryType = entryBase[entryOffset + 16];

    return (entryType == 27);
}

// ═════════════════════════════════════════════════════════════════════════════
// Audio Scene Helper
// ═════════════════════════════════════════════════════════════════════════════

// Global audio scene pointer @ 0x825E7864
extern void* lbl_825E7864;
extern void aud_67E0(void* self);
extern void phDemoWorld_67D0_g(void* list, void* obj, uint32_t flags);

// Audio scene list @ 0x825E7878
extern uint8_t lbl_825E7878[];

// ─────────────────────────────────────────────────────────────────────────────
// aud_AE30 @ 0x8246AE30 | size: 0x50
//
// Wraps audio scene cleanup. Calls aud_67E0 (which performs the main scene
// teardown), then if the input pointer is non-null, calls phDemoWorld_67D0_g
// to register it in the audio scene's object list. Returns 0 (null).
// ─────────────────────────────────────────────────────────────────────────────
void* aud_AE30(void* obj) // @ 0x8246AE30
{
    aud_67E0(obj);

    if (obj != nullptr) {
        phDemoWorld_67D0_g(lbl_825E7878, obj, 0x81020007);
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// aud_7B70 @ 0x82447B70 | size: 0x18
//
// Global thunk that loads the audio scene singleton pointer, reads its
// vtable, and tail-calls vfn_4 (vtable slot 4, byte offset 16).
// Effectively: return g_pAudioScene->vfn_4();
// ─────────────────────────────────────────────────────────────────────────────
void aud_7B70() // @ 0x82447B70
{
    void* scene = lbl_825E7864;
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(scene);
    typedef void (*VFn)(void*);
    VFn fn = reinterpret_cast<VFn>((*vtbl)[4]);
    fn(scene);
}

// ─────────────────────────────────────────────────────────────────────────────
// aud_7A80 @ 0x82447A80 | size: 0x30
//
// Global thunk that loads the audio scene singleton, prepends it as the
// first argument, and tail-calls vfn_5 (vtable slot 5, byte offset 20).
// Effectively: g_pAudioScene->vfn_5(arg1, arg2, arg3, arg4);
// ─────────────────────────────────────────────────────────────────────────────
void aud_7A80(void* a1, void* a2, void* a3, void* a4) // @ 0x82447A80
{
    void* scene = lbl_825E7864;
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(scene);
    typedef void (*VFn)(void*, void*, void*, void*, void*);
    VFn fn = reinterpret_cast<VFn>((*vtbl)[5]);
    fn(scene, a1, a2, a3, a4);
}

// ─────────────────────────────────────────────────────────────────────────────
// aud_7AB0 @ 0x82447AB0 | size: 0x30
//
// Global thunk that loads the audio scene singleton, prepends it as the
// first argument, and tail-calls vfn_6 (vtable slot 6, byte offset 24).
// Effectively: g_pAudioScene->vfn_6(arg1, arg2, arg3, arg4);
// ─────────────────────────────────────────────────────────────────────────────
void aud_7AB0(void* a1, void* a2, void* a3, void* a4) // @ 0x82447AB0
{
    void* scene = lbl_825E7864;
    uint32_t** vtbl = reinterpret_cast<uint32_t**>(scene);
    typedef void (*VFn)(void*, void*, void*, void*, void*);
    VFn fn = reinterpret_cast<VFn>((*vtbl)[6]);
    fn(scene, a1, a2, a3, a4);
}

} // namespace rage
