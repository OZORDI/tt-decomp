/**
 * rage::parStream — Parameter Stream Serialization System
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * The parStream family provides serialization/deserialization for RAGE engine
 * parameter files. Supports both XML and RBF (Rockstar Binary Format) streams.
 * 
 * Class Hierarchy:
 *   parStream (base)
 *     ├─ parStreamInXml  (XML input)
 *     ├─ parStreamOutXml (XML output)
 *     ├─ parStreamInRbf  (RBF input)
 *     └─ parStreamOutRbf (RBF output)
 */

#pragma once

#include <cstdint>
#include <cstddef>

namespace rage {

// Forward declarations
class parFileIO;
class parStructure;
class parMember;

/**
 * parStream @ 0x82078E34 | Base class for parameter streams
 * 
 * Abstract base class for serialization streams. Provides virtual interface
 * for reading/writing structured parameter data.
 */
class parStream {
public:
    virtual ~parStream();  // @ 0x824193E0
    
    // Virtual interface (implemented by derived classes)
    virtual bool BeginObject(const char* name) = 0;
    virtual void EndObject() = 0;
    virtual bool BeginArray(const char* name, uint32_t* count) = 0;
    virtual void EndArray() = 0;
    
    virtual bool ReadInt(const char* name, int32_t* value) = 0;
    virtual bool ReadFloat(const char* name, float* value) = 0;
    virtual bool ReadString(const char* name, char* buffer, size_t bufferSize) = 0;
    virtual bool ReadBool(const char* name, bool* value) = 0;
    
protected:
    parStream() = default;
};

/**
 * parStreamInXml @ 0x82078E40 | XML input stream
 * 
 * Reads structured parameter data from XML format.
 * Used for loading game configuration, level data, and asset metadata.
 */
class parStreamInXml : public parStream {
public:
    parStreamInXml();
    virtual ~parStreamInXml();  // @ 0x82419570 (scalar destructor)
    
    // Stream operations
    virtual bool BeginObject(const char* name) override;     // @ 0x8241ABF8
    virtual void EndObject() override;                        // @ 0x8241AC40
    virtual bool BeginArray(const char* name, uint32_t* count) override;  // @ 0x8241AD38
    virtual void EndArray() override;                         // @ 0x8241AD08
    
    // Read operations
    virtual bool ReadInt(const char* name, int32_t* value) override;      // @ 0x8241ADD0
    virtual bool ReadFloat(const char* name, float* value) override;      // @ 0x8241AEF8
    virtual bool ReadString(const char* name, char* buffer, size_t bufferSize) override;  // @ 0x8241ACA8
    virtual bool ReadBool(const char* name, bool* value) override;        // @ 0x82419568
    
    // XML-specific operations
    bool OpenFile(const char* filename);                      // @ 0x82419428
    void CloseFile();                                         // @ 0x824194E8
    bool IsValid() const;                                     // @ 0x824197B8
    
private:
    struct XmlNode* m_pCurrentNode;      // +0x04 Current XML node
    struct XmlNode* m_pRootNode;         // +0x08 Root XML node
    char* m_pXmlData;                    // +0x0C XML file data buffer
    uint32_t m_xmlDataSize;              // +0x10 Size of XML data
    uint32_t m_currentDepth;             // +0x14 Current nesting depth
    bool m_isValid;                      // +0x18 Stream validity flag
    
    // Internal helpers
    struct XmlNode* FindChildNode(const char* name);
    bool ParseXmlValue(const char* text, void* outValue, int type);
};

/**
 * parStreamOutXml @ 0x82078E70 | XML output stream
 * 
 * Writes structured parameter data to XML format.
 * Used for saving game configuration and exporting data.
 */
class parStreamOutXml : public parStream {
public:
    parStreamOutXml();
    virtual ~parStreamOutXml();  // @ 0x8241B810 (scalar destructor)
    
    // Stream operations
    virtual bool BeginObject(const char* name) override;     // @ 0x8241BB38
    virtual void EndObject() override;                        // @ 0x8241BB40
    virtual bool BeginArray(const char* name, uint32_t* count) override;  // @ 0x8241BBE8
    virtual void EndArray() override;                         // @ 0x8241BC20
    
    // Write operations
    virtual bool WriteInt(const char* name, int32_t value);   // @ 0x8241B840
    virtual bool WriteFloat(const char* name, float value);   // @ 0x822E3E40
    virtual bool WriteString(const char* name, const char* value);  // @ 0x822E5D48
    virtual bool WriteBool(const char* name, bool value);     // @ 0x822E5DC8
    
    // XML-specific operations
    bool CreateFile(const char* filename);                    // @ 0x82419488
    bool FlushToFile();                                       // @ 0x82432A70
    void SetIndentation(int spaces);
    
private:
    char* m_pXmlBuffer;                  // +0x04 XML output buffer
    uint32_t m_bufferSize;               // +0x08 Buffer size
    uint32_t m_bufferUsed;               // +0x0C Bytes written
    uint32_t m_currentDepth;             // +0x10 Current nesting depth
    int m_indentSpaces;                  // +0x14 Indentation per level
    bool m_isValid;                      // +0x18 Stream validity flag
    
    // Internal helpers
    void WriteIndentation();
    void AppendToBuffer(const char* text);
    void AppendFormatted(const char* format, ...);
};

/**
 * parStreamInRbf @ 0x82078ED4 | RBF (Rockstar Binary Format) input stream
 * 
 * Reads structured parameter data from binary RBF format.
 * RBF is a compact binary representation used for optimized runtime loading.
 */
class parStreamInRbf : public parStream {
public:
    parStreamInRbf();
    virtual ~parStreamInRbf();  // @ 0x8241C160
    
    // Stream operations
    virtual bool BeginObject(const char* name) override;     // @ 0x8241C7E8
    virtual void EndObject() override;                        // @ 0x8241C8C8
    virtual bool BeginArray(const char* name, uint32_t* count) override;  // @ 0x8241CA10
    virtual void EndArray() override;                         // @ 0x8241C868
    
    // Read operations
    virtual bool ReadInt(const char* name, int32_t* value) override;      // @ 0x8241C930
    virtual bool ReadFloat(const char* name, float* value) override;      // @ 0x8241CB38
    virtual bool ReadString(const char* name, char* buffer, size_t bufferSize) override;  // @ 0x8241C140
    virtual bool ReadBool(const char* name, bool* value) override;        // @ 0x8241C1D8
    
    // RBF-specific operations
    bool OpenFile(const char* filename);                      // @ 0x8241C260
    void CloseFile();                                         // @ 0x8241C628
    bool IsValid() const;
    
private:
    uint8_t* m_pRbfData;                 // +0x04 RBF file data
    uint32_t m_rbfDataSize;              // +0x08 Size of RBF data
    uint32_t m_currentOffset;            // +0x0C Current read position
    uint32_t m_currentDepth;             // +0x10 Current nesting depth
    bool m_isValid;                      // +0x14 Stream validity flag
    
    // Internal helpers
    bool ReadRbfHeader();
    bool SeekToField(const char* name);
    template<typename T> bool ReadValue(T* outValue);
};

/**
 * parStreamOutRbf @ 0x82078F00 | RBF output stream
 * 
 * Writes structured parameter data to binary RBF format.
 * Generates optimized binary files for runtime loading.
 */
class parStreamOutRbf : public parStream {
public:
    parStreamOutRbf();
    virtual ~parStreamOutRbf();  // @ 0x8241CD50 (scalar destructor @ 0x8241CDC0)
    
    // Stream operations
    virtual bool BeginObject(const char* name) override;     // @ 0x8241D2C8
    virtual void EndObject() override;                        // @ 0x8241D520
    virtual bool BeginArray(const char* name, uint32_t* count) override;  // @ 0x8241D570
    virtual void EndArray() override;                         // @ 0x8241CF90
    
    // Write operations
    virtual bool WriteInt(const char* name, int32_t value);   // @ 0x8241CDD8
    virtual bool WriteFloat(const char* name, float value);   // @ 0x8241CE58
    virtual bool WriteString(const char* name, const char* value);  // @ 0x8241CED0
    virtual bool WriteBool(const char* name, bool value);     // @ 0x8241D128
    
    // RBF-specific operations
    bool CreateFile(const char* filename);
    bool FlushToFile();
    
private:
    uint8_t* m_pRbfBuffer;               // +0x04 RBF output buffer
    uint32_t m_bufferSize;               // +0x08 Buffer size
    uint32_t m_bufferUsed;               // +0x0C Bytes written
    uint32_t m_currentDepth;             // +0x10 Current nesting depth
    bool m_isValid;                      // +0x14 Stream validity flag
    
    // Internal helpers
    void WriteRbfHeader();
    void WriteFieldHeader(const char* name, uint8_t type);
    template<typename T> void WriteValue(const T& value);
};

// RBF format constants
namespace RbfFormat {
    constexpr uint32_t MAGIC = 0x52424600;  // "RBF\0"
    constexpr uint32_t VERSION = 1;
    
    enum FieldType : uint8_t {
        TYPE_INT32 = 0,
        TYPE_FLOAT = 1,
        TYPE_STRING = 2,
        TYPE_BOOL = 3,
        TYPE_OBJECT_BEGIN = 4,
        TYPE_OBJECT_END = 5,
        TYPE_ARRAY_BEGIN = 6,
        TYPE_ARRAY_END = 7
    };
}

} // namespace rage
