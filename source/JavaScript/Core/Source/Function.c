// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Function.h"
#include "../Header/Macros.h"

#include "../../Header/Data.h"
#include "../../Header/Interpreter.h"

#include "../../../Core/Header/Memory.h"
#include "../../../Core/Header/HashMap.h"

#include <ctype.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result Nh_JS_Function_Function(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Function_arguments(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Function_caller(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Function_displayName(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Function_length(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Function_name(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_JS_DEBUG

NH_RESULT Nh_JS_createFunctionData(
    Nh_JS_Function *Function_p, Nh_JS_Object *Owner_p, int index)
{
NH_BEGIN()

    Function_p->name_p     = NULL;
    Function_p->internal_p = NULL;
    Function_p->external_p = NULL;

    if (Owner_p != NULL && index > -1) {
        Function_p->internal_p = Nh_JS_getFunctionPointer(Owner_p->type, index);
    }

NH_END(NH_SUCCESS)
}

