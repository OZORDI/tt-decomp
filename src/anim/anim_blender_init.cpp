/**
 * anim_blender_init.cpp — Animation Blender State Initialization & Helpers
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Three functions that manage the crAnimBlenderState sub-structure:
 *
 *   crAnimBlenderState_Init         — Resets the blender sub-object to defaults
 *   pcrAnimBlender_ApplyClipWeights — Looks up a clip by name hash and pushes
 *                                     blend weights to all matching blenders
 *   pcrAnimState_ComputePosition    — Extracts the world-space displacement
 *                                     from the locomotion clip, with mirror
 *
 * The crAnimBlenderState lives inline at offset +0x10 inside both
 * pcrEmoteBlender and pcrPostPointBlender.  It tracks two callback
 * pairs (debug trace + indirect dispatcher), three clip slots, blend
 * weights, and animation phase timers.
 */

#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// Debug trace stub — compiled to a single `blr` in release builds.
// @ 0x8240E6D0 | size: 0x4
extern void rage_debugLog(const void*, const void*, uint32_t);

// Indirect call dispatcher: loads vtable[0] from the object and calls it.
// Used as the "invoke" half of each callback pair in the blender state.
// @ 0x821E7BC0 | size: 0xC
extern void rage_IndirectCallDispatch(void* obj);

// PJW-style string hash used by RAGE's atHashMap.
// Iterates through each byte, accumulating a 32-bit hash with overflow folding.
// @ 0x8223A818 | size: 0x50
extern uint32_t rage_atStringHash(const char* str);

// Applies blend weights to a single blender slot in the clip's blender array.
// Returns non-zero (low byte) if the blender absorbed the weights and is done.
// @ 0x820DE6F8 | size: 0xE0
extern uint8_t pcrBlender_ApplyWeight(void* blenderEntry, void* animSystem,
                                       float weightA, float weightB);

// Extracts the world-space displacement vector from a locomotion clip
// at the given phase values.  Result is written to outDisplacement (vec3).
// @ 0x82248C58 | size: 0x158
extern void LocomotionStateAnim_ExtractDisplacement(void* locomotionClip,
                                                     float phaseAbsolute,
                                                     float phaseDelta,
                                                     float* outDisplacement);

// ─────────────────────────────────────────────────────────────────────────────
// Global constants & data
// ─────────────────────────────────────────────────────────────────────────────

// Default initial position/quaternion vector — 16 bytes, identity reset value.
// @ 0x8261A0C0 (.data, 16 bytes)
extern const float g_identityBlendVector[4];

// Float constants used throughout the animation blender system.
// kZeroPhase    @ 0x8202D108 (.rdata, 4 bytes) — 0.0f (zero phase / origin)
// kDefaultBlend @ 0x8202D110 (.rdata, 4 bytes) — 1.0f (full weight / default)
//
// In the scaffold, the Init function reads [+0] = kZeroPhase, [+8] = kDefaultBlend.
// The [+4] slot is unused padding between two 4-byte .rdata symbols.
extern const float kZeroPhase;        // @ 0x8202D108
extern const float kDefaultBlend;     // @ 0x8202D110

// Player input system global pointer.
// Used to resolve the player side offset for mirroring.
// @ 0x825EAB28 (.data, 4 bytes) — named SDA global
extern void* g_input_obj_ptr;

// Player-side mirror plane — 16-byte aligned vec4 in SDA region.
// When the player is on the mirrored side, positions are reflected
// across this plane: out[i] = mirrorPlane[i] - position[i]
// @ 0x82606750 (.data, 32 bytes) — SDA offset 26448
extern const float g_playerMirrorPlane[4];

// ─────────────────────────────────────────────────────────────────────────────
// Clip index sentinel
// ─────────────────────────────────────────────────────────────────────────────

static const uint16_t CLIP_INDEX_NONE = 0xFFFF;

// ─────────────────────────────────────────────────────────────────────────────
// crAnimBlenderState — Animation blender sub-structure
//
// Embedded at offset +0x10 inside pcrEmoteBlender and pcrPostPointBlender.
// Total sub-struct size: 160 bytes (padded from offset +16 through +175).
// The Init function writes fields at the following offsets from the
// sub-struct base (which itself starts at +0x10 from the owning object):
//
//   Offset shown below is from the owning pcrEmoteBlender/pcrPostPointBlender,
//   which is what the scaffold uses (r3 = owning object pointer at entry,
//   then r3 += 16 via `addi r3,r3,16` in the caller).
//
//   +0x00..+0x0F  (16 bytes) : m_blendVector    — initial blend position (vec4)
//   +0x10..+0x1F  (16 bytes) : m_callbacksA     — callback pair A (4 void*)
//   +0x20..+0x2F  (16 bytes) : m_callbacksB     — callback pair B (4 void*)
//   +0x30         ( 4 bytes) : m_frameCount      — zeroed on reset
//   +0x34         ( 4 bytes) : m_updateCount     — zeroed on reset
//   +0x38         ( 2 bytes) : m_blendFlags      — zeroed on reset
//   +0x3A         ( 2 bytes) : m_blendMode       — zeroed on reset
//   +0x40         ( 4 bytes) : m_blendWeightA    — set to kDefaultBlend
//   +0x44         ( 4 bytes) : m_blendWeightB    — set to kDefaultBlend
//   +0x58..+0x6F  (gap)      : intermediate state (not written by Init)
//   +0x68         ( 4 bytes) : m_phaseA          — set to kZeroPhase
//   +0x6C         ( 4 bytes) : m_targetPhaseA    — set to kDefaultBlend
//   +0x70         ( 4 bytes) : m_phaseB          — set to kZeroPhase
//   +0x74         ( 4 bytes) : m_targetPhaseB    — set to kDefaultBlend
//   +0x78         ( 4 bytes) : m_phaseC          — set to kZeroPhase
//   +0x7C         ( 2 bytes) : m_clipFlags       — zeroed
//   +0x7E         ( 2 bytes) : m_clipSlotA       — CLIP_INDEX_NONE
//   +0x80         ( 2 bytes) : m_clipSlotB       — CLIP_INDEX_NONE
//   +0x82         ( 2 bytes) : m_clipSlotC       — CLIP_INDEX_NONE
//   +0x88         ( 4 bytes) : m_finalBlendA     — set to kDefaultBlend
//   +0x8C         ( 4 bytes) : m_finalBlendB     — set to kDefaultBlend
// ─────────────────────────────────────────────────────────────────────────────

struct crAnimBlenderState {
    // +0x00: Blend position vector (vec4) — copied from g_identityBlendVector
    float        m_blendVector[4];

    // +0x10: Callback pair A — [trace, dispatcher, context, dispatcher]
    void*        m_callbacksA[4];

    // +0x20: Callback pair B — same layout as pair A
    void*        m_callbacksB[4];

    // +0x30: State counters
    uint32_t     m_frameCount;
    uint32_t     m_updateCount;
    uint16_t     m_blendFlags;
    uint16_t     m_blendMode;

    // +0x3C: padding (4 bytes to align to +0x40)
    uint8_t      _pad3C[4];

    // +0x40: Blend weights
    float        m_blendWeightA;
    float        m_blendWeightB;

    // +0x48..+0x67: Intermediate fields (24 bytes, not touched by Init)
    uint8_t      _pad48[32];

    // +0x68: Phase timers (alternating phase / target pairs)
    float        m_phaseA;
    float        m_targetPhaseA;
    float        m_phaseB;
    float        m_targetPhaseB;
    float        m_phaseC;

    // +0x7C: Clip slot control
    uint16_t     m_clipFlags;
    uint16_t     m_clipSlotA;        // active clip index A, CLIP_INDEX_NONE = none
    uint16_t     m_clipSlotB;        // active clip index B, CLIP_INDEX_NONE = none
    uint16_t     m_clipSlotC;        // active clip index C, CLIP_INDEX_NONE = none

    // +0x84: padding (4 bytes)
    uint8_t      _pad84[4];

    // +0x88: Final computed blend weights
    float        m_finalBlendA;
    float        m_finalBlendB;
};


/**
 * crAnimBlenderState_Init @ 0x8224C810 | size: 0x120
 *
 * Resets an animation blender sub-structure to its default state.
 * Called by pcrEmoteBlender::Reset() and pcrPostPointBlender::ResetState()
 * to clear all animation state when a blender is recycled.
 *
 * Sets:
 *   - Blend position vector to the identity from g_identityBlendVector
 *   - Both callback pairs to [trace_stub, dispatcher, nullptr, dispatcher]
 *   - All counters and flags to zero
 *   - Blend weights to kDefaultBlend (1.0f)
 *   - Phase timers alternating kZeroPhase / kDefaultBlend
 *   - All three clip slots to CLIP_INDEX_NONE (0xFFFF)
 *   - Final blend weights to kDefaultBlend
 *
 * Callers:
 *   pcrEmoteBlender_vfn_3 (Reset), pcrPostPointBlender_vfn_3 (ResetState),
 *   LocomotionStateAnim_BBC8_g, pongPlayer_D908_g, game_E2B8, util_CDF0
 */
void crAnimBlenderState_Init(crAnimBlenderState* state)
{
    // Copy identity blend vector from global constant data
    memcpy(state->m_blendVector, g_identityBlendVector, 16);

    // Install callback pairs — each pair is:
    //   [0] = debug trace stub (no-op in release)
    //   [1] = indirect call dispatcher (calls vtable[0] on the target)
    //   [2] = nullptr (context / target — not set until activation)
    //   [3] = indirect call dispatcher (backup/secondary)
    state->m_callbacksA[0] = (void*)rage_debugLog;
    state->m_callbacksA[1] = (void*)rage_IndirectCallDispatch;
    state->m_callbacksA[2] = nullptr;
    state->m_callbacksA[3] = (void*)rage_IndirectCallDispatch;

    state->m_callbacksB[0] = (void*)rage_debugLog;
    state->m_callbacksB[1] = (void*)rage_IndirectCallDispatch;
    state->m_callbacksB[2] = nullptr;
    state->m_callbacksB[3] = (void*)rage_IndirectCallDispatch;

    // Clear state counters and flags
    state->m_frameCount  = 0;
    state->m_updateCount = 0;
    state->m_blendFlags  = 0;
    state->m_blendMode   = 0;

    // Initialize blend weights to full (1.0f)
    state->m_blendWeightA = kDefaultBlend;
    state->m_blendWeightB = kDefaultBlend;

    // Initialize phase timers: phase = 0.0f, target = 1.0f
    state->m_phaseA       = kZeroPhase;
    state->m_targetPhaseA = kDefaultBlend;
    state->m_phaseB       = kZeroPhase;
    state->m_targetPhaseB = kDefaultBlend;
    state->m_phaseC       = kZeroPhase;

    // Reset all clip slots to inactive
    state->m_clipFlags = 0;
    state->m_clipSlotA = CLIP_INDEX_NONE;
    state->m_clipSlotB = CLIP_INDEX_NONE;
    state->m_clipSlotC = CLIP_INDEX_NONE;

    // Initialize final blend weights to full
    state->m_finalBlendA = kDefaultBlend;
    state->m_finalBlendB = kDefaultBlend;
}


// ─────────────────────────────────────────────────────────────────────────────
// Animation clip hash table structures
//
// The player's animation system stores clips in a hash map keyed by
// clip name strings.  Each bucket is a singly-linked list of entries.
// ─────────────────────────────────────────────────────────────────────────────

struct crClipHashEntry {
    const char*       m_pKeyName;       // +0x00: clip name string (key)
    void*             m_pBlenderList;   // +0x04: pointer to crClipBlenderList
    crClipHashEntry*  m_pNext;          // +0x08: next entry in hash chain
};

struct crClipBlenderList {
    uint8_t      _pad00[16];
    uint8_t      m_bStopOnComplete;     // +0x10: if set, stop after first successful apply
    uint8_t      _pad11[7];
    void**       m_pBlenderArray;       // +0x18 (+24): array of blender object pointers
    uint16_t     m_blenderCount;        // +0x1C (+28): number of blender entries
};

struct crClipDictEntry {
    void*        m_pClipData;           // +0x00
    void*        m_pNameString;         // +0x04
    crClipHashEntry** m_pHashTable;     // +0x08: hash bucket array
    uint16_t     m_hashTableSize;       // +0x0C: number of hash buckets
};

struct crAnimSystem {
    uint8_t      _pad00[12];
    crClipDictEntry** m_pClipDicts;     // +0x0C (+12): array of clip dictionary pointers
    uint16_t     m_clipDictCount;       // +0x10 (+16): number of clip dictionaries
};


/**
 * pcrAnimBlender_ApplyClipWeights @ 0x820CF420 | size: 0x178
 *
 * Looks up an animation clip by name string across all clip dictionaries
 * in the player's animation system, then applies the specified blend
 * weights to every blender attached to the matching clip.
 *
 * The lookup uses a hash table per dictionary:
 *   1. Compute rage_atStringHash(clipName) % bucketCount
 *   2. Walk the hash chain doing byte-by-byte strcmp
 *   3. On match, iterate the blender list calling pcrBlender_ApplyWeight
 *   4. If any blender returns non-zero AND the blender list has the
 *      "stop on complete" flag, return immediately
 *
 * Parameters:
 *   animSystem   — pointer to the player's crAnimSystem (at pPlayer base)
 *   clipName     — null-terminated clip name string to match
 *   blendWeightA — primary blend weight (typically current phase weight)
 *   blendWeightB — secondary blend weight (typically target phase weight)
 *
 * Callers:
 *   pcrEmoteBlender::Update, pongLocomotionController_vfn_7,
 *   LocomotionStateAnim_CDB8_g, pg_C540_g, game_ADC0
 */
void pcrAnimBlender_ApplyClipWeights(void* animSystem, const char* clipName,
                                      float blendWeightA, float blendWeightB)
{
    crAnimSystem* system = static_cast<crAnimSystem*>(animSystem);

    if (system->m_clipDictCount <= 0) {
        return;
    }

    // Search each clip dictionary for a matching clip name
    for (int dictIdx = 0; dictIdx < system->m_clipDictCount; dictIdx++) {
        crClipDictEntry* dictEntry = system->m_pClipDicts[dictIdx];

        if (dictEntry->m_hashTableSize == 0) {
            continue;
        }

        // Hash the clip name and find the bucket
        uint32_t hash = rage_atStringHash(clipName);
        uint32_t bucketIdx = hash % dictEntry->m_hashTableSize;

        // Walk the hash chain looking for a string match
        crClipHashEntry* hashEntry = dictEntry->m_pHashTable[bucketIdx];

        while (hashEntry != nullptr) {
            // Byte-by-byte string comparison (inlined strcmp in the binary)
            const char* entryKey = hashEntry->m_pKeyName;
            const char* searchKey = clipName;
            bool matched = true;

            while (*searchKey != '\0') {
                if (*entryKey != *searchKey) {
                    matched = false;
                    break;
                }
                entryKey++;
                searchKey++;
            }
            // Final check: both strings must have ended at the same point
            if (matched && *entryKey != *searchKey) {
                matched = false;
            }

            if (matched) {
                // Found matching clip — apply weights to all attached blenders
                crClipBlenderList* blenderList =
                    static_cast<crClipBlenderList*>(hashEntry->m_pBlenderList);

                if (blenderList == nullptr) {
                    return;
                }

                bool anyCompleted = false;

                for (int blenderIdx = 0; blenderIdx < blenderList->m_blenderCount; blenderIdx++) {
                    void* blenderObj = blenderList->m_pBlenderArray[blenderIdx];

                    uint8_t result = pcrBlender_ApplyWeight(
                        blenderObj, animSystem, blendWeightA, blendWeightB);

                    anyCompleted = anyCompleted || (result != 0);

                    // If any blender completed and stop-on-complete is set, bail
                    if (anyCompleted && blenderList->m_bStopOnComplete != 0) {
                        return;
                    }
                }

                // After processing all blenders, check stop-on-complete again
                if (anyCompleted && blenderList->m_bStopOnComplete != 0) {
                    return;
                }

                return;  // Found and processed the clip, done
            }

            // Move to next entry in hash chain
            hashEntry = hashEntry->m_pNext;
        }
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// Animation state structure for position computation
// ─────────────────────────────────────────────────────────────────────────────

struct pcrAnimState {
    uint8_t      _pad00[84];
    void*        m_pLocomotionClip;     // +0x54 (+84):  active locomotion clip
    uint16_t     m_locomotionMode;      // +0x58 (+88):  1 = active locomotion
    uint8_t      _pad5A[6];
    float        m_currentPhase;        // +0x60 (+96):  current animation phase [0..1]
    float        m_targetPhase;         // +0x64 (+100): target animation phase [0..1]
    uint8_t      _pad68[308];
    void*        m_pPlayerSideOffset;   // +0x198 (+408): player side index for mirroring
    float        m_remainingBlendTime;  // +0x19C (+412): remaining blend time (>0 = blending)
};

struct crLocomotionClip {
    uint8_t      _pad00[12];
    float        m_duration;            // +0x0C (+12): total clip duration in seconds
};


/**
 * pcrAnimState_ComputePosition @ 0x820DC938 | size: 0x104
 *
 * Extracts the world-space displacement from the current locomotion
 * animation clip and writes it to the output vec3.  If the player is
 * on the mirrored side, the displacement is reflected across the
 * g_playerMirrorPlane.
 *
 * Early returns with a zeroed vector if:
 *   - The remaining blend time is positive (still blending between animations)
 *   - Locomotion mode is not 1 (not in active locomotion)
 *   - The locomotion clip pointer is null
 *
 * Algorithm:
 *   1. Compute phase delta = currentPhase - targetPhase
 *   2. Scale both by clip duration to get absolute time values
 *   3. Call LocomotionStateAnim_ExtractDisplacement for the position
 *   4. If the player's mirror flag is clear, reflect across mirror plane
 *
 * Caller: pongPlayer_D7B0_g (player position update)
 */
void pcrAnimState_ComputePosition(float* outPosition, pcrAnimState* animState)
{
    // Initialize output to zero
    outPosition[0] = kZeroPhase;
    outPosition[1] = kZeroPhase;
    outPosition[2] = kZeroPhase;

    // If still blending between animations, no displacement to compute
    if (animState->m_remainingBlendTime > kZeroPhase) {
        return;
    }

    // Temporary displacement vector (16-byte aligned for SIMD)
    float displacement[4] = { kZeroPhase, kZeroPhase, kZeroPhase, 0.0f };

    // Only compute displacement when in active locomotion with a valid clip
    if (animState->m_locomotionMode == 1 && animState->m_pLocomotionClip != nullptr) {
        crLocomotionClip* clip =
            static_cast<crLocomotionClip*>(animState->m_pLocomotionClip);

        float phaseDelta = animState->m_currentPhase - animState->m_targetPhase;
        float duration = clip->m_duration;

        float phaseAbsolute = duration * animState->m_currentPhase;
        float phaseRelative = phaseDelta * duration;

        LocomotionStateAnim_ExtractDisplacement(
            animState->m_pLocomotionClip,
            phaseAbsolute,
            phaseRelative,
            displacement);
    }

    // Write computed displacement to output
    outPosition[0] = displacement[0];
    outPosition[1] = displacement[1];
    outPosition[2] = displacement[2];

    // Check player-side mirror flag
    // g_input_obj_ptr is added to the player side offset to find the
    // control block, then byte +64 in that block is the mirror flag.
    uint8_t* playerSideBase = static_cast<uint8_t*>(animState->m_pPlayerSideOffset);
    uint8_t* inputBase = static_cast<uint8_t*>(g_input_obj_ptr);
    uint8_t mirrorFlag = *(playerSideBase + reinterpret_cast<uintptr_t>(inputBase) + 64);

    if (mirrorFlag != 0) {
        return;  // Player is on the non-mirrored side, use position as-is
    }

    // Reflect position across the mirror plane
    outPosition[0] = g_playerMirrorPlane[0] - displacement[0];
    outPosition[1] = g_playerMirrorPlane[1] - displacement[1];
    outPosition[2] = g_playerMirrorPlane[2] - displacement[2];
}
