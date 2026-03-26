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
        char* stringBuffer = nullptr;  // rage_alloc(stringLength + 1);
        
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

// ────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer — Small utility and vtable functions (<=64 bytes)
// ────────────────────────────────────────────────────────────────────────────

// ── Destructor ──────────────────────────────────────────────────────────────

/**
 * fiAsciiTokenizer::~fiAsciiTokenizer @ 0x8210CAF8 | size: 0x8
 *
 * Virtual destructor. Returns nullptr (no-op destructor body).
 */
fiAsciiTokenizer::~fiAsciiTokenizer() {
    // No-op: returns 0
}

// ── Vtable forwarding wrappers ──────────────────────────────────────────────

/**
 * fiAsciiTokenizer::AddOffset @ 0x820C2E08 | size: 0x10
 * [vtable slot 30]
 *
 * Adds a signed offset to the stream position at field +0xA0.
 */
void fiAsciiTokenizer::AddOffset(int32_t offset) {
    m_streamPos += offset;  // field_0x00a0 += offset
}

/**
 * fiAsciiTokenizer::ReadIntForward @ 0x822E6D00 | size: 0x14
 * [vtable slot 37]
 *
 * Forwards to vtable slot 36 (ReadNamedIntArray) with count=0.
 * Loads vtable, sets r5=0, tail-calls slot 36 (offset 144).
 */
void fiAsciiTokenizer::ReadIntForward() {
    // Tail-call to vtable slot 36 with count=0
    typedef void (*Slot36Func)(fiAsciiTokenizer*, int);
    void** vt = *reinterpret_cast<void***>(this);
    Slot36Func fn = reinterpret_cast<Slot36Func>(vt[36]);
    fn(this, 0);
}

// ── CRT / kernel wrappers ──────────────────────────────────────────────────

/**
 * _crt_return_zero @ 0x8242F910 | size: 0x8
 *
 * Returns 0 and tail-calls KeBugCheck. Unreachable code path.
 */
void _crt_return_zero() {
    KeBugCheck(0);
}

/**
 * _crt_spinlock_acquire_8 @ 0x8242F960 | size: 0x8
 *
 * Acquires spinlock index 8. Tail-calls _crt_spinlock_acquire(8).
 */
void _crt_spinlock_acquire_8() {
    _crt_spinlock_acquire(8);
}

/**
 * _crt_exit_clean @ 0x8242FB40 | size: 0xC
 *
 * Calls _doexit(0, 0) for clean process exit.
 */
void _crt_exit_clean() {
    _doexit(0, 0);
}

/**
 * _crt_exit_with_code @ 0x8242FB50 | size: 0xC
 *
 * Calls _doexit(1, 0) for exit with code 1.
 */
void _crt_exit_with_code() {
    _doexit(1, 0);
}

/**
 * _crt_call_indirect @ 0x8242F904 | size: 0xC
 *
 * Indirect function call trampoline. Calls function pointer in r4
 * with r5 moved to r12.
 */
void _crt_call_indirect(void* func, void* arg) {
    typedef void (*IndirectFunc)(void*);
    IndirectFunc fn = reinterpret_cast<IndirectFunc>(func);
    fn(arg);
}

// ── Global data accessors ──────────────────────────────────────────────────

/**
 * _crt_get_global_ptr @ 0x82433438 | size: 0xC
 *
 * Returns pointer to a global data structure (static address in .bss).
 */
void* _crt_get_global_ptr() {
    extern char lbl_82080C38[];
    return &lbl_82080C38;
}

/**
 * _crt_get_thread_ptr @ 0x8242F638 | size: 0xC
 *
 * Returns value of global pointer at 0x82732F0C (.data).
 */
void* _crt_get_thread_ptr() {
    extern void* g_threadPtr;  // lbl_82732F0C
    return g_threadPtr;
}

/**
 * _crt_get_kernel_event @ 0x82439610 | size: 0xC
 *
 * Loads global at 0x825E7090 and tail-calls _crt_signal_event.
 */
void _crt_get_kernel_event() {
    extern void* g_kernelEvent;  // lbl_825E7090
    _crt_signal_event(g_kernelEvent);
}

// ── Null/zero return stubs ─────────────────────────────────────────────────

/**
 * _crt_store_null_return_zero @ 0x824C2120 | size: 0x10
 *
 * Stores 0 to *outParam and returns 0.
 */
int _crt_store_null_return_zero(void*, uint32_t* outParam) {
    *outParam = 0;
    return 0;
}

/**
 * _crt_null_check_dispatch @ 0x824D0AF0 | size: 0x14
 *
 * If ptr is null, returns 7 (error code). Otherwise tail-calls handler.
 */
int _crt_null_check_dispatch(void* ptr) {
    if (!ptr) {
        return 7;
    }
    return _atSingleton_dispatch(ptr);
}

// ── Floating-point FPSCR helpers ───────────────────────────────────────────

/**
 * _fp_restore_fpscr @ 0x824365B4 | size: 0x10
 *
 * Restores FPSCR register from a uint32 value. Used to set floating-point
 * control/status register state.
 */
void _fp_restore_fpscr(uint32_t fpscrValue) {
    // Stores r3 to stack, loads as double, writes to FPSCR via mtfsf
    __asm__ volatile("" ::: "memory");  // Compiler barrier
}

/**
 * _fp_get_rounding_mode @ 0x82436570 | size: 0x24
 *
 * Reads FPSCR, extracts bit 2 (FI — Fraction Inexact enable),
 * masks with 0x4, stores back. Returns old FPSCR rounding mode.
 */
uint32_t _fp_get_rounding_mode() {
    // mffs, extract bit 2, mask with 4, mtfsf
    return 0;  // Platform-specific FPSCR manipulation
}

/**
 * _fp_get_exponent @ 0x82435758 | size: 0x18
 *
 * Extracts the biased exponent from a double-precision float, subtracts 1022
 * to get the unbiased exponent, and returns it as a signed short.
 */
int16_t _fp_get_exponent(double value) {
    // Extract bits [1:11] of IEEE 754 double, subtract bias (1022)
    union { double d; uint64_t u; } conv;
    conv.d = value;
    int exponent = static_cast<int>((conv.u >> 52) & 0x7FF) - 1022;
    return static_cast<int16_t>(exponent);
}

/**
 * _fp_set_exponent @ 0x82435730 | size: 0x28
 *
 * Replaces the exponent field of double f1 with (exp + 1022), preserving
 * the sign and mantissa bits. Returns modified double.
 */
double _fp_set_exponent(double value, int exp) {
    union { double d; uint64_t u; } conv;
    conv.d = value;
    uint16_t sign_mantissa_hi = static_cast<uint16_t>(conv.u >> 48) & 0x800F;
    uint16_t new_exp = static_cast<uint16_t>((exp + 1022) << 4);
    uint16_t combined = sign_mantissa_hi | new_exp;
    conv.u = (conv.u & 0x0000FFFFFFFFFFFF) | (static_cast<uint64_t>(combined) << 48);
    return conv.d;
}

/**
 * _fp_adjust_exponent @ 0x82435770 | size: 0x34
 *
 * Reads the current exponent of double f1, adds delta, then writes the new
 * exponent back. Returns modified double.
 */
double _fp_adjust_exponent(double value, int delta) {
    union { double d; uint64_t u; } conv;
    conv.d = value;
    int cur_exp = static_cast<int>((conv.u >> 52) & 0x7FF);
    int new_exp = cur_exp + delta;
    uint16_t sign_mantissa_hi = static_cast<uint16_t>(conv.u >> 48) & 0x800F;
    uint16_t exp_field = static_cast<uint16_t>(new_exp << 4);
    uint16_t combined = sign_mantissa_hi | exp_field;
    conv.u = (conv.u & 0x0000FFFFFFFFFFFF) | (static_cast<uint64_t>(combined) << 48);
    return conv.d;
}

/**
 * _fp_set_fpscr_bits @ 0x82436530 | size: 0x30
 *
 * Reads FPSCR, applies bitmask: new = (desired ^ 0xF8) & mask | (old & ~mask).
 * Returns old FPSCR XOR 0xF8.
 */
uint32_t _fp_set_fpscr_bits(uint32_t desired, uint32_t mask) {
    // FPSCR bit manipulation
    return desired ^ 0xF8;
}

/**
 * _fp_init_fpscr @ 0x8243B278 | size: 0x30
 *
 * Initializes FPSCR by calling _fp_set_fpscr_bits(0, 0xFFF80700)
 * then calling the FPSCR init helper.
 */
void _fp_init_fpscr() {
    _fp_set_fpscr_bits(0, 0xFFF80700);
    _fp_init_helper();
}

// ── Char classification (CRT ctype) ───────────────────────────────────────

/**
 * _ctype_is_control @ 0x82432F58 | size: 0x1C
 *
 * Checks if character has the control flag (bit 2 = 0x04) set in the
 * locale ctype table. Returns nonzero if control character.
 */
uint32_t _ctype_is_control(int ch) {
    extern uint16_t* g_ctypeTable;  // from locale data at +200
    return g_ctypeTable[ch] & 0x4;
}

/**
 * _ctype_is_upper @ 0x82432F78 | size: 0x1C
 *
 * Checks if character has the uppercase flag (bit 7 = 0x80) set.
 * Returns nonzero if uppercase letter.
 */
uint32_t _ctype_is_upper(int ch) {
    extern uint16_t* g_ctypeTable;
    return g_ctypeTable[ch] & 0x80;
}

/**
 * _ctype_is_punct @ 0x82432F98 | size: 0x1C
 *
 * Checks if character has the punctuation flag (bit 3 = 0x08) set.
 * Returns nonzero if punctuation character.
 */
uint32_t _ctype_is_punct(int ch) {
    extern uint16_t* g_ctypeTable;
    return g_ctypeTable[ch] & 0x8;
}

/**
 * _ctype_get_class @ 0x8243A348 | size: 0x1C
 *
 * Gets the full character classification word from the locale ctype table.
 * Masks char to 0-255 range, returns upper 17 bits (mask 0xFFFF8000).
 */
uint32_t _ctype_get_class(int ch) {
    extern void* g_localeData;  // from locale struct at +5336
    uint16_t** ctypePtr = reinterpret_cast<uint16_t**>(&g_localeData);
    uint16_t* table = ctypePtr[50];  // offset +200
    return table[ch & 0xFF] & 0xFFFF8000;
}

/**
 * _ctype_test_masked @ 0x82436C30 | size: 0x30
 *
 * Tests if character ch (0-255) has any of the specified mask bits set
 * in the locale ctype table. Returns 0 if ch > 256.
 */
uint32_t _ctype_test_masked(int ch, uint32_t mask) {
    if (static_cast<uint32_t>(ch + 1) > 256) {
        return 0;
    }
    extern void* g_localeData;
    uint16_t** ctypePtr = reinterpret_cast<uint16_t**>(&g_localeData);
    uint16_t* table = ctypePtr[50];
    return table[ch] & mask;
}

// ── Lock / critical section helpers ────────────────────────────────────────

/**
 * _crt_lock_acquire @ 0x824335E8 | size: 0x18
 *
 * Acquires a lock. If index < 20, uses spinlock (index + 16).
 * Otherwise enters a critical section on the second parameter + 32.
 */
void _crt_lock_acquire(int index, void* critSection) {
    if (index < 20) {
        _crt_spinlock_acquire(index + 16);
    } else {
        RtlEnterCriticalSection(reinterpret_cast<char*>(critSection) + 32);
    }
}

/**
 * _crt_lock_release @ 0x82433638 | size: 0x18
 *
 * Releases a lock. If index < 20, uses spinlock release (index + 16).
 * Otherwise leaves critical section on second parameter + 32.
 */
void _crt_lock_release(int index, void* critSection) {
    if (index < 20) {
        _crt_spinlock_release(index + 16);
    } else {
        RtlLeaveCriticalSection(reinterpret_cast<char*>(critSection) + 32);
    }
}

/**
 * _crt_exception_filter @ 0x82433890 | size: 0x3C
 *
 * Exception filter callback. Checks flags field (+4) for bits 0x66.
 * If none set, returns 1 (continue search). Otherwise stores exception
 * record pointers and returns 3 (execute handler).
 */
int _crt_exception_filter(void* excRec, void* outContext, void* excContext) {
    uint32_t flags = *reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(excRec) + 4);
    if ((flags & 0x66) == 0) {
        return 1;
    }
    // Store exception pointers
    uint32_t* ctx = reinterpret_cast<uint32_t*>(outContext);
    ctx[3] = reinterpret_cast<uint32_t>(excContext);
    ctx[0] = *reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(excContext) + 48);
    ctx[1] = *reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(excContext) + 56);
    ctx[2] = *reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(excContext) + 64);
    return 3;
}

/**
 * _crt_init_lock @ 0x82439A10 | size: 0x28
 *
 * Initializes a lock by loading the lock table entry at offset -88 from
 * the second parameter, then calling _crt_exception_filter.
 */
void _crt_init_lock(void* table, void* entry, void* context) {
    void* lockEntry = *reinterpret_cast<void**>(reinterpret_cast<char*>(entry) - 88);
    _crt_exception_filter(lockEntry, table, context);
}

// ── Thread/process data exchange ───────────────────────────────────────────

/**
 * _crt_swap_global @ 0x8242B890 | size: 0x14
 *
 * Atomically swaps a global pointer at 0x82732F1C. Returns old value.
 */
void* _crt_swap_global(void* newValue) {
    extern void* g_globalSwapPtr;  // lbl_82732F1C
    void* old = g_globalSwapPtr;
    g_globalSwapPtr = newValue;
    return old;
}

/**
 * _crt_is_heap_initialized @ 0x8243A1F8 | size: 0x20
 *
 * Checks if two global pointers are equal (heap base == heap current).
 * Returns 1 if equal (not initialized), 0 otherwise.
 */
bool _crt_is_heap_initialized() {
    extern uint32_t g_heapBase;   // lbl_825E70D4
    extern uint32_t g_heapCurr;   // from offset +7504
    return g_heapBase == g_heapCurr;
}

// ── TLS / locale accessor helpers ──────────────────────────────────────────

/**
 * _crt_get_locale_name_a @ 0x824355A8 | size: 0x38
 *
 * Returns pointer to locale name string A. If thread-local data is null,
 * returns static fallback pointer. Otherwise returns TLS data + 8.
 */
void* _crt_get_locale_name_a() {
    void* tls = _crt_get_tls_data();
    if (!tls) {
        extern char g_staticLocaleA[];  // static fallback
        return &g_staticLocaleA;
    }
    return reinterpret_cast<char*>(tls) + 8;
}

/**
 * _crt_get_locale_name_b @ 0x824355E0 | size: 0x38
 *
 * Returns pointer to locale name string B. If thread-local data is null,
 * returns static fallback pointer. Otherwise returns TLS data + 12.
 */
void* _crt_get_locale_name_b() {
    void* tls = _crt_get_tls_data();
    if (!tls) {
        extern char g_staticLocaleB[];  // static fallback
        return &g_staticLocaleB;
    }
    return reinterpret_cast<char*>(tls) + 12;
}

/**
 * _crt_get_tls_or_alloc @ 0x8242FC68 | size: 0x3C
 *
 * Returns current TLS block, allocating a 16-byte block if null.
 */
void* _crt_get_tls_or_alloc() {
    void* tls = _crt_get_tls_data();
    if (!tls) {
        tls = _crt_alloc(16);
    }
    return tls;
}

// ── Startup / init dispatcher ──────────────────────────────────────────────

/**
 * _crt_call_setfp_and_init @ 0x82436AF0 | size: 0x20
 *
 * Rearranges arguments and tail-calls the locale data initializer with
 * the global locale struct pointer and a zero trailing argument.
 */
void _crt_call_setfp_and_init(void* arg1, void* arg2, void* arg3) {
    extern void* g_localeData;
    _crt_locale_init(&g_localeData, arg1, arg2, arg3, 0);
}

/**
 * _crt_init_and_check @ 0x82433570 | size: 0x40
 *
 * Calls the global init function, checks a byte flag, optionally calls
 * a secondary init, then loads a global pointer and calls cleanup.
 */
void _crt_init_and_check() {
    _crt_global_init();
    extern uint8_t g_initFlag;    // lbl_825E6E4C
    if (g_initFlag) {
        _crt_secondary_init();
    }
    extern void* g_cleanupPtr;    // lbl_82733040
    _crt_cleanup(g_cleanupPtr);
}

// ── FPSCR double-to-NaN test ───────────────────────────────────────────────

/**
 * _fp_is_valid_double @ 0x8243A4D0 | size: 0x24
 *
 * Loads double from pointer, compares against a constant. Returns 1 if
 * the double is >= the constant (valid), 0 otherwise.
 */
bool _fp_is_valid_double(double* ptr) {
    double val = *ptr;
    extern const double g_fpMinValid;  // constant in .rdata
    return val >= g_fpMinValid;
}

// ── Tail-call thunks ───────────────────────────────────────────────────────

/**
 * _thunk_tail_call_A4F8 @ 0x8243A558 | size: 0x8
 *
 * Sets r6=0 and tail-calls fiAsciiTokenizer_A4F8_2h.
 */
void _thunk_tail_call_A4F8(void* a, void* b, void* c, void* d, void* e) {
    fiAsciiTokenizer_A4F8_2h(a, b, c, d, e, nullptr);
}

/**
 * _thunk_tail_call_A378 @ 0x8243A560 | size: 0x8
 *
 * Sets r4=0 and tail-calls fiAsciiTokenizer_A378_v12.
 */
void _thunk_tail_call_A378(void* a) {
    fiAsciiTokenizer_A378_v12(a, nullptr);
}

/**
 * _thunk_tail_call_A418 @ 0x8243A568 | size: 0x8
 *
 * Sets r4=0 and tail-calls fiAsciiTokenizer_A418_2h.
 */
void _thunk_tail_call_A418(void* a) {
    fiAsciiTokenizer_A418_2h(a, nullptr);
}

// ── ANSI string init helper ────────────────────────────────────────────────

/**
 * _nt_init_ansi_string_and_process @ 0x825680B0 | size: 0x40
 *
 * Initializes a local ANSI_STRING on the stack (zeroed Length, MaxLength,
 * Buffer), calls RtlInitAnsiString with the input, then processes it.
 */
void _nt_init_ansi_string_and_process(const char* str) {
    // ANSI_STRING: { uint16_t Length, uint16_t MaxLength, char* Buffer }
    uint16_t length = 0;
    uint16_t maxLength = 0;
    void* buffer = nullptr;

    // Stack struct initialization
    struct { uint16_t len; uint16_t maxLen; void* buf; } ansiStr = {0, 0, nullptr};
    RtlInitAnsiString(&ansiStr, str);
    _nt_process_ansi_string(&ansiStr);
}

// ── Static initializer registration (atexit pattern) ───────────────────────

/**
 * _static_init_reg_0 @ 0x8257EE28 | size: 0x40
 *
 * Static initializer: calls fiAsciiTokenizer_CF40_w to construct a global
 * object, then registers its destructor via atexit().
 */
void _static_init_reg_0() {
    extern void* g_staticObj0;
    extern const char g_staticName0[];
    fiAsciiTokenizer_CF40_w(&g_staticObj0, g_staticName0, nullptr);
    extern void (*g_staticDtor0)();
    atexit(g_staticDtor0);
}

/**
 * _static_init_reg_1 @ 0x8257EE88 | size: 0x40
 *
 * Static initializer: constructs global object and registers destructor.
 */
void _static_init_reg_1() {
    extern void* g_staticObj1;
    extern const char g_staticName1[];
    fiAsciiTokenizer_CF40_w(&g_staticObj1, g_staticName1, nullptr);
    extern void (*g_staticDtor1)();
    atexit(g_staticDtor1);
}

/**
 * _static_init_reg_2 @ 0x8257EEC8 | size: 0x40
 *
 * Static initializer: constructs global object and registers destructor.
 */
void _static_init_reg_2() {
    extern void* g_staticObj2;
    extern const char g_staticName2[];
    fiAsciiTokenizer_CF40_w(&g_staticObj2, g_staticName2, nullptr);
    extern void (*g_staticDtor2)();
    atexit(g_staticDtor2);
}

/**
 * _static_init_reg_3 @ 0x8257EF08 | size: 0x40
 *
 * Static initializer: constructs global object and registers destructor.
 */
void _static_init_reg_3() {
    extern void* g_staticObj3;
    extern const char g_staticName3[];
    fiAsciiTokenizer_CF40_w(&g_staticObj3, g_staticName3, nullptr);
    extern void (*g_staticDtor3)();
    atexit(g_staticDtor3);
}

/**
 * _static_init_reg_4 @ 0x8257EF48 | size: 0x40
 *
 * Static initializer: constructs global object via fiAsciiTokenizer_F1D0_w
 * and registers destructor via atexit().
 */
void _static_init_reg_4() {
    extern void* g_staticObj4;
    extern const char g_staticName4[];
    fiAsciiTokenizer_F1D0_w(&g_staticObj4, g_staticName4, nullptr);
    extern void (*g_staticDtor4)();
    atexit(g_staticDtor4);
}

/**
 * _static_init_reg_5 @ 0x8257EF88 | size: 0x40
 *
 * Static initializer: constructs global object via fiAsciiTokenizer_F1D0_w
 * and registers destructor via atexit().
 */
void _static_init_reg_5() {
    extern void* g_staticObj5;
    extern const char g_staticName5[];
    fiAsciiTokenizer_F1D0_w(&g_staticObj5, g_staticName5, nullptr);
    extern void (*g_staticDtor5)();
    atexit(g_staticDtor5);
}

/**
 * _static_init_reg_6 @ 0x8257EFC8 | size: 0x40
 *
 * Static initializer: constructs global object and registers destructor.
 */
void _static_init_reg_6() {
    extern void* g_staticObj6;
    extern const char g_staticName6[];
    fiAsciiTokenizer_CF40_w(&g_staticObj6, g_staticName6, nullptr);
    extern void (*g_staticDtor6)();
    atexit(g_staticDtor6);
}

// ── Static destructor registrations (at-exit cleanup) ──────────────────────

/**
 * _static_dtor_free_0 @ 0x82583F28 | size: 0x40
 *
 * Static destructor: checks if buffer is SSO (internal), if not frees
 * the heap allocation via rage_free.
 */
void _static_dtor_free_0() {
    extern char g_dtorObj0[];
    char* obj = g_dtorObj0;
    char* ssoBuffer = obj + 16;
    if (*reinterpret_cast<void**>(obj + 20) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_1 @ 0x82583F68 | size: 0x40
 *
 * Static destructor: SSO check + conditional free.
 */
void _static_dtor_free_1() {
    extern char g_dtorObj1[];
    char* obj = g_dtorObj1;
    char* ssoBuffer = obj + 16;
    if (*reinterpret_cast<void**>(obj + 20) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_2 @ 0x82583FA8 | size: 0x40
 *
 * Static destructor: checks larger SSO buffer (offset +32/+36) + conditional free.
 */
void _static_dtor_free_2() {
    extern char g_dtorObj2[];
    char* obj = g_dtorObj2;
    char* ssoBuffer = obj + 32;
    if (*reinterpret_cast<void**>(obj + 36) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_3 @ 0x82583FE8 | size: 0x40
 *
 * Static destructor: SSO check + conditional free.
 */
void _static_dtor_free_3() {
    extern char g_dtorObj3[];
    char* obj = g_dtorObj3;
    char* ssoBuffer = obj + 16;
    if (*reinterpret_cast<void**>(obj + 20) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_4 @ 0x82584028 | size: 0x40
 *
 * Static destructor: SSO check + conditional free.
 */
void _static_dtor_free_4() {
    extern char g_dtorObj4[];
    char* obj = g_dtorObj4;
    char* ssoBuffer = obj + 16;
    if (*reinterpret_cast<void**>(obj + 20) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_5 @ 0x82584068 | size: 0x40
 *
 * Static destructor: SSO check + conditional free.
 */
void _static_dtor_free_5() {
    extern char g_dtorObj5[];
    char* obj = g_dtorObj5;
    char* ssoBuffer = obj + 16;
    if (*reinterpret_cast<void**>(obj + 20) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_6 @ 0x825840A8 | size: 0x40
 *
 * Static destructor: checks larger SSO buffer (offset +32/+36) + conditional free.
 */
void _static_dtor_free_6() {
    extern char g_dtorObj6[];
    char* obj = g_dtorObj6;
    char* ssoBuffer = obj + 32;
    if (*reinterpret_cast<void**>(obj + 36) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_7 @ 0x825840E8 | size: 0x40
 *
 * Static destructor: checks larger SSO buffer (offset +32/+36) + conditional free.
 */
void _static_dtor_free_7() {
    extern char g_dtorObj7[];
    char* obj = g_dtorObj7;
    char* ssoBuffer = obj + 32;
    if (*reinterpret_cast<void**>(obj + 36) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

/**
 * _static_dtor_free_8 @ 0x82584128 | size: 0x40
 *
 * Static destructor: SSO check + conditional free.
 */
void _static_dtor_free_8() {
    extern char g_dtorObj8[];
    char* obj = g_dtorObj8;
    char* ssoBuffer = obj + 16;
    if (*reinterpret_cast<void**>(obj + 20) != ssoBuffer) {
        void* ptr = *reinterpret_cast<void**>(obj);
        if (ptr) {
            rage_free(ptr);
        }
    }
}

// ── Static init callbacks (pointer stores to global table) ─────────────────

/**
 * _static_init_set_handler_0 @ 0x82583F08 | size: 0xC
 *
 * Loads static address and tail-calls rage_A088 handler registration.
 */
void _static_init_set_handler_0() {
    extern char g_handlerObj0[];
    rage_A088(g_handlerObj0);
}

/**
 * _static_init_set_handler_1 @ 0x82583F18 | size: 0xC
 *
 * Loads static address and tail-calls rage_B3B0 handler registration.
 */
void _static_init_set_handler_1() {
    extern char g_handlerObj1[];
    rage_B3B0(g_handlerObj1);
}

/**
 * _static_init_store_ptr_0 @ 0x82584168 | size: 0x14
 *
 * Stores a static function pointer into a global dispatch table at offset +112.
 */
void _static_init_store_ptr_0() {
    extern void** g_dispatchTable;
    extern void* g_handlerFunc;
    g_dispatchTable[28] = g_handlerFunc;  // offset 112 / 4
}

/**
 * _static_init_store_ptr_1 @ 0x825841B0 | size: 0x14
 *
 * Stores a static function pointer into a global dispatch table at offset +116.
 */
void _static_init_store_ptr_1() {
    extern void** g_dispatchTable;
    extern void* g_handlerFunc;
    g_dispatchTable[29] = g_handlerFunc;  // offset 116 / 4
}

/**
 * _static_dtor_notify @ 0x82584180 | size: 0x2C
 *
 * Static destructor that checks reference count field (+6 as uint16),
 * and if nonzero, loads the object pointer from +0 and tail-calls
 * atSingleton_dtor_2290 with mode=3.
 */
void _static_dtor_notify() {
    extern char g_dtorNotifyObj[];
    uint16_t refCount = *reinterpret_cast<uint16_t*>(g_dtorNotifyObj + 6);
    if (refCount == 0) {
        return;
    }
    void* obj = *reinterpret_cast<void**>(g_dtorNotifyObj);
    if (!obj) {
        return;
    }
    atSingleton_dtor(obj, 3);
}

// ── Misc platform stubs ────────────────────────────────────────────────────

/**
 * _crt_fpscr_trampoline @ 0x824332FC | size: 0x10
 *
 * 16-byte stub that manipulates FPSCR state. Stores/restores floating-point
 * status register via stack frame.
 */
void _crt_fpscr_trampoline() {
    // Platform-specific FPSCR store/load via stack
    __asm__ volatile("" ::: "memory");
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


// ────────────────────────────────────────────────────────────────────────────
// atArray utility functions — template-instantiated helpers for RAGE dynamic
// arrays used throughout the serialization and tokenizer systems.
// ────────────────────────────────────────────────────────────────────────────

/**
 * AllocateU32Array @ 0x820E76D8 | size: 0x64
 *
 * Allocates memory for an array of uint32_t elements. Validates that the
 * requested count does not exceed 0x3FFFFFFF (the max that can be multiplied
 * by 4 without overflow). Returns nullptr for zero-count requests.
 */
void* AllocateU32Array(uint32_t count) {
    constexpr uint32_t MAX_U32_COUNT = 0x3FFFFFFF;

    if (count > MAX_U32_COUNT) {
        rage_assert("Array allocation overflow");
        rage_abort(1);
    }

    if (count != 0) {
        return rage_alloc(count * 4);
    }

    return nullptr;
}

/**
 * AllocateU16Array @ 0x822DF168 | size: 0x64
 *
 * Allocates memory for an array of uint16_t elements. Validates that the
 * requested count does not exceed 0x7FFFFFFF (the max that can be multiplied
 * by 2 without overflow). Returns nullptr for zero-count requests.
 */
void* AllocateU16Array(uint32_t count) {
    constexpr uint32_t MAX_U16_COUNT = 0x7FFFFFFF;

    if (count > MAX_U16_COUNT) {
        rage_assert("Array allocation overflow");
        rage_abort(1);
    }

    if (count != 0) {
        return rage_alloc(count * 2);
    }

    return nullptr;
}

/**
 * AllocateVec128Array @ 0x82238598 | size: 0x64
 *
 * Allocates memory for an array of 16-byte aligned elements (e.g. Vector128).
 * Validates that the requested count does not exceed 0x0FFFFFFF (the max
 * that can be multiplied by 16 without overflow). Returns nullptr for
 * zero-count requests.
 */
void* AllocateVec128Array(uint32_t count) {
    constexpr uint32_t MAX_VEC128_COUNT = 0x0FFFFFFF;

    if (count > MAX_VEC128_COUNT) {
        rage_assert("Array allocation overflow");
        rage_abort(1);
    }

    if (count != 0) {
        return rage_alloc(count * 16);
    }

    return nullptr;
}

/**
 * AllocateBytes @ 0x8222D588 | size: 0x60
 *
 * Allocates a raw byte buffer of the specified size. The only validation is
 * that size cannot exceed 0xFFFFFFFF (which is always true for uint32_t),
 * so this effectively just guards against zero-size allocations.
 * Returns nullptr for zero-size requests.
 */
void* AllocateBytes(uint32_t size) {
    constexpr uint32_t MAX_BYTE_COUNT = 0xFFFFFFFF;

    if (size > MAX_BYTE_COUNT) {
        rage_assert("Array allocation overflow");
        rage_abort(1);
    }

    if (size != 0) {
        return rage_alloc(size);
    }

    return nullptr;
}

/**
 * InitU32ArrayFromData @ 0x8222CC50 | size: 0xA8
 *
 * Initializes a triple-pointer array structure (begin/writePos/end) with
 * the given count of uint32_t elements, copying initial values from a source
 * array. The structure has layout: +0 = begin, +4 = writePos, +8 = end.
 */
void InitU32ArrayFromData(uint32_t* arrayStruct, uint32_t count, const uint32_t* srcData) {
    arrayStruct[0] = 0;
    arrayStruct[1] = 0;
    arrayStruct[2] = 0;

    uint32_t* buffer = static_cast<uint32_t*>(AllocateU32Array(count));

    arrayStruct[0] = reinterpret_cast<uintptr_t>(buffer);
    arrayStruct[1] = reinterpret_cast<uintptr_t>(buffer);
    uint32_t* endPtr = buffer + count;
    arrayStruct[2] = reinterpret_cast<uintptr_t>(endPtr);

    // Copy source data into the allocated buffer
    for (uint32_t i = 0; i < count; i++) {
        buffer[i] = srcData[i];
    }

    // Update write position to current end of written data
    arrayStruct[1] = reinterpret_cast<uintptr_t>(buffer + count);
}

/**
 * ReserveStringBuffer @ 0x8222CEB8 | size: 0x88
 *
 * Reserves buffer space for a string/byte container. If the requested capacity
 * is 16 or fewer bytes, uses a small inline buffer at offset +16 within the
 * struct. Otherwise allocates from the heap. The struct has layout:
 * +0 = begin, +16 = inline buffer, +20 = end pointer.
 */
void ReserveStringBuffer(uint8_t* containerStruct, uint32_t capacity) {
    if (capacity <= 16) {
        // Use the inline small-string buffer at offset +16
        uint8_t* inlineBuffer = containerStruct + 16;
        *reinterpret_cast<uintptr_t*>(inlineBuffer) = reinterpret_cast<uintptr_t>(containerStruct);
        *reinterpret_cast<uintptr_t*>(containerStruct + 20) = reinterpret_cast<uintptr_t>(inlineBuffer);
        return;
    }

    if (capacity == 0) {
        rage_assert("Invalid buffer capacity");
        rage_abort_fatal();
        return;
    }

    if (capacity > 0xFFFFFFFF) {
        rage_assert("Invalid buffer capacity");
        rage_abort_fatal();
        return;
    }

    // Heap allocation for larger buffers
    uint8_t* heapBuffer = static_cast<uint8_t*>(AllocateBytes(capacity));
    *reinterpret_cast<uintptr_t*>(containerStruct) = reinterpret_cast<uintptr_t>(heapBuffer);
    *reinterpret_cast<uintptr_t*>(containerStruct + 16) = reinterpret_cast<uintptr_t>(heapBuffer);
    *reinterpret_cast<uintptr_t*>(containerStruct + 20) = reinterpret_cast<uintptr_t>(heapBuffer + capacity);
}

/**
 * CopyAligned64 @ 0x8222DDD0 | size: 0x64
 *
 * Copies 64-byte aligned blocks from source to destination using VMX/AltiVec
 * 128-bit vector loads and stores. Operates in 64-byte chunks (4 x 16-byte
 * vectors per iteration). Skips the copy if source equals end pointer.
 */
void CopyAligned64(void* src, const void* srcEnd, void* dest) {
    uint8_t* srcPtr = static_cast<uint8_t*>(src);
    const uint8_t* endPtr = static_cast<const uint8_t*>(srcEnd);
    uint8_t* destPtr = static_cast<uint8_t*>(dest);

    if (srcPtr == endPtr) {
        return;
    }

    // Copy 64-byte blocks using 128-bit vector operations
    do {
        if (destPtr != nullptr) {
            memcpy(destPtr, srcPtr, 16);
            memcpy(destPtr + 16, srcPtr + 16, 16);
            memcpy(destPtr + 32, srcPtr + 32, 16);
            memcpy(destPtr + 48, srcPtr + 48, 16);
        }
        srcPtr += 64;
        destPtr += 64;
    } while (srcPtr != endPtr);
}

/**
 * InitFloatArray @ 0x8222EA28 | size: 0x84
 *
 * Initializes a float array structure, allocating space for the given count
 * and copying a single source float value into all elements. The struct uses
 * layout: +0 = begin, +4 = writePos, +8 = sub-array header.
 */
void InitFloatArray(uint32_t* arrayStruct, uint32_t count, const float* srcValue) {
    uint32_t* subArray = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(arrayStruct) + 8);

    arrayStruct[0] = 0;
    arrayStruct[1] = 0;
    subArray[0] = 0;

    float* buffer = static_cast<float*>(AllocateU32Array(count));

    arrayStruct[0] = reinterpret_cast<uintptr_t>(buffer);
    arrayStruct[1] = reinterpret_cast<uintptr_t>(buffer);
    float* endPtr = buffer + count;
    subArray[0] = reinterpret_cast<uintptr_t>(endPtr);

    // Fill all elements with the source float value
    float fillValue = *srcValue;
    for (uint32_t i = 0; i < count; i++) {
        buffer[i] = fillValue;
    }

    // Update write position
    arrayStruct[1] = reinterpret_cast<uintptr_t>(buffer + count);
}

/**
 * CopyConstructU32Array @ 0x8222EAB0 | size: 0xA4
 *
 * Copy-constructs a uint32_t array from a source array. Computes the element
 * count from the source's begin/writePos pointers (writePos - begin) / 4,
 * allocates a new buffer, and copies the data via memcpy.
 */
void CopyConstructU32Array(uint32_t* destStruct, const uint32_t* srcStruct) {
    uint32_t* subDest = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(destStruct) + 8);

    uintptr_t srcBegin = srcStruct[0];
    uintptr_t srcWritePos = srcStruct[1];

    destStruct[0] = 0;
    destStruct[1] = 0;
    subDest[0] = 0;

    // Calculate element count from source pointers
    uint32_t byteCount = static_cast<uint32_t>(srcWritePos - srcBegin);
    uint32_t elementCount = byteCount >> 2;

    float* buffer = static_cast<float*>(AllocateU32Array(elementCount));

    destStruct[0] = reinterpret_cast<uintptr_t>(buffer);
    destStruct[1] = reinterpret_cast<uintptr_t>(buffer);
    uintptr_t endAddr = reinterpret_cast<uintptr_t>(buffer) + elementCount * 4;
    subDest[0] = static_cast<uint32_t>(endAddr);

    // Reload source pointers (may have changed)
    uintptr_t srcEnd = srcStruct[1];
    uintptr_t srcStart = srcStruct[0];

    if (srcEnd == srcStart) {
        // Empty source — just set writePos to begin
        destStruct[1] = reinterpret_cast<uintptr_t>(buffer);
        return;
    }

    // Copy source data into new buffer
    size_t copySize = srcEnd - srcStart;
    memcpy(buffer, reinterpret_cast<const void*>(srcStart), copySize);

    // Update write position: dest_begin + (srcEnd - srcStart offset adjusted)
    uintptr_t destBegin = reinterpret_cast<uintptr_t>(buffer);
    destStruct[1] = static_cast<uint32_t>(destBegin - srcStart + srcEnd);
}

/**
 * CompareArrayElements @ 0x82238F28 | size: 0x80
 *
 * Compares two indirect array element references. Each reference is a pointer
 * to a struct containing: +0 = key (int32_t, -1 means string), +4 = value ptr.
 * For string keys (-1), performs byte-by-byte string comparison.
 * For integer keys, returns comparison result (-1, 0, or 1-like value).
 */
int CompareArrayElements(const void* refA, const void* refB) {
    const int32_t* entryA = *reinterpret_cast<const int32_t* const*>(refA);
    const int32_t* entryB = *reinterpret_cast<const int32_t* const*>(refB);

    int32_t keyA = entryA[0];

    if (keyA == -1) {
        // String comparison mode
        int32_t keyB = entryB[0];
        if (keyB != -1) {
            return -1;
        }

        // Both are string entries — compare the string values byte-by-byte
        const char* strA = reinterpret_cast<const char*>(entryA[1]);
        const char* strB = reinterpret_cast<const char*>(entryB[1]);

        while (true) {
            char charA = *strA;
            char charB = *strB;
            int diff = charA - charB;

            if (charA == '\0') {
                return diff;
            }
            if (diff != 0) {
                return diff;
            }

            strA++;
            strB++;
        }
    }

    // Integer comparison mode
    int32_t keyB = entryB[0];
    if (keyB == -1) {
        return 1;
    }

    // Compare unsigned integer keys
    if (static_cast<uint32_t>(keyA) <= static_cast<uint32_t>(keyB)) {
        // subfc/subfe idiom: returns 0 if equal, -1 if less
        return (static_cast<uint32_t>(keyA) < static_cast<uint32_t>(keyB)) ? -1 : 0;
    }

    return 1;
}


} // namespace rage
