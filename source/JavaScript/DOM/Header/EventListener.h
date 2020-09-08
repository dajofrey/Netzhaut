/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_EVENT_LISTENER_H
#define NH_JS_EVENT_LISTENER_H

#include "Event.h"

#include "../../Header/Interpreter.h"
#include "../../Header/Variable.h"

/** @addtogroup Structs
 *  @{
 */

    typedef struct Nh_JS_EventListener {
        NH_JS_EVENT eventType;  /**<Defines the event type which triggers the listener.*/
        Nh_JS_Variable Target;
    } Nh_JS_EventListener;
   
/** @} */

/** @addtogroup Functions
 *  @{
 */

// API

    Nh_JS_Result NH_JS_EVENT_LISTENER_EventListener(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result NH_JS_EVENT_LISTENER_handleEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_addEventListener(
        Nh_JS_Script *Script_p, NH_JS_EVENT eventType, char *functionName_p, Nh_JS_Object *Target_p
    );

    void Nh_JS_freeEventListeners(
        Nh_JS_Script *Script_p
    );

    char *Nh_JS_stringifyEventListenerData(
        Nh_JS_Object *Object_p, bool newline
    );
  
/** @} */

#endif 
