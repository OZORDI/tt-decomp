// Forward declaration: sin() shim used by SetParameter (definition below)
extern "C" double sin(double);
static double sin_compat(double x) { return sin(x); }

/**
 * pong_audio_effects.cpp — XAudio2 IXAPO Audio Effect Implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * @ 0x821AD040-0x821B26A8
 *
 * Implements the IXAPO + IXAPOParameters surface for all in-game effects.
 * The observed vtable re-uses slots 3 and 4 as parameter get/set by index
 * (IXAPOParameters), guarded by a process-wide XAPO critical section.
 *
 * Effects implemented in this file:
 *   - CShelvingFilterEffect  (low/high shelf EQ)
 *   - CPeakMeterEffect       (audio level metering)
 *   - CCompEffect            (dynamic range compressor)
 *   - CBiquadFilterEffect    (parametric biquad IIR)
 */

#include "pong_audio.hpp"
#include <cstdint>
#include <cstring>

// ── External kernel / XAPO runtime helpers ─────────────────────────────────
// ke_EnterOrLeaveXapoLock takes an encoded <class:slot> identifier; the
// caller selects Enter vs. Leave by passing kXapoLockParamEnter /
// kXapoLockParamLeave. All parameter accessors wrap their work in this pair.
extern "C" void ke_EnterOrLeaveXapoLock(uint32_t lockSelector);
extern "C" void xapo_UnpackBuffer(const void* desc, void* outInfo);
// Natural-log and pow helpers used by SetParameter on dB / Q inputs.
extern "C" double rage_pow_decibel_to_linear(double valueDb);  // 10^(x/20)
extern "C" double rage_pow10(double exponent);                  // 10^x

namespace {

using pong_audio::kXapoLockParamEnter;
using pong_audio::kXapoLockParamLeave;
using pong_audio::kHrOk;
using pong_audio::kHrInvalidArg;

// Width of the IXAPOParameters value slot (two 32-bit words — float value
// plus padding, copied as a single 64-bit store in the scaffold).
struct ParamSlot {
    float    value;
    uint32_t _tailPad;
};

// ── Reference constant pools extracted from lbl_8202D0E8 and friends ──
// These were literal pools near the switch tables.  We expose them as named
// constants so the function bodies stay free of hex and magic-number
// literals.
constexpr float kOne              = 1.0f;
constexpr float kSampleRateLoHz   = 1.0f;       // min allowed sample rate
constexpr float kFreqRangeLo      = 10.0f;      // min allowed cut-off (Hz)
constexpr float kFreqRangeHi      = 22000.0f;   // Nyquist-ish upper bound
constexpr float kGainDbLo         = -96.0f;     // min allowed gain (dB)
constexpr float kGainDbHi         = 24.0f;      // max allowed gain (dB)
constexpr float kQLo              = 0.1f;
constexpr float kQHi              = 10.0f;
constexpr float kShelfModeLo      = 1.0f;
constexpr float kShelfModeHi      = 3.0f;
constexpr float kTwoPi            = 6.2831853071795864769f;

// RAII for the XAPO parameter critical section so every early return is
// balanced without repeating the Leave call.
class XapoParamLock {
public:
    XapoParamLock()  { ke_EnterOrLeaveXapoLock(kXapoLockParamEnter); }
    ~XapoParamLock() { ke_EnterOrLeaveXapoLock(kXapoLockParamLeave); }
    XapoParamLock(const XapoParamLock&) = delete;
    XapoParamLock& operator=(const XapoParamLock&) = delete;
};

// Buffer descriptor emitted by xapo_UnpackBuffer (see Process paths).
struct UnpackedBuffer {
    uint8_t  _pad0;
    uint8_t  channelCount;
    uint8_t  _pad2[6];
    float*   pSamples;
};

// Clamp helper that avoids pulling in <algorithm> in a freestanding unit.
inline bool inRangeInclusive(float value, float lo, float hi) {
    return value >= lo && value <= hi;
}

inline void writeFloatSlot(void* outValue, float v) {
    ParamSlot slot{v, 0u};
    std::memcpy(outValue, &slot, sizeof(slot));
}

inline float readFloatSlot(const void* pValue) {
    float v;
    std::memcpy(&v, pValue, sizeof(v));
    return v;
}

} // anonymous namespace


// ===========================================================================
// CShelvingFilterEffect — Low/High Shelf EQ Filter
// vtable @ 0x8203B288
// ===========================================================================

// ---------------------------------------------------------------------------
// CShelvingFilterEffect::GetRegistrationProperties @ 0x821AE198
//
// Minimal IXAPO registration metadata.  The scaffold zeroes a small header
// struct and sets the "valid" byte to 1.  Layout of the output record:
//   +0x00 valid flag (u8 = 1)
//   +0x02 flags     (u16 = 0)
//   +0x04 reserved  (u8  = 0)
// ---------------------------------------------------------------------------
void CShelvingFilterEffect::GetRegistrationProperties(void* outProps) {
    struct XapoRegistration {
        uint8_t  valid;
        uint8_t  _pad1;
        uint16_t flags;
        uint8_t  reserved;
    };
    XapoRegistration record{};
    record.valid    = 1;
    record.flags    = 0;
    record.reserved = 0;
    std::memcpy(outProps, &record, sizeof(record));
}

// ---------------------------------------------------------------------------
// CShelvingFilterEffect::GetParameter @ 0x821AE1B8 | size 0x10C
//
// IXAPOParameters::GetParameter style accessor.  Selects one of seven scalar
// fields by index and writes it into a float-sized output slot.  Returns
// E_INVALIDARG for any out-of-range index.
// ---------------------------------------------------------------------------
int CShelvingFilterEffect::GetParameter(uint8_t paramIndex, void* outValue) {
    XapoParamLock lock;

    ParamSlot slot{0.0f, 0u};
    int hr = kHrOk;

    switch (paramIndex) {
        case 0: slot.value = static_cast<float>(this->m_sampleRate); break;
        case 1: slot.value = this->m_frequency;                      break;
        case 2: slot.value = this->m_lowGain;                        break;
        case 3: slot.value = this->m_highGain;                       break;
        case 4: slot.value = this->m_lowQ;                           break;
        case 5: slot.value = this->m_highQ;                          break;
        case 6: slot.value = static_cast<float>(this->m_mode);       break;
        default: hr = kHrInvalidArg; break;
    }
    std::memcpy(outValue, &slot, sizeof(slot));
    return hr;
}

// ---------------------------------------------------------------------------
// CShelvingFilterEffect::SetParameter @ 0x821AE2C8 | size 0x218
//
// Range-validates the incoming float against each parameter's permitted
// interval, then stores it.  Index 1 (frequency) additionally precomputes
// m_sinFreq = sin(2π·f / sampleRate) · 0.5f — used as the output gain of
// the cascaded biquad in Process.
// ---------------------------------------------------------------------------
int CShelvingFilterEffect::SetParameter(uint8_t paramIndex, const void* pValue) {
    XapoParamLock lock;

    if (paramIndex > 6) {
        return kHrInvalidArg;
    }
    const float v = readFloatSlot(pValue);

    switch (paramIndex) {
        case 0: {
            // sampleRate: validated against [kSampleRateLoHz, kFreqRangeHi],
            // stored as int32.
            if (!inRangeInclusive(v, kSampleRateLoHz, kFreqRangeHi)) {
                return kHrInvalidArg;
            }
            this->m_sampleRate = static_cast<int32_t>(v);
            return kHrOk;
        }
        case 1: {
            if (!inRangeInclusive(v, kFreqRangeLo, kFreqRangeHi)) {
                return kHrInvalidArg;
            }
            this->m_frequency = v;
            // Precompute the biquad output scaling factor.
            const double twoPiF = static_cast<double>(kTwoPi) * static_cast<double>(v);
            const double sampleRate = static_cast<double>(this->m_sampleRate);
            const double arg = (sampleRate > 0.0) ? (twoPiF / sampleRate) : 0.0;
            // atSingleton_1308 is the game's sin() wrapper — results get
            // scaled by 0.5 (lbl_8202D128+0 literal) in the scaffold.
            this->m_sinFreq = static_cast<float>(0.5 * sin_compat(arg));
            return kHrOk;
        }
        case 2: {
            if (!inRangeInclusive(v, kGainDbLo, kGainDbHi)) return kHrInvalidArg;
            this->m_lowGain  = v; return kHrOk;
        }
        case 3: {
            if (!inRangeInclusive(v, kGainDbLo, kGainDbHi)) return kHrInvalidArg;
            this->m_highGain = v; return kHrOk;
        }
        case 4: {
            if (!inRangeInclusive(v, kQLo, kQHi)) return kHrInvalidArg;
            this->m_lowQ     = v; return kHrOk;
        }
        case 5: {
            if (!inRangeInclusive(v, kQLo, kQHi)) return kHrInvalidArg;
            this->m_highQ    = v; return kHrOk;
        }
        case 6: {
            if (!inRangeInclusive(v, kShelfModeLo, kShelfModeHi)) return kHrInvalidArg;
            this->m_mode     = static_cast<int32_t>(v);
            return kHrOk;
        }
    }
    return kHrInvalidArg;

    // ── Note: sin_compat() is provided by the rage math shim.  On the 360
    //    build it routes to atSingleton_1308 (the f64 sin wrapper at
    //    0x82431308).  The 0.5× scale comes from the literal at
    //    lbl_8202D128+0x00 (stored as a double pair).
}

// ---------------------------------------------------------------------------
// CShelvingFilterEffect::UnlockForProcess @ 0x821AE4E0 | size 0x10
//
// Copies the effect-wide ref-count / status word (+0x04) to the caller.
// No allocation is freed because the biquad state is embedded in the
// object itself.
// ---------------------------------------------------------------------------
int CShelvingFilterEffect::UnlockForProcess(void* outParam) {
    std::memcpy(outParam, &this->m_refCount, sizeof(int32_t));
    return kHrOk;
}

// ---------------------------------------------------------------------------
// CShelvingFilterEffect::Process @ 0x821ADE28 | size 0x36C
//
// Cascaded transposed-direct-form-II biquad shelf filter.  For each
// interleaved channel it applies the low-shelf and/or high-shelf section
// based on m_mode, then scales the output by m_sinFreq.
//
// Coefficient layout (per section, 5 floats each):
//   low shelf  : this + 0x0C ..0x1C  (b0,b1,b2,a1,a2)
//   high shelf : this + 0x20 ..0x30
//
// State layout (per channel, 2 floats per section, interleaved):
//   low shelf  : this + 0x34..+0x38 for ch0, +0x4C..+0x50 for ch1
//   high shelf : this + 0x64..+0x68 for ch0, +0x7C..+0x80 for ch1
//
// The game always feeds a 64-frame block.  No state is required between
// blocks besides what is stored on the object.
// ---------------------------------------------------------------------------
void CShelvingFilterEffect::Process(void* pInputBuffer) {
    UnpackedBuffer info{};
    xapo_UnpackBuffer(pInputBuffer, &info);
    if (info.channelCount == 0) return;

    // Coefficients — hoisted out of the per-sample loop.
    auto* coeffsBase = reinterpret_cast<const float*>(
        reinterpret_cast<const uint8_t*>(this) + 12);
    const float b0Lo = coeffsBase[0];
    const float b1Lo = coeffsBase[1];
    const float b2Lo = coeffsBase[2];
    const float a1Lo = coeffsBase[3];
    const float a2Lo = coeffsBase[4];
    const float b0Hi = coeffsBase[5];
    const float b1Hi = coeffsBase[6];
    const float b2Hi = coeffsBase[7];
    const float a1Hi = coeffsBase[8];
    const float a2Hi = coeffsBase[9];

    const int   mode       = this->m_mode;
    const float outputGain = this->m_sinFreq;

    // Per-channel state base pointer — indexed as state[ch * 6 + i].
    auto* stateBase = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(this) + 52);

    constexpr int kFramesPerBlock = 64;
    const int channelCount = info.channelCount;

    for (int ch = 0; ch < channelCount; ++ch) {
        float* loState = &stateBase[ch * 6 + 0];
        float* hiState = &stateBase[ch * 6 + 2];

        for (int frame = 0; frame < kFramesPerBlock; ++frame) {
            const int sampleIdx = frame * channelCount + ch;
            float x = info.pSamples[sampleIdx];
            float y = x;

            if (mode == 1 || mode == 3) {
                // Low shelf, transposed direct form II.
                const float out = b0Lo * x + loState[0];
                loState[0] = b1Lo * x - a1Lo * out + loState[1];
                loState[1] = b2Lo * x - a2Lo * out;
                y = out;
            }
            if (mode == 2 || mode == 3) {
                const float in = y;
                const float out = b0Hi * in + hiState[0];
                hiState[0] = b1Hi * in - a1Hi * out + hiState[1];
                hiState[1] = b2Hi * in - a2Hi * out;
                y = out;
            }

            info.pSamples[sampleIdx] = outputGain * y;
        }
    }
}


// ===========================================================================
// CPeakMeterEffect — Audio Level Meter
// vtable @ 0x8203B260
// ===========================================================================

// ---------------------------------------------------------------------------
// CPeakMeterEffect::GetRegistrationProperties @ 0x821AD9D8 | size 0x1C
//
// Same shape as the shelving filter's variant — zero, then mark valid.
// ---------------------------------------------------------------------------
void CPeakMeterEffect::GetRegistrationProperties(void* outProps) {
    struct XapoRegistration {
        uint8_t  valid;
        uint8_t  _pad1;
        uint16_t flags;
        uint8_t  reserved;
    };
    XapoRegistration record{};
    record.valid    = 1;
    record.flags    = 0;
    record.reserved = 0;
    std::memcpy(outProps, &record, sizeof(record));
}

// ---------------------------------------------------------------------------
// CPeakMeterEffect::GetPeakValues @ 0x821AD9F8 | size 0x248
//
// Parameter-style getter that also *recomputes* the current peak values
// across the active channels.  For each channel the routine scans
// m_frameCount samples (unrolled 4×) and tracks the maximum absolute value
// seen in two parallel taps (m_pInputBufA/B vs. m_pInputBufC/D — the scaffold
// reads from four parallel base pointers, matching the four stored
// m_pInputBuf* pointers in the object).
//
// The `channelMask` argument matches a subset of channels to expose.  A non-
// zero `channelMask` with no active channels short-circuits with E_INVALIDARG,
// the same behaviour the scaffold shows at loc_821ADA38.
//
// Output layout (per scaffold):
//   +0x04        refCount (m_refCount copied through)
//   +0x00,+0x20  peak values for ch0 / ch1 (packed into the output struct
//                 supplied by the caller at 'outPeaks'; treated as opaque
//                 40-byte record elsewhere).
//
// Only the higher-level summary writes are lifted here — the detailed per-
// frame unroll is preserved with the same semantics but in normal C++ form.
// ---------------------------------------------------------------------------
int CPeakMeterEffect::GetPeakValues(uint8_t channelMask, void* outPeaks) {
    XapoParamLock lock;

    // The scaffold treats a non-zero channelMask byte as "invalid", i.e. the
    // effect only exposes the aggregate [ch0 + ch1] snapshot — callers must
    // pass 0 to get the full record.
    if (channelMask != 0) {
        return kHrInvalidArg;
    }

    // outPeaks is a struct written via an output pointer that is itself
    // fetched from *(void**)pPeakRecord — we mirror that indirection.
    void* pPeakRecord = *reinterpret_cast<void**>(outPeaks);

    // Copy through the ref-count snapshot so callers can detect stale data.
    const int32_t refSnapshot = this->m_refCount;
    std::memcpy(static_cast<uint8_t*>(pPeakRecord) + 4, &refSnapshot, sizeof(refSnapshot));

    const int32_t frameCount = this->m_frameCount;
    if (frameCount <= 0) {
        return kHrOk;
    }

    float peakL = 0.0f;
    float peakR = 0.0f;

    // Walk the two parallel taps for each channel.  The scaffold unrolls by
    // 4 for the common path; a tail loop handles the remainder.
    const float* inputA = this->m_pInputBufA;
    const float* inputB = this->m_pInputBufB;
    const float* inputC = this->m_pInputBufC;
    const float* inputD = this->m_pInputBufD;
    if (inputA == nullptr || inputB == nullptr || inputC == nullptr || inputD == nullptr) {
        return kHrOk;
    }

    const uint32_t channelCount = this->m_channelCount;
    const int strideElements = static_cast<int>(channelCount);

    auto accumulate = [&](const float* src, float& peak) {
        const int unrolled = frameCount & ~3;
        int i = 0;
        for (; i < unrolled; i += 4) {
            const float v0 = src[(i + 0) * strideElements];
            const float v1 = src[(i + 1) * strideElements];
            const float v2 = src[(i + 2) * strideElements];
            const float v3 = src[(i + 3) * strideElements];
            if (peak < v0) peak = v0;
            if (peak < v1) peak = v1;
            if (peak < v2) peak = v2;
            if (peak < v3) peak = v3;
        }
        for (; i < frameCount; ++i) {
            const float v = src[i * strideElements];
            if (peak < v) peak = v;
        }
    };

    accumulate(inputA, peakL);
    accumulate(inputC, peakR);

    // Peak values live at +0x00 and +0x20 inside the peak record.
    std::memcpy(static_cast<uint8_t*>(pPeakRecord) + 0x00, &peakL, sizeof(peakL));
    std::memcpy(static_cast<uint8_t*>(pPeakRecord) + 0x20, &peakR, sizeof(peakR));

    return kHrOk;
}

// ---------------------------------------------------------------------------
// CPeakMeterEffect::LockForProcess @ 0x821ADC40 | size 0x6C
//
// Validates the caller-supplied `mode` byte and acquires the XAPO parameter
// lock.  The scaffold keeps two code paths:
//   * mode != 0  → return E_INVALIDARG without releasing the enter lock (the
//                  caller is expected to retry).  We preserve that behaviour.
//   * mode == 0  → take the Leave transition to pair with a later Enter and
//                  return S_OK.
// ---------------------------------------------------------------------------
int CPeakMeterEffect::LockForProcess(uint8_t mode) {
    ke_EnterOrLeaveXapoLock(kXapoLockParamEnter);
    if (mode != 0) {
        return kHrInvalidArg;
    }
    ke_EnterOrLeaveXapoLock(kXapoLockParamLeave);
    return kHrOk;
}

// ---------------------------------------------------------------------------
// CPeakMeterEffect::UnlockForProcess @ 0x82468980 | size 0x10
//
// Publishes the currently-cached peak snapshot flags to the caller.
// ---------------------------------------------------------------------------
int CPeakMeterEffect::UnlockForProcess(void* outFlags) {
    std::memcpy(outFlags, &this->m_peakSnapshotFlags, sizeof(uint32_t));
    return kHrOk;
}

// ---------------------------------------------------------------------------
// CPeakMeterEffect::Process @ 0x821AD788 | size 0x250
//
// Audio pass-through with peak accumulation.  The effect does not modify the
// sample data; it scans the interleaved buffer and updates the internal peak
// tracker for the 40-byte output record read later by GetPeakValues().
// ---------------------------------------------------------------------------
void CPeakMeterEffect::Process(void* pInputBuffer) {
    UnpackedBuffer info{};
    xapo_UnpackBuffer(pInputBuffer, &info);
    if (info.channelCount == 0) return;

    const int channelCount  = info.channelCount;
    const int frameCount    = this->m_frameCount;
    if (frameCount <= 0) return;

    float* samples = info.pSamples;
    for (int ch = 0; ch < channelCount; ++ch) {
        float peak = 0.0f;
        for (int frame = 0; frame < frameCount; ++frame) {
            const float v = samples[frame * channelCount + ch];
            const float a = (v < 0.0f) ? -v : v;
            if (peak < a) peak = a;
        }
        // Publish the latest peak into the per-channel slot of the output
        // record.  The record base lives at m_pInputBufA and each channel
        // takes 32 bytes.
        if (this->m_pInputBufA != nullptr) {
            std::memcpy(reinterpret_cast<uint8_t*>(this->m_pInputBufA) + ch * 0x20,
                        &peak, sizeof(peak));
        }
    }
}


// ===========================================================================
// CCompEffect — Compressor / Limiter
// vtable @ 0x8203B54C
// ===========================================================================

// ---------------------------------------------------------------------------
// CCompEffect::GetParameter @ 0x821B1260 | size 0x140
//
// Compressor parameters (index → field, inferred from scaffold+layout):
//   0: m_threshold  (dB)
//   1: m_ratio
//   2: m_attackMs
//   3: m_releaseMs
//   4: m_makeupDb
// The actual field offsets live past the 32-byte biquad prologue the effect
// shares with the other XAPOs; callers always go through this accessor so
// the concrete offsets do not leak out.
// ---------------------------------------------------------------------------
int CCompEffect::GetParameter(uint8_t paramIndex, void* outValue) {
    XapoParamLock lock;

    if (paramIndex > 4) {
        return kHrInvalidArg;
    }

    // Field offsets for compressor parameters (reconstructed from the
    // scaffold's switch targets).  Kept internal to the accessor.
    static constexpr size_t kCompParamOffsets[5] = {
        /* threshold  */ 0x80,
        /* ratio      */ 0x84,
        /* attackMs   */ 0x88,
        /* releaseMs  */ 0x8c,
        /* makeupDb   */ 0x90,
    };

    const auto* base = reinterpret_cast<const uint8_t*>(this);
    float value;
    std::memcpy(&value, base + kCompParamOffsets[paramIndex], sizeof(value));
    writeFloatSlot(outValue, value);
    return kHrOk;
}

// ---------------------------------------------------------------------------
// CCompEffect::SetParameter @ 0x821B13A0 | size 0x2F4
//
// Range-validates each compressor parameter against its allowed interval
// and stores the normalised value back on the effect object.  The exact
// conversion used by each case (pow10 on dB, ms→seconds on time constants,
// etc.) mirrors the scaffold's pow/div sequences.
// ---------------------------------------------------------------------------
int CCompEffect::SetParameter(uint8_t paramIndex, const void* pValue) {
    XapoParamLock lock;

    if (paramIndex > 4) {
        return kHrInvalidArg;
    }
    const float v = readFloatSlot(pValue);

    // Ranges cribbed from the literal pool referenced by the scaffold.
    struct ParamDef { float lo, hi; size_t offset; bool decibelToLinear; };
    static constexpr ParamDef kParams[5] = {
        /* threshold  */ { -60.0f,   0.0f, 0x80, true  },
        /* ratio      */ {   1.0f,  20.0f, 0x84, false },
        /* attackMs   */ {   0.1f, 500.0f, 0x88, false },
        /* releaseMs  */ {   1.0f, 5000.f, 0x8c, false },
        /* makeupDb   */ {   0.0f,  24.0f, 0x90, true  },
    };

    const ParamDef& def = kParams[paramIndex];
    if (!inRangeInclusive(v, def.lo, def.hi)) {
        return kHrInvalidArg;
    }

    const float stored = def.decibelToLinear
        ? static_cast<float>(rage_pow_decibel_to_linear(static_cast<double>(v)))
        : v;

    auto* base = reinterpret_cast<uint8_t*>(this);
    std::memcpy(base + def.offset, &stored, sizeof(stored));
    return kHrOk;
}


// ===========================================================================
// CBiquadFilterEffect — Parametric Biquad IIR
// vtable @ 0x8203B598
// ===========================================================================

// ---------------------------------------------------------------------------
// CBiquadFilterEffect::GetParameter @ 0x821B22F0 | size 0x118
//
// Parameter indices:
//   0: m_sampleRate   (int32, returned as float)
//   1: m_centerFreq
//   2: m_gainDb       (scaled: log10-based, via the game's pow helper)
//   3: m_bandwidth
//   4: m_qFactor      (scaled through pow10 on return)
//
// The scaffold reads a default value of 1.0f from a constant pool before the
// switch (lbl_8202D128-0x18) and overwrites it on every successful index.
// ---------------------------------------------------------------------------
int CBiquadFilterEffect::GetParameter(uint8_t paramIndex, void* outValue) {
    XapoParamLock lock;

    if (paramIndex > 4) {
        writeFloatSlot(outValue, kOne);
        return kHrInvalidArg;
    }

    float value = kOne;
    switch (paramIndex) {
        case 0:
            value = static_cast<float>(this->m_sampleRate);
            break;
        case 1:
            value = this->m_centerFreq;
            break;
        case 2: {
            // Stored as natural gain; surfaced as dB via 20·log10.
            const double linear = static_cast<double>(this->m_gainDb);
            const double scale  = static_cast<double>(kOne); // lbl_8202D128+0x00
            value = static_cast<float>(rage_pow10(linear) * scale);
            break;
        }
        case 3:
            value = this->m_bandwidth;
            break;
        case 4: {
            const double linear = static_cast<double>(this->m_qFactor);
            const double scale  = static_cast<double>(kOne);
            value = static_cast<float>(rage_pow10(linear) * scale);
            break;
        }
    }

    writeFloatSlot(outValue, value);
    return kHrOk;
}

// ---------------------------------------------------------------------------
// CBiquadFilterEffect::SetParameter @ 0x821B2408 | size 0x214
//
// Mirror of the getter: validate against the allowed interval, then store.
// Parameter 2 and 4 apply a log10 conversion before storage so that the
// cached value is the natural-domain exponent used inside Process.
// ---------------------------------------------------------------------------
int CBiquadFilterEffect::SetParameter(uint8_t paramIndex, const void* pValue) {
    XapoParamLock lock;

    if (paramIndex > 4) {
        return kHrInvalidArg;
    }
    const float v = readFloatSlot(pValue);

    struct ParamDef { float lo, hi; };
    static constexpr ParamDef kParams[5] = {
        /* sampleRate */ { kSampleRateLoHz, kFreqRangeHi },
        /* centerFreq */ { kFreqRangeLo,    kFreqRangeHi },
        /* gainDb     */ { kGainDbLo,       kGainDbHi    },
        /* bandwidth  */ { kQLo,            kQHi         },
        /* qFactor    */ { kQLo,            kQHi         },
    };
    const ParamDef& def = kParams[paramIndex];
    if (!inRangeInclusive(v, def.lo, def.hi)) {
        return kHrInvalidArg;
    }

    switch (paramIndex) {
        case 0: this->m_sampleRate = static_cast<int32_t>(v); return kHrOk;
        case 1: this->m_centerFreq = v;                        return kHrOk;
        case 2:
            // Store as ln/pow domain so Process can multiply without an
            // extra conversion.
            this->m_gainDb = static_cast<float>(
                rage_pow_decibel_to_linear(static_cast<double>(v)));
            return kHrOk;
        case 3: this->m_bandwidth = v;                         return kHrOk;
        case 4:
            this->m_qFactor = static_cast<float>(
                rage_pow_decibel_to_linear(static_cast<double>(v)));
            return kHrOk;
    }
    return kHrInvalidArg;
}

// ===========================================================================
// Local math shim — sin() routed through the game's atSingleton_1308 on the
// 360 build.  The host build just pulls the libc symbol; this unit stays
// free of <cmath> to match the rest of the decomp.
// ===========================================================================
// sin_compat moved to top of file (forward declaration before use)

//
// ── TODOs ─────────────────────────────────────────────────────────────────
//  * CCompEffect::Process @ 0x821B0FD8 — RMS → envelope → gain-reduction
//    loop.  Scaffold has no pseudocode; needs hand-lift from the PPC asm.
//  * CBiquadFilterEffect::Process @ 0x821B1FA0 — standard biquad direct
//    form II; state layout matches the shelving filter but without the
//    mode switch.  Straightforward once coefficient offsets are confirmed.
//  * CBiquadFilterEffect::CalcInputFrames @ 0x821B1F40 — small, likely
//    a pass-through.
//  * CDelayEffect full surface (vfn_1/2/3/4/6/7) — not touched in this
//    batch.
//  * rage_pow10 / rage_pow_decibel_to_linear / sin_compat — all three
//    stubs rely on host-side math runtime; swap for atSingleton_1308 and
//    the pow helper @ 0x82432450 when lifting those call sites.
//
