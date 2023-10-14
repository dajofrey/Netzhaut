#ifndef NH_RENDERER_VK_TEXT_H
#define NH_RENDERER_VK_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nhcss/Main/FragmentTree.h"

#include "../../nhgfx/Vulkan/GPU.h"
#include "../../nhgfx/Vulkan/Helper.h"

#include <stdint.h>

#endif

/** @addtogroup lib_nhrenderer_functions
 *  @{
 */

    NH_RENDERER_RESULT nh_renderer_vk_createText(
        nh_vk_GPU *GPU_p, nh_css_Fragment *Fragment_p, nh_gfx_TextSegment *Segment_p, NH_BOOL allocate, 
        float *vertices_p, uint32_t *indices_p, int index
    );

//    NH_CORE_RESULT nh_vk_updateText(
//        nh_Tab *Tab_p, nh_html_Node *Node_p
//    );

/** @} */

#endif 
