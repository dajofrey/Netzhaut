#ifndef NH_GFX_METAL_VIEWPORT_H
#define NH_GFX_METAL_VIEWPORT_H

#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalViewport {
    id<MTLCommandBuffer> commandBuffer;
} nh_gfx_MetalViewport;

NH_API_RESULT nh_gfx_createMetalViewport(
    nh_gfx_Viewport *Viewport_p
);
void nh_gfx_destroyMetalViewport(
    nh_gfx_Viewport *Viewport_p
);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_VIEWPORT_H 
