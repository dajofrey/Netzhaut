#ifndef NH_RENDERER_VK_PIPELINE_H
#define NH_RENDERER_VK_PIPELINE_H

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

/** @addtogroup lib_nhrenderer_enums
 *  @{
 */

    typedef enum NH_RENDERER_VK_PIPELINE {
        NH_RENDERER_VK_PIPELINE_COLOR,    
        NH_RENDERER_VK_PIPELINE_TEXT_SDF, 
        NH_RENDERER_VK_PIPELINE_IMAGE,   
        NH_RENDERER_VK_PIPELINE_BACKGROUND_IMAGE,
        NH_RENDERER_VK_PIPELINE_COUNT,     
    } NH_RENDERER_VK_PIPELINE;

/** @} */

/** @addtogroup lib_nhrenderer_functions
 *  @{
 */

    NH_RENDERER_RESULT nh_renderer_vk_createPipelines(
        nh_vk_Driver *Driver_p, nh_vk_Pipeline *Pipelines_p
    );
    
    void nh_renderer_vk_destroyPipelines(
        nh_vk_Driver *Driver_p, nh_vk_Pipeline *Pipelines_p
    );

/** @} */

#endif 
