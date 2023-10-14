// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CompletionOrReference.h"

#include "../Common/Macros.h"

// COMPLETION ======================================================================================

nh_ecmascript_CompletionOrReference nh_ecmascript_normalEmptyCompletionOrReference()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_CompletionOrReference CompletionOrReference;
    CompletionOrReference.Completion = nh_ecmascript_normalEmptyCompletion();
    CompletionOrReference.Reference = nh_ecmascript_initReference(); 

NH_ECMASCRIPT_END(CompletionOrReference)
}

nh_ecmascript_CompletionOrReference nh_ecmascript_wrapCompletion(
    nh_ecmascript_Completion Completion)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_CompletionOrReference CompletionOrReference;
    CompletionOrReference.Completion = Completion;
    CompletionOrReference.Reference = nh_ecmascript_initReference(); 

NH_ECMASCRIPT_END(CompletionOrReference)
}

nh_ecmascript_CompletionOrReference nh_ecmascript_wrapReference(
    nh_ecmascript_Reference Reference)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_CompletionOrReference CompletionOrReference;
    CompletionOrReference.Completion = nh_ecmascript_normalEmptyCompletion();
    CompletionOrReference.Reference = Reference; 

NH_ECMASCRIPT_END(CompletionOrReference)
}

