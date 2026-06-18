#ifndef NH_GFX_METAL_SURFACE_IMPL_H
#define NH_GFX_METAL_SURFACE_IMPL_H

#if defined(APPLE)
    #include <QuartzCore/CAMetalLayer.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct nh_gfx_MetalSurface {
#if defined(APPLE)
    CAMetalLayer *layer;
#else
    int placeholder;
#endif
};

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_SURFACE_IMPL_H
