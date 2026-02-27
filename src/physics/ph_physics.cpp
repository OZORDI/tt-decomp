/**
 * ph_physics.cpp — RAGE physics module utilities
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Small utility routines from the ph* physics subsystem that don't belong
 * to a specific class.  Placed in the physics compilation unit at
 * 0x824321xx (not in splits.txt → inferred from address range and prefix).
 */

#include "ph_physics.hpp"
#include <stddef.h>

/**
 * ph_21B0 @ 0x824321B0 | size: 0x3C
 *
 * Scan a string for a specific character, returning a pointer to its first
 * occurrence.  Functionally equivalent to strchr(3) with two variants:
 *
 *   ch != 0  →  Returns pointer to first byte equal to ch, or NULL if not found.
 *   ch == 0  →  Returns pointer to the NUL terminator (scan-to-end shortcut).
 *
 * This variant is used during argument/config parsing in rage_app_init_6418
 * to locate '/' or '-' prefix characters in command-line tokens.
 *
 * The function is an alias of util_21F0 in the binary (the linker merged the
 * two identical bodies into a single address).
 */
const char* ph_21B0(const char* str, int ch)
{
    unsigned char c = (unsigned char)*str;

    if (ch == 0) {
        /* Scan to end — return pointer to null terminator */
        while (c != 0)
            c = (unsigned char)*++str;
        return str;
    }

    /* Scan for ch */
    while (c != 0) {
        if (c == (unsigned char)ch)
            return str;
        c = (unsigned char)*++str;
    }

    /* Not found */
    return NULL;
}
