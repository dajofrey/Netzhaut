#ifndef NH_ECMASCRIPT_LOG_H
#define NH_ECMASCRIPT_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"

#include "../Parser/CST.h"
#include "../Parser/AST.h"

#endif

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    NH_API_RESULT nh_ecmascript_logDecoder(
        void *handle_p, nh_encoding_UTF32String *Codepoints_p
    );

    NH_API_RESULT nh_ecmascript_logLexer(
        void *handle_p, nh_core_Array *InputElements_p, bool dirty
    );

    NH_API_RESULT nh_ecmascript_logParseTree(
        void *handle_p, nh_ecmascript_ParseNode *ParseNode_p, nh_ecmascript_ParseNode *Parent_p, int depth, bool *branch_p
    );

    NH_API_RESULT nh_ecmascript_logAST(
        void *handle_p, nh_ecmascript_ASTNode *ASTNode_p, nh_ecmascript_ASTNode *Parent_p, int depth, bool *branch_p
    );

    NH_API_RESULT _nh_ecmascript_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_ecmascript_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_ecmascript_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_API_RESULT result, int line
    );

/** @} */

#endif 
