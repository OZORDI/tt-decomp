cmake_minimum_required(VERSION 3.10)
project(RockstarTableTennis VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Find required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add definitions
add_definitions(${SDL2_DEFINITIONS})

# Source files
set(SOURCES
    # CRT layer
    src/crt/entry.c
    src/crt/exit.c
    src/crt/heap.c
    src/crt/memory.c
    src/crt/startup.c
    src/crt/string.c
    src/globals.c
    src/globals_extended.c
    src/stubs_remaining.c
    
    # XAM (Xbox Application Model)
    src/xam/static_init.c
    
    # RAGE engine
    src/rage/app_init.c
    src/rage/audio_control_wrappers.cpp
    src/rage/fsmMachine.c
    src/rage/game_loop.c
    src/rage/main.c
    src/rage/rage_cm.cpp
    src/rage/rage_dat.cpp
    src/rage/rage_globals.cpp
    src/rage/rage_grm.cpp
    src/rage/rage_memory.cpp
    src/rage/render_loop.c
    src/rage/scene_render.c
    src/rage/session_events.cpp
    src/rage/subsystem_init.c
    src/rage/swf.cpp
    src/rage/timer.cpp
    
    # Game logic
    src/game/char_view.cpp
    src/game/gd_data.cpp
    src/game/gd_vib_event.cpp
    src/game/mc_memcard.cpp
    src/game/pong_audio.cpp
    src/game/pong_audio_effects.cpp
    src/game/pong_ball.cpp
    src/game/pong_camera.cpp
    src/game/pong_creature.cpp
    src/game/pong_drill.cpp
    src/game/pong_misc.cpp
    src/game/pong_network.cpp
    src/game/pong_network_classes.cpp
    src/game/pong_player.cpp
    src/game/pong_render.cpp
    src/game/pong_states.cpp
    
    # Animation
    src/anim/pcr_anim_blenders.cpp
    
    # Physics
    src/physics/ph_physics.cpp
    
    # Graphics
    src/graphics/texture_reference.cpp
    
    # Scene Graph
    src/scene/sg_node.cpp
    src/scene/sg_drawable.cpp
    src/scene/sg_physical.cpp
    src/scene/sg_scene_graph.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    pthread  # For POSIX threads on Linux/macOS
)

# Platform-specific linking
if(WIN32)
    # Windows-specific libraries
    target_link_libraries(${PROJECT_NAME}
        opengl32
        glew32
        SDL2main
        SDL2
    )
elseif(APPLE)
    # macOS-specific libraries
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
elseif(UNIX)
    # Linux-specific libraries
    target_link_libraries(${PROJECT_NAME}
        GL
        dl
        X11
    )
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy SDL2 DLLs on Windows
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_LIBRARY_DIRS}/SDL2.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print configuration info
message(STATUS "Rockstar Presents Table Tennis - Cross-Platform Rewrite")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SDL2 version: ${SDL2_VERSION}")
message(STATUS "OpenGL found: ${OPENGL_FOUND}")
message(STATUS "GLEW found: ${GLEW_FOUND}")
