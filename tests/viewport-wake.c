#include "../src/lib/nh-gfx/Base/Viewport.h"
#include "../src/lib/nh-core/System/Thread.h"

#include <assert.h>
#include <stdio.h>

static nh_gfx_Surface surface;
static nh_core_Workload workload;
static int wakeCount;
static int executeCount;
static bool failWake;

void nh_core_debug(char *message)
{
    fprintf(stderr, "%s\n", message);
}

NH_API_RESULT nh_gfx_endOpenGLRecording(nh_gfx_Viewport *viewport)
{
    assert(viewport->Surface_p == &surface);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_endMetalRecording(nh_gfx_Viewport *viewport)
{
    return nh_gfx_endOpenGLRecording(viewport);
}

nh_core_Workload *nh_core_getWorkload(void *handle)
{
    assert(handle == &surface);
    return &workload;
}

NH_API_RESULT nh_core_wakeWorkload(nh_core_Workload *target)
{
    assert(target == &workload);
    assert(surface.renderRequests > 0);
    ++wakeCount;
    return failWake ? NH_API_ERROR_BAD_STATE : NH_API_SUCCESS;
}

NH_API_RESULT nh_core_executeWorkload(void *handle)
{
    assert(handle == &surface);
    ++executeCount;
    return NH_API_SUCCESS;
}

int main(void)
{
    surface.api = NH_GFX_API_OPENGL;
    nh_gfx_Viewport viewport = {.Surface_p = &surface};
    assert(nh_gfx_endRecording(&viewport, false) == NH_API_SUCCESS);
    assert(wakeCount == 1 && executeCount == 0 && viewport.Sync.render);
    assert(nh_gfx_endRecording(&viewport, true) == NH_API_SUCCESS);
    assert(wakeCount == 1 && executeCount == 1);
    failWake = true;
    assert(nh_gfx_endRecording(&viewport, false) == NH_API_ERROR_BAD_STATE);
    puts("viewport wake tests passed");
    return 0;
}
