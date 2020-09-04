#ifndef NH_VK_RENDER_H
#define NH_VK_RENDER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Driver.h"

#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/Window.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  /ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_prepareRendering(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Vk_render(
        Nh_Window *Window_p
    );

/** @} */

#endif 
