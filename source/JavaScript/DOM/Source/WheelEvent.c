// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/WheelEvent.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Memory.h"

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result NH_JS_WHEEL_EVENT_WheelEvent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_WHEEL_EVENT_deltaX(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 0) // get 
    {
        NH_JS_WheelEvent *WheelEvent_p = Function_p->Inherit_p->data_p;
        NH_END(Nh_JS_getResult(NH_JS_TYPE_NUMBER, false, &WheelEvent_p->deltaX))
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_WHEEL_EVENT_deltaY(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 0) // get 
    {
        NH_JS_WheelEvent *WheelEvent_p = Function_p->Inherit_p->data_p;
        NH_END(Nh_JS_getResult(NH_JS_TYPE_NUMBER, false, &WheelEvent_p->deltaY))
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_WHEEL_EVENT_deltaZ(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 0) // get 
    {
        NH_JS_WheelEvent *WheelEvent_p = Function_p->Inherit_p->data_p;
        NH_END(Nh_JS_getResult(NH_JS_TYPE_NUMBER, false, &WheelEvent_p->deltaZ))
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_WHEEL_EVENT_deltaMode(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG

NH_RESULT Nh_JS_createWheelEventObject(
    Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_JS_Event *Event_p, NH_JS_WheelEvent *WheelEvent_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_WHEEL_EVENT, Object_pp))
    (*Object_pp)->data_p = Nh_allocate(sizeof(NH_JS_WheelEvent));
    NH_CHECK_MEM((*Object_pp)->data_p);
    *((NH_JS_WheelEvent*)(*Object_pp)->data_p) = *WheelEvent_p;

    Nh_JS_Object *Inherit_p = Nh_JS_getInheritedObject((*Object_pp), NH_JS_OBJECT_EVENT);
    Inherit_p->data_p = Nh_allocate(sizeof(Nh_JS_Event));
    NH_CHECK_MEM(Inherit_p->data_p);
    *((Nh_JS_Event*)Inherit_p->data_p) = *Event_p;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_destroyWheelEventObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    Nh_JS_Object *Inherit_p = Nh_JS_getInheritedObject(Object_p, NH_JS_OBJECT_EVENT);
    Nh_free(Inherit_p->data_p);

    Nh_JS_destroyObject(Object_p);
    Nh_free(Object_p->data_p);

NH_END(NH_SUCCESS)
}

