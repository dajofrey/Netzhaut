#ifndef NH_GFX_METAL_DRIVER_H
#define NH_GFX_METAL_DRIVER_H

#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalDriver {
    nh_gfx_MetalDevice *device;
} nh_gfx_MetalDriver;

nh_gfx_MetalDriver *nh_gfx_createMetalDriver();
void nh_gfx_destroyMetalDriver(nh_gfx_MetalDriver *driver);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_DRIVER_H 