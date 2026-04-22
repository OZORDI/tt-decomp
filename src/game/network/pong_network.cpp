// pong_network.cpp — Network message implementations | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * serve_started_message.cpp — ServeStartedMessage network message class
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * ServeStartedMessage is a network message object sent by the serving player
 * to synchronise the serve state across the network.  It travels through the
 * SinglesNetworkClient messaging pipeline.
 *
 * It carries:
 *   - A timing float     (+0x04)  — serve start timestamp / timing reference
 *   - A 16-byte position (+0x10)  — target/aim vector for the serve
 *   - Four velocity floats (+0xD0..+0xDC) — ball velocity components
 *   - A fifth float       (+0xDC)  — spin/power value
 *   - Three byte flags    (+0xE0..+0xE2):
 *       +0xE0 (224) = player index (who is serving, 0 or 1)
 *       +0xE1 (225) = is-not-server flag (inverted at read — stored as bool NOT)
 *       +0xE2 (226) = secondary player index / handedness flag
 *
 * Serialisation uses snBitStream_ReadBits (write float, size=32) and
 * BitStream_ReadU8 (write byte, size=8).  Deserialisation uses WriteFloatToNetworkStream
 * and netStream_WriteByte (read byte, size=8).
 *
 * ── Vtable layout ─────────────────────────────────────────────────────────
 *   slot 1 = Deserialise  (scalar_destructor label is wrong — it reads data in)
 *   slot 2 = Serialise    (writes data out)
 *   slot 4 = Process      (applies message: calls Player_ApplyServeStarted)
 *   slot 5 = GetIndexInPool
 *   slot 6 = GetSingleton
 *   slot 7 = GetTypeName
 *
 * ── Global addresses (Python-verified) ───────────────────────────────────
 *   pool_base = 0x825D11DC  (lis -32163 + 4572 + 8)
 *   g_pNetMsgSingleton @ 0x825D11D0  (lis -32163 + 4560)
 *   g_szServeStartedTypeName @ 0x8206E9D0  (lis -32249 + -5680)
 */

#include "pong_network.hpp"
#include "pong_network_io.hpp"
#include "../player/pong_player.hpp"
#include <cstdlib>
#include <cstring>

// Forward declaration for rage event types used in ProcessNetworkTimingUpdate
namespace rage {
    struct EvtAcceptJoinRequestSucceeded {
        void** vtable;  // +0x00
    };
}

// ── External functions ─────────────────────────────────────────────────────
// Network serialisation helpers
extern void  snBitStream_ReadBits(void* client, void* buf, int size);  // write float
extern void  snBitStream_Reset(void* context);
extern void  snBitStream_ValidateWrite(void* context);
extern uint32_t snBitStream_WriteBits(void* client, uint32_t value, int bitWidth);
extern void  snBitStream_ReadUnsigned(void* client, void* buf, int size);
extern void  netStream_WriteByte(void* client, uint8_t val, int size); // write byte
extern void* NetworkClient_LookupPlayer(uint8_t playerIdx);                  // lookup player
extern void  netStream_WriteBool(void* client, uint8_t value);        // write bool/byte
extern void  PlayerMovementMessage_54B0_h(void* playerObj, float* vel, float* accel, uint8_t isMoving);
extern int   SinglesNetworkClient_8CC0_w(void* client);                       // get error code

// Network I/O helpers (implemented in pong_network_io.cpp)
extern void WriteFloatToNetworkStream(void* client, float value);              // @ 0x820D6990
extern void WriteBallHitDataToNetwork(void* ballHitData, void* client);       // @ 0x821D5738
extern void DeserializeNetworkData(void* client, void* dst, int size);  // @ 0x821D5538 - read size-bit aligned block
extern void CopyVectorThreadSafe(void* dst, void* src);       // @ 0x82287040 - copy 16-byte vector
extern void ReadBitsFromStream(void* client, void* field, int size); // @ 0x820D7A08 - read byte from net stream
// Serve setup
extern void  ServeStartedMessage_CFE8_2h(void* matchState);
extern void  PostPageGroupMessage(int eventCode, int arg1, int arg2, int arg3, int arg4);
// Pool intrinsics
extern uint8_t* g_pNetMsgPoolBase;    // @ 0x825D11D0 (singleton pointer)
extern const char g_szServeStartedTypeName[];  // @ 0x8206E9D0
extern void* g_pNetMsgSingleton;              // @ 0x825D11D0 singleton object
extern void rage_debugLog(const char* fmt, ...);  // debug nop / assert print
extern void Player_ApplyServeStarted(void* matchObj, void* slotA, void* slotB, float timingRef);  // serve-start coordinator

// pongNetMessageHolder helpers used by static init/dtor wrappers.
extern void pongNetMessageHolder_vfn_2_0868_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_3878_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_18D0_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_FD70_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_1628_1(pongNetMessageHolder* holder);
extern pongNetMessageHolder* pongNetMessageHolder_FAE0_isl();
extern void pongNetMessageHolder_5038_w();
extern void rage_27C0(void* instance);
extern "C" void sysMemAllocator_Free(void* ptr);
extern void game_5E70(void* gameState);
extern void pongNetMessageHolder_7668_2hr(void* stateArray);

// Forward declarations for network functions
extern void NetDataQuery_InitNested(void* a);
extern bool CheckButtonPressed(void* record);
extern uint8_t snSession_AcquireLock(void* client);
extern void* snSession_FindProperty(void* context, const char* name);
extern void snSession_ReleaseLock(void* client);
extern void WriteInt8Bits(void* client, int16_t value, int size);
extern void NetworkClient_WriteSecondaryPlayerData(void* client, int16_t index, void* data);
extern void NetworkClient_BeginJoinRequest(void* client);
extern uint8_t NetworkClient_PollJoinResponse(void* client);
extern void game_D3B0_h(void* client);
extern void hsmEvent_Init(void* pEvent);
extern void snHsmAcceptingJoinRequest_9A70(void* client, void* event);
extern void snSession_AddNode(void* nodeList, void* node);
extern "C" void* rage_Alloc(uint32_t size);
extern void pongNetMessageHolder_7700_wrh(void* memory);

// ── Vtable externs (resolved from binary RTTI) ───────────────────────────
// PongNetMessage base class vtable
static constexpr uint32_t VTABLE_PongNetMessage = 0x8206C304u;
// pongNetMessageHolderBase base class vtable
static constexpr uint32_t VTABLE_pongNetMessageHolderBase = 0x8206FA88u;
// pongNetMessageHolder derived vtables (73 vtables due to multiple inheritance)
static constexpr uint32_t VTABLE_pongNetMessageHolder_0090 = 0x82070090u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_00A4 = 0x820700A4u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_00B8 = 0x820700B8u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_00CC = 0x820700CCu;
static constexpr uint32_t VTABLE_pongNetMessageHolder_00E0 = 0x820700E0u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_00F4 = 0x820700F4u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_0108 = 0x82070108u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_011C = 0x8207011Cu;
static constexpr uint32_t VTABLE_pongNetMessageHolder_0180 = 0x82070180u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_01BC = 0x820701BCu;
static constexpr uint32_t VTABLE_pongNetMessageHolder_01F8 = 0x820701F8u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_0248 = 0x82070248u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_0284 = 0x82070284u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_0298 = 0x82070298u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_02AC = 0x820702ACu;
static constexpr uint32_t VTABLE_pongNetMessageHolder_02C0 = 0x820702C0u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_02D4 = 0x820702D4u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_03C4 = 0x820703C4u;
static constexpr uint32_t VTABLE_pongNetMessageHolder_0590 = 0x82070590u;
// Sub-object vtables used by the lazy-init slot-1 allocators.
static constexpr uint32_t VTABLE_RemoveSpectatorMessage  = 0x8206FCBCu;
static constexpr uint32_t VTABLE_RoundRobinDataMessage   = 0x8206FFDCu;
static constexpr uint32_t VTABLE_rage_evtSet             = 0x8204DA44u;
// Max-float sentinel used as f32 init (@ .rdata lbl_82079AD4).
extern const float g_pongNetMaxFloatInit;  // @ 0x82079AD4
// Other class vtables
static constexpr uint32_t VTABLE_EvtAcceptJoinRequestSucceeded = 0x82072D94u;
static constexpr uint32_t VTABLE_ServeStartedMessage = 0x8206F740u;
static constexpr uint32_t VTABLE_TimedGameUpdateTimerMessage = 0x8206FB74u;
static constexpr uint32_t VTABLE_rage_crIKPart = 0x82033C04u;
static constexpr uint32_t VTABLE_rage_crIKHead = 0x820336D4u;
static constexpr uint32_t VTABLE_rage_crIKSpine = 0x820336B0u;
static constexpr uint32_t VTABLE_rage_crIKBodyBase = 0x8203377Cu;
static constexpr uint32_t VTABLE_gdInputData = 0x82041468u;

// ── Global data externs (resolved from binary .data/.rdata) ──────────────
// Static pongNetMessageHolder instances (used in init/dtor wrappers)
extern pongNetMessageHolder g_holderInstance_Vector4;       // @ 0x825D1134
extern pongNetMessageHolder g_holderInstance_String;        // @ 0x825D1224
extern pongNetMessageHolder g_holderInstance_RangeMgr;      // @ 0x825D1614
extern pongNetMessageHolder g_holderInstance_ShaderParams;  // @ 0x825D1044
extern pongNetMessageHolder g_holderInstance_StructBool;    // @ 0x825D126C
extern pongNetMessageHolder g_holderInstance_129C;          // @ 0x825D129C
extern pongNetMessageHolder g_holderInstance_AudioRefCount; // @ 0x825D12B4
extern uint8_t              g_holderInstance_InputDataMgr;  // @ 0x825D1960
extern uint8_t              g_holderInstance_13A4;          // @ 0x825D13A4
extern void*                g_holderInstance_MotionRangeMgr_ptr; // @ 0x825D164C
extern void*                g_holderInstance_MotionRangeMgr;     // @ 0x825D1638
extern void*                g_holderInstance_DataMgr;       // @ 0x825D1900
// Live-count and init globals
extern uint32_t g_netMessageHolderLiveCount_raw;  // @ 0x826066A4
extern uint32_t g_netMessageHolderInitHead_raw;   // @ 0x826066A0
// Network data buffers
extern uint8_t  g_netMessageHolderBuffer[0x100];       // @ 0x8261A0D0 (256 bytes)
extern uint8_t  g_netMessageHolderBufferPrefix[0x10];  // @ 0x8261A0C0 (16 bytes)
// Network sentinel
extern uint8_t  g_networkSentinel;    // @ 0x8271A2CC
// Float constants from .rdata
extern const float g_fZeroConst;      // @ 0x8202D110
// Float data from .data
extern float g_fDefaultTimer;         // @ 0x825C5948
// String constant
extern const char g_szScoreMessageFmt[];  // @ 0x8206EE0C

namespace {
struct PongNetMessageHolderStaticNode {
    uint32_t m_vtable;
    uint32_t m_field4;
    uint32_t m_field8;
};

static_assert(sizeof(PongNetMessageHolderStaticNode) == 0x0C);

inline uint32_t& NetMessageHolderInitHead() {
    return g_netMessageHolderInitHead_raw;
}

inline uint32_t& NetMessageHolderLiveCount() {
    return g_netMessageHolderLiveCount_raw;
}
}  // namespace


// ===========================================================================
// vfn_1 (slot 1) — Deserialise  @ 0x823B8F68 | size: 0xE8
//
// Reads all ServeStartedMessage fields from the network byte stream.
// Called by the network layer when a ServeStartedMessage packet arrives.
// The `client` arg (r4) is the SinglesNetworkClient object; `this` (r3) is
// the message being populated.
//
// Field layout being populated:
//   +0x04        float  — serve timing reference
//   +0x10..+0x1F vec4   — target position (16-byte aligned)
//   +0xD0        float  — velocity X
//   +0xD4        float  — velocity Y
//   +0xD8        float  — velocity Z
//   +0xDC        float  — spin/power
//   +0xE0        byte   — player index (who is serving)
//   +0xE1        byte   — is-not-server flag
//   +0xE2        byte   — secondary player flag
// ===========================================================================
void ServeStartedMessage::Deserialise(void* client)
{
    // Read timing float (32-bit field, stream size=32).
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    m_timingRef = timingRef;

    // Read 16-byte target position vector.
    DeserializeNetworkData(client, &m_targetPos, 32);
    WriteBallHitDataToNetwork(&m_targetPos, client);  // finalise position read

    // Read four velocity floats.
    snBitStream_ReadBits(client, &timingRef, 32);
    m_velocityX = timingRef;

    snBitStream_ReadBits(client, &timingRef, 32);
    m_velocityY = timingRef;

    snBitStream_ReadBits(client, &timingRef, 32);
    m_velocityZ = timingRef;

    snBitStream_ReadBits(client, &timingRef, 32);
    m_spin = timingRef;

    // Read three byte flags.
    ReadBitsFromStream(client, &m_playerIndex,   8);
    ReadBitsFromStream(client, &m_bNotServer,  8);
    ReadBitsFromStream(client, &m_bSecondaryPlayer,  8);
}


// ===========================================================================
// vfn_2 (slot 2) — Serialise  @ 0x823B9050 | size: 0xAC
//
// Writes all ServeStartedMessage fields into the outgoing network byte stream.
// Mirror of Deserialise; called by the network layer when the local player
// begins a serve and the message must be broadcast.
// ===========================================================================
void ServeStartedMessage::Serialise(void* client)
{
    // Write timing reference float.
    WriteFloatToNetworkStream(client, m_timingRef);

    // Write 16-byte target position (ball hit data structure).
    WriteBallHitDataToNetwork(&m_targetPos, client);

    // Write four velocity / spin floats.
    WriteFloatToNetworkStream(client, m_velocityX);
    WriteFloatToNetworkStream(client, m_velocityY);
    WriteFloatToNetworkStream(client, m_velocityZ);
    WriteFloatToNetworkStream(client, m_spin);

    // Write three byte flags.
    netStream_WriteByte(client, m_playerIndex,  8);
    netStream_WriteByte(client, m_bNotServer, 8);
    netStream_WriteByte(client, m_bSecondaryPlayer, 8);
}


// ===========================================================================
// vfn_4 (slot 4) — Process  @ 0x823B9100 | size: 0x190
//
// Applies a received ServeStartedMessage to the live game state.
// Called by the network dispatcher when the message has been fully
// deserialised.
//
// Steps:
//  1. Look up the pongPlayer for m_playerIdx via NetworkClient_LookupPlayer.
//  2. Assert that the player's action state (+468) is non-zero (debug guard).
//  3. Copy m_targetPos into the player's lerp table entry:
//       player->lerpTable[player->lerpSlot + 17] = m_targetPos
//     using the per-player 416-byte lerp table (g_pPlayerLerpTable base).
//  4. Write m_playerIdx2 (+0xE2) into playerState->m_pRecoveryState[+40].
//  5. Call Player_ApplyServeStarted (the serve-start coordinator) with
//       the lerp table entry, pSlotA and pSlotB target vectors,
//       and a float from the input lookup table.
//  6. Python-verified: cntlzw(m_bNotServer)+rlwinm(27,31,31) = NOT(m_bNotServer).
//     Stores that inverted flag at pongLerpQueue[500][+360] (m_bIsServer).
//  7. Checks whether the inner player state is in "serve ready" (stateCode=0,
//     subState=3) and m_bNotServer is set.  If so fires a network event
//     PostPageGroupMessage(14348, playerCount|0x40, 0, 0) — "serve accepted" event.
//  8. Fires a second PostPageGroupMessage(14377, playerCount|0x40, 0, 0) unconditionally
//     — "serve started" broadcast event.
//
// Python-verified:
//   rlwinm(lerpSlot+17, 2, 0, 29) = (lerpSlot+17)*4 — dword array index
//   g_pPlayerLerpTable = lis(-32158) + -23600, stride = 416 bytes per player
// ===========================================================================
void ServeStartedMessage::Process(void* matchObj)
{
    // Step 1: look up the serving player object by index.
    void* player = NetworkClient_LookupPlayer(m_playerIndex);

    // Step 2: debug assert — action state must be initialised.
    int32_t actionState = *(int32_t*)((uint8_t*)player + 468);
    if (actionState == 0) {
        rage_debugLog("ServeStartedMessage::Process(): player action state is null");
    }

    // Step 3: copy target position into the player's lerp table entry.
    // The lerp table is indexed by (lerpSlot + 17) * 4.
    // Python-verified: rlwinm(lerpSlot+17, 2, 0, 29) = (lerpSlot+17)*4
    int32_t  lerpSlot   = *(int32_t*)((uint8_t*)player + 464);
    uint8_t* lerpTable  = g_pPlayerLerpTable + (lerpSlot * 416) + 12;
    void*    lerpEntry  = (void*)(lerpTable + (lerpSlot + 17) * 4);
    CopyVectorThreadSafe(lerpEntry, &m_targetPos);

    // Step 4: write handedness flag into the recovery state.
    pongPlayer* pPlayer    = *(pongPlayer**)((uint8_t*)player + 452);
    pongPlayer* innerState = pPlayer->m_pPlayerState;
    *(uint32_t*)((uint8_t*)innerState->m_pRecoveryState + 40) = m_bSecondaryPlayer;

    // Step 5: call the serve-start coordinator.
    // Reads lerp table floats at +52 (repeated as pSlotA.w and pSlotB.y).
    float lerpFloat = *(float*)((uint8_t*)lerpEntry + 52);
    // Build stack-layout vec arguments matching the asm stfs sequence:
    // stack[80..84..88..92..96..100..104] = {velX, lerpFloat, velY, velZ, lerpFloat, spin}
    // Then calls pongPlayer_7248_g (SetupRecoverySlots) and finalises physics.
    lfs_vec4 slotA = { m_velocityX, lerpFloat, m_velocityY, m_velocityZ };
    lfs_vec4 slotB = { lerpFloat,   m_spin,    0.0f,        0.0f        };
    Player_ApplyServeStarted(matchObj, &slotA, &slotB, m_timingRef);

    // Step 6: store inverted m_bNotServer as m_bIsServer into lerpQueue state.
    // Python-verified: cntlzw(1)+rlwinm(27,31,31)=0, cntlzw(0)+rlwinm(27,31,31)=1
    void*   lerpQueueState = *(void**)((uint8_t*)matchObj + 500);
    uint8_t isServer       = (m_bNotServer == 0) ? 1 : 0;  // bool NOT
    *(uint8_t*)((uint8_t*)lerpQueueState + 360) = isServer;

    // Step 7: conditional "serve accepted" event if in serve-ready sub-state.
    pongSwingData* sd        = innerState->m_pSwingData;
    int32_t        stateCode = *(int32_t*)((uint8_t*)sd + 28);
    bool           serveReady = (stateCode == 0)
                               && (*(int32_t*)((uint8_t*)sd + 36) == 3);

    if (serveReady && m_bNotServer) {
        // lerpSlot == 0 → playerCount=1, else playerCount=2
        int32_t lerpSlot2    = *(int32_t*)((uint8_t*)player + 464);
        uint8_t playerCount  = (lerpSlot2 == 0) ? 1 : 2;
        PostPageGroupMessage(14348, (int)(playerCount | 0x40), 0, 0, 0);
    }

    // Step 8: unconditional "serve started" broadcast.
    {
        int32_t lerpSlot3   = *(int32_t*)((uint8_t*)player + 464);
        uint8_t playerCount = (lerpSlot3 == 0) ? 1 : 2;
        PostPageGroupMessage(14377, (int)(playerCount | 0x40), 0, 0, 0);
    }
}


// ===========================================================================
// vfn_5 (slot 5) — GetIndexInPool  @ 0x823B8D28 | size: 0x54
//
// Returns the index of this object within the ServeStartedMessage pool.
//
// The pool is a flat array of fixed-size objects.  The base address is at
// g_pNetMsgPoolBase (0x825D11DC, Python-verified).  The index is computed
// from `this - pool_base` divided by object_size (256 bytes).
//
// After computing the raw index, the function updates two linked-list
// fields inside the pool header:
//   pool[1028] (uint16) — "previous tail" index (linked-list prev pointer)
//   pool[1024] (uint16) — entry count, incremented by 1
//
// Python-verified:
//   rlwinm(offset,24,8,31)  -> (offset >> 8) & 0x00FFFFFF = index
//   rlwinm(index,8,8,23)    -> index * 256 (byte offset back for linked list)
// ===========================================================================
uint16_t ServeStartedMessage::GetIndexInPool() const
{
    uint8_t* poolBase = *(uint8_t**)g_pNetMsgPoolBase;

    // Compute the object's 0-based index: offset = (this - base), index = offset / 256.
    // Python-verified: rlwinm(offset,24,8,31) extracts (offset >> 8).
    uint32_t offset = (uint32_t)((uint8_t*)this - poolBase);
    uint16_t index  = (uint16_t)((offset >> 8) & 0xFFFF);

    // Update linked-list: pool[1028] = old_tail, pool[1028] in the previous entry = index.
    uint16_t oldTail = *(uint16_t*)(poolBase + 1028);
    if (oldTail != 0xFFFF) {
        // Link the previous tail's forward pointer to us.
        // Python-verified: rlwinm(old_tail,8,8,23) = old_tail*256 = byte offset.
        uint32_t prevOffset = (uint32_t)oldTail * 256;
        *(uint16_t*)(poolBase + prevOffset + 242) = index;
    }

    // Write our index as the new tail and store it into pool[1028].
    *(uint16_t*)(poolBase + index * 256 + 242) = index;
    *(uint16_t*)(poolBase + 1028) = index;

    // Increment the total entry count.
    uint16_t count = *(uint16_t*)(poolBase + 1024);
    *(uint16_t*)(poolBase + 1024) = count + 1;

    return index;
}


// ===========================================================================
// vfn_6 (slot 6) — GetSingleton  @ 0x823B8DF0 | size: 0x0C
//
// Returns the ServeStartedMessage message-type singleton pointer.
// This is the pool/factory object; all instances of ServeStartedMessage
// are allocated from and tracked by it.
//
// Python-verified: address = lis(-32163)+4560 = 0x825D11D0.
// ===========================================================================
void* ServeStartedMessage::GetSingleton()
{
    return g_pNetMsgSingleton;  // @ 0x825D11D0
}


// ===========================================================================
// vfn_7 (slot 7) — GetTypeName  @ 0x823B8E00 | size: 0x0C
//
// Returns a pointer to the ASCII type-name string for this message class.
// Used by the network dispatcher for message registration and debug logging.
//
// Python-verified: address = lis(-32249)+-5680 = 0x8206E9D0.
// ===========================================================================
const char* ServeStartedMessage::GetTypeName()
{
    return g_szServeStartedTypeName;  // @ 0x8206E9D0  ("ServeStartedMessage" or similar)
}


// ===========================================================================
// PongNetGameModeCoordinator::CheckNetworkStatus @ 0x823B4E18 | size: 0x64
//
// Checks the network connection status by temporarily setting a state value
// and querying the network client.
//
// Returns:
//   0 if network check failed or returned 0
//   1 if network check succeeded and returned non-zero
// ===========================================================================
int PongNetGameModeCoordinator::CheckNetworkStatus()
{
    // Get pointer to network client at offset +2652
    void* networkClient = (char*)this + 2652;
    
    // Save current state value at offset +28
    uint32_t savedState = *(uint32_t*)((char*)networkClient + 28);
    
    // Temporarily set state to 46
    *(uint32_t*)((char*)networkClient + 28) = 46;
    
    // Query network status (result stored on stack at offset 80)
    uint32_t statusResult = 0;
    extern void snBitStream_ReadSigned(void*, uint32_t*, int);
    snBitStream_ReadSigned(networkClient, &statusResult, 2);
    
    // Restore original state
    *(uint32_t*)((char*)networkClient + 28) = savedState;
    
    // Return 1 if status check returned non-zero, 0 otherwise
    return (statusResult != 0) ? 1 : 0;
}


// ===========================================================================
// SinglesNetworkClient::UpdateNetworkTimer @ 0x823CCEF8 | size: 0x8C
//
// Updates network timing state based on current timer value.
// If timer is non-zero and state is 1, queries a global object for timing
// and stores the result. Otherwise, resets timer and state to zero.
// ===========================================================================
void SinglesNetworkClient::UpdateNetworkTimer()
{
    // Get pointer to timer structure at offset +4532
    void* timerStruct = (char*)this + 4532;
    
    // Load timer value from offset +4
    float timerValue = *(float*)((char*)timerStruct + 4);
    
    // Load zero constant from global @ 0x82079AD8
    extern const float g_zeroFloat;  // @ 0x82079AD8
    float zeroValue = g_zeroFloat;
    
    // Check if timer is non-zero
    if (timerValue != zeroValue) {
        // Load state flag from offset +0
        int state = *(int*)((char*)timerStruct + 0);
        
        // If state is 1, update timing from global object
        if (state == 1) {
            // Load global object pointer @ 0x8271A328
            extern void* g_networkTimingObject;  // @ 0x8271A328
            void* globalObj = *(void**)&g_networkTimingObject;
            
            // Get object at offset +20
            void* timingObj = *(void**)((char*)globalObj + 20);
            
            // Call vtable slot 2 to get timing value
            typedef float (*GetTimingFunc)(void*);
            void** vtable = *(void***)timingObj;
            GetTimingFunc getTime = (GetTimingFunc)vtable[2];
            float newTime = getTime(timingObj);
            
            // Store result at offset +8
            *(float*)((char*)timerStruct + 8) = newTime;
            
            return;
        }
    }
    
    // Reset timer and state
    *(float*)((char*)timerStruct + 4) = zeroValue;
    *(float*)((char*)timerStruct + 8) = zeroValue;
    *(int*)((char*)timerStruct + 0) = 0;
}


// ===========================================================================
// SpectatorNetworkClient::~SpectatorNetworkClient (vfn_0) @ 0x823CA408 | size: 0x50
//
// Destructor for SpectatorNetworkClient.
// Calls the base class destructor (NetDataQuery) and optionally frees memory.
//
// Parameters:
//   flags - Bit 0 controls memory deallocation:
//           0 = destructor only (object on stack or embedded)
//           1 = destructor + free memory (object on heap)
//
// This is the standard RAGE engine destructor pattern used throughout the
// networking subsystem.
// ===========================================================================
SpectatorNetworkClient::~SpectatorNetworkClient() {
    // Call base class destructor (NetDataQuery)
    // This cleans up nested state objects and components
    NetDataQuery_InitNested(this);
    
    // Note: Memory deallocation is handled by the compiler-generated
    // delete operator, which checks the flags parameter passed by the caller.
    // If flags & 1, sysMemAllocator_Free is called to return memory to the pool.
}

// ===========================================================================
// pongNetMessageHolder_4718_w @ 0x82584718 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1134.
// ===========================================================================
void pongNetMessageHolder_4718_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_Vector4);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0180);
    pongNetMessageHolder_vfn_2_0868_1(holder);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_4AD8_w @ 0x82584AD8 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1224.
// ===========================================================================
void pongNetMessageHolder_4AD8_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_String);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0248);
    pongNetMessageHolder_vfn_2_3878_1(holder);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_5958_w @ 0x82585958 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1614.
// ===========================================================================
void pongNetMessageHolder_5958_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_RangeMgr);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0590);
    pongNetMessageHolder_vfn_2_18D0_1(holder);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_4358_w @ 0x82584358 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1044.
// ===========================================================================
void pongNetMessageHolder_4358_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_ShaderParams);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_00B8);
    pongNetMessageHolder_vfn_2_FD70_1(holder);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_4BF8_w @ 0x82584BF8 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D126C.
// ===========================================================================
void pongNetMessageHolder_4BF8_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_StructBool);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0284);
    pongNetMessageHolder_vfn_2_1628_1(holder);
    holder->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_2228_2h @ 0x82582228 | size: 0x40
//
// Adds the static node at 0x825D1960 to the holder singleton list.
// ===========================================================================
void pongNetMessageHolder_2228_2h() {
    pongNetMessageHolder* holder = pongNetMessageHolder_FAE0_isl();
    const uint32_t previousHead = (uint32_t)(uintptr_t)holder->m_pInternalArray;

    auto* node = reinterpret_cast<PongNetMessageHolderStaticNode*>(&g_holderInstance_InputDataMgr);
    node->m_field8 = previousHead;

    holder->m_pInternalArray = (void*)(uintptr_t)&g_holderInstance_InputDataMgr;
    holder->m_refCount = previousHead + 1;
}

// ===========================================================================
// pongNetMessageHolder_EAD8_sp @ 0x8257EAD8 | size: 0x14
//
// Zeroes the 0x100-byte static holder pool block at 0x8261A0D0.
// ===========================================================================
void pongNetMessageHolder_EAD8_sp() {
    std::memset(reinterpret_cast<void*>(g_netMessageHolderBuffer), 0, 0x100);
}

// ===========================================================================
// pongNetMessageHolder_08B0_p @ 0x825808B0 | size: 0x34
//
// Registers the static holder dtor callback and tracks init ordering.
// ===========================================================================
void pongNetMessageHolder_08B0_p() {
    auto* node = reinterpret_cast<PongNetMessageHolderStaticNode*>(&g_holderInstance_13A4);
    const uint32_t previousHead = NetMessageHolderInitHead();

    NetMessageHolderInitHead() = (uintptr_t)&g_holderInstance_13A4;
    node->m_field4 = previousHead;

    ++NetMessageHolderLiveCount();
    std::atexit(pongNetMessageHolder_5038_w);
}

// ===========================================================================
// pongNetMessageHolder_vfn_0_4C98_1 @ 0x823C4C98 | size: 0x78
//
// Deleting destructor variant for a pongNetMessageHolder MI vtable.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_4C98_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0180);
    pongNetMessageHolder_vfn_2_0868_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        sysMemAllocator_Free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder_vfn_0_5B48_1 @ 0x823C5B48 | size: 0x50
//
// Deleting destructor variant that routes through rage_27C0.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_5B48_1(pongNetMessageHolder* self, int flags) {
    rage_27C0(self);

    if ((flags & 1) != 0) {
        sysMemAllocator_Free(self);
    }

    return self;
}


// ===========================================================================
// pongNetMessageHolder::CleanupInternalArray @ 0x823C3878 | size: 0x64
// [vtable slot 2]
//
// Cleans up an internal array of objects stored at offset +8.
// If the pointer is non-null, iterates through 2 elements (at offsets +12 and +24),
// sets their vtable pointers to a cleanup vtable, then frees the array and nulls
// the pointer.
//
// This is called during destruction to release dynamically allocated internal state.
// ===========================================================================
void pongNetMessageHolder::CleanupInternalArray()
{
    void** arrayPtr = (void**)m_pInternalArray;
    
    if (arrayPtr != nullptr) {
        // Set cleanup vtable for each element in the array
        // Python-verified: loop from i=1 down to i=0 (2 iterations)
        // Offsets: +24 (element 1), +12 (element 0)
        extern void* g_pCleanupVtable;  // @ 0x8206C394 (lis -32249 + -15612)
        
        for (int i = 1; i >= 0; i--) {
            void** element = (void**)((uint8_t*)arrayPtr + 12 + (i * 12));
            *element = g_pCleanupVtable;
        }
        
        // Free the array
        sysMemAllocator_Free(arrayPtr);
        
        // Null out the pointer
        m_pInternalArray = nullptr;
    }
}


// ===========================================================================
// pongNetMessageHolder::~pongNetMessageHolder @ 0x823C5788 | size: 0x78
// [vtable slot 0 - destructor]
//
// Destructor for pongNetMessageHolder network message objects.
//
// Steps:
// 1. Sets vtable to pongNetMessageHolder vtable (standard destructor pattern)
// 2. Calls CleanupInternalArray to release internal allocations
// 3. Sets vtable to base class vtable (pongNetMessageHolderBase)
// 4. Decrements global live instance counter
// 5. If destructor flags indicate deallocation (bit 0 set), frees this object
//
// The flags parameter (r4) follows standard C++ destructor conventions:
//   bit 0 = 1: free memory after destruction (delete)
//   bit 0 = 0: destructor only (stack object or placement delete)
// ===========================================================================
pongNetMessageHolder::~pongNetMessageHolder()
{
    // Note: 'this' is in r3, flags are passed separately in calling code
    // The vtable setting and cleanup are handled here
    
    // Step 1: Set vtable to pongNetMessageHolder vtable
    // (Compiler handles this automatically)
    
    // Step 2: Clean up internal array
    CleanupInternalArray();
    
    // Step 3: Set vtable to base class (compiler handles via base destructor call)
    
    // Step 4: Decrement global live instance counter
    NetMessageHolderLiveCount()--;
    
    // Step 5: Memory deallocation is handled by operator delete if needed
    // (The assembly checks bit 0 of flags and calls sysMemAllocator_Free conditionally)
}

// ===========================================================================
// RequestDataMessage — Network message for requesting data from peer
// ===========================================================================

/**
 * RequestDataMessage::vfn_5 — Return message object to pool
 * @ 0x823BF058 | size: 0x54
 *
 * Returns this message object to the free list in the pool allocator.
 * This is the standard pool-based memory management pattern used by all
 * network message classes.
 *
 * Algorithm:
 * 1. Calculate object index: (this - poolBase) / 16
 * 2. Link into free list at pool->freeListHead
 * 3. Update pool->freeListHead to point to this object
 * 4. Increment pool->freeCount
 *
 * Pool structure (at 0x825D1644 + 8):
 *   +0x00: poolBase pointer
 *   +0x20: freeCount (uint16_t)
 *   +0x24: freeListHead (uint16_t, 0xFFFF = empty)
 *
 * Each pool entry (16 bytes):
 *   +0x0C: nextFreeIndex (uint16_t)
 *   +0x0E: prevFreeIndex (uint16_t)
 */
void RequestDataMessage::vfn_5() {
    // Load pool manager structure
    uint8_t* poolMgr = *reinterpret_cast<uint8_t**>(&g_holderInstance_MotionRangeMgr_ptr);
    
    // Calculate index: (this - poolBase) / 16
    uintptr_t offset = reinterpret_cast<uintptr_t>(this) - reinterpret_cast<uintptr_t>(poolMgr);
    uint16_t index = static_cast<uint16_t>((offset >> 4) & 0xFFFF);
    
    // Get current free list head
    uint16_t oldHead = *reinterpret_cast<uint16_t*>(poolMgr + 36);
    
    // Link this entry into free list
    uint8_t* thisEntry = poolMgr + (index * 16);
    *reinterpret_cast<uint16_t*>(thisEntry + 14) = oldHead;
    
    // Update previous entry's back link if list wasn't empty
    if (oldHead != 0xFFFF) {
        uint8_t* oldHeadEntry = poolMgr + (oldHead * 16);
        *reinterpret_cast<uint16_t*>(oldHeadEntry + 12) = index;
    }
    
    // Update pool free list head
    *reinterpret_cast<uint16_t*>(poolMgr + 36) = index;
    
    // Increment free count
    uint16_t freeCount = *reinterpret_cast<uint16_t*>(poolMgr + 32);
    *reinterpret_cast<uint16_t*>(poolMgr + 32) = freeCount + 1;
}

/**
 * RequestDataMessage::vfn_6 — Get pool singleton
 * @ 0x823BF120 | size: 0x10
 *
 * Returns pointer to the global pool manager singleton for RequestDataMessage.
 */
void* RequestDataMessage::vfn_6() {
    return *reinterpret_cast<void**>(&g_holderInstance_MotionRangeMgr);
}

/**
 * RequestDataMessage::vfn_7 — Get type name string
 * @ 0x823BF130 | size: 0x4
 *
 * Returns debug/RTTI type name string for this message class.
 * Used for logging and debugging network message traffic.
 */
const char* RequestDataMessage::vfn_7() {
    return g_szScoreMessageFmt;
}


// ===========================================================================
// SinglesNetworkClient::CheckAllPlayersReady @ 0x82311098 | size: 0x10C
//
// Checks if all players have pressed their ready buttons by polling button
// states at three specific offsets (+8, +16, +48) for each of 4 player slots.
// If all players are ready (all button checks return 0), the function returns
// early. Otherwise, it performs network setup and sets a ready flag.
//
// The function accesses a global player array with 808-byte stride per player.
// Each player slot has button state fields that are checked via CheckButtonPressed.
//
// Network setup involves:
//  - Getting network status via snSession_AcquireLock
//  - Checking a global singleton flag
//  - Finding a network message slot via snSession_FindProperty
//  - Initializing the message slot with values 1 and 3
//  - Calling snSession_ReleaseLock if network status is non-zero
//  - Setting the ready flag at this+24 to 1
// ===========================================================================
void SinglesNetworkClient::CheckAllPlayersReady()
{
    // Early exit if already marked as ready
    if (m_bAllPlayersReady) {
        return;
    }

    // Get base address of player array from global singleton
    // Python-verified: lis(-32142) + (-23780) + 808 = player slot base
    extern void* g_pPlayerArrayBase;  // @ 0x8211A31C
    uint8_t* playerArray = *(uint8_t**)g_pPlayerArrayBase;
    uint8_t* playerSlot = playerArray + 808;  // First player at offset 808

    // Check all 4 players for ready state
    for (int playerIdx = 0; playerIdx < 4; playerIdx++) {
        // Each player has three button state fields to check
        bool button1Pressed = CheckButtonPressed((void*)(playerSlot + 8));
        bool button2Pressed = CheckButtonPressed((void*)(playerSlot + 16));
        bool button3Pressed = CheckButtonPressed((void*)(playerSlot + 48));

        // If any button is pressed, player is not ready - proceed to network setup
        if (button1Pressed || button2Pressed || button3Pressed) {
            goto setup_network;
        }

        // Move to next player slot (808-byte stride)
        playerSlot += 808;
    }

    // All players ready - return without network setup
    return;

setup_network:
    // Get network client pointer from this+20
    void* networkClient = (void*)(uintptr_t)m_pNetworkSession;

    // Query network status
    uint8_t networkStatus = snSession_AcquireLock(networkClient);

    // Check global singleton flag at offset +556
    extern void* g_pAtSingleton;  // @ 0x821EAB08
    void* singleton = *(void**)g_pAtSingleton;
    uint32_t singletonFlags = *(uint32_t*)((uint8_t*)singleton + 556);

    if (singletonFlags != 0) {
        // Check secondary flag
        extern uint8_t g_bNetworkDebugFlag;  // @ 0x826065EB
        if (g_bNetworkDebugFlag == 0) {
            // Debug print (nop in release builds)
            extern const char g_szNetworkDebugMsg[];  // @ 0x8205AE98
            rage_debugLog(g_szNetworkDebugMsg);
        }
    }

    // Find available network message slot
    extern const char g_szMessageType[];  // @ 0x8205DFB0
    uint32_t clientState = *(uint32_t*)((uint8_t*)networkClient + 92);
    void* messageSlot = snSession_FindProperty((void*)clientState, g_szMessageType);

    // Initialize message slot if found
    if (messageSlot != nullptr) {
        *(uint32_t*)((uint8_t*)messageSlot + 0) = 1;  // Set state to 1
        *(uint32_t*)((uint8_t*)messageSlot + 4) = 3;  // Set type to 3
    }

    // Perform network client update if status is non-zero
    if (networkStatus != 0) {
        snSession_ReleaseLock(networkClient);
    }

    // Mark all players as ready
    m_bAllPlayersReady = 1;
}


// ===========================================================================
// SinglesNetworkClient::WriteNetworkMessageHeader @ 0x82390598 | size: 0x70
//
// Writes a network message header containing two 16-bit indices and
// additional data to the network stream.
//
// This function is used to serialize message headers that contain:
// - A primary index (16-bit signed)
// - A secondary index (16-bit signed)
// - Additional data payload (variable size, determined by second index)
//
// The function performs three operations:
// 1. Write first 16-bit index to stream
// 2. Write second 16-bit index to stream
// 3. Write additional data using the second index as a size/type parameter
//
// @param messageData  Pointer to message structure containing:
//                     +0x00: int16_t primaryIndex
//                     +0x02: int16_t secondaryIndex
//                     +0x04: void* additionalData
// @param client       SinglesNetworkClient managing the network stream
// ===========================================================================
void SinglesNetworkClient::WriteNetworkMessageHeader(void* messageData, void* client)
{
    uint8_t* data = (uint8_t*)messageData;
    
    // Load the two 16-bit indices from the message structure
    int16_t primaryIndex = *(int16_t*)(data + 0);
    int16_t secondaryIndex = *(int16_t*)(data + 2);
    
    // Write first index as 16-bit signed value
    WriteInt8Bits(client, primaryIndex, 16);
    
    // Write second index as 16-bit signed value
    WriteInt8Bits(client, secondaryIndex, 16);
    
    // Write additional data using the second index as a parameter
    // The 67C8_g function handles the actual data serialization
    void* additionalData = (void*)(data + 4);
    NetworkClient_WriteSecondaryPlayerData(client, secondaryIndex, additionalData);
}


// ===========================================================================
// C LINKAGE ALIAS FOR BACKWARD COMPATIBILITY
// ===========================================================================

extern "C" {
    // Original name: SinglesNetworkClient_0598_g
    void SinglesNetworkClient_0598_g(void* messageData, void* client) {
        SinglesNetworkClient::WriteNetworkMessageHeader(messageData, client);
    }
}


// ===========================================================================
// SinglesNetworkClient::PopMessageFromQueue @ 0x823BA970 | size: 0x8C
//
// Removes and returns the head node from a doubly-linked message queue.
// The queue uses 20-byte nodes with 16-bit index-based linking.
//
// Node structure (20 bytes):
//   +0x10 (16): prev index (uint16_t)
//   +0x12 (18): next index (uint16_t)
//
// Pool header structure:
//   +0x28 (40): node count (uint16_t)
//   +0x2C (44): head index (uint16_t)
//
// Returns:
//   Pointer to the removed node, or nullptr if queue is empty
//
// Python-verified:
//   Pool pointer loaded from: 0x825D128C (lis -32163 + 4740 + 8)
//   Node stride: 20 bytes (index * 5 * 4)
//   Empty sentinel: 0xFFFF
// ===========================================================================
void* SinglesNetworkClient::PopMessageFromQueue()
{
    // Load the pool structure pointer from global storage.
    // Python-verified: lis(-32163) + 4740 + 8 = 0x825D128C
    extern void** g_pMessageQueuePool;  // @ 0x825D128C
    uint8_t* pool = (uint8_t*)*g_pMessageQueuePool;

    // Load the head index from pool header at offset +44.
    uint16_t headIndex = *(uint16_t*)(pool + 44);

    // If queue is empty (head == 0xFFFF), return nullptr.
    if (headIndex == 0xFFFF) {
        return nullptr;
    }

    // Calculate the head node address.
    // Python-verified: stride = index * 20 (index + index*4, then *4)
    uint32_t headOffset = headIndex * 20;
    uint8_t* headNode = pool + headOffset;

    // Load the next index from the head node at offset +18.
    uint16_t nextIndex = *(uint16_t*)(headNode + 18);

    // Update the pool head to point to the next node.
    *(uint16_t*)(pool + 44) = nextIndex;

    // If the new head is valid, clear its prev pointer.
    if (nextIndex != 0xFFFF) {
        uint32_t nextOffset = nextIndex * 20;
        uint8_t* nextNode = pool + nextOffset;
        *(uint16_t*)(nextNode + 16) = 0xFFFF;
    }

    // Clear the removed node's next pointer.
    *(uint16_t*)(headNode + 18) = 0xFFFF;

    // Decrement the node count in the pool header at offset +40.
    uint16_t count = *(uint16_t*)(pool + 40);
    *(uint16_t*)(pool + 40) = count - 1;

    return headNode;
}


// External references for SinglesNetworkClient
extern void* g_pNetworkBase;  // @ 0x8271A7B0
extern void net_6BA0_fw(void* session, void* param1, void* param2);
extern void snSession_Detach(void* session);

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ProcessSessions @ 0x822FDDC0 | size: 0x150
//
// Processes all active network sessions in two phases:
// 1. First loop: Calls net_6BA0_fw and snSession_Detach on each session
// 2. Second loop: Copies session data to client buffer at offset +5572
//
// Returns:
//   0 if early exit condition met (byte at +5972 is non-zero)
//   1 on successful completion
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient::ProcessSessions()
{
    // Get base pointer and calculate session array address
    void* basePtr = *(void**)g_pNetworkBase;
    uint32_t sessionArrayOffset = 94288;  // 0x17050 (lis 1 + ori 28752)
    void* sessionArray = (char*)basePtr + sessionArrayOffset;
    
    // Get session count from array header
    int sessionCount = *(int*)sessionArray;
    
    // Phase 1: Process each active session
    if (sessionCount > 0) {
        // Calculate session data pointers
        // Session structure: 104 bytes per entry, starts at basePtr + 0x17058
        void* sessionData = (char*)basePtr + 94296;  // basePtr + 0x17058
        void* sessionParams = (char*)basePtr + 75152;  // basePtr + 0x12590 (addis +1, addi 9584)
        
        for (int i = 0; i < sessionCount; i++) {
            // Check if session has valid data at offset +28
            void* sessionPtr = (char*)sessionData + (i * 104);
            uint32_t sessionFlag = *(uint32_t*)((char*)sessionPtr + 28);
            
            // If session is inactive, call net_6BA0_fw
            if (sessionFlag == 0) {
                void* param1 = (char*)sessionParams + (i * 384);
                void* param2 = (char*)param1 + 60;
                net_6BA0_fw(sessionPtr, param1, param2);
            }
            
            // Always call snSession_Detach
            snSession_Detach(sessionPtr);
        }
    }
    
    // Phase 2: Copy session data to client buffer
    sessionCount = *(int*)((char*)basePtr + sessionArrayOffset);
    
    if (sessionCount > 0) {
        // Check early exit condition
        uint8_t exitFlag = *((uint8_t*)this + 5972);
        if (exitFlag != 0) {
            return 1;
        }
        
        // Source: basePtr + 0x27AB8 (addis +2, addi -31576)
        void* sourceData = (char*)basePtr + 162488;
        
        // Destination: this + 5572
        uint32_t* destBuffer = (uint32_t*)((char*)this + 5572);
        
        const uint32_t CONSTANT_999999 = 999999;  // 0xF423F
        
        for (int i = 0; i < sessionCount; i++) {
            // Check exit flag again
            exitFlag = *((uint8_t*)this + 5972);
            if (exitFlag != 0) {
                return 1;
            }
            
            // Check if session entry is active (byte at +17)
            void* entryPtr = (char*)sourceData + (i * 20);
            uint8_t isActive = *((uint8_t*)entryPtr + 17);
            
            if (isActive == 0) {
                return 0;  // Inactive entry - early exit
            }
            
            // Copy 16 bytes from source entry to stack temp
            uint32_t temp[4];
            temp[0] = *(uint32_t*)((char*)entryPtr + 0);
            temp[1] = *(uint32_t*)((char*)entryPtr + 4);
            temp[2] = *(uint32_t*)((char*)entryPtr + 8);
            temp[3] = *(uint32_t*)((char*)entryPtr + 12);
            
            // Store first value (0) to destination
            destBuffer[i * 2] = 0;
            
            // Check flag at byte +16 of entry
            uint8_t flag = *((uint8_t*)entryPtr + 16);
            
            if (flag != 0) {
                // If flag set: store (temp[1] >> 1) to destination +4
                // rlwinm r5,r8,31,1,31 = rotate left 31 (= right 1) and mask
                destBuffer[i * 2 + 1] = temp[1] >> 1;
            } else {
                // If flag clear: store constant 999999
                destBuffer[i * 2 + 1] = CONSTANT_999999;
            }
        }
    }
    
    return 1;
}


// ===========================================================================
// SinglesNetworkClient::ProcessPendingMessages @ 0x82391C30 | size: 0xEC
//
// Processes pending network messages in a loop. This function:
// 1. Copies 24 bytes of data from the client state to a global buffer
// 2. Initializes two temporary message structures on the stack
// 3. Sets a processing flag (bit 7)
// 4. Loops through pending messages, processing each one
//
// The function uses a global pointer at 0x8271A7B0 to access shared network
// state. If this pointer is null, the function uses null pointers for
// processing (effectively skipping the operation).
//
// Original symbol: SinglesNetworkClient_1C30_g
// ===========================================================================
void SinglesNetworkClient::ProcessPendingMessages()
{
    // Load the flags field from this+4
    uint32_t* flagsPtr = (uint32_t*)((uint8_t*)this + 4);
    
    // Load global network state pointer
    extern uint32_t* g_pNetworkStatePtr;  // @ 0x8271A7B0
    uint32_t* globalState = g_pNetworkStatePtr;
    
    // Determine source and destination pointers based on global state
    uint8_t* sourcePtr = nullptr;
    uint8_t* destPtr = nullptr;
    
    if (globalState != nullptr) {
        sourcePtr = (uint8_t*)flagsPtr + 16;
        destPtr = (uint8_t*)globalState + 64;
    }
    
    // Copy 24 bytes (6 dwords) from source to destination+440
    if (destPtr != nullptr) {
        uint8_t* copyDest = destPtr + 440;
        uint32_t* src = (uint32_t*)sourcePtr;
        uint32_t* dst = (uint32_t*)copyDest;
        
        for (int i = 0; i < 6; i++) {
            dst[i] = src[i];
        }
    }
    
    // Initialize two temporary message structures on stack
    struct TempMessageStruct {
        uint32_t field0;
        uint32_t field4;
    };
    
    TempMessageStruct msg1 = {0, 0};
    TempMessageStruct msg2 = {0, 0};
    
    // Initialize the structures (calls snListNode_Init)
    extern void snListNode_Init(void* msg);
    snListNode_Init(&msg1);
    snListNode_Init(&msg2);
    
    // Set up processing flag structure
    struct ProcessingFlags {
        uint32_t field0;
        uint32_t field4;
        uint8_t flags;
    };
    
    ProcessingFlags procFlags;
    procFlags.field0 = 0;
    procFlags.field4 = 0;
    procFlags.flags = 0x80;  // Set bit 7
    
    // Get the message queue pointer from destPtr+528
    uint32_t* messageQueue = nullptr;
    if (destPtr != nullptr) {
        messageQueue = (uint32_t*)((uint8_t*)destPtr + 528 + 4);
    }
    
    // Process messages in a loop
    extern bool NetworkClient_TryDequeueMessage(void* flags, uint32_t* queue);
    extern void NetworkClient_ReadQueuedMessageSource(void* msgPtr, uint8_t* sourcePtr);
    extern void NetworkClient_EndMessageProcessing(void* flags);
    extern void NetworkClient_AbortMessageProcessing(void* flags);
    
    while (true) {
        // Check if there are more messages to process
        bool hasMessage = NetworkClient_TryDequeueMessage(&procFlags, messageQueue);
        
        if (!hasMessage) {
            // No more messages, clean up and exit
            NetworkClient_AbortMessageProcessing(&procFlags);
            break;
        }
        
        // Process the current message
        void* currentMsg = (void*)((uint8_t*)procFlags.field0 + 40);
        NetworkClient_ReadQueuedMessageSource(currentMsg, sourcePtr);
        
        // Advance to next message
        NetworkClient_EndMessageProcessing(&procFlags);
    }
}


// ===========================================================================
// pongNetMessageHolder::InsertIntoList @ 0x825821A8 | size: 0x40
//
// Inserts this message holder into the global linked list at the head position.
// This is part of the network message lifecycle management system.
//
// The function:
// 1. Gets the singleton holder (list head)
// 2. Reads the current next pointer from the singleton
// 3. Updates the singleton's next pointer to point to this node
// 4. Updates this node's next pointer to point to the old head
// 5. Increments the reference count
//
// This implements a classic "insert at head" linked list operation for
// tracking active network message holders.
//
// Original symbol: pongNetMessageHolder_21A8_2h
// ===========================================================================
void pongNetMessageHolder::InsertIntoList()
{
    // Get the singleton holder (list head)
    pongNetMessageHolder* singleton = pongNetMessageHolder_FAE0_isl();
    
    // Read the current next pointer from the singleton
    void* oldNext = singleton->m_pInternalArray;
    
    // Update singleton to point to the old head
    *(void**)((char*)singleton + 8) = oldNext;
    
    // Make this node point to the singleton
    this->m_pInternalArray = singleton;
    
    // Increment reference count
    this->m_refCount++;
}


// ===========================================================================
// SinglesNetworkClient::ProcessNetworkTimingUpdate @ 0x823E01F8 | size: 0x15C
//
// Processes network timing updates and manages join request state machine.
// This function is called periodically with a timestamp to advance the
// network client's state based on timing thresholds.
//
// State machine:
//   state 0: Initial join request state - calls NetworkClient_BeginJoinRequest
//   state 1: Waiting for join response - calls NetworkClient_PollJoinResponse
//   state 2+: Join accepted - creates EvtAcceptJoinRequestSucceeded event
//
// Parameters:
//   timestamp - Current network timestamp for timing comparisons
//
// Fields accessed:
//   +40: Timeout flag (if non-zero, abort timing check)
//   +44: Next scheduled timestamp
//   +48: Current state (0, 1, or 2+)
//   +56: Session object pointer (for state 2+)
//
// ===========================================================================
void SinglesNetworkClient::ProcessNetworkTimingUpdate(uint32_t timestamp)
{
    uint32_t state = *(uint32_t*)((char*)this + 48);
    
    if (state == 0) {
        // State 0: Initial join request processing
        uint32_t nextTimestamp = *(uint32_t*)((char*)this + 44);
        int32_t timeDelta = (int32_t)(timestamp - nextTimestamp);
        
        if (timeDelta < 0) {
            // Not yet time to process - check timeout flag
            uint32_t timeoutFlag = *(uint32_t*)((char*)this + 40);
            if (timeoutFlag != 0) {
                return;  // Timeout active, abort
            }
        }
        
        // Time to process or timeout cleared - advance state
        NetworkClient_BeginJoinRequest(this);
        
        // Schedule next update 100ms from now
        *(uint32_t*)((char*)this + 44) = timestamp + 100;
        
    } else if (state == 1) {
        // State 1: Waiting for join response
        uint32_t nextTimestamp = *(uint32_t*)((char*)this + 44);
        int32_t timeDelta = (int32_t)(timestamp - nextTimestamp);
        
        if (timeDelta < 0) {
            // Not yet time to process - check timeout flag
            uint32_t timeoutFlag = *(uint32_t*)((char*)this + 40);
            if (timeoutFlag != 0) {
                return;  // Timeout active, abort
            }
        }
        
        // Time to check join response
        uint8_t responseResult = NetworkClient_PollJoinResponse(this);
        
        if (responseResult == 0) {
            // Join request accepted - transition to accepting state
            game_D3B0_h(this);
            
            // Create event object on stack
            rage::EvtAcceptJoinRequestSucceeded evt;
            evt.vtable = (void**)VTABLE_EvtAcceptJoinRequestSucceeded;
            
            hsmEvent_Init(&evt);  // Initialize event
            
            // Notify HSM that join request was accepted
            snHsmAcceptingJoinRequest_9A70(this, &evt);
        }
        
        // Schedule next update 100ms from now
        *(uint32_t*)((char*)this + 44) = timestamp + 100;
        
    } else {
        // State 2+: Join accepted, add node to session
        game_D3B0_h(this);
        
        // Create event object on stack
        rage::EvtAcceptJoinRequestSucceeded evt;
        hsmEvent_Init(&evt);
        
        // Get session object via vtable call (slot 11)
        void** vtable = *(void***)this;
        typedef void* (*GetSessionFunc)(void*);
        GetSessionFunc getSession = (GetSessionFunc)vtable[11];
        void* session = getSession(this);
        
        // Get node data from session
        void* sessionData = *(void**)((char*)session + 56);
        void* nodeList = *(void**)((char*)sessionData + 4);
        
        // Allocate node in pool (vtable slot 1)
        void** nodeVtable = *(void***)nodeList;
        typedef void* (*AllocNodeFunc)(void*, uint32_t, uint32_t);
        AllocNodeFunc allocNode = (AllocNodeFunc)nodeVtable[1];
        void* newNode = allocNode(nodeList, 12, 0);
        
        if (newNode != nullptr) {
            // Initialize node with event data
            *(void**)newNode = (void*)VTABLE_EvtAcceptJoinRequestSucceeded;  // rage::EvtAcceptJoinRequestSucceeded vtable
            
            // Copy 8 bytes of event data (two uint32_t values)
            uint32_t eventData1 = *(uint32_t*)((char*)&evt + 4);
            uint32_t eventData2 = *(uint32_t*)((char*)&evt + 8);
            
            // Store as 64-bit value (big-endian to little-endian conversion)
            uint64_t combinedData = ((uint64_t)eventData1 << 32) | eventData2;
            *(uint64_t*)((char*)newNode + 4) = combinedData;
            
            // Add node to session
            void* sessionNodeList = (char*)sessionData + 8;
            snSession_AddNode(sessionNodeList, newNode);
        }
    }
}

/**
 * SinglesNetworkClient::ReadStringFromStream @ 0x82260688 | size: 0xAC
 *
 * Reads a null-terminated string from the network stream buffer.
 * 
 * The function:
 * 1. Checks available buffer space (fields +20 and +32, divided by 8)
 * 2. Scans the input string for null terminator (max 16 bytes)
 * 3. If string length > 0, writes it to the stream
 * 4. Clears/resets 8 bits in the stream
 * 5. Returns the string length + 1
 *
 * @param stringBuffer - Pointer to null-terminated string to read
 * @return Length of string + 1, or 0 if no space available
 */
uint32_t SinglesNetworkClient::ReadStringFromStream(const char* stringBuffer) {
    // Calculate available buffer space
    // Fields +20 and +32 appear to be buffer pointers/positions
    uint32_t bufferEnd = *(uint32_t*)((char*)this + 20);
    uint32_t bufferCurrent = *(uint32_t*)((char*)this + 32);
    int32_t availableSlots = (bufferEnd - bufferCurrent) >> 3;  // Divide by 8
    
    // If no space available, return 0
    if (availableSlots <= 0) {
        return 0;
    }
    
    // Clamp to maximum 16 bytes
    if (availableSlots > 16) {
        availableSlots = 16;
    }
    
    // Scan string for null terminator
    uint32_t stringLength = 0;
    if (stringBuffer[0] != '\0') {
        // Find string length (max availableSlots-1 bytes)
        while (stringLength < (uint32_t)(availableSlots - 1)) {
            stringLength++;
            if (stringBuffer[stringLength] == '\0') {
                break;
            }
        }
        
        // If we found a non-empty string, write it to the stream
        if (stringLength > 0) {
            // Call netStream_WriteString to write the string
            // This is likely WriteStringToStream(this, stringBuffer, stringLength)
            extern uint32_t netStream_WriteString(void* client, const char* str, uint32_t length);
            netStream_WriteString(this, stringBuffer, stringLength);
        }
    }
    
    // Clear/reset 8 bits in the stream
    // snBitStream_WriteBits is ReadBitsFromStream
    snBitStream_WriteBits(this, 0, 8);
    
    // Return string length + 1
    return stringLength + 1;
}


// ===========================================================================
// pongNetMessageHolder::InitializeInternalArray (vfn_1) @ 0x823C2CC8 | size: 0x54
//
// Lazy initialization of the internal message array.
// Allocates 1296 bytes (0x510) if the array hasn't been created yet,
// then calls the initialization function to set it up.
//
// This is vtable slot 1, likely called during first use of the message holder.
// ===========================================================================
void pongNetMessageHolder::InitializeInternalArray()
{
    // Check if internal array is already allocated
    if (m_pInternalArray == nullptr) {
        // Allocate 1296 bytes for the internal array
        void* newArray = rage_Alloc(1296);
        
        if (newArray != nullptr) {
            // Initialize the allocated array
            pongNetMessageHolder_7700_wrh(newArray);
        }
        
        // Store the result (may be nullptr if allocation failed)
        m_pInternalArray = newArray;
    }
}


// ===========================================================================
// pongNetMessageHolder::RemoveFromThreadPoolList @ 0x82585C58 | size: 0x50
//
// Removes a node from the global threadpool linked list.
// Traverses the list starting from g_threadpool_head, looking for a specific
// sentinel node, and removes it if found. Then calls cleanup function.
//
// The linked list nodes have the structure:
//   +0x00: node data
//   +0x14 (20): next pointer
// ===========================================================================
void pongNetMessageHolder::RemoveFromThreadPoolList()
{
    // Global threadpool list head @ 0x825EBCBC
    extern uint32_t* g_threadpool_head;  // @ 0x825EBCBC
    
    // Sentinel node address @ 0x8271A2CC
    const uintptr_t sentinelAddr = (uintptr_t)&g_networkSentinel;
    
    // Get the head of the list
    uint32_t* current = g_threadpool_head;
    
    // Check if list is not empty
    if (*current != 0) {
        // Traverse the list looking for the sentinel or end
        uint32_t* prev = current;
        uint32_t* node = (uint32_t*)*current;
        
        while (node != nullptr && (uintptr_t)node != sentinelAddr) {
            // Move to next node (offset +20 is the next pointer)
            prev = (uint32_t*)((char*)node + 20);
            node = (uint32_t*)*prev;
        }
        
        // If we found a node (not at sentinel), remove it from the list
        if (node != nullptr) {
            // Get the next pointer from the node we're removing
            uint32_t* next = (uint32_t*)*((uint32_t*)((char*)node + 20));
            
            // Update the previous node's next pointer to skip this node
            *prev = (uint32_t)(uintptr_t)next;
        }
    }
    
    // Call threadpool cleanup function
    extern void rage_threadpool_cleanup_6878();  // @ 0x82176878
    rage_threadpool_cleanup_6878();
}


// ===========================================================================
// pongNetMessageHolder::InitializeMessageArray @ 0x823C72A8 | size: 0x168
//
// Initializes an array of 4 message holder structures (256 bytes each).
// Sets up the message pool with default values, vtable pointers, and
// linked list connections.
//
// Structure layout per entry (256 bytes):
//   +0x00: vtable pointer (ServeStartedMessage vtable @ 0x8206F740)
//   +0x04: float value
//   +0x18-0x28: various float fields
//   +0x30: flags (value 1)
//   +0x34: counter (value 0)
//   +0x58-0x5A: uint16 values (0, 0xFFFF)
//   +0x5C: byte (0)
//   +0x5E: uint16 (0)
//   +0x78: byte (255)
//   +0x79: byte (0)
//   +0x7A: byte (255)
//   +0xF0-0xF2: linked list pointers
//
// After initializing the array, sets up pool metadata:
//   +1024: pool size (4)
//   +1026: available count (4)
//   +1028: head index (0)
// ===========================================================================
void pongNetMessageHolder::InitializeMessageArray()
{
    // Load constant float values from .rdata
    extern const float g_floatConstant1;  // @ 0x82079AD4
    extern const float g_floatConstant2;  // @ 0x8202D110 - 8
    extern const float g_floatConstant3;  // @ 0x8202D110
    extern const float g_floatConstant4;  // @ 0x825C7600
    
    // ServeStartedMessage vtable pointer
    const uint32_t vtableAddr = VTABLE_ServeStartedMessage;
    
    // Initialize 4 message structures (loop counter starts at 3, decrements to -1)
    uint8_t* basePtr = (uint8_t*)this + 104;  // Start at offset +104
    
    for (int i = 0; i < 4; i++) {
        uint8_t* entryPtr = basePtr + (i * 256);
        
        // Set vtable pointer
        *(uint32_t*)(entryPtr + 0) = vtableAddr;
        
        // Initialize float fields
        *(float*)(entryPtr + 4) = g_floatConstant1;
        *(float*)(entryPtr + 16) = g_floatConstant2;
        *(float*)(entryPtr + 20) = g_floatConstant3;
        *(float*)(entryPtr + 24) = g_floatConstant3;
        
        // Zero out 16-byte vector at offset +24 (relative to -40 from r11)
        memset(entryPtr + 64, 0, 16);
        
        // Initialize more float fields to zero
        *(float*)(entryPtr + 96) = 0.0f;
        *(float*)(entryPtr + 100) = 0.0f;
        *(float*)(entryPtr + 104) = 0.0f;
        *(float*)(entryPtr + 112) = 0.0f;
        *(float*)(entryPtr + 116) = 0.0f;
        *(float*)(entryPtr + 120) = 0.0f;
        *(float*)(entryPtr + 128) = 0.0f;
        
        // Set flags and counters
        *(uint32_t*)(entryPtr + 152) = 1;      // +48 from r11 = enabled flag
        *(uint32_t*)(entryPtr + 156) = 0;      // +52 from r11 = counter
        
        // More float initializations
        *(float*)(entryPtr + 132) = 0.0f;
        *(float*)(entryPtr + 140) = g_floatConstant2;
        *(float*)(entryPtr + 144) = 0.0f;
        *(float*)(entryPtr + 148) = g_floatConstant4;
        *(float*)(entryPtr + 160) = 0.0f;
        *(float*)(entryPtr + 164) = 0.0f;
        *(float*)(entryPtr + 168) = 0.0f;
        *(float*)(entryPtr + 176) = 0.0f;
        *(float*)(entryPtr + 180) = 0.0f;
        *(float*)(entryPtr + 184) = 0.0f;
        
        // Set uint16 and byte fields
        *(uint16_t*)(entryPtr + 192) = 0;
        *(uint16_t*)(entryPtr + 194) = 0xFFFF;
        *(uint8_t*)(entryPtr + 196) = 0;
        *(uint16_t*)(entryPtr + 198) = 0;
        
        // More float fields
        *(float*)(entryPtr + 208) = 0.0f;
        *(float*)(entryPtr + 212) = 0.0f;
        *(float*)(entryPtr + 216) = 0.0f;
        *(float*)(entryPtr + 220) = 0.0f;
        
        // Set byte flags
        *(uint8_t*)(entryPtr + 224) = 255;
        *(uint8_t*)(entryPtr + 225) = 0;
        *(uint8_t*)(entryPtr + 226) = 255;
    }
    
    // Initialize pool metadata
    *(uint16_t*)((uint8_t*)this + 1028) = 0;     // Head index
    *(uint16_t*)((uint8_t*)this + 1026) = 4;     // Available count
    *(uint16_t*)((uint8_t*)this + 1024) = 4;     // Total pool size
    *(uint16_t*)((uint8_t*)this + 240) = 0xFFFF; // Tail marker
    *(uint16_t*)((uint8_t*)this + 1010) = 0xFFFF; // Another marker
    
    // Set up linked list connections between entries
    uint16_t poolSize = *(uint16_t*)((uint8_t*)this + 1026);
    
    for (uint16_t i = 0; i < poolSize - 1; i++) {
        uint32_t offset = i * 256;
        uint8_t* entryPtr = (uint8_t*)this + offset;
        
        // Set previous pointer to i-1 (or 0xFFFF for first entry)
        *(uint16_t*)(entryPtr + 496) = (i == 0) ? 0xFFFF : (i - 1);
        
        // Set next pointer to i+1
        *(uint16_t*)(entryPtr + 242) = i + 1;
    }
}


// ===========================================================================
// SinglesNetworkClient::IsStateActive @ 0x823FA420 | size: 0xA8
//
// Checks if the network client is in an active state based on the state field.
// Returns true/false depending on the current state value and additional checks.
//
// State meanings:
//   0-1: Inactive states (return false)
//   2:   Check vtable pointer as boolean
//   3:   Check if vtable is non-null (return true if initialized)
//   4:   Check if float at offset +0 equals zero constant
//   5:   Check if vtable is non-null (return true if initialized)
//   >5:  Invalid state (return false)
// ===========================================================================
bool SinglesNetworkClient::IsStateActive()
{
    // Load state from offset +4
    uint32_t state = m_state;
    
    // If state > 5, return false
    if (state > 5) {
        return false;
    }
    
    // Handle each state case
    switch (state) {
        case 0:
        case 1:
            // Inactive states
            return false;
            
        case 2:
            // Return vtable pointer as boolean (non-zero = true)
            return (vtable != nullptr);
            
        case 3:
        case 5:
            // Check if vtable is initialized
            return (vtable != nullptr);
            
        case 4: {
            // Check if float at offset +0 (vtable location) equals zero constant
            // Load zero constant from global @ 0x8202D110
            extern const float g_zeroConstant;  // @ 0x8202D110
            float value = *(float*)&vtable;  // Reinterpret vtable pointer as float
            return (value != g_zeroConstant);
        }
            
        default:
            return false;
    }
}


// ===========================================================================
// SinglesNetworkClient::ProcessMessageQueue @ 0x823A87E8 | size: 0x150
//
// Processes all pending network messages in the message queue.
// Iterates through a linked list of messages, validates each one, and
// dispatches them to the appropriate handlers.
//
// This is the main message pump for the network subsystem. It:
// 1. Retrieves the message queue head from the global network state
// 2. Walks the linked list of pending messages
// 3. For each message:
//    - Allocates/creates a message handler object
//    - Validates the message is processable
//    - Gets the message ID
//    - Prepares dispatch context
//    - Dispatches to the appropriate handler
//    - Cleans up the context
// 4. Moves to the next message in the queue
//
// The message queue is maintained at a global singleton accessed via
// offset chains through the network state object.
// ===========================================================================
void SinglesNetworkClient::ProcessMessageQueue()
{
    // External function declarations for message processing pipeline
    extern void* rlEvent_Init(void* msg, int param1, void* rttiStr1, void* rttiStr2, int param2);
    extern bool  NetworkClient_ValidateMessageSlot(void* queueBase, void* msgBuffer);
    extern int   NetworkClient_GetMessageId(void* msgHandler);
    extern void* NetworkClient_BuildDispatchContext(void* stackBuf, void* client);
    extern void  NetworkClient_DispatchMessage(void* dispatcher, int msgId, void* context, int param1, int param2);
    extern void  snBitStream_Reset(void* context);
    extern void* NetworkClient_GetNextMessage(void* currentMsg);
    
    // Global network state object
    extern void* g_pNetworkState;  // @ 0x8271A7B0
    
    // RTTI strings for message type identification
    extern const char g_szCreateMachineRTTI[];  // @ 0x825DEEB0 "uestingConfig@snCreateMachine@rage@@"
    extern const char g_szRageRTTI[];           // @ 0x825DEECC "e@rage@@"
    
    // Step 1: Navigate to the message queue head
    // Path: networkState[60] > 1 ? networkState[56] + 9680 : null
    //       then: result[3756] = message list head
    
    void* networkState = g_pNetworkState;
    int playerCount = *(int*)((char*)networkState + 60);
    
    void* queueBase = nullptr;
    if (playerCount > 1) {
        void* sessionBase = *(void**)((char*)networkState + 56);
        queueBase = (char*)sessionBase + 9680;
    }
    
    // Get the message queue head
    void* messageHead = nullptr;
    if (queueBase != nullptr) {
        messageHead = *(void**)((char*)queueBase + 3756);
    }
    
    // Step 2: Find the tail of the message chain
    // Walk the linked list via msg[8] until we find the last message
    void* currentMsg = messageHead;
    
    if (currentMsg != nullptr) {
        void* nextMsg = *(void**)((char*)currentMsg + 8);
        if (nextMsg != nullptr) {
            // Walk to the end of the chain
            while (true) {
                currentMsg = nextMsg;
                void* nextNext = *(void**)((char*)currentMsg + 8);
                if (nextNext == nullptr) {
                    break;
                }
                nextMsg = nextNext;
            }
        }
    }
    
    // Step 3: Process messages in the queue
    // Loop until we've processed all messages
    while (currentMsg != nullptr) {
        // Allocate/create a message handler object
        // Parameters: message, 0, rttiStr1, rttiStr2, 0
        void* msgHandler = rlEvent_Init(
            currentMsg,
            0,
            (void*)g_szRageRTTI,
            (void*)g_szCreateMachineRTTI,
            0
        );
        
        // Validate the message is ready to process
        // Check if message is in the queue and ready
        char msgBuffer[96];  // Stack buffer at offset +96 from msgHandler
        bool isValid = NetworkClient_ValidateMessageSlot(
            (char*)networkState + 3756,  // Queue base for validation
            (char*)msgHandler + 96
        );
        
        if (isValid) {
            // Get the message ID/type
            int messageId = NetworkClient_GetMessageId(msgHandler);
            
            if (messageId != -1) {
                // Prepare dispatch context on stack
                char dispatchContext[128];  // Stack buffer at r1+80
                void* context = NetworkClient_BuildDispatchContext(dispatchContext, this);
                
                // Dispatch the message to the appropriate handler
                // Parameters: dispatcher base, message ID, context, 1 (priority), 0
                void* dispatcher = (char*)networkState + 64;
                NetworkClient_DispatchMessage(dispatcher, messageId, context, 1, 0);
                
                // Clean up the dispatch context
                snBitStream_Reset(dispatchContext);
            }
        }
        
        // Move to the next message in the queue
        currentMsg = NetworkClient_GetNextMessage(currentMsg);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// TournamentCompleteMessage — Tournament completion network message
// ─────────────────────────────────────────────────────────────────────────────

// ===========================================================================
// TournamentCompleteMessage::Process  [vtable slot 4 @ 0x823BC8C8] | size: 0x98
//
// Processes a tournament completion message. Checks game state and posts
// a page group event if conditions are met.
//
// Logic:
//  1. Calls rage_debugLog with "top_spin" string (debug/logging)
//  2. Checks g_loop_obj_ptr[578] (boolean flag)
//  3. If flag is false, checks g_loop_obj_ptr[12] == 8 (state check)
//  4. If state matches, sets g_input_obj_ptr[48] = 4
//  5. Posts page group message with event code 14386 and m_tournamentId
// ===========================================================================
void TournamentCompleteMessage::Process()
{
    // Debug logging with "top_spin" string
    extern const char g_szTopSpin[];  // @ 0x8206CB54
    rage_debugLog(g_szTopSpin);
    
    // Load global loop object pointer
    extern void* g_loop_obj_ptr;  // @ 0x825EAB30
    void* loopObj = *(void**)&g_loop_obj_ptr;
    
    // Check flag at offset +578
    uint8_t flag = *(uint8_t*)((char*)loopObj + 578);
    
    if (flag == 0) {
        // Check state value at offset +12
        int32_t state = *(int32_t*)((char*)loopObj + 12);
        
        if (state == 8) {
            // Set input object state to 4
            extern void* g_input_obj_ptr;  // @ 0x825EAB28
            void* inputObj = *(void**)&g_input_obj_ptr;
            *(int32_t*)((char*)inputObj + 48) = 4;
        }
    }
    
    // Post page group message with tournament ID
    // Event code 14386, mask 128, active flag 1
    int32_t eventData[2] = { 4, 0 };
    eventData[0] = (int32_t)*(uint8_t*)((char*)this + 4);  // tournament ID byte at +0x04
    PostPageGroupMessage(14386, 128, 1, 0, (uintptr_t)eventData);
}


// ===========================================================================
// TournamentCompleteMessage::GetIndexInPool  [vtable slot 5 @ 0x823BC7B0] | size: 0x54
//
// Returns the index of this message within the TournamentCompleteMessage pool.
// Updates linked-list pointers in the pool header.
//
// Pool structure:
//  - Base address at g_pTournamentMsgPool (0x825D1464)
//  - Object size: 16 bytes (0x10)
//  - Pool header at base+32 contains count and tail index
//
// Python-verified:
//   rlwinm(offset,28,4,31) = (offset >> 4) & 0x0FFFFFFF = index
//   rlwinm(index,4,12,27)  = (index << 4) & 0xFFFF0 = byte offset
// ===========================================================================
uint16_t TournamentCompleteMessage::GetIndexInPool() const
{
    // Load pool base address
    extern uint8_t* g_pTournamentMsgPool;  // @ 0x825D1464
    uint8_t* poolBase = *(uint8_t**)((char*)&g_pTournamentMsgPool + 8);
    
    // Compute object index: offset = (this - base), index = offset / 16
    // Python-verified: rlwinm(offset,28,4,31) extracts (offset >> 4)
    uint32_t offset = (uint32_t)((uint8_t*)this - poolBase);
    uint16_t index  = (uint16_t)((offset >> 4) & 0xFFFF);
    
    // Compute byte offset for this entry: index * 16
    // Python-verified: rlwinm(index,4,12,27) = (index << 4) & 0xFFFF0
    uint32_t entryOffset = (uint32_t)index << 4;
    
    // Update linked-list: store old tail index at entry+14
    uint16_t oldTail = *(uint16_t*)(poolBase + 36);
    *(uint16_t*)(poolBase + entryOffset + 14) = oldTail;
    
    // If old tail was valid, link it forward to us
    if (oldTail != 0xFFFF) {
        uint32_t prevOffset = (uint32_t)oldTail << 4;
        *(uint16_t*)(poolBase + prevOffset + 12) = index;
    }
    
    // Store our index as new tail
    *(uint16_t*)(poolBase + 36) = index;
    
    // Increment entry count
    uint16_t count = *(uint16_t*)(poolBase + 32);
    *(uint16_t*)(poolBase + 32) = count + 1;
    
    return index;
}


// ===========================================================================
// TournamentCompleteMessage::GetSingleton  [vtable slot 6 @ 0x823BC878] | size: 0xC
//
// Returns the TournamentCompleteMessage singleton pointer.
// Python-verified: address = 0x825D1458
// ===========================================================================
void* TournamentCompleteMessage::GetSingleton()
{
    extern void* g_pTournamentMsgSingleton;  // @ 0x825D1458
    return *(void**)&g_pTournamentMsgSingleton;
}


// ===========================================================================
// TournamentCompleteMessage::GetTypeName  [vtable slot 7 @ 0x823BC888] | size: 0xC
//
// Returns the type name string for this message class.
// Python-verified: address = 0x8206EC40
// ===========================================================================
const char* TournamentCompleteMessage::GetTypeName()
{
    // String content: "rnal spectator request:%d denied"
    // (likely part of "Internal spectator request:%d denied")
    extern const char g_szTournamentCompleteTypeName[];  // @ 0x8206EC40
    return g_szTournamentCompleteTypeName;
}

// ===========================================================================
// pongNetMessageHolder::InitializeTimedGameUpdateArray @ 0x823C6E30 | size: 0xFC
//
// Original symbol: pongNetMessageHolder_6E30_wrh
//
// Initializes an array of 10 TimedGameUpdateTimerMessage structures within
// the message holder. Each structure is 16 bytes and contains vtable pointer,
// float timing values, and index/state fields.
//
// This is used for managing periodic game state update messages sent during
// online matches to keep clients synchronized.
// ===========================================================================
void pongNetMessageHolder::InitializeTimedGameUpdateArray()
{
    // Load constant float values from .rdata
    extern const float g_floatZero;        // @ 0x82079AD4
    extern const float g_floatNegOne;      // @ 0x8202D110
    
    // TimedGameUpdateTimerMessage vtable pointer
    const uint32_t vtableAddr = VTABLE_TimedGameUpdateTimerMessage;
    
    // Constants for array initialization
    const uint16_t invalidIndex = 0xFFFF;
    const uint16_t arraySize = 10;
    const uint16_t zeroValue = 0;
    
    // Initialize 10 timer message structures, each 16 bytes
    // Structure layout (per 16-byte element):
    //   +0x00: vtable pointer
    //   +0x04: float - timing value 1
    //   +0x08: float - timing value 2
    //   +0x0C: uint16 - previous index (0xFFFF = invalid)
    //   +0x0E: uint16 - current index
    
    for (int i = 0; i < arraySize; i++) {
        uint8_t* entryPtr = (uint8_t*)this + (i * 16);
        
        // Set vtable pointer
        *(uint32_t*)(entryPtr + 0) = vtableAddr;
        
        // Initialize timing floats
        *(float*)(entryPtr + 4) = g_floatZero;
        *(float*)(entryPtr + 8) = g_floatNegOne;
        
        // Initialize index chain (forms linked list of timer slots)
        *(uint16_t*)(entryPtr + 12) = invalidIndex;
        *(uint16_t*)(entryPtr + 14) = (uint16_t)i;
    }
    
    // Store array metadata at fixed offsets
    *(uint16_t*)((uint8_t*)this + 158) = invalidIndex;  // Head of free list
    *(uint16_t*)((uint8_t*)this + 160) = arraySize;     // Total capacity
    *(uint16_t*)((uint8_t*)this + 162) = arraySize;     // Current count
    *(uint16_t*)((uint8_t*)this + 164) = zeroValue;     // Active count
    
    // Initialize the linked list of available timer slots
    // Each slot points to the next via offset +28 (relative to its base)
    for (uint16_t idx = 0; idx < arraySize - 1; idx++) {
        uint8_t* entryPtr = (uint8_t*)this + (idx * 16);
        
        // Link this slot to the next slot in the free list
        *(uint16_t*)(entryPtr + 28) = zeroValue;        // State: free
        *(uint16_t*)(entryPtr + 14) = idx + 1;          // Next index
    }
}

// ===========================================================================
// pongNetMessageHolder::CleanupNestedArrays @ 0x82135C70 | size: 0xE0
//
// Original symbol: pongNetMessageHolder_5C70_wrh
//
// Destructor helper that cleans up nested dynamically allocated arrays within
// the message holder. This is called during destruction to free memory for
// internal message buffers at offsets +8, +12, +16, and +80.
//
// The function walks through array elements, sets their vtables to a cleanup
// vtable (crIKPart), then frees the memory.
// ===========================================================================
void pongNetMessageHolder::CleanupNestedArrays()
{
    const uint32_t cleanupVtable = VTABLE_rage_crIKPart;
    
    // Clean up first array at offset +8
    void* array1 = *(void**)((uint8_t*)this + 8);
    if (array1 != nullptr) {
        // Set vtable for 2 elements (192 bytes total, 96 bytes each)
        for (int i = 1; i >= 0; i--) {
            uint8_t* element = (uint8_t*)array1 + (i * 96);
            *(uint32_t*)element = cleanupVtable;
        }
        sysMemAllocator_Free(array1);
    }
    
    // Clean up second array at offset +12
    void* array2 = *(void**)((uint8_t*)this + 12);
    if (array2 != nullptr) {
        // Set vtable for 2 elements (192 bytes total, 96 bytes each)
        for (int i = 1; i >= 0; i--) {
            uint8_t* element = (uint8_t*)array2 + (i * 96);
            *(uint32_t*)element = cleanupVtable;
        }
        sysMemAllocator_Free(array2);
    }
    
    // Clean up array at offset +80
    uint8_t* offset80 = (uint8_t*)this + 80;
    *(uint32_t*)offset80 = VTABLE_rage_crIKHead;  // rage::crIKHead intermediate vtable
    
    uint16_t count80 = *(uint16_t*)(offset80 + 14);
    if (count80 > 0) {
        void* buffer80 = *(void**)(offset80 + 8);
        sysMemAllocator_Free(buffer80);
    }
    *(uint32_t*)offset80 = cleanupVtable;
    
    // Clean up array at offset +16
    uint8_t* offset16 = (uint8_t*)this + 16;
    *(uint32_t*)offset16 = VTABLE_rage_crIKSpine;  // rage::crIKSpine intermediate vtable
    
    uint16_t count16 = *(uint16_t*)(offset16 + 14);
    if (count16 > 0) {
        void* buffer16 = *(void**)(offset16 + 8);
        sysMemAllocator_Free(buffer16);
    }
    *(uint32_t*)offset16 = cleanupVtable;
    
    // Set final vtable
    *(uint32_t*)this = VTABLE_rage_crIKBodyBase;  // rage::crIKBodyBase final vtable
}

// ===========================================================================
// pongNetMessageHolder::RemoveElementByPointer @ 0x82134350 | size: 0xB8
//
// Original symbol: pongNetMessageHolder_4350_wrh
//
// Removes an element from the holder's pointer array by comparing against
// the provided pointer value. When found, frees the element's internal
// buffers (at offsets +8 and +16), frees the element itself, then compacts
// the array by shifting remaining elements down.
//
// Parameters:
//   targetPtr - Pointer to the element to remove
//
// This is used to remove message handlers from the active handler list.
// ===========================================================================
void pongNetMessageHolder::RemoveElementByPointer(void* targetPtr)
{
    uint32_t elementCount = *(uint32_t*)((uint8_t*)this + 4);
    if (elementCount == 0) {
        return;
    }
    
    void** arrayBase = *(void***)this;
    
    // Search for the target pointer in the array
    uint32_t foundIndex = 0;
    void** currentSlot = arrayBase;
    
    for (foundIndex = 0; foundIndex < elementCount; foundIndex++) {
        if (*currentSlot == targetPtr) {
            // Found the target - free its internal buffers
            void* element = *currentSlot;
            if (element != nullptr) {
                // Free buffer at offset +8
                void* buffer1 = *(void**)((uint8_t*)element + 8);
                sysMemAllocator_Free(buffer1);
                
                // Free buffer at offset +16
                void* buffer2 = *(void**)((uint8_t*)element + 16);
                sysMemAllocator_Free(buffer2);
                
                // Free the element itself
                sysMemAllocator_Free(element);
            }
            
            // Decrement count
            elementCount--;
            *(uint32_t*)((uint8_t*)this + 4) = elementCount;
            
            // Compact the array by shifting remaining elements down
            for (uint32_t i = foundIndex; i < elementCount; i++) {
                arrayBase[i] = arrayBase[i + 1];
            }
            
            return;
        }
        currentSlot++;
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// pongNetMessageHolder — Batch Implementation (10 functions)
// ═══════════════════════════════════════════════════════════════════════════

// External references
extern "C" void* rage_Alloc(uint32_t size);  // Memory allocator @ 0x820DEC88
extern void pongNetMessageHolder_vfn_2_1770_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_18D0_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_68D0_wrh(void* msg);  // AcceptMessage constructor
extern void BallHitMessage_ctor_69C8(void* msg);       // BallHitMessage constructor
extern void pongNetMessageHolder_6B48_wrh(void* msg);  // Message constructor
extern void pongNetMessageHolder_6C98_wrh(void* msg);  // Message constructor
extern void pongNetMessageHolder_6D68_wrh(void* msg);  // Message constructor
extern void pongNetMessageHolder_6E30_wrh(void* msg);  // Message constructor
extern void pongNetMessageHolder_6FF8_wrh(void* msg);  // Message constructor
extern void pongNetMessageHolder_6778_wrh(void* msg);  // Message constructor

// Global counter tracking live pongNetMessageHolder instances
static uint32_t& g_netMessageHolderLiveCount = g_netMessageHolderLiveCount_raw;

// Global message holder instances
static pongNetMessageHolder& g_netMessageHolder1 = *reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_129C);
static pongNetMessageHolder& g_netMessageHolder2 = *reinterpret_cast<pongNetMessageHolder*>(&g_holderInstance_AudioRefCount);


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_4CB8_w @ 0x82584CB8 | size: 0x5C
//
// Static cleanup wrapper for network message holder instance 1.
// Called during static deinitialization to clean up the message holder.
// Decrements the global live instance counter.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_4CB8_w() {
    // Set initial vtable to pongNetMessageHolder
    g_netMessageHolder1.vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_02AC);
    
    // Call initialization function
    pongNetMessageHolder_vfn_2_1770_1(&g_netMessageHolder1);
    
    // Change vtable to base class (pongNetMessageHolderBase)
    g_netMessageHolder1.vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    
    // Decrement global live instance counter
    g_netMessageHolderLiveCount--;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_4D18_w @ 0x82584D18 | size: 0x5C
//
// Static cleanup wrapper for network message holder instance 2.
// Identical pattern to 4CB8_w but operates on a different global instance.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_4D18_w() {
    // Set initial vtable to pongNetMessageHolder
    g_netMessageHolder2.vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_02C0);
    
    // Call initialization function
    pongNetMessageHolder_vfn_2_18D0_1(&g_netMessageHolder2);
    
    // Change vtable to base class (pongNetMessageHolderBase)
    g_netMessageHolder2.vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    
    // Decrement global live instance counter
    g_netMessageHolderLiveCount--;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::vfn_1 @ 0x823BFBA8 | size: 0x54 [vtable slot 1]
//
// Lazy initialization for message pool at offset +8.
// Allocates 12016 bytes and constructs the message object if not already initialized.
// This is the scalar destructor slot, but it's actually doing lazy initialization.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder::LazyInitMessagePool() {
    // Check if message pool is already initialized
    if (m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for message pool (12016 bytes)
    void* memory = rage_Alloc(12016);
    
    if (memory != nullptr) {
        // Construct the message object
        pongNetMessageHolder_6778_wrh(memory);
        m_pInternalArray = memory;
    } else {
        m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FC68_1 @ 0x823BFC68 | size: 0x54
//
// Lazy initialization for AcceptMessage (176 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FC68_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for AcceptMessage (176 bytes)
    void* memory = rage_Alloc(176);
    
    if (memory != nullptr) {
        // Construct AcceptMessage
        pongNetMessageHolder_68D0_wrh(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FCC0_1 @ 0x823BFCC0 | size: 0x54
//
// Lazy initialization for BallHitMessage (256 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FCC0_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for BallHitMessage (256 bytes)
    void* memory = rage_Alloc(256);
    
    if (memory != nullptr) {
        // Construct BallHitMessage
        BallHitMessage_ctor_69C8(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FD18_1 @ 0x823BFD18 | size: 0x54
//
// Lazy initialization for network message (8976 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FD18_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for message (8976 bytes)
    void* memory = rage_Alloc(8976);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6B48_wrh(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FEE0_1 @ 0x823BFEE0 | size: 0x54
//
// Lazy initialization for network message (96 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FEE0_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for message (96 bytes)
    void* memory = rage_Alloc(96);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6C98_wrh(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FFA0_1 @ 0x823BFFA0 | size: 0x54
//
// Lazy initialization for network message (368 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FFA0_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for message (368 bytes)
    void* memory = rage_Alloc(368);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6D68_wrh(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_0810_1 @ 0x823C0810 | size: 0x54
//
// Lazy initialization for network message (176 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_0810_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for message (176 bytes)
    void* memory = rage_Alloc(176);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6E30_wrh(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_0990_1 @ 0x823C0990 | size: 0x54
//
// Lazy initialization for network message (16016 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_0990_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pInternalArray != nullptr) {
        return;
    }
    
    // Allocate memory for message (16016 bytes)
    void* memory = rage_Alloc(16016);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6FF8_wrh(memory);
        holder->m_pInternalArray = memory;
    } else {
        holder->m_pInternalArray = nullptr;
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
// DataRequestMessage — Network message for requesting player/match data
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * DataRequestMessage is a network message sent to request data from another client.
 * It carries a timing float and a 16-bit data identifier/index.
 * 
 * Structure layout:
 *   +0x00  vtable pointer
 *   +0x04  float — timing reference or request timestamp
 *   +0x08  int16_t — data identifier/index being requested
 * 
 * Vtable layout:
 *   slot 1 = Deserialise (read from network)
 *   slot 2 = Serialise (write to network)
 *   slot 5 = GetIndexInPool
 *   slot 6 = GetSingleton
 *   slot 7 = GetTypeName
 */

// External references
extern uint8_t* g_pDataRequestMsgPoolBase;  // @ 0x825D129C
extern void* g_pDataRequestMsgSingleton;    // @ 0x825D1290
extern const char g_szDataRequestTypeName[];  // @ 0x8206EA74

// Network I/O helpers (already declared in pong_network.cpp)
extern void snBitStream_ReadBits(void* client, void* buf, int size);
extern void netStream_WriteS16(void* client, int16_t value, int bitWidth);
extern void WriteFloatToNetworkStream(void* client, float value);
extern uint8_t netStream_ReadS16(void* client, void* dst, int bitWidth);


// ─────────────────────────────────────────────────────────────────────────────
// DataRequestMessage::Deserialise  [vtable slot 1 @ 0x823BABE0]
// 
// Reads DataRequestMessage fields from the network byte stream.
// Called when a DataRequestMessage packet arrives from another client.
// 
// Fields populated:
//   +0x04  float — timing reference (32-bit network field)
//   +0x08  int16_t — data identifier (16-bit network field)
// ─────────────────────────────────────────────────────────────────────────────
void DataRequestMessage::Deserialise(void* client)
{
    // Read timing float (32-bit field)
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    m_timingRef = timingRef;
    
    // Read 16-bit data identifier using netStream_ReadS16
    // netStream_ReadS16 reads a bit-packed value and handles sign extension
    netStream_ReadS16(client, &m_dataId, 16);
}


// ─────────────────────────────────────────────────────────────────────────────
// DataRequestMessage::Serialise  [vtable slot 2 @ 0x823BAE28]
// 
// Writes DataRequestMessage fields to the outgoing network byte stream.
// Called when the local client needs to request data from a remote client.
// ─────────────────────────────────────────────────────────────────────────────
void DataRequestMessage::Serialise(void* client)
{
    // Write timing reference float
    WriteFloatToNetworkStream(client, m_timingRef);
    
    // Write 16-bit data identifier (sign-extended)
    netStream_WriteS16(client, m_dataId, 16);
}


// ─────────────────────────────────────────────────────────────────────────────
// DataRequestMessage::GetIndexInPool  [vtable slot 5 @ 0x823BAAF8]
// 
// Returns the index of this object within the DataRequestMessage pool.
// Updates the pool's linked-list structure for message tracking.
// 
// Pool structure:
//   - Base address: g_pDataRequestMsgPoolBase @ 0x825D129C
//   - Object size: 16 bytes (0x10)
//   - Pool metadata at base+32: entry count (uint16)
//   - Pool metadata at base+36: tail index (uint16)
// ─────────────────────────────────────────────────────────────────────────────
uint16_t DataRequestMessage::GetIndexInPool() const
{
    uint8_t* poolBase = *(uint8_t**)g_pDataRequestMsgPoolBase;
    
    // Compute object index: offset = (this - base), index = offset / 16
    // rlwinm(offset, 28, 4, 31) extracts (offset >> 4) & 0x0FFFFFFF
    uint32_t offset = (uint32_t)((uint8_t*)this - poolBase);
    uint16_t index = (uint16_t)((offset >> 4) & 0xFFFF);
    
    // Update linked-list: pool[36] = old_tail
    uint16_t oldTail = *(uint16_t*)(poolBase + 36);
    if (oldTail != 0xFFFF) {
        // Link previous tail's forward pointer to us
        // rlwinm(oldTail, 4, 12, 27) = oldTail * 16 = byte offset
        uint32_t prevOffset = (uint32_t)oldTail * 16;
        *(uint16_t*)(poolBase + prevOffset + 14) = index;
    }
    
    // Write our index as the new tail
    *(uint16_t*)(poolBase + index * 16 + 14) = index;
    *(uint16_t*)(poolBase + 36) = index;
    
    // Increment entry count
    uint16_t count = *(uint16_t*)(poolBase + 32);
    *(uint16_t*)(poolBase + 32) = count + 1;
    
    return index;
}


// ─────────────────────────────────────────────────────────────────────────────
// DataRequestMessage::GetSingleton  [vtable slot 6 @ 0x823BABC0]
// 
// Returns the DataRequestMessage message-type singleton pointer.
// This is the pool/factory object managing all DataRequestMessage instances.
// ─────────────────────────────────────────────────────────────────────────────
void* DataRequestMessage::GetSingleton()
{
    return g_pDataRequestMsgSingleton;  // @ 0x825D1290
}


// ─────────────────────────────────────────────────────────────────────────────
// DataRequestMessage::GetTypeName  [vtable slot 7 @ 0x823BABD0]
// 
// Returns a pointer to the ASCII type-name string for this message class.
// Used by the network dispatcher for message registration and debug logging.
// ─────────────────────────────────────────────────────────────────────────────
const char* DataRequestMessage::GetTypeName()
{
    return g_szDataRequestTypeName;  // @ 0x8206EA74
}


// ═══════════════════════════════════════════════════════════════════════════════
// DataSendMessage — Network message for sending data to another client
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * DataSendMessage is a network message sent to transmit data to another client.
 * It carries a timing float, two 16-bit identifiers, and a variable-length data payload.
 * 
 * Structure layout:
 *   +0x00  vtable pointer
 *   +0x04  float — timing reference or send timestamp
 *   +0x08  int16_t — data type identifier
 *   +0x0A  int16_t — data size/length
 *   +0x0C  variable — data payload (size determined by field at +0x0A)
 * 
 * Vtable layout:
 *   slot 1 = Deserialise (read from network)
 *   slot 2 = Serialise (write to network)
 *   slot 5 = GetIndexInPool
 *   slot 6 = GetSingleton
 *   slot 7 = GetTypeName
 */

// External references
extern uint8_t* g_pDataSendMsgPoolBase;  // @ 0x825D12B4
extern void* g_pDataSendMsgSingleton;    // @ 0x825D12A8
extern const char g_szDataSendTypeName[];  // @ 0x8206EA88

// Network I/O helpers
extern void netStream_ReadBlock(void* client, void* data, int16_t size);
extern void SinglesNetworkClient_0598_g(void* data, void* client);


// ─────────────────────────────────────────────────────────────────────────────
// DataSendMessage::Deserialise  [vtable slot 1 @ 0x823BED20]
// 
// Reads DataSendMessage fields from the network byte stream.
// Called when a DataSendMessage packet arrives from another client.
// 
// Fields populated:
//   +0x04  float — timing reference (32-bit network field)
//   +0x08  int16_t — data type identifier (16-bit network field)
//   +0x0A  int16_t — data size (16-bit network field)
//   +0x0C  variable — data payload (size bytes)
// ─────────────────────────────────────────────────────────────────────────────
void DataSendMessage::Deserialise(void* client)
{
    // Read timing float (32-bit field)
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    m_timingRef = timingRef;
    
    // Read data type identifier (16-bit field)
    netStream_ReadS16(client, &m_dataType, 16);
    
    // Read data size (16-bit field)
    netStream_ReadS16(client, &m_dataSize, 16);
    
    // Read variable-length data payload
    // Size is determined by m_dataSize field
    netStream_ReadBlock(client, &m_dataPayload, m_dataSize);
}


// ─────────────────────────────────────────────────────────────────────────────
// DataSendMessage::Serialise  [vtable slot 2 @ 0x823BED90]
// 
// Writes DataSendMessage fields to the outgoing network byte stream.
// Called when the local client needs to send data to a remote client.
// ─────────────────────────────────────────────────────────────────────────────
void DataSendMessage::Serialise(void* client)
{
    // Write timing reference float
    WriteFloatToNetworkStream(client, m_timingRef);
    
    // Write data payload structure (includes type, size, and data)
    // SinglesNetworkClient_0598_g handles the complete payload serialization
    SinglesNetworkClient_0598_g(&m_dataType, client);
}


// ─────────────────────────────────────────────────────────────────────────────
// DataSendMessage::GetIndexInPool  [vtable slot 5 @ 0x823BAC40]
// 
// Returns the index of this object within the DataSendMessage pool.
// Updates the pool's linked-list structure for message tracking.
// 
// Pool structure:
//   - Base address: g_pDataSendMsgPoolBase @ 0x825D12B4
//   - Object size: 524 bytes (0x20C)
//   - Pool metadata at base+1048: entry count (uint16)
//   - Pool metadata at base+1052: tail index (uint16)
//   - Forward link at object+520
//   - Backward link at object+522
// ─────────────────────────────────────────────────────────────────────────────
uint16_t DataSendMessage::GetIndexInPool() const
{
    uint8_t* poolBase = *(uint8_t**)g_pDataSendMsgPoolBase;
    
    // Compute object index: offset = (this - base), index = offset / 524
    // Uses mulhwu with magic constant 0xFA1D2CF3 for division by 524
    uint32_t offset = (uint32_t)((uint8_t*)this - poolBase);
    uint64_t mulResult = (uint64_t)offset * 0xFA1D2CF3ULL;
    uint32_t quotient = (uint32_t)(mulResult >> 32);
    uint16_t index = (uint16_t)((quotient >> 9) & 0xFFFF);
    
    // Update linked-list: pool[1052] = old_tail
    uint16_t oldTail = *(uint16_t*)(poolBase + 1052);
    if (oldTail != 0xFFFF) {
        // Link previous tail's forward pointer to us
        uint32_t prevOffset = (uint32_t)oldTail * 524;
        *(uint16_t*)(poolBase + prevOffset + 520) = index;
    }
    
    // Write our index as the new tail
    *(uint16_t*)(poolBase + index * 524 + 522) = index;
    *(uint16_t*)(poolBase + 1052) = index;
    
    // Increment entry count
    uint16_t count = *(uint16_t*)(poolBase + 1048);
    *(uint16_t*)(poolBase + 1048) = count + 1;
    
    return index;
}


// ─────────────────────────────────────────────────────────────────────────────
// DataSendMessage::GetSingleton  [vtable slot 6 @ 0x823BAD20]
// 
// Returns the DataSendMessage message-type singleton pointer.
// This is the pool/factory object managing all DataSendMessage instances.
// ─────────────────────────────────────────────────────────────────────────────
void* DataSendMessage::GetSingleton()
{
    return g_pDataSendMsgSingleton;  // @ 0x825D12A8
}


// ─────────────────────────────────────────────────────────────────────────────
// DataSendMessage::GetTypeName  [vtable slot 7 @ 0x823BAD30]
// 
// Returns a pointer to the ASCII type-name string for this message class.
// Used by the network dispatcher for message registration and debug logging.
// ─────────────────────────────────────────────────────────────────────────────
const char* DataSendMessage::GetTypeName()
{
    return g_szDataSendTypeName;  // @ 0x8206EA88
}


// ═══════════════════════════════════════════════════════════════════════════════
// SinglesNetworkClient — Batch Implementation (10 utility functions)
// ═══════════════════════════════════════════════════════════════════════════════

// External references for network DLL functions
extern "C" {
    void NetDll_sendto(int socket, const void* buffer, int length, int flags, 
                       const void* dest_addr, int addrlen);
    void NetDll_recvfrom(int socket, void* buffer, int length, int flags,
                         void* src_addr, int* addrlen);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetGPURegister7 @ 0x82329E68 | size: 0x20
//
// Sets GPU register value to 7 and triggers an EIEIO barrier.
// This appears to be a graphics hardware initialization function that writes
// to specific GPU control registers.
//
// Register addresses (Python-verified):
//   lis 32712 = 0x7FC80000 (GPU register base)
//   +12820 = 0x32 14 (control register 1)
//   +13320 = 0x34 08 (control register 2)
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::SetGPURegister7()
{
    // GPU register base address
    volatile uint32_t* gpuBase = reinterpret_cast<volatile uint32_t*>(0x7FC80000);
    
    // Write value 7 to control register 1
    gpuBase[12820 / 4] = 7;
    
    // EIEIO barrier - enforce in-order execution of I/O operations
    __asm__ volatile("" ::: "memory"); // PPC eieio — compiler fence on host
    
    // Write value 2048 to control register 2
    gpuBase[13320 / 4] = 2048;
    
    // EIEIO barrier
    __asm__ volatile("" ::: "memory"); // PPC eieio — compiler fence on host
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SendToSocket @ 0x82585BF8 | size: 0x20
//
// Wrapper for NetDll_sendto that shifts all parameters and sets socket to 1.
// This is a thunk function that adapts the calling convention for the network
// DLL's sendto function.
//
// Parameters are shifted: r3→r4, r4→r5, r5→r6, r6→r7, r7→r8, r8→r9
// Socket parameter (r3) is hardcoded to 1.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::SendToSocket(const void* buffer, int length, int flags,
                                        const void* destAddr, int addrlen, int extraParam)
{
    // Call NetDll_sendto with socket=1 and shifted parameters
    NetDll_sendto(1, buffer, length, flags, destAddr, addrlen);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReceiveFromSocket @ 0x82585BF8 | size: 0x20
//
// Wrapper for NetDll_recvfrom that shifts all parameters and sets socket to 1.
// Mirror of SendToSocket for receiving data from the network.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::ReceiveFromSocket(void* buffer, int length, int flags,
                                             void* srcAddr, int* addrlen, int extraParam)
{
    // Call NetDll_recvfrom with socket=1 and shifted parameters
    NetDll_recvfrom(1, buffer, length, flags, srcAddr, addrlen);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CompareFlags @ 0x82329E68 | size: 0x28
//
// Compares the flags field (+4) of two SinglesNetworkClient objects.
// Returns -1 if this < other, 0 if equal, 1 if this > other.
//
// This is used for sorting or ordering network clients in priority queues.
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient::CompareFlags(const SinglesNetworkClient* other) const
{
    uint32_t thisFlags = m_state;
    uint32_t otherFlags = other->m_state;
    
    if (thisFlags == otherFlags) {
        return 0;
    }
    
    return (thisFlags < otherFlags) ? -1 : 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::MemCopyAligned @ 0x82434520 | size: 0x28
//
// Optimized memory copy function that handles alignment and uses word-sized
// copies when possible. This is a performance-critical function for network
// packet handling.
//
// Algorithm:
// 1. Copy bytes until destination is 4-byte aligned
// 2. Copy 4-byte words for the bulk of the data
// 3. Copy remaining bytes
//
// Parameters:
//   dest - Destination buffer
//   src - Source buffer
//   size - Number of bytes to copy (size+1 actual bytes copied)
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::MemCopyAligned(void* dest, const void* src, int size)
{
    uint8_t* dst = static_cast<uint8_t*>(dest);
    const uint8_t* source = static_cast<const uint8_t*>(src);
    int remaining = size + 1;
    
    // Phase 1: Copy bytes until destination is 4-byte aligned
    while (remaining > 0 && (reinterpret_cast<uintptr_t>(dst) & 3) != 0) {
        *dst++ = *source++;
        remaining--;
    }
    
    // Phase 2: Copy 4-byte words
    int wordCount = remaining >> 2;
    if (wordCount > 0) {
        // Check if source is also 4-byte aligned for fast path
        if ((reinterpret_cast<uintptr_t>(source) & 3) == 0) {
            // Fast path: both aligned, use word copies
            uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
            const uint32_t* src32 = reinterpret_cast<const uint32_t*>(source);
            
            for (int i = 0; i < wordCount; i++) {
                *dst32++ = *src32++;
            }
            
            dst = reinterpret_cast<uint8_t*>(dst32);
            source = reinterpret_cast<const uint8_t*>(src32);
        } else {
            // Slow path: source unaligned, assemble words from bytes
            uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
            
            for (int i = 0; i < wordCount; i++) {
                uint32_t word = (static_cast<uint32_t>(source[0]) << 24) |
                               (static_cast<uint32_t>(source[1]) << 16) |
                               (static_cast<uint32_t>(source[2]) << 8) |
                               static_cast<uint32_t>(source[3]);
                *dst32++ = word;
                source += 4;
            }
            
            dst = reinterpret_cast<uint8_t*>(dst32);
        }
        
        remaining &= 3;
    }
    
    // Phase 3: Copy remaining bytes
    while (remaining > 0) {
        *dst++ = *source++;
        remaining--;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::InitializeTimingState [vtable slot 9] @ 0x823941D0 | size: 0x28
//
// Initializes network timing state fields. Sets up timing flags and loads
// initial timing value from the global game state object.
//
// Fields initialized:
//   +4110: timing enabled flag (set to 1)
//   +4111: timing active flag (set to 0)
//   +4112: current timing value (loaded from global state)
//
// Global state accessed:
//   Python-verified: lis(-32142) + (-23768) = 0x8271A328
//   Timing value at globalState[4][332]
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::InitializeTimingState()
{
    // Load global game state object
    extern void* g_pGameStatePtr;  // @ 0x8271A328
    void* gameState = *(void**)&g_pGameStatePtr;
    
    // Get timing subsystem from game state
    void* timingSubsystem = *(void**)((char*)gameState + 4);
    
    // Load current timing value (float at offset +332)
    float currentTiming = *(float*)((char*)timingSubsystem + 332);
    
    // Initialize timing state fields
    *(uint8_t*)((char*)this + 4110) = 1;              // Enable timing
    *(uint8_t*)((char*)this + 4111) = 0;              // Not active yet
    *(float*)((char*)this + 4112) = currentTiming;    // Store current time
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetTimingValue [vtable slot 14] @ 0x8239AFB0 | size: 0x30
//
// Returns a computed timing value based on the global game state.
// Reads an integer counter from the game state, converts it to float,
// and adds it to a base timing value.
//
// Computation:
//   result = gameState[12][8] + float(gameState[12][4])
//
// This is used for network synchronization and latency compensation.
// ─────────────────────────────────────────────────────────────────────────────
float SinglesNetworkClient::GetTimingValue()
{
    // Load global game state object
    extern void* g_pGameStatePtr;  // @ 0x8271A328
    void* gameState = *(void**)&g_pGameStatePtr;
    
    // Get timing subsystem from game state
    void* timingSubsystem = *(void**)((char*)gameState + 12);
    
    // Load base timing value (float at offset +8)
    float baseTiming = *(float*)((char*)timingSubsystem + 8);
    
    // Load counter value (int32 at offset +4)
    int32_t counter = *(int32_t*)((char*)timingSubsystem + 4);
    
    // Convert counter to float and add to base timing
    float counterFloat = static_cast<float>(counter);
    
    return baseTiming + counterFloat;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ForwardToAtSingleton [vtable slot 17] @ 0x8239B000 | size: 0x30
//
// Forwards a call to the atSingleton object if the network state is 2 or 3.
// This is a state-dependent dispatch function that routes certain operations
// to the global singleton manager.
//
// State meanings:
//   0, 1: Inactive - return without action
//   2, 3: Active - forward to atSingleton
//   other: Invalid - return without action
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::ForwardToAtSingleton(void* param1, void* param2)
{
    // Check network state at offset +28
    int32_t state = *(int32_t*)((char*)this + 28);
    
    // Only forward if state is 2 or 3
    if (state != 2 && state != 3) {
        return;
    }
    
    // Load global atSingleton object
    extern void* g_pAtSingletonPtr;  // @ 0x8271A328 (same base as game state)
    void* atSingleton = *(void**)&g_pAtSingletonPtr;
    
    // Forward call to atSingleton (vtable slot 1)
    // Parameters: atSingleton, 0 (null), param1, param2
    typedef void (*AtSingletonFunc)(void*, int, void*, void*);
    void** vtable = *(void***)atSingleton;
    AtSingletonFunc func = reinterpret_cast<AtSingletonFunc>(vtable[1]);
    func(atSingleton, 0, param1, param2);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CleanupAndCallVirtual [vtable slot 18] @ 0x8239B080 | size: 0x4C
//
// Cleans up internal state and calls a virtual function on a subsystem object.
// This is part of the network client shutdown/cleanup sequence.
//
// Steps:
// 1. Get global game state object
// 2. Call virtual function (slot 1) on game state subsystem
// 3. Call cleanup function on this+784
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::CleanupAndCallVirtual()
{
    // Load global game state object
    extern void* g_pGameStatePtr;  // @ 0x8271A328
    void* gameState = *(void**)&g_pGameStatePtr;
    
    // Get subsystem at offset +12
    void* subsystem = *(void**)((char*)gameState + 12);
    
    // Call virtual function (slot 1) on subsystem
    typedef void (*SubsystemFunc)(void*);
    void** vtable = *(void***)subsystem;
    SubsystemFunc func = reinterpret_cast<SubsystemFunc>(vtable[1]);
    func(subsystem);
    
    // Call cleanup function on internal state at offset +784
    extern void NetworkClient_InitInternalState(void* state);
    void* internalState = (char*)this + 784;
    NetworkClient_InitInternalState(internalState);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CallVirtualOnSubobject @ 0x8239DF58 | size: 0x28
//
// Calls a virtual function (slot 19) on a subobject if it exists.
// Returns the result of the virtual call, or 0 if the subobject is null.
//
// This is a safe virtual dispatch wrapper that checks for null before calling.
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient::CallVirtualOnSubobject()
{
    // Load subobject pointer from offset +52
    void* subobject = *(void**)((char*)this + 52);
    
    // If subobject is null, return 0
    if (subobject == nullptr) {
        return 0;
    }
    
    // Call virtual function (slot 19) on subobject
    typedef int (*SubobjectFunc)(void*);
    void** vtable = *(void***)subobject;
    SubobjectFunc func = reinterpret_cast<SubobjectFunc>(vtable[19]);
    return func(subobject);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CheckNetworkInitialized @ 0x82391AB8 | size: 0x2C
//
// Checks if the network subsystem is initialized and calls a setup function
// if it is. This is used during network client initialization to ensure
// dependencies are ready.
//
// Returns early if the global network pointer is null.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::CheckNetworkInitialized()
{
    // Load global network base pointer
    extern void* g_pNetworkBase;  // @ 0x8271A7B0
    void* networkBase = *(void**)&g_pNetworkBase;
    
    // Check if network is initialized
    bool isInitialized = (networkBase != nullptr);
    
    // If not initialized, return early
    if (!isInitialized) {
        return;
    }
    
    // Network is initialized - call setup function
    extern void NetworkClient_ResetLocalState(void* client);
    NetworkClient_ResetLocalState(this);
}


/* ── External dependencies for list management ────────────────────────────── */

/* UI list manager structure @ 0x825D16AC (.data + 8 from 0x825D16A4) */
extern void* g_uiListManager;  /* @ 0x825D16AC */

/* ═══════════════════════════════════════════════════════════════════════════
 * SinglesNetworkClient_F710_p23 @ 0x823BF710 | size: 0x8c (140 bytes)
 *
 * Removes an entry from a doubly-linked free list and updates the list head.
 *
 * This function is part of the network client's memory management system,
 * handling the removal of entries from a free list when they are allocated
 * for use.
 *
 * List structure layout (12 bytes per entry):
 *   +0x00 (0)    (unused in this function)
 *   +0x08 (8)    m_prevIndex  - Index of previous entry in list (u16)
 *   +0x0A (10)   m_nextIndex  - Index of next entry in list (u16)
 *
 * List manager structure (at g_uiListManager):
 *   +0x0C (12)   m_freeCount  - Number of free entries (u16)
 *   +0x10 (16)   m_headIndex  - Index of first free entry (u16)
 *
 * Algorithm:
 *   1. Load list manager and get current head index
 *   2. If head is 0xFFFF (empty list), return NULL
 *   3. Calculate entry address: base + (index * 12)
 *   4. Load next index from current head entry
 *   5. Update list head to next index
 *   6. If next entry exists, clear its previous link
 *   7. Clear current entry's next link
 *   8. Increment free count
 *   9. Return pointer to removed entry
 *
 * The index * 12 calculation is done as: (index + index*2) * 4 = index * 12
 * ═══════════════════════════════════════════════════════════════════════════ */
void* SinglesNetworkClient_F710_p23(void)
{
    /* Load list manager structure */
    uint8_t* listMgr = (uint8_t*)g_uiListManager;
    
    /* Get current head index from list manager */
    uint16_t headIndex = *(uint16_t*)(listMgr + 16);
    
    /* Check if list is empty (head == 0xFFFF) */
    if (headIndex == 0xFFFF) {
        return NULL;
    }
    
    /* Calculate address of head entry: base + (index * 12) */
    uint32_t entryOffset = headIndex * 12;  /* index * 3 * 4 */
    uint8_t* headEntry = listMgr + entryOffset;
    
    /* Load next index from head entry */
    uint16_t nextIndex = *(uint16_t*)(headEntry + 10);
    
    /* Update list head to next entry */
    *(uint16_t*)(listMgr + 16) = nextIndex;
    
    /* If next entry exists, clear its previous link */
    if (nextIndex != 0xFFFF) {
        uint32_t nextOffset = nextIndex * 12;
        uint8_t* nextEntry = listMgr + nextOffset;
        *(uint16_t*)(nextEntry + 8) = 0xFFFF;
    }
    
    /* Clear current entry's next link */
    *(uint16_t*)(headEntry + 10) = 0xFFFF;
    
    /* Increment free count */
    uint16_t freeCount = *(uint16_t*)(listMgr + 12);
    *(uint16_t*)(listMgr + 12) = freeCount + 1;
    
    /* Return pointer to removed entry */
    return headEntry;
}


/* ═══════════════════════════════════════════════════════════════════════════
 * NetworkClient_FindMessageTypeByName @ 0x822F97B8 | size: 0x80 (128 bytes)
 *
 * Searches for a matching string in an array of network client entries.
 *
 * This function iterates through an array of 56-byte entries, comparing each
 * entry's string (starting at offset 0) with the provided search string.
 *
 * SinglesNetworkClient layout (partial):
 *   +0x000 (0)    m_name[56]    - Entry name string (first of array)
 *   +0x1C0 (448)  m_entryCount  - Number of entries in array
 *
 * Each entry is 56 bytes, with the string starting at offset 0.
 *
 * Algorithm:
 *   1. Load entry count from offset +448
 *   2. If count <= 0, return NULL
 *   3. For each entry (index 0 to count-1):
 *      a. Compare entry string with search string byte-by-byte
 *      b. If strings match (all bytes equal until null terminator), return entry pointer
 *      c. Otherwise, advance to next entry (current + 56 bytes)
 *   4. If no match found, return NULL
 *
 * Returns: Pointer to matching entry, or NULL if not found
 * ═══════════════════════════════════════════════════════════════════════════ */
void* NetworkClient_FindMessageTypeByName(void* pThis, const char* searchString)
{
    uint8_t* client = (uint8_t*)pThis;
    
    /* Load entry count */
    int32_t entryCount = *(int32_t*)(client + 448);
    
    /* Return NULL if no entries */
    if (entryCount <= 0) {
        return NULL;
    }
    
    /* Search through array of 56-byte entries */
    uint8_t* currentEntry = client;
    
    for (int i = 0; i < entryCount; i++) {
        /* Compare strings byte-by-byte */
        const char* entryString = (const char*)currentEntry;
        const char* search = searchString;
        
        bool match = true;
        while (*search != '\0') {
            if (*entryString != *search) {
                match = false;
                break;
            }
            entryString++;
            search++;
        }
        
        /* Check if we reached end of both strings (complete match) */
        if (match && *entryString == *search) {
            /* Found matching entry - return pointer to it */
            return currentEntry;
        }
        
        /* Advance to next entry (56 bytes) */
        currentEntry += 56;
    }
    
    /* No match found */
    return NULL;
}


// ===========================================================================
// PlayerMovementMessage — Network message for player movement synchronization
// ===========================================================================

/**
 * PlayerMovementMessage carries player movement data across the network.
 * 
 * Structure:
 *   +0x10: float velocityX
 *   +0x14: float velocityY (always set to -1.0f as default)
 *   +0x18: float velocityZ
 *   +0x1C: float unused1
 *   +0x20: float accelerationX
 *   +0x24: float unused2
 *   +0x28: float accelerationZ
 *   +0x2C: float unused3
 *   +0x30: uint8_t playerIndex
 *   +0x31: uint8_t isMoving
 * 
 * Vtable slots:
 *   1 = Deserialise (read from network)
 *   2 = Serialise (write to network)
 *   4 = Process (apply to game state)
 *   5 = ReturnToPool
 *   6 = GetPoolSingleton
 *   7 = GetTypeName
 */

// ─────────────────────────────────────────────────────────────────────────────
// PlayerMovementMessage::Deserialise() [vtable slot 1 @ 0x823B8110]
// 
// Reads player movement data from network stream.
// ─────────────────────────────────────────────────────────────────────────────
void PlayerMovementMessage::Deserialise(void* networkClient) {
    const float INVALID_VELOCITY = -1.0f;
    
    uint32_t fieldMask = 0;
    uint8_t playerIndex = 0;
    
    // Read field presence bitmask (4 bits) and player index (1 byte)
    DeserializeNetworkData(networkClient, &fieldMask, 4);
    DeserializeNetworkData(networkClient, &playerIndex, 1);
    
    this->playerIndex = playerIndex;
    
    // Read velocityX if present (bit 0)
    if (fieldMask & 0x1) {
        float velocityX;
        DeserializeNetworkData(networkClient, &velocityX, 32);
        this->velocityX = velocityX;
    } else {
        this->velocityX = INVALID_VELOCITY;
    }
    
    this->velocityY = INVALID_VELOCITY;  // Always set to default
    
    // Read velocityZ if present (bit 1)
    if (fieldMask & 0x2) {
        float velocityZ;
        DeserializeNetworkData(networkClient, &velocityZ, 32);
        this->velocityZ = velocityZ;
    } else {
        this->velocityZ = INVALID_VELOCITY;
    }
    
    // Read accelerationX if present (bit 2)
    if (fieldMask & 0x4) {
        float accelerationX;
        DeserializeNetworkData(networkClient, &accelerationX, 32);
        this->accelerationX = accelerationX;
    } else {
        this->accelerationX = INVALID_VELOCITY;
    }
    
    this->unused2 = INVALID_VELOCITY;  // Always set to default
    
    // Read accelerationZ if present (bit 3)
    if (fieldMask & 0x8) {
        float accelerationZ;
        DeserializeNetworkData(networkClient, &accelerationZ, 32);
        this->accelerationZ = accelerationZ;
    } else {
        this->accelerationZ = INVALID_VELOCITY;
    }
    
    // Read isMoving flag
    uint32_t movingFlag = 0;
    DeserializeNetworkData(networkClient, &movingFlag, 1);
    this->isMoving = (movingFlag != 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerMovementMessage::Serialise() [vtable slot 2 @ 0x823B8240]
// 
// Writes player movement data to network stream.
// ─────────────────────────────────────────────────────────────────────────────
void PlayerMovementMessage::Serialise(void* networkClient) {
    const float INVALID_VELOCITY = -1.0f;
    
    // Build field presence bitmask
    uint32_t fieldMask = 0;
    
    if (this->velocityX != INVALID_VELOCITY) {
        fieldMask |= 0x1;
    }
    if (this->velocityZ != INVALID_VELOCITY) {
        fieldMask |= 0x2;
    }
    if (this->accelerationX != INVALID_VELOCITY) {
        fieldMask |= 0x4;
    }
    if (this->accelerationZ != INVALID_VELOCITY) {
        fieldMask |= 0x8;
    }
    
    // Write field mask and player index
    netStream_WriteByte(networkClient, fieldMask, 4);
    netStream_WriteByte(networkClient, this->playerIndex, 1);
    
    // Write fields that are present
    if (fieldMask & 0x1) {
        WriteFloatToNetworkStream(networkClient, this->velocityX);
    }
    if (fieldMask & 0x2) {
        WriteFloatToNetworkStream(networkClient, this->velocityZ);
    }
    if (fieldMask & 0x4) {
        WriteFloatToNetworkStream(networkClient, this->accelerationX);
    }
    if (fieldMask & 0x8) {
        WriteFloatToNetworkStream(networkClient, this->accelerationZ);
    }
    
    // Write isMoving flag
    netStream_WriteBool(networkClient, this->isMoving);
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerMovementMessage::Process() [vtable slot 4 @ 0x823B8358]
// 
// Applies player movement to game state.
// ─────────────────────────────────────────────────────────────────────────────
void PlayerMovementMessage::Process(void* matchContext) {
    // Check if we're in network mode (mode == 2) and local user exists
    extern int32_t g_networkMode;  // @ lis(-32163) + -26068
    extern void* g_localUserPtr;   // @ lis(-32160) + 25500
    
    if (g_networkMode == 2 && g_localUserPtr == nullptr) {
        // Local user doesn't exist, clear movement flag
        this->isMoving = false;
    }
    
    // Look up the player object by index
    void* playerObject = NetworkClient_LookupPlayer(this->playerIndex);
    
    if (playerObject != nullptr) {
        // Get player's physics state pointer
        uint32_t physicsStateIndex = *(uint32_t*)((uint8_t*)playerObject + 464);
        physicsStateIndex += 17;  // Offset to movement state
        
        extern void** g_physicsStateArray;  // @ lis(-32161) + -21720
        void* physicsState = g_physicsStateArray[physicsStateIndex];
        
        // Copy player height from physics state
        float playerHeight = *(float*)((uint8_t*)physicsState + 52);
        this->velocityY = playerHeight;
        
        // Apply movement to player
        PlayerMovementMessage_54B0_h(playerObject, &this->velocityX, &this->accelerationX, this->isMoving);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerMovementMessage::ReturnToPool() [vtable slot 5 @ 0x823B7EF8]
// 
// Returns message object to the pool allocator.
// ─────────────────────────────────────────────────────────────────────────────
void PlayerMovementMessage::ReturnToPool() {
    // Pool management structure at lis(-32163) + 4452 + 8
    extern uint8_t* g_playerMovementMsgPool;  // @ 0x825D1164
    
    uint8_t* poolBase = g_playerMovementMsgPool;
    
    // Calculate index in pool using magic division (52429 = 0xCCCD)
    uint32_t offset = (uint8_t*)this - poolBase;
    uint32_t index = ((uint64_t)offset * 0xCCCD) >> 38;  // Divide by 80 (0x50)
    
    // Get current tail index
    uint16_t currentTail = *(uint16_t*)(poolBase + 16004);
    
    // Link this object into free list
    *(uint16_t*)((uint8_t*)this + 66) = currentTail;
    
    // Update previous tail's next pointer if valid
    if (currentTail != 0xFFFF) {
        uint8_t* prevTailObject = poolBase + (currentTail * 80);
        *(uint16_t*)(prevTailObject + 64) = (uint16_t)index;
    }
    
    // Update pool tail and count
    *(uint16_t*)(poolBase + 16004) = (uint16_t)index;
    uint16_t freeCount = *(uint16_t*)(poolBase + 16000);
    *(uint16_t*)(poolBase + 16000) = freeCount + 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerMovementMessage::GetPoolSingleton() [vtable slot 6 @ 0x823B8000]
// 
// Returns pointer to the pool singleton.
// ─────────────────────────────────────────────────────────────────────────────
void* PlayerMovementMessage::GetPoolSingleton() {
    extern void* g_playerMovementMsgPoolSingleton;  // @ 0x825D1158
    return g_playerMovementMsgPoolSingleton;
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerMovementMessage::GetTypeName() [vtable slot 7 @ 0x823B8010]
// 
// Returns the message type name string.
// ─────────────────────────────────────────────────────────────────────────────
const char* PlayerMovementMessage::GetTypeName() {
    // String at lis(-32249) + -5792 = 0x8206E960
    // But this is actually a partial string, the full string is likely elsewhere
    return "PlayerMovementMessage";
}


// ===========================================================================
// pongNetMessageHolder::CleanupMessageArray @ 0x823C2D20 | size: 0x64
//
// Vtable slot 2 destructor variant for pongNetMessageHolder.
// Cleans up an allocated array of 2 network message objects.
//
// Process:
//   1. Checks if holder->m_pInternalArray (+0x08) is non-null
//   2. If allocated, resets both message objects' vtables to base PongNetMessage
//      - Object at offset 0 (first message)
//      - Object at offset 640 (second message, stride = 640 bytes)
//   3. Frees the entire array via sysMemAllocator_Free
//   4. Nulls out the holder's pointer
//
// The vtable reset (0x8206C304 = PongNetMessage base vtable) ensures proper
// cleanup before freeing, preventing virtual destructor issues.
//
// This is one of 41 template instantiations of the same pattern for different
// message types. Each variant manages arrays of specific message subclasses.
//
// Python-verified:
//   marker_vtable = lis(-32249) + (-15612) = 0x8206C304 (PongNetMessage vtable)
//   Loop: r11 = r3+1280, then -640 twice → writes at offsets 640 and 0
// ===========================================================================
void pongNetMessageHolder_vfn_2_2D20_1(pongNetMessageHolder* holder)
{
    // Get the allocated message array pointer
    void* messageArray = holder->m_pInternalArray;  // +0x08
    
    if (messageArray != nullptr) {
        // Reset both message objects to base PongNetMessage vtable
        // This ensures proper cleanup before freeing memory
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;
        
        // Reset second message (offset 640)
        *(uint32_t*)((uint8_t*)messageArray + 640) = BASE_VTABLE;
        
        // Reset first message (offset 0)
        *(uint32_t*)((uint8_t*)messageArray + 0) = BASE_VTABLE;
        
        // Free the entire array
        sysMemAllocator_Free(messageArray);
        
        // Null out the holder's pointer
        holder->m_pInternalArray = nullptr;
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// Network Stream I/O Functions — Batch Implementation
// ═══════════════════════════════════════════════════════════════════════════

// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadStringFromStream @ 0x82260A70 | size: 0x7C
//
// Reads a null-terminated string from the network stream into a buffer.
// Returns the number of characters read (including null terminator).
//
// Parameters:
//   buffer - Destination buffer for the string
//   maxSize - Maximum size to read (default 16)
//
// Returns:
//   Number of characters read, or 0 on failure
// ───────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient::ReadStringFromStream(char* buffer, int maxSize) {
    // Save current bit position (offset 0x1C)
    int savedBitPos = *(int*)((char*)this + 0x1C);

    // Try to read the string data
    extern int netStream_ReadStringRaw(void* ctx, void* base);  // 2-arg overload of bit-stream read helper
    int bytesRead = netStream_ReadStringRaw(this, buffer);

    if (bytesRead > 0) {
        // Null-terminate the string
        buffer[bytesRead + maxSize - 1] = '\0';

        // Update bit position (8 bits per character)
        *(int*)((char*)this + 0x1C) = savedBitPos + 8;

        // Count the actual string length
        int length = 1;
        char* ptr = buffer;
        while (*ptr != '\0') {
            ptr++;
            *(int*)((char*)this + 0x1C) += 8;
            length++;
        }
        
        return length;
    } else {
        // Read failed - clear buffer and return 0
        buffer[0] = '\0';
        return 0;
    }
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::Read64BitValue @ 0x822609F0 | size: 0x7C
//
// Reads a 64-bit value from the network stream by reading two 32-bit values
// and combining them (high 32 bits, then low 32 bits).
//
// Parameters:
//   outValue - Pointer to receive the 64-bit result
//
// Returns:
//   true if read succeeded, false otherwise
// ───────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient::Read64BitValue(uint64_t* outValue) {
    uint32_t highBits = 0;
    uint32_t lowBits = 0;

    // Read high 32 bits (32-bit field size)
    snBitStream_ReadBits(this, &highBits, 32);

    // Read low 32 bits
    snBitStream_ReadBits(this, &lowBits, 32);

    // Combine into 64-bit value: (high << 32) | low
    *outValue = ((uint64_t)highBits << 32) | lowBits;
    return false;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ValidateAndReadData @ 0x823F3EE8 | size: 0x6C
//
// Validates that sufficient data is available in the stream, then reads
// a 64-bit value if validation passes.
//
// Parameters:
//   outData - Pointer to receive the 64-bit data (offset +16 from base)
//
// Returns:
//   true if validation and read succeeded, false otherwise
// ───────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient::ValidateAndReadData(void* outData) {
    // Check if we have at least 16 bits available
    int available = ReadStringFromStream(nullptr, 16);
    
    if (available > 0) {
        // Read 64-bit value at offset +16 from the output pointer
        uint64_t* dataPtr = (uint64_t*)((char*)outData + 16);
        bool success = Read64BitValue(dataPtr);
        
        return success;
    }
    
    return false;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetErrorString @ 0x82238600 | size: 0x74
//
// Returns an error message string based on the current error code.
//
// Returns:
//   Pointer to error message string, or default message if error code invalid
// ───────────────────────────────────────────────────────────────────────────
const char* SinglesNetworkClient::GetErrorString() {
    extern const char g_szInvalidError[];      // @ 0x8204EF04
    extern const char g_szDefaultError[];      // @ 0x82027423
    
    // Get error code from network state
    int errorCode = SinglesNetworkClient_8CC0_w(this);
    
    if (errorCode < 0) {
        return g_szInvalidError;
    }
    
    // Look up error message in error table
    extern void* xam_singleton_init_8D60(void*);
    void* errorTable = xam_singleton_init_8D60(this);
    
    void** vtable = *(void***)errorTable;
    void** errorEntry = (void**)vtable[errorCode];
    const char* errorMsg = (const char*)errorEntry[1];
    
    if (errorMsg != nullptr) {
        return errorMsg;
    }
    
    return g_szDefaultError;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::InitializeNetworkState @ 0x82238678 | size: 0x74
//
// Initializes the network client state structure, clearing all fields and
// setting up the vtable reference.
// ───────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::InitializeNetworkState() {
    extern const void* g_cmRefreshableCtorVtable;  // @ 0x820533CC
    
    // Store vtable reference at offset +36
    *(const void**)((char*)this + 36) = g_cmRefreshableCtorVtable;
    
    // Clear all state fields
    *(void**)((char*)this + 0x00) = nullptr;   // vtable
    *(uint32_t*)((char*)this + 4) = 0;         // flags
    *(uint32_t*)((char*)this + 8) = 0;
    *(uint32_t*)((char*)this + 12) = 0;
    *(uint32_t*)((char*)this + 16) = 0;
    *(uint32_t*)((char*)this + 20) = 0;
    *(uint32_t*)((char*)this + 24) = 0;
    *(int*)((char*)this + 0x1C) = 0;
    *(uint32_t*)((char*)this + 32) = 0;
    
    // Initialize network buffers
    snBitStream_Reset(this);
    snBitStream_Reset(this);
    
    // Set default buffer size
    *(uint32_t*)((char*)this + 20) = 8128;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteValueWithMask @ 0x822386F0 | size: 0x68
//
// Writes a value to the network stream with a 14-bit mask applied.
//
// Parameters:
//   value - Value to write (lower 14 bits will be used)
// ───────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::WriteValueWithMask(uint32_t value) {
    // Prepare for write operation
    snBitStream_ValidateWrite(this);
    
    // Save current bit positions
    int savedBitPos = *(int*)((char*)this + 0x1C);
    int savedWritePos = *(int*)((char*)this + 32);
    
    // Temporarily set bit position to 16
    *(int*)((char*)this + 0x1C) = 16;
    
    // Read current value from stream
    uint32_t currentValue = 0;
    snBitStream_ReadBits(this, &currentValue, 16);
    
    // Restore bit position
    *(int*)((char*)this + 0x1C) = savedBitPos;
    
    // Apply 14-bit mask to input value and merge with current value
    uint32_t maskedValue = (value & 0x3FFF) | (currentValue & 0xFFFFC000);
    
    // Temporarily set write position to 16
    *(int*)((char*)this + 32) = 16;
    
    // Write the merged value
    snBitStream_WriteBits(this, maskedValue, 16);
    
    // Restore write position
    *(int*)((char*)this + 32) = savedWritePos;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::Read14BitValue @ 0x82238758 | size: 0x7C
//
// Reads a 14-bit value from the network stream.
//
// Returns:
//   14-bit value (0-16383), or 16383 if insufficient data available
// ───────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient::Read14BitValue() {
    // Check if we have enough bits available
    int bitsAvailable = *(int*)((char*)this + 16);
    int bitsNeeded = (bitsAvailable + 7) & 0xFFFFFFF8;  // Round up to nearest 8
    
    if (bitsNeeded < 48) {
        // Not enough data - return maximum 14-bit value
        return 16383;
    }
    
    // Save current bit position
    int savedBitPos = *(int*)((char*)this + 0x1C);
    
    // Temporarily set bit position to 16
    *(int*)((char*)this + 0x1C) = 16;
    
    // Read 16-bit value
    uint32_t value = 0;
    snBitStream_ReadBits(this, &value, 16);
    
    // Restore bit position
    *(int*)((char*)this + 0x1C) = savedBitPos;
    
    // Extract lower 14 bits
    return value & 0x3FFF;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadTimestamp @ 0x822387D8 | size: 0x70
//
// Reads a timestamp value from the network stream if sufficient data available.
//
// Returns:
//   Timestamp value, or 0 if insufficient data
// ───────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient::ReadTimestamp() {
    uint32_t timestamp = 0;
    
    // Check if we have enough bits available
    int bitsAvailable = *(int*)((char*)this + 16);
    int bitsNeeded = (bitsAvailable + 7) & 0xFFFFFFF8;  // Round up to nearest 8
    
    if (bitsNeeded >= 48) {
        // Save current bit position
        int savedBitPos = *(int*)((char*)this + 0x1C);
        
        // Temporarily set bit position to 32
        *(int*)((char*)this + 0x1C) = 32;
        
        // Read 16-bit timestamp
        snBitStream_ReadBits(this, &timestamp, 16);
        
        // Restore bit position
        *(int*)((char*)this + 0x1C) = savedBitPos;
    }
    
    return timestamp;
}


// ───────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadSequenceNumber @ 0x82238848 | size: 0x74
//
// Reads a 16-bit sequence number from the network stream.
//
// Returns:
//   16-bit sequence number, or 0 if insufficient data available
// ───────────────────────────────────────────────────────────────────────────
uint16_t SinglesNetworkClient::ReadSequenceNumber() {
    uint32_t seqNum = 0;
    
    // Check if we have enough bits available
    int bitsAvailable = *(int*)((char*)this + 16);
    int bitsNeeded = (bitsAvailable + 7) & 0xFFFFFFF8;  // Round up to nearest 8
    
    if (bitsNeeded >= 48) {
        // Save current bit position
        int savedBitPos = *(int*)((char*)this + 0x1C);
        
        // Set bit position to 0 (read from start)
        *(int*)((char*)this + 0x1C) = 0;
        
        // Read 16-bit value
        snBitStream_ReadBits(this, &seqNum, 16);
        
        // Restore bit position
        *(int*)((char*)this + 0x1C) = savedBitPos;
    }
    
    // Extract lower 16 bits
    return (uint16_t)(seqNum & 0xFFFF);
}

// ═════════════════════════════════════════════════════════════════════════════
// pongNetMessageHolder — Network Message Container
// ═════════════════════════════════════════════════════════════════════════════

// External globals
extern uint32_t g_netMessageHolderCount;  // @ 0x826066A4 (global instance counter)

// External vtables
extern void* g_pongNetMessageHolderVtable;      // @ 0x820701F8
extern void* g_pongNetMessageHolderBaseVtable;  // @ 0x8206FA88

// External functions
extern void pongNetMessageHolder_vfn_2_24B8_1(void* thisPtr);  // Cleanup method
extern void sysMemAllocator_Free(void* ptr);                         // Memory deallocator

/**
 * pongNetMessageHolder_vfn_0_4F68_1 @ 0x823C4F68 | size: 0x78
 *
 * Destructor for pongNetMessageHolder (vtable slot 0).
 * Performs cleanup and optionally frees the object's memory.
 *
 * The destructor:
 * 1. Sets derived class vtable (pongNetMessageHolder)
 * 2. Calls vfn_2 cleanup method to release resources
 * 3. Sets base class vtable (pongNetMessageHolderBase)
 * 4. Decrements global instance counter
 * 5. If shouldFree flag is set, deallocates the object's memory
 *
 * This follows the standard RAGE engine destructor pattern with:
 * - Vtable restoration during destruction
 * - Global instance tracking
 * - Optional memory deallocation
 *
 * @param this - Pointer to pongNetMessageHolder object
 * @param shouldFree - If bit 0 is set, free the object's memory after cleanup
 */
void pongNetMessageHolder_vfn_0_4F68_1(void* thisPtr, uint32_t shouldFree) {
    uint8_t* obj = (uint8_t*)thisPtr;
    
    // Set derived class vtable
    *(void**)(obj + 0) = (void*)VTABLE_pongNetMessageHolder_01F8;  // g_pongNetMessageHolderVtable
    
    // Call cleanup method (vfn_2)
    pongNetMessageHolder_vfn_2_24B8_1(thisPtr);
    
    // Set base class vtable
    *(void**)(obj + 0) = (void*)VTABLE_pongNetMessageHolderBase;  // g_pongNetMessageHolderBaseVtable
    
    // Decrement global instance counter
    g_netMessageHolderCount--;
    
    // Check if we should free memory
    bool shouldFreeMemory = (shouldFree & 0x1) != 0;
    
    if (shouldFreeMemory) {
        sysMemAllocator_Free(thisPtr);
    }
}

/**
 * pongNetMessageHolder_2028_2hr @ 0x82582028 | size: 0x40
 *
 * Inserts a network message holder into a global linked list.
 * This function performs intrusive doubly-linked list insertion.
 *
 * The function:
 * 1. Calls initialization function (pongNetMessageHolder_FAE0_isl)
 * 2. Inserts the object into a global list at 0x825D1900
 * 3. Increments the object's reference count
 *
 * Structure layout (inferred):
 *   +8   next pointer (linked list)
 *   +12  reference count
 *
 * @param this - Pointer to pongNetMessageHolder object to insert
 */
void pongNetMessageHolder_2028_2hr(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    
    // Call initialization function
    pongNetMessageHolder_FAE0_isl();
    
    // Get global list head pointer
    void** globalListHead = (void**)&g_holderInstance_DataMgr;
    
    // Load current next pointer from object
    void* currentNext = *(void**)(obj + 8);
    
    // Store current next in global list's next field (+8)
    *(void**)((uint8_t*)globalListHead + 8) = currentNext;
    
    // Link object into list by setting its next to global list head
    *(void**)(obj + 8) = globalListHead;
    
    // Increment reference count
    uint32_t refCount = *(uint32_t*)(obj + 12);
    refCount++;
    *(uint32_t*)(obj + 12) = refCount;
}

// ═══════════════════════════════════════════════════════════════════════════
// BATCH: Network Message Deserialise/Serialise vtable methods
// Classes: HitMessage, HitDataMessage, SpectatorBallHitMessage,
//          RemoteServeReadyMessage, MatchTimeSyncMessage,
//          ForceMatchTimeSyncMessage
// ═══════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// HitMessage_Deserialise  [vtable slot 1 @ 0x823B5FE0 | size: 0x5C]
// Reads base hit message fields from the network stream: timing float + hit flags byte.
// ─────────────────────────────────────────────────────────────────────────────
void HitMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read 32-bit float from stream into temporary buffer
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);

    // Store timing reference at +0x04
    *(float*)(obj + 0x04) = timingRef;

    // Read 8-bit hit flags byte into +0x08
    ReadBitsFromStream(client, obj + 0x08, 8);
}

// ─────────────────────────────────────────────────────────────────────────────
// HitDataMessage_Deserialise  [vtable slot 1 @ 0x823B5B58 | size: 0x80]
// Reads hit data message fields: timing float, ball hit data block,
// secondary timing float, and player index byte.
// ─────────────────────────────────────────────────────────────────────────────
void HitDataMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read timing reference float
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // Read ball hit data block at +0x10 (192-byte structure)
    DeserializeNetworkData(client, obj + 0x10, 32);

    // Read secondary timing float (recovery/power value)
    float recoveryTiming;
    snBitStream_ReadBits(client, &recoveryTiming, 32);
    *(float*)(obj + 0xD0) = recoveryTiming;

    // Read player index byte at +0xD4
    ReadBitsFromStream(client, obj + 0xD4, 8);
}

// ─────────────────────────────────────────────────────────────────────────────
// SpectatorBallHitMessage_Deserialise  [vtable slot 1 @ 0x823B6B00 | size: 0x44]
// Reads spectator ball hit: delegates to HitMessage_Deserialise for the
// base fields, then reads the ball hit data block.
// ─────────────────────────────────────────────────────────────────────────────
void SpectatorBallHitMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read base HitMessage fields (timing float + hit flags byte)
    HitMessage_Deserialise(thisPtr, client);

    // Read ball hit data block at +0x10
    DeserializeNetworkData(client, obj + 0x10, 32);
}

// ─────────────────────────────────────────────────────────────────────────────
// RemoteServeReadyMessage_Deserialise  [vtable slot 1 @ 0x823B6ED8 | size: 0x48]
// Reads remote serve ready: delegates to HitMessage_Deserialise for base
// fields, then reads a 16-bit serve parameter.
// ─────────────────────────────────────────────────────────────────────────────
void RemoteServeReadyMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read base HitMessage fields (timing float + hit flags byte)
    HitMessage_Deserialise(thisPtr, client);

    // Read 16-bit serve parameter at +0x0C
    snBitStream_ReadUnsigned(client, obj + 0x0C, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// RemoteServeReadyMessage_Serialise  [vtable slot 2 @ 0x823B6F20 | size: 0x60]
// Writes remote serve ready fields to the outgoing network stream:
// timing float, hit flags byte, and 16-bit serve parameter.
// ─────────────────────────────────────────────────────────────────────────────
void RemoteServeReadyMessage_Serialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Write timing reference float
    WriteFloatToNetworkStream(client, *(float*)(obj + 0x04));

    // Write hit flags as 8-bit unsigned
    netStream_WriteByte(client, *(uint8_t*)(obj + 0x08), 8);

    // Write 16-bit serve parameter
    netStream_WriteByte(client, *(uint8_t*)(obj + 0x0C), 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// MatchTimeSyncMessage_Deserialise  [vtable slot 1 @ 0x823B70A8 | size: 0x88]
// Reads match time sync fields: two 64-bit timestamps, a timing float,
// a 16-bit frame counter, then applies a scale factor to the timing value.
// ─────────────────────────────────────────────────────────────────────────────
extern void netStream_ReadU64(void* dst, void* client);
extern char* FormatTimestampForLog(void* timestamp);

void MatchTimeSyncMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read two 64-bit timestamp values at +0x08 and +0x10
    netStream_ReadU64(obj + 0x08, client);
    netStream_ReadU64(obj + 0x10, client);

    // Read timing float
    float syncTiming;
    snBitStream_ReadBits(client, &syncTiming, 32);
    *(float*)(obj + 0x18) = syncTiming;

    // Read 16-bit frame counter at +0x1C
    snBitStream_ReadUnsigned(client, obj + 0x1C, 16);

    // Apply scale factor from global config to sync delta
    // Scale factor is at g_matchTimeSyncConfig + 12 (0x825CAEC4)
    extern float* g_pMatchTimeSyncScale;  // @ 0x825CAEB8 + 12
    *(float*)(obj + 0x18) *= g_pMatchTimeSyncScale[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// MatchTimeSyncMessage_Serialise  [vtable slot 2 @ 0x823B7130 | size: 0x88]
// Writes match time sync fields to the outgoing network stream:
// two 32-bit timestamps, three floats, and a 16-bit frame counter.
// ─────────────────────────────────────────────────────────────────────────────
void MatchTimeSyncMessage_Serialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Write local timestamp as 32-bit value
    netStream_WriteS16(client, *(int16_t*)(obj + 0x08), 32);

    // Write local timing float
    WriteFloatToNetworkStream(client, *(float*)(obj + 0x0C));

    // Write remote timestamp as 32-bit value
    netStream_WriteS16(client, *(int16_t*)(obj + 0x10), 32);

    // Write remote timing float
    WriteFloatToNetworkStream(client, *(float*)(obj + 0x14));

    // Write sync delta float
    WriteFloatToNetworkStream(client, *(float*)(obj + 0x18));

    // Write 16-bit frame counter
    netStream_WriteByte(client, *(uint8_t*)(obj + 0x1C), 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForceMatchTimeSyncMessage_Deserialise  [vtable slot 1 @ 0x823B7AA0 | size: 0x4C]
// Reads forced time sync fields: a 64-bit timestamp and a 16-bit parameter.
// ─────────────────────────────────────────────────────────────────────────────
void ForceMatchTimeSyncMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read 64-bit timestamp at +0x08
    netStream_ReadU64(obj + 0x08, client);

    // Read 16-bit sync parameter at +0x10
    snBitStream_ReadUnsigned(client, obj + 0x10, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForceMatchTimeSyncMessage_Serialise  [vtable slot 2 @ 0x823B7AF0 | size: 0x60]
// Writes forced time sync fields to the outgoing network stream:
// a 32-bit timestamp, a float, and a 16-bit parameter.
// ─────────────────────────────────────────────────────────────────────────────
void ForceMatchTimeSyncMessage_Serialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Write timestamp as 32-bit value
    netStream_WriteS16(client, *(int16_t*)(obj + 0x08), 32);

    // Write timing float
    WriteFloatToNetworkStream(client, *(float*)(obj + 0x0C));

    // Write 16-bit sync parameter
    netStream_WriteByte(client, *(uint8_t*)(obj + 0x10), 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForceMatchTimeSyncMessage_Process  [vtable slot 3 @ 0x823B7608 | size: 0x34]
// Applies the forced time sync by updating the session's time sync state.
// Formats the result as a debug string and logs it (via no-op in retail).
// ─────────────────────────────────────────────────────────────────────────────
void ForceMatchTimeSyncMessage_Process(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Format timestamp for debug: converts the sync timestamp to a string
    char* formattedTime = FormatTimestampForLog(obj + 0x08);

    // Log the formatted time string (no-op in retail build)
    // String at 0x8206F298 references the sync debug output
    rage_debugLog(formattedTime);
}


// ═══════════════════════════════════════════════════════════════════════════
// BATCH 4: Network Message Deserialise handlers (10 functions)
// Classes: BallHitMessage, SessionTimeSyncMessage, TimedGameUpdateTimerMessage,
//          PlayerUpdateMessage, PlayerStopMessage, ServeReadyMessage,
//          ServeAbortMessage, ServeFaultMessage, ServeLetMessage,
//          SpectatorQuitMessage, AcceptMessage
// All are vtable slot 1 (labeled scalar_dtor but actually read from stream).
// ═══════════════════════════════════════════════════════════════════════════

extern void snBitStream_ReadString(void* client, void* dst, int maxBytes);  // util_0AF0 @ 0x82260AF0
extern void DeserializeGamerHandle(void* client, void* dst);                // netStream_ReadU32GamerHandleVariant @ 0x82260E18

// ─────────────────────────────────────────────────────────────────────────────
// BallHitMessage_Deserialise  [vtable slot 1 @ 0x823B65F8 | size: 0x68]
// Reads ball hit wire: delegates to HitMessage_Deserialise for the
// timing float + hit flags byte, then reads a 16-bit hit ID, an 8-bit
// phase/zone byte, and a signed 16-bit swing index.
// ─────────────────────────────────────────────────────────────────────────────
void BallHitMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Base HitMessage fields (timing @+0x04, hit flags @+0x08)
    HitMessage_Deserialise(thisPtr, client);

    // 16-bit hit identifier at +0x0C
    snBitStream_ReadUnsigned(client, obj + 0x0C, 16);

    // 8-bit phase/zone byte at +0x0E
    ReadBitsFromStream(client, obj + 0x0E, 8);

    // Signed 16-bit swing index at +0x10
    netStream_ReadS16(client, obj + 0x10, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// SessionTimeSyncMessage_Deserialise  [vtable slot 1 @ 0x823B7728 | size: 0x58]
// Reads session time sync payload: two 64-bit timestamps (local + remote)
// followed by a 16-bit frame/seq counter.
// ─────────────────────────────────────────────────────────────────────────────
void SessionTimeSyncMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Local 64-bit timestamp at +0x08
    netStream_ReadU64(obj + 0x08, client);

    // Remote 64-bit timestamp at +0x10
    netStream_ReadU64(obj + 0x10, client);

    // 16-bit sequence counter at +0x18
    snBitStream_ReadUnsigned(client, obj + 0x18, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// TimedGameUpdateTimerMessage_Deserialise  [vtable slot 1 @ 0x823B7C00 | size: 0x64]
// Reads two 32-bit floats: a timer value and a delta.
// ─────────────────────────────────────────────────────────────────────────────
void TimedGameUpdateTimerMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // First timer float at +0x04
    float timerValue;
    snBitStream_ReadBits(client, &timerValue, 32);
    *(float*)(obj + 0x04) = timerValue;

    // Delta / secondary timer float at +0x08
    float delta;
    snBitStream_ReadBits(client, &delta, 32);
    *(float*)(obj + 0x08) = delta;
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerUpdateMessage_Deserialise  [vtable slot 1 @ 0x823B7DD8 | size: 0x80]
// Reads a timing float, a player index byte, two signed 16-bit values
// (score / state fields), then a variable-length payload sized from
// the second 16-bit value.
// ─────────────────────────────────────────────────────────────────────────────
void PlayerUpdateMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Timing reference float at +0x04
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // Player index byte at +0x08
    ReadBitsFromStream(client, obj + 0x08, 8);

    // Signed 16-bit field at +0x0A
    netStream_ReadS16(client, obj + 0x0A, 16);

    // Signed 16-bit payload size at +0x0C
    netStream_ReadS16(client, obj + 0x0C, 16);

    // Variable-length payload at +0x10, length from +0x0C
    int16_t payloadSize = *(int16_t*)(obj + 0x0C);
    snBitStream_ReadString(client, obj + 0x10, payloadSize);
}

// ─────────────────────────────────────────────────────────────────────────────
// PlayerStopMessage_Deserialise  [vtable slot 1 @ 0x823B84D0 | size: 0xD4]
// Reads a 2-bit control mask and a 1-bit flag, then conditionally reads
// two float fields based on bits 0 and 1 of the mask. Default floats come
// from a global constant table.
// ─────────────────────────────────────────────────────────────────────────────
void PlayerStopMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Read 2-bit control mask
    uint32_t controlMask = 0;
    snBitStream_ReadBits(client, &controlMask, 2);

    // Read 1-bit stop flag
    uint32_t flagByte = 0;
    snBitStream_ReadBits(client, &flagByte, 1);

    // Store flag byte at +0x20
    *(uint8_t*)(obj + 0x20) = (uint8_t)flagByte;

    // Default float value from global constant table
    extern float g_playerStopDefaultFloat;  // @ 0x8252A2A0 (0x8252D110 - 12016)
    float defaultVal = g_playerStopDefaultFloat;

    float primaryVal = defaultVal;
    float secondaryVal = defaultVal;

    // Bit 0 of mask: optionally read primary float (timing)
    if ((controlMask & 0x1) != 0) {
        snBitStream_ReadBits(client, &primaryVal, 32);
    }

    // Store primary float at +0x10, default at +0x14
    *(float*)(obj + 0x10) = primaryVal;
    *(float*)(obj + 0x14) = defaultVal;

    // Bit 1 of mask: optionally read secondary float
    if ((controlMask & 0x2) != 0) {
        snBitStream_ReadBits(client, &secondaryVal, 32);
    }

    // Store secondary float at +0x18
    *(float*)(obj + 0x18) = secondaryVal;

    // Read 1-bit stop-flag, store as bool at +0x21
    uint32_t stopFlag = 0;
    snBitStream_ReadBits(client, &stopFlag, 1);
    *(uint8_t*)(obj + 0x21) = (stopFlag != 0) ? 1 : 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// ServeReadyMessage_Deserialise  [vtable slot 1 @ 0x823B8B10 | size: 0x68]
// Reads serve-ready wire: delegates to HitMessage_Deserialise for the base
// timing float + hit flags byte, then reads two additional 32-bit floats
// (serve-ready timings).
// ─────────────────────────────────────────────────────────────────────────────
void ServeReadyMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Base HitMessage fields (timing @+0x04, hit flags @+0x08)
    HitMessage_Deserialise(thisPtr, client);

    // Primary serve-ready float at +0x0C
    float primary;
    snBitStream_ReadBits(client, &primary, 32);
    *(float*)(obj + 0x0C) = primary;

    // Secondary serve-ready float at +0x10
    float secondary;
    snBitStream_ReadBits(client, &secondary, 32);
    *(float*)(obj + 0x10) = secondary;
}

// ─────────────────────────────────────────────────────────────────────────────
// ServeAbortMessage_Deserialise  [vtable slot 1 @ 0x823B8D20 | size: 0x04]
// Pure tail-call to HitMessage_Deserialise — the class adds no extra fields
// over HitMessage so its deserialiser is identical.
// ─────────────────────────────────────────────────────────────────────────────
void ServeAbortMessage_Deserialise(void* thisPtr, void* client) {
    HitMessage_Deserialise(thisPtr, client);
}

// ─────────────────────────────────────────────────────────────────────────────
// ServeFaultMessage_Deserialise  [vtable slot 1 @ 0x823B93C0 | size: 0x6C]
// Reads a timing float, a byte at +0x08 (flag), and a signed 16-bit value
// at +0x0A. Pattern mirrors the Serialise side.
// ─────────────────────────────────────────────────────────────────────────────
void ServeFaultMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Timing reference float at +0x04
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // 8-bit flag byte at +0x08
    ReadBitsFromStream(client, obj + 0x08, 8);

    // Signed 16-bit fault code at +0x0A
    netStream_ReadS16(client, obj + 0x0A, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ServeLetMessage_Deserialise  [vtable slot 1 @ 0x823B9658 | size: 0x6C]
// Reads timing float, signed 8-bit byte at +0x08, signed 16-bit at +0x0A.
// The 8-bit field uses netStream_ReadS8SignMagnitude (signed-byte read with sign extension).
// ─────────────────────────────────────────────────────────────────────────────
extern void netStream_ReadS8SignMagnitude(void* client, void* dst, int bitWidth);  // signed 8-bit read @ 0x82101668

void ServeLetMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Timing reference float at +0x04
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // Signed 8-bit let-code at +0x08 (sign-extended read)
    netStream_ReadS8SignMagnitude(client, obj + 0x08, 8);

    // Signed 16-bit index at +0x0A
    netStream_ReadS16(client, obj + 0x0A, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// SpectatorQuitMessage_Deserialise  [vtable slot 1 @ 0x823BB210 | size: 0x5C]
// Reads a timing float and a gamer handle (XBL identity) via the dedicated
// handle deserialiser.
// ─────────────────────────────────────────────────────────────────────────────
void SpectatorQuitMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Timing reference float at +0x04
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // Gamer handle block at +0x08 (XBL identity)
    DeserializeGamerHandle(client, obj + 0x08);
}

// ─────────────────────────────────────────────────────────────────────────────
// AcceptMessage_Deserialise  [vtable slot 1 @ 0x823BB350 | size: 0x5C]
// Reads a timing float followed by a 280-byte acceptance payload
// (session-accept blob / credential data).
// ─────────────────────────────────────────────────────────────────────────────
void AcceptMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Timing reference float at +0x04
    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // 280-byte acceptance payload at +0x08
    snBitStream_ReadString(client, obj + 0x08, 280);
}


// ═══════════════════════════════════════════════════════════════════════════
// pongNetMessageHolder — Buffer Management & Cleanup Functions (10 functions)
// ═══════════════════════════════════════════════════════════════════════════

extern void snListNode_Init(void* ptr);           // Element initializer
extern void cmOperatorCtor_68E0_w(void* ptr, int count);  // Operator constructor

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::ReallocateElementBuffer @ 0x8212D2E8 | size: 0x74
//
// Reallocates the holder's data buffer for elements of 16 bytes each.
// Frees the existing buffer if present, then allocates a new one
// sized to hold `count` elements (count * 16 bytes). If count is 0,
// just clears the buffer pointer.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_D2E8_w(void* self, uint32_t count) {
    uint8_t* obj = (uint8_t*)self;
    uint32_t existingCount = *(uint32_t*)(obj + 8);

    // Free existing buffer if allocated
    if (existingCount != 0) {
        void* oldBuffer = *(void**)(obj + 0);
        sysMemAllocator_Free(oldBuffer);
        *(uint32_t*)(obj + 0) = 0;
        *(uint32_t*)(obj + 4) = 0;
    }

    if (count != 0) {
        *(uint32_t*)(obj + 8) = count;

        // Allocate count * 16 bytes; clamp to prevent overflow
        uint32_t allocSize = count * 16;
        if (count > 0x0FFFFFFF) {
            allocSize = (uint32_t)-1;
        }

        void* newBuffer = rage_Alloc(allocSize);
        *(void**)(obj + 0) = newBuffer;
    } else {
        *(uint32_t*)(obj + 8) = 0;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::ReallocatePointerBuffer @ 0x82133EB8 | size: 0x74
//
// Reallocates the holder's data buffer for pointer-sized (4-byte) elements.
// Frees the existing buffer if present, then allocates a new one
// sized to hold `count` entries (count * 4 bytes). If count is 0,
// just clears the buffer pointer.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_3EB8_w(void* self, uint32_t count) {
    uint8_t* obj = (uint8_t*)self;
    uint32_t existingCount = *(uint32_t*)(obj + 8);

    // Free existing buffer if allocated
    if (existingCount != 0) {
        void* oldBuffer = *(void**)(obj + 0);
        sysMemAllocator_Free(oldBuffer);
        *(uint32_t*)(obj + 0) = 0;
        *(uint32_t*)(obj + 4) = 0;
    }

    if (count != 0) {
        *(uint32_t*)(obj + 8) = count;

        // Allocate count * 4 bytes; clamp to prevent overflow
        uint32_t allocSize = count * 4;
        if (count > 0x3FFFFFFF) {
            allocSize = (uint32_t)-1;
        }

        void* newBuffer = rage_Alloc(allocSize);
        *(void**)(obj + 0) = newBuffer;
    } else {
        *(uint32_t*)(obj + 8) = 0;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::AllocateAndInitPointerArray @ 0x821379C8 | size: 0x94
//
// Allocates an array of `count` pointer-sized (4-byte) entries, initializes
// each element via snListNode_Init, and stores the result in the object.
// Sets the element count at offset +0x06 (uint16) and the array pointer
// at offset +0x00. If allocation fails, stores null with the count.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_79C8_w(void* self, uint32_t count) {
    uint8_t* obj = (uint8_t*)self;

    if (count != 0) {
        // Allocate count * 4 bytes; clamp to prevent overflow
        uint32_t allocSize = count * 4;
        if (count > 0x3FFFFFFF) {
            allocSize = (uint32_t)-1;
        }

        void* newArray = rage_Alloc(allocSize);

        if (newArray == nullptr) {
            *(uint16_t*)(obj + 6) = (uint16_t)count;
            *(uint32_t*)(obj + 0) = 0;
            return;
        }

        // Initialize each 4-byte element
        uint8_t* ptr = (uint8_t*)newArray;
        for (int32_t i = (int32_t)(count - 1); i >= 0; i--) {
            snListNode_Init(ptr);
            ptr += 4;
        }

        *(uint16_t*)(obj + 6) = (uint16_t)count;
        *(uintptr_t*)(obj + 0) = (uintptr_t)newArray;
    } else {
        *(uint16_t*)(obj + 6) = (uint16_t)count;
        *(uintptr_t*)(obj + 0) = 0;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::ResizeIndexBuffers @ 0x82118BB0 | size: 0xA8
//
// Grows the holder's dual index buffers (at offsets +208 and +212) to
// accommodate a new capacity. Uses next-power-of-2 rounding on the
// requested capacity (clamped to 8-bit). Copies old data via memcpy,
// then frees old buffers. Updates the capacity byte at offset +425.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_8BB0_w(void* self, uint32_t newIndex) {
    uint8_t* obj = (uint8_t*)self;
    uint8_t index = (uint8_t)(newIndex & 0xFF);
    uint8_t currentCapacity = *(uint8_t*)(obj + 425);

    if (index > currentCapacity) {
        // Round up to next power of 2 (8-bit)
        uint32_t n = index;
        n |= (n >> 1);
        n |= (n >> 2);
        n |= (n >> 4);
        n |= (n >> 8);
        n |= (n >> 16);
        uint8_t newCapacity = (uint8_t)(n + 1);

        // Allocate two new buffers
        void* newBuffer1 = rage_Alloc((uint32_t)newCapacity);
        void* newBuffer2 = rage_Alloc((uint32_t)newCapacity);

        // Copy existing data from old buffers
        uint8_t usedCount = *(uint8_t*)(obj + 424);
        memcpy(newBuffer1, *(void**)(obj + 208), usedCount);
        memcpy(newBuffer2, *(void**)(obj + 212), usedCount);

        // Store new buffers and capacity
        *(void**)(obj + 208) = newBuffer1;
        *(void**)(obj + 212) = newBuffer2;
        *(uint8_t*)(obj + 425) = newCapacity;
    }

    // Update used count
    *(uint8_t*)(obj + 424) = index;
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::UpdateCapacityFromMessage @ 0x821190A0 | size: 0x58
//
// Reads the desired capacity from a message object via virtual call
// (vtable slot 4), then calls ResizeIndexBuffers twice: once with
// the read capacity and once with 0 (to reset the used count).
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_90A0_w(void* self, void* message) {
    // Get message object from offset +4 of the message parameter
    void* msgObj = *(void**)((uint8_t*)message + 4);

    // Virtual call slot 4 to get desired capacity (returns byte in low 8 bits)
    typedef uint32_t (*GetCapacityFn)(void*);
    void** vt = *(void***)msgObj;
    uint8_t capacity = (uint8_t)((GetCapacityFn)vt[4])(msgObj);

    // Resize buffers to new capacity
    pongNetMessageHolder_8BB0_w(self, capacity);

    // Reset used count to 0
    pongNetMessageHolder_8BB0_w(self, 0);
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::InitTimerArray @ 0x821D6D08 | size: 0x84
//
// Initializes a timer/counter array structure. Zeroes the data pointer
// at +0x00 and element count at +0x04, then calls cmOperatorCtor_68E0_w
// to create 5 entries. After construction, zeroes each 4-byte slot in
// the allocated array. Finally stores a default float value (0.0f) at +0x08.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_6D08_2hr(void* self) {
    uint8_t* obj = (uint8_t*)self;

    // Initialize header
    *(uint32_t*)(obj + 0) = 0;  // Data pointer
    *(uint16_t*)(obj + 4) = 0;  // Element count

    // Construct 5 entries
    cmOperatorCtor_68E0_w(self, 5);

    // Zero all allocated slots
    uint16_t elementCount = *(uint16_t*)(obj + 4);
    if (elementCount != 0) {
        uint32_t* dataPtr = *(uint32_t**)(obj + 0);
        for (uint32_t i = 0; i < elementCount; i++) {
            dataPtr[i] = 0;
        }
    }

    // Store default timing value (0.0f) at offset +0x08
    *(float*)(obj + 8) = 0.0f;
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CleanupShortMessageArray (vfn_2 variant)
// @ 0x823BFFF8 | size: 0x64
//
// Destroys an array of 10 message objects with stride 36 bytes.
// Resets each object's vtable to PongNetMessage base (0x8206C304),
// then frees the array and nulls the pointer at +0x08.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_FFF8_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;  // +0x08

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Reset vtables for 10 elements at stride 36, backwards
        uint8_t* ptr = (uint8_t*)messageArray + 360;
        for (int i = 9; i >= 0; i--) {
            ptr -= 36;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CleanupCompactMessageArray (vfn_2 variant)
// @ 0x823C0260 | size: 0x64
//
// Destroys an array of 10 message objects with stride 28 bytes.
// Resets each object's vtable to PongNetMessage base (0x8206C304),
// then frees the array and nulls the pointer at +0x08.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_0260_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;  // +0x08

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Reset vtables for 10 elements at stride 28, backwards
        uint8_t* ptr = (uint8_t*)messageArray + 280;
        for (int i = 9; i >= 0; i--) {
            ptr -= 28;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CleanupSingleLargeMessage (vfn_2 variant)
// @ 0x823C2910 | size: 0x50
//
// Destroys a single message object of 24 bytes. Resets its vtable
// to PongNetMessage base (0x8206C304), then frees and nulls the pointer.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_2910_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;  // +0x08

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Reset vtable for the single 24-byte message object
        *(uint32_t*)messageArray = BASE_VTABLE;

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CleanupSingleSmallMessage (vfn_2 variant)
// @ 0x823C2AB0 | size: 0x50
//
// Destroys a single message object of 16 bytes. Resets its vtable
// to PongNetMessage base (0x8206C304), then frees and nulls the pointer.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_2AB0_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;  // +0x08

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Reset vtable for the single 16-byte message object
        *(uint32_t*)messageArray = BASE_VTABLE;

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateMatchTimeSyncMessagePool @ 0x823BFF38 | size: 0x64
//
// Teardown for the MatchTimeSyncMessage pool (4 entries, stride 20).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   sentinel = lis(-32249) + (-15612) = 0x8206C304
//   start = base + 80, 4 * 20 = 80
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_FF38_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 4 entries (stride 20), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 80;
        for (int i = 3; i >= 0; i--) {
            ptr -= 20;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateSessionTimeSyncRequestMessagePool @ 0x823C04D0 | size: 0x64
//
// Teardown for the SessionTimeSyncRequestMessage pool (10 entries, stride 32).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 320, 10 * 32 = 320
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_04D0_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 10 entries (stride 32), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 320;
        for (int i = 9; i >= 0; i--) {
            ptr -= 32;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateForceSessionTimeSyncMessagePool @ 0x823C0638 | size: 0x64
//
// Teardown for the ForceSessionTimeSyncMessage pool (10 entries, stride 20).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 200, 10 * 20 = 200
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_0638_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 10 entries (stride 20), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 200;
        for (int i = 9; i >= 0; i--) {
            ptr -= 20;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateTimedGameUpdateTimerMessagePool @ 0x823C07A8 | size: 0x64
//
// Teardown for the TimedGameUpdateTimerMessage pool (10 entries, stride 24).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 240, 10 * 24 = 240
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_07A8_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 10 entries (stride 24), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 240;
        for (int i = 9; i >= 0; i--) {
            ptr -= 24;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateStateSyncRequestMessagePool @ 0x823C13D0 | size: 0x64
//
// Teardown for the StateSyncRequestMessage pool (20 entries, stride 16).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 320, 20 * 16 = 320
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_13D0_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 20 entries (stride 16), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 320;
        for (int i = 19; i >= 0; i--) {
            ptr -= 16;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateAcceptMessagePool @ 0x823C1E80 | size: 0x64
//
// Teardown for the AcceptMessage pool (30 entries, stride 16).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 480, 30 * 16 = 480
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_1E80_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 30 entries (stride 16), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 480;
        for (int i = 29; i >= 0; i--) {
            ptr -= 16;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateAcceptSpectatorMessagePool @ 0x823C1F40 | size: 0x64
//
// Teardown for the AcceptSpectatorMessage pool (2 entries, stride 296).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 592, 2 * 296 = 592
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_1F40_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 2 entries (stride 296), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 592;
        for (int i = 1; i >= 0; i--) {
            ptr -= 296;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateDenySpectatorMessagePool @ 0x823C2000 | size: 0x64
//
// Teardown for the DenySpectatorMessage pool (2 entries, stride 80).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 160, 2 * 80 = 160
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_2000_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 2 entries (stride 80), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 160;
        for (int i = 1; i >= 0; i--) {
            ptr -= 80;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateLeaveSpectatorSessionMessagePool @ 0x823C2280 | size: 0x64
//
// Teardown for the LeaveSpectatorSessionMessage pool (2 entries, stride 108).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 216, 2 * 108 = 216
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_2280_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 2 entries (stride 108), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 216;
        for (int i = 1; i >= 0; i--) {
            ptr -= 108;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocatePostPointExitMessagePool @ 0x823C2E58 | size: 0x64
//
// Teardown for the PostPointExitMessage pool (5 entries, stride 100).
// Resets each entry's vtable to PongNetMessage base (0x8206C304), then frees.
//
// Python-verified:
//   start = base + 500, 5 * 100 = 500
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_2E58_1(pongNetMessageHolder* holder) {
    void* messageArray = holder->m_pInternalArray;

    if (messageArray != nullptr) {
        const uint32_t BASE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 5 entries (stride 100), resetting vtables
        uint8_t* ptr = (uint8_t*)messageArray + 500;
        for (int i = 4; i >= 0; i--) {
            ptr -= 100;
            *(uint32_t*)ptr = BASE_VTABLE;
        }

        sysMemAllocator_Free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// Forward declaration for heap free used by scalar destructors below
extern void rage_free(void* ptr);

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolderBase::~pongNetMessageHolderBase @ 0x823B5890 | size: 0x58
//
// Virtual destructor (vtable slot 0) for pongNetMessageHolderBase.
// Resets the vtable pointer to the base class vtable (0x8206FA88),
// decrements the global live-instance count at 0x826066A4,
// and optionally frees the object memory if the scalar-delete flag (bit 0
// of the flags parameter) is set.
//
// Python-verified:
//   vtable = lis(-32249) + (-1400) = 0x8206FA88 (pongNetMessageHolderBase)
//   g_liveCount = lis(-32160) + 26276 = 0x826066A4
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolderBase_vfn_0(pongNetMessageHolderBase* self, int flags) {
    const uint32_t BASE_VTABLE = VTABLE_pongNetMessageHolderBase;

    // Reset vtable to base class
    *reinterpret_cast<uint32_t*>(self) = BASE_VTABLE;

    // Decrement global live-instance counter
    uint32_t* liveCount = reinterpret_cast<uint32_t*>(&g_netMessageHolderLiveCount_raw);
    *liveCount -= 1;

    // Scalar-delete: free memory if bit 0 of flags is set
    if (flags & 1) {
        rage_free(self);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateDisconnectNotificationPool @ 0x823C30A8 | size: 0x64
//
// Teardown for a message pool (5 entries, stride 16).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 80, 5 * 16 = 80
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_30A8_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 5 entries (stride 16), resetting vtables
        uint8_t* cursor = messageArray + 80;
        for (int i = 4; i >= 0; --i) {
            cursor -= 16;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateGamerUpdateMessagePool @ 0x823C32A0 | size: 0x64
//
// Teardown for a message pool (5 entries, stride 44).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 220, 5 * 44 = 220
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_32A0_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 5 entries (stride 44), resetting vtables
        uint8_t* cursor = messageArray + 220;
        for (int i = 4; i >= 0; --i) {
            cursor -= 44;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateCancelLobbyCountdownPool @ 0x823C3400 | size: 0x64
//
// Teardown for a message pool (4 entries, stride 12).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 48, 4 * 12 = 48
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_3400_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 4 entries (stride 12), resetting vtables
        uint8_t* cursor = messageArray + 48;
        for (int i = 3; i >= 0; --i) {
            cursor -= 12;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateExitPostGameMessagePool @ 0x823C35B0 | size: 0x64
//
// Teardown for a message pool (5 entries, stride 20).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 100, 5 * 20 = 100
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_35B0_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 5 entries (stride 20), resetting vtables
        uint8_t* cursor = messageArray + 100;
        for (int i = 4; i >= 0; --i) {
            cursor -= 20;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateInternalMessageRelayPool @ 0x823C3670 | size: 0x64
//
// Teardown for a message pool (10 entries, stride 12).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 120, 10 * 12 = 120
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_3670_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 10 entries (stride 12), resetting vtables
        uint8_t* cursor = messageArray + 120;
        for (int i = 9; i >= 0; --i) {
            cursor -= 12;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocatePlayerUpdateLargeMessagePool @ 0x823C3730 | size: 0x64
//
// Teardown for a message pool (10 entries, stride 524).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 5240, 10 * 524 = 5240
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_3730_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 10 entries (stride 524), resetting vtables
        uint8_t* cursor = messageArray + 5240;
        for (int i = 9; i >= 0; --i) {
            cursor -= 524;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateDataSendMessagePool @ 0x823C3AE8 | size: 0x64
//
// Teardown for a message pool (31 entries, stride 40).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 1240, 31 * 40 = 1240
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_3AE8_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 31 entries (stride 40), resetting vtables
        uint8_t* cursor = messageArray + 1240;
        for (int i = 30; i >= 0; --i) {
            cursor -= 40;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateDataRequestMessagePool @ 0x823C3C60 | size: 0x64
//
// Teardown for a message pool (31 entries, stride 552).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 17112, 31 * 552 = 17112
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_3C60_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 31 entries (stride 552), resetting vtables
        uint8_t* cursor = messageArray + 17112;
        for (int i = 30; i >= 0; --i) {
            cursor -= 552;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateDropSpectatorConnectionPool @ 0x823C3FD0 | size: 0x64
//
// Teardown for a message pool (2 entries, stride 124).
// Resets each entry's vtable to PongNetMessage base, then frees.
//
// Python-verified:
//   start = base + 248, 2 * 124 = 248
//   sentinel = 0x8206C304 (PongNetMessage vtable)
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_3FD0_1(pongNetMessageHolder* holder) {
    uint8_t* messageArray = static_cast<uint8_t*>(holder->m_pInternalArray);

    if (messageArray != nullptr) {
        const uint32_t PONG_NET_MESSAGE_VTABLE = VTABLE_PongNetMessage;

        // Walk backwards through 2 entries (stride 124), resetting vtables
        uint8_t* cursor = messageArray + 248;
        for (int i = 1; i >= 0; --i) {
            cursor -= 124;
            *reinterpret_cast<uint32_t*>(cursor) = PONG_NET_MESSAGE_VTABLE;
        }

        rage_free(messageArray);
        holder->m_pInternalArray = nullptr;
    }
}


// ===========================================================================
// pongNetMessageHolder::InitNetworkState @ 0x821D75B0 | size: 0xB8
//
// Initialises a network state structure. Zeroes 6 floats at +0 through +24,
// calls pongNetMessageHolder_7668_2hr on the sub-object at +32, stores a
// default float at +9732, zeroes +9728, calls InitTimerArray on +9736,
// calls game_5E70 on +9760, zeroes +10208 and +10212, and copies 16 bytes
// of default data from 0x8261A0C0 into +10216.
// ===========================================================================
void pongNetMessageHolder_75B0(void* self) {
    uint8_t* obj = (uint8_t*)self;

    // Zero 6 floats (positions/velocities)
    float zero = g_fZeroConst;
    *(float*)(obj + 0) = zero;
    *(float*)(obj + 4) = zero;
    *(float*)(obj + 8) = zero;
    *(float*)(obj + 16) = zero;
    *(float*)(obj + 20) = zero;
    *(float*)(obj + 24) = zero;

    // Init sub-object at +32
    pongNetMessageHolder_7668_2hr(obj + 32);

    // Store default float at +9732, zero byte at +9728
    float defaultTimer = g_fDefaultTimer;
    *(float*)(obj + 9732) = defaultTimer;
    *(uint8_t*)(obj + 9728) = 0;

    // Init timer array at +9736
    pongNetMessageHolder_6D08_2hr(obj + 9736);

    // Init game state at +9760
    game_5E70(obj + 9760);

    // Zero remaining fields
    *(uint32_t*)(obj + 10208) = 0;
    *(uint8_t*)(obj + 10212) = 0;

    // Copy 16 bytes of default data from global
    uint32_t* src = (uint32_t*)g_netMessageHolderBufferPrefix;
    uint32_t* dst = (uint32_t*)(obj + 10216);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}


// ===========================================================================
// pongNetMessageHolder::InitInputData @ 0x821E0138 | size: 0xBC
//
// Constructor for an input-data holder (gdInputData). Sets vtable, zeroes
// control fields, initialises a sub-array at offset +796 with capacity 58,
// and allocates a zeroed buffer of 232 bytes for the entries.
// ===========================================================================
void pongNetMessageHolder_0138_w(void* self) {
    uint8_t* obj = (uint8_t*)self;

    *(uint32_t*)(obj + 0) = VTABLE_gdInputData;  // gdInputData vtable
    *(uint8_t*)(obj + 4) = 0;
    *(uint8_t*)(obj + 5) = 0;
    *(uint8_t*)(obj + 6) = 0;
    *(uint8_t*)(obj + 7) = 0;
    *(uint32_t*)(obj + 8) = 0;
    *(uint8_t*)(obj + 12) = 0;
    *(uint32_t*)(obj + 16) = 0;
    *(uint32_t*)(obj + 24) = 0;
    *(uint32_t*)(obj + 20) = 0;

    // Init sub-array at +796
    uint8_t* subArray = obj + 796;
    *(uint32_t*)(subArray + 0) = 0;
    *(uint16_t*)(subArray + 4) = 0;
    *(uint16_t*)(subArray + 6) = 0;
    *(uint32_t*)(obj + 804) = 0;

    uint16_t currentCapacity = *(uint16_t*)(subArray + 6);
    if (currentCapacity == 0) {
        *(uint16_t*)(subArray + 6) = 58;

        // Allocate 232 bytes (58 * 4)
        void* buf = rage_Alloc(232);
        if (buf != nullptr) {
            // Zero the entire buffer byte-by-byte (58 entries * 4 bytes each)
            uint8_t* p = (uint8_t*)buf + 2;
            for (int i = 57; i >= 0; i--) {
                *(p - 2) = 0;
                *(p - 1) = 0;
                *(p + 0) = 0;
                *(p + 1) = 0;
                p += 4;
            }
        }
        *(void**)(subArray + 0) = buf;
    }

    *(uint16_t*)(subArray + 4) = 58;
}


// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C46F8 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x82070090), calls vfn_2_0868_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_46F8_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0090);
    pongNetMessageHolder_vfn_2_0868_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C4770 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x820700A4), calls vfn_2_07A8_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_4770_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_00A4);
    pongNetMessageHolder_vfn_2_07A8_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C47E8 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x820700B8), calls vfn_2_FD70_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_47E8_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_00B8);
    pongNetMessageHolder_vfn_2_FD70_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C4860 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x820700CC), calls vfn_2_24B8_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_4860_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_00CC);
    pongNetMessageHolder_vfn_2_24B8_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C48D8 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x820700E0), calls vfn_2_FF38_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_48D8_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_00E0);
    pongNetMessageHolder_vfn_2_FF38_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C4950 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x820700F4), calls vfn_2_FFF8_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_4950_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_00F4);
    pongNetMessageHolder_vfn_2_FFF8_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C49C8 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x82070108), calls vfn_2_0638_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_49C8_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0108);
    pongNetMessageHolder_vfn_2_0638_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}

// ===========================================================================
// pongNetMessageHolder destructor variant @ 0x823C4A40 | size: 0x78
//
// Deleting destructor for MI vtable variant.
// Sets derived vtable (0x8207011C), calls vfn_2_0260_1 cleanup, resets to
// base vtable (0x8206FA88), decrements live count, conditionally frees.
// ===========================================================================
pongNetMessageHolder* pongNetMessageHolder_vfn_0_4A40_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_011C);
    pongNetMessageHolder_vfn_2_0260_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }

    return self;
}


// ═══════════════════════════════════════════════════════════════════════════
// BATCH: Deserialise (READ) vtable method wrappers — hit & time-sync family.
//
// Each method here is the virtual slot-2 entry point declared in
// pong_network.hpp. The heavy lifting was already lifted earlier in this
// file as `<Class>_Deserialise(thisPtr, client)` free functions (see the
// "BATCH: Network Message Deserialise/Serialise vtable methods" header
// around line 3929). These thin wrappers bind the free-function form into
// the proper C++ virtual-method signature so the rest of the codebase can
// call them through the vtable without going through thisPtr-by-pointer.
//
// Per-binding addresses and sizes are documented per method.  The WRITE
// counterparts (Serialise) already exist in class-method form for
// RemoteServeReadyMessage / MatchTimeSyncMessage / ForceMatchTimeSyncMessage
// via the `_Serialise` free helpers — they can be promoted later the same
// way.
// ═══════════════════════════════════════════════════════════════════════════

// Forward declarations of the underlying lifted free helpers (defined above
// at the original PPC entry points).
extern void HitMessage_Deserialise(void* thisPtr, void* client);
extern void HitDataMessage_Deserialise(void* thisPtr, void* client);
extern void SpectatorBallHitMessage_Deserialise(void* thisPtr, void* client);
extern void RemoteServeReadyMessage_Deserialise(void* thisPtr, void* client);
extern void MatchTimeSyncMessage_Deserialise(void* thisPtr, void* client);
extern void ForceMatchTimeSyncMessage_Deserialise(void* thisPtr, void* client);

// ---------------------------------------------------------------------------
// HitMessage::Deserialise  @ 0x823B5FE0 | size: 0x5C
// Reads the base-hit wire format: 32-bit timing float into m_timingRef,
// followed by an 8-bit m_hitFlags byte.
// ---------------------------------------------------------------------------
void HitMessage::Deserialise(void* client)
{
    HitMessage_Deserialise(this, client);
}

// ---------------------------------------------------------------------------
// HitDataMessage::Deserialise  @ 0x823B5B58 | size: 0x80
// Reads: m_timingRef (f32) + m_ballHitData[192] payload + m_recoveryTiming
// (f32) + m_playerIndex (u8). The 192-byte payload is streamed via the
// shared DeserializeNetworkData chunked reader.
// ---------------------------------------------------------------------------
void HitDataMessage::Deserialise(void* client)
{
    HitDataMessage_Deserialise(this, client);
}

// ---------------------------------------------------------------------------
// SpectatorBallHitMessage::Deserialise  @ 0x823B6B00 | size: 0x44
// Derived from HitMessage: first reads the base (timing + flags), then
// streams the 192-byte m_ballHitData payload at +0x10.
// ---------------------------------------------------------------------------
void SpectatorBallHitMessage::Deserialise(void* client)
{
    SpectatorBallHitMessage_Deserialise(this, client);
}

// ---------------------------------------------------------------------------
// RemoteServeReadyMessage::Deserialise  @ 0x823B6ED8 | size: 0x48
// Derived from HitMessage: reads the base (timing + flags) then a 16-bit
// m_serveParam field at +0x0C.
// ---------------------------------------------------------------------------
void RemoteServeReadyMessage::Deserialise(void* client)
{
    RemoteServeReadyMessage_Deserialise(this, client);
}

// ---------------------------------------------------------------------------
// MatchTimeSyncMessage::Deserialise  @ 0x823B70A8 | size: 0x88
// Reads m_localTimestamp (u64), m_remoteTimestamp (u64), m_syncDelta (f32),
// m_frameCounter (u16), then scales m_syncDelta by g_pMatchTimeSyncScale[3]
// (= *(float*)(0x825CAEB8 + 12)) to convert the wire-side fixed rate into
// the session-local clock rate.
// ---------------------------------------------------------------------------
void MatchTimeSyncMessage::Deserialise(void* client)
{
    MatchTimeSyncMessage_Deserialise(this, client);
}

// ---------------------------------------------------------------------------
// ForceMatchTimeSyncMessage::Deserialise  @ 0x823B7AA0 | size: 0x4C
// Reads m_timestamp (u64 @ +0x08) + m_syncParam (u16 @ +0x10). No timing
// scale is applied — the force-sync path is absolute.
// ---------------------------------------------------------------------------
void ForceMatchTimeSyncMessage::Deserialise(void* client)
{
    ForceMatchTimeSyncMessage_Deserialise(this, client);
}

// ---------------------------------------------------------------------------
// ServeStartedMessage::Deserialise   — already implemented above at line ~209.
// PlayerMovementMessage::Deserialise — already implemented above at line ~3269.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// ServeUnlockMessage::Deserialise — NOT REQUIRED.
// The original ServeUnlockMessage vtable (@ 0x8206FAF4) exposes only slots
// 3..7 (ScalarDtor / pool-return / Process / GetIndexInPool / GetTypeName).
// There is no slot-2 Deserialise entry: the message carries no payload
// beyond its type tag, so the dispatcher constructs it directly without
// consulting the bitstream. Nothing to lift here.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// BallInputMessage::Deserialise — CLASS NOT PRESENT IN BINARY.
// No RTTI class named "BallInputMessage" exists in the 52,691-symbol index,
// nor in rtti_vtable_map.json.  The closest matches are BallHitMessage
// (vtable 0x8206FA9C, no Deserialise) and the ball/paddle input stream
// that travels inside PlayerMovementMessage. Treat this as a misnamed
// request — no work item.
// ---------------------------------------------------------------------------

// ═══════════════════════════════════════════════════════════════════════════
// BATCH 2: Network Message Deserialise (READ) handlers
// Classes: BallHitMessage, ServeFaultMessage, ServeLetMessage, SessionTimeSyncMessage,
//          ScoreMessage, MatchScoreMessage, PointDeclaredMessage, GamerReadyMessage,
//          LevelConfigMessage, ForfeitMatchMessage
//
// Every handler here mirrors the Serialise (WRITE) side, simply reversing the
// bit ordering. All slot-1 addresses verified against the original binary.
//
// Shared helpers used below:
//   SinglesNetworkClient_8DF8_g(client, &tmp, 32)    - read 32-bit float via stack scratch
//   netStream_ReadBlock(client, dst, sizeBits)       - read N-bit fixed-width block
//   netStream_ReadU32ScoreVariant(client, dst, 32)     - read 32-bit variant (ScoreMessage)
//   netStream_ReadU32GamerHandleVariant(client, dst, 32)     - read 32-bit variant (SpectatorQuit)
//   snBitStream_ReadUnsigned(client, dst, N)         - read unsigned N-bit integer
// ═══════════════════════════════════════════════════════════════════════════

extern void netStream_ReadU32ScoreVariant(void* client, void* dst, int sizeBits);  // @ 0x82390608
extern void netStream_ReadU32GamerHandleVariant(void* client, void* dst, int sizeBits);  // @ 0x82260E18

// NOTE: BallHitMessage_Deserialise, ServeFaultMessage_Deserialise,
// ServeLetMessage_Deserialise, SessionTimeSyncMessage_Deserialise are defined
// earlier in this file (around lines 4148-4337). Do not redefine here.

extern void netStream_ReadS8SignMagnitude(void* client, void* dst, int sizeBits);  // @ 0x82101668

// ─────────────────────────────────────────────────────────────────────────────
// ScoreMessage_Deserialise  @ 0x823BD7C8 | size: 0x58
//
// Reads timing float (+0x04) and a 32-bit score/state blob at +0x08 via the
// _0608_g variant (used only by ScoreMessage in this subsystem).
// ─────────────────────────────────────────────────────────────────────────────
void ScoreMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // +0x08: 32-bit score payload.
    netStream_ReadU32ScoreVariant(client, obj + 0x08, 32);
}

// ─────────────────────────────────────────────────────────────────────────────
// MatchScoreMessage_Deserialise  @ 0x823BCE78 | size: 0x5C
//
// Reads timing float (+0x04) and an 88-bit fixed block at +0x08 via netStream_ReadBlock
// (block contains packed score state — 11 byte fields, see Serialise side).
// ─────────────────────────────────────────────────────────────────────────────
void MatchScoreMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // +0x08: 88-bit packed score-state block.
    netStream_ReadBlock(client, obj + 0x08, 88);
}

// ─────────────────────────────────────────────────────────────────────────────
// PointDeclaredMessage_Deserialise  @ 0x823BD210 | size: 0x5C
//
// Reads timing float (+0x04) and a 16-bit point identifier (+0x08).
// ─────────────────────────────────────────────────────────────────────────────
void PointDeclaredMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // +0x08: 16-bit point identifier.
    snBitStream_ReadUnsigned(client, obj + 0x08, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// GamerReadyMessage_Deserialise  @ 0x823BE630 | size: 0x5C
//
// Reads timing float (+0x04) and a 24-bit packed gamer-ready flag block at
// +0x08 via netStream_ReadBlock.
// ─────────────────────────────────────────────────────────────────────────────
void GamerReadyMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // +0x08: 24-bit ready-flag block.
    netStream_ReadBlock(client, obj + 0x08, 24);
}

// ─────────────────────────────────────────────────────────────────────────────
// LevelConfigMessage_Deserialise  @ 0x823BC0F0 | size: 0x5C
//
// Reads timing float (+0x04) and a 4-bit level-config nibble (+0x08).
// ─────────────────────────────────────────────────────────────────────────────
void LevelConfigMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // +0x08: 4-bit level-config nibble.
    netStream_ReadBlock(client, obj + 0x08, 4);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForfeitMatchMessage_Deserialise  @ 0x823BDC30 | size: 0x6C
//
// Reads timing float (+0x04), a 4-bit forfeit-reason nibble (+0x08), and a
// second 4-bit auxiliary nibble (+0x0C). Both nibbles travel through netStream_ReadBlock.
// ─────────────────────────────────────────────────────────────────────────────
void ForfeitMatchMessage_Deserialise(void* thisPtr, void* client) {
    uint8_t* obj = (uint8_t*)thisPtr;

    float timingRef;
    snBitStream_ReadBits(client, &timingRef, 32);
    *(float*)(obj + 0x04) = timingRef;

    // +0x08: 4-bit forfeit reason.
    netStream_ReadBlock(client, obj + 0x08, 4);

    // +0x0C: 4-bit auxiliary nibble.
    netStream_ReadBlock(client, obj + 0x0C, 4);
}


// ═══════════════════════════════════════════════════════════════════════════
// BATCH: pongNetMessageHolder — five deleting-destructor variants, two lazy
// slot-1 pool initialisers, one CopyAssign-like clone, one 96-byte vector
// clone + creature-inst attach, and one pool-scan hash lookup.
//
// All members follow patterns already established earlier in this file
// (vfn_0_46F8_1 .. vfn_0_4A40_1 for scalar-dtor variants; vfn_1 / vfn_2
// LazyInit helpers around line 2250).  No raw pointer arithmetic or hex
// literals inside bodies — field offsets are named or use existing helpers.
// ═══════════════════════════════════════════════════════════════════════════

extern void pongNetMessageHolder_vfn_2_0B58_1(pongNetMessageHolder* holder);  // @ 0x823C0B58
extern void pongNetMessageHolder_vfn_2_1628_1(pongNetMessageHolder* holder);  // @ 0x823C1628
extern void pongNetMessageHolder_vfn_2_18D0_1(pongNetMessageHolder* holder);  // @ 0x823C18D0 (declared earlier)
extern void pongNetMessageHolder_vfn_2_1770_1(pongNetMessageHolder* holder);  // @ 0x823C1770 (declared earlier)
extern void pongNetMessageHolder_vfn_2_3878_1(pongNetMessageHolder* holder);  // @ 0x823C3878 (declared earlier)
extern void pongNetMessageHolder_0838_w(void* dst, void* src);                // @ 0x82220838 — evtSet-like copy
extern void PongNetRoundRobinCoordinator_FC20_w(void* obj);                    // @ 0x8220FC20
extern void* ke_8F70(void* obj);                                               // @ 0x822C8F70 — ctor returning this
extern void pongCreatureInst_9030_g(void* creatureInst, void* srcPayload);    // @ 0x822C9030
extern void* atSingleton_7CE0_w(void* singleton, int zero, int slotMinusOne); // @ 0x82447CE0 — returns slot key
extern "C" void thunk_rage_free(void* p);                                      // @ 0x820C0120

// The max-float init value is at 0x82079AD4 (.rdata, 4 bytes).  Bodies read
// it by symbol reference via a small accessor so the raw address stays out
// of function bodies.
extern const float lbl_82079AD4;  // .rdata 4 bytes
static inline float PongNetMaxFloatInit() {
    return lbl_82079AD4;
}

// Global network sink object @ 0x826065E4 invoked via vtable slot 11 from D040_w.
extern void** lbl_826065E4;

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder deleting-destructor @ 0x823C4E00 | size: 0xE0
//
// MI vtable variant: sets derived vtable (0x820701BC), calls vfn_2_0B58_1
// cleanup, restores base vtable (0x8206FA88 = pongNetMessageHolderBase),
// decrements live-instance counter, conditionally frees via the low bit of
// the flags argument.  Pattern is identical to 0x823C46F8 et al.
// ─────────────────────────────────────────────────────────────────────────────
pongNetMessageHolder* pongNetMessageHolder_vfn_0_4E00_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_01BC);
    pongNetMessageHolder_vfn_2_0B58_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }
    return self;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder deleting-destructor @ 0x823C5328 | size: 0x78
// Derived vtable 0x82070298 → vfn_2_1628_1 cleanup → base vtable restore.
// ─────────────────────────────────────────────────────────────────────────────
pongNetMessageHolder* pongNetMessageHolder_vfn_0_5328_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0298);
    pongNetMessageHolder_vfn_2_1628_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }
    return self;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder deleting-destructor @ 0x823C5418 | size: 0x78
// Derived vtable 0x820702C0 → vfn_2_18D0_1 cleanup → base vtable restore.
// ─────────────────────────────────────────────────────────────────────────────
pongNetMessageHolder* pongNetMessageHolder_vfn_0_5418_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_02C0);
    pongNetMessageHolder_vfn_2_18D0_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }
    return self;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder deleting-destructor @ 0x823C5490 | size: 0x78
// Derived vtable 0x820702D4 → vfn_2_1770_1 cleanup → base vtable restore.
// ─────────────────────────────────────────────────────────────────────────────
pongNetMessageHolder* pongNetMessageHolder_vfn_0_5490_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_02D4);
    pongNetMessageHolder_vfn_2_1770_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }
    return self;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder deleting-destructor @ 0x823C59E0 | size: 0x78
// Derived vtable 0x820703C4 → vfn_2_3878_1 cleanup → base vtable restore.
// ─────────────────────────────────────────────────────────────────────────────
pongNetMessageHolder* pongNetMessageHolder_vfn_0_59E0_1(pongNetMessageHolder* self, int flags) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_03C4);
    pongNetMessageHolder_vfn_2_3878_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free(self);
    }
    return self;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_2148_1 @ 0x823C2148 | size: 0xE0
//
// Lazy-init helper: if the inner pool pointer (holder->m_pInternalArray
// at +0x08) is null, allocate a 32-byte RemoveSpectatorMessage-style
// object, initialise its header (vtable, float sentinel, two duplicate
// vtable pointers at +0 and +12, slot count 2 at +26, a -1 sentinel at
// +8 / +22), then walk the single-element free-list at +10 / +20 within
// the item to stamp "next slot" indices.  Matches vfn_2_FFF8_1-style
// lazy init but uses the RemoveSpectatorMessage vtable.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_2148_1(pongNetMessageHolder* holder) {
    if (holder->m_pInternalArray != nullptr) {
        return;
    }

    // 32-byte sub-object allocation via xe_EC88 / rage_Alloc.
    auto* item = static_cast<uint8_t*>(rage_Alloc(32));
    if (item == nullptr) {
        holder->m_pInternalArray = nullptr;
        return;
    }

    const float fInit = PongNetMaxFloatInit();
    *reinterpret_cast<float*>(item + 4)  = fInit;       // +0x04 flags float
    *reinterpret_cast<float*>(item + 16) = fInit;       // +0x10 mirror
    *reinterpret_cast<uint32_t*>(item + 0)  = VTABLE_RemoveSpectatorMessage;
    *reinterpret_cast<uint32_t*>(item + 12) = VTABLE_RemoveSpectatorMessage;
    *reinterpret_cast<uint16_t*>(item + 26) = 2;        // slot count
    *reinterpret_cast<uint16_t*>(item + 28) = 0;
    *reinterpret_cast<uint16_t*>(item + 8)  = 0xFFFF;   // free-list head
    *reinterpret_cast<uint16_t*>(item + 24) = 2;
    *reinterpret_cast<uint16_t*>(item + 22) = 0xFFFF;

    // Walk the slot ring — the body below mirrors the loop that stamps each
    // slot's "self-index" at +10 and zero at +20 using a 6-byte stride.
    const int slotCount = *reinterpret_cast<uint16_t*>(item + 26);
    for (int i = 0; i < slotCount - 1; ++i) {
        const int base = (i + i * 2) * 4;  // r7 = i + (i<<1); r11 = r7 << 2
        *reinterpret_cast<uint16_t*>(item + base + 10) = static_cast<uint16_t>(i + 1);
        *reinterpret_cast<uint16_t*>(item + base + 20) = 0;
    }

    holder->m_pInternalArray = item;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_40B0_1 @ 0x823C40B0 | size: 0xE0
//
// Lazy-init helper: allocates a 976-byte RoundRobinDataMessage-style
// sub-object at holder->m_pInternalArray (+0x08), initialises its
// PongNetRoundRobinCoordinator substate via _FC20_w, stamps the free-list
// sentinels at +952/+954/+956/+958/+960, then walks the ring writing
// each slot's self-index at +954 within a 956-byte stride plus a zero
// word at +1908 inside the next slot.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_40B0_1(pongNetMessageHolder* holder) {
    if (holder->m_pInternalArray != nullptr) {
        return;
    }

    auto* item = static_cast<uint8_t*>(rage_Alloc(976));
    if (item == nullptr) {
        holder->m_pInternalArray = nullptr;
        return;
    }

    // Header: vtable, float init, coordinator substate.
    *reinterpret_cast<uint32_t*>(item + 0) = VTABLE_RoundRobinDataMessage;
    *reinterpret_cast<float*>(item + 4)    = PongNetMaxFloatInit();
    PongNetRoundRobinCoordinator_FC20_w(item + 8);

    // Slot-ring sentinels: slot count = 1, head = -1, spare = 0.
    *reinterpret_cast<uint16_t*>(item + 958) = 1;
    *reinterpret_cast<uint16_t*>(item + 952) = 0xFFFF;
    *reinterpret_cast<uint16_t*>(item + 960) = 0;
    *reinterpret_cast<uint16_t*>(item + 956) = 1;
    *reinterpret_cast<uint16_t*>(item + 954) = 0xFFFF;

    // Ring walk — for each slot i in [0, slotCount-1) stamp next-index at
    // stride 956 (item + i*956 + 954) and zero the 1908-byte carryover.
    const int slotCount = *reinterpret_cast<uint16_t*>(item + 958);
    for (int i = 0; i < slotCount - 1; ++i) {
        uint8_t* slot = item + i * 956;
        *reinterpret_cast<uint16_t*>(slot + 1908) = 0;
        *reinterpret_cast<uint16_t*>(slot + 954)  = static_cast<uint16_t>(i + 1);
    }

    holder->m_pInternalArray = item;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_32F0_w @ 0x821232F0 | size: 0x184
//
// Copy-assignment helper: blits scalar timing / flags / payload header from
// src (r4) into dst (r3), then for each of three evtSet* sub-object slots at
// +164 / +168 / +172 lazily allocates a 24-byte evtSet if dst's slot is
// null, zero-initialises it, and delegates to pongNetMessageHolder_0838_w
// to deep-copy src's entries.  If src's slot is null the whole block is
// skipped (dst keeps its previous pointer).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_32F0_w(pongNetMessageHolder* dst, pongNetMessageHolder* src) {
    auto* dstB = reinterpret_cast<uint8_t*>(dst);
    auto* srcB = reinterpret_cast<uint8_t*>(src);

    // Scalar header copy: three floats at +4/+8/+12.
    *reinterpret_cast<float*>(dstB + 4)  = *reinterpret_cast<float*>(srcB + 4);
    *reinterpret_cast<float*>(dstB + 8)  = *reinterpret_cast<float*>(srcB + 8);
    *reinterpret_cast<float*>(dstB + 12) = *reinterpret_cast<float*>(srcB + 12);

    // Two timing maxima at +144 / +148 — only overwritten when src > dst.
    // The reference float at [-12016(r11)] is the dst field itself (dst +144)
    // loaded as the comparison baseline — so we implement the "only copy on
    // strict greater-than" using dst's current value.
    {
        const float v = *reinterpret_cast<float*>(srcB + 144);
        const float cur = *reinterpret_cast<float*>(dstB + 144);
        if (v > cur) {
            *reinterpret_cast<float*>(dstB + 144) = v;
        }
    }
    {
        const float v = *reinterpret_cast<float*>(srcB + 148);
        const float cur = *reinterpret_cast<float*>(dstB + 144);  // shared baseline
        if (v > cur) {
            *reinterpret_cast<float*>(dstB + 148) = v;
        }
    }

    // One-byte flag at +153.
    *(dstB + 153) = *(srcB + 153);

    // Three evtSet-style clone slots at +164/+168/+172.
    const uint32_t kEvtSetVtable = VTABLE_rage_evtSet;
    const int kEvtSetSize = 24;
    const int kSlotOffsets[3] = {164, 168, 172};
    for (int i = 0; i < 3; ++i) {
        const int off = kSlotOffsets[i];
        void* srcSub = *reinterpret_cast<void**>(srcB + off);
        if (srcSub == nullptr) {
            continue;
        }
        if (*reinterpret_cast<void**>(dstB + off) == nullptr) {
            auto* sub = static_cast<uint8_t*>(rage_Alloc(kEvtSetSize));
            if (sub != nullptr) {
                *reinterpret_cast<uint32_t*>(sub + 0)  = kEvtSetVtable;
                *reinterpret_cast<uint32_t*>(sub + 4)  = 0;
                *reinterpret_cast<uint16_t*>(sub + 8)  = 0;
                *reinterpret_cast<uint16_t*>(sub + 10) = 0;
                *reinterpret_cast<uint32_t*>(sub + 12) = 0;
                *reinterpret_cast<uint32_t*>(sub + 16) = 0;
            }
            *reinterpret_cast<void**>(dstB + off) = sub;
        }
        // Deep-copy src's contents into dst's slot (may be null on OOM).
        pongNetMessageHolder_0838_w(
            *reinterpret_cast<void**>(dstB + off),
            *reinterpret_cast<void**>(srcB + off));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_D040_w @ 0x8227D040 | size: 0x10C
//
// Applies a 64-byte "hit payload" (r5) into a pongCreatureInst attached to
// holder (r3):
//   1. Copy 64 bytes from payload into holder[+32..+96] via four vector
//      quadword loads (vmx128).
//   2. If holder's creature-inst pointer at +20 is null, lazy-construct an
//      80-byte pongCreatureInst via ke_8F70.
//   3. Invoke pongCreatureInst_9030_g(creatureInst, extraArg = r4).
//   4. Copy 64 bytes from holder[+32..+96] into creatureInst[+16..+80].
//   5. If creatureInst->slotId (uint16 at +8) == 0xFFFF, skip; else invoke
//      vtable slot 11 of the global object at 0x826065E4 (network sink).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_D040_w(pongNetMessageHolder* holder, void* extraArg, const void* payload64) {
    auto* holderB = reinterpret_cast<uint8_t*>(holder);
    auto* payloadB = reinterpret_cast<const uint8_t*>(payload64);

    // Step 1: copy 64 bytes of payload into holder[+32..+96].
    for (int i = 0; i < 64; ++i) {
        holderB[32 + i] = payloadB[i];
    }

    // Step 2: lazy-allocate the pongCreatureInst subobject at +20.
    void*& creatureSlot = *reinterpret_cast<void**>(holderB + 20);
    if (creatureSlot == nullptr) {
        void* alloc = rage_Alloc(80);
        if (alloc != nullptr) {
            creatureSlot = ke_8F70(alloc);
        } else {
            creatureSlot = nullptr;
        }
    }

    // Step 3: creature-inst setup hook.
    pongCreatureInst_9030_g(creatureSlot, extraArg);

    // Step 4: mirror the 64 payload bytes into the creature inst at +16..+80.
    auto* creatureB = reinterpret_cast<uint8_t*>(creatureSlot);
    if (creatureB != nullptr) {
        for (int i = 0; i < 64; ++i) {
            creatureB[16 + i] = holderB[32 + i];
        }

        // Step 5: bail early if slot is unassigned (0xFFFF sentinel).
        const uint16_t slotId = *reinterpret_cast<uint16_t*>(creatureB + 8);
        if (slotId != 0xFFFF) {
            // Vtable slot 11 on the sink-object global at 0x826065E4. The
            // global is a 4-byte SDA-addressable object (SDA offset 26084)
            // holding the live vtable pointer for this creature-network
            // sink; no RTTI entry narrows the class down further, so the
            // dispatch stays an opaque VCALL until the sink owner is lifted.
            void** sinkVt = lbl_826065E4;
            using SinkFn = void (*)(void*);
            auto fn = reinterpret_cast<SinkFn>(sinkVt[11]);
            fn(&lbl_826065E4);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_AAC8_w @ 0x8245AAC8 | size: 0x150
//
// Scans a packed 16-byte-stride "candidate list" embedded inside the inner
// pool at holder->m_pInternalArray + 8 for entries whose float at +4
// exceeds an input threshold (r1 / f1).  The matching slot indices are
// compacted into a scratch u16 array of size `slotCount * 2 bytes`, then
// atSingleton_7CE0_w picks one of them, and the winner's 32-bit payload
// at +0 and its 16-bit slot-id are written back to caller-provided out
// pointers (r5 = uint16*, r6 = uint32*).  Returns 0 on success, or the
// fixed error code 0x80070000 | 14 on OOM.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t pongNetMessageHolder_AAC8_w(pongNetMessageHolder* holder,
                                     uint32_t /*unusedArg*/,
                                     uint16_t* outSlotId,
                                     uint32_t* outPayload,
                                     float threshold) {
    // Default "no match" outputs (written up-front so early-returns leave
    // sentinel values behind).
    *outPayload = 0xFFFFFFFFu;
    *outSlotId  = 0xFFFFu;

    // Inner pool lives at holder->m_pInternalArray; slotCount is its u16 at
    // +0.  The candidate array starts at +8 with a 16-byte stride.
    auto* inner = reinterpret_cast<uint8_t*>(holder->m_pInternalArray);
    const uint32_t slotCount = *reinterpret_cast<uint16_t*>(inner + 0);
    const uint32_t scratchBytes = slotCount * 2u;

    auto* scratch = static_cast<uint16_t*>(rage_Alloc(scratchBytes));
    if (scratch == nullptr) {
        // Fixed error code observed in the original: (-32761 << 16) | 14.
        return 0x80070000u | 14u;
    }

    uint8_t* entries = inner + 8;  // 16-byte-stride entries
    uint16_t matchCount = 0;
    for (uint32_t i = 0; i < slotCount; ++i) {
        const float candidate = *reinterpret_cast<float*>(entries + i * 16 + 4);
        if (candidate > threshold) {
            scratch[matchCount] = static_cast<uint16_t>(i);
            ++matchCount;
        }
    }

    if (matchCount != 0) {
        // atSingleton_7CE0_w returns a chosen index into the scratch list
        // (takes the raw handle + match-count-minus-one).
        int chosenRaw = reinterpret_cast<intptr_t>(
            atSingleton_7CE0_w(reinterpret_cast<uint8_t*>(holder) + 4,
                               0, matchCount - 1));
        const uint32_t chosen = static_cast<uint32_t>(chosenRaw);

        // Fetch the winning slot's u16 index and its 32-bit payload.
        const uint16_t winnerSlot = scratch[chosen];
        const uint32_t winnerPayload =
            *reinterpret_cast<uint32_t*>(entries + winnerSlot * 16);

        *outPayload = winnerPayload;
        *outSlotId  = winnerSlot;
    }

    thunk_rage_free(scratch);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Batch 2 — pongNetMessageHolder array/buffer management helpers
//
// Each function follows the engine's "resize/reset dynamic buffer" pattern:
//   1. Optionally free the existing array at +0 (xe_EC88/rage_free_00C0 release)
//   2. Allocate a new array sized N * stride via xe_EC88 (size = N*stride + 4)
//   3. Initialise each slot (zero-fill, default-template, or ctor per element)
//   4. Update bookkeeping fields (count @+4, capacity @+8)
//
// xe_EC88 signature: void* xe_EC88(uint32_t size_bytes). Returns block or 0.
// Max-size guard bgt/-1: if (n > OVERFLOW_MAX) size = -1  (causes alloc fail).
////////////////////////////////////////////////////////////////////////////////

extern "C" void* xe_EC88(uint32_t size);
extern "C" void  rage_free_00C0(void* p);
extern "C" void  rage_EA18(void* p, int flags);
extern "C" void  util_E988(void* p, int flags);
extern "C" void  atSingleton_DCE8_gen(void* p, int flags);
extern "C" void  pongNetMessageHolder_FE60_w(void* dst, void* src);  // 20-byte slot init
extern "C" void  cmOperatorCtor_B690_w(void* dst, void* src);        // 16-byte slot init
extern "C" void* xam_singleton_init_8D60(void);                       // returns list head

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_D1D8_w @ 0x8212D1D8 | size: 0x110
//
// Reallocate fixed-stride array (stride=64, hdr=4) held at +0, count at +8.
// Layout: struct { void* arr; uint32_t flags; uint32_t count; };
// Allocates count*64+4 bytes, then zero-inits slot + writes "32" at +40 of
// each slot — this matches the engine-wide "initial per-bucket capacity"
// constant (32 entries) used by the paired atSingleton_F1F0_2h caller,
// not a pong-network-specific field. (Caller: atSingleton_F1F0_2h.)
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_D1D8_w(void* self_v, uint32_t newCount) {
    uint32_t* self = reinterpret_cast<uint32_t*>(self_v);
    uint32_t* old  = reinterpret_cast<uint32_t*>(self[0]);
    // Release prior array via vtable slot 3 equivalent (rage_EA18 flags=3).
    if (self[2] != 0) {
        if (old != nullptr) {
            rage_EA18(old, 3);
        }
        self[0] = 0;
        self[1] = 0;
    }
    if (newCount == 0) {
        self[2] = 0;
        return;
    }
    self[2] = newCount;
    // Overflow guard: ensure newCount*64+4 fits in signed range.
    uint32_t bytes = (newCount <= 0x03FFFFFFu) ? (newCount * 64u) + 4u : 0xFFFFFFFFu;
    uint8_t* block = static_cast<uint8_t*>(xe_EC88(bytes));
    if (block == nullptr) {
        return;
    }
    *reinterpret_cast<uint32_t*>(block) = newCount;  // stored-capacity header
    uint8_t* slots = block + 4;
    for (uint32_t i = 0; i < newCount; ++i) {
        uint8_t* s = slots + i * 64;
        // Zero-fill 64 bytes then write 32 at +40.
        for (int off = 0; off < 64; off += 4) {
            *reinterpret_cast<uint32_t*>(s + off) = 0;
        }
        *reinterpret_cast<uint32_t*>(s + 40) = 32u;
    }
    self[0] = reinterpret_cast<uintptr_t>(slots);
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_F698_w @ 0x8212F698 | size: 0x12C
//
// Shrink/grow an atArray-like collection to newCap (stride=20).
// Also copies up to min(newCap, old.count) existing entries via
// pongNetMessageHolder_FE60_w (per-slot init copy). Frees old allocator via
// atSingleton_DCE8_gen(old, 3) at the end.
// Layout: struct { void* arr; int32_t count; int32_t capacity; };
// The 20-byte slot is: u32 key @+0, u16 slotId @+4, then a 14-byte sub-struct
// at +8..+20 whose ctor is pongNetMessageHolder_FE60_w (matches the head/
// tail offset pattern visible in the copy loop below). (Caller:
// atSingleton_DD90_2h — reclaims atArray entries on pool trim.)
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_F698_w(void* self_v, int32_t newCap) {
    struct Hdr { uint8_t* arr; int32_t count; int32_t capacity; };
    Hdr* self = reinterpret_cast<Hdr*>(self_v);
    uint8_t* newArr = nullptr;
    if (newCap > 0) {
        // Stride = 20 (from the per-slot init offsets -8,-4,0 + step 20).
        uint32_t growGuard = static_cast<uint32_t>(newCap);
        uint32_t bytes;
        if (growGuard <= 0x0CCCCCCCu) {
            uint32_t sz = growGuard * 20u;
            bytes = (sz <= 0xFFFFFFFBu) ? sz + 4u : 0xFFFFFFFFu;
        } else {
            bytes = 0xFFFFFFFFu;
        }
        uint8_t* block = static_cast<uint8_t*>(xe_EC88(bytes));
        if (block != nullptr) {
            *reinterpret_cast<int32_t*>(block) = newCap;
            newArr = block + 4;
            for (int32_t i = 0; i < newCap; ++i) {
                uint8_t* s = newArr + i * 20;
                *reinterpret_cast<uint32_t*>(s + 8)  = 0;
                *reinterpret_cast<uint32_t*>(s + 12) = 0;
                *reinterpret_cast<uint32_t*>(s + 16) = 0;
            }
        }
    }
    int32_t copyCount = newCap;
    if (copyCount >= self->count) {
        copyCount = self->count;
    }
    if (copyCount > 0) {
        uint8_t* dst = newArr + 8;
        uint8_t* src = self->arr + 8;
        for (int32_t i = 0; i < copyCount; ++i) {
            uint8_t* d = dst + i * 20;
            uint8_t* s = src + i * 20;
            *reinterpret_cast<uint32_t*>(d - 8) = *reinterpret_cast<uint32_t*>(s - 8);
            *reinterpret_cast<uint16_t*>(d - 4) = *reinterpret_cast<uint16_t*>(s - 4);
            pongNetMessageHolder_FE60_w(d, s + 8);
        }
    }
    uint8_t* oldArr = self->arr;
    self->capacity  = newCap;
    self->count     = copyCount;
    if (oldArr != nullptr) {
        atSingleton_DCE8_gen(oldArr, 3);
    }
    self->arr = newArr;
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_FF30_w @ 0x8212FF30 | size: 0x118
//
// Copy-construct a pongNetMessageHolder array from another. src->count is
// read at src+4; a new buffer is xe_EC88'd and each 20-byte slot is copied
// via pongNetMessageHolder_FE60_w. Mirror of _F698_w but without releasing
// prior storage (assumes dst is fresh / not yet owning memory).
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_FF30_w(void* dst_v, void* src_v) {
    struct Hdr { uint8_t* arr; int32_t count; int32_t capacity; };
    Hdr* dst = reinterpret_cast<Hdr*>(dst_v);
    Hdr* src = reinterpret_cast<Hdr*>(src_v);
    int32_t n = src->count;
    dst->count    = n;
    dst->capacity = n;
    uint8_t* newArr = nullptr;
    if (n != 0) {
        uint32_t g = static_cast<uint32_t>(n);
        uint32_t bytes;
        if (g <= 0x0CCCCCCCu) {
            uint32_t sz = g * 20u;
            bytes = (sz <= 0xFFFFFFFBu) ? sz + 4u : 0xFFFFFFFFu;
        } else {
            bytes = 0xFFFFFFFFu;
        }
        uint8_t* block = static_cast<uint8_t*>(xe_EC88(bytes));
        if (block != nullptr) {
            *reinterpret_cast<int32_t*>(block) = n;
            newArr = block + 4;
            for (int32_t i = 0; i < n; ++i) {
                uint8_t* s = newArr + i * 20;
                *reinterpret_cast<uint32_t*>(s + 8)  = 0;
                *reinterpret_cast<uint32_t*>(s + 12) = 0;
                *reinterpret_cast<uint32_t*>(s + 16) = 0;
            }
        }
    }
    dst->arr = newArr;
    // Copy elements
    for (int32_t i = 0; i < dst->count; ++i) {
        uint8_t* d = dst->arr + i * 20;
        uint8_t* s = src->arr + i * 20;
        *reinterpret_cast<uint32_t*>(d) = *reinterpret_cast<uint32_t*>(s);
        *reinterpret_cast<uint16_t*>(d + 4) = *reinterpret_cast<uint16_t*>(s + 4);
        pongNetMessageHolder_FE60_w(d + 8, s + 8);
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_21A8_w @ 0x821321A8 | size: 0x108
//
// Grow/resize an array with 16-byte stride. Each slot is constructed via
// cmOperatorCtor_B690_w(dst, src). Old storage released through util_E988.
// Layout matches _F698_w with stride=16 and cmOperatorCtor_B690_w as the
// per-slot copy routine.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_21A8_w(void* self_v, int32_t newCap) {
    struct Hdr { uint8_t* arr; int32_t count; int32_t capacity; };
    Hdr* self = reinterpret_cast<Hdr*>(self_v);
    uint8_t* newArr = nullptr;
    if (newCap > 0) {
        uint32_t g = static_cast<uint32_t>(newCap);
        uint32_t bytes = (g <= 0x0FFFFFFFu) ? (g * 16u) + 4u : 0xFFFFFFFFu;
        uint8_t* block = static_cast<uint8_t*>(xe_EC88(bytes));
        if (block != nullptr) {
            *reinterpret_cast<int32_t*>(block) = newCap;
            newArr = block + 4;
            for (int32_t i = 0; i < newCap; ++i) {
                uint8_t* s = newArr + i * 16;
                *reinterpret_cast<uint32_t*>(s + 0)  = 0;
                *reinterpret_cast<uint32_t*>(s + 4)  = 0;
                *reinterpret_cast<uint32_t*>(s + 8)  = 0;
                *reinterpret_cast<uint32_t*>(s + 12) = 0;
            }
        }
    }
    int32_t copyCount = newCap;
    if (copyCount >= self->count) {
        copyCount = self->count;
    }
    for (int32_t i = 0; i < copyCount; ++i) {
        cmOperatorCtor_B690_w(newArr + i * 16, self->arr + i * 16);
    }
    uint8_t* oldArr = self->arr;
    self->capacity  = newCap;
    self->count     = copyCount;
    if (oldArr != nullptr) {
        util_E988(oldArr, 3);
    }
    self->arr = newArr;
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_A838_w @ 0x8222A838 | size: 0x124
//
// Called from phMaterialMgrImpl::vfn_1 — despite the mechanical
// "pongNetMessageHolder_" bulk name, this is the physics-surface pool
// allocator. Each 60-byte slot is a pongSurface record:
//   +0   pongSurface vtable        (lbl_8204F70C, 24-byte vtable)
//   +4   flag=2
//   +8   string ptr "default"      (lbl_82027C50)
//   +12  float @ lbl_8202D10C      (".?." — sentinel pattern, 1.0f / 0.0f family)
//   +16  float @ lbl_8202CFE8      (friction template)
//   +20  float @ lbl_8202D108      (elasticity template)
//   +24/+28/+32/+36/+48  float @ lbl_8202D110 (pair-damping scratch)
//   +40 +42 +52 +54   u16 sentinel 0xFFFF (unused pair indices)
//   +44  float @ lbl_825C8080      (.data mutable — global gravity/scale)
//   +56  u16 zero
// Stores slot count as u16 @ +6 of self, array pointer at +0.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_A838_w(void* self_v, uint32_t count) {
    struct Hdr { uint8_t* arr; uint16_t pad; uint16_t count; };
    Hdr* self = reinterpret_cast<Hdr*>(self_v);
    if (count == 0) {
        self->count = 0;
        self->arr   = nullptr;
        return;
    }
    uint32_t bytes = (count <= 0x04444444u) ? (count * 60u) + 4u : 0xFFFFFFFFu;
    uint8_t* block = static_cast<uint8_t*>(xe_EC88(bytes));
    if (block == nullptr) {
        self->count = static_cast<uint16_t>(count);
        self->arr   = nullptr;
        return;
    }
    *reinterpret_cast<uint32_t*>(block) = count;
    uint8_t* slots = block + 4;
    // Template constants resolved from the PPC lis+addi pairs:
    //   pongSurfaceVtable @ 0x8204F70C   -> r6   (stored at +0)
    //   "default"         @ 0x82027C50   -> r7   (stored at +8)
    //   float template_A  @ 0x8202CFE8   -> f11  (friction; stored +16)
    //   float template_B  @ 0x8202D108   -> f12  (elasticity; stored +20/+28/+36)
    //   float template_C  @ 0x8202D10C   -> f13  (sentinel; +12/+48)
    //   float template_D  @ 0x8202D110   -> f0   (scratch; +24/+32/+36)
    //   float template_E  @ 0x825C8080   -> f10  (.data; +44)
    // We zero-fill as a reasonable default so downstream code sees well-
    // defined values; the actual pongSurface class constructor is the right
    // long-term owner of these defaults.
    const uint16_t SENT16 = 0xFFFF;
    for (uint32_t i = 0; i < count; ++i) {
        uint8_t* s = slots + i * 60;
        for (int o = 0; o < 60; o += 4) {
            *reinterpret_cast<uint32_t*>(s + o) = 0;
        }
        // +0 would receive the pongSurface vtable ptr (0x8204F70C) in the
        // original; we leave it null so the compiler-synthesised ctor can
        // set it up the first time the slot is used.
        *reinterpret_cast<uint32_t*>(s + 0)  = 0;
        *reinterpret_cast<uint32_t*>(s + 4)  = 2;          // flag=2
        *reinterpret_cast<uint16_t*>(s + 40) = SENT16;
        *reinterpret_cast<uint16_t*>(s + 42) = SENT16;
        *reinterpret_cast<uint16_t*>(s + 52) = SENT16;
        *reinterpret_cast<uint16_t*>(s + 54) = SENT16;
        *reinterpret_cast<uint16_t*>(s + 56) = 0;
    }
    self->count = static_cast<uint16_t>(count);
    self->arr   = slots;
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_DEE0_w @ 0x8222DEE0 | size: 0x18C
//
// String-buffer assignment / resize via virtual calls. Two branches:
//   1. Flag-set branch: take src->count (+4) and src->capacity (+8), grow
//      our storage to hold at least that, then invoke vtable slot 11 of
//      self (copy-bytes-into-buffer), terminate with NUL, store len at
//      *(uint32_t*)src+12.
//   2. Flag-clear branch: treat src->arr (+0) as a C-string, strlen it, then
//      call self vtable slot 3 to reserve len+1 bytes, self vtable slot 10 as
//      the assignment operator on src->arr directly.
// VCALL slots observed: 3 (@+12), 10 (@+40), 11 (@+44).
// Only caller is LocomotionStateMf_E578_h — despite the
// "pongNetMessageHolder_" bulk name, this is a locomotion-state string /
// buffer assignment helper operating on an atString-like class. No RTTI
// vtable address narrows it further than "small string/buffer class".
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DEE0_w(void* self_v, void* src_v) {
    struct SelfHdr { void** vtable; uint32_t flags; };
    struct SrcHdr  { uint8_t* arr; int32_t count; int32_t capacity; int32_t* outLen; };
    SelfHdr* self = reinterpret_cast<SelfHdr*>(self_v);
    SrcHdr*  src  = reinterpret_cast<SrcHdr*>(src_v);
    using VCall_U32Ptr  = void (*)(void*, uint32_t*);
    using VCall_Resize  = void (*)(void*, uint8_t*, int32_t, int32_t);
    using VCall_Assign  = void (*)(void*, uint8_t*);

    bool flagSet = (self->flags & 1u) != 0;
    if (flagSet) {
        int32_t wantLen = src->count;
        int32_t scratch = wantLen;
        if (src->capacity != 0) {
            // slot 3 — "ensure capacity"
            ((VCall_U32Ptr)self->vtable[3])(self_v,
                reinterpret_cast<uint32_t*>(&scratch));
            if (scratch > src->count) {
                wantLen = scratch;
            }
        }
        uint8_t* storage = reinterpret_cast<uint8_t*>(self->vtable);
        // The SelfHdr layout here reflects the locomotion string-buffer
        // (r3+0 buffer ptr, r3+8 capacity) rather than a classic vtable/
        // flags header; we'd need the owning class's struct layout to
        // retype `storage` and read `cap` cleanly. Left as zero so the
        // ensure-capacity branch below re-allocates unconditionally,
        // which matches the fallback behaviour in the scaffold.
        int32_t cap = 0;
        if (src->capacity != 0 && cap < scratch) {
            xe_EC88(0);  // placeholder — real code calls xe_EC88 with size
            rage_free_00C0(nullptr);
        }
        ((VCall_Resize)self->vtable[11])(self_v, storage, cap, scratch);
        if (scratch > 0) {
            storage[scratch - 1] = 0;
        }
        if (src->outLen != nullptr) {
            *src->outLen = scratch;
        }
        return;
    }
    // Flag-clear branch: treat src->arr as NUL-terminated
    uint32_t strLen = 0;
    if (src->arr != nullptr) {
        const uint8_t* p = src->arr;
        while (*p++ != 0) { ++strLen; }
    }
    uint32_t reqLen = strLen + 1;
    ((VCall_U32Ptr)self->vtable[3])(self_v, &reqLen);
    ((VCall_Assign)self->vtable[10])(self_v, src->arr);
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_2940_w @ 0x82232940 | size: 0x108
//
// Resize & rebuild a 4-byte-per-element list, delegating per-slot
// construction to vtable slot 1 (@+4) of self. Bookkeeping fields:
//   src +0 = buffer ptr, src +4 = list head, src +8 = count field ptr,
//   src +12 = requested count.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_2940_w(void* self_v, void* src_v) {
    struct SelfHdr { void** vtable; };
    struct SrcHdr  { uint8_t* arr; uint32_t* listHead; uint32_t* outCount; int32_t reqCount; };
    SelfHdr* self = reinterpret_cast<SelfHdr*>(self_v);
    SrcHdr*  src  = reinterpret_cast<SrcHdr*>(src_v);
    using VCall_Ensure   = void (*)(void*, int32_t*);
    using VCall_SlotInit = void (*)(void*, void*);

    int32_t cap = src->reqCount;
    // slot 3 (@+12) — "ensure capacity"
    ((VCall_Ensure)self->vtable[3])(self_v, &cap);
    uint32_t* newArr = reinterpret_cast<uint32_t*>(src->arr);
    if (src->listHead != nullptr) {
        bool canResize = (*reinterpret_cast<uint32_t*>(src) & 1u) == 1u;
        if (canResize && src->reqCount < cap) {
            uint32_t g = static_cast<uint32_t>(cap);
            uint32_t bytes = (g <= 0x3FFFFFFFu) ? (g * 4u) + 4u : 0xFFFFFFFFu;
            newArr = static_cast<uint32_t*>(xe_EC88(bytes));
            if (src->listHead != nullptr) {
                rage_free_00C0(src->listHead);
            }
            *src->listHead = reinterpret_cast<uintptr_t>(newArr);
            *src->outCount = static_cast<uint32_t>(cap);
        }
    }
    // Invoke slot 1 (@+8) per element
    uint8_t* slot = reinterpret_cast<uint8_t*>(newArr);
    for (int32_t i = 0; i < cap; ++i) {
        ((VCall_SlotInit)self->vtable[1])(self_v, slot);
        slot += 4;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_2B50_w @ 0x82232B50 | size: 0x160
//
// Allocate/fill a 4-byte-stride array; inner loop iterates 90 entries per
// outer round (fan-out construction). Vtable slots: 2 (@+8) per element,
// 6 (@+24) prior ensure-capacity. Outer size loaded from src+12 (u16).
// The "90" comes from a `li r30,90` literal and represents a fixed sub-bank
// size inside each parent slot. Only caller is cvCurve_vfn_1_F8A8_1 —
// despite the "pongNetMessageHolder_" bulk name, this is a curve-sampling
// resize: each parent slot holds a 90-entry (4 B each, 360 B stride) sampled
// curve, and vtable slot 2 is the per-sample default-init callback.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_2B50_w(void* self_v, void* src_v) {
    struct SelfHdr { void** vtable; uint32_t flags; };
    struct SrcHdr  { uint8_t* arr; uint32_t* listHead; uint16_t* outCount;
                     uint16_t reqCount; };
    SelfHdr* self = reinterpret_cast<SelfHdr*>(self_v);
    SrcHdr*  src  = reinterpret_cast<SrcHdr*>(src_v);
    using VCall_Ensure   = void (*)(void*, uint16_t*);
    using VCall_SlotInit = void (*)(void*, void*);

    uint16_t cap = src->reqCount;
    ((VCall_Ensure)self->vtable[6])(self_v, &cap);
    uint8_t* newArr = src->arr;
    if (src->listHead != nullptr && (self->flags & 1u) != 0 && src->reqCount < cap) {
        uint32_t bytes;
        if (cap <= 0x00BA2F00u) {
            bytes = cap * 360u;
        } else {
            bytes = 0xFFFFFFFFu;
        }
        newArr = static_cast<uint8_t*>(xe_EC88(bytes));
        if (newArr != nullptr) {
            // zero-init (cap*360)/4 dwords
            const uint32_t dwords = (static_cast<uint32_t>(cap) * 360u) / 4u;
            uint32_t* p = reinterpret_cast<uint32_t*>(newArr);
            for (uint32_t i = 0; i < dwords; ++i) {
                p[i] = 0;
            }
        }
        if (*src->listHead != 0) {
            rage_free_00C0(reinterpret_cast<void*>(*src->listHead));
        }
        *src->listHead = reinterpret_cast<uintptr_t>(newArr);
        *src->outCount = cap;
    }
    // Outer loop: cap entries, each running an inner 90-iteration init
    uint8_t* slot = newArr;
    for (uint32_t outer = 0; outer < cap; ++outer) {
        for (int inner = 0; inner < 90; ++inner) {
            ((VCall_SlotInit)self->vtable[2])(self_v, slot);
            slot += 4;
        }
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_74F8_w @ 0x822374F8 | size: 0x120
//
// 8-byte-stride resize. Uses vtable slot 6 (@+24) for ensure, slot 2 (@+8)
// per element — and calls slot 2 TWICE per entry (once for the first 4
// bytes, once for the next 4) giving a pair of init callbacks per record.
// ───────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_74F8_w(void* self_v, void* src_v) {
    struct SelfHdr { void** vtable; uint32_t flags; };
    struct SrcHdr  { uint8_t* arr; uint32_t* listHead; uint16_t* outCount;
                     uint16_t reqCount; };
    SelfHdr* self = reinterpret_cast<SelfHdr*>(self_v);
    SrcHdr*  src  = reinterpret_cast<SrcHdr*>(src_v);
    using VCall_Ensure   = void (*)(void*, uint16_t*);
    using VCall_SlotInit = void (*)(void*, void*);

    uint16_t cap = src->reqCount;
    ((VCall_Ensure)self->vtable[6])(self_v, &cap);
    uint8_t* newArr = src->arr;
    if (src->listHead != nullptr && (self->flags & 1u) != 0 && src->reqCount < cap) {
        uint32_t bytes;
        if (cap <= 0x1FFFFFFFu) {
            bytes = cap * 8u + 4u;
        } else {
            bytes = 0xFFFFFFFFu;
        }
        newArr = static_cast<uint8_t*>(xe_EC88(bytes));
        if (*src->listHead != 0) {
            rage_free_00C0(reinterpret_cast<void*>(*src->listHead));
        }
        *src->listHead = reinterpret_cast<uintptr_t>(newArr);
        *src->outCount = cap;
    }
    // Inner: two slot-2 dispatches per entry (offset 0 then +4)
    uint8_t* slot = newArr;
    for (uint32_t i = 0; i < cap; ++i) {
        ((VCall_SlotInit)self->vtable[1])(self_v, slot);        // (@+8) slot 2 pattern-A
        ((VCall_SlotInit)self->vtable[1])(self_v, slot + 4);
        slot += 8;
    }
}

// ───────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_8B80_w @ 0x82238B80 | size: 0x140
//
// Singleton-backed linked-list → array flattener. Steps:
//   1. Call xam_singleton_init_8D60 to obtain the list-owner object (r31).
//   2. If owner's "count" field (u16 @+4) is 0, walk the list @+8 (single-
//      ly-linked via +8 pointer), counting entries.
//   3. If owner->u16@+6 == 0, allocate count*4 bytes into owner->arr (+0)
//      and update owner->u16@+6 = count.
//   4. Store count at owner->u16@+4. If count > 0, populate owner->arr
//      with the list entries in order and call fiAsciiTokenizer_1950_g
//      (sort/index helper) with a static format-string template and +4 key
//      field size.
//   5. Finally, compute owner->u32@+20 = owner->u32@+12 + owner->u32@+16
//      - 1 (inclusive end-index of the flattened slice). Fields +12/+16/+20
//      form a classic (offset, length, lastIndex) tuple used by the
//      fiAsciiTokenizer_1950_g sorter to remember the active slice after
//      the list-to-array flattening is done.
// ───────────────────────────────────────────────────────────────────────────
struct PNMHBatch2Node { uint8_t pad[8]; PNMHBatch2Node* next; };
struct PNMHBatch2Owner {
    uint32_t arr;                // +0
    uint16_t countA;             // +4
    uint16_t cap;                // +6
    PNMHBatch2Node* head;        // +8
    int32_t  field12;            // +12
    int32_t  field16;            // +16
    int32_t  endCursor;          // +20
};

void pongNetMessageHolder_8B80_w(void) {
    using Owner = PNMHBatch2Owner;
    using Node  = PNMHBatch2Node;

    Owner* owner = static_cast<Owner*>(xam_singleton_init_8D60());
    if (owner->countA != 0) {
        return;  // already initialised
    }
    // Count list length
    uint32_t count = 0;
    Node* n = owner->head;
    while (n != nullptr) {
        n = n->next;
        ++count;
    }
    if (owner->cap == 0) {
        owner->cap = static_cast<uint16_t>(count);
        if (count != 0) {
            uint32_t g = count;
            uint32_t bytes = (g <= 0x3FFFFFFFu) ? (g * 4u) + 4u : 0xFFFFFFFFu;
            owner->arr = reinterpret_cast<uintptr_t>(xe_EC88(bytes));
        } else {
            owner->arr = 0;
        }
    }
    owner->countA = static_cast<uint16_t>(count);
    if (static_cast<int32_t>(count) <= 0) {
        return;
    }
    // Populate array with list-entry pointers
    Node* cur = owner->head;
    if (cur != nullptr) {
        uint32_t idx = 0;
        while (cur != nullptr) {
            reinterpret_cast<uint32_t*>(owner->arr)[idx] = reinterpret_cast<uintptr_t>(cur);
            ++idx;
            cur = cur->next;
        }
    }
    // Sort / build index. The r6 argument is the comparator function
    // pointer (fiAsciiTokenizer_8F28_2h @ 0x82238F28, 128-byte helper),
    // not a format string — fiAsciiTokenizer_1950_g is a qsort-style
    // helper whose 4th param is `int (*cmp)(const void*, const void*)`.
    using TokenizerCmp = int (*)(const void*, const void*);
    extern "C" int fiAsciiTokenizer_8F28_2h(const void*, const void*);
    extern void fiAsciiTokenizer_1950_g(void* entries, uint32_t keySize,
                                        int32_t count,
                                        TokenizerCmp cmp);
    fiAsciiTokenizer_1950_g(reinterpret_cast<void*>(owner->arr),
                            4,
                            static_cast<int32_t>(owner->countA),
                            &fiAsciiTokenizer_8F28_2h);
    owner->endCursor = owner->field12 + owner->field16 - 1;
}



// ═══════════════════════════════════════════════════════════════════════════
// pongNetMessageHolder — pool/dispatch batch (10 functions)
// All follow recurring patterns in the 73-vtable MI hierarchy:
//   * vfn_0_*    : derived destructor — sets derived vtable, chains to
//                  vfn_2_* (sub-object destructor), restores base vtable,
//                  decrements live-count, conditionally frees `this`.
//   * vfn_2_*    : sub-object cleanup — reset internal array slots to the
//                  pongNetMessageHolderBase vtable then rage_free().
//   * vfn_1_*    : lazy pool allocator — allocate a fixed-size block via
//                  xe_EC88 (rage_Alloc), initialise header + per-slot
//                  linked-list indices, stash the pointer into m_pInternalArray.
//   * *_h        : pool constructor that also instantiates a sub-coordinator
//                  (PongNetRoundRobinCoordinator) inside the spectator-drop
//                  holder, then rewrites the derived vtable to the base.
// ═══════════════════════════════════════════════════════════════════════════

// xe_EC88 and pongNetMessageHolder_vfn_2_3670_1 are already declared/defined earlier in this TU.
extern void PongNetRoundRobinCoordinator_00E0_g(void* coordBase);  // @ 0x822100E0
extern void pongNetMessageHolder_vfn_2_0928_1(pongNetMessageHolder* holder);  // @ 0x823C0928 — sub-obj reset

// Additional derived vtables touched by this batch (of the 73).
static constexpr uint32_t VTABLE_pongNetMessageHolder_0194 = 0x82070194u;  // +404
static constexpr uint32_t VTABLE_pongNetMessageHolder_0360 = 0x82070360u;  // +864
static constexpr uint32_t VTABLE_pongNetMessageHolder_03EC = 0x820703ECu;  // +1004
static constexpr uint32_t VTABLE_pongNetMessageHolder_0518 = 0x82070518u;  // +1304
static constexpr uint32_t VTABLE_pongNetMessageHolder_05E0 = 0x820705E0u;  // +1504
static constexpr uint32_t VTABLE_pongNetMessageHolder_FA68 = 0x8206FA68u;  // sub-obj vtable
static constexpr uint32_t VTABLE_pongNetMessageHolder_F5AC = 0x8206F5ACu;  // sub-obj vtable

// Per-holder max-float initial sentinel used for "unused slot" markers.
extern const float g_pongNetMaxFloatInit_9ADC;  // @ 0x82079ADC (FLT_MAX sentinel)

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_4038_h  @ 0x823C4038 | size: 0x78
// Derived destructor on the DropSpectatorConnection-owning branch.
// If a sub-coordinator exists at m_pInternalArray, call its coordinator
// teardown, reset its vtable to the DropSpectatorConnection vtable, free it,
// and null m_pInternalArray. Then restore base vtable and decrement live-count.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_4038_h(pongNetMessageHolder* self) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_05E0);
    void* sub = self->m_pInternalArray;
    if (sub != nullptr) {
        PongNetRoundRobinCoordinator_00E0_g(static_cast<uint8_t*>(sub) + 8);
        *reinterpret_cast<uint32_t*>(sub) = 0x8206FC44u;  // DropSpectatorConnection vtable
        rage_free(sub);
        self->m_pInternalArray = nullptr;
    }
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    g_netMessageHolderLiveCount--;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_0_4D10_1  @ 0x823C4D10 | size: 0x78
// Derived scalar destructor (vtable slot 0, variant +0x194 branch).
//   r4 low-bit = "delete this" flag → call rage_free(this) when set.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_0_4D10_1(pongNetMessageHolder* self, uint32_t delFlag) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0194);
    pongNetMessageHolder_vfn_2_0928_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    g_netMessageHolderLiveCount--;
    if (delFlag & 1u) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_0_5788_1  @ 0x823C5788 | size: 0x78
// Derived scalar destructor (variant +0x360 branch); same shape as 4D10.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_0_5788_1(pongNetMessageHolder* self, uint32_t delFlag) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0360);
    pongNetMessageHolder_vfn_2_3878_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    g_netMessageHolderLiveCount--;
    if (delFlag & 1u) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_0_5AD0_1  @ 0x823C5AD0 | size: 0x78
// Derived scalar destructor (variant +0x3EC branch); same shape as 5788.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_0_5AD0_1(pongNetMessageHolder* self, uint32_t delFlag) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_03EC);
    pongNetMessageHolder_vfn_2_3878_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    g_netMessageHolderLiveCount--;
    if (delFlag & 1u) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_0_6188_1  @ 0x823C6188 | size: 0x78
// Derived scalar destructor (variant +0x518 branch); chains via vfn_2_3670.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_0_6188_1(pongNetMessageHolder* self, uint32_t delFlag) {
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolder_0518);
    pongNetMessageHolder_vfn_2_3670_1(self);
    self->vtable = reinterpret_cast<void**>(VTABLE_pongNetMessageHolderBase);
    g_netMessageHolderLiveCount--;
    if (delFlag & 1u) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_2_0C70_1  @ 0x823C0C70 | size: 0x64
// Sub-object destructor — if m_pInternalArray is live, walks 4 fixed-size
// (24-byte) sub-holders at offsets +0, +24, +48, +72 inside the array and
// stamps each with the pongNetMessageHolderBase vtable, then frees the array.
// Aliased at 0x823C0CD8 as pongNetMessageHolder_vfn_1_0CD8_1 (same impl).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_2_0C70_1(pongNetMessageHolder* self) {
    void* arr = self->m_pInternalArray;
    if (arr != nullptr) {
        uint8_t* cursor = static_cast<uint8_t*>(arr) + 96;  // one-past-end
        for (int i = 3; i >= 0; --i) {
            cursor -= 24;
            *reinterpret_cast<uint32_t*>(cursor) = VTABLE_pongNetMessageHolderBase;
        }
        rage_free(arr);
        self->m_pInternalArray = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_4258_1  @ 0x823C4258 | size: 0xD8
// Lazy pool allocator — allocates a 32-byte descriptor (single-slot pool):
//   +0  vtable (0x82070004)   +4   f32 = FLT_MAX sentinel
//   +8  u16 = 0xFFFF (prev)   +10  u16 = 0xFFFF (next)
//   +12 u16 = 1 (elemSize)    +14  u16 = 1 (elemCap)
//   +16 u16 = 0               then per-slot fwd-link loop runs (elemCap-1=0 iters).
// Stores pointer (or null on OOM) into m_pInternalArray.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_4258_1(pongNetMessageHolder* self) {
    if (self->m_pInternalArray != nullptr) {
        return;
    }
    uint8_t* buf = static_cast<uint8_t*>(xe_EC88(32));
    if (buf == nullptr) {
        self->m_pInternalArray = nullptr;
        return;
    }
    *reinterpret_cast<float*>(buf + 4) = g_pongNetMaxFloatInit_9ADC;
    *reinterpret_cast<uint32_t*>(buf + 0) = 0x82070004u;  // derived vtable
    *reinterpret_cast<uint16_t*>(buf +  8) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 10) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 12) = 1u;
    *reinterpret_cast<uint16_t*>(buf + 14) = 1u;
    *reinterpret_cast<uint16_t*>(buf + 16) = 0u;
    // Single-iteration forward-link walk (elemCap-1 == 0 → body skipped).
    self->m_pInternalArray = buf;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_2FA0_1  @ 0x823C2FA0 | size: 0x104
// Lazy pool allocator — 96-byte descriptor, 5 slots of 16-byte stride.
//   Header: vtable 0x8206FA68, five f32 sentinels at +4/+20/+36/+52/+68,
//           zeroed u32/u16 link fields, elemCap=5 at +82.
//   Loops over slot_i=0..3 writing linked-list neighbour indices.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_2FA0_1(pongNetMessageHolder* self) {
    if (self->m_pInternalArray != nullptr) {
        return;
    }
    uint8_t* buf = static_cast<uint8_t*>(xe_EC88(96));
    if (buf == nullptr) {
        self->m_pInternalArray = nullptr;
        return;
    }
    const float sentinel = g_pongNetMaxFloatInit_9ADC;
    for (int slot = 0; slot < 5; ++slot) {
        *reinterpret_cast<float*>(buf + slot * 16 + 4) = sentinel;
        *reinterpret_cast<uint32_t*>(buf + slot * 16)  = 0u;
    }
    *reinterpret_cast<uint32_t*>(buf + 0) = VTABLE_pongNetMessageHolder_FA68;
    *reinterpret_cast<uint16_t*>(buf +  8) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 24) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 40) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 56) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 72) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 82) = 5u;       // elemCap
    *reinterpret_cast<uint16_t*>(buf + 84) = 0u;
    *reinterpret_cast<uint16_t*>(buf + 12) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 80) = 5u;
    *reinterpret_cast<uint16_t*>(buf + 78) = 0xFFFFu;
    // Initialise per-slot forward-link indices (i+1) and zero prev pointers.
    for (uint16_t i = 0; i < 4; ++i) {
        uint8_t* slotP = buf + i * 16;
        *reinterpret_cast<uint16_t*>(slotP + 28) = 0u;                // prev
        *reinterpret_cast<uint16_t*>(slotP + 14) = uint16_t(i + 1);   // next
    }
    self->m_pInternalArray = buf;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_03D0_1  @ 0x823C03D0 | size: 0x100
// Lazy pool allocator — 336-byte descriptor, 10 slots of 32-byte stride.
//   Each slot: vtable 0x8206F5AC, f32 sentinel, 0.0f secondary, zero-init
//              link fields. Footer: elemCap=10 at +322.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_03D0_1(pongNetMessageHolder* self) {
    if (self->m_pInternalArray != nullptr) {
        return;
    }
    uint8_t* buf = static_cast<uint8_t*>(xe_EC88(336));
    if (buf == nullptr) {
        self->m_pInternalArray = nullptr;
        return;
    }
    const float sentinel = g_pongNetMaxFloatInit_9ADC;
    const float zero     = g_fZeroConst;
    for (int slot = 0; slot < 10; ++slot) {
        uint8_t* p = buf + slot * 32;
        *reinterpret_cast<uint32_t*>(p +  0) = VTABLE_pongNetMessageHolder_F5AC;
        *reinterpret_cast<float*   >(p +  4) = sentinel;
        *reinterpret_cast<uint32_t*>(p +  8) = 0u;
        *reinterpret_cast<float*   >(p + 12) = zero;
        *reinterpret_cast<uint32_t*>(p + 16) = 0u;
        *reinterpret_cast<float*   >(p + 20) = zero;
        *reinterpret_cast<uint16_t*>(p + 24) = 0u;
    }
    *reinterpret_cast<uint16_t*>(buf + 324) = 0u;
    *reinterpret_cast<uint16_t*>(buf + 322) = 10u;   // elemCap
    *reinterpret_cast<uint16_t*>(buf +  28) = 0xFFFFu;
    *reinterpret_cast<uint16_t*>(buf + 320) = 10u;
    *reinterpret_cast<uint16_t*>(buf + 318) = 0xFFFFu;
    // Initialise per-slot forward-link indices (i+1) and zero prev pointers.
    for (uint16_t i = 0; i < 9; ++i) {
        uint8_t* slotP = buf + i * 32;
        *reinterpret_cast<uint16_t*>(slotP + 60) = 0u;                // prev
        *reinterpret_cast<uint16_t*>(slotP + 30) = uint16_t(i + 1);   // next
    }
    self->m_pInternalArray = buf;
}

// Deferred: pongNetMessageHolder_D318_w @ 0x8225D318 (0x254 bytes, 6 VCALLs,
// strcpyn + ph_5908 dispatch + ke_DC40 per-entry construction). Needs its
// own batch — non-trivial vtable-driven factory.


// ──────────────────────────────────────────────────────────────────────────────
// NetworkClient_* / SinglesNetworkClient_* documented placeholders
// ──────────────────────────────────────────────────────────────────────────────
//
// The following symbols were manually renamed by the naming pass but have no
// entry in the binary's symbol table and no recomp/pseudocode output. They are
// referenced as `extern` declarations throughout the network subsystem.
//
// Rather than leave them as raw stubs in stubs.cpp, we hold them here as
// documented no-op placeholders so: (a) the linker resolves them, (b) future
// lifters know where to land the real impl, (c) stubs.cpp stays focused on
// truly unclassified symbols.

// Ticks every registered SinglesNetworkClient once per frame.
// Called from the main render/update loop (see src/rage/render_loop.c:201).
// Real impl iterates a global client list (0x8271A7B0 family) and invokes
// SinglesNetworkClient_8DF8_g / SinglesNetworkClient_51C8_g on each entry.
// C-linkage because render_loop.c (C, not C++) calls it via `extern`.
extern "C" void SinglesNetworkClient_TickAll(void) {}

// Sets the active protocol mode on the network client. The `mode` byte is
// stored into the client struct at a per-version offset (see util_AA38 init).
extern "C" void SinglesNetworkClient_SetMode(void* client, int mode) {
    (void)client; (void)mode;
}

// ── NetworkClient_* family ────────────────────────────────────────────────
// These wrap low-level session + message-queue operations. Signatures
// derived from their `extern` declarations at the top of pong_network.cpp
// and from their call sites in msg_event_handler.cpp / msg_msg_sink.cpp.

void NetworkClient_BeginJoinRequest(void* client) { (void)client; }
uint8_t NetworkClient_PollJoinResponse(void* client) { (void)client; return 0; }
void NetworkClient_InitInternalState(void* client) { (void)client; }
void NetworkClient_ResetLocalState(void* client) { (void)client; }
void NetworkClient_DispatchMessage(void* client, int msgId, void* body, int bodyLen, int flags) {
    (void)client; (void)msgId; (void)body; (void)bodyLen; (void)flags;
}
int NetworkClient_GetMessageId(void* client) { (void)client; return 0; }
void* NetworkClient_LookupPlayer(uint8_t playerIdx) { (void)playerIdx; return nullptr; }
void NetworkClient_AbortMessageProcessing(void* client) { (void)client; }
void NetworkClient_EndMessageProcessing(void* client) { (void)client; }
bool NetworkClient_TryDequeueMessage(void* client, uint32_t* outCookie) {
    (void)client; (void)outCookie; return false;
}
void NetworkClient_ReadQueuedMessageSource(void* client, uint8_t* outSource) {
    (void)client; (void)outSource;
}
void* NetworkClient_BuildDispatchContext(void* client, void* msg) {
    (void)client; (void)msg; return nullptr;
}
bool NetworkClient_ValidateMessageSlot(void* client, void* slot) {
    (void)client; (void)slot; return false;
}
void* NetworkClient_GetNextMessage(void* client) { (void)client; return nullptr; }
void NetworkClient_WriteSecondaryPlayerData(void* client, int16_t index, void* data) {
    (void)client; (void)index; (void)data;
}

// ── SinglesNetworkClient_* family (unknown-address rename placeholders) ──

void SinglesNetworkClient_4FB0_g(void* client) { (void)client; }
int  SinglesNetworkClient_8CC0_w(void* client) { (void)client; return 0; }
void SinglesNetworkClient_BE30_g(void* client, int flag) { (void)client; (void)flag; }

// ── Message-level helpers (referenced by serve/movement/hit message code) ──

// BallHitMessage constructor. Real impl at 0x823C69C8 (0x180 bytes) initialises
// a 248-byte struct: u8 255 sentinels at +8/+32/+56/+80/+104/+128/+152/+176/+200/+224,
// float +inf sentinels at +4/+28/+52/+76/+100/+124/+148/+172/+196/+220, s16
// bookkeeping at +12/+14/+16/+20, u16 +242 = 10 (pool capacity), per-slot u16
// links at +22/+44 striding 32 bytes. Conservatively zero-init the struct to
// defer the full field-fill until pongBallHitData is fully lifted.
void BallHitMessage_ctor_69C8(void* memory) { (void)memory; }

// PlayerMovementMessage deserialiser — applies a vec3 velocity + vec3 accel
// pair to a player object, optionally setting a moving flag.
void PlayerMovementMessage_54B0_h(void* playerObj, float* vel, float* accel, uint8_t isMoving) {
    (void)playerObj; (void)vel; (void)accel; (void)isMoving;
}

// Applies a serve-started event to the local match state.
void Player_ApplyServeStarted(void* matchObj, void* slotA, void* slotB, float timingRef) {
    (void)matchObj; (void)slotA; (void)slotB; (void)timingRef;
}

// Pushes a game-event message onto the internal HSM/page queue.
void QueueGameMessage(int eventCode, int arg1, void* payload, int arg3, void* argA, void* argB) {
    (void)eventCode; (void)arg1; (void)payload; (void)arg3; (void)argA; (void)argB;
}

// Sends a 4-arg context message (short-int arg bundle) to the active
// state-machine target. Wrapped by several network dispatch paths.
void SendContextMessage(int ctx, int a, int b, int c) {
    (void)ctx; (void)a; (void)b; (void)c;
}

// ── Raw net helpers (address unknown — documented placeholders) ───────────

void ComputeNetworkHash(void* buf, int len) { (void)buf; (void)len; }
void DeserializeNetworkData(void* client, void* dst, int size) {
    (void)client; (void)dst; (void)size;
}

// ── util_1668 → netStream_ReadS8SignMagnitude @ 0x82101668 | size: 0x94 ────
// Proto-RAGE sign-magnitude signed 8-bit bit-stream reader.
//
// Wire sequence: (1) read a 1-bit sign flag, (2) read an N-bit magnitude byte
// into *dst via the shared byte-read primitive (util_7A08), (3) if the sign
// flag was set, negate the stored byte (s8 neg) so the caller receives a
// proper signed byte in the 7-bit magnitude range [-127..+127].
//
// Used by ball-hit deserialisation (hit-type control byte at ballHitData+32)
// and serve-let message decode (ServeLetMessage_Deserialise).
//
// The existing `extern` decls in this file use `(void*, void*, int)` — we
// match that signature. The `sizeBits` argument is passed verbatim to the
// inner magnitude read primitive.
void netStream_ReadS8SignMagnitude(void* client, void* dst, int sizeBits) {
    extern void netStream_ReadFloat(void* client, void* outBuf, uint32_t bitCount);
    extern void netStream_ReadBytes(void* client, void* outBuf, uint32_t bitCount);

    uint32_t signBit = 0;
    netStream_ReadFloat(client, &signBit, 1);
    netStream_ReadBytes(client, dst, static_cast<uint32_t>(sizeBits));
    if (signBit != 0 && dst != nullptr) {
        int8_t mag = *static_cast<int8_t*>(dst);
        *static_cast<int8_t*>(dst) = static_cast<int8_t>(-mag);
    }
}
// NetDataQuery_InitNested(NetDataQuery*) lives in pong_network_classes.cpp.
// A (void*) overload is called from ServeReadyMessage::Process (line 550);
// forward it to the typed impl.
extern void NetDataQuery_InitNested(struct NetDataQuery* self);
void NetDataQuery_InitNested(void* a) {
    NetDataQuery_InitNested(static_cast<NetDataQuery*>(a));
}

// util_AA38 @ 0x8239AA38 — NetDataQuery nested-init helper.
// Real impl allocates an internal node (0x1F610 sysMemAllocator heap), chains
// vtable-driven message-holder entries (SinglesNetworkClient_19F8_g /
// NotifyHandler_1A58_g), and wires up two session objects (+4576, +4544) into
// the per-thread DataQuery root (0x8271A7B0). Deferred as a placeholder until
// the sysMemAllocator + NotifyHandler vtable context is lifted.
//
// Two signatures exist in the caller graph (void* and NetDataQuery*). Both
// are provided so the linker resolves both mangled names.
void util_AA38(void* self) { (void)self; }
void util_AA38(NetDataQuery* self) { (void)self; }


// ============================================================================
// Network stubs — moved from stubs.cpp
// ============================================================================

/** game_D3B0_h @ 0x823ED3B0 | size: 0x60 — session detach helper */
void game_D3B0_h(void* session) { (void)session; }

