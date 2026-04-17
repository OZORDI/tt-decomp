// pong_drill.cpp — Training drill implementations | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * Training Drill Classes Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "pong_drill.hpp"
#include "globals.h"
#include "game/pong_strings.hpp"
#include <stdio.h>
#include <math.h>

// External function declarations
extern "C" {
    void ProcessPageGroupInput(int param1, int param2, const char* param3, int param4);
    void NotifyUIEvent(int eventId, int flags, int param3, int param4);  // pg_E6E0 @ 0x8225E6E0
    void SetTrainingState(void* pStateMachine, int nState);             // atSingleton_E9F8_w @ 0x821EE9F8
    void rage_debugLog(const char* message);
    void xmlNodeStruct_vfn_2(void* self);  // @ base class PostLoadProperties
}

// External globals
extern struct SinglesNetworkClient* g_singles_network_client;  // @ 0x82036614
extern void  ComputeNetworkHash(void* networkClient, int numSuccesses);

// Game state globals
extern struct pongGameState* g_pGameState;   // @ SDA+25648 (0x82606430) — player/game state manager
extern void*      g_pTrainingStateMachine;   // @ 0x8271A34C — training HSM singleton
extern struct pongTrainingDrill* g_pTrainingData;  // @ 0x8271A31C — training drill data manager
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
        rage_debugLog(g_str_pongDrill_tooManySuccesses);
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
        rage_debugLog("Invalid hit tip found: %d");  // @ 0x82043090
    }

    // Check score/consecutive field consistency
    if (m_consecutiveThreshold < 0) {
        if (m_scoreThreshold >= 0 && m_minScore >= 0) {
            // Both score and min thresholds are valid: call base and return
            xmlNodeStruct_vfn_2(this);
            return;
        }
        // Score fields invalid: log error and set default
        rage_debugLog("Invalid hit tip, setting InARow to default");  // @ 0x820430AC
        m_consecutiveThreshold = 2;  // Default to 2
    }

    // Call base class PostLoadProperties
    xmlNodeStruct_vfn_2(this);
}

/**
 * hitTipData::LoadProperties() @ 0x821F21D0 | size: 0xC8
 * vtable slot 21
 *
 * Registers 5 XML properties with the property system via RegisterSerializationField:
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
 * @return "HitTip" — the XML element name for hit tip data nodes
 */
const char* hitTipData::GetNodeTypeName() {
    return "HitTip";  // @ 0x820326DC (.rdata, "HitTip")
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

/**
 * noSoftShotsTipData::PostLoadProperties @ 0x821F2678 | size: 0x48
 * 
 * Validates that the provided type hash matches one of the allowed shot types
 * for the "no soft shots" tip. Returns true if the type is valid.
 * 
 * This tip data validates against three specific shot type hashes stored in globals.
 */
bool noSoftShotsTipData::PostLoadProperties(uint32_t typeHash) {
    // Check against first allowed type
    if (typeHash == g_shotType1_825C5F50) {
        return true;
    }

    // Check against second allowed type
    if (typeHash == g_shotType2_825C803C) {
        return true;
    }

    // Check against third allowed type
    return (typeHash == g_shotType3_825C8038);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongTrainingUIContext — drill / mini-games HSM context singleton
// Mirrors pongGameContext (src/game/match/pong_game.cpp). Multiple-inheritance
// layout with secondary vtable at +0x14; child drill session at +0x1C.
// ─────────────────────────────────────────────────────────────────────────────

// Forward declarations for helpers consumed by the 6 lifted slots.
extern "C" {
    // Guest memory management / debug log.
    void  rage_free_00C0(void* ptr);                                            // @ 0x820C00C0
    void  xe_main_thread_init_0038(void);                                       // @ 0x820C0038
    void  nop_8240E6D0(const char* fmt, ...);                                   // @ 0x8240E6D0 — debug/log sink
    void* rage_ADF8(void* classCtx, int param1, int allocSize);                 // @ 0x822EADF8 — allocator + ctor

    // Message-system bridge (shared with pongGameContext).
    int   msgEventHandler_E8C0_g(void* handlerTail);                            // @ 0x8225E8C0

    // Network/match helpers driven by OnUpdate when the drill completes.
    uint8_t SinglesNetworkClient_45D0_g(void* ctx);                             // @ 0x821045D0 — "is leaving?" probe
    uint8_t SinglesNetworkClient_B2A8_g(void* ctx);                             // @ 0x822EB2A8
    void    SinglesNetworkClient_B1E8_g(void* ctx);                             // @ 0x822EB1E8
    void*   SinglesNetworkClient_9318_g(void* ctx, const char* key);            // @ 0x823F9318 — key lookup
    void    SinglesNetworkClient_B320_g(void* ctx);                             // @ 0x822EB320
    void    SinglesNetworkClient_9C58_g(void* child);                           // @ 0x82319C58
    void    SinglesNetworkClient_9ED0_g(void* child);                           // @ 0x82319ED0
    uint8_t SinglesNetworkClient_9BC8_g(void* ctx);                             // @ 0x82309BC8
    void    SinglesNetworkClient_9B40_g(void* ctx);                             // @ 0x82309B40

    // HSM state transition.
    void  hsmContext_SetNextState_2800(void* hsmCtx, int stateIdx);             // @ 0x82222800

    // Child drill session factory tail.
    void* game_7EB8(void* raw);                                                 // @ 0x82317EB8
}

// SDA / global data consumed by pongTrainingUIContext.
extern void* lbl_8271A350;   // training-drill slot pool base (msg 3)
extern void* lbl_8271A32C;   // training-drill global count   (msg 3 gate)
extern void* lbl_8271A82C;   // finalize target (network finalize receiver)
extern void* lbl_8271884C;   // msg-handler registration table (byte + stride)
extern void* lbl_82606390;   // SDA: hsm-state root (read as r13+0x2F70)
extern void* lbl_82606514;   // SDA: UI context back-pointer

// ────────────────────────────────────────────────────────────────────────────
// pongTrainingUIContext — 6 true vtable slots lifted from 0x82307080..0x82307520
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongTrainingUIContext::Dtor  [0]  @ 0x82307080 | size: 0x9C
 *
 * MI-aware destructor. Writes the primary vtable stamp at +0x00 and the
 * secondary at +0x14, then invokes slot 0 on the child drill session at
 * +0x1C with flags=1 (also-free). After the child teardown, re-stamps
 * with the transient intermediate slots before returning control to the
 * caller; if the low bit of `flags` is set, hands the object back to
 * rage_free.
 *
 * Stamp encodings (from lis/addi pairs):
 *   0x8205E6D4  primary vtable       (pongTrainingUIContext)
 *   0x8205E73C  secondary vtable     (MI thunk block, 12 bytes)
 *   0x82027B34  transient base stamp (lis -32254, +31540)
 *   0x820276C4  transient secondary  (lis -32254, +30404)
 */
void pongTrainingUIContext::Dtor(int flags) {
    // Initial stamp pair sets the primary / secondary vtables to the
    // pongTrainingUIContext globals. The recomp emits two stw pairs — the
    // first writes the class-specific VAs, the child-chain slot 0 VCALL
    // runs, then the transient intermediates are stored before returning.
    vtable            = 0x8205E6D4u;  // primary
    m_vtableSecondary = 0x8205E73Cu;

    // Tear down the child drill-session object if present.
    if (m_pChildCtx != 0) {
        void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pChildCtx));
        using DtorFn = void(*)(void*, int);
        void** childVt = *(void***)child;
        reinterpret_cast<DtorFn>(childVt[0])(child, 1);
    }

    // Transient base-class stamps.
    m_vtableSecondary = 0x82027B34u;
    vtable            = 0x820276C4u;

    if ((flags & 0x1) != 0) {
        rage_free_00C0(this);
    }
}

/**
 * pongTrainingUIContext::OnEnter  [11]  @ 0x82307120 | size: 0x60
 *
 * Registers this context's embedded event-handler (`this + 20`) with the
 * global message-handler table at 0x8271884C. The table uses a parallel
 * 1-byte-strided table at 14952 + r11 for handler IDs, and a 4-byte-strided
 * table at 14696 + r11 for parameter slots; the slot count at 14436 is
 * incremented each call. The registration key (param at 14696 + r11) is
 * a constant 1040 — which is the drill-mode handler ID.
 */
void pongTrainingUIContext::OnEnter() {
    // Handler pointer = this + 20 (the MI-offset 'secondary' view of `this`).
    // When this==nullptr the recomp slams the pointer to 0; we replicate.
    void* handler = (this != nullptr)
        ? reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(this) + 20)
        : nullptr;

    // Global tables live inside the lbl_82713000..lbl_82718000 cluster; the
    // three addresses are:
    //   r9 + 14436  → u32 slot counter               (@ 0x82713024 + 14436)
    //   r8 + 14952  → u8  handler-kind byte-table
    //   r7 + 14440  → u32 handler pointer table
    //   r6 + 14696  → u32 key/stride table
    // Addresses are hoisted via lis -32161 = 0x82017000 — so:
    //   0x82017000 + 14436 = 0x820186A4
    //   0x82017000 + 14952 = 0x820188A8
    //   0x82017000 + 14440 = 0x820186A8
    //   0x82017000 + 14696 = 0x820187A8
    extern uint32_t lbl_820186A4_counter;   // u32 slot counter
    extern uint8_t  lbl_820188A8_kinds[];   // u8  handler-kind table (byte stride)
    extern uint32_t lbl_820186A8_handlers[];// u32 handler pointer table (word stride)
    extern uint32_t lbl_820187A8_keys[];    // u32 key/stride table (word stride)

    uint32_t slotIdx = lbl_820186A4_counter;
    lbl_820188A8_kinds[slotIdx]   = 128;   // handler-kind = 128 (MI-thunk dispatch)
    lbl_820186A8_handlers[slotIdx] = (uint32_t)(uintptr_t)handler;
    lbl_820187A8_keys[slotIdx]     = 1040; // drill-mode key
    lbl_820186A4_counter           = slotIdx + 1;
}

/**
 * pongTrainingUIContext::OnExit  [12]  @ 0x82307180 | size: 0x34
 *
 * Deregisters the embedded event-handler slot. The recomp delegates
 * straight to msgEventHandler_E8C0_g(this+20) which walks the same table
 * populated by OnEnter and removes the matching entry, then returns 1 to
 * the caller (always true — reports "registration was valid").
 */
void pongTrainingUIContext::OnExit() {
    void* handler = (this != nullptr)
        ? reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(this) + 20)
        : nullptr;
    msgEventHandler_E8C0_g(handler);
    // Original returns 1 in r3; this skeleton is void — no caller inspects.
}

/**
 * pongTrainingUIContext::OnUpdate  [16]  @ 0x823071B8 | size: 0x128
 *
 * Per-frame drill session driver. Two-phase:
 *
 *   Phase 1 (drill-complete handoff):
 *     If m_bPendingStateAdvance is set, walk the drill-save registry at
 *     lbl_8271A350 (base) + lbl_8271A32C (count).slot*15044 and stamp
 *     offset +536 = 3 (msg kind) on the matching record. Clears the flag
 *     and raises m_bNetFinalizePending.
 *
 *   Phase 2 (network finalize):
 *     If m_bNetFinalizePending is set AND SinglesNetworkClient_45D0_g says
 *     "not leaving", run the three-step finalize (+B2A8, +B1E8, +9318 key
 *     "Loading Traing" etc.) and clear the flag.
 *
 *   Tail: If HSM state +0xC == 13 (drill-active state), VCALL slot 2 on
 *         the child session; if the child's "post-eval" byte (+96) fires
 *         and the UI finalize probe (+9BC8/+9B40) returns nonzero, drive
 *         hsmContext_SetNextState to state 9 (post-drill summary).
 */
void pongTrainingUIContext::OnUpdate() {
    // Phase 1 — drill-session completion handoff.
    if (m_bPendingStateAdvance) {
        void* slotBase = lbl_8271A350;
        // Count of active drill-save records (u32 at lbl_8271A32C).
        uint32_t count = *(uint32_t*)((uint8_t*)lbl_8271A32C + 4);
        uint8_t* record = (uint8_t*)slotBase + count * 15044u;
        *(uint32_t*)(record + 536) = 3;  // msg kind 3 = "drill complete"
        m_bPendingStateAdvance = false;
        m_bNetFinalizePending  = true;
    }

    // Phase 2 — finalize the network slot now that the drill is tallied.
    if (m_bNetFinalizePending) {
        if (SinglesNetworkClient_45D0_g(this) == 0) {
            void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pChildCtx));
            uint8_t probe = SinglesNetworkClient_B2A8_g(child);
            SinglesNetworkClient_B1E8_g(child);

            // Slot lookup keyed on "Loading Traing" const string.
            void* slot = SinglesNetworkClient_9318_g(child, (const char*)0x8205E5D8);
            if (slot != nullptr) {
                *(uint32_t*)((uint8_t*)slot + 0) = 0;
                *(uint32_t*)((uint8_t*)slot + 4) = 3;
            }
            if (probe != 0) {
                SinglesNetworkClient_B320_g(child);
            }
            m_bNetFinalizePending = false;
        }
    }

    // Tail — HSM state gate on +0xC of SDA hsm-state root (r13+0x2F70 = 0x82606390).
    uint8_t* hsmRoot = (uint8_t*)lbl_82606390;
    int32_t hsmState = *(int32_t*)(hsmRoot + 12);
    if (hsmState != 13) {
        return;
    }

    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pChildCtx));
    using Vfn2 = void(*)(void*);
    void** childVt = *(void***)child;
    reinterpret_cast<Vfn2>(childVt[2])(child);  // slot 2 (byte +8) — tick drill

    uint8_t postEval = *((uint8_t*)child + 96);
    if (postEval == 0) {
        return;
    }

    // UI finalize probe lives at lbl_8271A82C — usually the post-drill HUD.
    void* uiFinalize = lbl_8271A82C;
    if (SinglesNetworkClient_9BC8_g(uiFinalize) != 0) {
        SinglesNetworkClient_9B40_g(uiFinalize);
    }
    hsmContext_SetNextState_2800(lbl_82606390, 9);  // state 9 = post-drill summary
}

/**
 * pongTrainingUIContext::OnRender  [18]  @ 0x823072E0 | size: 0x40
 *
 * Gated render pass. Pulls the HSM state from lbl_82606390+0xC; bails when
 * not 13 (drill-active). Also bails when the input-state singleton's
 * suspended byte (+27 of lbl_82606028) is nonzero — the UI is frozen.
 * Otherwise dispatches slot 6 (byte +24 in vtable) on the child drill
 * session at m_pChildCtx.
 */
void pongTrainingUIContext::OnRender() {
    uint8_t* hsmRoot = (uint8_t*)lbl_82606390;
    int32_t  hsmState = *(int32_t*)(hsmRoot + 12);
    if (hsmState != 13) {
        return;
    }

    // Input-state suspend byte (+27) — reused from pong_game.cpp wiring.
    extern void* lbl_82606028;   // input-state back-pointer (SDA +0x1308)
    uint8_t* input = (uint8_t*)lbl_82606028;
    if (input[27] != 0) {
        return;
    }

    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pChildCtx));
    using Vfn6 = void(*)(void*);
    void** childVt = *(void***)child;
    reinterpret_cast<Vfn6>(childVt[6])(child);  // slot 6 (byte +24)
}

/**
 * pongTrainingUIContext::Init  [23]  @ 0x82307420 | size: 0x100
 *
 * Constructs the child drill-session object. Logs "Loading Traing Mode
 * UI..." (0x8205E5D8, binary-original typo preserved), calls the engine
 * init sink, then walks SDA r13+0x4 (= 0x82600004) to reach the singles-
 * match logic factory — VCALL slot 1 (byte +4). Nonzero return feeds into
 * game_7EB8 (session wrap); result cached in r31. Allocates a 208-byte
 * drill-context object via rage_ADF8 with key-offset +50 (= 0x82714B8A:
 * "drillUIContext" string head), stores it at this+28. VCALL slot 3 on
 * the fresh child with f1 = *(float*)0x825C8C78 (the drill-tick dt).
 * Tail runs the three SinglesNetworkClient helpers + logs "Training Mode
 * UI loaded." (0x8205E5F4).
 */
void pongTrainingUIContext::Init() {
    nop_8240E6D0((const char*)0x8205E5D8);  // "Loading Traing Mode UI..."
    xe_main_thread_init_0038();

    // SDA root lookup: r13+0x4 → global class registry pointer.
    extern void* g_SDA_root;  // @ 0x82600000
    void* registry = *(void**)((uint8_t*)&g_SDA_root + 4);

    // VCALL slot 1 (byte +4) with (r4=29012, r5=16) — factory probe for
    // "singles-match" logic class; nonzero path runs game_7EB8 tail wrap.
    using FactoryFn = void*(*)(void*, int, int);
    void** regVt = *(void***)registry;
    void* factory = reinterpret_cast<FactoryFn>(regVt[1])(registry, 29012, 16);

    void* childRaw = nullptr;
    if (factory != nullptr) {
        childRaw = game_7EB8(factory);
    }

    m_pChildCtx = (uint32_t)(uintptr_t)childRaw;

    // Allocate the 208-byte drill-context from the class registry (param1=+50
    // is the "drillUIContext" class name index; param2=208 is object size).
    extern uint8_t lbl_82606028_base[]; // see OnRender
    uint32_t classNameIdx = *(uint32_t*)((uint8_t*)lbl_82606514) + 50;
    rage_ADF8(childRaw, (int)classNameIdx, 208);

    // VCALL slot 3 (byte +12) on the fresh child with the drill dt scalar.
    if (childRaw != nullptr) {
        extern const float lbl_825C90BC_drillDT;  // @ 0x825C90BC + 84
        using InitFn = void(*)(void*, float);
        void** childVt = *(void***)childRaw;
        reinterpret_cast<InitFn>(childVt[3])(childRaw, lbl_825C90BC_drillDT);
    }

    // Network finalize / post-init triad.
    uint8_t probe = SinglesNetworkClient_B2A8_g(childRaw);
    SinglesNetworkClient_9C58_g(childRaw);
    SinglesNetworkClient_9ED0_g(childRaw);
    if (probe != 0) {
        SinglesNetworkClient_B320_g(childRaw);
    }

    // Stamp default drill dt on child+8 and finish.
    extern const float lbl_825C90BC_drillDT;
    if (childRaw != nullptr) {
        *(float*)((uint8_t*)childRaw + 8) = lbl_825C90BC_drillDT;
    }

    nop_8240E6D0((const char*)0x8205E5F4);  // "Training Mode UI loaded."
}
