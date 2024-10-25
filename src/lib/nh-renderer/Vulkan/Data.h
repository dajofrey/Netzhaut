#ifndef NH_RENDERER_VULKAN_DATA_H
#define NH_RENDERER_VULKAN_DATA_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Base/Viewport.h"

// STRUCTS =========================================================================================

typedef struct nh_renderer_VulkanData{
    nh_core_Array Buffers;           
    nh_core_Array Uniforms;          
    nh_core_Array Descriptors;
} nh_renderer_VulkanData;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createFragmentTreeVulkanData(
    nh_css_FragmentTree *Tree_p, nh_gfx_Viewport *Viewport_p
);

#endif // NH_RENDERER_VULKAN_DATA_H 
