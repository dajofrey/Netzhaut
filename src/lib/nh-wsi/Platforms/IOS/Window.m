#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>

#include "Window.h"
#include "WindowPrivate.h"

#include "../../Window/Window.h"
#include "../../Window/WindowSettings.h"
#include "../../Window/Listener.h"
#include "../../Window/Event.h"
#include "../../../nh-api/nh-wsi.h"
#include "../../Common/Config.h"
#include "../../Common/Includes.h"
#include "../../Common/Log.h"

@interface NHCustomView : UIView
@property (nonatomic, assign) void *Window_p;
@end

@implementation NHCustomView

+ (Class)layerClass {
    return [CAMetalLayer class];
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    CGPoint localPoint = [touch locationInView:self];
    CGFloat scale = self.contentScaleFactor;

    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);

    nh_wsi_sendMouseEvent(self.Window_p, px, py, NH_API_TRIGGER_PRESS, NH_API_MOUSE_LEFT);
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    CGPoint localPoint = [touch locationInView:self];
    CGFloat scale = self.contentScaleFactor;

    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);

    nh_wsi_sendMouseEvent(self.Window_p, px, py, NH_API_TRIGGER_MOVE, NH_API_MOUSE_LEFT);
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    CGPoint localPoint = [touch locationInView:self];
    CGFloat scale = self.contentScaleFactor;

    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);

    nh_wsi_sendMouseEvent(self.Window_p, px, py, NH_API_TRIGGER_RELEASE, NH_API_MOUSE_LEFT);
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self touchesEnded:touches withEvent:event];
}

- (void)pressesBegan:(NSSet<UIPress *> *)presses withEvent:(UIPressesEvent *)event {
    for (UIPress *press in presses) {
        NH_ENCODING_UTF32 codepoint = 0;
        NSString *characters = press.key.characters;

        if (characters.length > 0) {
            codepoint = [characters characterAtIndex:0];
        }

        NH_API_MODIFIER_FLAG state = 0;
        UIKeyModifierFlags flags = press.key.modifierFlags;

        if (flags & UIKeyModifierShift) state |= NH_API_MODIFIER_SHIFT;
        if (flags & UIKeyModifierControl) state |= NH_API_MODIFIER_CONTROL;
        if (flags & UIKeyModifierAlternate) state |= NH_API_MODIFIER_MOD1;
        if (flags & UIKeyModifierCommand) state |= NH_API_MODIFIER_MOD4;

        NH_API_KEY_E special = 0;
        switch (press.key.keyCode) {
            case UIKeyboardHIDUsageKeyboardLeftArrow:  special = NH_API_KEY_KP_LEFT;  break;
            case UIKeyboardHIDUsageKeyboardUpArrow:    special = NH_API_KEY_KP_UP;    break;
            case UIKeyboardHIDUsageKeyboardRightArrow: special = NH_API_KEY_KP_RIGHT; break;
            case UIKeyboardHIDUsageKeyboardDownArrow:  special = NH_API_KEY_KP_DOWN;  break;
        }

        if (special) { codepoint = 0; }
        nh_wsi_sendKeyboardEvent(self.Window_p, codepoint, special, NH_API_TRIGGER_PRESS, state);
    }
    [super pressesBegan:presses withEvent:event];
}

- (void)pressesEnded:(NSSet<UIPress *> *)presses withEvent:(UIPressesEvent *)event {
    for (UIPress *press in presses) {
        NH_ENCODING_UTF32 codepoint = 0;
        NSString *characters = press.key.characters;

        if (characters.length > 0) {
            codepoint = [characters characterAtIndex:0];
        }

        NH_API_MODIFIER_FLAG state = 0;
        UIKeyModifierFlags flags = press.key.modifierFlags;

        if (flags & UIKeyModifierShift) state |= NH_API_MODIFIER_SHIFT;
        if (flags & UIKeyModifierControl) state |= NH_API_MODIFIER_CONTROL;
        if (flags & UIKeyModifierAlternate) state |= NH_API_MODIFIER_MOD1;
        if (flags & UIKeyModifierCommand) state |= NH_API_MODIFIER_MOD4;

        NH_API_KEY_E special = 0;
        switch (press.key.keyCode) {
            case UIKeyboardHIDUsageKeyboardLeftArrow:  special = NH_API_KEY_KP_LEFT;  break;
            case UIKeyboardHIDUsageKeyboardUpArrow:    special = NH_API_KEY_KP_UP;    break;
            case UIKeyboardHIDUsageKeyboardRightArrow: special = NH_API_KEY_KP_RIGHT; break;
            case UIKeyboardHIDUsageKeyboardDownArrow:  special = NH_API_KEY_KP_DOWN;  break;
        }

        if (special) { codepoint = 0; }
        nh_wsi_sendKeyboardEvent(self.Window_p, codepoint, special, NH_API_TRIGGER_RELEASE, state);
    }
    [super pressesEnded:presses withEvent:event];
}

@end

@interface NHCustomViewController : UIViewController
@property (nonatomic, assign) nh_wsi_Window *Window_p;
@end

@implementation NHCustomViewController

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];

    if (!self.Window_p) return;

    CGFloat scale = [[UIScreen mainScreen] scale];
    int pointWidth  = (int)size.width;
    int pointHeight = (int)size.height;
    int pixelWidth  = (int)lround(size.width * scale);
    int pixelHeight = (int)lround(size.height * scale);

    nh_wsi_sendWindowEvent(self.Window_p, NH_API_WINDOW_CONFIGURE,
                           0, 0, pointWidth, pointHeight, pixelWidth, pixelHeight);
}

@end

static void nh_wsi_configureIOSMetalLayer(CAMetalLayer *layer, UIView *view)
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

NH_API_RESULT nh_wsi_createIOSWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p)
{
    (void)Config;
    (void)Requirements_p;

    @autoreleasepool {
        CGRect screenBounds = [[UIScreen mainScreen] bounds];
        CGFloat screenScale = [[UIScreen mainScreen] scale]; // Get the Retina scaler

        UIWindow *window = [[UIWindow alloc] initWithFrame:screenBounds];
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        window.backgroundColor = [UIColor whiteColor];

        NHCustomViewController *viewController = [[NHCustomViewController alloc] init];
        viewController.Window_p = Window_p;

        NHCustomView *view = [[NHCustomView alloc] initWithFrame:screenBounds];
        view.Window_p = Window_p;
        viewController.view = view;

        window.rootViewController = viewController;
        [window makeKeyAndVisible];
        [view becomeFirstResponder];

        // Force a layout pass so UIKit calculates the safe area insets immediately
        [view layoutIfNeeded];

        nh_wsi_configureIOSMetalLayer((CAMetalLayer*)view.layer, view);

        Window_p->IOS.Handle = (__bridge_retained void*)window;
        Window_p->IOS.ViewController = (__bridge_retained void*)viewController;
        Window_p->IOS.Layer = (__bridge void*)view.layer;

        // retina scale
        Window_p->scale = (float)screenScale;

        // Extract safe area in points and convert to physical pixels
        UIEdgeInsets insets = view.safeAreaInsets;
        Window_p->safeAreaTop    = (int)(insets.top * screenScale);
        Window_p->safeAreaBottom = (int)(insets.bottom * screenScale);
        Window_p->safeAreaLeft   = (int)(insets.left * screenScale);
        Window_p->safeAreaRight  = (int)(insets.right * screenScale);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_destroyIOSWindow(
    nh_wsi_IOSWindow *Window_p)
{
    @autoreleasepool {
        if (Window_p->Handle) {
            UIWindow *window = (__bridge_transfer UIWindow*)Window_p->Handle;
            window.hidden = YES;
            Window_p->Handle = NULL;
        }

        if (Window_p->ViewController) {
            (__bridge_transfer NHCustomViewController*)Window_p->ViewController;
            Window_p->ViewController = NULL;
        }

        Window_p->Layer = NULL;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setIOSClipboardOwner(
    nh_wsi_IOSWindow *Window_p)
{
    (void)Window_p;
    @autoreleasepool {
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        pasteboard.string = @"";
    }
    return NH_API_SUCCESS;
}

bool nh_wsi_isIOSClipboardOwner(
    nh_wsi_IOSWindow *Window_p)
{
    (void)Window_p;
    return true;
}

NH_API_RESULT nh_wsi_requestIOSClipboardConversion(
    nh_wsi_IOSWindow *Window_p)
{
    (void)Window_p;
    @autoreleasepool {
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        if (pasteboard.hasStrings) {
            NSString *string = pasteboard.string;
            if (string) {
                const char *utf8 = [string UTF8String];
                nh_wsi_setClipboard(utf8, strlen(utf8), false);
            }
        }
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_getIOSInput(
    nh_wsi_Window *Window_p, bool *idle_p)
{
    (void)Window_p;
    SInt32 result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, YES);

    if (result == kCFRunLoopRunHandledSource) {
        *idle_p = false;
    } else {
        *idle_p = true;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_moveIOSWindow(
    nh_wsi_IOSWindow *Window_p)
{
    (void)Window_p;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setIOSWindowType(
    nh_wsi_IOSWindow *Window_p, NH_WSI_WINDOW_TYPE_E type)
{
    (void)Window_p;
    (void)type;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_getIOSWindowSize(
    nh_wsi_IOSWindow *Window_p, int *x_p, int *y_p)
{
    if (!Window_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    @autoreleasepool {
        UIWindow *window = (__bridge UIWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        CGRect bounds = [window bounds];
        CGFloat scaleFactor = [window contentScaleFactor];

        *x_p = bounds.size.width * scaleFactor;
        *y_p = bounds.size.height * scaleFactor;
    }

    return NH_API_SUCCESS;
}
