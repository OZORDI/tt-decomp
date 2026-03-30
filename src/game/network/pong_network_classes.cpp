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
extern "C" void rage_free(void* ptr);
extern "C" void rage::ReleaseSingleton(void* obj);
extern "C" void sysCallback::Invoke(void* obj, int flags);

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
 * Internal cleanup — iterates through all tracked packets,
 * calls AckHandling_3828 to release each one, and decrements the count.
 * Called by the destructor before freeing the object.
 */
void AckHandling_34D0_fw(AckHandling* self) {
    // Set vtable to AckHandling @ 0x82071654
    self->vtable = (void**)0x82071654;

    uint32_t count = *(uint32_t*)((char*)self + 96);
    if ((int32_t)count <= 0) {
        return;
    }

    uint8_t* base = (uint8_t*)self + 16;
    while (count > 0) {
        // Read current count, decrement, then load the last packet pointer
        uint32_t curCount = *(uint32_t*)(base + 80);
        curCount--;
        *(uint32_t*)(base + 80) = curCount;
        void* packet = *(void**)(base + curCount * 4);

        // Release the packet
        extern void AckHandling_3828(AckHandling* self, void* packet);
        AckHandling_3828(self, packet);

        count--;
    }
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
                sysCallback::Invoke(packet, 0);
                
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
 * Removes a packet from the pool and returns it to the free list.
 * Resets packet fields (copies 16 bytes from global default data,
 * clears sequence number/ack flag, stores default timestamp),
 * then links it into the free list at base+112.
 */
void AckHandling_3828(AckHandling* self, void* packet) {
    uint8_t* pkt = (uint8_t*)packet;
    uint8_t* poolBase = (uint8_t*)self + 112;

    // Copy 16 bytes of default data from global @ 0x8261A0C0
    extern uint32_t g_defaultPacketData[4];  // @ 0x8261A0C0
    uint32_t* dst = (uint32_t*)pkt;
    dst[0] = g_defaultPacketData[0];
    dst[1] = g_defaultPacketData[1];
    dst[2] = g_defaultPacketData[2];
    dst[3] = g_defaultPacketData[3];

    // Store default timestamp at +20, clear sequence at +16, clear ack at +18
    extern float g_defaultTimestamp;  // @ 0x82089A54
    *(float*)(pkt + 20) = g_defaultTimestamp;
    *(uint16_t*)(pkt + 16) = 0;
    *(uint8_t*)(pkt + 18) = 0;

    // Calculate slot index: (pkt - poolBase) / 28
    // Uses mulhwu trick: (diff * 0x249249B5) >> 32, then fixup
    uint32_t diff = (uint32_t)(pkt - poolBase);
    uint64_t prod = (uint64_t)diff * 0x249249B5ULL;
    uint32_t hi = (uint32_t)(prod >> 32);
    uint32_t rem = diff - hi;
    uint32_t slot = ((rem >> 1) + hi) >> 4;  // divide by 28
    slot &= 0xFFFF;

    // Link into free list: set next pointer on this slot
    uint16_t headSlot = *(uint16_t*)(poolBase + 564);
    *(uint16_t*)(poolBase + slot * 28 + 26) = (uint16_t)(slot + 1) & 0xFFFF;

    // If head isn't 0xFFFF, update head's prev to point to this slot
    if (headSlot != 0xFFFF) {
        *(uint16_t*)(poolBase + headSlot * 28 + 24) = slot;
    }

    // Set new head
    *(uint16_t*)(poolBase + 564) = slot;

    // Increment free count
    uint16_t freeCount = *(uint16_t*)(poolBase + 560);
    *(uint16_t*)(poolBase + 560) = freeCount + 1;
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
    sysCallback::Invoke((char*)self + 72, 0);
    
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
 * pongPaddle::ScalarDestructor (vfn_2) @ 0x823D4140 | size: 0x50
 *
 * Scalar destructor — calls cleanup then conditionally frees.
 */
void pongPaddle_vfn_2(pongPaddle* self, int flags) {
    pongPaddle_4190_h(self);

    if (flags & 0x1) {
        extern void sysMemAllocator_Free(void* ptr);
        sysMemAllocator_Free(self);
    }
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
    rage::ReleaseSingleton(self);
    
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
 * plrPropMgr::vfn_21 @ 0x823D4730 | size: 0x6C
 * plrPropMgr::vfn_23 @ 0x823D47A0 | size: 0x58
 * plrPropMgr::vfn_24 @ 0x823D47F8 | size: 0x64
 * plrPropMgr::vfn_25 @ 0x823D4860 | size: 0x40
 * 
 * Additional virtual methods - stubs for now.
 */
/**
 * plrPropMgr::PostLoadChildren (vfn_21) @ 0x823D4730 | size: 0x6C
 *
 * Registers two property types with the property manager.
 * Calls RegisterSerializationField twice with property vtables at offsets +16 and +20.
 */
void plrPropMgr_Validate(plrPropMgr* self) {
    extern void RegisterSerializationField(void* obj, void* propVtable, void* propData, void* storage, int flags);
    extern uint32_t g_plrPropStorage;  // @ 0x825CAF90

    // Register property type 1 (vtable @ 0x820716A8 = lis(-32249)+5800)
    RegisterSerializationField(self, (void*)0x820716A8, (char*)self + 16, &g_plrPropStorage, 0);

    // Register property type 2 (vtable @ 0x820716B8 = lis(-32249)+5816)
    RegisterSerializationField(self, (void*)0x820716B8, (char*)self + 20, &g_plrPropStorage, 0);
}

/**
 * plrPropMgr::PostLoadSetup (vfn_23) @ 0x823D47A0 | size: 0x58
 *
 * Loads default property instances. Loads one from +16, stores at +24,
 * then loads two from +20 and stores at +28 and +32.
 * Finally stores singleton pointer to global @ 0x826066C0.
 */
void plrPropMgr_PostLoadSetup(plrPropMgr* self) {
    extern void* game_8FB0(void* propTypeId, int defaultInstance);
    extern void* g_plrPropMgrSingleton;  // @ 0x826066C0

    // Load default for property type 1
    void* prop1 = game_8FB0(*(void**)((char*)self + 16), 0);
    *(void**)((char*)self + 24) = prop1;

    // Load 2 defaults for property type 2
    for (int i = 0; i < 2; i++) {
        void* prop2 = game_8FB0(*(void**)((char*)self + 20), 0);
        *(void**)((char*)self + 28 + i * 4) = prop2;
    }

    // Store singleton pointer
    g_plrPropMgrSingleton = self;
}

/**
 * plrPropMgr::OnDeactivate (vfn_24) @ 0x823D47F8 | size: 0x64
 *
 * Releases all 3 loaded property instances at +24, +28, and +32.
 * Calls game_8EE8 then sysMemAllocator_PlatformFree for each, then nulls the pointers.
 */
void plrPropMgr_OnActivate(plrPropMgr* self) {
    extern void game_8EE8(void* prop);
    extern void sysMemAllocator_PlatformFree(void* prop, int flags);

    uint8_t* base = (uint8_t*)self + 24;
    for (int i = 0; i < 3; i++) {
        void* prop = *(void**)(base + i * 4);
        if (prop) {
            game_8EE8(prop);
            sysMemAllocator_PlatformFree(prop, (int)0xE0010000);  // lis(-8191) = 0xE0010000
        }
    }

    // Null all 3 property pointers
    *(void**)((char*)self + 24) = nullptr;
    *(void**)((char*)self + 28) = nullptr;
    *(void**)((char*)self + 32) = nullptr;
}

/**
 * plrPropMgr::OnActivate (vfn_25) @ 0x823D4860 | size: 0x40
 *
 * Marks two sub-objects at +36 and +40 as needing refresh.
 * Sets bytes at +288, +290, +291 on each sub-object to 1.
 */
void plrPropMgr_OnDeactivate(plrPropMgr* self) {
    void* obj1 = *(void**)((char*)self + 36);
    if (obj1) {
        *((uint8_t*)obj1 + 288) = 1;
        *((uint8_t*)obj1 + 290) = 1;
        *((uint8_t*)obj1 + 291) = 1;
    }

    void* obj2 = *(void**)((char*)self + 40);
    if (obj2) {
        *((uint8_t*)obj2 + 288) = 1;
        *((uint8_t*)obj2 + 290) = 1;
        *((uint8_t*)obj2 + 291) = 1;
    }
}

// pongLookAtDriver base class method — implemented elsewhere
extern "C" void pongLookAtDriver_PostLoadProperties(void* obj);


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
    extern void fsmMachine_Destructor_27A8(void* obj);
    fsmMachine_Destructor_27A8(self);
    
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
    fsmMachine_Destructor_27A8(stateObj);
    
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
    extern void* rage_AssertMainThread();
    rage_AssertMainThread();
    
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
    rage_AssertMainThread();
    void* state0 = alloc(*allocator, 12, 16);
    if (state0) {
        *(uint32_t*)((char*)state0 + 4) = 0;
        *(uint32_t*)((char*)state0 + 8) = (uint32_t)self;
        *(void**)state0 = (void*)0x820711B4;
    }
    stateArray[0] = state0;
    
    // Allocate and initialize state 1 @ 0x820711FC
    rage_AssertMainThread();
    void* state1 = alloc(*allocator, 12, 16);
    if (state1) {
        *(uint32_t*)((char*)state1 + 4) = 0;
        *(uint32_t*)((char*)state1 + 8) = (uint32_t)self;
        *(void**)state1 = (void*)0x820711FC;
    }
    stateArray[1] = state1;
    
    // Allocate and initialize state 2: stateReceiveData @ 0x82071244
    rage_AssertMainThread();
    void* state2 = alloc(*allocator, 12, 16);
    if (state2) {
        *(uint32_t*)((char*)state2 + 4) = 0;
        *(uint32_t*)((char*)state2 + 8) = (uint32_t)self;
        *(void**)state2 = (void*)0x82071244;
    }
    stateArray[2] = state2;
    
    // Allocate and initialize state 3: stateFinish @ 0x8207128C
    rage_AssertMainThread();
    void* state3 = alloc(*allocator, 12, 16);
    if (state3) {
        *(uint32_t*)((char*)state3 + 4) = 0;
        *(uint32_t*)((char*)state3 + 8) = (uint32_t)self;
        *(void**)state3 = (void*)0x8207128C;
    }
    stateArray[3] = state3;
}

/**
 * NetDataQuery::Reset (vfn_10) @ 0x823D17C8 | size: 0x84
 *
 * Resets the query state to initial configuration.
 * Copies 16-byte default data to +540 and +556, clears counters and flags.
 */
void NetDataQuery_OnComplete(NetDataQuery* self) {
    extern uint32_t g_defaultData16[4];  // @ 0x8261A0C0

    uint8_t* p = (uint8_t*)self;

    // Set status word to "uninitialized"
    *(uint16_t*)(p + 528) = 0xFFFF;

    // Clear query counters
    *(uint16_t*)(p + 20) = 0;
    *(uint16_t*)(p + 22) = 0;

    // Copy 16-byte default data to +540 and +556
    for (int i = 0; i < 4; i++) {
        ((uint32_t*)(p + 540))[i] = g_defaultData16[i];
        ((uint32_t*)(p + 556))[i] = g_defaultData16[i];
    }

    // Clear remaining state
    *(uint8_t*)(p + 572) = 0;
    *(uint32_t*)(p + 532) = 0;
    *(uint32_t*)(p + 536) = 0;
}

/**
 * NetDataQuery::IsStateReady (vfn_13) @ 0x823D1850 | size: 0x5C
 *
 * Checks if the given state index is valid and the machine is ready.
 * If stateIndex >= stateCount, returns 0.
 * If a current state is set, dispatches to state vtable[14] (offset 56).
 * Otherwise returns 1 if stateIndex == 1, else 0.
 */
uint8_t NetDataQuery_GetName(NetDataQuery* self, int stateIndex) {
    uint8_t* p = (uint8_t*)self;
    int32_t stateCount = *(int32_t*)(p + 4);

    if (stateIndex >= stateCount) {
        return 0;
    }

    int32_t currentState = *(int32_t*)(p + 12);
    if (currentState != -1) {
        void** stateArray = *(void***)(p + 8);
        void* stateObj = stateArray[currentState];
        if (stateObj) {
            void** vtbl = *(void***)stateObj;
            typedef uint8_t (*IsReadyFn)(void*);
            IsReadyFn isReady = (IsReadyFn)vtbl[14];
            return isReady(stateObj);
        }
    }

    // Default: state 1 returns 1, anything else returns 0
    return (stateIndex == 1) ? 1 : 0;
}

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
    extern void SinglesNetworkClient_DA08(void* obj);
    extern float g_defaultTimeout;  // @ 0x82035928 (lis(-32164)+22840)
    extern uint32_t g_defaultData16[4];  // @ 0x8261A0C0

    uint8_t* p = (uint8_t*)self;

    // Set vtable @ 0x82070C84 (lis(-32249)+-15372)
    *(void**)p = (void*)0x82070C84;

    // Initialize sub-object at +4
    SinglesNetworkClient_DA08(p + 4);

    // Set vtable to 0x82070C84 -> 0x82070C30 (lis(-32249)+-15468)
    *(void**)p = (void*)0x82070C30;

    // Clear state fields
    *(uint32_t*)(p + 28) = 0;
    *(uint8_t*)(p + 32) = 0;
    *(uint8_t*)(p + 33) = 0;

    // Set timeout defaults
    float timeout = g_defaultTimeout;
    *(float*)(p + 40) = timeout;
    *(float*)(p + 44) = timeout;
    *(uint32_t*)(p + 36) = 0xFFFFFFFF;  // -1
    *(uint8_t*)(p + 48) = 0;
    *(uint8_t*)(p + 49) = 0;
    *(uint32_t*)(p + 52) = 0;

    // Copy 16 bytes of default data to +56 and +72
    for (int i = 0; i < 4; i++) {
        ((uint32_t*)(p + 56))[i] = g_defaultData16[i];
        ((uint32_t*)(p + 72))[i] = g_defaultData16[i];
    }

    // Set vtable at +96 to 0x82071654 (lis(-32249)+5716)
    *(void**)(p + 96) = (void*)0x82071654;
    *(uint32_t*)(p + 96 + 96) = 0;  // count = 0 at +192

    // Initialize pool at +96+16 via NetDataQuery_38D0_2h
    extern void NetDataQuery_38D0_2h(void* obj);
    NetDataQuery_38D0_2h(p + 96 + 16);

    // Initialize component at +784
    extern void SinglesNetworkClient_2E88_isl(void* obj);
    SinglesNetworkClient_2E88_isl(p + 784);

    *(uint8_t*)(p + 980) = 0;
}

void NetDataQuery_59F8_wrh(void* obj) {
    extern float g_defaultTimestamp;  // @ 0x82089A54 (lis(-32248)+-25900)
    extern float g_defaultTimeout;   // @ 0x82035928 (lis(-32164)+22840)
    extern void game_5128(void* obj);

    uint8_t* p = (uint8_t*)obj;

    // Set vtable @ 0x82070500 (lis(-32249)+-2816)
    *(void**)p = (void*)0x82070500;

    // Store default timestamp at +4
    *(float*)(p + 4) = g_defaultTimestamp;

    // Initialize sub-object at +16
    game_5128(p + 16);

    // Store default timeout at +208
    *(float*)(p + 208) = g_defaultTimeout;

    // Set byte at +212 to 0xFF (255)
    *(uint8_t*)(p + 212) = 0xFF;
}

void NetDataQuery_2A30_2h(void* obj) {
    extern float g_defaultTimestampAlt;  // @ 0x82089A58 (lis(-32248)+-25896)

    uint8_t* p = (uint8_t*)obj;
    float defVal = g_defaultTimestampAlt;

    // Initialize 4 blocks of 36 bytes each starting at offset 0
    // Each block: { -1, defVal, defVal, -1, 0, 0, 0, 0, defVal, defVal }
    for (int blk = 0; blk < 4; blk++) {
        int base = blk * 36;
        *(uint32_t*)(p + base + 0) = 0xFFFFFFFF;   // -1
        *(float*)(p + base + 4) = defVal;
        *(float*)(p + base + 8) = defVal;
        *(uint32_t*)(p + base + 16) = 0;
        *(uint32_t*)(p + base + 20) = 0;
        *(uint32_t*)(p + base + 24) = 0;
        *(uint32_t*)(p + base + 28) = 0;
        // Note: +12 = -1 (from next block's -4 store), handled by stride
        *(uint32_t*)(p + base + 36 - 36 + 12) = 0xFFFFFFFF; // overlap with -1 at next -4
    }

    // Set pool header fields
    *(uint16_t*)(p + 146) = 4;   // capacity per side
    *(uint16_t*)(p + 144) = 4;
    *(uint16_t*)(p + 148) = 0;   // used count
    *(uint16_t*)(p + 32) = 0xFFFF;   // head pointer
    *(uint16_t*)(p + 142) = 0xFFFF;  // tail pointer

    // Link free list: slot[i].next = i+1 for i in 0..capacity-2
    uint16_t capacity = *(uint16_t*)(p + 146);
    if ((int16_t)(capacity - 1) > 0) {
        uint16_t idx = 0;
        uint16_t prev = 0;
        for (uint16_t s = 0; s < capacity - 1; s++) {
            uint16_t nextIdx = idx + 1;
            // Each slot is at: idx * 3 * 4 = idx*12 bytes stride
            // slot offset = (idx + idx*2) * 4 = idx*3*4 → slot[idx] at p + idx*12
            // PPC: rlwinm r9,r11,3,0,28 => idx*8; add r7,r11,r9 => idx+idx*8=idx*9? No.
            // Actually: r9 = r11<<3; r7 = r11+r9 = r11*9; r11 = r7<<2 = r11*36
            // So stride is 36 bytes per slot (same as the blocks above)
            uint32_t slotOff = (uint32_t)idx * 36;
            *(uint16_t*)(p + slotOff + 34) = nextIdx;   // next link
            *(uint16_t*)(p + slotOff + 68) = prev;       // prev link
            prev = idx;
            idx = nextIdx;
        }
    }
}

void NetDataQuery_2B28_2h(void* obj) {
    uint8_t* p = (uint8_t*)obj;

    // Initialize 50 entries of 24 bytes each: { -1, -1, 0, 0, 0, <pad> }
    uint8_t* cursor = p + 4;
    for (int i = 49; i >= 0; i--) {
        *(uint32_t*)(cursor - 4) = 0xFFFFFFFF;  // -1
        *(uint32_t*)(cursor + 0) = 0xFFFFFFFF;  // -1
        *(uint32_t*)(cursor + 4) = 0;
        *(uint32_t*)(cursor + 8) = 0;
        *(uint32_t*)(cursor + 12) = 0;
        cursor += 24;
    }

    // Set pool metadata
    *(uint16_t*)(p + 1204) = 0;       // used count
    *(uint16_t*)(p + 1200) = 50;      // capacity
    *(uint16_t*)(p + 1202) = 50;      // total slots
    *(uint16_t*)(p + 20) = 0xFFFF;    // head = -1
    *(uint16_t*)(p + 1198) = 0xFFFF;  // tail = -1

    // Link free list for 50 slots, stride = 3 entries * 8 bytes = 24 bytes
    uint16_t capacity = *(uint16_t*)(p + 1202);
    if ((int16_t)(capacity - 1) > 0) {
        uint16_t idx = 0;
        uint16_t prev = 0;
        for (uint16_t s = 0; s < capacity - 1; s++) {
            uint16_t nextIdx = idx + 1;
            // stride = (idx + idx*2) * 8 = idx * 24
            // PPC: r9=r11<<1; r11=r11+r9=(idx*3); r11=r11<<3=(idx*24)
            uint32_t slotOff = (uint32_t)idx * 24;
            *(uint16_t*)(p + slotOff + 22) = nextIdx;  // next
            *(uint16_t*)(p + slotOff + 44) = prev;     // prev
            prev = idx;
            idx = nextIdx;
        }
    }

    // Clear trailing metadata
    *(uint32_t*)(p + 1216) = 0;
    *(uint32_t*)(p + 1220) = 0;
    *(uint32_t*)(p + 1224) = 0;
    *(uint16_t*)(p + 1228) = 0;
    *(uint16_t*)(p + 1230) = 0;
    *(uint16_t*)(p + 1736) = 0xFFFF;  // -1
}

/**
 * NetDataQuery::InitPool_38D0 @ 0x823D38D0 | size: 0xC8
 *
 * Initializes a pool of 20 entries with 28-byte stride.
 * Each entry gets 16 bytes copied from global default data,
 * a default float timestamp, and cleared link fields.
 *
 * Pool metadata layout:
 * +560: capacity (uint16_t) = 20
 * +562: total slots (uint16_t) = 20
 * +564: used count (uint16_t) = 0
 * +24:  head pointer (uint16_t) = 0xFFFF
 * +558: tail pointer (uint16_t) = 0xFFFF
 *
 * Entry layout (28 bytes):
 * +0-15:  default data (copied from global)
 * +16-17: link field (uint16_t) = 0
 * +18:    flag (uint8_t) = 0
 * +20-23: default float timestamp
 * +26:    next link (uint16_t)
 */
void NetDataQuery_38D0_2h(void* obj) {
    extern uint32_t g_defaultData16[4];  // @ 0x8261A0C0
    extern float g_defaultTimestamp;     // @ 0x82089A54

    uint8_t* p = (uint8_t*)obj;
    float defTimestamp = g_defaultTimestamp;

    // Initialize 20 entries of 28 bytes each
    for (int i = 0; i < 20; i++) {
        int base = i * 28;
        // Copy 16 bytes of default data
        for (int j = 0; j < 4; j++) {
            ((uint32_t*)(p + base))[j] = g_defaultData16[j];
        }
        // Set link field to 0
        *(uint16_t*)(p + base + 16) = 0;
        // Set flag to 0
        *(uint8_t*)(p + base + 18) = 0;
        // Set default timestamp
        *(float*)(p + base + 20) = defTimestamp;
    }

    // Set pool metadata
    *(uint16_t*)(p + 564) = 0;       // used count
    *(uint16_t*)(p + 560) = 20;      // capacity
    *(uint16_t*)(p + 562) = 20;      // total slots
    *(uint16_t*)(p + 24) = 0xFFFF;   // head = -1
    *(uint16_t*)(p + 558) = 0xFFFF;  // tail = -1

    // Link free list: slot[i].next = i+1, slot[i+1].prev = i
    uint16_t capacity = *(uint16_t*)(p + 562);
    if ((int16_t)(capacity - 1) > 0) {
        uint16_t idx = 0;
        uint16_t prev = 0;
        for (uint16_t s = 0; s < capacity - 1; s++) {
            uint16_t nextIdx = idx + 1;
            uint32_t slotOff = (uint32_t)idx * 28;
            *(uint16_t*)(p + slotOff + 26) = nextIdx;  // next link
            *(uint16_t*)(p + slotOff + 52) = prev;     // prev link (in next slot)
            prev = idx;
            idx = nextIdx;
        }
    }
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
    extern void fsmMachine_Destructor_27A8(void* obj);
    fsmMachine_Destructor_27A8(self);
    
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
    extern void* rage_AssertMainThread();
    rage_AssertMainThread();
    
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
    rage_AssertMainThread();
    void* state0 = alloc(*allocator, 12, 16);
    if (state0) {
        *(uint32_t*)((char*)state0 + 4) = 0;  // Clear field at +4
        *(uint32_t*)((char*)state0 + 8) = (uint32_t)self;  // Back pointer
        *(void**)state0 = (void*)0x82071404;  // vtable
    }
    self->m_pStateArray[0] = state0;
    
    // State 1: stateWaitForSyncState @ 0x8207144C
    rage_AssertMainThread();
    void* state1 = alloc(*allocator, 12, 16);
    if (state1) {
        *(uint32_t*)((char*)state1 + 4) = 0;
        *(uint32_t*)((char*)state1 + 8) = (uint32_t)self;
        *(void**)state1 = (void*)0x8207144C;
    }
    self->m_pStateArray[1] = state1;
    
    // State 2: stateEnterState @ 0x82071494
    rage_AssertMainThread();
    void* state2 = alloc(*allocator, 12, 16);
    if (state2) {
        *(uint32_t*)((char*)state2 + 4) = 0;
        *(uint32_t*)((char*)state2 + 8) = (uint32_t)self;
        *(void**)state2 = (void*)0x82071494;
    }
    self->m_pStateArray[2] = state2;
    
    // State 3: stateRequestSyncronization @ 0x820714DC
    rage_AssertMainThread();
    void* state3 = alloc(*allocator, 12, 16);
    if (state3) {
        *(uint32_t*)((char*)state3 + 4) = 0;
        *(uint32_t*)((char*)state3 + 8) = (uint32_t)self;
        *(void**)state3 = (void*)0x820714DC;
    }
    self->m_pStateArray[3] = state3;
    
    // State 4: stateSendSyncronization @ 0x82071524
    rage_AssertMainThread();
    void* state4 = alloc(*allocator, 12, 16);
    if (state4) {
        *(uint32_t*)((char*)state4 + 4) = 0;
        *(uint32_t*)((char*)state4 + 8) = (uint32_t)self;
        *(void**)state4 = (void*)0x82071524;
    }
    self->m_pStateArray[4] = state4;
    
    // State 5: stateReceiveSyncronization @ 0x8207156C
    rage_AssertMainThread();
    void* state5 = alloc(*allocator, 12, 16);
    if (state5) {
        *(uint32_t*)((char*)state5 + 4) = 0;
        *(uint32_t*)((char*)state5 + 8) = (uint32_t)self;
        *(void**)state5 = (void*)0x8207156C;
    }
    self->m_pStateArray[5] = state5;
    
    // State 6: stateWaitTime @ 0x820715B4 (16 bytes, has float at +12)
    rage_AssertMainThread();
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
/**
 * NetStateSync::Reset (vfn_10) @ 0x823D1D70 | size: 0x60
 *
 * Resets the state sync to initial configuration.
 * Copies 16 bytes of default data to +20, sets default timeout,
 * clears state tracking fields.
 */
void NetStateSync_OnComplete(NetStateSync* self) {
    extern uint32_t g_defaultData16[4];  // @ 0x8261A0C0
    extern float g_netSyncTimeout;       // @ 0x82033110 (lis(-32253)+-12016)

    uint8_t* p = (uint8_t*)self;

    // Copy 16 bytes from global default to +20
    for (int i = 0; i < 4; i++) {
        ((uint32_t*)(p + 20))[i] = g_defaultData16[i];
    }

    // Set default timeout at +56
    *(float*)(p + 56) = g_netSyncTimeout;

    // Clear state fields
    *(uint32_t*)(p + 52) = 0xFFFFFFFF;  // -1
    *(uint32_t*)(p + 60) = 0;
    *(uint8_t*)(p + 68) = 0;
    *(uint8_t*)(p + 69) = 0;
    *(uint8_t*)(p + 70) = 0;
    *(uint8_t*)(p + 71) = 0;
}

/**
 * NetStateSync::IsStateReady (vfn_13) @ 0x823D1DD0 | size: 0x64
 *
 * Checks if the given state index is valid and ready.
 * If stateIndex >= stateCount, returns 0.
 * If a current state is set, dispatches to state vtable[14].
 * Otherwise returns 1 if stateIndex is 1 or 2.
 */
uint8_t NetStateSync_GetName(NetStateSync* self, int stateIndex) {
    uint8_t* p = (uint8_t*)self;
    int stateCount = *(int32_t*)(p + 4);

    if (stateIndex >= stateCount) {
        return 0;
    }

    int32_t currentState = *(int32_t*)(p + 12);
    if (currentState != -1) {
        void** stateArray = *(void***)(p + 8);
        void* stateObj = stateArray[currentState];
        if (stateObj) {
            void** vtbl = *(void***)stateObj;
            typedef uint8_t (*IsReadyFn)(void*);
            IsReadyFn isReady = (IsReadyFn)vtbl[14];
            return isReady(stateObj);
        }
    }

    // Default: state 1 or 2 returns 1, anything else returns 0
    if (stateIndex == 1 || stateIndex == 2) {
        return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// External function declarations
////////////////////////////////////////////////////////////////////////////////

extern "C" {
    // fsmMachine destructor — implemented in src/rage/fsmMachine.c
    void fsmMachine_Destructor_27A8(void* obj);
    
    // Network/rendering subsystem initializers
    void grcDevice_FinalizeRenderSetup_1(void* obj);
    void SinglesNetworkClient_2BE8_g(void* obj);
    void SinglesNetworkClient_51C8_g(void* obj);
    void SinglesNetworkClient_2E88_isl(void* obj);
    void SinglesNetworkClient_DA08(void* obj);
    void util_AA38(void* obj);
    void game_5128(void* obj);
    
    // Thread assertion
    void* rage_AssertMainThread();
    
    // Global allocator pointer @ 0x82600004
    extern void* g_allocator_ptr;
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

// Forward declarations for memory allocation and message constructors
extern void* rage_Alloc(uint32_t size);
extern void pongNetMessageHolder_70C8_wrh(void* self);
extern void pongNetMessageHolder_71C0_wrh(void* self);
extern void pongNetMessageHolder_72A8_wrh(void* self);
extern void pongNetMessageHolder_6F30_wrh(void* self);

// PongNetMessage base vtable used as sentinel when releasing pool elements
static void** const g_PongNetMessageBaseVtable = (void**)0x8206C304;


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::CreateHitMessagePool @ 0x823C08D0 | size: 0x58
//
// Lazy-initializes a pool of HitMessage objects (104816 bytes total).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_CreateHitMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    void* memory = rage_Alloc(104816);

    if (memory != nullptr) {
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
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DestroyHitMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

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

    void* memory = rage_Alloc(176);

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

    void* memory = rage_Alloc(112);

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

    void* memory = rage_Alloc(1040);

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

    uint8_t* elementPtr = (uint8_t*)buffer + (200 * 80);
    for (int i = 199; i >= 0; i--) {
        elementPtr -= 80;
        *(void**)elementPtr = (void*)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateNextMatchMessagePool @ 0x823C2910 | size: 0x50
//
// Deallocates a single NextMatchMessage object (24 bytes).
// Resets the vtable to PongNetMessage base, then frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DeallocateNextMatchMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    *(uint32_t*)buffer = (uint32_t)(uintptr_t)g_PongNetMessageBaseVtable;

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateNextMatchSpectatorMessagePool @ 0x823C2AB0 | size: 0x50
//
// Deallocates a single NextMatchSpectatorMessage object (16 bytes).
// Resets the vtable to PongNetMessage base, then frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DeallocateNextMatchSpectatorMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    *(uint32_t*)buffer = (uint32_t)(uintptr_t)g_PongNetMessageBaseVtable;

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::AllocateScoreMessagePool @ 0x823C3248 | size: 0x54
//
// Lazy allocation for the ScoreMessage pool (240 bytes).
// Calls ScoreMessage_ctor_7910 to construct the pool of 5 entries (stride 44).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_AllocateScoreMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    extern void ScoreMessage_ctor_7910(void* memory);

    void* memory = rage_Alloc(240);
    if (memory != nullptr) {
        ScoreMessage_ctor_7910(memory);
    } else {
        memory = nullptr;
    }
    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateScoreMessagePool @ 0x823C32A0 | size: 0x64
//
// Teardown for the ScoreMessage pool (5 entries, stride 44).
// Resets each entry's vtable to PongNetMessage base, then frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DeallocateScoreMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    uint8_t* cursor = (uint8_t*)buffer + 220;
    for (int i = 4; i >= 0; --i) {
        cursor -= 44;
        *reinterpret_cast<uint32_t*>(cursor) = (uint32_t)(uintptr_t)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::AllocateForfeitMatchMessagePool @ 0x823C3558 | size: 0x54
//
// Lazy allocation for the ForfeitMatchMessage pool (112 bytes).
// Calls pongNetMessageHolder_7B68_wrh to construct pool of 5 entries (stride 20).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_AllocateForfeitMatchMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    extern void pongNetMessageHolder_7B68_wrh(void* memory);

    void* memory = rage_Alloc(112);
    if (memory != nullptr) {
        pongNetMessageHolder_7B68_wrh(memory);
    } else {
        memory = nullptr;
    }
    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateForfeitMatchMessagePool @ 0x823C35B0 | size: 0x64
//
// Teardown for the ForfeitMatchMessage pool (5 entries, stride 20).
// Resets each entry's vtable to PongNetMessage base, then frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DeallocateForfeitMatchMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    uint8_t* cursor = (uint8_t*)buffer + 100;
    for (int i = 4; i >= 0; --i) {
        cursor -= 20;
        *reinterpret_cast<uint32_t*>(cursor) = (uint32_t)(uintptr_t)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::AllocateTerminateRallyMessagePool @ 0x823C3618 | size: 0x54
//
// Lazy allocation for the TerminateRallyMessage pool (128 bytes).
// Calls pongNetMessageHolder_7C48_wrh to construct pool of 10 entries (stride 12).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_AllocateTerminateRallyMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    extern void pongNetMessageHolder_7C48_wrh(void* memory);

    void* memory = rage_Alloc(128);
    if (memory != nullptr) {
        pongNetMessageHolder_7C48_wrh(memory);
    } else {
        memory = nullptr;
    }
    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateTerminateRallyMessagePool @ 0x823C3670 | size: 0x64
//
// Teardown for the TerminateRallyMessage pool (10 entries, stride 12).
// Resets each entry's vtable to PongNetMessage base, then frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DeallocateTerminateRallyMessagePool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    uint8_t* cursor = (uint8_t*)buffer + 120;
    for (int i = 9; i >= 0; --i) {
        cursor -= 12;
        *reinterpret_cast<uint32_t*>(cursor) = (uint32_t)(uintptr_t)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::AllocateInternalMessageRelayPool @ 0x823C36D8 | size: 0x54
//
// Lazy allocation for the InternalMessageRelay pool (5248 bytes).
// Calls InternalMessageRelay_ctor_7D28 to construct pool of 10 entries (stride 524).
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_AllocateInternalMessageRelayPool(void* self) {
    uint8_t* obj = (uint8_t*)self;

    if (*(void**)(obj + 8) != nullptr) {
        return;
    }

    extern void InternalMessageRelay_ctor_7D28(void* memory);

    void* memory = rage_Alloc(5248);
    if (memory != nullptr) {
        InternalMessageRelay_ctor_7D28(memory);
    } else {
        memory = nullptr;
    }
    *(void**)(obj + 8) = memory;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongNetMessageHolder::DeallocateInternalMessageRelayPool @ 0x823C3730 | size: 0x64
//
// Teardown for the InternalMessageRelay pool (10 entries, stride 524).
// Resets each entry's vtable to PongNetMessage base, then frees.
// ─────────────────────────────────────────────────────────────────────────────
void pongNetMessageHolder_DeallocateInternalMessageRelayPool(void* self) {
    uint8_t* obj = (uint8_t*)self;
    void* buffer = *(void**)(obj + 8);

    if (buffer == nullptr) {
        return;
    }

    uint8_t* cursor = (uint8_t*)buffer + 5240;
    for (int i = 9; i >= 0; --i) {
        cursor -= 524;
        *reinterpret_cast<uint32_t*>(cursor) = (uint32_t)(uintptr_t)g_PongNetMessageBaseVtable;
    }

    rage_free(buffer);
    *(void**)(obj + 8) = nullptr;
}
