// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// COMPLETION ======================================================================================

#include "Completion.h"

#include "../Common/Macros.h"

#include "../../nhcore/Util/String.h"

// COMPLETION ======================================================================================

nh_ecmascript_Completion nh_ecmascript_throwTypeError()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.empty   = NH_TRUE;
    Completion.Value.Payload = nh_ecmascript_wrapUndefined();
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = nh_core_initString(8);

NH_ECMASCRIPT_END(Completion)
}

nh_ecmascript_Completion nh_ecmascript_throwReferenceError()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_THROW;
    Completion.Value.empty   = NH_TRUE;
    Completion.Value.Payload = nh_ecmascript_wrapUndefined();
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = nh_core_initString(8);

NH_ECMASCRIPT_END(Completion)
}

nh_ecmascript_Completion nh_ecmascript_normalCompletion(
    nh_ecmascript_Any Argument)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.empty   = NH_FALSE;
    Completion.Value.Payload = Argument;
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = nh_core_initString(8);

NH_ECMASCRIPT_END(Completion)
}

nh_ecmascript_Completion nh_ecmascript_normalEmptyCompletion()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Completion Completion;
    Completion.type = NH_ECMASCRIPT_COMPLETION_NORMAL;
    Completion.Value.empty   = NH_TRUE;
    Completion.Value.Payload = nh_ecmascript_wrapUndefined();
    Completion.Target.empty  = NH_TRUE;
    Completion.Target.String = nh_core_initString(8);

NH_ECMASCRIPT_END(Completion)
}

