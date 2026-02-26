/**
 * XAM Input API Implementation - SDL2 Backend
 * Rockstar Presents Table Tennis
 * 
 * Cross-platform implementation of Xbox 360 XInput and XamUser APIs using SDL2.
 * Maps Xbox 360 controller indices to SDL2 gamepad/joystick indices.
 * Provides fake user profiles for local play without Xbox Live.
 */

#include "xam/input.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

// Maximum controllers supported (Xbox 360 standard)
#define MAX_CONTROLLERS 4

// Controller state tracking
static struct {
    SDL_GameController* controller;
    SDL_Haptic* haptic;
    uint32_t packetNumber;
    uint8_t connected;
} g_controllers[MAX_CONTROLLERS] = {0};

// Initialization flag
static uint8_t g_initialized = 0;

/**
 * Initialize SDL2 gamepad subsystem if needed
 */
static void EnsureInitialized(void) {
    if (g_initialized) {
        return;
    }
    
    // Initialize SDL2 gamepad subsystem
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0) {
        fprintf(stderr, "XamInput: Failed to initialize SDL2 gamepad: %s\n", SDL_GetError());
        return;
    }
    
    // Open all available controllers
    for (int i = 0; i < MAX_CONTROLLERS && i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            g_controllers[i].controller = SDL_GameControllerOpen(i);
            if (g_controllers[i].controller) {
                g_controllers[i].connected = 1;
                g_controllers[i].packetNumber = 0;
                
                // Try to open haptic device for rumble
                SDL_Joystick* joystick = SDL_GameControllerGetJoystick(g_controllers[i].controller);
                if (joystick && SDL_JoystickIsHaptic(joystick)) {
                    g_controllers[i].haptic = SDL_HapticOpenFromJoystick(joystick);
                    if (g_controllers[i].haptic) {
                        SDL_HapticRumbleInit(g_controllers[i].haptic);
                    }
                }
                
                printf("XamInput: Controller %d connected: %s\n", i, 
                       SDL_GameControllerName(g_controllers[i].controller));
            }
        }
    }
    
    g_initialized = 1;
}

/**
 * XamInputGetState @ 0x825860FC | ordinal: 0x0191 (401)
 * 
 * Retrieves the current state of the specified controller.
 */
uint32_t XamInputGetState(uint32_t dwUserIndex, uint32_t dwFlags, XINPUT_STATE* pState) {
    EnsureInitialized();
    
    // Validate parameters
    if (dwUserIndex >= MAX_CONTROLLERS || !pState) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    
    // Check if controller is connected
    if (!g_controllers[dwUserIndex].connected || !g_controllers[dwUserIndex].controller) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    
    SDL_GameController* controller = g_controllers[dwUserIndex].controller;
    
    // Clear state
    memset(pState, 0, sizeof(XINPUT_STATE));
    
    // Update SDL2 events to get latest controller state
    SDL_GameControllerUpdate();
    
    // Map buttons
    uint16_t buttons = 0;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
        buttons |= XINPUT_GAMEPAD_DPAD_UP;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
        buttons |= XINPUT_GAMEPAD_DPAD_DOWN;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
        buttons |= XINPUT_GAMEPAD_DPAD_LEFT;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
        buttons |= XINPUT_GAMEPAD_DPAD_RIGHT;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START))
        buttons |= XINPUT_GAMEPAD_START;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
        buttons |= XINPUT_GAMEPAD_BACK;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK))
        buttons |= XINPUT_GAMEPAD_LEFT_THUMB;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
        buttons |= XINPUT_GAMEPAD_RIGHT_THUMB;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
        buttons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
        buttons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
        buttons |= XINPUT_GAMEPAD_A;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
        buttons |= XINPUT_GAMEPAD_B;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
        buttons |= XINPUT_GAMEPAD_X;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
        buttons |= XINPUT_GAMEPAD_Y;
    
    pState->Gamepad.wButtons = buttons;
    
    // Map triggers (SDL returns 0-32767, XInput expects 0-255)
    int16_t leftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    int16_t rightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    pState->Gamepad.bLeftTrigger = (uint8_t)((leftTrigger * 255) / 32767);
    pState->Gamepad.bRightTrigger = (uint8_t)((rightTrigger * 255) / 32767);
    
    // Map analog sticks (SDL returns -32768 to 32767, XInput expects same)
    pState->Gamepad.sThumbLX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    pState->Gamepad.sThumbLY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    pState->Gamepad.sThumbRX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    pState->Gamepad.sThumbRY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
    
    // Increment packet number if state changed
    g_controllers[dwUserIndex].packetNumber++;
    pState->dwPacketNumber = g_controllers[dwUserIndex].packetNumber;
    
    return ERROR_SUCCESS;
}

/**
 * XamInputSetState @ 0x8258610C | ordinal: 0x0192 (402)
 * 
 * Sets the vibration state of the specified controller.
 */
uint32_t XamInputSetState(uint32_t dwUserIndex, XINPUT_VIBRATION* pVibration) {
    EnsureInitialized();
    
    // Validate parameters
    if (dwUserIndex >= MAX_CONTROLLERS || !pVibration) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    
    // Check if controller is connected
    if (!g_controllers[dwUserIndex].connected || !g_controllers[dwUserIndex].controller) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }
    
    // Check if haptic is available
    if (!g_controllers[dwUserIndex].haptic) {
        return ERROR_NOT_SUPPORTED;
    }
    
    // Convert XInput motor speeds (0-65535) to SDL2 rumble strength (0.0-1.0)
    // Use average of both motors since SDL2 rumble is simpler
    float strength = ((float)pVibration->wLeftMotorSpeed + (float)pVibration->wRightMotorSpeed) / (2.0f * 65535.0f);
    
    // Set rumble (duration = 1000ms, will be updated next frame)
    if (SDL_HapticRumblePlay(g_controllers[dwUserIndex].haptic, strength, 1000) < 0) {
        fprintf(stderr, "XamInput: Failed to set rumble for controller %d: %s\n", 
                dwUserIndex, SDL_GetError());
        return ERROR_NOT_SUPPORTED;
    }
    
    return ERROR_SUCCESS;
}

/**
 * XamGetSystemVersion @ 0x825860CC | ordinal: 0x0282 (642)
 * 
 * Returns a fake Xbox 360 system version for compatibility.
 */
uint32_t XamGetSystemVersion(void) {
    // Return Xbox 360 kernel version 2.0.17559.0 (common retail version)
    // Format: 0xMMMMmmmm where M=major (2), m=minor (17559)
    return 0x00024497;
}

/**
 * XamNotifyCreateListener @ 0x8258611C | ordinal: 0x028A (650)
 * 
 * Stubbed - notification system not needed for cross-platform
 */
uint32_t XamNotifyCreateListener(uint64_t qwAreas, uint32_t fExternal, void** phListener) {
    (void)qwAreas;
    (void)fExternal;
    
    if (phListener) {
        *phListener = NULL;
    }
    
    return ERROR_SUCCESS;
}

// Xbox Live UI stubs - these are no-ops for cross-platform builds

uint32_t XamShowSigninUI(uint32_t cPanes, uint32_t dwFlags) {
    (void)cPanes;
    (void)dwFlags;
    printf("XamInput: XamShowSigninUI called (stubbed)\n");
    return ERROR_SUCCESS;
}

uint32_t XamShowFriendsUI(uint32_t dwUserIndex) {
    (void)dwUserIndex;
    printf("XamInput: XamShowFriendsUI called (stubbed)\n");
    return ERROR_SUCCESS;
}

uint32_t XamShowGamerCardUIForXUID(uint32_t dwUserIndex, uint64_t xuid) {
    (void)dwUserIndex;
    (void)xuid;
    printf("XamInput: XamShowGamerCardUIForXUID called (stubbed)\n");
    return ERROR_SUCCESS;
}

uint32_t XamShowAchievementsUI(uint32_t dwUserIndex) {
    (void)dwUserIndex;
    printf("XamInput: XamShowAchievementsUI called (stubbed)\n");
    return ERROR_SUCCESS;
}

uint32_t XamShowPlayerReviewUI(uint32_t dwUserIndex, uint64_t xuid) {
    (void)dwUserIndex;
    (void)xuid;
    printf("XamInput: XamShowPlayerReviewUI called (stubbed)\n");
    return ERROR_SUCCESS;
}

uint32_t XamShowDeviceSelectorUI(uint32_t dwUserIndex, uint32_t dwContentType,
                                  uint32_t dwContentFlags, uint64_t qwTotalRequested,
                                  void* pDeviceID) {
    (void)dwUserIndex;
    (void)dwContentType;
    (void)dwContentFlags;
    (void)qwTotalRequested;
    (void)pDeviceID;
    printf("XamInput: XamShowDeviceSelectorUI called (stubbed)\n");
    return ERROR_SUCCESS;
}

uint32_t XamShowDirtyDiscErrorUI(uint32_t dwUserIndex) {
    (void)dwUserIndex;
    printf("XamInput: XamShowDirtyDiscErrorUI called (stubbed)\n");
    return ERROR_SUCCESS;
}

//=============================================================================
// Xbox Live User Functions
//=============================================================================

// Error code for not logged in
#define ERROR_NOT_LOGGED_ON 1245

// Fake user data for cross-platform builds
static struct {
    uint8_t signedIn;
    uint64_t xuid;
    wchar_t gamertag[16];
} g_users[MAX_CONTROLLERS] = {
    {1, 0xE000000000000001ULL, L"Player1"},
    {0, 0, L""},
    {0, 0, L""},
    {0, 0, L""}
};

/**
 * XamUserGetXUID @ 0x8258686C | ordinal: 0x020A (522)
 * 
 * Retrieves the Xbox User ID (XUID) for a signed-in user.
 */
uint32_t XamUserGetXUID(uint32_t dwUserIndex, uint64_t* pXuid) {
    // Validate parameters
    if (dwUserIndex >= MAX_CONTROLLERS || !pXuid) {
        return ERROR_INVALID_PARAMETER;
    }
    
    // Check if user is signed in
    if (!g_users[dwUserIndex].signedIn) {
        *pXuid = 0;
        return ERROR_NOT_LOGGED_ON;
    }
    
    // Return fake XUID for local play
    *pXuid = g_users[dwUserIndex].xuid;
    return ERROR_SUCCESS;
}

/**
 * XamUserGetSigninState @ 0x825867CC | ordinal: 0x0210 (528)
 * 
 * Retrieves the sign-in state for a user.
 */
uint32_t XamUserGetSigninState(uint32_t dwUserIndex) {
    // Validate parameter
    if (dwUserIndex >= MAX_CONTROLLERS) {
        return eXUserSigninState_NotSignedIn;
    }
    
    // Return local sign-in for player 1, not signed in for others
    if (g_users[dwUserIndex].signedIn) {
        return eXUserSigninState_SignedInLocally;
    }
    
    return eXUserSigninState_NotSignedIn;
}

/**
 * XamUserGetName @ 0x8258682C | ordinal: 0x020E (526)
 * 
 * Retrieves the gamertag (display name) for a user.
 */
uint32_t XamUserGetName(uint32_t dwUserIndex, wchar_t* pchName, uint32_t cchName) {
    // Validate parameters
    if (dwUserIndex >= MAX_CONTROLLERS || !pchName || cchName == 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    // Check if user is signed in
    if (!g_users[dwUserIndex].signedIn) {
        pchName[0] = L'\0';
        return ERROR_NOT_LOGGED_ON;
    }
    
    // Copy gamertag to output buffer
    uint32_t len = 0;
    while (len < cchName - 1 && g_users[dwUserIndex].gamertag[len] != L'\0') {
        pchName[len] = g_users[dwUserIndex].gamertag[len];
        len++;
    }
    pchName[len] = L'\0';
    
    return ERROR_SUCCESS;
}
