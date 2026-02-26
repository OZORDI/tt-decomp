/**
 * hud_boot.cpp — Boot HUD Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * hudBoot manages the initial boot sequence HUD elements.
 * This class uses multiple inheritance (2 vtables) and handles
 * initialization of network client settings and boot-time UI.
 */

#include <stdint.h>
#include <cstring>

// Forward declarations
extern "C" void rage_free_00C0(void* ptr);
extern "C" void rage_AD88(void* obj);
extern "C" void xam_6C88_g(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4);
extern "C" void SinglesNetworkClient_B2A8_g(void* obj);
extern "C" void SinglesNetworkClient_B1E8_g(void* obj);
extern "C" void SinglesNetworkClient_9318_g(void* obj, const char* key);
extern "C" void SinglesNetworkClient_8990_g(const char* src, void* dst, uint32_t size);
extern "C" void SinglesNetworkClient_B320_g(void* obj);

// Global pointers
extern void* g_game_obj_ptr;  // @ 0x8271A81C (r13-relative SDA global)

/**
 * hudBoot — Boot HUD class
 * 
 * Structure layout (inferred from assembly):
 *   +0x00  vtable pointer (primary)
 *   +0x04  vtable pointer (secondary, MI base)
 *   +0x40  m_pInnerObject (freed in destructor)
 *   +0x60  m_bInitialized
 *   +0x64  m_xboxHandle1
 *   +0x68  m_xboxHandle2
 *   +0x6C  m_pNetworkClient (cleared in destructor)
 *   +0x70  m_buffer[1024] (string buffer for network config)
 */
struct hudBoot {
    void*       vtable_primary;      // +0x00
    void*       vtable_secondary;    // +0x04
    uint8_t     _pad_08[56];         // +0x08..+0x3F
    void*       m_pInnerObject;      // +0x40  freed in destructor
    uint8_t     _pad_44[28];         // +0x44..+0x5F
    uint8_t     m_bInitialized;      // +0x60
    uint8_t     _pad_61[3];          // +0x61..+0x63
    uint32_t    m_xboxHandle1;       // +0x64  Xbox system handle
    uint32_t    m_xboxHandle2;       // +0x68  Xbox system handle
    uint32_t    m_pNetworkClient;    // +0x6C  cleared in destructor
    char        m_buffer[1024];      // +0x70  network config buffer
};

// Vtable addresses (from RTTI)
// Primary vtable:   0x8205E444
// Secondary vtable: 0x8205E46C

/**
 * hudBoot::~hudBoot @ 0x82306988 | size: 0x50
 * 
 * Destructor for hudBoot. Calls base class destructor (rage_69D8),
 * then optionally frees the object itself if shouldFree flag is set.
 * 
 * The base destructor (rage_69D8) handles:
 * - Freeing m_pInnerObject at +0x40
 * - Clearing m_pNetworkClient at +0x6C
 * - Calling rage_AD88 (likely base class cleanup)
 */
extern "C" void hudBoot_vfn_0(hudBoot* self, bool shouldFree) {
    // Call base class destructor
    rage_69D8(self);
    
    // If shouldFree flag is set (bit 0 of second parameter), free the object
    if (shouldFree) {
        rage_free_00C0(self);
    }
}

/**
 * rage_69D8 @ 0x823069D8 | size: 0x74
 * 
 * Base class destructor for hudBoot. This is likely a rage::GameObject
 * or similar base class destructor that:
 * 1. Sets vtable pointers to base class vtables
 * 2. Frees the inner object at +0x40
 * 3. Clears the network client pointer at +0x6C
 * 4. Calls rage_AD88 for final cleanup
 */
extern "C" void rage_69D8(hudBoot* self) {
    // Vtable addresses calculated from assembly:
    // lis r11,-32250; addi r11,r11,-7100  -> 0x8205E444
    // lis r10,-32250; addi r10,r10,-7060  -> 0x8205E46C
    
    // Set vtables to base class vtables
    self->vtable_primary = (void*)0x8205E444;
    self->vtable_secondary = (void*)0x8205E46C;
    
    // Free inner object if it exists
    void* innerObj = self->m_pInnerObject;
    if (innerObj != nullptr) {
        // Free the object's vtable target first
        void** vtable = *(void***)innerObj;
        if (vtable) {
            rage_free_00C0(vtable);
        }
        // Then free the object itself
        rage_free_00C0(innerObj);
    }
    
    // Clear network client pointer
    self->m_pNetworkClient = 0;
    
    // Call base class cleanup
    rage_AD88(self);
}

/**
 * hudBoot::Initialize @ 0x82306A50 | size: 0x164
 * 
 * Initializes the boot HUD. This function:
 * 1. Creates two Xbox system handles via xam_6C88_g
 * 2. Calls virtual method slot 1 (likely Init or Setup)
 * 3. Initializes network client settings
 * 4. Configures various network parameters based on game state
 * 5. Sets up the network config buffer
 * 
 * This is called during the boot sequence to prepare the HUD
 * for the initial logos and loading screens.
 */
extern "C" void hudBoot_vfn_5(hudBoot* self) {
    // Create Xbox system handles
    // xam_6C88_g(0, 1, 0, 0) - likely XamUserGetSigninState or similar
    uint32_t handle1 = xam_6C88_g(0, 1, 0, 0);
    self->m_xboxHandle1 = handle1;
    
    uint32_t handle2 = xam_6C88_g(0, 1, 0, 0);
    self->m_xboxHandle2 = handle2;
    
    // Call virtual method slot 1 (Init/Setup)
    void** vtable = (void**)self->vtable_primary;
    typedef void (*InitFunc)(hudBoot*);
    InitFunc initFunc = (InitFunc)vtable[1];
    initFunc(self);
    
    // Clear initialized flag
    self->m_bInitialized = 0;
    
    // Get network client state
    uint8_t isNetworkActive = SinglesNetworkClient_B2A8_g(self);
    void* networkClient = SinglesNetworkClient_B1E8_g(self);
    
    // Check game state for network configuration
    // g_game_obj_ptr is at 0x8271A81C (calculated from r13-relative load)
    struct GameState {
        uint8_t _pad[512];
        uint32_t networkMode;  // +0x200
    };
    
    GameState* gameState = (GameState*)g_game_obj_ptr;
    bool useOnlineMode = (gameState->networkMode == 1);
    
    // Configure network settings based on mode
    // String addresses calculated from assembly:
    // -19736 offset: 0x8205B2E8 (likely "NetworkMode" or similar key)
    // -7400 offset:  0x8205E318
    // -8264 offset:  0x8205DFB8
    // -7392 offset:  0x8205E320
    // -7380 offset:  0x8205E32C
    
    const char* networkModeKey = (const char*)0x8205B2E8;
    void* setting1 = SinglesNetworkClient_9318_g(networkClient, networkModeKey);
    if (setting1) {
        *(uint32_t*)((char*)setting1 + 4) = 3;  // Set value to 3
        
        // Set boolean flag based on online mode
        // cntlzw + rlwinm idiom: converts 0->1, non-zero->0
        uint32_t boolValue = useOnlineMode ? 0 : 1;
        *(uint32_t*)setting1 = boolValue;
    }
    
    // Configure additional network settings
    const char* setting2Key = (const char*)0x8205E318;
    void* setting2 = SinglesNetworkClient_9318_g(networkClient, setting2Key);
    if (setting2) {
        *(uint32_t*)setting2 = 0;
        *(uint32_t*)((char*)setting2 + 4) = 3;
    }
    
    const char* setting3Key = (const char*)0x8205DFB8;
    void* setting3 = SinglesNetworkClient_9318_g(networkClient, setting3Key);
    if (setting3) {
        *(uint32_t*)setting3 = 0;
        *(uint32_t*)((char*)setting3 + 4) = 3;
    }
    
    // Initialize network config buffer
    const char* configTemplate = (const char*)0x8205E320;
    char* buffer = self->m_buffer;
    SinglesNetworkClient_8990_g(configTemplate, buffer, 1024);
    
    // Register buffer with network client
    const char* bufferKey = (const char*)0x8205E32C;
    void* bufferSetting = SinglesNetworkClient_9318_g(networkClient, bufferKey);
    if (bufferSetting) {
        *(void**)bufferSetting = buffer;
        *(uint32_t*)((char*)bufferSetting + 4) = 1;  // Set type to 1 (string)
    }
    
    // If network is active, perform additional setup
    if (isNetworkActive) {
        SinglesNetworkClient_B320_g(self);
    }
}
