// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/EventTarget.h"
#include "../Header/EventListener.h"
#include "../Header/HTMLElement.h"
#include "../Header/Document.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/HashMap.h"

#include <string.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result NH_JS_EVENT_TARGET_EventTarget(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_TARGET_addEventListener(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 2) // set
    {
        Nh_HashValue *HashValue_p;
        if (hashmap_get(Nh_getHashMaps()->JS.EventTypes, Arguments_p[0].data_p, (void**)(&HashValue_p)) != MAP_OK) {NH_END(Nh_JS_getNULLResult())}

        Nh_JS_Object *Object_p = Function_p->Inherit_p;
        while (Object_p->Inheriting_p != NULL) {Object_p = Object_p->Inheriting_p;}

        if (Object_p->type == NH_JS_OBJECT_DOCUMENT) {
            Object_p = ((Nh_JS_Document*)Object_p->data_p)->Tree.Root_p;
        }

        Nh_JS_addEventListener(Script_p, HashValue_p->number, Arguments_p[1].data_p, Object_p);
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_TARGET_removeEventListener(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_TARGET_dispatchEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 1 && Arguments_p->type == NH_JS_TYPE_OBJECT)
    {
        Nh_JS_Object *Object_p = Arguments_p->data_p;
        if (Nh_JS_getObject(Object_p, NH_JS_OBJECT_EVENT) == NULL) {NH_END(Nh_JS_getNULLResult())}
        if (Nh_JS_getObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT) == NULL) {NH_END(Nh_JS_getNULLResult())}
       
        Nh_JS_Object *Element_p = Nh_JS_getObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT);
        Nh_JS_Event *Event_p = Nh_JS_getObject(Object_p, NH_JS_OBJECT_EVENT)->data_p;
        Event_p->Target_p = Element_p;        

        NH_CHECK(Nh_JS_getNULLResult(), Nh_JS_broadcastEvent(Script_p->Run.Tab_p, Object_p))
    }

NH_END(Nh_JS_getNULLResult())
}

