// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Tab.h"
#include "../Header/Macros.h"

#include "../Vulkan/Header/Tab.h"
#include "../Vulkan/Header/Record.h"
#include "../Vulkan/Header/Node.h"
#include "../Vulkan/Header/GPU.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include "../../Core/Header/Sync.h"

// TAB =============================================================================================

NH_RESULT Nh_Gfx_createTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createTab(Tab_p)) break;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Gfx_updateTabRendering(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_syncRecordCommandBufferBegin(Tab_p);

    for (int i = 0; i < 2; ++i)
    {
        int bufferIndex = 2 * i;
        
        if (Tab_p->Window_p->Sync.rendering) {
            bufferIndex += Tab_p->Sync.submittedBuffers_p[i] == bufferIndex ? 1 : 0;
        } 
        else {bufferIndex += Tab_p->Sync.newestBuffers_p[i] == bufferIndex ? 1 : 0;}
 
        switch (Tab_p->Window_p->GPU.API)
        {
            case NH_API_VULKAN :
            { 
                Nh_Vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer; 
                NH_CHECK(Nh_Vk_record(&GPU_p->Driver, &Tab_p->Window_p->Vulkan.Surface, Tab_p, GPU_p->Pipelines_p, bufferIndex, i))
                break;
            }
            case NH_API_OPENGL :
            { 
                break;
            }
        }

        Tab_p->Sync.newestBuffers_p[i] = bufferIndex;
    }

    Nh_syncRecordCommandBufferEnd(Tab_p);

NH_END(NH_SUCCESS)
}

void Nh_Gfx_destroyTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : Nh_Vk_destroyTab(Tab_p); break;
    }

NH_SILENT_END()
}

