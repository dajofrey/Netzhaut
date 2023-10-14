#ifndef NH_ECMASCRIPT_LOG_H
#define NH_ECMASCRIPT_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"

#include "../Engine/Parser.h"

#endif

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    NH_ECMASCRIPT_RESULT nh_ecmascript_logDecoder(
        void *handle_p, nh_encoding_UTF32String *Codepoints_p
    );

    NH_ECMASCRIPT_RESULT nh_ecmascript_logLexer(
        void *handle_p, nh_Array *InputElements_p, NH_BOOL dirty
    );

    NH_ECMASCRIPT_RESULT nh_ecmascript_logParseTree(
        void *handle_p, nh_ecmascript_ParseNode *ParseNode_p, nh_ecmascript_ParseNode *Parent_p, int depth, NH_BOOL *branch_p
    );

    NH_ECMASCRIPT_RESULT _nh_ecmascript_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ECMASCRIPT_RESULT _nh_ecmascript_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ECMASCRIPT_RESULT _nh_ecmascript_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_ECMASCRIPT_RESULT result, int line
    );

/** @} */

#endif 
