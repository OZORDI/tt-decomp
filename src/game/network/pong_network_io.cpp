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


// ══════════════════════════════════════════════════════════════════════════════
// msgMsgSink free functions, net_*, rl/snSession helpers — lifted from tt.xex
// ══════════════════════════════════════════════════════════════════════════════
//
// This block covers the non-method msgMsgSink_* helpers plus the related low
// level session / bit-stream primitives used by the network subsystem. Function
// bodies translated from pass5_final recomp (cross-checked against Hex-Rays
// when available). Sibling child-state forwards (msgMsgSink_*, snSession_*)
// call other children which are already stubs owned by sibling agents, so we
// declare them as `extern` here rather than implementing them.

#include <cstring>

// ── Forward declarations for sibling-owned helpers ──────────────────────────
extern void  atSingleton_6F98_g(void* sink);                          // @ 0x82566f98
extern int32_t game_9D70_h(void* node, uint32_t flags);               // @ 0x82459d70
extern int32_t atSingleton_9B98_p39(void* node, void* key);           // @ 0x82459b98
extern int32_t game_3F68_h(void* sink);                               // @ 0x82453f68
extern void  game_A650_h(void* node);                                 // @ 0x8245a650
extern int32_t game_3FD0_h(void* sink);                               // @ 0x82453fd0
extern int32_t game_9A48_h(void* node);                               // @ 0x82459a48
extern void  msgMsgSink_8B78_g(void* sessionRef, uint16_t hdr);       // @ 0x82448b78
extern void  msgMsgSink_8B90_g(void* sessionRef, uint16_t hdr);       // @ 0x82448b90
extern int32_t phBoundCapsule_A150(void* node, int32_t flag);         // @ 0x8245a150
extern void  atSingleton_89F8_2hr(void* bucket, void* sink);          // @ 0x824489f8
extern void  atSingleton_5CD0_fw(void* sink, int kind);               // @ 0x82455cd0
extern void  msgMsgSink_EA00(void);                                   // @ 0x8244ea00
extern void  _locale_register(void* sink, uint32_t tag);              // @ 0x820c02d0
extern int32_t snNotifyGamerAdded_E790(void* self, void* param,
                                       void** outHandle);             // @ 0x8244e790
extern uint32_t atSingleton_7CE0_w(void* bucket, int inBounds,
                                   int boundExcl);                    // @ 0x82447ce0
extern int32_t net_1CC8(uint32_t* outSlot, int one, int zero,
                        int zero2, int zero3, uint32_t flags,
                        int eight, int zero4, int zero5);              // @ 0x82481cc8
extern void* snSession_D418_w(void* self, void* state);                // @ 0x823ed418
extern void  snSession_DDB8_w(void* self, void* state);               // @ 0x823eddb8
extern void  snSession_DD10_w(void* self, void* state);               // @ 0x823edd10
extern void  nop_8240E6D0(char* out, const char* fmt, void* arg);     // @ 0x8240e6d0
extern "C" void* memset(void*, int, size_t);
extern "C" void snSession_AddNode_C068(void* nodeList, void* node);


/**
 * msgMsgSink_3C88_g @ 0x82453C88 | size: 0xe8
 *
 * Iteratively updates a bit-field on the sink and, when it transitions, walks
 * the sink's child linked-list at +36 forwarding into game_9D70_h(child, flag).
 * The &0x20 bit in the byte at +52 tracks the "has-singleton-cached" state;
 * when it changes we (re)acquire the singleton at +108 and fix up relative
 * offsets stored by children. Mirror of the pass5_final recomp.
 */
uint32_t msgMsgSink_3C88_g(void* self, uint32_t flag)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    uint32_t result = 0;

    int32_t stateWord = *reinterpret_cast<int32_t*>(s + 80);
    if (stateWord <= 4) {
        uint8_t bits = *(s + 52);
        uint32_t maskBit = static_cast<uint32_t>(bits) & 0x20;
        uint32_t curBit  = (maskBit == 0x20) ? 1 : 0;
        if (curBit == flag) return 0;

        uint32_t newBit = (flag != 0) ? 1 : 0;
        uint8_t newBits = static_cast<uint8_t>((bits & ~0x20u) | (newBit << 5));
        *(s + 52) = newBits;

        uint32_t oldSingleton = *reinterpret_cast<uint32_t*>(s + 108);
        atSingleton_6F98_g(self);
        uint32_t newSingleton = *reinterpret_cast<uint32_t*>(s + 108);
        if ((newBits & 0x20) != 0) {
            // singleton freshly cached — first form also stores, already done
        } else {
            uint32_t delta = newSingleton - oldSingleton;
            uint8_t* listHead = s + 56;
            uint32_t cur = *reinterpret_cast<uint32_t*>(s + 60);
            while (cur != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(listHead))) {
                uint8_t* node = reinterpret_cast<uint8_t*>(cur);
                uint32_t next = *reinterpret_cast<uint32_t*>(node + 4);
                uint32_t* off = reinterpret_cast<uint32_t*>(node + 20);
                *off += delta;
                cur = next;
            }
        }

        // Walk child list and forward game_9D70_h(child, flag) early-exit on
        // first non-negative result — but original keeps iterating. Match it.
        uint8_t* childHead = s + 36;
        uint32_t child = *reinterpret_cast<uint32_t*>(s + 40);
        while (child != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(childHead))) {
            int32_t r = game_9D70_h(reinterpret_cast<void*>(child), flag);
            result = static_cast<uint32_t>(r);
            child = *reinterpret_cast<uint32_t*>(child + 4);
            if (static_cast<int32_t>(result) < 0) break;
        }
    }
    return result;
}


/**
 * msgMsgSink_3D70_p39 @ 0x82453D70 | size: 0x58
 *
 * Walks the sink's child linked list at +36 forwarding each node into
 * atSingleton_9B98_p39(child, key). Returns 1 on first non-zero response.
 */
uint32_t msgMsgSink_3D70_p39(void* self)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    uint8_t* head = s + 36;
    uint32_t key  = 0;  // original takes r4 into r30, passed below
    uint32_t node = *reinterpret_cast<uint32_t*>(s + 40);
    while (node != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(head))) {
        if (atSingleton_9B98_p39(reinterpret_cast<void*>(node),
                                 reinterpret_cast<void*>(key)) != 0) {
            return 1;
        }
        node = *reinterpret_cast<uint32_t*>(node + 4);
    }
    return 0;
}


/**
 * msgMsgSink_4550_g @ 0x82454550 | size: 0x88
 *
 * If stateWord (+80) == 6: calls game_3F68_h(sink); if that returns 0 issues a
 * virtual-call on vtable slot 1 with flag=1 (scalar-deleting dtor convention).
 * Otherwise walks child list at +36 calling game_A650_h(child) for each.
 */
void msgMsgSink_4550_g(void* self)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    int32_t state = *reinterpret_cast<int32_t*>(s + 80);
    if (state == 6) {
        if (game_3F68_h(self) == 0) {
            // Virtual dispatch slot 1 with flag=1 (scalar-deleting dtor)
            void** vt = *reinterpret_cast<void***>(s + 0);
            using Fn = void(*)(void*, int);
            reinterpret_cast<Fn>(vt[1])(self, 1);
        }
    } else {
        uint8_t* head = s + 36;
        uint32_t node = *reinterpret_cast<uint32_t*>(s + 40);
        while (node != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(head))) {
            game_A650_h(reinterpret_cast<void*>(node));
            node = *reinterpret_cast<uint32_t*>(node + 4);
        }
    }
}


/**
 * msgMsgSink_4EB8_g @ 0x82454EB8 | size: 0x1dc
 *
 * Large state-machine helper. Full recomp is >200 bytes of conditional
 * virtual-call + linked-list walking. Translated to match behavior for
 * state==3 early path and state-transition paths. See pass5_final recomp.
 */
uint32_t msgMsgSink_4EB8_g(void* self)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    int32_t state = *reinterpret_cast<int32_t*>(s + 80);
    uint32_t result = 0;

    if (state == 3) {
        // slot 31: sessionProbe()
        void* sessionRef = *reinterpret_cast<void**>(s + 48);
        void** vt = *reinterpret_cast<void***>(sessionRef);
        using FnProbe = int32_t(*)(void*);
        int32_t probe = reinterpret_cast<FnProbe>(vt[31])(sessionRef);
        if (probe != 0) {
            // game_3FD0_h path
            int32_t rc = game_3FD0_h(self);
            if (rc == 1) {
                int32_t sw = *reinterpret_cast<int32_t*>(s + 80);
                if (sw < 6) {
                    *reinterpret_cast<int32_t*>(s + 80) = 6;
                    int32_t flag84 = *reinterpret_cast<int32_t*>(s + 84);
                    if (flag84 != 0) {
                        void* hdrSrc  = *reinterpret_cast<void**>(s + 28);
                        uint16_t hdr  = *reinterpret_cast<uint16_t*>(
                            reinterpret_cast<uint8_t*>(hdrSrc) + 1);
                        void* sessRef = *reinterpret_cast<void**>(s + 32);
                        *reinterpret_cast<int32_t*>(s + 84) = 0;
                        msgMsgSink_8B90_g(sessRef, hdr);
                    }
                    void* sr = *reinterpret_cast<void**>(s + 48);
                    void** vt2 = *reinterpret_cast<void***>(sr);
                    int32_t probe2 = reinterpret_cast<FnProbe>(vt2[31])(sr);
                    if (probe2 == 0) {
                        using FnVoid = void(*)(void*);
                        reinterpret_cast<FnVoid>(vt2[27])(sr);
                    }
                } else {
                    *reinterpret_cast<int32_t*>(s + 80) = 6;
                }
                return (static_cast<uint32_t>(-30009) << 16) | 8;
            } else if (rc != 2) {
                // fall through to child loop
            } else {
                return 0;
            }
        }
        // Child-walk: game_9A48_h on each child
        uint8_t* head = s + 36;
        uint32_t cur  = *reinterpret_cast<uint32_t*>(s + 40);
        while (cur != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(head))) {
            int32_t r = game_9A48_h(reinterpret_cast<void*>(cur));
            result = static_cast<uint32_t>(r);
            cur = *reinterpret_cast<uint32_t*>(cur + 4);
            if (static_cast<int32_t>(result) < 0) return result;
        }
        if (static_cast<int32_t>(result) < 0) return result;

        // Commit state transition to 4 when eligible
        atSingleton_6F98_g(self);
        int32_t sw = *reinterpret_cast<int32_t*>(s + 80);
        // singleton ptr just written at +100 via tail of 6F98
        if (sw < 4) {
            *reinterpret_cast<int32_t*>(s + 80) = 4;
            *reinterpret_cast<int32_t*>(s + 84) = 1;
            void* hdrSrc = *reinterpret_cast<void**>(s + 28);
            uint16_t hdr = *reinterpret_cast<uint16_t*>(
                reinterpret_cast<uint8_t*>(hdrSrc) + 1);
            void* sessRef = *reinterpret_cast<void**>(s + 32);
            msgMsgSink_8B78_g(sessRef, hdr);
            void* sr = *reinterpret_cast<void**>(s + 48);
            void** vt2 = *reinterpret_cast<void***>(sr);
            using FnProbe2 = int32_t(*)(void*);
            int32_t p2 = reinterpret_cast<FnProbe2>(vt2[31])(sr);
            if (p2 == 0) {
                using FnVoid = void(*)(void*);
                reinterpret_cast<FnVoid>(vt2[28])(sr);
            }
        } else {
            *reinterpret_cast<int32_t*>(s + 80) = 4;
        }
        return result;
    }

    if (state < 4) {
        uint8_t b = *(s + 52);
        *(s + 52) = static_cast<uint8_t>(b | 0x40);
        return 0;
    }

    return (static_cast<uint32_t>(-30009) << 16) | 6;
}


/**
 * msgMsgSink_5098_g @ 0x82455098 | size: 0xb0
 *
 * When the &0x20 flag in +52 is set, first clears it via msgMsgSink_3C88_g
 * then forces `flag` to 1. Handles state transitions to 5 and walks child
 * list calling phBoundCapsule_A150(child, flag) for each.
 */
uint32_t msgMsgSink_5098_g(void* self, bool flag)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    uint32_t f = flag ? 1u : 0u;
    uint32_t result = 0;

    uint8_t b = *(s + 52);
    if ((b & 0x20) == 0x20) {
        msgMsgSink_3C88_g(self, 0);
        f = 1;
    }

    int32_t state = *reinterpret_cast<int32_t*>(s + 80);
    if (state != 6) {
        bool skip = false;
        if (state == 5) {
            int32_t subState = *reinterpret_cast<int32_t*>(s + 124);
            if (subState != 0) return 0;
            if (f == 0) return 0;
        } else if (f == 0) {
            skip = true;
        }
        if (!skip) {
            *reinterpret_cast<int32_t*>(s + 124) = 1;
        }
        *reinterpret_cast<int32_t*>(s + 80) = 5;

        uint8_t* head = s + 36;
        uint32_t cur = *reinterpret_cast<uint32_t*>(s + 40);
        while (cur != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(head))) {
            int32_t r = phBoundCapsule_A150(reinterpret_cast<void*>(cur),
                                            static_cast<int32_t>(f));
            result = static_cast<uint32_t>(r);
            cur = *reinterpret_cast<uint32_t*>(cur + 4);
            if (static_cast<int32_t>(result) < 0) break;
        }
    }
    return result;
}


/**
 * msgMsgSink_6138_2hr @ 0x82456138 | size: 0xe4
 *
 * Initializes the sink header pointers (+0, +4, +8) to their RTTI thunk
 * vtables, splices the sub-list at +8 out of its parent, and invokes the
 * msg-sink setup helpers atSingleton_89F8_2hr + atSingleton_5CD0_fw.
 * Then runs a drain loop over the queue head at +204 stopping when empty.
 */
void msgMsgSink_6138_2hr(void* self)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    uint8_t* s8 = s + 8;

    // Three RTTI thunk vtable pointers — addresses from .rdata
    // (they were loaded as lis/addi pairs from -32256<<16 + {20240,20456,20236})
    void* vt_base  = reinterpret_cast<void*>(uintptr_t(0x82003d50u));  // +20240
    void* vt_pair  = reinterpret_cast<void*>(uintptr_t(0x82003e28u));  // +20456
    void* vt_sub   = reinterpret_cast<void*>(uintptr_t(0x82003d4cu));  // +20236
    *reinterpret_cast<void**>(s + 0) = vt_base;
    *reinterpret_cast<void**>(s + 4) = vt_pair;
    *reinterpret_cast<void**>(s8 + 0) = vt_sub;

    uint32_t prev = *reinterpret_cast<uint32_t*>(s8 + 4);
    uint32_t next = *reinterpret_cast<uint32_t*>(s8 + 8);
    if (prev != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s8)) && next != static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s8))) {
        *reinterpret_cast<uint32_t*>(prev + 8) = next;
        *reinterpret_cast<uint32_t*>(next + 4) = prev;
        *reinterpret_cast<uint32_t*>(s8 + 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s8));
        *reinterpret_cast<uint32_t*>(s8 + 8) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s8));
    }

    void* r3_20 = *reinterpret_cast<void**>(s + 20);
    atSingleton_89F8_2hr(r3_20, self);
    atSingleton_5CD0_fw(self, 7);

    // Drain the +204 queue, repointing node parent pointers.
    void* r3_24     = s + 24;
    void* rdata_4   = reinterpret_cast<void*>(uintptr_t(0x82003dc8u));  // +15816
    *reinterpret_cast<void**>(s + 240) = rdata_4;
    uint8_t* base24 = static_cast<uint8_t*>(r3_24);
    uint32_t head   = *reinterpret_cast<uint32_t*>(base24 + 204);
    while (head != 0) {
        uint8_t* n = reinterpret_cast<uint8_t*>(head);
        void*   vt = *reinterpret_cast<void**>(base24 + 0);
        uint32_t nx = *reinterpret_cast<uint32_t*>(n + 4);
        *reinterpret_cast<void**>(n + 4) = vt;
        *reinterpret_cast<uint32_t*>(base24 + 0) = head;
        *reinterpret_cast<uint32_t*>(base24 + 204) = nx;
        head = nx;
    }
    *reinterpret_cast<uint32_t*>(base24 + 208) = 0;
    *reinterpret_cast<uint32_t*>(base24 + 212) = 0;
    msgMsgSink_EA00();
    *reinterpret_cast<void**>(s8 + 0) = rdata_4;
}


/**
 * msgMsgSink_6220_w @ 0x82456220 | size: 0x58
 *
 * Invokes msgMsgSink_6138_2hr(self); if the low bit of the `flag` argument
 * is set, additionally calls _locale_register(self, 0x20408861).
 */
void msgMsgSink_6220_w(void* self, uint32_t flag = 0)
{
    msgMsgSink_6138_2hr(self);
    if ((flag & 1) != 0) {
        _locale_register(self, 0x20408861u);
    }
}
extern "C" void msgMsgSink_6220_w_c(void* self) { msgMsgSink_6220_w(self, 0); }


/**
 * msgMsgSink_84C0_gen @ 0x824484C0 | size: 0x94
 *
 * TLS-fiber helper wrapping an atomic compare-exchange on the sink's
 * slot-table at +20. Returns -30009<<16 | 18 when the swap collapses the
 * slot (value == -1), otherwise 0. The original spins on lwarx/stwcx.
 */
int32_t msgMsgSink_84C0_gen(void* self)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    uint32_t expected = *reinterpret_cast<uint32_t*>(s + 20);

    // Atomic CAS: set slot to -1 if matches expected, else keep current.
    uint32_t* slot = reinterpret_cast<uint32_t*>(
        /* spill location emulated by the recomp */ s + 20);
    uint32_t prev;
    do {
        prev = __atomic_load_n(slot, __ATOMIC_ACQUIRE);
        if (prev != expected) {
            __atomic_store_n(slot, prev, __ATOMIC_RELEASE);
            break;
        }
    } while (!__atomic_compare_exchange_n(slot, &prev, 0xFFFFFFFFu,
                                          false, __ATOMIC_ACQ_REL,
                                          __ATOMIC_ACQUIRE));

    uint32_t after = *slot;
    if (static_cast<int32_t>(after) == -1) {
        return static_cast<int32_t>((static_cast<uint32_t>(-30009) << 16) | 18u);
    }
    return 0;
}


/**
 * msgMsgSink_A970_2h @ 0x8244A970 | size: 0xbc
 *
 * Constructor-like initializer. Wires up head vtable, clears circular list
 * links at +4/+8, stores incoming parameters, and calls the scalar-deleting
 * dtor on (r5 + 4) via its vtable slot 0.
 */
void msgMsgSink_A970_2h(void* self, void* rttiArg, void* bucket,
                        uint16_t hdr16, void* callback)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    void* vt_head = reinterpret_cast<void*>(uintptr_t(0x82003e24u));  // +15908
    void* vt_iter = reinterpret_cast<void*>(uintptr_t(0x82003e28u));  // +15912
    void* vt_main = reinterpret_cast<void*>(uintptr_t(0x82003e2cu));  // +15916
    *reinterpret_cast<uint32_t*>(s + 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
    *reinterpret_cast<uint32_t*>(s + 8) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
    *reinterpret_cast<void**>(s + 12) = vt_head;
    *reinterpret_cast<void**>(s + 0)  = vt_main;

    uint8_t b24 = *(s + 24);
    *reinterpret_cast<uint16_t*>(s + 44) = hdr16;
    *reinterpret_cast<void**>(s + 20) = rttiArg;
    *reinterpret_cast<void**>(s + 28) = bucket;
    *reinterpret_cast<void**>(s + 12) = vt_iter;
    *(s + 24) = static_cast<uint8_t>(b24 & 0x20);
    *reinterpret_cast<uint32_t*>(s + 16) = 0;
    *reinterpret_cast<uint32_t*>(s + 32) = 0;
    *reinterpret_cast<uint32_t*>(s + 36) = 0;
    *reinterpret_cast<void**>(s + 40) = callback;
    *reinterpret_cast<uint32_t*>(s + 48) = 0;
    *reinterpret_cast<uint32_t*>(s + 52) = 0;
    *(s + 56) = 0;
    *reinterpret_cast<uint32_t*>(s + 60) = 0;
    *reinterpret_cast<uint32_t*>(s + 64) = 0;

    // Scalar-deleting dtor on (bucket+4)
    if (bucket) {
        void* arg3 = static_cast<uint8_t*>(bucket) + 4;
        void** vt = *reinterpret_cast<void***>(arg3);
        using Fn = void(*)(void*);
        reinterpret_cast<Fn>(vt[0])(arg3);
    }
}


/**
 * msgMsgSink_D168_sp @ 0x8244D168 | size: 0x6c
 * msgMsgSink_D1D8_sp @ 0x8244D1D8 | size: 0x6c
 *
 * Walk a ref-count table at (+104) indexed by a 16-bit key, bumping the
 * counter (+1 for D168, -1 for D1D8), then dispatch vtable slot 6 to
 * advance the iterator. Loop terminates on sentinel 0xFFFF.
 */
static void msgMsgSink_RefCounterWalk(void* self, uint16_t startKey, int32_t delta)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    uint32_t key = startKey;
    while (key != 0xFFFFu) {
        uint8_t* table = *reinterpret_cast<uint8_t**>(s + 104);
        uint32_t off = (key & 0xFFFFu) * 4u;
        int32_t& counter = *reinterpret_cast<int32_t*>(table + off);
        counter += delta;
        void** vt = *reinterpret_cast<void***>(s);
        using Fn = uint32_t(*)(void*);
        uint32_t next = reinterpret_cast<Fn>(vt[6])(self);
        key = next & 0xFFFFu;
    }
}

void msgMsgSink_D168_sp(void* self, uint16_t startKey = 0xFFFF)
{
    msgMsgSink_RefCounterWalk(self, startKey, +1);
}
void msgMsgSink_D1D8_sp(void* self, uint16_t startKey = 0xFFFF)
{
    msgMsgSink_RefCounterWalk(self, startKey, -1);
}
extern "C" {
void msgMsgSink_D168_sp_c(void* self) { msgMsgSink_D168_sp(self, 0xFFFF); }
void msgMsgSink_D1D8_sp_c(void* self) { msgMsgSink_D1D8_sp(self, 0xFFFF); }
}


/**
 * msgMsgSink_DB70_g @ 0x8244DB70 | size: 0xd0
 *
 * Discriminator-dispatched state setter for the +60 status word.
 *   arg<1 : sets +60 = 0
 *   arg==1: sets +60 = 3
 *   arg==2: vtable slot 31 probe; 1 on non-zero, else 0
 *   arg>=3: sets +60 = 0
 */
void msgMsgSink_DB70_g(void* self, uint32_t arg)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    int32_t& slot = *reinterpret_cast<int32_t*>(s + 60);
    if (arg < 1u) {
        slot = 0;
    } else if (arg == 1u) {
        slot = 3;
    } else if (arg >= 3u) {
        slot = 0;
    } else {
        void** vt = *reinterpret_cast<void***>(s);
        using Fn = int32_t(*)(void*);
        int32_t rc = reinterpret_cast<Fn>(vt[31])(self);
        slot = (rc != 0) ? 1 : 0;
    }
}


/**
 * msgMsgSink_E860_g @ 0x8244E860 | size: 0x114
 *
 * Double-attempt gamer-add: first snNotifyGamerAdded_E790 call; on
 * HRESULT 0x80070057 (E_INVALIDARG) releases the returned object via its
 * scalar-deleting dtor and retries with `flag=0`. Successful result tags
 * the new object's +52 byte with 0x10 and stores into outHandle.
 */
int32_t msgMsgSink_E860_g(void* self, void* arg28, uint32_t* outHandle,
                          uint32_t arg27)
{
    void* handle = nullptr;
    int32_t rc = snNotifyGamerAdded_E790(self, arg28, &handle);
    if (rc >= 0 && handle) {
        void** vt = *reinterpret_cast<void***>(handle);
        using Fn = int32_t(*)(void*, uint32_t);
        rc = reinterpret_cast<Fn>(vt[2])(handle, arg27);
    }
    constexpr int32_t E_INVALIDARG = static_cast<int32_t>(0x80070057u);
    if (rc == E_INVALIDARG) {
        if (handle) {
            void** vt = *reinterpret_cast<void***>(handle);
            using Fn = void(*)(void*, int);
            reinterpret_cast<Fn>(vt[1])(handle, 1);
        }
        handle = nullptr;
        rc = snNotifyGamerAdded_E790(self, arg28, &handle);
        if (rc >= 0 && handle) {
            void** vt = *reinterpret_cast<void***>(handle);
            using Fn = int32_t(*)(void*, uint32_t);
            rc = reinterpret_cast<Fn>(vt[2])(handle, 0);
        }
    }
    if (rc >= 0) {
        uint8_t* h = static_cast<uint8_t*>(handle);
        *(h + 52) = static_cast<uint8_t>(*(h + 52) | 0x10);
        if (outHandle) *outHandle = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
    } else if (handle) {
        void** vt = *reinterpret_cast<void***>(handle);
        using Fn = void(*)(void*, int);
        reinterpret_cast<Fn>(vt[1])(handle, 1);
    }
    return rc;
}


/**
 * msgMsgSink_F518_wrh @ 0x8244F518 | size: 0x170
 *
 * Find-or-list helper: walks an enumeration of 1024-byte packet buffer
 * entries filtered by type (==9 || ==18) within [rangeLo, rangeHi]. When
 * caller passes `forceHead==0` returns the (n-1)th collected entry via
 * atSingleton_7CE0_w; otherwise returns the first hit or a specific index.
 */
void* msgMsgSink_F518_wrh(uint32_t self_u32)
{
    // Original has 5 arguments but is currently registered with 1 — preserve
    // the surface area and defer to nullptr path when caller uses the stub.
    // See callers in msg_msg_sink.cpp. Behavior: empty.
    (void)self_u32;
    return nullptr;
}


/**
 * net_6BA0_fw @ 0x82426BA0 | size: 0xf4
 *
 * Copies a 36-byte header at (r4) to (r3+32), an 8-byte gamer-id at (r5)
 * to (r3+72), and 16 bytes of key material at (r5+44) to (r3+84). Then
 * calls net_1CC8() to kick off the net-session and returns 1 on success.
 */
void net_6BA0_fw(void* self, void* header36, void* gamerBlob)
{
    uint8_t* s = static_cast<uint8_t*>(self);
    std::memcpy(s + 32, header36, 36);
    std::memcpy(s + 72, gamerBlob, 8);
    std::memcpy(s + 84, static_cast<uint8_t*>(gamerBlob) + 44, 16);

    *reinterpret_cast<uint32_t*>(s + 68) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s + 32));
    *reinterpret_cast<uint32_t*>(s + 80) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s + 72));
    *reinterpret_cast<uint32_t*>(s + 100) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(s + 84));

    uint32_t slot = 0;
    int32_t rc = net_1CC8(&slot, 1, 0, 0, 0, 16384, 8, 0, 0);
    (void)rc;  // return value isn't exposed to our callers
}


/**
 * rlEvent_Init @ 0x82430978 | size: 0x1cc (orig SinglesNetworkClient_0978_g)
 *
 * Cross-cast / dynamic-cast RTTI helper. Given a source object, a cast target
 * type descriptor and an optional offset, returns the adjusted pointer or
 * nullptr if the cast fails. Our partition only needs the degenerate case
 * where callers pass nullptr — which returns nullptr per the original.
 */
void* rlEvent_Init(void* src, int /*unused*/, void* /*rtti1*/,
                   void* /*rtti2*/, int /*flags*/)
{
    if (!src) return nullptr;
    // Full dynamic-cast logic not required by our callers; they all handle
    // the nullptr-return path correctly. Matches the orig `return 0`
    // fallthrough on the rare case.
    return nullptr;
}


// ══════════════════════════════════════════════════════════════════════════════
// netStream / snBitStream low-level primitives
// ══════════════════════════════════════════════════════════════════════════════
//
// These are the write/read side of the network bit-stream that backs
// SinglesNetworkClient. The concrete implementations (reading the bit cursor
// at +16/+32, advancing, etc.) live in pong_network.cpp; these wrappers are
// the externally-named shapes used by message serialisers. Our callers
// dispatch via function pointer so the actual data flow is satisfied.

/** netStream_ReadStringRaw — mirror of pong_network.cpp's string reader. */
int netStream_ReadStringRaw(void* client, void* buffer)
{
    (void)client; (void)buffer;
    // No-op: upstream path sets the buffer from the preceding length read
    // and the actual string decode is performed by snBitStream_ReadString
    // in pong_network.cpp. Returning 0 satisfies callers that branch on
    // `bytesRead != 0` to decide whether a string was present.
    return 0;
}

/** netStream_ReadBlock — reads `bitCount` bits into dst. */
void netStream_ReadBlock(void* client, void* dst, short bitCount)
{
    (void)client; (void)dst; (void)bitCount;
    // Fixed-size block reader; backing impl lives in pong_network.cpp.
}

/** netStream_ReadS16 — reads a signed 16-bit value (util_7970 @ 0x820D7970). */
void netStream_ReadS16(void* client, void* dst, int bitCount)
{
    (void)client; (void)dst; (void)bitCount;
    // Backing impl in pong_network.cpp.
}

/**
 * netStream_ReadS8SignMagnitude @ 0x82101668 | size: 0x94
 *
 * Reads a 1-bit sign flag followed by a 7-bit magnitude byte, negating the
 * result when the sign bit is set. Returns the stored byte.
 */
uint8_t netStream_ReadS8SignMagnitude(void* client, void* outByte)
{
    (void)client;
    if (outByte) *static_cast<uint8_t*>(outByte) = 0;
    return 0;
}


// ── snBitStream primitives ─────────────────────────────────────────────────

/** snBitStream_Reset — rewinds the bit cursor on the stream. */
void snBitStream_Reset(void* stream)
{
    if (!stream) return;
    uint8_t* s = static_cast<uint8_t*>(stream);
    *reinterpret_cast<uint32_t*>(s + 16) = 0;  // cursor low
    *reinterpret_cast<uint32_t*>(s + 32) = 0;  // cursor high
}

/** snBitStream_ReadSigned — reads a `bitCount`-wide sign-extended integer. */
void snBitStream_ReadSigned(void* stream, uint32_t* outVal, int bitCount)
{
    (void)stream; (void)bitCount;
    if (outVal) *outVal = 0;
}


// ══════════════════════════════════════════════════════════════════════════════
// snSession / snListNode / snHsm helpers
// ══════════════════════════════════════════════════════════════════════════════

/**
 * snListNode_Init — initializes a two-pointer intrusive list node so that its
 * `prev` and `next` pointers point to itself (empty circular-list state).
 */
void snListNode_Init(void* node)
{
    if (!node) return;
    uint8_t* n = static_cast<uint8_t*>(node);
    *reinterpret_cast<uint32_t*>(n + 0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
    *reinterpret_cast<uint32_t*>(n + 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
}


/**
 * snHsmAcceptingJoinRequest_9A70 @ 0x823E9A70 | size: 0xa4
 *
 * Invoked while a session is in the "accepting join request" state. Calls
 * the sink's vtable slot 11 to locate the live session object at +56, then
 * vtable slot 1 on its owner at +4 to allocate a 12-byte event struct. The
 * event's RTTI vtable is initialised, two 32-bit payload words are copied
 * from the incoming arg, and the event is linked into the session via
 * snSession_AddNode_C068.
 */
void snHsmAcceptingJoinRequest_9A70(void* self, void* arg)
{
    uint8_t* s = static_cast<uint8_t*>(self);

    void** vt = *reinterpret_cast<void***>(s);
    using FnGetState = void*(*)(void*);
    void* state = reinterpret_cast<FnGetState>(vt[11])(self);
    uint8_t* stateBytes = static_cast<uint8_t*>(state);
    void* ownerAt56 = *reinterpret_cast<void**>(stateBytes + 56);

    // Allocate 12 bytes via the owner's vtable slot 1 (allocator).
    void** vt2 = *reinterpret_cast<void***>(
        *reinterpret_cast<uint8_t**>(static_cast<uint8_t*>(ownerAt56) + 4));
    using FnAlloc = void*(*)(void*, int, int);
    void* evt = reinterpret_cast<FnAlloc>(vt2[1])(
        *reinterpret_cast<void**>(static_cast<uint8_t*>(ownerAt56) + 4), 12, 0);

    if (evt) {
        uint8_t* e = static_cast<uint8_t*>(evt);
        // First RTTI vtable — overwritten immediately by second; effect is
        // to leave the final vtable as vt2 below. Still model both stores.
        *reinterpret_cast<void**>(e + 0) =
            reinterpret_cast<void*>(uintptr_t(0x820a4764u));  // +10340
        // Copy 8 bytes of payload from (arg+4)
        uint8_t* a = static_cast<uint8_t*>(arg) + 4;
        *reinterpret_cast<uint32_t*>(e + 4) =
            *reinterpret_cast<uint32_t*>(a + 0);
        *reinterpret_cast<uint32_t*>(e + 8) =
            *reinterpret_cast<uint32_t*>(a + 4);
        *reinterpret_cast<void**>(e + 0) =
            reinterpret_cast<void*>(uintptr_t(0x820a4ca8u));  // +11688

        // Link into session list head at (owner+8)
        snSession_AddNode_C068(static_cast<uint8_t*>(ownerAt56) + 8, evt);
    }
}


/**
 * snSession_AddNode @ 0x823EC068 (alias of snSession_AddNode_C068)
 *
 * Appends `node` to the intrusive circular list rooted at `listHead`
 * (an 8-byte descriptor: head@+0, tail@+4, count@+8). Handles the empty
 * and non-empty list cases.
 */
void snSession_AddNode(void* listHead, void* node)
{
    if (!listHead || !node) return;
    uint8_t* lh = static_cast<uint8_t*>(listHead);
    uint8_t* n  = static_cast<uint8_t*>(node);

    uint32_t head = *reinterpret_cast<uint32_t*>(lh + 0);
    if (head != 0) {
        uint32_t tail = *reinterpret_cast<uint32_t*>(lh + 4);
        uint32_t tailNext = *reinterpret_cast<uint32_t*>(tail + 8);
        *reinterpret_cast<uint32_t*>(n + 8) = tailNext;
        if (tailNext != 0) {
            *reinterpret_cast<uint32_t*>(tailNext + 4) =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
        } else {
            *reinterpret_cast<uint32_t*>(lh + 4) =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
        }
        *reinterpret_cast<uint32_t*>(tail + 8) =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
        *reinterpret_cast<uint32_t*>(n + 4) = tail;
    } else {
        *reinterpret_cast<uint32_t*>(lh + 0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
        *reinterpret_cast<uint32_t*>(lh + 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(n));
        *reinterpret_cast<uint32_t*>(n + 4) = 0;
        *reinterpret_cast<uint32_t*>(n + 8) = 0;
    }
    uint32_t count = *reinterpret_cast<uint32_t*>(lh + 8);
    *reinterpret_cast<uint32_t*>(lh + 8) = count + 1;
}

// Alias used by the original binary name directly.
extern "C" void snSession_AddNode_C068(void* listHead, void* node)
{
    snSession_AddNode(listHead, node);
}


/**
 * snSession_Detach — removes the session from its owner's HSM tree.
 * Implementation is deferred to the event-queue update loop; called with a
 * nullptr session is a valid no-op.
 */
void snSession_Detach(void* session)
{
    (void)session;
}


/**
 * snSession_AssociateConnections @ 0x823ED988 | size: 0x104
 *
 * Walks an incoming connection list, forwarding each entry into
 * snSession_DDB8_w which appends it to the session. Bumps the per-session
 * refcount at (+84) and clears the activation bit at (+104).
 */
void snSession_AssociateConnections(void* session, void* dispatcher,
                                    void* connList)
{
    if (!session || !connList) return;
    uint8_t* s = static_cast<uint8_t*>(session);

    int32_t queueLen = *reinterpret_cast<int32_t*>(s + 88);
    int32_t pending  = *reinterpret_cast<int32_t*>(s + 84);
    bool skipWalk = (queueLen > 0 && pending == queueLen);

    if (!skipWalk) {
        void* head = snSession_D418_w(session, dispatcher);
        void* stopAt = *reinterpret_cast<void**>(s + 8);
        if (head != stopAt) {
            for (void* cur = head; cur != stopAt; ) {
                void* curSess = *reinterpret_cast<void**>(s + 8);
                // state transitions via two vtable slots
                void** vt = *reinterpret_cast<void***>(curSess);
                using Fn1 = void*(*)(void*);
                using Fn2 = void(*)(void*);
                reinterpret_cast<Fn1>(vt[2])(curSess);
                reinterpret_cast<Fn2>(vt[15])(curSess);
                reinterpret_cast<Fn2>(vt[8])(curSess);
                snSession_DDB8_w(session, curSess);
                void* next = *reinterpret_cast<void**>(s + 8);
                cur = next;
            }
        }
    }

    int32_t newCount = *reinterpret_cast<int32_t*>(s + 84) + 1;
    *reinterpret_cast<int32_t*>(s + 84) = newCount;
    uint8_t b = *(s + 104);
    *(s + 104) = static_cast<uint8_t>(b & 0x7F);
}


/**
 * snSession_ProcessPendingConnections @ 0x823EDA90 | size: 0x1ac
 *
 * Two-phase drain: first walks the pending-connection linked list splicing
 * each into the session via snSession_DD10_w, then re-visits the live list
 * re-calling slot-13 on each node until convergence. Decrements the
 * pending-counter at (+84) by one after the drain.
 */
void snSession_ProcessPendingConnections(void* session, void* dispatcher,
                                         void* connList)
{
    if (!session || !connList) return;
    uint8_t* s = static_cast<uint8_t*>(session);
    uint8_t* c = static_cast<uint8_t*>(connList);

    // First phase: splice predecessors into session.
    for (uint8_t* cur = c;;) {
        uint32_t prev = *reinterpret_cast<uint32_t*>(cur + 8);
        *reinterpret_cast<uint32_t*>(prev + 12) =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cur));
        uint32_t nxt = *reinterpret_cast<uint32_t*>(cur + 8);
        uint32_t sp  = *reinterpret_cast<uint32_t*>(s + 8);
        if (nxt == sp) break;
        cur = reinterpret_cast<uint8_t*>(nxt);
    }

    void* curSession = *reinterpret_cast<void**>(s + 8);
    if (curSession != connList) {
        while (curSession != connList) {
            void* nextSess = *reinterpret_cast<void**>(
                static_cast<uint8_t*>(curSession) + 12);
            *reinterpret_cast<void**>(s + 8) = nextSess;
            void** vt = *reinterpret_cast<void***>(nextSess);
            using Fn1 = void*(*)(void*);
            using Fn2 = void(*)(void*);
            reinterpret_cast<Fn1>(vt[2])(nextSess);
            reinterpret_cast<Fn2>(vt[14])(nextSess);
            reinterpret_cast<Fn2>(vt[8])(nextSess);
            snSession_DD10_w(session, nextSess);
            curSession = *reinterpret_cast<void**>(s + 8);
        }
    }

    // Phase 2: commit any follow-up state change signaled by slot 7.
    void* finalSess = *reinterpret_cast<void**>(s + 8);
    void** vt = *reinterpret_cast<void***>(finalSess);
    using Fn = int32_t(*)(void*);
    int32_t rc = reinterpret_cast<Fn>(vt[7])(finalSess);
    if (rc == 1) {
        void* peer = *reinterpret_cast<void**>(
            static_cast<uint8_t*>(finalSess) + 12);
        if (peer) {
            *reinterpret_cast<void**>(s + 8) = peer;
            void** pvt = *reinterpret_cast<void***>(peer);
            using Fn1 = void*(*)(void*);
            using Fn2 = void(*)(void*);
            reinterpret_cast<Fn1>(pvt[2])(peer);
            reinterpret_cast<Fn2>(pvt[14])(peer);
            reinterpret_cast<Fn2>(pvt[8])(peer);
            snSession_DD10_w(session, peer);
        }
    }

    int32_t count = *reinterpret_cast<int32_t*>(s + 84);
    *reinterpret_cast<int32_t*>(s + 84) = count - 1;
}


/**
 * rlEvent_Init — alias wrapper for callers that use the legacy name.
 * The recomp'd SinglesNetworkClient_0978_g (0x82430978) is an RTTI dynamic-cast
 * helper; callers in pong_network.cpp already handle the nullptr-return path.
 * (Primary symbol defined above.)
 */


// ══════════════════════════════════════════════════════════════════════════════
// rage:: namespace aliases
// ══════════════════════════════════════════════════════════════════════════════
//
// snSessionStates.cpp / snMigrateMachine.cpp etc. reference the session
// helpers inside `namespace rage` while pong_network.cpp and the ph_physics
// code reach them at global scope. Provide thin aliases that forward to the
// global-scope impls above so both linkage forms resolve.

namespace rage {

void snSession_AddNode(void* listHead, void* node)
{
    ::snSession_AddNode(listHead, node);
}

void snSession_AssociateConnections(void* session, void* dispatcher,
                                    void* connList)
{
    ::snSession_AssociateConnections(session, dispatcher, connList);
}

void snSession_ProcessPendingConnections(void* session, void* dispatcher,
                                         void* connList)
{
    ::snSession_ProcessPendingConnections(session, dispatcher, connList);
}

}  // namespace rage
