// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../Engine/Lexer.h"
#include "../../nh-core/System/Logger.h"

#include <stdio.h>

// DEFINE ==========================================================================================

#define NH_ECMASCRIPT_LOG_PREFIX "nh-ecmascript"
#define NH_ECMASCRIPT_LOG_CATEGORY_LOGGING "Logging"
#define NH_ECMASCRIPT_LOG_CATEGORY_DECODER "Decoder"

// LOG =============================================================================================

NH_API_RESULT _nh_ecmascript_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_ecmascript_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_ecmascript_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

//NH_API_RESULT nh_ecmascript_finishLogging(
//    nh_ecmascript_Script *Script_p, nh_core_Array *UnicodeCodePoints_p)
//{
//    char message_p[127] = {'\0'};
//    sprintf(message_p, "[%s:%d:%s](FINISH)", NH_ECMASCRIPT_LOG_PREFIX, NH_ECMASCRIPT_LOG_CATEGORY_LOGGING);
//    nh_core_sendLogMessage(message_p);
//
//return NH_API_SUCCESS;
//}

NH_API_RESULT nh_ecmascript_logDecoder(
    void *handle_p, nh_encoding_UTF32String *Codepoints_p)
{
//    for (int i = 0; i < Codepoints_p->length; ++i) 
//    {
//        char message_p[512] = {'\0'};
//        sprintf(
//            message_p, "[nh-ecmascript:%p:Decoder]{%s}", handle_p,
//            nh_encoding_getDescription(Codepoints_p->p[i])
//        );
//        nh_core_sendLogMessage(message_p);
//    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_ecmascript_logLexer(
    void *handle_p, nh_core_Array *InputElements_p, bool dirty)
{
//    for (int i = 0; i < InputElements_p->length; ++i) 
//    {
//        nh_ecmascript_InputElement *InputElement_p = &((nh_ecmascript_InputElement*)InputElements_p->p)[i];
//        char message_p[512] = {'\0'};
//
//        nh_core_String String = nh_core_initString(64);
//        nh_core_appendToString(&String, InputElement_p->String.p, InputElement_p->String.length);
//
//        for (int i = 0; i < String.length; ++i) {
//            if (String.p[i] == '\n' || String.p[i] == '\r') {String.p[i] = ' ';}
//        }
//
//        sprintf(
//            message_p, dirty ? "[nh-ecmascript:%p:LexerDirty]{%s [%s]}" : "[nh-ecmascript:%p:LexerClean]{%s [%s]}",
//            handle_p, NH_ECMASCRIPT_INPUT_ELEMENTS_PP[InputElement_p->type], String.p 
//        );
//
//        nh_core_sendLogMessage(message_p);
//        nh_core_freeString(&String);
//    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_ecmascript_logParseTree(
    void *handle_p, nh_ecmascript_ParseNode *ParseNode_p, nh_ecmascript_ParseNode *Parent_p, int depth, bool *branch_p)
{
    char message_p[1200] = {'\0'};
    char indent_p[1024] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == true ? '|' : ' ';
    }

    if (ParseNode_p->Value_p == NULL) {
        sprintf(message_p, "%s%s", indent_p, NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[ParseNode_p->type]);
    }
    else {
        sprintf(message_p, "%sTERMINAL %s", indent_p, ParseNode_p->Value_p->String.p);
    }

    char addr_p[255];
    sprintf(addr_p, "nh-ecmascript:%p:ParseTree", handle_p);
    nh_core_sendLogMessage(addr_p, NULL, message_p);

    branch_p[depth] = true;
    if (Parent_p != NULL &&  ParseNode_p == Parent_p->Children.pp[Parent_p->Children.size - 1]) {
        branch_p[depth - 1] = false;
    }

    for (int i = 0; i < ParseNode_p->Children.size; ++i) {
        nh_ecmascript_logParseTree(handle_p, ParseNode_p->Children.pp[i], ParseNode_p, depth + 1, branch_p);
    }

    return NH_API_SUCCESS;
}

