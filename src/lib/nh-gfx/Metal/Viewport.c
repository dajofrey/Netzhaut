#include "Viewport.h"
#include <stdlib.h>

NH_API_RESULT nh_gfx_createMetalViewport(struct nh_gfx_Viewport *Viewport_p) {
    if (!Viewport_p) return NH_API_ERROR_BAD_STATE;
    // Initialize Metal-specific viewport state from generic settings
    Viewport_p->Metal.x = (float)Viewport_p->Settings.Position.x;
    Viewport_p->Metal.y = (float)Viewport_p->Settings.Position.y;
    Viewport_p->Metal.width  = (float)Viewport_p->Settings.Size.width;
    Viewport_p->Metal.height = (float)Viewport_p->Settings.Size.height;
    Viewport_p->Metal.znear = 0.0f;
    Viewport_p->Metal.zfar  = 1.0f;
    // If you need to set up more Metal state, do it here
    return NH_API_SUCCESS;
}

void nh_gfx_destroyMetalViewport(struct nh_gfx_Viewport *Viewport_p) {
    // No dynamic Metal state to clean up yet
} 