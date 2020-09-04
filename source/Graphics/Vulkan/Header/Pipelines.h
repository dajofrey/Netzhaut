#ifndef NH_VK_PIPELINES_H
#define NH_VK_PIPELINES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../API/Header/Netzhaut.h"

typedef struct Nh_Vk_GPU Nh_Vk_GPU;

#endif

/** @addtogroup VulkanEnums Enums
 *  \ingroup Vulkan
 *  @{
 */

    typedef enum NH_VK_PIPELINE_ {
        NH_VK_PIPELINE_COLOR,    
        NH_VK_PIPELINE_TEXT_SDF, 
        NH_VK_PIPELINE_IMAGE,   
        NH_VK_PIPELINE_BACKGROUND_IMAGE,
        NH_VK_PIPELINE_CANVAS_RENDERING_CONTEXT_2D_ARC,
        NH_VK_PIPELINE_COUNT,     
    } NH_VK_PIPELINE_;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createPipelines(
        Nh_Vk_GPU *GPU_p
    );
    
    void Nh_Vk_destroyPipelines(
        Nh_Vk_GPU *GPU_p
    );

/** @} */

#endif 
