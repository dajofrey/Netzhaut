#ifndef NH_GFX_METAL_SURFACE_H
#define NH_GFX_METAL_SURFACE_H

#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalSurface {
#if defined(APPLE)
    #include <QuartzCore/CAMetalLayer.h>
    CAMetalLayer *layer;
#else
    int bla;
#endif
} nh_gfx_MetalSurface;

nh_gfx_MetalSurface *nh_gfx_createMetalSurface(void *view);
void nh_gfx_destroyMetalSurface(nh_gfx_MetalSurface *surface);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_SURFACE_H 
