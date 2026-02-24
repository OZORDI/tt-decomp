/**
 * rage_utils_simple.h - Simple RAGE utility functions
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Header for small utility functions from the RAGE engine.
 * These are typically simple getters, setters, and helper functions.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * rage_IncrementRefCount @ 0x8235EAC0 | size: 0x14
 * 
 * Increments a reference counter at offset +0xC in the object.
 * Classic AddRef() pattern for COM-style reference counting.
 * 
 * @param obj Object pointer with ref count at +0xC
 * @return New reference count value
 */
uint32_t rage_IncrementRefCount(void* obj);

/**
 * rage_AlignAndDelegate @ 0x82353130 | size: 0x10
 * 
 * Loads value from offset +0xC, aligns to 4-byte boundary,
 * and delegates to another function.
 * 
 * @param obj Object pointer
 * @return Result from delegated function
 */
int rage_AlignAndDelegate(void* obj);

/**
 * rage_InitAndStoreResult @ 0x82568800 | size: 0x38
 * 
 * Calls initialization function, stores 64-bit result at obj+0.
 * 
 * @param obj Object pointer to initialize
 * @return 1 (success indicator)
 */
int rage_InitAndStoreResult(void* obj);

/**
 * rage_SetFieldsAndReturnZero @ 0x82491D48 | size: 0x14
 * 
 * Sets byte field at +0x5F4 and word field at +0x5F8.
 * 
 * @param obj Object pointer
 * @param byte_value Byte value to store at +0x5F4
 * @param word_value Word value to store at +0x5F8
 * @return 0 (success indicator)
 */
int rage_SetFieldsAndReturnZero(void* obj, uint8_t byte_value, uint32_t word_value);

#ifdef __cplusplus
}
#endif

/**
 * rage_SwapAndFreeOld @ 0x820CEC58 | size: 0x4C
 * 
 * Swap/replace pattern: saves old value, processes new value,
 * stores it, and frees old value. Resource management pattern.
 * 
 * @param obj Object pointer with value at +0
 * @param new_param Parameter for processing function
 * @return Object pointer (unchanged)
 */
void* rage_SwapAndFreeOld(void* obj, void* new_param);

/**
 * rage_DestructorWithVtableCall @ 0x820C18B8 | size: 0x54
 * 
 * Destructor that calls vtable method on member at +4 if non-null,
 * then frees the object. RAII pattern with virtual destructor.
 * 
 * @param obj Object pointer with member at +4
 * @return Object pointer (unchanged)
 */
void* rage_DestructorWithVtableCall(void* obj);

/**
 * rage_DestroyArrayBackwards @ 0x820EEA18 | size: 0x54
 * 
 * Array destructor that loops backwards through elements.
 * Count at offset -4, elements are 0x40 bytes each.
 * 
 * @param array_base Pointer to first element (count at -4)
 * @return Pointer to allocation base (array_base - 4)
 */
void* rage_DestroyArrayBackwards(void* array_base);
