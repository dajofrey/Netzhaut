#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/EAGLDrawable.h>

// Matches the extern declaration in Render.c
int nh_gfx_makeOpenGLContextCurrentIOS(
    void* Context_p)
{
    EAGLContext *context = (__bridge EAGLContext *)Context_p;
    
    if (!context) {
        return -1; // Bad state
    }
    
    if (![EAGLContext setCurrentContext:context]) {
        return -1; // Failed to set
    }
    
    return 0; // Success
}

void nh_gfx_flushOpenGLDrawableIOS(
    void* Context_p, 
    unsigned int Framebuffer, 
    unsigned int ColorRenderbuffer)
{
    EAGLContext *context = (__bridge EAGLContext *)Context_p;
    if (context) {
        // Ensure our context is current on this thread execution block
        [EAGLContext setCurrentContext:context];

        // Restore the native iOS Framebuffer binding that the command buffer altered
        glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);

        // CRITICAL: Re-bind the exact color renderbuffer connected to the CAEAGLLayer.
        // If this isn't bound to GL_RENDERBUFFER right now, presentRenderbuffer fails.
        glBindRenderbuffer(GL_RENDERBUFFER, ColorRenderbuffer);

        // Flash the validated buffer to the Core Animation layer
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
}
