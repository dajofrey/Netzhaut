#include "GPU.h"
#include <stdlib.h>

nh_gfx_MetalGPUInfo *nh_gfx_getMetalGPUInfo(nh_gfx_MetalGPU *gpu) {
    nh_gfx_MetalGPUInfo *info = (nh_gfx_MetalGPUInfo *)malloc(sizeof(nh_gfx_MetalGPUInfo));
    info->name = "Apple GPU"; // TODO: Query real GPU name
    return info;
}

void nh_gfx_freeMetalGPUInfo(nh_gfx_MetalGPUInfo *info) {
    free(info);
} 