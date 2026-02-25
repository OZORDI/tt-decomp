// ── pongMover  [vtable @ 0x820280DC] ──────────────────────────
// Movement and positioning system for creatures.
// Handles position, velocity, matrix calculations, and movement state.
struct pongMover {
    void**      vtable;                  // +0x00
    uint8_t     _pad0x04[12];            // +0x04
    float       m_position[3];           // +0x10  Position vector (x, y, z)
    uint8_t     _pad0x1c[4];             // +0x1C
    float       m_velocity[3];           // +0x20  Velocity vector
    uint8_t     _pad0x2c[4];             // +0x2C
    float       m_acceleration[3];       // +0x30  Acceleration vector
    uint8_t     _pad0x3c[4];             // +0x3C
    uint8_t     m_bIsActive;             // +0x40  Active flag
    uint8_t     _pad0x41[3];             // +0x41
    void*       m_pCreature;             // +0x44  Pointer to creature
    float       m_rotation[3];           // +0x48  Rotation angles
    uint8_t     _pad0x54[28];            // +0x54
    void*       m_pCreaturePool;         // +0x70  Creature pool pointer
    uint8_t     m_flags;                 // +0x74  State flags
    uint8_t     _pad0x75[3];             // +0x75
    float       m_physicsVelocity[2];    // +0x78  Physics velocity (x, y)
    float       m_physicsForce[2];       // +0x80  Physics force (x, y)
    uint8_t     m_bHasTarget;            // +0x84  Has movement target
    uint8_t     m_bTargetReached;        // +0x85  Target reached flag
    uint8_t     m_bPathBlocked;          // +0x86  Path blocked flag
    uint8_t     _pad0x87[9];             // +0x87
    float       m_movementVector[4];     // +0x90  Movement direction (16 bytes)
    uint8_t     _pad0xa0[16];            // +0xA0
    uint8_t     m_bIsMoving;             // +0xB0  Currently moving flag
    uint8_t     _pad0xb1[3];             // +0xB1
    float       m_moveTimer;             // +0xB4  Movement timer
    uint8_t     _pad0xb8[20];            // +0xB8
    uint32_t    m_creatureIndex;         // +0xCC  Creature index in pool
    uint8_t     _pad0xd0[16];            // +0xD0
    uint32_t    m_timestamp1;            // +0xE0  Timestamp 1
    uint32_t    m_timestamp2;            // +0xE4  Timestamp 2
    uint8_t     _pad0xe8[32];            // +0xE8
    float       m_pathData[4];           // +0x108 Path data (16 bytes)
    float       m_targetTimer;           // +0x118 Target timer

    // ── virtual methods ──
    virtual ~pongMover();                           // [0] @ 0x820c9ef8

    // ── non-virtual methods ──
    pongMover();                                    // @ 0x820cb5f0
    void Reset(void* creatureData);                 // @ 0x820c9f40
    void CalcInitMatrix(float* outMatrix, 
                       pongMover* mover, 
                       void* creatureData);         // @ 0x820cac78
};
