// pong_network_io.cpp — Network I/O helper functions | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * Network serialization helpers for SinglesNetworkClient
 * 
 * These functions handle bit-packed network stream I/O for ball physics and hit data.
 * The network protocol uses variable-bit-width encoding to minimize bandwidth during
 * multiplayer matches. Ball hit data includes position, velocity, spin, and collision
 * information that must be synchronized across all clients.
 */

#include "../include/game/pong_network_io.hpp"
#include <stdint.h>

// Forward declaration for vector write helper
void WriteVector3ToNetwork(void* client, void* vec3Data);


/**
 * WriteFloatToNetworkStream @ 0x820D6990 | size: 0x68
 * 
 * Original symbol: game_6990
 * Subsystem: Network / Serialization
 * 
 * Writes a 32-bit float to the network bit stream with proper alignment.
 * Updates the client's bit position tracker and performs byte-alignment operations.
 * 
 * @param client  SinglesNetworkClient object managing the network stream
 * @param value   Float value to write to the stream
 */
void WriteFloatToNetworkStream(void* client, float value)
{
    // Prepare for write operation
    BeginNetworkWrite(client);
    
    // Write the float as a 32-bit value
    uint32_t floatBits = *(uint32_t*)&value;
    uint32_t result = ReadBitsFromStream(client, floatBits, 32);
    
    // Load current bit position from client state
    uint32_t currentBitPos = *(uint32_t*)((uint8_t*)client + 16);
    
    // Prepare for alignment write
    BeginNetworkWrite(client);
    
    // Calculate byte-aligned position: (bitPos + 7) / 8
    int32_t byteAlignedPos = (currentBitPos + 7) >> 3;
    
    // Save and temporarily update bit position
    uint32_t savedBitPos = *(uint32_t*)((uint8_t*)client + 32);
    *(uint32_t*)((uint8_t*)client + 32) = 32;
    
    // Write alignment marker (16-bit)
    ReadBitsFromStream(client, byteAlignedPos, 16);
    
    // Restore original bit position
    *(uint32_t*)((uint8_t*)client + 32) = savedBitPos;
}


/**
 * WriteBallHitDataToNetwork @ 0x821D5738 | size: 0x1fc
 * 
 * Original symbol: game_5738
 * Subsystem: Network / Ball Physics Serialization
 * 
 * Serializes ball hit/physics data to the network stream for multiplayer synchronization.
 * Used by HitDataMessage, BallHitMessage, and SpectatorBallHitMessage to transmit
 * ball state during rallies.
 * 
 * The pongBallHitData structure (~192 bytes) contains:
 * - Ball position and velocity vectors
 * - Spin and trajectory data
 * - Collision/bounce information
 * - Hit zone and swing type flags
 * - Conditional extended physics data (based on bit 12 of flags@182)
 * 
 * Structure layout:
 *   +0x00..+0x1F  : Primary vector group (position/initial velocity)
 *   +0x20 (32)    : Control byte (hit type/zone)
 *   +0x30..+0x4F  : Secondary vector group (spin/trajectory)
 *   +0x50..+0x74  : Physics scalars (bounce coefficients, timing)
 *   +0x78 (120)   : Conditional float (extended physics)
 *   +0x7C..+0x84  : Additional scalars (124,128,132)
 *   +0x88..+0x8C  : State bytes (136,140)
 *   +0xB0..+0xB6  : Flags and indices (176-182)
 *   +0x90 (144)   : Conditional vec3 (impact normal)
 *   +0xA0 (160)   : Conditional vec3 (surface velocity)
 * 
 * @param ballHitData  Pointer to pongBallHitData structure to serialize
 * @param client       SinglesNetworkClient object managing the network stream
 */
void WriteBallHitDataToNetwork(void* ballHitData, void* client)
{
    uint8_t* hitData = (uint8_t*)ballHitData;
    
    // Load hit type/zone control byte from offset 32
    int8_t hitTypeZone = *(int8_t*)(hitData + 32);
    
    // ── Primary vector group: ball position and initial velocity ──
    // Offsets 0, 4, 8, 16, 20, 24 represent two vec3 structures
    WriteFloatToNetworkStream(client, *(float*)(hitData + 0));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 4));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 8));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 16));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 20));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 24));
    
    // ── Secondary vector group: spin and trajectory data ──
    // Base offset 48, same pattern as primary group
    uint8_t* spinTrajectory = hitData + 48;
    WriteFloatToNetworkStream(client, *(float*)(spinTrajectory + 0));
    WriteFloatToNetworkStream(client, *(float*)(spinTrajectory + 4));
    WriteFloatToNetworkStream(client, *(float*)(spinTrajectory + 8));
    WriteFloatToNetworkStream(client, *(float*)(spinTrajectory + 16));
    WriteFloatToNetworkStream(client, *(float*)(spinTrajectory + 20));
    WriteFloatToNetworkStream(client, *(float*)(spinTrajectory + 24));
    
    // ── Physics scalars: bounce coefficients, timing, power ──
    WriteFloatToNetworkStream(client, *(float*)(hitData + 80));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 84));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 88));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 96));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 100));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 104));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 116));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 112));
    
    // ── Hit type/zone control byte (signed) ──
    WriteInt8Bits(client, hitTypeZone, 8);
    
    // ── Additional physics scalars ──
    WriteFloatToNetworkStream(client, *(float*)(hitData + 124));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 128));
    WriteFloatToNetworkStream(client, *(float*)(hitData + 132));
    
    // ── State bytes: player indices, hit flags ──
    uint8_t stateByte136 = *(uint8_t*)(hitData + 136);
    uint8_t stateByte140 = *(uint8_t*)(hitData + 140);
    WriteUInt8Bits(client, stateByte136, 8);
    WriteUInt8Bits(client, stateByte140, 8);
    
    // ── Flags and indices (16-bit values) ──
    uint16_t flags176 = *(uint16_t*)(hitData + 176);
    uint16_t index178 = *(uint16_t*)(hitData + 178);
    WriteUInt8Bits(client, flags176, 16);
    WriteInt8Bits(client, (int16_t)index178, 16);
    
    // ── Additional state byte ──
    uint8_t stateByte180 = *(uint8_t*)(hitData + 180);
    WriteUInt8Bits(client, stateByte180, 8);
    
    // ── Extended physics flags ──
    uint16_t extendedFlags = *(uint16_t*)(hitData + 182);
    WriteUInt8Bits(client, extendedFlags, 16);
    
    // Check bit 12 (0x1000) for extended collision/surface data
    bool hasExtendedPhysics = (extendedFlags & 0x1000) != 0;
    
    if (hasExtendedPhysics) {
        // Write extended physics scalar
        WriteFloatToNetworkStream(client, *(float*)(hitData + 120));
        
        // Write collision normal and surface velocity vectors
        WriteVector3ToNetwork(client, hitData + 144);  // Impact normal
        WriteVector3ToNetwork(client, hitData + 160);  // Surface velocity
    }
}


/**
 * WriteVector3ToNetwork @ 0x821D9BB8 | size: 0x54
 * 
 * Original symbol: game_9BB8_h
 * Subsystem: Network / Vector Serialization
 * 
 * Writes a 3-component vector (vec3) to the network stream.
 * Used for position, velocity, normal, and other 3D vector data.
 * 
 * @param client    SinglesNetworkClient object managing the network stream
 * @param vec3Data  Pointer to 3 consecutive floats (x, y, z)
 */
void WriteVector3ToNetwork(void* client, void* vec3Data) {
    float* vec = (float*)vec3Data;
    
    WriteFloatToNetworkStream(client, vec[0]);  // x component
    WriteFloatToNetworkStream(client, vec[1]);  // y component
    WriteFloatToNetworkStream(client, vec[2]);  // z component
}


/**
 * NETWORK I/O SYSTEM SUMMARY
 * 
 * This file implements the WRITE (serialize) side of the network protocol.
 * The READ (deserialize) counterparts are yet to be implemented.
 * 
 * FUNCTION CALL HIERARCHY:
 * 
 * Message Classes (vfn_2 - Serialize)
 *   ├─> HitDataMessage_vfn_2 @ 0x823B5BF0
 *   ├─> SpectatorBallHitMessage_vfn_2 @ 0x823B6B40
 *   ├─> ServeStartedMessage_vfn_2 @ 0x823B6D40
 *   └─> BallHitMessage_vfn_2 @ 0x823B5CA0
 *        │
 *        ├─> WriteBallHitDataToNetwork @ 0x821D5738 (this file)
 *        │    ├─> WriteFloatToNetworkStream @ 0x820D6990 (this file) [×13 calls]
 *        │    ├─> WriteVector3ToNetwork @ 0x821D9BB8 (this file) [×2 calls]
 *        │    ├─> WriteInt8Bits (signed byte write)
 *        │    └─> WriteUInt8Bits (unsigned byte write)
 *        │
 *        └─> WriteFloatToNetworkStream @ 0x820D6990 (this file)
 *             ├─> BeginNetworkWrite (prepare write)
 *             └─> ReadBitsFromStream (core bit write primitive)
 * 
 * RELATED FUNCTIONS (to be implemented):
 * 
 * Read Primitives:
 *   - SinglesNetworkClient_0CE8_g @ 0x82260CE8 (300 bytes) - Core bit read
 *   - SinglesNetworkClient_0128_g @ 0x82260128 (96 bytes) - Read float candidate
 *   - SinglesNetworkClient_0188_g @ 0x82260188 (220 bytes) - Read complex data
 * 
 * Message Handlers (vfn_3 - Deserialize):
 *   - HitDataMessage_vfn_3 @ 0x823B5C40 (96 bytes)
 *   - ServeUnlockMessage_vfn_3 @ 0x823B6C90 (60 bytes)
 *   - RemoteServeReadyMessage_vfn_3 @ 0x823B6E98 (60 bytes)
 * 
 * NETWORK PROTOCOL CHARACTERISTICS:
 * 
 * 1. Bit-Packed Encoding: Variable bit-width fields for efficiency
 * 2. Byte Alignment: Floats are byte-aligned after write
 * 3. Conditional Data: Flags control optional data transmission
 * 4. Event-Driven: Messages sent on significant events, not every frame
 * 5. Type Safety: Separate functions for signed/unsigned, different widths
 * 
 * BALL HIT DATA STRUCTURE (~192 bytes):
 * 
 * - Primary vectors (32 bytes): Position + initial velocity
 * - Control byte (1 byte): Hit type/zone classification
 * - Secondary vectors (32 bytes): Spin + trajectory
 * - Physics scalars (52 bytes): Bounce coefficients, timing, power
 * - State bytes (8 bytes): Player indices, flags
 * - Flags/indices (7 bytes): General flags, extended physics control
 * - Conditional extended data (24 bytes): Impact normal + surface velocity
 *   (only transmitted when bit 12 of flags@182 is set)
 * 
 * BANDWIDTH OPTIMIZATION:
 * 
 * The conditional extended physics data (24 bytes) is only transmitted when
 * needed, reducing average message size by ~12.5% for common ball hits.
 * Combined with bit-packing, this achieves significant bandwidth savings
 * compared to naive float array transmission.
 * 
 * FUTURE WORK:
 * 
 * 1. Implement READ counterparts (ReadFloatFromNetworkStream, etc.)
 * 2. Add message class implementations (vfn_3 handlers)
 * 3. Implement validation and error handling
 * 4. Add unit tests for round-trip serialization
 * 5. Document complete message type catalog (46+ types)
 * 6. Analyze prediction/interpolation algorithms
 * 7. Profile bandwidth usage and optimize further
 */


// ══════════════════════════════════════════════════════════════════════════════
// READ PRIMITIVES (deserialisation side)
// ══════════════════════════════════════════════════════════════════════════════

// ── External low-level bit-stream read helpers (lifted elsewhere) ───────────
//
// These four primitives share the same low-level prologue as their write
// counterparts — they advance the bit cursor on the client and write the
// decoded value into a caller-supplied destination buffer.

// Core read primitive @ 0x82238DF8 — reads `bitCount` bits into outBuf.
// Original symbol: SinglesNetworkClient_8DF8_g (size 0x130).
// Used with bitCount=32 to deserialise a float.
extern void netStream_ReadFloat(void* client, void* outBuf, uint32_t bitCount);

// Read unsigned bytes @ 0x820D7A08 (util_7A08).
// Used to read 8-bit unsigned values (state bytes, flags).
extern void netStream_ReadBytes(void* client, void* outBuf, uint32_t bitCount);

// Read unsigned 16-bit value @ 0x820D7830 (snBitStream_ReadUnsigned).
extern void snBitStream_ReadUnsigned(void* client, void* outBuf, int bitCount);

// Read signed 16-bit value @ 0x820D7970 (util_7970).
extern void netStream_ReadS16(void* client, void* outBuf, uint32_t bitCount);

// Sign-magnitude signed 8-bit reader @ 0x82101668 (util_1668).
// Reads a 1-bit sign flag followed by a 7-bit magnitude byte, then negates
// the stored byte when the sign bit is set. Used for the hit-type control
// byte stored at ballHitData+32.
extern uint8_t netStream_ReadS8SignMagnitude(void* client, void* outByte);


/**
 * ReadFloatFromNetworkStream @ 0x82238DF8 | size: 0x130 (wrapper)
 *
 * Exact mirror of WriteFloatToNetworkStream @ 0x820D6990. Reads 32 bits from
 * the bit stream into a local temporary and returns the result as a float.
 *
 * Original symbol: SinglesNetworkClient_8DF8_g (called with bitCount=32).
 *
 * @param client  SinglesNetworkClient object managing the network stream
 * @return        Deserialised float
 */
float ReadFloatFromNetworkStream(void* client)
{
    float value = 0.0f;
    netStream_ReadFloat(client, &value, 32);
    return value;
}


/**
 * ReadVector3FromNetwork @ 0x821D9B40 | size: 0x78
 *
 * Reads 3 consecutive floats (x, y, z) from the network stream into the
 * caller-supplied destination. Mirror of WriteVector3ToNetwork @ 0x821D9BB8.
 *
 * Original symbol: SinglesNetworkClient_9B40_fw.
 *
 * @param client   SinglesNetworkClient object managing the network stream
 * @param outVec3  Destination — 3 floats written at offsets 0, 4, 8
 */
void ReadVector3FromNetwork(void* client, void* outVec3)
{
    float* vec = static_cast<float*>(outVec3);

    netStream_ReadFloat(client, &vec[0], 32);  // x component
    netStream_ReadFloat(client, &vec[1], 32);  // y component
    netStream_ReadFloat(client, &vec[2], 32);  // z component
}


/**
 * ReadBallHitDataFromNetwork @ 0x821D5538 | size: 0x200
 *
 * Inverse of WriteBallHitDataToNetwork @ 0x821D5738. Deserialises the full
 * pongBallHitData (~192 bytes) from the network stream in the exact same
 * field order as the write side.
 *
 * Original symbol: util_5538. Called by the vfn_3 (deserialise) slot of
 * HitDataMessage, BallHitMessage and SpectatorBallHitMessage.
 *
 * Sequence (mirrors WriteBallHitDataToNetwork):
 *   - 3 scalar floats (+0, +4, +8)
 *   - 5 vec3 groups via ReadVector3FromNetwork (+16, +48, +64, +80, +96)
 *   - 2 scalar floats (+116, +112)
 *   - sign-magnitude i8 hit-type → stack, later stored as s32 at +32
 *   - 3 scalar floats (+124, +128, +132)
 *   - 2 u8 state bytes → stack, later stored as u32 at +136, +140
 *   - u16 flags at +176, s16 index at +178, u8 state at +180
 *   - u16 extended-flags at +182; conditional block when (extFlags & 0x1000)
 *     reads an extra float at +120 and two extra vec3s at +144 and +160.
 *
 * @param ballHitData  Destination pongBallHitData buffer
 * @param client       SinglesNetworkClient object managing the network stream
 */
void ReadBallHitDataFromNetwork(void* ballHitData, void* client)
{
    uint8_t* hitData = static_cast<uint8_t*>(ballHitData);

    // ── Primary vector group (position + initial velocity) ──
    // First 3 floats live at +0, +4, +8 as individual scalars.
    netStream_ReadFloat(client, hitData + 0,  32);
    netStream_ReadFloat(client, hitData + 4,  32);
    netStream_ReadFloat(client, hitData + 8,  32);

    // Next 3 floats read as a vec3 at +16 (velocity sub-vector).
    ReadVector3FromNetwork(client, hitData + 16);

    // ── Secondary vector group (spin + trajectory, base +48) ──
    ReadVector3FromNetwork(client, hitData + 48);  // spin vec3 at +48..+56
    ReadVector3FromNetwork(client, hitData + 64);  // trajectory vec3 at +64..+72

    // ── Physics vectors (bounce/timing/power scalars packed as vec3s) ──
    ReadVector3FromNetwork(client, hitData + 80);  // +80, +84, +88
    ReadVector3FromNetwork(client, hitData + 96);  // +96, +100, +104

    // ── Scalar physics floats at +116 and +112 (order matches write) ──
    netStream_ReadFloat(client, hitData + 116, 32);
    netStream_ReadFloat(client, hitData + 112, 32);

    // ── Hit-type / zone control byte: 1-bit sign + 7-bit magnitude ──
    // Temporarily decoded into a local, then sign-extended into the s32
    // slot at +32 to mirror the final `stw r11,32(r30)` in the recomp.
    uint8_t hitTypeMag = 0;
    netStream_ReadS8SignMagnitude(client, &hitTypeMag);
    int32_t hitTypeZone = static_cast<int32_t>(static_cast<int8_t>(hitTypeMag));

    // ── Additional physics scalars ──
    netStream_ReadFloat(client, hitData + 124, 32);
    netStream_ReadFloat(client, hitData + 128, 32);
    netStream_ReadFloat(client, hitData + 132, 32);

    // ── State bytes: 8-bit reads zero-extended into u32 fields at +136/+140 ──
    uint8_t stateByte136 = 0;
    uint8_t stateByte140 = 0;
    netStream_ReadBytes(client, &stateByte136, 8);
    netStream_ReadBytes(client, &stateByte140, 8);

    // ── Flags and indices ──
    snBitStream_ReadUnsigned(client, hitData + 176, 16);  // u16 flags
    netStream_ReadS16       (client, hitData + 178, 16);  // s16 index
    netStream_ReadBytes     (client, hitData + 180,  8);  // u8 state

    // ── Extended physics flags ──
    snBitStream_ReadUnsigned(client, hitData + 182, 16);

    // Bit 12 (0x1000) gates the conditional collision/surface block.
    uint16_t extendedFlags = *reinterpret_cast<uint16_t*>(hitData + 182);
    bool hasExtendedPhysics = (extendedFlags & 0x1000) != 0;

    if (hasExtendedPhysics) {
        netStream_ReadFloat(client, hitData + 120, 32);   // extended scalar
        ReadVector3FromNetwork(client, hitData + 144);    // impact normal
        ReadVector3FromNetwork(client, hitData + 160);    // surface velocity
    }

    // ── Final stores mirroring the recomp tail ──
    // u8 temporaries widen to u32 in the ball-hit struct's state fields.
    *reinterpret_cast<uint32_t*>(hitData + 136) = stateByte136;
    *reinterpret_cast<uint32_t*>(hitData + 140) = stateByte140;
    *reinterpret_cast<int32_t *>(hitData +  32) = hitTypeZone;
}


// ══════════════════════════════════════════════════════════════════════════════
// C LINKAGE ALIASES FOR BACKWARD COMPATIBILITY
// ══════════════════════════════════════════════════════════════════════════════

/**
 * These extern "C" aliases maintain backward compatibility with existing code
 * that references the original hex-based function names. All new code should
 * use the clean names defined in pong_network_io.hpp.
 */

extern "C" {
    // Original name: snBitStream_WriteBits
    uint32_t snBitStream_WriteBits(void* client, uint32_t value, int bitWidth) {
        return ReadBitsFromStream(client, value, bitWidth);
    }
    
    // Original name: snBitStream_ValidateWrite
    void snBitStream_ValidateWrite(void* client) {
        BeginNetworkWrite(client);
    }
    
    // Original name: snBitStream_ReadBits
    void snBitStream_ReadBits(void* client) {
        EndNetworkWrite(client);
    }
    
    // SinglesNetworkClient_6838_g — lifted in src/game/network.cpp
    // SinglesNetworkClient_68A8_g — lifted in src/game/network.cpp
    
    // Original name: SinglesNetworkClient_2BE8_g
    void SinglesNetworkClient_2BE8_g(void* client) {
        UpdateNetworkState(client);
    }
    
    // Original name: SinglesNetworkClient_51C8_g
    void SinglesNetworkClient_51C8_g(void* client) {
        ProcessNetworkPacket(client);
    }
    
    // Original name: rage_atStringCopy
    void rage_atStringCopy(const char* src, char* dest, int maxSize) {
        CopyNetworkString(src, dest, maxSize);
    }
    
    // SinglesNetworkClient_A818_g — lifted in src/game/network.cpp
    
    // Original name: snSession_AcquireLock
    uint8_t snSession_AcquireLock(void* pageGroup) {
        return PollButtonState(pageGroup);
    }
    
    // Original name: snSession_BeginOperation
    void* snSession_BeginOperation(void* context) {
        return GetNetworkClient(context);
    }
    
    // Original name: snSession_ReleaseLock
    void snSession_ReleaseLock(void* context) {
        InitiateConnection(context);
    }
    
    // Original name: SinglesNetworkClient_GetPlayerID_E408
    void SinglesNetworkClient_GetPlayerID_E408(void* client, uint32_t param) {
        GetPlayerID(client, param);
    }
}
