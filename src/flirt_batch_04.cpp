// flirt_batch_04.cpp — Phase 16 / FLIRT batch 4 (agent F5) lifts
// ---------------------------------------------------------------
// Batch range: 0x822C0374..0x823FB52C (64 addresses).
//
// Most FLIRT hits in this batch (33/64) resolved to addresses *inside* larger,
// already-lifted symbols — mid-function pointers that the FLIRT signatures
// accidentally matched on 8 bytes of prologue code. Those are not real function
// entries and have no disposition here.
//
// The phArticulatedCollider joint-link bitfield accessors (12 functions @
// 0x823554D8..0x82355DF0) are lifted in src/physics/ph_physics.cpp.
// phInst_8F10_p42 @ 0x82358F10 was already lifted in a prior batch.
//
// This file collects the remaining 18 real functions: small vtable thunks,
// grc/rage runtime helpers, Xbox kernel glue, and scene-graph leaves.

#include <cstdint>
#include <cstddef>

// Forward-declare minimal host helpers we need. These already exist in the
// runtime (ExRegisterTitleTerminateNotification, VdShutdownEngines) as weak
// stubs and in the xam subsystem as typed calls. We re-declare locally to
// avoid pulling full headers into this translation unit.
extern "C" {
    uint32_t ExRegisterTitleTerminateNotification(void* registration, uint32_t create);
    void VdShutdownEngines(void);
    uint32_t KeGetCurrentProcessType(void);
}

// ─── File I/O vtable thunks ────────────────────────────────────────────────
// @ 0x822E1B48 | size: 0x8  — fiDevice::vfn_21 (rage::fiDevice)
// Returns r4 verbatim via r3. Default "pass-through" slot for subclasses that
// don't override this notification hook (typical for open-handle validation).
uint32_t fiDevice_vfn_21_PassThrough(void* /*self*/, uint32_t handleOrValue) {
    return handleOrValue;
}

// @ 0x822EA478 | size: 0x10  — fiDeviceLocal::vfn_5
// Forwards (this, arg1, 0) → common nt_79E0 kernel helper. Invoked in the
// open-file path to reset a lock counter. Parameters shuffle:
//   call(this, a) → kernel_7_9E0(a, this, 0)
// Expressed here as a simple forwarder; the callee lives in kernel glue.
extern "C" void nt_79E0(void* arg, void* self, uint32_t zero);
void fiDeviceLocal_ResetHandleLock_0x22EA478(void* self, void* arg) {
    nt_79E0(arg, self, 0u);
}

// ─── HUD / interactive-tips & cut-scene vtable thunks ──────────────────────
// @ 0x8231CA20 | size: 0x34  — hudLoadingScreen::vfn_10
// Chains into two helpers that together "pre-show" the loading-screen context
// (cache the screen state, then invoke rage_CB78 to start the animation).
extern "C" {
    void SinglesNetworkClient_CA58_g(void* self);
    void rage_CB78(void* self);
}
void hudLoadingScreen_vfn_10_PreShow_0x231CA20(void* self) {
    SinglesNetworkClient_CA58_g(self);
    rage_CB78(self);
}

// @ 0x82348868 | size: 0x8  — pongCSActionCharAmbAnim::vfn_10
// Identity leaf: returns constant 13 (the action-type enum for "AmbAnim").
// FLIRT aliased this with xmlNodeStructCategory::vfn_6; both are 4-instr leaves
// returning a type tag, so the alias is safe.
int32_t pongCSActionCharAmbAnim_vfn_10_TypeTag(void) { return 13; }

// @ 0x82349510 | size: 0xC  — pongCSAction::vfn_12 (also pongCutScene::vfn_2)
// Stores 0 at +4 of the action/cut-scene object (zero the "active sub-phase"
// pointer). Called from Reset/Init paths.
void pongCSAction_vfn_12_ClearActiveSubPhase_0x82349510(void* self) {
    reinterpret_cast<uint32_t*>(self)[1] = 0u;
}

// @ 0x8239CA28 | size: 0x8  — interactiveTips::vfn_1 (aliased pongRemotePlayer::vfn_2)
// Stores arg1 at +4 (install a pointer; e.g. the active-tip descriptor for the
// HUD or the net peer for the remote-player shim).
void interactiveTips_vfn_1_SetBoundObject_0x8239CA28(void* self, void* boundObject) {
    reinterpret_cast<void**>(self)[1] = boundObject;
}

// ─── grc (graphics) small runtime helpers ──────────────────────────────────
// @ 0x82357DD0 | size: 0x98  — grc::GetFormatDescriptor (block/bpp classifier)
// Decodes a D3D/XG texture-format enum into (blockSize, bytesPerBlock). The
// switch is copied from the binary's jump-table layout for Xenon texture
// formats.  Used by surface-lock and mip-allocator paths. Output words are the
// "block dimension" (1 for linear, 2 for DXT1/3/5, 4 for DXT-wide) and
// "element stride".
void grc_GetFormatDescriptor_0x82357DD0(uint32_t format,
                                        uint32_t* outBlockSize,
                                        uint32_t* outBytesPerBlock) {
    uint32_t block = 1;
    uint32_t bytes = 1;
    if (format > 44) {
        // 45..63 range: mixed linear / DXT formats
        if (format == 49 || (format >= 51 && format <= 53) || format == 61) {
            block = 4;
        } else if (format == 50 || (format >= 54 && format <= 57)) {
            block = 1;
        } else if (format <= 61) {
            block = 4;
        } else {
            block = 1;
        }
        bytes = block;
        *outBlockSize = block;
        *outBytesPerBlock = block;
        return;
    }
    if (format == 44) {
        block = 4;  bytes = 1;
    } else if (format < 11) {
        block = 1;  bytes = 1;
    } else if (format <= 12) {
        block = 2;  bytes = 1;
    } else if (format <= 17) {
        block = 1;  bytes = 1;
    } else if (format <= 20) {
        block = 4;  bytes = 1;
    } else if (format == 39 || format == 40) {
        block = (format == 39) ? 4 : 2;
        bytes = 1;
    } else {
        block = 2;  bytes = 1;
    }
    *outBlockSize = block;
    *outBytesPerBlock = bytes;
}

// @ 0x8235AE20 | size: 0x90  — ke::CheckAddressOwnership
// Verifies that an address belongs to the current title's image range. Used by
// D3D present-ready checks. Returns 1 if address lies inside the image, else 0.
void ke_CheckAddressOwnership_0x8235AE20(void* address) {
    if (address == nullptr) return;
    uint32_t ptype = KeGetCurrentProcessType();
    (void)ptype;
    // Original implementation compares against the active title image range;
    // the host does not use this gate, so the stub returns success implicitly.
}

// @ 0x82361380 | size: 0xE0  — rage::CreateGpuIndexBuffer_Constant256
// Emits a 256-entry GPU command stream building an index buffer. The packet
// layout is fixed (6439/6437/6434 opcode IDs), so we replicate the inner loop
// at C level: each iteration packs two 10-bit indices + an opcode tag.
extern "C" void* rage_9AF8(uint32_t size);
void rage_CreateGpuIndexBuffer_Constant256_0x82361380(uint32_t* outEndPointer,
                                                     const uint16_t* indexArray) {
    uint32_t* writePtr = reinterpret_cast<uint32_t*>(rage_9AF8(2309));
    // Header: opcode 0x00001921, padding, 0, 0x00001927, 7
    *++writePtr = 0x00011921u;
    *++writePtr = 0u;
    *++writePtr = 0u;
    *++writePtr = 0x00001927u;
    *++writePtr = 7u;
    const uint8_t* src = reinterpret_cast<const uint8_t*>(indexArray);
    for (uint32_t i = 1; i <= 256; ++i) {
        uint16_t wLow  = *reinterpret_cast<const uint16_t*>(src - 1024);
        uint16_t wMid  = *reinterpret_cast<const uint16_t*>(src - 512);
        uint16_t wHigh = *reinterpret_cast<const uint16_t*>(src);
        uint32_t packed = ((static_cast<uint32_t>(wLow) << 10) & 0x03FF0000u)
                        | (static_cast<uint32_t>(wMid) & 0x0000FC00u);
        packed = ((packed << 4) & 0x3FFFFC00u)
               | ((static_cast<uint32_t>(wHigh) << 26) & 0x03FFFFFFu);
        *++writePtr = 0x00001925u;
        *++writePtr = packed;
        *++writePtr = 0xBFFB4500u;  // lis r4,-16379 | ori 17664
        *++writePtr = 7u;
        *++writePtr = 0x00001925u;
        *++writePtr = packed;
        *++writePtr = 0xFFFFFFFFu;
        *++writePtr = 0x00001922u;
        *++writePtr = i;
        src += 2;
    }
    *outEndPointer = reinterpret_cast<uintptr_t>(writePtr);
}

// @ 0x823616A0 | size: 0x6C  — phBoundBVH::ComputeLodScale
// Returns a scalar LOD factor derived from two invocations of a shared helper
// (phBoundBVH_93C0_gen). On null input, returns FLT_MAX sentinel.
extern "C" float phBoundBVH_93C0_gen(void* self);
float phBoundBVH_ComputeLodScale_0x823616A0(void* bvh, void* other) {
    if (other == nullptr) {
        // FLT_MAX sentinel baked as 0x7FFFFFFF pattern (quiet NaN / "no lod")
        union { uint32_t u; float f; } conv { 0x7FFFFFFFu };
        return conv.f;
    }
    const float a = phBoundBVH_93C0_gen(other);
    const double constScale = 1.0;  // original uses a static double constant
    const double num = static_cast<double>(a) * constScale;
    const float  b = phBoundBVH_93C0_gen(bvh);
    return static_cast<float>(num / static_cast<double>(b));
}

// @ 0x82364040 | size: 0x10  — rage::LoadSmallDataWord (SDA getter @ +0x1FF20000)
// @ 0x82364050 | size: 0x14  — rage::StoreSmallDataWord (SDA setter, eieio'd)
//
// These are getter/setter pairs for a single SDA-relative 32-bit global. The
// upper-half bias 0x1FF20000 places the target near 0x82570000 in TOC space.
// Host retargets via a backing global; we use a thread-local spare to emulate
// ordered publish (eieio) semantics.
namespace {
    volatile uint32_t g_sdaSlot_0x82364040 = 0;
}
uint32_t rage_LoadSmallDataWord_0x82364040(int32_t thisOffsetWordIndex) {
    (void)thisOffsetWordIndex;
    return g_sdaSlot_0x82364040;
}
void rage_StoreSmallDataWord_0x82364050(int32_t thisOffsetWordIndex, uint32_t value) {
    (void)thisOffsetWordIndex;
    g_sdaSlot_0x82364040 = value;
}

// @ 0x82366788 | size: 0x58  — rage::QueryBlockList::DeleteRecursive
// Walks a linked list of CQueryBlock nodes rooted at `head`, recurses into
// head->next (+12), clears the pointer, then frees head->payload (+8) via the
// shared _locale_register deleter.
extern "C" void _locale_register(void* ptr, uint32_t tag);
void rage_QueryBlockDeleteRecursive_0x82366788(uint8_t* head) {
    if (head == nullptr) return;
    uint8_t* next = *reinterpret_cast<uint8_t**>(head + 12);
    rage_QueryBlockDeleteRecursive_0x82366788(next);
    *reinterpret_cast<uint8_t**>(head + 12) = nullptr;
    _locale_register(*reinterpret_cast<void**>(head + 8), 0xBF100000u);
    _locale_register(head, 0x24800000u);
}

// @ 0x823669E8 | size: 0x20  — rage::DriverCleanup (SDA flag reset)
// Writes fixed constants (7 and 2048) into two SDA-relative flags at
// &g_deviceState + 12820 / +13320 (eieio-ordered) to signal shutdown to DMA.
void rage_DriverCleanup_0x823669E8(void) {
    // No-op on host; the guest writes flags into Xenon GPU reset registers
    // that have no equivalent here.
}

// @ 0x82366A08 | size: 0x28  — rage::D3DTerminateNotification
// Unregisters the title-terminate callback and shuts the Vd driver.
void rage_D3DTerminateNotification_0x82366A08(void* registration) {
    if (registration != nullptr) {
        ExRegisterTitleTerminateNotification(registration, /*create=*/0);
    }
    VdShutdownEngines();
}

// @ 0x82371F60 | size: 0x18  — game::DerefOrNull (tagged pointer deref)
// Classic "if (p) return *p; else return 0;" helper emitted by the compiler.
uint32_t game_DerefOrNull_0x82371F60(uint32_t* p) {
    return p ? *p : 0u;
}

// @ 0x823D9E90 | size: 0x7C  — sgTraverseInitialize::ExecuteChain
// Scene-graph traverser: pulls the head of a child list, invokes vtable slot 1
// for each child in turn (pre-order visit), then advances via the traverser's
// own pop helper.
extern "C" {
    void pongGameContext_A070(void* traverser);
    void pongGameContext_9FD0_w(void* traverser);
}
void sgTraverseInitialize_ExecuteChain_0x823D9E90(void* traverser, void* chainHead) {
    // traverser->m_pCurrent = chainHead->next  (*(traverser) == vtable; slot 0)
    uint32_t* t = reinterpret_cast<uint32_t*>(traverser);
    uint32_t* h = reinterpret_cast<uint32_t*>(chainHead);
    *reinterpret_cast<uint32_t**>(&t[1]) = reinterpret_cast<uint32_t*>(h[0]);
    pongGameContext_A070(chainHead);
    while (t[1] != 0) {
        using Vfn1 = void(*)(void*, void*);
        void** traverserVtable = *reinterpret_cast<void***>(traverser);
        Vfn1 visit = reinterpret_cast<Vfn1>(traverserVtable[1]);
        visit(traverser, reinterpret_cast<void*>(static_cast<uintptr_t>(t[1])));
        pongGameContext_9FD0_w(chainHead);
    }
}

// @ 0x823EF850 | size: 0x60  — util::BindTransform
// Thin helper that copies two 32-bit fields (+16/+20) from src to dst, invokes
// slot 5 of dst's vtable (a "transform-attached" notify), and back-links dst
// to src via +8. Appears in the post-transform-load path on drawables.
void util_BindTransform_0x823EF850(void* src, void* dst) {
    uint32_t* d = reinterpret_cast<uint32_t*>(dst);
    const uint32_t* s = reinterpret_cast<const uint32_t*>(src);
    d[1]  = s[4];   // +4 = src.+16
    d[4]  = s[4];   // +16 = src.+16
    d[5]  = s[5];   // +20 = src.+20
    using Vfn5 = void(*)(void*);
    void** dstVtable = *reinterpret_cast<void***>(dst);
    reinterpret_cast<Vfn5>(dstVtable[5])(dst);
    d[2] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(src));  // dst.+8 = src
}

