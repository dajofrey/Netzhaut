#ifndef NH_RENDERER_VK_DATA_H
#define NH_RENDERER_VK_DATA_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Base/Viewport.h"

#endif

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    typedef struct nh_renderer_vk_Data{
        nh_Array Buffers;           
        nh_Array Uniforms;          
        nh_Array Descriptors;
    } nh_renderer_vk_Data;

/** @} */

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    nh_renderer_vk_Data nh_renderer_vk_initData(
    );

    NH_API_RESULT nh_renderer_vk_createFragmentTreeData(
        nh_css_FragmentTree *Tree_p, nh_gfx_Viewport *Viewport_p
    );

//    void nh_renderer_vk_destroyRenderResources(
//        nh_gfx_Viewport *Viewport_p, nh_renderer_FormattingNodeFragment *Fragment_p
//    );

/** @} */

#endif 
