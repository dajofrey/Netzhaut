#ifndef NH_RENDERER_VK_RECORD_H
#define NH_RENDERER_VK_RECORD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-css/Main/FragmentTree.h"

#endif

/** @addtogroup lib_nh-renderer_functions 
 *  @{
 */

    void nh_renderer_vk_recordFragmentTree(
        nh_css_FragmentTree *Tree_p, nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif
