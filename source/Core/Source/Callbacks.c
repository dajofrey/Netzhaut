// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Callbacks.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"
#include "../Header/Config.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef struct NH_Callback {
    NH_CALLBACK type;
    char *elementId_p;
    void *callback_p;
} NH_Callback;

// VULKAN ==========================================================================================

NH_RESULT Nh_setVulkanCallback(
    Nh_Tab *Tab_p, char *elementId_p, NH_RESULT (*callback_p)(struct VolkDeviceTable, VkCommandBuffer*))
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p, elementId_p, callback_p)

NH_END(Nh_addCallback(&Tab_p->Callbacks, elementId_p, callback_p, NH_CALLBACK_VULKAN))
}

NH_VulkanCallback Nh_getVulkanCallback(
    Nh_Tab *Tab_p, char *elementId_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Callbacks.count; ++i) {
        NH_Callback *Callback_p = Nh_getListItem(&Tab_p->Callbacks, i);
        if (Callback_p->type == NH_CALLBACK_VULKAN && !strcmp(elementId_p, Callback_p->elementId_p)) {
            NH_END(Callback_p->callback_p)
        }
    }

NH_END(NULL)
}

// ADD =============================================================================================

NH_RESULT Nh_addCallback(
    Nh_List *Callbacks_p, char *elementId_p, void *callback_p, NH_CALLBACK type)
{
NH_BEGIN()

    NH_CHECK_NULL(Callbacks_p, callback_p)

    NH_Callback *Callback_p = Nh_allocate(sizeof(NH_Callback));
    NH_CHECK_MEM(Callback_p)
    Callback_p->type        = type;
    Callback_p->callback_p  = NULL;
    Callback_p->elementId_p = NULL;
   
    if (elementId_p != NULL)
    { 
        Callback_p->elementId_p = Nh_allocate(sizeof(char) * (strlen(elementId_p) + 1));
        NH_CHECK_MEM(Callback_p->elementId_p)
        strcpy(Callback_p->elementId_p, elementId_p);
    }

    NH_CHECK(Nh_addListItem(Callbacks_p, Callback_p))

NH_END(NH_SUCCESS)
}

// DELETE ==========================================================================================

void Nh_deleteCallbacks(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Callbacks.count; ++i) {
        NH_Callback *Callback_p = Nh_getListItem(&Tab_p->Callbacks, i);
        Nh_free(Callback_p->elementId_p);
    }
    Nh_destroyList(&Tab_p->Callbacks, true);

NH_END()
}

