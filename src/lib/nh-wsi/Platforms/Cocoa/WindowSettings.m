#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

#include "WindowSettings.h"
#include "Window.h"
#include "WindowPrivate.h"

#include "../../Window/Window.h"
#include "../../Window/WindowSettings.h"
#include "../../Window/Listener.h"
#include "../../Window/Event.h"
#include "../../Common/Log.h"

NH_API_RESULT nh_wsi_setCocoaWindowBackgroundColor(
    nh_wsi_CocoaWindow *Window_p, nh_css_Color Color)
{
    @autoreleasepool {
        NHWindow *window = (__bridge NHWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        NSColor *color = [NSColor colorWithRed:Color.r 
                                        green:Color.g 
                                         blue:Color.b 
                                        alpha:Color.a];
        window.backgroundColor = color;
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setCocoaWindowTitle(
    nh_wsi_CocoaWindow *Window_p, char *title_p)
{
    @autoreleasepool {
        NHWindow *window = (__bridge NHWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        [window setTitle:[NSString stringWithUTF8String:title_p]];
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setCocoaWindowDecorated(
    nh_wsi_CocoaWindow *Window_p, bool decorated)
{
    @autoreleasepool {
        NHWindow *window = (__bridge NHWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        NSWindowStyleMask styleMask = window.styleMask;
        if (decorated) {
            styleMask |= NSWindowStyleMaskTitled | 
                        NSWindowStyleMaskClosable | 
                        NSWindowStyleMaskMiniaturizable;
        } else {
            styleMask &= ~(NSWindowStyleMaskTitled | 
                          NSWindowStyleMaskClosable | 
                          NSWindowStyleMaskMiniaturizable);
        }
        [window setStyleMask:styleMask];
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setCocoaWindowState(
    nh_wsi_CocoaWindow *Window_p, bool *state_p)
{
    @autoreleasepool {
        NHWindow *window = (__bridge NHWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        if (state_p[NH_WSI_WINDOW_STATE_MAXIMIZED]) {
            // Store current frame for unmaximize
            Window_p->oldFrame.x = window.frame.origin.x;
            Window_p->oldFrame.y = window.frame.origin.y;
            Window_p->oldFrame.width = window.frame.size.width;
            Window_p->oldFrame.height = window.frame.size.height;

            // Maximize to screen
            [window setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
        } else {
            // Restore to old frame
            NSRect frame = NSMakeRect(Window_p->oldFrame.x, 
                                    Window_p->oldFrame.y,
                                    Window_p->oldFrame.width,
                                    Window_p->oldFrame.height);
            [window setFrame:frame display:YES];
        }
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setCocoaWindowMouseCursor(
    nh_wsi_CocoaWindow *Window_p, NH_WSI_CURSOR_E cursor)
{
    @autoreleasepool {
        NHWindow *window = (__bridge NHWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        NSCursor *nsCursor = nil;
        switch (cursor) {
            case NH_WSI_CURSOR_ARROW:
                nsCursor = [NSCursor arrowCursor];
                break;
            case NH_WSI_CURSOR_CROSSHAIR:
                nsCursor = [NSCursor crosshairCursor];
                break;
            case NH_WSI_CURSOR_HAND1:
            case NH_WSI_CURSOR_HAND2:
                nsCursor = [NSCursor pointingHandCursor];
                break;
            case NH_WSI_CURSOR_XTERM:
                nsCursor = [NSCursor IBeamCursor];
                break;
            case NH_WSI_CURSOR_SIZING:
                nsCursor = [NSCursor resizeLeftRightCursor];
                break;
            case NH_WSI_CURSOR_WATCH:
                nsCursor = [NSCursor operationNotAllowedCursor];
                break;
            case NH_WSI_CURSOR_TOP_LEFT_CORNER:
            case NH_WSI_CURSOR_TOP_RIGHT_CORNER:
            case NH_WSI_CURSOR_BOTTOM_LEFT_CORNER:
            case NH_WSI_CURSOR_BOTTOM_RIGHT_CORNER:
                nsCursor = [NSCursor resizeUpDownCursor];
                break;
            case NH_WSI_CURSOR_TOP_SIDE:
            case NH_WSI_CURSOR_BOTTOM_SIDE:
                nsCursor = [NSCursor resizeUpDownCursor];
                break;
            case NH_WSI_CURSOR_LEFT_SIDE:
            case NH_WSI_CURSOR_RIGHT_SIDE:
                nsCursor = [NSCursor resizeLeftRightCursor];
                break;
            default:
                nsCursor = [NSCursor arrowCursor];
                break;
        }

        [nsCursor set];
    }
    return NH_API_SUCCESS;
} 