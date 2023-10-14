#ifndef NH_TERMINAL_VULKAN_TEXT_H
#define NH_TERMINAL_VULKAN_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nhgfx/Vulkan/GPU.h"
#include "../../nhgfx/Vulkan/Helper.h"
#include "../../nhgfx/Fonts/FontManager.h"

#include <stdint.h>

#endif

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_VulkanText {
        NH_BOOL destroy;
        nh_vk_Buffer VertexBuffer;
        nh_vk_Buffer IndexBuffer;
        nh_vk_Buffer VertShaderBuffer;
        nh_vk_Buffer FragShaderBuffer;
        nh_vk_DescriptorSet DescriptorSet;
    } nh_terminal_VulkanText;

/** @} */

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_initVulkanText(
        nh_vk_GPU *GPU_p, nh_terminal_VulkanText *Text_p
    );

    NH_TERMINAL_RESULT nh_terminal_updateVulkanText(
        nh_vk_GPU *GPU_p, nh_gfx_FontInstance *FontInstance_p, nh_terminal_VulkanText *Text_p, nh_Array *Vertices_p, 
        nh_Array *Indices_p
    );

/** @} */

#endif 
