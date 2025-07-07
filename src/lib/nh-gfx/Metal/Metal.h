#ifndef NH_GFX_METAL_H
#define NH_GFX_METAL_H

#import <Metal/Metal.h>
#include "../Common/Includes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Device abstraction
typedef struct nh_gfx_MetalDevice {
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;
} nh_gfx_MetalDevice;

// Global Metal context
typedef struct nh_gfx_Metal {
    nh_gfx_MetalDevice *Device;
} nh_gfx_Metal;

extern nh_gfx_Metal NH_METAL;

// Surface abstraction (placeholder)
typedef struct nh_gfx_MetalSurface {
    void *layer; // CAMetalLayer*
} nh_gfx_MetalSurface;

// Command buffer abstraction
typedef struct nh_gfx_MetalCommandBuffer {
    id<MTLCommandBuffer> commandBuffer;
} nh_gfx_MetalCommandBuffer;

// Device management
nh_gfx_MetalDevice *nh_gfx_createMetalDevice();
void nh_gfx_destroyMetalDevice(nh_gfx_MetalDevice *device);

// Metal initialization/termination
NH_API_RESULT nh_gfx_initMetal(void);
NH_API_RESULT nh_gfx_terminateMetal(void);

// Command buffer management
nh_gfx_MetalCommandBuffer *nh_gfx_createMetalCommandBuffer(nh_gfx_MetalDevice *device);
void nh_gfx_commitMetalCommandBuffer(nh_gfx_MetalCommandBuffer *cmdBuffer);
void nh_gfx_destroyMetalCommandBuffer(nh_gfx_MetalCommandBuffer *cmdBuffer);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_H 