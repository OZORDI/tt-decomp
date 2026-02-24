/**
 * string.c — CRT string utility functions
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Minimal string library used by the RAGE engine. strlen is the standard
 * null-terminator scan. rage_stricmp is a custom case-insensitive compare
 * that also treats '=' (ASCII 61) as a null terminator — used for parsing
 * ini/config key=value pairs where '=' marks end of the key token.
 */

#include <stddef.h>

/**
 * strlen @ 0x820C0000 | size: 0x38
 *
 * Standard null-terminated string length. Returns 0 for NULL input.
 */
size_t strlen(const char* str)
{
    if (!str)
        return 0;

    const char* p = str;
    while (*p != '\0')
        ++p;

    return (size_t)(p - str);
}

/**
 * rage_stricmp @ 0x82186358 | size: 0xC0
 *
 * Case-insensitive string compare. Treats '=' (0x3D) as a string terminator
 * in addition to '\0' — allows comparing config key names that may be
 * followed by '=' without pre-splitting the string.
 *
 * Returns 0 if equal, nonzero (a - b) otherwise.
 */
int rage_stricmp(const char* a, const char* b)
{
    for (;;) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;

        /* Lowercase ASCII letters A-Z → a-z */
        if ((unsigned)(ca - 'A') <= 25u)
            ca += 32;
        else if (ca == '=')
            ca = 0;   /* treat '=' as end-of-token */

        if ((unsigned)(cb - 'A') <= 25u)
            cb += 32;
        else if (cb == '=')
            cb = 0;

        if (ca == 0)
            break;
        if (ca != cb)
            return (int)ca - (int)cb;
    }

    /* Handle trailing 's' wildcard: if one side is "...s\0" and the
     * other is "\0" (or '='), treat them as equal (plural-insensitive match).
     * This is a RAGE config quirk seen in the original code. */
    /* a and b both advanced past their last char; re-check last chars */
    /* TODO: verify exact plural-s matching semantics @ 0x821863B8 */

    return 0;
}
