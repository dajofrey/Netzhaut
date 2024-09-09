#ifndef NH_VK_VULKAN_H
#define NH_VK_VULKAN_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Host.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_vk_Vulkan {
        nh_vk_Host Host;
        nh_List GPUs;
    } nh_vk_Vulkan;

/** @} */

/** @addtogroup lib_nh-gfx_vars
 *  @{
 */

    extern nh_vk_Vulkan NH_VULKAN;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_vk_initVulkan(
    );
    
    NH_GFX_RESULT nh_vk_terminateVulkan(
    );

/** @} */

#endif 
