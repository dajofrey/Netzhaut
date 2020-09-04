#ifndef NH_OUTPUT_H
#define NH_OUTPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "System.h"
#include "Thread.h"
#include "Font.h"
#include "URI.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup CoreEnums Enums
 *  \ingroup Core
 *  @{
 */

    typedef enum NH_OUTPUT {
        NH_OUTPUT_CSS_SHEET,
        NH_OUTPUT_HTML_DOCUMENT,
        NH_OUTPUT_JS_SCRIPT,
        NH_OUTPUT_CHARS,
    } NH_OUTPUT;

/** @} */

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Output {
        void *pointer;
        NH_OUTPUT type;
    } Nh_Output;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_initOutputs(
        Nh_OutputCallback Callback
    );

    NH_RESULT Nh_freeOutputs(
    );

    void Nh_freeOutput(
        Nh_Output *Output_p
    );

    NH_RESULT Nh_addOutput(
        NH_OUTPUT type, void *pointer, NH_BOOL force
    ); 

    int Nh_getOutputCallbackCount(
    );

/** @} */

#endif
