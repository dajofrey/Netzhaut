// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Text.h"
#include "../Header/Macros.h"

#include "../Vulkan/Header/Text.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include "../../CSS/Header/Vertices.h"
#include "../../Core/Header/Memory.h"

// PUBLIC ==========================================================================================

NH_RESULT Nh_Gfx_updateText(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (!Nh_HTML_isTextNode(Node_p)) {NH_END(NH_SUCCESS)}

    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_updateText(Tab_p, Node_p))
    }

NH_END(NH_SUCCESS)
}

