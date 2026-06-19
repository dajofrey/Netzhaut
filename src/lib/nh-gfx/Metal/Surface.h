#ifndef NH_GFX_METAL_SURFACE_H
#define NH_GFX_METAL_SURFACE_H

#include "../Common/Includes.h"

#ifdef __cplusplus
extern "C" {
#endif

void *nh_gfx_createMetalSurface(nh_api_Window *Window_p);
void nh_gfx_destroyMetalSurface(void *surface);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_SURFACE_H 
