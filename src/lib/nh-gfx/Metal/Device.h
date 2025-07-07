#ifndef NH_GFX_METAL_DEVICE_H
#define NH_GFX_METAL_DEVICE_H

#import <Metal/Metal.h>
#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalGPU {
    id<MTLDevice> device;
} nh_gfx_MetalGPU;

nh_gfx_MetalGPU *nh_gfx_enumerateMetalGPUs(int *count);
void nh_gfx_freeMetalGPUs(nh_gfx_MetalGPU *gpus);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_DEVICE_H 