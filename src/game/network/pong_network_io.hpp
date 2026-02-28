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
