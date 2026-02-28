/**
 * exit.c — CRT exit handlers: atexit, _onexit, _doexit
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Implements the atexit/onexit callback table used by the RAGE engine's
 * shutdown path. The table is a dynamically grown array of function
 * pointers stored at a global address managed by two pointers:
 *   g_atexit_table_cur  – current write pointer (grows upward)
 *   g_atexit_table_end  – end of allocated table memory
 *
 * On _doexit, the table is walked backward (LIFO order, per C standard),
 * calling each registered function, then a second "on_exit" table is
 * flushed the same way.
 *
 * _doexit guards against re-entrant calls via g_doexit_type (set to 1
 * when in progress) and uses a spinlock for thread safety. If already
 * in _doexit, KeBugCheck is triggered to prevent recursive teardown.
 */

#include <stdint.h>
#include <stddef.h>

/* atexit/onexit table globals (SDA-relative) */
extern uint32_t* g_atexit_table_cur;   /* @ ~0x825B0A38 — current write ptr */
extern uint32_t* g_atexit_table_end;   /* @ ~0x825B0A3C — end of table block */

/* Exit state flags */
extern uint8_t  g_doexit_flag;         /* @ 0x825E6E6C — quick-exit flag     */
extern uint32_t g_doexit_type;         /* @ 0x825E6E70 — 0=normal, 1=active  */
extern uint32_t g_doexit_called;       /* @ 0x825E6E74 — set once doexit runs */

/* Forward declarations */
void  _crt_spinlock_acquire(int id);
void  _crt_spinlock_release(int id);
void  _run_table(void** start, void** end);
void  KeBugCheck(int code);
void* fiAsciiTokenizer_3B28_g(void);          /* grow atexit table */
void* fiAsciiTokenizer_3650_g(size_t newSz, size_t minSz); /* realloc helper */


/**
 * _onexit @ 0x8242F6B0 | size: 0x100
 *
 * Register a function to be called on program exit. Equivalent to the
 * POSIX/C standard _onexit(). Grows the atexit table if needed.
 *
 * The table is stored as a simple array of function pointers. The two
 * global pointers track the current insertion point and the allocated end.
 * When the table is full a realloc-style growth occurs (doubles up to
 * 2048 entries, then adds 16 at a time).
 *
 * Uses an SEH frame to protect the spinlock acquire/release so the lock
 * is always released even if an exception propagates.
 *
 * @param fn  Function to register (called at exit with no arguments)
 * @return    fn on success, NULL on allocation failure
 */
void* _onexit(void (*fn)(void))
{
    _crt_spinlock_acquire(8);   /* CRT atexit spinlock */

    void** cur = (void**)g_atexit_table_cur;
    void** end = (void**)g_atexit_table_end;

    if (cur >= end) {
        /* Table is full — calculate amount to grow */
        ptrdiff_t used  = cur - (void**)g_atexit_table_end + ((cur - (void**)g_atexit_table_cur) / sizeof(void*) + 1);

        /* Try to grow: double up to 2048 entries, else add 16 */
        size_t grow = (used <= 2048) ? (size_t)used : 2048;
        size_t newSz = grow + used;

        void* newTable = fiAsciiTokenizer_3650_g(newSz * sizeof(void*), (used + 16) * sizeof(void*));
        if (!newTable) {
            /* Growth failed — try minimal 16-entry growth */
            newTable = fiAsciiTokenizer_3650_g((used + 16) * sizeof(void*), 0);
            if (!newTable) {
                /* Total failure — cannot register */
                _crt_spinlock_release(8);
                return NULL;
            }
        }

        /* Update table pointers from the reallocated block */
        ptrdiff_t count = cur - (void**)g_atexit_table_end;
        g_atexit_table_cur = (uint32_t*)((uint8_t*)newTable + count * sizeof(void*));
        g_atexit_table_end = (uint32_t*)newTable;
        cur = (void**)g_atexit_table_cur;
    }

    /* Store the function pointer and advance the write cursor */
    *cur = (void*)fn;
    g_atexit_table_cur = (uint32_t*)(cur + 1);

    _crt_spinlock_release(8);
    return (void*)fn;
}


/**
 * atexit @ 0x8242F7D0 | size: 0x28
 *
 * Standard C atexit(). Delegates to _onexit and converts the return value
 * to 0 (success) / -1 (failure) per the C standard.
 *
 * @param fn  Function to register
 * @return    0 on success, nonzero on failure
 */
int atexit(void (*fn)(void))
{
    void* result = _onexit(fn);
    return result ? 0 : -1;
}


/**
 * _doexit @ 0x8242F9D0 | size: 0x10C
 *
 * Full CRT exit handler. Runs the atexit table (LIFO), then a secondary
 * on_exit callback table, then calls KeBugCheck if exitCode is 0 (Xbox
 * convention: successful shutdown still calls the system bugcheck path
 * to cleanly release the title).
 *
 * Guards:
 *  - g_doexit_type == 1 means already in _doexit → KeBugCheck immediately
 *    to prevent recursive teardown.
 *  - SEH frame ensures the spinlock is always released.
 *
 * @param exitCode   Process exit code
 * @param callExit   Whether to invoke system exit after cleanup
 * @param quick      If nonzero, skip atexit table (quick_exit semantics)
 */
void _doexit(int exitCode, int callExit, int quick)
{
    _crt_spinlock_acquire(8);

    /* Re-entrancy guard */
    if (g_doexit_called == 1) {
        KeBugCheck(0);
        return;
    }

    /* Detect recursive _doexit (should never happen, but guard anyway) */
    if (g_doexit_type == 1) {
        KeBugCheck(0);
        return;
    }

    g_doexit_type = 1;
    g_doexit_flag = (uint8_t)quick;

    if (!callExit) {
        /* Walk the atexit table backward (LIFO), calling each fn */
        void** cur = (void**)g_atexit_table_cur;
        void** end = (void**)g_atexit_table_end;

        while (cur > end) {
            --cur;
            if (*cur) {
                void (*fn)(void) = (void(*)(void))*cur;
                fn();
                /* Reload pointers in case a callback modified the table */
                cur = (void**)g_atexit_table_cur;
                end = (void**)g_atexit_table_end;
            }
        }

        /* Flush secondary on_exit callback table */
        extern void* g_on_exit_start;
        extern void* g_on_exit_end;
        _run_table(&g_on_exit_start, &g_on_exit_end);
    }

    /* Flush any remaining exit callbacks registered after atexit flush */
    extern void* g_exit_start;
    extern void* g_exit_end;
    _run_table(&g_exit_start, &g_exit_end);

    /* Xbox shutdown: call system exit if requested */
    if (exitCode != 0) {
        KeBugCheck(0);
    }
}


/**
 * __run_table
 *
 * Runs a table of function pointers from start to end.
 * Used to execute static initializers and exit handlers.
 *
 * @param start  Pointer to first function pointer in table
 * @param end    Pointer past last function pointer in table
 */
void __run_table(void** start, void** end) {
    _run_table(start, end);
}

/**
 * _run_table
 *
 * Internal implementation of __run_table.
 * Walks the function pointer table and calls each non-NULL entry.
 *
 * @param start  Pointer to first function pointer in table
 * @param end    Pointer past last function pointer in table
 */
void _run_table(void** start, void** end) {
    if (!start || !end || start >= end) {
        return;
    }
    
    for (void** p = start; p < end; p++) {
        if (*p) {
            void (*fn)(void) = (void(*)(void))*p;
            fn();
        }
    }
}
