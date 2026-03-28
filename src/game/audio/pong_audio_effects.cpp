/**
 * pong_audio_effects.cpp — XAudio2 IXAPO Audio Effect Implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * @ 0x821AD040-0x821B26A8
 * 
 * Implements IXAPO (XAudio2 Audio Processing Object) interface for various
 * audio effects used in the game. These effects process audio in real-time
 * during gameplay.
 * 
 * IXAPO Interface (COM-style vtable):
 *   [0] Destructor
 *   [1] ScalarDtor (COM reference counting destructor)
 *   [2] GetRegistrationProperties - Returns effect metadata
 *   [3] IsInputFormatSupported - Validates input audio format
 *   [4] LockForProcess - Prepares effect for processing
 *   [5] UnlockForProcess - Releases processing resources
 *   [6] Process - Main DSP processing function
 *   [7] CalcInputFrames/CalcOutputFrames - Frame count calculations
 * 
 * Effects Implemented:
 *   - CShelvingFilterEffect: Low/high shelf EQ filter (bass/treble boost/cut)
 *   - CPeakMeterEffect: Audio level metering
 *   - CDelayEffect: Echo/delay effect
 *   - CCompEffect: Dynamic range compressor
 *   - CBiquadFilterEffect: Parametric EQ (biquad IIR filter)
 */

#include "pong_audio.hpp"
#include <cstdint>
#include <cstring>

// External kernel/XAPO functions
extern "C" void ke_EnterCriticalSection(uint32_t lockId);
extern "C" void ke_LeaveCriticalSection(uint32_t lockId);
extern "C" void xapo_UnpackBuffer(void* desc, void* outInfo);

// ============================================================================
// CShelvingFilterEffect — Low/High Shelf EQ Filter
// @ vtable 0x8203B288
// ============================================================================

/**
 * CShelvingFilterEffect::GetRegistrationProperties
 * @ 0x821AE198 | size: 0x1C
 * 
 * Returns IXAPO registration properties describing this effect.
 * This is a stub implementation.
 */
/**
 * CShelvingFilterEffect::GetRegistrationProperties @ 0x821AE198 | size: 0x1C
 *
 * Fills an XAPO_REGISTRATION_PROPERTIES output struct with default values.
 * Zeroes the struct and sets the "valid" flag byte to 1.
 * Signature: void GetRegistrationProperties(this, XAPO_REGISTRATION_PROPERTIES* outProps)
 */
void CShelvingFilterEffect::GetRegistrationProperties(void* outProps) {
    *(uint16_t*)((char*)outProps + 2) = 0;
    *(uint8_t*)((char*)outProps + 0) = 1;   // valid flag
    *(uint8_t*)((char*)outProps + 4) = 0;
}

/**
 * CShelvingFilterEffect::IsInputFormatSupported
 * @ 0x821AE1B8 | size: 0x10C
 * 
 * Validates input audio format and returns effect parameters.
 * This is a stub implementation.
 */
/**
 * CShelvingFilterEffect::IsInputFormatSupported @ 0x821AE1B8 | size: 0x10C
 *
 * XAPO parameter getter. Takes a parameter index (0-6) and writes
 * the current value to the output struct. Returns 0 on success,
 * or E_INVALIDARG (0x80004005) if the index is out of range.
 *
 * Parameters (from scaffold switch-case analysis):
 *   0: m_sampleRate (+148) — returned as float-converted int32
 *   1: m_frequency (+152)
 *   2: m_lowGain (+160)
 *   3: m_highGain (+164)
 *   4: m_lowQ (+168)
 *   5: m_highQ (+172)
 *   6: m_mode (+176)
 */
int CShelvingFilterEffect::IsInputFormatSupported(uint8_t paramIndex, void* outValue) {
    // Default output to 0.0f
    struct ParamValue { float value; uint32_t pad; };
    ParamValue result = { 0.0f, 0 };

    ke_EnterCriticalSection(0x10001);  // XAPO lock

    int hr = 0;  // S_OK

    switch (paramIndex) {
        case 0: result.value = (float)*(int32_t*)((char*)this + 148); break;  // sampleRate
        case 1: result.value = *(float*)((char*)this + 152); break;  // frequency
        case 2: result.value = *(float*)((char*)this + 160); break;  // lowGain
        case 3: result.value = *(float*)((char*)this + 164); break;  // highGain
        case 4: result.value = *(float*)((char*)this + 168); break;  // lowQ
        case 5: result.value = *(float*)((char*)this + 172); break;  // highQ
        case 6: result.value = *(float*)((char*)this + 176); break;  // mode
        default:
            hr = (int)0x80004005;  // E_INVALIDARG
            break;
    }

    // Write result to output
    *(uint64_t*)outValue = *(uint64_t*)&result;

    ke_LeaveCriticalSection(0x10003);  // XAPO unlock
    return hr;
}

/**
 * CShelvingFilterEffect::UnlockForProcess
 * @ 0x821AE4E0 | size: 0x10
 * 
 * Releases processing resources after audio processing is complete.
 * This is a stub implementation.
 */
/**
 * CShelvingFilterEffect::UnlockForProcess @ 0x821AE4E0 | size: 0x10
 *
 * Copies the effect's parameter data (field at +4) to the output pointer.
 * Returns 0 (S_OK).
 */
int CShelvingFilterEffect::UnlockForProcess(void* outParam) {
    *(uint32_t*)outParam = *(uint32_t*)((char*)this + 4);
    return 0;
}

/**
 * CShelvingFilterEffect::Process
 * @ 0x821ADE28 | size: 0x36C
 * 
 * Main DSP processing function - applies shelving EQ filter to audio samples.
 * 
 * Implements a biquad IIR filter with the following difference equation:
 *   y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
 * 
 * Supports three modes:
 *   1: Low shelf only (bass boost/cut)
 *   2: High shelf only (treble boost/cut)
 *   3: Both shelves (full EQ)
 * 
 * This is a stub implementation.
 */
/**
 * CShelvingFilterEffect::Process @ 0x821ADE28 | size: 0x36C
 *
 * Main DSP processing function — applies shelving EQ filter to audio.
 *
 * Implements a cascaded biquad IIR filter with two sections:
 *   Section 1 (low shelf):  coefficients at this+12..+28, state at +52/+76
 *   Section 2 (high shelf): coefficients at this+32..+48, state at +100/+124
 *
 * Each biquad section computes:
 *   y[n] = b0*x[n] + state[0]
 *   state[0] = b1*x[n] - a1*y[n] + state[1]
 *   state[1] = b2*x[n] - a2*y[n]
 * (transposed direct form II)
 *
 * Mode (+148) controls which sections are active:
 *   1: Low shelf only
 *   2: High shelf only
 *   3: Both shelves (cascaded)
 *
 * After filtering, output is scaled by m_sinFreq (+156).
 * Processes interleaved stereo: even samples = left, odd = right.
 * Each channel has its own filter state (offsets differ by channel).
 *
 * @param pInputBuffer   XAPO input buffer descriptor
 */
void CShelvingFilterEffect::Process(void* pInputBuffer) {
    // Unpack XAPO buffer descriptor
    struct BufferInfo { uint8_t pad; uint8_t channelCount; uint8_t pad2[6]; float* pSamples; };
    BufferInfo info;
    xapo_UnpackBuffer(pInputBuffer, &info);

    if (info.channelCount == 0) return;

    int mode = *(int32_t*)((char*)this + 148);  // m_mode

    // Filter coefficients — low shelf (section 1)
    float b0_lo = *(float*)((char*)this + 12);
    float b1_lo = *(float*)((char*)this + 16);
    float b2_lo = *(float*)((char*)this + 20);
    float a1_lo = *(float*)((char*)this + 24);
    float a2_lo = *(float*)((char*)this + 28);

    // Filter coefficients — high shelf (section 2)
    float b0_hi = *(float*)((char*)this + 32);
    float b1_hi = *(float*)((char*)this + 36);
    float b2_hi = *(float*)((char*)this + 40);
    float a1_hi = *(float*)((char*)this + 44);
    float a2_hi = *(float*)((char*)this + 48);

    // Output gain
    float outputGain = *(float*)((char*)this + 156);

    // Process each stereo pair (channel stride = 76 - 52 = 24 bytes = 6 floats)
    // State layout per channel: [s0, s1] for low shelf, [s0, s1] for high shelf
    // Channel 0 state: low at +52/+76, high at +100/+124
    // Pointer walks: r11 starts at this+76, increments by 4 each stereo frame

    float* stateBase = (float*)((char*)this + 76);

    // Process 64 stereo sample frames at a time
    int framesRemaining = 64;
    float* samples = info.pSamples;
    int channelIdx = 0;

    while (channelIdx < info.channelCount) {
        for (int frame = 0; frame < framesRemaining; frame++) {
            float x = samples[frame * info.channelCount + channelIdx];
            float y = x;

            // Low shelf biquad (mode 1 or 3)
            if (mode == 1 || mode == 3) {
                float* s_lo = stateBase + channelIdx * 6 - 6;  // state offsets
                float w = b0_lo * x + s_lo[0];
                s_lo[0] = b1_lo * x - a1_lo * w + s_lo[1];
                s_lo[1] = b2_lo * x - a2_lo * w;
                y = w;
            }

            // High shelf biquad (mode 2 or 3)
            if (mode == 2 || mode == 3) {
                float* s_hi = stateBase + channelIdx * 6 + 0;  // state offsets
                float w = b0_hi * y + s_hi[0];
                s_hi[0] = b1_hi * y - a1_hi * w + s_hi[1];
                s_hi[1] = b2_hi * y - a2_hi * w;
                y = w;
            }

            // Apply output gain
            samples[frame * info.channelCount + channelIdx] = outputGain * y;
        }

        channelIdx++;
    }
}

