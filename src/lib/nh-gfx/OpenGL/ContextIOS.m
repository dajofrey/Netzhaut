// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ContextIOS.h"

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-wsi/Window/Window.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES3/gl.h>

// FUNCTIONS ======================================================================================

NH_API_RESULT nh_gfx_createOpenGLIOSContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p)
{
    // Retrieve the UIView from the window system interface
    UIView *view = (__bridge UIView *)Window_p->IOS.Handle;
    if (!view) {
        return NH_API_ERROR_BAD_STATE;
    }

    // 1. Configure the CAEAGLLayer
    // The view's backing layer MUST be a CAEAGLLayer. 
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)view.layer;
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = @{
        kEAGLDrawablePropertyRetainedBacking: @NO,
        kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8
    };

    // 2. Create the EAGL Context (Requesting OpenGL ES 3.0)
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!context || ![EAGLContext setCurrentContext:context]) {
        return NH_API_ERROR_BAD_STATE;
    }

    // 3. Generate and bind the Framebuffer and Renderbuffer
    GLuint framebuffer, colorRenderbuffer, depthRenderbuffer;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);

    // Link the renderbuffer storage to the CAEAGLLayer
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

    // 4. Generate the Depth Buffer (Matching your macOS 24-bit depth setup)
    GLint width, height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    // Verify framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return NH_API_ERROR_BAD_STATE;
    }

    // Save to the surface struct
    Surface_p->Context_p = (__bridge void *)context;
    Surface_p->OpenGLContext_p = (__bridge_retained void *)context;
    
    // NOTE: You must store these GLuint handles in your nh_gfx_OpenGLSurface struct 
    // so you can delete and reallocate them during resizing (device rotation).
    // Surface_p->Framebuffer = framebuffer;
    // Surface_p->ColorRenderbuffer = colorRenderbuffer;
    // Surface_p->DepthRenderbuffer = depthRenderbuffer;

    return NH_API_SUCCESS;
}

void nh_gfx_updateOpenGLIOSContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p)
{
    EAGLContext *context = (__bridge EAGLContext *)Surface_p->OpenGLContext_p;
    [EAGLContext setCurrentContext:context];

    // To handle resize on iOS, you must delete the old renderbuffers,
    // re-bind them, and call [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer]
    // again to update the dimensions.
}

NH_API_RESULT nh_gfx_destroyOpenGLIOSContext(
    nh_gfx_OpenGLSurface *Surface_p)
{
    if (Surface_p->OpenGLContext_p) {
        EAGLContext *context = (__bridge_transfer EAGLContext *)Surface_p->OpenGLContext_p;
        
        if ([EAGLContext currentContext] == context) {
            [EAGLContext setCurrentContext:nil];
        }
        
        // Clean up FBOs here using glDeleteFramebuffers / glDeleteRenderbuffers
        
        Surface_p->Context_p = NULL;
        Surface_p->OpenGLContext_p = NULL;
    }

    return NH_API_SUCCESS;
}
