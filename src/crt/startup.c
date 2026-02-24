/**
 * startup.c — CRT startup and C++ static initialization
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Responsible for running the C++ static constructors before main() is
 * called and the destructors on exit. The key function _cinit_setup sets
 * up four function pointer tables that hold the ctor/dtor arrays, then
 * calls __cinit_impl to invoke them in order.
 *
 * Table layout (four pointers stored in a global segment):
 *   [0] g_ctors_start    – first ctor table address
 *   [1] g_ctors_end      – last+1 ctor table address
 *   [2] g_dtors_start    – first dtor table address
 *   [3] g_dtors_end      – last+1 dtor table address
 *
 * The addresses for these table boundaries come from the linker script
 * (.ctors / .dtors sections). On Xbox 360 the CRT startup is responsible
 * for calling them since there is no OS CRT loader doing it.
 *
 * _doexit_entry is the tiny stub installed via atexit() so that when the
 * game calls exit() the static destructors and atexit table are flushed.
 */

#include <stdint.h>
#include <stddef.h>

/* Function pointer tables for static ctors/dtors. These four globals
 * (stored consecutively) are set by _cinit_setup before __cinit_impl runs.
 * Absolute addresses come from linker-provided symbols. */
extern void (*g_ctors_start)(void);    /* @ ~0x8242E6D4 linker .ctors start */
extern void (*g_ctors_end)(void);      /* @ ~0x8242E6D8 linker .ctors end   */
extern void (*g_dtors_start)(void);    /* @ ~0x8242E6DC linker .dtors start */
extern void (*g_dtors_end)(void);      /* @ ~0x8242E6E0 linker .dtors end   */

/* g_cinit_retval: result code stored by _cinit_setup, inspected later.
 * @ 0x825b0a74 */
extern int g_cinit_retval;

/* Forward declarations */
int  __cinit_impl(void);                  /* @ 0x82433D00 — walk ctor tables */
void fiAsciiTokenizer_3FB8_g(int a, int b); /* error reporting helper */
void _crt_tls_fiber_setup(void);          /* set up fiber-local storage */
void _crt_critsec_init(void);             /* initialize CRT critical section */
void _doexit_error(void);                 /* called on cinit failure exit */


/**
 * _cinit_setup @ 0x8242FDC8 | size: 0x108
 *
 * Sets up the four ctor/dtor table boundary pointers then calls __cinit_impl
 * to run every registered C++ static constructor.
 *
 * On success returns 1. On failure, uses error-reporting callbacks and
 * may invoke KeBugCheck via _doexit_error if the process cannot continue.
 *
 * The specific addresses are Xbox 360 module-relative linker values baked
 * in at the call sites via lis/addi pairs in the assembly.
 */
int _cinit_setup(void)
{
    /* Install ctor/dtor table boundaries from the binary's .ctors/.dtors */
    g_ctors_start = (void(*)(void))0x8241621C;   /* lbl: .ctors section start */
    g_ctors_end   = (void(*)(void))0x8241621C;   /* TODO: verify exact end addr */
    g_dtors_start = (void(*)(void))0x82416224;   /* .dtors section start */
    g_dtors_end   = (void(*)(void))0x82416244;   /* .dtors section end   */

    int result = __cinit_impl();

    if (result != 0) {
        /* cinit failure — call the registered error handler via the stored
         * function pointer, then try to determine the failure mode */
        g_ctors_start();                          /* error dispatch call */

        int retval = g_cinit_retval;
        if (retval != -1) {
            /* Numeric error code — report it */
            fiAsciiTokenizer_3FB8_g(1, 204);
        } else {
            /* -1 return: attempt fiber setup and critical section init,
             * then return success to the caller */
            if (result != 0) {
                _crt_tls_fiber_setup();
                _crt_critsec_init();
                return 1;
            }
        }
    }

    _doexit_error();
    return 0;
}


/**
 * _doexit_entry @ 0x8242FB60 | size: 0x10
 *
 * Minimal stub registered with atexit() that triggers the full CRT exit
 * path (_doexit) when the process terminates. The stub exists to decouple
 * the atexit registration from the full doexit implementation.
 */
void _doexit_entry(void)
{
    extern void _doexit(int exitCode, int callExit, int quick);
    _doexit(0, 0, 0);
}
