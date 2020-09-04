#ifndef NH_JS_INTERPRETER_H
#define NH_JS_INTERPRETER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Script.h"
#include "Result.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/System.h"
#include "../../Core/Header/List.h"

#include <stdbool.h>

typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_JS_Function Nh_JS_Function;

#endif

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    typedef Nh_JS_Result (*NH_JS_InterpreterPointer)(void *, void *);

    NH_RESULT Nh_JS_interpretScript(
        Nh_JS_Script *Script_p
    );
    
    Nh_JS_Result Nh_JS_interpretFunction(
        Nh_JS_Script *Script_p, Nh_JS_Object *Object_p, int aCount, Nh_JS_Result *Arguments_p
    );

    NH_JS_InterpreterPointer Nh_JS_getInterpreterPointer(
        int type
    );

/** @} */

#endif
