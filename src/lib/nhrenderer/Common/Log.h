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

/** @addtogroup lib_nhrenderer_functions
 *  @{
 */

    NH_RENDERER_RESULT _nh_renderer_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_RENDERER_RESULT _nh_renderer_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_RENDERER_RESULT _nh_renderer_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_RENDERER_RESULT result, int line
    );

/** @} */

#endif 
