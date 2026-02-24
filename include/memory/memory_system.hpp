/*
 * Xbox 360 Memory Management System Header
 * Professional memory engine organization
 */

#ifndef MEMORY_SYSTEM_HPP
#define MEMORY_SYSTEM_HPP

#include <cstdint>
#include <cstdlib>

// Memory constants - organized by category
namespace MemoryConstants {
    // Memory allocation types
    constexpr int ALLOC_TYPE_SYSTEM = 1;
    constexpr int ALLOC_TYPE_RAGE = 2;
    constexpr int ALLOC_TYPE_GRAPHICS = 3;
    constexpr int ALLOC_TYPE_AUDIO = 4;
    constexpr int ALLOC_TYPE_PHYSICS = 5;
    constexpr int ALLOC_TYPE_AI = 6;
    constexpr int ALLOC_TYPE_UI = 7;
    constexpr int ALLOC_TYPE_RESOURCES = 8;
    
    // Memory protection flags
    constexpr int PROTECT_READ = 0x1;
    constexpr int PROTECT_WRITE = 0x2;
    constexpr int PROTECT_EXECUTE = 0x4;
    constexpr int PROTECT_READ_WRITE = PROTECT_READ | PROTECT_WRITE;
    constexpr int PROTECT_READ_EXECUTE = PROTECT_READ | PROTECT_EXECUTE;
    constexpr int PROTECT_ALL = PROTECT_READ | PROTECT_WRITE | PROTECT_EXECUTE;
    
    // Memory alignment
    constexpr int ALIGNMENT_DEFAULT = 16;
    constexpr int ALIGNMENT_PAGE = 4096;
    constexpr int ALIGNMENT_CACHE_LINE = 64;
    constexpr int ALIGNMENT_GPU = 256;
    
    // Memory limits
    constexpr int MAX_ALLOCATIONS = 10000;
    constexpr int MAX_HEAP_SIZE = 512 * 1024 * 1024;  // 512MB
    constexpr int MAX_VIRTUAL_MEMORY = 1024 * 1024 * 1024;  // 1GB
    constexpr int MAX_PHYSICAL_MEMORY = 512 * 1024 * 1024;  // 512MB
    
    // GPU buffer sizes
    constexpr int GPU_COMMAND_BUFFER_SIZE = 64 * 1024;  // 64KB
    constexpr int GPU_RING_BUFFER_SIZE = 256 * 1024;     // 256KB
    constexpr int GPU_SCALER_BUFFER_SIZE = 32 * 1024;   // 32KB
    
    // Thread stack sizes
    constexpr int THREAD_STACK_SIZE = 64 * 1024;  // 64KB
    constexpr int TLS_SLOT_COUNT = 64;
    
    // Memory tracking
    constexpr int TRACK_ALLOCATIONS = 1;
    constexpr int TRACK_LEAKS = 1;
    constexpr int TRACK_FRAGMENTS = 1;
}

// Memory block structure
struct MemoryBlock {
    void* address;
    uint32_t size;
    uint32_t actual_size;
    int allocation_type;
    int protection_flags;
    bool is_allocated;
    bool is_pinned;
    
    // Debug information
    struct {
        const char* file;
        int line;
        const char* function;
        uint32_t allocation_id;
        uint64_t timestamp;
    } debug;
    
    // Linked list for free blocks
    struct {
        MemoryBlock* next;
        MemoryBlock* prev;
    } list;
    
    // Padding for alignment
    uint8_t padding[16];
};

// Memory allocator interface
struct MemoryAllocator {
    virtual void* allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT) = 0;
    virtual void free(void* ptr) = 0;
    virtual uint32_t get_allocated_size() = 0;
    virtual uint32_t get_free_size() = 0;
    virtual bool validate_heap() = 0;
    virtual void dump_statistics() = 0;
    
    // Allocator metadata
    struct {
        const char* name;
        int allocator_type;
        uint32_t total_allocated;
        uint32_t peak_allocated;
        uint32_t allocation_count;
        uint32_t free_count;
    } metadata;
};

// System memory allocator
struct SystemAllocator : public MemoryAllocator {
    void* allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT) override;
    void free(void* ptr) override;
    uint32_t get_allocated_size() override;
    uint32_t get_free_size() override;
    bool validate_heap() override;
    void dump_statistics() override;
    
    // System allocator specific
    struct {
        void* heap_base;
        uint32_t heap_size;
        MemoryBlock* free_list;
        uint32_t fragmentation_count;
    } system_data;
};

// Simple memory allocator
struct SimpleAllocator : public MemoryAllocator {
    void* allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT) override;
    void free(void* ptr) override;
    uint32_t get_allocated_size() override;
    uint32_t get_free_size() override;
    bool validate_heap() override;
    void dump_statistics() override;
    
    // Simple allocator specific
    struct {
        void* memory_pool;
        uint32_t pool_size;
        uint32_t pool_used;
        uint8_t* bitmap;
        uint32_t bitmap_size;
    } simple_data;
};

// RAGE memory allocator
struct RageAllocator : public MemoryAllocator {
    void* allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT) override;
    void free(void* ptr) override;
    uint32_t get_allocated_size() override;
    uint32_t get_free_size() override;
    bool validate_heap() override;
    void dump_statistics() override;
    
    // RAGE allocator specific
    struct {
        void* rage_heap;
        uint32_t rage_heap_size;
        uint32_t allocation_granularity;
        MemoryBlock* rage_blocks;
    } rage_data;
};

// Thread context structure
struct ThreadContext {
    uint32_t thread_id;
    void* stack_base;
    uint32_t stack_size;
    void* tls_slots[MemoryConstants::TLS_SLOT_COUNT];
    
    // Thread memory tracking
    struct {
        uint32_t allocations_count;
        uint32_t total_allocated;
        MemoryBlock* thread_blocks;
    } memory_tracking;
};

// Memory manager structure
struct MemoryManager {
    bool initialized;
    
    // Allocators
    SystemAllocator system_allocator;
    SimpleAllocator simple_allocator;
    RageAllocator rage_allocator;
    
    // Memory pools
    struct {
        void* virtual_memory;
        uint32_t virtual_size;
        void* physical_memory;
        uint32_t physical_size;
        uint32_t allocated_virtual;
        uint32_t allocated_physical;
    } memory_pools;
    
    // GPU buffers
    struct {
        void* command_buffer;
        uint32_t command_buffer_size;
        void* ring_buffer;
        uint32_t ring_buffer_size;
        void* scaler_buffer;
        uint32_t scaler_buffer_size;
        uint32_t gpu_identifier;
    } gpu_buffers;
    
    // Thread management
    struct {
        ThreadContext* contexts[64];
        int context_count;
        uint32_t next_thread_id;
    } thread_management;
    
    // Memory tracking
    struct {
        MemoryBlock allocations[MemoryConstants::MAX_ALLOCATIONS];
        int allocation_count;
        uint32_t total_allocated;
        uint32_t peak_allocated;
        uint32_t fragmentation_count;
        uint64_t allocation_counter;
    } tracking;
    
    // Memory protection
    struct {
        bool protection_enabled;
        uint32_t protection_violations;
        uint32_t last_violation_address;
    } protection;
};

// Memory interface - clean API design
namespace Memory {
    // Core memory functions
    int initialize_system();
    int shutdown_system();
    int start_memory_system();
    int stop_memory_system();
    
    // RAGE memory functions
    void* rage_allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT);
    void rage_free(void* ptr);
    int rage_threadpool_cleanup();
    
    // System allocator functions
    void* sys_mem_allocator_allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT);
    void sys_mem_allocator_free(void* ptr);
    void* sys_mem_simple_allocator_allocate(uint32_t size, int alignment = MemoryConstants::ALIGNMENT_DEFAULT);
    void sys_mem_simple_allocator_free(void* ptr);
    
    // Thread context functions
    ThreadContext* xe_alloc_thread_context();
    void xe_free_thread_context(ThreadContext* context);
    
    // CRT heap functions
    int heap_init_impl();
    int heap_init_check();
    
    // TLS functions
    int ke_tls_alloc();
    void ke_tls_free(int tls_index);
    void* ke_tls_get_value(int tls_index);
    void ke_tls_set_value(int tls_index, void* value);
    
    // Virtual memory functions
    void* nt_allocate_virtual_memory(uint32_t size, int protection_flags);
    void nt_free_virtual_memory(void* address, uint32_t size);
    uint32_t mm_query_allocation_size(void* address);
    int mm_query_address_protection(void* address);
    
    // Physical memory functions
    void* mm_allocate_physical_memory_ex(uint32_t size, int alignment);
    void mm_free_physical_memory(void* address, uint32_t size);
    
    // GPU buffer functions
    void vd_set_system_command_buffer_gpu_identifier_address(uint32_t identifier);
    void vd_enable_ring_buffer_r_ptr_write_back();
    void vd_initialize_ring_buffer();
    void* vd_get_system_command_buffer();
    void vd_initialize_scaler_command_buffer();
    
    // File buffer functions
    int nt_flush_buffers_file(void* file_handle);
    
    // Stack checking functions
    void rtl_check_stack();
    void rtl_check_stack12();
    
    // Memory protection functions
    int set_memory_protection(void* address, uint32_t size, int protection_flags);
    int validate_memory_protection(void* address, uint32_t size, int protection_flags);
    
    // Memory tracking functions
    void track_allocation(void* ptr, uint32_t size, int type, const char* file, int line, const char* function);
    void track_deallocation(void* ptr);
    void dump_memory_leaks();
    void dump_memory_statistics();
    bool validate_heap_integrity();
    
    // Performance monitoring
    uint32_t get_total_allocated();
    uint32_t get_peak_allocated();
    uint32_t get_fragmentation_count();
    float get_fragmentation_ratio();
    uint32_t get_allocation_count();
    uint32_t get_free_memory();
}

// Global memory state
extern MemoryManager g_memory_manager;

#endif // MEMORY_SYSTEM_HPP
