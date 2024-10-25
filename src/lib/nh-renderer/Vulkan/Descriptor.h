#ifndef NH_RENDERER_VULKAN_DESCRIPTOR_H
#define NH_RENDERER_VULKAN_DESCRIPTOR_H

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

typedef enum NH_RENDERER_VK_DESCRIPTOR {
    NH_RENDERER_VK_DESCRIPTOR_BACKGROUND,       
    NH_RENDERER_VK_DESCRIPTOR_TOP_BORDER,       
    NH_RENDERER_VK_DESCRIPTOR_RIGHT_BORDER,     
    NH_RENDERER_VK_DESCRIPTOR_BOTTOM_BORDER,    
    NH_RENDERER_VK_DESCRIPTOR_LEFT_BORDER,      
    NH_RENDERER_VK_DESCRIPTOR_TEXT_SDF,         
    NH_RENDERER_VK_DESCRIPTOR_IMAGE,         
    NH_RENDERER_VK_DESCRIPTOR_BACKGROUND_IMAGE,
} NH_RENDERER_VK_DESCRIPTOR;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanColorDescriptor(
    nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p, NH_RENDERER_VK_DESCRIPTOR type
);

void nh_renderer_destroyVulkanDescriptorArray(
    nh_gfx_VulkanDriver *Driver_p, nh_renderer_VulkanData *Data_p
);

nh_gfx_VulkanDescriptorSet *nh_renderer_getVulkanDescriptor(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_DESCRIPTOR type
);

nh_gfx_VulkanDescriptorSet *nh_renderer_getVulkanDescriptorFromIndex(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_DESCRIPTOR type, unsigned int index
);

#endif // NH_RENDERER_VULKAN_DESCRIPTOR_H
