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
        nop_8240E6D0("audVoiceSfx::PlayByEntry - invalid entry (%p)", entry);  /* UNVERIFIED — string not found in binary */
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

// External logging function (no-op in release builds)
extern "C" void nop_8240E6D0(const char* fmt, ...);

// External memory free function
extern "C" void rage_free(void* ptr);

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
    nop_8240E6D0(g_str_audVoice_PlayByEntry);
}

/**
 * audVoice::Play @ 0x821631A0 | size: 0xC
 * Pure virtual stub — plays audio with no arguments.
 */
void audVoice::Play() {
    nop_8240E6D0(g_str_audVoice_Play);
}

/**
 * audVoice::Stop @ 0x821631B0 | size: 0xC
 * Pure virtual stub — stops audio playback.
 */
void audVoice::Stop() {
    nop_8240E6D0(g_str_audVoice_Stop);
}

/**
 * audVoice::Release @ 0x821631C0 | size: 0xC
 * Pure virtual stub — releases audio resources.
 */
void audVoice::Release() {
    nop_8240E6D0(g_str_audVoice_Release);
}

/**
 * audVoice::SetEffect @ 0x821631D0 | size: 0xC
 * Pure virtual stub — applies an audio effect to the voice.
 */
void audVoice::SetEffect() {
    nop_8240E6D0(g_str_audVoice_SetEffect);
}

/**
 * audVoice::IsPlaying @ 0x821631E0 | size: 0x2C
 * Pure virtual stub — returns 0 (not playing) if base version called.
 */
bool audVoice::IsPlaying() {
    nop_8240E6D0(g_str_audVoice_IsPlaying);
    return false;
}

} // namespace rage
