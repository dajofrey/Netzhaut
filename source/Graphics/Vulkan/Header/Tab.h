#ifndef NH_VK_TAB_H
#define NH_VK_TAB_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../HTML/Header/Document.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/Tab.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createTab(
        Nh_Tab *Tab_p
    );
   
    void Nh_Vk_destroyTab(
        Nh_Tab *Tab_p
    );

/** @} */

#endif 
