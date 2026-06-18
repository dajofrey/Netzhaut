#ifndef NH_GFX_METAL_SURFACE_H
#define NH_GFX_METAL_SURFACE_H

#include "../Common/Includes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalSurface nh_gfx_MetalSurface;

void *nh_gfx_createMetalSurface(void *view);
void nh_gfx_destroyMetalSurface(void *surface);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_SURFACE_H 
