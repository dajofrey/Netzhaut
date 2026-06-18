#import "SurfaceIOS.h"

#import <QuartzCore/CAMetalLayer.h>
#import <UIKit/UIKit.h>

#include "../../nh-wsi/Platforms/IOS/Window.h"

static void nh_gfx_configureMetalLayer(CAMetalLayer *layer, UIView *view)
{
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;
    if ([layer respondsToSelector:@selector(setMaximumDrawableCount:)]) {
        [layer setMaximumDrawableCount:3];
    }
    layer.contentsScale = view.contentScaleFactor;
    layer.drawableSize = CGSizeMake(
        view.bounds.size.width * view.contentScaleFactor,
        view.bounds.size.height * view.contentScaleFactor
    );
}

NH_API_RESULT nh_gfx_createMetalIOSSurface(
    nh_gfx_MetalIOSSurface *Surface_p, nh_wsi_Window *Window_p)
{
    if (!Surface_p || !Window_p || !Window_p->IOS.Layer) {
        return NH_API_ERROR_BAD_STATE;
    }

    CAMetalLayer *layer = (__bridge CAMetalLayer*)Window_p->IOS.Layer;
    UIWindow *window = (__bridge UIWindow*)Window_p->IOS.Handle;
    UIView *view = window.rootViewController.view;

    nh_gfx_configureMetalLayer(layer, view);
    Surface_p->layer = Window_p->IOS.Layer;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_updateMetalIOSSurface(
    nh_gfx_MetalIOSSurface *Surface_p, nh_wsi_Window *Window_p)
{
    if (!Surface_p || !Window_p || !Window_p->IOS.Layer) {
        return NH_API_ERROR_BAD_STATE;
    }

    CAMetalLayer *layer = (__bridge CAMetalLayer*)Window_p->IOS.Layer;
    UIWindow *window = (__bridge UIWindow*)Window_p->IOS.Handle;
    UIView *view = window.rootViewController.view;

    nh_gfx_configureMetalLayer(layer, view);

    return NH_API_SUCCESS;
}

void nh_gfx_destroyMetalIOSSurface(
    nh_gfx_MetalIOSSurface *Surface_p)
{
    if (Surface_p) {
        Surface_p->layer = NULL;
    }
}
