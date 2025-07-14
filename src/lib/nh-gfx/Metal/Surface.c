#import "Surface.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#include <stdlib.h>

NH_API_RESULT nh_gfx_createMetalSurface(
    nh_gfx_MetalSurface *Surface_p, nh_wsi_Window *Window_p)
{
    NSWindow *nsWindow = (__bridge NSWindow*)Window_p->Handle;
    NH_API_CHECK_NULL(nsWindow)

    NSView *contentView = [nsWindow contentView];
    NH_API_CHECK_NULL(contentView)

    CAMetalLayer *layer = [CAMetalLayer layer];
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;
    if ([layer respondsToSelector:@selector(setMaximumDrawableCount:)]) {
        [layer setMaximumDrawableCount:3]; // Triple buffering, like Vulkan default
    }
    layer.drawableSize = contentView.bounds.size;
    layer.contentsScale = [contentView window].backingScaleFactor;

    [contentView setWantsLayer:YES];
    [contentView setLayer:layer];

    Surface_p->layer = (__bridge_retained void*)layer;

    return NH_API_SUCCESS;
}

void nh_gfx_destroyMetalSurface(
    nh_gfx_MetalSurface *surface)
{
    if (surface && surface->layer) {
        CAMetalLayer *layer = (__bridge_transfer CAMetalLayer*)surface->layer;
        surface->layer = NULL;
        // The layer will be released by ARC when no longer attached to a view
    }
} 
