#import "Surface.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#include <stdlib.h>

NH_API_RESULT nh_gfx_createMetalSurface(nh_gfx_MetalSurface *Surface_p, nh_wsi_Window *Window_p) {
    if (!Surface_p || !Window_p) return NH_API_ERROR_BAD_STATE;

    // Get the NSWindow* from the window struct (Handle is assumed to be NSWindow*)
    NSWindow *nsWindow = (__bridge NSWindow*)Window_p->Handle;
    if (!nsWindow) return NH_API_ERROR_BAD_STATE;

    // Get the content view
    NSView *contentView = [nsWindow contentView];
    if (!contentView) return NH_API_ERROR_BAD_STATE;

    // Create and configure the CAMetalLayer
    CAMetalLayer *layer = [CAMetalLayer layer];
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;
    // Set drawable count (Metal API: preferredDrawableCount, available on macOS 10.13+)
    if ([layer respondsToSelector:@selector(setMaximumDrawableCount:)]) {
        [layer setMaximumDrawableCount:3]; // Triple buffering, like Vulkan default
    }
    // Set layer size to match the view
    layer.drawableSize = contentView.bounds.size;
    layer.contentsScale = [contentView window].backingScaleFactor;

    // Attach the layer to the view
    [contentView setWantsLayer:YES];
    [contentView setLayer:layer];

    // Store the layer in the Metal surface struct
    Surface_p->layer = (__bridge_retained void*)layer;

    return NH_API_SUCCESS;
}

void nh_gfx_destroyMetalSurface(nh_gfx_MetalSurface *surface) {
    if (surface && surface->layer) {
        CAMetalLayer *layer = (__bridge_transfer CAMetalLayer*)surface->layer;
        surface->layer = NULL;
        // The layer will be released by ARC when no longer attached to a view
    }
} 