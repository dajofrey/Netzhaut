#ifndef NH_RENDERER_VULKAN_RECORD_H
#define NH_RENDERER_VULKAN_RECORD_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Base/Viewport.h"

// FUNCTIONS =======================================================================================

void nh_renderer_recordVulkanFragmentTree(
    nh_css_FragmentTree *Tree_p, nh_gfx_Viewport *Viewport_p
);

#endif // NH_RENDERER_VULKAN_RECORD_H
