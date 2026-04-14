/**
 * dialog_loader.cpp — Dialog data loader (pongDialogState bootstrap)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifts rage_C960 @ 0x822EC960 (size 0x22c) — the one-shot loader invoked
 * from xe_EAA0 during the pong boot sequence that ingests the per-language
 * "$/tune/dialogs/dialogs_<lang>" XML, walks the resulting xmlTree node list,
 * and populates the game's 210-slot dialog-ID lookup table. After the XML is
 * parsed the loader iterates slots 1..209 and emits a formatted error for
 * every slot that was not filled.
 *
 * String anchors (all verified against orig/434C4803/default_base.bin):
 *   0x8205AFB8  "Loading dialog data..."              g_str_dialog_loadingData
 *   0x8205AFD0  "$/tune/dialogs"                      g_str_dialog_tuneDialogsPath
 *   0x8205AFE0  "dialogs_%s"                          g_str_dialog_fmt_dialogsFile
 *   0x8205AFEC  "Invalid line count %d, for dialog %d [%s]"
 *                                                     g_str_dialog_invalidLineCount
 *   0x8205B018  "Dialog data load done..."            g_str_dialog_loadDone
 *   0x8205B0AC  "dialog_title_%d"                     g_str_dialog_fmt_dialogTitle
 */

#include <cstdint>
#include <cstdio>

// Dialog string externs — defined in src/game/dialog_strings.cpp
extern const char* g_str_dialog_Dialog;
extern const char* g_str_dialog_loadingData;
extern const char* g_str_dialog_loadDone;
extern const char* g_str_dialog_tuneDialogsPath;
extern const char* g_str_dialog_fmt_dialogsFile;
extern const char* g_str_dialog_invalidLineCount;

// ────────────────────────────────────────────────────────────────────────────
// External runtime helpers (referenced by rage_C960)
// ────────────────────────────────────────────────────────────────────────────

// Debug/telemetry console print — invoked with the current format string in r3
// and up to four extra args. Matches the pass5 callsite `nop_8240E6D0`.
extern "C" void rage_PrintDebugLine(const char* fmt, ...);                // @ 0x8240E6D0

// Singleton accessors used to fetch the xmlTree vtable shim and the
// dialogData vtable at runtime.
extern "C" void* atSingleton_GetXmlTreeReader();                          // @ 0x821A8588
extern "C" void* atSingleton_GetDialogDataVtable();                       // @ 0x820C29E0

// Destructor invoked on the pre-existing dialogData::vtable object before the
// new instance is installed — matches `rage_free_00C0` (a virtual release
// trampoline driven off vtable slot 0).
extern "C" void rage_ReleaseObject(void* obj);                            // @ 0x820C00C0

// Factory lookup used to resolve the active language index into an 8-char
// language suffix (e.g. "en", "fr", "de") that is then baked into the
// "dialogs_%s" filename.
extern "C" void atFactory_GetLanguageFactory(void* factory);              // @ 0x822E2E60

// xmlTree virtual slot 1 — the "load + parse XML under this base path into
// this tree" entry point. Signature matches the CRT snprintf convention
// already established in sibling loaders (tree, filename, base-path, mode).
extern "C" void xmlTree_LoadFromPath(void* tree, const char* filename,
                                     const char* basePath, int mode);     // @ 0x821A8180

// atString 1024-char copy helper used to widen the line into the final
// diagnostic message buffer.
extern "C" void rage_atStringCopy(char* dst, const char* src, int cap);   // @ 0x82228990

// atSingleton tracked-destroy for the xmlTree factory shim stored at
// sp+96 during the load.
extern "C" void atSingleton_ReleaseTracked(void* slot);                   // @ 0x821A9420

// Stack xmlTree destructor.
extern "C" void xmlTree_Destroy(void* tree);                              // @ 0x821A8070

// ────────────────────────────────────────────────────────────────────────────
// Vtable / global anchors consumed by the loader
// ────────────────────────────────────────────────────────────────────────────

extern "C" void* __vft_rage_xmlTree;    // const rage::xmlTree::`vftable' @ 0x821E0828 area
extern "C" void* __vft_dialogData;      // const dialogData::`vftable'

// Per-dialog error format string at 0x820327E4 — "Exiting Singles state %s, time %s"
// shares .rdata space with many other telemetry strings; the dialog loader
// reuses the slot as its unfilled-slot diagnostic prefix.
extern "C" const char* g_str_dialog_exitStateFmt;                         // @ 0x820327E4

// MaxSameShotInARoll assert format @ 0x82042330 — used as a negative-count
// diagnostic header when a dialog slot has count <= 0.
extern "C" const char* g_str_dialog_negativeCountHdr;                     // @ 0x82042330

// Global language-factory registry singleton at 0x825D0080.
extern "C" void* g_LanguageFactoryRegistry;                               // @ 0x825D0080

// Pointer to the 16-entry language-suffix table lookup (indexed by the
// factory's active-language slot at +52).
extern "C" const char* const* g_LanguageSuffixTable;                      // @ 0x82041D04

// ────────────────────────────────────────────────────────────────────────────
// xmlTree node shape (only the fields the loader touches)
// ────────────────────────────────────────────────────────────────────────────
struct xmlTreeNode {
    void*           m_vtable;       // +0x00
    uint32_t        m_pad04;        // +0x04
    xmlTreeNode*    m_pNext;        // +0x08 — sibling list pointer
    uint32_t        m_dialogIndex;  // +0x10 — 1..209 slot id
    uint32_t        m_lineCount;    // +0x14 — parsed line count
};

struct xmlTreeHandle {
    void*           m_vtable;       // +0x00 (installed: rage::xmlTree::vftable)
    uint32_t        m_pad04;
    void*           m_pPad08;
    xmlTreeNode*    m_pHead;        // +0x0C — first parsed dialog entry
    // ... remainder is stack scratch used by xmlTree_LoadFromPath
};

// dialogData layout — the 840-byte global dialog table. Offsets come from the
// two stwx/lwz patterns in rage_C960:
//   +0x00..0x1B  preamble (vtable, flags, owner, ...)
//   +0x1C..0x34C 210 u32 line-count slots (indexed 0..209)
//   +0x350..0x685 parallel payload-pointer array (stored via rlwinm ,,2,,29)
struct dialogData {
    void*    m_vtable;               // +0x000 — set to new dialogData vtable
    uint32_t m_flags;                // +0x004 — telemetry flags (stored 0x1C @ +4)
    uint32_t m_reserved[5];          // +0x008..0x01B
    uint32_t m_lineCount[210];       // +0x01C — slot 0 unused, 1..209 valid
    void*    m_payload[210];         // +0x350 — matching payload pointers
};

// ────────────────────────────────────────────────────────────────────────────
// LoadDialogData @ 0x822EC960
//
// One-shot dialog table loader. Called exactly once from xe_EAA0 (boot path).
// Flow:
//   1. Emit "Loading dialog data..." telemetry line.
//   2. Acquire the xmlTree reader singleton + dialogData vtable singleton.
//   3. Release the previous dialogData vtable (vtable-slot-0 virtual free)
//      and install the new vtable pointer / reset preamble fields.
//   4. Build the per-language filename "dialogs_<lang>" via snprintf, keyed
//      off the language factory's active-slot index.
//   5. Invoke xmlTree::vfn_1 to parse "$/tune/dialogs/dialogs_<lang>".
//   6. Walk the parsed sibling list. For each node:
//        * Ask the node's vtable slot 20 whether it is a dialog entry.
//        * If yes and its dialogIndex is in [1, 209], stash the line count
//          into m_payload[index + 6] of the dialogData table.
//        * Otherwise, ask vtable slot 19 for a diagnostic string and route
//          it through the "Exiting state" telemetry channel.
//   7. Iterate the 210-slot line-count array. Every slot with count <= 0
//      synthesizes the "Invalid line count %d, for dialog %d [%s]" error
//      (widened to a 1024-char atString) and emits it.
//   8. Emit "Dialog data load done...", release the xmlTree singleton
//      reference, destroy the local xmlTree, return.
// ────────────────────────────────────────────────────────────────────────────
void LoadDialogData(dialogData* pDialogTable) {
    // (1) Announce the load.
    rage_PrintDebugLine(g_str_dialog_loadingData);

    // (2) Local xmlTree handle allocated on the stack (sp+80 in the scaffold).
    //     Zero-initialize the preamble exactly as the original does with
    //     interleaved stw/sth instructions.
    xmlTreeHandle tree;
    tree.m_vtable  = &__vft_rage_xmlTree;
    tree.m_pad04   = static_cast<uint32_t>(-1);
    tree.m_pPad08  = nullptr;
    tree.m_pHead   = nullptr;

    void* xmlReader      = atSingleton_GetXmlTreeReader();
    void* newDialogVft   = atSingleton_GetDialogDataVtable();

    // (3) Virtually destruct the pre-existing vtable object, then overwrite
    //     the dialog table's vtable pointer and reset the preamble flag word.
    rage_ReleaseObject(pDialogTable->m_vtable);
    pDialogTable->m_vtable = newDialogVft;
    pDialogTable->m_flags  = 0;
    // Install the "Dialog" tag pointer stored at +0x04 in the original —
    // offset -14480 from the 0x8270 anchor resolves to the "Dialog" literal.
    // We reuse the already-declared dialog tag extern.
    *reinterpret_cast<const char**>(
        reinterpret_cast<char*>(pDialogTable) + 4) = g_str_dialog_Dialog;

    // (4) Resolve the active language index, then format "dialogs_<suffix>"
    //     into a 64-char local buffer.
    atFactory_GetLanguageFactory(&g_LanguageFactoryRegistry);
    struct LangFactory { uint32_t pad[13]; uint32_t activeSlot; };
    LangFactory* factory = *reinterpret_cast<LangFactory**>(
        reinterpret_cast<char*>(&g_LanguageFactoryRegistry) - 23708 + 0x80);
    const char* langSuffix = g_LanguageSuffixTable[factory->activeSlot];

    char filenameBuf[64];
    std::snprintf(filenameBuf, sizeof(filenameBuf),
                  g_str_dialog_fmt_dialogsFile, langSuffix);

    // (5) Parse the XML into the local tree.
    xmlTree_LoadFromPath(&tree, filenameBuf, g_str_dialog_tuneDialogsPath, 1);

    // Decrement the language-factory active-slot refcount (the "lwz/addi -1/stw"
    // sequence at +0x600 from the factory base).
    uint32_t* refcount = reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(&g_LanguageFactoryRegistry) + 1536);
    (*refcount)--;

    // (6) Walk the parsed sibling list.
    for (xmlTreeNode* node = tree.m_pHead; node != nullptr; node = node->m_pNext) {
        using IsDialogFn = bool (*)(xmlTreeNode*);
        using GetNameFn  = const char* (*)(xmlTreeNode*);

        void** nodeVft = reinterpret_cast<void**>(node->m_vtable);
        IsDialogFn isDialog = reinterpret_cast<IsDialogFn>(nodeVft[20]);

        if (isDialog(node)) {
            // In-range dialog entry — stash the line count into
            // m_payload[dialogIndex + 6]. Out-of-range entries silently fall
            // through (the original `bgt cr6, loc_822ECAF0`).
            uint32_t idx = node->m_dialogIndex - 1;
            if (idx <= 208) {
                pDialogTable->m_payload[idx + 6] =
                    reinterpret_cast<void*>(
                        static_cast<uintptr_t>(node->m_lineCount));
            }
        } else {
            // Non-dialog entry — route the node's name through the
            // "Exiting state" telemetry channel.
            GetNameFn getName = reinterpret_cast<GetNameFn>(nodeVft[19]);
            const char* name = getName(node);
            char scratch[64];
            std::snprintf(scratch, sizeof(scratch), "%s", name);
            rage_PrintDebugLine(g_str_dialog_exitStateFmt, scratch);
        }
    }

    // (7) Validate every dialog slot 1..209 — anything still <= 0 gets an
    //     "Invalid line count %d, for dialog %d [%s]" diagnostic.
    for (int slot = 1; slot < 210; ++slot) {
        if (static_cast<int32_t>(pDialogTable->m_lineCount[slot]) > 0) {
            continue;
        }

        // Build the two-part diagnostic exactly like the asm: first snprintf
        // the header into a 16-byte scratch, then atStringCopy it into a
        // 1024-char buffer that is handed to the logger together with the
        // current line-count value.
        char hdr[16];
        std::snprintf(hdr, sizeof(hdr), g_str_dialog_negativeCountHdr, slot);

        char msgBuf[1024];
        rage_atStringCopy(msgBuf, hdr, static_cast<int>(sizeof(msgBuf)));

        rage_PrintDebugLine(g_str_dialog_invalidLineCount,
                            pDialogTable->m_lineCount[slot],
                            slot,
                            msgBuf);
    }

    // (8) Announce completion and tear down the local xmlTree.
    rage_PrintDebugLine(g_str_dialog_loadDone);
    atSingleton_ReleaseTracked(&xmlReader);
    xmlTree_Destroy(&tree);
}
