#import <Cocoa/Cocoa.h>
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

// Private Objective-C interfaces and implementations

@implementation NHWindowDelegate

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
    NSRect contentRect = [window contentRectForFrameRect:[window frame]];
    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_CONFIGURE, 
        contentRect.origin.x, contentRect.origin.y, 
        contentRect.size.width, contentRect.size.height);
}

- (void)windowDidMove:(NSNotification *)notification
{
    if (!Window_p) return;
    
    NSWindow *window = [notification object];
    NSRect contentRect = [window contentRectForFrameRect:[window frame]];
    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_CONFIGURE, 
        contentRect.origin.x, contentRect.origin.y, 
        contentRect.size.width, contentRect.size.height);
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
    if (!Window_p) return;
    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_IN, 0, 0, 0, 0);
}

- (void)windowDidResignKey:(NSNotification *)notification
{
    if (!Window_p) return;
    nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_OUT, 0, 0, 0, 0);
}

- (BOOL)windowShouldClose:(NSWindow *)sender
{
    if (!Window_p) return NO;
    // TODO: Send close event instead of exiting
    exit(0);
    return NO;
}

@end


@implementation NHWindow

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
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_PRESS, NH_API_MOUSE_LEFT);
}

- (void)mouseUp:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_RELEASE, NH_API_MOUSE_LEFT);
}

- (void)mouseMoved:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_MOVE);
}

- (void)mouseDragged:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_LEFT);
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_PRESS, NH_API_MOUSE_RIGHT);
}

- (void)rightMouseUp:(NSEvent *)event
{
    NSPoint point = [self convertPointFromScreen:[event locationInWindow]];
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_RELEASE, NH_API_MOUSE_RIGHT);
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
    nh_wsi_sendMouseEvent(Window_p, point.x, point.y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_SCROLL);
}

- (void)keyDown:(NSEvent *)event
{
    NH_ENCODING_UTF32 codepoint = [event.characters characterAtIndex:0];
    NH_API_KEY_E special = 0;
    NH_API_TRIGGER_E trigger = NH_API_TRIGGER_PRESS;
    NH_API_MODIFIER_FLAG state = 0;
    
    if (event.modifierFlags & NSEventModifierFlagShift) state |= NH_API_MODIFIER_SHIFT;
    if (event.modifierFlags & NSEventModifierFlagControl) state |= NH_API_MODIFIER_CONTROL;
    if (event.modifierFlags & NSEventModifierFlagOption) state |= NH_API_MODIFIER_MOD1;
    if (event.modifierFlags & NSEventModifierFlagCommand) state |= NH_API_MODIFIER_MOD4;
    
    nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, state);
}

- (void)keyUp:(NSEvent *)event
{
    NH_ENCODING_UTF32 codepoint = [event.characters characterAtIndex:0];
    NH_API_KEY_E special = 0;
    NH_API_TRIGGER_E trigger = NH_API_TRIGGER_RELEASE;
    NH_API_MODIFIER_FLAG state = 0;
    
    if (event.modifierFlags & NSEventModifierFlagShift) state |= NH_API_MODIFIER_SHIFT;
    if (event.modifierFlags & NSEventModifierFlagControl) state |= NH_API_MODIFIER_CONTROL;
    if (event.modifierFlags & NSEventModifierFlagOption) state |= NH_API_MODIFIER_MOD1;
    if (event.modifierFlags & NSEventModifierFlagCommand) state |= NH_API_MODIFIER_MOD4;
    
    nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, state);
}

- (void)flagsChanged:(NSEvent *)event
{
    NH_ENCODING_UTF32 codepoint = 0;
    NH_API_KEY_E special = 0;
    NH_API_TRIGGER_E trigger = NH_API_TRIGGER_PRESS;
    NH_API_MODIFIER_FLAG state = 0;
    
    if (event.modifierFlags & NSEventModifierFlagShift) state |= NH_API_MODIFIER_SHIFT;
    if (event.modifierFlags & NSEventModifierFlagControl) state |= NH_API_MODIFIER_CONTROL;
    if (event.modifierFlags & NSEventModifierFlagOption) state |= NH_API_MODIFIER_MOD1;
    if (event.modifierFlags & NSEventModifierFlagCommand) state |= NH_API_MODIFIER_MOD4;
    
    nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, state);
}

@end

// C interface implementations

NH_API_RESULT nh_wsi_createCocoaWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p)
{
    @autoreleasepool {
        // Create window delegate
        NHWindowDelegate *delegate = [[NHWindowDelegate alloc] initWithWindow:(nh_wsi_Window*)Window_p];
        
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
        NHWindow *window = [[NHWindow alloc] initWithWindow:(nh_wsi_Window*)Window_p
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
        
        // Create metal layer for rendering
        CAMetalLayer *layer = [CAMetalLayer layer];
        [window.contentView setWantsLayer:YES];
        [window.contentView setLayer:layer];
        
        // Store references
        Window_p->Cocoa.Handle = (__bridge_retained void*)window;
        Window_p->Cocoa.Delegate = (__bridge_retained void*)delegate;
        Window_p->Cocoa.Layer = (__bridge_retained void*)layer;
        
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
            NHWindow *window = (__bridge_transfer NHWindow*)Window_p->Handle;
            [window setDelegate:nil];
            [window close];
            Window_p->Handle = NULL;
        }
        
        if (Window_p->Delegate) {
            NHWindowDelegate *delegate = (__bridge_transfer NHWindowDelegate*)Window_p->Delegate;
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
        NHWindow *window = (__bridge NHWindow*)Window_p->Handle;
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
