#ifndef NH_JS_DATA_H
#define NH_JS_DATA_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Interpreter.h"

#include <stddef.h>

#endif

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    typedef Nh_JS_Result (*Nh_JS_FunctionPointer)(void *, void *, int, Nh_JS_Result *);

    const char** Nh_JS_getFunctionNames(
        int type, size_t *size_p
    );

    const char** Nh_JS_getEventTypes(
        size_t *size_p
    );

    const char** Nh_JS_getKeywords(
        size_t *size_p
    );

    Nh_JS_FunctionPointer Nh_JS_getFunctionPointer(
        int type, int index
    );

/** @} */

#endif 
