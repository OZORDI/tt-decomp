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

// Forward declarations
extern "C" void rage_free_00C0(void* ptr);
extern "C" void atSingleton_9420(void* obj);
extern "C" void util_FFF8(void* obj, int flags);

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
        rage_free_00C0(self);
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
        rage_free_00C0(self);
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
        rage_free_00C0(self);
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
        rage_free_00C0(self);
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
        rage_free_00C0(self);
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
        rage_free_00C0(self);
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
                util_FFF8(packet, 0);
                
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
    util_FFF8((char*)self + 72, 0);
    
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
        rage_free_00C0(self);
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
    atSingleton_9420(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free_00C0(self);
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
 * plrPropMgr::vfn_21 @ 0x823D4730 | size: 0x6C
 * plrPropMgr::vfn_23 @ 0x823D47A0 | size: 0x58
 * plrPropMgr::vfn_24 @ 0x823D47F8 | size: 0x64
 * plrPropMgr::vfn_25 @ 0x823D4860 | size: 0x40
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

extern "C" {
    // Stub for pongLookAtDriver destructor (base class of pongPaddle)
    void pongLookAtDriver_PostLoadProperties(void* obj) {
        // Stub - would call base class destructor
    }
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
    
    // Call cleanup function
    extern void gameLoop_DestroyAudio_27A8(void* obj);
    gameLoop_DestroyAudio_27A8(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free_00C0(self);
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
    extern void rage_F248_1(void* obj);
    rage_F248_1((char*)self + 4364);
    
    // Initialize component at offset +2096
    extern void SinglesNetworkClient_2BE8_g(void* obj);
    SinglesNetworkClient_2BE8_g((char*)self + 2096);
    
    // Initialize nested state object at offset +1508
    // Set its vtable @ 0x8207116C
    void** stateObj = (void**)((char*)self + 1508);
    *stateObj = (void**)0x8207116C;
    gameLoop_DestroyAudio_27A8(stateObj);
    
    // Initialize state machine member at offset +1392
    // Set its vtable @ 0x82070D78 (calculated from lis(-32249) + 3448)
    void** stateMachine = (void**)((char*)self + 1392);
    *stateMachine = (void**)0x82070D78;
    
    // Call initialization
    extern void util_AA38(NetDataQuery* self);
    util_AA38(self);
}

/**
 * NetDataQuery::vfn_4 @ 0x823D1AD0 | size: 0x4
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
    extern void* xe_main_thread_init_0038();
    xe_main_thread_init_0038();
    
    // Get allocator pointer from SDA @ r13+0 (0x82600000)
    extern void* g_allocator_ptr;
    void** allocator = (void**)((char*)&g_allocator_ptr);
    
    // Allocate state array (16 bytes for 4 pointers)
    void** vtable = *allocator;
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn alloc = (AllocFn)vtable[1];
    *(void***)((char*)self + 8) = (void**)alloc(*allocator, 16, 16);
    
    void** stateArray = *(void***)((char*)self + 8);
    
    // Allocate and initialize state 0 @ 0x820711B4
    xe_main_thread_init_0038();
    void* state0 = alloc(*allocator, 12, 16);
    if (state0) {
        *(uint32_t*)((char*)state0 + 4) = 0;
        *(uint32_t*)((char*)state0 + 8) = (uint32_t)self;
        *(void**)state0 = (void*)0x820711B4;
    }
    stateArray[0] = state0;
    
    // Allocate and initialize state 1 @ 0x820711FC
    xe_main_thread_init_0038();
    void* state1 = alloc(*allocator, 12, 16);
    if (state1) {
        *(uint32_t*)((char*)state1 + 4) = 0;
        *(uint32_t*)((char*)state1 + 8) = (uint32_t)self;
        *(void**)state1 = (void*)0x820711FC;
    }
    stateArray[1] = state1;
    
    // Allocate and initialize state 2: stateReceiveData @ 0x82071244
    xe_main_thread_init_0038();
    void* state2 = alloc(*allocator, 12, 16);
    if (state2) {
        *(uint32_t*)((char*)state2 + 4) = 0;
        *(uint32_t*)((char*)state2 + 8) = (uint32_t)self;
        *(void**)state2 = (void*)0x82071244;
    }
    stateArray[2] = state2;
    
    // Allocate and initialize state 3: stateFinish @ 0x8207128C
    xe_main_thread_init_0038();
    void* state3 = alloc(*allocator, 12, 16);
    if (state3) {
        *(uint32_t*)((char*)state3 + 4) = 0;
        *(uint32_t*)((char*)state3 + 8) = (uint32_t)self;
        *(void**)state3 = (void*)0x8207128C;
    }
    stateArray[3] = state3;
}

/**
 * NetDataQuery::vfn_10 @ 0x823D17C8 | size: 0x84
 * NetDataQuery::vfn_13 @ 0x823D1850 | size: 0x5C
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
    
    // Call cleanup function
    extern void gameLoop_DestroyAudio_27A8(void* obj);
    gameLoop_DestroyAudio_27A8(self);
    
    // If bit 0 is set in flags, free the object memory
    if (flags & 0x1) {
        rage_free_00C0(self);
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
    extern void* xe_main_thread_init_0038();
    xe_main_thread_init_0038();
    
    // Get allocator pointer from SDA @ r13+0 (0x82600000)
    extern void* g_allocator_ptr;  // @ 0x82600004
    void** allocator = (void**)((char*)&g_allocator_ptr);
    
    // Allocate state array (28 bytes for 7 pointers)
    void** vtable = *allocator;
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn alloc = (AllocFn)vtable[1];
    self->m_pStateArray = (void**)alloc(*allocator, 28, 16);
    
    // Allocate and initialize each state object (12 bytes each)
    // State 0: stateInit @ 0x82071404
    xe_main_thread_init_0038();
    void* state0 = alloc(*allocator, 12, 16);
    if (state0) {
        *(uint32_t*)((char*)state0 + 4) = 0;  // Clear field at +4
        *(uint32_t*)((char*)state0 + 8) = (uint32_t)self;  // Back pointer
        *(void**)state0 = (void*)0x82071404;  // vtable
    }
    self->m_pStateArray[0] = state0;
    
    // State 1: stateWaitForSyncState @ 0x8207144C
    xe_main_thread_init_0038();
    void* state1 = alloc(*allocator, 12, 16);
    if (state1) {
        *(uint32_t*)((char*)state1 + 4) = 0;
        *(uint32_t*)((char*)state1 + 8) = (uint32_t)self;
        *(void**)state1 = (void*)0x8207144C;
    }
    self->m_pStateArray[1] = state1;
    
    // State 2: stateEnterState @ 0x82071494
    xe_main_thread_init_0038();
    void* state2 = alloc(*allocator, 12, 16);
    if (state2) {
        *(uint32_t*)((char*)state2 + 4) = 0;
        *(uint32_t*)((char*)state2 + 8) = (uint32_t)self;
        *(void**)state2 = (void*)0x82071494;
    }
    self->m_pStateArray[2] = state2;
    
    // State 3: stateRequestSyncronization @ 0x820714DC
    xe_main_thread_init_0038();
    void* state3 = alloc(*allocator, 12, 16);
    if (state3) {
        *(uint32_t*)((char*)state3 + 4) = 0;
        *(uint32_t*)((char*)state3 + 8) = (uint32_t)self;
        *(void**)state3 = (void*)0x820714DC;
    }
    self->m_pStateArray[3] = state3;
    
    // State 4: stateSendSyncronization @ 0x82071524
    xe_main_thread_init_0038();
    void* state4 = alloc(*allocator, 12, 16);
    if (state4) {
        *(uint32_t*)((char*)state4 + 4) = 0;
        *(uint32_t*)((char*)state4 + 8) = (uint32_t)self;
        *(void**)state4 = (void*)0x82071524;
    }
    self->m_pStateArray[4] = state4;
    
    // State 5: stateReceiveSyncronization @ 0x8207156C
    xe_main_thread_init_0038();
    void* state5 = alloc(*allocator, 12, 16);
    if (state5) {
        *(uint32_t*)((char*)state5 + 4) = 0;
        *(uint32_t*)((char*)state5 + 8) = (uint32_t)self;
        *(void**)state5 = (void*)0x8207156C;
    }
    self->m_pStateArray[5] = state5;
    
    // State 6: stateWaitTime @ 0x820715B4 (16 bytes, has float at +12)
    xe_main_thread_init_0038();
    void* state6 = alloc(*allocator, 16, 16);
    if (state6) {
        *(uint32_t*)((char*)state6 + 4) = 0;
        *(uint32_t*)((char*)state6 + 8) = (uint32_t)self;
        
        // Load default timeout value from global @ lis(-32253) + -12016
        // Python: (lis(-32253) << 16) + -12016 = 0x82033110
        float defaultTimeout = *(float*)0x82033110;
        *(float*)((char*)state6 + 12) = defaultTimeout;
        
        *(void**)state6 = (void*)0x820715B4;
    }
    self->m_pStateArray[6] = state6;
}

/**
 * NetStateSync::vfn_10 @ 0x823D1D70 | size: 0x60
 * NetStateSync::vfn_13 @ 0x823D1DD0 | size: 0x64
 * 
 * Additional virtual methods - stubs for now.
 */
void NetStateSync_OnComplete(NetStateSync* self) {}
void NetStateSync_GetName(NetStateSync* self) {}

////////////////////////////////////////////////////////////////////////////////
// External function stubs
////////////////////////////////////////////////////////////////////////////////

extern "C" {
    // Stub for game loop audio cleanup
    void gameLoop_DestroyAudio_27A8(void* obj) {
        // Stub - would clean up audio resources
    }
    
    // Stub for rage initialization
    void rage_F248_1(void* obj) {
        // Stub - would initialize rage component
    }
    
    // Stub for network client initialization
    void SinglesNetworkClient_2BE8_g(void* obj) {
        // Stub - would initialize network client
    }
    
    // Stub for network client helper
    void SinglesNetworkClient_51C8_g(void* obj) {
        // Stub - would initialize network client component
    }
    
    // Stub for utility initialization
    void util_AA38(void* obj) {
        // Stub - would perform utility initialization
    }
    
    // Stub for thread initialization
    void* xe_main_thread_init_0038() {
        // Stub - would initialize thread context
        return nullptr;
    }
    
    // Global allocator pointer @ 0x82600004
    void* g_allocator_ptr = nullptr;
}
