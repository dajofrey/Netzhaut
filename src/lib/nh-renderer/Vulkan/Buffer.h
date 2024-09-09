#ifndef NH_RENDERER_VK_BUFFER_H
#define NH_RENDERER_VK_BUFFER_H

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

    typedef enum NH_RENDERER_VK_BUFFER {
        NH_RENDERER_VK_BUFFER_TEXT_SDF,               
        NH_RENDERER_VK_BUFFER_TEXT_SDF_INDEX,         
        NH_RENDERER_VK_BUFFER_BACKGROUND,   
        NH_RENDERER_VK_BUFFER_TOP_BORDER_VERTICES,    
        NH_RENDERER_VK_BUFFER_RIGHT_BORDER_VERTICES,  
        NH_RENDERER_VK_BUFFER_BOTTOM_BORDER_VERTICES, 
        NH_RENDERER_VK_BUFFER_LEFT_BORDER_VERTICES,   
        NH_RENDERER_VK_BUFFER_LIST_ITEM_MARKER,
        NH_RENDERER_VK_BUFFER_BACKGROUND_IMAGE,       
        NH_RENDERER_VK_BUFFER_TEXTURE,            
        NH_RENDERER_VK_BUFFER_COUNT,                  
    } NH_RENDERER_VK_BUFFER;

/** @} */

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    NH_RENDERER_RESULT nh_renderer_vk_createBackgroundVertices(
        nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p
    );
 
    NH_RENDERER_RESULT nh_renderer_vk_createBorderVertices(
        nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p, 
        NH_RENDERER_VK_BUFFER type
    );

    void nh_renderer_vk_destroyBufferArray(
        nh_vk_Driver *Driver_p, nh_renderer_vk_Data *Data_p
    );

    nh_vk_Buffer *nh_renderer_vk_getBuffer(
        nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_BUFFER type
    );
    
    nh_vk_Buffer *nh_renderer_vk_getBufferFromIndex(
        nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_BUFFER type, unsigned int index
    );

/** @} */

#endif 
