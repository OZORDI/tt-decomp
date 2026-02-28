/**
 * rage/ui_page_group.h — UI Page Group Management System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Page groups are UI containers that manage screens/menus with transitions,
 * fading, input handling, and message dispatching.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct PageGroup PageGroup;
typedef struct UIManager UIManager;

/**
 * CreatePageGroup @ 0x82305D50 | size: 0x124
 * 
 * Creates and initializes a page group with fade transition support.
 * Handles page group lifecycle including current/next page management
 * and fade timing.
 * 
 * @param pageGroup  Pointer to page group object to initialize
 */
void CreatePageGroup(PageGroup* pageGroup);

/**
 * DismissPageGroup @ 0x82306000 | size: 0xD8
 * 
 * Dismisses/hides a page group UI element. Calls the page group's
 * close/hide virtual method (slot 21) and updates render state.
 * 
 * @param uiManager  Pointer to UI manager containing the page group
 */
void DismissPageGroup(UIManager* uiManager);

/**
 * ShowPageGroup @ 0x823060D8 | size: 0x110
 * 
 * Shows/displays a page group UI element with optional fade-in.
 * 
 * @param pageGroup  Pointer to page group to show
 */
void ShowPageGroup(PageGroup* pageGroup);

/**
 * FadePageGroup @ 0x823061E8 | size: 0x108
 * 
 * Fades a page group in or out with specified parameters.
 * 
 * @param uiManager  Pointer to UI manager
 * @param fadeType   Type of fade (0=out, 1=in, 2=crossfade)
 * @param fadeMode   Fade mode flags
 * @param fadeTime   Duration of fade in seconds
 * @param param2     Additional fade parameter
 */
void FadePageGroup(UIManager* uiManager, int fadeType, int fadeMode, 
                   float fadeTime, float param2);

/**
 * UpdatePageGroup @ 0x82566C40 | size: 0x3C
 * 
 * Updates page group state per frame. Handles transitions, animations,
 * and state changes.
 * 
 * @param pageGroup  Pointer to page group to update
 */
void UpdatePageGroup(PageGroup* pageGroup);

/**
 * RenderPageGroup @ 0x82566DC0 | size: 0x38
 * 
 * Renders a page group to the screen.
 * 
 * @param pageGroup  Pointer to page group to render
 * @param visible    Visibility flag (0=hidden, 1=visible)
 * @param param2     Additional render parameter
 */
void RenderPageGroup(PageGroup* pageGroup, int visible, int param2);

/**
 * DestroyPageGroup @ 0x820F8250 | size: 0xB4
 * 
 * Destroys and cleans up a page group, releasing resources.
 * 
 * @param pageGroup  Pointer to page group to destroy
 */
void DestroyPageGroup(PageGroup* pageGroup);

/**
 * GetPageGroupState @ 0x8242C3B8 | size: 0x8
 * 
 * Gets the current state of a page group.
 * 
 * @param pageGroup  Pointer to page group
 * @param stateId    State ID to query (-1 for current state)
 * @return Current state value
 */
int GetPageGroupState(PageGroup* pageGroup, int stateId);

/**
 * SetPageGroupVisible @ 0x8224CFE0 | size: 0x68
 * 
 * Sets the visibility state of a page group.
 * 
 * @param pageGroup  Pointer to page group
 */
void SetPageGroupVisible(PageGroup* pageGroup);

/**
 * ProcessPageGroupInput @ 0x8225E480 | size: 0x25C
 * 
 * Processes input events for a page group, handling button presses
 * and navigation.
 * 
 * @param pageGroup  Pointer to page group
 * @param inputData  Input data structure
 */
void ProcessPageGroupInput(PageGroup* pageGroup, void* inputData);

/**
 * PostPageGroupMessage @ 0x8225E6E0 | size: 0x118
 * 
 * Posts a message/event to a page group for processing.
 * 
 * @param target     Target object (page group or context)
 * @param messageId  Message ID code
 * @param param1     First message parameter
 * @param param2     Second message parameter
 * @param param3     Third message parameter
 */
void PostPageGroupMessage(void* target, int messageId, int param1, 
                          int param2, int param3);

/**
 * CheckButtonPressed @ 0x821DFFF8 | size: 0x6C
 * 
 * Checks if a button is currently pressed in the input state.
 * 
 * @param inputRecord  Pointer to input record/button state
 * @return true if button is pressed, false otherwise
 */
bool CheckButtonPressed(void* inputRecord);

#ifdef __cplusplus
}
#endif
