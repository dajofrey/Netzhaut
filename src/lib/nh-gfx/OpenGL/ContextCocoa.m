// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ContextCocoa.h"

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-wsi/Window/Window.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>
#import <OpenGL/OpenGL.h>

// FUNCTIONS ======================================================================================

NH_API_RESULT nh_gfx_createOpenGLCocoaContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p)
{
    NSWindow *window = (__bridge NSWindow *)Window_p->Cocoa.Handle;

    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
        NSOpenGLPFAColorSize,     24,
        NSOpenGLPFAAlphaSize,     8,
        NSOpenGLPFADepthSize,     24,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        0
    };

    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pixelFormat) {
        return NH_API_ERROR_BAD_STATE;
    }

    NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    if (!context) {
        return NH_API_ERROR_BAD_STATE;
    }

    // Attach to the content view
    NSView *view = [window contentView];
    if (!view) {
        return NH_API_ERROR_BAD_STATE;
    }

    [context setView:view];
    [context makeCurrentContext];

    // Enable vsync (optional)
    GLint swapInterval = 1;
    [context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

    // Force buffer allocation to avoid lazy drawable crashes
    [context update];

    CGLContextObj cglContext = [context CGLContextObj];
    Surface_p->Context_p = cglContext;
    Surface_p->PixelFormat_p = (__bridge_retained void *)pixelFormat;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_destroyOpenGLCocoaContext(
    nh_gfx_OpenGLSurface *Surface_p)
{
    if (Surface_p->Context_p) {
        [Surface_p->Context_p clearDrawable];
        Surface_p->Context_p = nil;
    }
    Surface_p->PixelFormat_p = nil;

    return NH_API_SUCCESS;
}
