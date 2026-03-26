/**
 * hud_boot.cpp — Boot Screen HUD State Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Handles the initial boot UI flow: Xbox Live sign-in, storage device
 * selection, and profile loading. Creates XAM handles for async UI
 * operations and configures network boot settings.
 */

#include "hud.hpp"
#include "rage/game_loop_types.h"

// ─────────────────────────────────────────────────────────────────────────────
// External Functions
// ─────────────────────────────────────────────────────────────────────────────

// PollButtonState @ 0x822EB2A8 — checks if any controller button is pressed
extern uint8_t PollButtonState(void* pageGroup);

// GetNetworkClient @ 0x822EB1E8 — retrieves the network client from context
extern void* GetNetworkClient(void* context);

// CopyNetworkString @ 0x82228990 — safe string copy for network buffers
extern void CopyNetworkString(const char* src, char* dest, int maxSize);

// InitiateConnection @ 0x822EB320 — begins Xbox Live connection flow
extern void InitiateConnection(void* context);

// FindSettingByKey @ 0x823F9318 — looks up a boot setting by string key
extern void* FindSettingByKey(void* client, const char* key);

// XAM_CreateHandle @ 0x82566C88 — creates an Xbox kernel event handle
extern uint32_t XAM_CreateHandle(uint32_t type, uint32_t initialCount,
                                 uint32_t maxCount, uint32_t flags);

// hudBoot_BaseDtor @ 0x823069D8 — base class destructor chain
extern void hudBoot_BaseDtor(hudBoot* self);

// ─────────────────────────────────────────────────────────────────────────────
// External Globals
// ─────────────────────────────────────────────────────────────────────────────

// g_loop_obj_ptr @ 0x825EAB30 — main game loop object
extern gameLoop* g_loop_obj_ptr;

// ─────────────────────────────────────────────────────────────────────────────
// Boot Setting Keys (binary .rdata strings)
// ─────────────────────────────────────────────────────────────────────────────

// These string keys identify boot configuration entries in the network
// client's setting table. Each key maps to a BootNumericSetting or
// BootStringSetting struct returned by FindSettingByKey.

extern const char g_bootKey_Mode[];          // @ 0x8205B2E8
extern const char g_bootKey_ErrorMessage[];  // @ 0x8205E318
extern const char g_bootKey_Saving[];        // @ 0x8205DFB8
extern const char g_bootKey_NameTemplate[];  // @ 0x8205E320 — "tourney_name_%d_%d"
extern const char g_bootKey_Name[];          // @ 0x8205E32C

// ─────────────────────────────────────────────────────────────────────────────
// Boot Setting Types
// ─────────────────────────────────────────────────────────────────────────────

namespace {

struct BootNumericSetting {
    uint32_t m_value;
    uint32_t m_type;
};

struct BootStringSetting {
    char*    m_pString;
    uint32_t m_type;
};

enum BootSettingType : uint32_t {
    BOOT_SETTING_STRING  = 1u,
    BOOT_SETTING_NUMERIC = 3u,
};

// XAM handle creation parameters (all zero = default synchronization object)
constexpr uint32_t kXamHandleType         = 0u;
constexpr uint32_t kXamHandleInitialCount = 1u;
constexpr uint32_t kXamHandleMaxCount     = 0u;
constexpr uint32_t kXamHandleFlags        = 0u;

static void SetNumericBootSetting(void* networkClient, const char* key, uint32_t value) {
    auto* setting = reinterpret_cast<BootNumericSetting*>(FindSettingByKey(networkClient, key));
    if (!setting) return;
    setting->m_value = value;
    setting->m_type = BOOT_SETTING_NUMERIC;
}

static void SetStringBootSetting(void* networkClient, const char* key, char* value) {
    auto* setting = reinterpret_cast<BootStringSetting*>(FindSettingByKey(networkClient, key));
    if (!setting) return;
    setting->m_pString = value;
    setting->m_type = BOOT_SETTING_STRING;
}

} // anonymous namespace

// ─────────────────────────────────────────────────────────────────────────────
// hudBoot Implementation
// ─────────────────────────────────────────────────────────────────────────────

/**
 * hudBoot::~hudBoot @ 0x82306C80 | size: 0x48
 *
 * Destructor — delegates to base class cleanup chain.
 */
hudBoot::~hudBoot() {
    hudBoot_BaseDtor(this);
}

/**
 * hudBoot::OnEnter @ 0x82306900 | size: 0xD8
 *
 * Called when the boot HUD state is entered. Sets up Xbox Live handles,
 * initializes boot settings (display mode, error state, save state),
 * copies the tournament name template, and optionally initiates the
 * network connection if a controller button was already pressed.
 */
void hudBoot::OnEnter() {
    // Create two XAM synchronization handles for async UI operations
    m_xboxHandle1 = XAM_CreateHandle(kXamHandleType, kXamHandleInitialCount,
                                     kXamHandleMaxCount, kXamHandleFlags);
    m_xboxHandle2 = XAM_CreateHandle(kXamHandleType, kXamHandleInitialCount,
                                     kXamHandleMaxCount, kXamHandleFlags);

    // Call base class initialization (vtable slot 1)
    ScalarDtor();

    m_bInitialized = 0;

    // Check if input was already active before entering boot state
    const bool wasInputActive = (PollButtonState(this) != 0);
    void* networkClient = GetNetworkClient(this);

    // Set display mode flag: 1 if widescreen (displayModeIdx == 1), 0 otherwise
    const uint32_t modeFlag =
        (g_loop_obj_ptr != nullptr && g_loop_obj_ptr->m_nDisplayModeIdx == 1) ? 1u : 0u;

    // Configure boot settings in the network client
    SetNumericBootSetting(networkClient, g_bootKey_Mode, modeFlag);
    SetNumericBootSetting(networkClient, g_bootKey_ErrorMessage, 0u);
    SetNumericBootSetting(networkClient, g_bootKey_Saving, 0u);

    // Copy tournament name template into local buffer for network use
    CopyNetworkString(g_bootKey_NameTemplate, m_buffer, static_cast<int>(sizeof(m_buffer)));
    SetStringBootSetting(networkClient, g_bootKey_Name, m_buffer);

    // If a button was pressed during the boot splash, skip directly to connection
    if (wasInputActive) {
        InitiateConnection(this);
    }
}
