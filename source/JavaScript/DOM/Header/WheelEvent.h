/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_WHEEL_EVENT_H
#define NH_JS_WHEEL_EVENT_H

#include "Event.h"

#include "../../Header/Data.h"
#include "../../Header/Interpreter.h"

#include "../../../Core/Header/System.h"
#include "../../../Core/Header/Tab.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/List.h"

/** @addtogroup Structs
 *  @{
 */
    typedef struct NH_JS_WheelEvent {
        double deltaX, deltaY, deltaZ;
    } NH_JS_WheelEvent;

/** @} */

/** @addtogroup Functions
 *  @{
 */

// API

    Nh_JS_Result NH_JS_WHEEL_EVENT_WheelEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result NH_JS_WHEEL_EVENT_deltaX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_WHEEL_EVENT_deltaY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_WHEEL_EVENT_deltaZ(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_WHEEL_EVENT_deltaMode(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_createWheelEventObject(
        Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_JS_Event *Event_p, NH_JS_WheelEvent *WheelEvent_p
    );

    NH_RESULT Nh_JS_destroyWheelEventObject(
        Nh_JS_Object *Object_p
    );

/** @} */

#endif 

