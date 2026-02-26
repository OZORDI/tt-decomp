#pragma once

/**
 * XAM Input API - Xbox 360 Kernel Imports
 * Rockstar Presents Table Tennis
 * 
 * These are kernel import thunks that redirect to XAM.xex (Xbox Application Manager).
 * On Xbox 360, these are 16-byte stubs with ordinal metadata that get patched at load time.
 * 
 * For cross-platform builds, we implement these using SDL2 gamepad/haptic APIs.
 */

#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

// Xbox 360 error codes
#define ERROR_SUCCESS                0
#define ERROR_DEVICE_NOT_CONNECTED   1167
#define ERROR_NOT_SUPPORTED          50

// XInput button bitmask constants
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000

/**
 * XINPUT_GAMEPAD - Controller state
 * Size: 12 bytes
 */
typedef struct _XINPUT_GAMEPAD {
    uint16_t wButtons;        // Button bitmask
    uint8_t  bLeftTrigger;    // 0-255
    uint8_t  bRightTrigger;   // 0-255
    int16_t  sThumbLX;        // -32768 to 32767
    int16_t  sThumbLY;
    int16_t  sThumbRX;
    int16_t  sThumbRY;
} XINPUT_GAMEPAD;

/**
 * XINPUT_STATE - Complete controller state with packet number
 * Size: 16 bytes
 */
typedef struct _XINPUT_STATE {
    uint32_t       dwPacketNumber;  // Increments when state changes
    XINPUT_GAMEPAD Gamepad;
} XINPUT_STATE;

/**
 * XINPUT_VIBRATION - Rumble motor speeds
 * Size: 4 bytes
 */
typedef struct _XINPUT_VIBRATION {
    uint16_t wLeftMotorSpeed;   // 0-65535 (low frequency, large motor)
    uint16_t wRightMotorSpeed;  // 0-65535 (high frequency, small motor)
} XINPUT_VIBRATION;

/**
 * XamInputGetState @ 0x825860FC | ordinal: 0x0191 (401)
 * 
 * Retrieves the current state of the specified controller.
 * 
 * @param dwUserIndex Controller index (0-3)
 * @param dwFlags     Reserved, must be 0
 * @param pState      Pointer to XINPUT_STATE structure to receive state
 * @return ERROR_SUCCESS if successful, ERROR_DEVICE_NOT_CONNECTED if controller not connected
 */
uint32_t XamInputGetState(uint32_t dwUserIndex, uint32_t dwFlags, XINPUT_STATE* pState);

/**
 * XamInputSetState @ 0x8258610C | ordinal: 0x0192 (402)
 * 
 * Sets the vibration state of the specified controller.
 * 
 * @param dwUserIndex  Controller index (0-3)
 * @param pVibration   Pointer to XINPUT_VIBRATION structure with motor speeds
 * @return ERROR_SUCCESS if successful, ERROR_DEVICE_NOT_CONNECTED if controller not connected
 */
uint32_t XamInputSetState(uint32_t dwUserIndex, XINPUT_VIBRATION* pVibration);

/**
 * XamGetSystemVersion @ 0x825860CC | ordinal: 0x0282 (642)
 * 
 * Returns the Xbox 360 system version.
 * For cross-platform builds, returns a fake version number.
 * 
 * @return System version (format: 0xMMMMmmmm where M=major, m=minor)
 */
uint32_t XamGetSystemVersion(void);

/**
 * XamNotifyCreateListener @ 0x8258611C | ordinal: 0x028A (650)
 * 
 * Creates a notification listener for system events.
 * Stubbed for cross-platform builds.
 * 
 * @param qwAreas     Notification areas bitmask
 * @param fExternal   External notifications flag
 * @param phListener  Pointer to receive listener handle
 * @return ERROR_SUCCESS (stubbed)
 */
uint32_t XamNotifyCreateListener(uint64_t qwAreas, uint32_t fExternal, void** phListener);

// Xbox Live UI functions (stubbed for cross-platform)

/**
 * XamShowSigninUI @ 0x8258612C | ordinal: 0x02BC (700)
 * Shows Xbox Live sign-in UI (stubbed)
 */
uint32_t XamShowSigninUI(uint32_t cPanes, uint32_t dwFlags);

/**
 * XamShowFriendsUI @ 0x8258613C | ordinal: 0x02BF (703)
 * Shows friends list UI (stubbed)
 */
uint32_t XamShowFriendsUI(uint32_t dwUserIndex);

/**
 * XamShowGamerCardUIForXUID @ 0x8258614C | ordinal: 0x02D5 (725)
 * Shows gamer card for specific XUID (stubbed)
 */
uint32_t XamShowGamerCardUIForXUID(uint32_t dwUserIndex, uint64_t xuid);

/**
 * XamShowAchievementsUI @ 0x8258615C | ordinal: 0x02C5 (709)
 * Shows achievements UI (stubbed)
 */
uint32_t XamShowAchievementsUI(uint32_t dwUserIndex);

/**
 * XamShowPlayerReviewUI @ 0x8258616C | ordinal: 0x02C6 (710)
 * Shows player review/feedback UI (stubbed)
 */
uint32_t XamShowPlayerReviewUI(uint32_t dwUserIndex, uint64_t xuid);

/**
 * XamShowDeviceSelectorUI @ 0x8258617C | ordinal: 0x02CB (715)
 * Shows storage device selector UI (stubbed)
 */
uint32_t XamShowDeviceSelectorUI(uint32_t dwUserIndex, uint32_t dwContentType, 
                                  uint32_t dwContentFlags, uint64_t qwTotalRequested,
                                  void* pDeviceID);

/**
 * XamShowDirtyDiscErrorUI @ 0x8258618C | ordinal: 0x02D9 (729)
 * Shows disc read error UI (stubbed)
 */
uint32_t XamShowDirtyDiscErrorUI(uint32_t dwUserIndex);

//=============================================================================
// Xbox Live User Functions
//=============================================================================

// User sign-in states
typedef enum _XUSER_SIGNIN_STATE {
    eXUserSigninState_NotSignedIn = 0,
    eXUserSigninState_SignedInLocally = 1,
    eXUserSigninState_SignedInToLive = 2
} XUSER_SIGNIN_STATE;

/**
 * XamUserGetXUID @ 0x8258686C | ordinal: 0x020A (522)
 * 
 * Retrieves the Xbox User ID (XUID) for a signed-in user.
 * XUID is a 64-bit unique identifier for Xbox Live accounts.
 * 
 * @param dwUserIndex Controller index (0-3)
 * @param pXuid       Pointer to receive 64-bit XUID
 * @return ERROR_SUCCESS if successful, ERROR_NOT_LOGGED_ON if not signed in
 */
uint32_t XamUserGetXUID(uint32_t dwUserIndex, uint64_t* pXuid);

/**
 * XamUserGetSigninState @ 0x825867CC | ordinal: 0x0210 (528)
 * 
 * Retrieves the sign-in state for a user.
 * 
 * @param dwUserIndex Controller index (0-3)
 * @return XUSER_SIGNIN_STATE value (0=not signed in, 1=local, 2=Live)
 */
uint32_t XamUserGetSigninState(uint32_t dwUserIndex);

/**
 * XamUserGetName @ 0x8258682C | ordinal: 0x020E (526)
 * 
 * Retrieves the gamertag (display name) for a user.
 * 
 * @param dwUserIndex Controller index (0-3)
 * @param pchName     Buffer to receive name (wide-character string)
 * @param cchName     Size of buffer in characters (max 16 for gamertag)
 * @return ERROR_SUCCESS if successful, ERROR_NOT_LOGGED_ON if not signed in
 */
uint32_t XamUserGetName(uint32_t dwUserIndex, wchar_t* pchName, uint32_t cchName);

#ifdef __cplusplus
}
#endif
