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

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_2()  [vtable slot 2 @ 0x822E5E78]
// Forwards to vfn_4 (tail call optimization in original code)
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_2() {
    // Load vtable and call slot 4 directly
    void** vtable = reinterpret_cast<void**>(this);
    typedef void (*VtableFunc)(fiAsciiTokenizer*);
    VtableFunc func = reinterpret_cast<VtableFunc>(vtable[4]);
    func(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_4()  [vtable slot 4 @ 0x822E5E88]
// Checks if next token is a valid integer (starts with '-' or digit '0'-'9')
// Copies token to internal buffer if valid
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_4() {
    char tokenBuffer[32];
    
    // Call vtable slot 1 to read next token
    void** vtable = reinterpret_cast<void**>(this);
    typedef bool (*ReadTokenFunc)(fiAsciiTokenizer*, char*, int);
    ReadTokenFunc readToken = reinterpret_cast<ReadTokenFunc>(vtable[1]);
    bool hasToken = readToken(this, tokenBuffer, 32);
    
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

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_5()  [vtable slot 5 @ 0x822E5F08]
// Reads next token and parses it as a float
// Returns parsed float value, or 0.0f if token doesn't start with valid float char
// ─────────────────────────────────────────────────────────────────────────────
float fiAsciiTokenizer::vfn_5() {
    char tokenBuffer[32];
    
    // Call vtable slot 1 to read next token
    void** vtable = reinterpret_cast<void**>(this);
    typedef bool (*ReadTokenFunc)(fiAsciiTokenizer*, char*, int);
    ReadTokenFunc readToken = reinterpret_cast<ReadTokenFunc>(vtable[1]);
    bool hasToken = readToken(this, tokenBuffer, 32);
    
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

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_6()  [vtable slot 6 @ 0x822E6190]
// Reads 4 consecutive float tokens into output array (vec4/quaternion)
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_6(float* outVec4) {
    // Call vfn_5 four times through vtable
    void** vtable = reinterpret_cast<void**>(this);
    typedef float (*ParseFloatFunc)(fiAsciiTokenizer*);
    ParseFloatFunc parseFloat = reinterpret_cast<ParseFloatFunc>(vtable[5]);
    
    outVec4[0] = parseFloat(this);
    outVec4[1] = parseFloat(this);
    outVec4[2] = parseFloat(this);
    outVec4[3] = parseFloat(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_7()  [vtable slot 7 @ 0x822E6100]
// Reads 3 consecutive float tokens into output array (vec3/position)
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_7(float* outVec3) {
    // Call vfn_5 three times through vtable
    void** vtable = reinterpret_cast<void**>(this);
    typedef float (*ParseFloatFunc)(fiAsciiTokenizer*);
    ParseFloatFunc parseFloat = reinterpret_cast<ParseFloatFunc>(vtable[5]);
    
    outVec3[0] = parseFloat(this);
    outVec3[1] = parseFloat(this);
    outVec3[2] = parseFloat(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_8()  [vtable slot 8 @ 0x822E6070]
// Reads 4 consecutive float tokens into output array
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_8(float* outArray) {
    // Call vfn_5 four times through vtable
    void** vtable = reinterpret_cast<void**>(this);
    typedef float (*ParseFloatFunc)(fiAsciiTokenizer*);
    ParseFloatFunc parseFloat = reinterpret_cast<ParseFloatFunc>(vtable[5]);
    
    outArray[0] = parseFloat(this);
    outArray[1] = parseFloat(this);
    outArray[2] = parseFloat(this);
    outArray[3] = parseFloat(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_9()  [vtable slot 9 @ 0x822E5FF8]
// Reads 3 consecutive float tokens into output array
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_9(float* outArray) {
    // Call vfn_5 three times through vtable
    void** vtable = reinterpret_cast<void**>(this);
    typedef float (*ParseFloatFunc)(fiAsciiTokenizer*);
    ParseFloatFunc parseFloat = reinterpret_cast<ParseFloatFunc>(vtable[5]);
    
    outArray[0] = parseFloat(this);
    outArray[1] = parseFloat(this);
    outArray[2] = parseFloat(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_10()  [vtable slot 10 @ 0x822E5F98]
// Reads 2 consecutive float tokens into output array (vec2/UV coordinate)
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_10(float* outVec2) {
    // Call vfn_5 twice through vtable
    void** vtable = reinterpret_cast<void**>(this);
    typedef float (*ParseFloatFunc)(fiAsciiTokenizer*);
    ParseFloatFunc parseFloat = reinterpret_cast<ParseFloatFunc>(vtable[5]);
    
    outVec2[0] = parseFloat(this);
    outVec2[1] = parseFloat(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_11()  [vtable slot 11 @ 0x822E6260]
// Reads next token into provided buffer (max 64 bytes)
// Initializes buffer with null terminator before reading
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_11(char* outBuffer) {
    // Initialize buffer
    outBuffer[0] = '\0';
    
    // Call vtable slot 1 to read token
    void** vtable = reinterpret_cast<void**>(this);
    typedef bool (*ReadTokenFunc)(fiAsciiTokenizer*, char*, int);
    ReadTokenFunc readToken = reinterpret_cast<ReadTokenFunc>(vtable[1]);
    readToken(this, outBuffer, 64);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_12()  [vtable slot 12 @ 0x822E62A0]
// Reads next token into buffer and advances to next token
// Combines token read with skip operation
// ─────────────────────────────────────────────────────────────────────────────
void fiAsciiTokenizer::vfn_12(char* outBuffer) {
    // Initialize buffer
    outBuffer[0] = '\0';
    
    // Call vtable slot 1 to read token
    void** vtable = reinterpret_cast<void**>(this);
    typedef bool (*ReadTokenFunc)(fiAsciiTokenizer*, char*, int);
    ReadTokenFunc readToken = reinterpret_cast<ReadTokenFunc>(vtable[1]);
    readToken(this, outBuffer, 64);
    
    // Call vtable slot 4 to skip/advance
    typedef void (*SkipTokenFunc)(fiAsciiTokenizer*);
    SkipTokenFunc skipToken = reinterpret_cast<SkipTokenFunc>(vtable[4]);
    skipToken(this);
}

} // namespace rage
