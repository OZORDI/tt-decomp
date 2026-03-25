/**
 * rage::parStream — Parameter Stream Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Serialization system for RAGE engine parameter files.
 * Supports XML and RBF (Rockstar Binary Format) streams.
 */

#include "rage/parStream.hpp"
#include <cstring>
#include <cstdio>

namespace rage {

// ────────────────────────────────────────────────────────────────────────────
// parStream — Base class
// ────────────────────────────────────────────────────────────────────────────

/**
 * parStream::~parStream @ 0x824193E0 | size: 0x48
 * 
 * Base destructor. Sets vtable pointer and optionally frees memory.
 */
parStream::~parStream() {
    // Destructor implementation handled by derived classes
}

// ────────────────────────────────────────────────────────────────────────────
// parStreamInXml — XML input stream
// ────────────────────────────────────────────────────────────────────────────

parStreamInXml::parStreamInXml() 
    : m_pCurrentNode(nullptr)
    , m_pRootNode(nullptr)
    , m_pXmlData(nullptr)
    , m_xmlDataSize(0)
    , m_currentDepth(0)
    , m_isValid(false) {
}

/**
 * parStreamInXml::~parStreamInXml @ 0x82419570 | size: 0x34
 * 
 * Scalar destructor for XML input stream.
 */
parStreamInXml::~parStreamInXml() {
    CloseFile();
}

/**
 * parStreamInXml::OpenFile @ 0x82419428 | size: 0x60
 * 
 * Opens an XML file for reading. Copies string data into internal buffer.
 * This function copies the input string character by character in reverse order.
 */
bool parStreamInXml::OpenFile(const char* filename) {
    if (!filename) {
        return false;
    }
    
    // Calculate string length if not provided
    int length = -1;
    if (length < 0) {
        const char* p = filename;
        while (*p) p++;
        length = static_cast<int>(p - filename);
    }
    
    // Copy string data (assembly does this in reverse)
    // This is likely copying into an internal buffer at offset +64
    // with a counter at offset +1024
    
    m_isValid = true;
    return true;
}

void parStreamInXml::CloseFile() {
    if (m_pXmlData) {
        // Free XML data buffer
        m_pXmlData = nullptr;
    }
    m_isValid = false;
}

bool parStreamInXml::IsValid() const {
    return m_isValid;
}
/**
 * parStreamInRbf::BeginObject @ 0x8241C7E8 | size: 0x7C
 * 
 * Begins reading an object from the RBF stream. Reads field header and validates
 * that the current field is an object type.
 */
bool parStreamInRbf::BeginObject(const char* name) {
    uint16_t fieldType = 0;
    uint16_t fieldId = 0;
    uint32_t fieldData = 0;
    uint32_t nameHash = 0;
    
    // Read next field header from stream
    ReadFieldHeader(&fieldType, &fieldId, &fieldData, &nameHash);
    
    // Check if we have a valid name hash (non-zero means named field)
    bool hasName = (nameHash != 0);
    
    // Validate field name matches if provided
    if (name) {
        uint32_t expectedHash = 0; // TODO: Call hash function on name
        // util_9410(name, expectedHash, hasName);
    }
    
    // Process the field based on type
    ProcessField(name, fieldId);
    
    // Mark that we're inside an object
    m_isValid = true;
    m_currentDepth++;
    
    return true;
}

/**
 * parStreamInRbf::EndObject @ 0x8241C8C8 | size: 0x64
 * 
 * Ends reading an object from the RBF stream. Calls BeginObject to skip to the
 * matching end marker, then calls EndArray to finalize.
 */
void parStreamInRbf::EndObject() {
    // Read until we find the matching end marker
    BeginObject(nullptr);
    
    // Call EndArray to clean up state
    EndArray();
    
    // Clear state flags
    m_isValid = false;
    m_currentDepth = 0;
    m_currentOffset = 0;
}

/**
 * parStreamInRbf::EndArray @ 0x8241C868 | size: 0x60
 * 
 * Ends reading an array from the RBF stream. Reads field header if not already
 * at end marker, then clears array state.
 */
void parStreamInRbf::EndArray() {
    // Check if we're already at the end marker
    if (!m_isValid) {
        uint16_t fieldType = 0;
        uint16_t fieldId = 0;
        uint32_t fieldData = 0;
        uint32_t nameHash = 0;
        
        // Read next field header
        ReadFieldHeader(&fieldType, &fieldId, &fieldData, &nameHash);
        
        // Clear state
        m_isValid = false;
        m_currentDepth = 0;
        m_currentOffset = 0;
    }
}

/**
 * parStreamInRbf::ReadInt @ 0x8241C930 | size: 0xE0
 * 
 * Reads an integer value from the RBF stream. Handles nested structures and
 * arrays by recursively reading until the target field is found.
 */
bool parStreamInRbf::ReadInt(const char* name, int32_t* value) {
    constexpr uint16_t TYPE_INT32 = 0xFFFC;
    constexpr uint16_t TYPE_FLOAT = 0xFFFD;
    constexpr uint16_t TYPE_END = 0xFFFF;
    
    int nestingLevel = 1;
    *value = 0;
    
    // Read fields until we find the target or reach end
    while (nestingLevel > 0) {
        uint16_t fieldType = 0;
        uint16_t fieldId = 0;
        uint32_t fieldData = 0;
        uint32_t nameHash = 0;
        
        // Read next field header
        ReadFieldHeader(&fieldType, &fieldId, &fieldData, &nameHash);
        
        // Check field type
        if (fieldType >= TYPE_INT32) {
            if (fieldType <= TYPE_FLOAT) {
                // Found a primitive type - read the value
                ReadBool(name, nullptr);  // Helper to skip field
            } else if (fieldType == TYPE_END) {
                // End marker - decrease nesting level
                nestingLevel--;
            }
        } else {
            // Complex type - read and process
            uint32_t tempValue = 0;
            bool success = ProcessField(name, fieldId);
            if (!success) {
                nestingLevel++;
            }
        }
    }
    
    // Clear state after reading
    m_isValid = false;
    m_currentDepth = 0;
    m_currentOffset = 0;
    
    // Extract the integer value
    // rage_9100(value);  // TODO: Call extraction helper
    
    return true;
}

/**
 * parStreamInRbf::BeginArray @ 0x8241CA10 | size: 0x124
 * 
 * Begins reading an array from the RBF stream. Reads array header and sets up
 * iteration state for reading array elements.
 */
bool parStreamInRbf::BeginArray(const char* name, uint32_t* count) {
    // Use provided count or get from current offset
    uint32_t arrayCount = count ? *count : m_currentOffset;
    
    // Check if we need to read array header
    if (!m_currentDepth) {
        uint16_t fieldType = 0;
        uint16_t fieldId = 0;
        uint32_t fieldData = 0;
        uint32_t nameHash = 0;
        
        // Read array header
        ReadFieldHeader(&fieldType, &fieldId, &fieldData, &nameHash);
        
        // Initialize array reading state
        m_currentDepth = 1;
        // grc_3CD8(m_pRbfData, &m_rbfDataSize, 1);  // TODO: Setup buffer
    }
    
    // Get array size from buffer
    uint32_t bufferSize = m_rbfDataSize;
    uint32_t elementSize = arrayCount;
    
    // Adjust size based on array type (from switch statement in scaffold)
    if (arrayCount <= 11) {
        switch (arrayCount) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 6:
                // Single element types
                // rage_obj_bind_3828(m_pRbfData, name, elementSize);
                break;
                
            case 2:
            case 7:
                // Half-size elements (divide by 2)
                elementSize = elementSize >> 1;
                // grc_3C68(m_pRbfData, name, elementSize);
                break;
                
            case 5:
            case 8:
            case 9:
            case 10:
            case 11:
                // Quarter-size elements (divide by 4)
                elementSize = elementSize >> 2;
                // grc_3CD8(m_pRbfData, name, elementSize);
                break;
        }
    }
    
    // Update buffer position
    uint32_t bytesRead = bufferSize - elementSize;
    m_rbfDataSize = bytesRead;
    
    return (bytesRead > 0);
}

/**
 * parStreamInRbf::ReadString @ 0x8241C140 | size: 0x1C
 * 
 * Checks if there are more bytes available in the buffer for reading a string.
 */
bool parStreamInRbf::ReadString(const char* name, char* buffer, size_t bufferSize) {
    // Check if buffer has data remaining
    return (m_rbfDataSize > 0);
}

/**
 * parStreamInRbf::ReadFloat @ 0x8241CB38 | size: 0x214
 * 
 * Reads a float value from the RBF stream. Handles complex nested structures
 * with arrays and objects.
 */
bool parStreamInRbf::ReadFloat(const char* name, float* value) {
    constexpr uint16_t TYPE_INT32 = 0xFFFC;
    constexpr uint16_t TYPE_FLOAT = 0xFFFD;
    constexpr uint16_t TYPE_END = 0xFFFF;
    
    int nestingLevel = 1;
    int arrayDepth = 0;
    
    // Read fields until we find the target
    while (true) {
        uint16_t fieldType = 0;
        uint16_t fieldId = 0;
        uint32_t fieldData = 0;
        uint32_t nameHash = 0;
        
        // Read next field header
        ReadFieldHeader(&fieldType, &fieldId, &fieldData, &nameHash);
        
        // Handle different field types
        if (fieldType == TYPE_INT32) {
            // Integer field - read and convert
            ReadBool(name, nullptr);
        } else if (fieldType == TYPE_FLOAT) {
            // Float field - this is what we're looking for
            uint32_t tempData = 0;
            bool hasName = (nameHash != 0);
            
            // Validate name if provided
            if (name) {
                // util_9410(name, fieldData, hasName);
            }
            
            // Process the field
            bool success = ProcessField(name, fieldId);
            
            if (success) {
                // Successfully read - increment array depth
                // rage_9100(&tempData);
                arrayDepth++;
            } else {
                // Failed - just increment counter
                arrayDepth++;
            }
        } else if (fieldType == TYPE_END) {
            // End marker - handle array/object closure
            // TODO: Call array end handler
            arrayDepth--;
            
            if (arrayDepth <= 0) {
                nestingLevel = 0;
                break;
            }
        } else if (fieldType == 0xFFFE) {
            // String/complex type - allocate and read
            uint32_t stringSize = 0;
            // _RtlCheckStack12();  // Stack check for large allocation
            
            // Setup buffer for reading
            uint32_t bufferOffset = m_rbfDataSize;
            // grc_3CD8(m_pRbfData, &bufferOffset, 1);
            m_currentDepth = 1;
            
            // Read array elements
            while (bufferOffset > 0) {
                uint32_t elementSize = bufferOffset;
                uint32_t maxSize = 0;  // m_maxArraySize from offset +56
                
                bool shouldTruncate = (bufferOffset > maxSize);
                
                // Read element using virtual call (slot 5)
                uint32_t bytesRead = 0;
                // VCALL: BeginArray(name, &bytesRead, 0);
                
                // Add to collection
                bool addSuccess = !shouldTruncate;
                // collection->Add(name, bytesRead, addSuccess);
                
                // Update buffer position
                bufferOffset = m_rbfDataSize;
            }
            
            m_currentDepth = 0;
        } else {
            // Other field type
            ReadBool(name, nullptr);
            arrayDepth++;
        }
        
        // Check if we're done
        if (nestingLevel == 0) {
            break;
        }
    }
    
    return true;
}

/**
 * parStreamInRbf::ReadBool @ 0x8241C1D8 | size: 0x84
 * 
 * Reads a boolean or small data value from the RBF stream. Handles variable-length
 * encoding where size is determined by the field header.
 */
bool parStreamInRbf::ReadBool(const char* name, bool* value) {
    uint32_t bytesToRead = 0;
    
    // Read size from buffer
    // grc_3CD8(m_pRbfData, &bytesToRead, 1);
    
    // Read data in chunks
    while (bytesToRead > 0) {
        uint32_t chunkSize = bytesToRead;
        uint32_t maxChunkSize = 0;  // From offset +56
        
        // Clamp to max chunk size if needed
        if (chunkSize > maxChunkSize) {
            chunkSize = maxChunkSize;
        }
        
        // Read chunk from buffer
        uint32_t bytesRead = 0;
        // rage_obj_bind_3828(m_pRbfData, name, chunkSize);
        
        // Update remaining bytes
        bytesToRead -= bytesRead;
    }
    
    return true;
}

/**
 * parStreamInRbf::ProcessField @ 0x8241C628 | size: 0x1C0
 * 
 * Processes a field based on its type. Handles integers, floats, strings, booleans,
 * and vectors by reading the appropriate number of bytes and calling type-specific handlers.
 */
bool parStreamInRbf::ProcessField(const char* name, uint16_t fieldType) {
    // Check field type range
    if (fieldType > 5) {
        return false;
    }
    
    // Dispatch based on field type
    switch (fieldType) {
        case 0: {
            // Object/structure type
            // grc_C3B8(this, name);
            return false;
        }
        
        case 1: {
            // Integer type
            uint32_t intValue = 0;
            // grc_3CD8(m_pRbfData, &intValue, 1);
            
            // Format and store
            // jumptable_95F0(name, "%d", intValue, 0);
            return true;
        }
        
        case 2: {
            // Boolean type
            // jumptable_9548(name, "%d", 1, 0);
            return true;
        }
        
        case 3: {
            // Boolean false
            // jumptable_9548(name, "%d", 0, 0);
            return true;
        }
        
        case 4: {
            // Float type
            float floatValue = 0.0f;
            // grc_3CD8(m_pRbfData, &floatValue, 1);
            
            // Format and store
            // jumptable_9698(name, "%f", floatValue, 0);
            return true;
        }
        
        case 5: {
            // Vector3 type (3 floats)
            float vec[3] = {0.0f, 0.0f, 0.0f};
            // grc_3CD8(m_pRbfData, vec, 3);
            
            // Format and store each component
            // jumptable_9698(name, "x", vec[0], 0);
            // jumptable_9698(name, "y", vec[1], 0);
            // jumptable_9698(name, "z", vec[2], 0);
            return true;
        }
        
        default:
            return false;
    }
}

/**
 * parStreamInRbf::ReadFieldHeader @ 0x8241C260 | size: 0x158
 * 
 * Reads a field header from the RBF stream. Handles both compact (16-bit) and
 * extended (32-bit) field encoding formats.
 */
void parStreamInRbf::ReadFieldHeader(uint16_t* outFieldType, uint16_t* outFieldId, 
                                      uint32_t* outFieldData, uint32_t* outNameHash) {
    constexpr uint16_t TYPE_INT32 = 0xFFFC;
    
    uint16_t fieldMarker = 0xFFFF;  // Invalid marker
    bool foundValid = false;
    
    // Read until we find a valid field or hit a special marker
    while (!foundValid) {
        // Read 16-bit field marker
        // grc_3C68(m_pRbfData, &fieldMarker, 1);
        
        // Check if this is a special marker that needs processing
        if (fieldMarker == TYPE_INT32) {
            // Integer marker - process and continue
            ReadBool(nullptr, nullptr);
        } else {
            // Valid field found
            foundValid = true;
        }
    }
    
    // Store field marker
    *outFieldType = fieldMarker;
    
    // Check if this is an extended format field
    if (fieldMarker == 0xFFFE) {
        // Extended format: read 32-bit value
        uint32_t extendedValue = 0;
        // grc_3CD8(m_pRbfData, &extendedValue, 1);
        
        // Extract type (upper 4 bits) and field ID (lower 28 bits)
        *outFieldId = (extendedValue >> 28) & 0xF;
        *outFieldData = extendedValue & 0xFFFFFFF;
    } else {
        // Compact format: type in upper 4 bits, field ID in lower 12 bits
        *outFieldId = (fieldMarker >> 12) & 0xF;
        *outFieldData = fieldMarker & 0xFFF;
    }
    
    // Check if field type is valid (15 = end marker)
    if (*outFieldId == 15) {
        *outNameHash = 0;
        return;
    }
    
    // Look up field name in dictionary
    uint32_t dictionaryPtr = m_currentOffset;  // From offset +60
    if (dictionaryPtr == 0) {
        dictionaryPtr = reinterpret_cast<uint32_t>(this) + 68;  // Default dictionary offset
    }
    
    // Check if field ID matches dictionary entry
    uint16_t dictFieldId = 0;  // Read from dictionary at +4
    if (*outFieldData == dictFieldId) {
        // Allocate string buffer
        uint32_t stringLength = 0;
        // grc_3C68(m_pRbfData, &stringLength, 1);
        
        // Allocate buffer
        char* stringBuffer = nullptr;  // xe_EC88(stringLength + 1);
        
        // Read string data
        // rage_obj_bind_3828(m_pRbfData, stringBuffer, stringLength);
        
        // Null-terminate
        if (stringBuffer) {
            stringBuffer[stringLength] = '\0';
        }
    }
    
    // Get field data pointer from dictionary
    // uint32_t* fieldDataArray = dictionary[0];
    // *outNameHash = fieldDataArray[*outFieldData];
}



bool parStreamInXml::BeginArray(const char* name, uint32_t* count) {
    // @ 0x8241AD38
    return false;  // TODO: implement
}

void parStreamInXml::EndArray() {
    // @ 0x8241AD08
}

bool parStreamInXml::ReadInt(const char* name, int32_t* value) {
    // @ 0x8241ADD0
    return false;  // TODO: implement
}

bool parStreamInXml::ReadFloat(const char* name, float* value) {
    // @ 0x8241AEF8
    return false;  // TODO: implement
}

bool parStreamInXml::ReadString(const char* name, char* buffer, size_t bufferSize) {
    // @ 0x8241ACA8
    return false;  // TODO: implement
}

/**
 * parStreamInXml::ReadBool @ 0x82419568 | size: 0x08
 * Returns the boolean state flag at offset +1095 (m_bLastReadWasBool).
 */
bool parStreamInXml::ReadBool(const char* name, bool* value) {
    return *(uint8_t*)((char*)this + 1095) != 0;
}

// ────────────────────────────────────────────────────────────────────────────
// parStreamOutXml — XML output stream
// ────────────────────────────────────────────────────────────────────────────

parStreamOutXml::parStreamOutXml()
    : m_pXmlBuffer(nullptr)
    , m_bufferSize(0)
    , m_bufferUsed(0)
    , m_currentDepth(0)
    , m_indentSpaces(2)
    , m_isValid(false) {
}

/**
 * parStreamOutXml::~parStreamOutXml @ 0x8241B810 | size: 0x2C
 * 
 * Scalar destructor for XML output stream.
 */
parStreamOutXml::~parStreamOutXml() {
    if (m_pXmlBuffer) {
        // Free buffer
        m_pXmlBuffer = nullptr;
    }
}

/**
 * parStreamOutXml_9488_2hr @ 0x82419488 | size: 0x60
 * 
 * Base64-like encoding helper. Converts 6-bit values to ASCII characters.
 * Used for encoding binary data in XML output.
 */
static uint8_t EncodeBase64Char(uint8_t value) {
    uint8_t v = value & 0xFF;
    
    if (v < 26) {
        return v + 65;  // 'A'-'Z'
    }
    if (v < 52) {
        return v + 71;  // 'a'-'z'
    }
    if (v < 62) {
        return v + 252;  // '0'-'9' (wraps around due to uint8_t)
    }
    if (v == 62) {
        return 43;  // '+'
    }
    if (v == 63) {
        return 47;  // '/'
    }
    
    return 0;
}

bool parStreamOutXml::CreateFile(const char* filename) {
    // TODO: implement
    m_isValid = true;
    return true;
}

/**
 * parStreamOutXml::FlushToFile @ 0x82432A70 | size: 0x0C
 * Flushes the XML output buffer to the file handle.
 */
bool parStreamOutXml::FlushToFile() {
    extern "C" void fiAsciiTokenizer_FlushBuffer(void* file, int flags);
    fiAsciiTokenizer_FlushBuffer(*(void**)((char*)this + 4), 0);
    return true;
}

void parStreamOutXml::SetIndentation(int spaces) {
    m_indentSpaces = spaces;
}

/**
 * parStreamOutXml::BeginObject @ 0x8241BB38 | size: 0x08
 * Opens an XML element tag. Tail-calls the tokenizer with isArray=false.
 */
bool parStreamOutXml::BeginObject(const char* name) {
    extern "C" bool fiAsciiTokenizer_BeginElement(void* stream, const char* name, int isArray);
    return fiAsciiTokenizer_BeginElement(this, name, 0);
}

void parStreamOutXml::EndObject() {
    // @ 0x8241BB40
}

/**
 * parStreamOutXml::BeginArray @ 0x8241BBE8 | size: 0x38
 * Opens an XML array element. Calls tokenizer with isArray=true,
 * then resets m_arrayElementIndex (+28) to 0.
 */
bool parStreamOutXml::BeginArray(const char* name, uint32_t* count) {
    extern "C" bool fiAsciiTokenizer_BeginElement(void* stream, const char* name, int isArray);
    fiAsciiTokenizer_BeginElement(this, name, 1);
    *(uint32_t*)((char*)this + 28) = 0;  // m_arrayElementIndex = 0
    return true;
}

void parStreamOutXml::EndArray() {
    // @ 0x8241BC20
}

/**
 * parStreamOutXml::WriteInt @ 0x8241B810 | size: 0x2C
 * Writes an integer value as XML text. Gets the file handle at +4,
 * loads a "%d" format string, and calls the XML formatter.
 */
bool parStreamOutXml::WriteInt(const char* name, int32_t value) {
    extern "C" void parStreamOutXml_WriteFormatted(void* file, const char* fmt, int32_t value);
    void* file = *(void**)((char*)this + 4);
    parStreamOutXml_WriteFormatted(file, "%d", value);
    return true;
}

bool parStreamOutXml::WriteFloat(const char* name, float value) {
    // @ 0x822E3E40
    return false;  // TODO: implement
}

bool parStreamOutXml::WriteString(const char* name, const char* value) {
    // @ 0x822E5D48
    return false;  // TODO: implement
}

bool parStreamOutXml::WriteBool(const char* name, bool value) {
    // @ 0x822E5DC8
    return false;  // TODO: implement
}

// ────────────────────────────────────────────────────────────────────────────
// parStreamInRbf — RBF input stream
// ────────────────────────────────────────────────────────────────────────────

parStreamInRbf::parStreamInRbf()
    : m_pRbfData(nullptr)
    , m_rbfDataSize(0)
    , m_currentOffset(0)
    , m_currentDepth(0)
    , m_isValid(false) {
}

/**
 * parStreamInRbf::~parStreamInRbf @ 0x8241C160 | size: 0x78
 * 
 * Destructor for RBF input stream. Frees allocated buffer at offset +68
 * if flag at offset +74 is set.
 */
parStreamInRbf::~parStreamInRbf() {
    // Check flag at offset +74 (m_ownsBuffer)
    // If set, free buffer at offset +68
    if (m_pRbfData) {
        // rage_free would be called here
        m_pRbfData = nullptr;
    }
}

bool parStreamInRbf::OpenFile(const char* filename) {
    // @ 0x8241C260
    return false;  // TODO: implement
}

void parStreamInRbf::CloseFile() {
    // @ 0x8241C628
    if (m_pRbfData) {
        m_pRbfData = nullptr;
    }
    m_isValid = false;
}

bool parStreamInRbf::IsValid() const {
    return m_isValid;
}

bool parStreamInRbf::BeginObject(const char* name) {
    // @ 0x8241C7E8
    return false;  // TODO: implement
}

void parStreamInRbf::EndObject() {
    // @ 0x8241C8C8
}

bool parStreamInRbf::BeginArray(const char* name, uint32_t* count) {
    // @ 0x8241CA10
    return false;  // TODO: implement
}

void parStreamInRbf::EndArray() {
    // @ 0x8241C868
}

bool parStreamInRbf::ReadInt(const char* name, int32_t* value) {
    // @ 0x8241C930
    return false;  // TODO: implement
}

bool parStreamInRbf::ReadFloat(const char* name, float* value) {
    // @ 0x8241CB38
    return false;  // TODO: implement
}

bool parStreamInRbf::ReadString(const char* name, char* buffer, size_t bufferSize) {
    // @ 0x8241C140
    return false;  // TODO: implement
}

bool parStreamInRbf::ReadBool(const char* name, bool* value) {
    // @ 0x8241C1D8
    return false;  // TODO: implement
}

// ────────────────────────────────────────────────────────────────────────────
// parStreamOutRbf — RBF output stream
// ────────────────────────────────────────────────────────────────────────────

parStreamOutRbf::parStreamOutRbf()
    : m_pRbfBuffer(nullptr)
    , m_bufferSize(0)
    , m_bufferUsed(0)
    , m_currentDepth(0)
    , m_isValid(false) {
}

/**
 * parStreamOutRbf::~parStreamOutRbf @ 0x8241CD50 | size: 0x70
 * Scalar destructor @ 0x8241CDC0
 * 
 * Destructor for RBF output stream.
 */
parStreamOutRbf::~parStreamOutRbf() {
    if (m_pRbfBuffer) {
        m_pRbfBuffer = nullptr;
    }
}

bool parStreamOutRbf::CreateFile(const char* filename) {
    m_isValid = true;
    return true;
}

bool parStreamOutRbf::FlushToFile() {
    return false;  // TODO: implement
}

bool parStreamOutRbf::BeginObject(const char* name) {
    // @ 0x8241D2C8
    return false;  // TODO: implement
}

void parStreamOutRbf::EndObject() {
    // @ 0x8241D520
}

bool parStreamOutRbf::BeginArray(const char* name, uint32_t* count) {
    // @ 0x8241D570
    return false;  // TODO: implement
}

void parStreamOutRbf::EndArray() {
    // @ 0x8241CF90
}

bool parStreamOutRbf::WriteInt(const char* name, int32_t value) {
    // @ 0x8241CDD8
    return false;  // TODO: implement
}

bool parStreamOutRbf::WriteFloat(const char* name, float value) {
    // @ 0x8241CE58
    return false;  // TODO: implement
}

bool parStreamOutRbf::WriteString(const char* name, const char* value) {
    // @ 0x8241CED0
    return false;  // TODO: implement
}

bool parStreamOutRbf::WriteBool(const char* name, bool value) {
    // @ 0x8241D128
    return false;  // TODO: implement
}

} // namespace rage
