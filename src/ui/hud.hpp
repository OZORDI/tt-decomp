/**
 * hud.hpp — HUD and Flash-based UI class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 27 classes covering the full in-game HUD system built on top of the
 * Scaleform/Flash integration layer (hudFlashBase).
 *
 * Hierarchy (all HUD classes ultimately inherit from hudFlashBase):
 *
 *   hudFlashBase             — base class for all Flash-driven HUD screens.
 *     hudBoot                — boot-sequence logo / network initialisation HUD
 *     hudCharView            — character selection view
 *     hudController          — controller overlay / prompt display
 *     hudCredits             — credits roll screen
 *     hudDialog              — generic modal dialog box
 *     hudFrontEnd            — main front-end / menu shell
 *     hudHUD                 — in-game HUD (score, stamina, etc.)
 *     hudLeaderboard         — online leaderboard screen
 *     hudLegals              — legal / age-rating screen
 *     hudList                — generic scrolling list screen
 *     hudLoadingScreen       — loading/transition screen
 *       ::interactiveStats   — nested: stats panel on loading screen
 *       ::interactiveTips    — nested: tips panel on loading screen
 *       ::interactiveUnlocks — nested: unlockables panel on loading screen
 *     hudLogosScreen         — publisher logo screen
 *     hudPause               — pause menu
 *     hudShell               — outer shell / system popup layer
 *     hudTrainingHUD         — in-game training HUD
 *     hudTrainingLoadScreen  — training-mode loading screen
 *     hudTrainingPopUp       — training tip pop-up panel
 *     hudUnlocks             — unlockable content screen
 *
 *   SpectatorHudItem         — single spectator-mode HUD element
 *   SpectatorHudItemPage     — page of spectator HUD elements
 *   SpectatorHudItemPageMgr  — manager for spectator HUD pages
 *   SpectatorHudItemWithText — spectator HUD element that carries a text label
 *
 *   uiList                   — generic UI list control (independent base)
 *
 * Virtual method conventions for hudFlashBase subclasses:
 *   slot 0 = destructor
 *   slot 1 = ScalarDtor (scalar-delete thunk)
 *   slot 2 = Update / Process (per-frame logic)          [not all classes]
 *   slot 5 = OnEnter / Initialize (called when screen is shown)
 *   slot 6 = OnExit / Close (called when screen is hidden) [not all classes]
 *
 * Sources: rtti_vtable_map | vtable_layout_map | offset_cluster_map
 *          | structured_pass5_final recomp (recomp.14–16.cpp)
 *          | orig/pseudocode/ | pong_states.cpp cross-reference
 */
#pragma once
#include <stdint.h>


// ─────────────────────────────────────────────────────────────────────────────
// hudFlashBase — base class for all Flash/Scaleform-driven HUD screens
//   Primary vtable:   0x8205AF7C
//   Secondary vtable: 0x8205AFA4  (virtual-base / MI)
//
// This class owns a Flash movie context, a page-group object, and various
// render state fields.  Field names are inferred from access-frequency
    virtual ~hudFlashBase();           // [0] @ 0x822EAD38
    virtual void BeginTransition();    // [1] @ 0x822EAFC8  reads m_pPageGroup size,
                                       //   computes float ratio, then calls Update(dt)
                                       //   (same binary as Update per PPC_WEAK alias);
                                       //   also triggers connection if already active.
                                       //   TODO: confirm exact semantics — may be
                                       //   "start page-group animated transition".
//   +0x00  vtable          — primary vtable pointer   (implicit in C++)
//   +0x04  vtable2         — secondary MI vtable ptr  (implicit in C++)
//   +0x38  m_pResource1    — pointer freed in base dtor (rage_2E18 called)
//   +0x3C  m_pResource2    — pointer freed in base dtor (rage_2E18 called)
//   +0x40  m_pInnerObject  — inner Flash context object; freed in derived dtor
//   +0x54  m_bEnabled      — uint8 visibility / active flag (checked in vfn_3)
//   +0x5C  m_pPageGroup    — pointer to SinglesNetworkClient page-group object
//   +0x74  m_pFlashCtx     — most-accessed field (62 reads); likely the Flash
//                             movie or renderer context pointer
//   +0x570 m_pExtra        — secondary resource pointer freed in derived dtor
//   +0x576 m_nExtraCount   — uint16 count for m_pExtra array
//
// The remaining field_0xXXXX entries below are placeholders from the
// auto-generated offset cluster map; they need further lifting to be named.
// ─────────────────────────────────────────────────────────────────────────────
class hudFlashBase {
public:
    virtual ~hudFlashBase();           // [0] @ 0x822EAD38
    virtual void ScalarDtor();         // [1] @ 0x822EAFC8  (scalar delete thunk)
    virtual void InnerDtor();          // [2] @ 0x8231EFD0  derived-class teardown;
                                       //   resets vtables, frees m_pInnerObject and
                                       //   m_pExtra, then calls base Flash cleanup
    virtual void Update(float dt);     // [3] @ 0x822EB058  per-frame update;
                                       //   calls SinglesNetworkClient_B2A8/BF88
    virtual void vfn_4();              // [4] @ 0x822EB0E8  TODO: input/close check
    virtual void OnExit();             // [6] @ 0x822EB148  hide/close the screen;
                                       //   adds draw bucket entry or destroys
                                       //   the secondary MI sub-object

    // Pending fields (from offset_cluster_map, high-access, not yet named):
    uint32_t     field_0x000c;  // +0x000c  R:16   (likely width or size)
    uint32_t     field_0x0010;  // +0x0010  R:7 W:5
    uint32_t     field_0x0020;  // +0x0020  R:12
    uint32_t     field_0x002c;  // +0x002c  R:13
    uint32_t     field_0x0044;  // +0x0044  R:11
    uint32_t     field_0x007c;  // +0x007c  R:16
    uint32_t     field_0x008c;  // +0x008c  R:16
    uint32_t     field_0x00a8;  // +0x00a8  R:16
    uint32_t     field_0x00ac;  // +0x00ac  R:14
    uint32_t     field_0x00b0;  // +0x00b0  R:8
    // (all remaining field_0xXXXX entries omitted here — see offset_cluster_map.json)
};


// ─────────────────────────────────────────────────────────────────────────────
// hudBoot — boot-sequence HUD  [2 vtables: 0x8205E444 / 0x8205E46C]
//
// Manages the initial boot sequence: displays publisher logos, initialises
// the SinglesNetworkClient with Xbox Live / LAN mode settings, and queues
// network configuration keys before handing off to the main front-end.
//
// Field layout (offsets relative to object start):
//   +0x00  (vtable — primary)          implicit
//   +0x04  (vtable — secondary MI)     implicit
//   +0x08..+0x3F                        hudFlashBase-inherited fields
//   +0x40  m_pInnerObject  — Flash inner context; freed in destructor
//   +0x60  m_bInitialized  — uint8  set to 0 during Initialize()
//   +0x64  m_xboxHandle1   — uint32 Xbox system handle (from XAM)
//   +0x68  m_xboxHandle2   — uint32 Xbox system handle (from XAM)
//   +0x6C  m_pNetworkClient— uint32 cleared in destructor (not owned)
//   +0x70  m_buffer[1024]  — char   network config scratch buffer
// ─────────────────────────────────────────────────────────────────────────────
class hudBoot : public hudFlashBase {
public:
    virtual ~hudBoot();          // [0] @ 0x82306988  calls rage_69D8 then free
    virtual void OnEnter();      // [5] @ 0x82306A50  creates XAM handles,
                                 //   configures SinglesNetworkClient settings
                                 //   for online/offline mode, fills m_buffer

    // hudBoot-specific fields (hudFlashBase fields at +0x00..+0x5F are inherited):
    uint8_t     m_bInitialized;  // +0x60  cleared in OnEnter; true once setup complete
    uint32_t    m_xboxHandle1;   // +0x64  Xbox system handle (XAM_CreateHandle)
    uint32_t    m_xboxHandle2;   // +0x68  Xbox system handle (XAM_CreateHandle)
    void*       m_pNetworkClient;// +0x6C  cached network client ref; zeroed in dtor
    char        m_buffer[1024];  // +0x70  scratch buffer for tourney name string



// ─────────────────────────────────────────────────────────────────────────────
// hudCharView — character selection view  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudCharView : public hudFlashBase {
public:
    virtual ~hudCharView();        // [0] @ 0x82320B20
    virtual void vfn_2();          // [2] @ 0x82320D68  TODO: Update/Process
    virtual void OnEnter();        // [5] @ 0x82320BE0
};


// ─────────────────────────────────────────────────────────────────────────────
// hudController — controller overlay / button-prompt display  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudController : public hudFlashBase {
public:
    virtual ~hudController();      // [0] @ 0x8231EE68
    virtual void OnEnter();        // [5] @ 0x8231EF28
};


// ─────────────────────────────────────────────────────────────────────────────
// hudCredits — credits roll screen  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudCredits : public hudFlashBase {
public:
    virtual ~hudCredits();         // [0] @ 0x8231F630
    virtual void vfn_2();          // [2] @ 0x823202D0  TODO: Update tick
    virtual void OnEnter();        // [5] @ 0x82320138
    virtual void OnExit();         // [6] @ 0x82320880
};


// ─────────────────────────────────────────────────────────────────────────────
// hudDialog — generic modal dialog box  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudDialog : public hudFlashBase {
public:
    virtual ~hudDialog();          // [0] @ 0x82321798
    virtual void vfn_2();          // [2] @ 0x82322090  TODO: Update / button handler
    virtual void OnEnter();        // [5] @ 0x82321858
};


// ─────────────────────────────────────────────────────────────────────────────
// hudFrontEnd — main front-end / menu shell  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudFrontEnd : public hudFlashBase {
public:
    virtual ~hudFrontEnd();        // [0] @ 0x823233C8
    virtual void vfn_2();          // [2] @ 0x823240A0  TODO: Update / navigation
};


// ─────────────────────────────────────────────────────────────────────────────
// hudHUD — in-game HUD (score, stamina, spin meters, etc.)  [3 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudHUD : public hudFlashBase {
public:
    virtual ~hudHUD();             // [0] @ 0x822EDE90
    virtual void OnEnter();        // [5] @ 0x822EF778
    virtual void OnExit();         // [6] @ 0x822F0030
};


// ─────────────────────────────────────────────────────────────────────────────
// hudLeaderboard — online leaderboard screen  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudLeaderboard : public hudFlashBase {
public:
    virtual ~hudLeaderboard();     // [0] @ 0x8232A808
    virtual void vfn_2();          // [2] @ 0x8232BC50  TODO: Update / scroll
    virtual void OnEnter();        // [5] @ 0x8232BA78
};


// ─────────────────────────────────────────────────────────────────────────────
// hudLegals — legal / age-rating screen  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudLegals : public hudFlashBase {
public:
    virtual ~hudLegals();          // [0] @ 0x82309448
    virtual void OnEnter();        // [5] @ 0x82309580
};


// ─────────────────────────────────────────────────────────────────────────────
// hudList — generic scrolling list screen  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudList : public hudFlashBase {
public:
    virtual ~hudList();            // [0] @ 0x82304F90
    virtual void vfn_2();          // [2] @ 0x82305258  TODO: Update / navigation
    virtual void OnEnter();        // [5] @ 0x82305050
};


// ─────────────────────────────────────────────────────────────────────────────
// hudLoadingScreen — loading/transition screen  [2 vtables]
//
// Owns three optional interactive panels (interactiveStats / interactiveTips /
// interactiveUnlocks) that can be shown while content streams in.
// ─────────────────────────────────────────────────────────────────────────────
class hudLoadingScreen : public hudFlashBase {
public:
    virtual ~hudLoadingScreen();   // [0]  @ 0x8231E6E0
    virtual void vfn_2();          // [2]  @ 0x8231C748  TODO: Update
    virtual void OnEnter();        // [5]  @ 0x8231B6C8
    virtual void OnExit();         // [6]  @ 0x8231C9B8
    virtual void vfn_9();          // [9]  @ 0x8231B5B0  TODO: async-load hook?
    virtual void vfn_10();         // [10] @ 0x8231CA20  TODO
    virtual void vfn_11();         // [11] @ 0x8231CC58  TODO
    virtual void vfn_12();         // [12] @ 0x8231CD20  TODO
    virtual void vfn_13();         // [13] @ 0x8231CD40  TODO
    virtual void vfn_14();         // [14] @ 0x8231B2B8  TODO: lookup callback

    // ── Nested interactive panels shown during loading ──────────────────────

    // interactiveStats — statistics panel  [vtable @ 0x82062218]
    class interactiveStats {
    public:
        virtual void vfn_2();      // [2] @ 0x8231E530
        virtual void vfn_3();      // [3] @ 0x8231E570
        virtual void vfn_4();      // [4] @ 0x8231E5E8
        virtual void vfn_5();      // [5] @ 0x8231E618
        virtual void vfn_6();      // [6] @ 0x8231E6B0
        virtual void vfn_7();      // [7] @ 0x8231E6C8
    };

    // interactiveTips — tips carousel panel  [vtable @ 0x820621C8]
    class interactiveTips {
    public:
        virtual void ScalarDtor(); // [1] @ 0x8239CA28
        virtual void vfn_2();      // [2] @ 0x8231DF60
        virtual void vfn_3();      // [3] @ 0x8231DFF8
        virtual void vfn_4();      // [4] @ 0x8231E010
        virtual void vfn_5();      // [5] @ 0x8231E028
        virtual void vfn_7();      // [7] @ 0x8231E080
    };

    // interactiveUnlocks — unlocks reveal panel  [vtable @ 0x820621F0]
    class interactiveUnlocks {
    public:
        virtual void vfn_2();      // [2] @ 0x8231E2D0
        virtual void vfn_3();      // [3] @ 0x8231E378
        virtual void vfn_4();      // [4] @ 0x8231E440
        virtual void vfn_5();      // [5] @ 0x8231E470
        virtual void vfn_6();      // [6] @ 0x8231E480
        virtual void vfn_7();      // [7] @ 0x8231E498
    };
};


// ─────────────────────────────────────────────────────────────────────────────
// hudLogosScreen — publisher logo splash screen  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudLogosScreen : public hudFlashBase {
public:
    virtual ~hudLogosScreen();     // [0] @ 0x82317E50
};


// ─────────────────────────────────────────────────────────────────────────────
// hudPause — pause menu  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudPause : public hudFlashBase {
public:
    virtual ~hudPause();           // [0] @ 0x82312C68
    virtual void vfn_2();          // [2] @ 0x823132F0  TODO: Update / navigation
    virtual void OnEnter();        // [5] @ 0x82312D28
    virtual void OnExit();         // [6] @ 0x82314018
};


// ─────────────────────────────────────────────────────────────────────────────
// hudShell — system popup / outer shell layer  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudShell : public hudFlashBase {
public:
    virtual ~hudShell();           // [0] @ 0x82316880
    virtual void vfn_2();          // [2] @ 0x82316A20  TODO: Update
    virtual void OnEnter();        // [5] @ 0x82316940
};


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingHUD — in-game training overlay  [3 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudTrainingHUD : public hudFlashBase {
public:
    virtual ~hudTrainingHUD();     // [0] @ 0x822FA110
    virtual void vfn_2();          // [2] @ 0x822FA340  TODO: Update
    virtual void OnEnter();        // [5] @ 0x822FA1B0
};


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingLoadScreen — training-mode loading screen  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudTrainingLoadScreen : public hudFlashBase {
public:
    virtual void vfn_2();          // [2]  @ 0x8231E990  TODO: Update
    virtual void OnEnter();        // [5]  @ 0x8231E730
    virtual void vfn_9();          // [9]  @ 0x8231ECA8  TODO
    virtual void vfn_10();         // [10] @ 0x8231EA20  TODO
    virtual void vfn_11();         // [11] @ 0x8231EBB0  TODO
    virtual void vfn_12();         // [12] @ 0x8231EC38  TODO
};


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingPopUp — training tip pop-up panel  [2 vtables]
// Confirmed method: Restart (from debug string)
// ─────────────────────────────────────────────────────────────────────────────
class hudTrainingPopUp : public hudFlashBase {
public:
    virtual ~hudTrainingPopUp();   // [0] @ 0x82318540
    virtual void vfn_2();          // [2] @ 0x82318B80  TODO: Update
    virtual void OnEnter();        // [5] @ 0x823185B0

    void Restart();                // non-virtual; confirmed from debug string
};


// ─────────────────────────────────────────────────────────────────────────────
// hudUnlocks — unlockable content browser  [2 vtables]
// ─────────────────────────────────────────────────────────────────────────────
class hudUnlocks : public hudFlashBase {
public:
    virtual ~hudUnlocks();         // [0] @ 0x82315180
    virtual void vfn_2();          // [2] @ 0x82315440  TODO: Update / navigation
    virtual void OnEnter();        // [5] @ 0x82315250
};


// ─────────────────────────────────────────────────────────────────────────────
// SpectatorHudItem — single element in the spectator-mode overlay
//   Vtable: 0x8205FCB8
// ─────────────────────────────────────────────────────────────────────────────
class SpectatorHudItem {
public:
    virtual void ScalarDtor();     // [1] @ 0x82311448
    virtual void vfn_2();          // [2] @ 0x82311460  TODO: Update
    virtual void vfn_3();          // [3] @ 0x823114F8  TODO: Draw / Render
    virtual void vfn_4();          // [4] @ 0x82311610  TODO: Resize / Layout
};


// ─────────────────────────────────────────────────────────────────────────────
// SpectatorHudItemPage — a page (group) of SpectatorHudItem elements
//   Vtable: 0x8205FCF8
// ─────────────────────────────────────────────────────────────────────────────
class SpectatorHudItemPage {
    // No virtual overrides identified beyond the base destructor.
};


// ─────────────────────────────────────────────────────────────────────────────
// SpectatorHudItemPageMgr — manages all SpectatorHudItemPage instances
//   Vtable: 0x8205FCAC
// ─────────────────────────────────────────────────────────────────────────────
class SpectatorHudItemPageMgr {
public:
    virtual ~SpectatorHudItemPageMgr();  // [0] @ 0x82311DB0
};


// ─────────────────────────────────────────────────────────────────────────────
// SpectatorHudItemWithText — SpectatorHudItem variant that owns a text label
//   Vtable: 0x8205FCD8
// ─────────────────────────────────────────────────────────────────────────────
class SpectatorHudItemWithText : public SpectatorHudItem {
public:
    virtual void ScalarDtor();     // [1] @ 0x823117E8
    virtual void vfn_2();          // [2] @ 0x82311800  TODO: Update (with text)
    virtual void vfn_3();          // [3] @ 0x82311898  TODO: Draw (with text)
    virtual void vfn_4();          // [4] @ 0x82311938  TODO: Resize (with text)
};


// ─────────────────────────────────────────────────────────────────────────────
// uiList — generic UI list control (standalone, does not inherit hudFlashBase)
//   Vtable: 0x82041614
// ─────────────────────────────────────────────────────────────────────────────
class uiList {
    // No virtual overrides identified in vtable_layout_map.
    // Field layout unknown.
};

