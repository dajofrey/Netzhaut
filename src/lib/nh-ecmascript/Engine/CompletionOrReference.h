#ifndef NH_ECMASCRIPT_ENGINE_COMPLETION_OR_REFERENCE_H
#define NH_ECMASCRIPT_ENGINE_COMPLETION_OR_REFERENCE_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Completion.h"
#include "Reference.h"

// STRUCTS =========================================================================================

typedef struct nh_ecmascript_CompletionOrReference {
    nh_ecmascript_Completion Completion;
    nh_ecmascript_Reference Reference;
} nh_ecmascript_CompletionOrReference;

// FUNCTIONS =======================================================================================

nh_ecmascript_CompletionOrReference nh_ecmascript_normalEmptyCompletionOrReference(
);

nh_ecmascript_CompletionOrReference nh_ecmascript_wrapCompletion(
    nh_ecmascript_Completion Completion
);

nh_ecmascript_CompletionOrReference nh_ecmascript_wrapReference(
    nh_ecmascript_Reference Reference
);

#endif // NH_ECMASCRIPT_ENGINE_COMPLETION_OR_REFERENCE_H
