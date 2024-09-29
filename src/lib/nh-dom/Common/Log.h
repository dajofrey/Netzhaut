#ifndef NH_DOM_LOG_H
#define NH_DOM_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"

#endif

/** @addtogroup lib_nh-dom_functions
 *  @{
 */

    NH_API_RESULT _nh_dom_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_dom_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_dom_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_API_RESULT result, int line
    );

/** @} */

#endif 
