/**
 * RAGE String Utilities
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Duplicates a string using RAGE's memory allocator.
 * Returns NULL if input is NULL.
 */
char* atSingleton_29E0_g(const char* str);

/**
 * Case-insensitive string comparison with RAGE-specific rules.
 * Treats '=' as terminator, allows trailing 's' to match null.
 * Returns 0 if equal, <0 if str1 < str2, >0 if str1 > str2.
 */
int rage_stricmp(const char* str1, const char* str2);

#ifdef __cplusplus
}
#endif
