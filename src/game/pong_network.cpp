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
