#ifndef NH_VK_TEXT_H
#define NH_VK_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include <stdint.h>

#include "../../../HTML/Header/Document.h"
#include "../../../API/Header/Netzhaut.h"

#include "../../../Core/Header/Window.h"
#include "../../../Core/Header/System.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createText(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, bool allocate, float *vertices_p, uint32_t *indices_p
    );
    
    NH_RESULT Nh_Vk_updateText(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );

/** @} */

#endif 
