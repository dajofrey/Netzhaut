#include "Viewport.h"
#include <stdlib.h>

NH_API_RESULT nh_gfx_createMetalViewport(
    nh_gfx_Viewport *Viewport_p)
{
    Viewport_p->Metal.commandBuffer = [dev->commandQueue commandBuffer];
    return NH_API_SUCCESS;
}

void nh_gfx_destroyMetalViewport(
    nh_gfx_Viewport *Viewport_p)
{
    // No dynamic Metal state to clean up yet
} 
