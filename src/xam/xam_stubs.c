/**
 * Xbox 360 Kernel/XAM Stub Implementations
 * Rockstar Presents Table Tennis
 *
 * Provides host-side implementations of Xbox 360 kernel and XAM functions
 * that are referenced by the decomp but not yet covered by the dedicated
 * xam/*.c translation units.
 *
 *  DbgPrint                    - debug printf  (vprintf to stdout)
 *  XamLoaderTerminateTitle     - terminate game (exit(0))
 *  NetDll_recvfrom             - network recv   (stub, returns SOCKET_ERROR)
 *  NetDll_sendto               - network send   (stub, returns SOCKET_ERROR)
 *  RtlMultiByteToUnicodeN_6FA8_w - multibyte->unicode (simple widening stub)
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Xbox 360 SOCKET_ERROR equivalent */
#define XE_SOCKET_ERROR (-1)

/* ========================================================================= */
/* Debug Output                                                              */
/* ========================================================================= */

/**
 * DbgPrint @ 0x82585DCC
 *
 * Xbox 360 kernel debug print.  On the host we simply forward to vprintf
 * so that debug spew appears on stdout.
 */
void DbgPrint(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

/* ========================================================================= */
/* Title Loader                                                              */
/* ========================================================================= */

/**
 * XamLoaderTerminateTitle @ 0x82585D0C
 *
 * Called by the CRT exit path to hard-terminate the title.
 * On the host we simply call exit(0).
 */
void XamLoaderTerminateTitle(void) {
    exit(0);
}

/* ========================================================================= */
/* Network DLL Functions                                                     */
/* ========================================================================= */

/**
 * NetDll_recvfrom @ 0x82585FEC
 *
 * Xbox 360 network DLL recvfrom.  Takes an extra XNet handle parameter
 * before the standard BSD recvfrom args.  On the host, networking is not
 * supported so we return SOCKET_ERROR.
 */
int NetDll_recvfrom(int handle, void* socket, void* buf, int len,
                    int flags, void* from, int* fromlen) {
    (void)handle;
    (void)socket;
    (void)buf;
    (void)len;
    (void)flags;
    (void)from;
    (void)fromlen;
    return XE_SOCKET_ERROR;
}

/**
 * NetDll_sendto @ 0x8258600C
 *
 * Xbox 360 network DLL sendto.  Takes an extra XNet handle parameter
 * before the standard BSD sendto args.  On the host, networking is not
 * supported so we return SOCKET_ERROR.
 */
int NetDll_sendto(int handle, void* socket, const void* buf, int len,
                  int flags, const void* to, int tolen) {
    (void)handle;
    (void)socket;
    (void)buf;
    (void)len;
    (void)flags;
    (void)to;
    (void)tolen;
    return XE_SOCKET_ERROR;
}

/* ========================================================================= */
/* String Conversion                                                         */
/* ========================================================================= */

/**
 * RtlMultiByteToUnicodeN_6FA8_w
 *
 * Xbox 360 wrapper around the kernel's RtlMultiByteToUnicodeN with a
 * game-specific calling convention (codePage, flags, source, sourceLength,
 * dest, destChars).
 *
 * On the host we do a trivial ASCII-to-UTF-16 widening (sufficient for the
 * ASCII strings the game actually passes through this path).  Returns 0
 * on success.
 */
int RtlMultiByteToUnicodeN_6FA8_w(uint32_t codePage, uint32_t flags,
                                   const char* source, int sourceLength,
                                   uint16_t* dest, int destChars) {
    (void)codePage;
    (void)flags;

    if (!source || !dest || destChars <= 0) {
        return -1;
    }

    /* Determine how many characters to convert */
    int srcLen = sourceLength;
    if (srcLen < 0) {
        srcLen = (int)strlen(source) + 1; /* include NUL */
    }

    int count = (srcLen < destChars) ? srcLen : destChars;

    /* Simple ASCII widening — one byte per char16 */
    for (int i = 0; i < count; i++) {
        dest[i] = (uint16_t)(unsigned char)source[i];
    }

    /* NUL-terminate if room */
    if (count < destChars) {
        dest[count] = 0;
    }

    return 0; /* STATUS_SUCCESS */
}
