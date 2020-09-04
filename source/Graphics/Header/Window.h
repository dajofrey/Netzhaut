#ifndef NH_GFX_WINDOW_H
#define NH_GFX_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/Window.h"

#endif

/** @addtogroup GraphicsFunctions Functions
 *  \ingroup Graphics
 *  @{
 */

    NH_RESULT Nh_Gfx_createWindow(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Gfx_destroyWindow(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Gfx_prepareRendering(
        Nh_Window *Window_p
    );
    
    NH_RESULT Nh_Gfx_render(
        Nh_Window *Window_p
    );
    
    NH_RESULT Nh_Gfx_resize(
        Nh_Window *Window_p
    );
    
/** @} */

#endif 
