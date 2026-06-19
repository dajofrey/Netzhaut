#include "Surface.h"
#include "SurfaceImpl.h"
#include "DeviceImpl.h"
#include "Metal.h"

#include "../../nh-wsi/Window/Window.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

#include <stdlib.h>

void *nh_gfx_createMetalSurface(
    nh_api_Window *Window_p)
{
    nh_gfx_MetalSurface *Surface_p = malloc(sizeof(nh_gfx_MetalSurface));

    NSWindow *nsWindow = (__bridge NSWindow*)((nh_wsi_Window*)Window_p)->Cocoa.Handle;
    if (!nsWindow) { return NULL; }

    NSView *contentView = [nsWindow contentView];
    if (!contentView) { return NULL; }

    CAMetalLayer *layer = [CAMetalLayer layer];
    layer.device = NH_METAL.Device_p->device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;
    
    if ([layer respondsToSelector:@selector(setMaximumDrawableCount:)]) {
        [layer setMaximumDrawableCount:3]; // Triple buffering
    }

    // --- FIX APPLIED HERE ---
    CGFloat scaleFactor = nsWindow.backingScaleFactor;
    CGSize logicalSize = contentView.bounds.size;
    
    layer.contentsScale = scaleFactor;
    layer.drawableSize = CGSizeMake(logicalSize.width * scaleFactor, logicalSize.height * scaleFactor);
    // ------------------------

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
