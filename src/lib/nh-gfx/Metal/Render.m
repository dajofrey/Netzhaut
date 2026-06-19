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

    nh_gfx_MetalSurface *metalSurface = (nh_gfx_MetalSurface *)Surface_p->Metal_p;
    
    // Retrieve the drawable that was cached by beginMetalRecording
    id<CAMetalDrawable> currentDrawable = metalSurface->currentDrawable;

    for (int i = 0; i < SortedViewports_p->size; ++i) {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
        
        if (!Viewport_p->Metal_p || !Viewport_p->Metal_p->commandBuffer) {
            continue;
        }

        // If this is the final viewport, schedule the presentation of the drawable
        if (i == SortedViewports_p->size - 1 && currentDrawable) {
            [Viewport_p->Metal_p->commandBuffer presentDrawable:currentDrawable];
        }

        // Submit the command buffer to the GPU hardware
        [Viewport_p->Metal_p->commandBuffer commit];

        // Release our reference so a new one can be created next frame
        Viewport_p->Metal_p->commandBuffer = nil;
    }

    // CRITICAL: Reset the cache so the next frame grabs a fresh drawable
    metalSurface->currentDrawable = nil;

    return NH_API_SUCCESS;
}
