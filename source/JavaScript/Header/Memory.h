#ifndef NH_JS_MEMORY_H
#define NH_JS_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#endif

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    void Nh_JS_free(
        NH_JS_TYPE type, void *data_p
    );

/** @} */

#endif
