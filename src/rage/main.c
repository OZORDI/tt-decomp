/**
 * rage/main.c — RAGE engine entry point and application bootstrap
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Contains the main entry function (rage_main_6970) which:
 *  - parses command-line arguments
 *  - negotiates network hardware connectivity
 *  - calls the application init and main run-loop
 *  - tears down after the run-loop returns
 *
 * Also forward-declares the other entry-path helpers defined in this TU:
 *   rage_setup_args_67F8    — argument parsing / command-line setup
 *   rage_get_exe_name_6628  — resolves the .exe / title name
 *   rage_app_init_6418      — application-level init (subsystems, allocs)
 *   rage_run_6780           — main run-loop (blocks until exit)
 */

#include "rage_system.hpp"
#include <stdint.h>

// ---------------------------------------------------------------------------
// External declarations
// ---------------------------------------------------------------------------

// Argument parsing helper.  Fills two arginfo structs on the stack.
extern void rage_setup_args_67F8(void* argOut0, void* argOut1);  // @ 0x821867F8

// Network hardware init.  Probes Xbox Live hardware state.
// Returns a status struct at r3+8 (we read bytes from +112 here).
extern void netHardware_1C60_v12(void* statusOut);  // @ 0x82481C60

// Network utility: fill status buffer (type, result).
// r3=type(2), r4=outBuf → status byte at buf+0, secondary byte at buf+1.
extern void util_1AF8(uint32_t type, void* buf);  // @ 0x82481AF8

// Assertion / log for a net-error condition (address of string arg varies).
extern void nop_8240E6D0(const char* msg);  // @ 0x8240E6D0

// Networking teardown (called on clean exit and on net error).
extern void rage_1B08(void);  // @ 0x82481B08

// Exe/title name resolution.
extern void rage_get_exe_name_6628(char* outName);  // @ 0x82186628

// Application-level init.  Sets up all subsystems (audio, gfx, etc.).
extern void rage_app_init_6418(void* argBuf0, void* argBuf1);  // @ 0x82186418

// Main run-loop.  Blocks until the game requests exit.
// Returns an int exit code.
extern int rage_run_6780(void);  // @ 0x82186780

// Virtual Log method on the global allocator/log object (vtable slot 7).
// Logs the title name at startup.
extern struct rageAllocator* g_pAllocator;  // @ SDA:0x82600000 + 4

// Global re-entrancy lock for the main-loop guard @ (lis(-32160)+7760)
extern int32_t g_mainCallDepth;  // @ 0x82066530 (approx)

// Global game-object flags used as an "app is running" indicator
// @ 0x825CAE80 = lis(-32163) + -20888
extern struct rageGameObjFlags {
    void*    pFlags;   // +0x00
    uint32_t flags;    // +0x04
} g_gameObjFlags0;  // @ 0x825CAE80

// Secondary game-object flags
// @ 0x825CAE68 = lis(-32163) + -20908
extern struct rageGameObjFlags g_gameObjFlags1;  // @ 0x825CAE68

// Tertiary game-object flags
// @ 0x825CAE78 = lis(-32163) + -20868
extern struct rageGameObjFlags g_gameObjFlags2;  // @ 0x825CAE78

// Arguments structures @ 0x825D83E8 (lis(-32252) + -31240)
//                      @ 0x825D83D4 (lis(-32252) + -31276)  — alternate
extern struct rageArgs g_rageArgs0;    // @ 0x825D83E8
extern struct rageArgs g_rageArgsAlt;  // @ 0x825D83D4


// ===========================================================================
// rage_main  @ 0x82186970 | size: 0x278
//
// Top-level entry point for Rockstar Table Tennis on Xbox 360.
//
// Flow:
//   1. Parse command-line arguments via rage_setup_args_67F8.
//   2. If this is the first (non-reentrant) call, negotiate Xbox Live
//      network hardware:
//        a. Probe hardware via netHardware_1C60_v12.
//        b. Query status via util_1AF8(2, buf).
//        c. If status byte == 2 AND result byte != 0, log the net-error
//           string and proceed to exit.
//        d. Otherwise, if status is not "clean", call rage_1B08 (teardown)
//           and log the status via nop_8240E6D0.
//   3. Call rage_app_init_6418 to initialise all subsystems.
//   4. If g_gameObjFlags0 is active (flags != 0), log the exe title name
//      via the global allocator's Log vtable slot.
//   5. If g_gameObjFlags1 is active, call the logger vtable slot 15 with
//      g_rageArgsAlt and an "app is active" bool derived from
//      g_gameObjFlags2.
//   6. Enter rage_run_6780 (the main game-loop).
//   7. After the loop returns, if g_gameObjFlags0 is active, call vtable
//      slot 16 (shutdown log).
//   8. Call rage_run_6780 ... // TODO verify second call / finalise path
//   9. On final exit, call rage_1B08 if and only if the call depth drops to 0.
//   10. Decrement g_mainCallDepth.
//
// Re-entrancy: the function increments g_mainCallDepth on entry and skips
// the network negotiation block if depth > 1 (already inside a main call).
// ===========================================================================
void rage_main(void* rawArgs0, void* rawArgs1) {
    // -------------------------------------------------------------------------
    // Step 1: parse arguments.
    // -------------------------------------------------------------------------
    rage_setup_args_67F8(rawArgs0, rawArgs1);

    // -------------------------------------------------------------------------
    // Step 2: network hardware negotiation (first call only).
    // -------------------------------------------------------------------------
    g_mainCallDepth++;

    if (g_mainCallDepth <= 1) {
        // Prepare a small status buffer on the stack (13 bytes minimum).
        struct {
            uint8_t  status;      // +0
            uint8_t  bSuccess;    // +1  (active flag)
            uint8_t  pad[11];
        } netStatus = { 0 };

        // Set a known initial state (type=13, flag=1).
        netStatus.status  = 13;
        netStatus.bSuccess = 1;

        // Probe Xbox Live hardware.
        netHardware_1C60_v12(&netStatus);

        // Query detailed status code via util_1AF8.
        uint16_t statusBuf = 0;
        util_1AF8(2, &statusBuf);

        // Check for a specific error condition:
        //   statusBuf low byte == 2 AND high byte != 0 → hard net error.
        uint8_t statusType   = (uint8_t)(statusBuf & 0xFF);
        uint8_t statusResult = (uint8_t)((statusBuf >> 8) & 0xFF);

        if (statusType == 2 && statusResult != 0) {
            // TODO: look up the exact error-string address
            //       @ lis(-32250) + -21104 = 0x823FEE50 (approx)
            nop_8240E6D0("net hardware error");
        }

        if (statusType != 2 || statusResult == 0) {
            // Not the clean path: tear down and log.
            rage_1B08();
            // TODO: second nop_8240E6D0 call reads statusBuf back — confirm
            //       it logs the raw status word.  @ 0x82186A18
            nop_8240E6D0("net status mismatch");
        }
    }

    // -------------------------------------------------------------------------
    // Step 3–5: application init and pre-loop logging.
    // -------------------------------------------------------------------------
    rage_app_init_6418(rawArgs0, rawArgs1);

    // Log the title/exe name if the primary game-object flags are active.
    if (g_gameObjFlags0.flags != 0) {
        char titleName[64];
        rage_get_exe_name_6628(titleName);
        // Dispatch to Log method (vtable slot 7) on the global allocator object.
        g_pAllocator->vtable->Log(g_pAllocator, titleName);
    }

    // If the secondary flags object is live, pass the args and active bool
    // to vtable slot 15 (some "register run args" call).
    if (g_gameObjFlags1.pFlags != NULL) {
        // Derive the "is active" bool from g_gameObjFlags2.
        int bActive = (g_gameObjFlags2.pFlags != NULL)
                    ? (*((uint8_t*)g_gameObjFlags2.pFlags) != 0 ? 1 : 0)
                    : 0;

        // vtable slot 15 — log / register args  @ SDA[4] vtable[60]
        g_pAllocator->vtable->RegisterArgs(g_pAllocator, &g_rageArgsAlt,
                                           (uint8_t)bActive);
    }

    // -------------------------------------------------------------------------
    // Step 6–8: main run-loop.
    // -------------------------------------------------------------------------
    int exitCode = rage_run_6780();

    // Notify the log object that the run-loop finished.
    if (g_gameObjFlags0.flags != 0) {
        // vtable slot 16 — shutdown notification
        g_pAllocator->vtable->OnRunComplete(g_pAllocator);
    }

    // Secondary run-loop result reporting.
    {
        // Determine whether g_gameObjFlagsAlt has its active byte set.
        // @ 0x825D83D4 area  TODO: confirm exact object
        int bActive = 0;
        if (g_gameObjFlags1.pFlags != NULL) {
            uint8_t activeByte = *((uint8_t*)g_gameObjFlags1.pFlags);
            bActive = (activeByte != 0) ? 1 : 0;
        }
        // vtable slot 14: submit exit code + args + active flag
        // @ SDA[4] vtable[56]
        g_pAllocator->vtable->OnExit(g_pAllocator, &g_rageArgs0,
                                     (uint8_t)(exitCode & 0xFF), (uint8_t)bActive);
    }

    // -------------------------------------------------------------------------
    // Step 9–10: teardown on final exit.
    // -------------------------------------------------------------------------
    g_mainCallDepth--;

    if (g_mainCallDepth == 0) {
        // Last nested call — fully tear down networking.
        rage_1B08();
    }
}
