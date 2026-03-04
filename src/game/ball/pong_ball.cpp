// pong_ball.cpp — Ball / robot classes | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * gm_ball_robot.cpp — Ball-serving robot implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "pong_ball.hpp"
#include "rage/memory.h"

// External function declarations
extern void audControl_Destructor(void* obj);  // @ 0x82161568
extern void* xe_EC88(uint32_t size);

namespace rage {
    void  ReleaseSingleton(void* singleton);         // @ 0x821A9420
    void  BindObject(void* obj1, void* obj2, uint32_t param3, uint32_t param4);
}
extern void* mfMotionClipRAGE_B8D8_g(void* clip, uint32_t param2, uint32_t param3, uint32_t param4);

// Global pointers
extern void* g_render_obj_ptr;

namespace {

constexpr uint16_t kEventRobotInit = 2071;
constexpr uint16_t kEventRobotDeactivate = 37;

constexpr uintptr_t kRobotVtablePrimary = 0x82032338u;
constexpr uintptr_t kRobotVtableSecondary = 0x82032344u;
constexpr uintptr_t kSingletonVtablePrimary = 0x820176C4u;
constexpr uintptr_t kSingletonVtableSecondary = 0x82017B34u;
constexpr uintptr_t kAudControlVtable = 0x82035594u;
constexpr uint32_t kCrAnimFrameVtable = 0x8204FCBCu;

constexpr uintptr_t kCharacterLookupKey = 0x8206A770u;
constexpr uintptr_t kAttachmentLookupKey = 0x82032318u;
constexpr uintptr_t kMotionClipLookupKey = 0x82032328u;

struct CrAnimFrameStub {
    uint32_t vtable;
    uint32_t field_0x04;
    uint32_t field_0x08;
    uint16_t field_0x0C;
    uint16_t field_0x0E;
};

int StringCompare(const char* lhs, const char* rhs) {
    if (lhs == rhs) {
        return 0;
    }
    if (lhs == nullptr || rhs == nullptr) {
        return 1;
    }

    while (*lhs != 0 && *lhs == *rhs) {
        ++lhs;
        ++rhs;
    }

    return static_cast<int>(static_cast<unsigned char>(*lhs)) -
           static_cast<int>(static_cast<unsigned char>(*rhs));
}

uint8_t* FindCharacterNode(uint8_t* node, const char* lookupName) {
    while (node != nullptr) {
        const char* candidate = *reinterpret_cast<const char**>(node + 20u);
        if (StringCompare(lookupName, candidate) == 0) {
            return node;
        }
        node = *reinterpret_cast<uint8_t**>(node + 184u);
    }
    return nullptr;
}

void* FindAttachmentObject(uint8_t* node, const char* lookupName) {
    while (node != nullptr) {
        uint8_t* attachment = *reinterpret_cast<uint8_t**>(node + 0u);
        if (attachment != nullptr) {
            const char* candidate = *reinterpret_cast<const char**>(attachment + 32u);
            if (StringCompare(lookupName, candidate) == 0) {
                return attachment;
            }
        }
        node = *reinterpret_cast<uint8_t**>(node + 128u);
    }
    return nullptr;
}

} // namespace

/**
 * gmBallRobot::~gmBallRobot() @ 0x821120A8 | size: 0xB8
 *
 * Destructor for ball robot.
 * Cleans up owned object, member audio control, and singleton registration.
 */
gmBallRobot::~gmBallRobot() {
    if (m_pOwnedObject != nullptr) {
        void** vtable = *(void***)m_pOwnedObject;
        void (*destructor)(void*, int) = reinterpret_cast<void (*)(void*, int)>(vtable[0]);
        destructor(m_pOwnedObject, 1);
    }

    m_vtable1 = reinterpret_cast<void*>(kRobotVtablePrimary);
    m_vtable2 = reinterpret_cast<void*>(kRobotVtableSecondary);

    *reinterpret_cast<void**>(&m_audControl3d[0]) = reinterpret_cast<void*>(kAudControlVtable);
    audControl_Destructor(&m_audControl3d[0]);
    rage::ReleaseSingleton(&m_singletonData[0]);

    m_vtable2 = reinterpret_cast<void*>(kSingletonVtableSecondary);
    m_vtable1 = reinterpret_cast<void*>(kSingletonVtablePrimary);
}

/**
 * gmBallRobot::HandleEvent() @ 0x82112160 | size: 0x20C
 *
 * Event handler for ball robot state changes.
 *
 * Event types:
 *   2071 (0x817): Initialize/activate robot
 *   37 (0x25): Deactivate robot
 */
void gmBallRobot::HandleEvent(uint16_t eventType) {
    if (eventType == kEventRobotInit) {
        auto* renderObj = reinterpret_cast<uint8_t*>(g_render_obj_ptr);
        if (renderObj == nullptr || m_pCharacter != nullptr) {
            return;
        }

        const char* characterLookupName =
            reinterpret_cast<const char*>(static_cast<uintptr_t>(kCharacterLookupKey));
        auto* characterNode = *reinterpret_cast<uint8_t**>(renderObj + 20u);
        characterNode = FindCharacterNode(characterNode, characterLookupName);
        m_pCharacter = characterNode;
        if (m_pCharacter == nullptr) {
            return;
        }

        m_pAnimationData = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(m_pCharacter) + 28u);

        const char* attachmentLookupName =
            reinterpret_cast<const char*>(static_cast<uintptr_t>(kAttachmentLookupKey));
        auto* attachmentNode = *reinterpret_cast<uint8_t**>(reinterpret_cast<uint8_t*>(m_pCharacter) + 148u);
        m_pMotionClip = FindAttachmentObject(attachmentNode, attachmentLookupName);

        m_pAllocatedObject = xe_EC88(16u);
        if (m_pAllocatedObject != nullptr) {
            auto* frame = reinterpret_cast<CrAnimFrameStub*>(m_pAllocatedObject);
            frame->vtable = kCrAnimFrameVtable;
            frame->field_0x04 = 0u;
            frame->field_0x08 = 0u;
            frame->field_0x0C = 0u;
            frame->field_0x0E = 0u;
        }

        rage::BindObject(m_pMotionClip, m_pAllocatedObject, 0u, 1u);

        void* motionClip = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(m_pAnimationData) + 4u);
        void* motionClipData = mfMotionClipRAGE_B8D8_g(
            motionClip,
            static_cast<uint32_t>(kMotionClipLookupKey),
            0u,
            0u);

        const uint32_t boneCount = static_cast<uint32_t>(
            *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(motionClipData) + 20u));
        const uint32_t animBufferBase1 =
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(m_pAnimationData) + 12u);
        const uint32_t animBufferBase2 =
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(m_pAnimationData) + 20u);

        const uint32_t offset1 = boneCount * 80u;
        const uint32_t offset2 = boneCount * 64u;

        m_pAnimBuffer1 = reinterpret_cast<void*>(static_cast<uintptr_t>(animBufferBase1 + offset1));
        m_pAnimBuffer2 = reinterpret_cast<void*>(static_cast<uintptr_t>(animBufferBase2 + offset2));
        return;
    }

    if (eventType == kEventRobotDeactivate) {
        m_isActive = 0;
    }
}

/**
 * gmBallRobot::DestructorThunk() @ 0x821129C8 | size: 0x8
 *
 * Thunk for secondary base class destructor.
 * Adjusts this pointer by -4 bytes for multiple inheritance.
 */
void gmBallRobot::DestructorThunk(gmBallRobot* ptr) {
    gmBallRobot* adjusted = reinterpret_cast<gmBallRobot*>(reinterpret_cast<uint8_t*>(ptr) - 4u);
    adjusted->~gmBallRobot();
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance vtable implementations
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongBallInstance::~pongBallInstance() @ 0x8227F990 | size: 0x98
 *
 * Destructor for ball instance.
 * Cleans up owned audio object and releases singleton resources.
 */
pongBallInstance::~pongBallInstance() {
    // Set primary and secondary vtable pointers
    constexpr uintptr_t kBallInstanceVtable1 = 0x8205700Cu;
    constexpr uintptr_t kBallInstanceVtable2 = 0x82057094u;
    
    vtable = reinterpret_cast<void**>(kBallInstanceVtable1);
    
    // Clean up audio object at offset +88
    void* audioObj = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(this) + 88);
    if (audioObj != nullptr) {
        void** audioVtable = *reinterpret_cast<void***>(audioObj);
        void (*destructor)(void*, int) = reinterpret_cast<void (*)(void*, int)>(audioVtable[0]);
        destructor(audioObj, 1);
    }
    
    // Set secondary vtable pointer at offset +80
    constexpr uintptr_t kUtilVtable = 0x82027B34u;
    *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(this) + 80) = reinterpret_cast<void*>(kUtilVtable);
    
    // Call utility cleanup function
    extern void util_8FD0(void* obj);
    util_8FD0(this);
}

/**
 * pongBallInstance::vfn_2() @ 0x82280028 | size: 0x8
 *
 * Returns pointer to embedded data structure at offset +96.
 * Likely returns a pointer to transform or physics data.
 */
void* pongBallInstance::vfn_2() {
    return reinterpret_cast<uint8_t*>(this) + 96;
}

/**
 * pongBallInstance::vfn_3() @ 0x82280030 | size: 0x40
 *
 * Copies 64 bytes (4 x 16-byte vectors) from source to offset +96.
 * Likely copies transform matrix or physics state.
 */
void pongBallInstance::vfn_3(const void* sourceData) {
    uint8_t* dest = reinterpret_cast<uint8_t*>(this) + 96;
    const uint8_t* src = reinterpret_cast<const uint8_t*>(sourceData);
    
    // Copy 4 x 16-byte vectors (64 bytes total)
    for (int i = 0; i < 4; ++i) {
        memcpy(dest + (i * 16), src + (i * 16), 16);
    }
}

/**
 * pongBallInstance::vfn_4() @ 0x8227FF38 | size: 0x78
 *
 * Gets ball position vector from game state.
 * Returns position from circular buffer based on frame counter.
 */
void pongBallInstance::vfn_4(void* outPosition) {
    extern void* g_game_state_ptr;  // @ 0x8271A2F8
    
    auto* gameState = reinterpret_cast<uint8_t*>(g_game_state_ptr);
    if (gameState == nullptr) {
        // Zero out the output position
        memset(outPosition, 0, 16);
        return;
    }
    
    auto* coreState = *reinterpret_cast<uint8_t**>(gameState + 20);
    int32_t frameCount = *reinterpret_cast<int32_t*>(coreState + 9640);
    
    if (frameCount <= 0) {
        memset(outPosition, 0, 16);
        return;
    }
    
    // Calculate circular buffer index: (frameCount + 1) % 120
    // Using fast modulo: (frameCount + 1) - ((frameCount + 1) / 120) * 120
    int32_t index = (frameCount + 1) % 120;
    int32_t offset = index * 80;  // 80 bytes per entry
    
    uint8_t* bufferBase = coreState + 32;
    uint8_t* entry = bufferBase + offset + 32;  // +32 for position offset within entry
    
    memcpy(outPosition, entry, 16);
}

/**
 * pongBallInstance::vfn_5() @ 0x8227FFB0 | size: 0x78
 *
 * Gets ball velocity vector from game state.
 * Returns velocity from circular buffer based on frame counter.
 */
void pongBallInstance::vfn_5(void* outVelocity) {
    extern void* g_game_state_ptr;  // @ 0x8271A2F8
    
    auto* gameState = reinterpret_cast<uint8_t*>(g_game_state_ptr);
    if (gameState == nullptr) {
        memset(outVelocity, 0, 16);
        return;
    }
    
    auto* coreState = *reinterpret_cast<uint8_t**>(gameState + 20);
    int32_t frameCount = *reinterpret_cast<int32_t*>(coreState + 9640);
    
    if (frameCount <= 0) {
        memset(outVelocity, 0, 16);
        return;
    }
    
    // Calculate circular buffer index
    int32_t index = (frameCount + 1) % 120;
    int32_t offset = index * 80;
    
    uint8_t* bufferBase = coreState + 32;
    uint8_t* entry = bufferBase + offset + 48;  // +48 for velocity offset within entry
    
    memcpy(outVelocity, entry, 16);
}

/**
 * pongBallInstance::OnEnter() @ 0x822C90D0 | size: 0x10
 *
 * State machine entry callback.
 * Calls virtual function at slot 16 (likely OnEnterState).
 */
void pongBallInstance::OnEnter() {
    void** vtable = *reinterpret_cast<void***>(this);
    void (*callback)(pongBallInstance*) = reinterpret_cast<void (*)(pongBallInstance*)>(vtable[16]);
    callback(this);
}

/**
 * pongBallInstance::vfn_24() @ 0x8227FA48 | size: 0x364
 *
 * Complex ball collision/interaction handler.
 * Handles ball-player collisions, activation, and event triggering.
 * 
 * This is a large function that processes ball physics interactions.
 * TODO: Full implementation requires understanding game state structure.
 */
void pongBallInstance::vfn_24(void* gameState) {
    // Stub implementation - function is very complex
    // Full implementation requires detailed analysis of:
    // - Ball collision detection
    // - Player interaction logic
    // - Event system integration
    // - Physics state updates
    
    // TODO: Implement full collision handling logic
}

/**
 * pongBallInstance::vfn_30() @ 0x8227FA28 | size: 0x20
 *
 * Validates parameters for ball operation.
 * Returns 0 if any required parameter is missing.
 */
int pongBallInstance::vfn_30(int param1, int param2, int param3) {
    // Check if param1 is zero
    if (param1 == 0) {
        // If param2 is non-zero, return 0
        if (param2 != 0) {
            return 0;
        }
        // If param3 is zero, return early
        if (param3 == 0) {
            return 0;
        }
    }
    
    return 0;
}
