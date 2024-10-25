#ifndef NH_RENDERER_VULKAN_PIPELINE_H
#define NH_RENDERER_VULKAN_PIPELINE_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

#include "../../nh-gfx/Vulkan/Driver.h"
#include "../../nh-gfx/Vulkan/Helper.h"

// ENUMS ===========================================================================================

typedef enum NH_RENDERER_VK_PIPELINE {
    NH_RENDERER_VK_PIPELINE_COLOR,    
    NH_RENDERER_VK_PIPELINE_TEXT_SDF, 
    NH_RENDERER_VK_PIPELINE_IMAGE,   
    NH_RENDERER_VK_PIPELINE_BACKGROUND_IMAGE,
    NH_RENDERER_VK_PIPELINE_COUNT,     
} NH_RENDERER_VK_PIPELINE;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanPipelines(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipelines_p
);

void nh_renderer_destroyVulkanPipelines(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipelines_p
);

#endif // NH_RENDERER_VULKAN_PIPELINE_H 
