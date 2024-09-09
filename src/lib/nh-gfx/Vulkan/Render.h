#ifndef NH_VK_RENDER_H
#define NH_VK_RENDER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "GPU.h"
#include "Surface.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_vk_prepareRendering(
        nh_vk_Surface *Surface_p
    );

    NH_GFX_RESULT nh_vk_render(
        nh_gfx_Surface *Surface_p, nh_List *SortedViewports_p
    );

/** @} */

#endif
