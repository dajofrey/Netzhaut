// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Script.h"
#include "Lexer.h"
#include "Parser.h"
#include "ExecutionContext.h"
#include "Instantiation.h"
#include "Evaluation.h"
#include "Agent.h"

#include "../../nhcore/System/Memory.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include <string.h>
#include <stdio.h>

// PARSE ===========================================================================================

static NH_ECMASCRIPT_RESULT nh_ecmascript_prepareText(
    nh_ecmascript_Script *Script_p, nh_encoding_UTF32String *UnicodeCodepoints_p, 
    nh_Array *DirtyInputElements_p, nh_Array *CleanInputElements_p)
{
NH_ECMASCRIPT_BEGIN()

    // https://tc39.es/ecma262/#sec-ecmascript-language-lexical-grammar
    nh_ecmascript_logDecoder(Script_p, UnicodeCodepoints_p);

    *DirtyInputElements_p = nh_ecmascript_getInputElements(*UnicodeCodepoints_p);
    nh_ecmascript_logLexer(Script_p, DirtyInputElements_p, NH_TRUE);

    *CleanInputElements_p = nh_ecmascript_discardRedundantInputElements(*DirtyInputElements_p);
    nh_ecmascript_logLexer(Script_p, CleanInputElements_p, NH_FALSE);

NH_ECMASCRIPT_END(NH_ECMASCRIPT_SUCCESS)
}

// https://tc39.es/ecma262/#sec-parse-script
nh_ecmascript_Script *nh_ecmascript_parseScript(
    NH_BYTE *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Script *Script_p = nh_core_allocate(sizeof(nh_ecmascript_Script));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Script_p)

    Script_p->Realm_p = Realm_p;
    Script_p->Environment_p = NULL;

    nh_encoding_UTF32String UnicodeCodepoints = nh_encoding_initUTF32(128);

//    switch (encoding) 
//    {
//        case NH_UNICODE_ENCODING_UTF8 :
            UnicodeCodepoints = nh_encoding_decodeUTF8(sourceText_p, strlen(sourceText_p), NULL);
//            break;
//    }

    if (UnicodeCodepoints.length <= 0) {
        nh_core_free(Script_p);
        NH_ECMASCRIPT_END(NULL)
    }

    nh_Array DirtyInputElements, CleanInputElements;
    NH_ECMASCRIPT_CHECK_2(NULL, nh_ecmascript_prepareText(Script_p, &UnicodeCodepoints, &DirtyInputElements, &CleanInputElements))

    nh_ecmascript_ParseResult Result = nh_ecmascript_parseText(CleanInputElements, NH_ECMASCRIPT_PARSE_NODE_SCRIPT);
    if (Result.Node_p == NULL || Result.SyntaxErrors.size > 0) {
        nh_core_free(Script_p);
        NH_ECMASCRIPT_END(NULL);
    }

    NH_BOOL branch_p[1024] = {0};
    nh_ecmascript_logParseTree(Script_p, Result.Node_p, NULL, 0, branch_p);
    Script_p->ECMAScriptCode_p = Result.Node_p;

NH_ECMASCRIPT_END(Script_p)
}

// EVALUATE ========================================================================================

// https://tc39.es/ecma262/#sec-runtime-semantics-scriptevaluation
nh_ecmascript_Completion nh_ecmascript_evaluateScript(
    nh_ecmascript_Script *Script_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Environment *GlobalEnvironment_p = Script_p->Realm_p->GlobalEnvironment_p;

    nh_ecmascript_ExecutionContext *ScriptContext_p = nh_ecmascript_allocateExecutionContext();
//    NH_ECMASCRIPT_CHECK_MEM(NewContext_p)

    ScriptContext_p->Function_p = NULL;
    ScriptContext_p->Realm_p    = Script_p->Realm_p;

    ScriptContext_p->ScriptOrModule.isScript = NH_TRUE;
    ScriptContext_p->ScriptOrModule.handle_p = Script_p;

    ScriptContext_p->VariableEnvironment_p = GlobalEnvironment_p; 
    ScriptContext_p->LexicalEnvironment_p  = GlobalEnvironment_p; 
// TODO Suspend the currently running execution context. 

    nh_pushStack(&nh_ecmascript_getCurrentAgent()->ExecutionContextStack, ScriptContext_p);

    nh_ecmascript_ParseNode *ScriptBody_p = Script_p->ECMAScriptCode_p->Children.pp[0];
    nh_ecmascript_Completion Result = nh_ecmascript_globalDeclarationInstantiation(ScriptBody_p, GlobalEnvironment_p);

    if (Result.type == NH_ECMASCRIPT_COMPLETION_NORMAL) {
        Result = nh_ecmascript_evaluateScriptBody(ScriptBody_p);
    }

NH_ECMASCRIPT_END(Result)
}

