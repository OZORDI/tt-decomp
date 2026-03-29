/**
 * rage_anim.cpp — Animation system implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * RAGE animation DOF (Degree of Freedom) classes for blending and interpolation.
 */

#include "rage_anim.hpp"
#include "rage/rage.h"
#include <algorithm>
#include <cmath>
#include <cmath>

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::Blend()  [vtable slot 7 @ 0x8216EC30]
// 
// Blends the current float value towards a target value using linear interpolation.
// The blend factor is clamped to [0.0, 1.0] range.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::Blend(void* targetObj, float blendFactor) {
    // Clamp blend factor to [0.0, 1.0]
    float clampedBlend = std::clamp(blendFactor, 0.0f, 1.0f);
    
    // Get target value by calling virtual method slot 16 on the target object
    // This is a virtual dispatch to get the target float value
    float targetValue = 0.0f;
    if (targetObj) {
        void** vtable = *reinterpret_cast<void***>(targetObj);
        typedef float (*GetValueFunc)(void*);
        GetValueFunc getValueFn = reinterpret_cast<GetValueFunc>(vtable[16]);
        targetValue = getValueFn(targetObj);
    }
    
    // Linear interpolation: current + (target - current) * blend
    float currentValue = m_value;
    float delta = targetValue - currentValue;
    m_value = currentValue + delta * clampedBlend;
}

} // namespace rage

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::GetValue()  [vtable slot 2 @ 0x820C46F0]
// 
// Allocates and initializes a new crAnimDofVector3 instance by cloning this object.
// Returns nullptr if allocation fails.
// ─────────────────────────────────────────────────────────────────────────────
void* crAnimDofVector3::GetValue() {
    // Allocate memory for new instance (32 bytes with 16-byte alignment)
    void* newInstance = rage::Allocate(32, 16);
    
    if (newInstance) {
        // Set vtable pointer
        *reinterpret_cast<void**>(newInstance) = reinterpret_cast<void*>(0x82027B9C);
        
        // Copy flags and metadata from offset +4 to +7
        uint8_t* src = reinterpret_cast<uint8_t*>(this);
        uint8_t* dst = reinterpret_cast<uint8_t*>(newInstance);
        dst[4] = src[4];
        dst[5] = src[5];
        *reinterpret_cast<uint16_t*>(dst + 6) = *reinterpret_cast<uint16_t*>(src + 6);
        
        // Copy the 3D vector data at offset +16 (12 bytes: x, y, z floats)
        float* srcVec = reinterpret_cast<float*>(src + 16);
        float* dstVec = reinterpret_cast<float*>(dst + 16);
        dstVec[0] = srcVec[0];
        dstVec[1] = srcVec[1];
        dstVec[2] = srcVec[2];
    }
    
    return newInstance;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::GetChannelCount()  [vtable slot 3 @ 0x820C4BE8]
// 
// Returns the size of the crAnimDofVector3 structure (32 bytes).
// ─────────────────────────────────────────────────────────────────────────────
int crAnimDofVector3::GetChannelCount() {
    return 32;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::Evaluate()  [vtable slot 4 @ 0x820C47A0]
// 
// Evaluates animation channels and stores the result in the output vector.
// Delegates to helper function for channel evaluation.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::Evaluate(void* animContext, float time, float* outVector) {
    EvaluateChannels(animContext, time, outVector);
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::Blend()  [vtable slot 5 @ 0x820C47B0]
// 
// Blends the current vector towards a target vector using linear interpolation.
// The blend factor is clamped to [0.0, 1.0] range.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::Blend(void* animContext, float blendFactor, float* outVector) {
    // Early exit if blend factor is too small
    if (blendFactor <= 0.0f) {
        return;
    }
    
    // Evaluate target vector
    float targetVec[4] __attribute__((aligned(16)));
    EvaluateChannels(animContext, 0.0f, targetVec);
    
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Clamp blend factor and perform linear interpolation
    if (blendFactor < 1.0f) {
        // Lerp: current + (target - current) * blend
        currentVec[0] += (targetVec[0] - currentVec[0]) * blendFactor;
        currentVec[1] += (targetVec[1] - currentVec[1]) * blendFactor;
        currentVec[2] += (targetVec[2] - currentVec[2]) * blendFactor;
    } else {
        // Full blend - just copy target
        currentVec[0] = targetVec[0];
        currentVec[1] = targetVec[1];
        currentVec[2] = targetVec[2];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::CopyFrom()  [vtable slot 6 @ 0x820C4850]
// 
// Copies the vector value from another animation DOF object.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::CopyFrom(void* sourceAnimDof) {
    // Get the vector from source via virtual call to slot 14
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float* (*GetVectorFunc)(void*);
    GetVectorFunc getVectorFn = reinterpret_cast<GetVectorFunc>(vtable[14]);
    float* sourceVec = getVectorFn(sourceAnimDof);
    
    // Copy to our vector at offset +16
    float* destVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    destVec[0] = sourceVec[0];
    destVec[1] = sourceVec[1];
    destVec[2] = sourceVec[2];
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::BlendFrom()  [vtable slot 7 @ 0x820C4898]
// 
// Blends towards another animation DOF's vector value with interpolation.
// Similar to Blend but takes another DOF object as source instead of evaluating channels.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::BlendFrom(void* sourceAnimDof, float blendFactor) {
    // Early exit if blend factor is too small
    if (blendFactor <= 0.0f) {
        return;
    }
    
    // Get source vector via virtual call to slot 14
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float* (*GetVectorFunc)(void*);
    GetVectorFunc getVectorFn = reinterpret_cast<GetVectorFunc>(vtable[14]);
    float* sourceVec = getVectorFn(sourceAnimDof);
    
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Perform blending
    if (blendFactor < 1.0f) {
        // Lerp: current + (source - current) * blend
        currentVec[0] += (sourceVec[0] - currentVec[0]) * blendFactor;
        currentVec[1] += (sourceVec[1] - currentVec[1]) * blendFactor;
        currentVec[2] += (sourceVec[2] - currentVec[2]) * blendFactor;
    } else {
        // Full blend - just copy source
        currentVec[0] = sourceVec[0];
        currentVec[1] = sourceVec[1];
        currentVec[2] = sourceVec[2];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::ScaleFrom()  [vtable slot 8 @ 0x820C4948]
// 
// Adds a scaled vector from another animation DOF to the current vector.
// Only performs the operation if the scale factor squared exceeds a threshold.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::ScaleFrom(void* sourceAnimDof, float scaleFactor) {
    // Only proceed if scale factor is significant (squared > epsilon)
    float scaleSquared = scaleFactor * scaleFactor;
    const float epsilon = 1e-6f; // Small threshold to avoid unnecessary operations
    
    if (scaleSquared > epsilon) {
        // Get source vector via virtual call to slot 14
        void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
        typedef float* (*GetVectorFunc)(void*);
        GetVectorFunc getVectorFn = reinterpret_cast<GetVectorFunc>(vtable[14]);
        float* sourceVec = getVectorFn(sourceAnimDof);
        
        // Get current vector
        float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
        
        // Add scaled source vector: current += source * scale
        currentVec[0] += sourceVec[0] * scaleFactor;
        currentVec[1] += sourceVec[1] * scaleFactor;
        currentVec[2] += sourceVec[2] * scaleFactor;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::ThresholdFrom()  [vtable slot 9 @ 0x820C49D0]
// 
// Normalizes the vector if its magnitude exceeds a threshold.
// Replaces the vector with a normalized version from another DOF if needed.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::ThresholdFrom(void* sourceAnimDof, float threshold) {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Calculate magnitude squared
    float magSquared = currentVec[0] * currentVec[0] + 
                       currentVec[1] * currentVec[1] + 
                       currentVec[2] * currentVec[2];
    
    // Compare with threshold squared
    float thresholdSquared = threshold * threshold;
    
    if (magSquared > thresholdSquared) {
        // Get normalized vector from source via virtual call to slot 14
        void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
        typedef float* (*GetVectorFunc)(void*);
        GetVectorFunc getVectorFn = reinterpret_cast<GetVectorFunc>(vtable[14]);
        float* sourceVec = getVectorFn(sourceAnimDof);
        
        // Replace with normalized vector
        currentVec[0] = sourceVec[0];
        currentVec[1] = sourceVec[1];
        currentVec[2] = sourceVec[2];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::ScaleInPlace()  [vtable slot 10 @ 0x820C4A38]
// 
// Scales the vector by a uniform scalar value.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::ScaleInPlace(float scaleFactor) {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Scale all components uniformly
    currentVec[0] *= scaleFactor;
    currentVec[1] *= scaleFactor;
    currentVec[2] *= scaleFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::Reset()  [vtable slot 11 @ 0x820C4A60]
// 
// Scales only the X component of the vector by a constant factor.
// The constant appears to be a conversion or normalization factor.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::Reset() {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Scale X component by constant (appears to be degrees-to-radians or similar)
    // The constant is loaded from a global at a specific address
    const float scaleFactor = 0.017453292f; // Approximately PI/180 (degrees to radians)
    currentVec[0] *= scaleFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::TransformBy()  [vtable slot 12 @ 0x820C4A78]
// 
// Transforms the vector through another animation DOF's coordinate space.
// Gets the transform vector and applies it via virtual call to slot 17.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::TransformBy(void* transformAnimDof) {
    // Get transform vector via virtual call to slot 14
    void** vtable = *reinterpret_cast<void***>(transformAnimDof);
    typedef float* (*GetVectorFunc)(void*);
    GetVectorFunc getVectorFn = reinterpret_cast<GetVectorFunc>(vtable[14]);
    float* transformVec = getVectorFn(transformAnimDof);
    
    // Get pointer to our vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Apply transform via virtual call to slot 17
    typedef void (*TransformFunc)(void*, float*);
    TransformFunc transformFn = reinterpret_cast<TransformFunc>(vtable[17]);
    transformFn(transformAnimDof, currentVec);
    
    // Store transformed result back
    currentVec[0] = transformVec[0];
    currentVec[1] = transformVec[1];
    currentVec[2] = transformVec[2];
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::Normalize()  [vtable slot 13 @ 0x820C4AF8]
// 
// Resets the vector to zero (0, 0, 0).
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::Normalize() {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Reset to zero
    currentVec[0] = 0.0f;
    currentVec[1] = 0.0f;
    currentVec[2] = 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::SetValue()  [vtable slot 17 @ 0x820C4B10]
// 
// Copies a vector from an external source into this DOF's vector storage.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::SetValue(const float* sourceVec) {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Copy source vector
    currentVec[0] = sourceVec[0];
    currentVec[1] = sourceVec[1];
    currentVec[2] = sourceVec[2];
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::EvaluateChannels() - Helper function
// 
// Evaluates animation channels to compute the vector value at a given time.
// This is a complex function that handles different channel types.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::EvaluateChannels(void* animContext, float time, float* outVector) {
    // Get channel configuration from offset +1
    uint8_t* thisPtr = reinterpret_cast<uint8_t*>(this);
    uint8_t channelConfig = thisPtr[1] & 0xF0;
    
    if (channelConfig == 0) {
        // Single channel mode - evaluate one channel for all components
        void** channels = reinterpret_cast<void**>(thisPtr + 4);
        void* channel = channels[0];
        
        if (channel) {
            void** vtable = *reinterpret_cast<void***>(channel);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[3]);
            evalFn(channel, time, outVector);
        }
    } else if (channelConfig == 0x10) {
        // Three channel mode - separate channels for X, Y, Z
        void** channels = reinterpret_cast<void**>(thisPtr + 4);
        
        // Evaluate X channel
        if (channels[0]) {
            void** vtable = *reinterpret_cast<void***>(channels[0]);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[5]);
            evalFn(channels[0], time, outVector);
        }
        
        // Evaluate Y channel
        if (channels[1]) {
            void** vtable = *reinterpret_cast<void***>(channels[1]);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[5]);
            evalFn(channels[1], time, outVector + 1);
        }
        
        // Evaluate Z channel
        if (channels[2]) {
            void** vtable = *reinterpret_cast<void***>(channels[2]);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[5]);
            evalFn(channels[2], time, outVector + 2);
        }
    }
}

} // namespace rage

namespace rage {

// ═════════════════════════════════════════════════════════════════════════════
// crAnimDofQuaternion vtable implementations
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::Blend()  [vtable slot 5 @ 0x820C4C00]
// 
// Blends the current quaternion towards a target quaternion using lerp+normalize.
// Uses dot product sign check to ensure shortest-path interpolation.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::Blend(void* animContext, float blendFactor, float* outQuat) {
    // Early exit if blend factor is too small
    if (blendFactor <= 0.0f) {
        return;
    }

    // Evaluate target quaternion from animation channels
    float targetQuat[4] __attribute__((aligned(16)));
    EvaluateChannels(animContext, 0.0f, targetQuat);

    // Get current quaternion at this+16
    float* currentQuat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);

    // Compute dot product to check hemisphere
    float dot = targetQuat[0] * currentQuat[0] +
                targetQuat[1] * currentQuat[1] +
                targetQuat[2] * currentQuat[2] +
                targetQuat[3] * currentQuat[3];

    // If dot product is negative, negate current quat for shortest path
    if (dot < 0.0f) {
        currentQuat[0] = -currentQuat[0];
        currentQuat[1] = -currentQuat[1];
        currentQuat[2] = -currentQuat[2];
        currentQuat[3] = -currentQuat[3];
    }

    if (blendFactor < 1.0f) {
        // Lerp: current += (target - current) * blendFactor
        currentQuat[0] += (targetQuat[0] - currentQuat[0]) * blendFactor;
        currentQuat[1] += (targetQuat[1] - currentQuat[1]) * blendFactor;
        currentQuat[2] += (targetQuat[2] - currentQuat[2]) * blendFactor;
        currentQuat[3] += (targetQuat[3] - currentQuat[3]) * blendFactor;

        // Normalize the resulting quaternion
        float mag = sqrtf(currentQuat[0] * currentQuat[0] +
                          currentQuat[1] * currentQuat[1] +
                          currentQuat[2] * currentQuat[2] +
                          currentQuat[3] * currentQuat[3]);
        if (mag != 0.0f) {
            float inv = 1.0f / mag;
            currentQuat[0] *= inv;
            currentQuat[1] *= inv;
            currentQuat[2] *= inv;
            currentQuat[3] *= inv;
        }
    } else {
        // Full blend - direct copy via 64-bit stores
        uint64_t* src = reinterpret_cast<uint64_t*>(targetQuat);
        uint64_t* dst = reinterpret_cast<uint64_t*>(currentQuat);
        dst[0] = src[0];
        dst[1] = src[1];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::CopyFrom()  [vtable slot 6 @ 0x820C4D30]
// 
// Copies the quaternion value from another animation DOF object.
// Uses vtable slot 15 (GetQuaternionPtr) to obtain the source quaternion.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::CopyFrom(void* sourceAnimDof) {
    // Get the quaternion from source via virtual call to slot 15
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float* (*GetQuatFunc)(void*);
    GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
    float* sourceQuat = getQuatFn(sourceAnimDof);

    // Copy 16 bytes (4 floats) via 64-bit stores to this+16
    uint64_t* src = reinterpret_cast<uint64_t*>(sourceQuat);
    uint64_t* dst = reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(this) + 16);
    dst[0] = src[0];
    dst[1] = src[1];
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::ScaleFrom()  [vtable slot 8 @ 0x820C4EC8]
// 
// Applies a scaled rotation from another quaternion DOF to the current quaternion.
// Three paths based on scaleFactor:
//   Path A (≈ 1.0): Quaternion multiply  result = current * source
//   Path B (≈ -1.0): Quaternion multiply with conjugate  result = current * conj(source)
//   Path C (general): Scale the rotation angle by scaleFactor, then multiply
// Uses util_4248 (quaternion power / angle scaling) for the general case.
// ─────────────────────────────────────────────────────────────────────────────

// Forward declaration: quaternion rotation scaling (scales angle by factor)
// @ 0x820C4248 — converts quat to axis-angle, scales angle, converts back
extern void QuaternionScaleRotation(float* quat, float scaleFactor);

void crAnimDofQuaternion::ScaleFrom(void* sourceAnimDof, float scaleFactor) {
    const float epsilon = 1e-6f;

    // Get current quaternion at this+16
    float* curQuat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);

    // Path A: scaleFactor ≈ 1.0 — direct quaternion multiply
    float delta = scaleFactor - 1.0f;
    float deltaSquared = delta * delta;

    if (deltaSquared < epsilon) {
        // Get source quaternion via vtable slot 15
        void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
        typedef float* (*GetQuatFunc)(void*);
        GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
        float* src = getQuatFn(sourceAnimDof);

        float s0 = src[0], s1 = src[1], s2 = src[2], s3 = src[3];
        float c0 = curQuat[0], c1 = curQuat[1], c2 = curQuat[2], c3 = curQuat[3];

        // Hamilton product: result = current * source
        curQuat[0] = c3*s0 + c0*s3 + c1*s2 - c2*s1;
        curQuat[1] = c3*s1 - c0*s2 + c1*s3 + c2*s0;
        curQuat[2] = c3*s2 + c0*s1 - c1*s0 + c2*s3;
        curQuat[3] = c3*s3 - c0*s0 - c1*s1 - c2*s2;
        return;
    }

    // Path B: scaleFactor ≈ -1.0 — multiply with conjugate
    float sum = scaleFactor + 1.0f;
    float sumSquared = sum * sum;

    if (sumSquared < epsilon) {
        // Get source quaternion via vtable slot 15
        void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
        typedef float* (*GetQuatFunc)(void*);
        GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
        float* src = getQuatFn(sourceAnimDof);

        // Conjugate: negate x,y,z, keep w
        float s0 = -src[0], s1 = -src[1], s2 = -src[2], s3 = src[3];
        float c0 = curQuat[0], c1 = curQuat[1], c2 = curQuat[2], c3 = curQuat[3];

        // Hamilton product: result = current * conjugate(source)
        curQuat[0] = c3*s0 + c0*s3 + c1*s2 - c2*s1;
        curQuat[1] = c3*s1 - c0*s2 + c1*s3 + c2*s0;
        curQuat[2] = c3*s2 + c0*s1 - c1*s0 + c2*s3;
        curQuat[3] = c3*s3 - c0*s0 - c1*s1 - c2*s2;
        return;
    }

    // Path C: general case — scale the rotation, then multiply
    float scaleSquared = scaleFactor * scaleFactor;
    if (scaleSquared <= epsilon) {
        // Scale factor is near zero — no operation
        return;
    }

    // Get source quaternion via vtable slot 15
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float* (*GetQuatFunc)(void*);
    GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
    float* src = getQuatFn(sourceAnimDof);

    // Copy source quaternion to local storage
    float scaledQuat[4];
    scaledQuat[0] = src[0];
    scaledQuat[1] = src[1];
    scaledQuat[2] = src[2];
    scaledQuat[3] = src[3];

    // If scaleFactor < 0, negate quaternion xyz and use absolute scale
    if (scaleFactor < 0.0f) {
        scaledQuat[0] = -scaledQuat[0];
        scaledQuat[1] = -scaledQuat[1];
        scaledQuat[2] = -scaledQuat[2];
        scaleFactor = -scaleFactor;
    }

    // Scale the rotation angle by the factor (quaternion power)
    QuaternionScaleRotation(scaledQuat, scaleFactor);

    // Quaternion multiply: result = current * scaledSource
    float s0 = scaledQuat[0], s1 = scaledQuat[1], s2 = scaledQuat[2], s3 = scaledQuat[3];
    float c0 = curQuat[0], c1 = curQuat[1], c2 = curQuat[2], c3 = curQuat[3];

    curQuat[0] = c3*s0 + c0*s3 + c1*s2 - c2*s1;
    curQuat[1] = c3*s1 - c0*s2 + c1*s3 + c2*s0;
    curQuat[2] = c3*s2 + c0*s1 - c1*s0 + c2*s3;
    curQuat[3] = c3*s3 - c0*s0 - c1*s1 - c2*s2;
}

} // namespace rage

namespace rage {

// ═════════════════════════════════════════════════════════════════════════════
// crAnimDofFloat vtable implementations
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::GetValue()  [vtable slot 2 @ 0x8216EA78]
// 
// Clones this animation DOF by allocating a new instance and copying its state.
// Returns nullptr if allocation fails.
// ─────────────────────────────────────────────────────────────────────────────
void* crAnimDofFloat::GetValue() {
    // Allocate memory for new instance (12 bytes with 16-byte alignment)
    void* newInstance = rage::Allocate(12, 16);
    
    if (newInstance) {
        // Set vtable pointer to crAnimDofFloat vtable
        *reinterpret_cast<void**>(newInstance) = reinterpret_cast<void*>(0x82036884);
        
        // Copy metadata flags from offset +4 to +7
        uint8_t* src = reinterpret_cast<uint8_t*>(this);
        uint8_t* dst = reinterpret_cast<uint8_t*>(newInstance);
        dst[4] = src[4];
        dst[5] = src[5];
        *reinterpret_cast<uint16_t*>(dst + 6) = *reinterpret_cast<uint16_t*>(src + 6);
        
        // Copy the float value at offset +8
        *reinterpret_cast<float*>(dst + 8) = m_value;
    }
    
    return newInstance;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::GetChannelCount()  [vtable slot 3 @ 0x8216EB20]
// 
// Returns the size of the crAnimDofFloat structure (12 bytes).
// ─────────────────────────────────────────────────────────────────────────────
int crAnimDofFloat::GetChannelCount() {
    return 12;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::Evaluate()  [vtable slot 4 @ 0x8216EB28]
// 
// Evaluates animation channel and stores the result in the output float.
// Only processes single-channel mode (when channel config is 0).
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::Evaluate(void* animContext, float time, float* outValue) {
    // Check channel configuration at offset +1
    uint8_t* contextPtr = reinterpret_cast<uint8_t*>(animContext);
    uint8_t channelConfig = contextPtr[1] & 0xF0;
    
    // Only process if single-channel mode (config == 0)
    if (channelConfig == 0) {
        // Get the channel pointer from offset +4
        void** channels = reinterpret_cast<void**>(contextPtr + 4);
        void* channel = channels[0];
        
        if (channel) {
            // Call virtual method slot 5 on the channel to evaluate
            void** vtable = *reinterpret_cast<void***>(channel);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[5]);
            evalFn(channel, time, outValue);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::Blend()  [vtable slot 5 @ 0x8216EB58]
// 
// Blends the current float value towards a target value from animation context.
// The blend factor is clamped to [0.0, 1.0] range.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::Blend(void* animContext, float blendFactor, float* outValue) {
    // Evaluate target value from animation context
    float targetValue = 0.0f;
    
    // Check channel configuration
    uint8_t* contextPtr = reinterpret_cast<uint8_t*>(animContext);
    uint8_t channelConfig = contextPtr[1] & 0xF0;
    
    if (channelConfig == 0) {
        // Get channel and evaluate
        void** channels = reinterpret_cast<void**>(contextPtr + 4);
        void* channel = channels[0];
        
        if (channel) {
            void** vtable = *reinterpret_cast<void***>(channel);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[5]);
            evalFn(channel, 0.0f, &targetValue);
        }
    }
    
    // Clamp blend factor to [0.0, 1.0]
    float clampedBlend = std::clamp(blendFactor, 0.0f, 1.0f);
    
    // Linear interpolation: current + (target - current) * blend
    float currentValue = m_value;
    float delta = targetValue - currentValue;
    m_value = currentValue + delta * clampedBlend;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::CopyFrom()  [vtable slot 6 @ 0x8216EBF0]
// 
// Copies the float value from another animation DOF object.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::CopyFrom(void* sourceAnimDof) {
    // Get the value from source via virtual call to slot 16
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float (*GetValueFunc)(void*);
    GetValueFunc getValueFn = reinterpret_cast<GetValueFunc>(vtable[16]);
    float sourceValue = getValueFn(sourceAnimDof);
    
    // Copy to our value
    m_value = sourceValue;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::ScaleFrom()  [vtable slot 8 @ 0x8216ECA8]
// 
// Adds a scaled value from another animation DOF to the current value.
// Performs: current += source * scaleFactor
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::ScaleFrom(void* sourceAnimDof, float scaleFactor) {
    // Get source value via virtual call to slot 16
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float (*GetValueFunc)(void*);
    GetValueFunc getValueFn = reinterpret_cast<GetValueFunc>(vtable[16]);
    float sourceValue = getValueFn(sourceAnimDof);
    
    // Add scaled source value: current += source * scale
    m_value += sourceValue * scaleFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::ThresholdFrom()  [vtable slot 9 @ 0x8216ED00]
// 
// Replaces the current value with source value if source magnitude exceeds threshold.
// Compares squared values to avoid expensive square root operation.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::ThresholdFrom(void* sourceAnimDof, float threshold) {
    // Get source value via virtual call to slot 16
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float (*GetValueFunc)(void*);
    GetValueFunc getValueFn = reinterpret_cast<GetValueFunc>(vtable[16]);
    float sourceValue = getValueFn(sourceAnimDof);
    
    // Compare squared values (avoids sqrt)
    float sourceSquared = sourceValue * sourceValue;
    float thresholdSquared = threshold * threshold;
    
    if (sourceSquared > thresholdSquared) {
        // Replace with source value
        m_value = sourceValue;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::ScaleInPlace()  [vtable slot 10 @ 0x8216ED80]
// 
// Scales the float value by a uniform scalar.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::ScaleInPlace(float scaleFactor) {
    m_value *= scaleFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::TransformBy()  [vtable slot 12 @ 0x8216ED90]
// 
// Transforms the value through another animation DOF's coordinate space.
// Gets the transform value and applies it via virtual call to slot 19.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::TransformBy(void* transformAnimDof) {
    // Get transform value via virtual call to slot 16
    void** vtable = *reinterpret_cast<void***>(transformAnimDof);
    typedef float (*GetValueFunc)(void*);
    GetValueFunc getValueFn = reinterpret_cast<GetValueFunc>(vtable[16]);
    float transformValue = getValueFn(transformAnimDof);
    
    // Apply transform via virtual call to slot 19
    typedef void (*TransformFunc)(void*, float);
    TransformFunc transformFn = reinterpret_cast<TransformFunc>(vtable[19]);
    transformFn(transformAnimDof, m_value);
    
    // Store transformed result
    m_value = transformValue;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::Normalize()  [vtable slot 13 @ 0x8216EE00]
// 
// Resets the float value to zero (0.0f).
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::Normalize() {
    m_value = 0.0f;
}

} // namespace rage

namespace rage {

// ═════════════════════════════════════════════════════════════════════════════
// crAnimDofQuaternion vtable implementations
// ═════════════════════════════════════════════════════════════════════════════

namespace {

// ─────────────────────────────────────────────────────────────────────────────
// NormalizeQuaternion — normalizes a quaternion in-place
// Source: util_42D8 @ 0x820C42D8
// ─────────────────────────────────────────────────────────────────────────────
void NormalizeQuaternion(float* quat) {
    float magSquared = quat[0] * quat[0] + quat[1] * quat[1] +
                       quat[2] * quat[2] + quat[3] * quat[3];

    if (magSquared != 0.0f) {
        float invMag = 1.0f / std::sqrt(magSquared);
        quat[0] *= invMag;
        quat[1] *= invMag;
        quat[2] *= invMag;
        quat[3] *= invMag;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// GetQuaternionAngle — returns the rotation angle of a quaternion
// Source: util_4340 @ 0x820C4340
// ─────────────────────────────────────────────────────────────────────────────
float GetQuaternionAngle(const float* quat) {
    float w = quat[3];

    if (w > 0.0f) {
        if (w < 1.0f) {
            return 2.0f * std::acos(w);
        }
        return 0.0f;
    }

    return 3.14159265f;
}

} // anonymous namespace

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::GetValue()  [vtable slot 2 @ 0x820C4B20]
//
// Allocates and initializes a new crAnimDofQuaternion instance by cloning.
// Returns nullptr if allocation fails.
// ─────────────────────────────────────────────────────────────────────────────
void* crAnimDofQuaternion::GetValue() {
    void* newInstance = rage::Allocate(32, 16);

    if (newInstance) {
        // Set vtable pointer
        *reinterpret_cast<void**>(newInstance) = reinterpret_cast<void*>(0x82027BF4);

        // Copy flags and metadata from offset +4 to +7
        uint8_t* src = reinterpret_cast<uint8_t*>(this);
        uint8_t* dst = reinterpret_cast<uint8_t*>(newInstance);
        dst[4] = src[4];
        dst[5] = src[5];
        *reinterpret_cast<uint16_t*>(dst + 6) = *reinterpret_cast<uint16_t*>(src + 6);

        // Copy the quaternion data at offset +16 (16 bytes: x, y, z, w)
        float* srcQuat = reinterpret_cast<float*>(src + 16);
        float* dstQuat = reinterpret_cast<float*>(dst + 16);
        dstQuat[0] = srcQuat[0];
        dstQuat[1] = srcQuat[1];
        dstQuat[2] = srcQuat[2];
        dstQuat[3] = srcQuat[3];
    }

    return newInstance;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::Evaluate()  [vtable slot 4 @ 0x820C4BF0]
//
// Evaluates animation channels and stores the result quaternion.
// Delegates to EvaluateChannels helper.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::Evaluate(void* animContext, float time, float* outQuat) {
    EvaluateChannels(animContext, time, outQuat);
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::BlendFrom()  [vtable slot 7 @ 0x820C4D80]
//
// Blends towards another DOF's quaternion value with shortest-path interpolation.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::BlendFrom(void* sourceAnimDof, float blendFactor) {
    if (blendFactor <= 0.0f) {
        return;
    }

    // Get source quaternion via virtual call to slot 15
    void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
    typedef float* (*GetQuatFunc)(void*);
    GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
    float* sourceQuat = getQuatFn(sourceAnimDof);

    // Get current quaternion
    float* currentQuat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);

    if (blendFactor < 1.0f) {
        // Dot product for hemisphere check
        float dot = sourceQuat[3] * currentQuat[3] + sourceQuat[0] * currentQuat[0] +
                    currentQuat[1] * sourceQuat[1] + currentQuat[2] * sourceQuat[2];

        // Negate current quaternion if dot < 0 (shortest path)
        if (dot < 0.0f) {
            currentQuat[0] = -currentQuat[0];
            currentQuat[1] = -currentQuat[1];
            currentQuat[2] = -currentQuat[2];
            currentQuat[3] = -currentQuat[3];
        }

        // Lerp: current += (source - current) * blendFactor
        currentQuat[0] += (sourceQuat[0] - currentQuat[0]) * blendFactor;
        *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 20) +=
            (sourceQuat[1] - *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 20)) * blendFactor;
        *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 24) +=
            (sourceQuat[2] - *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 24)) * blendFactor;
        *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 28) +=
            (sourceQuat[3] - *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 28)) * blendFactor;

        // Normalize the result
        NormalizeQuaternion(currentQuat);
    } else {
        // Full blend — direct copy
        currentQuat[0] = sourceQuat[0];
        currentQuat[1] = sourceQuat[1];
        currentQuat[2] = sourceQuat[2];
        currentQuat[3] = sourceQuat[3];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::ThresholdFrom()  [vtable slot 9 @ 0x820C5120]
//
// Replaces the current quaternion with source if rotation angle exceeds threshold.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::ThresholdFrom(void* sourceAnimDof, float threshold) {
    float* currentQuat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);

    // Get angular magnitude of current quaternion
    float angle = GetQuaternionAngle(currentQuat);

    // Compare angle² vs threshold²
    float angleSquared = angle * angle;
    float thresholdSquared = threshold * threshold;

    if (angleSquared > thresholdSquared) {
        // Get source quaternion via virtual call to slot 15
        void** vtable = *reinterpret_cast<void***>(sourceAnimDof);
        typedef float* (*GetQuatFunc)(void*);
        GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
        float* sourceQuat = getQuatFn(sourceAnimDof);

        currentQuat[0] = sourceQuat[0];
        currentQuat[1] = sourceQuat[1];
        currentQuat[2] = sourceQuat[2];
        currentQuat[3] = sourceQuat[3];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::TransformBy()  [vtable slot 12 @ 0x820C5220]
//
// Transforms the quaternion through another DOF's coordinate space.
// Gets the transform quaternion via slot 15, dispatches slot 18 to apply.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::TransformBy(void* transformAnimDof) {
    void** vtable = *reinterpret_cast<void***>(transformAnimDof);

    // Get transform's quaternion via virtual slot 15
    typedef float* (*GetQuatFunc)(void*);
    GetQuatFunc getQuatFn = reinterpret_cast<GetQuatFunc>(vtable[15]);
    float* transformQuat = getQuatFn(transformAnimDof);

    // Save transform's quaternion locally
    float savedQuat[4];
    savedQuat[0] = transformQuat[0];
    savedQuat[1] = transformQuat[1];
    savedQuat[2] = transformQuat[2];
    savedQuat[3] = transformQuat[3];

    // Apply transform via virtual slot 18 with our quaternion
    float* currentQuat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);

    typedef void (*TransformFunc)(void*, float*);
    TransformFunc transformFn = reinterpret_cast<TransformFunc>(vtable[18]);
    transformFn(transformAnimDof, currentQuat);

    // Copy saved quaternion to self
    currentQuat[0] = savedQuat[0];
    currentQuat[1] = savedQuat[1];
    currentQuat[2] = savedQuat[2];
    currentQuat[3] = savedQuat[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::Normalize()  [vtable slot 13 @ 0x820C52B0]
//
// Resets the quaternion to identity (0, 0, 0, 1).
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::Normalize() {
    float* quat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    quat[0] = 0.0f;
    quat[1] = 0.0f;
    quat[2] = 0.0f;
    quat[3] = 1.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::SetValue()  [vtable slot 18 @ 0x820C52D8]
//
// Copies a quaternion from an external source into this DOF's storage.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::SetValue(const float* sourceQuat) {
    float* quat = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    quat[0] = sourceQuat[0];
    quat[1] = sourceQuat[1];
    quat[2] = sourceQuat[2];
    quat[3] = sourceQuat[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::IsQuaternion()  [vtable slot 20 @ 0x8210CBD0]
//
// Returns 1 to indicate this DOF is quaternion-typed.
// ─────────────────────────────────────────────────────────────────────────────
int crAnimDofQuaternion::IsQuaternion() {
    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::EvaluateChannels() — Helper function
//
// Evaluates animation channels to compute the quaternion value.
// Handles different channel configurations:
//   0x00 — Single channel evaluating entire quaternion
//   0x10 — Four separate channels for x, y, z, w with normalization
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofQuaternion::EvaluateChannels(void* animContext, float time, float* outQuat) {
    uint8_t* thisPtr = reinterpret_cast<uint8_t*>(this);
    uint8_t channelConfig = thisPtr[1] & 0xF0;

    if (channelConfig == 0) {
        // Single channel mode — evaluate one channel for all quaternion components
        void** channels = reinterpret_cast<void**>(thisPtr + 4);
        void* channel = channels[0];

        if (channel) {
            void** vtable = *reinterpret_cast<void***>(channel);
            typedef void (*EvalFunc)(void*, float, float*);
            EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[4]);
            evalFn(channel, time, outQuat);
        }
    } else if (channelConfig == 0x10) {
        // Four channel mode — separate channels for x, y, z, w
        void** channels = reinterpret_cast<void**>(thisPtr + 4);

        for (int i = 0; i < 4; i++) {
            if (channels[i]) {
                void** vtable = *reinterpret_cast<void***>(channels[i]);
                typedef void (*EvalFunc)(void*, float, float*);
                EvalFunc evalFn = reinterpret_cast<EvalFunc>(vtable[5]);
                evalFn(channels[i], time, outQuat + i);
            }
        }

        // Normalize the resulting quaternion
        NormalizeQuaternion(outQuat);
    }
}

} // namespace rage
