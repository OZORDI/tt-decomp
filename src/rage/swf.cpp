/**
 * rage::swf* — Scaleform Flash UI System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implementation of the SWF/Scaleform Flash UI system classes.
 * These handle Flash content loading, ActionScript execution, and
 * display list management for the game's menu and HUD.
 */

#include "rage/swf.hpp"
#include "rage/memory.h"
#include <new>      // For placement new
#include <cstring>  // For strcmp
#include <cstdlib>  // For strtol

// External functions
extern "C" {
    void rage_free(void* ptr);                  // @ 0x820C00C0 — canonical heap free (see src/crt/heap.c)
    void swfContext_Cleanup(void* ctx);         // @ 0x823F9B80 — cleanup stub (no-op in final binary)
    uint8_t atSingleton_Find(void* ptr);        // @ 0x820F90D0 — singleton registry lookup, returns nonzero if found
    void atHashMap_Clear(void* ptr);            // @ 0x8234C0E0 — iterates linked-list hash buckets, frees all nodes
    void swfFile_DestroyResources(void* ptr);   // @ 0x824035B0 — releases child resource array entries
    void swfInstance_Cleanup(void* ptr);        // @ 0x823FB670 — unlinks instance from display list
    void swfSymtab_Enumerate(void* symtab);     // @ 0x823FED10 — enumerate symtab members
    const char* swfInternString(const char* name, void* buffer, int maxLen);  // string interning
    void* swfSymtab_Lookup(void* symtab, const char* key);   // symtab key lookup
    void rage_DebugLog(const char* msg, ...);    // debug log stub (no-op)
    void swfSymtab_Insert(void* symtab, const char* key, void* value);  // symtab insert
    void swfSymtab_Delete(void* symtab, const char* key);   // symtab key delete
    void swfPopResult(void* dst);               // pop result from call stack
    int _stricmp(const char*, const char*);     // case-insensitive string compare
    float swfComputeFramePosition(void* ctx, void* entry, float* outMin, float* outMax);
}

// Global SWF object pools (SDA - Small Data Area)
extern void* g_swfScriptObjectPool;  // @ SDA offset 19540
extern void* g_swfInstancePool;      // @ SDA offset 19536
extern void* g_swfScriptArrayPool;   // @ SDA offset 19552

namespace rage {

namespace {
constexpr int SWF_INTERN_BUFFER_SIZE = 1024;
constexpr uint32_t SWF_ACTION_NODE_VISITED_BIT = 1;
constexpr ptrdiff_t SWF_DEFINITION_OFFSET = 0x04;
constexpr ptrdiff_t SWF_FILE_TOTAL_FRAMES_OFFSET = 0x08;
constexpr ptrdiff_t SWF_FILE_FRAME_TABLE_OFFSET = 0x0C;
constexpr ptrdiff_t SWF_FILE_RESOURCE_ARRAY_OFFSET = 0x14;
constexpr ptrdiff_t SWF_FILE_RESOURCE_COUNT_OFFSET = 0x2E;
constexpr ptrdiff_t SWF_INSTANCE_CURRENT_FRAME_OFFSET = 0xA4;
constexpr ptrdiff_t SWF_INSTANCE_IS_PLAYING_OFFSET = 0xAA;
constexpr ptrdiff_t SWF_INSTANCE_WRAP_RESET_FLAG_OFFSET = 0xAF;
constexpr ptrdiff_t SWF_INSTANCE_CHILD_LIST_OFFSET = 0xB4;
constexpr ptrdiff_t SWF_INSTANCE_NEXT_SIBLING_OFFSET = 0xB8;

struct swfValuePair {
    uint32_t data;
    uint32_t type;
};

inline void* SwfGetDefinition(const void* object) {
    return *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(object) + SWF_DEFINITION_OFFSET);
}

inline uint16_t SwfGetTotalFrames(const void* fileDef) {
    return *reinterpret_cast<const uint16_t*>(reinterpret_cast<const char*>(fileDef) + SWF_FILE_TOTAL_FRAMES_OFFSET);
}

inline void* SwfGetFrameTable(const void* fileDef) {
    return *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(fileDef) + SWF_FILE_FRAME_TABLE_OFFSET);
}

inline void** SwfGetResourceArray(const void* fileDef) {
    return *reinterpret_cast<void***>(const_cast<char*>(reinterpret_cast<const char*>(fileDef)) + SWF_FILE_RESOURCE_ARRAY_OFFSET);
}

inline uint16_t SwfGetResourceCount(const void* fileDef) {
    return *reinterpret_cast<const uint16_t*>(reinterpret_cast<const char*>(fileDef) + SWF_FILE_RESOURCE_COUNT_OFFSET);
}

inline uint16_t& SwfCurrentFrameRef(swfINSTANCE* instance) {
    return *reinterpret_cast<uint16_t*>(reinterpret_cast<char*>(instance) + SWF_INSTANCE_CURRENT_FRAME_OFFSET);
}

inline uint8_t& SwfIsPlayingRef(swfINSTANCE* instance) {
    return *reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(instance) + SWF_INSTANCE_IS_PLAYING_OFFSET);
}

inline uint8_t& SwfWrapResetFlagRef(swfINSTANCE* instance) {
    return *reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(instance) + SWF_INSTANCE_WRAP_RESET_FLAG_OFFSET);
}

inline swfINSTANCE* SwfChildList(const swfINSTANCE* instance) {
    return *reinterpret_cast<swfINSTANCE* const*>(reinterpret_cast<const char*>(instance) + SWF_INSTANCE_CHILD_LIST_OFFSET);
}

inline swfINSTANCE* SwfNextSibling(const swfINSTANCE* instance) {
    return *reinterpret_cast<swfINSTANCE* const*>(reinterpret_cast<const char*>(instance) + SWF_INSTANCE_NEXT_SIBLING_OFFSET);
}
}  // namespace

// ===========================================================================
// swfBASE — Base class for all SWF objects
// ===========================================================================

/**
 * swfBASE::~swfBASE() @ 0x823F8A98 | size: 0x48
 * 
 * Standard RAGE destructor pattern. Sets vtable pointer then conditionally
 * frees the object if the flags parameter has bit 0 set.
 * 
 * @param flags  Bit 0 set → free this pointer after destructor runs
 */
swfBASE::~swfBASE() {
    // Vtable pointer is set by derived class destructors
}

void swfBASE_ScalarDestructor(swfBASE* obj, int flags) {
    // Set vtable to swfBASE vtable (0x82074CCC → points to 0x82074D0C + offset)
    // The lis + addi pattern: lis r11,-32249; addi r11,r11,19660
    // Calculation: (-32249 << 16) + 19660 = 0x82074D0C
    
    if (flags & 0x1) {
        rage_free(obj);
    }
}


// ===========================================================================
// swfSCRIPTOBJECT — ActionScript object representation
// ===========================================================================

/**
 * swfSCRIPTOBJECT::swfSCRIPTOBJECT() @ 0x823F8AE0 | size: 0x64
 * 
 * Constructor. Initializes the script object and links it into the global
 * script object pool. The pool is a doubly-linked list maintained in SDA.
 */
swfSCRIPTOBJECT::swfSCRIPTOBJECT() {
    // Set vtable pointer (lis r11,-32249; addi r11,r11,19732)
    // Calculation: (-32249 << 16) + 19732 = 0x82074D14
    
    // Initialize linked list pointers
    m_pNext = nullptr;  // +0x04
    m_pPrev = nullptr;  // +0x08
    
    // Link into global script object pool
    void* pool = g_swfScriptObjectPool;  // SDA offset 19540
    if (pool) {
        void* currentTail = *((void**)((char*)pool + 12));  // pool->tail
        
        // Walk to end of list
        while (currentTail) {
            void* next = *((void**)((char*)currentTail + 12));  // currentTail->next
            if (!next) break;
            currentTail = next;
        }
        
        // Link this object at the end
        if (currentTail) {
            *((void**)((char*)currentTail + 0)) = this;  // currentTail->next = this
        }
        *((void**)((char*)pool + 12)) = this;  // pool->tail = this
        
        // Increment pool count
        uint16_t count = *((uint16_t*)((char*)pool + 6));
        *((uint16_t*)((char*)pool + 6)) = count + 1;
    }
    
    // Update vtable to final swfSCRIPTOBJECT vtable
    // lis r11,-32249; addi r11,r11,19660 = 0x82074D0C
}

/**
 * swfSCRIPTOBJECT::~swfSCRIPTOBJECT() @ 0x823F8B70 | size: 0x70
 * 
 * Destructor. Calls base constructor then conditionally frees if needed.
 * Also handles unlinking from the global script object pool.
 */
swfSCRIPTOBJECT::~swfSCRIPTOBJECT() {
    // Call base constructor (sets vtable)
    // The actual unlinking from pool happens in the scalar destructor
}

/**
 * swfSCRIPTOBJECT::EnumerateMembers @ 0x823FED10 | size: 0x8
 *
 * Delegates to the SYMTAB member table at +4 for property enumeration.
 */
void swfSCRIPTOBJECT::EnumerateMembers() {
    swfSymtab_Enumerate((char*)this + 4);
}

/**
 * swfSCRIPTOBJECT::GetMemberCount @ 0x823F8B48 | size: 0x24
 *
 * Returns the number of properties on this script object by walking
 * the SYMTAB linked list at +4. Each node's next pointer is at +12.
 */
int swfSCRIPTOBJECT::GetMemberCount() {
    int count = 0;
    void* node = m_pMemberTable;
    while (node) {
        count++;
        node = *(void**)((char*)node + 12);
    }
    return count;
}

/**
 * swfSCRIPTOBJECT::VisitMembers @ 0x823FC1A0 | size: 0x8
 *
 * No-op stub — intended for garbage collection or debug traversal.
 */
int swfSCRIPTOBJECT::VisitMembers() {
    // No-op
    return 0;
}

/**
 * swfSCRIPTOBJECT::GetMember @ 0x823FBFC0 | size: 0x1DC
 *
 * ActionScript GetMember — resolves a named property on this object.
 * Special-cases "this" (returns self as type 5/object) and "prototype"
 * (returns prototype chain entry at +12, allocating from pool if needed).
 * Falls back to SYMTAB lookup on the local member table, then the global
 * object table, then the global scope chain.
 *
 * @param name   Interned property name to look up
 * @param result Output: {value, type} pair written on success
 * @return       true if found, false otherwise
 */
bool swfSCRIPTOBJECT::GetMember(const char* name, void* result) {
    extern void* g_swfStringBuffer;       // @ 0x82604850
    extern void* g_swfGlobalObject;       // @ 0x82602818
    extern void* g_swfGlobalScope;        // @ 0x8260282C
    // Intern the property name
    const char* interned = swfInternString(name, &g_swfStringBuffer, SWF_INTERN_BUFFER_SIZE);

    // Special case: "this" — return self as object
    if (strcmp(interned, "this") == 0) {
        *(void**)result = this;
        *((uint32_t*)result + 1) = SWF_VALUE_OBJECT;
        return true;
    }

    // Special case: "prototype" — return prototype chain object
    if (strcmp(interned, "prototype") == 0) {
        void* proto = *(void**)((char*)this + 12);  // m_pPrototype
        if (!proto) {
            // Allocate a new prototype object from the pool
            extern void* g_swfScriptObjectPool;
            void** pool = (void**)g_swfScriptObjectPool;
            void* freeObj = *(void**)((char*)pool + 12);
            uint16_t count = *(uint16_t*)((char*)pool + 6);
            void* next = *(void**)freeObj;

            *(uint16_t*)((char*)pool + 6) = count + 1;
            *(void**)((char*)pool + 12) = next;

            if (freeObj) {
                // Initialize the new object
                extern void* g_vtable_swfSCRIPTOBJECT;  // @ 0x82074D14
                *(void**)freeObj = &g_vtable_swfSCRIPTOBJECT;
                *((uint32_t*)((char*)freeObj + 8)) = 0;
                *((uint32_t*)((char*)freeObj + 4)) = 0;
                *((uint32_t*)((char*)freeObj + 12)) = 0;
            }
            *(void**)((char*)this + 12) = freeObj;
        }
        // Return prototype
        *((uint32_t*)result + 1) = SWF_VALUE_OBJECT;
        *(void**)result = *(void**)((char*)this + 12);
        return true;
    }

    // General case: search local SYMTAB
    void* entry = swfSymtab_Lookup((char*)this + 4, interned);
    if (entry) {
        *(uint32_t*)result = *(uint32_t*)entry;
        *((uint32_t*)result + 1) = *((uint32_t*)entry + 1);
        return true;
    }

    // Fallback: search global object table
    entry = swfSymtab_Lookup(g_swfGlobalObject, interned);
    if (entry) {
        *(uint32_t*)result = *(uint32_t*)entry;
        *((uint32_t*)result + 1) = *((uint32_t*)entry + 1);
        return true;
    }

    // Fallback: search global scope chain
    void* scopeSymtab = (char*)g_swfGlobalScope + 4;
    entry = swfSymtab_Lookup(scopeSymtab, interned);
    if (entry) {
        *(uint32_t*)result = *(uint32_t*)entry;
        *((uint32_t*)result + 1) = *((uint32_t*)entry + 1);
        return true;
    }

    return false;
}

/**
 * swfSCRIPTOBJECT::SetMember @ 0x823FC8A8 | size: 0x5C
 *
 * Sets a named property on this script object. Interns the name string,
 * then inserts the (name, value) pair into the SYMTAB member table at +4.
 */
void swfSCRIPTOBJECT::SetMember(const char* name, void* value) {
    extern void* g_swfStringBuffer;  // @ 0x82604850

    const char* internedName = swfInternString(name, &g_swfStringBuffer, SWF_INTERN_BUFFER_SIZE);
    swfSymtab_Insert((char*)this + 4, internedName, value);
}

/**
 * swfSCRIPTOBJECT::DeleteMember @ 0x823FC848 | size: 0x60
 *
 * Deletes a named property. Builds an {name, 1} "undefined" pair on
 * the stack, calls GetMember (vtable slot 10) to notify watchers,
 * then removes the key from the SYMTAB.
 */
void swfSCRIPTOBJECT::DeleteMember(const char* name) {
    // Build a stack pair: {name_ptr, type=1 (undefined)}
    struct { uint32_t nameRef; uint32_t type; } undefinedPair;
    undefinedPair.nameRef = (uint32_t)(uintptr_t)name;
    undefinedPair.type = SWF_VALUE_UNDEFINED;

    // Notify via GetMember (virtual slot 10) — virtual dispatch on this
    this->GetMember((const char*)&undefinedPair, &undefinedPair);

    // Remove from symbol table
    swfSymtab_Delete((char*)this + 4, name);
}

/**
 * swfSCRIPTOBJECT::Invoke @ 0x823FEA28 | size: 0x120
 *
 * ActionScript method invocation. Looks up the named method via
 * GetMember (vtable slot 10), then dispatches based on result type:
 *   type 6: native C function — direct call with (this, args, nArgs, outResult)
 *   type 5: script object — calls Execute (vtable slot 14) on it
 *   not found: logs "Unsupported script method: %s", sets result to {0, type=3}
 *
 * For type-5 calls, checks if the call stack depth increased (indicating
 * a return value was pushed), and copies the result from the stack.
 */
void swfSCRIPTOBJECT::Invoke(const char* methodName, void* args, int argCount, void* outResult) {

    // Set up GetMember call: lookupKey = {methodName, type=7 (string)}
    // lookupResult = {0, 0} (will be filled in)
    swfValuePair lookupKey = { (uint32_t)(uintptr_t)methodName, SWF_VALUE_STRING };
    swfValuePair lookupResult = { 0, 0 };
    swfValuePair* resultValue = (swfValuePair*)outResult;

    // Call GetMember (virtual slot 10) — virtual dispatch on this
    bool found = this->GetMember((const char*)&lookupKey, &lookupResult);

    if (!found) {
        // Method not found
        extern const char* g_str_swf_unsupportedMethod;  // @ 0x82077054
        rage_DebugLog(g_str_swf_unsupportedMethod, methodName);
        resultValue->data = 0;
        resultValue->type = SWF_VALUE_DELETED;  // undefined
        return;
    }

    // Initialize result to {0, type=object}
    resultValue->data = 0;
    resultValue->type = SWF_VALUE_OBJECT;

    if (lookupResult.type == SWF_VALUE_NATIVE_FUNC) {
        // Type 6: native function pointer — call directly
        typedef void (*NativeFunc)(void*, void*, int, void*);
        NativeFunc fn = (NativeFunc)(uintptr_t)lookupResult.data;
        fn(this, args, argCount, outResult);
    } else if (lookupResult.type == SWF_VALUE_OBJECT) {
        // Type 5: script object — call its Execute method (vtable slot 14).
        // Callable script objects are always swfACTIONFUNC (slot 14 is only
        // defined there); virtual dispatch picks the right derived override.
        swfACTIONFUNC* callableObj = (swfACTIONFUNC*)(uintptr_t)lookupResult.data;

        extern uint32_t g_swfCallDepth;  // @ 0x826064EC
        uint32_t prevDepth = g_swfCallDepth;

        callableObj->Execute(args, argCount, this, 0);

        // If call stack depth increased, a return value was pushed
        if ((int32_t)g_swfCallDepth > (int32_t)prevDepth) {
            swfPopResult(outResult);
        }
    }
    // Other types: return the default {0, 5}
}

void swfSCRIPTOBJECT_ScalarDestructor(swfSCRIPTOBJECT* obj, int flags) {
    // Call constructor to reset vtable
    new (obj) swfSCRIPTOBJECT();
    
    if (flags & 0x1) {
        if (obj) {
            // Unlink from global pool
            void* pool = g_swfScriptArrayPool;  // SDA offset 19552
            void* tail = *((void**)((char*)pool + 12));
            *((void**)((char*)obj + 0)) = tail;
            
            uint16_t count = *((uint16_t*)((char*)pool + 6));
            *((uint16_t*)((char*)pool + 6)) = count + 1;
            *((void**)((char*)pool + 12)) = obj;
        }
    }
}


// ===========================================================================
// swfCONTEXT — Flash execution context
// ===========================================================================

/**
 * swfCONTEXT::~swfCONTEXT() @ 0x823F8FF0 | size: 0x50
 * 
 * Destructor. Cleans up the ActionScript execution context.
 */
swfCONTEXT::~swfCONTEXT() {
    swfContext_Cleanup(this);
}

void swfCONTEXT::ScalarDestructor(int flags) {
    swfContext_Cleanup(this);
    
    if (flags & 0x1) {
        rage_free(this);
    }
}

void swfCONTEXT::Execute() {
    // External function declaration
    extern void hudFlashBase_ExecuteFromContext(void* flashObj, void* param1, void* param2);  // @ 0x823F9CA8
    extern void* g_currentSwfContext;  // @ 0x8260281C (SDA offset 10268)
    
    // Store this context as the current global context
    g_currentSwfContext = this;
    
    // Call hudFlashBase processing with internal state
    // Offsets: +24 (flashObj), +28 (param1), +32 (param2)
    void* flashObj = *((void**)((char*)this + 24));
    void* param1 = *((void**)((char*)this + 28));
    void* param2 = *((void**)((char*)this + 32));
    
    hudFlashBase_ExecuteFromContext(flashObj, param1, param2);
    
    // Clear the global context pointer
    g_currentSwfContext = nullptr;
}



// ===========================================================================
// swfFILE — SWF file representation
// ===========================================================================

/**
 * swfFILE::~swfFILE() @ 0x823F8BE0 | size: 0xDC
 * 
 * Destructor. Cleans up all resources in the SWF file including sprites,
 * fonts, shapes, and other display objects.
 */
swfFILE::~swfFILE() {
    // Set vtable pointer (lis r11,-32249; addi r11,r11,19820)
    // Calculation: (-32249 << 16) + 19820 = 0x82074D8C

    void** resourceArray = SwfGetResourceArray(this);
    uint16_t resourceCount = SwfGetResourceCount(this);

    // Destroy all child resources via virtual destructor dispatch.
    // The resource array holds pointers to swfOBJECT-derived items.
    if (resourceCount > 0) {
        for (uint16_t i = 0; i < resourceCount; i++) {
            swfOBJECT* resource = reinterpret_cast<swfOBJECT*>(resourceArray[i]);
            if (resource) {
                void** vtable = *((void***)resource);
                typedef void (*DestructorFn)(swfOBJECT*, int);
                ((DestructorFn)vtable[0])(resource, 1);
            }
        }
    }

    if (resourceArray) {
        rage_free(resourceArray);
    }
}


/**
 * swfCMD::~swfCMD() @ 0x82407E90 | size: 0x48
 * 
 * Base destructor for SWF command objects. Sets vtable and conditionally
 * frees the object if the flags parameter has bit 0 set.
 */

swfCMD::~swfCMD() {
    // Vtable is set by derived destructors
}

void swfCMD_ScalarDestructor(swfCMD* obj, int flags) {
    if (flags & 0x1) {
        rage_free(obj);
    }
}

/**
 * swfCMD_DoAction::~swfCMD_DoAction() @ 0x82407ED8 | size: 0x88
 * 
 * Destructor for DoAction command. Decrements reference count on the
 * ActionScript bytecode buffer and frees it when count reaches zero.
 */
swfCMD_DoAction::~swfCMD_DoAction() {
    // Access the bytecode buffer at offset +16
    void* bytecodeBuffer = *((void**)((char*)this + 16));
    
    if (bytecodeBuffer) {
        // Decrement reference count (uint16_t at offset +0 of buffer)
        uint16_t* refCountPtr = (uint16_t*)bytecodeBuffer;
        uint16_t refCount = *refCountPtr;
        
        // Decrement: (refCount + 0x10000 - 1) & 0xFFFF
        refCount = (refCount - 1) & 0xFFFF;
        *refCountPtr = refCount;
        
        // Free buffer if refcount reached zero
        if (refCount == 0) {
            rage_free(bytecodeBuffer);
        }
    }
}

void swfCMD_DoAction_ScalarDestructor(swfCMD_DoAction* obj, int flags) {
    // Call destructor
    obj->~swfCMD_DoAction();
    
    if (flags & 0x1) {
        rage_free(obj);
    }
}

/**
 * swfCMD_DoInitAction::~swfCMD_DoInitAction() @ 0x82407F60 | size: 0x88
 * 
 * Destructor for DoInitAction command. Same pattern as DoAction - decrements
 * reference count on the ActionScript initialization bytecode buffer.
 */
swfCMD_DoInitAction::~swfCMD_DoInitAction() {
    // Access the bytecode buffer at offset +16
    void* bytecodeBuffer = *((void**)((char*)this + 16));
    
    if (bytecodeBuffer) {
        // Decrement reference count (uint16_t at offset +0 of buffer)
        uint16_t* refCountPtr = (uint16_t*)bytecodeBuffer;
        uint16_t refCount = *refCountPtr;
        
        // Decrement: (refCount + 0x10000 - 1) & 0xFFFF
        refCount = (refCount - 1) & 0xFFFF;
        *refCountPtr = refCount;
        
        // Free buffer if refcount reached zero
        if (refCount == 0) {
            rage_free(bytecodeBuffer);
        }
    }
}

void swfCMD_DoInitAction_ScalarDestructor(swfCMD_DoInitAction* obj, int flags) {
    // Call destructor
    obj->~swfCMD_DoInitAction();
    
    if (flags & 0x1) {
        rage_free(obj);
    }
}

/**
 * swfCMD_PlaceObject2::~swfCMD_PlaceObject2() @ 0x82407FF0 | size: 0x74
 * 
 * Destructor for PlaceObject2 command. Frees two allocated buffers
 * (likely color transform and matrix data).
 */
swfCMD_PlaceObject2::~swfCMD_PlaceObject2() {
    // Free color transform buffer at offset +24
    void* colorTransform = *((void**)((char*)this + 24));
    if (colorTransform) {
        rage_free(colorTransform);
    }
    
    // Free matrix buffer at offset +28
    void* matrix = *((void**)((char*)this + 28));
    if (matrix) {
        rage_free(matrix);
    }
}

void swfCMD_PlaceObject2_ScalarDestructor(swfCMD_PlaceObject2* obj, int flags) {
    // Call destructor
    obj->~swfCMD_PlaceObject2();
    
    if (flags & 0x1) {
        rage_free(obj);
    }
}

/**
 * swfCMD_PlaceObject2ClipEvent::~swfCMD_PlaceObject2ClipEvent() @ 0x82408160 | size: 0x50
 * 
 * Destructor for PlaceObject2 clip event. Calls cleanup helper first.
 */
swfCMD_PlaceObject2ClipEvent::~swfCMD_PlaceObject2ClipEvent() {
    // Call cleanup helper (frees event handler list)
    extern void swfPlaceObject2ClipEvent_Cleanup(void* ptr);  // @ 0x82408068
    swfPlaceObject2ClipEvent_Cleanup(this);
}

void swfCMD_PlaceObject2ClipEvent_ScalarDestructor(swfCMD_PlaceObject2ClipEvent* obj, int flags) {
    // Call destructor
    obj->~swfCMD_PlaceObject2ClipEvent();
    
    if (flags & 0x1) {
        rage_free(obj);
    }
}


/**
 * swfCMD_DoInitAction::Cleanup() @ 0x821EFC00 | size: 0x10
 * 
 * Virtual function slot 2 - tail calls to scalar destructor (slot 1).
 * This is a common RAGE pattern for cleanup functions.
 */
void swfCMD_DoInitAction::Cleanup() {
    // Tail-calls the scalar destructor (vtable slot 1) with flags=0
    // (destroy object contents but do not free the allocation).
    // Slot 1 is the compiler-synthesised scalar dtor, not declared in
    // the class header — retain explicit vtable indirection.
    void** vtable = *((void***)this);
    typedef void (*ScalarDtorFn)(swfCMD_DoInitAction*, int);
    ((ScalarDtorFn)vtable[1])(this, 0);
}



/**
 * swfFILE::FindExportFrame @ 0x823F9DC8 | size: 0xBC
 *
 * Searches the file's export/label table for a named frame label.
 * The export array is at +20, count at +46. Each entry with type
 * byte == 5 (at entry+4) has a name string at entry+192.
 * Comparison is case-insensitive via _stricmp.
 *
 * If found, computes the frame-relative position by calling a helper
 * that returns a normalized float, then scales by a global factor
 * and the input frameRate. If not found, logs an error and returns 0.0f.
 *
 * @param frameRate   Input time scale (passed in f1)
 * @param labelName   Frame label to search for
 * @param context     Timeline context for position computation
 * @return            Frame position scaled by frameRate, or 0.0f if not found
 */
float swfFILE::FindExportFrame(float frameRate, const char* labelName, void* context) {

    uint16_t exportCount = *(uint16_t*)((char*)this + 46);
    void** exportArray = *(void***)((char*)this + 20);

    for (int i = 0; i < exportCount; i++) {
        void* entry = exportArray[i];
        if (!entry) continue;

        uint8_t entryType = *(uint8_t*)((char*)entry + 4);
        if (entryType != 5) continue;

        const char* entryName = (const char*)((char*)entry + 192);
        if (_stricmp(entryName, labelName) == 0) {
            // Found — compute frame position and scale
            float outMin, outMax;
            float basePosition = swfComputeFramePosition(context, entry, &outMin, &outMax);
            // Scale: basePosition * globalFrameScale * inputFrameRate
            extern float g_swfFrameScale;  // @ 0x82079B90
            return basePosition * g_swfFrameScale * frameRate;
        }
    }

    // Not found — log and return zero
    // Error string @ 0x82076C80: "Couldn't find font with which..."
    // (reused for "not found" in this context)
    rage_DebugLog("Couldn't find frame label '%s'", labelName);
    return 0.0f;
}


// ===========================================================================
// swfINSTANCE — Flash display object instance
// ===========================================================================

/**
 * swfINSTANCE::~swfINSTANCE() @ 0x823FB600 | size: 0x70
 * 
 * Destructor. Cleans up a Flash display object instance and unlinks it
 * from the instance pool.
 */
swfINSTANCE::~swfINSTANCE() {
    swfInstance_Cleanup(this);
}

void swfINSTANCE_ScalarDestructor(swfINSTANCE* obj, int flags) {
    swfInstance_Cleanup(obj);
    
    if (flags & 0x1) {
        if (obj) {
            // Unlink from global instance pool
            void* pool = g_swfInstancePool;  // SDA offset 19536
            void* tail = *((void**)((char*)pool + 12));
            *((void**)((char*)obj + 0)) = tail;
            
            uint16_t count = *((uint16_t*)((char*)pool + 6));
            *((uint16_t*)((char*)pool + 6)) = count + 1;
            *((void**)((char*)pool + 12)) = obj;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// swfINSTANCE — Flash display list instance (MovieClip)
//
// Memory layout (key fields):
//   +0x00: vtable          +0x04: m_pDefinition (swfFILE/sprite def)
//   +0x48: m_pActionList    +0xA0: m_pSprite (child sprite/shape)
//   +0xA4: m_currentFrame   +0xA6: m_depth
//   +0xA8: m_clipDepth      +0xAA: m_bIsPlaying (bool)
//   +0xAB: m_bDirty         +0xAC: m_bVisible
//   +0xB0: m_pParent        +0xB4: m_pChildList
//   +0xB8: m_pNextSibling
// ─────────────────────────────────────────────────────────────────────────────

/**
 * swfINSTANCE::GotoFrame @ 0x823FB7A8 | size: 0xF8
 *
 * Advances the timeline to target frame, executing frame commands along
 * the way. If the target is past the last frame, wraps to 0 and clears
 * all child instances' dirty flags. On each frame step, executes the
 * frame's command list (DoAction tags) if skipActions==0.
 */
void swfINSTANCE::GotoFrame(uint16_t targetFrame, uint8_t skipActions) {
    void* def = SwfGetDefinition(this);
    uint16_t totalFrames = SwfGetTotalFrames(def);

    if (targetFrame >= totalFrames) return;

    uint16_t& currentFrame = SwfCurrentFrameRef(this);
    uint16_t curFrame = currentFrame;
    if (curFrame == targetFrame) return;

    while (curFrame != targetFrame) {
        uint16_t nextFrame = (currentFrame + 1) & 0xFFFF;
        uint16_t maxFrames = SwfGetTotalFrames(SwfGetDefinition(this));

        if (nextFrame >= maxFrames) {
            for (swfINSTANCE* child = SwfChildList(this); child; child = SwfNextSibling(child)) {
                SwfWrapResetFlagRef(child) = 0;
            }
            nextFrame = 0;
        }
        currentFrame = nextFrame;

        if (skipActions == 0) {
            void* curDef2 = SwfGetDefinition(this);
            uint16_t frameIdx = currentFrame;
            void* frameTable = SwfGetFrameTable(curDef2);
            void* cmdList = *(void**)((char*)frameTable + frameIdx * 8 + 4);
            while (cmdList) {
                typedef void (*ExecFunc)(void*, swfINSTANCE*);
                void** cmdVtable = *(void***)cmdList;
                ExecFunc exec = (ExecFunc)cmdVtable[2];
                exec(cmdList, this);
                cmdList = *(void**)((char*)cmdList + 12);
            }
        } else {
            void* curDef2 = SwfGetDefinition(this);
            typedef void (*DefExecFunc)(void*, swfINSTANCE*);
            void** defVtable = *(void***)curDef2;
            DefExecFunc defExec = (DefExecFunc)defVtable[2];
            defExec(curDef2, this);
        }

        curFrame = currentFrame;
    }
}

/**
 * swfINSTANCE::NextFrame @ 0x823FB8A0 | size: 0x80
 *
 * Advances to next frame with wrapping. Saves and restores the
 * m_bIsPlaying flag, calling GotoFrame with play=true.
 */
void swfINSTANCE::NextFrame(void* context) {
    uint16_t curFrame = SwfCurrentFrameRef(this);
    uint16_t nextFrame = (curFrame + 1) & 0xFFFF;
    uint16_t maxFrames = SwfGetTotalFrames(SwfGetDefinition(this));

    if (nextFrame >= maxFrames) {
        nextFrame = 0;
    }

    uint8_t& isPlaying = SwfIsPlayingRef(this);
    uint8_t savedPlaying = isPlaying;
    isPlaying = 1;

    this->GotoFrame(nextFrame, (uint8_t)(uintptr_t)context);

    isPlaying = savedPlaying;
}

/**
 * swfINSTANCE::PrevFrame @ 0x823FB920 | size: 0x50
 *
 * Goes to previous frame. If at frame 0, wraps to last frame.
 * Calls GotoFrame (vtable slot 1) with stop flag.
 */
void swfINSTANCE::PrevFrame() {
    uint16_t curFrame = SwfCurrentFrameRef(this);

    if (curFrame == 0) {
        uint16_t maxFrames = SwfGetTotalFrames(SwfGetDefinition(this));
        uint16_t lastFrame = (maxFrames + 0x10000 - 1) & 0xFFFF;
        this->GotoFrame(lastFrame, 0);
    } else {
        uint16_t prevFrame = (curFrame + 0x10000 - 1) & 0xFFFF;
        this->GotoFrame(prevFrame, 0);
    }
}

/**
 * swfINSTANCE::MarkDirty @ 0x823FB760 | size: 0x48
 *
 * Sets the dirty flag (+171) to 1. If the parent sprite at +176 exists
 * and its definition type byte is not 2 (shape), calls the sprite's
 * Advance method (vtable slot 4).
 */
void swfINSTANCE::MarkDirty() {
    *(uint8_t*)((char*)this + 171) = 1;  // m_bDirty = true

    // Parent is another display-list instance (swfINSTANCE).
    swfINSTANCE* parentSprite = *(swfINSTANCE**)((char*)this + 176);
    if (!parentSprite) return;

    void* memberTable = *(void**)((char*)this + 4);
    if (!memberTable) return;

    uint8_t defType = *(uint8_t*)((char*)memberTable + 4);
    if (defType == 2) return;  // shapes don't need advance

    // Call parent's MarkDirty (virtual slot 4) via virtual dispatch
    parentSprite->MarkDirty();
}

/**
 * swfINSTANCE::SetVisible @ 0x823FB150 | size: 0x8
 *
 * Sets the visibility flag at +172 to true.
 */
void swfINSTANCE::SetVisible() {
    *(uint8_t*)((char*)this + 172) = 1;
}

/**
 * swfINSTANCE::EnumerateMembers @ 0x823FED18 | size: 0xBC
 *
 * Recursively enumerates all members: calls EnumerateMembers (vtable
 * slot 6) on the next sibling (+184), child list (+180), and sprite
 * (+160). Also walks the action list at +72, setting bit 0 of each
 * node's flags (+12).
 */
void swfINSTANCE::EnumerateMembers() {
    // Enumerate next sibling (another swfINSTANCE on the display list)
    swfINSTANCE* sibling = *(swfINSTANCE**)((char*)this + 184);
    if (sibling) {
        sibling->EnumerateMembers();
    }

    // Enumerate child list (head of children, also a swfINSTANCE)
    swfINSTANCE* childList = *(swfINSTANCE**)((char*)this + 180);
    if (childList) {
        childList->EnumerateMembers();
    }

    // Enumerate attached sprite/definition at +160.
    // This is a swfSCRIPTOBJECT-derived resource (sprite definition).
    swfSCRIPTOBJECT* sprite = *(swfSCRIPTOBJECT**)((char*)this + 160);
    if (sprite) {
        sprite->EnumerateMembers();
    }

    // Walk action list and set the visited bit without clobbering the next link.
    void* actionNode = *(void**)((char*)this + 72);
    while (actionNode) {
        void* nextNode = *(void**)((char*)actionNode + 12);
        *(uint32_t*)((char*)actionNode + 12) = ((uint32_t)(uintptr_t)nextNode) | SWF_ACTION_NODE_VISITED_BIT;
        actionNode = nextNode;
    }
}

/**
 * swfINSTANCE::GetMemberCount @ 0x823FB160 | size: 0x28
 *
 * Returns member count by delegating to the sprite at +160.
 * If no sprite, returns 0.
 */
int swfINSTANCE::GetMemberCount() {
    swfSCRIPTOBJECT* sprite = *(swfSCRIPTOBJECT**)((char*)this + 160);
    if (!sprite) return 0;

    // Virtual GetMemberCount (slot 8) via virtual dispatch
    return sprite->GetMemberCount();
}

/**
 * swfINSTANCE::VisitMembers @ 0x823FBF98 | size: 0x28
 *
 * Delegates to sprite at +160's VisitMembers (vtable slot 9).
 * If no sprite, returns 0.
 */
int swfINSTANCE::VisitMembers() {
    swfSCRIPTOBJECT* sprite = *(swfSCRIPTOBJECT**)((char*)this + 160);
    if (!sprite) return 0;

    // Virtual VisitMembers (slot 9) via virtual dispatch
    return sprite->VisitMembers();
}

/**
 * swfACTIONFUNC::SetMember() @ 0x823FF4A0 | size: 0x14
 * 
 * Virtual function slot 11 - forwards to inner object's SetMember.
 * The inner object is stored at offset +7332.
 */
void swfACTIONFUNC::SetMember(const char* name, void* value) {
    // Forward to inner MovieClip's SetMember (virtual slot 11).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) {
        innerObj->SetMember(name, value);
    }
}

/**
 * swfACTIONFUNC::DeleteMember() @ 0x823FF4B8 | size: 0x14
 *
 * Virtual function slot 12 - forwards to inner object's DeleteMember.
 */
void swfACTIONFUNC::DeleteMember(const char* name) {
    // Forward to inner MovieClip's DeleteMember (virtual slot 12).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) {
        innerObj->DeleteMember(name);
    }
}


// swfINSTANCE overrides for Flash display object properties:
// slot 10 = GetMember (770 lines — dispatches "this", "_parent", "_root", textColor,
//          _name, _width, _height, _xscale, _yscale, _alpha, _rotation, _x, _y, etc.)
bool swfINSTANCE::GetMember(const char* name, void* result) {
    // Full body @ 0x823FB970 is a 770-line property dispatcher that fans
    // out to "this", "_parent", "_root", textColor, _name, _width,
    // _height, _xscale, _yscale, _alpha, _rotation, _x, _y and friends
    // before falling back to the script-object base. Pending dedicated
    // lift; forwarding to base keeps non-display lookups working today.
    return swfSCRIPTOBJECT::GetMember(name, result);
}

// slot 11 = SetMember (736 lines — handles _x, _y, _xscale, _yscale, _alpha,
//          _rotation, _name, __texturename, __drawcallback, textColor, _visible, etc.)
void swfINSTANCE::SetMember(const char* name, void* value) {
    // Full body @ 0x823FC1A8 (736 lines) mirrors GetMember on the write side:
    // _x, _y, _xscale, _yscale, _alpha, _rotation, _name, __texturename,
    // __drawcallback, textColor, _visible, etc. Pending dedicated lift;
    // forwarding to the script-object base covers plain member writes.
    swfSCRIPTOBJECT::SetMember(name, value);
}

// slot 12 = DeleteMember (same as base — no special display property handling)
void swfINSTANCE::DeleteMember(const char* name) {
    swfSCRIPTOBJECT::DeleteMember(name);
}

// slot 13 = Invoke (dispatches gotoAndPlay, gotoAndStop, play, nextFrame, prevFrame,
//          getBytesLoaded, getBytesTotal, attachMovie + falls through to base)
void swfINSTANCE::Invoke(const char* methodName, void* args, int argCount, void* outResult) {
    // Full body @ 0x823FE500 dispatches gotoAndPlay, gotoAndStop, play,
    // nextFrame, prevFrame, getBytesLoaded, getBytesTotal, attachMovie,
    // then falls through to the script-object base. Pending dedicated
    // lift; base Invoke handles the non-display-list AS methods.
    swfSCRIPTOBJECT::Invoke(methodName, args, argCount, outResult);
}


// ===========================================================================
// ===========================================================================
// swfACTIONFUNC — ActionScript function object
//
// swfACTIONFUNC wraps an inner swfSCRIPTOBJECT (at +7332, m_pInnerObject)
// and extends it with a local variable table for ActionScript closures.
// The local table stores up to m_localCount (at +132) named bindings,
// each as a 128-byte name string at +136 + index*128, with corresponding
// 8-byte {value, type} pairs in m_pLocalValues (at +7328).
//
// Most vtable methods are simple forwarding thunks that delegate to the
// inner object's corresponding vtable slot.
// ===========================================================================

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::ScalarDtor  [vtable slot 1 @ 0x823FF4E8]
// Forwards to inner object's scalar destructor (vtable slot 1).
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::ScalarDtor(int flags) {
    // Forward to inner MovieClip's scalar destructor (vtable slot 1).
    // Slot 1 is compiler-synthesised (not a declared virtual), so retain
    // explicit vtable indirection through a typed function pointer.
    swfINSTANCE* innerObj = m_pInnerObject;
    typedef void (*ScalarDtorFn)(swfINSTANCE*, int);
    void** vtable = *(void***)innerObj;
    ((ScalarDtorFn)vtable[1])(innerObj, flags);
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::NextFrame  [vtable slot 2 @ 0x823FF500]
// Forwards to inner object's vtable slot 2.
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::NextFrame() {
    // Forward to inner MovieClip's NextFrame (virtual slot 2).
    // Original binary passes no context argument.
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->NextFrame(nullptr);
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::PrevFrame  [vtable slot 3 @ 0x823FF518]
// Forwards to inner object's vtable slot 3.
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::PrevFrame() {
    // Forward to inner MovieClip's PrevFrame (virtual slot 3).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->PrevFrame();
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::MarkDirty  [vtable slot 4 @ 0x823FF530]
// Forwards to inner object's vtable slot 4.
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::MarkDirty() {
    // Forward to inner MovieClip's MarkDirty (virtual slot 4).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->MarkDirty();
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::SetVisible  [vtable slot 5 @ 0x823FF548]
// Forwards to inner object's vtable slot 5.
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::SetVisible() {
    // Forward to inner MovieClip's SetVisible (virtual slot 5).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->SetVisible();
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::EnumerateMembers  [vtable slot 6 @ 0x823FF560]
// Forwards to inner object's vtable slot 6.
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::EnumerateMembers() {
    // Forward to inner MovieClip's EnumerateMembers (virtual slot 6).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->EnumerateMembers();
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::VisitChildren  [vtable slot 7 @ 0x823FF578]
// Forwards to inner object's vtable slot 7.
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::VisitChildren() {
    // Forward to inner MovieClip's child lookup/create helper (virtual slot 7).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->FindOrCreateChildByDepth();
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::GetMemberCount  [vtable slot 8 @ 0x823FF2A8]
// Returns total member count: inner object's count + local variable count.
// ─────────────────────────────────────────────────────────────────────────────
int swfACTIONFUNC::GetMemberCount() {
    // Inner MovieClip's member count + this closure's local bindings.
    swfINSTANCE* innerObj = m_pInnerObject;  // +7332
    int innerCount = innerObj ? innerObj->GetMemberCount() : 0;
    return innerCount + m_localCount;  // +132
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::Invoke  [vtable slot 13 @ 0x823FF4D0]
// Forwards to inner object's Invoke (vtable slot 13).
// ─────────────────────────────────────────────────────────────────────────────
void swfACTIONFUNC::Invoke(const char* methodName, void* args, int argCount, void* outResult) {
    // Forward to inner MovieClip's Invoke (virtual slot 13).
    swfINSTANCE* innerObj = m_pInnerObject;
    if (innerObj) innerObj->Invoke(methodName, args, argCount, outResult);
}

// ─────────────────────────────────────────────────────────────────────────────
// swfACTIONFUNC::GetMember  [vtable slot 10 @ 0x823FF2F0]
//
// Searches local variable table first (name strings at +136, stride 128),
// then delegates to inner object's GetMember (vtable slot 10).
// Local entries store 8-byte {value, type} pairs in m_pLocalValues (+7328).
// ─────────────────────────────────────────────────────────────────────────────
bool swfACTIONFUNC::GetMember(const char* name, void* result) {
    // Intern the name string
    char internBuffer[SWF_INTERN_BUFFER_SIZE];
    extern const char* swfInternString(const char* name, void* buffer, int maxLen);
    const char* interned = swfInternString(name, internBuffer, SWF_INTERN_BUFFER_SIZE);

    // Search local variable table
    int localCount = m_localCount;  // +132
    if (localCount > 0) {
        const char* localNames = (const char*)this + 136;  // local name table
        for (int i = 0; i < localCount; i++) {
            // Compare interned name against local name entry (byte-by-byte)
            const char* entryName = localNames + i * 128;
            const char* src = interned;
            const char* dst = entryName;
            int diff = 0;
            while (*src != '\0') {
                diff = (int)(uint8_t)*src - (int)(uint8_t)*dst;
                if (diff != 0) break;
                src++;
                dst++;
            }
            if (diff == 0) {
                // Found — copy 8-byte value pair from m_pLocalValues
                uint32_t* localValues = (uint32_t*)m_pLocalValues;  // +7328
                uint32_t* out = (uint32_t*)result;
                out[0] = localValues[i * 2];
                out[1] = localValues[i * 2 + 1];
                return true;
            }
        }
    }

    // Not found locally — delegate to inner MovieClip's GetMember (slot 10).
    swfINSTANCE* innerObj = m_pInnerObject;  // +7332
    if (!innerObj) return false;
    return innerObj->GetMember(name, result);
}

int swfACTIONFUNC::VisitMembers() {
    // Forwards to inner MovieClip's VisitMembers (virtual slot 9).
    swfINSTANCE* innerObj = m_pInnerObject;
    return innerObj ? innerObj->VisitMembers() : 0;
}


// ===========================================================================
// swfSCRIPTARRAY — ActionScript array object
// ===========================================================================

/**
 * swfSCRIPTARRAY::~swfSCRIPTARRAY @ 0x823FF218 | size: 0x80
 *
 * Destructor — sets vtable to swfSCRIPTARRAY, frees the element array
 * at +16 if capacity (+22) > 0, then calls swfSCRIPTOBJECT base
 * destructor. If the delete flag is set, adds this object back to
 * the SWF object pool's free list.
 */
swfSCRIPTARRAY::~swfSCRIPTARRAY() {
    // Free element array (at +16) if capacity (uint16 at +22) > 0
    uint16_t capacity = *(uint16_t*)((char*)this + 22);
    if (capacity > 0) {
        void* elements = *(void**)((char*)this + 16);
        rage_free(elements);
    }
    // swfSCRIPTOBJECT base destructor runs automatically via C++ chain
}



// swfSCRIPTARRAY overrides GetMember/SetMember for array-style access.
// Both overrides @ 0x823FEB48 / 0x823FEC38 recognise numeric index keys
// ("0", "1", ...) and "length", falling through to the script-object
// base for everything else. Pending dedicated lift; base lookup still
// handles string-named members correctly.
bool swfSCRIPTARRAY::GetMember(const char* name, void* result) { return swfSCRIPTOBJECT::GetMember(name, result); }
void swfSCRIPTARRAY::SetMember(const char* name, void* value) { swfSCRIPTOBJECT::SetMember(name, value); }


// ===========================================================================
// swfOBJECT and derived display object classes
// ===========================================================================

// swfOBJECT/swfSPRITE destructors @ 0x82403518 / 0x82403560 just reseat
// the vtable pointer and return; any owned heap buffers are released by
// derived destructors (swfFONT / swfSHAPE / swfBUTTON / swfEDITTEXT).
swfOBJECT::~swfOBJECT() {}
swfSPRITE::~swfSPRITE() {}

// ===========================================================================
// swfCONTEXT Additional Methods
// ===========================================================================

/**
 * swfContext_IsGlobalSlot13AltMode @ 0x823FB1F0 | size: 0x64
 * 
 * Checks if the global SWF object's slot 13 has mismatched sign bits
 * between byte[0] and byte[3], indicating an alternate execution mode.
 * Returns true if the XOR of two bytes exceeds 127 (sign bit differs).
 */
bool swfContext_IsGlobalSlot13AltMode() {
    // External declarations
    extern void* swfGlobalObject_GetSlotValuePtr(void* obj, int slotIndex);  // @ 0x823FB188
    extern void* g_swfGlobalObject;  // @ 0x82604848 (SDA offset 18504)
    
    // Get slot 13 value from the global SWF object
    uint8_t* result = (uint8_t*)swfGlobalObject_GetSlotValuePtr(g_swfGlobalObject, 13);
    
    // Load bytes at offsets +0 and +3
    uint8_t byte0 = result[0];
    uint8_t byte3 = result[3];
    
    // XOR the bytes and check if result > 127
    uint8_t xorResult = byte0 ^ byte3;
    return (xorResult > 127);
}


/**
 * swfCONTEXT_ProcessContextTree @ 0x823FCA50 | size: 0x118
 * 
 * Recursive context processing function. Checks context state flags,
 * calls virtual methods, and manages a context stack. Recursively
 * processes child contexts in a linked list.
 */
void swfCONTEXT_ProcessContextTree(swfCONTEXT* ctx) {
    // External declarations
    extern void* g_currentSwfContext;  // @ 0x8260281C (SDA offset 10268)
    extern uint8_t g_contextStack[128];  // @ 0x82604C68 (SDA offset 19560)
    
    // Check flag at offset +173
    uint8_t flag173 = *((uint8_t*)((char*)ctx + 173));
    if (flag173 == 0) {
        return;
    }
    
    // Check nested object at offset +176
    void* nestedObj = *((void**)((char*)ctx + 176));
    bool shouldCallVirtual = true;
    
    if (nestedObj != nullptr) {
        uint8_t nestedFlag = *((uint8_t*)((char*)nestedObj + 170));
        if (nestedFlag != 0) {
            shouldCallVirtual = false;
        }
    }
    
    // Call virtual method slot 6 on object at offset +4 (a swfINSTANCE).
    // Slot 6 on swfINSTANCE is EnumerateMembers; in ProcessContextTree the
    // original binary reads the return value as a uint8_t — treat as bool.
    // The declared signature in rage_swf.hpp is `void EnumerateMembers()`,
    // but the binary leaves r3 populated from the internal walker so the
    // caller re-interprets it as a bool — we keep the explicit vtable
    // indirection to preserve the sampled return value.
    uint8_t callResult = 0;
    if (shouldCallVirtual) {
        swfINSTANCE* obj = *((swfINSTANCE**)((char*)ctx + 4));
        void** vtable = *((void***)obj);
        typedef uint8_t (*VirtualFn)(swfINSTANCE*);
        callResult = ((VirtualFn)vtable[6])(obj);
    }
    
    // If virtual call returned non-zero, push context onto stack
    if (callResult != 0) {
        // Access global context stack
        int* stackCount = (int*)((char*)g_contextStack + 128 - 1052);
        
        if (*stackCount < 32) {
            // Check if this context is the current global context
            if (g_currentSwfContext == ctx) {
                // Store previous stack top
                int* prevTop = (int*)((char*)g_contextStack + 128 - 4);
                *prevTop = *stackCount;
            }
            
            // Push context onto stack
            int index = *stackCount;
            void** stackArray = (void**)g_contextStack;
            stackArray[index] = ctx;
            (*stackCount)++;
        }
    } else {
        // If this is the current global context, call virtual slot 3.
        // On swfINSTANCE slot 3 is PrevFrame() but the original binary
        // passes (ctx, 2, 1) — indicating the real signature is a
        // different dispatch (likely ClipEvent / state transition) on
        // a derived display-list node. Keep explicit vtable indirection.
        if (g_currentSwfContext == ctx) {
            swfINSTANCE* obj = *((swfINSTANCE**)((char*)ctx + 4));
            void** vtable = *((void***)obj);
            typedef void (*VirtualFn3)(swfINSTANCE*, void*, int, int);
            ((VirtualFn3)vtable[3])(obj, ctx, 2, 1);

            // Clear global context
            g_currentSwfContext = nullptr;
        }
    }
    
    // Recursively process child contexts (linked list at offset +180/+184)
    void* childCtx = *((void**)((char*)ctx + 180));
    while (childCtx != nullptr) {
        swfCONTEXT_ProcessContextTree((swfCONTEXT*)childCtx);
        childCtx = *((void**)((char*)childCtx + 184));
    }
}

// ============================================================================
// SWF namespace-rage globals (moved from stubs.cpp)
// These are the canonical definitions used across the UI/SWF subsystem.
// ============================================================================
void*    g_currentSwfContext = nullptr;
uint32_t g_swfCallDepth = 0;
float    g_swfFrameScale = 1.0f;
void*    g_swfGlobalObject = nullptr;
void*    g_swfGlobalScope = nullptr;
void*    g_swfStringBuffer = nullptr;
void*    g_vtable_swfSCRIPTOBJECT = nullptr;

// ── swfINSTANCE::FindOrCreateChildByDepth @ 0x823FC908 size:0x144 ──
// Looks up a child instance by 16-bit depth key (+164/+166/+168/+176/+184).
// Allocates from g_swfInstancePool when missing. Pending full decomp.
void swfINSTANCE::FindOrCreateChildByDepth() {}

} // namespace rage

// ============================================================================
// Stubs pending full implementation — pg_/PageGroup/swf*/hudFlashBase/Dialog
// (moved from src/stubs.cpp and src/stubs_final.cpp). These bodies remain
// as no-ops until their callees (pg_5B10_fw, pg_5BC8_fw, pg_80D0, pg_C3B8_g,
// UpdatePageGroup, pg_6C80_g, pongCameraMgr_5CE8_2hr, etc.) are themselves lifted.
// ============================================================================

// Forward decls for typed params used below.
struct TransitionParams;
struct TransitionFlags;

// ── pg_* / PageGroup / Dialog / TextEntry / CreditsRoll ─────────────────────
bool PageGroup_DispatchEvent(void* group, void* event, int eventType, unsigned int* outResult, int flags) {
    (void)group; (void)event; (void)eventType; (void)outResult; (void)flags; return false;
}
void* PageGroupNames_ResolveByHandle(void* namesMgr) { (void)namesMgr; return nullptr; }
void* PageGroupNames_ResolveByHandle(void* namesMgr, int handle) { (void)namesMgr; (void)handle; return nullptr; }
uint8_t PageGroup_ApplyTransition(void* gameState, TransitionParams* params, int transitionType,
                                  TransitionFlags* flags, int enableFlag) {
    (void)gameState; (void)params; (void)transitionType; (void)flags; (void)enableFlag; return 0;
}
uint8_t pg_ApplyTransition(void* gameState, TransitionParams* params, int transitionType,
                           TransitionFlags* flags, int enableFlag) {
    return PageGroup_ApplyTransition(gameState, params, transitionType, flags, enableFlag);
}
void PostPageGroupMessage_Internal(int messageId, int param1, int param2, int param3) { (void)messageId; (void)param1; (void)param2; (void)param3; }
void GeneratePageGroupCommands(void) {}

extern "C" void UpdatePageGroup(void* group) { (void)group; }
extern "C" void pgBase_AcquireRef(void* a, int b) { (void)a; (void)b; }

void PageGroup_DispatchEventCompat(void* group, void* event, int eventType,
                                   unsigned int* outResult, int flags) {
    (void)group; (void)event; (void)eventType; (void)outResult; (void)flags;
}
void pgPageGroup_DispatchEvent(void* group, void* event, int eventType,
                               unsigned int* outResult, int flags) {
    PageGroup_DispatchEventCompat(group, event, eventType, outResult, flags);
}

extern "C" {
void _c_pgPageGroup_DispatchEvent(void* a, void* b, int c, unsigned* d, int e) __asm__("_pg_6F68");
void _c_pgPageGroup_DispatchEvent(void* a, void* b, int c, unsigned* d, int e) {
    PageGroup_DispatchEvent(a, b, c, d, e);
}
} // extern "C"

void* PageGroup_GetTextEntry(void* a, const char* b) { (void)a; (void)b; return nullptr; }
void* PageGroup_LookupText(uint32_t a, const char* b) { (void)a; (void)b; return nullptr; }
void PageGroup_Register(void* a) { (void)a; }
void* PageGroup_SetState(void* a) { (void)a; return nullptr; }

extern "C" {
void DestroyPageGroup(void* group) { (void)group; }
int GetPageGroupState(void* group) { (void)group; return 0; }
void ProcessPageGroupInput(void* group) { (void)group; }
void NotifyUIEvent(int event) { (void)event; }
void* GetStateContextName(void* a) { (void)a; return nullptr; }
void FadePageGroup(void* a, float b, uint32_t c, uint32_t d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void FadePageGroupOut(void* a, float b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
} // extern "C"

// C++-linkage FadePageGroup overload (different signature).
void FadePageGroup(void* a, int b, int c, float d, float e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// C++-linkage PostPageGroupMessage overloads.
void PostPageGroupMessage(int a, int b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void PostPageGroupMessage(uint32_t a, uint8_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}
void PostPageGroupMessage(void* a, int b, int c, int d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

extern "C" void PostPageGroupMessage_C(int a, int b, int c, int d) __asm__("_PostPageGroupMessage");
extern "C" void PostPageGroupMessage_C(int a, int b, int c, int d) {
    (void)a; (void)b; (void)c; (void)d;
}

void DismissPageGroup(void* a) { (void)a; }
void InitializePageGroup(void* a) { (void)a; }
void SetPageGroupVisible(void* a) { (void)a; }

uint8_t Dialog_IsComplete(void* a) { (void)a; return 0; }
void* DialogPageGroup_Construct(void* a) { (void)a; return nullptr; }
void* LegalsPageGroup_Construct(void* a) { (void)a; return nullptr; }

int32_t TextEntry_GetValue(void* a) { (void)a; return 0; }

void CreditsRoll_Deactivate(void* a, int b, int c) { (void)a; (void)b; (void)c; }
void* CreditsRoll_NotifyEntry(void* a, uint32_t b) { (void)a; (void)b; return nullptr; }

// ── hudFlashBase_* ──────────────────────────────────────────────────────────
void hudFlashBase_RenderElement(void* hudObj, int modeIndex, float param1, float param2,
                                int flag1, int flag2, int flag3) {
    (void)hudObj; (void)modeIndex; (void)param1; (void)param2; (void)flag1; (void)flag2; (void)flag3;
}
void hudFlashBase_0F08_g(void* hudObj, int modeIndex, float param1, float param2,
                         int flag1, int flag2, int flag3) {
    hudFlashBase_RenderElement(hudObj, modeIndex, param1, param2, flag1, flag2, flag3);
}
void hudFlashBase_DrawFlashOverlay(void* a, void* b, void* c, float d, int e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

namespace rage {
void hudFlashBase_ExecuteFromContext(void* a, void* b, void* c) { (void)a; (void)b; (void)c; }
} // namespace rage

// ── swf* free functions ─────────────────────────────────────────────────────
extern "C" {
void swfContext_Cleanup(void* context) { (void)context; }
void swfFile_DestroyResources(void* file) { (void)file; }
void swfInstance_Cleanup(void* instance) { (void)instance; }
float swfComputeFramePosition(void* a, void* b, float* c, float* d) {
    (void)a; (void)b; if (c) *c = 0.0f; if (d) *d = 0.0f; return 0.0f;
}
const char* swfInternString(const char* a, void* b, int c) {
    (void)b; (void)c; return a;
}
void swfPopResult(void* a) { (void)a; }
void swfSymtab_Delete(void* a, const char* b) { (void)a; (void)b; }
void swfSymtab_Enumerate(void* a) { (void)a; }
void swfSymtab_Insert(void* a, const char* b, void* c) { (void)a; (void)b; (void)c; }
void* swfSymtab_Lookup(void* a, const char* b) { (void)a; (void)b; return nullptr; }
} // extern "C"
