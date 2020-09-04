// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Window.h"
#include "../Header/Macros.h"

#include "../Vulkan/Header/Surface.h"
#include "../Vulkan/Header/Render.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <limits.h>

// CREATE ==========================================================================================

NH_RESULT Nh_Gfx_createWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()

    switch (Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createSurface(Window_p)) break;
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

NH_RESULT Nh_Gfx_destroyWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()
  
    switch (Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_destroySurface(Window_p)) break;
    }

NH_END(NH_SUCCESS)
}

// RENDER ==========================================================================================

NH_RESULT Nh_Gfx_prepareRendering(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_RESULT result = NH_ERROR_BAD_STATE;

    switch (Window_p->GPU.API)
    {
        case NH_API_VULKAN : result = Nh_Vk_prepareRendering(Window_p); break;
    }

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        result = NH_ERROR_RESIZE_NEEDED;
    }

NH_END(result)
}

NH_RESULT Nh_Gfx_render(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_RESULT result = NH_ERROR_BAD_STATE;

    switch (Window_p->GPU.API)
    {
        case NH_API_VULKAN : result = Nh_Vk_render(Window_p); break;
    }

    if (result == NH_VULKAN_ERROR_OUT_OF_DATE_KHR) {
        result = NH_ERROR_RESIZE_NEEDED;
    }

NH_END(result)
}

// RESIZE ==========================================================================================

NH_RESULT Nh_Gfx_resize(
    Nh_Window *Window_p)
{
NH_BEGIN();

    switch (Window_p->GPU.API) 
    { 
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_resize(Window_p)) break;
    }

NH_END(NH_SUCCESS);
}

