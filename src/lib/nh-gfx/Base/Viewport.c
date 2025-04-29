// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Viewport.h"

#include "../Vulkan/Viewport.h"
#include "../OpenGL/Viewport.h"

#include "../Common/Config.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"
#include "../../nh-core/Util/List.h"

#include <string.h>

// FUNCTIONS =======================================================================================

nh_gfx_Viewport *nh_gfx_createViewport(
    nh_gfx_Surface *Surface_p, nh_api_PixelPosition *Position_p, nh_api_PixelSize *Size_p)
{
    NH_CORE_CHECK_NULL_2(NULL, Surface_p)

    nh_gfx_Viewport *Viewport_p = (nh_gfx_Viewport*)nh_core_allocate(sizeof(nh_gfx_Viewport));
    NH_CORE_CHECK_MEM_2(NULL, Viewport_p)

    Viewport_p->Owner.type = NH_GFX_VIEWPORT_OWNER_NONE;
    Viewport_p->Owner.p = NULL;

    Viewport_p->Surface_p = Surface_p;

    Viewport_p->Settings.ClearColor.r = 1.0f;
    Viewport_p->Settings.ClearColor.g = 1.0f;
    Viewport_p->Settings.ClearColor.b = 1.0f;
    Viewport_p->Settings.ClearColor.a = 1.0f;

    Viewport_p->Settings.BorderColor.r = 1.0f;
    Viewport_p->Settings.BorderColor.g = 1.0f;
    Viewport_p->Settings.BorderColor.b = 1.0f;
    Viewport_p->Settings.BorderColor.a = 1.0f;
 
    Viewport_p->Settings.border   = false;
    Viewport_p->Settings.borderWidth = 0;
    Viewport_p->Settings.priority = 0;

    Viewport_p->Sync.newestBuffers_p[0] = 0;
    Viewport_p->Sync.newestBuffers_p[1] = 2;
    Viewport_p->Sync.newestBuffers_p[2] = 4;

    Viewport_p->Sync.atomicRecording = false;
    Viewport_p->Sync.render          = false;
    Viewport_p->Sync.atomic          = false;

    Viewport_p->scroll = 0;

    if (Size_p != NULL) {
        Viewport_p->Settings.Size = *Size_p;
    } else {
        Viewport_p->Settings.Size.width = nh_gfx_getConfig().ViewportSize.width;
        Viewport_p->Settings.Size.height = nh_gfx_getConfig().ViewportSize.height;
    }

    if (Position_p != NULL) {
        Viewport_p->Settings.Position = *Position_p;
    } else {
        Viewport_p->Settings.Position.x = nh_gfx_getConfig().ViewportPosition.x;
        Viewport_p->Settings.Position.y = nh_gfx_getConfig().ViewportPosition.y;
    }

    switch (Surface_p->api)
    {
        case NH_API_GRAPHICS_BACKEND_VULKAN : 
            NH_CORE_CHECK_2(NULL, nh_gfx_createVulkanViewport(Viewport_p)) 
            break;
        case NH_API_GRAPHICS_BACKEND_OPENGL : 
            NH_CORE_CHECK_2(NULL, nh_gfx_createOpenGLViewport(Viewport_p)) 
            break;

        default : return NULL;
    }

    nh_core_appendToList(&Surface_p->Viewports, Viewport_p);

    return Viewport_p;
}

void nh_gfx_destroyViewport(
    nh_gfx_Surface *Surface_p, nh_gfx_Viewport *Viewport_p)
{
    switch (Surface_p->api)
    {
        case NH_API_GRAPHICS_BACKEND_VULKAN : 
            nh_gfx_destroyVulkanViewport(Viewport_p); 
            break;
        case NH_API_GRAPHICS_BACKEND_OPENGL : 
            nh_gfx_destroyOpenGLViewport(Viewport_p);
            break;
    }

    nh_core_free(Viewport_p);

    return;
}
 
NH_API_RESULT nh_gfx_configureViewport(
    nh_gfx_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size)
{
    NH_CORE_CHECK_NULL(Viewport_p)

    Viewport_p->Settings.Size     = Size;
    Viewport_p->Settings.Position = Position;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_beginRecording(
    nh_gfx_Viewport *Viewport_p)
{
    Viewport_p->Sync.atomic = Viewport_p->Sync.atomicRecording;

    int imageCount = 0;
    switch (Viewport_p->Surface_p->api)
    {
        case NH_API_GRAPHICS_BACKEND_VULKAN : 
            imageCount = Viewport_p->Vulkan.images = Viewport_p->Surface_p->Vulkan.imageCount; 
            break;
        case NH_API_GRAPHICS_BACKEND_OPENGL : 
            imageCount = 1; 
            break;
        default : return NH_API_ERROR_BAD_STATE;
    }

    if (imageCount > 3) {return NH_API_ERROR_BAD_STATE;}

    for (int i = 0; i < imageCount; ++i)
    {
        int bufferIndex = 2 * i;

        if (Viewport_p->Surface_p->Sync.rendering) {
            bufferIndex += Viewport_p->Sync.submittedBuffers_p[i] == bufferIndex ? 1 : 0;
        } 
        else {bufferIndex += Viewport_p->Sync.newestBuffers_p[i] == bufferIndex ? 1 : 0;}
        
        switch (Viewport_p->Surface_p->api)
        {
            case NH_API_GRAPHICS_BACKEND_VULKAN : 
                Viewport_p->Vulkan.CommandBuffers_pp[i] = &Viewport_p->Vulkan.CommandBuffers_p[bufferIndex];
                break;
            case NH_API_GRAPHICS_BACKEND_OPENGL : 
                Viewport_p->OpenGL.CommandBuffer_p = &Viewport_p->OpenGL.CommandBuffers_p[bufferIndex];
                break;
        }

        Viewport_p->Sync.newestBuffers_p[i] = bufferIndex;
    }

    switch (Viewport_p->Surface_p->api)
    {
        case NH_API_GRAPHICS_BACKEND_VULKAN : 
            NH_CORE_CHECK(nh_gfx_beginVulkanRecording(Viewport_p))
            break;
        case NH_API_GRAPHICS_BACKEND_OPENGL : 
            NH_CORE_CHECK(nh_gfx_beginOpenGLRecording(Viewport_p))
            break;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_endRecording(
    nh_gfx_Viewport *Viewport_p, bool blockUntilRender)
{
    switch (Viewport_p->Surface_p->api)
    {
        case NH_API_GRAPHICS_BACKEND_VULKAN : 
            NH_CORE_CHECK(nh_gfx_endVulkanRecording(Viewport_p))
            break;
        case NH_API_GRAPHICS_BACKEND_OPENGL : 
            NH_CORE_CHECK(nh_gfx_endOpenGLRecording(Viewport_p))
            break;
        default : return NH_API_ERROR_BAD_STATE;
    }

    Viewport_p->Sync.render = true;

    if (Viewport_p->Surface_p->renderRequests <= 1) {
        Viewport_p->Surface_p->renderRequests++;
    }
 
    if (blockUntilRender) {
        nh_core_executeWorkload(Viewport_p->Surface_p);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_claimViewport(
    nh_gfx_Viewport *Viewport_p, NH_GFX_VIEWPORT_OWNER owner, void *owner_p)
{
    if (Viewport_p->Owner.type != NH_GFX_VIEWPORT_OWNER_NONE || Viewport_p->Owner.p != NULL) {
        return NH_API_ERROR_BAD_STATE;
    }
 
    Viewport_p->Owner.type = owner;
    Viewport_p->Owner.p = owner_p;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_scroll(
    nh_gfx_Viewport *Viewport_p, float amount)
{
    Viewport_p->scroll = amount;
    return NH_API_SUCCESS;
}
