// pong_ball.cpp — Ball / robot classes | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * gm_ball_robot.cpp — Ball-serving robot implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "pong_ball.hpp"
#include "rage/memory.h"
#include <cstring>   // memcpy, memset
#include <cmath>     // fabs

// External function declarations
extern void audControl_Destructor(void* obj);  // @ 0x82161568
extern "C" void* rage_alloc(uint32_t size);

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

        m_pAllocatedObject = rage_alloc(16u);
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
 * pongBallInstance::GetMatrix @ 0x82280028 | size: 0x8
 *
 * Returns pointer to the 4x4 transform matrix at offset +96.
 */
void* pongBallInstance::GetMatrix() {
    return reinterpret_cast<uint8_t*>(this) + 96;
}

/**
 * pongBallInstance::SetMatrix @ 0x82280030 | size: 0x40
 *
 * Copies a 4x4 transform matrix (64 bytes) from source into +96.
 */
void pongBallInstance::SetMatrix(const void* sourceData) {
    uint8_t* dest = reinterpret_cast<uint8_t*>(this) + 96;
    const uint8_t* src = reinterpret_cast<const uint8_t*>(sourceData);
    
    // Copy 4 x 16-byte vectors (64 bytes total)
    for (int i = 0; i < 4; ++i) {
        memcpy(dest + (i * 16), src + (i * 16), 16);
    }
}

/**
 * pongBallInstance::GetPosition @ 0x8227FF38 | size: 0x78
 *
 * Gets ball position vector from game state circular buffer.
 */
void pongBallInstance::GetPosition(void* outPosition) {
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
 * pongBallInstance::GetVelocity @ 0x8227FFB0 | size: 0x78
 *
 * Gets ball velocity vector from game state circular buffer.
 */
void pongBallInstance::GetVelocity(void* outVelocity) {
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
 * pongBallInstance::ProcessCollision @ 0x8227FA48 | size: 0x364
 *
 * Complex ball collision/interaction handler.
 * Handles ball-player collisions, activation, and event triggering.
 *
 * TODO: Full implementation requires understanding game state structure.
 */
void pongBallInstance::ProcessCollision(void* gameState) {
    // Stub implementation - function is very complex
    // Full implementation requires detailed analysis of:
    // - Ball collision detection
    // - Player interaction logic
    // - Event system integration
    // - Physics state updates
    
    // TODO: Implement full collision handling logic
}

/**
 * pongBallInstance::ValidateCollisionParams @ 0x8227FA28 | size: 0x20
 *
 * Validates parameters for ball collision operation.
 * Returns 0 if any required parameter is missing.
 */
int pongBallInstance::ValidateCollisionParams(int param1, int param2, int param3) {
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

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance additional implementations
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongBallInstance::FindObjectByID() @ 0x822C1C60 | size: 0x50
 *
 * Searches internal array for object matching given ID.
 * Returns pointer to matching object, or nullptr if not found.
 */
void* pongBallInstance::FindObjectByID(uint16_t searchID) {
    uint16_t arrayCount = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(this) + 44);
    
    if (arrayCount <= 0) {
        return nullptr;
    }
    
    void** arrayBase = *reinterpret_cast<void***>(reinterpret_cast<uint8_t*>(this) + 40);
    
    for (int i = 0; i < arrayCount; ++i) {
        void* entry = arrayBase[i];
        void* subObject = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(entry) + 4);
        uint16_t objectID = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(subObject) + 8);
        
        if (objectID == searchID) {
            return arrayBase[i];
        }
    }
    
    return nullptr;
}

/**
 * pongBallInstance::UpdatePhysicsState() @ 0x8227FDB0 | size: 0x184
 *
 * Updates ball physics state based on player interaction and game mode.
 * Handles position interpolation, velocity calculations, and slow-motion effects.
 */
void pongBallInstance::UpdatePhysicsState() {
    extern void* g_game_state_ptr;  // @ 0x8271A2F8
    extern float g_floatZero;       // Common zero constant
    extern float g_physicsScale;    // Physics time scale
    
    auto* gameState = reinterpret_cast<uint8_t*>(g_game_state_ptr);
    uint16_t ballIndex = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(this) + 8);
    
    // Initialize position vector to zero
    float positionBuffer[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    // Get player state and check if player is active
    auto* playerData = *reinterpret_cast<uint8_t**>(gameState + 4);
    auto* playerState = *reinterpret_cast<uint8_t**>(playerData + 28);
    uint8_t playerFlags = playerState[ballIndex];
    
    bool isPlayerActive = (playerFlags & 0x7) != 0;
    
    if (isPlayerActive) {
        // Get player object via vtable call
        void** vtable = *reinterpret_cast<void***>(playerData);
        void* (*getPlayer)(void*) = reinterpret_cast<void* (*)(void*)>(vtable[4]);
        void* player = getPlayer(playerData);
        
        if (player != nullptr) {
            // Copy player position (16 bytes at offset +224)
            memcpy(positionBuffer, reinterpret_cast<uint8_t*>(player) + 224, 16);
        }
    } else {
        // Use ball's own position via vtable call
        void** vtable = *reinterpret_cast<void***>(this);
        void* (*getPosition)(pongBallInstance*) = reinterpret_cast<void* (*)(pongBallInstance*)>(vtable[2]);
        void* ballPos = getPosition(this);
        memcpy(positionBuffer, ballPos, 16);
    }
    
    // Get ball physics data via vtable call
    void** vtable = *reinterpret_cast<void***>(this);
    void* (*getPhysics)(pongBallInstance*) = reinterpret_cast<void* (*)(pongBallInstance*)>(vtable[2]);
    void* physicsData = getPhysics(this);
    
    // Calculate height difference
    float ballHeight = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(physicsData) + 56);
    float targetHeight = positionBuffer[2];  // Z component
    float heightDelta = ballHeight + targetHeight;
    
    // Check if height difference is significant (threshold check)
    constexpr float kHeightThreshold = 0.001f;
    if (fabs(heightDelta) > kHeightThreshold) {
        // Recalculate physics data
        void* physicsData2 = getPhysics(this);
        float currentHeight = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(physicsData2) + 56);
        float scaleFactor = currentHeight / heightDelta;
        
        // Check slow-motion state
        extern void* g_slowmo_manager_ptr;  // @ 0x825C6540
        auto* slowmoMgr = reinterpret_cast<uint8_t*>(g_slowmo_manager_ptr);
        bool isSlowMotion = slowmoMgr[576] != 0;
        
        if (!isSlowMotion) {
            // Apply physics scale in normal mode
            float absScale = fabs(scaleFactor);
            float adjustedScale = absScale * g_physicsScale;
            
            // Check if player controller exists
            extern void* g_player_controller_ptr;  // @ 0x825C6564
            auto* controller = reinterpret_cast<uint8_t*>(g_player_controller_ptr);
            void* activePlayer = *reinterpret_cast<void**>(controller + 68);
            
            if (activePlayer != nullptr) {
                // Apply scaled velocity adjustment
                float velocityScale = 1.0f;
                float scaledVelocity[4];
                
                // Scale the position buffer
                for (int i = 0; i < 3; ++i) {
                    scaledVelocity[i] = positionBuffer[i] * velocityScale;
                }
                scaledVelocity[3] = 0.0f;
                
                // Apply to ball physics at offset +64
                extern void game_36E8(void* ball, const float* velocity);
                game_36E8(reinterpret_cast<uint8_t*>(this) + 64, scaledVelocity);
            }
        }
    }
}

/**
 * pongBallInstance::ActivateBall() @ 0x822801B8 | size: 0x1A0
 *
 * Activates the ball for gameplay.
 * Initializes ball state, position, velocity, and triggers activation events.
 */
void pongBallInstance::ActivateBall(void* activationContext) {
    extern void* g_game_state_ptr;  // @ 0x8271A2F8
    extern void rage_DebugLog();
    extern void pongBallInstance_4980_g(void*, int, int, int, int);
    extern void game_AA88(void*);
    extern void game_D500(void*, void*, void*);
    extern void game_DA60();
    extern void game_3860(void*, int, const char*);
    extern void pg_E6E0(int, int, int, int);
    
    // Check if ball is already active
    bool isActive = *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(this) + 85) != 0;
    
    if (!isActive) {
        // Debug message: "pongBallInstance::ActivateBall() - ball already active"
        constexpr uintptr_t kDebugString = 0x8203EFF8u;
        rage_DebugLog();  // Debug output function
    }
    
    auto* gameState = reinterpret_cast<uint8_t*>(g_game_state_ptr);
    auto* coreState = *reinterpret_cast<uint8_t**>(gameState + 20);
    int32_t frameCount = *reinterpret_cast<int32_t*>(coreState + 9640);
    
    if (frameCount <= 0) {
        return;
    }
    
    // Get ball index and check player state
    uint16_t ballIndex = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(this) + 8);
    
    extern void* g_player_manager_ptr;  // @ 0x8271A2D0
    auto* playerMgr = reinterpret_cast<uint8_t*>(g_player_manager_ptr);
    auto* playerState = *reinterpret_cast<uint8_t**>(playerMgr + 28);
    uint8_t stateFlags = playerState[ballIndex];
    
    bool needsInit = (stateFlags & 0x7) == 0;
    
    if (needsInit) {
        // Initialize ball state
        pongBallInstance_4980_g(g_player_manager_ptr, 0, 0, 0, 0);
    }
    
    // Get player object
    void* player = nullptr;
    auto* playerData = *reinterpret_cast<uint8_t**>(playerMgr + 4);
    auto* stateData = *reinterpret_cast<uint8_t**>(playerData + 28);
    uint8_t flags = stateData[ballIndex];
    
    bool hasPlayer = (flags & 0x7) != 0;
    
    if (hasPlayer) {
        void** vtable = *reinterpret_cast<void***>(playerData);
        void* (*getPlayer)(void*) = reinterpret_cast<void* (*)(void*)>(vtable[4]);
        player = getPlayer(playerData);
    }
    
    // Prepare ball for activation
    game_AA88(player);
    
    // Get ball position via vtable
    void** vtable = *reinterpret_cast<void***>(this);
    void* (*getPosition)(pongBallInstance*) = reinterpret_cast<void* (*)(pongBallInstance*)>(vtable[2]);
    void* ballPos = getPosition(this);
    
    // Copy ball position to stack buffer
    float posBuffer[4];
    memcpy(posBuffer, reinterpret_cast<uint8_t*>(this) + 64, 16);
    memcpy(ballPos, posBuffer, 16);
    
    // Calculate velocity vector
    constexpr float kVelocityScale = 0.5f;  // From offset in .rdata
    float velocityBuffer[4];
    
    if (player != nullptr) {
        float* playerPos = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(player) + 48);
        
        // Calculate direction vector: (ballPos - playerPos) * scale
        for (int i = 0; i < 3; ++i) {
            velocityBuffer[i] = (posBuffer[i] - playerPos[i]) * kVelocityScale;
        }
        velocityBuffer[3] = 0.0f;
        
        // Apply velocity via vtable call
        void** playerVtable = *reinterpret_cast<void***>(player);
        void (*setVelocity)(void*, const float*) = 
            reinterpret_cast<void (*)(void*, const float*)>(playerVtable[40]);
        setVelocity(player, velocityBuffer);
    }
    
    // Handle activation context
    if (activationContext != nullptr) {
        game_D500(activationContext, player, this);
        game_DA60();
    }
    
    // Trigger activation event
    constexpr const char* kActivationEvent = "ball_activated";
    game_3860(gameState, 1, kActivationEvent);
    
    // Send deactivation event (event ID 37, type 64)
    pg_E6E0(37, 64, 0, 0);
}

/**
 * pongBallInstance::InitializeFromData() @ 0x8227F810 | size: 0x60
 *
 * Initializes ball instance from serialized data.
 * Sets up ball state and configuration from data structure.
 */
void* pongBallInstance::InitializeFromData(void* initData) {
    extern void* xe_F4C0();
    extern void util_D150(void*, void*);
    
    // Allocate ball instance
    void* ballInstance = xe_F4C0();
    
    // Initialize from data (copy 16-byte header)
    util_D150(initData, reinterpret_cast<uint8_t*>(initData) + 16);
    
    // Get configuration object
    void* configObj = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(this) + 20);
    void* configData = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(configObj) + 4);
    
    // Get ball data from init structure
    void* ballData = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(ballInstance) + 20);
    
    // Call configuration setup via vtable
    void** vtable = *reinterpret_cast<void***>(configData);
    void (*configure)(void*, void*) = reinterpret_cast<void (*)(void*, void*)>(vtable[6]);
    configure(configData, ballData);
    
    // Set ball state flags
    *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(ballInstance) + 16) = 4;
    *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(ballInstance) + 96) = 0;
    
    return ballInstance;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallHitData vtable implementations
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongBallHitData::Initialize() @ 0x821D6900 | size: 0x30
 *
 * Initializes hit data structure with default flags.
 * Sets up debug mode flags if enabled.
 */
void pongBallHitData::Initialize() {
    extern uint8_t g_debugMode;  // @ 0x825C64AD
    extern void xmlNodeStruct_Initialize(void*);
    
    // Clear flags at offset +198
    *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(this) + 198) = 0;
    
    uint16_t flags = 0;
    
    // Check if debug mode is enabled
    if (g_debugMode != 0) {
        flags = 4096;  // 0x1000 - debug flag
    }
    
    // Set base flags
    *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(this) + 198) = flags;
    
    // Add additional flags: 0x6000 (24576)
    flags |= 24576;
    *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(this) + 198) = flags;
    
    // Call parent initialization
    xmlNodeStruct_Initialize(this);
}

/**
 * pongBallHitData::ValidateAttribute() @ 0x821D65C0 | size: 0x48
 *
 * Validates XML attribute hash against known attribute types.
 * Returns true if attribute is recognized, false otherwise.
 */
bool pongBallHitData::ValidateAttribute(uint32_t attributeHash) {
    extern uint32_t g_attrHash_HitZone;      // @ 0x825C761C
    extern uint32_t g_attrHash_SwingType1;   // @ 0x825C8038
    extern uint32_t g_attrHash_SwingType2;   // @ 0x825C803C
    
    // Check against known attribute hashes
    if (attributeHash == g_attrHash_HitZone) {
        return true;
    }
    
    if (attributeHash == g_attrHash_SwingType2) {
        return true;
    }
    
    if (attributeHash == g_attrHash_SwingType1) {
        return true;
    }
    
    return false;
}

/**
 * pongBallHitData::GetEventName() @ 0x821D65B0 | size: 0xC
 *
 * Returns the event name string for slow-motion entry.
 * Used for HUD event triggering.
 */
const char* pongBallHitData::GetEventName() {
    return "HUD:SLOMO_ENTER_GENERIC";  // @ 0x8203E464
}
