#import "Viewport.h"
#import "ViewportImpl.h"
#import "DeviceImpl.h"
#import "Metal.h"
#include "SurfaceImpl.h"

#import "../Base/Viewport.h"

#include "../../nh-core/System/Memory.h"
#include <stdlib.h>

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

NH_API_RESULT nh_gfx_createMetalViewport(
    nh_gfx_Viewport *Viewport_p)
{
    if (!NH_METAL.Device_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    Viewport_p->Metal_p = (nh_gfx_MetalViewport *)nh_core_allocate(sizeof(nh_gfx_MetalViewport));
    if (!Viewport_p->Metal_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    Viewport_p->Metal_p->commandBuffer = nil; 
    Viewport_p->Metal_p->renderEncoder = nil;

    return NH_API_SUCCESS;
}

void nh_gfx_destroyMetalViewport(
    nh_gfx_Viewport *Viewport_p)
{
    if (Viewport_p->Metal_p) {
        if (Viewport_p->Metal_p->renderEncoder) {
            [Viewport_p->Metal_p->renderEncoder endEncoding];
            Viewport_p->Metal_p->renderEncoder = nil;
        }
        if (Viewport_p->Metal_p->commandBuffer) {
            Viewport_p->Metal_p->commandBuffer = nil;
        }
        
        nh_core_free(Viewport_p->Metal_p);
        Viewport_p->Metal_p = NULL;
    }
}

NH_API_RESULT nh_gfx_beginMetalRecording(
    nh_gfx_Viewport *Viewport_p)
{
    if (!NH_METAL.Device_p || !Viewport_p->Metal_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    // Create a new command buffer for this specific frame
    Viewport_p->Metal_p->commandBuffer = [NH_METAL.Device_p->commandQueue commandBuffer];

    nh_gfx_MetalSurface *metalSurface = (nh_gfx_MetalSurface *)Viewport_p->Surface_p->Metal_p;
    
    // CHECKLIST #2: Fetch the drawable ONCE per frame and cache it. 
    // If multiple viewports are drawn, the first one fetches it; the rest reuse it.
    if (!metalSurface->currentDrawable) {
        metalSurface->currentDrawable = [metalSurface->layer nextDrawable];
    }

    id<CAMetalDrawable> drawable = metalSurface->currentDrawable;
    if (!drawable) {
        return NH_API_ERROR_BAD_STATE; // Drop the frame if the OS refuses to provide a drawable
    }

    // Delete the caching logic and just create it fresh:
    MTLRenderPassDescriptor *passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];

    // CHECKLIST #2: Explicitly bind the destination texture so Metal knows where to draw
    passDescriptor.colorAttachments[0].texture = drawable.texture;

    // Set clear colors
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(
        Viewport_p->Settings.BorderColor.r,
        Viewport_p->Settings.BorderColor.g,
        Viewport_p->Settings.BorderColor.b,
        Viewport_p->Settings.BorderColor.a
    );
    
    // Pro-Tip: If you have multiple viewports drawing on top of each other, 
    // only the FIRST viewport should use MTLLoadActionClear. Subsequent viewports 
    // should use MTLLoadActionLoad, otherwise they will erase the previous viewports.
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear; 
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

    // Create the render command encoder
    Viewport_p->Metal_p->renderEncoder = [Viewport_p->Metal_p->commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    if (!Viewport_p->Metal_p->renderEncoder) {
        return NH_API_ERROR_BAD_STATE;
    }

    int surfaceHeight = metalSurface->layer.drawableSize.height;
    
    // 2. Calculate the OpenGL (bottom-left) coordinates
    int innerX = Viewport_p->Settings.Position.x + Viewport_p->Settings.borderWidth;
    int glInnerY = Viewport_p->Settings.Position.y + Viewport_p->Settings.borderWidth;
    int innerW = Viewport_p->Settings.Size.width - (Viewport_p->Settings.borderWidth * 2);
    int innerH = Viewport_p->Settings.Size.height - (Viewport_p->Settings.borderWidth * 2);
    
    // Guard against negative dimensions
    if (innerW < 0) innerW = 0;
    if (innerH < 0) innerH = 0;
    
    // 3. Convert to Metal (top-left) coordinates
    // Formula: Surface Height - OpenGL Y - Viewport Height
    int metalInnerY = surfaceHeight - glInnerY - innerH;
    
    // 4. Set Viewport
    MTLViewport viewport;
    viewport.originX = innerX;
    viewport.originY = metalInnerY; // <-- Use the flipped Y
    viewport.width   = innerW;
    viewport.height  = innerH;
    viewport.znear   = 0.0;
    viewport.zfar    = 1.0;
    [Viewport_p->Metal_p->renderEncoder setViewport:viewport];
    
    // 5. Set Scissor Rect
    MTLScissorRect scissor;
    scissor.x      = MAX(0, innerX);
    scissor.y      = MAX(0, metalInnerY); // <-- Use the flipped Y
    scissor.width  = innerW;
    scissor.height = innerH;
    [Viewport_p->Metal_p->renderEncoder setScissorRect:scissor];

    // NOTE: To perfectly replicate the inner OpenGL clear to Settings.ClearColor, 
    // you must submit a solid colored quad draw call here using your engine's pipeline state, 
    // since Metal cannot issue a second hardware "clear" command mid-pass.

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_endMetalRecording(
    nh_gfx_Viewport *Viewport_p)
{
    if (!Viewport_p->Metal_p || !Viewport_p->Metal_p->commandBuffer) {
        return NH_API_ERROR_BAD_STATE;
    }

    if (Viewport_p->Metal_p->renderEncoder) {
        [Viewport_p->Metal_p->renderEncoder endEncoding];
        Viewport_p->Metal_p->renderEncoder = nil;
    }

    nh_gfx_MetalSurface *metalSurface = (nh_gfx_MetalSurface *)Viewport_p->Surface_p->Metal_p;

    // We must present the drawable before we commit the command buffer
    if (metalSurface->currentDrawable) {
        [Viewport_p->Metal_p->commandBuffer presentDrawable:metalSurface->currentDrawable];
    }

    // Commit the commands to the GPU
    [Viewport_p->Metal_p->commandBuffer commit];
    
    // Clear the drawable cache so the NEXT frame requests a fresh one
    metalSurface->currentDrawable = nil;
    Viewport_p->Metal_p->commandBuffer = nil;

    return NH_API_SUCCESS;
}
