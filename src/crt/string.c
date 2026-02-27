/**
 * string.c — CRT string utility functions
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Minimal string library used by the RAGE engine. rage_stricmp_6358 is a
 * custom case-insensitive compare that:
 *   1. Treats '=' (ASCII 0x3D) as a string terminator — lets callers pass a
 *      raw "key=value" command-line token and compare just the key portion.
 *   2. Ignores a trailing 's' on either side — "option" and "options" are
 *      considered equal.  This is the RAGE argument/config quirk that lets
 *      both "-cheat" and "-cheats" activate the same feature.
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
 * rage_stricmp_6358 @ 0x82186358 | size: 0xC0
 *
 * Case-insensitive string compare with two RAGE-specific extensions:
 *   - '=' terminates the comparison (key-only match for "key=value" tokens)
 *   - A trailing lone 's' on either side is optional (plural-insensitive)
 *
 * Returns 0 if equal, (a_char - b_char) for first difference otherwise.
 */
int rage_stricmp_6358(const char* a, const char* b)
{
    unsigned char ca, cb;

    for (;;) {
        ca = (unsigned char)*a++;
        cb = (unsigned char)*b++;

        /* Lowercase A-Z → a-z; treat '=' like '\0' */
        if ((unsigned)(ca - 'A') <= 25u)
            ca += 32;
        else if (ca == '=')
            ca = 0;

        if ((unsigned)(cb - 'A') <= 25u)
            cb += 32;
        else if (cb == '=')
            cb = 0;

        /* Keep looping while chars are equal and non-null */
        if (ca == 0)
            break;
        if (ca != cb)
            break;
    }

    /*
     * Plural-s handling: if the side that is 's' has nothing (or '=') after
     * it, and the other side is already at end, treat them as equal.
     *
     * Covers: "options" vs "option", "option" vs "options",
     *         "options=1" vs "option".
     */
    if (ca == 's') {
        /* Peek at next char in a (a was already incremented past the 's') */
        signed char next_a = (signed char)*a;
        if (next_a == 0 || next_a == '=') {
            /* Trailing 's' — match only if b is also exhausted */
            if (cb == 0)
                return 0;
        }
        return (int)ca - (int)cb;
    }

    if (ca != 0) {
        /* Mid-string mismatch, neither char is 's' */
        return (int)ca - (int)cb;
    }

    /* ca == 0: a is exhausted. Check whether b has an optional trailing 's' */
    if (cb == 's') {
        signed char next_b = (signed char)*b;
        if (next_b == 0 || next_b == '=')
            return 0;
    }

    return (int)ca - (int)cb;
}