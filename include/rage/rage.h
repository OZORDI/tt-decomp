#pragma once

/**
 * RAGE Engine Core
 * 
 * Main header for RAGE (Rockstar Advanced Game Engine) functionality.
 */

#include <cstdint>
#include <cstddef>

namespace rage {

// Forward declarations
class Allocator;
struct GameObject;
struct InputState;

// Memory management
Allocator* GetAllocator();
void* Allocate(size_t size);
void Free(void* ptr);

// Application state
bool IsAppStarted();
void SetAppStarted(bool started);

// Subsystem initialization
void InitializeSubsystems();

// Logging
void Log(const char* fmt, ...);

// Game object management
void InitializeGameObject(GameObject* obj);

// Game time
uint32_t GetGameTime();
void UpdateGameTime(uint32_t delta_ms);
void SetGameTimePaused(bool paused);
uint32_t GetDeltaTime();

// Input
void ProcessInput();
const InputState* GetInputState();
bool IsButtonPressed(uint32_t button_mask);
bool IsButtonHeld(uint32_t button_mask);
bool IsButtonReleased(uint32_t button_mask);

// Physics
void UpdatePhysics(float delta_time);
void SetPhysicsTimeScale(float scale);
void SetPhysicsPaused(bool paused);
uint32_t GetActivePhysicsBodies();

// Rendering
void RenderFrame();
uint32_t GetFrameCount();
float GetFrameTime();
void SetVSyncEnabled(bool enabled);

// Audio
void UpdateAudio(float delta_time);
void SetMasterVolume(float volume);
void SetMusicVolume(float volume);
void SetSFXVolume(float volume);
void SetAudioMuted(bool muted);
uint32_t GetActiveSounds();

// Network
void UpdateNetwork(float delta_time);
bool ConnectToSession(const char* session_id);
bool HostSession();
void Disconnect();
bool IsConnected();
bool IsHost();
uint32_t GetPlayerCount();
float GetPing();

} // namespace rage
