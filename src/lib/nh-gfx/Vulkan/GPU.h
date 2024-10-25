#ifndef NH_VULKAN_GPU_H
#define NH_VULKAN_GPU_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Helper.h"
#include "Driver.h"

#include "../../nh-core/Util/List.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_VulkanGPU {
    char *name_p;                       
    nh_gfx_VulkanDriver Driver;
    nh_core_List Textures;
    struct {
        nh_gfx_VulkanPipeline *Pipelines_p;
    } Renderer;
    struct {
        nh_gfx_VulkanPipeline *Pipelines_p;
    } Term;
} nh_gfx_VulkanGPU;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_initVulkanGPUs(
    nh_core_List *GPUs_p, nh_gfx_VulkanHost *Host_p
);

void nh_gfx_freeVulkanGPUs(
    nh_core_List *GPUs_p
);

#endif // NH_VULKAN_GPU_H
