/**
 * message.cpp — Game Message Class Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Original source: .\message.cpp (path embedded at 0x8206F0F4)
 *
 * Implements the network message classes used for game events during
 * table tennis matches.  These messages are serialized across the network
 * in online matches and processed locally in offline play.
 *
 * All message classes use a free-list pool allocator pattern:
 *   - Allocate() returns the message to the pool's free list
 *   - GetPool() returns the global pool for this message type
 *   - Each pool is a fixed-size array with a linked free list using
 *     16-byte (HitMessage) or 24-byte (BallHitMessage) entries
 *
 * Lifted functions: 28 total across 4 classes
 */

#include "message.hpp"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions — Network serialization
// ─────────────────────────────────────────────────────────────────────────────

// Read a 32-bit float from a network bitstream.
// @ 0x82238DF8
extern void netStream_ReadFloat(void* stream, float* outValue, uint32_t bitCount);

// Read bytes from a network bitstream into a buffer.
// @ 0x820D7A08
extern void netStream_ReadBytes(void* stream, void* outBuf, uint32_t bitCount);

// Read a uint16 from a network bitstream.
// @ 0x820D7830
extern void netStream_ReadU16(void* stream, void* outBuf, uint32_t bitCount);

// Read a uint16 from a network bitstream (variant for signed data).
// @ 0x820D7970
extern void netStream_ReadS16(void* stream, void* outBuf, uint32_t bitCount);

// Write a float to a network bitstream.
// @ 0x820D6990
extern void netStream_WriteFloat(void* stream, float value);

// Write an unsigned integer to a network bitstream.
// @ 0x820D6838
extern void netStream_WriteUint(void* stream, uint32_t value, uint32_t bitCount);

// Write a signed integer to a network bitstream.
// @ 0x820D68A8
extern void netStream_WriteSint(void* stream, int32_t value, uint32_t bitCount);

// ─────────────────────────────────────────────────────────────────────────────
// External functions — Game logic
// ─────────────────────────────────────────────────────────────────────────────

// Resolve a player index to a pongPlayer pointer from the singles client.
// @ 0x823B58E8
extern void* SinglesNetworkClient_GetPlayer(uint8_t playerIndex);

// Enter serve mode for a player.
// @ 0x82192668
extern void pongPlayer_EnterServeMode(void* player);

// Post a game event message to the HSM / page-group system.
// @ 0x8225E6E0
extern void PostGameMessage(uint32_t messageId, uint32_t flags,
                            uint32_t param1, uint32_t param2);

// Post a game event with arbitrary data payload.
// @ 0x8225E480
extern void PostGameEvent(uint32_t eventId, uint32_t flags,
                          void* data, uint8_t param);

// Copy a vec3 from a source into a destination buffer.
// @ 0x821E6470
extern void vec3_Copy(float* dst, const float* src);

// Debug trace — no-op in release.
// @ 0x8240E6D0
extern void rage_debugLog(const char* msg);

// ─────────────────────────────────────────────────────────────────────────────
// Global data
// ─────────────────────────────────────────────────────────────────────────────

// Free-list pools for each message type (SDA-relative .data globals)
extern void* g_hitMessagePool;              // @ 0x825D1008 (.data, 12 bytes)
extern void* g_ballHitMessagePool;          // @ 0x825D1020 (.data, 12 bytes)

// Pool structure pointers (contain the actual pool arrays)
extern void* g_hitMessagePoolStruct;        // @ 0x825D1014 + 8 → pool base
extern void* g_ballHitMessagePoolStruct;    // @ 0x825D102C + 8 → pool base

// Render / game state objects
extern void* g_render_obj_ptr;              // @ 0x825EAB2C (.data)

// Zero float constant
// @ 0x8202D110 (.rdata)
extern const float kZeroFloat;

// Global float for ball hit time tracking
extern float g_ballHitTime;                 // @ 0x8260657C (.data)

// Network client state (used by SignalReadyToServe)
extern int32_t g_networkMode;               // @ 0x825D0000-area (SDA)

// Game state manager
extern void* g_gameStateMgr;               // @ 0x826063AC (.data, SDA offset 25500)

// Class name strings (embedded in .rdata)
// HitMessage:                  "HitMessage"
// BallHitMessage:              "BallHitMessage"
// SpectatorBallHitMessage:     "SpectatorBallHitMessage"
// SignalReadyToServeMessage:   "SignalReadyToServeMessage"


// ═══════════════════════════════════════════════════════════════════════════════
//  HitMessage — Base hit event message
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * HitMessage::SetData  [vtable slot 8]
 * @ 0x823B5FD0 | size: 0xC
 *
 * Populates the message with a timestamp and player index.
 * Called before sending the message across the network.
 *
 * r3 = this, f1 = timestamp, r4 = playerIndex
 */
void HitMessage_SetData(HitMessage* self, float timestamp, uint8_t playerIndex)
{
    self->m_timestamp   = timestamp;
    self->m_playerIndex = playerIndex;
}

/**
 * HitMessage::Deserialize  [vtable slot 1]
 * @ 0x823B5FE0 | size: 0x5C
 *
 * Reads the hit message fields from a network bitstream.
 *   1. Reads a 32-bit float (timestamp) from the stream
 *   2. Reads 8 bits of player data (player index + flags) into +8
 */
void HitMessage_Deserialize(HitMessage* self, void* stream)
{
    float timestamp;
    netStream_ReadFloat(stream, &timestamp, 32);
    self->m_timestamp = timestamp;

    netStream_ReadBytes(stream, &self->m_playerIndex, 8);
}

/**
 * HitMessage::Serialize  [vtable slot 2]
 * @ 0x823BA5F0 | size: 0x50
 *
 * Writes the hit message fields to a network bitstream.
 *   1. Writes the timestamp as a 32-bit float
 *   2. Writes the player index as 8 bits
 */
void HitMessage_Serialize(HitMessage* self, void* stream)
{
    netStream_WriteFloat(stream, self->m_timestamp);
    netStream_WriteUint(stream, self->m_playerIndex, 8);
}

/**
 * HitMessage::GetPool  [vtable slot 6]
 * @ 0x823B6108 | size: 0xC
 *
 * Returns the global free-list pool for HitMessage objects.
 */
void* HitMessage_GetPool()
{
    return g_hitMessagePool;
}

/**
 * HitMessage::GetClassName  [vtable slot 7]
 * @ 0x823B6118 | size: 0xC
 *
 * Returns the RTTI-style class name string "HitMessage".
 */
const char* HitMessage_GetClassName()
{
    // Returns string constant at 0x8206E80C area — "HitMessage"
    static const char* name = "HitMessage";
    return name;
}

/**
 * HitMessage::Allocate  [vtable slot 5]
 * @ 0x823B6040 | size: 0x54
 *
 * Returns this message object to the HitMessage free-list pool.
 * The pool is a fixed-size array of 16-byte entries with a linked
 * free list using uint16_t indices at entry+12 (next) and entry+14 (prev).
 * Pool header at +160 = active count, +164 = free head index.
 */
void HitMessage_Allocate(HitMessage* self)
{
    // Pool free-list management — returns object to pool
    // This uses index math: (self - poolBase) >> 4 to get the slot index
    // Then links into the free list chain.
    // Implementation is a standard RAGE pool pattern.

    uint8_t* poolBase = *(uint8_t**)((uint8_t*)g_hitMessagePoolStruct + 8);
    uint32_t offset = (uint32_t)((uint8_t*)self - poolBase);
    uint16_t slotIndex = (uint16_t)(offset >> 4);  // 16-byte entries

    // Link into free list
    uint16_t oldHead = *(uint16_t*)(poolBase + 164);
    *(uint16_t*)((uint8_t*)self + 14) = oldHead;  // this->next = oldHead

    if (oldHead != 0xFFFF) {
        uint8_t* oldEntry = poolBase + (oldHead << 4);
        *(uint16_t*)(oldEntry + 12) = slotIndex;  // oldHead->prev = this
    }

    *(uint16_t*)(poolBase + 164) = slotIndex;  // pool->freeHead = this

    uint16_t count = *(uint16_t*)(poolBase + 160);
    *(uint16_t*)(poolBase + 160) = count + 1;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  BallHitMessage — Extended hit message with frame timing
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * BallHitMessage::SetData  [vtable slot 8]
 * @ 0x823B6508 | size: 0x80
 *
 * Populates all fields from the current game state:
 *   - Stores timestamp and player index (base fields)
 *   - Resolves the player to get the rally hit count
 *   - Reads frame offset from the render state (+28 + +32)
 *   - Reads match time from the render state (+20)
 */
void BallHitMessage_SetData(BallHitMessage* self, float timestamp, uint8_t playerIndex)
{
    self->m_timestamp   = timestamp;
    self->m_playerIndex = playerIndex;

    // Resolve player and get rally hit count
    void* player = SinglesNetworkClient_GetPlayer(playerIndex);

    // Read frame offset from render state
    uint8_t* renderState = (uint8_t*)g_render_obj_ptr;
    uint32_t frameA = *(uint32_t*)(renderState + 28);
    uint32_t frameB = *(uint32_t*)(renderState + 32);
    self->m_frameOffset = (uint8_t)(frameA + frameB);

    // Get hit count from player's rally data
    // player+464 = rally index, used to look into stats table
    uint32_t rallyIdx = *(uint32_t*)((uint8_t*)player + 464);

    // Stats table lookup (base + rallyIdx * 416 + offset 240)
    // Implementation detail: reads from a per-player stats array
    // The exact table address comes from a lis/addi pattern in the scaffold
    extern uint8_t* g_playerStatsBase;  // @ 0x8261A350 area
    uint16_t hitCount = *(uint16_t*)(g_playerStatsBase + rallyIdx * 416 + 240);
    self->m_hitCount = hitCount;

    // Match time from render state
    uint16_t matchTime = *(uint16_t*)(renderState + 20);
    self->m_matchTime = (int16_t)matchTime;
}

/**
 * BallHitMessage::Deserialize  [vtable slot 1]
 * @ 0x823B65F8 | size: 0x68
 *
 * Reads all fields: first calls base HitMessage::Deserialize,
 * then reads the extended fields (hitCount, frameOffset, matchTime).
 */
void BallHitMessage_Deserialize(BallHitMessage* self, void* stream)
{
    // Deserialize base fields
    HitMessage_Deserialize(self, stream);

    // Read extended fields
    netStream_ReadU16(stream, &self->m_hitCount, 16);
    netStream_ReadBytes(stream, &self->m_frameOffset, 8);
    netStream_ReadS16(stream, &self->m_matchTime, 16);
}

/**
 * BallHitMessage::Serialize  [vtable slot 2]
 * @ 0x823B6660 | size: 0x84
 *
 * Writes all fields to the network bitstream.
 */
void BallHitMessage_Serialize(BallHitMessage* self, void* stream)
{
    // Write base fields
    netStream_WriteFloat(stream, self->m_timestamp);
    netStream_WriteUint(stream, self->m_playerIndex, 8);

    // Write extended fields
    netStream_WriteUint(stream, self->m_hitCount, 16);
    netStream_WriteUint(stream, self->m_frameOffset, 8);
    netStream_WriteSint(stream, (int32_t)self->m_matchTime, 16);
}

/**
 * BallHitMessage::Use  [vtable slot 3]
 * @ 0x823B6588 | size: 0x70
 *
 * Dispatches the ball hit event into the game:
 *   1. Resolves the player from the stored index
 *   2. Reads the player's rally state (+464) to check if spectating
 *   3. Zeroes the global ball-hit-time tracker
 *   4. Logs a debug message (no-op in release)
 *   5. Posts a game message: ID=192 (0xC0), state=3 or 4 depending on rally
 */
void BallHitMessage_Use(BallHitMessage* self)
{
    void* player = SinglesNetworkClient_GetPlayer(self->m_playerIndex);
    uint32_t rallyState = *(uint32_t*)((uint8_t*)player + 464);

    // Clear ball hit time tracker
    g_ballHitTime = kZeroFloat;

    // Debug log (no-op in release)
    rage_debugLog("BallHitMessage::Use");

    // Determine message state: 4 if rally active, 3 if idle
    uint32_t msgState = (rallyState != 0) ? 4 : 3;

    // Post game message
    PostGameMessage(msgState, 192, 0, 0);
}

/**
 * BallHitMessage::GetPool  [vtable slot 6]
 * @ 0x823B64E8 | size: 0xC
 */
void* BallHitMessage_GetPool()
{
    return g_ballHitMessagePool;
}

/**
 * BallHitMessage::GetClassName  [vtable slot 7]
 * @ 0x823B64F8 | size: 0xC
 */
const char* BallHitMessage_GetClassName()
{
    static const char* name = "BallHitMessage";
    return name;
}

/**
 * BallHitMessage::Allocate  [vtable slot 5]
 * @ 0x823B63E0 | size: 0x74
 *
 * Returns this message to the BallHitMessage pool free list.
 * Uses 24-byte entries (entry size = index * 3 * 8).
 */
void BallHitMessage_Allocate(BallHitMessage* self)
{
    uint8_t* poolBase = *(uint8_t**)((uint8_t*)g_ballHitMessagePoolStruct + 8);
    uint32_t offset = (uint32_t)((uint8_t*)self - poolBase);

    // Division by 24 using multiply-by-reciprocal:
    // 0xAAAAAAAB * offset >> 32 then >> 4 = offset / 24 (truncated)
    uint16_t slotIndex = (uint16_t)((uint64_t)offset * 0xAAAAAAABULL >> 32 >> 4);

    // Each entry is 24 bytes: index * 3 * 8
    uint32_t entryOffset = slotIndex * 3 * 8;

    // Link into free list
    uint16_t oldHead = *(uint16_t*)(poolBase + 244);
    *(uint16_t*)(poolBase + entryOffset + 22) = oldHead;

    if (oldHead != 0xFFFF) {
        uint32_t oldOffset = oldHead * 3 * 8;
        *(uint16_t*)(poolBase + oldOffset + 20) = slotIndex;
    }

    *(uint16_t*)(poolBase + 244) = slotIndex;

    uint16_t count = *(uint16_t*)(poolBase + 240);
    *(uint16_t*)(poolBase + 240) = count + 1;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  SpectatorBallHitMessage — Spectator view with ball position
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * SpectatorBallHitMessage::SetData  [vtable slot 8]
 * @ 0x823B68F0 | size: 0x60
 *
 * Populates base fields, then copies the ball's current position
 * from the player's creature/physics system into m_position.
 */
void SpectatorBallHitMessage_SetData(SpectatorBallHitMessage* self,
                                      float timestamp, uint8_t playerIndex)
{
    self->m_timestamp   = timestamp;
    self->m_playerIndex = playerIndex;

    // Resolve player and copy ball position from creature state
    void* player = SinglesNetworkClient_GetPlayer(playerIndex);
    uint32_t rallyIdx = *(uint32_t*)((uint8_t*)player + 464);

    // Look up ball position in the per-player stats table
    extern uint8_t* g_playerStatsBase;
    float* ballPos = (float*)(g_playerStatsBase + rallyIdx * 416 + 64);

    // Copy vec3 position
    vec3_Copy(self->m_position, ballPos);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  SignalReadyToServeMessage — Serve ready notification
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * SignalReadyToServeMessage::SetData  [vtable slot 8]
 * @ 0x823B86E8 | size: 0x38
 *
 * Populates the message from the game timer state.
 * The timestamp is computed as: gameTimer.elapsed + (float)gameTimer.frameCount
 * This gives a precise sub-frame timestamp for serve synchronization.
 */
void SignalReadyToServeMessage_SetData(SignalReadyToServeMessage* self,
                                        uint8_t playerIndex)
{
    // Read from game timer global
    extern void* g_gameTimerPtr;  // @ 0x82619A28 area (SDA)
    void* timer = g_gameTimerPtr;
    void* timerData = *(void**)((uint8_t*)timer + 12);

    int32_t frameCount = *(int32_t*)((uint8_t*)timerData + 4);
    float elapsed = *(float*)((uint8_t*)timerData + 8);

    self->m_playerIndex = playerIndex;
    self->m_timestamp = elapsed + (float)frameCount;
}

/**
 * SignalReadyToServeMessage::Use  [vtable slot 4]
 * @ 0x823B8808 | size: 0xD0
 *
 * Dispatches the serve-ready signal.  Behavior depends on game state:
 *
 * - If g_networkMode == 0 (offline / host):
 *     Switch on current HSM state ID (from g_gameStateMgr->+12):
 *       State 26: Post message 10244 (0x2804) with flags=64,param=1 and data=-1
 *       States 23/24/25: Post event 14371 (0x3823) with playerIndex
 *       States <= 8: Return immediately (no action)
 *       Other states: Fall through to enter serve mode
 *
 * - If g_networkMode == 2 (spectator):
 *     Return immediately (no action)
 *
 * - Otherwise (client):
 *     Resolve the player and call pongPlayer_EnterServeMode
 */
void SignalReadyToServeMessage_Use(SignalReadyToServeMessage* self)
{
    if (g_networkMode == 0) {
        // Offline/host path — check game state
        void* stateMgr = g_gameStateMgr;
        int32_t stateId = *(int32_t*)((uint8_t*)stateMgr + 12);

        if (stateId == 26) {
            // Waiting-for-serve state → post serve-ready message
            int32_t negOne = -1;
            PostGameMessage(10244, 64, 1, (uint32_t)negOne);
            goto enterServe;
        }

        if (stateId == 25 || stateId == 23 || stateId == 24) {
            // Play states → post serve event with player index
            extern const uint8_t kServeEventData[];  // @ 0x82027A18 area
            PostGameEvent(14371, 64, (void*)kServeEventData, self->m_playerIndex);
            return;
        }

        if (stateId <= 8) {
            // Early/boot states — no action
            return;
        }

        // Fall through to enter serve mode
    } else if (g_networkMode == 2) {
        // Spectator — no action
        return;
    }

enterServe:
    // Resolve player and enter serve mode
    void* player = SinglesNetworkClient_GetPlayer(self->m_playerIndex);
    pongPlayer_EnterServeMode(player);
}
