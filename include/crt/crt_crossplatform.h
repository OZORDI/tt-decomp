#pragma once

// Cross-platform CRT replacements
// Xbox 360 → Windows/macOS/Linux

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Memory functions
void* crt_malloc(size_t size);
void crt_free(void* ptr);
void* crt_realloc(void* ptr, size_t size);

// String functions
size_t crt_strlen(const char* str);
char* crt_strcpy(char* dest, const char* src);
int crt_strcmp(const char* s1, const char* s2);

#ifdef __cplusplus
}
#endif
