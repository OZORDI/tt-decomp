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
// C LINKAGE ALIASES FOR BACKWARD COMPATIBILITY
// ══════════════════════════════════════════════════════════════════════════════

/**
 * These extern "C" aliases maintain backward compatibility with existing code
 * that references the original hex-based function names. All new code should
 * use the clean names defined in pong_network_io.hpp.
 */

extern "C" {
    // Original name: SinglesNetworkClient_0448_g
    uint32_t SinglesNetworkClient_0448_g(void* client, uint32_t value, int bitWidth) {
        return ReadBitsFromStream(client, value, bitWidth);
    }
    
    // Original name: SinglesNetworkClient_8AE0_g
    void SinglesNetworkClient_8AE0_g(void* client) {
        BeginNetworkWrite(client);
    }
    
    // Original name: SinglesNetworkClient_8DF8_g
    void SinglesNetworkClient_8DF8_g(void* client) {
        EndNetworkWrite(client);
    }
    
    // Original name: SinglesNetworkClient_6838_g
    void SinglesNetworkClient_6838_g(void* client, uint8_t value, int bitWidth) {
        WriteUInt8Bits(client, value, bitWidth);
    }
    
    // Original name: SinglesNetworkClient_68A8_g
    void SinglesNetworkClient_68A8_g(void* client, int8_t value, int bitWidth) {
        WriteInt8Bits(client, value, bitWidth);
    }
    
    // Original name: SinglesNetworkClient_2BE8_g
    void SinglesNetworkClient_2BE8_g(void* client) {
        UpdateNetworkState(client);
    }
    
    // Original name: SinglesNetworkClient_51C8_g
    void SinglesNetworkClient_51C8_g(void* client) {
        ProcessNetworkPacket(client);
    }
    
    // Original name: SinglesNetworkClient_8990_g
    void SinglesNetworkClient_8990_g(const char* src, char* dest, int maxSize) {
        CopyNetworkString(src, dest, maxSize);
    }
    
    // Original name: SinglesNetworkClient_A818_g
    void SinglesNetworkClient_A818_g(void* client, int value) {
        ComputeNetworkHash(client, value);
    }
    
    // Original name: SinglesNetworkClient_B2A8_g
    uint8_t SinglesNetworkClient_B2A8_g(void* pageGroup) {
        return PollButtonState(pageGroup);
    }
    
    // Original name: SinglesNetworkClient_B1E8_g
    void* SinglesNetworkClient_B1E8_g(void* context) {
        return GetNetworkClient(context);
    }
    
    // Original name: SinglesNetworkClient_B320_g
    void SinglesNetworkClient_B320_g(void* context) {
        InitiateConnection(context);
    }
    
    // Original name: SinglesNetworkClient_GetPlayerID_E408
    void SinglesNetworkClient_GetPlayerID_E408(void* client, uint32_t param) {
        GetPlayerID(client, param);
    }
}
