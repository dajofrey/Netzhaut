#import "Device.h"
#include <stdlib.h>

nh_gfx_MetalGPU *nh_gfx_enumerateMetalGPUs(int *count) {
    // Metal only exposes the default device in most cases
    nh_gfx_MetalGPU *gpus = (nh_gfx_MetalGPU *)malloc(sizeof(nh_gfx_MetalGPU));
    gpus[0].device = MTLCreateSystemDefaultDevice();
    if (count) *count = 1;
    return gpus;
}

void nh_gfx_freeMetalGPUs(nh_gfx_MetalGPU *gpus) {
    free(gpus);
} 