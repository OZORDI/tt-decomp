/**
 * rage_xml.cpp -- xmlNodeStruct virtual methods
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * rage::xmlNodeStruct is the base serialization node for RAGE's XML-based
 * data system.  Derived types (Bool, Char, etc.) provide format/parse
 * callbacks for each primitive type.
 *
 * This file lifts 5 small (16-56 byte) methods from the original binary.

 */

#include "rage_xml.hpp"
#include <cstdint>

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// Globals — type ID constants for xmlNodeStruct::IsType checks
// ─────────────────────────────────────────────────────────────────────────────

// Known xmlNodeStruct type identifiers.
// @ 0x825C803C (8 bytes) / 0x825C8038 (4 bytes)
extern uint32_t lbl_825C803C;
extern uint32_t lbl_825C8038;


// ═════════════════════════════════════════════════════════════════════════════
//  xmlNodeStruct — type identification
// ═════════════════════════════════════════════════════════════════════════════

/**
 * IsType
 * @ 0x821A8898 | size: 0x38 (56 bytes) | vtable slot 20
 *
 * Checks whether this xmlNodeStruct matches a given type ID.
 * Compares against two known type IDs stored as globals:
 *   - lbl_825C803C: primary xmlNodeStruct type ID (returns true immediately)
 *   - lbl_825C8038: secondary/base type ID (returns true)
 * Returns false for all other type IDs.
 */
// xmlNodeStruct_vfn_20 @ 0x821A8898
bool xmlNodeStruct_IsType(xmlNodeStruct* self, uint32_t typeId)
{
    if (typeId == lbl_825C803C)
        return true;

    return (typeId == lbl_825C8038);
}

/**
 * ScalarDtor
 * @ 0x821A88D0 | size: 0x10 (16 bytes) | vtable slot 1
 *
 * Scalar deleting destructor for xmlNodeStruct.  Dispatches through
 * the vtable to slot 21 (offset 84), which is the actual destructor
 * implementation.  This indirection allows derived classes to override
 * destruction behavior.
 */
// xmlNodeStruct_vfn_1 @ 0x821A88D0
void xmlNodeStruct_ScalarDtor(xmlNodeStruct* self, int flags)
{
    void** vtable = *reinterpret_cast<void***>(self);
    typedef void (*DtorFn)(xmlNodeStruct*, int);
    DtorFn dtorFn = reinterpret_cast<DtorFn>(vtable[21]);
    dtorFn(self, flags);
}


// ═════════════════════════════════════════════════════════════════════════════
//  xmlNodeStructBool — format value to string
// ═════════════════════════════════════════════════════════════════════════════

/**
 * FormatValue
 * @ 0x821A8A80 | size: 0x18 (24 bytes) | vtable slot 2
 *
 * Converts a boolean value to a single-character string representation.
 * Writes the ASCII character '0' (0x30) plus the boolean value (0 or 1)
 * into the output buffer, followed by a null terminator.
 * Result is "0" for false, "1" for true.
 */
// xmlNodeStructBool_vfn_2 @ 0x821A8A80
void xmlNodeStructBool_FormatValue(xmlNodeStructBool* self, char* outBuf,
                                    const uint8_t* value)
{
    outBuf[0] = static_cast<char>(*value + 48);
    outBuf[1] = '\0';
}


// ═════════════════════════════════════════════════════════════════════════════
//  xmlNodeStructChar — format value to string
// ═════════════════════════════════════════════════════════════════════════════

/**
 * FormatValue
 * @ 0x821A8AA8 | size: 0x14 (20 bytes) | vtable slot 2
 *
 * Converts a char value to a null-terminated single-character string.
 * Copies the byte directly into the output buffer and appends a null
 * terminator.
 */
// xmlNodeStructChar_vfn_2 @ 0x821A8AA8
void xmlNodeStructChar_FormatValue(xmlNodeStructChar* self, char* outBuf,
                                    const uint8_t* value)
{
    outBuf[0] = static_cast<char>(*value);
    outBuf[1] = '\0';
}




// ═════════════════════════════════════════════════════════════════════════════
//  xmlNodeStructChar — parse value from string
// ═════════════════════════════════════════════════════════════════════════════

/**
 * ParseValue
 * @ 0x821A8A98 | size: 0x10 (16 bytes) | vtable slot 1
 *
 * Parses a single character from the input string into the output buffer.
 * Copies one byte from the source to the destination and returns 1
 * to indicate success.
 */
// xmlNodeStructChar_vfn_1 @ 0x821A8A98
int xmlNodeStructChar_ParseValue(xmlNodeStructChar* self, uint8_t* outVal,
                                  const uint8_t* inStr)
{
    *outVal = *inStr;
    return 1;
}

} // namespace rage
