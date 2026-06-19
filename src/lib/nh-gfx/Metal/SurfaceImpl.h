#ifndef NH_GFX_METAL_SURFACE_IMPL_H
#define NH_GFX_METAL_SURFACE_IMPL_H

#if defined(__APPLE__)
    #include <QuartzCore/CAMetalLayer.h>
#endif

typedef struct nh_gfx_MetalSurface {
#if defined(__APPLE__)
    CAMetalLayer *layer;
#else
    int placeholder;
#endif
} nh_gfx_MetalSurface;

#endif // NH_GFX_METAL_SURFACE_IMPL_H
