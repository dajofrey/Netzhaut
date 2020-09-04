#ifndef NH_VK_RECORD_H
#define NH_VK_RECORD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/System.h"
#include "../../../Core/Header/Tab.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_record(
        Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, Nh_Tab *Tab_p, NH_Vk_Pipeline *Pipelines_p, 
        int bufferIndex, int imageIndex
    );

/** @} */

#endif
