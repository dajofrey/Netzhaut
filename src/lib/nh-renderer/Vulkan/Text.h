#ifndef NH_RENDERER_VK_TEXT_H
#define NH_RENDERER_VK_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nh-css/Main/FragmentTree.h"

#include "../../nh-gfx/Vulkan/GPU.h"
#include "../../nh-gfx/Vulkan/Helper.h"

#include <stdint.h>

#endif

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    NH_API_RESULT nh_renderer_vk_createText(
        nh_vk_GPU *GPU_p, nh_css_Fragment *Fragment_p, nh_gfx_TextSegment *Segment_p, bool allocate, 
        float *vertices_p, uint32_t *indices_p, int index
    );

//    NH_API_RESULT nh_vk_updateText(
//        nh_Tab *Tab_p, nh_html_Node *Node_p
//    );

/** @} */

#endif 
