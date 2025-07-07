#include "Render.h"
#include <stdlib.h>
#include "../Base/Viewport.h"

nh_gfx_MetalRenderPass *nh_gfx_createMetalRenderPass() {
    nh_gfx_MetalRenderPass *rp = (nh_gfx_MetalRenderPass *)malloc(sizeof(nh_gfx_MetalRenderPass));
    rp->placeholder = 0;
    return rp;
}

void nh_gfx_destroyMetalRenderPass(nh_gfx_MetalRenderPass *rp) {
    if (rp) {
        free(rp);
    }
}

NH_API_RESULT nh_gfx_renderMetal(nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p) {
    // Framework: iterate over viewports and call a (future) per-viewport Metal render function
    for (int i = 0; i < SortedViewports_p->size; ++i) {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
        // Placeholder: actual rendering will be implemented elsewhere
        // e.g., nh_gfx_renderMetalViewport(Viewport_p);
    }
    return NH_API_SUCCESS;
} 