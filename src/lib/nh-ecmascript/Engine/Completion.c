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
    Completion.Value.empty   = true;
    Completion.Value.Payload = nh_ecmascript_wrapUndefined();
    Completion.Target.empty  = true;
    Completion.Target.String = nh_core_initString(8);

    return Completion;
}

nh_ecmascript_Completion nh_ecmascript_throwReferenceError()
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.empty   = true;
    Completion.Value.Payload = nh_ecmascript_wrapUndefined();
    Completion.Target.empty  = true;
    Completion.Target.String = nh_core_initString(8);

    return Completion;
}

nh_ecmascript_Completion nh_ecmascript_normalCompletion(
    nh_ecmascript_Any Argument)
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.empty   = false;
    Completion.Value.Payload = Argument;
    Completion.Target.empty  = true;
    Completion.Target.String = nh_core_initString(8);

    return Completion;
}

nh_ecmascript_Completion nh_ecmascript_normalEmptyCompletion()
{
    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.empty   = true;
    Completion.Value.Payload = nh_ecmascript_wrapUndefined();
    Completion.Target.empty  = true;
    Completion.Target.String = nh_core_initString(8);

    return Completion;
}

