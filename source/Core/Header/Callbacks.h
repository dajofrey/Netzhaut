#ifndef NH_CALLBACKS_H
#define NH_CALLBACKS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tab.h"
#include "List.h"

#include "../../API/Header/Vulkan.h"

#endif

/** @addtogroup CoreEnums Enums
 *  \ingroup Core
 *  @{
 */

    typedef enum NH_CALLBACK {
        NH_CALLBACK_VULKAN,
    } NH_CALLBACK;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    typedef NH_RESULT (*NH_VulkanCallback)(struct VolkDeviceTable, VkCommandBuffer*);

    NH_RESULT Nh_addCallback(
        Nh_List *Callbacks_p, char *elementId_p, void *callback_p, NH_CALLBACK type
    );

    NH_VulkanCallback Nh_getVulkanCallback(
        Nh_Tab *Tab_p, char *elementId_p
    );

    void Nh_deleteCallbacks(
        Nh_Tab *Tab_p
    );

/** @} */

#endif 
