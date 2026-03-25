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
#include <new>  // For placement new

// External functions
extern "C" {
    void rage_free(void* ptr);                  // @ 0x820C00C0 — canonical heap free (see src/crt/heap.c)
    void swfContext_Cleanup(void* ctx);         // @ 0x823F9B80 — cleanup stub (no-op in final binary)
    uint8_t atSingleton_Find(void* ptr);        // @ 0x820F90D0 — singleton registry lookup, returns nonzero if found
    void atHashMap_Clear(void* ptr);            // @ 0x8234C0E0 — iterates linked-list hash buckets, frees all nodes
    void swfFile_DestroyResources(void* ptr);   // @ 0x824035B0 — releases child resource array entries
    void swfInstance_Cleanup(void* ptr);        // @ 0x823FB670 — unlinks instance from display list
}

// Global SWF object pools (SDA - Small Data Area)
extern void* g_swfScriptObjectPool;  // @ SDA offset 19540
extern void* g_swfInstancePool;      // @ SDA offset 19536
extern void* g_swfScriptArrayPool;   // @ SDA offset 19552

namespace rage {

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
    extern "C" void swfSymtab_Enumerate(void* symtab);
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
void swfSCRIPTOBJECT::VisitMembers() {
    // No-op
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
    extern "C" const char* swfInternString(const char* name, void* buffer, int maxLen);
    extern "C" void* swfSymtab_Lookup(void* symtab, const char* key);
    extern "C" void nop_8240E6D0(const char* msg, ...);

    // Intern the property name
    const char* interned = swfInternString(name, &g_swfStringBuffer, 1024);

    // Special case: "this" — return self as object
    if (strcmp(interned, "this") == 0) {
        *(void**)result = this;
        *((uint32_t*)result + 1) = 5;  // type 5 = object
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
        *((uint32_t*)result + 1) = 5;  // type 5 = object
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
    extern "C" const char* swfInternString(const char* name, void* buffer, int maxLen);
    extern "C" void swfSymtab_Insert(void* symtab, const char* key, void* value);

    const char* internedName = swfInternString(name, &g_swfStringBuffer, 1024);
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
    extern "C" void swfSymtab_Delete(void* symtab, const char* key);

    // Build a stack pair: {name_ptr, type=1 (undefined)}
    struct { uint32_t nameRef; uint32_t type; } undefinedPair;
    undefinedPair.nameRef = (uint32_t)(uintptr_t)name;
    undefinedPair.type = 1;

    // Notify via GetMember (vtable slot 10)
    typedef bool (*GetMemberFunc)(void*, void*, void*);
    void** vtable = *(void***)this;
    GetMemberFunc getMember = (GetMemberFunc)vtable[10];
    getMember(this, &undefinedPair, &undefinedPair);

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
    extern "C" void nop_8240E6D0(const char* msg, ...);

    // Set up GetMember call: lookupKey = {methodName, type=7 (string)}
    // lookupResult = {0, 0} (will be filled in)
    struct swfValue { uint32_t data; uint32_t type; };
    swfValue lookupKey = { (uint32_t)(uintptr_t)methodName, 7 };
    swfValue lookupResult = { 0, 0 };

    // Call GetMember (vtable slot 10)
    typedef bool (*GetMemberFunc)(void*, swfValue*, swfValue*);
    void** vtable = *(void***)this;
    GetMemberFunc getMember = (GetMemberFunc)vtable[10];
    bool found = getMember(this, &lookupKey, &lookupResult);

    if (!found) {
        // Method not found
        nop_8240E6D0("Unsupported script method: %s", methodName);
        ((swfValue*)outResult)->data = 0;
        ((swfValue*)outResult)->type = 3;  // type 3 = undefined
        return;
    }

    // Initialize result to {0, type=5}
    ((swfValue*)outResult)->data = 0;
    ((swfValue*)outResult)->type = 5;

    if (lookupResult.type == 6) {
        // Type 6: native function pointer — call directly
        typedef void (*NativeFunc)(void*, void*, int, void*);
        NativeFunc fn = (NativeFunc)(uintptr_t)lookupResult.data;
        fn(this, args, argCount, outResult);
    } else if (lookupResult.type == 5) {
        // Type 5: script object — call its Execute method (vtable slot 14)
        void* callableObj = (void*)(uintptr_t)lookupResult.data;

        extern uint32_t g_swfCallDepth;  // @ 0x826064EC
        uint32_t prevDepth = g_swfCallDepth;

        typedef void (*ExecuteFunc)(void*, void*, int, void*, int);
        void** objVtable = *(void***)callableObj;
        ExecuteFunc execute = (ExecuteFunc)objVtable[14];
        execute(callableObj, args, argCount, this, 0);

        // If call stack depth increased, a return value was pushed
        if ((int32_t)g_swfCallDepth > (int32_t)prevDepth) {
            extern "C" void swfPopResult(void* dst);
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

void swfCONTEXT::vfn_2() {
    // External function declaration
    extern void hudFlashBase_9CA8_h(void* flashObj, void* param1, void* param2);  // @ 0x823F9CA8
    extern void* g_currentSwfContext;  // @ 0x8260281C (SDA offset 10268)
    
    // Store this context as the current global context
    g_currentSwfContext = this;
    
    // Call hudFlashBase processing with internal state
    // Offsets: +24 (flashObj), +28 (param1), +32 (param2)
    void* flashObj = *((void**)((char*)this + 24));
    void* param1 = *((void**)((char*)this + 28));
    void* param2 = *((void**)((char*)this + 32));
    
    hudFlashBase_9CA8_h(flashObj, param1, param2);
    
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
    
    // Destroy all child resources
    if (m_resourceCount > 0) {
        for (int i = 0; i < m_resourceCount; i++) {
            void* resource = m_pResourceArray[i];
            if (resource) {
                // Call virtual destructor (slot 0) with flags=1
                void** vtable = *((void***)resource);
                typedef void (*DestructorFn)(void*, int);
                ((DestructorFn)vtable[0])(resource, 1);
            }
        }
    }
    
    // Free the resource array itself
    if (m_pResourceArray) {
        rage_free(m_pResourceArray);
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
    // Call cleanup helper (likely frees event handler list)
    extern void atSingleton_8068_h(void* ptr);
    atSingleton_8068_h(this);
}

void swfCMD_PlaceObject2ClipEvent_ScalarDestructor(swfCMD_PlaceObject2ClipEvent* obj, int flags) {
    // Call destructor
    obj->~swfCMD_PlaceObject2ClipEvent();
    
    if (flags & 0x1) {
        rage_free(obj);
    }
}


/**
 * swfCMD_DoInitAction::vfn_2() @ 0x821EFC00 | size: 0x10
 * 
 * Virtual function slot 2 - tail calls to scalar destructor (slot 1).
 * This is a common RAGE pattern for cleanup functions.
 */
void swfCMD_DoInitAction::vfn_2() {
    // Load vtable and call slot 1 (scalar destructor)
    void** vtable = *((void***)this);
    typedef void (*ScalarDtorFn)(void*, int);
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
    extern "C" int _stricmp(const char*, const char*);
    extern "C" void nop_8240E6D0(const char* msg, ...);
    extern "C" float swfComputeFramePosition(void* ctx, void* entry, float* outMin, float* outMax);

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
    nop_8240E6D0("Couldn't find frame label '%s'", labelName);
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

// Virtual function stubs for swfINSTANCE
void swfINSTANCE::vfn_1() { /* TODO */ }
void swfINSTANCE::vfn_2() { /* TODO */ }
void swfINSTANCE::vfn_3() { /* TODO */ }
void swfINSTANCE::vfn_4() { /* TODO */ }
void swfINSTANCE::vfn_5() { /* TODO */ }
void swfINSTANCE::vfn_6() { /* TODO */ }
void swfINSTANCE::vfn_7() { /* TODO */ }
void swfINSTANCE::vfn_8() { /* TODO */ }
void swfINSTANCE::vfn_9() { /* TODO */ }

/**
 * swfACTIONFUNC::vfn_11() @ 0x823FF4A0 | size: 0x14
 * 
 * Virtual function slot 11 - forwards to inner object's vfn_11.
 * The inner object is stored at offset +7332.
 */
void swfACTIONFUNC::vfn_11() {
    // Load inner object at offset +7332
    void* innerObj = *((void**)((char*)this + 7332));
    
    if (innerObj) {
        // Call its virtual method at slot 11
        void** vtable = *((void***)innerObj);
        typedef void (*VirtualFn)(void*);
        ((VirtualFn)vtable[11])(innerObj);
    }
}

/**
 * swfACTIONFUNC::vfn_12() @ 0x823FF4B8 | size: 0x14
 * 
 * Virtual function slot 12 - forwards to inner object's vfn_12.
 */
void swfACTIONFUNC::vfn_12() {
    // Load inner object at offset +7332
    void* innerObj = *((void**)((char*)this + 7332));
    
    if (innerObj) {
        // Call its virtual method at slot 12
        void** vtable = *((void***)innerObj);
        typedef void (*VirtualFn)(void*);
        ((VirtualFn)vtable[12])(innerObj);
    }
}


void swfINSTANCE::vfn_10() { /* TODO */ }
void swfINSTANCE::vfn_11() { /* TODO */ }
void swfINSTANCE::vfn_12() { /* TODO */ }
void swfINSTANCE::vfn_13() { /* TODO */ }


// ===========================================================================
// ===========================================================================
// swfACTIONFUNC — ActionScript function object
// ===========================================================================

// Implementations moved above - see lines 469-519

void swfACTIONFUNC::vfn_8() { /* TODO */ }
void swfACTIONFUNC::vfn_9() { /* TODO */ }
void swfACTIONFUNC::vfn_10() { /* TODO */ }


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



// swfSCRIPTARRAY overrides GetMember/SetMember for array-style access
void swfSCRIPTARRAY::GetMember() { /* TODO: array-indexed GetMember */ }
void swfSCRIPTARRAY::SetMember() { /* TODO: array-indexed SetMember */ }


// ===========================================================================
// swfOBJECT and derived display object classes
// ===========================================================================

swfOBJECT::~swfOBJECT() { /* TODO */ }
swfSPRITE::~swfSPRITE() { /* TODO */ }

// ===========================================================================
// swfCONTEXT Additional Methods
// ===========================================================================

/**
 * swfCONTEXT_B1F0_w @ 0x823FB1F0 | size: 0x64
 * 
 * Checks if a specific condition is met by comparing byte values.
 * Returns true if the XOR of two bytes exceeds 127, false otherwise.
 * This appears to be a validation or state check function.
 */
bool swfCONTEXT_B1F0_w() {
    // External declarations
    extern void* util_B188(void* obj, int param);  // @ 0x823FB188 - returns pointer
    extern void* g_swfGlobalObject;  // @ 0x82604848 (SDA offset 18504)
    
    // Call utility function with parameter 13
    uint8_t* result = (uint8_t*)util_B188(g_swfGlobalObject, 13);
    
    // Load bytes at offsets +0 and +3
    uint8_t byte0 = result[0];
    uint8_t byte3 = result[3];
    
    // XOR the bytes and check if result > 127
    uint8_t xorResult = byte0 ^ byte3;
    return (xorResult > 127);
}


/**
 * swfCONTEXT_CA50_2hr @ 0x823FCA50 | size: 0x118
 * 
 * Recursive context processing function. Checks context state flags,
 * calls virtual methods, and manages a context stack. Recursively
 * processes child contexts in a linked list.
 */
void swfCONTEXT_CA50_2hr(swfCONTEXT* ctx) {
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
    
    // Call virtual method slot 6 on object at offset +4
    uint8_t callResult = 0;
    if (shouldCallVirtual) {
        void* obj = *((void**)((char*)ctx + 4));
        void** vtable = *((void***)obj);
        typedef uint8_t (*VirtualFn)(void*);
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
        // If this is the current global context, call virtual method slot 3
        if (g_currentSwfContext == ctx) {
            void* obj = *((void**)((char*)ctx + 4));
            void** vtable = *((void***)obj);
            typedef void (*VirtualFn3)(void*, void*, int, int);
            ((VirtualFn3)vtable[3])(obj, ctx, 2, 1);
            
            // Clear global context
            g_currentSwfContext = nullptr;
        }
    }
    
    // Recursively process child contexts (linked list at offset +180/+184)
    void* childCtx = *((void**)((char*)ctx + 180));
    while (childCtx != nullptr) {
        swfCONTEXT_CA50_2hr((swfCONTEXT*)childCtx);
        childCtx = *((void**)((char*)childCtx + 184));
    }
}


} // namespace rage
