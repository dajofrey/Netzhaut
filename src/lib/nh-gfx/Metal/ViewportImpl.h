#ifndef NH_GFX_METAL_VIEWPORT_IMPL_H
#define NH_GFX_METAL_VIEWPORT_IMPL_H

#include "Metal.h"
#import <Metal/Metal.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalViewport {
    id<MTLCommandBuffer> commandBuffer;
    id<MTLRenderCommandEncoder> renderEncoder;
} nh_gfx_MetalViewport;

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_VIEWPORT_IMPL_H 
