/**
 * rage/main.c — RAGE engine entry and command-line bootstrap
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Complete Phase-3 "RAGE Engine Init" — all eight functions that carry
 * execution from CRT startup through to the game's main() loop:
 *
 *   rage_Main                 @ 0x82186970  — top-level engine entry
 *   rage_ParseCommandLine     @ 0x821867F8  — XEX cmdline → argc/argv
 *   rage_AppInit              @ 0x82186418  — store globals + process options
 *   rage_RunMain              @ 0x82186780  — thin wrapper that calls main()
 *   rage_GetExeName           @ 0x82186628  — retrieve exe-name token
 *   rage_IsAppStarted         @ 0x82186BE8  — app-started predicate
 *   netHardware_XNetStartup   @ 0x82481C60  — XNetStartup wrapper
 *   rage_WSAStartup           @ 0x82481AF8  — WSAStartup wrapper
 *   rage_WSACleanup           @ 0x82481B08  — WSACleanup wrapper
 *
 * ─── Memory tracker dispatch ─────────────────────────────────────────────
 *
 * rage_main_6970 makes five virtual calls on the memory-tracker object that
 * lives at [g_pAllocatorBase+4] before and after the game runs:
 *
 *   Slot  7 : SetProcessName(exeName)   — register exe name for leak reports
 *   Slot 13 : Startup()                 — init tracker before the game loop
 *   Slot 14 : DumpLeaks(key, logFile)   — write "c:/leaks.txt" post-run
 *   Slot 15 : StartLogging(csvPath, on) — begin "c:/logmemory.csv" logging
 *   Slot 16 : StopLogging()             — flush CSV log after the run
 *
 * ─── Feature-gate mechanism ──────────────────────────────────────────────
 *
 * Four addresses in the .data section serve as presence checks for optional
 * memory-tracking features.  Each happens to be the address of an RTTI
 * TypeDescriptor for one of the gdUnlockCondition* classes; the check reads
 * offset +4 (the TypeDescriptor's mangled-name pointer), which is always
 * non-null once the CRT has set up RTTI.  Every gate therefore fires on
 * every run of this shipped build — they only differ in dev / stripped builds.
 *
 *   g_gate_setProcessName @ 0x825CAE40 — guards SetProcessName (slot 7)
 *   g_gate_dumpLeaks      @ 0x825CAE54 — guards DumpLeaks       (slot 14)
 *   g_gate_startLogging   @ 0x825CAE68 — guards StartLogging/StopLogging
 *   g_gate_logActiveFlag  @ 0x825CAE7C — secondary active flag for slot 15
 *
 * ─── Key globals (SDA r13 = 0x82600000) ──────────────────────────────────
 *
 *   g_pAllocatorBase      @ r13[0]       — allocator root; tracker at [+4]
 *   g_rage_main_call_depth @ 0x82601E50  — re-entrancy counter
 *   g_app_started_flag    @ 0x825EBCA1  — set to 1 by rage_app_init
 *   g_pExeName            @ 0x825EBCE8  — stored exe-name string pointer
 *   g_argc                @ 0x825EBCEC  — stored argc
 *   g_argv                @ 0x825EBCF0  — stored argv pointer
 *   g_pCmdArgRegistry     @ 0x825EBCF4  — head of registered option list
 */

#include "rage/rage_system.hpp"
#include "rage/rage_allocator.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ─── Forward declarations ─────────────────────────────────────────────── */
void        rage_ParseCommandLine(int* pArgc, const char*** pArgv);
void        rage_AppInit(int argc, const char** argv);
int         rage_RunMain(void);
void        rage_GetExeName(void* self, const char** outName);
bool        rage_IsAppStarted(void);
void        netHardware_XNetStartup(void* xnetParams);
int         rage_WSAStartup(int version, void* wsaDataOut);
void        rage_WSACleanup(void);

/* CRT / platform */
const char* _get_xex_cmdline(void);                          /* @ 0x8242BFB0 */
const char* _xe_strcpyn_10(const char* str);                 /* @ 0x82432120 */
int         strncmp(const char* a, const char* b, size_t n); /* @ 0x824320D0 */
int         main(void);                                      /* @ 0x820C0378 */

/* Cross-subsystem debug / log nop (4 bytes, true no-op in release) */
void        rage_DebugLog(const char* fmt, ...);              /* @ 0x8240E6D0 */

/* Xbox 360 network SDK */
int         NetDll_XNetStartup(int version, void* params);   /* @ 0x8258603C */
int         NetDll_WSAStartup(int version, void* wsaData);   /* @ 0x82585F2C */
int         NetDll_WSACleanup(int reserved);                 /* @ 0x82585F3C */

/* rage_stricmp / strchr used in arg parsing */
int         rage_StringCompareNoCase(const char* a, const char* b); /* @ 0x82186358 */
const char* rage_StringFindChar(const char* s, int ch);              /* @ 0x824321B0 */

/* ─── Globals ──────────────────────────────────────────────────────────── */

extern int32_t     g_rage_main_call_depth;  /* @ 0x82601E50  SDA r13+7760 */
extern uint8_t     g_app_started_flag;      /* @ 0x825EBCA1 */
extern const char* g_pExeName;              /* @ 0x825EBCE8 */
extern int         g_argc;                  /* @ 0x825EBCEC */
extern const char** g_argv;                 /* @ 0x825EBCF0 */

/* Argv buffer written by rage_setup_args (BSS, zero at load) @ 0x8271A878 */
extern const char* g_argv_buf[256];

/* Empty-string sentinel for unmatched value arguments @ 0x8271A38C */
extern const char  g_empty_str[1];

/* .rdata string constants */
extern const char  k_default_xex_path[];  /* "A:\default.xex"                         @ 0x82038580 */
extern const char  k_unknown_opt_fmt[];   /* "*** Unknown option '%s' on command line."@ 0x82038538 */
extern const char  k_memtrack_key[];      /* "Main()"                                  @ 0x820385F8 */
extern const char  k_leaks_path[];        /* "c:/leaks.txt"                            @ 0x820385E8 */
extern const char  k_logmem_csv[];        /* "c:/logmemory.csv"                        @ 0x820385D4 */
extern const char  k_winsock_no_dll[];    /* "InitWinSock - Couldn't find a usable WinSock DLL"          @ 0x8205AD90 */
extern const char  k_winsock_bad_ver[];   /* "InitWinSock - Couldn't find a usable WinSock DLL (version=%x)" @ 0x8205ADC4 */
extern const char  k_disc_path_prefix[];  /* "A:"                                      @ 0x8203860C */

/* ─── CmdArgRecord — registered command-line option node ──────────────── */

typedef struct CmdArgRecord {
    const char*          m_pName;      /* +0  option name, without leading '-'  */
    const char*          m_pValue;     /* +4  value string, filled when matched */
    int                  m_reserved;   /* +8  0 = eligible; non-zero = skip     */
    struct CmdArgRecord* m_pNext;      /* +12 next node in singly-linked list   */
} CmdArgRecord;

extern CmdArgRecord* g_pCmdArgRegistry;  /* @ 0x825EBCF4 */

/* ─── Feature-gate type (RTTI TypeDescriptor layout) ─────────────────── */

typedef struct FeatureGate {
    uint32_t  m_vftable;   /* +0 TypeDescriptor pVFTable (NULL in static data) */
    uint8_t*  m_pName;     /* +4 mangled-name ptr — non-null = feature present  */
} FeatureGate;

extern FeatureGate g_gate_setProcessName;  /* @ 0x825CAE40 */
extern FeatureGate g_gate_dumpLeaks;       /* @ 0x825CAE54 */
extern FeatureGate g_gate_startLogging;    /* @ 0x825CAE68 */
extern FeatureGate g_gate_logActiveFlag;   /* @ 0x825CAE7C */

/* App-started gate (separate TypeDescriptor) @ 0x825CAEA4 */
typedef struct AppStartedGate {
    uint32_t  m_vftable;
    uint8_t*  m_pTypeName;
} AppStartedGate;
extern AppStartedGate g_gate_appStarted;

/* ─── Memory-tracker dispatch helpers ─────────────────────────────────── */

typedef struct MemTracker { void** vtable; } MemTracker;

static MemTracker* get_mem_tracker(void)
{
    extern void* g_pAllocatorBase;     /* @ SDA r13+0 */
    return (MemTracker*)(((void**)g_pAllocatorBase)[1]);   /* field +4 */
}

static void mem_SetProcessName(MemTracker* t, const char* name)
{
    ((void (*)(MemTracker*, const char*))t->vtable[7])(t, name);
}
static void mem_Startup(MemTracker* t)
{
    ((void (*)(MemTracker*))t->vtable[13])(t);
}
static void mem_DumpLeaks(MemTracker* t, const char* key, const char* path)
{
    ((void (*)(MemTracker*, const char*, const char*))t->vtable[14])(t, key, path);
}
static void mem_StartLogging(MemTracker* t, const char* csv, bool active)
{
    ((void (*)(MemTracker*, const char*, bool))t->vtable[15])(t, csv, active);
}
static void mem_StopLogging(MemTracker* t)
{
    ((void (*)(MemTracker*))t->vtable[16])(t);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * netHardware_1C60_v12 @ 0x82481C60 | size: 0xC
 *
 * Initialises Xbox Live low-level networking via XNetStartup.
 * xnetParams is a caller-allocated struct with type=13, enabled=1 at offsets
 * 0 and 1.
 * ═══════════════════════════════════════════════════════════════════════════ */
void netHardware_XNetStartup(void* xnetParams)
{
    NetDll_XNetStartup(1, xnetParams);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_WSAStartup @ 0x82481AF8 | size: 0xC
 *
 * Initialises WinSock using WSAStartup.  Returns 0 on success.
 * ═══════════════════════════════════════════════════════════════════════════ */
int rage_WSAStartup(int version, void* wsaDataOut)
{
    return NetDll_WSAStartup(version, wsaDataOut);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_WSACleanup @ 0x82481B08 | size: 0xC
 *
 * Shuts down WinSock via WSACleanup.
 * ═══════════════════════════════════════════════════════════════════════════ */
void rage_WSACleanup(void)
{
    NetDll_WSACleanup(1);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_setup_args_67F8 @ 0x821867F8 | size: 0x178
 *
 * Parses the XEX system command line into an argc/argv array stored in
 * the global g_argv_buf[].
 *
 * The XEX command-line format is: "exename arg1 arg2 ..."
 *
 *  1. Calls _get_xex_cmdline() to obtain the raw system string.
 *  2. Writes k_default_xex_path ("A:\default.xex") as argv[initial_argc],
 *     increments *pArgc, and sets *pArgv = g_argv_buf.
 *  3. Iterates over tokens in the raw string:
 *       - Skips space / tab separators.
 *       - Handles "quoted tokens" — strips quotes, respects embedded spaces.
 *       - Null-terminates each token in-place in the cmdline buffer.
 *       - Skips the FIRST token (the exe name already present in the cmdline)
 *         and stores every subsequent token into g_argv_buf[argc++].
 * ═══════════════════════════════════════════════════════════════════════════ */
void rage_ParseCommandLine(int* pArgc, const char*** pArgv)
{
    char* cmdline = (char*)_get_xex_cmdline();
    if (!cmdline)
        return;

    /* Set up the argv base and store the default exe path as argv[0]. */
    *pArgv                = g_argv_buf;
    int argc              = *pArgc;
    g_argv_buf[argc]      = k_default_xex_path;
    g_argv_buf[argc + 1]  = NULL;   /* null sentinel */
    ++argc;
    *pArgc = argc;

    char* p               = cmdline;
    bool  skip_first_token = true;   /* first cmdline token is the exe name */

    while (*p != '\0') {
        /* Skip leading whitespace. */
        while (*p == ' ' || *p == '\t') ++p;
        if (*p == '\0') break;

        const char* token_start;

        if (*p == '"') {
            /* Quoted token: scan for closing '"' (or end of string). */
            ++p;
            token_start = p;
            while (*p != '\0' && *p != '"') ++p;
        } else {
            /* Unquoted token: runs until whitespace or end. */
            token_start = p;
            while (*p != '\0' && *p != ' ' && *p != '\t') ++p;
        }

        if (skip_first_token) {
            skip_first_token = false;
        } else {
            /* Store token pointer in the argv buffer. */
            argc             = *pArgc;
            g_argv_buf[argc] = token_start;
            ++argc;
            *pArgc           = argc;
        }

        /* Null-terminate this token in the cmdline buffer. */
        if (*p != '\0') {
            *p = '\0';
            ++p;
        }
    }

    /* Ensure null sentinel after last stored token. */
    g_argv_buf[*pArgc] = NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_app_init_6418 @ 0x82186418 | size: 0x20C
 *
 * Stores exe name, argc, and argv into SDA globals, then matches every
 * argv token against the registered CmdArgRecord linked list.
 *
 * Option prefix characters: '-', '/', 0x96 (en-dash / code-page 1252).
 * Options where the first char after the prefix is '0'..'9' are positional
 * numeric arguments and are silently skipped.
 * Named options are compared case-insensitively (via rage_stricmp_6358).
 *   - If the match contains '=', the value is everything after '='.
 *   - If no '=' and the next argv does not start with a prefix char, the
 *     next argv entry is used as the value.
 *   - Otherwise the value is set to the empty-string sentinel (g_empty_str).
 * Unknown options are logged via nop_8240E6D0 (no-op in release).
 * ═══════════════════════════════════════════════════════════════════════════ */
void rage_AppInit(int argc, const char** argv)
{
    /* Mark application as started. */
    g_app_started_flag = 1;

    /* Resolve exe name: argv[0] if present, else the hard-coded default. */
    const char* exeName;
    if (argc > 0 && argv && argv[0])
        exeName = argv[0];
    else
        exeName = k_default_xex_path;

    g_pExeName = exeName;
    g_argc     = argc;
    g_argv     = argv;

    if (argc <= 1)
        return;

    CmdArgRecord* listHead = g_pCmdArgRegistry;

    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (!arg) continue;

        uint8_t prefix = (uint8_t)*arg;
        bool hasPrefix = (prefix == '-') || (prefix == '/') || (prefix == 0x96u);
        if (!hasPrefix) continue;

        const char* name = arg + 1;

        /* Skip numeric arguments (e.g. "-1", "/2"). */
        uint8_t first = (uint8_t)*name;
        if (first >= '0' && first <= '9') continue;

        /* Try to match against every registered named option. */
        bool matched = false;
        for (CmdArgRecord* node = listHead; node; node = node->m_pNext) {
            if (node->m_reserved != 0) continue;  /* positional/pre-set, skip */
            if (rage_StringCompareNoCase(name, node->m_pName) != 0) continue;

            /* Match found — resolve value. */
            const char* eq    = rage_StringFindChar(name, '=');
            const char* value;

            if (!eq) {
                /* No '=' separator: peek at the next argv. */
                const char* nextValue = NULL;
                if (i + 1 < argc) {
                    const char* next = argv[i + 1];
                    if (next) {
                        uint8_t np = (uint8_t)*next;
                        bool nextHasPrefix = (np == '-') || (np == '/')
                                             || (np == 0x96u);
                        if (!nextHasPrefix) {
                            nextValue = next;
                        } else {
                            /* next starts with a prefix — only a value if
                               the char after the prefix is NOT a digit. */
                            uint8_t nc = (uint8_t)*(next + 1);
                            if (nc < '0' || nc > '9')
                                nextValue = NULL;
                            else
                                nextValue = NULL;  /* it's a numeric arg, skip */
                        }
                    }
                }
                value = nextValue ? nextValue : g_empty_str;
            } else {
                /* Value follows '=' in the same token. */
                value = eq + 1;
            }

            node->m_pValue = value;
            matched = true;
            break;
        }

        /* Unknown option warning (no-op in release builds). */
        if (!matched) {
            uint8_t c = (uint8_t)*name;
            if (c != 'X' && c != 'x')    /* suppress internal 'X'-prefixed opts */
                rage_DebugLog(k_unknown_opt_fmt, name);
        }
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_get_exe_name_6628 @ 0x82186628 | size: 0x70
 *
 * Retrieves a short exe-name token from the gate object's m_pName field.
 * Passes the raw name through _xe_strcpyn_10 (copies up to 10 chars) to
 * strip any long path prefix, then stores the result at *outName.
 *
 * Parameters:
 *   self    — FeatureGate* whose m_pName holds the raw exe path
 *   outName — receives the resolved short token, or NULL on failure
 * ═══════════════════════════════════════════════════════════════════════════ */
void rage_GetExeName(void* self, const char** outName)
{
    const char* rawName = ((FeatureGate*)self)->m_pName;
    if (!rawName || !*rawName) {
        *outName = NULL;
        return;
    }
    *outName = _xe_strcpyn_10(rawName);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_IsAppStarted @ 0x82186BE8 | size: 0x70
 *
 * Returns true if the application is active and ready to run.
 *
 * Checks g_gate_appStarted.m_pTypeName:
 *   - Non-null (always in this build): return true.
 *   - Null: compare g_pExeName against "A:" (Xbox 360 disc prefix).
 *           If the exe lives on disc return false (not yet started),
 *           otherwise return true.
 * ═══════════════════════════════════════════════════════════════════════════ */
bool rage_IsAppStarted(void)
{
    if (g_gate_appStarted.m_pTypeName != NULL)
        return true;

    /* Gate not active: exe must reside on disc (path starts with "A:").
     * Assembly: strncmp==0 (match) -> li r11,1 -> return true
     *           strncmp!=0 (miss)  -> bne skips, r11=0 -> return false */
    if (g_pExeName && strncmp(g_pExeName, k_disc_path_prefix, 2) == 0)
        return true;

    return false;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_run_6780 @ 0x82186780 | size: 0x48
 *
 * Wraps main() with an SEH frame (on Xbox 360; irrelevant in recomp form).
 * ═══════════════════════════════════════════════════════════════════════════ */
int rage_RunMain(void)
{
    return main();
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_main_6970 @ 0x82186970 | size: 0x278
 *
 * Top-level RAGE engine entry.  Called from __crt_main_entry after the CRT
 * has run all static initialisers.
 * ═══════════════════════════════════════════════════════════════════════════ */
int rage_Main(int argc, const char** argv)
{
    /* ── 1. Canonicalise command-line arguments ────────────────────────── */
    rage_ParseCommandLine(&argc, &argv);

    /* ── 2. One-time network init (re-entrancy guarded) ──────────────── */
    ++g_rage_main_call_depth;
    if (g_rage_main_call_depth <= 1) {
        /*
         * XNet parameters struct (13 bytes):
         *   [+0] = 13 (XNET_STARTUP_PARAMS type identifier)
         *   [+1] = 1  (enabled flag)
         */
        uint8_t xnetParams[13] = {0};
        xnetParams[0] = 13;
        xnetParams[1] = 1;
        netHardware_XNetStartup(xnetParams);

        /* WSAStartup — version 2.0, fill a WSADATA-sized buffer. */
        uint8_t wsaData[512] = {0};
        int wsaErr = util_1AF8(2, wsaData);
        if (wsaErr != 0)
            nop_8240E6D0(k_winsock_no_dll);

        /*
         * Validate WinSock version negotiated by WSAStartup.
         * On big-endian Xbox 360, the WSADATA wVersion field is read as a
         * 16-bit big-endian value: high byte = wsaData[0], low byte = wsaData[1].
         * The assembly checks low byte == 2 (major) and high byte == 0 (minor).
         */
        uint8_t wsMajor = wsaData[1];   /* low byte of big-endian u16 */
        uint8_t wsMinor = wsaData[0];   /* high byte */
        if (wsMajor != 2 || wsMinor != 0) {
            rage_1B08();
            nop_8240E6D0(k_winsock_bad_ver,
                         (unsigned)(wsMajor | ((unsigned)wsMinor << 8)));
        }

    }

    /* ── 3. Store globals + match command-line options ────────────────── */
    rage_AppInit(argc, argv);

    /* ── 4. Memory tracker: set process name + startup ───────────────── */
    MemTracker* tracker = get_mem_tracker();

    if (g_gate_setProcessName.m_pName) {
        const char* exeToken = NULL;
        rage_GetExeName(&g_gate_setProcessName, &exeToken);
        if (exeToken)
            mem_SetProcessName(tracker, exeToken);
    }
    mem_Startup(tracker);

    /* ── 5. Memory CSV logging ────────────────────────────────────────── */
    if (g_gate_startLogging.m_pName) {
        const char* csvPath = (g_gate_startLogging.m_pName[0] != '\0')
                              ? (const char*)g_gate_startLogging.m_pName
                              : k_logmem_csv;
        bool logActive = (g_gate_logActiveFlag.m_pName != NULL);
        mem_StartLogging(tracker, csvPath, logActive);
    }

    /* ── 6. Run the game ─────────────────────────────────────────────── */
    int exitCode = rage_RunMain();

    /* ── 7. Stop CSV logging ─────────────────────────────────────────── */
    if (g_gate_startLogging.m_pName)
        mem_StopLogging(tracker);

    /* ── 8. Dump post-run leak report ───────────────────────────────── */
    {
        const char* leakPath = NULL;
        if (g_gate_dumpLeaks.m_pName) {
            leakPath = (g_gate_dumpLeaks.m_pName[0] != '\0')
                       ? (const char*)g_gate_dumpLeaks.m_pName
                       : k_leaks_path;
        }
        mem_DumpLeaks(tracker, k_memtrack_key, leakPath);
    }

    /* ── 9. Network teardown (last caller only) ─────────────────────── */
    --g_rage_main_call_depth;
    if (g_rage_main_call_depth == 0)
        rage_WSACleanup();

    return exitCode;
}

