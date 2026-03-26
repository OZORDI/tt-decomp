/**
 * pong_network_classes.cpp — Network utility classes implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements 5 networking/utility classes with proper C++ semantics:
 * - FloatAverager: Utility for averaging float values over time
 * - FrameTimeEstimate: Frame timing estimation for network synchronization
 * - AckHandling: Network packet acknowledgment management
 * - pongPaddle: Paddle/racket rendering and state management
 * - plrPropMgr: Player property manager (rackets, clothing, etc)
 */

#include <stdint.h>
#include <cstring>

// Minimal definition for SinglesNetworkClient methods defined in this file.
// Full definition is in pong_network.hpp but we can't include it due to
// struct redefinition conflicts (FloatAverager, AckHandling, etc.)
struct SinglesNetworkClient {
    void** vtable;
    uint8_t _pad[0x2000];  // large enough for all offset accesses

    // Methods implemented in this file
    int NetRecvFrom(void* socket, void* buf, int len, int flags, void* from, int* fromlen);
    int NetSendTo(void* socket, const void* buf, int len, int flags, const void* to, int tolen);
    int GetInputState(int userIndex, void* state);
    int SetInputState(int userIndex, void* vibration);
    int ShowGamerCard(int userIndex, uint64_t xuid);
    void ClearNetState();
    void InitSyncState();
    void GetSessionField(uint32_t* outVal);
    void MarkTimestamp();
    bool IsSessionReady();
    bool IsStateComplete();
    void SetMode3();
    void SetMode9();
    void PostNetMessage_14395();
    void PostNetMessage_14406();
    void WriteHWReg(uint32_t regOffset, uint32_t value);
    void InitHWRegs();
    void DispatchSlot6();
};

// Forward declarations
extern "C" void rage_free(void* ptr);
extern "C" void rage_ReleaseSingleton(void* obj);
extern "C" void sysCallback_Invoke(void* obj, int flags);

// Forward declarations
extern void fsmMachine_Destroy(void* stateObj);

// External globals
extern void* g_pNetworkTimer;           // @ 0x8201A328
extern uint32_t g_playerPropType1;      // @ 0x82062D7C
extern uint32_t g_playerPropType2;      // @ 0x820693D0

////////////////////////////////////////////////////////////////////////////////
// FloatAverager @ 0x8207166C
////////////////////////////////////////////////////////////////////////////////

/**
 * FloatAverager — Utility class for averaging float values over time
 * 
 * A lightweight utility class used throughout the networking subsystem for
 * smoothing time-varying values like ping times, frame deltas, and network
 * latency measurements.
 * 
 * VTABLE VARIANTS (4 total):
 * - 0x8207166C: Primary vtable (base class)
 * - 0x8203A91C: Template instantiation variant 1
 * - 0x82070D78: Template instantiation variant 2
 * - 0x8203A910: Template instantiation variant 3
 * 
 * The multiple vtable addresses suggest this is either:
 * 1. A template class instantiated with different parameters
 * 2. Part of a multiple inheritance hierarchy
 * 3. Used in different compilation units with different linkage
 * 
 * Structure (minimal - only destructor visible):
 * +0x00: vtable pointer
 * +0x04+: Unknown fields (likely: sum, count, window_size, current_avg)
 * 
 * The actual field layout needs to be discovered through:
 * - Finding constructor implementations
 * - Analyzing methods that access fields beyond vtable
 * - Examining usage patterns in networking code
 */
struct FloatAverager {
    void** vtable;  // +0x00

    // Fields to be discovered:
    // float m_sum;           // Running sum of values
    // uint32_t m_count;      // Number of samples
    // uint32_t m_windowSize; // Maximum samples to average
    // float m_currentAvg;    // Cached average value

    ~FloatAverager();
};

/**
 * FloatAverager::~FloatAverager @ 0x821A7AA0 | size: 0x48
 * 
 * Primary destructor - sets vtable to primary address and conditionally frees.
 * 
 * @param self Pointer to FloatAverager instance
 * @param flags Destruction flags (bit 0: free memory if set)
 */
void FloatAverager_Destroy(FloatAverager* self, int flags) {
    // Set vtable to primary FloatAverager vtable @ 0x8207166C
    // This is the base class vtable address
    self->vtable = (void**)0x8207166C;
    
    // If bit 0 is set in flags, free the object memory
    // This follows the standard RAGE engine destruction pattern
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * FloatAverager::~FloatAverager (variant 1) @ 0x821A7AE8 | size: 0x48
 * 
 * Destructor variant for template instantiation or MI scenario.
 * Uses alternate vtable @ 0x8203A91C.
 * 
 * Assembly calculation:
 *   lis r11, -32252  ; Load high 16 bits
 *   addi r11, r11, -22244  ; Add low 16 bits
 *   Result: 0x8203A91C
 */
void FloatAverager_Destroy_7AE8(FloatAverager* self, int flags) {
    // Set vtable to alternate FloatAverager vtable @ 0x8203A91C
    // Python verification:
    //   r11 = (-32252 << 16) + (-22244)
    //   print(f"0x{r11 & 0xFFFFFFFF:08X}")  # 0x8203A91C
    self->vtable = (void**)0x8203A91C;
    
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * FloatAverager::~FloatAverager (variant 2) @ 0x823CD538 | size: 0x48
 * 
 * Destructor variant with vtable @ 0x82070D78.
 * Likely used in a different inheritance context.
 */
void FloatAverager_Destroy_D538(FloatAverager* self, int flags) {
    // Set vtable to alternate FloatAverager vtable @ 0x82070D78
    self->vtable = (void**)0x82070D78;
    
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * FloatAverager::~FloatAverager (variant 3) @ 0x823D3EE8 | size: 0x48
 * 
 * Destructor variant with vtable @ 0x8203A910.
 * 
 * Assembly calculation:
 *   lis r11, -32252  ; Load high 16 bits
 *   addi r11, r11, -22256  ; Add low 16 bits
 *   Result: 0x8203A910
 */
void FloatAverager_Destroy_3EE8(FloatAverager* self, int flags) {
    // Set vtable to alternate FloatAverager vtable @ 0x8203A910
    // Python verification:
    //   r11 = (-32252 << 16) + (-22256)
    //   print(f"0x{r11 & 0xFFFFFFFF:08X}")  # 0x8203A910
    self->vtable = (void**)0x8203A910;
    
    if (flags & 0x1) {
        rage_free(self);
    }
}

////////////////////////////////////////////////////////////////////////////////
// FrameTimeEstimate @ 0x82071660
////////////////////////////////////////////////////////////////////////////////

/**
 * FrameTimeEstimate — Frame timing estimation for network sync
 * 
 * Estimates frame timing for network synchronization. Has two vtable
 * pointers, likely for multiple inheritance or composition pattern.
 */
struct FrameTimeEstimate {
    void** vtable1;  // +0x00 @ 0x82071660
    void** vtable2;  // +0x04 @ 0x8207166C
    // Additional timing fields would follow
};

/**
 * FrameTimeEstimate::~FrameTimeEstimate @ 0x823D39F8 | size: 0x54
 * 
 * Destructor - sets both vtable pointers and conditionally frees memory.
 */
void FrameTimeEstimate_Destroy(FrameTimeEstimate* self, int flags) {
    // Set both vtable pointers
    self->vtable1 = (void**)0x82071660;  // lis(-32249)+5728
    self->vtable2 = (void**)0x8207166C;  // lis(-32249)+5740
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free(self);
    }
}

////////////////////////////////////////////////////////////////////////////////
// AckHandling @ 0x82071654
////////////////////////////////////////////////////////////////////////////////

/**
 * AckHandling — Network packet acknowledgment handler
 * 
 * Manages acknowledgment of network packets, tracking sequence numbers
 * and timing information for reliable network communication.
 * 
 * Structure layout (inferred from usage):
 * +0x00: vtable pointer
 * +0x04-0x0F: unknown fields (12 bytes)
 * +0x10-0x5F: packet pointer array (20 pointers, 80 bytes)
 * +0x60: packet count (uint32_t)
 */
struct AckHandling {
    void** vtable;              // +0x00
    uint8_t _pad0[12];          // +0x04
    void* m_packets[20];        // +0x10  array of packet pointers (80 bytes)
    uint32_t m_packetCount;     // +0x60  number of packets in array
    uint8_t _pad1[32];          // +0x64  additional fields
    
    // Packet structure (inferred from usage)
    struct Packet {
        uint8_t _pad0[16];
        uint16_t m_sequenceNum;  // +0x10
        uint8_t m_bAcked;        // +0x12
        uint8_t _pad1;
        float m_timestamp;       // +0x14
    };
};

/**
 * AckHandling::~AckHandling @ 0x823D3480 | size: 0x50
 * 
 * Destructor - calls cleanup function then conditionally frees memory.
 */
void AckHandling_Destroy(AckHandling* self, int flags) {
    // Call cleanup function first
    extern void AckHandling_34D0_fw(AckHandling* self);
    AckHandling_34D0_fw(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * AckHandling::Cleanup @ 0x823D34D0 | size: 0x5C
 * 
 * Internal cleanup - releases packet resources.
 * Called by destructor to clean up internal state.
 */
void AckHandling_34D0_fw(AckHandling* self) {
    // Implementation would release packet memory
    // Stub for now - actual implementation needs packet management
}

/**
 * AckHandling::ProcessSequence @ 0x823D3740 | size: 0xE8
 * 
 * Processes acknowledgment sequence numbers.
 * Removes packets from the queue that have been acknowledged.
 * 
 * @param sequenceInfo Pointer to sequence number info (uint16_t at offset 0)
 */
void AckHandling_ProcessSequence(AckHandling* self, void* sequenceInfo) {
    if (self->m_packetCount <= 0) {
        return;
    }
    
    uint16_t targetSeq = *(uint16_t*)sequenceInfo;
    int writeIdx = 0;
    
    // Iterate through all packets
    for (int i = 0; i < (int)self->m_packetCount; i++) {
        AckHandling::Packet* packet = (AckHandling::Packet*)self->m_packets[i];
        
        // Check if packet is marked as acknowledged
        if (packet->m_bAcked) {
            uint16_t packetSeq = packet->m_sequenceNum;
            
            // Check if sequence number is less than or equal to target
            int16_t diff = (int16_t)(targetSeq - packetSeq);
            bool shouldRemove = (diff >= 0);
            
            if (shouldRemove) {
                // Release packet
                sysCallback_Invoke(packet, 0);
                
                // Remove from array
                extern void AckHandling_3828(AckHandling* self, void* packet);
                AckHandling_3828(self, packet);
                
                // Compact array
                self->m_packetCount--;
                if (writeIdx < (int)self->m_packetCount) {
                    self->m_packets[writeIdx] = self->m_packets[self->m_packetCount];
                }
                writeIdx--;
                i--;
            }
        }
        writeIdx++;
    }
}

/**
 * AckHandling::Update @ 0x823D3530 | size: 0x10C
 * 
 * Updates acknowledgment state, checking for timeouts.
 * Removes packets that have exceeded their timeout threshold.
 * 
 * @param currentTime Current network time (float)
 */
void AckHandling_3530_w(AckHandling* self, float currentTime) {
    // Get network timer and call its update method
    if (g_pNetworkTimer) {
        void** vtable = *(void***)g_pNetworkTimer;
        typedef void (*UpdateFn)(void*);
        UpdateFn update = (UpdateFn)vtable[2];
        update(g_pNetworkTimer);
    }
    
    if (self->m_packetCount <= 0) {
        return;
    }
    
    const float TIMEOUT_THRESHOLD = 5.0f;  // 5 second timeout
    int writeIdx = 0;
    
    for (int i = 0; i < (int)self->m_packetCount; i++) {
        AckHandling::Packet* packet = (AckHandling::Packet*)self->m_packets[i];
        
        float timeDiff = currentTime - packet->m_timestamp;
        bool timedOut = (timeDiff > TIMEOUT_THRESHOLD);
        
        if (timedOut) {
            // Remove timed-out packet
            extern void AckHandling_3828(AckHandling* self, void* packet);
            AckHandling_3828(self, packet);
            
            self->m_packetCount--;
            if (writeIdx < (int)self->m_packetCount) {
                self->m_packets[writeIdx] = self->m_packets[self->m_packetCount];
            }
            writeIdx--;
            i--;

        }
        writeIdx++;
    }
}

/**
 * AckHandling::RemovePacket @ 0x823D3828 | size: 0xA8
 * 
 * Removes a packet from the acknowledgment queue.
 */
void AckHandling_3828(AckHandling* self, void* packet) {
    // Stub - would remove packet from internal tracking
}

/**
 * AckHandling::Initialize @ 0x8239AB18 | size: 0xBC
 * 
 * Initializes the acknowledgment handler with default values.
 * Sets up timeout values, clears state, and initializes packet array.
 */
void AckHandling_AB18_w(AckHandling* self) {
    // Initialize timeout values at offsets +40 and +44
    // Load default timeout value from global
    float defaultTimeout = 5.0f;  // Would be loaded from global @ lis(-32164)+22840
    *(float*)((char*)self + 40) = defaultTimeout;
    *(float*)((char*)self + 44) = defaultTimeout;
    
    // Clear flags at +48, +49
    *((uint8_t*)self + 48) = 0;
    *((uint8_t*)self + 49) = 1;
    
    // Set sequence number to -1 at +36
    *(uint32_t*)((char*)self + 36) = 0xFFFFFFFF;
    
    // Release existing packet at +52 if present
    void* existingPacket = *(void**)((char*)self + 52);
    if (existingPacket) {
        void** vtable = *(void***)existingPacket;
        typedef void (*DestructorFn)(void*, int);
        DestructorFn dtor = (DestructorFn)vtable[20];
        dtor(existingPacket, 0);
        *(void**)((char*)self + 52) = nullptr;
    }
    
    // Clear flag at +33 if set
    if (*((uint8_t*)self + 33)) {
        *(uint32_t*)((char*)self + 28) = 0;
        *((uint8_t*)self + 33) = 0;
    }
    
    // Clean up packet array at offset +72
    sysCallback_Invoke((char*)self + 72, 0);
    
    // Clear all packets in the array at offset +96
    uint32_t* packetArray = (uint32_t*)((char*)self + 96);
    uint32_t packetCount = packetArray[24];  // Count at +96 bytes from array start
    
    if (packetCount > 0) {
        for (uint32_t i = 0; i < packetCount; i++) {
            void* packet = (void*)packetArray[4 + i];  // Packets start at +16 from array base
            if (packet) {
                AckHandling_3828(self, packet);
            }
        }
        packetArray[24] = 0;  // Clear count
    }
}

////////////////////////////////////////////////////////////////////////////////
// pongPaddle @ 0x82071678
////////////////////////////////////////////////////////////////////////////////

/**
 * pongPaddle — Paddle/racket rendering and state management
 * 
 * Manages the visual representation and state of player paddles.
 * Handles rendering, physics integration, and animation.
 */
struct pongPaddle {
    void** vtable;          // +0x00
    uint8_t _pad0[4];       // +0x04
    void* m_pModel;         // +0x08
    void* m_pPhysics;       // +0x0C
    void* m_pRenderer;      // +0x10
    void* m_pAnimation;     // +0x14
};

/**
 * pongPaddle::Cleanup @ 0x823D4190 | size: 0x78
 * 
 * Cleans up paddle resources before destruction.
 * Releases renderer and other internal resources.
 */
void pongPaddle_4190_h(pongPaddle* self) {
    // Set vtable
    self->vtable = (void**)0x82071678;
    
    // Release renderer if present
    if (self->m_pRenderer) {
        void** vtable = *(void***)self->m_pRenderer;
        typedef void (*DestructorFn)(void*, int);
        DestructorFn dtor = (DestructorFn)vtable[0];
        dtor(self->m_pRenderer, 1);
        self->m_pRenderer = nullptr;
    }
}

/**
 * pongPaddle::~pongPaddle @ 0x823D4208 | size: 0x27C
 * 
 * Destructor - complex cleanup involving rendering state.
 * This is a large function that handles various rendering scenarios.
 */
void pongPaddle_Destroy(pongPaddle* self, int flags) {
    // Cleanup internal resources
    pongPaddle_4190_h(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        // Call pongLookAtDriver cleanup (base class destructor)
        extern void pongLookAtDriver_PostLoadProperties(void* obj);
        pongLookAtDriver_PostLoadProperties(self);
    }
}

/**
 * pongPaddle::ScalarDestructor @ 0x823D4488 | size: 0x54
 * 
 * Scalar destructor variant (called for array deletion).
 */
void pongPaddle_ScalarDtor(pongPaddle* self, int flags) {
    pongPaddle_4190_h(self);
    
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * pongPaddle::Render @ 0x823D4140 | size: 0x50
 * 
 * Virtual method for rendering the paddle.
 */
void pongPaddle_Init(pongPaddle* self) {
    // Stub - would render paddle geometry
}

////////////////////////////////////////////////////////////////////////////////
// plrPropMgr @ 0x820717C4
////////////////////////////////////////////////////////////////////////////////

/**
 * plrPropMgr — Player property manager
 * 
 * Manages player-specific properties and assets (rackets, clothing, etc).
 * Inherits from atSingleton for singleton pattern.
 */
struct plrPropMgr {
    void** vtable;  // +0x00
    // Additional fields would be defined based on usage
};

/**
 * plrPropMgr::~plrPropMgr @ 0x823D45C0 | size: 0x70
 * 
 * Destructor - cleans up player properties.
 * Calls cleanup method, then atSingleton destructor, then conditionally frees.
 */
void plrPropMgr_Destroy(plrPropMgr* self, int flags) {
    // Set vtable to plrPropMgr vtable @ 0x820717C4
    self->vtable = (void**)0x820717C4;
    
    // Call cleanup method
    extern void plrPropMgr_OnActivate(plrPropMgr* self);
    plrPropMgr_OnActivate(self);
    
    // Call atSingleton destructor (base class)
    self->vtable = (void**)0x82033C8C;  // atSingleton vtable
    rage_ReleaseSingleton(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * plrPropMgr::CheckPropertyType @ 0x823D4630 | size: 0x38
 * 
 * Checks if a property type matches known player property types.
 * 
 * @param propType Property type ID to check
 * @return 1 if matches, 0 otherwise
 */
uint8_t plrPropMgr_GetPropertyInfo(plrPropMgr* self, uint32_t propType) {
    // Check against first property type @ 0x82062D7C
    if (propType == g_playerPropType1) {
        return 1;
    }
    
    // Check against second property type @ 0x820693D0
    if (propType == g_playerPropType2) {
        return 1;
    }
    
    return 0;
}

/**
 * plrPropMgr::GetPropertyName @ 0x823D4668 | size: 0xC
 * 
 * Returns a pointer to the property name string.
 * Address calculation: lis(-32249)+5788 = 0x82071660 + 5788 = 0x820732BC
 */
void* plrPropMgr_PostLoadChildren(plrPropMgr* self) {
    // Calculate address: lis(-32249)+5788
    // Python verification:
    // r11 = -2113470464  # lis(-32249)
    // result = r11 + 5788
    // print(f"0x{result & 0xFFFFFFFF:08X}")  # 0x820732BC
    return (void*)0x820732BC;
}

/**
 * plrPropMgr::RegisterFields @ 0x823D4730 | size: 0x6C
 * plrPropMgr::InitAssets @ 0x823D47A0 | size: 0x58
 * plrPropMgr::ReleaseAssets @ 0x823D47F8 | size: 0x64
 * plrPropMgr::SetObjectFlags @ 0x823D4860 | size: 0x40
 * 
 * Additional virtual methods - stubs for now.
 */
void plrPropMgr_Validate(plrPropMgr* self) {}
void plrPropMgr_PostLoadSetup(plrPropMgr* self) {}
void plrPropMgr_OnActivate(plrPropMgr* self) {}
void plrPropMgr_OnDeactivate(plrPropMgr* self) {}

////////////////////////////////////////////////////////////////////////////////
// External function stubs
////////////////////////////////////////////////////////////////////////////////

// Stub for pongLookAtDriver destructor (base class of pongPaddle)
void pongLookAtDriver_PostLoadProperties(void* obj) {
    (void)obj;
    // Stub - would call base class destructor
}


////////////////////////////////////////////////////////////////////////////////
// NetDataQuery @ 0x82070D14
////////////////////////////////////////////////////////////////////////////////

/**
 * NetDataQuery — Network data query state machine
 * 
 * Manages querying and receiving network data through a hierarchical
 * state machine pattern. Contains nested state objects for different
 * query phases.
 * 
 * Structure layout (inferred from constructor):
 * +0x000: vtable pointer @ 0x82070D14
 * +0x570: state machine member (1392 bytes from start)
 * +0x5E4: nested state object (1508 bytes from start) @ 0x8207116C
 * +0x830: component at 2096 bytes
 * +0x110C: component at 4364 bytes
 */
struct NetDataQuery {
    void** vtable;  // +0x00 @ 0x82070D14
    // ... additional fields discovered through usage
};

/**
 * NetDataQuery::~NetDataQuery @ 0x823D15C8 | size: 0x5C
 * 
 * Destructor - cleans up state machine and conditionally frees memory.
 */
void NetDataQuery_Destroy(NetDataQuery* self, int flags) {
    // Set vtable to nested state vtable @ 0x8207116C
    // Python: (lis(-32249) << 16) + 4460 = 0x8207116C
    self->vtable = (void**)0x8207116C;
    
    // Call fsmMachine base destructor
    extern void fsmMachine_Destroy(void* obj);
    fsmMachine_Destroy(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * NetDataQuery::Constructor @ 0x823CA458 | size: 0x6C
 * 
 * Initializes the network data query state machine.
 * Sets up nested state objects and components.
 */
void NetDataQuery_ctor_A458(NetDataQuery* self) {
    // Set main vtable @ 0x82070D14
    // Python: (lis(-32249) << 16) + 3348 = 0x82070D14
    self->vtable = (void**)0x82070D14;
    
    // Initialize component at offset +4364
    extern void grcDevice_FinalizeRenderSetup_1(void* obj);
    grcDevice_FinalizeRenderSetup_1((char*)self + 4364);
    
    // Initialize component at offset +2096
    extern void SinglesNetworkClient_2BE8_g(void* obj);
    SinglesNetworkClient_2BE8_g((char*)self + 2096);
    
    // Initialize nested state object at offset +1508
    // Set its vtable @ 0x8207116C
    void** stateObj = (void**)((char*)self + 1508);
    *stateObj = (void**)0x8207116C;
    fsmMachine_Destroy(stateObj);
    
    // Initialize state machine member at offset +1392
    // Set its vtable @ 0x82070D78 (calculated from lis(-32249) + 3448)
    void** stateMachine = (void**)((char*)self + 1392);
    *stateMachine = (void**)0x82070D78;
    
    // Call initialization
    extern void util_AA38(NetDataQuery* self);
    util_AA38(self);
}

/**
 * NetDataQuery::Reset @ 0x823D1AD0 | size: 0x4
 * 
 * Empty virtual method (likely pure virtual in base).
 */
void NetDataQuery_Reset(NetDataQuery* self) {}

/**
 * NetDataQuery::InitializeStates @ 0x823D1628 | size: 0x19C
 * 
 * Initializes the state machine with 4 states.
 * Allocates state objects and sets up state array.
 * 
 * State 0: @ 0x820711B4
 * State 1: @ 0x820711FC
 * State 2: stateReceiveData @ 0x82071244
 * State 3: stateFinish @ 0x8207128C
 */
void NetDataQuery_Process(NetDataQuery* self) {
    // Set state ID to 4
    *(uint32_t*)((char*)self + 4) = 4;
    
    // Get allocator from TLS
    extern void* sysMemAllocator_InitMainThread();
    sysMemAllocator_InitMainThread();
    
    // Get allocator pointer from SDA @ r13+0 (0x82600000)
    extern void* g_allocator_ptr;
    void** allocator = (void**)((char*)&g_allocator_ptr);
    
    // Allocate state array (16 bytes for 4 pointers)
    void** vtable = (void**)*allocator;
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn alloc = (AllocFn)vtable[1];
    *(void***)((char*)self + 8) = (void**)alloc(*allocator, 16, 16);
    
    void** stateArray = *(void***)((char*)self + 8);
    
    // Allocate and initialize state 0 @ 0x820711B4
    sysMemAllocator_InitMainThread();
    void* state0 = alloc(*allocator, 12, 16);
    if (state0) {
        *(uint32_t*)((char*)state0 + 4) = 0;
        *(uint32_t*)((char*)state0 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state0 = (void*)0x820711B4;
    }
    stateArray[0] = state0;
    
    // Allocate and initialize state 1 @ 0x820711FC
    sysMemAllocator_InitMainThread();
    void* state1 = alloc(*allocator, 12, 16);
    if (state1) {
        *(uint32_t*)((char*)state1 + 4) = 0;
        *(uint32_t*)((char*)state1 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state1 = (void*)0x820711FC;
    }
    stateArray[1] = state1;
    
    // Allocate and initialize state 2: stateReceiveData @ 0x82071244
    sysMemAllocator_InitMainThread();
    void* state2 = alloc(*allocator, 12, 16);
    if (state2) {
        *(uint32_t*)((char*)state2 + 4) = 0;
        *(uint32_t*)((char*)state2 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state2 = (void*)0x82071244;
    }
    stateArray[2] = state2;
    
    // Allocate and initialize state 3: stateFinish @ 0x8207128C
    sysMemAllocator_InitMainThread();
    void* state3 = alloc(*allocator, 12, 16);
    if (state3) {
        *(uint32_t*)((char*)state3 + 4) = 0;
        *(uint32_t*)((char*)state3 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state3 = (void*)0x8207128C;
    }
    stateArray[3] = state3;
}

/**
 * NetDataQuery::OnComplete @ 0x823D17C8 | size: 0x84
 * NetDataQuery::GetName @ 0x823D1850 | size: 0x5C
 * 
 * Additional virtual methods - stubs for now.
 */
void NetDataQuery_OnComplete(NetDataQuery* self) {}
void NetDataQuery_GetName(NetDataQuery* self) {}

/**
 * NetDataQuery::Constructor (alternate) @ 0x823D1530 | size: 0x98
 * 
 * Simpler constructor that initializes a smaller NetDataQuery object.
 * This appears to be for a nested or derived version.
 * 
 * Structure size: 573 bytes (0x23D)
 */
void NetDataQuery_ctor_1530(NetDataQuery* self) {
    // Set vtable @ 0x8207116C
    // Python: (lis(-32249) << 16) + 4460 = 0x8207116C
    self->vtable = (void**)0x8207116C;
    
    // Initialize fields
    *(uint32_t*)((char*)self + 4) = 0;
    *(uint32_t*)((char*)self + 8) = 0;
    *(uint32_t*)((char*)self + 12) = 0xFFFFFFFF;  // -1
    *(uint32_t*)((char*)self + 16) = 0xFFFFFFFF;  // -1
    *(uint16_t*)((char*)self + 20) = 0;
    *(uint16_t*)((char*)self + 22) = 0;
    *(uint16_t*)((char*)self + 528) = 0xFFFF;  // -1
    *(uint32_t*)((char*)self + 532) = 0;
    *(uint32_t*)((char*)self + 536) = 0;
    
    // Copy 16-byte data blocks from global @ 0x8261A0C0
    // Python: (lis(-32158) << 16) + -24384 = 0x8261A0C0
    uint32_t* src = (uint32_t*)0x8261A0C0;
    uint32_t* dst1 = (uint32_t*)((char*)self + 540);
    uint32_t* dst2 = (uint32_t*)((char*)self + 556);
    
    for (int i = 0; i < 4; i++) {
        dst1[i] = src[i];
        dst2[i] = src[i];
    }
    
    *(uint8_t*)((char*)self + 572) = 0;
}

/**
 * NetDataQuery::Helper functions
 * 
 * Additional helper methods for network data query operations.
 */
void NetDataQuery_A8D8(NetDataQuery* self) {
    // Stub - helper function
}

void NetDataQuery_59F8_wrh(void* obj) {
    // Stub - initialization helper
}

void NetDataQuery_2A30_2h(void* obj) {
    // Stub - component initialization
}

void NetDataQuery_2B28_2h(void* obj) {
    // Stub - component initialization
}

////////////////////////////////////////////////////////////////////////////////
// NetStateSync @ 0x820713BC
////////////////////////////////////////////////////////////////////////////////

/**
 * NetStateSync — Network state synchronization state machine
 * 
 * Manages network state synchronization through a hierarchical state machine
 * with 7 distinct states. Allocates state objects dynamically and maintains
 * a state array for transitions.
 * 
 * Structure layout:
 * +0x00: vtable pointer @ 0x820713BC
 * +0x04: state ID (set to 7)
 * +0x08: state array pointer (points to 28-byte array of 7 state pointers)
 * 
 * State array layout (28 bytes = 7 pointers * 4 bytes):
 * +0x00: stateInit @ 0x82071404
 * +0x04: stateWaitForSyncState @ 0x8207144C
 * +0x08: stateEnterState @ 0x82071494
 * +0x0C: stateRequestSyncronization @ 0x820714DC
 * +0x10: stateSendSyncronization @ 0x82071524
 * +0x14: stateReceiveSyncronization @ 0x8207156C
 * +0x18: stateWaitTime @ 0x820715B4
 */
struct NetStateSync {
    void** vtable;          // +0x00 @ 0x820713BC
    uint32_t m_stateID;     // +0x04 (set to 7)
    void** m_pStateArray;   // +0x08 (points to 28-byte array)
};

/**
 * NetStateSync::~NetStateSync @ 0x823D1A70 | size: 0x5C
 * 
 * Destructor - cleans up state machine and conditionally frees memory.
 */
void NetStateSync_Destroy(NetStateSync* self, int flags) {
    // Set vtable @ 0x820713BC
    // Python: (lis(-32249) << 16) + 5052 = 0x820713BC
    self->vtable = (void**)0x820713BC;
    
    // Call fsmMachine base destructor
    extern void fsmMachine_Destroy(void* obj);
    fsmMachine_Destroy(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free(self);
    }
}

/**
 * NetStateSync::Initialize @ 0x823D1AD8 | size: 0x298
 * 
 * Initializes the state synchronization machine.
 * Allocates 7 state objects and sets up the state array.
 * 
 * This function allocates memory for each state and initializes them
 * with their respective vtable pointers.
 */
void NetStateSync_Process(NetStateSync* self) {
    // Set state ID to 7
    self->m_stateID = 7;
    
    // Get allocator from TLS
    extern void* sysMemAllocator_InitMainThread();
    sysMemAllocator_InitMainThread();
    
    // Get allocator pointer from SDA @ r13+0 (0x82600000)
    extern void* g_allocator_ptr;  // @ 0x82600004
    void** allocator = (void**)((char*)&g_allocator_ptr);
    
    // Allocate state array (28 bytes for 7 pointers)
    void** vtable = (void**)*allocator;
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn alloc = (AllocFn)vtable[1];
    self->m_pStateArray = (void**)alloc(*allocator, 28, 16);
    
    // Allocate and initialize each state object (12 bytes each)
    // State 0: stateInit @ 0x82071404
    sysMemAllocator_InitMainThread();
    void* state0 = alloc(*allocator, 12, 16);
    if (state0) {
        *(uint32_t*)((char*)state0 + 4) = 0;  // Clear field at +4
        *(uint32_t*)((char*)state0 + 8) = (uint32_t)(uintptr_t)self;  // Back pointer
        *(void**)state0 = (void*)0x82071404;  // vtable
    }
    self->m_pStateArray[0] = state0;
    
    // State 1: stateWaitForSyncState @ 0x8207144C
    sysMemAllocator_InitMainThread();
    void* state1 = alloc(*allocator, 12, 16);
    if (state1) {
        *(uint32_t*)((char*)state1 + 4) = 0;
        *(uint32_t*)((char*)state1 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state1 = (void*)0x8207144C;
    }
    self->m_pStateArray[1] = state1;
    
    // State 2: stateEnterState @ 0x82071494
    sysMemAllocator_InitMainThread();
    void* state2 = alloc(*allocator, 12, 16);
    if (state2) {
        *(uint32_t*)((char*)state2 + 4) = 0;
        *(uint32_t*)((char*)state2 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state2 = (void*)0x82071494;
    }
    self->m_pStateArray[2] = state2;
    
    // State 3: stateRequestSyncronization @ 0x820714DC
    sysMemAllocator_InitMainThread();
    void* state3 = alloc(*allocator, 12, 16);
    if (state3) {
        *(uint32_t*)((char*)state3 + 4) = 0;
        *(uint32_t*)((char*)state3 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state3 = (void*)0x820714DC;
    }
    self->m_pStateArray[3] = state3;
    
    // State 4: stateSendSyncronization @ 0x82071524
    sysMemAllocator_InitMainThread();
    void* state4 = alloc(*allocator, 12, 16);
    if (state4) {
        *(uint32_t*)((char*)state4 + 4) = 0;
        *(uint32_t*)((char*)state4 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state4 = (void*)0x82071524;
    }
    self->m_pStateArray[4] = state4;
    
    // State 5: stateReceiveSyncronization @ 0x8207156C
    sysMemAllocator_InitMainThread();
    void* state5 = alloc(*allocator, 12, 16);
    if (state5) {
        *(uint32_t*)((char*)state5 + 4) = 0;
        *(uint32_t*)((char*)state5 + 8) = (uint32_t)(uintptr_t)self;
        *(void**)state5 = (void*)0x8207156C;
    }
    self->m_pStateArray[5] = state5;
    
    // State 6: stateWaitTime @ 0x820715B4 (16 bytes, has float at +12)
    sysMemAllocator_InitMainThread();
    void* state6 = alloc(*allocator, 16, 16);
    if (state6) {
        *(uint32_t*)((char*)state6 + 4) = 0;
        *(uint32_t*)((char*)state6 + 8) = (uint32_t)(uintptr_t)self;
        
        // Load default timeout value from global @ lis(-32253) + -12016
        // Python: (lis(-32253) << 16) + -12016 = 0x82033110
        float defaultTimeout = *(float*)0x82033110;
        *(float*)((char*)state6 + 12) = defaultTimeout;
        
        *(void**)state6 = (void*)0x820715B4;
    }
    self->m_pStateArray[6] = state6;
}

/**
 * NetStateSync::OnComplete @ 0x823D1D70 | size: 0x60
 * NetStateSync::GetName @ 0x823D1DD0 | size: 0x64
 * 
 * Additional virtual methods - stubs for now.
 */
void NetStateSync_OnComplete(NetStateSync* self) {}
void NetStateSync_GetName(NetStateSync* self) {}

////////////////////////////////////////////////////////////////////////////////
// External function stubs
////////////////////////////////////////////////////////////////////////////////

// fsmMachine destructor — already forward-declared above
// (implementation in src/rage/fsmMachine.c; stub here for link-time)

// Stub for rage initialization
void grcDevice_FinalizeRenderSetup_1(void* obj) {
    (void)obj;
    // Stub - would initialize rage component
}

// Stub for network client initialization
void SinglesNetworkClient_2BE8_g(void* obj) {
    (void)obj;
    // Stub - would initialize network client
}

// Stub for network client helper
void SinglesNetworkClient_51C8_g_stub(void* obj) {
    (void)obj;
    // Stub - would initialize network client component
}

// Stub for utility initialization
void util_AA38_stub(void* obj) {
    (void)obj;
    // Stub - would perform utility initialization
}

// Stub for thread initialization
void* sysMemAllocator_InitMainThread_stub() {
    // Stub - would initialize thread context
    return nullptr;
}

// Global allocator pointer @ 0x82600004
void* g_allocator_ptr = nullptr;

// ═════════════════════════════════════════════════════════════════════════════
// FloatAverager — Utility class for averaging float values over time
// ═════════════════════════════════════════════════════════════════════════════
//
// FloatAverager is a lightweight utility class used throughout the networking
// subsystem for smoothing time-varying values like ping times, frame deltas,
// and network latency. It has 4 vtable variants, likely from template
// instantiations or different usage contexts.
//
// The class has 4 identical scalar destructors, one for each vtable variant:
//   - vtable @ 0x8203A910 → FloatAverager_vfn_0 @ 0x821A7AA0
//   - vtable @ 0x8203A91C → FloatAverager_vfn_0_7AE8_1 @ 0x821A7AE8
//   - vtable @ 0x82070D78 → FloatAverager_vfn_0_D538_1 @ 0x823CD538
//   - vtable @ 0x8207166C → FloatAverager_vfn_0_3EE8_1 @ 0x823D3EE8
//
// Each destructor follows the standard scalar destructor pattern:
//   1. Store the vtable pointer
//   2. Check the low bit of the flags parameter (r4 & 0x1)
//   3. If set, call rage_free to deallocate memory
//
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// FloatAverager::~FloatAverager()  [vtable slot 0 @ 0x821A7AA0]
// Vtable: 0x8203A910
//
// Scalar destructor for FloatAverager (variant 1). Restores the vtable pointer
// and optionally frees the object's memory if the deallocate flag is set.
//
// Parameters:
//   this (r3) - pointer to FloatAverager object
//   flags (r4) - destruction flags (bit 0: deallocate memory)
//
// Globals:
//   vtable @ 0x8203A910 - FloatAverager vtable variant 1
//
// Logic:
//   1. Store vtable pointer at +0x00
//   2. Extract bit 0 of flags (r4 & 0x1)
//   3. If bit 0 is set, call rage_free(this) to deallocate
// ─────────────────────────────────────────────────────────────────────────────
FloatAverager::~FloatAverager() {
    // Vtable address for variant 1
    extern void* g_FloatAverager_vtable_1;  // @ 0x8203A910

    // Restore vtable pointer
    *(void**)this = &g_FloatAverager_vtable_1;

    // Note: Memory deallocation handled by scalar destructor wrapper
}

// ─────────────────────────────────────────────────────────────────────────────
// FloatAverager scalar destructor variant 1  [@ 0x821A7AA0]
// Wrapper that calls destructor and optionally frees memory
// ─────────────────────────────────────────────────────────────────────────────
extern void* g_FloatAverager_vtable_1;  // @ 0x8203A910 (for scalar dtor)
extern "C" void FloatAverager_vfn_0(FloatAverager* thisPtr, int flags) {

    // Restore vtable
    *(void**)thisPtr = &g_FloatAverager_vtable_1;

    // If deallocate flag is set (bit 0), free the memory
    if (flags & 0x1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// FloatAverager scalar destructor variant 2  [@ 0x821A7AE8]
// Vtable: 0x8203A91C
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void FloatAverager_vfn_0_7AE8_1(FloatAverager* thisPtr, int flags) {
    extern void* g_FloatAverager_vtable_2;  // @ 0x8203A91C
    extern void rage_free(void* ptr);

    // Restore vtable
    *(void**)thisPtr = &g_FloatAverager_vtable_2;

    // If deallocate flag is set (bit 0), free the memory
    if (flags & 0x1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// FloatAverager scalar destructor variant 3  [@ 0x823CD538]
// Vtable: 0x82070D78
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void FloatAverager_vfn_0_D538_1(FloatAverager* thisPtr, int flags) {
    extern void* g_FloatAverager_vtable_3;  // @ 0x82070D78
    extern void rage_free(void* ptr);

    // Restore vtable
    *(void**)thisPtr = &g_FloatAverager_vtable_3;

    // If deallocate flag is set (bit 0), free the memory
    if (flags & 0x1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// FloatAverager scalar destructor variant 4  [@ 0x823D3EE8]
// Vtable: 0x8207166C
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void FloatAverager_vfn_0_3EE8_1(FloatAverager* thisPtr, int flags) {
    extern void* g_FloatAverager_vtable_4;  // @ 0x8207166C
    extern void rage_free(void* ptr);

    // Restore vtable
    *(void**)thisPtr = &g_FloatAverager_vtable_4;

    // If deallocate flag is set (bit 0), free the memory
    if (flags & 0x1) {
        rage_free(thisPtr);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient — Small Functions (22 non-thunk functions, ≤48B)
// ─────────────────────────────────────────────────────────────────────────────

// Xbox network/input APIs
extern "C" int NetDll_recvfrom(int handle, void* socket, void* buf, int len, int flags, void* from, int* fromlen);
extern "C" int NetDll_sendto(int handle, void* socket, const void* buf, int len, int flags, const void* to, int tolen);
extern "C" int XamInputGetState(int userIndex, int flags, void* state);
extern "C" int XamInputSetState(int userIndex, void* vibration);
extern "C" int XamShowGamerCardUIForXUID(int userIndex, uint64_t xuid);

// ── Xbox API Wrappers ────────────────────────────────────────────────────

/**
 * Network recv wrapper @ 0x822F1BF8 | size: 0x20
 * Prepends the XNet startup handle (=1) before calling NetDll_recvfrom.
 */
int SinglesNetworkClient::NetRecvFrom(void* socket, void* buf, int len, int flags, void* from, int* fromlen) {
    return NetDll_recvfrom(1, socket, buf, len, flags, from, fromlen);
}

/**
 * Network send wrapper @ 0x822F1C30 | size: 0x20
 * Prepends the XNet startup handle (=1) before calling NetDll_sendto.
 */
int SinglesNetworkClient::NetSendTo(void* socket, const void* buf, int len, int flags, const void* to, int tolen) {
    return NetDll_sendto(1, socket, buf, len, flags, to, tolen);
}

/**
 * XamInputGetState wrapper @ 0x822F7080 | size: 0xC
 * Rearranges args for XamInputGetState call.
 */
int SinglesNetworkClient::GetInputState(int userIndex, void* state) {
    return XamInputGetState(userIndex, 0, state);
}

/**
 * XamInputSetState wrapper @ 0x822F7090 | size: 0x18
 * Calls XamInputSetState with zeroed extra args.
 */
int SinglesNetworkClient::SetInputState(int userIndex, void* vibration) {
    return XamInputSetState(userIndex, vibration);
}

/**
 * XamShowGamerCardUI wrapper @ 0x822F70C0 | size: 0x8
 * Shows gamer card for a given XUID with no extra flags.
 */
int SinglesNetworkClient::ShowGamerCard(int userIndex, uint64_t xuid) {
    return XamShowGamerCardUIForXUID(userIndex, xuid);
}

// ── Field Accessors ──────────────────────────────────────────────────────

/**
 * SinglesNetworkClient::ClearNetState @ 0x822F4E48 | size: 0x10
 * Zeroes the active flag (byte at +0) and state field (uint32 at +64).
 */
void SinglesNetworkClient::ClearNetState() {  // 4E48_p44
    *(uint8_t*)((char*)this + 0) = 0;
    *(uint32_t*)((char*)this + 64) = 0;
}

/**
 * SinglesNetworkClient::InitSyncState @ 0x822F9E50 | size: 0x14
 * Initializes sync fields: field_0=0, field_4=-1 (invalid handle).
 */
void SinglesNetworkClient::InitSyncState() {  // 9E50_p39
    *(uint32_t*)((char*)this + 0) = 0;
    *(int32_t*)((char*)this + 4) = -1;
}

/**
 * SinglesNetworkClient::GetSessionField @ 0x822F6690 | size: 0x10
 * Reads this->field_0x34->field_0xC and stores to output.
 */
void SinglesNetworkClient::GetSessionField(uint32_t* outVal) {  // 6690_p39
    void* session = *(void**)((char*)this + 0x34);
    *outVal = *(uint32_t*)((char*)session + 0xC);
}

/**
 * SinglesNetworkClient::MarkTimestamp @ 0x822F3328 | size: 0x1C
 * Sets the "dirty" byte at +0x175D to 1 and copies a global
 * timestamp value into field +0x1760.
 */
void SinglesNetworkClient::MarkTimestamp() {  // 3328_p33
    extern uint32_t g_networkTimestamp;  // global timestamp
    *(uint8_t*)((char*)this + 0x175D) = 1;
    *(uint32_t*)((char*)this + 0x1760) = g_networkTimestamp;
}

// ── Bool Queries ─────────────────────────────────────────────────────────

/**
 * SinglesNetworkClient::IsSessionReady @ 0x822F67C0 | size: 0x1C
 * Returns true if field_0x18 >= 0 (valid session handle).
 */
bool SinglesNetworkClient::IsSessionReady() {  // 67C0_w
    return *(int32_t*)((char*)this + 0x18) >= 0;
}

/**
 * SinglesNetworkClient::IsStateComplete @ 0x822F72B8 | size: 0x1C
 * Returns true if field_0xC0 == 3 (state machine completed).
 */
bool SinglesNetworkClient::IsStateComplete() {  // 72B8_g
    return *(int32_t*)((char*)this + 0xC0) == 3;
}

// ── Const-arg Tail-calls ─────────────────────────────────────────────────

extern "C" void SinglesNetworkClient_SetMode(void* obj, int mode);  // @ 0x822F23A0
extern "C" void PostPageGroupMessage(int code, int param1, int param2, int param3);

/**
 * SinglesNetworkClient::SetMode3 @ 0x822F66B8 | size: 0xC
 * Sets network mode to 3 on the session sub-object at this+36.
 */
void SinglesNetworkClient::SetMode3() {  // 66B8_p33
    SinglesNetworkClient_SetMode((char*)this + 36, 3);
}

/**
 * SinglesNetworkClient::SetMode9 @ 0x822F66C8 | size: 0xC
 * Sets network mode to 9.
 */
void SinglesNetworkClient::SetMode9() {  // 66C8_p33
    SinglesNetworkClient_SetMode((char*)this + 36, 9);
}

/**
 * SinglesNetworkClient::PostMessage_6518 @ 0x822F6518 | size: 0x14
 * Posts page group message 14395 with param 64.
 */
void SinglesNetworkClient::PostNetMessage_14395() {  // 6518_p39
    PostPageGroupMessage(14395, 64, 0, 0);
}

/**
 * SinglesNetworkClient::PostMessage_66D8 @ 0x822F66D8 | size: 0x14
 * Posts page group message 14406 with param 64.
 */
void SinglesNetworkClient::PostNetMessage_14406() {  // 66D8_p33
    PostPageGroupMessage(14406, 64, 0, 0);
}

// ── MMIO Register Writes (Xbox hardware) ─────────────────────────────────

/**
 * SinglesNetworkClient::WriteHWReg @ 0x822F4050 | size: 0x14
 * Computes hardware register address from r3, writes r4 with eieio barrier.
 */
void SinglesNetworkClient::WriteHWReg(uint32_t regOffset, uint32_t value) {  // 4050_p39
    volatile uint32_t* reg = (volatile uint32_t*)(uintptr_t)(0x7FC80000 + regOffset);
    *reg = value;
    // eieio — enforce in-order I/O (PowerPC memory barrier)
}

/**
 * SinglesNetworkClient::InitHWRegs @ 0x822F69E8 | size: 0x20
 * Writes two hardware register values (7 and 2048) with eieio barriers.
 */
void SinglesNetworkClient::InitHWRegs() {  // 69E8_p39
    volatile uint32_t* reg1 = (volatile uint32_t*)(uintptr_t)0x7FC80100;
    volatile uint32_t* reg2 = (volatile uint32_t*)(uintptr_t)0x7FC80104;
    *reg1 = 7;
    // eieio
    *reg2 = 2048;
    // eieio
}

/**
 * SinglesNetworkClient::VtableDispatch @ 0x822F9B10 | size: 0x10
 * Loads vtable, calls slot 6. Generic vtable forwarding thunk.
 */
void SinglesNetworkClient::DispatchSlot6() {  // 9B10_2hr
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[6])(this);
}

// ── Network Bitstream Accessors (SinglesNetworkClient) ───────────────────
// These functions provide typed read/write access to individual fields within
// the network bitstream.  The stream object has:
//   +0x00: data pointer array (vtable-like, holds pointers to entries)
//   +0x04: entry count (uint16)
//   +0x0C: current read index
//   +0x10: total size in bits
//   +0x14: capacity / upper bound
//   +0x18: reserved
//   +0x1C: read cursor position
//   +0x20: write cursor position
//   +0x24: flags field (16-bit packed bitfield)
//
// The read/write helpers (8DF8, 0448) operate on the cursor at +28 (read)
// and +32 (write) respectively.  Callers temporarily set these cursors to
// the desired field offset before calling the helper, then restore them.

// Forward declarations for helpers already lifted elsewhere
extern int  SinglesNetworkClient_8CC0_w(void* self);
extern void SinglesNetworkClient_8DF8_g(void* client, void* buf, int size);
extern void SinglesNetworkClient_0448_g(void* self, uint32_t value, int bits);
extern void SinglesNetworkClient_8AE0_g(void* self);
extern void SinglesNetworkClient_0268_g(void* self);
extern void* xam_singleton_init_8D60();

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetEntryName @ 0x82238600 | size: 0x74
// Looks up a registered entry by index and returns its name string.
// Returns the name at entry[index]+4, or an empty string if null,
// or a fallback error string if index is negative.
// ─────────────────────────────────────────────────────────────────────────────
const char* SinglesNetworkClient::GetEntryName(int entryIndex) {
    static const char* const s_emptyString = "";  // @ 0x82027423

    int slotIndex = SinglesNetworkClient_8CC0_w(this);

    if (slotIndex >= 0) {
        void* registry = xam_singleton_init_8D60();
        // registry+0 = pointer to entry table
        void** entryTable = *(void***)registry;
        // Index into the table: entryTable[slotIndex]
        void* entry = entryTable[slotIndex];
        // Name string at entry+4
        const char* name = *(const char**)((uint8_t*)entry + 4);
        if (name == nullptr) {
            return s_emptyString;
        }
        return name;
    }

    // Negative index — return fallback string fragment
    // @ 0x8204EF04: "vice, userIndex[%d], fileSize[%d]..."
    return (const char*)0x8204EF04;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::InitBitstreamState @ 0x82238678 | size: 0x74
// Initializes the bitstream object: zeroes all cursor/state fields,
// sets the type tag to the cmRefreshableCtor vtable pointer,
// resets the data buffer via two calls to ResetReadCursor (0268),
// and sets the capacity to 8128 bits.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::InitBitstreamState() {
    // Store type tag from cmRefreshableCtor vtable @ 0x820533CC
    uint32_t typeTag = *(uint32_t*)0x820533CC;
    *(uint32_t*)((uint8_t*)this + 36) = typeTag;

    // Zero all primary state fields
    *(uint32_t*)((uint8_t*)this + 0) = 0;   // data pointer
    *(uint32_t*)((uint8_t*)this + 4) = 0;   // entry count
    *(uint32_t*)((uint8_t*)this + 8) = 0;   // field_08
    *(uint32_t*)((uint8_t*)this + 12) = 0;  // read index
    *(uint32_t*)((uint8_t*)this + 16) = 0;  // total size
    *(uint32_t*)((uint8_t*)this + 24) = 0;  // reserved
    *(uint32_t*)((uint8_t*)this + 28) = 0;  // read cursor
    *(uint32_t*)((uint8_t*)this + 32) = 0;  // write cursor
    *(uint32_t*)((uint8_t*)this + 20) = 0;  // capacity (set below)

    // Reset the read cursor state twice (paired init pattern)
    SinglesNetworkClient_0268_g(this);
    SinglesNetworkClient_0268_g(this);

    // Set capacity to 8128 bits (1016 bytes = maximum payload)
    *(uint32_t*)((uint8_t*)this + 20) = 8128;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::WriteAlignedField @ 0x822386F0 | size: 0x68
// Writes a 14-bit value into the stream at offset 16, merging the low
// 14 bits of the input with the existing high bits.  Uses temporary
// cursor save/restore to avoid corrupting the main read/write position.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::WriteAlignedField(uint32_t fieldValue) {
    // Flush any pending writes
    SinglesNetworkClient_8AE0_g(this);

    // Save current cursors
    uint32_t savedReadCursor  = *(uint32_t*)((uint8_t*)this + 28);
    uint32_t savedWriteCursor = *(uint32_t*)((uint8_t*)this + 32);

    // Read the existing 16-bit value at bit offset 16
    uint32_t existingValue = 0;
    *(uint32_t*)((uint8_t*)this + 28) = 16;  // set read cursor to offset 16
    SinglesNetworkClient_8DF8_g(this, &existingValue, 16);

    // Merge: keep high 2 bits from existing, replace low 14 bits with fieldValue
    // Python-verified: rlwimi mask(18,31) = 0x3FFF
    uint32_t merged = (existingValue & 0xFFFFC000) | (fieldValue & 0x3FFF);

    // Restore read cursor
    *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;

    // Write the merged value back at bit offset 16
    *(uint32_t*)((uint8_t*)this + 32) = 16;  // set write cursor to offset 16
    SinglesNetworkClient_0448_g(this, merged, 16);

    // Restore write cursor
    *(uint32_t*)((uint8_t*)this + 32) = savedWriteCursor;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadAlignedField @ 0x82238758 | size: 0x7C
// Reads the 14-bit aligned field value from the stream at offset 16.
// Returns the low 14 bits of the stored value, or 16383 (0x3FFF) if the
// stream doesn't have enough data (aligned size < 48 bits).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient::ReadAlignedField() {
    uint32_t result = 0;
    uint32_t totalBits = *(uint32_t*)((uint8_t*)this + 16);

    // Align to 8-byte boundary: (totalBits + 7) & ~7
    // Python-verified: rlwinm(v,0,0,28) mask = 0xFFFFFFF8
    uint32_t alignedSize = (totalBits + 7) & 0xFFFFFFF8;

    if (alignedSize >= 48) {
        // Save read cursor, set to offset 16, read 16 bits
        uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
        *(uint32_t*)((uint8_t*)this + 28) = 16;
        SinglesNetworkClient_8DF8_g(this, &result, 16);
        *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;

        // Extract low 14 bits
        // Python-verified: clrlwi(v,18) = v & 0x3FFF
        return result & 0x3FFF;
    }

    // Not enough data — return maximum 14-bit value
    return 0x3FFF;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadRawField32 @ 0x822387D8 | size: 0x70
// Reads the raw 32-bit value from the stream at bit offset 32.
// Returns 0 if the stream doesn't have enough data (aligned size < 48).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t SinglesNetworkClient::ReadRawField32() {
    uint32_t result = 0;
    uint32_t totalBits = *(uint32_t*)((uint8_t*)this + 16);

    // Align to 8-byte boundary
    uint32_t alignedSize = (totalBits + 7) & 0xFFFFFFF8;

    if (alignedSize >= 48) {
        // Save read cursor, set to offset 32, read 16 bits
        uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
        *(uint32_t*)((uint8_t*)this + 28) = 32;
        SinglesNetworkClient_8DF8_g(this, &result, 16);
        *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;
    }

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::ReadFieldUint16 @ 0x82238848 | size: 0x74
// Reads a 16-bit unsigned value from the stream at bit offset 0.
// Returns the low 16 bits, or 0 if stream has insufficient data.
// ─────────────────────────────────────────────────────────────────────────────
uint16_t SinglesNetworkClient::ReadFieldUint16() {
    uint32_t result = 0;
    uint32_t totalBits = *(uint32_t*)((uint8_t*)this + 16);

    // Align to 8-byte boundary
    uint32_t alignedSize = (totalBits + 7) & 0xFFFFFFF8;

    if (alignedSize >= 48) {
        // Save read cursor, set to offset 0, read 16 bits
        uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
        *(uint32_t*)((uint8_t*)this + 28) = 0;
        SinglesNetworkClient_8DF8_g(this, &result, 16);
        *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;

        // Mask to 16 bits
        // Python-verified: clrlwi(v,16) = v & 0xFFFF
        return (uint16_t)(result & 0xFFFF);
    }

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetReliableFlag @ 0x822388C0 | size: 0x80
// Sets or clears the "reliable" flag (bit 15 / 0x8000) in the 16-bit
// header field at stream offset 16.  If enable is true, sets the bit;
// otherwise clears it.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::SetReliableFlag(bool enable) {
    // Flush pending writes
    SinglesNetworkClient_8AE0_g(this);

    // Read current 16-bit header at offset 16
    uint32_t headerValue = 0;
    uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
    *(uint32_t*)((uint8_t*)this + 28) = 16;
    SinglesNetworkClient_8DF8_g(this, &headerValue, 16);
    *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;

    uint32_t updatedValue;
    if (enable) {
        // Set bit 15: ori with 0x8000
        updatedValue = headerValue | 0x8000;
    } else {
        // Clear bit 15
        // Python-verified: rlwinm(v,0,17,15) mask = 0xFFFF7FFF
        updatedValue = headerValue & 0xFFFF7FFF;
    }

    // Write back at offset 16
    uint32_t savedWriteCursor = *(uint32_t*)((uint8_t*)this + 32);
    *(uint32_t*)((uint8_t*)this + 32) = 16;
    SinglesNetworkClient_0448_g(this, updatedValue, 16);
    *(uint32_t*)((uint8_t*)this + 32) = savedWriteCursor;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetReliableFlag @ 0x82238940 | size: 0x8C
// Reads the "reliable" flag (bit 15 / 0x8000) from the 16-bit header
// field at stream offset 16.  Returns true if the bit is set.
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient::GetReliableFlag() {
    uint32_t headerValue = 0;
    uint32_t totalBits = *(uint32_t*)((uint8_t*)this + 16);

    // Align to 8-byte boundary
    uint32_t alignedSize = (totalBits + 7) & 0xFFFFFFF8;

    if (alignedSize >= 48) {
        // Read 16-bit header at offset 16
        uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
        *(uint32_t*)((uint8_t*)this + 28) = 16;
        SinglesNetworkClient_8DF8_g(this, &headerValue, 16);
        *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;
    }

    // Isolate bit 15 and convert to bool
    // Python-verified: rlwinm(v,0,16,16) mask = 0x8000
    return (headerValue & 0x8000) != 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::SetOrderedFlag @ 0x822389D0 | size: 0x80
// Sets or clears the "ordered delivery" flag (bit 14 / 0x4000) in the
// 16-bit header field at stream offset 16.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient::SetOrderedFlag(bool enable) {
    // Flush pending writes
    SinglesNetworkClient_8AE0_g(this);

    // Read current 16-bit header at offset 16
    uint32_t headerValue = 0;
    uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
    *(uint32_t*)((uint8_t*)this + 28) = 16;
    SinglesNetworkClient_8DF8_g(this, &headerValue, 16);
    *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;

    uint32_t updatedValue;
    if (enable) {
        // Set bit 14: ori with 0x4000
        updatedValue = headerValue | 0x4000;
    } else {
        // Clear bit 14
        // Python-verified: rlwinm(v,0,18,16) mask = 0xFFFFBFFF
        updatedValue = headerValue & 0xFFFFBFFF;
    }

    // Write back at offset 16
    uint32_t savedWriteCursor = *(uint32_t*)((uint8_t*)this + 32);
    *(uint32_t*)((uint8_t*)this + 32) = 16;
    SinglesNetworkClient_0448_g(this, updatedValue, 16);
    *(uint32_t*)((uint8_t*)this + 32) = savedWriteCursor;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient::GetOrderedFlag @ 0x82238A50 | size: 0x8C
// Reads the "ordered delivery" flag (bit 14 / 0x4000) from the 16-bit
// header field at stream offset 16.  Returns true if the bit is set.
// ─────────────────────────────────────────────────────────────────────────────
bool SinglesNetworkClient::GetOrderedFlag() {
    uint32_t headerValue = 0;
    uint32_t totalBits = *(uint32_t*)((uint8_t*)this + 16);

    // Align to 8-byte boundary
    uint32_t alignedSize = (totalBits + 7) & 0xFFFFFFF8;

    if (alignedSize >= 48) {
        // Read 16-bit header at offset 16
        uint32_t savedReadCursor = *(uint32_t*)((uint8_t*)this + 28);
        *(uint32_t*)((uint8_t*)this + 28) = 16;
        SinglesNetworkClient_8DF8_g(this, &headerValue, 16);
        *(uint32_t*)((uint8_t*)this + 28) = savedReadCursor;
    }

    // Isolate bit 14 and convert to bool
    // Python-verified: rlwinm(v,0,17,17) mask = 0x4000
    return (headerValue & 0x4000) != 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolderBase::~pongNetMessageHolderBase  [vtable slot 0 @ 0x823B5890]
// Destructor: resets vtable, decrements global live-object count, conditionally frees.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolderBase::~pongNetMessageHolderBase @ 0x823B5890 | size: 0x58
 *
 * Virtual destructor for the base class of all pongNetMessageHolder objects.
 * Sets vtable to pongNetMessageHolderBase (0x8206FA88), decrements the global
 * live-object count at 0x826066A4, and frees memory if bit 0 of flags is set.
 */
void pongNetMessageHolderBase_Destroy(void* self, int flags) {
    // Reset vtable to pongNetMessageHolderBase @ 0x8206FA88
    *(void**)self = (void*)0x8206FA88;

    // Decrement global live-object reference count @ 0x826066A4
    uint32_t* refCount = (uint32_t*)0x826066A4;
    *refCount = *refCount - 1;

    // Free memory if bit 0 of flags is set (scalar delete)
    if (flags & 1) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::SetNetIndex  @ 0x82118BB0
// Grows the dual send/recv buffers to hold the new index, using power-of-2
// rounding for the capacity.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::SetNetIndex @ 0x82118BB0 | size: 0xA8
 *
 * Sets the active network index (byte at +424) and ensures the internal
 * dual buffer pair at +208/+212 has sufficient capacity (+425).
 * If the new index exceeds current capacity, allocates new buffers rounded
 * up to the next power of 2 (clamped to 8 bits), copies existing data, and
 * replaces both buffer pointers.
 */
void pongNetMessageHolder_SetNetIndex(void* self, uint8_t netIndex) {
    uint8_t* obj = (uint8_t*)self;
    uint8_t currentCapacity = *(obj + 425);

    if (netIndex > currentCapacity) {
        // Round up to next power of 2 (8-bit)
        uint32_t newCap = netIndex;
        newCap |= newCap >> 1;
        newCap |= newCap >> 2;
        newCap |= newCap >> 4;
        newCap |= newCap >> 8;
        newCap |= newCap >> 16;
        newCap = (newCap + 1) & 0xFF;

        // Allocate new send buffer
        void* newSendBuf = xe_EC88(newCap);

        // Allocate new recv buffer
        void* newRecvBuf = xe_EC88(newCap);

        // Copy existing send buffer contents
        if (newSendBuf) {
            memcpy(newSendBuf, *(void**)(obj + 208), currentCapacity);
        }

        // Copy existing recv buffer contents
        if (newRecvBuf) {
            memcpy(newRecvBuf, *(void**)(obj + 212), currentCapacity);
        }

        // Replace buffer pointers
        *(void**)(obj + 208) = newSendBuf;
        *(void**)(obj + 212) = newRecvBuf;

        // Store new capacity
        *(obj + 425) = (uint8_t)newCap;
    }

    // Always store the active index
    *(obj + 424) = netIndex;
}

// Forward declaration for xe_EC88 (memory allocator)
extern void* xe_EC88(uint32_t size);

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::UpdateNetIndices  @ 0x821190A0
// Queries a vtable slot 4 result from r4, then calls SetNetIndex twice.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::UpdateNetIndices @ 0x821190A0 | size: 0x58
 *
 * Reads a network index by calling virtual slot 4 on the object at r4+4,
 * then calls SetNetIndex with that value followed by a second call with 0.
 * This sets up the send/recv buffers for a new connection and then resets
 * the active index.
 */
void pongNetMessageHolder_UpdateNetIndices(void* self, void* connectionInfo) {
    // Get the connection object at connectionInfo+4
    void* connection = *(void**)((uint8_t*)connectionInfo + 4);

    // Call vtable slot 4 to get the network index (returns byte in low 8 bits)
    typedef uint32_t (*GetIndexFn)(void*);
    void** vtable = *(void***)connection;
    uint8_t index = (uint8_t)((GetIndexFn)vtable[4])(connection);

    // Set up buffers for this index
    pongNetMessageHolder_SetNetIndex(self, index);

    // Reset active index to 0
    pongNetMessageHolder_SetNetIndex(self, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::ResizeBuffer16  @ 0x8212D2E8
// Resizes a buffer with 16-byte elements: frees old, allocates count*16.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::ResizeBuffer16 @ 0x8212D2E8 | size: 0x74
 *
 * Manages a dynamically-sized buffer stored as {dataPtr, sizeField, count}.
 * Element size is 16 bytes. If count changes:
 *   - Frees existing buffer at +0 if the old count (+8) was non-zero
 *   - Clears dataPtr (+0) and sizeField (+4)
 *   - Allocates count * 16 bytes (capped at 0x0FFFFFFF elements)
 *   - Stores new buffer pointer at +0 and count at +8
 */
void pongNetMessageHolder_ResizeBuffer16(void* self, uint32_t count) {
    uint8_t* obj = (uint8_t*)self;
    uint32_t oldCount = *(uint32_t*)(obj + 8);

    // Free existing buffer if any
    if (oldCount != 0) {
        void* oldBuf = *(void**)(obj + 0);
        rage_free(oldBuf);
        *(void**)(obj + 0) = nullptr;
        *(uint32_t*)(obj + 4) = 0;
    }

    if (count != 0) {
        *(uint32_t*)(obj + 8) = count;

        // Compute allocation size: count * 16
        // Overflow guard: if count > 0x0FFFFFFF, request (uint32_t)-1
        uint32_t allocSize = count * 16;
        if (count > 0x0FFFFFFF) {
            allocSize = (uint32_t)-1;
        }

        void* newBuf = xe_EC88(allocSize);
        *(void**)(obj + 0) = newBuf;
    } else {
        *(uint32_t*)(obj + 8) = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::ResizeBuffer4  @ 0x82133EB8
// Resizes a buffer with 4-byte elements: frees old, allocates count*4.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::ResizeBuffer4 @ 0x82133EB8 | size: 0x74
 *
 * Identical pattern to ResizeBuffer16 but with 4-byte element size.
 * Element overflow cap at 0x3FFFFFFF.
 */
void pongNetMessageHolder_ResizeBuffer4(void* self, uint32_t count) {
    uint8_t* obj = (uint8_t*)self;
    uint32_t oldCount = *(uint32_t*)(obj + 8);

    // Free existing buffer if any
    if (oldCount != 0) {
        void* oldBuf = *(void**)(obj + 0);
        rage_free(oldBuf);
        *(void**)(obj + 0) = nullptr;
        *(uint32_t*)(obj + 4) = 0;
    }

    if (count != 0) {
        *(uint32_t*)(obj + 8) = count;

        // Compute allocation size: count * 4
        // Overflow guard: if count > 0x3FFFFFFF, request (uint32_t)-1
        uint32_t allocSize = count * 4;
        if (count > 0x3FFFFFFF) {
            allocSize = (uint32_t)-1;
        }

        void* newBuf = xe_EC88(allocSize);
        *(void**)(obj + 0) = newBuf;
    } else {
        *(uint32_t*)(obj + 8) = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::InitOperatorArray  @ 0x821D6D08
// Initializes a {dataPtr, count, timerValue} structure with operator ctors.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::InitOperatorArray @ 0x821D6D08 | size: 0x84
 *
 * Initializes a small structure used for tracking network operators:
 *   +0x00: data pointer (set to 0 initially, then filled)
 *   +0x04: element count (uint16, set to 0 initially)
 *   +0x08: timer/timestamp (float, set from global default)
 *
 * Calls cmOperatorCtor_68E0_w with capacity=5 to allocate storage,
 * then zero-fills all elements, and sets the timer field from a global
 * float constant (g_defaultTimerValue @ 0x825C5938).
 */
void pongNetMessageHolder_InitOperatorArray(void* self) {
    uint8_t* obj = (uint8_t*)self;

    // Clear data pointer and count
    *(uint32_t*)(obj + 0) = 0;
    *(uint16_t*)(obj + 4) = 0;

    // Initialize with capacity 5
    extern void cmOperatorCtor_68E0_w(void* self, int capacity);
    cmOperatorCtor_68E0_w(self, 5);

    // Zero-fill all allocated elements
    uint16_t elementCount = *(uint16_t*)(obj + 4);
    if (elementCount > 0) {
        uint32_t* dataPtr = *(uint32_t**)(obj + 0);
        for (uint16_t i = 0; i < elementCount; i++) {
            dataPtr[i] = 0;
        }
    }

    // Set timer field from global default float @ 0x825C5938
    extern float g_defaultTimerValue;  // @ 0x825C5938
    *(float*)(obj + 8) = g_defaultTimerValue;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::AllocateArray4  @ 0x821379C8
// Allocates an array of 4-byte elements with per-element initialization.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::AllocateArray4 @ 0x821379C8 | size: 0x94
 *
 * Allocates an array of `count` 4-byte elements and calls ke_1B00 on each
 * element to initialize it. Stores the array pointer at +0 and the count
 * as a uint16 at +6.
 *
 * If count is 0 or allocation fails, stores nullptr at +0.
 * Overflow guard: if count > 0x3FFFFFFF, requests (uint32_t)-1 bytes.
 */
void pongNetMessageHolder_AllocateArray4(void* self, uint32_t count) {
    uint8_t* obj = (uint8_t*)self;

    if (count == 0) {
        *(uint16_t*)(obj + 6) = 0;
        *(void**)(obj + 0) = nullptr;
        return;
    }

    // Compute allocation size: count * 4
    uint32_t allocSize = count * 4;
    if (count > 0x3FFFFFFF) {
        allocSize = (uint32_t)-1;
    }

    void* buffer = xe_EC88(allocSize);

    if (buffer == nullptr) {
        *(uint16_t*)(obj + 6) = (uint16_t)count;
        *(void**)(obj + 0) = nullptr;
        return;
    }

    // Initialize each element with ke_1B00
    extern void ke_1B00(void* element);
    uint8_t* elementPtr = (uint8_t*)buffer;
    for (uint32_t i = 0; i < count; i++) {
        ke_1B00(elementPtr);
        elementPtr += 4;
    }

    *(uint16_t*)(obj + 6) = (uint16_t)count;
    *(void**)(obj + 0) = buffer;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::InitPlayerNetState  @ 0x821D75B0
// Large initializer for per-player network state (positions, timers, arrays).
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::InitPlayerNetState @ 0x821D75B0 | size: 0xB8
 *
 * Initializes a large per-player network state structure (~10200+ bytes):
 *   +0x00..+0x18: six floats set to 0.0 (position/velocity vectors)
 *   +0x20: sub-structure initialized via pongNetMessageHolder_7668_2hr
 *          (119 x 80-byte player state slots + trailing summary block)
 *   +0x2600 (9728): bool flag = false
 *   +0x2604 (9732): float timestamp = g_defaultTimerValue
 *   +0x2608 (9736): operator array initialized via InitOperatorArray
 *   +0x2620 (9760): game state block initialized via game_5E70
 *   +0x27E0 (10208): uint32 = 0
 *   +0x27E4 (10212): byte = 0
 *   +0x27E8 (10216): 16 bytes copied from template data @ 0x8261A0C0
 */
void pongNetMessageHolder_InitPlayerNetState(void* self) {
    uint8_t* obj = (uint8_t*)self;

    // Load zero float constant from .rdata @ 0x8202D110
    float zeroVal = 0.0f;

    // Zero out six float fields (two 3D vectors: position + velocity)
    *(float*)(obj + 0)  = zeroVal;
    *(float*)(obj + 4)  = zeroVal;
    *(float*)(obj + 8)  = zeroVal;
    *(float*)(obj + 16) = zeroVal;
    *(float*)(obj + 20) = zeroVal;
    *(float*)(obj + 24) = zeroVal;

    // Initialize the 119-slot player state array at +32
    extern void pongNetMessageHolder_7668_2hr(void* stateArray);
    pongNetMessageHolder_7668_2hr(obj + 32);

    // Set timestamp and flag near offset 9728
    extern float g_defaultTimerValue;  // @ 0x825C5938
    *(uint8_t*)(obj + 9728) = 0;                       // m_bNetStateActive
    *(float*)(obj + 9732)   = g_defaultTimerValue;      // m_lastUpdateTime

    // Initialize operator array at +9736
    pongNetMessageHolder_InitOperatorArray(obj + 9736);

    // Initialize game state block at +9760
    extern void game_5E70(void* gameState);
    game_5E70(obj + 9760);

    // Clear trailing state fields
    *(uint32_t*)(obj + 10208) = 0;   // m_pendingEvents
    *(uint8_t*)(obj + 10212) = 0;    // m_bSyncPending

    // Copy 16 bytes of template data from global @ 0x8261A0C0
    uint32_t* templateSrc = (uint32_t*)0x8261A0C0;
    uint32_t* dst = (uint32_t*)(obj + 10216);
    dst[0] = templateSrc[0];
    dst[1] = templateSrc[1];
    dst[2] = templateSrc[2];
    dst[3] = templateSrc[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// SwingStartedMessage::SwingStartedMessage  [constructor @ 0x823B99E8]
// Constructs a SwingStartedMessage with physics sub-object and operator arrays.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * SwingStartedMessage::SwingStartedMessage @ 0x823B99E8 | size: 0xD0
 *
 * Constructor for the SwingStartedMessage network message.
 * Sets vtable to SwingStartedMessage @ 0x8206F4D8, initializes:
 *   +0x04: timing float from global @ 0x82079AD4
 *   +0x10: physics sub-object via ph_ctor_69F8 (336 bytes)
 *   +0x150 (336): three int16 fields set to -1 (invalid indices)
 *   +0x160 (352): float timestamp from zero constant
 *   +0x174 (372): int16 = -1
 *   +0x164 (356): float = 0.0
 *   +0x176 (374): byte = 255 (invalid player index)
 *   +0x168 (360..368): three more floats = 0.0
 *   +0x178 (376): operator array initialized via InitOperatorArray
 *   +0x184 (388..389): two bytes = 255 (invalid indices)
 *   +0x186 (390): byte = 0
 */
void SwingStartedMessage_Ctor(void* self) {
    uint8_t* obj = (uint8_t*)self;

    // Set vtable to SwingStartedMessage @ 0x8206F4D8
    extern float g_swingTimingDefault;  // @ 0x82079AD4 (rdata float)
    *(float*)(obj + 4) = g_swingTimingDefault;
    *(void**)(obj + 0) = (void*)0x8206F4D8;

    // Construct physics sub-object at +16
    extern void ph_ctor_69F8(void* physics);
    ph_ctor_69F8(obj + 16);

    // Initialize invalid-index fields in the data region
    // Three int16 fields at +336, +338, +340 (relative to +16 base: +320, +322, +324)
    *(int16_t*)(obj + 336) = -1;
    *(int16_t*)(obj + 338) = -1;
    *(int16_t*)(obj + 340) = -1;

    // Timing and state floats (0.0f from .rdata)
    float zeroFloat = 0.0f;
    *(float*)(obj + 352) = zeroFloat;
    *(int16_t*)(obj + 372) = -1;
    *(float*)(obj + 356) = zeroFloat;
    *(uint8_t*)(obj + 374) = 255;  // invalid player index sentinel
    *(float*)(obj + 360) = zeroFloat;
    *(float*)(obj + 364) = zeroFloat;
    *(float*)(obj + 368) = zeroFloat;

    // Initialize operator array at +376
    pongNetMessageHolder_InitOperatorArray(obj + 376);

    // Set trailing invalid-index bytes
    *(uint8_t*)(obj + 388) = 255;
    *(uint8_t*)(obj + 389) = 255;
    *(uint8_t*)(obj + 390) = 0;

    // Zero-fill operator array elements (same pattern as InitOperatorArray tail)
    uint16_t opCount = *(uint16_t*)(obj + 376 + 4);  // count at +380
    if (opCount > 0) {
        uint32_t* opData = *(uint32_t**)(obj + 376);
        for (uint16_t i = 0; i < opCount; i++) {
            opData[i] = 0;
        }
    }

    // Set operator timer from global
    extern float g_defaultTimerValue;  // @ 0x825C5938
    *(float*)(obj + 376 + 8) = g_defaultTimerValue;  // +384
}

// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::ResetPlayerSlotState  @ 0x821D7668
// Bulk-zeros 119 player state slots (80 bytes each) plus trailing summary.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * pongNetMessageHolder::ResetPlayerSlotState @ 0x821D7668 | size: 0xE4
 *
 * Clears 119 player-state slots, each 80 bytes, starting at self+16.
 * Each slot contains:
 *   +0..+47:  zeroed via three 16-byte vector stores (vxor)
 *   +48..+59: three floats set from global (likely max/sentinel values)
 *   +60:      float set from a second global constant
 *   -16..-12: two floats set from first global
 *   -8..-6:   three bytes zeroed (flags)
 *
 * After the 119 slots, clears three uint32 counters at +9600..+9608
 * and initializes a trailing 80-byte summary block with the same pattern.
 */
void pongNetMessageHolder_ResetPlayerSlotState(void* self) {
    uint8_t* obj = (uint8_t*)self;
    float sentinelFloat = 0.0f;   // loaded from .rdata @ 0x8202D108 (sentinel/max)
    float defaultFloat  = 0.0f;   // loaded from .rdata @ 0x8202D110 (zero)

    // Note: actual sentinel values would be loaded from binary .rdata globals.
    // Using 0.0f as placeholder since the exact binary values are unresolvable here.

    uint8_t* slotBase = obj + 16;

    // Clear 119 player state slots, 80 bytes each
    for (int i = 0; i < 119; i++) {
        uint8_t* slot = slotBase + (i * 80);

        // Zero three 16-byte blocks (48 bytes)
        memset(slot, 0, 48);

        // Set three sentinel floats at +48, +52, +56
        *(float*)(slot + 48) = sentinelFloat;
        *(float*)(slot + 52) = sentinelFloat;
        *(float*)(slot + 56) = sentinelFloat;

        // Set default float at +60
        *(float*)(slot + 60) = defaultFloat;

        // Set two floats before the slot header (relative -16, -12)
        *(float*)(slot - 16) = sentinelFloat;
        *(float*)(slot - 12) = sentinelFloat;

        // Clear three flag bytes at -8, -7, -6
        *(slot - 8) = 0;
        *(slot - 7) = 0;
        *(slot - 6) = 0;
    }

    // Clear three trailing counters at +9600, +9604, +9608
    *(uint32_t*)(obj + 9600) = 0;
    *(uint32_t*)(obj + 9604) = 0;
    *(uint32_t*)(obj + 9608) = 0;

    // Initialize trailing 80-byte summary block at +9616
    uint8_t* summary = obj + 9616;

    // Zero two 16-byte blocks (at +16, +32 relative to summary)
    memset(summary + 16, 0, 16);
    memset(summary + 32, 0, 16);
    memset(summary + 48, 0, 16);

    // Set four floats at +64, +68, +72
    *(float*)(summary + 64) = sentinelFloat;
    *(float*)(summary + 68) = sentinelFloat;
    *(float*)(summary + 72) = sentinelFloat;
    *(float*)(summary + 76) = defaultFloat;

    // Set floats and flags in summary header
    *(float*)(summary + 0) = sentinelFloat;
    *(uint8_t*)(summary + 8) = 0;
    *(float*)(summary + 4) = sentinelFloat;
    *(uint8_t*)(summary + 9) = 0;
    *(uint8_t*)(summary + 10) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// gdInputData::gdInputData  [constructor @ 0x821E0138]
// Constructs gdInputData with vtable, flag bytes, and a pool of 58 entries.
// ─────────────────────────────────────────────────────────────────────────────
/**
 * gdInputData::gdInputData @ 0x821E0138 | size: 0xBC
 *
 * Constructor for the gdInputData structure, used for gamepad/input data
 * collection in network play. Sets vtable to gdInputData @ 0x82041468.
 *
 * Layout initialized:
 *   +0x000: vtable pointer (0x82041468)
 *   +0x004..+0x007: four flag bytes = 0
 *   +0x008: uint32 = 0 (input state)
 *   +0x00C: byte = 0 (input flags)
 *   +0x010: uint32 = 0 (timestamp)
 *   +0x014: uint32 = 0 (frame counter)
 *   +0x018: uint32 = 0 (sequence number)
 *   +0x31C (796): sub-array {dataPtr, count, capacity} for 58 input entries
 *   +0x324 (804): uint32 = 0 (pending count)
 *
 * Each input entry is 4 bytes (232 total bytes for 58 entries).
 * Entries are zero-filled on allocation.
 */
void gdInputData_Ctor(void* self) {
    uint8_t* obj = (uint8_t*)self;
    const uint16_t INPUT_ENTRY_COUNT = 58;

    // Set vtable to gdInputData @ 0x82041468
    *(void**)(obj + 0) = (void*)0x82041468;

    // Clear flag bytes and state fields
    *(obj + 4) = 0;
    *(obj + 5) = 0;
    *(obj + 6) = 0;
    *(obj + 7) = 0;
    *(uint32_t*)(obj + 8) = 0;
    *(obj + 12) = 0;
    *(uint32_t*)(obj + 16) = 0;
    *(uint32_t*)(obj + 20) = 0;
    *(uint32_t*)(obj + 24) = 0;

    // Initialize the input entry sub-array at +796
    uint8_t* entryArray = obj + 796;
    *(uint32_t*)(entryArray + 0) = 0;      // data pointer (filled below)
    *(uint16_t*)(entryArray + 4) = 0;      // current count
    *(uint16_t*)(entryArray + 6) = 0;      // capacity
    *(uint32_t*)(obj + 804) = 0;           // pending count

    // Check if capacity needs initialization
    uint16_t currentCapacity = *(uint16_t*)(entryArray + 6);
    if (currentCapacity == 0) {
        // Set desired capacity
        *(uint16_t*)(entryArray + 6) = INPUT_ENTRY_COUNT;

        // Allocate 232 bytes (58 entries * 4 bytes each)
        void* inputBuffer = xe_EC88(232);

        if (inputBuffer != nullptr) {
            // Zero-fill all entries (58 * 4 = 232 bytes)
            uint8_t* fillPtr = (uint8_t*)inputBuffer;
            for (int i = 57; i >= 0; i--) {
                fillPtr[0] = 0;
                fillPtr[1] = 0;
                fillPtr[2] = 0;
                fillPtr[3] = 0;
                fillPtr += 4;
            }
        }

        // Store allocated buffer pointer
        *(void**)(entryArray + 0) = inputBuffer;
    }

    // Set the active count to the entry capacity
    *(uint16_t*)(entryArray + 4) = INPUT_ENTRY_COUNT;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder pool Create/Destroy functions
//
// These are vtable slot 1 (Create) and slot 2 (Destroy) implementations for
// the pongNetMessageHolder message pool system. Each pongNetMessageHolder
// vtable variant manages a pool of a specific network message type.
//
// Create (vfn_1): Lazy-initializes the pool buffer at +8. Allocates memory,
//   calls the message constructor, and stores the pointer.
// Destroy (vfn_2): Resets all element vtables to PongNetMessage base
//   (0x8206C304), frees the buffer, and nulls the pointer at +8.
// ─────────────────────────────────────────────────────────────────────────────

// Forward declarations for message constructors
extern void pongNetMessageHolder_70C8_wrh(void* self);  // constructs 176B pool message
extern void pongNetMessageHolder_71C0_wrh(void* self);  // constructs 112B pool message
extern void pongNetMessageHolder_72A8_wrh(void* self);  // constructs 1040B pool message
extern void pongNetMessageHolder_6F30_wrh(void* self);  // constructs 104816B pool message

// PongNetMessage base vtable used as sentinel when releasing pool elements
static void** const g_PongNetMessageBaseVtable = (void**)0x8206C304;


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CreateHitMessagePool @ 0x823C08D0 | size: 0x58
//
// Lazy-initializes a pool of HitMessage objects (104816 bytes total).
// Called via vtable slot 1 to ensure the pool buffer is allocated before use.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_CreateHitMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    // Already initialized?
    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    // Allocate 104816 bytes (0x19970) for the HitMessage pool
    void* memory = xe_EC88(104816);

    if (memory != nullptr) {
        // Construct the pool via pongNetMessageHolder_6F30_wrh
        pongNetMessageHolder_6F30_wrh(memory);
    } else {
        memory = nullptr;
    }

    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DestroyHitMessagePool @ 0x823C0928 | size: 0x68
//
// Releases a pool of 200 HitMessage objects (524-byte stride each).
// Resets each element's vtable to PongNetMessage base before freeing.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyHitMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    // Reset vtables for 200 elements at 524-byte stride
    // Iterates backwards: starts at buffer + 200*524, decrements by 524
    uint8_t* elementPtr = (uint8_t*)buffer + (200 * 524);
    for (int i = 199; i >= 0; i--) {
        elementPtr -= 524;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CreateSwingPool @ 0x823C0BC0 | size: 0x54
//
// Lazy-initializes a pool for swing message objects (176 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_CreateSwingPool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    void* memory = xe_EC88(176);

    if (memory != nullptr) {
        pongNetMessageHolder_70C8_wrh(memory);
    } else {
        memory = nullptr;
    }

    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CreateFocusPool @ 0x823C0C18 | size: 0x54
//
// Lazy-initializes a pool for focus message objects (112 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_CreateFocusPool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    void* memory = xe_EC88(112);

    if (memory != nullptr) {
        pongNetMessageHolder_71C0_wrh(memory);
    } else {
        memory = nullptr;
    }

    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DestroyFocusPool @ 0x823C0C70 | size: 0x64
//
// Releases a pool of 4 focus message objects (24-byte stride each).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyFocusPool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    // Reset vtables for 4 elements at 24-byte stride
    uint8_t* elementPtr = (uint8_t*)buffer + (4 * 24);
    for (int i = 3; i >= 0; i--) {
        elementPtr -= 24;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CreatePlayerStatePool @ 0x823C0DE0 | size: 0x54
//
// Lazy-initializes a pool for player state message objects (1040 bytes).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_CreatePlayerStatePool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    void* memory = xe_EC88(1040);

    if (memory != nullptr) {
        pongNetMessageHolder_72A8_wrh(memory);
    } else {
        memory = nullptr;
    }

    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DestroyPlayerStatePool @ 0x823C0E38 | size: 0x64
//
// Releases a pool of 4 player state message objects (256-byte stride each).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyPlayerStatePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    // Reset vtables for 4 elements at 256-byte stride
    uint8_t* elementPtr = (uint8_t*)buffer + (4 * 256);
    for (int i = 3; i >= 0; i--) {
        elementPtr -= 256;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DestroyMatchStatePool @ 0x823BFF38 | size: 0x64
//
// Releases a pool of 4 match state message objects (20-byte stride each).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyMatchStatePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    // Reset vtables for 4 elements at 20-byte stride
    uint8_t* elementPtr = (uint8_t*)buffer + (4 * 20);
    for (int i = 3; i >= 0; i--) {
        elementPtr -= 20;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DestroyTimeSyncPool @ 0x823BFFF8 | size: 0x64
//
// Releases a pool of 10 time sync message objects (36-byte stride each).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyTimeSyncPool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    // Reset vtables for 10 elements at 36-byte stride
    uint8_t* elementPtr = (uint8_t*)buffer + (10 * 36);
    for (int i = 9; i >= 0; i--) {
        elementPtr -= 36;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DestroyMovementPool @ 0x823C09E8 | size: 0x64
//
// Releases a pool of 200 movement message objects (80-byte stride each).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyMovementPool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    // Reset vtables for 200 elements at 80-byte stride
    uint8_t* elementPtr = (uint8_t*)buffer + (200 * 80);
    for (int i = 199; i >= 0; i--) {
        elementPtr -= 80;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}
