#ifndef NH_GFX_METAL_SURFACE_IMPL_H
#define NH_GFX_METAL_SURFACE_IMPL_H

#if defined(__APPLE__) && defined(__OBJC__)
    #include <QuartzCore/CAMetalLayer.h>
    #include <Metal/Metal.h>
#endif

typedef struct nh_gfx_MetalSurface {
#if defined(__APPLE__) && defined(__OBJC__)
    CAMetalLayer *layer;
    id<CAMetalDrawable> currentDrawable;
#elif defined(__APPLE__)
    void *layer;
    void *currentDrawable;
#else
    int placeholder;
#endif
} nh_gfx_MetalSurface;

#endif // NH_GFX_METAL_SURFACE_IMPL_H
