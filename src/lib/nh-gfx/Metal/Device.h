#ifndef NH_GFX_METAL_DEVICE_H
#define NH_GFX_METAL_DEVICE_H

#include "../Common/Includes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalDevice nh_gfx_MetalDevice;

nh_gfx_MetalDevice *nh_gfx_createMetalDevice();
void nh_gfx_freeMetalDevice(nh_gfx_MetalDevice *Device_p);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_DEVICE_H 
