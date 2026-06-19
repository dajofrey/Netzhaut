#import "Viewport.h"
#import "ViewportImpl.h"
#import "DeviceImpl.h"
#import "Metal.h"
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

    Viewport_p->Metal_p->commandBuffer = [NH_METAL.Device_p->commandQueue commandBuffer];
    // Initialization of renderEncoder and passDescriptor should be nil initially
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

    // Create a new command buffer for this frame
    Viewport_p->Metal_p->commandBuffer = [NH_METAL.Device_p->commandQueue commandBuffer];

    // Ensure we have a render pass descriptor. In a real engine context, this is usually 
    // provided by your swapchain, offscreen texture pool, or an MTKView.
    MTLRenderPassDescriptor *passDescriptor = Viewport_p->Metal_p->renderPassDescriptor;
    if (!passDescriptor) {
        passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    }

    // 1. Clear the viewport base (Equivalent to the first OpenGL glClearColor/glClear)
    // Metal load actions apply to the entire attachment. We use BorderColor as the baseline.
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(
        Viewport_p->Settings.BorderColor.r,
        Viewport_p->Settings.BorderColor.g,
        Viewport_p->Settings.BorderColor.b,
        Viewport_p->Settings.BorderColor.a
    );
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

    // Typically, you would present a drawable here if drawing to screen:
    // [Viewport_p->Metal_p->commandBuffer presentDrawable:currentDrawable];

    [Viewport_p->Metal_p->commandBuffer commit];

    return NH_API_SUCCESS;
}
