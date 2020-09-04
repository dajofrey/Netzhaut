// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Result.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// GET =============================================================================================

Nh_JS_Result Nh_JS_getResult(
    NH_JS_TYPE type, bool freeData, void *data_p)
{
NH_BEGIN()

    Nh_JS_Result Result;
    Result.type     = type;
    Result.freeData = freeData;
    Result.data_p   = data_p;

NH_END(Result)
}

Nh_JS_Result Nh_JS_getNULLResult()
{
NH_BEGIN()

    Nh_JS_Result Result;
    Result.type     = NH_JS_TYPE_NULL;
    Result.freeData = false;
    Result.data_p   = NULL;

NH_END(Result)
}

// RESET ===========================================================================================

void Nh_JS_resetResult(
    Nh_JS_Result *Result_p)
{
NH_BEGIN()

    if (!Result_p->freeData) {NH_SILENT_END()}

    Nh_JS_free(Result_p->type, Result_p->data_p);
    *Result_p = Nh_JS_getNULLResult();

NH_SILENT_END()
}

