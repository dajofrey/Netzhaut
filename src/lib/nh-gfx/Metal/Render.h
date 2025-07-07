#ifndef NH_GFX_METAL_RENDER_H
#define NH_GFX_METAL_RENDER_H

#include "../Base/Surface.h"
#include "../Common/Includes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalRenderPass {
    // TODO: Add Metal render pass state
    int placeholder;
} nh_gfx_MetalRenderPass;

// Render pass management
nh_gfx_MetalRenderPass *nh_gfx_createMetalRenderPass();
void nh_gfx_destroyMetalRenderPass(nh_gfx_MetalRenderPass *renderPass);

NH_API_RESULT nh_gfx_renderMetal(nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_RENDER_H 