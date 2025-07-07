#ifndef NH_GFX_METAL_GPU_H
#define NH_GFX_METAL_GPU_H

#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalGPUInfo {
    const char *name;
    // TODO: Add more GPU info fields
} nh_gfx_MetalGPUInfo;

nh_gfx_MetalGPUInfo *nh_gfx_getMetalGPUInfo(nh_gfx_MetalGPU *gpu);
void nh_gfx_freeMetalGPUInfo(nh_gfx_MetalGPUInfo *info);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_GPU_H 