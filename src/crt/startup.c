/**
 * startup.c — CRT phase-1 startup helpers
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * This file lifts the CRT bootstrap routines executed from xstart:
 *   - __mainCRTStartup (the XEX entry point / CRT startup driver)
 *   - _heap_init_check
 *   - _initterm_e
 *   - _initterm
 *   - _initterm_e2
 *   - _cinit_setup
 *
 * Address notes:
 *   __mainCRTStartup @ 0x8242BD20
 *   _heap_init_check @ 0x8242C2C0
 *   _initterm_e      @ 0x8242C118
 *   _initterm        @ 0x8242C0A0
 *   _initterm_e2     @ 0x8242BFC0
 *   _cinit_setup     @ 0x8242FDC8
 */

#include <stddef.h>
#include <stdint.h>

typedef int (*CrtInitResultFn)(void);
typedef void (*CrtInitVoidFn)(void);

typedef struct CrtListEntry {
    struct CrtListEntry* m_pNext;
    struct CrtListEntry* m_pPrev;
} CrtListEntry;

typedef struct CrtInitCallbackNode {
    CrtListEntry m_links;
    void (*m_callback)(int initFlag);
} CrtInitCallbackNode;

typedef struct CrtFiberContext {
    uint32_t m_tlsFiberValue;
    int32_t m_tlsFiberIndex;
    uint8_t m_pad0[12];
    uint32_t m_isFiberReady;
    uint8_t m_pad1[68];
    void* m_tlsBackingStore;
} CrtFiberContext;

typedef struct CrtTlsDispatch {
    int (*m_tlsAlloc)(void* destructorThunk);
    void* (*m_tlsGetValue)(uint32_t tlsIndex);
    int (*m_tlsSetValue)(uint32_t tlsIndex, void* value);
    int (*m_tlsFree)(uint32_t tlsIndex);
} CrtTlsDispatch;

/* _initterm table ranges in .data */
static const uintptr_t kInittermStart = 0x825B0000u;
static const uintptr_t kInittermEnd = 0x825B000Cu;
static const uintptr_t kInittermRunStart = 0x825B0010u;
static const uintptr_t kInittermRunEnd = 0x825B09F0u;
static const uintptr_t kInittermE2Start = 0x825B09F4u;
static const uintptr_t kInittermE2End = 0x825B0A00u;

/* Imported kernel pointers used by _heap_init_check/_heap_init_impl path */
static const uintptr_t kImpKeDebugMonitorData = 0x820008F4u;

/* _cinit_setup constants resolved from lis/addi */
static const uintptr_t kCrtFiberTemplate = 0x825B1090u;

/* Globals mirrored from binary data area */
void (*g_single_init_fn)(void) = NULL;                     /* @ 0x825B1298 */
int g_cinit_retval = -1;                                   /* @ 0x825B0A74 */
static uint8_t g_crt_init_critical_section[8];             /* @ 0x825B0A30 */
static CrtListEntry g_crt_init_callback_list = {           /* @ 0x825B0A4C */
    &g_crt_init_callback_list,
    &g_crt_init_callback_list
};
static CrtInitCallbackNode g_cinit_list_node;              /* @ 0x825E6E78 */
static CrtTlsDispatch g_tls_dispatch;                      /* @ 0x825E6E84 */

/* External functions (resolved by import hooks/runtime) */
int _heap_init_impl(void);                                 /* @ 0x8242C1E8 */
void HalReturnToFirmware(uint32_t routine);                /* @ 0x82585E4C */
void RtlEnterCriticalSection(void* criticalSection);       /* @ 0x82585E0C */
void RtlLeaveCriticalSection(void* criticalSection);       /* @ 0x82585DFC */
int __cinit_impl(void);                                    /* @ 0x82433D00 */
void* _crt_tls_fiber_setup(void);                          /* @ 0x82566B78 */
void* _calloc_crt(int count, int size);                    /* @ 0x82433FB8 */
void _tls_dtor_cleanup(void);                              /* @ 0x82433D88 */
void _crt_tls_callback(int initFlag);                      /* @ 0x82434090 */
void _crt_fiber_destroy(void);                             /* @ 0x8242FCA8 */
int _KeTlsAlloc_thunk(void* destructorThunk);              /* @ 0x8242FB70 */
void* ke_KeTlsGetValue_621C(uint32_t tlsIndex);            /* @ 0x8258621C */
int KeTlsSetValue_stub(uint32_t tlsIndex, void* value); /* @ 0x8258622C */
int KeTlsFree_stub(uint32_t tlsIndex);                  /* @ 0x8258624C */
extern const char* __imp_ExLoadedCommandLine;              /* @ 0x820008D4 */

/**
 * _crt_critsec_init @ 0x8242C178 | size: 0x6C
 *
 * Inserts/removes a callback node from the CRT init list under lock.
 */
void _crt_critsec_init(CrtListEntry* entry, int insert)
{
    RtlEnterCriticalSection(g_crt_init_critical_section);

    if (insert != 0) {
        CrtListEntry* head = &g_crt_init_callback_list;
        CrtListEntry* tail = head->m_pPrev;
        entry->m_pNext = head;
        entry->m_pPrev = tail;
        tail->m_pNext = entry;
        head->m_pPrev = entry;
    } else {
        CrtListEntry* next = entry->m_pNext;
        CrtListEntry* prev = entry->m_pPrev;
        prev->m_pNext = next;
        next->m_pPrev = prev;
    }

    RtlLeaveCriticalSection(g_crt_init_critical_section);
}

/**
 * _initterm_e @ 0x8242C118 | size: 0x60
 *
 * Runs every callback currently registered in the CRT init list.
 */
void _initterm_e(int initFlag)
{
    RtlEnterCriticalSection(g_crt_init_critical_section);

    CrtListEntry* head = &g_crt_init_callback_list;
    CrtListEntry* current = head->m_pNext;
    while (current != head) {
        CrtInitCallbackNode* node = (CrtInitCallbackNode*)current;
        current = current->m_pNext;
        node->m_callback(initFlag);
    }

    RtlLeaveCriticalSection(g_crt_init_critical_section);
}

/**
 * _initterm @ 0x8242C0A0 | size: 0x74
 *
 * Walks the early static initializer table from 0x825B0000..0x825B000C.
 * Stops early if any initializer returns nonzero.
 */
int _initterm(void)
{
    CrtInitResultFn* current = (CrtInitResultFn*)kInittermStart;
    CrtInitResultFn* end = (CrtInitResultFn*)kInittermEnd;
    int result = 0;

    while (current < end && result == 0) {
        CrtInitResultFn fn = *current++;
        if (fn != NULL) {
            result = fn();
        }
    }

    return result;
}

/**
 * _initterm_e2 @ 0x8242BFC0 | size: 0xE0
 *
 * Executes staged startup tables:
 *   1) optional g_single_init_fn
 *   2) error-propagating table 0x825B09F4..0x825B0A00
 *   3) plain callback table 0x825B0010..0x825B09F0 (skip 0 and -1)
 */
int _initterm_e2(int unused)
{
    (void)unused;

    if (g_single_init_fn != NULL) {
        g_single_init_fn();
    }

    CrtInitResultFn* stage = (CrtInitResultFn*)kInittermE2Start;
    CrtInitResultFn* stageEnd = (CrtInitResultFn*)kInittermE2End;
    int result = 0;

    while (stage < stageEnd) {
        if (result != 0) {
            return result;
        }

        CrtInitResultFn fn = *stage++;
        if (fn != NULL) {
            result = fn();
        }
    }

    if (result != 0) {
        return result;
    }

    CrtInitVoidFn* run = (CrtInitVoidFn*)kInittermRunStart;
    CrtInitVoidFn* runEnd = (CrtInitVoidFn*)kInittermRunEnd;
    while (run < runEnd) {
        intptr_t raw = (intptr_t)(*run++);
        if (raw != 0 && raw != -1) {
            ((CrtInitVoidFn)raw)();
        }
    }

    return 0;
}

/**
 * _get_xex_cmdline @ 0x8242BFB0 | size: 0x0C
 *
 * Returns the Xbox loader-provided command-line pointer imported through
 * __imp_ExLoadedCommandLine in the XEX import table.
 */
const char* _get_xex_cmdline(void)
{
    return __imp_ExLoadedCommandLine;
}

/**
 * _doexit_error @ 0x8242FB78 | size: 0x50
 *
 * Releases the TLS slot allocated during _cinit_setup, then calls the
 * CRT fatal-exit helper.
 */
void _doexit_error(void)
{
    if (g_cinit_retval != -1) {
        g_tls_dispatch.m_tlsFree((uint32_t)g_cinit_retval);
        g_cinit_retval = -1;
    }

    _tls_dtor_cleanup();
}

/**
 * _cinit_setup @ 0x8242FDC8 | size: 0x108
 *
 * Sets up TLS dispatch callbacks, runs __cinit_impl, allocates one TLS slot
 * with destructor thunk _crt_fiber_destroy, creates the fiber context,
 * and registers _crt_tls_callback in the CRT init callback list.
 */
int _cinit_setup(void)
{
    g_tls_dispatch.m_tlsAlloc = _KeTlsAlloc_thunk;
    g_tls_dispatch.m_tlsGetValue = ke_KeTlsGetValue_621C;
    g_tls_dispatch.m_tlsSetValue = KeTlsSetValue_stub;
    g_tls_dispatch.m_tlsFree = KeTlsFree_stub;

    if (__cinit_impl() == 0) {
        _doexit_error();
        return 0;
    }

    g_cinit_retval = g_tls_dispatch.m_tlsAlloc((void*)(uintptr_t)_crt_fiber_destroy);
    if (g_cinit_retval == -1) {
        _doexit_error();
        return 0;
    }

    CrtFiberContext* fiberContext = (CrtFiberContext*)_calloc_crt(1, 204);
    if (fiberContext == NULL) {
        _doexit_error();
        return 0;
    }

    if (g_tls_dispatch.m_tlsSetValue((uint32_t)g_cinit_retval, fiberContext) == 0) {
        _doexit_error();
        return 0;
    }

    fiberContext->m_isFiberReady = 1;
    fiberContext->m_tlsBackingStore = (void*)kCrtFiberTemplate;
    fiberContext->m_tlsFiberValue = (uint32_t)(uintptr_t)_crt_tls_fiber_setup();
    fiberContext->m_tlsFiberIndex = -1;

    g_cinit_list_node.m_callback = _crt_tls_callback;
    _crt_critsec_init(&g_cinit_list_node.m_links, 1);
    return 1;
}

/**
 * _heap_init_check @ 0x8242C2C0 | size: 0x58
 *
 * Runs _heap_init_impl; on failure, notifies KeDebugMonitorData slot 6
 * with (2, 0), then returns to firmware routine 1.
 */
void _heap_init_check(void)
{
    if (_heap_init_impl() != 0) {
        return;
    }

    uintptr_t debugMonitorImport = *(uintptr_t*)kImpKeDebugMonitorData;
    void** debugMonitorData = NULL;
    if (debugMonitorImport != 0u) {
        debugMonitorData = *(void***)debugMonitorImport;
    }
    if (debugMonitorData != NULL) {
        void (*notifyFailure)(int, int) = (void (*)(int, int))debugMonitorData[6];
        notifyFailure(2, 0);
    }

    HalReturnToFirmware(1);
}

/**
 * _doexit_entry @ 0x8242FB60 | size: 0x10
 *
 * atexit trampoline into _doexit.
 */
void _doexit_entry(void)
{
    extern void _doexit(int exitCode, int callExit, int quick);
    _doexit(0, 0, 0);
}

/* =====================================================================
 * __mainCRTStartup — XEX CRT entry point
 * ===================================================================== */

/* Maximum number of command-line arguments the parser can handle.
 * The on-stack argv array occupies offsets 112..192 of the 496-byte
 * frame, giving room for (192-112)/4 = 20 pointer slots. */
#define CRT_MAX_ARGS 20

/* Maximum characters in the tokenized argument string buffer.
 * Offsets 192..496 of the frame = 304 bytes. */
#define CRT_ARG_BUF_SIZE 304

/**
 * On-stack state used by the __mainCRTStartup command-line parser.
 * Mirrors the frame layout at [r31+80..r31+496].
 */
typedef struct CrtCmdLineState {
    char     m_currentChar;        /* +80  current byte from cmdline  */
    uint8_t  m_pad81[3];
    char**   m_pArgvWrite;         /* +84  next slot in argv array    */
    char*    m_pCmdRead;           /* +88  read cursor in cmdline     */
    char*    m_pArgWrite;          /* +92  write cursor in arg buffer */
    int      m_argc;               /* +96  argument count             */
    int      m_inQuote;            /* +100 inside double-quoted span  */
    uint8_t  m_pad104[8];
    char*    m_argv[CRT_MAX_ARGS]; /* +112 argv pointer array         */
    char     m_argBuf[CRT_ARG_BUF_SIZE]; /* +192 tokenized arg strings */
} CrtCmdLineState;

/* External functions used only by __mainCRTStartup */
int  _check_xdk_version(void);                           /* @ 0x8242BB70 */
void __crt_main_entry(void* pStartupParms, void* pBase);  /* @ 0x820C0128 */
void DbgPrint(const char* fmt, ...);                      /* @ 0x82585DCC */
void XamLoaderTerminateTitle(void);                       /* @ 0x82585D0C */

/* Globals written by __mainCRTStartup */
extern int32_t g_atexit_guard_0;      /* @ 0x82733048 */
extern int32_t g_atexit_guard_1;      /* @ 0x8273304C */
extern void*   g_xex_exec_info;       /* @ 0x825E6E64 */

/* DbgPrint format string @ 0x820013D0 (.rdata, 48 bytes) */
static const char* const kExitFmtStr =
    "exit() returned to __mainCRTStartup (code %d)\n";

/**
 * __mainCRTStartup @ 0x8242BD20 | size: 0x1C8
 *
 * The CRT entry point — the very first function that runs when the XEX
 * loads. This is the outermost frame of the entire game process.
 *
 * Execution sequence:
 *   1. Invalidate atexit guards (set to -1)
 *   2. Initialize the CRT heap
 *   3. Run static initializers (_initterm_e, _cinit_setup, _initterm, _initterm_e2)
 *   4. Validate XDK version / locale setup
 *   5. Parse XEX command line into argc/argv
 *   6. Call __crt_main_entry(argc, argv, 0)  =>  rage_main  =>  game loop
 *   7. On return: run atexit handlers, print exit code, terminate
 *
 * The function has an SEH try/catch wrapper in the original binary
 * (except_data_8242BD20 / except_record_8242BD20).
 */
void __mainCRTStartup(void)
{
    int exitCode;

    /* 1. Invalidate atexit guards so the atexit table is clean. */
    g_atexit_guard_0 = -1;
    g_atexit_guard_1 = -1;

    /* 2. Initialize the CRT heap; fatal-halts on failure. */
    _heap_init_check();

    /* 3. Run CRT static initializer phases in order. */
    _initterm_e(1);       /* Phase 1: registered CRT init callbacks     */
    _cinit_setup();       /* TLS / fiber context setup                   */
    _initterm();          /* Phase 2: early static initializer table     */
    _initterm_e2(1);      /* Phase 3: staged startup tables              */

    /* 4. Validate XDK version and set up locale.
     *    Nonzero return = version mismatch => fatal terminate. */
    if (_check_xdk_version() != 0) {
        _doexit_entry();
        XamLoaderTerminateTitle();
        return;
    }

    /* 5. Parse the XEX command line into argc/argv.
     *    If g_xex_exec_info is NULL (no exec info block), or the
     *    command line string is NULL, pass argc=0, argv=NULL. */
    if (g_xex_exec_info != NULL) {
        CrtCmdLineState state;
        char* cmdline;
        char  ch;
        char* argOut;

        state.m_pArgvWrite = state.m_argv;
        state.m_argc = 0;
        state.m_inQuote = 0;

        cmdline = (char*)_get_xex_cmdline();
        state.m_pCmdRead = cmdline;

        if (cmdline != NULL) {
            argOut = state.m_argBuf;
            state.m_pArgWrite = argOut;

            /* Skip leading whitespace. */
            ch = *cmdline;
            state.m_currentChar = ch;
            while (ch != '\0') {
                if (ch != ' ' && ch != '\t') {
                    break;
                }
                cmdline++;
                state.m_pCmdRead = cmdline;
                ch = *cmdline;
                state.m_currentChar = ch;
            }

            /* Tokenize: split by whitespace, handle double-quote toggling. */
            while (state.m_currentChar != '\0') {
                /* Start a new argument. */
                state.m_argc++;
                state.m_inQuote = 0;
                *state.m_pArgvWrite = state.m_pArgWrite;
                state.m_pArgvWrite++;

                /* Copy characters for this argument. */
                while (state.m_currentChar != '\0') {
                    ch = state.m_currentChar;

                    if (ch == '"') {
                        /* Toggle in-quote flag. */
                        state.m_inQuote = (state.m_inQuote == 0) ? 1 : 0;
                    } else {
                        /* Outside quotes: space/tab ends the argument. */
                        if (state.m_inQuote == 0) {
                            if (ch == ' ' || ch == '\t') {
                                break;
                            }
                        }
                        /* Copy character to output buffer. */
                        *state.m_pArgWrite = ch;
                        state.m_pArgWrite++;
                    }

                    /* Advance read cursor. */
                    cmdline++;
                    state.m_pCmdRead = cmdline;
                    ch = *cmdline;
                    state.m_currentChar = ch;
                }

                /* Null-terminate this argument string. */
                *state.m_pArgWrite = '\0';
                state.m_pArgWrite++;

                if (state.m_argc > CRT_MAX_ARGS) {
                    break;
                }

                /* Skip whitespace between arguments. */
                while (state.m_currentChar != '\0') {
                    ch = state.m_currentChar;
                    if (ch != ' ' && ch != '\t') {
                        break;
                    }
                    cmdline++;
                    state.m_pCmdRead = cmdline;
                    ch = *cmdline;
                    state.m_currentChar = ch;
                }
            }

            /* Null-terminate the argv array. */
            *state.m_pArgvWrite = NULL;

            /* 6. Enter the game via __crt_main_entry. */
            __crt_main_entry((void*)(uintptr_t)state.m_argc, (void*)state.m_argv);
            exitCode = state.m_argc;
        } else {
            /* No command line string available. */
            __crt_main_entry(NULL, NULL);
            exitCode = 0;
        }
    } else {
        /* No exec info block — pass empty argc/argv. */
        __crt_main_entry(NULL, NULL);
        exitCode = 0;
    }

    /* 7. Process atexit handlers. */
    _doexit_entry();

    /* Print exit code for debugging, then terminate. */
    DbgPrint(kExitFmtStr, exitCode);
    XamLoaderTerminateTitle();
}
