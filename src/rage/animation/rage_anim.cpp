/**
 * rage_anim.cpp — Animation system implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * RAGE animation DOF (Degree of Freedom) classes for blending and interpolation.
 */

#include "rage/animation/rage_anim.hpp"
#include <algorithm>

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

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_2()  [vtable slot 2 @ 0x820C46F0]
// 
// Allocates and initializes a new crAnimDofVector3 instance by cloning this object.
// Returns nullptr if allocation fails.
// ─────────────────────────────────────────────────────────────────────────────
void* crAnimDofVector3::vfn_2() {
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
// crAnimDofVector3::vfn_3()  [vtable slot 3 @ 0x820C4BE8]
// 
// Returns the size of the crAnimDofVector3 structure (32 bytes).
// ─────────────────────────────────────────────────────────────────────────────
int crAnimDofVector3::vfn_3() {
    return 32;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_4()  [vtable slot 4 @ 0x820C47A0]
// 
// Evaluates animation channels and stores the result in the output vector.
// Delegates to helper function for channel evaluation.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_4(void* animContext, float time, float* outVector) {
    EvaluateChannels(animContext, time, outVector);
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_5()  [vtable slot 5 @ 0x820C47B0]
// 
// Blends the current vector towards a target vector using linear interpolation.
// The blend factor is clamped to [0.0, 1.0] range.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_5(void* animContext, float blendFactor, float* outVector) {
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
// crAnimDofVector3::vfn_6()  [vtable slot 6 @ 0x820C4850]
// 
// Copies the vector value from another animation DOF object.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_6(void* sourceAnimDof) {
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
// crAnimDofVector3::vfn_7()  [vtable slot 7 @ 0x820C4898]
// 
// Blends towards another animation DOF's vector value with interpolation.
// Similar to vfn_5 but takes another DOF object as source instead of evaluating channels.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_7(void* sourceAnimDof, float blendFactor) {
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
// crAnimDofVector3::vfn_8()  [vtable slot 8 @ 0x820C4948]
// 
// Adds a scaled vector from another animation DOF to the current vector.
// Only performs the operation if the scale factor squared exceeds a threshold.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_8(void* sourceAnimDof, float scaleFactor) {
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
// crAnimDofVector3::vfn_9()  [vtable slot 9 @ 0x820C49D0]
// 
// Normalizes the vector if its magnitude exceeds a threshold.
// Replaces the vector with a normalized version from another DOF if needed.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_9(void* sourceAnimDof, float threshold) {
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
// crAnimDofVector3::vfn_10()  [vtable slot 10 @ 0x820C4A38]
// 
// Scales the vector by a uniform scalar value.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_10(float scaleFactor) {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Scale all components uniformly
    currentVec[0] *= scaleFactor;
    currentVec[1] *= scaleFactor;
    currentVec[2] *= scaleFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_11()  [vtable slot 11 @ 0x820C4A60]
// 
// Scales only the X component of the vector by a constant factor.
// The constant appears to be a conversion or normalization factor.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_11() {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Scale X component by constant (appears to be degrees-to-radians or similar)
    // The constant is loaded from a global at a specific address
    const float scaleFactor = 0.017453292f; // Approximately PI/180 (degrees to radians)
    currentVec[0] *= scaleFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_12()  [vtable slot 12 @ 0x820C4A78]
// 
// Transforms the vector through another animation DOF's coordinate space.
// Gets the transform vector and applies it via virtual call to slot 17.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_12(void* transformAnimDof) {
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
// crAnimDofVector3::vfn_13()  [vtable slot 13 @ 0x820C4AF8]
// 
// Resets the vector to zero (0, 0, 0).
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_13() {
    // Get current vector
    float* currentVec = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this) + 16);
    
    // Reset to zero
    currentVec[0] = 0.0f;
    currentVec[1] = 0.0f;
    currentVec[2] = 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_17()  [vtable slot 17 @ 0x820C4B10]
// 
// Copies a vector from an external source into this DOF's vector storage.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofVector3::vfn_17(const float* sourceVec) {
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
