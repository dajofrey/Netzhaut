#ifndef NH_RENDERER_LOG_H
#define NH_RENDERER_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    NH_API_RESULT _nh_renderer_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_renderer_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_renderer_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_API_RESULT result, int line
    );

/** @} */

#endif 
