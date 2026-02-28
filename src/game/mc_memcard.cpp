/**
 * mc_memcard.cpp — Xbox 360 memory card/save-data state machine
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "mc_memcard.hpp"

#include "rage/heap.hpp"
#include "rage/subsystems.h"

#include <cstdint>
#include <cstring>

extern "C" {
void atSingleton_2E60_g(void* singleton);  // @ 0x822E2E60
void* rage_obj_factory_create_3040(void* singleton,
                                   const void* typeInfo,
                                   const void* typeOps,
                                   std::int32_t flags,
                                   std::int32_t isRootType);  // @ 0x822E3040
void rage_obj_bind_3828(void* factoryContext, void* storage, std::uint32_t size);  // @ 0x822E3828
void rage_obj_finalize_3B38(void* factoryContext);  // @ 0x822E3B38
void nop_8240E6D0(const void* messageTag,
                  const void* typeInfo,
                  const void* typeOps,
                  std::uint32_t typeSize);  // @ 0x8240E6D0
}

namespace {

constexpr std::int32_t kMemcardStateCount = 11;

constexpr std::uintptr_t kDatBaseVtable               = 0x820276C4;
constexpr std::uintptr_t kFsmMachineVtable            = 0x8204DD14;
constexpr std::uintptr_t kMemcardControlVtable        = 0x8204D684;
constexpr std::uintptr_t kMemcardTypeInfoDescriptor   = 0x8204D650;
constexpr std::uintptr_t kMemcardTypeOpsDescriptor    = 0x8204D660;
constexpr std::uintptr_t kMemcardTypeDebugLabel       = 0x8204D664;
constexpr std::uintptr_t kTypeFactorySingletonAddress = 0x825D0080;
constexpr std::uintptr_t kTypeFactoryRefCountAddress  = 0x825D0680;

constexpr std::uintptr_t kIdleStateVtable         = 0x8204D6D4;
constexpr std::uintptr_t kFindStateVtable         = 0x8204D714;
constexpr std::uintptr_t kSelectStateVtable       = 0x8204D754;
constexpr std::uintptr_t kLoadStateVtable         = 0x8204D794;
constexpr std::uintptr_t kSaveStateVtable         = 0x8204D7D4;
constexpr std::uintptr_t kSaveIconStateVtable     = 0x8204D814;
constexpr std::uintptr_t kSearchStateVtable       = 0x8204D854;
constexpr std::uintptr_t kRemoveStateVtable       = 0x8204D894;
constexpr std::uintptr_t kGetFreeSpaceStateVtable = 0x8204D8D4;
constexpr std::uintptr_t kFormatStateVtable       = 0x8204D914;
constexpr std::uintptr_t kUnformatStateVtable     = 0x8204D954;
constexpr std::uintptr_t kSegmentContainerVtable  = 0x8204D9B0;

constexpr mcMemcardControl::State kShutdownOrder[kMemcardStateCount] = {
    mcMemcardControl::STATE_UNFORMAT,
    mcMemcardControl::STATE_FORMAT,
    mcMemcardControl::STATE_GET_FREE_SPACE,
    mcMemcardControl::STATE_REMOVE,
    mcMemcardControl::STATE_SAVE,
    mcMemcardControl::STATE_SAVE_ICON,
    mcMemcardControl::STATE_SEARCH,
    mcMemcardControl::STATE_LOAD,
    mcMemcardControl::STATE_SELECT,
    mcMemcardControl::STATE_FIND,
    mcMemcardControl::STATE_IDLE,
};

struct mcFactoryBuildContext {
    void* m_pFactoryObject;
    void* m_pFactoryType;
};

using QueryFactoryObjectSizeFn = std::uint32_t (*)(void*, void*);
using DeletingStateDtorFn = void (*)(mcMemcardState*, std::uint32_t);

static void DestroyFsmMachineStorage(fsmMachine* machine)
{
    machine->m_pVtable = reinterpret_cast<void*>(kFsmMachineVtable);

    if (machine->m_ppRegisteredStates != nullptr) {
        rage_free(machine->m_ppRegisteredStates);
        machine->m_ppRegisteredStates = nullptr;
    }

    machine->m_pVtable = reinterpret_cast<void*>(kDatBaseVtable);
}

static mcMemcardControl::StateRecord* AllocateStateRecord(std::uintptr_t stateVtable)
{
    xe_main_thread_init();

    auto* state = static_cast<mcMemcardControl::StateRecord*>(
        rage_malloc(sizeof(mcMemcardControl::StateRecord)));
    if (state == nullptr) {
        return nullptr;
    }

    state->m_pVtable = reinterpret_cast<void*>(stateVtable);
    state->m_statePhase = 0;
    return state;
}

static mcMemcardControl::ExtendedStateRecord* AllocateExtendedStateRecord(std::uintptr_t stateVtable)
{
    xe_main_thread_init();

    auto* state = static_cast<mcMemcardControl::ExtendedStateRecord*>(
        rage_malloc(sizeof(mcMemcardControl::ExtendedStateRecord)));
    if (state == nullptr) {
        return nullptr;
    }

    state->m_pVtable = reinterpret_cast<void*>(stateVtable);
    state->m_statePhase = 0;
    state->m_stateToken = 0;
    return state;
}

static void DestroyStateRecord(mcMemcardState* state)
{
    if (state == nullptr) {
        return;
    }

    auto** vtable = *reinterpret_cast<void***>(state);
    auto deletingDestructor = reinterpret_cast<DeletingStateDtorFn>(vtable[0]);
    deletingDestructor(state, 1);
}

static std::uint32_t QueryFactoryObjectSize(void* factoryContext)
{
    if (factoryContext == nullptr) {
        return 0;
    }

    auto* buildContext = static_cast<mcFactoryBuildContext*>(factoryContext);
    if (buildContext->m_pFactoryObject == nullptr) {
        return 0;
    }

    auto** vtable = *reinterpret_cast<void***>(buildContext->m_pFactoryObject);
    auto querySize = reinterpret_cast<QueryFactoryObjectSizeFn>(vtable[7]);
    return querySize(buildContext->m_pFactoryObject, buildContext->m_pFactoryType);
}

}  // namespace

mcMemcardControl::~mcMemcardControl()
{
    m_pVtable = reinterpret_cast<void*>(kMemcardControlVtable);
    m_saveOperation.m_pVtable = reinterpret_cast<void*>(kDatBaseVtable);
    DestroyFsmMachineStorage(this);
}

void mcMemcardControl::Init()
{
    m_registeredStateCount = kMemcardStateCount;

    xe_main_thread_init();
    m_ppRegisteredStates = static_cast<mcMemcardState**>(
        rage_malloc(sizeof(mcMemcardState*) * kMemcardStateCount));
    if (m_ppRegisteredStates == nullptr) {
        return;
    }

    for (std::int32_t i = 0; i < kMemcardStateCount; ++i) {
        m_ppRegisteredStates[i] = nullptr;
    }

    m_ppRegisteredStates[STATE_IDLE] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kIdleStateVtable));
    m_ppRegisteredStates[STATE_FIND] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kFindStateVtable));
    m_ppRegisteredStates[STATE_SELECT] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kSelectStateVtable));
    m_ppRegisteredStates[STATE_LOAD] = reinterpret_cast<mcMemcardState*>(
        AllocateExtendedStateRecord(kLoadStateVtable));
    m_ppRegisteredStates[STATE_SAVE] = reinterpret_cast<mcMemcardState*>(
        AllocateExtendedStateRecord(kSaveStateVtable));
    m_ppRegisteredStates[STATE_SAVE_ICON] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kSaveIconStateVtable));
    m_ppRegisteredStates[STATE_SEARCH] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kSearchStateVtable));
    m_ppRegisteredStates[STATE_REMOVE] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kRemoveStateVtable));
    m_ppRegisteredStates[STATE_GET_FREE_SPACE] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kGetFreeSpaceStateVtable));
    m_ppRegisteredStates[STATE_FORMAT] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kFormatStateVtable));
    m_ppRegisteredStates[STATE_UNFORMAT] =
        reinterpret_cast<mcMemcardState*>(AllocateStateRecord(kUnformatStateVtable));

    m_typeRegistration.m_pTypeInfo = reinterpret_cast<const void*>(kMemcardTypeInfoDescriptor);
    m_typeRegistration.m_pTypeOps = reinterpret_cast<const void*>(kMemcardTypeOpsDescriptor);
    m_typeRegistration.m_pTypeStorage = nullptr;
    m_typeRegistration.m_typeStorageSize = 0;

    void* typeFactorySingleton = reinterpret_cast<void*>(kTypeFactorySingletonAddress);
    atSingleton_2E60_g(typeFactorySingleton);

    void* factoryContext = rage_obj_factory_create_3040(typeFactorySingleton,
                                                        m_typeRegistration.m_pTypeInfo,
                                                        m_typeRegistration.m_pTypeOps,
                                                        0,
                                                        1);

    if (factoryContext != nullptr) {
        m_typeRegistration.m_typeStorageSize = QueryFactoryObjectSize(factoryContext);

        xe_main_thread_init();
        m_typeRegistration.m_pTypeStorage = rage_malloc(m_typeRegistration.m_typeStorageSize);

        rage_obj_bind_3828(factoryContext,
                           m_typeRegistration.m_pTypeStorage,
                           m_typeRegistration.m_typeStorageSize);
        rage_obj_finalize_3B38(factoryContext);
    }

    nop_8240E6D0(reinterpret_cast<const void*>(kMemcardTypeDebugLabel),
                 m_typeRegistration.m_pTypeInfo,
                 m_typeRegistration.m_pTypeOps,
                 m_typeRegistration.m_typeStorageSize);

    --(*reinterpret_cast<std::uint32_t*>(kTypeFactoryRefCountAddress));
}

void mcMemcardControl::Shutdown()
{
    if (m_ppRegisteredStates == nullptr) {
        return;
    }

    for (std::int32_t i = 0; i < kMemcardStateCount; ++i) {
        const State stateIndex = kShutdownOrder[i];
        DestroyStateRecord(m_ppRegisteredStates[stateIndex]);
        m_ppRegisteredStates[stateIndex] = nullptr;
    }

    rage_free(m_ppRegisteredStates);
    m_ppRegisteredStates = nullptr;
}

void mcMemcardControl::Reset()
{
    m_primaryResultCode = -1;
    m_secondaryResultCode = -1;

    m_saveOperation.m_deviceResultCode = -1;
    m_saveOperation.m_operationResultCode = 0;
    m_saveOperation.m_hasPendingRequest = false;

    std::memset(m_saveOperation.m_szContentFileName,
                0,
                sizeof(m_saveOperation.m_szContentFileName));
}

bool mcMemcardControl::IsTransitionAllowed(std::int32_t targetState) const
{
    if (m_activeState == STATE_IDLE) {
        return true;
    }

    if (targetState == STATE_IDLE) {
        return true;
    }

    if (m_activeState == STATE_SAVE &&
        (targetState == STATE_SAVE_ICON || targetState == STATE_REMOVE)) {
        return true;
    }

    if (m_activeState == STATE_REMOVE && targetState == STATE_SAVE) {
        return true;
    }

    return false;
}

void mcMemcardControl::RequestOp()
{
    if (m_activeState != STATE_IDLE) {
        return;
    }

    m_saveOperation.m_hasPendingRequest = true;
}

fsmMachine::~fsmMachine()
{
    DestroyFsmMachineStorage(this);
}

mcSegmentContainer::~mcSegmentContainer()
{
    m_pVtable = reinterpret_cast<void*>(kSegmentContainerVtable);

    if (m_hasSegmentData != 0) {
        rage_free(m_pSegmentData);
    }

    m_pVtable = reinterpret_cast<void*>(kDatBaseVtable);
}

// mcFileSegment has RTTI/vtable presence but no observed usage in the game code.

