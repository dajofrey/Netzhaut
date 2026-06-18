#ifndef NH_GFX_METAL_DEVICE_IMPL_H
#define NH_GFX_METAL_DEVICE_IMPL_H

#import <Metal/Metal.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nh_gfx_MetalDevice {
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;
};

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_DEVICE_IMPL_H
