#pragma once

#include <stdint.h>
#include <stdbool.h>

void InputInit(void);
void InputUpdate(void);
bool InputButtonPressed(uint32_t button);
bool InputButtonDown(uint32_t button);
