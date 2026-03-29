#include "rage_dat.hpp"

#include <cmath>
#include <cstdint>

extern "C" void sysMemAllocator_InitMainThread(void);
extern "C" void rage_free(void* ptr);
extern "C" std::uint32_t* g_sda_base;

// ── Debug text rendering ────────────────────────────────────────────────────

// Debug font global — pointer to font descriptor struct
// Descriptor fields: +8 = line count (int32_t), +36 = line height (float)
extern void* g_debugFont;                 // @ 0x825EBC3C (.data)

// Renders a text string glyph-by-glyph through the debug font system
extern void util_84E8(void* font,         // @ 0x821584E8
    float x, float y, float scale,
    float spacingX, float spacingY,
    std::uint32_t color, const char* text);

// Float constants from .rdata @ 0x8202D108
extern const float kDebugShadowOffsetX;   // +0x00 — shadow pixel offset
extern const float kDebugFontScale;       // +0x08 — font scale / alpha threshold
extern const float kDebugShadowOffsetY;   // +0x7C — shadow Y offset

// ── Polar camera input ──────────────────────────────────────────────────────

// Updates input state for singles network client
extern void SinglesNetworkClient_F090(void* inputState);  // @ 0x8221F090

// Speed multiplier constants
extern const float kDefaultPanY;          // rdata @ 0x8202D110 + 0
extern const float kSlowOrbitMul;         // rdata @ 0x8202D110 + 8
extern const float kFastTurnMul;          // rdata @ 0x8202D110 + 116
extern float g_fastOrbitMul;              // @ 0x82028A50 (.data)
extern float g_slowTurnMul;              // @ 0x825C90BC (.data)

// Semantic aliases for recovered engine entry points.
extern "C" void DestroyDatParserImpl(rage::datParser* self, int deleteThis)
    __asm__("datParser_rtti_74A0_0");
extern "C" void DestroyDatParserRecordImpl(rage::datParserRecord* self, int deleteThis)
    __asm__("datParserRecord_vfn_0");
extern "C" void DestroyDatSerializeImpl(rage::datSerialize* self, int deleteThis)
    __asm__("datSerialize_vfn_0");

extern "C" void DestroyPolarCameraImpl(rage::dcamPolarCam* self, int deleteThis)
    __asm__("dcamPolarCam_vfn_0");
extern "C" void ConfigurePolarFrameImpl(
    rage::dcamPolarCam* self,
    const float* eyePosition,
    const float* lookAtPosition
) __asm__("dcamPolarCam_vfn_1");
extern "C" int UpdatePolarStateImpl(rage::dcamPolarCam* self, float deltaTime)
    __asm__("dcamPolarCam_vfn_3");
extern "C" void ApplyPolarStateImpl(rage::dcamPolarCam* self)
    __asm__("dcamPolarCam_vfn_4");
extern "C" float* GetPolarStateBufferImpl(rage::dcamPolarCam* self)
    __asm__("dcamPolarCam_vfn_5");
extern "C" void SetPolarStateBufferImpl(rage::dcamPolarCam* self, const float* stateBuffer)
    __asm__("dcamPolarCam_vfn_6");
extern "C" void SetFocusDirectionImpl(rage::dcamPolarCam* self, const float* direction)
    __asm__("dcamPolarCam_vfn_7");
extern "C" void DrawPolarDebugOverlayImpl(
    rage::dcamPolarCam* self,
    void* renderContext,
    const char* label,
    float blend
) __asm__("dcamPolarCam_vfn_8");
extern "C" void DrawPolarDebugValueImpl(
    rage::dcamPolarCam* self,
    void* renderContext,
    const char* label,
    float value
) __asm__("dcamPolarCam_vfn_9");
extern "C" const char* GetPolarCameraDebugNameImpl(rage::dcamPolarCam* self)
    __asm__("dcamPolarCam_vfn_10");

namespace {

class MainThreadHeapAllocator {
public:
    virtual ~MainThreadHeapAllocator() = default;
    virtual void* Allocate(std::uint32_t sizeBytes, std::uint32_t alignment) = 0;
};

MainThreadHeapAllocator* GetMainThreadHeapAllocator() {
    if (g_sda_base == nullptr || g_sda_base[0] == 0u) {
        return nullptr;
    }

    auto* allocatorState = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(g_sda_base[0])
    );
    if (allocatorState[1] == 0u) {
        return nullptr;
    }

    return reinterpret_cast<MainThreadHeapAllocator*>(
        static_cast<std::uintptr_t>(allocatorState[1])
    );
}

constexpr std::uint32_t kBitBufferAlignment = 16u;

} // namespace

namespace rage {

bool datBitBufferDefaultAllocator::CanAllocate(std::uint32_t sizeBytes) const {
    return sizeBytes > 0u;
}

void* datBitBufferDefaultAllocator::Allocate(std::uint32_t sizeBytes) {
    sysMemAllocator_InitMainThread();

    MainThreadHeapAllocator* allocator = GetMainThreadHeapAllocator();
    if (allocator == nullptr) {
        return nullptr;
    }

    return allocator->Allocate(sizeBytes, kBitBufferAlignment);
}

void datBitBufferDefaultAllocator::Free(void* memory) {
    if (memory != nullptr) {
        rage_free(memory);
    }
}

datParser::~datParser() {
    DestroyDatParserImpl(this, 0);
}

datParserRecord::~datParserRecord() {
    DestroyDatParserRecordImpl(this, 0);
}

datSerialize::~datSerialize() {
    DestroyDatSerializeImpl(this, 0);
}

dcamPolarCam::~dcamPolarCam() {
    DestroyPolarCameraImpl(this, 0);
}

void dcamPolarCam::ConfigurePolarFrame(const float* eyePosition, const float* lookAtPosition) {
    ConfigurePolarFrameImpl(this, eyePosition, lookAtPosition);
}

bool dcamPolarCam::UpdatePolarState(float deltaTime) {
    return UpdatePolarStateImpl(this, deltaTime) != 0;
}

void dcamPolarCam::ApplyPolarState() {
    ApplyPolarStateImpl(this);
}

float* dcamPolarCam::GetPolarStateBuffer() {
    return GetPolarStateBufferImpl(this);
}

void dcamPolarCam::SetPolarStateBuffer(const float* stateBuffer) {
    SetPolarStateBufferImpl(this, stateBuffer);
}

void dcamPolarCam::SetFocusDirection(const float* direction) {
    SetFocusDirectionImpl(this, direction);
}

void dcamPolarCam::DrawDebugOverlay(void* renderContext, const char* label, float blend) {
    DrawPolarDebugOverlayImpl(this, renderContext, label, blend);
}

void dcamPolarCam::DrawDebugValue(void* renderContext, const char* label, float value) {
    DrawPolarDebugValueImpl(this, renderContext, label, value);
}

const char* dcamPolarCam::GetDebugName() const {
    return GetPolarCameraDebugNameImpl(const_cast<dcamPolarCam*>(this));
}

class netBBAllocator::Backend {
public:
    virtual ~Backend() = default;

    virtual void* Allocate(std::uint32_t sizeBytes, std::uint32_t allocationFlags) = 0;
    virtual void* GetBufferBase() = 0;
    virtual std::uint32_t GetAvailableBytes() const = 0;
};

netBBAllocator::~netBBAllocator() {
    delete m_pBackend;
    m_pBackend = nullptr;
}

bool netBBAllocator::CanAllocate(std::uint32_t sizeBytes) const {
    if (m_pBackend == nullptr) {
        return false;
    }

    return m_pBackend->GetAvailableBytes() >= sizeBytes;
}

void* netBBAllocator::Allocate(std::uint32_t sizeBytes) {
    if (m_pBackend == nullptr) {
        return nullptr;
    }

    return m_pBackend->Allocate(sizeBytes, 0u);
}

void* netBBAllocator::GetBufferBase() const {
    if (m_pBackend == nullptr) {
        return nullptr;
    }

    return const_cast<Backend*>(m_pBackend)->GetBufferBase();
}

// ─────────────────────────────────────────────────────────────────────────────
// datParserRecord::Cleanup @ 0x82177A68 | size: 0x68
// Destructor body helper — resets vtable and deletes owned sub-object
// if the record type is 10 (pointer-owning record). Called by
// ~datParserRecord during teardown.
//
// PPC scaffold:
//   cmpwi  r10, 10          ; m_type == 10?
//   lwz    r3, 76(r31)      ; m_pOwnedObject
//   lwz    r9, 0(r3)        ; vtable
//   li     r4, 1            ; delete-self flag
//   lwz    r8, 0(r9)        ; slot 0 = scalar dtor
//   bctrl                   ; call dtor(obj, 1)
//   stw    r7, 76(r31)      ; m_pOwnedObject = nullptr
// ─────────────────────────────────────────────────────────────────────────────
void datParserRecord::Cleanup() {
    if (m_type == 10) {
        if (m_pOwnedObject != nullptr) {
            // Call scalar deleting destructor through vtable slot 0
            auto vtable = reinterpret_cast<void**>(m_pOwnedObject->m_pVtable);
            auto scalarDtor = reinterpret_cast<void(*)(datBase*, int)>(vtable[0]);
            scalarDtor(m_pOwnedObject, 1);  // 1 = delete self
        }
        m_pOwnedObject = nullptr;
    }
}

} // namespace rage

// ─────────────────────────────────────────────────────────────────────────────
// DrawDebugText @ 0x8227DEC0 | size: 0x114
// Draws debug text with a drop shadow at screen position (x, y).
// Returns the next Y position (y + lineAdvance + 2) for stacking text.
//
// The function loads the global debug font descriptor, extracts the alpha
// channel from the color, and if alpha exceeds a threshold, draws:
//   1. Shadow pass — black text offset by (shadowOffsetX, shadowOffsetY)
//   2. Foreground pass — text at (x, y) with the original color
//
// NOT a dcamPolarCam member — free function in the same translation unit.
// ─────────────────────────────────────────────────────────────────────────────
int DrawDebugText(int x, int y, const char* text, std::uint32_t color) {
    auto* font = static_cast<std::uint8_t*>(g_debugFont);

    // Extract alpha channel
    std::uint8_t alpha = (color >> 24) & 0xFF;
    float alphaF = static_cast<float>(alpha);

    if (alphaF > kDebugFontScale) {
        float fx = static_cast<float>(x);
        float fy = static_cast<float>(y);

        // Shadow pass: black text with same alpha, offset by shadow amounts
        std::uint32_t shadowColor = static_cast<std::uint32_t>(alpha) << 24;
        util_84E8(font,
            fx + kDebugShadowOffsetX, fy + kDebugShadowOffsetY,
            kDebugFontScale, kDebugShadowOffsetX, kDebugShadowOffsetX,
            shadowColor, text);

        // Foreground pass: original color at exact position
        util_84E8(font,
            fx, fy,
            kDebugFontScale, kDebugShadowOffsetX, kDebugShadowOffsetX,
            color, text);
    }

    // Calculate line advance from font metrics
    std::int32_t lineCount = *reinterpret_cast<std::int32_t*>(font + 8);
    float lineHeight = *reinterpret_cast<float*>(font + 36);
    int lineAdvance = static_cast<int>(static_cast<float>(lineCount) * lineHeight);
    return y + lineAdvance + 2;
}

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// dcamPolarCam::ProcessPolarInput @ 0x823F7128 | size: 0x5D8
// Processes polar camera input in two modes:
//
// Orbit mode (default):
//   Buttons 0/1 — zoom in/out (distance ±= orbitSpeed)
//   Buttons 2/3 — rotate left/right (azimuth ±= turnSpeed)
//   Buttons 4/5 — tilt up/down (elevation ±= turnSpeed)
//   Buttons 9/10 — raise/lower lookAt Y (or Z in alt mode)
//
// Look mode (button 6 held):
//   Buttons 2/3 — pan left/right along panDirection vector
//   Buttons 4/5 — raise/lower lookAt Y (or Z in alt mode)
//   Buttons 0/1 — move forward/backward along normalized panOffset
//
// Speed modifiers:
//   Button 7 — fast mode (turnSpeed *= kFastTurnMul, etc.)
//   Button 8 — slow mode (turnSpeed *= kSlowTurnMul, etc.)
//
// Object layout (PPC32 offsets from this):
//   +0x10  m_altMode       (uint8_t)   alt-axis flag
//   +0x34  m_distance      (float)     camera distance
//   +0x38  m_azimuth       (float)     horizontal angle
//   +0x40  m_elevation     (float)     vertical angle
//   +0x44  m_orbitSpeed    (float)     base orbit speed
//   +0x48  m_turnSpeed     (float)     base turn speed
//   +0x50  m_lookAt[4]     (float[4])  lookAt target position
//   +0xD0  m_panDirection[4] (float[4]) pan direction vector
//   +0xF0  m_panOffset[4]  (float[4])  pan offset vector
//   +0x110 m_inputState    (opaque)    input state block
//   +0x418 m_buttons[11]   (4 bytes ea) button state array
//
// Each button is 4 bytes. Active when (byte[3] ^ byte[0]) > 127
// (MSB differs between current and previous frame = state changed).
// ─────────────────────────────────────────────────────────────────────────────
bool dcamPolarCam::ProcessPolarInput(float dt) {
    auto* self = reinterpret_cast<std::uint8_t*>(this);
    bool changed = false;

    // Scale base speeds by delta time
    float turnSpeed = *reinterpret_cast<float*>(self + 72) * dt;   // +0x48
    float orbitSpeed = *reinterpret_cast<float*>(self + 68) * dt;  // +0x44

    // Update input state
    SinglesNetworkClient_F090(self + 272);

    // Button state helper: each button is 4 bytes at self + 1048 + i*4
    // Active when MSB of (byte[3] ^ byte[0]) is set
    auto isActive = [self](int idx) -> bool {
        int off = 1048 + idx * 4;
        return static_cast<std::uint8_t>(self[off + 3] ^ self[off]) > 127;
    };

    // Apply speed modifiers
    if (isActive(7)) {
        // Fast mode
        turnSpeed *= kFastTurnMul;
        orbitSpeed *= g_fastOrbitMul;
    } else if (isActive(8)) {
        // Slow mode
        turnSpeed *= g_slowTurnMul;
        orbitSpeed *= kSlowOrbitMul;
    }

    bool altMode = self[16] != 0;                                  // +0x10
    float* lookAt = reinterpret_cast<float*>(self + 80);           // +0x50
    float* panDir = reinterpret_cast<float*>(self + 208);          // +0xD0
    float* panOff = reinterpret_cast<float*>(self + 240);          // +0xF0
    float& distance = *reinterpret_cast<float*>(self + 52);        // +0x34
    float& azimuth = *reinterpret_cast<float*>(self + 56);         // +0x38
    float& elevation = *reinterpret_cast<float*>(self + 64);       // +0x40

    if (isActive(6)) {
        // ═══ Look mode ═══

        if (isActive(2)) {
            // Pan left: lookAt += panDir * (-orbitSpeed)
            for (int i = 0; i < 4; i++)
                lookAt[i] += panDir[i] * (-orbitSpeed);
            changed = true;
        }
        if (isActive(3)) {
            // Pan right: lookAt += panDir * orbitSpeed
            for (int i = 0; i < 4; i++)
                lookAt[i] += panDir[i] * orbitSpeed;
            changed = true;
        }
        if (isActive(4)) {
            // Look up
            if (altMode) lookAt[2] += orbitSpeed;
            else         lookAt[1] += orbitSpeed;
            changed = true;
        }
        if (isActive(5)) {
            // Look down
            if (altMode) lookAt[2] -= orbitSpeed;
            else         lookAt[1] -= orbitSpeed;
            changed = true;
        }
        if (isActive(1) || isActive(0)) {
            // Forward / backward — normalize panOffset, then translate lookAt
            float dir[4];
            for (int i = 0; i < 4; i++) dir[i] = panOff[i];
            dir[1] = kDefaultPanY;

            // Normalize direction vector
            float dot = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
            float invLen = 1.0f / std::sqrtf(dot);
            for (int i = 0; i < 4; i++) dir[i] *= invLen;

            float sign = isActive(1) ? -orbitSpeed : orbitSpeed;
            for (int i = 0; i < 4; i++) lookAt[i] += dir[i] * sign;
            return true;
        }
        return changed;

    } else {
        // ═══ Orbit mode ═══

        if (isActive(9)) {
            if (altMode) lookAt[2] += orbitSpeed;
            else         lookAt[1] += orbitSpeed;
            changed = true;
        }
        if (isActive(10)) {
            if (altMode) lookAt[2] -= orbitSpeed;
            else         lookAt[1] -= orbitSpeed;
            changed = true;
        }
        if (isActive(0)) {
            distance += orbitSpeed;
            changed = true;
        }
        if (isActive(1)) {
            distance -= orbitSpeed;
            changed = true;
        }
        if (isActive(2)) {
            azimuth -= turnSpeed;
            changed = true;
        }
        if (isActive(3)) {
            azimuth += turnSpeed;
            changed = true;
        }
        if (isActive(4)) {
            elevation += turnSpeed;
            changed = true;
        }
        if (isActive(5)) {
            elevation -= turnSpeed;
            return true;
        }
        return changed;
    }
}

} // namespace rage
