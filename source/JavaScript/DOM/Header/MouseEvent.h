/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_MOUSE_EVENT_H
#define NH_JS_MOUSE_EVENT_H

#include "Event.h"

#include "../../Header/Data.h"
#include "../../Header/Interpreter.h"

#include "../../../Core/Header/System.h"
#include "../../../Core/Header/Tab.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/List.h"

/** @addtogroup Functions
 *  @{
 */

    typedef struct NH_JS_MouseEvent {
        int position_p[2];
    } NH_JS_MouseEvent;

/** @} */

/** @addtogroup Functions
 *  @{
 */

// API

    Nh_JS_Result Nh_JS_MouseEvent_MouseEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_MouseEvent_altKey(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_button(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_buttons(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_clientX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_clientY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_ctrlKey(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_metaKey(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_movementX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_movementY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_offsetX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_offsetY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_pageX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_pageY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_region(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_relatedTarget(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_screenX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_screenY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_shiftKey(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_which(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_mozInputSource(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_webkitForce(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_x(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_y(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_getModifierState(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_MouseEvent_initMouseEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_createMouseEventObject(
        Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_JS_Event *Event_p, NH_JS_MouseEvent *MouseEvent_p
    );

    NH_RESULT Nh_JS_destroyMouseEventObject(
        Nh_JS_Object *Object_p
    );

/** @} */

#endif 

