/**
 * rage_xml.cpp -- XML Node Struct Type Serialization
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements ParseValue/FormatValue virtual methods for rage::xmlNodeStruct
 * primitive type wrappers (Bool, Char, String, Short, Int).
 */

#include "rage_xml.hpp"
#include <cstring>

// External helpers
extern "C" long _xe_strcpyn_10(const char* str);               // strtol(str, NULL, 10) @ 0x82432120
extern void* rage_strDuplicate(const char* str);               // String intern/copy @ 0x820C29E0
extern void sysMemAllocator_Free(void* ptr);                          // sysMemFree @ 0x820C00C0
extern "C" int fiAsciiTokenizer_0030_g(char* buf, const char* fmt, ...); // sprintf @ 0x82430030

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructBool::ParseValue  [vfn_1 / slot 1 @ 0x821A8A38]  68 bytes
// Parses a boolean from a string: 't','T','y','Y','1' => true, else false.
// ─────────────────────────────────────────────────────────────────────────────
bool xmlNodeStructBool::ParseValue(unsigned char* dest, const char* src) {
    char ch = static_cast<char>(src[0]);

    if (ch == 't' || ch == 'T' || ch == 'y' || ch == 'Y' || ch == '1') {
        *dest = 1;
    } else {
        *dest = 0;
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructBool::FormatValue  [vfn_2 / slot 2 @ 0x821A8A80]  24 bytes
// Writes bool as '0'/'1' digit character followed by null terminator.
// ─────────────────────────────────────────────────────────────────────────────
void xmlNodeStructBool::FormatValue(char* dest, const unsigned char* src) {
    dest[0] = static_cast<char>(src[0] + '0');
    dest[1] = '\0';
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructChar::ParseValue  [vfn_1 / slot 1 @ 0x821A8A98]  16 bytes
// Copies a single byte from source string to destination.
// ─────────────────────────────────────────────────────────────────────────────
bool xmlNodeStructChar::ParseValue(unsigned char* dest, const char* src) {
    *dest = static_cast<unsigned char>(src[0]);
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructChar::FormatValue  [vfn_2 / slot 2 @ 0x821A8AA8]  20 bytes
// Writes a single character followed by null terminator.
// ─────────────────────────────────────────────────────────────────────────────
void xmlNodeStructChar::FormatValue(char* dest, const unsigned char* src) {
    dest[0] = static_cast<char>(src[0]);
    dest[1] = '\0';
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructString::ParseValue  [vfn_1 / slot 1 @ 0x821A8AC0]  76 bytes
// Interns the source string (allocates a copy), frees the old string.
// ─────────────────────────────────────────────────────────────────────────────
bool xmlNodeStructString::ParseValue(const char** dest, const char* src) {
    const char* interned = static_cast<const char*>(rage_strDuplicate(src));
    sysMemAllocator_Free(const_cast<char*>(*dest));
    *dest = interned;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructString::FormatValue  [vfn_2 / slot 2 @ 0x821A8B10]  52 bytes
// Copies source string to destination buffer (manual strcpy).
// ─────────────────────────────────────────────────────────────────────────────
void xmlNodeStructString::FormatValue(char* dest, const char* const* src) {
    const char* str = *src;
    if (str != nullptr) {
        while (*str != '\0') {
            *dest++ = *str++;
        }
        *dest = '\0';
    } else {
        dest[0] = '\0';
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructShort::ParseValue  [vfn_1 / slot 1 @ 0x821A8B48]  60 bytes
// Parses a decimal integer string and stores as 16-bit short.
// ─────────────────────────────────────────────────────────────────────────────
bool xmlNodeStructShort::ParseValue(short* dest, const char* src) {
    *dest = static_cast<short>(_xe_strcpyn_10(src));
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructShort::FormatValue  [vfn_2 / slot 2 @ 0x821A8B88]  24 bytes
// Formats a short as a decimal integer string using "%d".
// ─────────────────────────────────────────────────────────────────────────────
void xmlNodeStructShort::FormatValue(char* dest, const short* src) {
    fiAsciiTokenizer_0030_g(dest, "%d", static_cast<int>(*src));
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructInt::ParseValue  [vfn_1 / slot 1 @ 0x821A8BA0]  60 bytes
// Parses a decimal integer string and stores as 32-bit int.
// ─────────────────────────────────────────────────────────────────────────────
bool xmlNodeStructInt::ParseValue(int* dest, const char* src) {
    *dest = static_cast<int>(_xe_strcpyn_10(src));
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructInt::FormatValue  [vfn_2 / slot 2 @ 0x821A8BE0]  20 bytes
// Formats an int as a decimal integer string using "%d".
// ─────────────────────────────────────────────────────────────────────────────
void xmlNodeStructInt::FormatValue(char* dest, const int* src) {
    fiAsciiTokenizer_0030_g(dest, "%d", *src);
}

} // namespace rage
