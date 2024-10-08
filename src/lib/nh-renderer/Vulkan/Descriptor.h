#ifndef NH_RENDERER_VK_DESCRIPTOR_H
#define NH_RENDERER_VK_DESCRIPTOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Data.h"

#include "../Common/Includes.h"

#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Vulkan/Driver.h"
#include "../../nh-gfx/Vulkan/Helper.h"

#endif

/** @addtogroup lib_nh-renderer_enums
 *  @{
 */

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

/** @} */

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    NH_API_RESULT nh_renderer_vk_createColorDescriptor(
        nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p, NH_RENDERER_VK_DESCRIPTOR type
    );

    void nh_renderer_vk_destroyDescriptorArray(
        nh_vk_Driver *Driver_p, nh_renderer_vk_Data *Data_p
    );
    
    nh_vk_DescriptorSet *nh_renderer_vk_getDescriptor(
        nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_DESCRIPTOR type
    );
    
    nh_vk_DescriptorSet *nh_renderer_vk_getDescriptorFromIndex(
        nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_DESCRIPTOR type, unsigned int index
    );

/** @} */

#endif
