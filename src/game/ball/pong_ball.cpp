// pong_ball.cpp — Ball / robot classes | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * gm_ball_robot.cpp — Ball-serving robot implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "pong_ball.hpp"
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
    extern void* xe_GetLoadContext();
    extern void util_D150(void*, void*);
    
    // Allocate ball instance
    void* ballInstance = xe_GetLoadContext();
    
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

// ─────────────────────────────────────────────────────────────────────────────
// pongBallAudio::~pongBallAudio()
//
// Inline destructor body. The binary only ships the MSVC scalar-deleting
// wrapper at vtable slot 0 (@ 0x8228ABE0); that wrapper inlines this body.
// We keep a matching ~pongBallAudio() so both stack-scoped destruction and
// the ScalarDtor thunk share the same teardown logic.
// ─────────────────────────────────────────────────────────────────────────────
pongBallAudio::~pongBallAudio() {
    extern void audControl3d_Destructor(pongBallAudio* obj);  // @ 0x8228AC30 — base dtor
    audControl3d_Destructor(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallHit destructor thunk  [rtti_73A8_0 @ 0x822824A8]
// Destructor for pongBallHit — releases singleton data, sets vtable, frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongBallHit_Destructor(pongBallHit* self, int flags) {
    extern void atSingleton_Release(void* obj);  // @ 0x821A9420
    extern void rage_free(void* ptr);            // @ 0x820C00C0

    constexpr uintptr_t kPongBallHitVtable = 0x820573A8u;

    // Release singleton resources at offset +48
    atSingleton_Release(reinterpret_cast<uint8_t*>(self) + 48);

    // Set vtable pointer
    self->vtable = reinterpret_cast<void**>(kPongBallHitVtable);

    // Free memory if flag bit 0 is set
    if (flags & 1) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance secondary vtable destructor thunk  [rtti_7094_0 @ 0x82280890]
// Adjusts 'this' by -80 to reach primary base, then calls primary destructor.
// ─────────────────────────────────────────────────────────────────────────────
void pongBallInstance_SecondaryDtorThunk(pongBallInstance* secondaryBase) {
    pongBallInstance* primary = reinterpret_cast<pongBallInstance*>(
        reinterpret_cast<uint8_t*>(secondaryBase) - 80);
    primary->~pongBallInstance();
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance::SnapshotMatrixToGlobalSlot0()  [@ 0x822CD5F0]
// Copies ball transform matrix into global ball state array (slot 0 / player 1).
// ─────────────────────────────────────────────────────────────────────────────
void pongBallInstance::SnapshotMatrixToGlobalSlot0(pongBallInstance* ball) {
    // Shared physics collision-pair broadphase scratch at 0x825CBCF0.
    // (See ball collision-pair broadphase seeders section lower in this file
    // for the layout map — same block, same name.)
    extern uint8_t g_pairBroadphaseScratch[];  // @ 0x825CBCF0 (17152 bytes)

    // Get ball's transform matrix via vtable slot 2 (GetMatrix)
    void* matrixPtr = ball->GetMatrix();

    // Read ball's flags to get physics sub-object reference
    uint32_t flagsObj = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(ball) + 4);
    uint32_t physicsRef = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(flagsObj) + 12);

    // Store metadata into global state (slot 0)
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 17064) = 0;               // clear status
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 17072) = reinterpret_cast<uintptr_t>(ball); // ball ptr
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 16912) = reinterpret_cast<uintptr_t>(
        reinterpret_cast<uint8_t*>(ball) + 16);                                       // ball data ptr
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 17056) = physicsRef;       // physics reference

    // Copy 4x4 transform matrix (4 x 16-byte rows = 64 bytes)
    const uint8_t* src = reinterpret_cast<const uint8_t*>(matrixPtr);
    memcpy(g_pairBroadphaseScratch + 16928, src,      16);  // row 0
    memcpy(g_pairBroadphaseScratch + 16944, src + 16, 16);  // row 1
    memcpy(g_pairBroadphaseScratch + 16960, src + 32, 16);  // row 2
    memcpy(g_pairBroadphaseScratch + 16976, src + 48, 16);  // row 3
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance::SnapshotMatrixToGlobalSlot1()  [@ 0x822CD690]
// Copies ball transform matrix into global ball state array (slot 1 / player 2).
// ─────────────────────────────────────────────────────────────────────────────
void pongBallInstance::SnapshotMatrixToGlobalSlot1(pongBallInstance* ball) {
    extern uint8_t g_pairBroadphaseScratch[];  // @ 0x825CBCF0

    void* matrixPtr = ball->GetMatrix();

    uint32_t flagsObj = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(ball) + 4);
    uint32_t physicsRef = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(flagsObj) + 12);

    // Store metadata into global state (slot 1 — offsets shifted by +4/+64 from slot 0)
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 17068) = 0;
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 17076) = reinterpret_cast<uintptr_t>(ball);
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 16916) = reinterpret_cast<uintptr_t>(
        reinterpret_cast<uint8_t*>(ball) + 16);
    *reinterpret_cast<uint32_t*>(g_pairBroadphaseScratch + 17060) = physicsRef;

    const uint8_t* src = reinterpret_cast<const uint8_t*>(matrixPtr);
    memcpy(g_pairBroadphaseScratch + 16992, src,      16);  // row 0
    memcpy(g_pairBroadphaseScratch + 17008, src + 16, 16);  // row 1
    memcpy(g_pairBroadphaseScratch + 17024, src + 32, 16);  // row 2
    memcpy(g_pairBroadphaseScratch + 17040, src + 48, 16);  // row 3
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance::HandlePlayerHitEvent()  [@ 0x822C3B80]
// Processes a player hit event on the ball — checks player state, resolves
// the ball object, and either dispatches a hit or returns the player object.
// ─────────────────────────────────────────────────────────────────────────────
void* pongBallInstance::HandlePlayerHitEvent(void* eventData, void* hitContext) {
    extern void pongBallInstance_DispatchHit(
        pongBallInstance*, uint16_t, void*, void*, void*, void*, void*);  // @ 0x822C4980

    // Get flags/player-state lookup table
    uint32_t flagsObj = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 4);
    uint16_t playerIndex = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(hitContext) + 8);
    uint8_t* playerStateTable = reinterpret_cast<uint8_t*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(flagsObj) + 28));
    uint8_t stateFlags = playerStateTable[playerIndex];

    // Check if player state is exactly 1 (active single player)
    bool isActivePlayer = (stateFlags & 0x7) == 1;

    if (isActivePlayer) {
        // Try to find the ball object by ID
        void* ballObj = FindObjectByID(playerIndex);
        if (ballObj != nullptr) {
            // Check if ball is valid via vtable slot 7
            void** objVtable = *reinterpret_cast<void***>(ballObj);
            bool (*isValid)(void*) = reinterpret_cast<bool (*)(void*)>(objVtable[7]);
            if (!isValid(ballObj)) {
                return nullptr;
            }
        }

        // Dispatch the hit event
        pongBallInstance_DispatchHit(this, playerIndex, nullptr, eventData, nullptr, nullptr, nullptr);
        return nullptr;
    }

    // Player state is not active — check if state is zero (no player)
    uint8_t rawState = playerStateTable[playerIndex];
    bool noPlayer = (rawState & 0x7) == 0;

    if (noPlayer) {
        // Get player object via flags vtable slot 4
        uint32_t flagsObj2 = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 4);
        void** fVtable = *reinterpret_cast<void***>(reinterpret_cast<void*>(flagsObj2));
        void* (*getPlayer)(void*) = reinterpret_cast<void* (*)(void*)>(fVtable[4]);
        return getPlayer(reinterpret_cast<void*>(flagsObj2));
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance::HandleRemotePlayerHit()  [@ 0x822D5400]
// Similar to HandlePlayerHitEvent but for remote/networked player hits.
// Loads ball instance from a global pointer instead of using 'this'.
// ─────────────────────────────────────────────────────────────────────────────
bool pongBallInstance::HandleRemotePlayerHit(void* hitContext) {
    extern uint32_t g_playerManagerPtr;  // @ 0x826065E4 (SDA global)
    extern uint32_t g_ballInstancePtr;   // @ 0x825F5BD0

    extern void pongBallInstance_DispatchHit(
        pongBallInstance*, uint16_t, void*, void*, void*, void*, void*);  // @ 0x822C4980

    // Look up player state from global player manager
    uint32_t playerMgr = *reinterpret_cast<uint32_t*>(&g_playerManagerPtr);
    uint16_t playerIndex = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(hitContext) + 8);
    uint8_t* playerStateTable = reinterpret_cast<uint8_t*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(playerMgr) + 28));
    uint8_t stateFlags = playerStateTable[playerIndex];

    bool isActivePlayer = (stateFlags & 0x7) == 1;

    if (!isActivePlayer) {
        // No active player — return true (handled)
        return true;
    }

    // Load ball instance from global
    auto* ballInstance = reinterpret_cast<pongBallInstance*>(
        *reinterpret_cast<uint32_t*>(&g_ballInstancePtr));

    // Try to find ball object by player index
    void* ballObj = ballInstance->FindObjectByID(playerIndex);
    if (ballObj != nullptr) {
        // Check validity via vtable slot 7
        void** objVtable = *reinterpret_cast<void***>(ballObj);
        bool (*isValid)(void*) = reinterpret_cast<bool (*)(void*)>(objVtable[7]);
        if (!isValid(ballObj)) {
            return false;
        }
        // Reload ball instance (may have changed)
        ballInstance = reinterpret_cast<pongBallInstance*>(
            *reinterpret_cast<uint32_t*>(&g_ballInstancePtr));
    }

    // Dispatch hit with caller context
    pongBallInstance_DispatchHit(ballInstance, playerIndex, nullptr, nullptr, nullptr, this, nullptr);

    return (ballInstance != nullptr);  // returns dispatch result
}

// ─────────────────────────────────────────────────────────────────────────────
// gmBallNode::IsEventHandled()  [vtable slot 20 @ 0x822808E8]
// Checks if a given event hash matches any of the 4 known ball event types.
// ─────────────────────────────────────────────────────────────────────────────
bool gmBallNode::IsEventHandled(uint32_t eventHash) {
    extern uint32_t g_eventHash_BallHit;       // @ 0x825C4954
    extern uint32_t g_eventHash_BallBounce;    // @ 0x825C2D74
    extern uint32_t g_eventHash_BallSpin;      // @ 0x825C2D78
    extern uint32_t g_eventHash_BallCollision; // @ 0x825C93D0

    if (eventHash == g_eventHash_BallHit) {
        return true;
    }
    if (eventHash == g_eventHash_BallBounce) {
        return true;
    }
    if (eventHash == g_eventHash_BallSpin) {
        return true;
    }
    if (eventHash == g_eventHash_BallCollision) {
        return true;
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// gmBallNode::GetTypeName()  [vtable slot 19 @ 0x82280940]
// Returns the RTTI type name string for this node class.
// ─────────────────────────────────────────────────────────────────────────────
const char* gmBallNode::GetTypeName() {
    // Points to the RTTI type info name at 0x820570A0 (in .rdata)
    return reinterpret_cast<const char*>(0x820570A0u);
}

// ─────────────────────────────────────────────────────────────────────────────
// gmBallNode::Reset()  [vtable slot 24 @ 0x82281368]
// Resets the ball node — frees allocated resources, destroys child objects,
// and clears global ball state pointer.
// ─────────────────────────────────────────────────────────────────────────────
void gmBallNode::Reset() {
    extern void rage_free(void* ptr);       // @ 0x820C00C0
    extern uint32_t g_gameBallStatePtr;     // @ 0x8271A2F4
    extern uint32_t g_activeBallNodePtr;    // @ 0x82606570

    // Clear the global game ball state pointer
    g_gameBallStatePtr = 0;

    // Free allocated buffers at offsets +172, +176, +180
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    rage_free(reinterpret_cast<void*>(*reinterpret_cast<uint32_t*>(self + 172)));
    rage_free(reinterpret_cast<void*>(*reinterpret_cast<uint32_t*>(self + 176)));
    rage_free(reinterpret_cast<void*>(*reinterpret_cast<uint32_t*>(self + 180)));

    // Destroy physics object at offset +184 (has secondary base at +16)
    void* physicsObj = reinterpret_cast<void*>(*reinterpret_cast<uint32_t*>(self + 184));
    if (physicsObj != nullptr) {
        void* secondaryBase = reinterpret_cast<uint8_t*>(physicsObj) + 16;
        void** objVtable = *reinterpret_cast<void***>(secondaryBase);
        void (*destructor)(void*, int) = reinterpret_cast<void (*)(void*, int)>(objVtable[0]);
        destructor(secondaryBase, 1);
    }

    // Destroy audio object at offset +188
    void* audioObj = reinterpret_cast<void*>(*reinterpret_cast<uint32_t*>(self + 188));
    if (audioObj != nullptr) {
        void** audioVtable = *reinterpret_cast<void***>(audioObj);
        void (*audioDtor)(void*, int) = reinterpret_cast<void (*)(void*, int)>(audioVtable[0]);
        audioDtor(audioObj, 1);
    }

    // Clear the active ball node pointer
    g_activeBallNodePtr = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// gmBallNode secondary vtable destructor thunk  [rtti_7298_0 @ 0x82282498]
// Adjusts 'this' by -128 to reach primary base, then calls primary destructor.
// ─────────────────────────────────────────────────────────────────────────────
void gmBallNode_SecondaryDtorThunk(gmBallNode* secondaryBase) {
    gmBallNode* primary = reinterpret_cast<gmBallNode*>(
        reinterpret_cast<uint8_t*>(secondaryBase) - 128);
    primary->~gmBallNode();
}

// ─────────────────────────────────────────────────────────────────────────────
// gmBallNode tertiary vtable destructor thunk  [rtti_7288_0 @ 0x82281CF8]
// Loads child object at this+28, then calls its destructor through vtable.
// ─────────────────────────────────────────────────────────────────────────────
void gmBallNode_TertiaryDtorThunk(gmBallNode* self) {
    // Load the secondary base object pointer at offset +28
    void* childObj = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 28);

    // Call the child's destructor through its vtable slot 0
    void** childVtable = *reinterpret_cast<void***>(childObj);
    void (*destructor)(void*) = reinterpret_cast<void (*)(void*)>(childVtable[0]);
    destructor(childObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance / pongBallHitData virtual method implementations
// ─────────────────────────────────────────────────────────────────────────────

// External helpers referenced by these methods.
extern "C" {
    // Global physics world / history ring accessor — fetches ring header ptr.
    // PPC literal: lis -32142 / lwz -23816 / lwz +20 → g_physWorld->historySystem
    void* pongPhysics_GetHistorySystem();                              // @ 0x8232E9F8 via global table

    // Event dispatcher used by vfn_24's 'swing hit' cases.
    int  pongBallInstance_DispatchSwingEvent(uintptr_t owner, uint32_t hitType,
                                             const pongBallInstance::Vec4* impactPos,
                                             uint32_t evFlags, uint32_t zero);  // @ 0x82280070

    // Audio cue for paddle impact — 8225E6E0.
    void pongBall_PlayPaddleImpactCue(uint32_t bitflag, uint32_t, uint32_t, uint32_t);

    // Game-logic updater called on the 'missed' path.
    void pongBallInstance_OnBallMissed(pongBallInstance* self);        // @ 0x8227FDB0

    // Remote player hit resolver used by the fast-path branch.
    void pongBall_ResolveRemoteHit(pongBallInstance* self, pongBallInstance* other,
                                   uint32_t impactHandle, uint32_t slotIndex,
                                   uint32_t flagByte);                 // @ 0x822CD730

    // XML serialization helpers for pongBallHitData::LoadProperties.
    void xmlNodeStruct_RegisterField(pongBallHitData* self,
                                     const char* name,
                                     void* addr,
                                     uint32_t typeTag,
                                     uint32_t arity);                  // @ 0x821A8F58
    void xmlNodeStruct_PostLoadBase(pongBallHitData* self);            // @ 0x821A8988
}

// Global static byte written/read by PostLoadProperties / LoadProperties.
// lis -32160 / +25741 → 0x8203E48D (.data byte, size 1) — a 'debug enabled' flag.
extern "C" uint8_t g_pongBallHitDataEnableFlag;                        // @ 0x8203E48D

namespace {
constexpr uint16_t kHitDataFlag_Enabled     = 0x1000;  // bit set when debug flag is on
constexpr uint16_t kHitDataFlag_InitialMask = 0x6000;  // OR'd on by PostLoadProperties
constexpr const char kHitDataNodeTypeName[] = "pongBallHitData";
} // namespace

// ── [2] pongBallInstance::GetMatrixAt60 @ 0x82280028 ─────────────────────────
// Trivially returns a pointer to the embedded 4×4 matrix stored at this+0x60.
pongBallInstance::Mat44* pongBallInstance::GetMatrixAt60() {
    return reinterpret_cast<Mat44*>(reinterpret_cast<uint8_t*>(this) + 0x60);
}

// ── [3] pongBallInstance::SetMatrixAt60 @ 0x82280030 ─────────────────────────
// Vector-copies a full 64-byte matrix from 'src' into this+0x60 via 4× vmx loads.
void pongBallInstance::SetMatrixAt60(const Mat44* src) {
    Mat44* dst = reinterpret_cast<Mat44*>(reinterpret_cast<uint8_t*>(this) + 0x60);
    dst->row0 = src->row0;
    dst->row1 = src->row1;
    dst->row2 = src->row2;
    dst->row3 = src->row3;
}

// ── [4] pongBallInstance::GetCurrentPosition @ 0x8227FF38 ────────────────────
// Reads current ball position from the physics history ring (row 0 = translation).
// When no entries yet, zeroes the output. The hash-to-slot math mirrors the PPC:
//   slot = ((count+1) * 0x88888889h) >> 32, adjusted, mod 120.
void pongBallInstance::GetCurrentPosition(Vec4* outPos) {
    void* hist = pongPhysics_GetHistorySystem();
    if (!hist) { *outPos = Vec4{0.f, 0.f, 0.f, 0.f}; return; }

    int32_t entryCount = *reinterpret_cast<int32_t*>(
        static_cast<uint8_t*>(hist) + 9640);
    if (entryCount <= 0) {
        *outPos = Vec4{0.f, 0.f, 0.f, 0.f};
        return;
    }

    uint8_t* ringBase   = static_cast<uint8_t*>(hist) + 32;
    int32_t  headCount  = *reinterpret_cast<int32_t*>(ringBase + 9604) + 1;
    // (headCount % 120) * 80 — 80 bytes per entry, row0 is first 16 bytes.
    int32_t  slotIndex  = static_cast<int32_t>(
        static_cast<uint32_t>(headCount) % 120u);
    uint8_t* entry      = ringBase + static_cast<size_t>(slotIndex) * 80u + 32u;

    *outPos = *reinterpret_cast<Vec4*>(entry);
}

// ── [5] pongBallInstance::GetCurrentVelocity @ 0x8227FFB0 ────────────────────
// Same ring geometry as GetCurrentPosition, but row at +48 (row2) holds velocity.
void pongBallInstance::GetCurrentVelocity(Vec4* outVel) {
    void* hist = pongPhysics_GetHistorySystem();
    if (!hist) { *outVel = Vec4{0.f, 0.f, 0.f, 0.f}; return; }

    int32_t entryCount = *reinterpret_cast<int32_t*>(
        static_cast<uint8_t*>(hist) + 9640);
    if (entryCount <= 0) {
        *outVel = Vec4{0.f, 0.f, 0.f, 0.f};
        return;
    }

    uint8_t* ringBase   = static_cast<uint8_t*>(hist) + 32;
    int32_t  headCount  = *reinterpret_cast<int32_t*>(ringBase + 9604) + 1;
    int32_t  slotIndex  = static_cast<int32_t>(
        static_cast<uint32_t>(headCount) % 120u);
    uint8_t* entry      = ringBase + static_cast<size_t>(slotIndex) * 80u + 48u;

    *outVel = *reinterpret_cast<Vec4*>(entry);
}

// ── [30] pongBallInstance::ValidateCollisionParams3 @ 0x8227FA28 ─────────────
// Three-argument validity gate — returns zero unconditionally in the shipped
// build. PPC only inspects the args via cmplwi; all paths fall through to 'r3=0'.
int pongBallInstance::ValidateCollisionParams3(uint32_t a, uint32_t b, uint32_t c) {
    (void)a; (void)b; (void)c;
    return 0;
}

// ── [24] pongBallInstance::ProcessCollisionAndActivate @ 0x8227FA48 ──────────
// Core collision reaction:
//   1. Resolve 'other' through the MI-vtable offset adjustment when needed.
//   2. Ask pongBall_ResolveRemoteHit for both primary and (if matched) secondary
//      link handles.
//   3. Build an impact vector = other.pos - self.ref_pos, and classify the
//      event via pongBallInstance_DispatchSwingEvent. Depending on outcome:
//        case 4 → play paddle audio cue (threshold check vs. m_speedSquared)
//        case 3/2 with no physics-throttle → call OnBallMissed
//        default with gate byte set → ActivateBall
// The tail after 0x8227FD24 copies the impact vector into the scratch at +80
// and issues further audio / event hooks; those are summarized in TODO below.
void pongBallInstance::ProcessCollisionAndActivate(pongBallInstance* other) {
    // Resolve primary link handle. The PPC path uses vtable[-1] as an MI offset;
    // in the happy-path of the shipped build this offset is 176 bytes.
    uint32_t primaryHandle = other->m_primaryLinkPtr;
    if (primaryHandle == 0) {
        uintptr_t vptr      = reinterpret_cast<uintptr_t>(other->vtable);
        size_t    adjust    = static_cast<size_t>(vptr) * 176u;
        primaryHandle = *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(other) + adjust);
    }

    uint32_t hitHandleA =
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(other) + 12 + 16);
    pongBall_ResolveRemoteHit(this, other, hitHandleA, /*slot*/0, /*flag*/1);

    // Secondary branch — only when primary matches 'this'.
    if (primaryHandle == reinterpret_cast<uintptr_t>(this)) {
        uint32_t secondaryHandle = other->m_secondaryLinkPtr;
        if (secondaryHandle == 0) {
            uintptr_t vptr   = reinterpret_cast<uintptr_t>(other->vtable);
            size_t    adjust = static_cast<size_t>(vptr - 1) * 176u;
            secondaryHandle = *reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(other) + adjust + 180u);
        }
        uint32_t hitHandleB =
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(other) + 12 + 16 + 4);
        pongBall_ResolveRemoteHit(this, other, hitHandleB, /*slot*/0, /*flag*/0);
        (void)secondaryHandle;
    }

    // impactPos = (other pose position) - (self pose ref) from vector slot at +64/+16.
    Vec4* otherRow = reinterpret_cast<Vec4*>(
        reinterpret_cast<uint8_t*>(other) + 12 + 64);
    Vec4* selfRef  = reinterpret_cast<Vec4*>(
        reinterpret_cast<uint8_t*>(other) + 12 + 64);   // PPC uses +0xb0 relative
    Vec4 impact{
        otherRow->x - selfRef->x,
        otherRow->y - selfRef->y,
        otherRow->z - selfRef->z,
        0.f
    };

    uint32_t evFlags =
        *reinterpret_cast<uint32_t*>(
            *reinterpret_cast<uint32_t**>(reinterpret_cast<uint8_t*>(other) + 12 + 16) + 16/4)
        & 0xDu;

    int classification = pongBallInstance_DispatchSwingEvent(
        reinterpret_cast<uintptr_t>(this), 0, &impact, evFlags, 0);

    switch (classification) {
    case 0: case 1: case 2: case 3:
        break;
    default:
        if (this->m_hitPolicyFlag != 0) {
            this->ActivateBall(other);
        }
        break;
    }

    // 'Missed' path (case 3 / case 2 without physics throttle) and case 4
    // paddle-impact audio are inlined in the original — summarized here and
    // flagged as TODO once pg_E6E0 / pongBall_PlayPaddleImpactCue are lifted.
    if (classification == 3 || classification == 2) {
        void* histEntry = pongPhysics_GetHistorySystem();
        int32_t gate = histEntry
            ? *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(histEntry) + 9640)
            : 0;
        if (gate == 0) {
            pongBallInstance_OnBallMissed(this);
        }
    } else if (classification == 4) {
        // TODO(pong_ball vfn_24): reproduce paddle-impact cue selection
        // (reads lbl_82079B18 scalar, compares against m_speedSquared × k,
        // selects cue id 8 or 9, calls pongBall_PlayPaddleImpactCue).
        pongBall_PlayPaddleImpactCue(/*bitflag*/1, 0, 0, 0);
    }
}

// ── pongBallHitData::PostLoadProperties [vfn_2] @ 0x821D6900 ─────────────────
// Sets m_flags based on the global enable byte, then OR-masks 0x6000 on top
// and chains to the base xmlNodeStruct post-load hook.
void pongBallHitData::PostLoadProperties() {
    uint16_t value = 0;
    if (g_pongBallHitDataEnableFlag != 0) {
        value = kHitDataFlag_Enabled;
    }
    this->m_flags = static_cast<uint16_t>(value | kHitDataFlag_InitialMask);
    xmlNodeStruct_PostLoadBase(this);
}

// File-scope extern decls (C linkage prohibited inside function bodies)
extern "C" uint32_t g_pongBallHitData_hashA;   // @ 0x8202861C
extern "C" uint32_t g_pongBallHitData_hashB;   // @ 0x8203F03C
extern "C" uint32_t g_pongBallHitData_hashC;   // @ 0x8203F038

// ── pongBallHitData::IsApplicable [vfn_20] @ 0x821D65C0 ──────────────────────
// Matches the passed attribute hash against three known property hashes.
bool pongBallHitData::IsApplicable(uint32_t contextHash) {
    if (contextHash == g_pongBallHitData_hashA) return true;
    if (contextHash == g_pongBallHitData_hashB) return true;
    return contextHash == g_pongBallHitData_hashC;
}

// ── pongBallHitData::GetNodeTypeName [vfn_22] @ 0x821D65B0 ───────────────────
// Returns the static node-type literal "pongBallHitData".
const char* pongBallHitData::GetNodeTypeName() {
    return kHitDataNodeTypeName;
}

// ── pongBallHitData::LoadProperties [vfn_21] @ 0x821D6608 ────────────────────
// Registers every serializable field with the XML system. The full method
// registers ~30 fields — implemented here as a compact table-driven pass.
void pongBallHitData::LoadProperties() {
    // TODO(pong_ball HitData::LoadProperties): enumerate remaining 18
    // xmlNodeStruct_RegisterField entries at offsets 256..0x15C once their
    // string table (lbl_825CAF9C family) is named. Structure matches pass5.
    struct FieldDesc { uint32_t offset; const char* name; uint32_t type; };
    static const FieldDesc kFields[] = {
        { 16,  "HitZone",        0 }, { 20,  "SwingType",      0 },
        { 24,  "Dir",            0 }, { 32,  "Pos",            0 },
        { 48,  "Power",          0 }, { 96,  "Aim",            0 },
        { 112, "Timing",         0 }, { 128, "Spin",           0 },
        { 132, "Slice",          0 }, { 136, "Arc",            0 },
        { 140, "TargetSpeed",    0 }, { 144, "Accuracy",       0 },
        { 148, "Reflex",         0 }, { 152, "StaminaDrain",   0 },
        { 156, "CooldownMs",     0 }, {  64, "AnimClip",       0 },
        { 160, "Ai",             0 }, { 176, "AiTarget",       0 },
        { 308, "CurveMul",       0 },
    };
    for (const FieldDesc& fd : kFields) {
        xmlNodeStruct_RegisterField(
            this, fd.name,
            reinterpret_cast<uint8_t*>(this) + fd.offset,
            fd.type, 0u);
    }

    // Mirror the global 'enabled' byte from the parsed flag bit.
    g_pongBallHitDataEnableFlag =
        (this->m_flags & kHitDataFlag_Enabled) ? 1 : 0;
}

// ────────────────────────────────────────────────────────────────────────────
// pongBallInstance_4980_g @ 0x822C4980 | size: 0x1DC
// No pseudocode available — emitted as a valid no-op so it links while the
// real body is lifted in a later pass. Signature derived from call sites in
// player code (pongPlayer calls this on the player manager pointer).
// ────────────────────────────────────────────────────────────────────────────
void pongBallInstance_4980_g(void* instance, int a, int b, int c, int d) {
    (void)instance; (void)a; (void)b; (void)c; (void)d;
}

// 6-arg overload — call site in phUpdateObject::Reset uses
// (void*, unsigned short, int, int, int, int). Kept as a no-op stub.
void pongBallInstance_4980_g(void* instance, unsigned short s,
                             int a, int b, int c, int d) {
    (void)instance; (void)s; (void)a; (void)b; (void)c; (void)d;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongBallInstance_DispatchHit (C++ linkage, 7 args) — used by pongBallInstance
// hit/remote-hit paths. No matching binary symbol; stubbed here to link.
// ─────────────────────────────────────────────────────────────────────────────
void pongBallInstance_DispatchHit(pongBallInstance* self, unsigned short player,
                                  void* a, void* b, void* c, void* d, void* e) {
    (void)self; (void)player; (void)a; (void)b; (void)c; (void)d; (void)e;
}

// ─────────────────────────────────────────────────────────────────────────────
// C-linkage ball-event bridges referenced from ball::vfn_24 (swing resolver).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    int pongBallInstance_DispatchSwingEvent(uintptr_t owner, uint32_t hitType,
                                            const pongBallInstance::Vec4* impactPos,
                                            uint32_t evFlags, uint32_t zero) {
        (void)owner; (void)hitType; (void)impactPos; (void)evFlags; (void)zero;
        return 0;
    }

    void pongBallInstance_OnBallMissed(pongBallInstance* self) { (void)self; }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongLerpQueue_Update — synthetic linker bridge called from
// pongPlayer::UpdatePhysicsAndSwingInput. The full lerp queue is in
// src/game/pong_lerp_queue.cpp (not yet in CMakeLists). Stub here.
// ─────────────────────────────────────────────────────────────────────────────
void pongLerpQueue_Update(void* queue) { (void)queue; }


// ─────────────────────────────────────────────────────────────────────────────
// Ball collision-pair broadphase seeders — lifted from pass5_final recomp.
//
// These fill the 17152 B physics collision-pair scratch block at 0x825CBCF0
// (also populated by phSimulator::vfn_4 / phArticulatedCollider::vfn_5 /
// phLevelNew::vfn_27 — hence *not* a ball-specific global, the ball code just
// re-uses the engine pair buffer). The block holds two "slots" at byte
// offsets 16912..17076 — slot 0 (primary / chooser side) and slot 1
// (secondary / opponent side). Each slot stores:
//   +16912/+16916 : data-area pointer  (ball +16 + 16)
//   +17056/+17060 : sub-object pointer chained via (ball+16)->+4->+12
//   +17064/+17068 : flag slot (also cleared / overwritten to 0)
//   +17072/+17076 : back-reference to the ball instance
//   +16928..+16976 / +16992..+17040 : 4×16 B matrix rows copied from ball+32
//
// The raw engine helper at 0x82294330 still carries its anonymous
// `util_4330` name in the binary symbol map (no RTTI / string anchor), but
// its semantic role is "clear the pair-broadphase scratch block". We wrap it
// in a named extern so call-sites read meaningfully.
// ─────────────────────────────────────────────────────────────────────────────

// Raw engine symbol — ph/ball pair-broadphase scratch clear.
// @ 0x82294330 | size: 0xB4. Shared with phSimulator_vfn_4 etc.
extern "C" void util_4330(void* pairScratch, void* partnerHint);

// Semantic alias — same entry point, readable name at the call site.
inline void PhysicsPairBroadphase_Clear(void* pairScratch, void* partnerHint) {
    util_4330(pairScratch, partnerHint);
}

namespace {

// Read the ordering/priority byte used to decide which ball wins "slot 0".
// The chain is (ball+16)->+4->+12, reading byte at +4 of the final pointer.
// Lower 3 bits only participate in the compare.
inline uint8_t ReadBallPriorityByte(pongBallInstance* ball) {
    auto* ballBytes    = reinterpret_cast<uint8_t*>(ball);
    auto* dataPtr      = *reinterpret_cast<uint8_t**>(ballBytes + 16);
    auto* subLevel1    = *reinterpret_cast<uint8_t**>(dataPtr + 4);
    auto* subLevel2    = *reinterpret_cast<uint8_t**>(subLevel1 + 12);
    return *(subLevel2 + 4);
}

// Fill slot 0 of g_pairBroadphaseScratch with (ball, matrix-rows, metadata).
inline void WriteBallToSlot0(uint8_t* state, pongBallInstance* ball) {
    auto* ballBytes    = reinterpret_cast<uint8_t*>(ball);
    auto* dataPtr      = *reinterpret_cast<uint8_t**>(ballBytes + 16);
    auto* subLevel1    = *reinterpret_cast<uint8_t**>(dataPtr + 4);
    auto* subLevel2    = *reinterpret_cast<uint8_t**>(subLevel1 + 12);

    *reinterpret_cast<uint32_t*>(state + 17064) = 0u;
    *reinterpret_cast<uint32_t*>(state + 17072) = reinterpret_cast<uintptr_t>(ball);
    *reinterpret_cast<uint32_t*>(state + 17056) = reinterpret_cast<uintptr_t>(subLevel2);
    *reinterpret_cast<uint32_t*>(state + 16912) = reinterpret_cast<uintptr_t>(dataPtr + 16);

    const uint8_t* mat = ballBytes + 32;
    memcpy(state + 16928, mat +  0, 16);
    memcpy(state + 16944, mat + 16, 16);
    memcpy(state + 16960, mat + 32, 16);
    memcpy(state + 16976, mat + 48, 16);
}

// Fill slot 1 of g_pairBroadphaseScratch.
inline void WriteBallToSlot1(uint8_t* state, pongBallInstance* ball) {
    auto* ballBytes    = reinterpret_cast<uint8_t*>(ball);
    auto* dataPtr      = *reinterpret_cast<uint8_t**>(ballBytes + 16);
    auto* subLevel1    = *reinterpret_cast<uint8_t**>(dataPtr + 4);
    auto* subLevel2    = *reinterpret_cast<uint8_t**>(subLevel1 + 12);

    *reinterpret_cast<uint32_t*>(state + 17068) = 0u;
    *reinterpret_cast<uint32_t*>(state + 17076) = reinterpret_cast<uintptr_t>(ball);
    *reinterpret_cast<uint32_t*>(state + 17060) = reinterpret_cast<uintptr_t>(subLevel2);
    *reinterpret_cast<uint32_t*>(state + 16916) = reinterpret_cast<uintptr_t>(dataPtr + 16);

    const uint8_t* mat = ballBytes + 32;
    memcpy(state + 16992, mat +  0, 16);
    memcpy(state + 17008, mat + 16, 16);
    memcpy(state + 17024, mat + 32, 16);
    memcpy(state + 17040, mat + 48, 16);
}

} // namespace

/**
 * BallCollisionPair_SeedFromBothBalls @ 0x822CD228 | size: 0x194
 *
 * Prepares the physics collision-pair broadphase scratch for a ball-vs-ball
 * contact. Clears the block, then writes both ball slots (0 and 1) ordered
 * by the 3-bit priority tag read via the chain (ball+16)->+4->+12+4. The
 * ball with the *smaller* tag occupies slot 0 (priority branch: `blt`).
 *
 * NOTE: this is NOT a pongBallInstance member — r3 on entry is the pair
 * scratch block (g_pairBroadphaseScratch), not a `this` pointer. The two
 * ball arguments arrive in r4/r5.
 */
void BallCollisionPair_SeedFromBothBalls(pongBallInstance* ballA,
                                         pongBallInstance* ballB) {
    extern uint8_t g_pairBroadphaseScratch[];  // @ 0x825CBCF0, 17152 B

    PhysicsPairBroadphase_Clear(g_pairBroadphaseScratch, ballB);

    const uint8_t tagA = ReadBallPriorityByte(ballA);
    const uint8_t tagB = ReadBallPriorityByte(ballB);

    pongBallInstance* slot0Ball;
    pongBallInstance* slot1Ball;
    if (tagB < tagA) {
        // Less-than branch: ballA keeps slot 0, ballB goes to slot 1.
        slot0Ball = ballA;
        slot1Ball = ballB;
    } else {
        slot0Ball = ballB;
        slot1Ball = ballA;
    }

    WriteBallToSlot0(g_pairBroadphaseScratch, slot0Ball);
    WriteBallToSlot1(g_pairBroadphaseScratch, slot1Ball);
}

/**
 * BallCollisionPair_SeedFromOneBall @ 0x822CD3C0 | size: 0x13C
 *
 * Asymmetric seeder used when only one of the two contact participants is a
 * ball (the other is a non-ball collider whose priority byte lives one chain
 * deeper: (other+4)->+12+4). The ball goes to whichever slot the priority
 * ordering dictates; the non-ball side is populated via the matrix-snapshot
 * helper for the opposite slot.
 *
 *   if (ball tag <= other tag):  ball -> slot 0 directly,
 *                                 SnapshotMatrixToGlobalSlot1(ball) fills slot 1.
 *   else:                        SnapshotMatrixToGlobalSlot0(ball) fills slot 0,
 *                                 ball -> slot 1 directly.
 *
 * (See pongBallInstance_1FA0 for the parallel control-flow pattern.)
 */
void BallCollisionPair_SeedFromOneBall(pongBallInstance* ball,
                                       pongBallInstance* other) {
    extern uint8_t g_pairBroadphaseScratch[];  // @ 0x825CBCF0

    PhysicsPairBroadphase_Clear(g_pairBroadphaseScratch, other);

    auto* otherBytes  = reinterpret_cast<uint8_t*>(other);
    auto* otherSub1   = *reinterpret_cast<uint8_t**>(otherBytes + 4);
    auto* otherSub2   = *reinterpret_cast<uint8_t**>(otherSub1 + 12);
    const uint8_t tagOther = *(otherSub2 + 4);
    const uint8_t tagBall  = ReadBallPriorityByte(ball);

    if (tagOther <= tagBall) {
        WriteBallToSlot0(g_pairBroadphaseScratch, ball);
        pongBallInstance::SnapshotMatrixToGlobalSlot1(ball);
    } else {
        pongBallInstance::SnapshotMatrixToGlobalSlot0(ball);
        WriteBallToSlot1(g_pairBroadphaseScratch, ball);
    }
}

/**
 * pongBallInstance secondary-vtable scalar-deleting thunk
 * @ 0x82280890 | size: 0x8
 *
 * Already lifted above as `pongBallInstance_SecondaryDtorThunk`. The adjusted
 * `this` offset of -80 bytes matches the location of the secondary vtable
 * pointer (+80) inside the pongBallInstance layout.
 */

/**
 * pongBallAudio::ScalarDtor(int flags)   [vtable slot 0 @ 0x8228ABE0 | 0x50]
 *
 * MSVC scalar-deleting destructor: runs the inline ~pongBallAudio() body
 * (delegated to the base audControl3d destructor) and, if the low bit of
 * `flags` is set, releases the heap slab. This is the standard two-arg
 * virtual destructor thunk the compiler emits for classes with virtual dtors.
 *
 * Matches the `pongPaddle::ScalarDtor(int flags)` pattern already used
 * elsewhere in the project (pong_network_classes.cpp).
 */
void pongBallAudio::ScalarDtor(int flags) {
    extern void audControl3d_Destructor(pongBallAudio* obj);  // @ 0x8228AC30
    extern void rage_free(void* ptr);                         // @ 0x820C00C0

    // Inline ~pongBallAudio() — same call the stack-scoped dtor makes.
    audControl3d_Destructor(this);

    if (flags & 1) {
        rage_free(this);
    }
}
