#ifndef NH_JS_DEBUG_H
#define NH_JS_DEBUG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Core/Header/Object.h"
#include "../DOM/Header/Event.h"

#endif

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    void Nh_JS_debugEventListener(
        Nh_JS_Script *Script_p, Nh_JS_Object *Listener_p, int action
    );

    void Nh_JS_debugEvent(
        Nh_JS_Script *Script_p, Nh_JS_Event *Event_p
    );

    void Nh_JS_debugInterpreter(
        Nh_JS_Script *Script_p, const char *func_p, void *data_p, int action
    );

    void Nh_JS_debugFunction(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p,
        const char *func_p, int action
    );

    void Nh_JS_debugScript(
        Nh_JS_Script *Script_p
    );

/** @} */

#endif 
