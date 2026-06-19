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
    Viewport_p->Metal_p->renderPassDescriptor = nil;

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
        if (Viewport_p->Metal_p->renderPassDescriptor) {
            Viewport_p->Metal_p->renderPassDescriptor = nil;
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
        metalSurface->currentDrawable = [(__bridge CAMetalLayer *)metalSurface->layer nextDrawable];
    }

    id<CAMetalDrawable> drawable = metalSurface->currentDrawable;
    if (!drawable) {
        return NH_API_ERROR_BAD_STATE; // Drop the frame if the OS refuses to provide a drawable
    }

    MTLRenderPassDescriptor *passDescriptor = Viewport_p->Metal_p->renderPassDescriptor;
    if (!passDescriptor) {
        passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
        Viewport_p->Metal_p->renderPassDescriptor = passDescriptor;
    }

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
 
    // Calculate dimensions exactly as OpenGL does
    int innerX = Viewport_p->Settings.Position.x + Viewport_p->Settings.borderWidth;
    int innerY = Viewport_p->Settings.Position.y + Viewport_p->Settings.borderWidth;
    int innerW = Viewport_p->Settings.Size.width - (Viewport_p->Settings.borderWidth * 2);
    int innerH = Viewport_p->Settings.Size.height - (Viewport_p->Settings.borderWidth * 2);

    // Guard against negative dimensions to prevent Metal validation crashes
    if (innerW < 0) innerW = 0;
    if (innerH < 0) innerH = 0;

    // 2. Set Viewport (Equivalent to the first glViewport)
    MTLViewport viewport;
    viewport.originX = innerX;
    viewport.originY = innerY;
    viewport.width   = innerW;
    viewport.height  = innerH;
    viewport.znear   = 0.0;
    viewport.zfar    = 1.0;
    [Viewport_p->Metal_p->renderEncoder setViewport:viewport];

    // 3. Set Scissor Rect (Equivalent to the inner glScissor call)
    // Note: MTLScissorRect uses NSUInteger, so we must clamp to 0.
    MTLScissorRect scissor;
    scissor.x      = MAX(0, innerX);
    scissor.y      = MAX(0, innerY);
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
        // NOTE: Your OpenGL code sets glColorMask(0,0,0,1) and clears the alpha channel.
        // To replicate this in Metal, you need to draw a fullscreen quad with a pipeline
        // state configured with `colorAttachment[0].writeMask = MTLColorWriteMaskAlpha` 
        // right here before ending the encoding.
        
        [Viewport_p->Metal_p->renderEncoder endEncoding];
        Viewport_p->Metal_p->renderEncoder = nil;
    }

    return NH_API_SUCCESS;
}
