/**
 * pong_audio.hpp — Audio effect and manager class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 8 classes — cleaned from auto-generated skeleton
 * Sources: rtti_vtable_map | vtable_layout_map | offset_cluster_map
 *          | debug_string_field_map | rtti_class_hierarchy
 *
 * Audio effect classes (CBiquadFilterEffect, CCompEffect, CDelayEffect,
 * CPeakMeterEffect, CShelvingFilterEffect) implement the IXAPO COM interface
 * from Xbox 360 XAudio2. The vtable layout follows the standard IXAPO slots:
 *   [0] Destructor
 *   [1] ScalarDtor
 *   [2] GetRegistrationProperties
 *   [3] IsInputFormatSupported
 *   [4] LockForProcess
 *   [5] UnlockForProcess
 *   [6] Process
 *   [7] CalcInputFrames / CalcOutputFrames
 *
 * pongAudioManager inherits from rage::audControlMgr. Its vtable slots:
 *   [0] Destructor
 *   [3] EnableAll  (iterates linked list, enables each controller)
 *   [4] DisableAll (iterates linked list, disables each controller)
 */
#pragma once
#include <stdint.h>

// ── CBiquadFilterEffect  [vtable @ 0x8203B598] ─────────────────────
// IXAPO implementation — biquad (parametric EQ) audio filter
struct CBiquadFilterEffect {
    void**      vtable;           // +0x00

    // ── IXAPO virtual methods ──
    virtual void IsInputFormatSupported();   // [3] @ 0x821b22f0
    virtual void LockForProcess();           // [4] @ 0x821b2408
    virtual void Process();                  // [6] @ 0x821b1fa0
    virtual void CalcInputFrames();          // [7] @ 0x821b1f40
};

// ── CCompEffect  [vtable @ 0x8203B54C] ──────────────────────────
// IXAPO implementation — compressor/limiter effect
struct CCompEffect {
    void**      vtable;           // +0x00

    // ── IXAPO virtual methods ──
    virtual void IsInputFormatSupported();   // [3] @ 0x821b1260
    virtual void LockForProcess();           // [4] @ 0x821b13a0
    virtual void Process();                  // [6] @ 0x821b0fd8
};

// ── CDelayEffect  [vtable @ 0x8203B570] ──────────────────────────
// IXAPO implementation — delay/echo audio effect
struct CDelayEffect {
    void**      vtable;           // +0x00

    // ── IXAPO virtual methods ──
    virtual void ScalarDtor(int flags);              // [1] @ 0x821b1e30
    virtual void GetRegistrationProperties();        // [2] @ 0x821b1a30
    virtual void IsInputFormatSupported();            // [3] @ 0x821b1a50
    virtual void LockForProcess();                    // [4] @ 0x821b1b28
    virtual void Process();                           // [6] @ 0x821b1868
    virtual void CalcInputFrames();                   // [7] @ 0x821b1808
};

// ── CPeakMeterEffect  [vtable @ 0x8203B260] ─────────────────────
// IXAPO implementation — peak level meter for audio monitoring
struct CPeakMeterEffect {
    void**      vtable;           // +0x00

    // ── IXAPO virtual methods ──
    virtual ~CPeakMeterEffect();                     // [0] @ 0x821add88
    virtual void ScalarDtor(int flags);              // [1] @ 0x821adda0
    virtual void GetRegistrationProperties();        // [2] @ 0x821ad9d8
    virtual void IsInputFormatSupported();            // [3] @ 0x821ad9f8
    virtual void LockForProcess();                    // [4] @ 0x821adc40
    virtual void UnlockForProcess();                  // [5] @ 0x82468980
    virtual void Process();                           // [6] @ 0x821ad788
    virtual void CalcInputFrames();                   // [7] @ 0x821ad6b0
};

// ── CShelvingFilterEffect  [vtable @ 0x8203B288] ────────────────
// IXAPO implementation — shelving EQ filter (low/high shelf)
struct CShelvingFilterEffect {
    void**      vtable;           // +0x00

    // ── IXAPO virtual methods ──
    virtual ~CShelvingFilterEffect();                // [0] @ 0x821b26a8
    virtual void ScalarDtor(int flags);              // [1] @ 0x821ae618
    virtual void GetRegistrationProperties();        // [2] @ 0x821ae198
    virtual void IsInputFormatSupported();            // [3] @ 0x821ae1b8
    virtual void LockForProcess();                    // [4] @ 0x821ae2c8
    virtual void UnlockForProcess();                  // [5] @ 0x821ae4e0
    virtual void Process();                           // [6] @ 0x821ade28
    virtual void CalcInputFrames();                   // [7] @ 0x821addc8
};

// ── audMsgSink  [vtable @ 0x8203C750] ───────────────────────────
// Message sink for the RAGE audio system message bus
struct audMsgSink {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void ScalarDtor(int flags); // [1] @ 0x821bea98
};

// ── pongAmbiencePlayer  [vtable @ 0x8203C36C] ──────────────────
// Manages ambient/environmental audio playback during matches
struct pongAmbiencePlayer {
    void**      vtable;           // +0x00
};

// ── pongAudioManager  [vtable @ 0x8203BB6C] ────────────────────
// Inherits from rage::audControlMgr
// Central audio manager — coordinates all game audio controllers
struct pongAudioManager {
    void**      vtable;           // +0x00

    // ── rage::audControlMgr virtual methods ──
    virtual ~pongAudioManager();                     // [0] @ 0x821b3ec8
    virtual void EnableAll();                         // [3] @ 0x821b5c78 — sets +76 flag, delegates to audControlMgr::EnableAll
    virtual void DisableAll();                        // [4] @ 0x821b5c88 — clears +76 flag, delegates to audControlMgr::DisableAll
};
