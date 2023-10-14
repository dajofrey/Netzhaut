#ifndef NH_TERMINAL_VULKAN_PIPELINE_H
#define NH_TERMINAL_VULKAN_PIPELINE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nhgfx/Vulkan/Driver.h"
#include "../../nhgfx/Vulkan/Helper.h"

#endif

/** @addtogroup lib_nhcss_enums
 *  @{
 */

    typedef enum NH_TERMINAL_VULKAN_PIPELINE {
        NH_TERMINAL_VULKAN_PIPELINE_SDF, 
       _NH_TERMINAL_VULKAN_PIPELINE_COUNT,     
    } NH_TERMINAL_VULKAN_PIPELINE;

/** @} */

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_createVulkanPipelines(
        nh_vk_Driver *Driver_p, nh_vk_Pipeline *Pipelines_p
    );
    
    void nh_terminal_destroyVulkanPipelines(
        nh_vk_Driver *Driver_p, nh_vk_Pipeline *Pipelines_p
    );

/** @} */

#endif 
