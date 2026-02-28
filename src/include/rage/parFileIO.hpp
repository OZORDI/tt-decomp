/**
 * rage::parFileIO — File I/O for Parameter Streams
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Provides file I/O operations for parameter stream serialization.
 * Base class for file-based parameter stream implementations.
 */

#pragma once

#include <cstdint>

namespace rage {

// Forward declarations
class datBase;

/**
 * datBase — Base class for data objects
 * 
 * Minimal base class providing reference counting and memory management.
 */
class datBase {
public:
    virtual ~datBase();  // @ 0x82177630
    
protected:
    void* m_pData;  // +0x04 Owned data pointer (freed in destructor)
};

/**
 * parFileIO @ 0x82037450 | File I/O for parameter streams
 * 
 * Handles file operations for parameter stream serialization.
 * Inherits from datBase for memory management.
 */
class parFileIO : public datBase {
public:
    virtual ~parFileIO();  // @ 0x821775E0 (scalar destructor)
    
protected:
    parFileIO() = default;
};

} // namespace rage
