#ifndef NH_JS_RESULT_H
#define NH_JS_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Script.h"

#include <stdbool.h>

#endif

/** @addtogroup JSStructs Structs
 *  \ingroup JS
 *  @{
 */

    typedef struct Nh_JS_Result {
        bool freeData;
        NH_JS_TYPE type;
        void *data_p;
    } Nh_JS_Result;

/** @} */

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    Nh_JS_Result Nh_JS_getResult(
        NH_JS_TYPE type, bool freeData, void *data_p
    );

    Nh_JS_Result Nh_JS_getNULLResult(
    );

    void Nh_JS_resetResult(
        Nh_JS_Result *Result_p
    );

/** @} */

#endif
