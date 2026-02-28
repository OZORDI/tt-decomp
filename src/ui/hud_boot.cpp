#include "hud.hpp"

#include "game/pong_network_io.hpp"
#include "rage/game_loop_types.h"

// SinglesNetworkClient_9318_g @ 0x823F9318
extern void* SinglesNetworkClient_9318_g(void* client, const char* key);

namespace {


struct BootNumericSetting {
    uint32_t m_value;
    uint32_t m_type;
};

struct BootStringSetting {
    char* m_pString;
    uint32_t m_type;
};

enum BootSettingType : uint32_t {
    BOOT_SETTING_STRING = 1u,
    BOOT_SETTING_NUMERIC = 3u,
};

constexpr uint32_t kXamHandleType = 0u;
constexpr uint32_t kXamHandleInitialCount = 1u;
constexpr uint32_t kXamHandleMaxCount = 0u;
constexpr uint32_t kXamHandleFlags = 0u;

// .rdata keys used by hudBoot_vfn_5.
static const char* const k_bootModeKey = reinterpret_cast<const char*>(0x8205B2E8);
static const char* const k_bootEyMsgKey = reinterpret_cast<const char*>(0x8205E318);
static const char* const k_bootSavingKey = reinterpret_cast<const char*>(0x8205DFB8);
static const char* const k_bootNameTemplate = reinterpret_cast<const char*>(0x8205E320);
static const char* const k_bootNameKey = reinterpret_cast<const char*>(0x8205E32C);

static void SetNumericBootSetting(void* networkClient, const char* key, uint32_t value) {
    BootNumericSetting* setting =
        reinterpret_cast<BootNumericSetting*>(SinglesNetworkClient_9318_g(networkClient, key));
    if (setting == nullptr) {
        return;
    }

    setting->m_value = value;
    setting->m_type = BOOT_SETTING_NUMERIC;
}

static void SetStringBootSetting(void* networkClient, const char* key, char* value) {
    BootStringSetting* setting =
        reinterpret_cast<BootStringSetting*>(SinglesNetworkClient_9318_g(networkClient, key));
    if (setting == nullptr) {
        return;
    }

    setting->m_pString = value;
    setting->m_type = BOOT_SETTING_STRING;
}

}  // namespace

// @ 0x823069D8
extern void hudBoot_BaseDtor(hudBoot* self);

// xam_6C88_g @ 0x82566C88
extern uint32_t XAM_CreateHandle(uint32_t type, uint32_t initialCount,
                                 uint32_t maxCount, uint32_t flags);


// g_loop_obj_ptr @ 0x825EAB30
extern gameLoop* g_loop_obj_ptr;

hudBoot::~hudBoot() {
    hudBoot_BaseDtor(this);
}

void hudBoot::OnEnter() {
    // Allocate two XAM handles used by the boot UI flow.
    m_xboxHandle1 = XAM_CreateHandle(kXamHandleType, kXamHandleInitialCount,
                                     kXamHandleMaxCount, kXamHandleFlags);
    m_xboxHandle2 = XAM_CreateHandle(kXamHandleType, kXamHandleInitialCount,
                                     kXamHandleMaxCount, kXamHandleFlags);

    // Preserve the original slot-1 virtual call from hudBoot_vfn_5.
    ScalarDtor();

    m_bInitialized = 0;

    const bool wasInputActive = (PollButtonState(this) != 0);
    void* networkClient = GetNetworkClient(this);

    // PPC logic writes 1 only when the +0x200 gameLoop field equals 1.
    const uint32_t modeFlag =
        (g_loop_obj_ptr != nullptr && g_loop_obj_ptr->m_nDisplayModeIdx == 1) ? 1u : 0u;

    SetNumericBootSetting(networkClient, k_bootModeKey, modeFlag);
    SetNumericBootSetting(networkClient, k_bootEyMsgKey, 0u);
    SetNumericBootSetting(networkClient, k_bootSavingKey, 0u);

    CopyNetworkString(k_bootNameTemplate, m_buffer, static_cast<int>(sizeof(m_buffer)));
    SetStringBootSetting(networkClient, k_bootNameKey, m_buffer);

    if (wasInputActive) {
        InitiateConnection(this);
    }
}

