// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Render.h"

#include "../Base/Viewport.h"
#include "../Base/Surface.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/Time.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-wsi/Window/Window.h"
#include "../../nh-wsi/Platforms/X11/Init.h"

// RENDER ==========================================================================================

NH_API_RESULT nh_opengl_render(
    nh_gfx_Surface *Surface_p, nh_List *SortedViewports_p)
{
NH_GFX_BEGIN()

    glXMakeCurrent(Surface_p->Window_p->X11.Common_p->Display_p, Surface_p->Window_p->X11.Handle, 
        Surface_p->OpenGL.Context_p);

    for (int i = 0; i < SortedViewports_p->size; ++i) {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
        NH_GFX_CHECK(nh_opengl_executeCommandBuffer(Viewport_p->OpenGL.CommandBuffer_p))
        NH_GFX_CHECK(nh_opengl_freeCommandBuffer(Viewport_p->OpenGL.CommandBuffer_p))
    }

    glXSwapBuffers(Surface_p->Window_p->X11.Common_p->Display_p, Surface_p->Window_p->X11.Handle);

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

