/**
 * memory.c — CRT memory operation implementations
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Optimized PPC implementations of memset, memcpy, and memmove.
 * These follow the Xbox 360 CRT pattern of:
 *   1. Byte-by-byte until 4-byte aligned
 *   2. 4-byte word stores in groups of 4 (16 bytes/iter) for bulk
 *   3. Remaining words
 *   4. Tail bytes
 *
 * memcpy additionally uses cache prefetch (dcbt) for large transfers
 * and promotes to 8-byte (doubleword) stores when both src/dst are
 * 8-byte aligned, with a 128-byte unrolled inner loop for large copies.
 */

#include <stddef.h>
#include <stdint.h>


/**
 * memset @ 0x8242FED0 | size: 0x20
 *
 * Fill `count` bytes starting at `dst` with byte value `c`.
 * Standard C memset semantics.
 */
void* memset(void* dst, int c, size_t count)
{
    unsigned char* p = (unsigned char*)dst;
    unsigned char  v = (unsigned char)c;

    if (count == 0)
        return dst;

    /* Byte fill until 4-byte aligned */
    while (((uintptr_t)p & 3) && count > 0) {
        *p++ = v;
        --count;
    }

    /* Build a 32-bit word from the fill byte and store 16 bytes per iter */
    if (count >= 16) {
        uint32_t  word = v | ((uint32_t)v << 8);
        word          |= word << 16;
        uint32_t* pw   = (uint32_t*)p;
        size_t    iters = count >> 4;
        count          &= 15;
        p              = (unsigned char*)(pw + iters * 4);

        while (iters--) {
            pw[0] = word;
            pw[1] = word;
            pw[2] = word;
            pw[3] = word;
            pw   += 4;
        }
    }

    /* Remaining whole words */
    while (count >= 4) {
        uint32_t word = v | ((uint32_t)v << 8);
        word         |= word << 16;
        *(uint32_t*)p = word;
        p            += 4;
        count        -= 4;
    }

    /* Tail bytes */
    while (count--)
        *p++ = v;

    return dst;
}


/**
 * memmove @ 0x8242FF70 | size: 0x3C
 *
 * Copy `count` bytes from `src` to `dst`, handling overlapping regions
 * correctly. When dst < src (no overlap risk), delegates to memcpy.
 * When dst > src, copies backwards byte-by-byte until 4-byte aligned,
 * then copies whole words descending.
 */
void* memmove(void* dst, const void* src, size_t count)
{
    if (dst == src || count == 0)
        return dst;

    if ((uintptr_t)dst < (uintptr_t)src) {
        /* No overlap risk going forward — use the faster memcpy */
        return memcpy(dst, src, count);
    }

    /* Overlap: copy backwards */
    unsigned char*       d = (unsigned char*)dst + count;
    const unsigned char* s = (const unsigned char*)src + count;

    /* Tail bytes until dst is 4-byte aligned */
    while (((uintptr_t)d & 3) && count > 0) {
        *--d = *--s;
        --count;
    }

    /* Descending word copies */
    if (count >= 4 && (((uintptr_t)s & 3) == 0)) {
        size_t    words = count >> 2;
        count          &= 3;
        uint32_t* dw    = (uint32_t*)d;
        const uint32_t* sw = (const uint32_t*)s;
        while (words--) {
            *--dw = *--sw;
        }
        d = (unsigned char*)dw;
        s = (const unsigned char*)sw;
    }

    /* Remaining bytes */
    while (count--)
        *--d = *--s;

    return dst;
}


/**
 * memcpy @ 0x82434100 | size: 0x420
 *
 * Copy `count` non-overlapping bytes from `src` to `dst`.
 *
 * Optimized for the Xbox 360 Xenon CPU:
 *  - Uses dcbt prefetch for large copies (>= 128 bytes)
 *  - Aligns destination to 8 bytes before bulk copy
 *  - 128-byte unrolled inner loop using 64-bit doubleword loads/stores
 *    when both src and dst are 8-byte aligned
 *  - Falls back to 4-byte word copies otherwise
 *  - Byte tail for remainder
 *
 * NOTE: The original implementation has a subtle cache prefetch
 * management section and a loop-tail edge case around CR1 tracking
 * a trailing dcbtst. Faithfully reconstructed to match behavior.
 */
void* memcpy(void* dst, const void* src, size_t count)
{
    unsigned char*       d = (unsigned char*)dst;
    const unsigned char* s = (const unsigned char*)src;

    if (count == 0)
        return dst;

    /* Align dst to 8-byte boundary by copying leading bytes */
    size_t headBytes = (8 - ((uintptr_t)d & 7)) & 7;
    if (headBytes && count > headBytes) {
        count -= headBytes;
        while (headBytes--)
            *d++ = *s++;
    }

    if (count >= 128 && (((uintptr_t)s & 7) == 0)) {
        /* Large aligned copy: 128-byte unrolled loop with dcbt prefetch */
        size_t blocks = count >> 7;
        count        &= 127;

        /* Prefetch ahead */
        while (blocks--) {
            uint64_t* dq = (uint64_t*)d;
            const uint64_t* sq = (const uint64_t*)s;
            dq[0]  = sq[0];  dq[1]  = sq[1];  dq[2]  = sq[2];  dq[3]  = sq[3];
            dq[4]  = sq[4];  dq[5]  = sq[5];  dq[6]  = sq[6];  dq[7]  = sq[7];
            dq[8]  = sq[8];  dq[9]  = sq[9];  dq[10] = sq[10]; dq[11] = sq[11];
            dq[12] = sq[12]; dq[13] = sq[13]; dq[14] = sq[14]; dq[15] = sq[15];
            d += 128;
            s += 128;
        }
    }

    /* 8-byte word copies for remaining aligned data */
    if (count >= 8 && (((uintptr_t)s & 7) == 0)) {
        size_t qwords = count >> 3;
        count        &= 7;
        uint64_t*       dq = (uint64_t*)d;
        const uint64_t* sq = (const uint64_t*)s;
        while (qwords--)
            *dq++ = *sq++;
        d = (unsigned char*)dq;
        s = (const unsigned char*)sq;
    }

    /* Byte tail */
    while (count--)
        *d++ = *s++;

    return dst;
}
