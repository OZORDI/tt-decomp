/**
 * rage_globals.cpp — RAGE Engine Global Variables
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * This file defines global variables used throughout the RAGE engine.
 * These are referenced from multiple subsystems and must have external linkage.
 */

#include <cstdint>

namespace rage {

// ============================================================================
// CM (Curve/Machine) System Globals
// ============================================================================

/**
 * g_cm_frameRate
 * 
 * Current frame rate for the CM system (frames per second).
 * Used by cmDifferential and other CM nodes for time-based calculations.
 */
float g_cm_frameRate = 60.0f;

/**
 * g_cm_frameTime
 * 
 * Current frame time for the CM system (seconds per frame).
 * Used by cmIntegrate, cmDifferential, and cmApproachOperator.
 */
float g_cm_frameTime = 1.0f / 60.0f;

/**
 * g_cm_zeroFloat
 * 
 * Constant zero float value used by CM arithmetic nodes.
 * Used by cmAdd, cmSubtract, cmNegate, cmAbs, cmSine, and others.
 */
float g_cm_zeroFloat = 0.0f;

/**
 * g_cm_negateVec
 * 
 * Vector used for negation operations in CM system.
 * Used by cmNegate::GetVector.
 */
float g_cm_negateVec[4] = {-1.0f, -1.0f, -1.0f, -1.0f};

// ============================================================================
// Graphics/Texture System Globals
// ============================================================================

/**
 * g_pTextureFactory
 * 
 * Pointer to the global texture factory instance.
 * Used by grcTextureReferenceBase for texture management.
 */
void* g_pTextureFactory = nullptr;

/**
 * g_bTextureEndianDirty
 * 
 * Flag indicating whether texture endianness needs to be corrected.
 * Xbox 360 is big-endian, modern platforms are little-endian.
 * Used by grcTextureReferenceBase::Serialize.
 */
bool g_bTextureEndianDirty = true;  // Always true on little-endian platforms

} // namespace rage
