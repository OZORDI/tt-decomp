/**
 * holder.cpp — Holder Template Class Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implementation of the Holder class for game parameter/fragment management.
 * Manages tuning data, float parameters, and vector/matrix data for game
 * fragments loaded from the tuning system ($/tune/types/fragments).
 *
 * The class has 6 template specializations, each with its own vtable and
 * singleton instance (28 bytes each at g_HolderSingletons @ 0x825D4560).
 *
 * Functions implemented:
 *   GetSingleton           — returns per-specialization singleton pointer
 *   AllocateAndInitialize  — TLS allocator + AltiVec copy
 *   Constructor            — initializes all fields to defaults
 *   SetParam 1-3, 8-10    — vtable-dispatched float extraction
 *   SetInitializedFlag     — sets bit 0 of init flags
 *   DispatchVtableSlot55   — tail-call to large vtable slot 55
 */

#include "holder.h"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// TLS memory allocator initialization — ensures the per-thread allocator
// table is set up before allocation.
// @ 0x820C0XXX (sysMemAllocator)
extern "C" void sysMemAllocator_InitMainThread();

// ─────────────────────────────────────────────────────────────────────────────
// External globals
// ─────────────────────────────────────────────────────────────────────────────

// TLS base pointer — r13 SDA base, points to the thread-local storage root.
// @ 0x82600000
extern void** g_tls_base;

// Holder vtable (primary, 20 bytes = 5 slots).
// @ 0x82032E34 | RTTI-confirmed: Holder
extern void* g_Holder_vtable[];

// Default constant data — 64 bytes of identity/default transform values.
// Located adjacent to rage::spdApical vtable in .rdata (0x82032F48).
// @ 0x82032F48
extern uint8_t g_Holder_defaultTransformData[64];

// Global singleton array — 6 specializations, 28 bytes each.
// @ 0x825D4560
uint8_t g_HolderSingletons[168] __attribute__((aligned(16)));


// ═══════════════════════════════════════════════════════════════════════════════
//  Holder::GetSingleton  [vtable slot 1]
//
//  @ 0x82121BF8 | size: 0x0C
//
//  Returns the global singleton instance for this Holder specialization.
//  There are 6 variants, each returning a different 28-byte entry:
//    vfn_1:         base + 0x00  (0x825D4560)
//    vfn_1_1C98_1:  base + 0x1C  (0x825D457C)
//    vfn_1_1D30_1:  base + 0x38  (0x825D4598)
//    vfn_1_1DC8_1:  base + 0x54  (0x825D45B4)
//    vfn_1_1E60_1:  base + 0x70  (0x825D45EC)
//    vfn_1_AC90_1:  base + 0x8C  (0x825D45EC)
// ═══════════════════════════════════════════════════════════════════════════════
void* Holder::GetSingleton()
{
    return &g_HolderSingletons[0];
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Holder::AllocateAndInitialize  [vtable slot 2]
//
//  @ 0x82121C08 | size: 0x8C
//
//  Allocates a 32-byte, 16-byte-aligned object via the TLS memory allocator,
//  sets its vtable to g_Holder_vtable, and copies the 16-byte default vector
//  from this+0x10 to the new object+0x10 (using AltiVec on original hardware).
//
//  Returns: Pointer to new Holder, or nullptr on allocation failure.
// ═══════════════════════════════════════════════════════════════════════════════
void* Holder::AllocateAndInitialize()
{
    sysMemAllocator_InitMainThread();

    // Get allocator from TLS — r13+0 → allocator table, entry at index 1
    void** pTLS = *(void***)g_tls_base;
    void*  pAllocator = pTLS[1];

    // Allocate 32 bytes with 16-byte alignment via allocator vtable slot 1
    typedef void* (*AllocFn)(void*, uint32_t size, uint32_t alignment);
    void** allocVtable = *(void***)pAllocator;
    AllocFn alloc = reinterpret_cast<AllocFn>(allocVtable[1]);
    void* newObj = alloc(pAllocator, 32, 16);

    if (newObj != nullptr) {
        // Set vtable pointer
        *static_cast<void**>(newObj) = g_Holder_vtable;

        // Copy 16-byte default vector from this+0x10 to new+0x10
        // Original uses AltiVec lvx/stvx for aligned 16-byte copy
        memcpy(static_cast<char*>(newObj) + 0x10,
               reinterpret_cast<char*>(this) + 0x10, 16);
    }

    return newObj;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Holder::Holder (Constructor)
//
//  @ 0x82122798 | size: 0x11C (284 bytes)
//
//  Initializes a Holder instance with default values for fragment/tuning
//  management.  Called when creating fragment type holders for game resources
//  and tuning data (see debug string: "$/tune/types/fragments").
//
//  Initialization sequence:
//    1. Default vector → (0,0,0,0)
//    2. Constant transform data → copy from g_Holder_defaultTransformData
//    3. Tuning params [0..7] → 0.0f
//    4. Transform cache (96 bytes) → zeroed (AltiVec vxor + stvx loop)
//    5. Animation state (128 bytes) → zeroed (AltiVec vxor + stvx loop)
//    6. Extended params → 0.0f
//    7. State/status flags → 0
//
//  NOTE: m_tuningParam1/2/3 and m_tuningParam8/9/10 are NOT initialized
//  here — they are set later via SetParam methods which call external
//  vtable slot 5 on a source object.
// ═══════════════════════════════════════════════════════════════════════════════
Holder::Holder()
{
    // Initialize default identity vector to (0,0,0,0) at +0x10
    m_defaultVectorX = 0.0f;
    m_defaultVectorY = 0.0f;
    m_defaultVectorZ = 0.0f;
    m_defaultVectorW = 0.0f;

    // Load constant transform data from .rdata @ 0x82032F48
    // Original uses lvx128 to load 4 × 16-byte vectors (64 bytes total)
    memcpy(m_constantVectors, g_Holder_defaultTransformData,
           sizeof(m_constantVectors));

    // Initialize tuning parameter array to 0.0f
    for (int i = 0; i < 8; i++) {
        m_tuningParams[i] = 0.0f;
    }

    // Clear transform cache (6 × vec4 = 96 bytes)
    // Original: AltiVec vxor v0,v0,v0 + stvx loop
    memset(m_transformCache, 0, sizeof(m_transformCache));

    // Clear animation state (8 × vec4 = 128 bytes)
    // Original: AltiVec vxor v12,v12,v12 + stvx loop
    memset(m_animationState, 0, sizeof(m_animationState));

    // Initialize extended parameters to 0.0f
    m_extendedParam0 = 0.0f;
    m_extendedParam1 = 0.0f;
    m_extendedParam2 = 0.0f;
    // gap at +0x29C (likely int/enum field, not initialized)
    m_extendedParam3 = 0.0f;
    m_extendedParam4 = 0.0f;
    m_extendedParam5 = 0.0f;
    // gap at +0x2AC (likely int/enum field, not initialized)
    m_extendedParam6 = 0.0f;

    // Initialize state flags to 0
    m_stateFlags  = 0;
    m_statusFlags = 0;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  SetParam1..3, SetParam8..10
//
//  @ 0x82123488..0x821235C8 | size: 0x40 each
//
//  Each method extracts a float from a source object by:
//    1. Loading the object pointer from source+4
//    2. Calling that object's vtable slot 5 (which returns a float in f1)
//    3. Storing the result in the corresponding member field
//
//  This is the RAGE tuning system's way of extracting typed parameter values
//  from fragment descriptor objects.
// ═══════════════════════════════════════════════════════════════════════════════

// Helper: Extract float from source via vtable slot 5
static float ExtractFloatParam(void* source)
{
    void* obj = *reinterpret_cast<void**>(static_cast<char*>(source) + 4);
    typedef float (*GetFloatFn)(void*);
    void** vtable = *reinterpret_cast<void***>(obj);
    return reinterpret_cast<GetFloatFn>(vtable[5])(obj);
}

void Holder::SetParam1(void* source)  { m_tuningParam1  = ExtractFloatParam(source); }  // @ 0x82123488
void Holder::SetParam2(void* source)  { m_tuningParam2  = ExtractFloatParam(source); }  // @ 0x821234C8
void Holder::SetParam3(void* source)  { m_tuningParam3  = ExtractFloatParam(source); }  // @ 0x82123508
void Holder::SetParam8(void* source)  { m_tuningParam8  = ExtractFloatParam(source); }  // @ 0x82123548
void Holder::SetParam9(void* source)  { m_tuningParam9  = ExtractFloatParam(source); }  // @ 0x82123588
void Holder::SetParam10(void* source) { m_tuningParam10 = ExtractFloatParam(source); }  // @ 0x821235C8


// ═══════════════════════════════════════════════════════════════════════════════
//  Holder::SetInitializedFlag
//
//  @ 0x82123478 | size: 0x10
//
//  Sets bit 0 of the initialization flags byte at +0x99.
//  Marks the Holder as initialized and ready for use.
// ═══════════════════════════════════════════════════════════════════════════════
void Holder::SetInitializedFlag()
{
    m_initFlags |= 0x01;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Holder::DispatchVtableSlot55
//
//  @ 0x82122BF0 | size: 0x10
//
//  Tail-calls vtable slot 55 (byte offset +220).  This indicates the
//  Holder class has a LARGE vtable with at least 56 slots — likely due
//  to virtual-base MI with many inherited virtual methods.
// ═══════════════════════════════════════════════════════════════════════════════
void Holder::DispatchVtableSlot55()
{
    typedef void (*VFunc)(void*);
    void** vtable = *reinterpret_cast<void***>(this);
    VFunc fn = reinterpret_cast<VFunc>(vtable[55]);
    fn(this);
}
