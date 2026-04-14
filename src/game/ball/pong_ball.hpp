/**
 * pong_ball.hpp — Ball / paddle / robot declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <stdint.h>

// ── gdBallFudgeData  [vtable @ 0x8203E8FC] ──────────────────────────
struct gdBallFudgeData {
    void** vtable;  // +0x00

    // ── virtual methods ──
    virtual void IsApplicable();    // [20] @ 0x821d9cc0
    virtual void LoadProperties();  // [21] @ 0x821d9d08
    virtual void GetNodeTypeName(); // [22] @ 0x821d9c10
};

// ── gdBallFudgeMgr  [vtable @ 0x8203E960] ──────────────────────────
struct gdBallFudgeMgr {
    void** vtable;  // +0x00
};

// ── gdBallPredictManager  [vtable @ 0x8203DF30] ─────────────────────
struct gdBallPredictManager {
    void** vtable;  // +0x00

    // ── virtual methods ──
    virtual ~gdBallPredictManager(); // [0] @ 0x821d31c8
};

// ── gmBallNode  [3 vtables — template/MI] ───────────────────────────
struct gmBallNode {
    void** vtable;  // +0x00

    // ── virtual methods ──
    virtual ~gmBallNode(); // [0] @ 0x82280b18
    virtual const char* GetTypeName(); // [19] @ 0x82280940
    virtual bool IsEventHandled(uint32_t eventHash); // [20] @ 0x822808e8
    virtual void vfn_21(); // [21] @ 0x82280b98
    virtual void vfn_23(); // [23] @ 0x82280d48
    virtual void Reset();  // [24] @ 0x82281368
    virtual void vfn_27(); // [27] @ 0x82281630
    virtual void vfn_29(); // [29] @ 0x82281c10
    virtual void vfn_37(); // [37] @ 0x82282430
};

// ── gmBallRobot  [2 vtables — template/MI] ──────────────────────────
class gmBallRobot {
public:
    virtual ~gmBallRobot();                   // [0] @ 0x821120a8
    virtual void HandleEvent(uint16_t eventType); // [1] @ 0x82112160
    static void DestructorThunk(gmBallRobot* ptr); // @ 0x821129c8

private:
    void* m_vtable1;              // +0x000 primary vtable @ 0x82032338
    void* m_vtable2;              // +0x004 secondary vtable @ 0x82032344
    uint8_t m_padding08[8];       // +0x008
    uint8_t m_singletonData[368]; // +0x010 atSingleton base storage
    uint8_t m_audControl3d[176];  // +0x180 audControl3d storage
    void* m_pOwnedObject;         // +0x230
    uint8_t m_isActive;           // +0x21c
    uint8_t m_padding21D[3];      // +0x21d
    void* m_pCharacter;           // +0x220
    void* m_pAnimationData;       // +0x224
    void* m_pMotionClip;          // +0x228
    void* m_pAllocatedObject;     // +0x22c
    void* m_pAnimBuffer1;         // +0x238
    void* m_pAnimBuffer2;         // +0x23c
};

// ── pongBallAudio  [vtable @ 0x82057CDC] ────────────────────────────
struct pongBallAudio {
    void** vtable;  // +0x00

    // ── virtual methods ──
    virtual ~pongBallAudio(); // [0] @ 0x8228abe0
};

// ── pongBallHit  [vtable @ 0x820573A8] ──────────────────────────────
struct pongBallHit {
    void** vtable;  // +0x00
};

// ── pongBallHitData  [vtable @ 0x8203E59C] ──────────────────────────
struct pongBallHitData {
    void** vtable;              // +0x00
    uint8_t m_payloadLo[194];   // +0x04..0xC5  (serialized fields registered by LoadProperties)
    uint16_t m_flags;           // +0xC6  (W:PostLoadProperties, R:LoadProperties — bit 0x1000 = 'enabled')
    uint8_t m_payloadHi[148];   // +0xC8..0x15B (remaining serialized fields)

    // ── virtual methods ──
    virtual void PostLoadProperties();                     // [2] @ 0x821d6900
    virtual bool IsApplicable(uint32_t contextHash);       // [20] @ 0x821d65c0
    virtual void LoadProperties();                         // [21] @ 0x821d6608
    virtual const char* GetNodeTypeName();                 // [22] @ 0x821d65b0

    void Initialize();
    bool ValidateAttribute(uint32_t attributeHash);
    const char* GetEventName();

    // ── nested helper nodes ──
    struct xmlNodeStructHitZone {
        void** vtable;  // +0x00

        // ── virtual methods ──
        virtual void vfn_5(); // [5] @ 0x821d6930
        virtual void vfn_6(); // [6] @ 0x821d6940
    };

    struct xmlNodeStructSwingType {
        void** vtable;  // +0x00

        // ── virtual methods ──
        virtual void vfn_5(); // [5] @ 0x821d6948
    };
};

// ── pongBallInstance  [2 vtables — template/MI] ─────────────────────
// Confirmed methods: ActivateBall
struct pongBallInstance {
    void** vtable;                 // +0x0000  primary vtable @ 0x8205700C

    // ── named members (derived from access pattern analysis) ──
    uint32_t m_flags;              // +0x0004  (R:22) — general flag word
    uint8_t  m_isActive;           // +0x0005  R:1 W:4 — activation byte read by vfn_24 @ +85
    uint8_t  _pad0x06[2];          // +0x0006
    uint16_t m_slotIndex;          // +0x0008  R:2
    uint8_t  _pad0x0a[2];          // +0x000a
    uint32_t m_typeId;             // +0x000c  R:1
    uint32_t field_0x0010;         // +0x0010
    uint32_t m_ownerIndex;         // +0x0014  R:3 W:1
    uint32_t m_assocHandle;        // +0x0018  R:1
    uint32_t m_assocSecondary;     // +0x001c  R:1
    uint32_t field_0x0020;         // +0x0020
    uint32_t m_linkHandleA;        // +0x0024  R:1
    uint32_t m_linkHandleB;        // +0x0028  R:1
    uint16_t m_stateCode;          // +0x002c  R:1
    uint8_t  _pad0x2e[2];          // +0x002e
    uint32_t m_physicsBlockPtr;    // +0x0030  R:4 W:2
    uint32_t m_physicsSlotIndex;   // +0x0034  R:2
    float    m_speedSquared;       // +0x0038  R:6 W:2 — read @ +56 as float
    uint32_t m_lastHitContext;     // +0x003c  R:2
    uint8_t  _pad0x40[16];         // +0x0040
    uint32_t m_collisionCounter;   // +0x0050  W:2
    uint8_t  _pad0x54;             // +0x0054
    uint8_t  m_hitPolicyFlag;      // +0x0055  R:2 — read by vfn_24 as 'can activate' gate
    uint8_t  _pad0x56[2];          // +0x0056
    uint32_t m_lastImpactPtr;      // +0x0058  R:3
    uint8_t  _pad0x5c[20];         // +0x005c
    uint32_t m_pendingEventA;      // +0x0070  W:1
    uint32_t m_pendingEventB;      // +0x0074  W:1
    uint32_t m_pendingEventC;      // +0x0078  W:1
    uint8_t  _pad0x7c[68];         // +0x007c
    uint32_t m_primaryLinkPtr;     // +0x00c0  R:1 W:1
    uint32_t m_secondaryLinkPtr;   // +0x00c4  R:1 W:1
    uint8_t  _pad0xc8[16712];      // +0x00c8
    uint32_t m_runtimeStateA;      // +0x4210  W:2
    uint32_t m_runtimeStateB;      // +0x4214  W:1
    uint8_t  _pad0x4218[136];      // +0x4218
    uint32_t m_spawnOwnerPtr;      // +0x42a0  W:2 — written by ActivateBall as owner handle (vfn_24 reads as m_primaryLinkPtr at +17072)
    uint32_t m_spawnOwnerAlt;      // +0x42a4  W:1
    uint32_t m_spawnTargetPtr;     // +0x42a8  W:2 — vfn_24 reads at +17076 as m_secondaryLinkPtr
    uint32_t m_spawnTargetAlt;     // +0x42ac  W:2
    uint32_t m_spawnExtraA;        // +0x42b0  W:2
    uint32_t m_spawnExtraB;        // +0x42b4  W:2

    // ── nested POD types used by vtable methods ──
    struct Vec4 { float x, y, z, w; };               // 16-byte vector (XYZ + pad / W)
    struct Mat44 { Vec4 row0, row1, row2, row3; };   // 64-byte matrix stored @ this+0x60

    // ── virtual methods ──
    virtual ~pongBallInstance();                                        // [0] @ 0x8227f990
    virtual Mat44* GetMatrixAt60();                                      // [2] @ 0x82280028 — returns &this->m_matrix
    virtual void SetMatrixAt60(const Mat44* src);                        // [3] @ 0x82280030 — copies src into m_matrix
    virtual void GetCurrentPosition(Vec4* outPos);                       // [4] @ 0x8227ff38 — pulls position from history slot
    virtual void GetCurrentVelocity(Vec4* outVel);                       // [5] @ 0x8227ffb0 — pulls velocity from history slot
    virtual void OnEnter();                                              // [11] @ 0x822c90d0
    virtual void ProcessCollisionAndActivate(pongBallInstance* other);   // [24] @ 0x8227fa48
    virtual int  ValidateCollisionParams3(uint32_t a, uint32_t b, uint32_t c);  // [30] @ 0x8227fa28

    // ── non-virtual methods ──
    void* GetMatrix();                     // [3] @ 0x82280028
    void SetMatrix(const void* sourceData);  // [3] @ 0x82280030
    void GetPosition(void* outPosition);   // [4] @ 0x8227FF38
    void GetVelocity(void* outVelocity);   // [5] @ 0x8227FFB0
    void ProcessCollision(void* gameState);  // @ 0x8227FA48
    int ValidateCollisionParams(int param1, int param2, int param3);  // @ 0x8227FA28
    void* FindObjectByID(uint16_t searchID);  // @ 0x822C1C60
    void UpdatePhysicsState();             // @ 0x8227FDB0
    void ActivateBall(void* activationContext);  // @ 0x822801B8
    void* InitializeFromData(void* initData);  // @ 0x8227F810
    void SnapshotMatrixToGlobalSlot0(pongBallInstance* ball);  // @ 0x822CD5F0
    void SnapshotMatrixToGlobalSlot1(pongBallInstance* ball);  // @ 0x822CD690
    void* HandlePlayerHitEvent(void* eventData, void* hitContext);  // @ 0x822C3B80
    bool HandleRemotePlayerHit(void* hitContext);  // @ 0x822D5400
};

// ── pongPaddle  [vtable @ 0x82071678] ───────────────────────────────
struct pongPaddle {
    void** vtable; // +0x00

    // ── virtual methods ──
    virtual ~pongPaddle();          // [0] @ 0x823d4208
    virtual void vfn_1();           // [1] @ 0x823d4488
    virtual void ScalarDtor(int flags); // [2] @ 0x823d4140
};
