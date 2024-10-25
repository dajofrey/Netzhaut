#ifndef NH_GFX_COMMON_DEBUG_H
#define NH_GFX_COMMON_DEBUG_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../../nh-core/Util/Debug.h"

// MACROS ==========================================================================================

#undef  NH_GFX_CHECK_VULKAN 
#define NH_GFX_CHECK_VULKAN(checkable) \
{ \
    VkResult checkResult = checkable; \
    if (checkResult != VK_SUCCESS) {NH_CORE_CHECK(nh_vk_getResult(checkResult))} \
} 
 
#undef  NH_GFX_CHECK_VULKAN_2 
#define NH_GFX_CHECK_VULKAN_2(checkReturn, checkable) \
{ \
    VkResult checkResult = checkable; \
    if (checkResult != VK_SUCCESS) {NH_CORE_CHECK_2(nh_vk_getResult(checkResult), checkReturn);} \
} 
 
#endif // NH_GFX_COMMON_DEBUG_H
