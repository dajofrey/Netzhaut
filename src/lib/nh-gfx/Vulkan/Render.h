#ifndef NH_GFX_VULKAN_RENDER_H
#define NH_GFX_VULKAN_RENDER_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "GPU.h"
#include "Surface.h"

#include "../Base/Surface.h"
#include "../Common/Includes.h"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_prepareVulkanRendering(
    nh_gfx_VulkanSurface *Surface_p
);

NH_API_RESULT nh_gfx_renderVulkan(
    nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p
);

#endif // NH_GFX_VULKAN_RENDER_H
