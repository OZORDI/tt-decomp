/**
 * pong_audio.hpp — Audio effect and manager class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 8 classes — cleaned from auto-generated skeleton
 * Sources: rtti_vtable_map | vtable_layout_map | offset_cluster_map
 *          | debug_string_field_map | rtti_class_hierarchy
 *
 * Audio effect classes (CBiquadFilterEffect, CCompEffect, CDelayEffect,
 * CPeakMeterEffect, CShelvingFilterEffect) implement Xbox 360 XAudio2's
 * IXAPO + IXAPOParameters COM interfaces. The observed vtable is the
 * combined effect-object layout:
 *   [0] Destructor
 *   [1] ScalarDtor
 *   [2] GetRegistrationProperties      (IXAPO)
 *   [3] IsInputFormatSupported / GetParameter(index, out)   (hot path)
 *   [4] LockForProcess / SetParameter(index, value)
 *   [5] UnlockForProcess
 *   [6] Process
 *   [7] CalcInputFrames / CalcOutputFrames
 *
 * The vfn_3/vfn_4 slots are re-used by this codebase as parameter
 * accessors (by-index getter/setter over a runtime critical section),
 * which is consistent with how Rockstar's in-house XAPO wrapper routed
 * its dispatch table on the 360.
 *
 * pongAudioManager inherits from rage::audControlMgr. Its vtable slots:
 *   [0] Destructor
 *   [3] EnableAll  (iterates linked list, enables each controller)
 *   [4] DisableAll (iterates linked list, disables each controller)
 */
#pragma once
#include <stdint.h>
#include "../../rage/audio/rage_audio.hpp"

namespace pong_audio {

// XAPO runtime lock identifiers used by every effect's parameter I/O.
// Packed as <class(hi16):slot(lo16)>:
//   kXapoLockParamEnter = (1<<16) | 1  — enter param critical section
//   kXapoLockParamLeave = (1<<16) | 3  — leave param critical section
constexpr uint32_t kXapoLockParamEnter = (1u << 16) | 1u;
constexpr uint32_t kXapoLockParamLeave = (1u << 16) | 3u;

// HRESULT codes returned from parameter accessors.
constexpr int32_t kHrOk        = 0;
constexpr int32_t kHrInvalidArg = static_cast<int32_t>(0x80070057); // E_INVALIDARG

} // namespace pong_audio

// ── CBiquadFilterEffect  [vtable @ 0x8203B598] ─────────────────────
// IXAPO implementation — biquad (parametric EQ) audio filter.
//
// Parameter indices (for GetParameter/SetParameter, slots vfn_3/vfn_4):
//   0: m_sampleRate (int32 stored as float-converted)
//   1: m_centerFreq
//   2: m_gainDb   (setter applies pow10 scaling)
//   3: m_bandwidth
//   4: m_qFactor  (setter applies pow10 scaling)
struct CBiquadFilterEffect {
    void**      vtable;           // +0x00
    int32_t     m_refCount;       // +0x04

    uint8_t     _pad0x08[120];    // +0x08..+0x7f — biquad coefficients & state

    int32_t     m_sampleRate;     // +0x80  — param 0
    float       m_centerFreq;     // +0x84  — param 1
    float       m_gainDb;         // +0x88  — param 2 (pre-pow10)
    float       m_bandwidth;      // +0x8c  — param 3
    float       m_qFactor;        // +0x90  — param 4 (pre-pow10)

    // ── IXAPO virtual methods ──
    virtual int  GetParameter(uint8_t paramIndex, void* outValue);   // [3] @ 0x821b22f0
    virtual int  SetParameter(uint8_t paramIndex, const void* pValue); // [4] @ 0x821b2408
    virtual void Process();                  // [6] @ 0x821b1fa0
    virtual void CalcInputFrames();          // [7] @ 0x821b1f40
};

// ── CCompEffect  [vtable @ 0x8203B54C] ──────────────────────────
// IXAPO implementation — compressor/limiter effect.
struct CCompEffect {
    void**      vtable;           // +0x00
    int32_t     m_refCount;       // +0x04

    // ── IXAPO virtual methods ──
    virtual int  GetParameter(uint8_t paramIndex, void* outValue);   // [3] @ 0x821b1260
    virtual int  SetParameter(uint8_t paramIndex, const void* pValue); // [4] @ 0x821b13a0
    virtual void Process();                  // [6] @ 0x821b0fd8
};

// ── CDelayEffect  [vtable @ 0x8203B570] ──────────────────────────
// IXAPO implementation — delay/echo audio effect
struct CDelayEffect {
    void**      vtable;           // +0x00
    int32_t     m_refCount;       // +0x04
    uint8_t     _pad0x08[20];
    void*       m_pEffectData1;   // +0x1C
    void*       m_pEffectData2;   // +0x20

    virtual void ScalarDtor(int flags);              // [1] @ 0x821b1e30
    virtual void GetRegistrationProperties();        // [2] @ 0x821b1a30
    virtual void GetParameter();                      // [3] @ 0x821b1a50
    virtual void SetParameter();                      // [4] @ 0x821b1b28
    virtual void Process();                           // [6] @ 0x821b1868
    virtual void CalcInputFrames();                   // [7] @ 0x821b1808
};

// ── CPeakMeterEffect  [vtable @ 0x8203B260] ─────────────────────
// IXAPO implementation — peak level meter for audio monitoring.
//
// The peak meter layout has been reconstructed from the Process and
// GetParameter scaffolds (see @ 0x821AD788, @ 0x821AD9F8):
//   +0x04  m_refCount
//   +0x0C  m_peakSnapshotFlags (written by UnlockForProcess/vfn_5)
//   +0x10  m_channelCount
//   +0x14  m_frameCount
//   +0x18  m_ioBufferA  (base of current input peak values)
//   +0x1C  m_ioBufferB  (also pulled as output)
//   +0x20  m_ioBufferC  (also pulled as output)
//   +0x24  m_ioBufferD  (also pulled as output)
//
// The peak output record is 40 bytes long (peak L at +0x00, peak R at +0x20).
struct CPeakMeterEffect {
    void**      vtable;           // +0x00
    int32_t     m_refCount;       // +0x04
    uint8_t     _pad0x08[4];
    uint32_t    m_peakSnapshotFlags; // +0x0C — surfaced via vfn_5
    uint32_t    m_channelCount;   // +0x10
    int32_t     m_frameCount;     // +0x14
    float*      m_pInputBufA;     // +0x18
    float*      m_pInputBufB;     // +0x1C
    float*      m_pInputBufC;     // +0x20
    float*      m_pInputBufD;     // +0x24

    virtual ~CPeakMeterEffect();                     // [0] @ 0x821add88
    virtual void ScalarDtor(int flags);              // [1] @ 0x821adda0
    virtual void GetRegistrationProperties(void* outProps); // [2] @ 0x821ad9d8
    virtual int  GetPeakValues(uint8_t channelMask, void* outPeaks);  // [3] @ 0x821ad9f8
    virtual int  LockForProcess(uint8_t mode);       // [4] @ 0x821adc40
    virtual int  UnlockForProcess(void* outFlags);   // [5] @ 0x82468980
    virtual void Process(void* pInputBuffer);        // [6] @ 0x821ad788
    virtual void CalcInputFrames();                   // [7] @ 0x821ad6b0
};

// ── CShelvingFilterEffect  [vtable @ 0x8203B288] ────────────────
// IXAPO implementation — shelving EQ filter (low/high shelf).
//
// Parameter indices (for GetParameter/SetParameter, slots vfn_3/vfn_4):
//   0: m_sampleRate     +0x94
//   1: m_frequency      +0x98 (setter computes m_sinFreq at +0x9C)
//   2: m_lowGain        +0xA0
//   3: m_highGain       +0xA4
//   4: m_lowQ           +0xA8
//   5: m_highQ          +0xAC
//   6: m_mode           +0xB0
struct CShelvingFilterEffect {
    void**      vtable;           // +0x00
    int32_t     m_refCount;       // +0x04
    uint8_t     _pad0x08[140];    // +0x08..+0x93 — biquad coeffs & per-channel state

    int32_t     m_sampleRate;     // +0x94
    float       m_frequency;      // +0x98
    float       m_sinFreq;        // +0x9C — precomputed sin(2πf/fs)*0.5, written by setter param 1
    float       m_lowGain;        // +0xA0
    float       m_highGain;       // +0xA4
    float       m_lowQ;           // +0xA8
    float       m_highQ;          // +0xAC
    int32_t     m_mode;           // +0xB0 — 1=low, 2=high, 3=both

    virtual ~CShelvingFilterEffect();                // [0] @ 0x821b26a8
    virtual void ScalarDtor(int flags);              // [1] @ 0x821ae618
    virtual void GetRegistrationProperties(void* outProps); // [2] @ 0x821ae198
    virtual int  GetParameter(uint8_t paramIndex, void* outValue);   // [3] @ 0x821ae1b8
    virtual int  SetParameter(uint8_t paramIndex, const void* pValue); // [4] @ 0x821ae2c8
    virtual int  UnlockForProcess(void* outParam);   // [5] @ 0x821ae4e0
    virtual void Process(void* pInputBuffer);        // [6] @ 0x821ade28
    virtual void CalcInputFrames();                   // [7] @ 0x821addc8
};

// ── audMsgSink  [vtable @ 0x8203C750] ───────────────────────────
struct audMsgSink {
    void**      vtable;           // +0x00
    virtual void ScalarDtor(int flags); // [1] @ 0x821bea98
};

// ── pongAmbiencePlayer  [vtable @ 0x8203C36C] ──────────────────
struct pongAmbiencePlayer {
    void**      vtable;           // +0x00
};

// ── pongAudioManager  [vtable @ 0x8203BB6C] ────────────────────
struct pongAudioManager : public rage::audControlMgr {
    virtual ~pongAudioManager();                     // [0] @ 0x821b3ec8
    virtual void EnableAll();                         // [3] @ 0x821b5c78
    virtual void DisableAll();                        // [4] @ 0x821b5c88
};
