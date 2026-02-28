#include "sg_node.hpp"

// RAGE engine heap free — canonical name declared in rage/rage_cm_types.hpp
extern void rage_free(void* ptr);               // @ 0x820C00C0

// atSingleton base-class destructor body @ 0x821A9420 - ReleaseSingleton
// Tears down the XML/atSingleton child-node list at field +0x000C,
// then resets the vtable pointer to the base atSingleton vtable.
// Equivalent to the body of atSingleton::~atSingleton() in the source tree.
namespace rage { void ReleaseSingleton(void* obj); }  // @ 0x821A9420

/**
 * sgNode::~sgNode
 * @ 0x8212B350 | size: 0x5C
 *
 * Resets the vtable to sgNode's own vtable, then invokes the atSingleton
 * base destructor which walks and destroys the child-node list.
 * The conditional rage_free(this) (delete-flag check in r4) is generated
 * by the scalar-dtor wrapper in the recomp ABI and is not written here.
 */
sgNode::~sgNode() {
    rage::ReleaseSingleton(this);
}

/**
 * sgNode::IsType
 * @ 0x8212B330 | size: 0x20
 *
 * Compares typeId against the global sgNode type-ID stored in the SDA.
 * Returns 1 if the type matches, 0 otherwise.
 */
int sgNode::IsType(uint32_t typeId) {
    // TODO: load global sgNode typeId from SDA and compare
    (void)typeId;
    return 0;
}

/**
 * sgNode::GetTypeName
 * @ 0x8212B2A0 | size: 0x0C
 *
 * Returns the static class-name string for RAGE RTTI queries.
 */
const char* sgNode::GetTypeName() const {
    return "sgNode";
}
