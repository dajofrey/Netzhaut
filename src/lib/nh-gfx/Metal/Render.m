// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#import "Render.h"
#import "Metal.h"
#import "ViewportImpl.h"
#import "SurfaceImpl.h"

#import "../Base/Viewport.h"
#import "../Base/Surface.h"
#import "../../nh-wsi/Window/Window.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <stdlib.h>

// METAL RENDER PASS (Placeholders from your original file) ========================================

nh_gfx_MetalRenderPass *nh_gfx_createMetalRenderPass() 
{
    nh_gfx_MetalRenderPass *rp = (nh_gfx_MetalRenderPass *)malloc(sizeof(nh_gfx_MetalRenderPass));
    if (rp) {
        rp->placeholder = 0;
    }
    return rp;
}

void nh_gfx_destroyMetalRenderPass(nh_gfx_MetalRenderPass *rp) 
{
    if (rp) {
        free(rp);
    }
}

// RENDER ==========================================================================================

NH_API_RESULT nh_gfx_renderMetal(
    nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p)
{
    if (!Surface_p || !SortedViewports_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    // 1. Acquire the current drawable from the CAMetalLayer attached to the Surface.
    // (This assumes you store the CAMetalLayer reference in Surface_p->Metal.Layer_p)
    id<CAMetalDrawable> currentDrawable = nil;
    if (((nh_gfx_MetalSurface*)Surface_p->Metal_p)->layer) {
        currentDrawable = [(__bridge CAMetalLayer *)((nh_gfx_MetalSurface*)Surface_p->Metal_p)->layer nextDrawable];
    }

    // 2. Iterate through viewports to commit their encoded command buffers
    for (int i = 0; i < SortedViewports_p->size; ++i) {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
        
        if (!Viewport_p->Metal_p || !Viewport_p->Metal_p->commandBuffer) {
            continue;
        }

        // 3. If this is the final viewport rendering to the surface, and we have a valid 
        // screen drawable, schedule it for presentation.
        if (i == SortedViewports_p->size - 1 && currentDrawable) {
            [Viewport_p->Metal_p->commandBuffer presentDrawable:currentDrawable];
        }

        // 4. Execute the command buffer (Equivalent to nh_gfx_executeOpenGLCommandBuffer)
        [Viewport_p->Metal_p->commandBuffer commit];

        // 5. Free/Release the command buffer for this frame (Equivalent to nh_gfx_freeOpenGLCommandBuffer)
        Viewport_p->Metal_p->commandBuffer = nil;
    }

    return NH_API_SUCCESS;
}
