#ifndef NH_RENDERER_VULKAN_UNIFORM_H
#define NH_RENDERER_VULKAN_UNIFORM_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Data.h"

#include "../Common/Includes.h"

#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Vulkan/Driver.h"
#include "../../nh-gfx/Vulkan/Helper.h"

// ENUMS ===========================================================================================

typedef enum NH_RENDERER_VK_UNIFORM {
    NH_RENDERER_VK_UNIFORM_BACKGROUND,       
    NH_RENDERER_VK_UNIFORM_TOP_BORDER,       
    NH_RENDERER_VK_UNIFORM_RIGHT_BORDER,     
    NH_RENDERER_VK_UNIFORM_BOTTOM_BORDER,    
    NH_RENDERER_VK_UNIFORM_LEFT_BORDER,
    NH_RENDERER_VK_UNIFORM_TEXT_SDF_VS,
    NH_RENDERER_VK_UNIFORM_TEXT_SDF_FS,
    NH_RENDERER_VK_UNIFORM_IMAGE,
    NH_RENDERER_VK_UNIFORM_BACKGROUND_IMAGE,
    NH_RENDERER_VK_UNIFORM_COUNT,
} NH_RENDERER_VK_UNIFORM;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanColorUniform(
    nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p, NH_RENDERER_VK_UNIFORM type
);

void nh_renderer_destroyVulkanUniformArray(
    nh_gfx_VulkanDriver *Driver_p, nh_renderer_VulkanData *Data_p
);

nh_gfx_VulkanBuffer *nh_renderer_getVulkanUniform(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_UNIFORM type
);

nh_gfx_VulkanBuffer *nh_renderer_getVulkanUniformFromIndex(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_UNIFORM type, unsigned int index
);

#endif // NH_RENDERER_VULKAN_UNIFORM_H
