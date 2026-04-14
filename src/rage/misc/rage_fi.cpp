/**
 * rage_fi.cpp — File I/O and Tokenizer Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements rage::fiAsciiTokenizer vtable methods for parsing text files.
 */

#include "rage_fi.hpp"
#include <cstring>
#include <cstdlib>

// External CRT function for string copy
extern "C" void _xe_strcpyn_10(char* dest, const char* src);

// External function for float parsing (atof equivalent)
extern "C" double jumptable_2B80(const char* str);

// External CRT function for case-insensitive string comparison
extern "C" int _stricmp(const char* str1, const char* str2);

namespace rage {

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadInt()  [vtable slot 2 @ 0x822E5E78]
// Forwards to SkipToInt (tail call optimization in original code)
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadInt() {
    SkipToInt();  // vtable slot 4
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::SkipToInt()  [vtable slot 4 @ 0x822E5E88]
// Checks if next token is a valid integer (starts with '-' or digit '0'-'9')
// Copies token to internal buffer if valid
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::SkipToInt() {
    char tokenBuffer[32];

    bool hasToken = ReadToken(tokenBuffer, 32);  // vtable slot 1

    if (!hasToken) {
        return;
    }

    char firstChar = tokenBuffer[0];

    // Check if starts with minus sign (negative number)
    if (firstChar == '-') {
        _xe_strcpyn_10(tokenBuffer, tokenBuffer);
        return;
    }

    // Check if starts with digit 0-9
    if (firstChar >= '0' && firstChar <= '9') {
        _xe_strcpyn_10(tokenBuffer, tokenBuffer);
        return;
    }
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadFloat()  [vtable slot 5 @ 0x822E5F08]
// Reads next token and parses it as a float
// Returns parsed float value, or 0.0f if token doesn't start with valid float char
// ---------------------------------------------------------------------------
float fiAsciiTokenizer::ReadFloat() {
    char tokenBuffer[32];

    bool hasToken = ReadToken(tokenBuffer, 32);  // vtable slot 1

    if (!hasToken) {
        return 0.0f;
    }

    char firstChar = tokenBuffer[0];

    // Check if starts with minus sign
    if (firstChar == '-') {
        return static_cast<float>(jumptable_2B80(tokenBuffer));
    }

    // Check if starts with decimal point
    if (firstChar == '.') {
        return static_cast<float>(jumptable_2B80(tokenBuffer));
    }

    // Check if starts with digit 0-9
    if (firstChar >= '0' && firstChar <= '9') {
        return static_cast<float>(jumptable_2B80(tokenBuffer));
    }

    // Invalid float token
    return 0.0f;
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadVec4()  [vtable slot 6 @ 0x822E6190]
// Reads 4 consecutive float tokens into output array (vec4/quaternion)
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadVec4(float* outVec4) {
    outVec4[0] = ReadFloat();  // vtable slot 5
    outVec4[1] = ReadFloat();
    outVec4[2] = ReadFloat();
    outVec4[3] = ReadFloat();
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadVec3()  [vtable slot 7 @ 0x822E6100]
// Reads 3 consecutive float tokens into output array (vec3/position)
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadVec3(float* outVec3) {
    outVec3[0] = ReadFloat();  // vtable slot 5
    outVec3[1] = ReadFloat();
    outVec3[2] = ReadFloat();
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadFloat4()  [vtable slot 8 @ 0x822E6070]
// Reads 4 consecutive float tokens into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadFloat4(float* outArray) {
    outArray[0] = ReadFloat();  // vtable slot 5
    outArray[1] = ReadFloat();
    outArray[2] = ReadFloat();
    outArray[3] = ReadFloat();
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadFloat3()  [vtable slot 9 @ 0x822E5FF8]
// Reads 3 consecutive float tokens into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadFloat3(float* outArray) {
    outArray[0] = ReadFloat();  // vtable slot 5
    outArray[1] = ReadFloat();
    outArray[2] = ReadFloat();
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadVec2()  [vtable slot 10 @ 0x822E5F98]
// Reads 2 consecutive float tokens into output array (vec2/UV coordinate)
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadVec2(float* outVec2) {
    outVec2[0] = ReadFloat();  // vtable slot 5
    outVec2[1] = ReadFloat();
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadString()  [vtable slot 11 @ 0x822E6260]
// Reads next token into provided buffer (max 64 bytes)
// Initializes buffer with null terminator before reading
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadString(char* outBuffer) {
    outBuffer[0] = '\0';
    ReadToken(outBuffer, 64);  // vtable slot 1
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadStringAndAdvance()  [vtable slot 12 @ 0x822E62A0]
// Reads next token into buffer and advances to next token
// Combines token read with skip operation
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadStringAndAdvance(char* outBuffer) {
    outBuffer[0] = '\0';
    ReadToken(outBuffer, 64);  // vtable slot 1
    SkipToInt();               // vtable slot 4
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadTokenAndCompare()  [helper @ 0x822E58A8]
// Reads next token and performs case-insensitive comparison with expected string
// Returns comparison result from _stricmp (0 if equal)
// ---------------------------------------------------------------------------
int fiAsciiTokenizer::ReadTokenAndCompare(const char* expectedToken) {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    bool hasToken = ReadToken(tokenBuffer, 64);  // vtable slot 1

    if (!hasToken) {
        return -1;  // No token available
    }

    // Perform case-insensitive comparison
    return _stricmp(tokenBuffer, expectedToken);
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadNamedFloat()  [vtable slot 13 @ 0x822E6300]
// Reads next token into buffer, then parses and returns a float value
// ---------------------------------------------------------------------------
float fiAsciiTokenizer::ReadNamedFloat() {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    ReadToken(tokenBuffer, 64);  // vtable slot 1
    return ReadFloat();          // vtable slot 5
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadNamedVec4()  [vtable slot 14 @ 0x822E6520]
// Reads next token into buffer, then reads 4 consecutive floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadNamedVec4(float* outArray) {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    ReadToken(tokenBuffer, 64);  // vtable slot 1
    ReadVec4(outArray);          // vtable slot 6
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadNamedVec3()  [vtable slot 15 @ 0x822E64B0]
// Reads next token into buffer, then reads 3 consecutive floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadNamedVec3(float* outArray) {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    ReadToken(tokenBuffer, 64);  // vtable slot 1
    ReadVec3(outArray);          // vtable slot 7
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadNamedFloat4()  [vtable slot 16 @ 0x822E6440]
// Reads next token into buffer, then reads 4 consecutive floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadNamedFloat4(float* outArray) {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    ReadToken(tokenBuffer, 64);  // vtable slot 1
    ReadFloat4(outArray);        // vtable slot 8
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadNamedFloat3()  [vtable slot 17 @ 0x822E63D0]
// Reads next token into buffer, then reads 3 consecutive floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadNamedFloat3(float* outArray) {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    ReadToken(tokenBuffer, 64);  // vtable slot 1
    ReadFloat3(outArray);        // vtable slot 9
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ReadNamedVec2()  [vtable slot 18 @ 0x822E6360]
// Reads next token into buffer, then reads 2 consecutive floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ReadNamedVec2(float* outArray) {
    char tokenBuffer[64];
    tokenBuffer[0] = '\0';

    ReadToken(tokenBuffer, 64);  // vtable slot 1
    ReadVec2(outArray);          // vtable slot 10
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ExpectTokenThenInt()  [vtable slot 19 @ 0x822E6590]
// Reads token with case-insensitive compare, then checks if next token is valid int
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ExpectTokenThenInt(const char* expectedToken) {
    ReadTokenAndCompare(expectedToken);
    SkipToInt();  // vtable slot 4
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ExpectTokenThenFloat()  [vtable slot 20 @ 0x822E65D0]
// Reads token with case-insensitive compare, then parses next token as float
// ---------------------------------------------------------------------------
float fiAsciiTokenizer::ExpectTokenThenFloat(const char* expectedToken) {
    ReadTokenAndCompare(expectedToken);
    return ReadFloat();  // vtable slot 5
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ExpectTokenThenVec4()  [vtable slot 21 @ 0x822E6750]
// Reads token with case-insensitive compare, then reads 4 floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ExpectTokenThenVec4(const char* expectedToken, float* outArray) {
    ReadTokenAndCompare(expectedToken);
    ReadVec4(outArray);  // vtable slot 6
}

// ---------------------------------------------------------------------------
// fiAsciiTokenizer::ExpectTokenThenVec3()  [vtable slot 22 @ 0x822E6700]
// Reads token with case-insensitive compare, then reads 3 floats into output array
// ---------------------------------------------------------------------------
void fiAsciiTokenizer::ExpectTokenThenVec3(const char* expectedToken, float* outArray) {
    ReadTokenAndCompare(expectedToken);
    ReadVec3(outArray);  // vtable slot 7
}

} // namespace rage
