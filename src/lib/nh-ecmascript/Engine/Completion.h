#ifndef NH_ECMASCRIPT_COMPLETION_H
#define NH_ECMASCRIPT_COMPLETION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Type.h"

#endif

/** @addtogroup lib_nh-ecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_COMPLETION {
        NH_ECMASCRIPT_COMPLETION_NORMAL,
        NH_ECMASCRIPT_COMPLETION_BREAK,
        NH_ECMASCRIPT_COMPLETION_CONTINUE,
        NH_ECMASCRIPT_COMPLETION_RETURN,
        NH_ECMASCRIPT_COMPLETION_THROW,
    } NH_ECMASCRIPT_COMPLETION;

/** @} */

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_CompletionValue {
        NH_BOOL empty;
        nh_ecmascript_Any Payload;
    } nh_ecmascript_CompletionValue;

    typedef struct nh_ecmascript_CompletionTarget {
        NH_BOOL empty;
        nh_encoding_UTF8String String;
    } nh_ecmascript_CompletionTarget;

    // https://tc39.es/ecma262/#sec-completion-record-specification-type
    typedef struct nh_ecmascript_Completion {
        NH_ECMASCRIPT_COMPLETION type;
        nh_ecmascript_CompletionValue Value;
        nh_ecmascript_CompletionTarget Target;
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
