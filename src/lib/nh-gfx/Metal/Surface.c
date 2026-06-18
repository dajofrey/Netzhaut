#import "Surface.h"
#import "SurfaceImpl.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#include <stdlib.h>

void *nh_gfx_createMetalSurface(
    nh_wsi_Window *Window_p)
{
    nh_gfx_MetalSurface *Surface_p = malloc(sizeof(nh_gfx_MetalSurface));

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

    Surface_p->layer = layer;

    return Surface_p;
}

void nh_gfx_destroyMetalSurface(
    void *surface)
{
    nh_gfx_MetalSurface *Surface_p = (nh_gfx_MetalSurface *)surface;
    if (Surface_p && Surface_p->layer) {
        Surface_p->layer = NULL;
        free(Surface_p);
    }
} 
