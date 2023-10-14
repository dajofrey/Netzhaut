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

/** @addtogroup lib_nhhtml_functions
 *  @{
 */

    NH_HTML_RESULT _nh_html_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_HTML_RESULT _nh_html_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_HTML_RESULT _nh_html_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_HTML_RESULT result, int line
    );

    NH_HTML_RESULT nh_html_logDocument(
        NH_BYTE *logId_p, nh_webidl_Object *Document_p
    );

/** @} */

#endif 
