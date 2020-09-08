// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Event.h"
#include "../Header/EventListener.h"
#include "../Header/MouseEvent.h"
#include "../Header/WheelEvent.h"
#include "../Header/Macros.h"
#include "../Header/HTMLElement.h"

#include "../../Header/Debug.h"
#include "../../Header/Interpreter.h"

#include "../../../Core/Header/HashMap.h"
#include "../../../Core/Header/String.h"
#include "../../../Core/Header/Memory.h"

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_dispatchEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Object_p
);
static NH_RESULT Nh_JS_dispatchInternalEvent(
    Nh_JS_Script *Script_p, Nh_JS_Event *Event_p, va_list args
);

// API =============================================================================================

Nh_JS_Result NH_JS_EVENT_Event(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 1 && Arguments_p[0].type == NH_JS_TYPE_STRING)
    {
        Nh_JS_Event Event;
        Event.type     = -1;
        Event.type_p   = NULL;
        Event.Target_p = NULL;

        Nh_HashValue *HashValue_p = NULL;
        hashmap_get(Nh_getHashMaps()->JS.EventTypes, Arguments_p[0].data_p, (void**)(&HashValue_p));

        if (HashValue_p != NULL) {Event.type = HashValue_p->number;}
        else {Event.type_p = Arguments_p[0].data_p;}

        Nh_JS_Object *Object_p;
        Nh_JS_createEventObject(Script_p, &Object_p, &Event);

        NH_END(Nh_JS_getResult(NH_JS_TYPE_OBJECT, true, Object_p)) 
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_bubbles(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_cancelBubble(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_cancelable(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_composed(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_currentTarget(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_deepPath(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_defaultPrevented(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_eventPhase(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_explicitOriginalTarget(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_originalTarget(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_returnValue(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_srcElement(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_target(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()
    
    if (aCount == 0)
    {
        Nh_JS_Object *Object_p = Nh_JS_getObject(Function_p, NH_JS_OBJECT_EVENT);
        Nh_JS_Event *Event_p = Object_p->data_p;
        NH_END(Nh_JS_getResult(NH_JS_TYPE_OBJECT, false, Event_p->Target_p))
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_timeStamp(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_type(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_isTrusted(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_createEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_composedPath(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_initEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_preventDefault(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_stopImmediatePropagation(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_EVENT_stopPropagation(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG

NH_RESULT Nh_JS_createEventObject(
    Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_JS_Event *Event_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_EVENT, Object_pp))
    (*Object_pp)->data_p = Nh_allocate(sizeof(Nh_JS_Event));
    NH_CHECK_MEM((*Object_pp)->data_p);

    Nh_JS_Event Event;
    Event.type     = -1;
    Event.type_p   = NULL;
    Event.Target_p = NULL;        

    *((Nh_JS_Event*)(*Object_pp)->data_p) = Event_p == NULL ? Event : *Event_p;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_destroyEventObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    Nh_free(Object_p->data_p);
    Nh_JS_destroyObject(Object_p);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_broadcastEvent(
    Nh_Tab *Tab_p, Nh_JS_Object *Object_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i) {
        NH_CHECK(Nh_JS_dispatchEvent(Nh_getListItem(&Tab_p->Document.Scripts, i), Object_p))
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_broadcastInternalEvent(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_JS_EVENT type, char *type_p, ...)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i)
    {
        va_list args;
        va_start(args, type_p);

        Nh_JS_Script *Script_p = Nh_getListItem(&Tab_p->Document.Scripts, i);

        Nh_JS_Event Event;
        Event.type     = type;
        Event.type_p   = type_p;
        Event.Target_p = Nh_JS_getHTMLElementObject(Script_p, Node_p);

        NH_CHECK(Nh_JS_dispatchInternalEvent(Script_p, &Event, args))

        va_end(args);
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_dispatchEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Object_p)
{
NH_BEGIN()

    Nh_JS_Event *Event_p = Nh_JS_getObject(Object_p, NH_JS_OBJECT_EVENT)->data_p;
    Nh_JS_debugEvent(Script_p, Event_p);

    for (int i = 0; i < Script_p->DOM.EventListeners.count; ++i) 
    {
        Nh_JS_Object *ListenerObject_p = Nh_getListItem(&Script_p->DOM.EventListeners, i);
        Nh_JS_EventListener *Listener_p = ListenerObject_p->data_p;

        if (Listener_p != NULL && Listener_p->eventType == Event_p->type && Listener_p->Target.data_p == Event_p->Target_p) 
        {
            Nh_JS_Result Result = {
                .freeData = false,
                .type     = NH_JS_TYPE_OBJECT,
                .data_p   = Object_p,
            };

            Nh_JS_debugEventListener(Script_p, ListenerObject_p, 1);
            Nh_JS_interpretFunction(Script_p, &ListenerObject_p->Functions_p[0], 1, &Result);
        }
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_dispatchInternalEvent(
    Nh_JS_Script *Script_p, Nh_JS_Event *Event_p, va_list args)
{
NH_BEGIN()

    Nh_JS_debugEvent(Script_p, Event_p);

    for (int i = 0; i < Script_p->DOM.EventListeners.count; ++i) 
    {
        Nh_JS_Object *ListenerObject_p = Nh_getListItem(&Script_p->DOM.EventListeners, i);
        Nh_JS_EventListener *Listener_p = ListenerObject_p->data_p;

        if (Listener_p != NULL && Listener_p->eventType == Event_p->type && Listener_p->Target.data_p == Event_p->Target_p) 
        {
            Nh_JS_Object *Object_p = Nh_allocate(sizeof(Nh_JS_Object));
            NH_CHECK_MEM(Object_p)

            switch (Event_p->type)
            {
                case NH_JS_EVENT_MOUSE_MOVE :
                case NH_JS_EVENT_CLICK :
                    NH_CHECK(Nh_JS_createMouseEventObject(Script_p, &Object_p, Event_p, va_arg(args, void*)))
                    break;

                case NH_JS_EVENT_WHEEL :
                case NH_JS_EVENT_SCROLL :
                    NH_CHECK(Nh_JS_createWheelEventObject(Script_p, &Object_p, Event_p, va_arg(args, void*)))
                    break;

                default :
                    NH_CHECK(Nh_JS_createEventObject(Script_p, &Object_p, Event_p))
            }

            Nh_JS_Result Result = {
                .freeData = true,
                .type     = NH_JS_TYPE_OBJECT,
                .data_p   = Object_p,
            };

            Nh_JS_debugEventListener(Script_p, ListenerObject_p, 1);
            Nh_JS_interpretFunction(Script_p, &ListenerObject_p->Functions_p[0], 1, &Result);
        }
    }

NH_END(NH_SUCCESS)
}

#include NH_CUSTOM_CHECK

char *Nh_JS_stringifyEventData(
    Nh_JS_Event *Event_p, bool newline)
{
NH_BEGIN()

    Nh_String *String_p = Nh_allocateString(NULL);

    char *obj_p = Nh_JS_stringifyObject(Event_p->Target_p, false, true);
    NH_CHECK_NULL(NULL, obj_p)

    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "%s %s", Nh_JS_getEventTypes(NULL)[Event_p->type], obj_p))

    if (newline) {NH_CHECK(NULL, Nh_appendToString(String_p, "\n"))}

    char *chars_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);
    Nh_free(obj_p);

NH_END(chars_p)
}

