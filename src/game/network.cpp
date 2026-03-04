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
