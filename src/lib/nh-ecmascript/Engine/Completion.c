// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// COMPLETION ======================================================================================

#include "Completion.h"

#include "../../nh-core/Util/String.h"

// COMPLETION ======================================================================================

nh_ecmascript_Completion nh_ecmascript_throwTypeError()
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.tag = NH_ECMASCRIPT_VALUE_UNDEFINED;
    Completion.target = NULL;

    return Completion;
}

nh_ecmascript_Completion nh_ecmascript_throwReferenceError()
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.tag = NH_ECMASCRIPT_VALUE_UNDEFINED;
    Completion.target = NULL;

    return Completion;
}

nh_ecmascript_Completion nh_ecmascript_normalCompletion(
    nh_ecmascript_Value Argument)
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value = Argument;
    Completion.target = NULL;

    return Completion;
}

nh_ecmascript_Completion nh_ecmascript_normalEmptyCompletion()
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.tag = NH_ECMASCRIPT_VALUE_UNDEFINED;
    Completion.target = NULL;

    return Completion;
}
