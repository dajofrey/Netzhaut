#ifndef NH_ECMASCRIPT_COMPLETION_H
#define NH_ECMASCRIPT_COMPLETION_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Type.h"

// https://tc39.es/ecma262/#sec-completion-record-specification-type
typedef enum NH_ECMASCRIPT_COMPLETION_E {
    NH_ECMASCRIPT_COMPLETION_NORMAL = 0,
    NH_ECMASCRIPT_COMPLETION_BREAK,
    NH_ECMASCRIPT_COMPLETION_CONTINUE,
    NH_ECMASCRIPT_COMPLETION_RETURN,
    NH_ECMASCRIPT_COMPLETION_THROW,
} NH_ECMASCRIPT_COMPLETION_E;

typedef struct nh_ecmascript_Completion {
    NH_ECMASCRIPT_COMPLETION_E type;
    nh_ecmascript_Value Value;  // Use your new Value struct here
    void *target;               // Usually a String pointer or NULL
} nh_ecmascript_Completion;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_Completion nh_ecmascript_await(
    );

    nh_ecmascript_Completion nh_ecmascript_throwError(
        nh_ecmascript_Any Argument 
    );

    nh_ecmascript_Completion nh_ecmascript_throwTypeError(
    );

    nh_ecmascript_Completion nh_ecmascript_throwReferenceError(
    );

    nh_ecmascript_Completion nh_ecmascript_normalCompletion(
        nh_ecmascript_Any Argument
    );

    nh_ecmascript_Completion nh_ecmascript_normalEmptyCompletion(
    );

/** @} */

#endif
