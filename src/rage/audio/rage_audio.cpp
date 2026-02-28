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

} // namespace rage

