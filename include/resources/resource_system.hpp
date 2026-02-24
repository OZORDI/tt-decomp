/*
 * Xbox 360 Resource Management System Header
 * Professional resource engine organization
 */

#ifndef RESOURCE_SYSTEM_HPP
#define RESOURCE_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// Resource constants - organized by category
namespace ResourceConstants {
    // Resource types
    constexpr int RESOURCE_TYPE_TEXTURE = 1;
    constexpr int RESOURCE_TYPE_MESH = 2;
    constexpr int RESOURCE_TYPE_SHADER = 3;
    constexpr int RESOURCE_TYPE_AUDIO = 4;
    constexpr int RESOURCE_TYPE_MATERIAL = 5;
    constexpr int RESOURCE_TYPE_ANIMATION = 6;
    constexpr int RESOURCE_TYPE_SCENE = 7;
    constexpr int RESOURCE_TYPE_RENDER_TARGET = 8;
    
    // Resource states
    constexpr int RESOURCE_STATE_UNLOADED = 0;
    constexpr int RESOURCE_STATE_LOADING = 1;
    constexpr int RESOURCE_STATE_LOADED = 2;
    constexpr int RESOURCE_STATE_ERROR = 3;
    
    // Resource flags
    constexpr int RESOURCE_FLAG_PERSISTENT = 0x1;
    constexpr int RESOURCE_FLAG_STREAMING = 0x2;
    constexpr int RESOURCE_FLAG_COMPRESSED = 0x4;
    constexpr int RESOURCE_FLAG_CACHED = 0x8;
    
    // Resource limits
    constexpr int MAX_RESOURCES = 1024;
    constexpr int MAX_RESOURCE_NAME_LENGTH = 256;
    constexpr int MAX_RESOURCE_PATH_LENGTH = 512;
    constexpr int RESOURCE_CACHE_SIZE = 64 * 1024 * 1024;  // 64MB
    
    // Memory management
    constexpr int MEMORY_BLOCK_SIZE = 4096;
    constexpr int MEMORY_ALIGNMENT = 16;
}

// Resource structure
struct Resource {
    uint32_t id;
    int type;
    int state;
    uint32_t flags;
    char name[ResourceConstants::MAX_RESOURCE_NAME_LENGTH];
    char path[ResourceConstants::MAX_RESOURCE_PATH_LENGTH];
    
    // Resource data
    void* data;
    uint32_t size;
    uint32_t compressed_size;
    
    // Memory management
    void* memory_block;
    uint32_t memory_offset;
    uint32_t memory_size;
    
    // Reference counting
    int ref_count;
    uint32_t last_access_time;
    
    // Loading
    struct {
        bool async_loading;
        float progress;
        int priority;
    } loading;
    
    // Cache management
    struct {
        bool cached;
        uint32_t cache_size;
        uint32_t cache_hits;
        uint32_t cache_misses;
    } cache;
};

// Resource manager structure
struct ResourceManager {
    bool initialized;
    
    // Resource pool
    Resource resources[ResourceConstants::MAX_RESOURCES];
    int resource_count;
    uint32_t next_resource_id;
    
    // Memory management
    struct {
        void* memory_pool;
        uint32_t pool_size;
        uint32_t used_memory;
        uint32_t peak_memory;
        int allocation_count;
    } memory;
    
    // Cache system
    struct {
        uint32_t cache_size;
        uint32_t cache_used;
        uint32_t cache_hits;
        uint32_t cache_misses;
        uint32_t evictions;
    } cache;
    
    // Loading system
    struct {
        bool async_loading_enabled;
        int loading_queue[64];
        int queue_head;
        int queue_tail;
        int active_loads;
    } loading;
    
    // Resource vtable
    void* resource_vtable;
};

// Resource loader interface
struct ResourceLoader {
    int (*load)(const char* path, void** data, uint32_t* size);
    int (*unload)(void* data);
    int (*get_size)(const char* path);
    bool (*can_load)(const char* path);
};

// Resource interface - clean API design
namespace Resources {
    // Core resource functions
    int initialize_system();
    int shutdown_system();
    int start_resource_system();
    int stop_resource_system();
    
    // Resource management
    uint32_t load_resource(const char* path, int type, uint32_t flags);
    uint32_t load_resource_async(const char* path, int type, uint32_t flags, int priority);
    int unload_resource(uint32_t resource_id);
    int reload_resource(uint32_t resource_id);
    
    // Resource access
    Resource* get_resource(uint32_t resource_id);
    Resource* find_resource(const char* name);
    void* get_resource_data(uint32_t resource_id);
    uint32_t get_resource_size(uint32_t resource_id);
    
    // Resource state
    int get_resource_state(uint32_t resource_id);
    float get_loading_progress(uint32_t resource_id);
    bool is_resource_loaded(uint32_t resource_id);
    
    // Memory management
    void* allocate_memory(uint32_t size);
    void free_memory(void* ptr);
    uint32_t get_memory_usage();
    uint32_t get_peak_memory_usage();
    
    // Cache management
    int cache_resource(uint32_t resource_id);
    int uncache_resource(uint32_t resource_id);
    void clear_cache();
    uint32_t get_cache_usage();
    uint32_t get_cache_hits();
    uint32_t get_cache_misses();
    
    // Resource processing
    int process_loading_queue();
    int update_resource_states();
    int cleanup_unused_resources();
    
    // Resource validation
    int validate_resource(const Resource* resource);
    int validate_resource_manager();
    
    // Performance monitoring
    int get_resource_count();
    float get_average_load_time();
    int get_loading_queue_size();
}

// Global resource state
extern ResourceManager g_resource_manager;

#endif // RESOURCE_SYSTEM_HPP
