#ifndef NH_HTML_LOG_H
#define NH_HTML_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    NH_API_RESULT _nh_html_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_html_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_html_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_API_RESULT result, int line
    );

    NH_API_RESULT nh_html_logDocument(
        char *logId_p, nh_webidl_Object *Document_p
    );

/** @} */

#endif 
