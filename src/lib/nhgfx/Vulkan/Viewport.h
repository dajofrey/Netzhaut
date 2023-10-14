#ifndef NH_VK_VIEWPORT_H
#define NH_VK_VIEWPORT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_vk_Viewport {
        VkCommandBuffer CommandBuffers_p[6];
        VkCommandBuffer *CommandBuffers_pp[3];
        int images;
    } nh_vk_Viewport;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_vk_createViewport(
        nh_gfx_Viewport *Viewport_p
    );
    
    void nh_vk_destroyViewport(
        nh_gfx_Viewport *Viewport_p
    );
    
    NH_GFX_RESULT nh_vk_beginRecording(
        nh_gfx_Viewport *Viewport_p
    );
    
    NH_GFX_RESULT nh_vk_endRecording(
        nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif

