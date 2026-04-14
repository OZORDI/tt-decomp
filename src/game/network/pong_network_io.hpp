/**
 * pong_network_io.hpp — Network I/O helper function declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Low-level network serialization functions for ball physics and hit data.
 * These handle bit-packed encoding/decoding for the SinglesNetworkClient protocol
 * used during multiplayer matches to synchronize ball state across clients.
 * 
 * NETWORK PROTOCOL ARCHITECTURE:
 * 
 * The game uses a sophisticated bit-packed protocol for bandwidth efficiency.
 * All network messages follow a vtable pattern:
 *   - vfn_0: Destructor
 *   - vfn_1: GetMessageType (likely)
 *   - vfn_2: Serialize (WRITE to network stream)
 *   - vfn_3: Deserialize (READ from network stream)
 *   - vfn_4: Validation/processing (likely)
 * 
 * WRITE OPERATIONS (this file):
 * - WriteFloatToNetworkStream: 32-bit float with alignment
 * - WriteBallHitDataToNetwork: Complete ball physics state (~192 bytes)
 * - WriteVector3ToNetwork: 3-component vectors
 * 
 * READ OPERATIONS (to be implemented):
 * - ReadFloatFromNetworkStream: Deserialize float
 * - ReadBallHitDataFromNetwork: Deserialize ball physics
 * - ReadVector3FromNetwork: Deserialize vectors
 * 
 * USAGE:
 * These functions are called by network message classes (46+ message types)
 * during multiplayer synchronization. Primary use case is ball hit events
 * during rallies, serves, and spectator mode.
 */

#pragma once

#include <stdint.h>

/**
 * WriteFloatToNetworkStream @ 0x820D6990
 * 
 * Writes a 32-bit float to the network bit stream with proper alignment.
 * Handles bit position tracking and byte-alignment operations.
 * 
 * @param client  SinglesNetworkClient object managing the network stream
 * @param value   Float value to write to the stream
 */
void WriteFloatToNetworkStream(void* client, float value);

/**
 * WriteBallHitDataToNetwork @ 0x821D5738
 * 
 * Serializes ball hit/physics data to the network stream for multiplayer sync.
 * Handles position, velocity, spin, collision data, and conditional extended physics.
 * 
 * Used by network messages:
 * - HitDataMessage (ball hit during rally)
 * - BallHitMessage (ball contact event)
 * - SpectatorBallHitMessage (spectator view of ball hit)
 * 
 * The pongBallHitData structure layout (~192 bytes):
 * - Vector groups for position, velocity, spin, trajectory
 * - Physics scalars (bounce coefficients, timing, power)
 * - Hit zone and swing type control bytes
 * - State flags and player indices
 * - Conditional extended data (impact normal, surface velocity)
 * 
 * @param ballHitData  Pointer to pongBallHitData structure to serialize
 * @param client       SinglesNetworkClient object managing the network stream
 */
void WriteBallHitDataToNetwork(void* ballHitData, void* client);

/**
 * WriteVector3ToNetwork @ 0x821D9BB8
 * 
 * Writes a 3-component vector (vec3) to the network stream.
 * Used for position, velocity, normal, and other 3D vector data.
 * 
 * @param client    SinglesNetworkClient object managing the network stream
 * @param vec3Data  Pointer to 3 consecutive floats (x, y, z)
 */
void WriteVector3ToNetwork(void* client, void* vec3Data);

/**
 * ReadFloatFromNetworkStream @ 0x82238DF8
 *
 * Reads a 32-bit float from the network bit stream. Mirror of
 * WriteFloatToNetworkStream (game_6990 @ 0x820D6990). Original symbol:
 * SinglesNetworkClient_8DF8_g. The low-level primitive is invoked with
 * bitWidth=32 and the output is written back to the caller-supplied buffer.
 *
 * @param client  SinglesNetworkClient object managing the network stream
 * @return        Float value read from the stream
 */
float ReadFloatFromNetworkStream(void* client);

/**
 * ReadVector3FromNetwork @ 0x821D9B40
 *
 * Reads 3 consecutive floats (x, y, z) from the network stream. Mirror of
 * WriteVector3ToNetwork (game_9BB8_h @ 0x821D9BB8). Original symbol:
 * SinglesNetworkClient_9B40_fw (size 0x78).
 *
 * @param client   SinglesNetworkClient object managing the network stream
 * @param outVec3  Destination pointer — 3 floats will be written (offsets 0/4/8)
 */
void ReadVector3FromNetwork(void* client, void* outVec3);

/**
 * ReadBallHitDataFromNetwork @ 0x821D5538
 *
 * Deserialises pongBallHitData from the network stream. Exact inverse of
 * WriteBallHitDataToNetwork (game_5738 @ 0x821D5738, size 0x1fc). Original
 * symbol: util_5538 (size 0x200).
 *
 * Layout read order matches the write side:
 *   +0..+8, +16..+24   : primary position/velocity floats
 *   +48..+56, +64..+72 : spin/trajectory via ReadVector3FromNetwork (×2)
 *   +80..+88           : physics vec3 via ReadVector3FromNetwork
 *   +96..+104          : physics vec3 via ReadVector3FromNetwork
 *   +116, +112         : additional physics scalars (+112 read from 8-bit)
 *   +32                : sign-magnitude i8 hit-type control (via 1-bit sign + 7-bit mag)
 *   +124, +128, +132   : physics scalars (+132 from 8-bit via netStream_ReadBytes)
 *   +136, +140         : state bytes (8 bits each, zero-extended into u32 fields)
 *   +176 (u16)         : flags (16-bit unsigned)
 *   +178 (u16)         : index (16-bit signed-semantic via netStream_ReadS16)
 *   +180 (u8)          : state byte (8 bits)
 *   +182 (u16)         : extended-flags (16-bit unsigned)
 *   If (extendedFlags & 0x1000):
 *     +120             : extended physics scalar
 *     +144             : impact normal vec3
 *     +160             : surface velocity vec3
 *
 * @param client       SinglesNetworkClient object managing the network stream
 * @param ballHitData  Destination pongBallHitData buffer (~192 bytes)
 */
void ReadBallHitDataFromNetwork(void* client, void* ballHitData);
