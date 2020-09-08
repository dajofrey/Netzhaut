// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/EventListener.h"
#include "../Header/WheelEvent.h"
#include "../Header/MouseEvent.h"
#include "../Header/Event.h"
#include "../Header/Macros.h"

#include "../../Header/Debug.h"
#include "../../Header/Variable.h"

#include "../../Core/Header/Function.h"

#include "../../../Core/Header/Memory.h"
#include "../../../Core/Header/HashMap.h"
#include "../../../Core/Header/String.h"

#include <string.h>

#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result NH_JS_EVENT_LISTENER_EventListener(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_LISTENER_handleEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    // this function is usually overwritten

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_JS_DEBUG

NH_RESULT Nh_JS_addEventListener(
    Nh_JS_Script *Script_p, NH_JS_EVENT eventType, char *functionName_p, Nh_JS_Object *Target_p)
{
NH_BEGIN()

#include NH_DEFAULT_CHECK

    NH_CHECK_NULL(Script_p, Target_p)

    Nh_JS_Parsable *Function_p = Nh_JS_getFunctionDeclaration(Script_p, functionName_p);

    for (int i = 0; i < Script_p->DOM.EventListeners.count; ++i) {
        Nh_JS_Object *Object_p = Nh_getListItem(&Script_p->DOM.EventListeners, i);
        Nh_JS_EventListener *Listener_p = Object_p->data_p;
        if (Listener_p->eventType == eventType && Listener_p->Target.data_p == Target_p && ((Nh_JS_Function*)Object_p->Functions_p[0].data_p)->external_p == Function_p) {NH_END(NH_SUCCESS)}
    }

    Nh_JS_Object *ListenerObject_p;
    NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_EVENT_LISTENER, &ListenerObject_p))

    if (Function_p != NULL) {
        ((Nh_JS_Function*)ListenerObject_p->Functions_p[0].data_p)->external_p = Function_p; // overwrite handleEvent
    }

    Nh_JS_EventListener *ListenerData_p = Nh_allocate(sizeof(Nh_JS_EventListener));
    NH_CHECK_MEM(ListenerData_p)

    Nh_JS_initVariable(&ListenerData_p->Target, NULL, -1, 0);
    ListenerData_p->Target.data_p     = Target_p;
    ListenerData_p->Target.type       = NH_JS_TYPE_OBJECT;
    ListenerData_p->Target.distinct_p = Nh_JS_getDistinction(Script_p, Target_p);
    ListenerData_p->eventType         = eventType;

    ListenerObject_p->data_p = ListenerData_p;

    Nh_JS_debugEventListener(Script_p, ListenerObject_p, 0);
    NH_CHECK(Nh_addListItem(&Script_p->DOM.EventListeners, ListenerObject_p))

#include NH_CUSTOM_CHECK

NH_END(NH_SUCCESS)
}

void Nh_JS_freeEventListeners(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    for (int i = 0; i < Script_p->DOM.EventListeners.count; ++i) {
        Nh_JS_Object *Listener_p = Nh_getListItem(&Script_p->DOM.EventListeners, i);
        Nh_free(Listener_p->data_p);
        Nh_JS_destroyObject(Listener_p);
    }
    Nh_destroyList(&Script_p->DOM.EventListeners, false);

NH_SILENT_END()
}

char *Nh_JS_stringifyEventListenerData(
    Nh_JS_Object *Object_p, bool newline)
{
NH_BEGIN()

    Nh_JS_EventListener *Listener_p = Object_p->data_p;

    Nh_String *String_p = Nh_allocateString(NULL);

    char *object_p = Nh_JS_stringifyObject(Listener_p->Target.data_p, false, true);
    NH_CHECK_NULL(NULL, object_p)

    NH_CHECK(NULL, Nh_appendFormatToString(String_p, 
        "for %s on %s", 
        Nh_JS_getEventTypes(NULL)[Listener_p->eventType], object_p
    ))

    if (newline) {NH_CHECK(NULL, Nh_appendToString(String_p, "\n"))}

    Nh_free(object_p);

    char *chars_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

NH_END(chars_p)
}

