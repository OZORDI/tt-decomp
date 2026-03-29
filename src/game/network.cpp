/**
 * network.cpp — Network client implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <cstdint>
#include <cstring>

// Forward declarations
struct SinglesNetworkClient;

// External function declarations
extern void snDevice_ValidateIndex(void* ctx, void* base);
extern void* xam_GetInitSingleton(void* ctx, void* base);
extern void snBitStream_Reset(void* ctx, void* base);
extern void snBitStream_ReadBits(void* ctx, void* base);
extern void snBitStream_WriteBits(void* ctx, void* base);
extern void snBitStream_ValidateWrite(void* ctx, void* base);


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ComputeStringHash @ 0x8223A868 | size: 0x68
//
// Computes a case-insensitive hash of a null-terminated string.
// Converts lowercase letters (a-z) to uppercase before hashing.
// Uses a 4-bit left rotation with XOR avalanche for hash mixing.
//
// Algorithm:
//   1. For each character, convert lowercase to uppercase
//   2. Rotate hash left by 4 bits and add character value
//   3. If top 4 bits are set, XOR them back in for avalanche effect
//
// Parameters:
//   str - Null-terminated string to hash
//
// Returns:
//   32-bit hash value
// ─────────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient_ComputeStringHash(const char* str)
{
    if (!str || *str == '\0') {
        return 0;
    }
    
    uint32_t hash = 0;
    
    while (*str != '\0') {
        char ch = *str++;
        
        // Convert lowercase to uppercase (a-z -> A-Z)
        if (ch >= 'a' && ch <= 'z') {
            ch = ch - 32;
        }
        
        // Rotate hash left by 4 bits and add character
        hash = (hash << 4) + ch;
        
        // Avalanche: if top 4 bits are set, XOR them back in
        uint32_t topBits = hash & 0xF0000000;
        if (topBits != 0) {
            hash ^= (topBits >> 24);
            hash ^= topBits;
        }
    }
    
    return hash;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetDeviceString @ 0x82238600 | size: 0x74
//
// Retrieves a device-related string from the network client's device array.
// Returns an error string if the device index is invalid.
//
// Parameters:
//   deviceIndex - Index into the device array
//
// Returns:
//   Pointer to device string, or error string if index is invalid
// ─────────────────────────────────────────────────────────────────────────────
const char* SinglesNetworkClient_GetDeviceString(int deviceIndex)
{
    // Call helper to validate device index
    // snDevice_ValidateIndex(ctx, base);
    
    if (deviceIndex < 0) {
        // Return error string for invalid device index
        return "vice, userIndex[%d], fileSize[%d]...";
    }
    
    // Get singleton instance
    // void* singleton = xam_GetInitSingleton(ctx, base);
    
    // Access device array through vtable
    // uint32_t* vtable = *(uint32_t**)singleton;
    // uint32_t* deviceArray = (uint32_t*)vtable[deviceIndex];
    // const char* deviceStr = (const char*)deviceArray[1];
    
    // if (deviceStr == nullptr) {
    //     return "";  // Empty string at 0x82027423
    // }
    
    // return deviceStr;
    
    // Stub implementation
    return "";
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::Initialize @ 0x82238678 | size: 0x74
//
// Initializes a SinglesNetworkClient instance by zeroing all fields and
// setting up default values.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance to initialize
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_Initialize(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Load vtable pointer from global (cmRefreshableCtor vtable)
    // External vtable at 0x820533CC
    extern uint32_t g_cmRefreshableCtorVtable;
    clientData[9] = (uint32_t)&g_cmRefreshableCtorVtable;  // offset +36
    
    // Zero out core fields
    clientData[0] = 0;   // vtable
    clientData[1] = 0;   // flags
    clientData[2] = 0;   // field +8
    clientData[3] = 0;   // field +12
    clientData[4] = 0;   // field +16
    clientData[6] = 0;   // field +24
    clientData[7] = 0;   // field +28
    clientData[8] = 0;   // field +32
    clientData[5] = 0;   // field +20
    
    // Call initialization helper twice
    // snBitStream_Reset(ctx, base);
    // snBitStream_Reset(ctx, base);
    
    // Set field +20 to 8128
    clientData[5] = 8128;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadBufferSize @ 0x822386F0 | size: 0x68
//
// Reads and validates the buffer size field, inserting bits into a message.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   bitValue - Value to insert into message bits [18:31]
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ReadBufferSize(void* client, uint16_t bitValue)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Call pre-read helper
    // snBitStream_ValidateWrite(ctx, base);
    
    // Save current field +28 value
    uint32_t savedField28 = clientData[7];
    
    // Temporarily set field +28 to 16
    clientData[7] = 16;
    
    // Read message data
    uint32_t messageData = 0;
    // snBitStream_ReadBits(ctx, base);  // reads into messageData
    
    // Restore field +28
    clientData[7] = savedField28;
    
    // Insert bitValue into bits [18:31] of messageData
    messageData = (messageData & 0xFFFFC000) | (bitValue & 0x3FFF);
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 16
    clientData[8] = 16;
    
    // Write modified message data
    // snBitStream_WriteBits(ctx, base);
    
    // Restore field +32
    clientData[8] = savedField32;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetMaxBufferIndex @ 0x82238758 | size: 0x7C
//
// Returns the maximum buffer index based on buffer size alignment.
// If aligned size >= 48, reads from message; otherwise returns 16383.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   Maximum buffer index (14-bit value, 0-16383)
// ─────────────────────────────────────────────────────────────────────────────
uint16_t SinglesNetworkClient_GetMaxBufferIndex(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Align to 8-byte boundary: (size + 7) & ~7
    uint32_t alignedSize = (bufferSize + 7) & 0xFFFFFFF8;
    
    if (alignedSize >= 48) {
        // Save current field +28 value
        uint32_t savedField28 = clientData[7];
        
        // Temporarily set field +28 to 16
        clientData[7] = 16;
        
        // Read message data
        uint32_t messageData = 0;
        // snBitStream_ReadBits(ctx, base);  // reads into messageData
        
        // Restore field +28
        clientData[7] = savedField28;
        
        // Extract 14-bit value (bits [18:31])
        return (uint16_t)(messageData & 0x3FFF);
    }
    
    // Return maximum value if buffer too small
    return 16383;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadMessageHeader @ 0x822387D8 | size: 0x70
//
// Reads a message header if the buffer is large enough.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   Message header value, or 0 if buffer too small
// ─────────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient_ReadMessageHeader(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Align to 8-byte boundary
    uint32_t alignedSize = (bufferSize + 7) & 0xFFFFFFF8;
    
    if (alignedSize >= 48) {
        // Save current field +28 value
        uint32_t savedField28 = clientData[7];
        
        // Temporarily set field +28 to 32
        clientData[7] = 32;
        
        // Read message data
        uint32_t messageData = 0;
        // snBitStream_ReadBits(ctx, base);  // reads into messageData
        
        // Restore field +28
        clientData[7] = savedField28;
        
        return messageData;
    }
    
    return 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadUInt16 @ 0x82238848 | size: 0x74
//
// Reads a 16-bit unsigned integer from the message buffer.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   16-bit value read from message, or 0 if buffer too small
// ─────────────────────────────────────────────────────────────────────────────
uint16_t SinglesNetworkClient_ReadUInt16(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Align to 8-byte boundary
    uint32_t alignedSize = (bufferSize + 7) & 0xFFFFFFF8;
    
    if (alignedSize >= 48) {
        // Save current field +28 value
        uint32_t savedField28 = clientData[7];
        
        // Temporarily set field +28 to 0 (read mode)
        clientData[7] = 0;
        
        // Read message data
        uint32_t messageData = 0;
        // snBitStream_ReadBits(ctx, base);  // reads into messageData
        
        // Restore field +28
        clientData[7] = savedField28;
        
        // Extract 16-bit value
        return (uint16_t)(messageData & 0xFFFF);
    }
    
    return 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadBooleanFlag @ 0x82238940 | size: 0x8C
//
// Reads a boolean flag from bit 15 of the message data.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   true if bit 15 is set, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_ReadBooleanFlag(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Align to 8-byte boundary
    uint32_t alignedSize = (bufferSize + 7) & 0xFFFFFFF8;
    
    uint32_t messageData = 0;
    
    if (alignedSize >= 48) {
        // Save current field +28 value
        uint32_t savedField28 = clientData[7];
        
        // Temporarily set field +28 to 16
        clientData[7] = 16;
        
        // Read message data
        // snBitStream_ReadBits(ctx, base);  // reads into messageData
        
        // Restore field +28
        clientData[7] = savedField28;
    }
    
    // Check bit 15 (0x8000)
    return (messageData & 0x8000) != 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteBooleanFlag1 @ 0x822388C0 | size: 0x80
//
// Writes a boolean flag by setting or clearing bit 15 in the message.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   flag - Boolean value to write (true sets bit 15, false clears it)
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_WriteBooleanFlag1(void* client, bool flag)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Call pre-write helper
    // snBitStream_ValidateWrite(ctx, base);
    
    // Save current field +28 value
    uint32_t savedField28 = clientData[7];
    
    // Temporarily set field +28 to 16
    clientData[7] = 16;
    
    // Read current message data
    uint32_t messageData = 0;
    // snBitStream_ReadBits(ctx, base);  // reads into messageData
    
    // Restore field +28
    clientData[7] = savedField28;
    
    // Modify bit 15 based on flag
    if (flag) {
        messageData |= 0x8000;  // Set bit 15
    } else {
        messageData &= 0xFFFF7FFF;  // Clear bit 15
    }
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 16
    clientData[8] = 16;
    
    // Write modified message data
    // snBitStream_WriteBits(ctx, base);
    
    // Restore field +32
    clientData[8] = savedField32;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteBooleanFlag2 @ 0x822389D0 | size: 0x80
//
// Writes a boolean flag by setting or clearing bit 14 in the message.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   flag - Boolean value to write (true sets bit 14, false clears it)
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_WriteBooleanFlag2(void* client, bool flag)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Call pre-write helper
    // snBitStream_ValidateWrite(ctx, base);
    
    // Save current field +28 value
    uint32_t savedField28 = clientData[7];
    
    // Temporarily set field +28 to 16
    clientData[7] = 16;
    
    // Read current message data
    uint32_t messageData = 0;
    // snBitStream_ReadBits(ctx, base);  // reads into messageData
    
    // Restore field +28
    clientData[7] = savedField28;
    
    // Modify bit 14 based on flag
    if (flag) {
        messageData |= 0x4000;  // Set bit 14
    } else {
        messageData &= 0xFFFFBFFF;  // Clear bit 14
    }
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 16
    clientData[8] = 16;
    
    // Write modified message data
    // snBitStream_WriteBits(ctx, base);
    
    // Restore field +32
    clientData[8] = savedField32;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CheckAnyPlayerSlotOccupied @ 0x821045D0 | size: 0x60
//
// Checks if any of the 5 player slots (0-4) are occupied.
// Each slot is at offset 536 + (slotIndex * 15044).
//
// Returns:
//   true if at least one slot is occupied, false if all slots are empty
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_CheckAnyPlayerSlotOccupied(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Check flag at offset 75772 (0x1287C = 1*65536 + 10220)
    uint32_t globalFlag = clientData[75772 / 4];
    if (globalFlag != 0) {
        return true;
    }
    
    // Check each of the 5 player slots
    for (int slotIndex = 0; slotIndex <= 4; slotIndex++) {
        uint32_t slotOffset = 536 + (slotIndex * 15044);
        uint32_t slotValue = clientData[slotOffset / 4];
        
        if (slotValue != 0) {
            return true;
        }
    }
    
    return false;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteUInt16WithBufferSize @ 0x821D9AD8 | size: 0x68
//
// Writes a 16-bit value to the network message, then writes the buffer size
// divided by 8 as a 16-bit value.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   value - 16-bit value to write
//
// Returns:
//   The value that was written
// ─────────────────────────────────────────────────────────────────────────────
uint16_t SinglesNetworkClient_WriteUInt16WithBufferSize(void* client, uint16_t value)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Call pre-write helper
    // snBitStream_ValidateWrite(client, base);
    
    // Write the 16-bit value
    // snBitStream_WriteField(client, value, 16);
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Call pre-write helper again
    // snBitStream_ValidateWrite(client, base);
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 32
    clientData[8] = 32;
    
    // Write buffer size divided by 8 as 16-bit value
    uint16_t bufferSizeDiv8 = (uint16_t)((bufferSize + 7) / 8);
    // snBitStream_WriteBits(client, bufferSizeDiv8, 16);
    
    // Restore field +32
    clientData[8] = savedField32;
    
    return value;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ValidatePlayerStates @ 0x8218A3E8 | size: 0x50
//
// Validates that both player state objects at offsets +116 and +120 have
// their field at offset +472 set to non-zero. If validation fails, logs
// an error message.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   0 if validation passes, undefined if validation fails
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient_ValidatePlayerStates(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get player state 1 at offset +116
    uint32_t* playerState1 = (uint32_t*)clientData[116 / 4];
    if (playerState1 == nullptr || playerState1[472 / 4] == 0) {
        return 0;
    }
    
    // Get player state 2 at offset +120
    uint32_t* playerState2 = (uint32_t*)clientData[120 / 4];
    if (playerState2 == nullptr || playerState2[472 / 4] == 0) {
        return 0;
    }
    
    // Both states are invalid - log error
    // External error string at 0x82037948
    // rage_DebugLog("error message");  /* UNVERIFIED — string not found in binary */
    
    return 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadVector3 @ 0x821D9B40 | size: 0x78
//
// Reads three 32-bit float values from the network message into a vector.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   outVector - Pointer to output array of 3 floats
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ReadVector3(void* client, float* outVector)
{
    // Read first float (X component)
    // snBitStream_ReadBits(client, &tempBuffer, 32);
    // outVector[0] = tempBuffer;
    
    // Read second float (Y component)
    // snBitStream_ReadBits(client, &tempBuffer, 32);
    // outVector[1] = tempBuffer;
    
    // Read third float (Z component)
    // snBitStream_ReadBits(client, &tempBuffer, 32);
    // outVector[2] = tempBuffer;
    
    // Stub implementation
    outVector[0] = 0.0f;
    outVector[1] = 0.0f;
    outVector[2] = 0.0f;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadUInt64 @ 0x822609F0 | size: 0x7C
//
// Reads a 64-bit unsigned integer from the network message by reading two
// 32-bit values and combining them.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   outValue - Pointer to output 64-bit value
//
// Returns:
//   true if read succeeded, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_ReadUInt64(void* client, uint64_t* outValue)
{
    uint32_t highWord = 0;
    uint32_t lowWord = 0;
    
    // Read high 32 bits
    // bool success1 = snBitStream_ReadBits(client, &highWord, 32);
    bool success1 = false;
    
    if (!success1) {
        *outValue = 0;
        return false;
    }
    
    // Read low 32 bits
    // bool success2 = snBitStream_ReadBits(client, &lowWord, 32);
    bool success2 = false;
    
    if (!success2) {
        *outValue = 0;
        return false;
    }
    
    // Combine into 64-bit value (high word in upper 32 bits)
    *outValue = ((uint64_t)highWord << 32) | lowWord;
    return true;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadString @ 0x82260A70 | size: 0x7C
//
// Reads a null-terminated string from the network buffer.
// Updates the bit position counter based on string length.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   outBuffer - Output buffer for the string
//   maxLength - Maximum length to read (16 bits)
//
// Returns:
//   Number of characters read (including null terminator)
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient_ReadString(void* client, char* outBuffer, int maxLength)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Save current bit position from field +28
    uint32_t savedBitPos = clientData[7];
    
    // Read string data
    // int bytesRead = util_0AF0(client, outBuffer, maxLength);
    int bytesRead = 0;
    
    if (bytesRead > 0) {
        // Null-terminate the string
        outBuffer[bytesRead] = '\0';
        
        // Update bit position: add 8 bits per character
        clientData[7] = savedBitPos + 8;
        
        // Count total characters including null terminator
        int charCount = 1;
        for (char* p = outBuffer; *p != '\0'; p++) {
            charCount++;
            clientData[7] += 8;
        }
        
        return charCount;
    }
    
    // Read failed - write null terminator
    outBuffer[0] = '\0';
    return 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetPlayerSlotIndex @ 0x820CE380 | size: 0x6C
//
// Maps a player ID (stored at offset +120) to a slot index (0-4) by comparing
// against player IDs stored at offsets +100, +104, +108, +112, +116.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   Slot index (0-4) if player ID matches, -1 if not found
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient_GetPlayerSlotIndex(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get current player ID from offset +120
    uint32_t currentPlayerId = clientData[120 / 4];
    
    // Check slot 0 at offset +100
    if (currentPlayerId == clientData[100 / 4]) {
        return 0;
    }
    
    // Check slot 1 at offset +104
    if (currentPlayerId == clientData[104 / 4]) {
        return 1;
    }
    
    // Check slot 2 at offset +108
    if (currentPlayerId == clientData[108 / 4]) {
        return 2;
    }
    
    // Check slot 3 at offset +112
    if (currentPlayerId == clientData[112 / 4]) {
        return 3;
    }
    
    // Check slot 4 at offset +116
    if (currentPlayerId == clientData[116 / 4]) {
        return 4;
    }
    
    // Player ID not found in any slot
    return -1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::AllocateNetworkEvent @ 0x82243DA0 | size: 0x80
//
// Allocates and initializes a network event object from a pool.
// The event object is allocated at a calculated offset based on frame timing.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   outEventPtr - Pointer to receive the allocated event object pointer
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_AllocateNetworkEvent(void* client, void** outEventPtr)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get timestamp from outEventPtr[0]
    uint32_t* eventData = (uint32_t*)*outEventPtr;
    uint32_t timestamp = eventData[0];
    
    if (timestamp == 0) {
        return;
    }
    
    // Calculate pool index based on timing
    uint32_t baseTime = clientData[1];  // field +4
    uint32_t frameInterval = clientData[76 / 4];  // field +76
    uint32_t timeDelta = timestamp - baseTime;
    uint32_t poolIndex = timeDelta / frameInterval;
    
    // Get event object from pool at calculated offset
    uint32_t poolSlotOffset = (poolIndex + 2) * 4;
    uint32_t* poolSlot = (uint32_t*)((char*)client + poolSlotOffset);
    uint32_t* eventObject = (uint32_t*)(*poolSlot + timestamp);
    
    if (eventObject == nullptr) {
        return;
    }
    
    // Initialize event object
    // Set vtable pointer (external vtable at calculated address)
    // eventObject[0] = vtableAddress;
    
    // Initialize event fields
    // snListNode_Init(eventObject + 2);  // Initialize at offset +8
    // snSession_InitNode(eventObject + 3);  // Initialize at offset +12
    
    *outEventPtr = eventObject + 2;  // Return pointer to offset +8
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteUInt64 @ 0x82260578 | size: 0x78
//
// Writes a 64-bit unsigned integer to the network message by writing two
// 32-bit values (low word first, then high word).
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   value - 64-bit value to write
//
// Returns:
//   true if write succeeded, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_WriteUInt64(void* client, uint64_t value)
{
    // Extract low and high 32-bit words
    uint32_t lowWord = (uint32_t)(value & 0xFFFFFFFF);
    uint32_t highWord = (uint32_t)(value >> 32);
    
    // Write low 32 bits
    // bool success1 = snBitStream_WriteBits(client, lowWord, 32);
    bool success1 = false;
    
    if (!success1) {
        return false;
    }
    
    // Write high 32 bits
    // bool success2 = snBitStream_WriteBits(client, highWord, 32);
    bool success2 = false;
    
    return success2;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteBooleanFlag @ 0x820D6918 | size: 0x78
//
// Writes a boolean value as a 1-bit flag to the network message.
// Also writes the buffer size divided by 8 as a 16-bit value.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   flag - Boolean value to write
//
// Returns:
//   Result of the write operation
// ─────────────────────────────────────────────────────────────────────────────
uint8_t SinglesNetworkClient_WriteBooleanFlag(void* client, bool flag)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Call pre-write helper
    // snBitStream_ValidateWrite(client, base);
    
    // Write boolean as 1-bit value
    uint8_t bitValue = flag ? 1 : 0;
    // uint8_t result = snBitStream_WriteBits(client, bitValue, 1);
    uint8_t result = 0;
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Call pre-write helper again
    // snBitStream_ValidateWrite(client, base);
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 32
    clientData[8] = 32;
    
    // Write buffer size divided by 8 as 16-bit value
    uint16_t bufferSizeDiv8 = (uint16_t)((bufferSize + 7) / 8);
    // snBitStream_WriteBits(client, bufferSizeDiv8, 16);
    
    // Restore field +32
    clientData[8] = savedField32;
    
    return result;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::LinkNode @ 0x82416F20 | size: 0x60
//
// Links a node into a doubly-linked list structure. Updates the vtable pointer
// chain and increments the reference count.
//
// Parameters:
//   source - Source node containing the list head
//   node - Node to link into the list
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_LinkNode(void* source, void* node)
{
    uint32_t* sourceData = (uint32_t*)source;
    uint32_t* nodeData = (uint32_t*)node;
    
    // Initialize the node structure at offset +28
    uint32_t* nodeStruct = (uint32_t*)((char*)node + 28);
    snBitStream_Reset(nodeStruct, nullptr);
    
    // Get the list head from source vtable
    uint32_t* listHead = (uint32_t*)sourceData[0];
    
    if (node != nullptr) {
        // Get the next pointer from list head
        uint32_t* nextNode = (uint32_t*)listHead[3];
        
        // Link node into the list
        nodeData[0] = (uint32_t)nextNode;
        listHead[3] = (uint32_t)node;
        
        // Increment reference count (stored as uint16_t at offset +6)
        uint16_t* refCount = (uint16_t*)((char*)listHead + 6);
        (*refCount)++;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::InitializeWithParent @ 0x82260128 | size: 0x60
//
// Initializes a network client structure by copying the parent pointer and
// zeroing all fields, then calling the initialization helper.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance to initialize
//   parent - Parent structure containing initialization data
//
// Returns:
//   Pointer to initialized client
// ─────────────────────────────────────────────────────────────────────────────
void* SinglesNetworkClient_InitializeWithParent(void* client, void* parent)
{
    uint32_t* clientData = (uint32_t*)client;
    uint32_t* parentData = (uint32_t*)parent;
    
    // Copy parent pointer from offset +36
    clientData[9] = parentData[9];
    
    // Zero out all core fields
    clientData[0] = 0;   // vtable
    clientData[1] = 0;   // flags
    clientData[2] = 0;   // field +8
    clientData[3] = 0;   // field +12
    clientData[4] = 0;   // field +16
    clientData[5] = 0;   // field +20
    clientData[6] = 0;   // field +24
    clientData[7] = 0;   // field +28
    clientData[8] = 0;   // field +32
    
    // Call initialization helper
    snBitStream_Init(client, nullptr);
    
    return client;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetOrCreateSession @ 0x82105CA0 | size: 0x94
//
// Gets an existing session or creates a new one based on game state.
// Checks if the game loop has an active session, and if not, creates one
// with appropriate parameters based on network configuration.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   Pointer to session object (stored at offset +48)
// ─────────────────────────────────────────────────────────────────────────────
void* SinglesNetworkClient_GetOrCreateSession(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Access global game loop object
    extern uint32_t g_loop_obj_ptr;
    uint32_t* loopObj = (uint32_t*)g_loop_obj_ptr;
    uint32_t* loopData = (uint32_t*)loopObj[0];
    
    // Check if loop has an active session at offset +36
    uint32_t activeSession = loopData[9];
    
    if (activeSession == 0) {
        // No active session - check if we already have one
        uint32_t existingSession = clientData[12];
        
        if (existingSession == 0) {
            // Create new session
            snSession_CreateLocal(nullptr, nullptr);
            clientData[12] = 0;  // Store result at offset +48
            return (void*)clientData[12];
        }
    }
    
    // Create session with parameters
    snSession_CreateRemote(nullptr, nullptr);
    uint32_t newSession = 0;  // Result from function call
    clientData[12] = newSession;
    
    // Determine session type based on network configuration
    int sessionType = 1;
    
    // Access network configuration global
    extern uint32_t g_network_config_ptr;
    uint32_t* netConfig = (uint32_t*)g_network_config_ptr;
    
    // Check flag at offset +333
    uint8_t* configFlags = (uint8_t*)netConfig;
    if (configFlags[333] == 0) {
        sessionType = 2;
    }
    
    // Call vtable function at slot 22 (offset +88)
    uint32_t* sessionVtable = (uint32_t*)newSession;
    uint32_t* vtable = (uint32_t*)sessionVtable[0];
    typedef void (*VtableFunc)(uint32_t, void*, int);
    VtableFunc func = (VtableFunc)vtable[22];
    func(newSession, client, sessionType);
    
    return (void*)clientData[12];
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CleanupGlobalArrays @ 0x8221EFB8 | size: 0x80
//
// Cleans up two global arrays used by the network client system.
// First array: 4 elements of 36 bytes each at 0x82619AF0
// Second array: 4 function pointers at 0x825F2E18
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_CleanupGlobalArrays()
{
    // First array: 4 elements of 36 bytes each
    extern uint8_t g_network_array1[144];  // 0x82619AF0
    uint8_t* arrayPtr = g_network_array1;
    
    for (int i = 0; i < 4; i++) {
        snSession_CleanupEntry((void*)arrayPtr, nullptr);
        arrayPtr += 36;
    }
    
    // Second array: 4 function pointers
    extern uint32_t g_network_callbacks[4];  // 0x825F2E18
    uint32_t* callbackPtr = g_network_callbacks;
    
    for (int i = 0; i < 4; i++) {
        uint32_t funcPtr = *callbackPtr;
        if (funcPtr != 0) {
            typedef void (*CallbackFunc)();
            CallbackFunc callback = (CallbackFunc)funcPtr;
            callback();
        }
        callbackPtr++;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetPlayerID @ 0x8225E408 | size: 0x78
//
// Returns the player ID based on flags stored at offset +2.
// Extracts a 5-bit value and maps it to player indices.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   errorFlag - If true, logs an error for invalid player IDs
//
// Returns:
//   Player ID: 0 for player 1, 1 for player 2, -1 for invalid
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient_GetPlayerID(void* client, bool errorFlag)
{
    uint8_t* clientData = (uint8_t*)client;
    
    // Read flags from offset +2 and extract lower 5 bits
    uint8_t flags = clientData[2];
    uint8_t playerType = flags & 0x1F;
    
    // Map player type to player ID
    if (playerType == 1) {
        return 0;  // Player 1
    }
    
    if (playerType == 2) {
        return 1;  // Player 2
    }
    
    // Invalid player type
    if (errorFlag) {
        // Log error message
        extern void rage_DebugLog(void*, void*);
        const char* errorMsg = "Invalid player type";
        rage_DebugLog((void*)errorMsg, nullptr);
    }
    
    return -1;  // Invalid player ID
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadSignedValue @ 0x82260E18 | size: 0x94
//
// Reads a signed value from the message buffer and optionally negates it.
// Performs two reads: first checks if value exists, second reads the actual value.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   outValue - Pointer to store the read value
//   bitCount - Number of bits to read (minus 1)
//
// Returns:
//   true if value was read successfully, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_ReadSignedValue(void* client, uint32_t* outValue, int bitCount)
{
    uint32_t* clientData = (uint32_t*)client;
    uint32_t tempValue = 0;
    bool hasValue = false;
    
    // First read: check if value exists (1 bit)
    snBitStream_ReadBits(client, nullptr);
    uint32_t checkResult = 0;  // Result from read
    
    if (checkResult != 0) {
        hasValue = true;
    }
    
    // Check if read was successful
    uint8_t readSuccess = 0;  // Result from first read
    if (readSuccess == 0) {
        return false;
    }
    
    // Second read: get the actual value
    snBitStream_ReadBits(client, nullptr);
    uint32_t actualValue = 0;  // Result from second read
    
    // Check if second read was successful
    uint8_t secondReadSuccess = 0;
    if (secondReadSuccess == 0) {
        return false;
    }
    
    // If value exists, negate it
    if (hasValue) {
        uint32_t originalValue = outValue[0];
        outValue[0] = (uint32_t)(-(int32_t)originalValue);
    }
    
    return true;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ProcessActiveDevices @ 0x822EAC20 | size: 0x64
//
// Iterates through active devices and processes each one that has its active
// flag set. Accesses a global device array and calls cleanup on active devices.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ProcessActiveDevices(void* client)
{
    uint32_t* clientData = (uint32_t*)client;
    
    // Get device count from offset +1024
    int deviceCount = (int)clientData[256];
    
    if (deviceCount <= 0) {
        return;
    }
    
    // Access global device array at 0x82619AF0
    extern uint8_t g_network_array1[144];
    
    // Pointer to device IDs at offset +1028
    uint32_t* deviceIds = &clientData[257];
    
    // Pointer to device flags (offset +28 from array base)
    uint8_t* deviceFlags = g_network_array1 + 28;
    
    for (int i = 0; i < deviceCount; i++) {
        // Check if device is active
        if (deviceFlags[0] != 0) {
            // Get device ID and call cleanup
            uint32_t deviceId = deviceIds[0];
            snSession_ProcessNode((void*)deviceId, nullptr);
        }
        
        // Move to next device
        deviceIds++;
        deviceFlags += 36;  // Each device entry is 36 bytes
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CheckAndSetNetworkFlag @ 0x822EB2A8 | size: 0x78
//
// Checks if network system is initialized and sets a global flag if not already set.
// Returns true if flag was successfully set, false if already set or system not ready.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   true if flag was set, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_CheckAndSetNetworkFlag(void* client)
{
    // Access global game loop object
    extern uint32_t g_loop_obj_ptr;
    uint32_t* loopObj = (uint32_t*)g_loop_obj_ptr;
    uint32_t* loopData = (uint32_t*)loopObj[0];
    
    // Check if network system is initialized at offset +556
    uint32_t networkSystem = loopData[139];
    
    if (networkSystem == 0) {
        return false;
    }
    
    // Access global flag at 0x826065EB
    extern uint8_t g_network_active_flag;
    
    // Check if flag is already set
    if (g_network_active_flag != 0) {
        return false;
    }
    
    // Get network interface at offset +52
    uint32_t* networkInterface = (uint32_t*)((char*)networkSystem + 52);
    
    // Call network initialization function
    pgBase_AcquireRef((void*)networkInterface, nullptr);
    
    // Set the flag
    g_network_active_flag = 1;
    
    return true;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ClearNetworkFlag @ 0x822EB320 | size: 0x60
//
// Clears the network active flag if the network system is initialized.
// Calls a network shutdown function before clearing the flag.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ClearNetworkFlag(void* client)
{
    // Access global game loop object
    extern uint32_t g_loop_obj_ptr;
    uint32_t* loopObj = (uint32_t*)g_loop_obj_ptr;
    uint32_t* loopData = (uint32_t*)loopObj[0];
    
    // Check if network system is initialized at offset +556
    uint32_t networkSystem = loopData[139];
    
    if (networkSystem == 0) {
        return;
    }
    
    // Access global flag at 0x826065EB
    extern uint8_t g_network_active_flag;
    
    // Check if flag is set
    if (g_network_active_flag == 0) {
        return;
    }
    
    // Get network interface at offset +52
    uint32_t* networkInterface = (uint32_t*)((char*)networkSystem + 52);
    
    // Call network shutdown function
    pgBase_ReleaseRef((void*)networkInterface, nullptr);
    
    // Clear the flag
    g_network_active_flag = 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetupNetworkContext @ 0x822FAC68 | size: 0x60
//
// Sets up a network context by checking and setting the network flag, then
// initializing a context structure with the provided value.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   contextValue - Value to store in the context structure
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SetupNetworkContext(void* client, uint32_t contextValue)
{
    // Check and set network flag
    bool flagSet = snSession_AcquireLock(client, nullptr);
    
    // Get or create network context
    snSession_BeginOperation(client, nullptr);
    
    // Find context structure
    const char* contextName = "NetworkContext";
    void* context = snSession_FindProperty(nullptr, (void*)contextName);
    
    if (context != nullptr) {
        uint32_t* contextData = (uint32_t*)context;
        
        // Store context value at offset +0
        contextData[0] = contextValue;
        
        // Store type identifier at offset +4
        contextData[1] = 3;
    }
    
    // If flag was set, clear it
    if (flagSet) {
        snSession_ReleaseLock(client, nullptr);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::FindNodeSlotIndex @ 0x820CE380 | size: 0x6C
//
// Searches a 5-element node array (offsets +100..+116) for the node matching
// the current active node pointer at +120. Returns the 0-based index (0-4)
// of the matching slot, or -1 if no match is found.
//
// Used to determine which session node slot the active node occupies.
// ─────────────────────────────────────────────────────────────────────────────
int SinglesNetworkClient::FindNodeSlotIndex() {
    uint32_t activeNode = *(uint32_t*)((uint8_t*)this + 120);

    if (activeNode == *(uint32_t*)((uint8_t*)this + 100)) {
        return 0;
    }
    if (activeNode == *(uint32_t*)((uint8_t*)this + 104)) {
        return 1;
    }
    if (activeNode == *(uint32_t*)((uint8_t*)this + 108)) {
        return 2;
    }
    if (activeNode == *(uint32_t*)((uint8_t*)this + 112)) {
        return 3;
    }
    if (activeNode == *(uint32_t*)((uint8_t*)this + 116)) {
        return 4;
    }
    return -1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteAlignedBufferSize @ 0x821D9AD8 | size: 0x68
//
// Validates the write buffer, computes the 8-byte-aligned buffer size,
// then writes it as a 16-bit value into the network stream at the
// header position (offset +32 temporarily set to 32).
//
// Parameters:
//   value - The 16-bit value to write into the stream body
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::WriteAlignedBufferSize(uint32_t value) {
    snBitStream_ValidateWrite(this);

    // Write value into stream with 16 bits
    uint32_t result = snBitStream_WriteField(this, value, 16);

    // Compute aligned buffer size: (bufferSize + 7) / 8
    uint32_t bufferSize = *(uint32_t*)((uint8_t*)this + 16);
    uint32_t alignedSize = (int32_t)(bufferSize + 7) >> 3;

    // Save and restore write offset at +32, temporarily setting to 32
    uint32_t savedWriteOffset = *(uint32_t*)((uint8_t*)this + 32);
    *(uint32_t*)((uint8_t*)this + 32) = 32;

    snBitStream_WriteBits(this, alignedSize, 16);

    *(uint32_t*)((uint8_t*)this + 32) = savedWriteOffset;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadVector3FromStream @ 0x821D9B40 | size: 0x78
//
// Reads three consecutive 32-bit float values from the network stream
// and stores them into a destination Vector3 structure.
//
// Parameters:
//   outVec - Pointer to a 3-float destination (x, y, z)
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::ReadVector3FromStream(float* outVec) {
    float temp;

    // Read X component
    snBitStream_ReadBits(this, &temp, 32);
    outVec[0] = temp;

    // Read Y component
    snBitStream_ReadBits(this, &temp, 32);
    outVec[1] = temp;

    // Read Z component
    snBitStream_ReadBits(this, &temp, 32);
    outVec[2] = temp;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ProcessActiveNodes @ 0x822EAC20 | size: 0x64
//
// Iterates over all active node entries (up to m_nodeCount at +1024) and
// calls the ProcessNode handler for each node whose enabled flag is set.
// The node pointers are stored starting at offset +1028, and the enabled
// flags are at a stride of 36 bytes in a global data table at 0x8262001C.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::ProcessActiveNodes() {
    int32_t nodeCount = *(int32_t*)((uint8_t*)this + 1024);
    if (nodeCount <= 0) {
        return;
    }

    uint32_t* nodeArray = (uint32_t*)((uint8_t*)this + 1028);
    uint8_t* enabledFlags = (uint8_t*)0x8262001C;  // global node enabled table, stride=36

    for (int32_t i = 0; i < nodeCount; i++) {
        if (enabledFlags[i * 36] != 0) {
            snSession_ProcessNode(nodeArray[i]);
        }

        // Re-read count each iteration (may be modified by ProcessNode)
        nodeCount = *(int32_t*)((uint8_t*)this + 1024);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ActivateAndNotify @ 0x822EBF20 | size: 0x64
//
// Called by HUD screens (pause, unlocks, training, loading, leaderboard)
// during their vfn_5 activation. Sets the active flag at +84, polls the
// network state, and posts a notification message with value {1, 3}.
// If the poll indicated changes, flushes the notification queue.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::ActivateAndNotify() {
    // Set active flag
    *(uint8_t*)((uint8_t*)this + 84) = 1;

    // Poll network state
    uint8_t pollResult = snSession_AcquireLock(this);

    // Get network interface pointer from offset +92
    void* networkInterface = *(void**)((uint8_t*)this + 92);

    // Find message slot for this notification type
    void* messageSlot = snSession_FindProperty(networkInterface,
        (const char*)0x8205AEF0);  // message type string @ 0x8205AEF0

    if (messageSlot != nullptr) {
        *(uint32_t*)((uint8_t*)messageSlot + 0) = 1;  // value = 1 (active)
        *(uint32_t*)((uint8_t*)messageSlot + 4) = 3;  // type = 3
    }

    // If poll returned nonzero, flush notification queue
    if ((pollResult & 0xFF) != 0) {
        snSession_ReleaseLock(this);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::PostDualSlotMessage @ 0x822EBC10 | size: 0x88
//
// Posts two messages to an event handler via its vtable slot 10. The first
// message carries an event descriptor {0, 7} with a type string pointer.
// The second message carries the provided data values {dataValue, 3} with
// a second type string pointer and an extra parameter.
//
// Parameters:
//   eventHandler - Pointer to the handler (read from *r4)
//   dataValue    - Value to store in the second message slot
//   extraParam   - Additional parameter passed to the second vtable call
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::PostDualSlotMessage(void** eventHandlerPtr,
    uint32_t dataValue, uint32_t extraParam) {
    void* eventHandler = *eventHandlerPtr;

    // Build first message: event descriptor {0, 7}
    uint32_t eventDesc[2] = { 0, 7 };
    const char* eventTypeStr = (const char*)0x8205AED8;  // event type @ 0x8205AED8

    // Call vtable slot 10 with event descriptor
    void** vtable = *(void***)eventHandler;
    typedef void (*VtSlot10Fn)(void*, const char*, uint32_t*, uint32_t);
    VtSlot10Fn postEvent = (VtSlot10Fn)vtable[10];
    postEvent(eventHandler, eventTypeStr, eventDesc, 1);

    // Build second message: data payload {dataValue, 3}
    uint32_t dataPayload[2] = { dataValue, 3 };

    // Call vtable slot 10 again with data payload
    vtable = *(void***)eventHandler;
    postEvent = (VtSlot10Fn)vtable[10];
    postEvent(eventHandler, (const char*)&dataPayload, nullptr, extraParam);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::PostFloatMessage @ 0x822F1408 | size: 0x9C
//
// Posts a float value as an integer message to the network notification
// system. Increments a global message counter, finds the message slot,
// converts the float to int via truncation, and stores {intValue, 3}.
// Decrements the counter on completion (with or without notification flush).
//
// Parameters:
//   floatValue - Float value to convert and post
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::PostFloatMessage(float floatValue) {
    // Poll network state
    uint8_t pollResult = snSession_AcquireLock(this);

    // Increment global message counter
    uint32_t* pMessageCounter = (uint32_t*)0x8271A834;
    uint32_t savedCounter = *pMessageCounter + 1;
    *pMessageCounter = savedCounter;

    // Prepare message
    snSession_BeginOperation(this);

    // Find message slot for float message type
    void* messageSlot = snSession_FindProperty(this,
        (const char*)0x8205B474);  // message type string @ 0x8205B474

    if (messageSlot != nullptr) {
        // Convert float to int and store
        int32_t intValue = (int32_t)floatValue;
        *(int32_t*)((uint8_t*)messageSlot + 0) = intValue;
        *(uint32_t*)((uint8_t*)messageSlot + 4) = 3;  // type = 3
    }

    // Flush or decrement counter
    if ((pollResult & 0xFF) != 0) {
        snSession_ReleaseLock(this);
        *pMessageCounter = *pMessageCounter - 1;
        return;
    }

    *pMessageCounter = savedCounter - 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetModeAndNotify @ 0x822F14A8 | size: 0xAC
//
// Updates the game mode value at offset +3488 if it differs from the
// current value. Creates a snapshot of the current state, stores the
// new mode, posts a boolean message indicating whether the new mode
// is nonzero, then restores state and decrements the global message
// counter.
//
// Parameters:
//   newMode - New game mode value to set
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::SetModeAndNotify(int32_t newMode) {
    int32_t currentMode = *(int32_t*)((uint8_t*)this + 3488);
    if (newMode == currentMode) {
        return;
    }

    // Create state snapshot on stack
    uint8_t stateSnapshot[16];
    snSession_SaveSnapshot(stateSnapshot, this);

    // Store new mode
    *(int32_t*)((uint8_t*)this + 3488) = newMode;

    // Determine boolean value: 1 if newMode was nonzero, 0 otherwise
    uint32_t boolValue = (newMode != 0) ? 1 : 0;

    // Prepare and post message
    snSession_BeginOperation(this);

    void* messageSlot = snSession_FindProperty(this,
        (const char*)0x8205B1D4);  // message type string @ 0x8205B1D4

    if (messageSlot != nullptr) {
        *(uint32_t*)((uint8_t*)messageSlot + 0) = boolValue;
        *(uint32_t*)((uint8_t*)messageSlot + 4) = 3;  // type = 3
    }

    // Check if snapshot requires notification flush
    uint8_t snapshotFlag = stateSnapshot[4];
    if (snapshotFlag != 0) {
        void* snapshotClient = *(void**)&stateSnapshot[0];
        snSession_ReleaseLock(snapshotClient);
    }

    // Decrement global message counter
    uint32_t* pMessageCounter = (uint32_t*)0x8271A834;
    *pMessageCounter = *pMessageCounter - 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::PostIntMessage @ 0x822F2D68 | size: 0x8C
//
// Posts an integer value as a network message. Increments the global message
// counter, finds the message slot, stores {value, 3}, and handles
// notification flushing and counter management.
//
// Parameters:
//   value - Integer value to post
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::PostIntMessage(int32_t value) {
    // Poll network state
    uint8_t pollResult = snSession_AcquireLock(this);

    // Increment global message counter
    uint32_t* pMessageCounter = (uint32_t*)0x8271A834;
    uint32_t savedCounter = *pMessageCounter + 1;
    *pMessageCounter = savedCounter;

    // Prepare message
    snSession_BeginOperation(this);

    // Find message slot
    void* messageSlot = snSession_FindProperty(this,
        (const char*)0x8205BECC);  // message type string @ 0x8205BECC

    if (messageSlot != nullptr) {
        *(int32_t*)((uint8_t*)messageSlot + 0) = value;
        *(uint32_t*)((uint8_t*)messageSlot + 4) = 3;  // type = 3
    }

    // Flush or decrement counter
    if ((pollResult & 0xFF) != 0) {
        snSession_ReleaseLock(this);
        *pMessageCounter = *pMessageCounter - 1;
        return;
    }

    *pMessageCounter = savedCounter - 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::FireStartEvent @ 0x823E6C08 | size: 0xAC
//
// Creates a rage::EvtStart event and attempts to add it to the session's
// event node list. Initializes the event with the EvtStart vtable, then
// calls the session's event allocator (vtable slot 1). If allocation
// succeeds, copies event data into the allocated node and adds it to
// the session node list via snSession_AddNode.
//
// Returns:
//   true if the event was successfully allocated and added, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient::FireStartEvent() {
    // Initialize a stack-local EvtStart event
    uint8_t evtStartData[16];
    snEvent_Init(evtStartData);

    // Access the session at offset +212 from this
    uint8_t* sessionBase = (uint8_t*)this + 212;

    // Set EvtStart vtable pointer in event data
    void* evtStartVtable = (void*)0x820728F0;  // vtable<rage::EvtStart>
    *(void**)&evtStartData[0] = evtStartVtable;

    // Get session event list pointer at sessionBase+4
    void* sessionEventList = *(void**)((uint8_t*)sessionBase + 4);

    // Call vtable slot 1 to allocate an event node (event type=12, flags=0)
    void** vtable = *(void***)sessionEventList;
    typedef void* (*AllocEventFn)(void*, int, int);
    AllocEventFn allocEvent = (AllocEventFn)vtable[1];
    void* eventNode = allocEvent(sessionEventList, 12, 0);

    if (eventNode != nullptr) {
        // Copy event data into the allocated node
        // Store EvtStart vtable at node+0
        *(void**)((uint8_t*)eventNode + 0) = evtStartVtable;

        // Copy the 8-byte event payload (timestamp data from stack)
        uint32_t payloadHi = *(uint32_t*)&evtStartData[4];
        uint32_t payloadLo = *(uint32_t*)&evtStartData[8];
        uint64_t payload = ((uint64_t)payloadHi << 32) | payloadLo;
        *(uint64_t*)((uint8_t*)eventNode + 4) = payload;

        // Add node to the session node list at sessionBase+8
        snSession_AddNode((uint8_t*)sessionBase + 8, eventNode);
        return true;
    }

    return false;
}


// External function declarations for newly decompiled functions
extern void snBitStream_ReadField(void* client, uint32_t value, int bitWidth);
extern void snBitStream_WriteExtended(void* client, uint32_t value, int bitWidth);
extern void snListNode_Init(void* node);
extern void* snSession_LookupProperty(void* context, const char* name);
extern void snSession_GetPropertyInt(void* result);
extern void* snSession_FindProperty(void* context, const char* name);
extern void snSession_ReleaseLock(void* client);
extern void snSession_AcquireLock(void* client);
extern void rage_debugLog(const char* msg);

// Global flag: network debug assertions disabled
extern uint8_t g_networkAssertionsDisabled;  // lbl_826065EB


// -----------------------------------------------------------------------
// SinglesNetworkClient::ReadDataAndWriteAlignedSize @ 0x820D67C8 | size: 0x6C
//
// Reads data from the message buffer using ReadBits, then writes the
// byte-aligned buffer size as a 16-bit value at write offset 32.
// Used by Serialise methods of network messages.
//
// Parameters:
//   client   - Pointer to message buffer
//   value    - Data value to read
//   bitWidth - Number of bits to read
// -----------------------------------------------------------------------
void SinglesNetworkClient_ReadDataAndWriteAlignedSize(void* client, uint32_t value, int bitWidth)
{
    uint32_t* data = (uint32_t*)client;

    // Validate buffer state
    snBitStream_ValidateWrite(client, nullptr);

    // Read bits from the buffer
    snBitStream_ReadField(client, value, bitWidth);

    // Compute byte-aligned size: (bufferBits + 7) / 8
    uint32_t bufferBits = data[4];  // field +16: bit count
    int32_t alignedBytes = (int32_t)(bufferBits + 7) >> 3;

    // Save and restore write offset, writing aligned size at offset 32
    uint32_t savedWriteOffset = data[8];  // field +32
    data[8] = 32;

    snBitStream_WriteBits(client, (void*)(uintptr_t)alignedBytes);

    data[8] = savedWriteOffset;
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::WriteDataAndUpdateAlignedSize @ 0x820D6838 | size: 0x6C
//
// Writes data to the message buffer, then updates the byte-aligned
// buffer size as a 16-bit value at write offset 32.
// Used by Serialise methods of network messages.
//
// Parameters:
//   client   - Pointer to message buffer
//   value    - Data value to write
//   bitWidth - Number of bits to write
// -----------------------------------------------------------------------
void SinglesNetworkClient_WriteDataAndUpdateAlignedSize(void* client, uint32_t value, int bitWidth)
{
    uint32_t* data = (uint32_t*)client;

    // Validate buffer state
    snBitStream_ValidateWrite(client, nullptr);

    // Write bits to the buffer
    snBitStream_WriteBits(client, (void*)(uintptr_t)value);

    // Compute byte-aligned size: (bufferBits + 7) / 8
    uint32_t bufferBits = data[4];  // field +16: bit count
    int32_t alignedBytes = (int32_t)(bufferBits + 7) >> 3;

    // Save and restore write offset, writing aligned size at offset 32
    uint32_t savedWriteOffset = data[8];  // field +32
    data[8] = 32;

    snBitStream_WriteBits(client, (void*)(uintptr_t)alignedBytes);

    data[8] = savedWriteOffset;
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::WriteExtDataAndUpdateAlignedSize @ 0x820D68A8 | size: 0x6C
//
// Writes extended data to the message buffer using atSingleton write,
// then updates the byte-aligned buffer size at write offset 32.
// Used by Serialise methods for extended-format fields (timestamps, etc).
//
// Parameters:
//   client   - Pointer to message buffer
//   value    - Data value to write
//   bitWidth - Number of bits to write
// -----------------------------------------------------------------------
void SinglesNetworkClient_WriteExtDataAndUpdateAlignedSize(void* client, uint32_t value, int bitWidth)
{
    uint32_t* data = (uint32_t*)client;

    // Validate buffer state
    snBitStream_ValidateWrite(client, nullptr);

    // Write extended data to the buffer
    snBitStream_WriteExtended(client, value, bitWidth);

    // Compute byte-aligned size: (bufferBits + 7) / 8
    uint32_t bufferBits = data[4];  // field +16: bit count
    int32_t alignedBytes = (int32_t)(bufferBits + 7) >> 3;

    // Save and restore write offset, writing aligned size at offset 32
    uint32_t savedWriteOffset = data[8];  // field +32
    data[8] = 32;

    snBitStream_WriteBits(client, (void*)(uintptr_t)alignedBytes);

    data[8] = savedWriteOffset;
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::GetSessionContext @ 0x822EB1E8 | size: 0x5C
//
// Returns the session context pointer stored at offset +92.
// Includes a debug assertion that validates the global network state
// before returning, unless assertions are disabled.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//
// Returns:
//   Session context pointer from field +92
// -----------------------------------------------------------------------
void* SinglesNetworkClient_GetSessionContext(void* client)
{
    uint32_t* data = (uint32_t*)client;

    // Check if global network state pointer is valid
    extern uint32_t* g_networkState;  // lis/lwz pattern at 0x8205AB50
    if (g_networkState != nullptr) {
        uint32_t* stateObj = (uint32_t*)g_networkState[139];  // offset +556
        if (stateObj != nullptr && !g_networkAssertionsDisabled) {
            // Fire debug assertion (nop in release)
            rage_debugLog("SinglesNetworkClient::GetSessionContext");
        }
    }

    // Return session context at offset +92
    return (void*)(uintptr_t)data[23];  // +92 = 23 * 4
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::MessageHandlerCtor @ 0x822EDA08 | size: 0x58
//
// Constructor for a message handler node. Initializes the vtable pointer,
// zeros all fields, and sets up two linked list nodes at offsets +12 and +16.
//
// Parameters:
//   handler - Pointer to the MessageHandler to construct
//
// Returns:
//   Pointer to the constructed handler (in r3)
// -----------------------------------------------------------------------
void* SinglesNetworkClient_MessageHandlerCtor(void* handler)
{
    uint32_t* data = (uint32_t*)handler;

    // Set vtable pointer
    extern uint32_t g_MessageHandlerVtable[];  // MessageHandler vtable
    data[0] = (uint32_t)(uintptr_t)g_MessageHandlerVtable;

    // Zero out fields
    data[1] = 0;  // flags (+4)
    data[2] = 0;  // field +8

    // Initialize linked list node at offset +12
    snListNode_Init((void*)((uint8_t*)handler + 12));

    // Initialize linked list node at offset +16
    uint8_t* node2 = (uint8_t*)handler + 16;
    snListNode_Init(node2);

    // Clear both list node pointers and user data
    *(uint32_t*)node2 = 0;            // +16
    *(uint32_t*)((uint8_t*)handler + 12) = 0;  // +12
    data[5] = 0;                      // +20

    return handler;
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::FindAndInvokeNetCallback @ 0x822F54E8 | size: 0x4C
//
// Looks up a network callback by name string. If found, invokes it.
// If not found, restores r3 from the stack frame.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
// -----------------------------------------------------------------------
void* SinglesNetworkClient_FindAndInvokeNetCallback(void* client)
{
    // Get the session context
    void* context = SinglesNetworkClient_GetSessionContext(client);

    // Look up the callback by name
    void* result = snSession_LookupProperty(context, (const char*)0x8205C440);  // string key

    if (result != nullptr) {
        // Found callback - invoke it
        snSession_GetPropertyInt(result);
        return result;
    }

    // Not found - return the original context
    return context;
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::SendDrillScoreMessage @ 0x822FA7B8 | size: 0x60
//
// Sends a drill score value as a network message. Creates a message
// with the given score value and type=3. Called from training drill
// completion handlers (pongTrainingDrill vtable slots).
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   score  - Score value to send
// -----------------------------------------------------------------------
void SinglesNetworkClient_SendDrillScoreMessage(void* client, uint32_t score)
{
    // Begin network transaction
    snSession_AcquireLock(client);
    uint8_t shouldSend = (uint8_t)(uintptr_t)client;  // transaction result in low byte

    // Get session context
    void* context = SinglesNetworkClient_GetSessionContext(client);

    // Create the message
    void* msg = snSession_FindProperty(context, (const char*)0x8205C83C);

    if (msg != nullptr) {
        // Store score value and message type (3)
        uint32_t* msgData = (uint32_t*)msg;
        msgData[0] = score;
        msgData[1] = 3;
    }

    // Send if transaction was started
    if ((shouldSend & 0xFF) != 0) {
        snSession_ReleaseLock(client);
    }
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::SendDrillSuccessMessage @ 0x822FA818 | size: 0x60
//
// Sends a drill success count as a network message. Creates a message
// with the given value and type=3. Called when training drill success
// count is incremented.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   value  - Success count value to send
// -----------------------------------------------------------------------
void SinglesNetworkClient_SendDrillSuccessMessage(void* client, uint32_t value)
{
    // Begin network transaction
    snSession_AcquireLock(client);
    uint8_t shouldSend = (uint8_t)(uintptr_t)client;

    // Get session context
    void* context = SinglesNetworkClient_GetSessionContext(client);

    // Create the message
    void* msg = snSession_FindProperty(context, (const char*)0x8205C848);

    if (msg != nullptr) {
        uint32_t* msgData = (uint32_t*)msg;
        msgData[0] = value;
        msgData[1] = 3;
    }

    // Send if transaction was started
    if ((shouldSend & 0xFF) != 0) {
        snSession_ReleaseLock(client);
    }
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::SendDrillMovementMessage @ 0x822FA8F0 | size: 0x60
//
// Sends a drill movement value as a network message. Reads the value
// from the client's field at offset +184, creates a message with
// type=3. Called from movement drill handlers and HUD updates.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
// -----------------------------------------------------------------------
void SinglesNetworkClient_SendDrillMovementMessage(void* client)
{
    uint32_t* data = (uint32_t*)client;

    // Begin network transaction
    snSession_AcquireLock(client);
    uint8_t shouldSend = (uint8_t)(uintptr_t)client;

    // Get session context
    void* context = SinglesNetworkClient_GetSessionContext(client);

    // Read movement value from field +184
    uint32_t movementValue = data[46];  // +184 = 46 * 4

    // Create the message
    void* msg = snSession_FindProperty(context, (const char*)0x8205C86C);

    if (msg != nullptr) {
        uint32_t* msgData = (uint32_t*)msg;
        msgData[0] = movementValue;
        msgData[1] = 3;
    }

    // Send if transaction was started
    if ((shouldSend & 0xFF) != 0) {
        snSession_ReleaseLock(client);
    }
}


// -----------------------------------------------------------------------
// SinglesNetworkClient::SendDrillChargingMessage @ 0x822FAC08 | size: 0x60
//
// Sends a drill charging value as a network message. Creates a message
// with the given value and type=3. Called from charging drill state
// handlers.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   value  - Charging value to send
// -----------------------------------------------------------------------
void SinglesNetworkClient_SendDrillChargingMessage(void* client, uint32_t value)
{
    // Begin network transaction
    snSession_AcquireLock(client);
    uint8_t shouldSend = (uint8_t)(uintptr_t)client;

    // Get session context
    void* context = SinglesNetworkClient_GetSessionContext(client);

    // Create the message
    void* msg = snSession_FindProperty(context, (const char*)0x8205C898);

    if (msg != nullptr) {
        uint32_t* msgData = (uint32_t*)msg;
        msgData[0] = value;
        msgData[1] = 3;
    }

    // Send if transaction was started
    if ((shouldSend & 0xFF) != 0) {
        snSession_ReleaseLock(client);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CopyPositionVector @ 0x8239DA50 | size: 0x40
//
// Copies a 16-byte aligned position vector from the result of
// snSession_ComputePosition into the caller's storage. Uses
// VMX128 vector load/store for aligned 128-bit copy.
//
// Parameters:
//   this - Destination pointer for the 16-byte vector (aligned)
// ─────────────────────────────────────────────────────────────────────────────
extern void* snSession_ComputePosition(void* stackBuf);

void SinglesNetworkClient_CopyPositionVector(void* self)
{
    // Compute source position into a stack-local buffer
    uint8_t alignedBuf[16] __attribute__((aligned(16)));
    void* srcPtr = snSession_ComputePosition(alignedBuf);

    // Copy 16 bytes (vector register width) from source to self
    memcpy(self, srcPtr, 16);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ValidatePlayerTransition @ 0x82399998 | size: 0x44
//
// Validates whether a player state transition is permitted by checking
// the target state against the current state at offset +992. If states
// differ, calls snSession_FindState on the state buffer at
// offset +996 to search for the target state. Returns true if the
// transition is valid (state matches or found in buffer).
//
// Parameters:
//   this        - Pointer to SinglesNetworkClient instance
//   targetState - State index to validate against
//
// Returns:
//   true if transition is valid, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
extern int snSession_FindState(void* stateBuffer);

bool SinglesNetworkClient_ValidatePlayerTransition(void* self, int targetState)
{
    uint32_t* data = (uint32_t*)self;

    // Check if target matches current state at offset +992
    int currentState = (int)data[992 / 4];
    if (targetState == currentState) {
        return true;
    }

    // Search state buffer at offset +996 for target state
    int result = snSession_FindState((void*)&data[996 / 4]);
    if (result < 0) {
        return false;
    }

    return true;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::IsSessionConnected @ 0x824172D8 | size: 0x4C
//
// Checks whether the network session is in a connected state by
// examining the connection status field at offset +192. Returns true
// if the status is 1 (connecting) or 2 (connected).
//
// Parameters:
//   this - Pointer to SinglesNetworkClient instance
//
// Returns:
//   true if session status is 1 or 2, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_IsSessionConnected(void* self)
{
    uint32_t* data = (uint32_t*)self;

    int status = (int)data[192 / 4];

    if (status == 2) {
        return true;
    }
    if (status == 1) {
        return true;
    }

    return false;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadStreamField16 @ 0x8236A3A0 | size: 0x50
//
// Reads a 16-bit field from the network stream at read cursor position 16.
// Temporarily sets the read cursor (offset +28) to 16, reads via
// snBitStream_ReadBits, then restores the original cursor.
//
// Parameters:
//   this - Pointer to SinglesNetworkClient (network stream)
//
// Returns:
//   The 32-bit value read from the stream at position 16
// ─────────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient_ReadStreamField16(void* self)
{
    uint32_t* data = (uint32_t*)self;

    // Save current read cursor (offset +28)
    uint32_t savedCursor = data[28 / 4];

    // Set cursor to position 16
    data[28 / 4] = 16;

    // Read 16-bit value into stack buffer
    uint32_t result = 0;
    snBitStream_ReadBits(self, &result, 16);

    // Restore original cursor
    data[28 / 4] = savedCursor;

    return result;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::IsSessionInReplayState @ 0x82303348 | size: 0x54
//
// Checks whether the current network session is in the "replay" state
// (state == 4). Looks up the session property via the session state
// pointer at offset +5556 (0x15B4) using the "play" message type key.
// If found, calls snSession_GetPropertyInt to get the current state
// value.
//
// Parameters:
//   this - Pointer to SinglesNetworkClient instance
//
// Returns:
//   true if the session state equals 4 (replay), false otherwise
// ─────────────────────────────────────────────────────────────────────────────
extern void* snSession_LookupProperty(void* sessionState, const char* key);
extern int snSession_GetPropertyInt(void* property);

bool SinglesNetworkClient_IsSessionInReplayState(void* self)
{
    uint32_t* data = (uint32_t*)self;

    // Get session state pointer from offset +5556 (0x15B4)
    void* sessionState = (void*)data[5556 / 4];

    // Look up property with "play" key
    extern const char g_szReplayPropertyKey[];  // @ 0x8205DC90
    void* property = snSession_LookupProperty(sessionState, g_szReplayPropertyKey);

    int stateValue;
    if (property != nullptr) {
        stateValue = snSession_GetPropertyInt(property);
    } else {
        stateValue = 0;
    }

    return (stateValue == 4);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadMatchRoundIndex @ 0x823B4868 | size: 0x54
//
// Reads a 3-bit signed value from the match data stream at cursor
// position 28. Operates on the sub-buffer at offset +2652 within the
// network client, temporarily setting the read cursor and reading
// 3 bits via snBitStream_ReadSigned.
//
// Parameters:
//   this - Pointer to SinglesNetworkClient instance
//
// Returns:
//   3-bit signed value (match round index)
// ─────────────────────────────────────────────────────────────────────────────
int32_t SinglesNetworkClient_ReadMatchRoundIndex(void* self)
{
    uint8_t* base = (uint8_t*)self;
    uint32_t* subStream = (uint32_t*)(base + 2652);

    // Save current read cursor (offset +28 within sub-stream)
    uint32_t savedCursor = subStream[28 / 4];

    // Set cursor to position 28
    subStream[28 / 4] = 28;

    // Read 3-bit signed value
    uint32_t result = 0;
    snBitStream_ReadSigned((void*)subStream, &result, 3);

    // Restore cursor
    subStream[28 / 4] = savedCursor;

    return (int32_t)result;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadMatchSetCount @ 0x823B4F20 | size: 0x54
//
// Reads a 6-bit signed value from the match data stream at cursor
// position 48. Operates on the sub-buffer at offset +2652, temporarily
// setting the read cursor and reading 6 bits via
// snBitStream_ReadSigned.
//
// Parameters:
//   this - Pointer to SinglesNetworkClient instance
//
// Returns:
//   6-bit signed value (match set count)
// ─────────────────────────────────────────────────────────────────────────────
int32_t SinglesNetworkClient_ReadMatchSetCount(void* self)
{
    uint8_t* base = (uint8_t*)self;
    uint32_t* subStream = (uint32_t*)(base + 2652);

    // Save current read cursor (offset +28 within sub-stream)
    uint32_t savedCursor = subStream[28 / 4];

    // Set cursor to position 48
    subStream[28 / 4] = 48;

    // Read 6-bit signed value
    uint32_t result = 0;
    snBitStream_ReadSigned((void*)subStream, &result, 6);

    // Restore cursor
    subStream[28 / 4] = savedCursor;

    return (int32_t)result;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::VsnNotifyHandlerCtor @ 0x822F9760 | size: 0x58
//
// Constructor for a rage::VsnSession notify handler. Sets the vtable for
// rage::VsnNotifyBase (at 0x8205C7A4), zeros state fields, initializes
// two linked-list nodes via snListNode_Init, clears node pointers and the
// pending-notification field.
//
// Parameters:
//   this - Pointer to VsnNotifyHandler instance to construct
//
// Returns:
//   Initialized handler (returned in r3)
// ─────────────────────────────────────────────────────────────────────────────
// rage::VsnNotifyBase vtable @ 0x8205C7A4
extern uint32_t g_VsnNotifyHandlerVtable;

void SinglesNetworkClient_VsnNotifyHandlerCtor(void* self)
{
    uint32_t* data = (uint32_t*)self;

    // Set vtable pointer
    data[0] = (uint32_t)&g_VsnNotifyHandlerVtable;

    // Zero state fields
    data[1] = 0;  // offset +4
    data[2] = 0;  // offset +8

    // Initialize linked-list node at offset +12
    snListNode_Init((void*)&data[12 / 4]);

    // Initialize linked-list node at offset +16
    snListNode_Init((void*)&data[16 / 4]);

    // Clear node data
    data[16 / 4] = 0;
    data[12 / 4] = 0;

    // Clear pending notification field
    data[20 / 4] = 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetCoordinatorRoundValue @ 0x82300E68 | size: 0x5C
//
// Retrieves the current round value from the network coordinator.
// Looks up a property from the session state at offset +5556 using
// the "t" key string. If found, calls snSession_GetPropertyValue
// to extract the value; otherwise returns 0.
//
// Parameters:
//   this - Pointer to SinglesNetworkClient instance
//
// Returns:
//   Current round value, or 0 if property not found
// ─────────────────────────────────────────────────────────────────────────────
extern void* snSession_GetPropertyValue(void* property);

uint32_t SinglesNetworkClient_GetCoordinatorRoundValue(void* self)
{
    uint32_t* data = (uint32_t*)self;

    // Get session state from offset +5556 (0x15B4)
    void* sessionState = (void*)data[5556 / 4];

    // Look up property with "t" key
    extern const char g_szRoundPropertyKey[];  // @ 0x8205D63C
    void* property = snSession_LookupProperty(sessionState, g_szRoundPropertyKey);

    if (property != nullptr) {
        return (uint32_t)(uintptr_t)snSession_GetPropertyValue(property);
    }

    return 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::LookupSessionPropertyValue @ 0x823F9808 | size: 0x5C
//
// Looks up a session property by key and stores its integer value.
// Takes the session state and key, plus a pointer where the result
// should be stored. If found, extracts the value via
// snSession_GetPropertyInt, stores it, and returns true.
//
// Parameters:
//   sessionState - Session state pointer
//   key          - Property key string
//   outValue     - Pointer to store the result
//
// Returns:
//   true if property was found and value stored, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_LookupSessionPropertyValue(void* sessionState, const char* key, uint32_t* outValue)
{
    // Look up property
    void* property = snSession_LookupProperty(sessionState, key);

    if (property != nullptr) {
        // Get property value and store it
        int value = snSession_GetPropertyInt(property);
        *outValue = (uint32_t)value;
        return true;
    }

    return false;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::InitMessageHandler @ 0x822EDA08 | size: 0x58
//
// Constructor for rage::snConnectionManager::MessageHandler.
// Sets the vtable pointer, zeroes control fields, and initializes two
// linked-list node sub-objects at offsets +12 and +16.
//
// Parameters:
//   handler - Pointer to MessageHandler instance to initialize
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_InitMessageHandler(void* handler)
{
    // Vtable for rage::snConnectionManager::MessageHandler @ 0x8205C750
    extern uint32_t g_snConnectionManagerMsgHandlerVtable;

    uint32_t* h = (uint32_t*)handler;

    // Set vtable pointer
    h[0] = (uint32_t)&g_snConnectionManagerMsgHandlerVtable;

    // Zero flags and state fields
    h[1] = 0;  // +4
    h[2] = 0;  // +8

    // Initialize linked-list nodes at +12 and +16
    uint8_t* nodeA = (uint8_t*)handler + 12;
    uint8_t* nodeB = (uint8_t*)handler + 16;

    snListNode_Init(nodeA);
    snListNode_Init(nodeB);

    // Zero node pointers after init
    *(uint32_t*)nodeB = 0;
    *(uint32_t*)nodeA = 0;

    // Zero session reference at +20
    h[5] = 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ActivateSessionContext @ 0x822EBF20 | size: 0x64
//
// Sets the active session flag, acquires the network lock, looks up a
// context property by name, writes initial values (enabled=1, type=3),
// and releases the lock if it was acquired.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ActivateSessionContext(void* client)
{
    uint8_t* c = (uint8_t*)client;

    // Set active flag at offset +84
    c[84] = 1;

    // Acquire network lock
    bool lockAcquired = SinglesNetworkClient_CheckAndSetNetworkFlag(client);

    // Get session pointer at offset +92
    void* session = *(void**)(c + 92);

    // Look up context property by name
    extern const char g_szActivatePropertyKey[];
    void* context = snSession_FindProperty(session, g_szActivatePropertyKey);

    if (context != nullptr) {
        uint32_t* contextData = (uint32_t*)context;
        contextData[0] = 1;  // enabled
        contextData[1] = 3;  // type identifier
    }

    // Release lock if it was acquired
    if (lockAcquired) {
        SinglesNetworkClient_ClearNetworkFlag(client);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetSessionProperties @ 0x822EBC10 | size: 0x88
//
// Sets two session context properties via virtual dispatch (vtable slot 10).
// The first property is set with type=7 and enabled=1, using a data name
// string. The second property is set with type=3 and the provided callback.
//
// Parameters:
//   client   - Unused (context object loaded from args)
//   args     - Pointer to argument struct; args[0] = session object
//   dataPtr  - Data pointer for the second property
//   callback - Callback pointer for the second property
//   vtable   - Vtable pointer for virtual dispatch
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SetSessionProperties(void* client, void* args, uint32_t dataPtr,
                                                uint32_t callback, void* vtable)
{
    // Load session from args[0]
    uint32_t* argArray = (uint32_t*)args;
    void* session = (void*)argArray[0];

    // String at 0x8205AED8
    extern const char g_szEventDescPropertyKey[];

    // Build first property: {value=0, type=7, name=g_szEventDescPropertyKey, enabled=1}
    struct {
        uint32_t value;
        uint32_t type;
        const char* name;
        uint32_t enabled;
    } prop1 = {0, 7, g_szEventDescPropertyKey, 1};

    // Call vtable slot 10 to set the property
    typedef void (*SetPropertyFn)(void*, void*, void*);
    uint32_t* vt = *(uint32_t**)session;
    SetPropertyFn setProperty = (SetPropertyFn)vt[10];
    setProperty(session, (void*)&prop1, (void*)&g_szEventDescPropertyKey);

    // Build second property: {value=dataPtr, type=3}
    struct {
        uint32_t value;
        uint32_t type;
    } prop2 = {dataPtr, 3};

    // Call vtable slot 10 again with callback
    setProperty(session, (void*)&prop2, (void*)(uintptr_t)callback);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::RelocateAndInitEntries @ 0x820E7350 | size: 0x8C
//
// Relocates a vtable pointer using a relocation table, then iterates
// over an array of 16-byte entries and initializes each one via snListNode_Init.
// Used during deserialization to fix up pointers after loading.
//
// Parameters:
//   obj      - Object with vtable at +0 and entry count at +4
//   relocTbl - Relocation table for pointer fixup
//
// Returns:
//   Pointer to the object (for chaining)
// ─────────────────────────────────────────────────────────────────────────────
void* SinglesNetworkClient_RelocateAndInitEntries(void* obj, void* relocTbl)
{
    uint32_t* o = (uint32_t*)obj;
    uint32_t* tbl = (uint32_t*)relocTbl;

    // Relocate vtable pointer if non-null
    uint32_t vtablePtr = o[0];
    if (vtablePtr != 0) {
        // Compute index into relocation table
        uint32_t baseAddr = tbl[1];
        uint32_t entrySize = tbl[19];  // offset +76
        uint32_t index = (vtablePtr - baseAddr) / entrySize;

        // Apply relocation delta from table entry at (index+2)*4
        uint32_t delta = tbl[index + 2];
        o[0] = vtablePtr + delta;
    }

    // Initialize each 16-byte entry
    uint16_t entryCount = *(uint16_t*)((uint8_t*)obj + 4);
    for (int i = 0; i < entryCount; i++) {
        uint8_t* entry = (uint8_t*)o[0] + (i * 16);
        if (entry != nullptr) {
            snListNode_Init(entry);
        }
    }

    return obj;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::CheckStreamAlignment @ 0x82238A50 | size: 0x8C
//
// Checks whether the bit stream has enough data for a 48-bit aligned read.
// If so, reads 16 bits from a temporary position and tests bit 14
// to determine the alignment status.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient bit stream instance
//
// Returns:
//   true if bit 14 is set in the alignment marker, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_CheckStreamAlignment(void* client)
{
    uint32_t* c = (uint32_t*)client;
    uint32_t tempBuf = 0;

    // Get current bit position at offset +16
    uint32_t bitPos = c[4];

    // Align to 8-byte boundary: (bitPos + 7) & ~7
    uint32_t aligned = (bitPos + 7) & ~7u;

    // Need at least 48 bits of aligned data
    if (aligned < 48) {
        return false;
    }

    // Save and temporarily override read position at offset +28
    uint32_t savedPos = c[7];
    c[7] = 16;

    // Read 16 bits into temp buffer
    snBitStream_ReadBits(client, &tempBuf, 16);

    // Restore original position
    c[7] = savedPos;

    // Test bit 14 (0x4000)
    return (tempBuf & 0x4000) != 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::TryDestroyInactiveSession @ 0x822EE268 | size: 0x84
//
// Checks multiple conditions before destroying an inactive session object.
// Guards: event must be zero, match state must be 7, active flag must be set,
// game manager must exist, and the session must be valid with a destructor.
// Called via indirect dispatch (likely a timer/polling callback).
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   event  - Event data pointer (must be null/zero to proceed)
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_TryDestroyInactiveSession(void* client, void* event)
{
    uint8_t* cb = (uint8_t*)client;

    // Event value must be zero
    uint32_t eventVal = *(uint32_t*)event;
    if (eventVal != 0) return;

    // Match state at offset +3484 must be 7 (completed)
    int matchState = *(int32_t*)(cb + 3484);
    if (matchState != 7) return;

    // Active flag at offset +208 must be set
    uint8_t activeFlag = cb[208];
    if (activeFlag == 0) return;

    // Check if game manager exists
    extern uint32_t g_pGameManagerBase;
    uint32_t* managerRef = (uint32_t*)((uint8_t*)&g_pGameManagerBase + 4);
    if (*managerRef == 0) return;

    // Check global session manager
    extern uint32_t g_pSnSessionManager;
    void* sessionMgr = *(void**)&g_pSnSessionManager;
    if (sessionMgr == nullptr) return;

    // Check active flag at offset +112 of session manager
    uint8_t* smgr = (uint8_t*)sessionMgr;
    if (smgr[112] == 0) return;

    // Get session object at offset +100
    void* session = *(void**)(smgr + 100);
    if (session == nullptr) return;

    // Call virtual destructor (vtable slot 0)
    typedef void (*DestructorFn)(void*, int);
    uint32_t* vt = *(uint32_t**)session;
    DestructorFn destroy = (DestructorFn)vt[0];
    destroy(session, 0);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetFloatContextProperty @ 0x822F1408 | size: 0x9C
//
// Acquires the network lock, increments a global recursion counter,
// begins a network operation, looks up a named context property, stores
// a float value (converted to int) with type=3, then releases the lock
// and decrements the counter.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   value  - Float value to store in the context property
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SetFloatContextProperty(void* client, float value)
{
    // Acquire network lock
    bool lockAcquired = SinglesNetworkClient_CheckAndSetNetworkFlag(client);

    // Increment global recursion counter
    extern uint32_t g_networkRecursionCounter;
    uint32_t savedCounter = g_networkRecursionCounter + 1;
    g_networkRecursionCounter = savedCounter;

    // Begin network operation
    snSession_BeginOperation(client);

    // Look up context property by name
    extern const char g_szFloatPropertyKey[];
    void* context = snSession_FindProperty(nullptr, g_szFloatPropertyKey);

    if (context != nullptr) {
        uint32_t* contextData = (uint32_t*)context;
        // Convert float to integer and store
        contextData[0] = (int32_t)value;
        contextData[1] = 3;  // type identifier
    }

    // Release lock and decrement counter
    if (lockAcquired) {
        SinglesNetworkClient_ClearNetworkFlag(client);
        g_networkRecursionCounter--;
        return;
    }

    // Decrement counter (lock wasn't held)
    g_networkRecursionCounter = savedCounter - 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetIntContextProperty @ 0x822F2D68 | size: 0x8C
//
// Acquires the network lock, increments a global recursion counter,
// begins a network operation, looks up a named context property, stores
// an integer value with type=3, then releases the lock and decrements
// the counter.
//
// Parameters:
//   client - Pointer to SinglesNetworkClient instance
//   value  - Integer value to store in the context property
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SetIntContextProperty(void* client, uint32_t value)
{
    // Acquire network lock
    bool lockAcquired = SinglesNetworkClient_CheckAndSetNetworkFlag(client);

    // Increment global recursion counter
    extern uint32_t g_networkRecursionCounter;
    uint32_t savedCounter = g_networkRecursionCounter + 1;
    g_networkRecursionCounter = savedCounter;

    // Begin network operation
    snSession_BeginOperation(client);

    // Look up context property by name
    extern const char g_szIntPropertyKey[];
    void* context = snSession_FindProperty(nullptr, g_szIntPropertyKey);

    if (context != nullptr) {
        uint32_t* contextData = (uint32_t*)context;
        contextData[0] = value;
        contextData[1] = 3;  // type identifier
    }

    // Release lock and decrement counter
    if (lockAcquired) {
        SinglesNetworkClient_ClearNetworkFlag(client);
        g_networkRecursionCounter--;
        return;
    }

    // Decrement counter (lock wasn't held)
    g_networkRecursionCounter = savedCounter - 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::HandleSessionStateTransition @ 0x82105D38 | size: 0xB8
//
// Handles state transitions for the match session. For states 5 and 6,
// resets score data and transitions to a new session phase. State 5 sets
// an invalid marker (-1) on the session score. State 6 conditionally
// resets based on a timing check against a threshold value.
// States 2 and 3 are terminal (no-op return).
//
// Parameters:
//   session - Pointer to match session state structure
// ─────────────────────────────────────────────────────────────────────────────
extern void snSession_GetTimingValue(void* player);
extern void snSession_TransitionPhase(void* sessionData, int newState);

void SinglesNetworkClient_HandleSessionStateTransition(void* session)
{
    uint8_t* sb = (uint8_t*)session;

    // Get current state from offset +12
    int32_t state = *(int32_t*)(sb + 12);

    // States 2 and 3 are terminal
    if (state == 2 || state == 3) {
        return;
    }

    uint8_t* sessionData = *(uint8_t**)(sb + 48);

    if (state == 5) {
        // Set invalid score marker at +4108
        *(int16_t*)(sessionData + 4108) = -1;
    } else if (state == 6) {
        // Look up player from player array
        void* player = nullptr;
        if (state != -1) {
            uint32_t* playerArray = *(uint32_t**)(sb + 8);
            player = (void*)playerArray[state];
        }

        // Get timing value from player
        snSession_GetTimingValue(player);

        // Check against threshold from global tuning data
        extern uint32_t g_pTuningData;
        void* tuning = *(void**)&g_pTuningData;
        float* tuningData = (float*)((uint8_t*)tuning + 4);
        float threshold = *(float*)((uint8_t*)tuningData + 308);

        // Set invalid score marker
        *(int16_t*)(sessionData + 4108) = -1;
    } else {
        return;
    }

    // Reset score counters at +3600 and +3602
    *(uint16_t*)(sessionData + 3600) = 0;
    *(uint16_t*)(sessionData + 3602) = 0;

    // Transition to new session phase (state=2)
    void* activeSessionData = *(void**)(sb + 48);
    snSession_TransitionPhase(activeSessionData, 2);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ValidateFrontendEntry @ 0x821882F0 | size: 0xBC
//
// Validates a frontend context table entry by checking two fields match
// expected values. Computes a table index from the player slot and round,
// then verifies that the stored values at computed offsets match the
// provided expected player ID and expected round ID.
//
// Parameters:
//   table          - Pointer to the frontend context table
//   playerSlot     - Player slot index
//   expectedPlayer - Expected player ID value at the computed entry
//   expectedRound  - Expected round value to check
//   round          - Round index for entry lookup
//
// Returns:
//   true if both fields match, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
extern bool pongFrontendContext_Validate(void* table, int playerSlot, int round);

bool SinglesNetworkClient_ValidateFrontendEntry(void* table, int playerSlot,
                                                 int expectedPlayer, int expectedRound,
                                                 int round)
{
    uint32_t* t = (uint32_t*)table;

    // First validate via frontend context helper
    bool valid = pongFrontendContext_Validate(table, playerSlot, round);

    if (!valid) {
        return false;
    }

    // Compute table index: entry = table[(playerSlot + 33) * 4]
    uint32_t entryIndex = t[playerSlot + 33];

    // Compute slot within entry: slot = entryIndex * 2 + round + 1
    uint32_t slot = entryIndex * 2 + round + 1;

    // Compute final record offset: record = slot * 3, byte offset = record * 8
    uint32_t record = slot + slot * 2;  // slot * 3
    uint32_t byteOffset = record * 8;

    // Check first field (player ID) at the computed offset
    if ((int32_t)t[byteOffset / 4] != expectedPlayer) {
        return false;
    }

    // Recompute for second field check at +28 bytes into the record
    uint32_t entryIndex2 = t[playerSlot + 33];
    uint32_t slot2 = entryIndex2 * 2 + round;
    uint32_t record2 = slot2 + slot2 * 2;  // slot2 * 3
    uint32_t byteOffset2 = record2 * 8;

    // Check second field (round ID) at byteOffset2 + 28
    uint8_t* base = (uint8_t*)table;
    if (*(int32_t*)(base + byteOffset2 + 28) != expectedRound) {
        return false;
    }

    return true;
}


// ── External function declarations for session management ──────────────────
extern void snListNode_Init(void* listNode);
extern uint8_t snSession_AcquireLock(void* client);
extern void* snSession_BeginOperation(void* client);
extern void* snSession_FindProperty(void* clientState, const char* msgType);
extern void snSession_ReleaseLock(void* client);
extern uint32_t snBitStream_GetReadPos(void* client);
extern void snSession_ProcessNode(void* session);
extern void snSession_FindState(void* sessionList, int sessionId);
extern void pgBase_PostEvent(void* pageGroup, int eventCode);
extern void pgBase_PostEventWithParams(void* pageGroup, uint32_t connectionCount, int param1, int param2);
extern void snSession_ResetState(void* sessionData);
extern void snSession_InsertNode(void* sessionList, void* node);

// ── Globals ────────────────────────────────────────────────────────────────
extern uint32_t g_sessionRegistrationCount;  // @ global counter for session registrations
extern void* g_pSessionManager;              // @ 0x8271A358 session manager singleton
extern void* g_pConnectionManager;           // @ 0x8271A39C connection manager
extern void* g_pPageGroupManager;            // @ 0x8271A49C page group manager
extern uint8_t* g_pSessionSlotTable;         // @ 0x8261AB40 session slot table


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::InitSessionNode @ 0x822F9760 | size: 0x58
//
// Initializes a network session node structure. Sets the vtable pointer,
// zeroes all core fields, and detaches the two internal linked-list nodes.
//
// Parameters:
//   self - Pointer to session node structure to initialize
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_InitSessionNode(void* self)
{
    uint32_t* node = (uint32_t*)self;

    // Set vtable pointer (cmRefreshableCtor vtable @ 0x820533CC)
    extern uint32_t g_cmRefreshableNodeVtable;
    node[0] = (uint32_t)&g_cmRefreshableNodeVtable;

    // Zero core state fields
    node[1] = 0;  // flags
    node[2] = 0;  // field +8

    // Detach both internal linked-list nodes at offsets +12 and +16
    snListNode_Init((void*)(node + 3));  // list node at +12
    snListNode_Init((void*)(node + 4));  // list node at +16

    // Clear the list node pointers and status field
    node[4] = 0;  // +16
    node[3] = 0;  // +12
    node[5] = 0;  // +20
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::RegisterSessionEntry @ 0x822F9838 | size: 0x50
//
// Registers a session entry by storing its identifier and incrementing a
// global session registration counter.
//
// Parameters:
//   self  - Pointer to session registration structure
//   entry - Session entry identifier to register
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_RegisterSessionEntry(void* self, uint32_t entry)
{
    uint32_t* regData = (uint32_t*)self;

    // Store entry identifier at offset +0
    regData[0] = entry;

    // Validate the entry through the polling subsystem
    uint8_t result = snSession_AcquireLock((void*)entry);

    // Store validation result as byte at offset +4
    *(uint8_t*)((uint8_t*)self + 4) = (uint8_t)result;

    // Increment global session registration counter
    g_sessionRegistrationCount++;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SendTrainingDrillResult @ 0x822FA7B8 | size: 0x60
//
// Sends a training drill result message through the network client.
// Called by pongTrainingDrill vtable methods (vfn_4, vfn_9) and various
// drill types (serve meter, return, soft shot, spin, focus shot, smash).
//
// Parameters:
//   self  - Pointer to training context
//   value - Drill result value to send
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SendTrainingDrillResult(void* self, uint32_t value)
{
    // Poll current button state
    uint8_t wasActive = snSession_AcquireLock(self);

    // Get the network client instance
    void* client = snSession_BeginOperation(self);

    // Look up the "TrainingDrillResult" message type string (@ 0x8205C83C)
    extern const char g_szDrillResultMsgType[];
    void* msgSlot = snSession_FindProperty(client, g_szDrillResultMsgType);

    if (msgSlot != nullptr) {
        uint32_t* slot = (uint32_t*)msgSlot;
        slot[0] = value;  // store drill result value
        slot[1] = 3;      // priority level
    }

    // If polling was active, re-initiate the connection
    if ((wasActive & 0xFF) != 0) {
        snSession_ReleaseLock(self);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SendDrillMovementState @ 0x822FA8F0 | size: 0x60
//
// Sends the current drill movement state over the network. Reads the
// movement value from the context at offset +184 and posts it as a
// network message with priority 3.
//
// Called by hudTrainingHUD_vfn_5, pongDrillMovement_vfn_4/vfn_5.
//
// Parameters:
//   self - Pointer to drill movement context
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SendDrillMovementState(void* self)
{
    // Poll current button state
    uint8_t wasActive = snSession_AcquireLock(self);

    // Get the network client instance
    void* client = snSession_BeginOperation(self);

    // Read movement value from context at offset +184
    uint32_t movementValue = *(uint32_t*)((uint8_t*)self + 184);

    // Look up the "DrillMovementState" message type string (@ 0x8205C86C)
    extern const char g_szDrillMovementMsgType[];
    void* msgSlot = snSession_FindProperty(client, g_szDrillMovementMsgType);

    if (msgSlot != nullptr) {
        uint32_t* slot = (uint32_t*)msgSlot;
        slot[0] = movementValue;  // store movement state value
        slot[1] = 3;              // priority level
    }

    // If polling was active, re-initiate the connection
    if ((wasActive & 0xFF) != 0) {
        snSession_ReleaseLock(self);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SendDrillChargingState @ 0x822FAC08 | size: 0x60
//
// Sends a drill charging state message through the network client.
// Called by pongDrillCharging_vfn_4 and pongDrillCharging_vfn_26.
//
// Parameters:
//   self  - Pointer to drill charging context
//   value - Charging state value to send
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SendDrillChargingState(void* self, uint32_t value)
{
    // Poll current button state
    uint8_t wasActive = snSession_AcquireLock(self);

    // Get the network client instance
    void* client = snSession_BeginOperation(self);

    // Look up the "DrillChargingState" message type string (@ 0x8205C898)
    extern const char g_szDrillChargingMsgType[];
    void* msgSlot = snSession_FindProperty(client, g_szDrillChargingMsgType);

    if (msgSlot != nullptr) {
        uint32_t* slot = (uint32_t*)msgSlot;
        slot[0] = value;  // store charging state value
        slot[1] = 3;      // priority level
    }

    // If polling was active, re-initiate the connection
    if ((wasActive & 0xFF) != 0) {
        snSession_ReleaseLock(self);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SendTrainingHUDUpdate @ 0x822FAC68 | size: 0x60
//
// Sends a training HUD update message through the network client.
// Called by hudTrainingHUD_vfn_5.
//
// Parameters:
//   self  - Pointer to training HUD context
//   value - HUD update value to send
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_SendTrainingHUDUpdate(void* self, uint32_t value)
{
    // Poll current button state
    uint8_t wasActive = snSession_AcquireLock(self);

    // Get the network client instance
    void* client = snSession_BeginOperation(self);

    // Look up the "TrainingHUDUpdate" message type string (@ 0x8205C8A8)
    extern const char g_szTrainingHUDMsgType[];
    void* msgSlot = snSession_FindProperty(client, g_szTrainingHUDMsgType);

    if (msgSlot != nullptr) {
        uint32_t* slot = (uint32_t*)msgSlot;
        slot[0] = value;  // store HUD update value
        slot[1] = 3;      // priority level
    }

    // If polling was active, re-initiate the connection
    if ((wasActive & 0xFF) != 0) {
        snSession_ReleaseLock(self);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ValidateMessageBounds @ 0x82418CE8 | size: 0x64
//
// Validates that a message's data payload fits within the remaining buffer
// space. Reads a 16-bit data length from offset +2 of the header (byte-
// swapped for endianness), obtains the current read position from the
// message buffer, and checks whether position + dataLength <= totalSize.
//
// Parameters:
//   header  - Pointer to message header (uint16 length at +2)
//   message - Pointer to network message buffer object
//
// Returns:
//   true if message data fits within buffer bounds, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_ValidateMessageBounds(void* header, void* message)
{
    uint8_t* hdr = (uint8_t*)header;

    // Read total buffer size from header at offset +4
    uint32_t totalSize = *(uint32_t*)((uint8_t*)header + 4);

    // Read 16-bit data length from offset +2 and byte-swap (big-endian to host)
    uint16_t rawLen = *(uint16_t*)(hdr + 2);
    uint16_t dataLength = (uint16_t)((rawLen >> 8) | (rawLen << 8));

    // Get current read position from the message buffer
    uint32_t readPos = snBitStream_GetReadPos(message);

    // Compute the data start offset (header is 8 bytes)
    uint32_t dataStart = 8;  // hdr + 8

    // Check: does readPos + totalSize encompass dataStart + dataLength?
    if (totalSize >= dataStart) {
        if ((readPos + totalSize) <= (dataLength + dataStart)) {
            return true;
        }
    }

    return false;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ProcessActiveSessionUpdates @ 0x822EAC20 | size: 0x64
//
// Iterates through all active connection slots and processes pending session
// updates for each connected slot. Checks the session slot table to
// determine which slots are active before processing.
//
// Parameters:
//   self - Pointer to the session manager context
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ProcessActiveSessionUpdates(void* self)
{
    uint32_t* mgr = (uint32_t*)self;

    // Get the number of active connection slots from offset +1024
    int32_t slotCount = (int32_t)mgr[256];  // offset 1024 / 4

    if (slotCount <= 0) {
        return;
    }

    // Iterate through connection slot array starting at offset +1028
    uint32_t* slotArray = &mgr[257];  // offset 1028 / 4

    // Session slot table entry stride is 36 bytes, active flag at offset +28
    uint8_t* slotEntry = g_pSessionSlotTable + 28;

    for (int32_t i = 0; i < slotCount; i++) {
        // Check if this slot is active (non-zero byte in slot table)
        uint8_t isActive = *slotEntry;

        if (isActive != 0) {
            // Process the session update for this slot's connection
            uint32_t sessionHandle = slotArray[i];
            snSession_ProcessNode((void*)sessionHandle);
        }

        // Advance to next slot table entry (stride = 36 bytes)
        slotEntry += 36;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::FindSessionByName @ 0x8236A0F8 | size: 0x80
//
// Searches the session entry list for an entry matching the given name
// string. Performs a byte-by-byte string comparison against each entry's
// name field (stored at offset +80 within each 24-byte-stride entry).
//
// Parameters:
//   self - Pointer to session list context (entry count at +176, entries at +80)
//   name - Null-terminated session name string to search for
//
// Returns:
//   true if a matching session entry was found, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient_FindSessionByName(void* self, const char* name)
{
    uint8_t* ctx = (uint8_t*)self;

    // Get number of entries from offset +176
    int32_t entryCount = *(int32_t*)(ctx + 176);

    if (entryCount <= 0) {
        return false;
    }

    // Entry names start at offset +80, each entry is 24 bytes apart
    const char* entryName = (const char*)(ctx + 80);

    for (int32_t i = 0; i < entryCount; i++) {
        // Byte-by-byte string comparison (strcmp equivalent)
        const char* a = name;
        const char* b = entryName;

        while (*a != '\0') {
            if (*a != *b) {
                break;
            }
            a++;
            b++;
        }

        // Check if strings matched completely
        if ((*a - *b) == 0) {
            return true;
        }

        // Advance to next entry (24-byte stride)
        entryName += 24;
    }

    return false;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::RoutePageGroupEvent @ 0x82329D08 | size: 0x60
//
// Routes a page group event based on the current connection mode. If the
// connection mode indicates a high-priority connection (mode > 1) or the
// connection count exceeds 3, routes directly via pgBase_PostEvent with a forced
// event code of 1. Otherwise routes through pgBase_PostEventWithParams with the full
// connection parameters.
//
// Parameters:
//   self        - Pointer to network context (connection mode at +196)
//   connCount   - Connection count parameter
//   eventParam  - Event parameter to pass through
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_RoutePageGroupEvent(void* self, uint32_t connCount, int eventParam)
{
    uint8_t* ctx = (uint8_t*)self;

    // Read connection mode from offset +196
    int32_t connectionMode = *(int32_t*)(ctx + 196);

    // Load the connection manager singleton and get its connection count
    uint32_t* connMgr = *(uint32_t**)&g_pConnectionManager;
    uint32_t activeConns = connMgr[1];  // connection count at offset +4

    // Determine if we should use the high-priority route
    bool highPriority = false;
    if (connectionMode > 1) {
        highPriority = true;
    } else if (activeConns > 3) {
        highPriority = true;
    }

    if (highPriority) {
        // Route directly with forced event code 1
        pgBase_PostEvent(g_pPageGroupManager, 1);
    } else {
        // Route with full connection parameters
        pgBase_PostEventWithParams(g_pPageGroupManager, activeConns, connCount, eventParam);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetStateId (vfn_4) @ 0x82392310 | size: 0x8
//
// Returns the network state identifier constant (151) for SinglesNetworkClient.
// This identifies the client type in the network state machine.
//
// Returns:
//   State ID constant (151)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int SinglesNetworkClient_GetStateId()
{
    return 151;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::EnablePaused (vfn_12) @ 0x8239AF80 | size: 0x18
//
// Sets the paused flag to true (1) on the match instance accessed through
// the global singles network client pointer at g_pSinglesNetworkClient.
// Writes to byte offset +88 of the match object at slot 3 (+12) of the global.
// ─────────────────────────────────────────────────────────────────────────────
extern uint32_t* g_pSinglesNetworkClient;

extern "C" void SinglesNetworkClient_EnablePaused()
{
    uint32_t* global = (uint32_t*)(uintptr_t)g_pSinglesNetworkClient;
    uint32_t* matchObj = (uint32_t*)(uintptr_t)global[3];
    *(uint8_t*)((uint8_t*)matchObj + 88) = 1;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::DisablePaused (vfn_13) @ 0x8239AF98 | size: 0x18
//
// Clears the paused flag to false (0) on the match instance accessed through
// the global singles network client pointer at g_pSinglesNetworkClient.
// Writes to byte offset +88 of the match object at slot 3 (+12) of the global.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void SinglesNetworkClient_DisablePaused()
{
    uint32_t* global = (uint32_t*)(uintptr_t)g_pSinglesNetworkClient;
    uint32_t* matchObj = (uint32_t*)(uintptr_t)global[3];
    *(uint8_t*)((uint8_t*)matchObj + 88) = 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetElapsedTime (vfn_14) @ 0x8239AFB0 | size: 0x30
//
// Computes the elapsed time by reading an integer tick count from offset +4
// and a float base time from offset +8 of the match object, converting the
// tick count to float, and adding them together.
//
// Returns:
//   Elapsed time as float (basetime + ticks)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" float SinglesNetworkClient_GetElapsedTime()
{
    uint32_t* global = (uint32_t*)(uintptr_t)g_pSinglesNetworkClient;
    uint32_t* matchObj = (uint32_t*)(uintptr_t)global[3];

    int32_t ticks = (int32_t)matchObj[1];            // offset +4
    float baseTime = *(float*)&matchObj[2];           // offset +8

    return baseTime + (float)ticks;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::DelegateToBaseUpdate (vfn_15) @ 0x8239AFE0 | size: 0x1C
//
// Retrieves the match object from the global pointer and dispatches to its
// vtable slot 2 (the base update virtual function). This is a thin forwarding
// wrapper that delegates to the underlying match object's update method.
//
// Parameters:
//   self - Pointer to SinglesNetworkClient instance (this, unused)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void SinglesNetworkClient_DelegateToBaseUpdate(void* self)
{
    uint32_t* global = (uint32_t*)(uintptr_t)g_pSinglesNetworkClient;
    uint32_t* matchObj = (uint32_t*)(uintptr_t)global[3];

    // Tail-call vtable slot 2
    uint32_t* vtable = *(uint32_t**)matchObj;
    typedef void (*VFn2)(void*);
    VFn2 updateFn = (VFn2)(uintptr_t)vtable[2];
    updateFn(matchObj);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::StartMatchTiming (vfn_9) @ 0x823941D0 | size: 0x28
//
// Initializes match timing on the client object (this = r3).
// Reads the current match time from the global timer object (slot 1 at
// offset +332), then sets:
//   - byte at +0x100E (4110) = 1 (timing active flag)
//   - float at +0x1010 (4112) = current match time
//   - byte at +0x100F (4111) = 0 (timing phase reset)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void SinglesNetworkClient_StartMatchTiming(void* self)
{
    uint8_t* data = (uint8_t*)self;
    uint32_t* global = (uint32_t*)(uintptr_t)g_pSinglesNetworkClient;
    uint32_t* timerObj = (uint32_t*)(uintptr_t)global[1];

    float matchTime = *(float*)((uint8_t*)timerObj + 332);

    data[4110] = 1;
    *(float*)(data + 4112) = matchTime;
    data[4111] = 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ForwardMessageIfConnected (vfn_17) @ 0x8239B000 | size: 0x30
//
// Checks the connection state at offset +28 of this object. If the state
// is 2 or 3 (connected/active), forwards the message by calling
// snSession_ForwardMessage with the global pointer and rearranged parameters.
//
// Parameters:
//   self  - Pointer to SinglesNetworkClient (this)
//   param - Message parameter (r4)
//   data  - Message data (r5)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void snSession_ForwardMessage(void* global, int zero, void* param, void* data);

extern "C" void SinglesNetworkClient_ForwardMessageIfConnected(void* self, void* param, void* data)
{
    uint32_t* selfData = (uint32_t*)self;
    int32_t state = (int32_t)selfData[7]; // offset +28

    if (state != 2 && state != 3) {
        return;
    }

    void* global = (void*)(uintptr_t)g_pSinglesNetworkClient;
    snSession_ForwardMessage(global, 0, param, data);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::~SinglesNetworkClient (vfn_0) @ 0x82391F98 | size: 0x50
//
// Destructor for SinglesNetworkClient. Calls the base class destructor
// (snSession_BaseDtor), then conditionally frees memory if the scalar deleting
// destructor flag (bit 0 of param) is set.
//
// Parameters:
//   self  - Pointer to SinglesNetworkClient instance (this)
//   param - Destructor flags (bit 0 = free memory after destruct)
//
// Returns:
//   Pointer to the destroyed object
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void snSession_BaseDtor(void* self);
extern "C" void rage_free(void* ptr);

extern "C" void* SinglesNetworkClient_Destructor(void* self, uint32_t param)
{
    snSession_BaseDtor(self);

    if ((param & 1) != 0) {
        rage_free(self);
    }

    return self;
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::TransformAndDispatchEvent (vfn_16) @ 0x8239B030 | size: 0x50
//
// Copies an 8-byte event structure from the source pointer (r5) into a
// local buffer, transforms it via snEvent_Transform, then dispatches the transformed
// event through the object's own vtable slot 17.
//
// Parameters:
//   self   - Pointer to SinglesNetworkClient instance (this)
//   param  - Event parameter (r4, forwarded to vtable call)
//   source - Pointer to 8-byte source event data (r5)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void snEvent_Transform(void* dest);

extern "C" void SinglesNetworkClient_TransformAndDispatchEvent(void* self, void* param, void* source)
{
    // Copy 8-byte event struct to local buffer
    uint32_t localBuf[2];
    uint32_t* src = (uint32_t*)source;
    localBuf[0] = src[0];
    *(float*)&localBuf[1] = *(float*)&src[1];

    // Transform the event
    snEvent_Transform((void*)localBuf);

    // Dispatch via vtable slot 17 (vfn_17)
    // self->vfn_17(self, param, &localBuf)
    uint32_t* vtable = *(uint32_t**)self;
    typedef void (*VFn17)(void*, void*, void*);
    VFn17 fn = (VFn17)(uintptr_t)vtable[17];
    fn(self, param, (void*)localBuf);
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ResetMatchState (vfn_18) @ 0x8239B080 | size: 0x4C
//
// Resets match state by first calling the scalar deleting destructor
// (vtable slot 1) on the match object from the global pointer, then
// reinitializing the sub-object at offset +784 via snSession_ReinitSubObject.
//
// Parameters:
//   self - Pointer to SinglesNetworkClient instance (this)
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void snSession_ReinitSubObject(void* subObj);

extern "C" void SinglesNetworkClient_ResetMatchState(void* self)
{
    uint32_t* global = (uint32_t*)(uintptr_t)g_pSinglesNetworkClient;
    uint32_t* matchObj = (uint32_t*)(uintptr_t)global[3];

    // Call match object's vtable slot 1 (scalar deleting destructor)
    uint32_t* vtable = *(uint32_t**)matchObj;
    typedef void (*VFn1)(void*);
    VFn1 dtor = (VFn1)(uintptr_t)vtable[1];
    dtor(matchObj);

    // Reinitialize sub-object at offset +784
    snSession_ReinitSubObject((void*)((uint8_t*)self + 784));
}
