#ifndef NH_WSI_COCOA_WINDOW_PRIVATE_H
#define NH_WSI_COCOA_WINDOW_PRIVATE_H

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

#include "Window.h"

@interface NHWindowDelegate : NSObject <NSWindowDelegate>
{
    nh_wsi_Window *Window_p;
}
- (instancetype)initWithWindow:(nh_wsi_Window *)window;
@end

@interface NHWindow : NSWindow
{
    nh_wsi_Window *Window_p;
}
@property (nonatomic, strong) NSColor *backgroundColor;
- (instancetype)initWithWindow:(nh_wsi_Window *)window 
                  contentRect:(NSRect)contentRect
                  styleMask:(NSWindowStyleMask)style
                  backing:(NSBackingStoreType)backingStoreType
                  defer:(BOOL)flag;
@end

#endif // NH_WSI_COCOA_WINDOW_PRIVATE_H 