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
    void* Context_p)
{
    EAGLContext *context = (__bridge EAGLContext *)Context_p;
    if (context) {
        // In iOS OpenGL ES, you typically present the renderbuffer bound to GL_RENDERBUFFER
        // Ensure you have bound your color renderbuffer before calling this in your pipeline
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
}
