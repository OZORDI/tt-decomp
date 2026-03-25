/**
 * stubs_linker_3b.cpp — pongCharViewContext stub implementations
 * Separated to avoid header conflicts with pong_states.hpp
 */

// char_view.hpp forward-declares `class atSingleton;` (without rage:: namespace)
// then uses it as base class for charViewData. Provide a minimal definition.
class atSingleton {
public:
    virtual ~atSingleton() = default;
};

#include "game/char_view.hpp"

void pongCharViewContext::vfn_11() {
}

void pongCharViewContext::vfn_12() {
}

void pongCharViewContext::vfn_16() {
}
