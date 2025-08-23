#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Carbon/Carbon.h>

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

// Private Objective-C interfaces and implementations

@interface CustomView : NSView
@property (nonatomic, assign) void *Window_p;
@end

@implementation CustomView

// Tell AppKit: yes, I want key events
- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)becomeFirstResponder {
    return YES;
}

- (BOOL)resignFirstResponder {
    return YES;
}

- (void)rightMouseDown:(NSEvent *)event
{
    // Convert from window coordinates into this view's local coordinates
    NSPoint winPoint   = [event locationInWindow];
    NSPoint localPoint = [self convertPoint:winPoint fromView:nil];

    // Flip Y if your C API expects origin at bottom-left
    localPoint.y = self.bounds.size.height - localPoint.y;

    // Use the window's backingScaleFactor for Retina support
    CGFloat scale = self.window.backingScaleFactor;
    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);

    // Use the pointer you injected into the view
    nh_wsi_sendMouseEvent(self.Window_p, px, py,
                          NH_API_TRIGGER_PRESS, NH_API_MOUSE_RIGHT);
}

- (void)rightMouseUp:(NSEvent *)event
{
    // Convert from window coordinates into this view's local coordinates
    NSPoint winPoint   = [event locationInWindow];
    NSPoint localPoint = [self convertPoint:winPoint fromView:nil];

    // Flip Y if your API expects origin at bottom-left
    localPoint.y = self.bounds.size.height - localPoint.y;

    // Use the window's backingScaleFactor for Retina displays
    CGFloat scale = self.window.backingScaleFactor;
    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);

    // Send mouse release event to your API
    nh_wsi_sendMouseEvent(self.Window_p, px, py,
                          NH_API_TRIGGER_RELEASE,
                          NH_API_MOUSE_RIGHT);
}

@end

@implementation CustomWindowDelegate

- (instancetype)initWithWindow:(nh_wsi_Window *)window
{
    self = [super init];
    if (self != nil) {
        Window_p = window;
    }
    return self;
}

- (void)windowDidResize:(NSNotification *)notification
{
    if (!Window_p) return;

    NSWindow *window = [notification object];
    NSView *view = [window contentView];

    NSRect bounds = view.bounds; // in points
    CGFloat scale = [window backingScaleFactor];

    int pointWidth  = (int)bounds.size.width;
    int pointHeight = (int)bounds.size.height;
    int pixelWidth  = (int)lround(bounds.size.width  * scale);
    int pixelHeight = (int)lround(bounds.size.height * scale);

    // Optional: window position in screen coordinates
    NSRect screenFrame = [window frame];

    nh_wsi_sendWindowEvent(Window_p,
        NH_API_WINDOW_CONFIGURE,
        (int)screenFrame.origin.x, (int)screenFrame.origin.y,
        pointWidth, pointHeight,
        pixelWidth, pixelHeight);
}

- (void)windowDidMove:(NSNotification *)notification
{
    if (!Window_p) return;
    
    NSWindow *window = [notification object];

    NSRect contentRect = [window contentRectForFrameRect:[window frame]];
    NSView *view = [window contentView];
    CGFloat scale = [window backingScaleFactor];

    int pointWidth  = (int)contentRect.size.width;
    int pointHeight = (int)contentRect.size.height;

    int pixelWidth  = (int)(contentRect.size.width  * scale);
    int pixelHeight = (int)(contentRect.size.height * scale);

    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_CONFIGURE, 
        contentRect.origin.x, contentRect.origin.y, 
        pointWidth, pointHeight,
        pixelWidth, pixelHeight);
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
//    if (!Window_p) return;
//    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_IN, 0, 0, 0, 0, 0, 0);
}

- (void)windowDidResignKey:(NSNotification *)notification
{
//    if (!Window_p) return;
//    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_OUT, 0, 0, 0, 0, 0, 0);
}

- (BOOL)windowShouldClose:(NSWindow *)sender
{
    if (!Window_p) return NO;
    // TODO: Send close event instead of exiting
    exit(0);
    return NO;
}

@end

@implementation CustomWindow

- (instancetype)initWithWindow:(nh_wsi_Window *)window 
                  contentRect:(NSRect)contentRect
                  styleMask:(NSWindowStyleMask)style
                  backing:(NSBackingStoreType)backingStoreType
                  defer:(BOOL)flag
{
    self = [super initWithContentRect:contentRect
                           styleMask:style
                             backing:backingStoreType
                               defer:flag];
    if (self != nil) {
        Window_p = window;
        self.backgroundColor = [NSColor whiteColor];
    }
    return self;
}

- (void)mouseDown:(NSEvent *)event
{
    NSPoint winPoint   = [event locationInWindow];
    NSPoint localPoint = [self.contentView convertPoint:winPoint fromView:nil];
    localPoint.y = self.contentView.bounds.size.height - localPoint.y;
    CGFloat scale = self.backingScaleFactor;
    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);
    nh_wsi_sendMouseEvent(Window_p, px, py, NH_API_TRIGGER_PRESS, NH_API_MOUSE_LEFT);
}

- (void)mouseUp:(NSEvent *)event
{
    NSPoint winPoint   = [event locationInWindow];
    NSPoint localPoint = [self.contentView convertPoint:winPoint fromView:nil];
    localPoint.y = self.contentView.bounds.size.height - localPoint.y;
    CGFloat scale = self.backingScaleFactor;
    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);
    nh_wsi_sendMouseEvent(Window_p, px, py, NH_API_TRIGGER_RELEASE, NH_API_MOUSE_LEFT);
}

- (void)mouseMoved:(NSEvent *)event
{
    NSPoint winPoint   = [event locationInWindow];
    NSPoint localPoint = [self.contentView convertPoint:winPoint fromView:nil];
    localPoint.y = self.contentView.bounds.size.height - localPoint.y;
    CGFloat scale = self.backingScaleFactor;
    int px = (int)lround(localPoint.x * scale);
    int py = (int)lround(localPoint.y * scale);
    nh_wsi_sendMouseEvent(Window_p, px, py, NH_API_TRIGGER_MOVE, NH_API_MOUSE_MOVE);
}

- (void)mouseDragged:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_LEFT);
}

- (void)rightMouseDragged:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_RIGHT);
}

- (void)otherMouseDown:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_PRESS, NH_API_MOUSE_MIDDLE);
}

- (void)otherMouseUp:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_RELEASE, NH_API_MOUSE_MIDDLE);
}

- (void)otherMouseDragged:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_MIDDLE);
}

- (void)scrollWheel:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];

    // Use precise deltas if available (trackpad / Magic Mouse)
    CGFloat dy = event.hasPreciseScrollingDeltas ? event.scrollingDeltaY : event.deltaY;
    CGFloat dx = event.hasPreciseScrollingDeltas ? event.scrollingDeltaX : event.deltaX;

    if (dy > 0) {
        nh_wsi_sendMouseEvent(Window_p, point.x, point.y,
                              NH_API_TRIGGER_UP, NH_API_MOUSE_SCROLL);
    } else if (dy < 0) {
        nh_wsi_sendMouseEvent(Window_p, point.x, point.y,
                              NH_API_TRIGGER_DOWN, NH_API_MOUSE_SCROLL);
    }
}

- (void)keyDown:(NSEvent *)event
{
    NH_ENCODING_UTF32 codepoint = 0;
 
    if (event.characters.length > 0) {
        codepoint = [event.characters characterAtIndex:0];
    }

    NH_API_TRIGGER_E trigger = NH_API_TRIGGER_PRESS;
    NH_API_MODIFIER_FLAG state = 0;
    NH_API_KEY_E special = 0;
   
    if (event.modifierFlags & NSEventModifierFlagShift) state |= NH_API_MODIFIER_SHIFT;
    if (event.modifierFlags & NSEventModifierFlagControl) state |= NH_API_MODIFIER_CONTROL;
    if (event.modifierFlags & NSEventModifierFlagOption) state |= NH_API_MODIFIER_MOD1;
    if (event.modifierFlags & NSEventModifierFlagCommand) state |= NH_API_MODIFIER_MOD4;

    switch ([event keyCode]) {
        case kVK_LeftArrow:  special = NH_API_KEY_KP_LEFT;  break;
        case kVK_UpArrow:    special = NH_API_KEY_KP_UP;    break;
        case kVK_RightArrow: special = NH_API_KEY_KP_RIGHT; break;
        case kVK_DownArrow:  special = NH_API_KEY_KP_DOWN;  break;
    }

    if (special) {codepoint = 0;}

    nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, state);
}

- (void)keyUp:(NSEvent *)event
{
    NH_ENCODING_UTF32 codepoint = 0;

    if (event.characters.length > 0) {
        codepoint = [event.characters characterAtIndex:0];
    }

    NH_API_TRIGGER_E trigger = NH_API_TRIGGER_RELEASE;
    NH_API_MODIFIER_FLAG state = 0;
    NH_API_KEY_E special = 0;
   
    if (event.modifierFlags & NSEventModifierFlagShift) state |= NH_API_MODIFIER_SHIFT;
    if (event.modifierFlags & NSEventModifierFlagControl) state |= NH_API_MODIFIER_CONTROL;
    if (event.modifierFlags & NSEventModifierFlagOption) state |= NH_API_MODIFIER_MOD1;
    if (event.modifierFlags & NSEventModifierFlagCommand) state |= NH_API_MODIFIER_MOD4;

    switch ([event keyCode]) {
        case kVK_LeftArrow:  special = NH_API_KEY_KP_LEFT;  break;
        case kVK_UpArrow:    special = NH_API_KEY_KP_UP;    break;
        case kVK_RightArrow: special = NH_API_KEY_KP_RIGHT; break;
        case kVK_DownArrow:  special = NH_API_KEY_KP_DOWN;  break;
    }

    if (special) {codepoint = 0;}

    nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, state);
}

- (void)flagsChanged:(NSEvent *)event
{
    NH_ENCODING_UTF32 codepoint = 0;
    NH_API_MODIFIER_FLAG state = 0;
    
    if (event.modifierFlags & NSEventModifierFlagShift) state |= NH_API_MODIFIER_SHIFT;
    if (event.modifierFlags & NSEventModifierFlagControl) state |= NH_API_MODIFIER_CONTROL;
    if (event.modifierFlags & NSEventModifierFlagOption) state |= NH_API_MODIFIER_MOD1;
    if (event.modifierFlags & NSEventModifierFlagCommand) state |= NH_API_MODIFIER_MOD4;
    
    NH_API_KEY_E special = 0;
    NH_API_TRIGGER_E trigger = NH_API_TRIGGER_PRESS;
 
    switch (event.keyCode) {
        case kVK_Control:
            if (event.modifierFlags & NSEventModifierFlagControl) {
                special = NH_API_KEY_CONTROL_L;
                trigger = NH_API_TRIGGER_PRESS;
            } else {
                special = NH_API_KEY_CONTROL_L;
                trigger = NH_API_TRIGGER_RELEASE;
            }
            break;

        case kVK_RightControl:
            if (event.modifierFlags & NSEventModifierFlagControl) {
                special = NH_API_KEY_CONTROL_R;
                trigger = NH_API_TRIGGER_PRESS;
            } else {
                special = NH_API_KEY_CONTROL_R;
                trigger = NH_API_TRIGGER_RELEASE;
            }
            break;

        case kVK_Option:
            if (event.modifierFlags & NSEventModifierFlagOption) {
                special = NH_API_KEY_OPTION_L;
                trigger = NH_API_TRIGGER_PRESS;
            } else {
                special = NH_API_KEY_OPTION_L;
                trigger = NH_API_TRIGGER_RELEASE;
            }
            break;

        case kVK_RightOption:
            if (event.modifierFlags & NSEventModifierFlagOption) {
                special = NH_API_KEY_OPTION_R;
                trigger = NH_API_TRIGGER_PRESS;
            } else {
                special = NH_API_KEY_OPTION_R;
                trigger = NH_API_TRIGGER_RELEASE;
            }
            break;

        case kVK_Command:
            if (event.modifierFlags & NSEventModifierFlagCommand) {
                special = NH_API_KEY_COMMAND_L;
                trigger = NH_API_TRIGGER_PRESS;
            } else {
                special = NH_API_KEY_COMMAND_L;
                trigger = NH_API_TRIGGER_RELEASE;
            }
            break;

        case kVK_RightCommand:
            if (event.modifierFlags & NSEventModifierFlagCommand) {
                special = NH_API_KEY_COMMAND_R;
                trigger = NH_API_TRIGGER_PRESS;
            } else {
                special = NH_API_KEY_COMMAND_R;
                trigger = NH_API_TRIGGER_RELEASE;
            }
            break;
    }

    nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, state);
}

@end

// C interface implementations

NH_API_RESULT nh_wsi_createCocoaWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p)
{
    @autoreleasepool {
        // Create window delegate
        CustomWindowDelegate *delegate = [[CustomWindowDelegate alloc] initWithWindow:(nh_wsi_Window*)Window_p];
        
        // Convert coordinates from bottom-left to top-left origin
        NSRect screenRect = [[NSScreen mainScreen] frame];
        NSRect contentRect = NSMakeRect(Config.Position.x,
                                      screenRect.size.height - Config.Position.y - Config.Size.height,
                                      Config.Size.width,
                                      Config.Size.height);
        
        // Set window style
        NSWindowStyleMask styleMask = NSWindowStyleMaskTitled |
                                    NSWindowStyleMaskClosable |
                                    NSWindowStyleMaskMiniaturizable;
        
        if (Config.resizable) {
            styleMask |= NSWindowStyleMaskResizable;
        }
        
        // Create window
        CustomWindow *window = [[CustomWindow alloc] initWithWindow:(nh_wsi_Window*)Window_p
                                              contentRect:contentRect
                                              styleMask:styleMask
                                              backing:NSBackingStoreBuffered
                                              defer:NO];
        
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }
        
        [window setDelegate:delegate];
        [window setTitle:[NSString stringWithUTF8String:Config.title_p]];
        [window setAcceptsMouseMovedEvents:YES];
        
        // Replace default contentView with custom one
        CustomView *view = [[CustomView alloc] initWithFrame:contentRect];
        view.Window_p = Window_p;
        [window setContentView:view];
        
        // Make sure it gets keyboard focus
        [window makeFirstResponder:view];

//        // Create metal layer for rendering
//        CAMetalLayer *layer = [CAMetalLayer layer];
//        [window.contentView setWantsLayer:YES];
//        [window.contentView setLayer:layer];
        
        // Store references
        Window_p->Cocoa.Handle = (__bridge_retained void*)window;
        Window_p->Cocoa.Delegate = (__bridge_retained void*)delegate;
//        Window_p->Cocoa.Layer = (__bridge_retained void*)layer;
        
        // Show window
        [window makeKeyAndOrderFront:nil];
    }
    
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_destroyCocoaWindow(
    nh_wsi_CocoaWindow *Window_p)
{
    @autoreleasepool {
        if (Window_p->Handle) {
            CustomWindow *window = (__bridge_transfer CustomWindow*)Window_p->Handle;
            [window setDelegate:nil];
            [window close];
            Window_p->Handle = NULL;
        }
        
        if (Window_p->Delegate) {
            CustomWindowDelegate *delegate = (__bridge_transfer CustomWindowDelegate*)Window_p->Delegate;
            Window_p->Delegate = NULL;
        }
        
        if (Window_p->Layer) {
            CAMetalLayer *layer = (__bridge_transfer CAMetalLayer*)Window_p->Layer;
            Window_p->Layer = NULL;
        }
    }
    
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setCocoaClipboardOwner(
    nh_wsi_CocoaWindow *Window_p)
{
    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        [pasteboard clearContents];
    }
    return NH_API_SUCCESS;
}

bool nh_wsi_isCocoaClipboardOwner(
    nh_wsi_CocoaWindow *Window_p)
{
    // On macOS, we don't need to track clipboard ownership
    return true;
}

NH_API_RESULT nh_wsi_requestCocoaClipboardConversion(
    nh_wsi_CocoaWindow *Window_p)
{
    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSString *string = [pasteboard stringForType:NSPasteboardTypeString];
        if (string) {
            const char *utf8 = [string UTF8String];
            nh_wsi_setClipboard(utf8, strlen(utf8), false);
        }
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_getCocoaInput(
    nh_wsi_Window *Window_p, bool *idle_p)
{
    @autoreleasepool {
        NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES];
        if (event) {
            [NSApp sendEvent:event];
            *idle_p = false;
        } else {
            *idle_p = true;
        }
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_moveCocoaWindow(
    nh_wsi_CocoaWindow *Window_p)
{
    @autoreleasepool {
        CustomWindow *window = (__bridge CustomWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        // Get current mouse position
        NSPoint mouseLocation = [NSEvent mouseLocation];
        
        // Convert coordinates from bottom-left to top-left origin
        NSRect screenRect = [[NSScreen mainScreen] frame];
        NSRect frame = window.frame;
        frame.origin.x = mouseLocation.x;
        frame.origin.y = screenRect.size.height - mouseLocation.y - frame.size.height;
        
        [window setFrame:frame display:YES];
    }
    return NH_API_SUCCESS;
} 

NH_API_RESULT nh_wsi_getCocoaWindowSize(
    nh_wsi_CocoaWindow *Window_p, int *x_p, int *y_p)
{
    if (!Window_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    @autoreleasepool {
        CustomWindow *window = (__bridge CustomWindow*)Window_p->Handle;
        if (!window) {
            return NH_API_ERROR_BAD_STATE;
        }

        NSRect frame = [window frame];

        // Get backing scale factor (1.0 for non-Retina, 2.0 for Retina, etc.)
        CGFloat scaleFactor = [window backingScaleFactor];
        *x_p = frame.size.width * scaleFactor;
        *y_p = frame.size.height * scaleFactor;
    }

    return NH_API_SUCCESS;
}
