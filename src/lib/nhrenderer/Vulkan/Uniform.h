#ifndef NH_RENDERER_VK_UNIFORM_H
#define NH_RENDERER_VK_UNIFORM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Data.h"

#include "../Common/Includes.h"

#include "../../nhcss/Main/FragmentTree.h"
#include "../../nhgfx/Vulkan/Driver.h"
#include "../../nhgfx/Vulkan/Helper.h"

#endif

/** @addtogroup lib_nhrenderer_enums
 *  @{
 */

    typedef enum NH_RENDERER_VK_UNIFORM {
        NH_RENDERER_VK_UNIFORM_BACKGROUND,       
        NH_RENDERER_VK_UNIFORM_TOP_BORDER,       
        NH_RENDERER_VK_UNIFORM_RIGHT_BORDER,     
        NH_RENDERER_VK_UNIFORM_BOTTOM_BORDER,    
        NH_RENDERER_VK_UNIFORM_LEFT_BORDER,
        NH_RENDERER_VK_UNIFORM_TEXT_SDF_VS,
        NH_RENDERER_VK_UNIFORM_TEXT_SDF_FS,
        NH_RENDERER_VK_UNIFORM_IMAGE,
        NH_RENDERER_VK_UNIFORM_BACKGROUND_IMAGE,
        NH_RENDERER_VK_UNIFORM_COUNT,
    } NH_RENDERER_VK_UNIFORM;

/** @} */

/** @addtogroup lib_nhrenderer_functions
 *  @{
 */

    NH_RENDERER_RESULT nh_renderer_vk_createColorUniform(
        nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p, NH_RENDERER_VK_UNIFORM type
    );
    
    void nh_renderer_vk_destroyUniformArray(
        nh_vk_Driver *Driver_p, nh_renderer_vk_Data *Data_p
    );
    
    nh_vk_Buffer *nh_renderer_vk_getUniform(
        nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_UNIFORM type
    );
    
    nh_vk_Buffer *nh_renderer_vk_getUniformFromIndex(
        nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_UNIFORM type, unsigned int index
    );

/** @} */

#endif
