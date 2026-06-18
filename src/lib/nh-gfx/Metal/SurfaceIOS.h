#ifndef NH_GFX_METAL_SURFACE_IOS_H
#define NH_GFX_METAL_SURFACE_IOS_H

#include "../../nh-wsi/Window/Window.h"

typedef struct nh_gfx_MetalIOSSurface {
    void *layer;
} nh_gfx_MetalIOSSurface;

NH_API_RESULT nh_gfx_createMetalIOSSurface(
    nh_gfx_MetalIOSSurface *Surface_p, nh_wsi_Window *Window_p);

NH_API_RESULT nh_gfx_updateMetalIOSSurface(
    nh_gfx_MetalIOSSurface *Surface_p, nh_wsi_Window *Window_p);

void nh_gfx_destroyMetalIOSSurface(
    nh_gfx_MetalIOSSurface *Surface_p);

#endif
