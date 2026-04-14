/**
 * rage::parStream — Parameter Stream Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Serialization system for RAGE engine parameter files.
 * Supports XML and RBF (Rockstar Binary Format) streams.
 */

#include "rage/parStream.hpp"
#include "rage/misc/rage_fi.hpp"
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cstdlib>

// Forward declarations for external C functions used by parStreamOutXml
extern "C" void fiAsciiTokenizer_FlushBuffer(void* file, int flags);
extern "C" bool fiAsciiTokenizer_BeginElement(void* stream, const char* name, int isArray);
extern "C" void parStreamOutXml_WriteFormatted(void* file, const char* fmt, int32_t value);
extern "C" void parStreamOutXml_FormatToBuffer(void* outBuf, const char* fmt, ...);  // 2A70 (vsnprintf)
extern "C" void util_WriteBytes(void* file, const void* data, uint32_t size);  // 39B0
extern "C" void util_WriteIndentation(void* self, int depth);  // B870
extern "C" void fiAsciiTokenizer_CloseElement(void* self);  // B708
extern "C" void fiAsciiTokenizer_WriteChar(void* fileHandle, int charCode);

// Forward declarations for external C functions used by parStreamOutRbf
extern "C" void util_WriteBytes(void* file, const void* data, uint32_t size);
extern "C" void util_WriteUint32Array(void* file, const uint32_t* data, uint32_t count);
extern "C" void rage_SeekFile(void* file, uint32_t pos);  // 3AC8
extern "C" void parStreamOutRbf_FlushPending(void* self);  // CE58_gen
extern "C" void parStreamOutRbf_WriteFieldHeader(void* self, uint32_t fieldType, const char* fieldName);  // D128
extern "C" void parStreamOutRbf_WriteData(void* self, const void* data, uint32_t size, uint32_t dataType);  // CED0
extern "C" uint32_t rage_HashString(const char* str);  // DA60
extern "C" void* rage_FindMember(void* structure, const char* name);  // 97A8
extern "C" bool parStreamInXml_ReadElement(void* self, const char* name);  // A8E8
extern "C" bool parStreamInXml_ProcessFormat(void* self, const char* fmt, void* outBuf);  // 9920
extern "C" int parStreamInXml_ReadArrayData(void* self, const char* name, uint32_t* count, uint32_t dataType);  // A3A0
extern "C" void parStreamInXml_SkipUntilChar(void* self, const char* pattern);  // 9B38
extern "C" bool parStreamInXml_ExpectChar(void* self, int ch, int pushBack);  // 9890

// Forward declarations for CRT/kernel functions
extern "C" {
void KeBugCheck(uint32_t code);
void _crt_spinlock_acquire(int index);
void _crt_spinlock_release(int index);
void _doexit(int code, int flags);
int _atSingleton_dispatch(void* ptr);
void _crt_signal_event(void* event);
void RtlEnterCriticalSection(void* cs);
void RtlLeaveCriticalSection(void* cs);
void _fp_init_helper();
void* _crt_get_tls_data();
void* _crt_alloc(size_t size);
void _crt_locale_init(void* locale, void* a1, void* a2, void* a3, int a4);
void _crt_global_init();
void _crt_secondary_init();
void _crt_cleanup(void* ptr);
}

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
        // cmOperator_SetName(name, expectedHash, hasName);
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
                // fiStreamBuf_Read_stub(m_pRbfData, name, elementSize);
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
                // cmOperator_SetName(name, fieldData, hasName);
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
        // fiStreamBuf_Read_stub(m_pRbfData, name, chunkSize);
        
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
            // cmOperator_SetInt(name, "%d", intValue, 0);
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
            // cmOperator_SetFloat(name, "%f", floatValue, 0);
            return true;
        }
        
        case 5: {
            // Vector3 type (3 floats)
            float vec[3] = {0.0f, 0.0f, 0.0f};
            // grc_3CD8(m_pRbfData, vec, 3);
            
            // Format and store each component
            // cmOperator_SetFloat(name, "x", vec[0], 0);
            // cmOperator_SetFloat(name, "y", vec[1], 0);
            // cmOperator_SetFloat(name, "z", vec[2], 0);
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
        dictionaryPtr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this) + 68);  // Default dictionary offset
    }
    
    // Check if field ID matches dictionary entry
    uint16_t dictFieldId = 0;  // Read from dictionary at +4
    if (*outFieldData == dictFieldId) {
        // Allocate string buffer
        uint32_t stringLength = 0;
        // grc_3C68(m_pRbfData, &stringLength, 1);
        
        // Allocate buffer
        char* stringBuffer = nullptr;  // rage_Alloc(stringLength + 1);
        
        // Read string data
        // fiStreamBuf_Read_stub(m_pRbfData, stringBuffer, stringLength);
        
        // Null-terminate
        if (stringBuffer) {
            stringBuffer[stringLength] = '\0';
        }
    }
    
    // Get field data pointer from dictionary
    // uint32_t* fieldDataArray = dictionary[0];
    // *outNameHash = fieldDataArray[*outFieldData];
}



bool parStreamInXml::BeginObject(const char* name) {
    // @ 0x8241ABF8
    bool result = parStreamInXml_ReadElement(this, name);
    *(uint8_t*)((char*)this + 1092) = result ? 1 : 0;
    *(uint8_t*)((char*)this + 1094) = 0;
    *(uint8_t*)((char*)this + 1093) = 1;
    *(uint8_t*)((char*)this + 1095) = 0;
    return result;
}

void parStreamInXml::EndObject() {
    // @ 0x8241AC40
    bool result = parStreamInXml_ReadElement(this, nullptr);
    if (!(result & 0xFF)) {
        char buf[64];
        parStreamInXml_ProcessFormat(this, "Sw", buf);
    }
    *(uint8_t*)((char*)this + 1092) = 0;
    *(uint8_t*)((char*)this + 1093) = 0;
    *(uint8_t*)((char*)this + 1094) = 0;
    *(uint8_t*)((char*)this + 1095) = 0;
    *(uint32_t*)((char*)this + 1096) = 0;
}

bool parStreamInXml::BeginArray(const char* name, uint32_t* count) {
    // @ 0x8241AD38
    uint8_t hasAttr = *(uint8_t*)((char*)this + 1092);
    if (hasAttr != 0) {
        return false;
    }

    uint8_t isInElem = *(uint8_t*)((char*)this + 1093);
    if (isInElem == 0) {
        uint8_t isInArray = *(uint8_t*)((char*)this + 1094);
        if (isInArray == 0) {
            return false;
        }
        uint8_t lastRead = *(uint8_t*)((char*)this + 1095);
        if (lastRead == 0) {
            return false;
        }
    }

    // Use element count from +1096 as the data type hint
    uint32_t dataType = *(uint32_t*)((char*)this + 1096);

    int result = parStreamInXml_ReadArrayData(this, name, count, dataType);

    // Update state flags
    *(uint8_t*)((char*)this + 1092) = 0;  // hasAttribute = false
    *(uint8_t*)((char*)this + 1094) = 1;  // isInArray = true
    *(uint8_t*)((char*)this + 1093) = 0;  // isInElement = false

    return result != 0;
}

void parStreamInXml::EndArray() {
    // @ 0x8241AD08
    char buf[80];
    ReadString(nullptr, buf, sizeof(buf));
}

bool parStreamInXml::ReadInt(const char* name, int32_t* value) {
    // @ 0x8241ADD0 — XML tag nesting skipper / EndObject helper
    // Clear state flags
    *(uint8_t*)((char*)this + 1094) = 0;   // isInArray = false
    *(uint8_t*)((char*)this + 1093) = 0;   // isInElement = false
    *(uint8_t*)((char*)this + 1095) = 0;   // lastRead = false
    *(uint32_t*)((char*)this + 1096) = 0;  // element counter = 0

    uint8_t hasAttr = *(uint8_t*)((char*)this + 1092);
    if (hasAttr != 0) {
        *(uint8_t*)((char*)this + 1092) = 0;
        return true;
    }

    // Skip nested XML tags until nesting level reaches 0
    int32_t nestLevel = 1;
    while (nestLevel > 0) {
        parStreamInXml_SkipUntilChar(this, "<");

        bool isClosing = parStreamInXml_ExpectChar(this, '/', 1);
        if (isClosing) {
            parStreamInXml_SkipUntilChar(this, ">");
            nestLevel--;
        } else {
            // Read chars until '>' found, track last char for self-closing detection
            int lastChar = 0;
            int32_t bufCount = *(int32_t*)((char*)this + 1088);
            uint8_t* ringBuf = (uint8_t*)((char*)this + 64);

            while (true) {
                // Read next character from ring buffer
                if (bufCount <= 0) {
                    break;
                }
                bufCount--;
                int ch = ringBuf[bufCount];
                *(int32_t*)((char*)this + 1088) = bufCount;

                if (ch == '>') {
                    break;
                }
                lastChar = ch;
            }

            if (lastChar == '/') {
                // Self-closing tag <.../> — no nesting change
            } else {
                nestLevel++;
            }
        }
    }

    return true;
}

bool parStreamInXml::ReadFloat(const char* name, float* value) {
    // @ 0x8241AEF8
    return false;  // TODO: implement
}

bool parStreamInXml::ReadString(const char* name, char* buffer, size_t bufferSize) {
    // @ 0x8241ACA8
    uint8_t flag = *(uint8_t*)((char*)this + 1092);
    if (flag == 0) {
        parStreamInXml_ProcessFormat(this, "Sw", buffer);
    }
    *(uint8_t*)((char*)this + 1092) = 0;
    *(uint8_t*)((char*)this + 1094) = 0;
    *(uint8_t*)((char*)this + 1093) = 0;
    *(uint8_t*)((char*)this + 1095) = 0;
    *(uint32_t*)((char*)this + 1096) = 0;
    return true;
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
    return fiAsciiTokenizer_BeginElement(this, name, 0);
}

/**
 * parStreamOutXml::EndObject @ 0x8241BB40
 * 
 * Closes an XML element tag. Decrements indent depth, writes CRLF
 * and indentation (or calls close element for inline arrays),
 * then clears flags.
 */
void parStreamOutXml::EndObject() {
    // @ 0x8241BB40
    uint32_t indent = *(uint32_t*)((char*)this + 8);
    uint8_t flags = *(uint8_t*)((char*)this + 12);
    indent--;
    *(uint32_t*)((char*)this + 8) = indent;

    if (flags != 0) {
        uint32_t arrayType = *(uint32_t*)((char*)this + 28);
        if (arrayType == 3) {
            // Inline array element — close without CRLF
            fiAsciiTokenizer_CloseElement(this);
        }
        // Otherwise skip to write formatted element count
    } else {
        // Write CR LF and indentation
        void* file = *(void**)((char*)this + 4);
        fiAsciiTokenizer_WriteChar(file, '\r');
        fiAsciiTokenizer_WriteChar(file, '\n');
        util_WriteIndentation(this, indent);
    }

    // Clear flags and array type
    *(uint8_t*)((char*)this + 12) = 0;
    *(uint32_t*)((char*)this + 28) = 0;
}

/**
 * parStreamOutXml::BeginArray @ 0x8241BBE8 | size: 0x38
 * Opens an XML array element. Calls tokenizer with isArray=true,
 * then resets m_arrayElementIndex (+28) to 0.
 */
bool parStreamOutXml::BeginArray(const char* name, uint32_t* count) {
    fiAsciiTokenizer_BeginElement(this, name, 1);
    *(uint32_t*)((char*)this + 28) = 0;  // m_arrayElementIndex = 0
    return true;
}

/**
 * parStreamOutXml::EndArray @ 0x8241BC20 | size: ~642 PPC insns
 * 
 * Closes an XML array element. Contains a large switch(0-11)
 * that writes array data in different formats based on element type.
 * TODO: Full implementation of switch cases for all 12 array element types.
 */
void parStreamOutXml::EndArray() {
    // @ 0x8241BC20
    // TODO: implement — massive switch(0-11) over array element types
    // Each case formats array data differently (ints, floats, vectors, etc.)
    // For now, delegate to EndObject behavior as a baseline
    EndObject();
}

/**
 * parStreamOutXml::WriteInt @ 0x8241B810 | size: 0x2C
 * Writes an integer value as XML text. Gets the file handle at +4,
 * loads a "%d" format string, and calls the XML formatter.
 */
bool parStreamOutXml::WriteInt(const char* name, int32_t value) {
    void* file = *(void**)((char*)this + 4);
    parStreamOutXml_WriteFormatted(file, "%d", value);
    return true;
}

/**
 * parStreamOutXml::WriteFloat @ 0x822E3E40
 * 
 * Writes a float value as XML text. Formats via vsnprintf into a
 * stack buffer, then writes bytes to the file handle.
 */
bool parStreamOutXml::WriteFloat(const char* name, float value) {
    // @ 0x822E3E40
    char buf[256];
    void* file = *(void**)((char*)this + 4);
    parStreamOutXml_FormatToBuffer(buf, "%g", (double)value);
    util_WriteBytes(file, buf, strlen(buf));
    return true;
}

/**
 * parStreamOutXml::WriteString @ 0x822E5D48
 * 
 * Writes a string value as XML text. Clears counter at +20,
 * formats the string into a buffer, then writes to file.
 */
bool parStreamOutXml::WriteString(const char* name, const char* value) {
    // @ 0x822E5D48
    *(uint32_t*)((char*)this + 20) = 0;  // clear counter
    char buf[1024];
    void* file = *(void**)((char*)this + 4);
    parStreamOutXml_FormatToBuffer(buf, "%s", value);
    util_WriteBytes(file, buf, strlen(buf));
    return true;
}

/**
 * parStreamOutXml::WriteBool @ 0x822E5DC8
 * 
 * Writes a boolean value as XML text. Writes indentation,
 * formats "true"/"false", writes the value, then writes newline.
 */
bool parStreamOutXml::WriteBool(const char* name, bool value) {
    // @ 0x822E5DC8
    void* file = *(void**)((char*)this + 4);
    uint32_t indent = *(uint32_t*)((char*)this + 8);
    util_WriteIndentation(this, indent);
    char buf[64];
    parStreamOutXml_FormatToBuffer(buf, "%s", value ? "true" : "false");
    util_WriteBytes(file, buf, strlen(buf));
    // Write CRLF newline
    fiAsciiTokenizer_WriteChar(file, '\r');
    fiAsciiTokenizer_WriteChar(file, '\n');
    return true;
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
    // Flush any pending data from previous writes
    parStreamOutRbf_FlushPending(this);
    
    // Clear the field counter at +8
    *(uint32_t*)((char*)this + 8) = 0;
    
    // Write field header with type=0 (object) and the structure name
    parStreamOutRbf_WriteFieldHeader(this, 0, name);
    
    // TODO: Full implementation requires reading parStructure descriptor from 'name' param:
    //   - Extract member count (upper 10 bits of +12) and data size (lower 16 bits of +12)
    //   - Write member count, data size, hash bytes count as byte-swapped uint16
    //   - Iterate member array at +8, for each member:
    //     type 0 (string): D128 type=6, write strlen + string bytes
    //     type 1 (int):    D128 type=1, write uint32 value
    //     type 2 (float):  D128 type=4, write float as uint32
    //     type 3 (bool):   D128 type=2/3, write uint32 value
    
    return true;
}

void parStreamOutRbf::EndObject() {
    // @ 0x8241D520
    // Flush any pending accumulated data
    parStreamOutRbf_FlushPending(this);
    
    // Write end-object marker (0xFFFF) to stream
    void* file = *(void**)((char*)this + 4);
    uint16_t marker = 0xFFFF;
    util_WriteBytes(file, &marker, 2);
    
    // Clear field counter
    *(uint32_t*)((char*)this + 8) = 0;
}

bool parStreamOutRbf::BeginArray(const char* name, uint32_t* count) {
    // @ 0x8241D570
    // Flush any pending data from previous writes
    parStreamOutRbf_FlushPending(this);
    
    // Clear the field counter at +8
    *(uint32_t*)((char*)this + 8) = 0;
    
    // TODO: Full implementation should read member count from structure at name+12 as uint16
    //   If count == 1: single element type optimization
    //     - Get single member via rage_FindMember
    //     - Check member type: 1=int, 2=float, 3=bool
    //     - For vector3 (count==3, all floats): D128 type=5, write 3 floats
    //   If count != 1 and != 3: fallback below
    
    // Fallback: delegate to BeginObject + EndObject
    BeginObject(name);
    EndObject();
    return true;
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
    m_streamPos += offset;
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

// ── ExpectToken + read vector wrappers ──────────────────────────────────────

/**
 * fiAsciiTokenizer::ExpectTokenThenFloat4 @ 0x822E66B0 | size: 0x50
 * [vtable slot 23]
 *
 * Reads and compares a token, then reads 4 floats via vtable slot 8 (ReadFloat4).
 */
void fiAsciiTokenizer::ExpectTokenThenFloat4(const char* expectedToken, float* outArray) {
    ReadTokenAndCompare(expectedToken);

    // Call vtable slot 8 (ReadFloat4)
    void** vt = *reinterpret_cast<void***>(this);
    typedef void (*ReadFloat4Func)(fiAsciiTokenizer*, float*);
    ReadFloat4Func readFloat4 = reinterpret_cast<ReadFloat4Func>(vt[8]);
    readFloat4(this, outArray);
}

/**
 * fiAsciiTokenizer::ExpectTokenThenFloat3 @ 0x822E6660 | size: 0x50
 * [vtable slot 24]
 *
 * Reads and compares a token, then reads 3 floats via vtable slot 9 (ReadFloat3).
 */
void fiAsciiTokenizer::ExpectTokenThenFloat3(const char* expectedToken, float* outArray) {
    ReadTokenAndCompare(expectedToken);

    // Call vtable slot 9 (ReadFloat3)
    void** vt = *reinterpret_cast<void***>(this);
    typedef void (*ReadFloat3Func)(fiAsciiTokenizer*, float*);
    ReadFloat3Func readFloat3 = reinterpret_cast<ReadFloat3Func>(vt[9]);
    readFloat3(this, outArray);
}

/**
 * fiAsciiTokenizer::ExpectTokenThenVec2 @ 0x822E6610 | size: 0x50
 * [vtable slot 25]
 *
 * Reads and compares a token, then reads 2 floats via vtable slot 10 (ReadVec2).
 */
void fiAsciiTokenizer::ExpectTokenThenVec2(const char* expectedToken, float* outArray) {
    ReadTokenAndCompare(expectedToken);

    // Call vtable slot 10 (ReadVec2)
    void** vt = *reinterpret_cast<void***>(this);
    typedef void (*ReadVec2Func)(fiAsciiTokenizer*, float*);
    ReadVec2Func readVec2 = reinterpret_cast<ReadVec2Func>(vt[10]);
    readVec2(this, outArray);
}

// ── Write/output formatting functions ───────────────────────────────────────

// External function: writes a single character to the output stream
extern "C" void fiAsciiTokenizer_WriteChar(void* fileHandle, int charCode);

/**
 * fiAsciiTokenizer::WriteBeginBlock @ 0x822E67A0 | size: 0x84
 * [vtable slot 26]
 *
 * Writes opening block: emits m_streamPos tab characters for indentation,
 * then '{', CR, LF. Increments m_streamPos (indent level).
 */
void fiAsciiTokenizer::WriteBeginBlock() {
    int32_t indent = m_streamPos;
    void* file = reinterpret_cast<void*>(m_fileHandle);
    while (indent != 0) {
        fiAsciiTokenizer_WriteChar(file, '\t');
        indent--;
    }
    fiAsciiTokenizer_WriteChar(file, '{');
    fiAsciiTokenizer_WriteChar(file, '\r');
    fiAsciiTokenizer_WriteChar(file, '\n');
    m_streamPos++;
}

/**
 * fiAsciiTokenizer::WriteEndBlock @ 0x822E6828 | size: 0x80
 * [vtable slot 27]
 *
 * Decrements m_streamPos (indent level), writes indent tabs,
 * then '}', CR, LF.
 */
void fiAsciiTokenizer::WriteEndBlock() {
    m_streamPos--;
    int32_t indent = m_streamPos;
    void* file = reinterpret_cast<void*>(m_fileHandle);
    while (indent != 0) {
        fiAsciiTokenizer_WriteChar(file, '\t');
        indent--;
    }
    fiAsciiTokenizer_WriteChar(file, '}');
    fiAsciiTokenizer_WriteChar(file, '\r');
    fiAsciiTokenizer_WriteChar(file, '\n');
}

/**
 * fiAsciiTokenizer::WriteIndent @ 0x822E68A8 | size: 0x80
 * [vtable slot 28]
 *
 * If state (m_writeState) != 1, writes m_streamPos tab characters
 * for indentation and sets state to 1.
 */
void fiAsciiTokenizer::WriteIndent() {
    if (m_writeState == 1) {
        m_writeState = 1;
        return;
    }

    int32_t indent = m_streamPos;
    void* file = reinterpret_cast<void*>(m_fileHandle);
    if (indent != 0) {
        while (indent != 0) {
            fiAsciiTokenizer_WriteChar(file, '\t');
            indent--;
        }
        m_writeState = 1;
        return;
    }

    m_writeState = 1;
}

/**
 * fiAsciiTokenizer::WriteNewline @ 0x822E6928 | size: 0x70
 * [vtable slot 29]
 *
 * If state (m_writeState) != 2, writes CR and LF characters,
 * then sets state to 2.
 */
void fiAsciiTokenizer::WriteNewline() {
    if (m_writeState == 2) {
        m_writeState = 2;
        return;
    }

    void* file = reinterpret_cast<void*>(m_fileHandle);
    fiAsciiTokenizer_WriteChar(file, '\r');
    fiAsciiTokenizer_WriteChar(file, '\n');
    m_writeState = 2;
}

// ── Memory allocation helpers ───────────────────────────────────────────────

// External functions used by allocation helpers
extern "C" void fiAsciiTokenizer_ReportAllocError(const char* errorMsg);
extern "C" void fiAsciiTokenizer_ExitWithCode(int exitCode);
extern "C" void* rage_Alloc(uint32_t size);

/**
 * AllocateUint32Array @ 0x820E76D8 | size: 0x64
 *
 * Allocates an array of uint32_t elements. Validates count against
 * max 0x3FFFFFFF. Returns nullptr if count is 0. Each element is 4 bytes.
 */
extern "C" uint32_t* fiAsciiTokenizer_AllocateUint32Array(void* unused, uint32_t count) {
    if (count > 0x3FFFFFFFu) {
        fiAsciiTokenizer_ReportAllocError("allocation overflow");
        fiAsciiTokenizer_ExitWithCode(1);
    }

    if (count != 0) {
        return static_cast<uint32_t*>(rage_Alloc(count * 4));
    }

    return nullptr;
}


/**
 * AllocateBytes @ 0x8222D588 | size: 0x60
 *
 * Allocates a byte array. Validates count against max 0xFFFFFFFF.
 * Returns nullptr if count is 0. Each element is 1 byte.
 */
extern "C" uint8_t* fiAsciiTokenizer_AllocateByteArray(void* unused, uint32_t count) {
    if (count > 0xFFFFFFFFu) {
        fiAsciiTokenizer_ReportAllocError("allocation overflow");
        fiAsciiTokenizer_ExitWithCode(1);
    }

    if (count != 0) {
        return static_cast<uint8_t*>(rage_Alloc(count));
    }

    return nullptr;
}


/**
 * AllocateUint16Array @ 0x822DF168 | size: 0x64
 *
 * Allocates an array of uint16_t elements. Validates count against
 * max 0x7FFFFFFF. Returns nullptr if count is 0. Each element is 2 bytes.
 */
extern "C" uint16_t* fiAsciiTokenizer_AllocateUint16Array(void* unused, uint32_t count) {
    if (count > 0x7FFFFFFFu) {
        fiAsciiTokenizer_ReportAllocError("allocation overflow");
        fiAsciiTokenizer_ExitWithCode(1);
    }

    if (count != 0) {
        return static_cast<uint16_t*>(rage_Alloc(count * 2));
    }

    return nullptr;
}


} // namespace rage

// Forward declarations for CRT helpers used below
extern "C" {
void fiAsciiTokenizer_InvokeWithSixArgs(void* a, void* b, void* c, void* d, void* e, void* f);
void fiAsciiTokenizer_InvokeWithTwoArgs(void* a, void* b);
void fiAsciiTokenizer_InvokeWithTwoArgsAlt(void* a, void* b);
void RtlInitAnsiString(void* ansiStr, const char* str);
void _nt_process_ansi_string(void* ansiStr);
void fiAsciiTokenizer_ConstructGlobal(void* obj, const char* name, void* arg);
void fiAsciiTokenizer_ConstructGlobalAlt(void* obj, const char* name, void* arg);
void rage_free(void* ptr);
void parStructure_Install(void* obj);
void rage_RegisterParSubsystemHandler(void* obj);
void atSingleton_dtor(void* obj, int mode);
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
    ctx[3] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(excContext));
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
 * _thunk_InvokeWithSixArgs_DefaultNull @ 0x8243A558 | size: 0x8
 *
 * Sets r6=0 and tail-calls fiAsciiTokenizer_InvokeWithSixArgs.
 */
void _thunk_InvokeWithSixArgs_DefaultNull(void* a, void* b, void* c, void* d, void* e) {
    fiAsciiTokenizer_InvokeWithSixArgs(a, b, c, d, e, nullptr);
}

/**
 * _thunk_InvokeWithTwoArgs_DefaultNull @ 0x8243A560 | size: 0x8
 *
 * Sets r4=0 and tail-calls fiAsciiTokenizer_InvokeWithTwoArgs.
 */
void _thunk_InvokeWithTwoArgs_DefaultNull(void* a) {
    fiAsciiTokenizer_InvokeWithTwoArgs(a, nullptr);
}

/**
 * _thunk_InvokeWithTwoArgsAlt_DefaultNull @ 0x8243A568 | size: 0x8
 *
 * Sets r4=0 and tail-calls fiAsciiTokenizer_InvokeWithTwoArgsAlt.
 */
void _thunk_InvokeWithTwoArgsAlt_DefaultNull(void* a) {
    fiAsciiTokenizer_InvokeWithTwoArgsAlt(a, nullptr);
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
    struct ANSI_STRING { uint16_t len; uint16_t maxLen; char* buf; };
    ANSI_STRING ansiStr = {0, 0, nullptr};
    RtlInitAnsiString(&ansiStr, str);
    _nt_process_ansi_string(&ansiStr);
}

// ── Static initializer registration (atexit pattern) ───────────────────────

/**
 * _static_init_reg_0 @ 0x8257EE28 | size: 0x40
 *
 * Static initializer: calls fiAsciiTokenizer_ConstructGlobal to construct a global
 * object, then registers its destructor via atexit().
 */
void _static_init_reg_0() {
    extern void* g_staticObj0;
    extern const char g_staticName0[];
    fiAsciiTokenizer_ConstructGlobal(&g_staticObj0, g_staticName0, nullptr);
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
    fiAsciiTokenizer_ConstructGlobal(&g_staticObj1, g_staticName1, nullptr);
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
    fiAsciiTokenizer_ConstructGlobal(&g_staticObj2, g_staticName2, nullptr);
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
    fiAsciiTokenizer_ConstructGlobal(&g_staticObj3, g_staticName3, nullptr);
    extern void (*g_staticDtor3)();
    atexit(g_staticDtor3);
}

/**
 * _static_init_reg_4 @ 0x8257EF48 | size: 0x40
 *
 * Static initializer: constructs global object via fiAsciiTokenizer_ConstructGlobalAlt
 * and registers destructor via atexit().
 */
void _static_init_reg_4() {
    extern void* g_staticObj4;
    extern const char g_staticName4[];
    fiAsciiTokenizer_ConstructGlobalAlt(&g_staticObj4, g_staticName4, nullptr);
    extern void (*g_staticDtor4)();
    atexit(g_staticDtor4);
}

/**
 * _static_init_reg_5 @ 0x8257EF88 | size: 0x40
 *
 * Static initializer: constructs global object via fiAsciiTokenizer_ConstructGlobalAlt
 * and registers destructor via atexit().
 */
void _static_init_reg_5() {
    extern void* g_staticObj5;
    extern const char g_staticName5[];
    fiAsciiTokenizer_ConstructGlobalAlt(&g_staticObj5, g_staticName5, nullptr);
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
    fiAsciiTokenizer_ConstructGlobal(&g_staticObj6, g_staticName6, nullptr);
    extern void (*g_staticDtor6)();
    atexit(g_staticDtor6);
}

// ── Static destructor registrations (at-exit cleanup) ──────────────────────

/**
 * _static_dtor_free_0 @ 0x82583F28 | size: 0x40
 *
 * Static destructor: checks if buffer is SSO (internal), if not frees
 * the heap allocation via sysMemAllocator_Free.
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
 * Loads static address and tail-calls parStructure_Install handler registration.
 */
void _static_init_set_handler_0() {
    extern char g_handlerObj0[];
    parStructure_Install(g_handlerObj0);
}

/**
 * _static_init_set_handler_1 @ 0x82583F18 | size: 0xC
 *
 * Loads static address and tail-calls rage_RegisterParSubsystemHandler handler registration.
 */
void _static_init_set_handler_1() {
    extern char g_handlerObj1[];
    rage_RegisterParSubsystemHandler(g_handlerObj1);
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

// ────────────────────────────────────────────────────────────────────────────
// D128 algorithm — write RBF field header (parStreamOutRbf_WriteFieldHeader)
// Binary params: (void* self, uint32_t fieldType, const char* fieldName)
// C++ vtable slot WriteBool has incompatible signature; this static helper
// implements the actual algorithm callable via the extern "C" wrapper.
// ────────────────────────────────────────────────────────────────────────────
static bool parStreamOutRbf_WriteFieldHeaderImpl(rage::parStreamOutRbf* self, uint32_t fieldType, const char* fieldName) {
    char* obj = reinterpret_cast<char*>(self);
    void* file = *(void**)(obj + 4);

    // Hash table struct at self+24:
    //   +0: bucket array pointer (void**)
    //   +4: bucket count (uint16_t)
    //   +6: next field ID counter (uint16_t)
    void** buckets = *(void***)(obj + 24);
    uint16_t bucketCount = *(uint16_t*)(obj + 28);
    uint16_t* pNextID = (uint16_t*)(obj + 30);

    // Hash field name and search bucket chain
    uint32_t hash = rage_HashString(fieldName);
    uint32_t bucketIdx = (bucketCount > 0) ? (hash % bucketCount) : 0;

    uint16_t fieldID = 0;
    bool isNewName = true;

    // TODO: Traverse linked list at buckets[bucketIdx]
    // Each entry: { const char* name, uint16_t fieldID, void* next }
    // Compare fieldName against each entry->name to find existing fieldID.
    // For now, always assigns a new ID:
    fieldID = (*pNextID)++;

    // Write compact or extended field header
    if (fieldID < 8192) {
        // Compact: type in bits 15-12, fieldID in bits 11-0
        uint16_t compact = (uint16_t)((fieldType << 12) | fieldID);
        // Byte-swap for big-endian RBF format
        uint16_t swapped = (uint16_t)((compact >> 8) | (compact << 8));
        util_WriteBytes(file, &swapped, 2);
    } else {
        // Extended: 0xFEFF marker + 32-bit header
        uint16_t marker = 0xFEFF;
        util_WriteBytes(file, &marker, 2);
        uint32_t extended = (fieldType << 28) | fieldID;
        util_WriteUint32Array(file, &extended, 1);
    }

    // If new name, write name length (byte-swapped uint16) + name bytes
    if (isNewName && fieldName) {
        uint32_t nameLen = (uint32_t)strlen(fieldName);
        uint16_t len16 = (uint16_t)nameLen;
        uint16_t lenSwapped = (uint16_t)((len16 >> 8) | (len16 << 8));
        util_WriteBytes(file, &lenSwapped, 2);
        util_WriteBytes(file, fieldName, nameLen);
    }

    return true;
}

// ────────────────────────────────────────────────────────────────────────────
// CED0 algorithm — type-dispatched data writer (parStreamOutRbf_WriteData)
// Binary params: (void* self, const void* data, uint32_t size, uint32_t dataType)
// C++ vtable slot WriteString has incompatible signature; this static helper
// implements the actual algorithm callable via the extern "C" wrapper.
// ────────────────────────────────────────────────────────────────────────────
static bool parStreamOutRbf_WriteDataImpl(rage::parStreamOutRbf* self, const void* data, uint32_t size, uint32_t dataType) {
    if (!data) return false;
    if (dataType > 11) return false;

    void* file = *(void**)((char*)self + 4);

    switch (dataType) {
        case 0: case 1: case 3: case 4: case 6:
            util_WriteBytes(file, data, size);
            break;
        case 2: case 7:
            // TODO: fiBinTokenizer_3D50_w(file, data, size/2) — uint16 byte-swap writer
            util_WriteBytes(file, data, size);  // fallback: raw write
            break;
        case 5: case 8: case 9: case 10: case 11:
            util_WriteUint32Array(file, (const uint32_t*)data, size / 4);
            break;
        default:
            break;
    }

    return true;
}

namespace rage {

void parStreamOutRbf::EndArray() {
    // @ 0x8241CF90
    // TODO: Complex internal function (~300+ lines) that handles flushing
    // pending array element data. The vtable slot maps to an internal
    // data-flushing function with type-based dispatch via CED0_wrh.
    // Scaffold involves: pending flag check, 0xFDFF marker write,
    // sentinel writes, and accumulated size tracking at +20.
}

bool parStreamOutRbf::WriteInt(const char* name, int32_t value) {
    // @ 0x8241CDD8 — writes string block marker to RBF stream
    (void)value;  // unused in RBF implementation

    // Flush any pending accumulated data
    parStreamOutRbf_FlushPending(this);

    // Compute string length of name
    uint32_t len = 0;
    if (name) {
        const char* p = name;
        while (*p) p++;
        len = (uint32_t)(p - name);
    }

    // Write string block marker (0xFCFF) + length + name bytes
    void* file = *(void**)((char*)this + 4);
    uint16_t marker = 0xFCFF;
    util_WriteBytes(file, &marker, 2);
    util_WriteUint32Array(file, &len, 1);
    if (name && len > 0) {
        util_WriteBytes(file, name, len);
    }
    return true;
}

bool parStreamOutRbf::WriteFloat(const char* name, float value) {
    // @ 0x8241CE58 — CE58_gen: flush pending accumulated data size
    (void)name; (void)value;  // params unused in RBF flush implementation

    uint8_t* pending = (uint8_t*)((char*)this + 16);
    if (*pending != 0) {
        void* file = *(void**)((char*)this + 4);
        uint32_t savedPos = *(uint32_t*)((char*)this + 12);

        // Compute current file position: file->pos + file->base
        // (fields at file+16 and file+12 in the file handle struct)
        uint32_t currentPos = *(uint32_t*)((char*)file + 16) + *(uint32_t*)((char*)file + 12);

        // Seek to saved position, write accumulated data size, seek back
        rage_SeekFile(file, savedPos);
        uint32_t* accumSize = (uint32_t*)((char*)this + 20);
        util_WriteUint32Array(file, accumSize, 1);
        rage_SeekFile(file, currentPos);

        *pending = 0;
    }
    return true;
}

bool parStreamOutRbf::WriteString(const char* name, const char* value) {
    // @ 0x8241CED0 — CED0_wrh: type-dispatched data writer
    // Binary params: (data, size, dataType). C++ vtable sig is a mismatch.
    // Called internally via parStreamOutRbf_WriteData(self, data, size, dataType).
    // See parStreamOutRbf_WriteDataImpl above for the full algorithm.
    (void)name; (void)value;
    return true;  // vtable stub — real calls go through extern "C" wrapper
}

bool parStreamOutRbf::WriteBool(const char* name, bool value) {
    // @ 0x8241D128 — D128: write RBF field header
    // Binary params: (fieldType, fieldName). C++ vtable sig is a mismatch.
    // Called internally via parStreamOutRbf_WriteFieldHeader(self, type, name).
    // See parStreamOutRbf_WriteFieldHeaderImpl above for the full algorithm.
    (void)name; (void)value;
    return true;  // vtable stub — real calls go through extern "C" wrapper
}

} // namespace rage
