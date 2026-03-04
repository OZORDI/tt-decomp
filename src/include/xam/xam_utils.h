#pragma once

/**
 * Xbox 360 XAM Utility Functions
 * Rockstar Presents Table Tennis
 * 
 * Various XAM helper functions for state management, list operations,
 * and flag checking.
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * xam_4780 @ 0x82574780 | size: 0xBC
 * 
 * Cleans up and resets multiple linked lists in a manager structure.
 * Iterates through an array of lists, calling cleanup on each entry,
 * then unlinking all nodes.
 * 
 * @param manager Pointer to manager object containing list arrays
 */
void xam_4780(void* manager);

/**
 * xam_9608_g @ 0x82369608 | size: 0x60
 * 
 * Checks multiple flag conditions to determine if an operation is allowed.
 * Returns true if any of three conditions are met:
 * - Field at +36 is non-negative
 * - Byte flag at +76 is clear AND bit 6 of byte at +436 is set
 * 
 * @param obj Pointer to object with flag fields
 * @return true if operation is allowed, false otherwise
 */
bool xam_9608_g(void* obj);

/**
 * xam_F460_g @ 0x8236F460 | size: 0x20
 * 
 * Checks if bit 16 of the field at offset +36 is set.
 * Simple flag test function.
 * 
 * @param obj Pointer to object
 * @return true if bit 16 is set, false otherwise
 */
bool xam_F460_g(void* obj);

/**
 * xam_F2D8_g @ 0x8236F2D8 | size: 0x80
 * 
 * Searches for a matching string in an array of string entries.
 * Each entry is 40 bytes. Returns true if a match is found.
 * 
 * @param obj Pointer to object containing string array
 * @param searchStr String to search for
 * @return true if string is found, false otherwise
 */
bool xam_F2D8_g(void* obj, const char* searchStr);

/**
 * xam_FC58 @ 0x8236FC58 | size: 0x74
 * 
 * Combines multiple flag bits into a result value.
 * Calls another function and ORs the result with extracted flag bits.
 * 
 * @param obj Pointer to object with flag fields
 * @return Combined flag value
 */
uint32_t xam_FC58(void* obj);

/**
 * xam_01A8_g @ 0x823701A8 | size: 0x98
 * 
 * Calculates a pointer into one of two arrays based on string matching.
 * Returns NULL if no match is found.
 * 
 * @param obj Pointer to object containing arrays
 * @param str String to search for
 * @return Pointer to matching entry, or NULL if not found
 */
void* xam_01A8_g(void* obj, const char* str);

/**
 * xam_18A0_fw @ 0x823718A0 | size: 0x5C
 * 
 * Constructor/initializer for a worker object.
 * Sets up vtable pointer and initializes fields.
 * 
 * @param obj Pointer to object to initialize
 */
void xam_18A0_fw(void* obj);

/**
 * xam_1978_wrh @ 0x82371978 | size: 0x68
 * 
 * Decrements a reference count and performs cleanup if it reaches zero.
 * Returns the calculated value from flags.
 * 
 * @param obj Pointer to object with reference count
 * @return Calculated flag value
 */
uint32_t xam_1978_wrh(void* obj);

/**
 * xam_1F78_g @ 0x82371F78 | size: 0x60
 * 
 * Tests if a specific bit is set in a 64-bit bitfield.
 * Returns true if the bit at the given index is set.
 * 
 * @param obj Pointer to object containing bitfield
 * @param bitIndex Index of bit to test (0-63)
 * @return true if bit is set, false otherwise
 */
bool xam_1F78_g(void* obj, int32_t bitIndex);

/**
 * xam_9758_g @ 0x82369758 | size: 0x9C
 * 
 * Validates a string parameter and checks if it's valid.
 * Returns true if validation succeeds.
 * 
 * @param obj Pointer to object
 * @param str String to validate
 * @return true if validation succeeds, false otherwise
 */
bool xam_9758_g(void* obj, const char* str);

#ifdef __cplusplus
}
#endif
