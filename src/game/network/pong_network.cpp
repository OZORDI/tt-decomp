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
        void* newArray = xe_EC88(1296);
        
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
    const uint32_t sentinelAddr = 0x8271A2CC;
    
    // Get the head of the list
    uint32_t* current = g_threadpool_head;
    
    // Check if list is not empty
    if (*current != 0) {
        // Traverse the list looking for the sentinel or end
        uint32_t* prev = current;
        uint32_t* node = (uint32_t*)*current;
        
        while (node != nullptr && (uint32_t)node != sentinelAddr) {
            // Move to next node (offset +20 is the next pointer)
            prev = (uint32_t*)((char*)node + 20);
            node = (uint32_t*)*prev;
        }
        
        // If we found a node (not at sentinel), remove it from the list
        if (node != nullptr) {
            // Get the next pointer from the node we're removing
            uint32_t* next = (uint32_t*)*((uint32_t*)((char*)node + 20));
            
            // Update the previous node's next pointer to skip this node
            *prev = (uint32_t)next;
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
    const uint32_t vtableAddr = 0x8206F740;
    
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
        *(float*>(entryPtr + 120) = 0.0f;
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
    uint32_t state = field_0x0004;
    
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
    extern void* SinglesNetworkClient_0978_g(void* msg, int param1, void* rttiStr1, void* rttiStr2, int param2);
    extern bool  SinglesNetworkClient_A250_g(void* queueBase, void* msgBuffer);
    extern int   SinglesNetworkClient_4928_g(void* msgHandler);
    extern void* SinglesNetworkClient_9720_g(void* stackBuf, void* client);
    extern void  SinglesNetworkClient_4128_g(void* dispatcher, int msgId, void* context, int param1, int param2);
    extern void  SinglesNetworkClient_0268_g(void* context);
    extern void* SinglesNetworkClient_C838_g(void* currentMsg);
    
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
        void* msgHandler = SinglesNetworkClient_0978_g(
            currentMsg,
            0,
            (void*)g_szRageRTTI,
            (void*)g_szCreateMachineRTTI,
            0
        );
        
        // Validate the message is ready to process
        // Check if message is in the queue and ready
        char msgBuffer[96];  // Stack buffer at offset +96 from msgHandler
        bool isValid = SinglesNetworkClient_A250_g(
            (char*)networkState + 3756,  // Queue base for validation
            (char*)msgHandler + 96
        );
        
        if (isValid) {
            // Get the message ID/type
            int messageId = SinglesNetworkClient_4928_g(msgHandler);
            
            if (messageId != -1) {
                // Prepare dispatch context on stack
                char dispatchContext[128];  // Stack buffer at r1+80
                void* context = SinglesNetworkClient_9720_g(dispatchContext, this);
                
                // Dispatch the message to the appropriate handler
                // Parameters: dispatcher base, message ID, context, 1 (priority), 0
                void* dispatcher = (char*)networkState + 64;
                SinglesNetworkClient_4128_g(dispatcher, messageId, context, 1, 0);
                
                // Clean up the dispatch context
                SinglesNetworkClient_0268_g(dispatchContext);
            }
        }
        
        // Move to the next message in the queue
        currentMsg = SinglesNetworkClient_C838_g(currentMsg);
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
//  1. Calls nop_8240E6D0 with "top_spin" string (debug/logging)
//  2. Checks g_loop_obj_ptr[578] (boolean flag)
//  3. If flag is false, checks g_loop_obj_ptr[12] == 8 (state check)
//  4. If state matches, sets g_input_obj_ptr[48] = 4
//  5. Posts page group message with event code 14386 and m_tournamentId
// ===========================================================================
void TournamentCompleteMessage::Process()
{
    // Debug logging with "top_spin" string
    extern const char g_szTopSpin[];  // @ 0x8206CB54
    nop_8240E6D0(g_szTopSpin);
    
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
    eventData[0] = (int32_t)m_tournamentId;  // Sign-extend byte to int
    PostPageGroupMessage(14386, 128, 1, 0, (int)eventData);
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
    const uint32_t vtableAddr = 0x8206FB74;
    
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
    const uint32_t cleanupVtable = 0x82033C04;  // crIKPart vtable for cleanup
    
    // Clean up first array at offset +8
    void* array1 = *(void**)((uint8_t*)this + 8);
    if (array1 != nullptr) {
        // Set vtable for 2 elements (192 bytes total, 96 bytes each)
        for (int i = 1; i >= 0; i--) {
            uint8_t* element = (uint8_t*)array1 + (i * 96);
            *(uint32_t*)element = cleanupVtable;
        }
        rage_free_00C0(array1);
    }
    
    // Clean up second array at offset +12
    void* array2 = *(void**)((uint8_t*)this + 12);
    if (array2 != nullptr) {
        // Set vtable for 2 elements (192 bytes total, 96 bytes each)
        for (int i = 1; i >= 0; i--) {
            uint8_t* element = (uint8_t*)array2 + (i * 96);
            *(uint32_t*)element = cleanupVtable;
        }
        rage_free_00C0(array2);
    }
    
    // Clean up array at offset +80
    uint8_t* offset80 = (uint8_t*)this + 80;
    *(uint32_t*)offset80 = 0x820336D4;  // Set intermediate vtable
    
    uint16_t count80 = *(uint16_t*)(offset80 + 14);
    if (count80 > 0) {
        void* buffer80 = *(void**)(offset80 + 8);
        rage_free_00C0(buffer80);
    }
    *(uint32_t*)offset80 = cleanupVtable;
    
    // Clean up array at offset +16
    uint8_t* offset16 = (uint8_t*)this + 16;
    *(uint32_t*)offset16 = 0x820336B0;  // Set intermediate vtable
    
    uint16_t count16 = *(uint16_t*)(offset16 + 14);
    if (count16 > 0) {
        void* buffer16 = *(void**)(offset16 + 8);
        rage_free_00C0(buffer16);
    }
    *(uint32_t*)offset16 = cleanupVtable;
    
    // Set final vtable
    *(uint32_t*)this = 0x8203377C;
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
                rage_free_00C0(buffer1);
                
                // Free buffer at offset +16
                void* buffer2 = *(void**)((uint8_t*)element + 16);
                rage_free_00C0(buffer2);
                
                // Free the element itself
                rage_free_00C0(element);
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
extern "C" void* xe_EC88(uint32_t size);  // Memory allocator @ 0x820DEC88
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
static uint32_t& g_netMessageHolderLiveCount = *reinterpret_cast<uint32_t*>(0x826066A4);

// Global message holder instances
static pongNetMessageHolder& g_netMessageHolder1 = *reinterpret_cast<pongNetMessageHolder*>(0x825D129C);
static pongNetMessageHolder& g_netMessageHolder2 = *reinterpret_cast<pongNetMessageHolder*>(0x825D12B4);


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_4CB8_w @ 0x82584CB8 | size: 0x5C
//
// Static cleanup wrapper for network message holder instance 1.
// Called during static deinitialization to clean up the message holder.
// Decrements the global live instance counter.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_4CB8_w() {
    // Set initial vtable to pongNetMessageHolder
    g_netMessageHolder1.vtable = reinterpret_cast<void**>(0x820702AC);
    
    // Call initialization function
    pongNetMessageHolder_vfn_2_1770_1(&g_netMessageHolder1);
    
    // Change vtable to base class (pongNetMessageHolderBase)
    g_netMessageHolder1.vtable = reinterpret_cast<void**>(0x8206FA88);
    
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
    g_netMessageHolder2.vtable = reinterpret_cast<void**>(0x820702C0);
    
    // Call initialization function
    pongNetMessageHolder_vfn_2_18D0_1(&g_netMessageHolder2);
    
    // Change vtable to base class (pongNetMessageHolderBase)
    g_netMessageHolder2.vtable = reinterpret_cast<void**>(0x8206FA88);
    
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
    if (m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for message pool (12016 bytes)
    void* memory = xe_EC88(12016);
    
    if (memory != nullptr) {
        // Construct the message object
        pongNetMessageHolder_6778_wrh(memory);
        m_pMessagePool = memory;
    } else {
        m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FC68_1 @ 0x823BFC68 | size: 0x54
//
// Lazy initialization for AcceptMessage (176 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FC68_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for AcceptMessage (176 bytes)
    void* memory = xe_EC88(176);
    
    if (memory != nullptr) {
        // Construct AcceptMessage
        pongNetMessageHolder_68D0_wrh(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FCC0_1 @ 0x823BFCC0 | size: 0x54
//
// Lazy initialization for BallHitMessage (256 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FCC0_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for BallHitMessage (256 bytes)
    void* memory = xe_EC88(256);
    
    if (memory != nullptr) {
        // Construct BallHitMessage
        BallHitMessage_ctor_69C8(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FD18_1 @ 0x823BFD18 | size: 0x54
//
// Lazy initialization for network message (8976 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FD18_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for message (8976 bytes)
    void* memory = xe_EC88(8976);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6B48_wrh(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FEE0_1 @ 0x823BFEE0 | size: 0x54
//
// Lazy initialization for network message (96 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FEE0_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for message (96 bytes)
    void* memory = xe_EC88(96);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6C98_wrh(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_FFA0_1 @ 0x823BFFA0 | size: 0x54
//
// Lazy initialization for network message (368 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_FFA0_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for message (368 bytes)
    void* memory = xe_EC88(368);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6D68_wrh(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_0810_1 @ 0x823C0810 | size: 0x54
//
// Lazy initialization for network message (176 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_0810_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for message (176 bytes)
    void* memory = xe_EC88(176);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6E30_wrh(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder_vfn_1_0990_1 @ 0x823C0990 | size: 0x54
//
// Lazy initialization for network message (16016 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_vfn_1_0990_1(pongNetMessageHolder* holder) {
    // Check if message is already initialized
    if (holder->m_pMessagePool != nullptr) {
        return;
    }
    
    // Allocate memory for message (16016 bytes)
    void* memory = xe_EC88(16016);
    
    if (memory != nullptr) {
        // Construct message
        pongNetMessageHolder_6FF8_wrh(memory);
        holder->m_pMessagePool = memory;
    } else {
        holder->m_pMessagePool = nullptr;
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
extern void SinglesNetworkClient_8DF8_g(void* client, void* buf, int size);
extern void SinglesNetworkClient_68A8_g(void* client, int16_t value, int bitWidth);
extern void WriteFloatToNetworkStream(void* client, float value);
extern uint8_t util_7970(void* client, void* dst, int bitWidth);


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
    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_timingRef = timingRef;
    
    // Read 16-bit data identifier using util_7970
    // util_7970 reads a bit-packed value and handles sign extension
    util_7970(client, &m_dataId, 16);
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
    SinglesNetworkClient_68A8_g(client, m_dataId, 16);
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
extern void util_0AF0(void* client, void* data, int16_t size);
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
    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_timingRef = timingRef;
    
    // Read data type identifier (16-bit field)
    util_7970(client, &m_dataType, 16);
    
    // Read data size (16-bit field)
    util_7970(client, &m_dataSize, 16);
    
    // Read variable-length data payload
    // Size is determined by m_dataSize field
    util_0AF0(client, &m_dataPayload, m_dataSize);
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
    __asm__ volatile("eieio");
    
    // Write value 2048 to control register 2
    gpuBase[13320 / 4] = 2048;
    
    // EIEIO barrier
    __asm__ volatile("eieio");
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
    uint32_t thisFlags = field_0x0004;
    uint32_t otherFlags = other->field_0x0004;
    
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
    extern void SinglesNetworkClient_2F28_g(void* state);
    void* internalState = (char*)this + 784;
    SinglesNetworkClient_2F28_g(internalState);
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
    extern void SinglesNetworkClient_2FD8_g(void* client);
    SinglesNetworkClient_2FD8_g(this);
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
 * SinglesNetworkClient_97B8_g @ 0x822F97B8 | size: 0x80 (128 bytes)
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
void* SinglesNetworkClient_97B8_g(void* pThis, const char* searchString)
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
    SinglesNetworkClient_6838_g(networkClient, fieldMask, 4);
    SinglesNetworkClient_6838_g(networkClient, this->playerIndex, 1);
    
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
    SinglesNetworkClient_6918_g(networkClient, this->isMoving);
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
    void* playerObject = SinglesNetworkClient_58E8_g(this->playerIndex);
    
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
//   3. Frees the entire array via rage_free_00C0
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
        const uint32_t BASE_VTABLE = 0x8206C304;  // PongNetMessage base vtable
        
        // Reset second message (offset 640)
        *(uint32_t*)((uint8_t*)messageArray + 640) = BASE_VTABLE;
        
        // Reset first message (offset 0)
        *(uint32_t*)((uint8_t*)messageArray + 0) = BASE_VTABLE;
        
        // Free the entire array
        rage_free_00C0(messageArray);
        
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
    // Save current bit position
    int savedBitPos = m_bitPosition;
    
    // Try to read the string data
    extern int util_0AF0(void* ctx, void* base);  // Network read helper
    int bytesRead = util_0AF0(this, buffer);
    
    if (bytesRead > 0) {
        // Null-terminate the string
        buffer[bytesRead + maxSize - 1] = '\0';
        
        // Update bit position (8 bits per character)
        m_bitPosition = savedBitPos + 8;
        
        // Count the actual string length
        int length = 1;
        char* ptr = buffer;
        while (*ptr != '\0') {
            ptr++;
            m_bitPosition += 8;
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
    bool readHigh = SinglesNetworkClient_8DF8_g(this, &highBits, 32);
    
    if (readHigh) {
        // Read low 32 bits
        bool readLow = SinglesNetworkClient_8DF8_g(this, &lowBits, 32);
        
        if (readLow) {
            // Combine into 64-bit value: (high << 32) | low
            *outValue = ((uint64_t)highBits << 32) | lowBits;
            return true;
        }
    }
    
    // Read failed - still store the partial result
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
        bool success = Read64BitValue(dataPtr, 64);
        
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
    m_vtable = nullptr;
    m_flags = 0;
    *(uint32_t*)((char*)this + 8) = 0;
    *(uint32_t*)((char*)this + 12) = 0;
    *(uint32_t*)((char*)this + 16) = 0;
    *(uint32_t*)((char*)this + 20) = 0;
    *(uint32_t*)((char*)this + 24) = 0;
    m_bitPosition = 0;
    *(uint32_t*)((char*)this + 32) = 0;
    
    // Initialize network buffers
    SinglesNetworkClient_0268_g(this);
    SinglesNetworkClient_0268_g(this);
    
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
    SinglesNetworkClient_8AE0_g(this);
    
    // Save current bit positions
    int savedBitPos = m_bitPosition;
    int savedWritePos = *(int*)((char*)this + 32);
    
    // Temporarily set bit position to 16
    m_bitPosition = 16;
    
    // Read current value from stream
    uint32_t currentValue = 0;
    SinglesNetworkClient_8DF8_g(this, &currentValue, 16);
    
    // Restore bit position
    m_bitPosition = savedBitPos;
    
    // Apply 14-bit mask to input value and merge with current value
    uint32_t maskedValue = (value & 0x3FFF) | (currentValue & 0xFFFFC000);
    
    // Temporarily set write position to 16
    *(int*)((char*)this + 32) = 16;
    
    // Write the merged value
    SinglesNetworkClient_0448_g(this, maskedValue, 16);
    
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
    int savedBitPos = m_bitPosition;
    
    // Temporarily set bit position to 16
    m_bitPosition = 16;
    
    // Read 16-bit value
    uint32_t value = 0;
    SinglesNetworkClient_8DF8_g(this, &value, 16);
    
    // Restore bit position
    m_bitPosition = savedBitPos;
    
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
        int savedBitPos = m_bitPosition;
        
        // Temporarily set bit position to 32
        m_bitPosition = 32;
        
        // Read 16-bit timestamp
        SinglesNetworkClient_8DF8_g(this, &timestamp, 16);
        
        // Restore bit position
        m_bitPosition = savedBitPos;
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
        int savedBitPos = m_bitPosition;
        
        // Set bit position to 0 (read from start)
        m_bitPosition = 0;
        
        // Read 16-bit value
        SinglesNetworkClient_8DF8_g(this, &seqNum, 16);
        
        // Restore bit position
        m_bitPosition = savedBitPos;
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
extern void rage_free_00C0(void* ptr);                         // Memory deallocator

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
    *(void**)(obj + 0) = (void*)0x820701F8;  // g_pongNetMessageHolderVtable
    
    // Call cleanup method (vfn_2)
    pongNetMessageHolder_vfn_2_24B8_1(thisPtr);
    
    // Set base class vtable
    *(void**)(obj + 0) = (void*)0x8206FA88;  // g_pongNetMessageHolderBaseVtable
    
    // Decrement global instance counter
    g_netMessageHolderCount--;
    
    // Check if we should free memory
    bool shouldFreeMemory = (shouldFree & 0x1) != 0;
    
    if (shouldFreeMemory) {
        rage_free_00C0(thisPtr);
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
    pongNetMessageHolder_FAE0_isl(thisPtr);
    
    // Get global list head pointer
    void** globalListHead = (void**)0x825D1900;
    
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
// HitMessage::Deserialise  [vtable slot 1 @ 0x823B5FE0 | size: 0x5C]
// Reads base hit message fields from the network stream: timing float + hit flags byte.
// ─────────────────────────────────────────────────────────────────────────────
void HitMessage::Deserialise(void* client) {
    // Read 32-bit float from stream into temporary buffer
    float timingRef;
    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);

    // Store timing reference at +0x04
    m_timingRef = timingRef;

    // Read 8-bit hit flags byte into +0x08
    ReadBitsFromStream(client, &m_hitFlags, 8);
}

// ─────────────────────────────────────────────────────────────────────────────
// HitDataMessage::Deserialise  [vtable slot 1 @ 0x823B5B58 | size: 0x80]
// Reads hit data message fields: timing float, ball hit data block,
// secondary timing float, and player index byte.
// ─────────────────────────────────────────────────────────────────────────────
void HitDataMessage::Deserialise(void* client) {
    // Read timing reference float
    float timingRef;
    SinglesNetworkClient_8DF8_g(client, &timingRef, 32);
    m_timingRef = timingRef;

    // Read ball hit data block at +0x10 (192-byte structure)
    DeserializeNetworkData(client, &m_ballHitData, 32);

    // Read secondary timing float (recovery/power value)
    float recoveryTiming;
    SinglesNetworkClient_8DF8_g(client, &recoveryTiming, 32);
    m_recoveryTiming = recoveryTiming;

    // Read player index byte at +0xD4
    ReadBitsFromStream(client, &m_playerIndex, 8);
}

// ─────────────────────────────────────────────────────────────────────────────
// SpectatorBallHitMessage::Deserialise  [vtable slot 1 @ 0x823B6B00 | size: 0x44]
// Reads spectator ball hit: delegates to HitMessage::Deserialise for the
// base fields, then reads the ball hit data block.
// ─────────────────────────────────────────────────────────────────────────────
void SpectatorBallHitMessage::Deserialise(void* client) {
    // Read base HitMessage fields (timing float + hit flags byte)
    HitMessage::Deserialise(client);

    // Read ball hit data block at +0x10
    DeserializeNetworkData(client, &m_ballHitData, 32);
}

// ─────────────────────────────────────────────────────────────────────────────
// RemoteServeReadyMessage::Deserialise  [vtable slot 1 @ 0x823B6ED8 | size: 0x48]
// Reads remote serve ready: delegates to HitMessage::Deserialise for base
// fields, then reads a 16-bit serve parameter.
// ─────────────────────────────────────────────────────────────────────────────
void RemoteServeReadyMessage::Deserialise(void* client) {
    // Read base HitMessage fields (timing float + hit flags byte)
    HitMessage::Deserialise(client);

    // Read 16-bit serve parameter at +0x0C
    util_7830(client, &m_serveParam, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// RemoteServeReadyMessage::Serialise  [vtable slot 2 @ 0x823B6F20 | size: 0x60]
// Writes remote serve ready fields to the outgoing network stream:
// timing float, hit flags byte, and 16-bit serve parameter.
// ─────────────────────────────────────────────────────────────────────────────
void RemoteServeReadyMessage::Serialise(void* client) {
    // Write timing reference float
    WriteFloatToNetworkStream(client, m_timingRef);

    // Write hit flags as 8-bit unsigned
    SinglesNetworkClient_6838_g(client, m_hitFlags, 8);

    // Write 16-bit serve parameter
    SinglesNetworkClient_6838_g(client, m_serveParam, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// MatchTimeSyncMessage::Deserialise  [vtable slot 1 @ 0x823B70A8 | size: 0x88]
// Reads match time sync fields: two 64-bit timestamps, a timing float,
// a 16-bit frame counter, then applies a scale factor to the timing value.
// ─────────────────────────────────────────────────────────────────────────────
void MatchTimeSyncMessage::Deserialise(void* client) {
    // Read two 64-bit timestamp values at +0x08 and +0x10
    SinglesNetworkClient_EA98_g(&m_localTimestamp, client);
    SinglesNetworkClient_EA98_g(&m_remoteTimestamp, client);

    // Read timing float
    float syncTiming;
    SinglesNetworkClient_8DF8_g(client, &syncTiming, 32);
    m_syncDelta = syncTiming;

    // Read 16-bit frame counter at +0x1C
    util_7830(client, &m_frameCounter, 16);

    // Apply scale factor from global config to sync delta
    // Scale factor is at g_matchTimeSyncConfig + 12 (0x825CAEC4)
    extern float* g_pMatchTimeSyncScale;  // @ 0x825CAEB8 + 12
    m_syncDelta *= g_pMatchTimeSyncScale[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// MatchTimeSyncMessage::Serialise  [vtable slot 2 @ 0x823B7130 | size: 0x88]
// Writes match time sync fields to the outgoing network stream:
// two 32-bit timestamps, three floats, and a 16-bit frame counter.
// ─────────────────────────────────────────────────────────────────────────────
void MatchTimeSyncMessage::Serialise(void* client) {
    // Write local timestamp as 32-bit value
    SinglesNetworkClient_68A8_g(client, m_localTimestamp, 32);

    // Write local timing float
    WriteFloatToNetworkStream(client, m_localTiming);

    // Write remote timestamp as 32-bit value
    SinglesNetworkClient_68A8_g(client, m_remoteTimestamp, 32);

    // Write remote timing float
    WriteFloatToNetworkStream(client, m_remoteTiming);

    // Write sync delta float
    WriteFloatToNetworkStream(client, m_syncDelta);

    // Write 16-bit frame counter
    SinglesNetworkClient_6838_g(client, m_frameCounter, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForceMatchTimeSyncMessage::Deserialise  [vtable slot 1 @ 0x823B7AA0 | size: 0x4C]
// Reads forced time sync fields: a 64-bit timestamp and a 16-bit parameter.
// ─────────────────────────────────────────────────────────────────────────────
void ForceMatchTimeSyncMessage::Deserialise(void* client) {
    // Read 64-bit timestamp at +0x08
    SinglesNetworkClient_EA98_g(&m_timestamp, client);

    // Read 16-bit sync parameter at +0x10
    util_7830(client, &m_syncParam, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForceMatchTimeSyncMessage::Serialise  [vtable slot 2 @ 0x823B7AF0 | size: 0x60]
// Writes forced time sync fields to the outgoing network stream:
// a 32-bit timestamp, a float, and a 16-bit parameter.
// ─────────────────────────────────────────────────────────────────────────────
void ForceMatchTimeSyncMessage::Serialise(void* client) {
    // Write timestamp as 32-bit value
    SinglesNetworkClient_68A8_g(client, m_timestamp, 32);

    // Write timing float
    WriteFloatToNetworkStream(client, m_timing);

    // Write 16-bit sync parameter
    SinglesNetworkClient_6838_g(client, m_syncParam, 16);
}

// ─────────────────────────────────────────────────────────────────────────────
// ForceMatchTimeSyncMessage::Process  [vtable slot 3 @ 0x823B7608 | size: 0x34]
// Applies the forced time sync by updating the session's time sync state.
// Formats the result as a debug string and logs it (via no-op in retail).
// ─────────────────────────────────────────────────────────────────────────────
void ForceMatchTimeSyncMessage::Process() {
    // Format timestamp for debug: converts the sync timestamp to a string
    char* formattedTime = SinglesNetworkClient_BF30(&m_timestamp);

    // Log the formatted time string (no-op in retail build)
    // String at 0x8206F298 references the sync debug output
    nop_8240E6D0(formattedTime);
}
