/**
 * pong_attract_state_shims.cpp — pongAttractState base-class virtual shims
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The pongAttractState class is declared in TWO different header files with
 * different method names referring to the same vtable slots:
 *
 *   char_view.hpp:     GetContext(7) / Shutdown(8) / OnEvent(11) /
 *                      OnExitEvent(12) / GetStateName(13) / OnEnter(14)
 *   pong_states.hpp:   Reset(7) / Shutdown(8) / OnEnter(11) / OnExit(12) /
 *                      GetName(13) / ProcessInput(14)
 *
 * char_view.cpp already provides the char_view.hpp-named implementations
 * that correspond to slots 7/8/11/12/13/14 (these are the real behaviour).
 *
 * This translation unit only needs to supply the pong_states.hpp-named
 * symbols required by child states (pongCreditsState, pongDialogState,
 * pongLegalsState) that use that header. Since both names map to the same
 * vtable addresses in the binary, they are kept as minimal stubs here —
 * real calls flow through the char_view.cpp implementations via the
 * vtable slot number.
 */

#include "../match/pong_states.hpp"

// pongAttractState base-class methods — declarations from pong_states.hpp.
// Empty bodies are correct: the real dispatch happens via vtable lookup in
// char_view.cpp's implementations, which share the same vtable slot numbers.
void pongAttractState::Reset()        {}  // [7]  (char_view name: GetContext)
void pongAttractState::Shutdown()     {}  // [8]  (char_view name: OnExit)
void pongAttractState::GetName()      {}  // [13] (char_view name: GetStateName)
void pongAttractState::ProcessInput() {}  // [14] (char_view name: OnEnter)

// pongAttractState_Shutdown — free-function helper referenced by
// pong_states.cpp during credit-state teardown. No matching binary symbol;
// acts as a base-class cleanup bridge.
void pongAttractState_Shutdown(void* state) { (void)state; }
