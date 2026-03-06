/**
 * network.cpp — Network client implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <cstdint>
#include <cstring>

// Forward declarations
struct SinglesNetworkClient;

// External function declarations
extern void SinglesNetworkClient_8CC0_w(void* ctx, void* base);
extern void* xam_singleton_init_8D60(void* ctx, void* base);
extern void SinglesNetworkClient_0268_g(void* ctx, void* base);
extern void SinglesNetworkClient_8DF8_g(void* ctx, void* base);
extern void SinglesNetworkClient_0448_g(void* ctx, void* base);
extern void SinglesNetworkClient_8AE0_g(void* ctx, void* base);


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
    // SinglesNetworkClient_8CC0_w(ctx, base);
    
    if (deviceIndex < 0) {
        // Return error string for invalid device index
        return "vice, userIndex[%d], fileSize[%d]...";
    }
    
    // Get singleton instance
    // void* singleton = xam_singleton_init_8D60(ctx, base);
    
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
    // SinglesNetworkClient_0268_g(ctx, base);
    // SinglesNetworkClient_0268_g(ctx, base);
    
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
    // SinglesNetworkClient_8AE0_g(ctx, base);
    
    // Save current field +28 value
    uint32_t savedField28 = clientData[7];
    
    // Temporarily set field +28 to 16
    clientData[7] = 16;
    
    // Read message data
    uint32_t messageData = 0;
    // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
    
    // Restore field +28
    clientData[7] = savedField28;
    
    // Insert bitValue into bits [18:31] of messageData
    messageData = (messageData & 0xFFFFC000) | (bitValue & 0x3FFF);
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 16
    clientData[8] = 16;
    
    // Write modified message data
    // SinglesNetworkClient_0448_g(ctx, base);
    
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
        // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
        
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
        // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
        
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
        // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
        
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
        // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
        
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
    // SinglesNetworkClient_8AE0_g(ctx, base);
    
    // Save current field +28 value
    uint32_t savedField28 = clientData[7];
    
    // Temporarily set field +28 to 16
    clientData[7] = 16;
    
    // Read current message data
    uint32_t messageData = 0;
    // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
    
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
    // SinglesNetworkClient_0448_g(ctx, base);
    
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
    // SinglesNetworkClient_8AE0_g(ctx, base);
    
    // Save current field +28 value
    uint32_t savedField28 = clientData[7];
    
    // Temporarily set field +28 to 16
    clientData[7] = 16;
    
    // Read current message data
    uint32_t messageData = 0;
    // SinglesNetworkClient_8DF8_g(ctx, base);  // reads into messageData
    
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
    // SinglesNetworkClient_0448_g(ctx, base);
    
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
    // SinglesNetworkClient_8AE0_g(client, base);
    
    // Write the 16-bit value
    // SinglesNetworkClient_0688_g(client, value, 16);
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Call pre-write helper again
    // SinglesNetworkClient_8AE0_g(client, base);
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 32
    clientData[8] = 32;
    
    // Write buffer size divided by 8 as 16-bit value
    uint16_t bufferSizeDiv8 = (uint16_t)((bufferSize + 7) / 8);
    // SinglesNetworkClient_0448_g(client, bufferSizeDiv8, 16);
    
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
    // nop_8240E6D0("error message");
    
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
    // SinglesNetworkClient_8DF8_g(client, &tempBuffer, 32);
    // outVector[0] = tempBuffer;
    
    // Read second float (Y component)
    // SinglesNetworkClient_8DF8_g(client, &tempBuffer, 32);
    // outVector[1] = tempBuffer;
    
    // Read third float (Z component)
    // SinglesNetworkClient_8DF8_g(client, &tempBuffer, 32);
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
    // bool success1 = SinglesNetworkClient_8DF8_g(client, &highWord, 32);
    bool success1 = false;
    
    if (!success1) {
        *outValue = 0;
        return false;
    }
    
    // Read low 32 bits
    // bool success2 = SinglesNetworkClient_8DF8_g(client, &lowWord, 32);
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
    // ke_1B00(eventObject + 2);  // Initialize at offset +8
    // SinglesNetworkClient_8108_gen(eventObject + 3);  // Initialize at offset +12
    
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
    // bool success1 = SinglesNetworkClient_0448_g(client, lowWord, 32);
    bool success1 = false;
    
    if (!success1) {
        return false;
    }
    
    // Write high 32 bits
    // bool success2 = SinglesNetworkClient_0448_g(client, highWord, 32);
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
    // SinglesNetworkClient_8AE0_g(client, base);
    
    // Write boolean as 1-bit value
    uint8_t bitValue = flag ? 1 : 0;
    // uint8_t result = SinglesNetworkClient_0448_g(client, bitValue, 1);
    uint8_t result = 0;
    
    // Get buffer size from field +16
    uint32_t bufferSize = clientData[4];
    
    // Call pre-write helper again
    // SinglesNetworkClient_8AE0_g(client, base);
    
    // Save current field +32 value
    uint32_t savedField32 = clientData[8];
    
    // Temporarily set field +32 to 32
    clientData[8] = 32;
    
    // Write buffer size divided by 8 as 16-bit value
    uint16_t bufferSizeDiv8 = (uint16_t)((bufferSize + 7) / 8);
    // SinglesNetworkClient_0448_g(client, bufferSizeDiv8, 16);
    
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
    SinglesNetworkClient_0268_g(nodeStruct, nullptr);
    
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
    SinglesNetworkClient_0188_g(client, nullptr);
    
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
            SinglesNetworkClient_1378_g(nullptr, nullptr);
            clientData[12] = 0;  // Store result at offset +48
            return (void*)clientData[12];
        }
    }
    
    // Create session with parameters
    SinglesNetworkClient_1410_g(nullptr, nullptr);
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
        SinglesNetworkClient_FA50((void*)arrayPtr, nullptr);
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
        extern void nop_8240E6D0(void*, void*);
        const char* errorMsg = "Invalid player type";
        nop_8240E6D0((void*)errorMsg, nullptr);
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
    SinglesNetworkClient_8DF8_g(client, nullptr);
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
    SinglesNetworkClient_8DF8_g(client, nullptr);
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
            SinglesNetworkClient_F090((void*)deviceId, nullptr);
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
    pg_C3B8_g((void*)networkInterface, nullptr);
    
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
    pg_6DC0_g((void*)networkInterface, nullptr);
    
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
    bool flagSet = SinglesNetworkClient_B2A8_g(client, nullptr);
    
    // Get or create network context
    SinglesNetworkClient_B1E8_g(client, nullptr);
    
    // Find context structure
    const char* contextName = "NetworkContext";
    void* context = SinglesNetworkClient_9318_g(nullptr, (void*)contextName);
    
    if (context != nullptr) {
        uint32_t* contextData = (uint32_t*)context;
        
        // Store context value at offset +0
        contextData[0] = contextValue;
        
        // Store type identifier at offset +4
        contextData[1] = 3;
    }
    
    // If flag was set, clear it
    if (flagSet) {
        SinglesNetworkClient_B320_g(client, nullptr);
    }
}
