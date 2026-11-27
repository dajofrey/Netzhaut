#import <Cocoa/Cocoa.h>

#include "../src/lib/nh-api/nh-core.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

extern NH_API_RESULT nh_wsi_waitCocoaInput(int milliseconds, bool *idle);
extern void nh_wsi_wakeCocoa(void);

static int dispatched;

@interface TestApplication : NSApplication
@end
@implementation TestApplication
- (void)sendEvent:(NSEvent *)event
{
    if (event.type == NSEventTypeApplicationDefined && event.subtype == 42) ++dispatched;
    [super sendEvent:event];
}
@end

static double seconds(void)
{
    struct timespec time;
    assert(!clock_gettime(CLOCK_MONOTONIC, &time));
    return time.tv_sec + time.tv_nsec / 1000000000.0;
}

static void *wakeLater(void *unused)
{
    (void)unused;
    struct timespec delay = {0, 50000000};
    nanosleep(&delay, NULL);
    for (int i = 0; i < 100; ++i) nh_wsi_wakeCocoa();
    return NULL;
}

int main(void)
{
    @autoreleasepool {
        [TestApplication sharedApplication];
        bool idle;
        do {
            assert(nh_wsi_waitCocoaInput(0, &idle) == NH_API_SUCCESS);
        } while (!idle);
        NSEvent *event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
            location:NSZeroPoint modifierFlags:0 timestamp:0 windowNumber:0 context:nil
            subtype:42 data1:0 data2:0];
        [NSApp postEvent:event atStart:NO];
        assert(nh_wsi_waitCocoaInput(1000, &idle) == NH_API_SUCCESS && !idle);
        assert(dispatched == 1);
        pthread_t thread;
        assert(!pthread_create(&thread, NULL, wakeLater, NULL));
        double start = seconds();
        assert(nh_wsi_waitCocoaInput(2000, &idle) == NH_API_SUCCESS && !idle);
        assert(seconds() - start < 1.0);
        assert(!pthread_join(thread, NULL));
        start = seconds();
        assert(nh_wsi_waitCocoaInput(50, &idle) == NH_API_SUCCESS && idle);
        assert(seconds() - start >= 0.035);
        puts("Cocoa event wake and timeout tests passed");
    }
    return 0;
}
