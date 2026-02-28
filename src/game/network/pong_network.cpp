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
 * Serialisation uses SinglesNetworkClient_8DF8_g (write float, size=32) and
 * util_7A08 (write byte, size=8).  Deserialisation uses WriteFloatToNetworkStream
 * and SinglesNetworkClient_6838_g (read byte, size=8).
 *
 * ── Vtable layout ─────────────────────────────────────────────────────────
 *   slot 1 = Deserialise  (scalar_destructor label is wrong — it reads data in)
 *   slot 2 = Serialise    (writes data out)
 *   slot 4 = Process      (applies message: calls ServeStartedMessage_5728)
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
#include "pong_player.hpp"
#include <cstdlib>
#include <cstring>

// ── External functions ─────────────────────────────────────────────────────
// Network serialisation helpers
extern void  SinglesNetworkClient_8DF8_g(void* client, void* buf, int size);  // write float
extern void  SinglesNetworkClient_6838_g(void* client, uint8_t val, int size); // write byte
extern void* SinglesNetworkClient_58E8_g(uint8_t playerIdx);                  // lookup player

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
extern void nop_8240E6D0(const char* fmt, ...);  // debug nop / assert print
extern void ServeStartedMessage_5728(void* matchObj, void* slotA, void* slotB, float timingRef);  // serve-start coordinator

// pongNetMessageHolder helpers used by static init/dtor wrappers.
extern void pongNetMessageHolder_vfn_2_0868_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_3878_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_18D0_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_FD70_1(pongNetMessageHolder* holder);
extern void pongNetMessageHolder_vfn_2_1628_1(pongNetMessageHolder* holder);
extern pongNetMessageHolder* pongNetMessageHolder_FAE0_isl();
extern void pongNetMessageHolder_5038_w();
extern void rage_27C0(void* instance);
extern "C" void rage_free_00C0(void* ptr);

namespace {
struct PongNetMessageHolderStaticNode {
    void** m_vtable;
    uint32_t m_field4;
    uint32_t m_field8;
};

static_assert(sizeof(PongNetMessageHolderStaticNode) == 0x0C);

inline uint32_t& NetMessageHolderInitHead() {
    return *reinterpret_cast<uint32_t*>(0x826066A0u);
}

inline uint32_t& NetMessageHolderLiveCount() {
    return *reinterpret_cast<uint32_t*>(0x826066A4u);
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
    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_timingRef = timingRef;

    // Read 16-byte target position vector.
    DeserializeNetworkData(client, &m_targetPos, 32);
    WriteBallHitDataToNetwork(&m_targetPos, client);  // finalise position read

    // Read four velocity floats.
    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_velocityX = timingRef;

    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_velocityY = timingRef;

    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_velocityZ = timingRef;

    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
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
    SinglesNetworkClient_6838_g(client, m_playerIndex,  8);
    SinglesNetworkClient_6838_g(client, m_bNotServer, 8);
    SinglesNetworkClient_6838_g(client, m_bSecondaryPlayer, 8);
}


// ===========================================================================
// vfn_4 (slot 4) — Process  @ 0x823B9100 | size: 0x190
//
// Applies a received ServeStartedMessage to the live game state.
// Called by the network dispatcher when the message has been fully
// deserialised.
//
// Steps:
//  1. Look up the pongPlayer for m_playerIdx via SinglesNetworkClient_58E8_g.
//  2. Assert that the player's action state (+468) is non-zero (debug guard).
//  3. Copy m_targetPos into the player's lerp table entry:
//       player->lerpTable[player->lerpSlot + 17] = m_targetPos
//     using the per-player 416-byte lerp table (g_pPlayerLerpTable base).
//  4. Write m_playerIdx2 (+0xE2) into playerState->m_pRecoveryState[+40].
//  5. Call ServeStartedMessage_5728 (the serve-start coordinator) with
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
    void* player = SinglesNetworkClient_58E8_g(m_playerIndex);

    // Step 2: debug assert — action state must be initialised.
    int32_t actionState = *(int32_t*)((uint8_t*)player + 468);
    if (actionState == 0) {
        nop_8240E6D0("ServeStartedMessage::Process(): player action state is null");
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
    ServeStartedMessage_5728(matchObj, &slotA, &slotB, m_timingRef);

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
    extern void SinglesNetworkClient_0E18_g(void*, uint32_t*, int);
    SinglesNetworkClient_0E18_g(networkClient, &statusResult, 2);
    
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
    NetDataQuery_ctor_A458(this);
    
    // Note: Memory deallocation is handled by the compiler-generated
    // delete operator, which checks the flags parameter passed by the caller.
    // If flags & 1, rage_free_00C0 is called to return memory to the pool.
}

// ===========================================================================
// pongNetMessageHolder_4718_w @ 0x82584718 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1134.
// ===========================================================================
void pongNetMessageHolder_4718_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(0x825D1134u);
    holder->vtable = reinterpret_cast<void**>(0x82070180u);
    pongNetMessageHolder_vfn_2_0868_1(holder);
    holder->vtable = reinterpret_cast<void**>(0x8206FA88u);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_4AD8_w @ 0x82584AD8 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1224.
// ===========================================================================
void pongNetMessageHolder_4AD8_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(0x825D1224u);
    holder->vtable = reinterpret_cast<void**>(0x82070248u);
    pongNetMessageHolder_vfn_2_3878_1(holder);
    holder->vtable = reinterpret_cast<void**>(0x8206FA88u);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_5958_w @ 0x82585958 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1614.
// ===========================================================================
void pongNetMessageHolder_5958_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(0x825D1614u);
    holder->vtable = reinterpret_cast<void**>(0x82070590u);
    pongNetMessageHolder_vfn_2_18D0_1(holder);
    holder->vtable = reinterpret_cast<void**>(0x8206FA88u);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_4358_w @ 0x82584358 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D1044.
// ===========================================================================
void pongNetMessageHolder_4358_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(0x825D1044u);
    holder->vtable = reinterpret_cast<void**>(0x820700B8u);
    pongNetMessageHolder_vfn_2_FD70_1(holder);
    holder->vtable = reinterpret_cast<void**>(0x8206FA88u);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_4BF8_w @ 0x82584BF8 | size: 0x5C
//
// Static dtor wrapper for the holder instance at 0x825D126C.
// ===========================================================================
void pongNetMessageHolder_4BF8_w() {
    auto* holder = reinterpret_cast<pongNetMessageHolder*>(0x825D126Cu);
    holder->vtable = reinterpret_cast<void**>(0x82070284u);
    pongNetMessageHolder_vfn_2_1628_1(holder);
    holder->vtable = reinterpret_cast<void**>(0x8206FA88u);
    --NetMessageHolderLiveCount();
}

// ===========================================================================
// pongNetMessageHolder_2228_2h @ 0x82582228 | size: 0x40
//
// Adds the static node at 0x825D1960 to the holder singleton list.
// ===========================================================================
void pongNetMessageHolder_2228_2h() {
    pongNetMessageHolder* holder = pongNetMessageHolder_FAE0_isl();
    const uint32_t previousHead = holder->field_0x0008;

    auto* node = reinterpret_cast<PongNetMessageHolderStaticNode*>(0x825D1960u);
    node->m_field8 = previousHead;

    holder->field_0x0008 = 0x825D1960u;
    holder->field_0x000c = previousHead + 1;
}

// ===========================================================================
// pongNetMessageHolder_EAD8_sp @ 0x8257EAD8 | size: 0x14
//
// Zeroes the 0x100-byte static holder pool block at 0x8261A0D0.
// ===========================================================================
void pongNetMessageHolder_EAD8_sp() {
    std::memset(reinterpret_cast<void*>(0x8261A0D0u), 0, 0x100);
}

// ===========================================================================
// pongNetMessageHolder_08B0_p @ 0x825808B0 | size: 0x34
//
// Registers the static holder dtor callback and tracks init ordering.
// ===========================================================================
void pongNetMessageHolder_08B0_p() {
    auto* node = reinterpret_cast<PongNetMessageHolderStaticNode*>(0x825D13A4u);
    const uint32_t previousHead = NetMessageHolderInitHead();

    NetMessageHolderInitHead() = 0x825D13A4u;
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
    self->vtable = reinterpret_cast<void**>(0x82070180u);
    pongNetMessageHolder_vfn_2_0868_1(self);
    self->vtable = reinterpret_cast<void**>(0x8206FA88u);
    --NetMessageHolderLiveCount();

    if ((flags & 1) != 0) {
        rage_free_00C0(self);
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
        rage_free_00C0(self);
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
        rage_free_00C0(arrayPtr);
        
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
    // (The assembly checks bit 0 of flags and calls rage_free_00C0 conditionally)
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
    uint8_t* poolMgr = *reinterpret_cast<uint8_t**>(0x825D164C);
    
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
    return *reinterpret_cast<void**>(0x825D1638);
}

/**
 * RequestDataMessage::vfn_7 — Get type name string
 * @ 0x823BF130 | size: 0x4
 *
 * Returns debug/RTTI type name string for this message class.
 * Used for logging and debugging network message traffic.
 */
const char* RequestDataMessage::vfn_7() {
    return reinterpret_cast<const char*>(0x8206EE0C);
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
//  - Getting network status via SinglesNetworkClient_B2A8_g
//  - Checking a global singleton flag
//  - Finding a network message slot via SinglesNetworkClient_9318_g
//  - Initializing the message slot with values 1 and 3
//  - Calling SinglesNetworkClient_B320_g if network status is non-zero
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
    void* networkClient = m_pNetworkClient;

    // Query network status
    uint8_t networkStatus = SinglesNetworkClient_B2A8_g(networkClient);

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
            nop_8240E6D0(g_szNetworkDebugMsg);
        }
    }

    // Find available network message slot
    extern const char g_szMessageType[];  // @ 0x8205DFB0
    uint32_t clientState = *(uint32_t*)((uint8_t*)networkClient + 92);
    void* messageSlot = SinglesNetworkClient_9318_g((void*)clientState, g_szMessageType);

    // Initialize message slot if found
    if (messageSlot != nullptr) {
        *(uint32_t*)((uint8_t*)messageSlot + 0) = 1;  // Set state to 1
        *(uint32_t*)((uint8_t*)messageSlot + 4) = 3;  // Set type to 3
    }

    // Perform network client update if status is non-zero
    if (networkStatus != 0) {
        SinglesNetworkClient_B320_g(networkClient);
    }

    // Mark all players as ready
    m_bAllPlayersReady = true;
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
    SinglesNetworkClient_67C8_g(client, secondaryIndex, additionalData);
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
extern void snSession_6C98_h(void* session);

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ProcessSessions @ 0x822FDDC0 | size: 0x150
//
// Processes all active network sessions in two phases:
// 1. First loop: Calls net_6BA0_fw and snSession_6C98_h on each session
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
            
            // Always call snSession_6C98_h
            snSession_6C98_h(sessionPtr);
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
    
    // Initialize the structures (calls ke_1B00)
    extern void ke_1B00(void* msg);
    ke_1B00(&msg1);
    ke_1B00(&msg2);
    
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
    extern bool SinglesNetworkClient_5EA0_g(ProcessingFlags* flags, uint32_t* queue);
    extern void SinglesNetworkClient_70A0_g(void* msgPtr, uint8_t* sourcePtr);
    extern void SinglesNetworkClient_5A40_g(ProcessingFlags* flags);
    extern void SinglesNetworkClient_5998_g(ProcessingFlags* flags);
    
    while (true) {
        // Check if there are more messages to process
        bool hasMessage = SinglesNetworkClient_5EA0_g(&procFlags, messageQueue);
        
        if (!hasMessage) {
            // No more messages, clean up and exit
            SinglesNetworkClient_5998_g(&procFlags);
            break;
        }
        
        // Process the current message
        void* currentMsg = (void*)((uint8_t*)procFlags.field0 + 40);
        SinglesNetworkClient_70A0_g(currentMsg, sourcePtr);
        
        // Advance to next message
        SinglesNetworkClient_5A40_g(&procFlags);
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
    this->field_0x000c++;
}


// ===========================================================================
// SinglesNetworkClient::ProcessNetworkTimingUpdate @ 0x823E01F8 | size: 0x15C
//
// Processes network timing updates and manages join request state machine.
// This function is called periodically with a timestamp to advance the
// network client's state based on timing thresholds.
//
// State machine:
//   state 0: Initial join request state - calls SinglesNetworkClient_0F80_g
//   state 1: Waiting for join response - calls SinglesNetworkClient_1178_g
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
        SinglesNetworkClient_0F80_g(this);
        
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
        uint8_t responseResult = SinglesNetworkClient_1178_g(this);
        
        if (responseResult == 0) {
            // Join request accepted - transition to accepting state
            game_D3B0_h(this);
            
            // Create event object on stack
            rage::EvtAcceptJoinRequestSucceeded evt;
            evt.vtable = (void**)0x82072D94;  // EvtAcceptJoinRequestSucceeded vtable
            
            util_DA08(&evt);  // Initialize event
            
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
        util_DA08(&evt);
        
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
            *(void**)newNode = (void*)0x82072D94;  // EvtAcceptJoinRequestSucceeded vtable
            
            // Copy 8 bytes of event data (two uint32_t values)
            uint32_t eventData1 = *(uint32_t*)((char*)&evt + 4);
            uint32_t eventData2 = *(uint32_t*)((char*)&evt + 8);
            
            // Store as 64-bit value (big-endian to little-endian conversion)
            uint64_t combinedData = ((uint64_t)eventData1 << 32) | eventData2;
            *(uint64_t*)((char*)newNode + 4) = combinedData;
            
            // Add node to session
            void* sessionNodeList = (char*)sessionData + 8;
            snSession_AddNode_C068(sessionNodeList, newNode);
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
            // Call SinglesNetworkClient_0738_g to write the string
            // This is likely WriteStringToStream(this, stringBuffer, stringLength)
            extern uint32_t SinglesNetworkClient_0738_g(void* client, const char* str, uint32_t length);
            SinglesNetworkClient_0738_g(this, stringBuffer, stringLength);
        }
    }
    
    // Clear/reset 8 bits in the stream
    // SinglesNetworkClient_0448_g is ReadBitsFromStream
    extern uint32_t SinglesNetworkClient_0448_g(void* client, uint32_t value, int bitWidth);
    SinglesNetworkClient_0448_g(this, 0, 8);
    
    // Return string length + 1
    return stringLength + 1;
}
