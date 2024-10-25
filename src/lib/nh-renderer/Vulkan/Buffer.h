#ifndef NH_RENDERER_VULKAN_BUFFER_H
#define NH_RENDERER_VULKAN_BUFFER_H

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

typedef enum NH_RENDERER_VULKAN_BUFFER {
    NH_RENDERER_VULKAN_BUFFER_TEXT_SDF,               
    NH_RENDERER_VULKAN_BUFFER_TEXT_SDF_INDEX,         
    NH_RENDERER_VULKAN_BUFFER_BACKGROUND,   
    NH_RENDERER_VULKAN_BUFFER_TOP_BORDER_VERTICES,    
    NH_RENDERER_VULKAN_BUFFER_RIGHT_BORDER_VERTICES,  
    NH_RENDERER_VULKAN_BUFFER_BOTTOM_BORDER_VERTICES, 
    NH_RENDERER_VULKAN_BUFFER_LEFT_BORDER_VERTICES,   
    NH_RENDERER_VULKAN_BUFFER_LIST_ITEM_MARKER,
    NH_RENDERER_VULKAN_BUFFER_BACKGROUND_IMAGE,       
    NH_RENDERER_VULKAN_BUFFER_TEXTURE,            
    NH_RENDERER_VULKAN_BUFFER_COUNT,                  
} NH_RENDERER_VULKAN_BUFFER;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanBackgroundVertices(
    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p
);

NH_API_RESULT nh_renderer_createVulkanBorderVertices(
    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p, 
    NH_RENDERER_VULKAN_BUFFER type
);

void nh_renderer_destroyVulkanBufferArray(
    nh_gfx_VulkanDriver *Driver_p, nh_renderer_VulkanData *Data_p
);

nh_gfx_VulkanBuffer *nh_renderer_getVulkanBuffer(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VULKAN_BUFFER type
);

nh_gfx_VulkanBuffer *nh_renderer_getVulkanBufferFromIndex(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VULKAN_BUFFER type, unsigned int index
);

#endif // NH_RENDERER_VULKAN_BUFFER_H
