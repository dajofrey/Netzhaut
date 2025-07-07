#ifndef NH_GFX_METAL_VIEWPORT_H
#define NH_GFX_METAL_VIEWPORT_H

#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalViewport {
    float x, y, width, height, znear, zfar;
    // Add Metal-specific state as needed
} nh_gfx_MetalViewport;

NH_API_RESULT nh_gfx_createMetalViewport(struct nh_gfx_Viewport *Viewport_p);
void nh_gfx_destroyMetalViewport(struct nh_gfx_Viewport *Viewport_p);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_VIEWPORT_H 