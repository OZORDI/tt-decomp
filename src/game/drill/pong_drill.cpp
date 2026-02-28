// pong_drill.cpp — Training drill implementations | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * Training Drill Classes Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "pong_drill.hpp"
#include <stdio.h>
#include <math.h>

// External function declarations
extern "C" {
    void ProcessPageGroupInput(int param1, int param2, const char* param3, int param4);
    void NotifyUIEvent(int eventId, int flags, int param3, int param4);  // pg_E6E0 @ 0x8225E6E0
    void SetTrainingState(void* pStateMachine, int nState);             // atSingleton_E9F8_w @ 0x821EE9F8
    void nop_8240E6D0(const char* message);
    void xmlNodeStruct_vfn_2(void* self);  // @ base class PostLoadProperties
}

// External globals
extern void* g_singles_network_client;  // @ 0x82036614
extern void  ComputeNetworkHash(void* networkClient, int numSuccesses);

// Game state globals
extern void*      g_pGameState;              // @ SDA+25648 (0x82606430) — player/game state manager
extern void*      g_pTrainingStateMachine;   // @ 0x8271A34C — training HSM singleton
extern void*      g_pTrainingData;           // @ 0x8271A31C — training drill data manager
extern uint32_t*  g_pDrillSaveData;          // @ 0x8271A35C — drill high-score save array

// PRNG state (Multiply-With-Carry LCG)
extern uint32_t   g_prngState[2];            // @ 0x825CA2C8 — [0]=seed, [1]=carry
extern float      g_fPrngNormalize;          // @ 0x82079E68 — 1.0f / 2^23, scales 23-bit int to [0,1)

// Drill score scaling constant
extern float      g_fDrillScoreScale;        // @ 0x8202C504 — multiplier for bonus score computation

/**
 * pongTrainingDrill::Init
 * @ 0x8210CDB8 | size: 0x30
 * 
 * Initializes/resets the training drill to default state.
 * Sets all counters to zero, loads default time limit, and resets target index.
 * 
 * Assembly details:
 *   - Loads float constant from 0x825C5938
 *   - Initializes 7 fields to known values
 *   - Sets target index to -1 (no target)
 */
void pongTrainingDrill::Init() {
    // Initialize counters and state
    m_numAttempts = 0;
    m_numSuccesses = 0;
    m_prevSuccesses = 0;
    m_numFailures = 0;
    
    // Load default time limit from data section @ 0x825C5938
    // Original: lis r10,-32164; lfs f0,22840(r10)
    m_timeRemaining = 0.0f;  // TODO: Load actual constant value
    
    m_completionFlag = 0;
    m_targetIndex = 0xFFFFFFFF;  // -1 = no target selected
}

/**
 * pongTrainingDrill::IncreaseNumSuccesses
 * @ 0x8210D400 | size: 0x84
 * 
 * Increases the success counter by the specified amount.
 * Clamps to maximum allowed successes from config.
 * Updates network state and UI.
 * 
 * Error message @ 0x82031970:
 *   "pongTrainingDrill::IncreaseNumSuccesses() - winning more than total possible, something is"
 * 
 * @param increment Number of successes to add
 */
void pongTrainingDrill::IncreaseNumSuccesses(int increment) {
    // Add increment to current successes
    m_numSuccesses += increment;
    
    // Get maximum allowed successes from config
    uint32_t maxSuccesses = m_pConfig ? *(uint32_t*)((uint8_t*)m_pConfig + 16) : 0xFFFFFFFF;
    
    // Clamp to maximum
    if (m_numSuccesses > maxSuccesses) {
        // Error: winning more than total possible
        nop_8240E6D0("pongTrainingDrill::IncreaseNumSuccesses() - winning more than total possible, something is");
        m_numSuccesses = maxSuccesses;
    }
    
    // Update network state
    // Original: lwz r3,26132(r11) where r11 = -32160 (lis)
    // Address: 0x82036614 (g_singles_network_client)
    if (g_singles_network_client) {
        ComputeNetworkHash(g_singles_network_client, m_numSuccesses);
    }
    
    // Update UI
    // Original: li r3,20488; li r4,64; addi r5,r11,30872; li r6,1
    ProcessPageGroupInput(20488, 64, nullptr, 1);
}

/**
 * pongDrillMovement::Init
 * @ 0x8210DB48 | size: 0x54
 * 
 * Initializes the movement drill with extended state.
 * Calls base class Init() and sets up movement-specific fields.
 * 
 * Assembly details:
 *   - Loads config pointer from +56 (0x38)
 *   - Reads distance value from config +108
 *   - Initializes movement-specific fields at +36, +40, +44, +48, +52
 */
void pongDrillMovement::Init() {
    // Call base class initialization
    pongTrainingDrill::Init();
    
    // Movement-specific initialization
    m_movementFlag = 0;
    
    // Load target distance from config
    // Original: lwz r9,56(r3); lfs f13,108(r9)
    if (m_pConfig) {
        void* configExt = (void*)((uint8_t*)m_pConfig + 56);
        if (configExt) {
            m_targetDistance = *(float*)((uint8_t*)configExt + 108);
        }
    }
    
    m_movementState = MOVEMENT_STATE_0;
    m_moveTimer = 0.0f;  // Load from data section @ 0x82031970
    m_isComplete = 0;
}


/**
 * pongTrainingDrill::OnStart
 * @ 0x8210CFF0 | size: 0xA4 (164 bytes)
 *
 * Called when the training drill begins. Notifies the UI, clears the active
 * player's training-controller flag, resets hit/miss counters via virtual
 * dispatch, and transitions the training state machine to state 5 (active).
 *
 * Global accesses:
 *   - g_pGameState    (SDA+25648) — player manager, indexed by active slot
 *   - g_pTrainingStateMachine (0x8271A34C) — training HSM singleton
 */
void pongTrainingDrill::OnStart() {
    // Notify UI that the drill is starting
    NotifyUIEvent(20484, 64, 0, 0);

    // Access active player's training controller and clear its active flag
    uint32_t* pGameState = (uint32_t*)g_pGameState;
    int32_t nPlayerSlot = (int32_t)pGameState[33];                     // +132 = active player slot
    void* pPlayer = ((void**)pGameState)[nPlayerSlot + 29];            // pointer array at +116
    void* pTrainingCtrl = *(void**)((uint8_t*)pPlayer + 6208);        // +0x1840
    if (pTrainingCtrl != NULL) {
        *(uint8_t*)((uint8_t*)pTrainingCtrl + 24) = 0;                // clear active flag
    }

    // Reset hit/miss state through virtual dispatch
    OnBallHit(1);    // vfn_27 — reset with save-data update enabled
    OnBallMiss();    // vfn_28 — reset miss state

    // Transition training state machine to state 5 (drill active)
    SetTrainingState(g_pTrainingStateMachine, 5);
}

/**
 * pongTrainingDrill::OnBallHit
 * @ 0x8210D488 | size: 0x144 (324 bytes)
 *
 * Called when the ball is successfully hit during a drill. If the player
 * has met or exceeded the required successes, computes a performance score
 * based on the ratio of excess hits to the available range, scaled by a
 * constant, floored, and clamped >= 1.
 *
 * If bUpdateSaveData is true, compares the computed score against the
 * player's saved best for this drill type, and updates if improved.
 *
 * @param bUpdateSaveData  If nonzero, check and update drill save data
 *
 * Key addresses:
 *   - g_fDrillScoreScale  (0x8202C504) — scaling multiplier for bonus score
 *   - g_pTrainingData     (0x8271A31C) — +4 gives drill-set index
 *   - g_pDrillSaveData    (0x8271A35C) — per-drill save array
 */
void pongTrainingDrill::OnBallHit(int bUpdateSaveData) {
    int32_t nSuccesses = (int32_t)m_numSuccesses;
    int32_t nRequired  = (int32_t)m_pConfig->m_nRequiredSuccesses;

    // Only compute score if player has met or exceeded the requirement
    if (nSuccesses < nRequired) {
        return;
    }

    int32_t nMaxTotal = (int32_t)m_pConfig->m_nMaxSuccesses;
    int32_t nRange = nMaxTotal - nRequired;

    if (nRange == 0) {
        // No extra shots available — assign default score of 5
        m_numFailures = 5;
    } else {
        // Compute performance score: how far into the extra range the player reached
        int32_t nExcess = nSuccesses - nRequired;
        float fRatio  = (float)nExcess / (float)nRange;
        float fScaled = fRatio * g_fDrillScoreScale;
        int nScore = (int)floorf(fScaled);

        // Clamp to minimum of 1
        if (nScore < 1) {
            nScore = 1;
        }
        m_numFailures = (uint32_t)nScore;
    }

    // If save-data update is enabled, check and update the player's best score
    if ((uint8_t)bUpdateSaveData == 0) {
        return;
    }

    // Load drill-set index from training data manager (+4)
    int32_t nDrillSetIdx = *(int32_t*)((uint8_t*)g_pTrainingData + 4);

    // Get this drill's type index via virtual dispatch (vfn_17)
    int nDrillTypeIdx = GetDrillTypeIndex();

    // Compute save data array index: stride = drillSetIdx * 12, entry = stride + typeIdx + 3
    int nSaveIdx = nDrillSetIdx * 12 + nDrillTypeIdx + 3;
    int32_t nSavedBest = (int32_t)g_pDrillSaveData[nSaveIdx];

    // Only update if current score exceeds saved best
    if (nSavedBest >= (int32_t)m_numFailures) {
        return;
    }

    // Re-query drill type index (matches original — two VCALLs in scaffold)
    nDrillTypeIdx = GetDrillTypeIndex();
    nSaveIdx = nDrillSetIdx * 12 + nDrillTypeIdx + 3;

    // Write new best score and notify UI
    g_pDrillSaveData[nSaveIdx] = m_numFailures;
    NotifyUIEvent(20491, 64, 0, 0);
}

/**
 * pongTrainingDrill::OnRallyEnd
 * @ 0x8210D660 | size: 0xD4 (212 bytes)
 *
 * Called when a rally ends. If the drill config specifies a valid
 * random range (min > 0, max > 0, min <= max), generates a random
 * float in [min, max] using a Multiply-With-Carry PRNG and stores
 * it at pRallyEvent+128. If min == max, stores the exact value.
 *
 * PRNG details:
 *   - Multiply-With-Carry LCG: next = seed * 0x5CDCFAA7 + carry
 *   - Extracts 23-bit random, normalizes to [0,1), lerps in [min, max]
 *   - State at 0x825CA2C8: [0]=seed, [1]=carry
 *
 * @param pRallyEvent  Rally event data; result written to +128
 */
void pongTrainingDrill::OnRallyEnd(void* pRallyEvent) {
    float fMin = m_pConfig->m_fRangeMin;
    float fMax = m_pConfig->m_fRangeMax;

    // Validate: both must be positive and min <= max
    if (!(fMin > 0.0f && fMax > 0.0f && fMin <= fMax)) {
        return;
    }

    // If min == max, store exact value (no randomness needed)
    if (fMin == fMax) {
        *(float*)((uint8_t*)pRallyEvent + 128) = m_pConfig->m_fRangeMin;
        return;
    }

    // Generate random float in [min, max] using MWC-LCG
    uint32_t nSeed  = g_prngState[0];
    uint32_t nCarry = g_prngState[1];
    uint64_t nNext  = (uint64_t)nSeed * 0x5CDCFAA7ULL + nCarry;

    uint32_t nNewSeed  = (uint32_t)nNext;
    uint32_t nNewCarry = (uint32_t)(nNext >> 32);
    g_prngState[0] = nNewSeed;
    g_prngState[1] = nNewCarry;

    // Extract 23-bit random value and normalize to [0, 1)
    uint32_t nRandomBits = nNewSeed & 0x7FFFFF;
    float fNormalized = (float)nRandomBits * g_fPrngNormalize;

    // Lerp: result = min + (max - min) * t
    float fRange  = fMax - fMin;
    float fResult = fRange * fNormalized + fMin;
    *(float*)((uint8_t*)pRallyEvent + 128) = fResult;
}

// ─────────────────────────────────────────────────────────────────────────────
// hitTipData — XML data node for hit tip configuration
//
// Inherits from rage::xmlNodeStruct (2 vtables — multiple inheritance).
// Describes a play state tip that triggers based on shot type, score,
// and consecutive shot thresholds.
//
// Vtable 1 @ 0x82043300 (primary, 12 bytes)
// Vtable 2 @ 0x8204330C (secondary, 100 bytes)
// Inner class: hitTipData::xmlNodeStructGroupType @ 0x82043370
// ─────────────────────────────────────────────────────────────────────────────

// External type descriptor globals (used for XML property type tags)
extern uint32_t g_xmlType_ShotType;         // @ 0x825CB5B4 (.data, 4 bytes) — shot type enum descriptor
extern uint32_t g_xmlType_Int;              // @ 0x825CAF90 (.data, 4 bytes) — integer type descriptor
extern uint32_t g_xmlType_Bool;             // @ 0x825CAF94 (.data, 4 bytes) — boolean type descriptor

// XML property registration function
// Signature: RegisterProperty(this, propertyName, fieldPtr, typeDescriptor, flags)
extern void RegisterSerializedField(void* node, const char* name, void* field, void* typedesc, int flags);

// Base class destructor
extern void atSingleton_29E8_p25(void* obj);

// Type hash globals for IsApplicable checks
extern uint32_t g_tipType_Play1;            // @ 0x825C5EC4 (.data, 4 bytes)
extern uint32_t g_tipType_Play2;            // @ 0x825C803C (.data, 4 bytes)
extern uint32_t g_tipType_Play3;            // @ 0x825C8038 (.data, 4 bytes)

// Enum string lookup table for xmlNodeStructGroupType::GetGroupName
extern const char* g_shotTypeNames[];       // @ 0x8202D034 (.rdata, 24 bytes = 6 entries)

/**
 * hitTipData::~hitTipData() @ 0x821F2AB8 | size: 0x8
 *
 * Virtual destructor. Adjusts 'this' by -20 (to unwind MI offset)
 * and delegates to base class destructor at 0x821F29E8.
 */
hitTipData::~hitTipData() {
    // The MI destructor adjusts 'this' by -20 to reach the primary base
    // then calls the base xmlNodeStruct destructor chain
    // Original: addi r3,r3,-20; b atSingleton_29E8_p25
    atSingleton_29E8_p25((char*)this - 20);
}

/**
 * hitTipData::PostLoadProperties() @ 0x821F2298 | size: 0x88
 * vtable slot 2
 *
 * Validates XML properties after they are loaded from the data file.
 * Ensures ShotType is in range [0,5], and that the score/consecutive
 * fields have consistent values (or applies defaults).
 *
 * Assertion strings:
 *   @ 0x82043090 (26 bytes) — ShotType range assertion
 *   @ 0x820430AC (43 bytes) — score/consecutive consistency assertion
 */
void hitTipData::PostLoadProperties() {
    // Validate ShotType: must be in range [0, 5]
    if (m_shotType < 0 || m_shotType >= 6) {
        nop_8240E6D0("hitTipData::PostLoadProperties() - invalid ShotType");  // @ 0x82043090
    }

    // Check score/consecutive field consistency
    if (m_consecutiveThreshold < 0) {
        if (m_scoreThreshold >= 0 && m_minScore >= 0) {
            // Both score and min thresholds are valid: call base and return
            xmlNodeStruct_vfn_2(this);
            return;
        }
        // Score fields invalid: log error and set default
        nop_8240E6D0("hitTipData::PostLoadProperties() - invalid score/consecutive thresholds");  // @ 0x820430AC
        m_consecutiveThreshold = 2;  // Default to 2
    }

    // Call base class PostLoadProperties
    xmlNodeStruct_vfn_2(this);
}

/**
 * hitTipData::LoadProperties() @ 0x821F21D0 | size: 0xC8
 * vtable slot 21
 *
 * Registers 5 XML properties with the property system via game_8F58:
 *   - "ShotType"  → m_shotType     (+16), type: ShotType enum (@ 0x825CB5B4)
 *   - property2   → m_minScore     (+20), type: int (@ 0x825CAF90)
 *   - property3   → m_scoreThreshold (+24), type: int
 *   - property4   → m_consecutiveThreshold (+28), type: int
 *   - property5   → m_enabled      (+32), type: bool (@ 0x825CAF94)
 */
void hitTipData::LoadProperties() {
    // Register ShotType enum property
    // r4 = "ShotType" @ 0x82033438, r5 = this+16, r6 = &g_xmlType_ShotType @ 0x825CB5B4
    RegisterSerializedField(this, "ShotType", &m_shotType, &g_xmlType_ShotType, 0);

    // Register integer properties with type descriptor @ 0x825CAF90
    // r4 @ 0x82043070 (8 bytes), r5 = this+20
    RegisterSerializedField(this, "MinScore", &m_minScore, &g_xmlType_Int, 0);

    // r4 @ 0x82043078 (6 bytes), r5 = this+24
    RegisterSerializedField(this, "Score", &m_scoreThreshold, &g_xmlType_Int, 0);

    // r4 @ 0x82043080 (7 bytes), r5 = this+28
    RegisterSerializedField(this, "InARow", &m_consecutiveThreshold, &g_xmlType_Int, 0);

    // Register boolean property with type descriptor @ 0x825CAF94
    // r4 @ 0x82043088 (7 bytes), r5 = this+32
    RegisterSerializedField(this, "Active", &m_enabled, &g_xmlType_Bool, 0);
}

/**
 * hitTipData::IsApplicable() @ 0x821F2188 | size: 0x48
 * vtable slot 20
 *
 * Checks whether this tip data applies to the given type hash.
 * Compares against three global type hashes:
 *   - g_tipType_Play1 @ 0x825C5EC4
 *   - g_tipType_Play2 @ 0x825C803C
 *   - g_tipType_Play3 @ 0x825C8038
 *
 * @param typeHash  The type identifier to check against
 * @return true if this tip applies to the given type
 */
bool hitTipData::IsApplicable(uint32_t typeHash) {
    // Check against first two type hashes (either matches → true)
    if (typeHash == g_tipType_Play1 || typeHash == g_tipType_Play2) {
        return true;
    }

    // Check against third type hash
    return (typeHash == g_tipType_Play3);
}

/**
 * hitTipData::GetNodeTypeName() @ 0x82115BD8 | size: 0xC
 * vtable slot 22
 *
 * Returns the XML node type name string for this data class.
 * Used by the XML serialization system to identify node types.
 *
 * @return "play" — the XML element name for hit tip data nodes
 */
const char* hitTipData::GetNodeTypeName() {
    return "play";  // @ 0x820326DC (.rdata, "play")
}

/**
 * hitTipData::xmlNodeStructGroupType::GetGroupName() @ 0x821F2130 | size: 0x18
 * vtable slot 5 of inner class xmlNodeStructGroupType @ 0x82043370
 *
 * Returns the display name for the shot type group based on index.
 * Looks up index from field +36 (m_groupIndex) in a 6-element string table
 * at 0x8202D034.
 *
 * The table contains shot type names (6 entries × 4 bytes = 24 bytes).
 *
 * @return The shot type group display name string
 */
const char* hitTipData_xmlNodeStructGroupType_GetGroupName(void* self) {
    // Read m_groupIndex from self+36
    uint32_t groupIndex = *(uint32_t*)((uint8_t*)self + 36);

    // Index into the shot type names table (shift left by 2 = multiply by 4)
    // Original: rlwinm r9,r10,2,0,29 → index * 4
    return g_shotTypeNames[groupIndex];
}

/**
 * pongDrillSoftShot::ScalarDestructor
 * @ 0x8210EDA0 | size: 0x64 (100 bytes)
 * vtable slot 1
 *
 * Scalar destructor for pongDrillSoftShot. Stores destruction flags
 * and conditionally updates extended state based on a virtual call result.
 *
 * This is the deleting destructor variant used for array deletion.
 * It stores the destruction flag at two locations (+32 and conditionally +56)
 * after calling a virtual function to check if extended cleanup is needed.
 *
 * @param flags Destruction flags (typically 1 for delete, 0 for in-place)
 */
void pongDrillSoftShot::ScalarDestructor(int flags) {
    // Store destruction flag at offset +32
    // This appears to be a cleanup state flag in the derived class
    *(uint32_t*)((uint8_t*)this + 32) = flags;
    
    // Load parameter from global data section
    // Address: 0x825C74FC (.data, 4 bytes)
    extern uint32_t g_drillCleanupParam;  // @ 0x825C74FC
    
    // Call virtual function slot 20 with the global parameter
    // This is likely a cleanup validation or state check function
    typedef uint8_t (*CleanupCheckFn)(void*, uint32_t);
    CleanupCheckFn checkFn = (CleanupCheckFn)(((void***)this)[0][20]);
    uint8_t needsExtendedCleanup = checkFn(this, g_drillCleanupParam);
    
    // If the check returns non-zero, store flag at extended offset +56
    // This suggests conditional cleanup of additional derived class state
    if (needsExtendedCleanup != 0) {
        *(uint32_t*)((uint8_t*)this + 56) = flags;
    }
}
