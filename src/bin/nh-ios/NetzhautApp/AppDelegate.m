#import "AppDelegate.h"

#include "nh-api/nh-api.h"

static void handleInput(nh_api_Window *Window_p, nh_api_WSIEvent Event)
{
    (void)Window_p;
    (void)Event;
}

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    (void)application;
    (void)launchOptions;

    if (nh_api_initialize(NULL, NULL, 0) != NH_API_SUCCESS) {
        return NO;
    }

    nh_api_Window *window = nh_api_createWindow(NULL, NULL);
    if (!window) {
        nh_api_terminate();
        return NO;
    }

    nh_api_setWindowEventListener(window, handleInput);

    self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(tick:)];
    [self.displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];

    return YES;
}

- (void)tick:(CADisplayLink *)link
{
    (void)link;
    int result = nh_api_run();
    if (result == -1 || !nh_api_keepRunning()) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        nh_api_terminate();
    }
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    (void)application;
    [self.displayLink invalidate];
    self.displayLink = nil;
    nh_api_terminate();
}

@end
