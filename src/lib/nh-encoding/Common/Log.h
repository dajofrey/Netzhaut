#ifndef NH_ENCODING_LOG_H
#define NH_ENCODING_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-encoding_functions
 *  @{
 */

    NH_ENCODING_RESULT _nh_encoding_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ENCODING_RESULT _nh_encoding_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ENCODING_RESULT _nh_encoding_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_ENCODING_RESULT result, int line
    );

    NH_ENCODING_RESULT nh_encoding_logUnicodeCodepointDescriptions(
    );

/** @} */

#endif 
