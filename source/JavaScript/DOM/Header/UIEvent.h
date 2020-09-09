/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_UI_EVENT_H
#define NH_JS_UI_EVENT_H

#include "../../Header/Data.h"
#include "../../Header/Interpreter.h"

#include "../../../Core/Header/System.h"
#include "../../../Core/Header/Tab.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/List.h"

/** @addtogroup Functions
 *  @{
 */

    Nh_JS_Result Nh_JS_UIEvent_UIEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_UIEvent_detail(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_isChar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_layerX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_layerY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_pageX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_pageY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_sourceCapabilities(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_view(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_UIEvent_which(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 

