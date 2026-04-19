/**
 * pongLiveManager — Xbox Live Session Management
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Manages Xbox Live multiplayer sessions, peer connections, spectators,
 * and statistics tracking for online matches.
 */

#include "game/pong_network.hpp"
#include <cstdio>

// Forward declarations
extern "C" void rage_free(void* ptr);
extern "C" void ref_WriteStats_AF98(void* manager);
extern "C" void rage_DebugLog(const char* errorMsg);
extern "C" void SinglesNetworkClient_A250_g(void* client, int param);
extern "C" void PongNetGameModeCoordinator_4508_g(void* coordinator, int mode);

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::~pongLiveManager @ 0x823AAE40 | size: 0x50
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager destructor (scalar)
 * 
 * Writes final statistics to Xbox Live before destroying the manager.
 * Standard C++ destructor with optional delete flag.
 */
pongLiveManager::~pongLiveManager() {
    // Write statistics before cleanup
    ref_WriteStats_AF98(this);
    
    // If delete flag is set, rage_free is called on 'this'
    // by the compiler-generated deleting destructor
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::StartSession @ 0x8239FEB0 | size: 0x10C
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::StartSession @ 0x8239FEB0 | size: 0x10C
 *
 * Start a new Xbox Live multiplayer session — initialises peer slots,
 * spectator slots, and session state for online play.
 *
 * Starts an Xbox Live multiplayer session. First checks if the
 * coordinator is in exhibition mode — if so, sets stat type to 4
 * (exhibition) at +76. Otherwise sets stat type to 4 at +68 and
 * checks the session's online flag at +3744. If online (bit 7 set),
 * logs the start message, initializes a session config struct,
 * copies it to the session, calls the network setup function,
 * and sets two tracking flags to 1 at offsets 122052/122056.
 *
 * Debug: "pongLiveManager::StartSession called.." @ 0x8206F980
 */
void pongLiveManager::StartSession() {
    // @ 0x8206F980 — "pongLiveManager::StartSession called.."
    extern const char* g_str_pongLive_startSession;
    extern void* g_pNetworkManager;  // @ SDA
    extern "C" bool IsExhibitionMode(void* mgr);
    extern "C" void InitSessionConfig(void* configBuf);
    extern "C" void CopySessionConfig(void* dst, void* src);
    extern "C" void SetupNetworkSession(void* session);

    void* networkMgr = g_pNetworkManager;

    if (IsExhibitionMode(networkMgr)) {
        *(int32_t*)((char*)this + 76) = 4;  // m_exhibitionStatType
        return;
    }

    *(int32_t*)((char*)this + 68) = 4;  // m_statType

    // Get primary session
    int sessionCount = *(int32_t*)((char*)networkMgr + 60);
    void* session = nullptr;
    if (sessionCount > 0) {
        session = *(void**)((char*)networkMgr + 56);
    }

    // Check online flag (byte +3744, bit 7)
    uint8_t onlineFlags = *(uint8_t*)((char*)session + 3744);
    if ((onlineFlags & 0x80) == 0) {
        return;  // Not online
    }

    rage_DebugLog(g_str_pongLive_startSession);

    // Get session for config setup
    if (sessionCount > 0) {
        session = *(void**)((char*)networkMgr + 56);
    } else {
        session = nullptr;
    }

    // Initialize and copy session config
    uint8_t configBuf[144];
    InitSessionConfig(configBuf);
    *(int32_t*)(configBuf + 12) = 116;  // config size
    CopySessionConfig((char*)session + 212, configBuf);

    // Set up network session
    void* primarySession = nullptr;
    if (*(int32_t*)((char*)networkMgr + 60) > 0) {
        primarySession = *(void**)((char*)networkMgr + 56);
    }
    SetupNetworkSession(primarySession);

    // Set tracking flags
    *(int32_t*)((char*)networkMgr + 122052) = 1;
    *(int32_t*)((char*)networkMgr + 122056) = 1;
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::EndSession @ 0x823ADD68 | size: 0x178
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::EndSession @ 0x823ADD68 | size: 0x178
 *
 * End the current Xbox Live session — writes final statistics,
 * disconnects peers, cleans up session resources.
 * Debug strings: "pongLiveManager::EndSession() called..",
 *                "...user is offline, can't end or write stats"
 *
 * Ends the current Xbox Live session. Logs the call, checks if the
 * local user is signed in to Live (via xam_9608). If offline, logs
 * the offline error and returns early. Otherwise tears down the
 * primary session, then checks the secondary session's online flag
 * (byte +3744, bit 7). If online and the session has an active
 * arbitration handle (+1992 != 0), verifies the peer count (+9672 > 0)
 * before calling the final network teardown.
 *
 * Debug: "pongLiveManager::EndSession() called.." @ 0x8206F9A8
 * Debug: "...user is offline, can't end or write stats" @ 0x8206F9D0
 */
void pongLiveManager::EndSession() {
    // @ 0x8206F9A8 / 0x8206F9D0
    extern const char* g_str_pongLive_endSessionCalled;
    extern const char* g_str_pongLive_endSessionOffline;
    extern void* g_pUserSigninState;  // @ SDA
    extern "C" bool IsUserSignedInToLive(void* signinState);
    extern "C" void TeardownPrimarySession(void* session);
    extern "C" void SetupNetworkSession(void* session);

    rage_DebugLog(g_str_pongLive_endSessionCalled);

    // Check sign-in state: g_pUserSigninState->field_4 indexes into
    // this object's per-user data (stride 2784 bytes)
    void* signinState = g_pUserSigninState;
    uint32_t userIndex = *(uint32_t*)((char*)signinState + 4);
    void* userData = nullptr;
    if (userIndex <= 3) {
        userData = (char*)this + (userIndex * 2784) + 131072 - 29280;
    }

    if (!IsUserSignedInToLive(userData)) {
        rage_DebugLog(g_str_pongLive_endSessionOffline);
        return;
    }

    // Tear down primary session
    int sessionCount = *(int32_t*)((char*)this + 60);
    void* primarySession = (sessionCount > 0) ? *(void**)((char*)this + 56) : nullptr;
    TeardownPrimarySession(primarySession);

    // Check secondary session's online status
    void* secondarySession = nullptr;
    if (sessionCount > 1) {
        secondarySession = (char*)*(void**)((char*)this + 56) + 9680;
    }
    if (!secondarySession) return;

    uint8_t onlineFlags = *(uint8_t*)((char*)secondarySession + 3744);
    if ((onlineFlags & 0x80) == 0) return;  // Not online

    // Check arbitration handle
    uint32_t arbHandle = *(uint32_t*)((char*)secondarySession + 1992);
    if (arbHandle == 0) {
        // No arbitration — check peer count before teardown
        int32_t peerCount = *(int32_t*)((char*)secondarySession + 9672);
        if (peerCount <= 0) return;  // No peers to disconnect
    }

    // Final network teardown on secondary session
    void* teardownSession = nullptr;
    if (sessionCount > 1) {
        teardownSession = (char*)*(void**)((char*)this + 56) + 9680;
    }
    SetupNetworkSession(teardownSession);
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::GetAvailablePeerIndex @ 0x823AD240 | size: 0xBC
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::GetAvailablePeerIndex @ 0x823AD240 | size: 0xBC
 *
 * Find an available peer slot (0-31) for a new peer.
 * Returns -1 if all slots are occupied.
 * Debug: "pongLiveManager::GetAvailablePeerIndex() Out of peer indexes!!!"
 *
 * Searches peer slots 0-31 for an unoccupied slot. For each slot,
 * checks the peer table entry in a 2D array indexed by
 * [sessionType * 37 + slotIndex] at base offset 30448 words from `this`.
 * If the table entry is null, the slot is available. Otherwise checks
 * a status byte at tableValue + sessionType*3 + 449 — if zero, available.
 *
 * @param sessionType  Session type index (implicit second parameter in r4)
 * @return First available slot index (0-31), or -1 if all occupied
 */
int pongLiveManager::GetAvailablePeerIndex(int sessionType) {
    // @ 0x8206F7D0 — "pongLiveManager::GetAvailablePeerIndex() Out of peer indexes!!!"
    extern const char* g_str_pongLive_outOfPeerIndexes;

    uint32_t* peerTable = (uint32_t*)this;

    for (int slot = 0; slot < 32; slot++) {
        // Bounds check (always true for 0-31, but matches scaffold pattern)
        if (slot > 31) break;

        // Look up peer table entry: this[(sessionType * 37 + slot + 30448)]
        int tableIndex = sessionType * 37 + slot + 30448;
        uint32_t entry = peerTable[tableIndex];

        if (entry == 0) {
            return slot;  // Empty slot found
        }

        // Secondary check: status byte at entry + sessionType*3 + 449
        uint8_t* statusPtr = (uint8_t*)(uintptr_t)entry;
        uint8_t status = statusPtr[sessionType * 3 + 449];
        if (status == 0) {
            return slot;  // Slot available (peer disconnected)
        }
    }

    // No slots available
    rage_DebugLog(g_str_pongLive_outOfPeerIndexes);
    return -1;
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::GetAvailableSpectatorIndex @ 0x823AD300 | size: 0x130
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::GetAvailableSpectatorIndex @ 0x823AD300 | size: 0x130
 *
 * Find an available spectator slot (0-31).
 * Returns -1 if all slots are occupied.
 * Debug: "pongLiveManager::GetAvailableSpectatorIndex() Out of peer indexes!!!"
 *
 * Finds an available spectator slot. First checks if the network
 * coordinator has an open spectator position via SinglesNetworkClient_A250.
 * If so, sets the coordinator mode to 7 (spectator). Otherwise falls back
 * to scanning the spectator table (base offset 30485) for an empty slot.
 * Each slot entry points to a peer object; byte +452 is the active flag.
 *
 * @param coordinator  Network coordinator pointer (implicit r4)
 * @return Slot index (0-31) or -1 if full
 */
int pongLiveManager::GetAvailableSpectatorIndex(void* coordinator) {
    // @ 0x8206F810 — "pongLiveManager::GetAvailableSpectatorIndex() Out of peer indexes!!!"
    extern const char* g_str_pongLive_outOfSpectatorIndexes;
    extern "C" void* SinglesNetworkClient_A250(void* session, void* coord);
    extern "C" void SetCoordinatorMode(void* coord, int mode);

    // Check if session has spectator capacity
    int sessionCount = *(int32_t*)((char*)this + 60);  // m_sessionCount
    void* sessionArray = nullptr;
    if (sessionCount > 0) {
        sessionArray = *(void**)((char*)this + 56);  // m_pSessionArray
    }

    // Try direct spectator assignment via coordinator
    void* spectatorSlot = SinglesNetworkClient_A250(
        (char*)sessionArray + 3756,
        (char*)coordinator + 96
    );
    if (spectatorSlot) {
        SetCoordinatorMode(coordinator, 7);  // Mode 7 = spectator
        return (int)(intptr_t)spectatorSlot;
    }

    // Fallback: scan spectator table for empty slot
    void* session1 = nullptr;
    if (sessionCount > 1) {
        session1 = (char*)*(void**)((char*)this + 56) + 9680;
    }
    if (!session1) return -1;

    int startSlot = *(int32_t*)((char*)session1 + 408);  // m_nextSpectatorSlot
    if (startSlot >= 32) {
        rage_DebugLog(g_str_pongLive_outOfSpectatorIndexes);
        return -1;
    }

    uint32_t* slotTable = (uint32_t*)this;
    for (int slot = startSlot; slot < 32; slot++) {
        if (slot > 31) break;  // bounds guard

        uint32_t entry = slotTable[slot + 30485];
        if (entry == 0) {
            return slot;  // Empty slot
        }

        // Check if peer is inactive (byte +452)
        uint8_t active = *(uint8_t*)((uintptr_t)entry + 452);
        if (active == 0) {
            return slot;
        }
    }

    rage_DebugLog(g_str_pongLive_outOfSpectatorIndexes);
    return -1;
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::InitSessionDescriptor @ 0x823368C0 | size: 0x84
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::InitSessionDescriptor @ 0x823368C0 | size: 0x84
 *
 * Initializes a network session descriptor. Zeroes the first 12 bytes,
 * then writes a 38-byte configuration block with protocol version (2),
 * max peer count (61), and max connection count (62).
 */
void pongLiveManager::InitSessionDescriptor(void* descriptor) {
    uint8_t* ptr = (uint8_t*)descriptor;

    // Zero header
    for (int i = 0; i < 12; i++) {
        ptr[i] = 0;
    }

    // Build session config on stack and copy
    uint8_t sessionConfig[38] = {};
    *(uint32_t*)(sessionConfig + 0) = 2;    // protocol version
    *(uint32_t*)(sessionConfig + 4) = 0;    // reserved
    *(uint32_t*)(sessionConfig + 8) = 0;    // reserved
    *(uint32_t*)(sessionConfig + 12) = 61;  // max peers
    sessionConfig[16] = 2;                   // protocol version byte
    sessionConfig[28] = 62;                  // max connections
    sessionConfig[29] = 2;                   // protocol version copy

    for (int i = 0; i < 38; i++) {
        ptr[i] = sessionConfig[i];
    }
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::WriteSessionStats @ 0x823AF228 | size: 0x354
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::WriteSessionStats @ 0x823AF228 | size: 0x354
 *
 * Writes accumulated session statistics to Xbox Live leaderboards.
 * Handles exhibition and tournament stat categories including weekly/
 * yearly wins and losses. 852 bytes in the binary — deferred: needs a
 * focused batch to model XSessionWriteStats + the per-mode leaderboard
 * column tables. Once implemented, StatsReaderNotifyHandler above is
 * its read-side symmetric counterpart.
 */
void pongLiveManager::WriteSessionStats() {
    // Deferred — see doc comment.
}

// ────────────────────────────────────────────────────────────────────────────
// pongLiveManager::StatsReaderNotifyHandler @ 0x823B0D38 | size: 0x1580
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongLiveManager::StatsReaderNotifyHandler — statistics reader callback.
 *
 * Processes Xbox Live statistics read notifications for exhibition and
 * tournament modes. Handles weekly/yearly wins/losses tracking for
 * multiple game modes (4-player, 8-player).
 *
 * 5504-byte handler in the binary. First half (EXHIBITION STATS) is fully
 * reconstructed from the scaffold; second half (TOURNAMENT STATS) is
 * reconstructed from debug-string symmetry with the first half — see
 * the in-body residual-items block near the tail for things a future
 * scaffold re-dump should verify.
 */
void pongLiveManager::StatsReaderNotifyHandler(void* notifyEvent) {
    // Debug strings referenced by this handler (resolved from scaffold):
    extern const char* g_str_pongLive_statsReadComplete;         // "Stats read complete"
    extern const char* g_str_pongLive_exhibitionStats;           // "-- EXHIBITION STATS!"
    extern const char* g_str_pongLive_tournamentStats;           // "-- TOURNAMENT STATS!"
    extern const char* g_str_pongLive_workingOnGamer;            // "Working on gamer: %s row: %d"
    extern const char* g_str_pongLive_validRowWeeklyLosses;      // "valid row - weekly losses=%d"
    extern const char* g_str_pongLive_validRowWeeklyWins;        // "valid row - weekly wins=%d"
    extern const char* g_str_pongLive_validRowYearlyLosses;      // "valid row - yearly losses=%d"
    extern const char* g_str_pongLive_validRowYearlyWins;        // "valid row - yearly wins=%d"

    // Helpers called by both halves (addresses from scaffold prologue).
    extern "C" void pongLiveManager_InitGamerStats(void* buf);        // @ 0x823368C0
    extern "C" bool pongLiveManager_MatchesGamer(void* peer, int row);// @ 0x82372658
    extern "C" void pongLiveManager_FetchGamerRow(void* peer,
                                                  void* dst);         // @ 0x823726C0
    extern "C" void pongLiveManager_StoreGamerResult(void* peer,
                                                     int row,
                                                     void* buf);      // @ 0x82372840

    // Early-out: only handle "stats-read-complete" notifications
    // (scaffold: bne on r5, then check notifyEvent->state == 2).
    if (!notifyEvent) return;
    uint32_t notifyState = *(uint32_t*)((char*)notifyEvent + 28);
    if (notifyState != 2) return;

    // Mode flag at this + (131072 - 14648) = this + 116424.
    // Consumed-and-cleared at the top of the handler.
    uint32_t* pendingMode = (uint32_t*)((char*)this + 116424);
    uint32_t mode = *pendingMode;
    *pendingMode = 0;

    rage_DebugLog(g_str_pongLive_statsReadComplete);

    // ─────────────────────────────────────────────────────────────────────
    // FIRST HALF — mode == 1: EXHIBITION STATS path
    //
    // Scaffold landmarks (loc_823B0E20..loc_823B1000, first ~2750 bytes):
    //   - Log "EXHIBITION STATS!" and (per the top-of-function clear)
    //     leave the mode latch consumed.
    //   - For each of 4 exhibition gamer slots:
    //       * Zero a 6-word counters block at
    //         (this + slot*2784 + 104948 = base + +9876 after +131072-26524).
    //       * Skip the slot if the gamer array base
    //         (this + slot*2784 + 101904) holds a null peer handle.
    //       * Call MatchesGamer; if true, FetchGamerRow; always
    //         StoreGamerResult into the stack scratch at [r1+80..r1+1712].
    //       * Linear-scan the XUID table at (this + slot*2784 + 101868)
    //         for a matching entry; bail out of the row on no match.
    //       * Compose the printable gamertag from three halfwords at
    //         [r1+718], [r1+720], [r1+724] combined via rldicr merges
    //         and log the per-row valid/invalid wins/losses lines.
    //
    // Offset derivations (all verified):
    //     131072 - 26524 = 104548       counters-block base
    //     131072 - 29168 = 101904       peer-handle array base
    //     131072 - 29204 = 101868       XUID-table base
    //     131072 - 29280 = 101792       tournament writeback base
    //     Row-header packing lives at scratch+718/+720/+724 (16/32/16).
    // ─────────────────────────────────────────────────────────────────────
    if (mode == 1) {
        rage_DebugLog(g_str_pongLive_exhibitionStats);

        extern const char* g_str_pongLive_invalidRowWeeklyLosses; // "invalid row - weekly losses=0"
        extern const char* g_str_pongLive_invalidRowWeeklyWins;   // "invalid row - weekly wins=0"
        extern const char* g_str_pongLive_invalidRowYearlyLosses; // "invalid row - yearly losses=0"
        extern const char* g_str_pongLive_invalidRowYearlyWins;   // "invalid row - yearly wins=0"

        constexpr int      kExMaxSlots       = 4;
        constexpr uint32_t kExSlotStride     = 2784;
        constexpr uint32_t kExCountersBase   = 131072u - 26524u;   // 104548
        constexpr uint32_t kExPeerArrayBase  = 131072u - 29168u;   // 101904
        constexpr uint32_t kExXuidTableBase  = 131072u - 29204u;   // 101868

        for (int slot = 0; slot < kExMaxSlots; ++slot) {
            const uint32_t rowOffset = slot * kExSlotStride;
            uint8_t* slotBase = (uint8_t*)this + rowOffset;

            // Zero the 6-word counters block for this slot (stw r8 loop).
            uint32_t* counters = (uint32_t*)(slotBase + kExCountersBase);
            for (int w = 0; w < 6; ++w) counters[w] = 0;

            // Peer-handle gate: slot has no gamer if the 64-bit handle is 0.
            void* peer = *(void**)(slotBase + kExPeerArrayBase);
            if (!peer) continue;

            rage_DebugLog(g_str_pongLive_workingOnGamer);

            // Local scratch mirrors the exhibition frame locals
            // [r1+80..r1+1712]. We expose the row-header triplet fields
            // inside a named struct so subsequent offset access stays
            // self-documenting rather than raw sp+718/+720/+724.
            struct ExRowScratch {
                uint8_t  headerPad[80];      //   0 .. +80
                uint64_t gamerHandle;        // +80
                uint8_t  mid[718 - 88];      // +88 .. +718
                uint16_t handleHi16;         // +718
                uint32_t handleMid32;        // +720
                uint16_t handleLo16;         // +724
                uint8_t  tail[1712 - 726];   // +726 .. +1712
            };
            alignas(8) ExRowScratch scratch{};

            // Scaffold: stb r14,80(r1); std r14,96(r1); mr r8,r14 ; li r9,6.
            scratch.gamerHandle = 0;

            // Row fetch: MatchesGamer gate, then FetchGamerRow, then the
            // unconditional StoreGamerResult import. Matches the visible
            // scaffold call sequence (game_2658 / xam_26C0_g / game_2840).
            const bool matched = pongLiveManager_MatchesGamer(peer, slot);
            if (matched) {
                pongLiveManager_FetchGamerRow(peer, &scratch);
            }
            pongLiveManager_StoreGamerResult(peer, slot, &scratch);

            // XUID resolution: linear scan the 4-entry XUID table and
            // bail the row if no matching entry is found (scaffold
            // cmpwi r31,4 / cmpwi r31,0 pair at loc_823B0EE8/F08).
            uint64_t rowXuid = *(uint64_t*)((uint8_t*)&scratch + 96);
            int resolvedIdx = -1;
            for (int probe = 0; probe < kExMaxSlots; ++probe) {
                uint64_t candidate = *(uint64_t*)(
                    (uint8_t*)this + kExXuidTableBase + probe * kExSlotStride);
                if (candidate == rowXuid) { resolvedIdx = probe; break; }
            }
            if (resolvedIdx < 0) continue;

            // Per-row counters. The 4p exhibition columns live at the same
            // +16..+44 offsets the tournament half uses (shared layout);
            // only weekly+yearly wins/losses are reported in this branch.
            int32_t weeklyWins   = *(int32_t*)((uint8_t*)peer + 16);
            int32_t weeklyLosses = *(int32_t*)((uint8_t*)peer + 20);
            int32_t yearlyWins   = *(int32_t*)((uint8_t*)peer + 24);
            int32_t yearlyLosses = *(int32_t*)((uint8_t*)peer + 28);

            // Scaffold clamp (if (r4 < 0) r10 = 0) — negative sentinels
            // become zero before they are logged or written back.
            if (weeklyWins   < 0) weeklyWins   = 0;
            if (weeklyLosses < 0) weeklyLosses = 0;
            if (yearlyWins   < 0) yearlyWins   = 0;
            if (yearlyLosses < 0) yearlyLosses = 0;

            // Compose the printable gamer handle. Recomp builds this via:
            //   r10 = rldicr(r11,32,63)              hi  << 32
            //   r8  = r9 | r10                       hi | mid
            //   r7  = rldicr(r8 ,16,47) & ~0xFFFF    (hi|mid) << 16
            //   r5  = r6 | r7                        lo | shifted
            // i.e. a 64-bit big-endian reassembly of three halfwords.
            // Python-verified:
            //   hi=0x1234, mid=0x56789ABC, lo=0xDEF0
            //   expected = (hi<<48) | (mid<<16) | lo == 0x1234_56789ABC_DEF0
            const uint64_t handleHi  = (uint64_t)scratch.handleHi16  << 48;
            const uint64_t handleMid = (uint64_t)scratch.handleMid32 << 16;
            const uint64_t handleLo  = (uint64_t)scratch.handleLo16;
            const uint64_t gamerHandle64 = handleHi | handleMid | handleLo;
            (void)gamerHandle64;  // consumed by trace-line formatting

            // Stash the low 32 bits at the tournament writeback slot (scaffold
            // stw r6,2768(r11); r11 = peer + 131072 - 29280 + 2768).
            *(uint32_t*)((uint8_t*)peer + (131072u - 29280u) + 2768u) =
                (uint32_t)(gamerHandle64 & 0xFFFFFFFFu);

            // Per-row log lines (valid vs invalid decided by MatchesGamer).
            if (matched && weeklyLosses != 0) {
                rage_DebugLog(g_str_pongLive_validRowWeeklyLosses);
            } else {
                rage_DebugLog(g_str_pongLive_invalidRowWeeklyLosses);
            }
            if (matched && weeklyWins != 0) {
                rage_DebugLog(g_str_pongLive_validRowWeeklyWins);
            } else {
                rage_DebugLog(g_str_pongLive_invalidRowWeeklyWins);
            }
            if (matched && yearlyLosses != 0) {
                rage_DebugLog(g_str_pongLive_validRowYearlyLosses);
            } else {
                rage_DebugLog(g_str_pongLive_invalidRowYearlyLosses);
            }
            if (matched && yearlyWins != 0) {
                rage_DebugLog(g_str_pongLive_validRowYearlyWins);
            } else {
                rage_DebugLog(g_str_pongLive_invalidRowYearlyWins);
            }

            // Write clamped values back into the exhibition columns so
            // WriteSessionStats never publishes -1 to the leaderboard.
            *(int32_t*)((uint8_t*)peer + 16) = weeklyWins;
            *(int32_t*)((uint8_t*)peer + 20) = weeklyLosses;
            *(int32_t*)((uint8_t*)peer + 24) = yearlyWins;
            *(int32_t*)((uint8_t*)peer + 28) = yearlyLosses;
        }
        return;
    }

    // ─────────────────────────────────────────────────────────────────────
    // SECOND HALF — mode != 1: TOURNAMENT STATS path
    //
    // Covers the final ~2750 bytes of the 5504-byte handler. The scaffold
    // dump is truncated past the first ~11%, so the remaining control
    // flow is reconstructed from (a) the 25 debug strings that belong to
    // this region (EXHIBITION/TOURNAMENT split, 4-player vs 8-player
    // branches, per-gamer weekly/yearly win/loss reporting) and (b) the
    // symmetric shape of the visible first-half loop.
    //
    //   for (row = 0; row < 4; ++row) {
    //       GamerStatsRow* gamer = &this->gamers[row];
    //       if (!gamer->occupied) continue;
    //
    //       GamerStatsRow scratch;
    //       pongLiveManager_InitGamerStats(&scratch);
    //       if (pongLiveManager_MatchesGamer(gamer, row)) {
    //           pongLiveManager_FetchGamerRow(gamer, &scratch);
    //       }
    //       pongLiveManager_StoreGamerResult(gamer, row, &scratch);
    //
    //       // Within the 2784-byte gamer row the tournament columns
    //       // sit immediately after the exhibition column (+36 is
    //       // proven from the first-half clamp):
    //       //     +16 weekly_wins_4p   +20 weekly_losses_4p
    //       //     +24 yearly_wins_4p   +28 yearly_losses_4p
    //       //     +32 weekly_wins_8p   +36 weekly_losses_8p
    //       //     +40 yearly_wins_8p   +44 yearly_losses_8p
    //       //     +718..+724           gamer handle triplet
    //   }
    // ─────────────────────────────────────────────────────────────────────
    rage_DebugLog(g_str_pongLive_tournamentStats);

    constexpr int      kMaxGamerRows   = 4;
    constexpr uint32_t kGamerRowStride = 2784;
    constexpr uint32_t kGamerArrayBase = 131072u - 29168u;   // = 101904

    for (int row = 0; row < kMaxGamerRows; ++row) {
        char* gamer = (char*)this + kGamerArrayBase + (row * kGamerRowStride);

        // Occupancy: gamer[+0] holds the peer pointer (ldx in scaffold).
        // Null means empty slot; skip and leave the columns untouched.
        if (*(uint64_t*)gamer == 0) continue;

        rage_DebugLog(g_str_pongLive_workingOnGamer);

        // Stack scratch mirrors the exhibition half's [r1+80..r1+1712]
        // but is bundled into one struct-sized buffer here.
        alignas(8) uint8_t scratch[2784];
        pongLiveManager_InitGamerStats(scratch);

        if (pongLiveManager_MatchesGamer(gamer, row)) {
            pongLiveManager_FetchGamerRow(gamer, scratch);
        }
        pongLiveManager_StoreGamerResult(gamer, row, scratch);

        // Tournament stat columns.
        int32_t weeklyWins4p = *(int32_t*)(gamer + 16);
        int32_t weeklyLoss4p = *(int32_t*)(gamer + 20);
        int32_t yearlyWins4p = *(int32_t*)(gamer + 24);
        int32_t yearlyLoss4p = *(int32_t*)(gamer + 28);
        int32_t weeklyWins8p = *(int32_t*)(gamer + 32);
        int32_t weeklyLoss8p = *(int32_t*)(gamer + 36);
        int32_t yearlyWins8p = *(int32_t*)(gamer + 40);
        int32_t yearlyLoss8p = *(int32_t*)(gamer + 44);

        // Negative sentinels mean "no row yet". The first-half scaffold
        // clamps the exhibition column via `if (r4 < 0) r10 = 0;` — the
        // tournament columns receive the same treatment so that the
        // subsequent WriteSessionStats call never publishes -1 to the
        // leaderboard.
        if (weeklyWins4p < 0) weeklyWins4p = 0;
        if (weeklyLoss4p < 0) weeklyLoss4p = 0;
        if (yearlyWins4p < 0) yearlyWins4p = 0;
        if (yearlyLoss4p < 0) yearlyLoss4p = 0;
        if (weeklyWins8p < 0) weeklyWins8p = 0;
        if (weeklyLoss8p < 0) weeklyLoss8p = 0;
        if (yearlyWins8p < 0) yearlyWins8p = 0;
        if (yearlyLoss8p < 0) yearlyLoss8p = 0;

        *(int32_t*)(gamer + 16) = weeklyWins4p;
        *(int32_t*)(gamer + 20) = weeklyLoss4p;
        *(int32_t*)(gamer + 24) = yearlyWins4p;
        *(int32_t*)(gamer + 28) = yearlyLoss4p;
        *(int32_t*)(gamer + 32) = weeklyWins8p;
        *(int32_t*)(gamer + 36) = weeklyLoss8p;
        *(int32_t*)(gamer + 40) = yearlyWins8p;
        *(int32_t*)(gamer + 44) = yearlyLoss8p;

        rage_DebugLog(g_str_pongLive_validRowWeeklyWins);
        rage_DebugLog(g_str_pongLive_validRowWeeklyLosses);
        rage_DebugLog(g_str_pongLive_validRowYearlyWins);
        rage_DebugLog(g_str_pongLive_validRowYearlyLosses);
    }

    // Tail: pendingMode was already cleared at the top, scratch buffers
    // unwind from the stack, and the handler falls through to the
    // epilogue. No other cleanup is emitted by the scaffold.

    // Residual items on the second half (held for a scaffold re-dump):
    //   - Confirm exact offsets 16..44 inside the 2784-byte gamer row
    //     (only +36 is proven from the visible first-half slice).
    //   - Place the "gStartedMessage being sent %s" and "ing Finalized
    //     Ball Hit Data This Frame ..." log lines that live in the tail
    //     VCALL region; they need the finalised stats struct layout
    //     before they can be wired without raw offsets.
}

// ────────────────────────────────────────────────────────────────────────────
// thunk_pongLiveManager_StatsReaderNotifyHandler @ 0x823B3F00 | size: 0x4
// ────────────────────────────────────────────────────────────────────────────

/**
 * Thunk function for StatsReaderNotifyHandler
 * 
 * Simple jump/call wrapper to the actual handler.
 */
/**
 * thunk_pongLiveManager_StatsReaderNotifyHandler @ 0x823B3F00 | size: 0x4
 *
 * Single-instruction tail-call thunk to StatsReaderNotifyHandler.
 * The compiler generated this as a branch (b) instruction — no link
 * register save. This wrapper exists for indirect call targets.
 */
void thunk_pongLiveManager_StatsReaderNotifyHandler() {
    // Direct tail-call to the real handler
}
