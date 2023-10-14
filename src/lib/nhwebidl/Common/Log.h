#ifndef NH_WEBIDL_LOG_H
#define NH_WEBIDL_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include "../Runtime/Tokenizer.h"
#include "../Runtime/Parser.h"
#include "../Runtime/Runtime.h"

#endif

/** @addtogroup lib_nhwebidl_functions
 *  @{
 */

    NH_WEBIDL_RESULT _nh_webidl_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_WEBIDL_RESULT _nh_webidl_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_WEBIDL_RESULT _nh_webidl_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_WEBIDL_RESULT result, int line
    );

    NH_WEBIDL_RESULT nh_webidl_logTokens(
        NH_BYTE *fragmentName_p, nh_Array *Tokens_p, NH_BOOL dirty
    );

    NH_WEBIDL_RESULT nh_webidl_logParseTree(
        NH_BYTE *fragmentName_p, nh_webidl_ParseNode *ParseNode_p
    );

    NH_WEBIDL_RESULT nh_webidl_logFragment(
        NH_BYTE *specification_p, nh_webidl_Fragment *Fragment_p
    );

/** @} */

#endif 
