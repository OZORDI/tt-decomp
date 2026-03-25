/**
 * pong_ui_navigation.cpp — UI Navigation and Page Group Functions
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Handles menu navigation, grid selection, and page group state management.
 */

#include <stdint.h>
#include <stdbool.h>

// Forward declarations
extern void* g_input_obj_ptr;
extern void pg_E480(int code, int flags, const char* message, void* data1, void* data2);

// ─────────────────────────────────────────────────────────────────────────────
// pg_0CB8 @ 0x82100CB8 | size: 0xAC
// Initializes a page group structure with default values
// ─────────────────────────────────────────────────────────────────────────────
void pg_0CB8(void* pageGroup) {
    uint32_t* pg = (uint32_t*)pageGroup;
    
    // Initialize core fields
    pg[1] = 0;   // +0x04: current selection
    pg[2] = 0;   // +0x08: selection count
    pg[3] = 0;   // +0x0C: scroll offset
    pg[4] = -1;  // +0x10: previous selection (-1 = none)
    pg[5] = 0;   // +0x14: flags
    pg[6] = 0;   // +0x18: reserved
    pg[7] = 0;   // +0x1C: reserved
    
    // Clear array 1 (8 elements at +0x20)
    for (int i = 0; i < 8; i++) {
        pg[8 + i] = 0;
    }
    
    // Clear array 2 (8 elements at +0x40)
    for (int i = 0; i < 8; i++) {
        pg[16 + i] = 0;
    }
    
    // Initialize state flags
    uint8_t* pgBytes = (uint8_t*)pageGroup;
    pgBytes[100] = 0;  // +0x64: input enabled
    pgBytes[101] = 0;  // +0x65: navigation active
    pgBytes[102] = 0;  // +0x66: hint visible
    pg[25] = -1;       // +0x64 (as uint32): state = -1
    pgBytes[108] = 0;  // +0x6C: initialized flag
    
    // Log initialization if not in silent mode
    if (pgBytes[288] == 0) {
        uint32_t data1 = pg[7];  // +0x1C
        uint32_t data2 = pg[8];  // +0x20
        pg_E480(2096, 128, "im vel - to prevent poor mover tracking will clamp at runtim…", 
                (void*)(uintptr_t)data2, (void*)(uintptr_t)data1);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pg_0D68 @ 0x82100D68 | size: 0x68
// Updates page group selection index
// ─────────────────────────────────────────────────────────────────────────────
void pg_0D68(void* pageGroup, int newSelection) {
    uint32_t* pg = (uint32_t*)pageGroup;
    uint8_t* pgBytes = (uint8_t*)pageGroup;
    
    int currentSelection = (int)pg[1];  // +0x04
    int selectionCount = (int)pg[2];    // +0x08
    
    // Check if selection actually changed or count is zero
    if (newSelection == currentSelection && selectionCount != 0) {
        return;
    }
    
    // Update selection
    pg[1] = (uint32_t)newSelection;  // +0x04
    pg[2] = 0;                        // +0x08: reset count
    
    // Log selection change if not in silent mode
    if (pgBytes[288] == 0) {
        pg_E480(2095, 128, "im vel - to prevent poor mover tracking will clamp at runtim…",
                nullptr, nullptr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pg_0DD0 @ 0x82100DD0 | size: 0x80
// Logs debug information for page group state
// ─────────────────────────────────────────────────────────────────────────────
void pg_0DD0(void* pageGroup) {
    uint32_t* pg = (uint32_t*)pageGroup;
    const char* debugMsg = "im vel - to prevent poor mover tracking will clamp at runtim…";
    
    // Log three debug messages with different data
    pg_E480(2118, 64, debugMsg, (void*)(uintptr_t)pg[6], nullptr);
    pg_E480(2096, 128, debugMsg, (void*)(uintptr_t)pg[7], (void*)(uintptr_t)pg[8]);
    pg_E480(2095, 128, debugMsg, (void*)(uintptr_t)pg[1], (void*)(uintptr_t)pg[2]);
}

// ─────────────────────────────────────────────────────────────────────────────
// pg_0E50 @ 0x82100E50 | size: 0x104
// Copies page group state from source to destination
// ─────────────────────────────────────────────────────────────────────────────
void pg_0E50(void* destPageGroup, const void* srcPageGroup) {
    uint32_t* dest = (uint32_t*)destPageGroup;
    const uint32_t* src = (const uint32_t*)srcPageGroup;
    uint8_t* destBytes = (uint8_t*)destPageGroup;
    
    // Copy 27 uint32_t values (108 bytes total)
    for (int i = 0; i < 27; i++) {
        dest[i + 1] = src[i + 1];  // Start at offset +0x04
    }
    
    // Handle state flag conversion
    int srcState = (int)src[25];  // +0x64 (100 bytes offset / 4)
    if (srcState == 0) {
        dest[45] = 1;  // +0xB4 (180 bytes offset / 4)
    } else if (srcState == 1) {
        dest[45] = 0;
    }
    
    // Log state change if not in silent mode
    if (destBytes[288] == 0) {
        pg_0DD0(destPageGroup);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// PageGroup_CanAcceptInput @ 0x82101048 | size: 0x94
// Checks if page group should accept input based on state and cursor position
// ─────────────────────────────────────────────────────────────────────────────
bool PageGroup_CanAcceptInput(const void* pageGroup) {
    const uint8_t* pgBytes = (const uint8_t*)pageGroup;
    const uint32_t* pg = (const uint32_t*)pageGroup;
    
    // Check if initialized
    if (pgBytes[108] == 0) {  // +0x6C
        return false;
    }
    
    // Check if input enabled
    if (pgBytes[100] != 0) {  // +0x64
        return true;
    }
    
    // Get input system pointer
    uint32_t** inputSystem = (uint32_t**)g_input_obj_ptr;
    if (!inputSystem) return false;
    
    uint32_t* input = *inputSystem;
    int cursorFrame = (int)input[78];  // +312 bytes / 4 = index 78
    int selectionStart = (int)pg[1];   // +0x04
    int selectionEnd = (int)pg[2];     // +0x08
    
    // Check if cursor is within selection range
    if (selectionStart >= cursorFrame) {
        int scrollSize = (int)input[81];  // +324 bytes / 4 = index 81
        int rangeEnd = scrollSize + selectionEnd;
        if (selectionStart >= rangeEnd) {
            return true;
        }
    }
    
    if (selectionEnd >= cursorFrame) {
        int scrollSize = (int)input[81];
        int rangeEnd = scrollSize + selectionStart;
        if (selectionEnd >= rangeEnd) {
            return true;
        }
    }
    
    // Check bounds
    if (selectionStart >= 99 || selectionEnd >= 99) {
        return true;
    }
    
    // Check if input polling is active
    if (input[3] == 0) {  // +12 bytes / 4 = index 3
        return false;
    }
    
    // Return hint visibility flag
    return pgBytes[102] != 0;  // +0x66
}

// ─────────────────────────────────────────────────────────────────────────────
// PageGroup_IsAtBottomEdge @ 0x821010E0 | size: 0x60
// Checks if vertical navigation is at bottom edge
// ─────────────────────────────────────────────────────────────────────────────
bool PageGroup_IsAtBottomEdge(const void* pageGroup) {
    const uint8_t* pgBytes = (const uint8_t*)pageGroup;
    const uint32_t* pg = (const uint32_t*)pageGroup;
    
    // Check if initialized
    if (pgBytes[108] == 0) {  // +0x6C
        return false;
    }
    
    // Check if input enabled
    if (pgBytes[100] != 0) {  // +0x64
        return true;
    }
    
    // Get input system
    uint32_t** inputSystem = (uint32_t**)g_input_obj_ptr;
    if (!inputSystem) return false;
    
    uint32_t* input = *inputSystem;
    
    // Check if input polling is active
    if (input[3] != 0) {  // +12 bytes / 4 = index 3
        return PageGroup_IsInputVisible(pageGroup);
    }
    
    // Check vertical bounds
    int maxVertical = (int)input[79];  // +316 bytes / 4 = index 79
    int rowStart = (int)pg[7];         // +0x1C (28 bytes / 4 = index 7)
    int rowEnd = (int)pg[8];           // +0x20 (32 bytes / 4 = index 8)
    
    if (rowStart >= maxVertical) {
        return true;
    }
    
    if (rowEnd < maxVertical) {
        return false;
    }
    
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// PageGroup_IsAtRightEdge @ 0x82101140 | size: 0x58
// Checks if in horizontal scroll mode
// ─────────────────────────────────────────────────────────────────────────────
bool PageGroup_IsAtRightEdge(const void* pageGroup) {
    const uint32_t* pg = (const uint32_t*)pageGroup;
    
    int selectionStart = (int)pg[1];  // +0x04
    int selectionEnd = (int)pg[2];    // +0x08
    
    // Check if single column selected
    bool isSingleColumn = (selectionStart == selectionEnd);
    
    if (!isSingleColumn) {
        return false;
    }
    
    // Get input system
    uint32_t** inputSystem = (uint32_t**)g_input_obj_ptr;
    if (!inputSystem) return false;
    
    uint32_t* input = *inputSystem;
    int scrollSize = (int)input[81];  // +324 bytes / 4 = index 81
    
    // Check if scroll size allows horizontal movement
    if (scrollSize <= 1) {
        return false;
    }
    
    int cursorFrame = (int)input[78];  // +312 bytes / 4 = index 78
    
    // Check if at right edge
    if (selectionStart >= cursorFrame - 1) {
        return true;
    }
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// PageGroup_IsAtTopEdge @ 0x82101198 | size: 0x68
// Checks if selection is at edge of grid
// ─────────────────────────────────────────────────────────────────────────────
bool PageGroup_IsAtTopEdge(const void* pageGroup) {
    const uint32_t* pg = (const uint32_t*)pageGroup;
    
    // Get input system
    uint32_t** inputSystem = (uint32_t**)g_input_obj_ptr;
    if (!inputSystem) return false;
    
    uint32_t* input = *inputSystem;
    
    int selectionStart = (int)pg[1];  // +0x04
    int selectionEnd = (int)pg[2];    // +0x08
    int cursorFrame = (int)input[78]; // +312 bytes / 4 = index 78
    
    // Check if at right edge
    if (selectionStart >= cursorFrame - 1) {
        int scrollSize = (int)input[81];  // +324 bytes / 4 = index 81
        int rangeEnd = scrollSize + selectionEnd - 1;
        if (selectionStart >= rangeEnd) {
            return true;
        }
    }
    
    // Check if at left edge
    if (selectionEnd >= cursorFrame - 1) {
        int scrollSize = (int)input[81];
        int rangeEnd = scrollSize + selectionStart - 1;
        if (selectionEnd >= rangeEnd) {
            return true;
        }
    }
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// PageGroup_IsAtLeftEdge @ 0x82101200 | size: 0xA0
// Checks if can navigate down in grid
// ─────────────────────────────────────────────────────────────────────────────
bool PageGroup_IsAtLeftEdge(const void* pageGroup) {
    const uint32_t* pg = (const uint32_t*)pageGroup;
    
    // Get input system
    uint32_t** inputSystem = (uint32_t**)g_input_obj_ptr;
    if (!inputSystem) return false;
    
    uint32_t* input = *inputSystem;
    
    // Check if input polling is active
    if (input[3] != 0) {  // +12 bytes / 4 = index 3
        return false;
    }
    
    // Check if at edge
    if (!PageGroup_IsAtTopEdge(pageGroup)) {
        return false;
    }
    
    int selectionStart = (int)pg[1];  // +0x04
    int selectionEnd = (int)pg[2];    // +0x08
    int maxVertical = (int)input[79]; // +316 bytes / 4 = index 79
    
    // Check vertical bounds for down navigation
    if (selectionEnd <= selectionStart) {
        int rowStart = (int)pg[7];  // +0x1C (28 bytes / 4 = index 7)
        if (rowStart >= maxVertical - 1) {
            return true;
        }
        
        if (selectionEnd >= selectionStart) {
            return false;
        }
    }
    
    int rowEnd = (int)pg[8];  // +0x20 (32 bytes / 4 = index 8)
    if (rowEnd >= maxVertical - 1) {
        return true;
    }
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// PageGroup_IsInputVisible @ 0x821015F0 | size: 0x78
// Checks if should show navigation hint
// ─────────────────────────────────────────────────────────────────────────────
bool PageGroup_IsInputVisible(const void* pageGroup) {
    const uint8_t* pgBytes = (const uint8_t*)pageGroup;
    const uint32_t* pg = (const uint32_t*)pageGroup;
    
    // Check if should accept input
    if (PageGroup_CanAcceptInput(pageGroup)) {
        // Check hint flag
        if (pgBytes[102] == 0) {  // +0x66
            int rowEnd = (int)pg[8];    // +0x20 (32 bytes / 4 = index 8)
            int rowStart = (int)pg[7];  // +0x1C (28 bytes / 4 = index 7)
            int totalRows = rowEnd + rowStart;
            
            // Show hint if total rows >= 7
            if (totalRows < 7) {
                return false;
            }
        }
        return true;
    }
    
    // Check if input enabled
    if (pgBytes[100] != 0) {  // +0x64
        return true;
    }
    
    return false;
}
