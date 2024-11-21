#ifndef NH_OPENGL_RENDER_H
#define NH_OPENGL_RENDER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Surface.h"
#include "../Base/Surface.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_renderOpenGL(
        nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p
    );

/** @} */

#endif
