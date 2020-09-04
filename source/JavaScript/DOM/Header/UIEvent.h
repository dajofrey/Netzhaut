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

    Nh_JS_Result NH_JS_UI_EVENT_UIEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result NH_JS_UI_EVENT_detail(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_isChar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_layerX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_layerY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_pageX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_pageY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_sourceCapabilities(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_view(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_UI_EVENT_which(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 

