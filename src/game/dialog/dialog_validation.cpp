/**
 * dialog_validation.cpp — Dialog event payload validators
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifts the three small dialog-related state-machine PreInit validator
 * trampolines. Each one reads a payload field off the event buffer (a1 + 16,
 * +20 or +24), range-checks it, emits a debug diagnostic through the runtime
 * `nullsub_1` printf trampoline when the check fails, and then hands off to
 * the shared `sub_821B2788` state-machine PostEvent dispatcher so the event
 * still propagates (the asserts are soft warnings, not fatal).
 *
 * Functions lifted:
 *   dialog_ValidateDialogIDAndLines   @ 0x82414D20  (sub_82414D20)
 *     - Walks dialog ID (+16) against 1..209 and line count (+20) >= 1 before
 *       dispatching. Emits:
 *         "Invalid dialog ID[%d]"                 @ 0x82075E78
 *         "Invalid Lines[%d] for dialog[%d]"      @ 0x82075E90
 *
 *   dialog_ValidateUnlockMessage      @ 0x82226400  (sub_82226400)
 *     - Validates an unlock-item string (+16) length (< 32 chars) and the
 *       message id (+24) range (< 0xD2 / 210). Emits:
 *         "no item name specified"
 *         "unlock item name [%s] is too long by %d characters!"
 *         "Invalid unlock dialog message found: %d for item: %s" @ 0x8204CC8C
 *
 *   dialog_HandleCreateNewMatchDialog @ 0x8230CDB0  (sub_8230CDB0)
 *     - Dispatches the CreateNewMatch dialog result (+5984): 1 -> search-failed,
 *       2 -> user-declined, 0 -> aborted. Emits:
 *         "Search failed -- user chose to create new match"
 *         "Search failed -- user didn't want to create match"
 *         "CreateNewMatch Dialog aborted?"        @ 0x8205D61C
 *     - NOTE: this is a larger function that also performs game-state
 *       dispatch via sub_8230C308/sub_8230CF20; only the validator/error
 *       path is lifted here. The non-error dispatch is left as a stubbed
 *       forward call until the pongDialogState vtable is fully lifted.
 *
 * String anchors (verified against orig/434C4803/default_base.bin):
 *   0x820432D0  "Invalid Dialog ID %d, for review only tip!"
 *               g_str_dialog_invalidID_reviewTip  (reserved; stripped asserts)
 *   0x8204CC8C  g_str_dialog_invalidUnlockMsg
 *   0x8205D61C  g_str_dialog_createNewMatchAbort
 *   0x82075E78  g_str_dialog_invalidDialogID
 *   0x82075E90  g_str_dialog_invalidLines
 */

#include <cstdint>

// ────────────────────────────────────────────────────────────────────────────
// Named string externs (defined in src/game/dialog_strings.cpp)
// ────────────────────────────────────────────────────────────────────────────
extern "C" const char* g_str_dialog_invalidID_reviewTip;     // @ 0x820432D0
extern "C" const char* g_str_dialog_invalidUnlockMsg;        // @ 0x8204CC8C
extern "C" const char* g_str_dialog_createNewMatchAbort;     // @ 0x8205D61C
extern "C" const char* g_str_dialog_invalidDialogID;         // @ 0x82075E78
extern "C" const char* g_str_dialog_invalidLines;            // @ 0x82075E90

// Supporting strings already defined alongside the unlock / match paths.
extern "C" const char* g_str_dialog_unlockItemNoName;        // "no item name specified"
extern "C" const char* g_str_dialog_unlockItemNameTooLong;   // "unlock item name [%s] is too long by %d characters!"
extern "C" const char* g_str_dialog_searchFailedCreate;      // "Search failed -- user chose to create new match"
extern "C" const char* g_str_dialog_searchFailedDecline;     // "Search failed -- user didn't want to create match"

// ────────────────────────────────────────────────────────────────────────────
// Runtime helpers (see dialog_loader.cpp / pong_player.cpp for the matching
// prototypes / hook installations).
// ────────────────────────────────────────────────────────────────────────────

// Debug/telemetry printf trampoline @ 0x8240E6D0 (a.k.a. nullsub_1 / nop_8240E6D0).
extern "C" void rage_PrintDebugLine(const char* fmt, ...);

// State-machine PostEvent dispatcher @ 0x821B2788 — shared by every pong
// state PreInit that needs to forward a normalised payload value. Not a real
// exported symbol (inlined into the phBoundCapsule_26C0_g block); treat as a
// forward-declared helper so the lift still matches the PPC call.
extern "C" int pongState_PostEventInt(void* eventSlot,
                                      int   normalisedValue,
                                      int   a3, int a4,
                                      int   a5, int a6,
                                      int   a7, int a8);

// CreateNewMatch dialog handler callees (pong match/network layer — stubs
// until those vtables are fully lifted).
extern "C" void pongDialogState_TransitionNewMatch(void* self, int stateId,
                                                   int flag,
                                                   int a4, int a5,
                                                   int a6, int a7, int a8);
extern "C" void pongDialogState_ReturnToSearch(void* self);

// ────────────────────────────────────────────────────────────────────────────
// Event-payload layout (the "a1" passed in by the state machine is the 32-byte
// event slot; +16/+20/+24 are the three payload DWORDs the validators read).
// ────────────────────────────────────────────────────────────────────────────
struct DialogEventSlot {
    uint32_t pad0;          // +0
    uint32_t pad4;           // +4
    uint32_t pad8;           // +8
    uint32_t padC;           // +12
    uint32_t payload0;       // +16  (dialog ID  /  unlock item-name pointer)
    uint32_t payload1;       // +20  (line count)
    uint32_t payload2;       // +24  (unlock message id)
    uint32_t pad1C;          // +28
};
static_assert(sizeof(DialogEventSlot) == 32, "DialogEventSlot must match PPC layout");

// ────────────────────────────────────────────────────────────────────────────
// sub_82414D20 — dialog-ID + line-count validator
// ────────────────────────────────────────────────────────────────────────────
extern "C" int dialog_ValidateDialogIDAndLines(DialogEventSlot* slot,
                                               int a2,
                                               int a3, int a4,
                                               int a5, int a6,
                                               int a7, int a8) {
    int dialogId = static_cast<int>(slot->payload0);
    if (dialogId <= 0 || dialogId >= 210) {
        rage_PrintDebugLine(g_str_dialog_invalidDialogID, dialogId);
    }

    int lineCount = static_cast<int>(slot->payload1);
    if (lineCount < 1) {
        rage_PrintDebugLine(g_str_dialog_invalidLines,
                            lineCount,
                            static_cast<int>(slot->payload0));
    }

    return pongState_PostEventInt(slot, lineCount, a3, a4, a5, a6, a7, a8);
    (void)a2;
}

// ────────────────────────────────────────────────────────────────────────────
// sub_82226400 — unlock-dialog message validator
// ────────────────────────────────────────────────────────────────────────────
extern "C" int dialog_ValidateUnlockMessage(DialogEventSlot* slot,
                                            int a2,
                                            int a3, int a4) {
    const unsigned char* itemName =
        reinterpret_cast<const unsigned char*>(
            static_cast<uintptr_t>(slot->payload0));

    // (a) Item-name present + length check.
    unsigned int   nameLen   = 0;
    int            lastByte  = 0;
    int            basePtr   = 0;
    unsigned int   namePtrU  = static_cast<unsigned int>(slot->payload0);

    if (itemName) {
        const unsigned char* walker = itemName;
        basePtr = static_cast<int>(reinterpret_cast<uintptr_t>(walker));
        do {
            lastByte = *walker++;
        } while (lastByte);
        nameLen = static_cast<unsigned int>(walker - itemName - 1);

        if (nameLen >= 0x20) {
            // Re-walk to reproduce the PPC `while ( *v5++ ) ;` advance that
            // yields the offset delta stashed into the third printf arg.
            const unsigned char* v5 = itemName;
            while (*v5++) {
                /* empty */
            }
            rage_PrintDebugLine(
                g_str_dialog_unlockItemNameTooLong,
                reinterpret_cast<const char*>(itemName),
                reinterpret_cast<intptr_t>(v5)
                    - static_cast<intptr_t>(namePtrU) - 32);
        }
    } else {
        rage_PrintDebugLine(g_str_dialog_unlockItemNoName);
    }

    // (b) Message id range check (< 0xD2 = 210, the dialog table size).
    unsigned int msgId = slot->payload2;
    if (msgId >= 0xD2u) {
        rage_PrintDebugLine(
            g_str_dialog_invalidUnlockMsg,
            msgId,
            reinterpret_cast<const char*>(
                static_cast<uintptr_t>(slot->payload0)));
    }

    return pongState_PostEventInt(slot,
                                  static_cast<int>(msgId),
                                  a3, a4,
                                  static_cast<int>(nameLen),
                                  lastByte,
                                  basePtr,
                                  static_cast<int>(nameLen));
    (void)a2;
}

// ────────────────────────────────────────────────────────────────────────────
// sub_8230CDB0 — CreateNewMatch dialog dispatch (error-path portion)
//
// The PPC function is large and dispatches on a1+5980 / +5981 / +5976 / +5984
// to either a network-result path or a generic fall-through.  Only the three
// terminal branches that feed the "Search failed …" / "CreateNewMatch
// aborted?" diagnostics are lifted here — the non-error sub_8230C308 /
// sub_8230CF20 calls are forwarded to thin shims until those functions are
// themselves lifted.
// ────────────────────────────────────────────────────────────────────────────
struct PongDialogSelf {
    uint32_t headerPad[4];       // +0..+15
    uint32_t currentState;       // +16
    uint8_t  pad14[5564 - 20];   // pad to +5564
    uint32_t fallbackState;      // +5564
    uint8_t  pad23C[5976 - 5568]; // pad to +5976
    uint32_t resultMode;         // +5976
    uint32_t pad5980Byte;        // +5980  (byte gate)
    uint32_t pad5981Byte;        // +5981  (byte gate)
    uint32_t createChoice;       // +5984
};

extern "C" void dialog_HandleCreateNewMatchDialog(PongDialogSelf* self,
                                                  int a2,
                                                  int a3, int a4,
                                                  int a5, int a6,
                                                  int a7, int a8) {
    // The leading "is the dialog active" gate (+5980 / +5981) — when either
    // byte is zero the function diverts into sub_8230CF20 (dialog teardown)
    // which is left as a shim until that path is lifted.
    const uint8_t gate0 = *reinterpret_cast<const uint8_t*>(
        reinterpret_cast<const char*>(self) + 5980);
    const uint8_t gate1 = *reinterpret_cast<const uint8_t*>(
        reinterpret_cast<const char*>(self) + 5981);

    if (!gate0) {
        pongDialogState_ReturnToSearch(self);
        return;
    }
    if (!gate1) {
        return;
    }

    if (self->resultMode != 2) {
        pongDialogState_TransitionNewMatch(self,
                                           static_cast<int>(self->fallbackState),
                                           0,
                                           a3, a4, a5, a6, a7);
        return;
    }

    const uint32_t choice = self->createChoice;
    if (choice == 1) {
        rage_PrintDebugLine(g_str_dialog_searchFailedCreate);
        pongDialogState_TransitionNewMatch(self, 58, 0, a3, a4, a5, a6, a7);
        self->currentState = 0;
    } else if (choice == 2) {
        rage_PrintDebugLine(g_str_dialog_searchFailedDecline);
        pongDialogState_TransitionNewMatch(self,
                                           static_cast<int>(self->fallbackState),
                                           0, a3, a4, a5, a6, a7);
        self->currentState = 0;
    } else {
        if (choice == 0) {
            rage_PrintDebugLine(g_str_dialog_createNewMatchAbort);
        }
        self->currentState = 0;
    }

    (void)a2;
    (void)a8;
}
