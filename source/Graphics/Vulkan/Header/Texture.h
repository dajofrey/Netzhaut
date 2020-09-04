#ifndef NH_VK_TEXTURE_H
#define NH_VK_TEXTURE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Header/GPU.h"
#include "../../Header/Texture.h"
#include "../../../API/Header/Netzhaut.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_hardLoadTexture(
        Nh_Vk_GPU *GPU_p, Nh_Gfx_Texture *Texture_p
    );

/** @} */

#endif
