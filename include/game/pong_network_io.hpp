/**
 * pong_network_io.hpp — Network I/O Function Declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Clean, human-readable declarations for SinglesNetworkClient network I/O primitives.
 * These functions handle bit-packed serialization/deserialization for multiplayer networking.
 */

#pragma once
#include <stdint.h>

// Forward declaration
struct SinglesNetworkClient;

// ══════════════════════════════════════════════════════════════════════════════
// NETWORK STREAM READ OPERATIONS
// ══════════════════════════════════════════════════════════════════════════════

/**
 * ReadBitsFromStream @ 0x82260448 | size: 0x12c
 * 
 * Reads a variable-width bit field from the network stream.
 * Core deserialization primitive used by all network message handlers.
 * 
 * @param client    SinglesNetworkClient managing the network stream
 * @param value     Value to read into (output parameter)
 * @param bitWidth  Number of bits to read (1-32)
 * @return          Success flag (1 = success, 0 = failure/overflow)
 * 
 * Original symbol: SinglesNetworkClient_0448_g
 */
uint32_t ReadBitsFromStream(void* client, uint32_t value, int bitWidth);

// ══════════════════════════════════════════════════════════════════════════════
// NETWORK STREAM WRITE OPERATIONS
// ══════════════════════════════════════════════════════════════════════════════

/**
 * BeginNetworkWrite @ 0x822288AE0 | size: unknown
 * 
 * Prepares the network stream for a write operation.
 * Must be called before any Write* functions.
 * 
 * @param client  SinglesNetworkClient managing the network stream
 * 
 * Original symbol: SinglesNetworkClient_8AE0_g
 */
void BeginNetworkWrite(void* client);

/**
 * EndNetworkWrite @ 0x822288DF8 | size: unknown
 * 
 * Finalizes a network write operation and flushes the stream.
 * Must be called after all Write* functions complete.
 * 
 * @param client  SinglesNetworkClient managing the network stream
 * 
 * Original symbol: SinglesNetworkClient_8DF8_g
 */
void EndNetworkWrite(void* client);

/**
 * WriteUInt8Bits @ 0x820D6838 | size: 0x6c
 * 
 * Writes an unsigned 8-bit value with variable bit width to the network stream.
 * Used for flags, indices, and small unsigned integers.
 * 
 * @param client    SinglesNetworkClient managing the network stream
 * @param value     Unsigned 8-bit value to write
 * @param bitWidth  Number of bits to use (1-8)
 * 
 * Original symbol: SinglesNetworkClient_6838_g
 */
void WriteUInt8Bits(void* client, uint8_t value, int bitWidth);

/**
 * WriteInt8Bits @ 0x820D68A8 | size: 0x6c
 * 
 * Writes a signed 8-bit value with variable bit width to the network stream.
 * Used for signed indices and small signed integers.
 * 
 * @param client    SinglesNetworkClient managing the network stream
 * @param value     Signed 8-bit value to write
 * @param bitWidth  Number of bits to use (1-8)
 * 
 * Original symbol: SinglesNetworkClient_68A8_g
 */
void WriteInt8Bits(void* client, int8_t value, int bitWidth);

// ══════════════════════════════════════════════════════════════════════════════
// NETWORK STATE MANAGEMENT
// ══════════════════════════════════════════════════════════════════════════════

/**
 * UpdateNetworkState @ 0x823D2BE8 | size: 0x60
 * 
 * Updates the network client's internal state machine.
 * Called periodically to process pending network events.
 * 
 * @param client  SinglesNetworkClient to update
 * 
 * Original symbol: SinglesNetworkClient_2BE8_g
 */
void UpdateNetworkState(void* client);

/**
 * ProcessNetworkPacket @ 0x821D51C8 | size: 0xC8
 * 
 * Processes an incoming network packet.
 * Deserializes packet data and dispatches to appropriate message handlers.
 * 
 * @param client  SinglesNetworkClient receiving the packet
 * 
 * Original symbol: SinglesNetworkClient_51C8_g
 */
void ProcessNetworkPacket(void* client);

// ══════════════════════════════════════════════════════════════════════════════
// NETWORK UTILITY FUNCTIONS
// ══════════════════════════════════════════════════════════════════════════════

/**
 * CopyNetworkString @ 0x82228990 | size: 0x15C
 * 
 * Copies a string for network transmission with proper formatting.
 * Handles null-termination and buffer size constraints.
 * 
 * @param src       Source string to copy
 * @param dest      Destination buffer
 * @param maxSize   Maximum size of destination buffer
 * 
 * Original symbol: SinglesNetworkClient_8990_g
 */
void CopyNetworkString(const char* src, char* dest, int maxSize);

/**
 * ComputeNetworkHash @ 0x822FA818 | size: 0x60
 * 
 * Computes a hash value for network data integrity checking.
 * Used for packet validation and synchronization.
 * 
 * @param client  SinglesNetworkClient context
 * @param value   Value to hash
 * 
 * Original symbol: SinglesNetworkClient_A818_g
 */
void ComputeNetworkHash(void* client, int value);

/**
 * PollButtonState @ 0x822EB2A8 | size: 0x78
 * 
 * Polls the current controller button state for network synchronization.
 * Returns a bitmask of pressed buttons.
 * 
 * @param pageGroup  UI page group context (contains controller state)
 * @return           Button state bitmask (1 = button pressed)
 * 
 * Original symbol: SinglesNetworkClient_B2A8_g
 */
uint8_t PollButtonState(void* pageGroup);

/**
 * GetNetworkClient @ 0x822EB1E8 | size: 0x5C
 * 
 * Retrieves the SinglesNetworkClient instance from a context object.
 * 
 * @param context  Context object (e.g., hudBoot, pageGroup)
 * @return         Pointer to SinglesNetworkClient instance
 * 
 * Original symbol: SinglesNetworkClient_B1E8_g
 */
void* GetNetworkClient(void* context);

/**
 * InitiateConnection @ 0x822EB320 | size: 0x60
 * 
 * Initiates a network connection to a remote peer.
 * Kicks off the connection handshake process.
 * 
 * @param context  Context object containing connection parameters
 * 
 * Original symbol: SinglesNetworkClient_B320_g
 */
void InitiateConnection(void* context);

/**
 * GetPlayerID @ 0x8225E408 | size: 0x78
 * 
 * Retrieves the player ID from the network client.
 * 
 * @param client  SinglesNetworkClient instance
 * @param param   Query parameter (purpose TBD)
 * 
 * Original symbol: SinglesNetworkClient_GetPlayerID_E408
 */
void GetPlayerID(void* client, uint32_t param);
