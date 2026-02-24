#pragma once

#include <cstdint>
#include <stdbool.h>

struct GameLoop;

// Game loop lifecycle
void gameLoop_Init(void);
void gameLoop_Shutdown(void);

// Frame execution
void gameLoop_StepFrame(void);

// Frame phases
void gameLoop_Update(void);
void gameLoop_Tick(void);
void gameLoop_PreRender(void);
void gameLoop_BeginRender(void);
void gameLoop_Render(void);
void gameLoop_EndRender(void);
void gameLoop_PostRender(void);
void gameLoop_Present(void);
