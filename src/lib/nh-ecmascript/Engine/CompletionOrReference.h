#ifndef NH_ECMASCRIPT_COMPLETION_OR_REFERENCE_H
#define NH_ECMASCRIPT_COMPLETION_OR_REFERENCE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "Reference.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_CompletionOrReference {
        nh_ecmascript_Completion Completion;
        nh_ecmascript_Reference Reference;
    } nh_ecmascript_CompletionOrReference;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_CompletionOrReference nh_ecmascript_normalEmptyCompletionOrReference(
    );

    nh_ecmascript_CompletionOrReference nh_ecmascript_wrapCompletion(
        nh_ecmascript_Completion Completion
    );

    nh_ecmascript_CompletionOrReference nh_ecmascript_wrapReference(
        nh_ecmascript_Reference Reference
    );

/** @} */

#endif
