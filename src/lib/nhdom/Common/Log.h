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

/** @addtogroup lib_nhdom_functions
 *  @{
 */

    NH_DOM_RESULT _nh_dom_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_DOM_RESULT _nh_dom_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_DOM_RESULT _nh_dom_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_DOM_RESULT result, int line
    );

/** @} */

#endif 
