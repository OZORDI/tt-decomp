/**
 * data.cpp — Player Property Manager & Camera Shot Attachment Data
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Two classes that manage game data:
 *
 *   plrPropMgr   — Manages player rendering properties (ball/paddle buffer sizes).
 *                  Inherits from `atSingleton`-style base with property registration.
 *                  Registered properties: "BallBufferSize", "PaddleBufferSize".
 *
 *   gdShotSet    — Camera shot attachment data: maps bone names to skeleton indices
 *                  for positioning cinematic camera angles on the player skeleton.
 *                  Uses `crSkeletonData` for runtime bone name resolution.
 */

#include "data/gd_data.hpp"
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// atSingleton base destructor body — tears down the singleton registration.
// @ 0x821A9420 | size: 0x7C
extern void atSingleton_Destroy(void* obj);

// RAGE heap deallocator.
// @ 0x820C00C0
extern void rage_free(void* ptr);

// gdDataMgr property registration — allocates a property node in the global
// data manager, keyed by name hash.  Stores (name → dest, default, flags).
// @ 0x821A8F58 | size: 0xC8
extern void gdDataMgr_RegisterProperty(void* obj, const char* name,
                                        void* dest, void* defaultVal, int flags);

// datResourceMgr asset loader — resolves an asset by its hash value.
// On failure, logs extensive debug info (8 fields from a resource info struct).
// Returns a pointer to the loaded resource, or nullptr if not found.
// @ 0x820F8FB0 | size: 0x11C
extern void* datResourceMgr_LoadAsset(void* assetHash, int flags);

// datResourceMgr entry removal — unregisters a previously loaded resource.
// @ 0x820F9188 (approx)
extern void datResourceMgr_RemoveEntry(void* asset);

// datResource release — deallocates a resource.
// If top bit of flags is set (0x80000000), calls the deferred-destroy path;
// otherwise calls the immediate-free path.
// @ 0x82566C20 | size: 0x1C
extern void datResource_Release(void* resource, uint32_t flags);

// Buffer allocator from the XAM buffer system.
// Allocates a typed buffer array of `count` elements.
// @ 0x820C8E30 | size: 0x68
extern void xe_AllocateBuffer(void* outPtr, uint32_t count);

// crSkeletonData bone name lookup — linear strcmp search through the
// skeleton's bone name array.  Returns the bone index (0-based) or
// -1 (0xFFFFFFFF) if not found.
//
// Algorithm:
//   for (i = 0; i < boneCount; i++):
//     if strcmp(boneArray[i]->name, searchName) == 0:
//       return i
//   return -1
//
// @ 0x821DDAF0 | size: 0x80
extern int32_t crSkeletonData_FindBoneByName(void* skeletonData, const char* boneName);

// Integer wrap — wraps `value` into the range [min, max] using modular arithmetic.
// Equivalent to: min + ((value - min) % (max - min + 1))
// @ 0x820DC4E8 | size: 0x58
extern int32_t math_WrapIndex(int32_t value, int32_t min, int32_t max);

// Debug logger — no-op in release builds (single `blr` instruction).
// @ 0x8240E6D0
extern void nop_DebugLog(const char* fmt, uint32_t arg);

// ─────────────────────────────────────────────────────────────────────────────
// Global data
// ─────────────────────────────────────────────────────────────────────────────

// Property ID comparison values — these are pre-computed hashes representing
// known valid property configurations.  PostLoadProperties checks the current
// property ID against these to validate the loaded data.
// @ 0x825C2D7C (.data, 52 bytes) — valid config hash A
extern uint32_t g_validPropConfigA;

// @ 0x825C93D0 (.data, 8 bytes) — valid config hash B
extern uint32_t g_validPropConfigB;

// Default value for property registration — fallback value (4 bytes).
// @ 0x825CAF90 (.data, 4 bytes)
extern uint32_t g_defaultPropValue;

// Global singleton instance pointer for plrPropMgr.
// Set during InitAssets, used by other systems to access the property manager.
// @ 0x826066C0 (.data, 4 bytes) — SDA offset 26304
extern uint32_t g_plrPropMgrInstance;

// Global skeleton dictionary pointer — used by gdShotSet for bone name
// resolution.  The skeleton data object at +36 contains the bone array.
// @ 0x8271A314 (.data, 4 bytes)
extern void* g_skeletonData;

// Sentinel value for invalid bone indices
static const uint32_t BONE_INDEX_INVALID = 0xFFFFFFFF;


// ═══════════════════════════════════════════════════════════════════════════════
//  plrPropMgr — Player Property Manager
//
//  RTTI name: plrPropMgr  [vtable @ 0x820717C4]
//
//  Manages two rendering properties:
//    - "BallBufferSize"   — vertex buffer size for ball rendering
//    - "PaddleBufferSize" — vertex buffer size for paddle rendering
//
//  Loads these from the game data configuration, resolves them to asset
//  resources via datResourceMgr, and stores a global singleton pointer.
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * plrPropMgr::~plrPropMgr  [vtable slot 0]
 * @ 0x823D45C0 | size: 0x68
 *
 * Destructor — releases all loaded assets, then calls the atSingleton
 * base destructor to unregister from the singleton system.
 */
plrPropMgr::~plrPropMgr()
{
    // Release all loaded assets
    ReleaseAssets();

    // Call atSingleton base destructor for cleanup
    atSingleton_Destroy(this);
}

/**
 * plrPropMgr::PostLoadProperties  [vtable slot 20]
 * @ 0x823D4630 | size: 0x34
 *
 * Validates the loaded property configuration by checking the property ID
 * against two known-valid configuration hashes.
 *
 * Returns 1 if the configuration is valid, 0 otherwise.
 */
uint8_t plrPropMgr::PostLoadProperties()
{
    if (m_propertyId == g_validPropConfigA) {
        return 1;
    }

    if (m_propertyId == g_validPropConfigB) {
        return 1;
    }

    return 0;
}

/**
 * plrPropMgr::Validate  [vtable slot 21]
 * @ 0x823D4730 | size: 0x68
 *
 * Registers two rendering properties with the global data manager:
 *   - "BallBufferSize"   → stored in m_prop1 (ball vertex buffer size)
 *   - "PaddleBufferSize" → stored in m_prop2 (paddle vertex buffer size)
 *
 * Both use g_defaultPropValue as their fallback if not found in config.
 */
void plrPropMgr::Validate()
{
    gdDataMgr_RegisterProperty(this, "BallBufferSize",
                                &m_prop1, &g_defaultPropValue, 0);

    gdDataMgr_RegisterProperty(this, "PaddleBufferSize",
                                &m_prop2, &g_defaultPropValue, 0);
}

/**
 * plrPropMgr::PostLoadChildren  [vtable slot 22]
 * @ 0x823D4668 | size: 0xC
 *
 * Returns the class identifier string "plrPropMgr" for the data manager's
 * child-node resolution system.
 */
void* plrPropMgr::PostLoadChildren()
{
    return (void*)"plrPropMgr";
}

/**
 * plrPropMgr::InitAssets  [vtable slot 23]
 * @ 0x823D47A0 | size: 0x54
 *
 * Loads the three asset resources:
 *   - m_assets[0] from ball buffer property (m_prop1)
 *   - m_assets[1] and m_assets[2] from paddle buffer property (m_prop2)
 *
 * After loading, stores `this` pointer into the global singleton variable
 * so other systems can access the property manager.
 */
void plrPropMgr::InitAssets()
{
    // Load ball buffer asset
    m_assets[0] = datResourceMgr_LoadAsset(m_prop1, 0);

    // Load paddle buffer assets (two slots share the same property)
    for (int i = 1; i < 3; i++) {
        m_assets[i] = datResourceMgr_LoadAsset(m_prop2, 0);
    }

    // Register as the global plrPropMgr singleton
    g_plrPropMgrInstance = (uint32_t)this;
}

/**
 * plrPropMgr::ReleaseAssets  [vtable slot 24]
 * @ 0x823D47F8 | size: 0x60
 *
 * Releases all three loaded assets:
 *   1. Removes the entry from datResourceMgr's registry
 *   2. Releases the resource memory via datResource_Release
 *      with flags 0xFFFFE001 (top bit set = deferred destroy, lower bits = type)
 *   3. Nulls out all asset pointers
 */
void plrPropMgr::ReleaseAssets()
{
    for (int i = 0; i < 3; i++) {
        void* asset = m_assets[i];
        if (asset != nullptr) {
            datResourceMgr_RemoveEntry(asset);
            datResource_Release(asset, 0xFFFFE001);
        }
    }

    m_assets[0] = nullptr;
    m_assets[1] = nullptr;
    m_assets[2] = nullptr;
}

/**
 * plrPropMgr::SetObjectFlags  [vtable slot 25]
 * @ 0x823D4860 | size: 0x40
 *
 * Sets rendering flags on the two related render objects:
 *   +288 (0x120) = renderEnabled flag
 *   +290 (0x122) = shadowEnabled flag
 *   +291 (0x123) = reflectionEnabled flag
 *
 * These enable the ball and paddle to be rendered with shadows and
 * reflections in the scene.
 */
void plrPropMgr::SetObjectFlags()
{
    if (m_relatedObj1 != nullptr) {
        uint8_t* obj = (uint8_t*)m_relatedObj1;
        obj[288] = 1;  // renderEnabled
        obj[290] = 1;  // shadowEnabled
        obj[291] = 1;  // reflectionEnabled
    }

    if (m_relatedObj2 != nullptr) {
        uint8_t* obj = (uint8_t*)m_relatedObj2;
        obj[288] = 1;  // renderEnabled
        obj[290] = 1;  // shadowEnabled
        obj[291] = 1;  // reflectionEnabled
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  gdShotSet — Camera Shot Attachment Data
//
//  Manages the mapping between bone names and skeleton indices for
//  cinematic camera positioning.  Each "shot" attachment references a bone
//  on the player skeleton (e.g., head, hand, chest) that the camera
//  system uses as an anchor point.
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * gdShotSet::PostLoadChildren  [vtable slot 22]
 * @ 0x821DCF88 | size: 0xC
 *
 * Returns the identifier string for attachment data nodes.
 */
const char* gdShotSet::PostLoadChildren()
{
    return "AttachmentData";
}

/**
 * gdShotSet::GetAttachmentCount  [vtable slot 23]
 * @ 0x821DD1D8 | size: 0x8
 */
uint32_t gdShotSet::GetAttachmentCount()
{
    return m_attachmentCount;
}

/**
 * gdShotSet::GetResolvedBoneValue  [vtable slot 24]
 * @ 0x821DD1E0 | size: 0x34
 *
 * Retrieves the resolved bone data from the global skeleton dictionary.
 * Returns 0 if the bone index is invalid (BONE_INDEX_INVALID).
 *
 * Skeleton data layout:
 *   skeleton+36 → bone data pointer array (each entry is a bone object)
 */
uint32_t gdShotSet::GetResolvedBoneValue(uint32_t index)
{
    uint32_t boneIndex = m_pBoneIndices[index];
    if (boneIndex == BONE_INDEX_INVALID) {
        return 0;
    }

    uint32_t* skeletonDict = (uint32_t*)g_skeletonData;
    uint32_t* boneArray = (uint32_t*)skeletonDict[9];  // +36: bone data array
    return boneArray[boneIndex];
}

/**
 * gdShotSet::GetBoneIndex  [vtable slot 25]
 * @ 0x821DD218 | size: 0xC
 */
uint32_t gdShotSet::GetBoneIndex(uint32_t index)
{
    return m_pBoneIndices[index];
}

/**
 * gdShotSet::GetBoneName  [vtable slot 26]
 * @ 0x821DD228 | size: 0x3C
 *
 * Retrieves the bone name pointer for a given attachment index.
 * If the bone index is valid, reads from the global skeleton's bone data.
 * If invalid (not yet resolved), falls back to the attachment's stored name.
 *
 * Bone object layout: +16 = bone name string pointer
 */
void* gdShotSet::GetBoneName(uint32_t index)
{
    uint32_t boneIndex = m_pBoneIndices[index];
    if (boneIndex == BONE_INDEX_INVALID) {
        // Bone not yet resolved — read name from attachment data
        uint32_t* attachment = (uint32_t*)m_pAttachments[index];
        return (void*)attachment[4];  // +16: bone name string
    }

    // Resolved — read name from the global skeleton dictionary
    uint32_t* skeletonDict = (uint32_t*)g_skeletonData;
    uint32_t** bonePtrArray = (uint32_t**)skeletonDict[9];  // +36
    uint32_t* boneObj = bonePtrArray[boneIndex];
    return (void*)boneObj[4];  // +16: bone name string
}

/**
 * gdShotSet::GetCurrentBoneIndex  [vtable slot 27]
 * @ 0x821DD268 | size: 0x14
 */
uint32_t gdShotSet::GetCurrentBoneIndex()
{
    return GetBoneIndex(m_currentIdx);
}

/**
 * gdShotSet::GetCurrentBoneName  [vtable slot 28]
 * @ 0x821DD280 | size: 0x14
 */
void* gdShotSet::GetCurrentBoneName()
{
    return GetBoneName(m_currentIdx);
}

/**
 * gdShotSet::SetCurrentIndex  [vtable slot 29]
 * @ 0x821DD318 | size: 0x8
 */
void gdShotSet::SetCurrentIndex(uint32_t idx)
{
    m_currentIdx = idx;
}

/**
 * gdShotSet::ResolveBoneNames  [vtable slot 30]
 * @ 0x821DD320 | size: 0xA4
 *
 * Resolves each attachment's bone name string to a skeleton bone index
 * using the global skeleton dictionary.  Called after a creature is loaded.
 *
 * For each attachment:
 *   1. Reads the bone name from attachment+16
 *   2. Calls crSkeletonData_FindBoneByName to look up the index
 *   3. Stores the resolved index in m_pBoneIndices
 *   4. If the bone was NOT found (-1), logs a warning:
 *      "gdCrAttachmentData::PostLoadCreature() - bone '%s' does not exist"
 */
void gdShotSet::ResolveBoneNames()
{
    // Allocate the bone index array
    xe_AllocateBuffer(&m_pBoneIndices, m_attachmentCount);

    if (m_attachmentCount == 0) {
        return;
    }

    for (uint32_t i = 0; i < m_attachmentCount; i++) {
        uint32_t* attachment = (uint32_t*)m_pAttachments[i];
        const char* boneName = (const char*)attachment[4];  // +16: bone name

        // Look up bone index by name in the global skeleton
        int32_t resolvedIndex = crSkeletonData_FindBoneByName(g_skeletonData, boneName);

        // Store resolved index
        uint16_t currentCount = m_boneIndexCount;
        m_boneIndexCount = currentCount + 1;
        m_pBoneIndices[currentCount] = (uint32_t)resolvedIndex;

        // Log warning if bone was not found
        if (m_pBoneIndices[i] == BONE_INDEX_INVALID) {
            const char* failedBoneName = (const char*)((uint32_t*)m_pAttachments[i])[4];
            nop_DebugLog(
                "gdCrAttachmentData::PostLoadCreature() - bone '%s' does not exist",
                (uint32_t)failedBoneName);
        }
    }
}

/**
 * gdShotSet::AdvanceIndex  [vtable slot 31]
 * @ 0x821DD3C8 | size: 0x38
 *
 * Advances to the next attachment, wrapping around using modular arithmetic.
 * Only advances if the shot set is currently active.
 * Returns true if active (index was advanced or could be advanced).
 */
bool gdShotSet::AdvanceIndex()
{
    if (m_bIsActive) {
        m_currentIdx = (uint32_t)math_WrapIndex(
            m_currentIdx + 1, 0, m_boneIndexCount - 1);
    }
    return m_bIsActive;
}
