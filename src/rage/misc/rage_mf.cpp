/**
 * rage_mf.cpp — RAGE Motion Framework Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * rage::mfNativeAnimationController — the native implementation of the
 * motion-framework animation controller.  Wraps a playback state object
 * (at field +44) that contains:
 *   +36  totalFrames   (int32)  — total frame count in the clip
 *   +40  startFrame    (int32)  — start frame offset
 *   +44  currentFrame  (int32)  — current frame index
 *   +200 animFramePtr  — pointer to the evaluated crAnimFrame data
 *   +216 accumulatedTime (float) — accumulated playback time
 *
 * 5 vtable methods implemented:
 *   [0] ~mfNativeAnimationController  — destructor
 *   [2] ResetPlayback                 — resets time and evaluates frame
 *   [3] Tick                          — advances time, handles clip wrapping
 *   [4] GetPlaybackProgress           — returns normalized [0..1] progress
 *   [5] GetAnimFrame                  — returns pointer to anim frame data
 */

#include "rage_mf.hpp"
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// RAGE heap deallocator.
// @ 0x820C00C0
extern "C" void rage_free(void* ptr);

// Playback position evaluator — sets the playback time on the playback
// object, then evaluates the current frame.  Calls LocomotionStateAnim
// for the actual frame computation.
// @ 0x822E0458 | size: 0x178
extern void mfPlayback_SetTimeAndEvaluate(void* playbackObj, float time);

namespace rage {

// ═══════════════════════════════════════════════════════════════════════════════
//  mfNativeAnimationController
//
//  RTTI name: rage::mfNativeAnimationController  [vtable @ 0x8205A08C]
//  Inherits: rage::mfAnimationController         [vtable @ 0x8205A054]
//
//  Layout:
//    +0x00  vtable pointer
//    +0x2C  m_pPlayback (void*)  — pointer to the playback state object
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * ~mfNativeAnimationController  [vtable slot 0]
 * @ 0x822DF688 | size: 0x5C
 *
 * Deleting destructor.  Sets vtable to 0x8205A08C, calls parent
 * mfAnimationController destructor, conditionally frees memory.
 */
mfNativeAnimationController::~mfNativeAnimationController()
{
    // Parent class destructor called automatically by C++
}

/**
 * ResetPlayback  [vtable slot 2]
 * @ 0x822E0D60 | size: 0x64
 *
 * Resets the animation playback to the beginning.  Loads a float constant
 * (initial time value) and calls the playback evaluator, then retrieves
 * the anim frame and passes it to the scalar-dtor-style cleanup (vfn_1).
 *
 * Flow:
 *   1. Load playback object from this+44
 *   2. Call mfPlayback_SetTimeAndEvaluate(playback, initialTime)
 *   3. Call this->GetAnimFrame() [vfn_5]
 *   4. Call this->ScalarDtor(animFrame) [vfn_1] — updates animation state
 */
void mfNativeAnimationController::ResetPlayback()
{
    void* playback = *reinterpret_cast<void**>(
        reinterpret_cast<char*>(this) + 44);

    // Reset with initial time constant
    // The assembly loads from a .rdata float constant table
    float initialTime = 0.0f;
    mfPlayback_SetTimeAndEvaluate(playback, initialTime);

    // Get the evaluated anim frame and update state
    void* animFrame = this->GetAnimFrame();
    this->ScalarDtor(reinterpret_cast<intptr_t>(animFrame));
}

/**
 * Tick  [vtable slot 3]
 * @ 0x822E0DC8 | size: 0x1E4
 *
 * Advances the animation by deltaTime (passed in f1).
 *
 * Algorithm:
 *   1. Load playback object from this+44
 *   2. Read playback fields: totalFrames(+36), currentFrame(+44),
 *      startFrame(+40), accumulatedTime(+216)
 *   3. Compute clip duration: (float)currentFrame/(float)totalFrames
 *                            - (float)startFrame/(float)totalFrames
 *   4. accumTime += deltaTime
 *   5. If accumTime > duration:
 *      a. Loop: while accumTime > per-frame-duration, subtract it
 *      b. This handles clip wrapping for looping animations
 *   6. Else: accumTime = accumulatedTime + deltaTime
 *   7. Call mfPlayback_SetTimeAndEvaluate(playback, finalTime)
 *   8. Call this->GetAnimFrame() → this->ScalarDtor(animFrame)
 */
void mfNativeAnimationController::Tick(float deltaTime)
{
    void* playback = *reinterpret_cast<void**>(
        reinterpret_cast<char*>(this) + 44);

    int32_t totalFrames  = *reinterpret_cast<int32_t*>(
        static_cast<char*>(playback) + 36);
    int32_t currentFrame = *reinterpret_cast<int32_t*>(
        static_cast<char*>(playback) + 44);
    int32_t startFrame   = *reinterpret_cast<int32_t*>(
        static_cast<char*>(playback) + 40);

    float fTotalFrames  = static_cast<float>(totalFrames);
    float fCurrentFrame = static_cast<float>(currentFrame);
    float fStartFrame   = static_cast<float>(startFrame);

    // Compute clip duration as normalized range
    float clipDuration = fCurrentFrame / fTotalFrames
                       - fStartFrame / fTotalFrames;

    // Read accumulated time from playback+216
    float accumTime = *reinterpret_cast<float*>(
        static_cast<char*>(playback) + 216);
    accumTime += deltaTime;

    if (accumTime > clipDuration) {
        // Recompute per-frame duration using the playback's own fields
        // (playback+36, +44, +40 — which may differ if clip was re-bound)
        int32_t pbTotalFrames  = *reinterpret_cast<int32_t*>(
            static_cast<char*>(playback) + 36);
        int32_t pbCurrentFrame = *reinterpret_cast<int32_t*>(
            static_cast<char*>(playback) + 44);
        int32_t pbStartFrame   = *reinterpret_cast<int32_t*>(
            static_cast<char*>(playback) + 40);

        float pbDuration = static_cast<float>(pbCurrentFrame) /
                           static_cast<float>(pbTotalFrames)
                         - static_cast<float>(pbStartFrame) /
                           static_cast<float>(pbTotalFrames);

        if (accumTime > pbDuration) {
            // Wrap: keep subtracting clip duration until within range
            float wrapDuration;
            do {
                int32_t wTotalFrames  = *reinterpret_cast<int32_t*>(
                    static_cast<char*>(playback) + 36);
                int32_t wCurrentFrame = *reinterpret_cast<int32_t*>(
                    static_cast<char*>(playback) + 44);
                int32_t wStartFrame   = *reinterpret_cast<int32_t*>(
                    static_cast<char*>(playback) + 40);

                wrapDuration = static_cast<float>(wCurrentFrame) /
                               static_cast<float>(wTotalFrames)
                             - static_cast<float>(wStartFrame) /
                               static_cast<float>(wTotalFrames);

                accumTime -= wrapDuration;
            } while (accumTime > wrapDuration);
        }
    } else {
        // Simple case: just accumulate
        float prevTime = *reinterpret_cast<float*>(
            static_cast<char*>(playback) + 216);
        accumTime = prevTime + deltaTime;
    }

    // Evaluate at the new time position
    mfPlayback_SetTimeAndEvaluate(playback, accumTime);

    // Get the evaluated frame and update state
    void* animFrame = this->GetAnimFrame();
    this->ScalarDtor(reinterpret_cast<intptr_t>(animFrame));
}

/**
 * GetPlaybackProgress  [vtable slot 4]
 * @ 0x822E0FB0 | size: 0x5C
 *
 * Returns the normalized playback progress as a float in [0..1].
 *
 * Computes:
 *   progress = (currentFrame / totalFrames) - (startFrame / totalFrames)
 *
 * This gives the fraction of the clip that has been played, normalized
 * to the clip's own frame range.
 */
float mfNativeAnimationController::GetPlaybackProgress()
{
    void* playback = *reinterpret_cast<void**>(
        reinterpret_cast<char*>(this) + 44);

    int32_t totalFrames  = *reinterpret_cast<int32_t*>(
        static_cast<char*>(playback) + 36);
    int32_t currentFrame = *reinterpret_cast<int32_t*>(
        static_cast<char*>(playback) + 44);
    int32_t startFrame   = *reinterpret_cast<int32_t*>(
        static_cast<char*>(playback) + 40);

    float fTotal   = static_cast<float>(totalFrames);
    float fCurrent = static_cast<float>(currentFrame);
    float fStart   = static_cast<float>(startFrame);

    return (fCurrent / fTotal) - (fStart / fTotal);
}

/**
 * GetAnimFrame  [vtable slot 5]
 * @ 0x822E1010 | size: 0x0C
 *
 * Returns a pointer to the animation frame data within the playback
 * object.  The frame data lives at offset +200 from the playback object.
 */
void* mfNativeAnimationController::GetAnimFrame()
{
    void* playback = *reinterpret_cast<void**>(
        reinterpret_cast<char*>(this) + 44);
    return static_cast<char*>(playback) + 200;
}

} // namespace rage
