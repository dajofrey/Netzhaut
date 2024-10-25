#ifndef NH_RENDERER_VULKAN_TEXT_H
#define NH_RENDERER_VULKAN_TEXT_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Vulkan/GPU.h"
#include "../../nh-gfx/Vulkan/Helper.h"

#include <stdint.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanText(
    nh_gfx_VulkanGPU *GPU_p, nh_css_Fragment *Fragment_p, nh_gfx_TextSegment *Segment_p, bool allocate, 
    float *vertices_p, uint32_t *indices_p, int index
);

//    NH_API_RESULT nh_vk_updateText(
//        nh_Tab *Tab_p, nh_html_Node *Node_p
//    );

#endif // NH_RENDERER_VULKAN_TEXT_H 
