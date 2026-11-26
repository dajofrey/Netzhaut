#import "../src/lib/nh-wsi/Platforms/Cocoa/Window.m"

#include <assert.h>

static int mouse_x, mouse_y, mouse_calls;
static NH_API_TRIGGER_E mouse_trigger;

void nh_wsi_sendMouseEvent(nh_wsi_Window *window, int x, int y,
    NH_API_TRIGGER_E trigger, NH_API_MOUSE_E type)
{
    assert(window && type == NH_API_MOUSE_SCROLL);
    mouse_x = x;
    mouse_y = y;
    mouse_trigger = trigger;
    ++mouse_calls;
}

@interface TestWindow : CustomWindow
@property CGFloat testScale;
@end
@implementation TestWindow
- (CGFloat)backingScaleFactor { return self.testScale ? self.testScale : 1; }
@end

@interface TestScrollEvent : NSEvent
@property NSPoint testLocation;
@property CGFloat testX;
@property CGFloat testY;
@end
@implementation TestScrollEvent
- (NSPoint)locationInWindow { return self.testLocation; }
- (BOOL)hasPreciseScrollingDeltas { return YES; }
- (CGFloat)scrollingDeltaX { return self.testX; }
- (CGFloat)scrollingDeltaY { return self.testY; }
@end

int main(void)
{
    @autoreleasepool {
        [NSApplication sharedApplication];
        nh_wsi_Window window = {0};
        TestWindow *native = [[TestWindow alloc] initWithWindow:&window
            contentRect:NSMakeRect(240, 180, 400, 300)
            styleMask:NSWindowStyleMaskTitled backing:NSBackingStoreBuffered defer:NO];
        native.releasedWhenClosed = NO;
        native.testScale = 2;
        TestScrollEvent *event = [TestScrollEvent new];
        event.testLocation = NSMakePoint(30, 250);
        event.testY = -1;
        [native scrollWheel:event];
        assert(mouse_calls == 1 && mouse_x == 60 && mouse_y == 100 &&
            mouse_trigger == NH_API_TRIGGER_DOWN);
        native.testScale = 1;
        event.testLocation = NSMakePoint(310, 25);
        event.testX = 2;
        event.testY = 1;
        [native scrollWheel:event];
        assert(mouse_calls == 2 && mouse_x == 310 && mouse_y == 275 &&
            mouse_trigger == NH_API_TRIGGER_RIGHT);
        [native close];
    }
    return 0;
}
