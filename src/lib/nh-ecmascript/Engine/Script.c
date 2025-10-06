// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Script.h"
#include "Lexer.h"
#include "CST.h"
#include "ExecutionContext.h"
#include "Instantiation.h"
#include "Evaluation.h"
#include "Agent.h"

#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_ecmascript_prepareText(
    nh_ecmascript_Script *Script_p, nh_encoding_UTF32String *UnicodeCodepoints_p, 
    nh_core_Array *DirtyInputElements_p, nh_core_Array *CleanInputElements_p)
{
    // https://tc39.es/ecma262/#sec-ecmascript-language-lexical-grammar
    nh_ecmascript_logDecoder(Script_p, UnicodeCodepoints_p);

    *DirtyInputElements_p = nh_ecmascript_getInputElements(*UnicodeCodepoints_p);
    nh_ecmascript_logLexer(Script_p, DirtyInputElements_p, true);

    *CleanInputElements_p = nh_ecmascript_discardRedundantInputElements(*DirtyInputElements_p);
    nh_ecmascript_logLexer(Script_p, CleanInputElements_p, false);

    return NH_API_SUCCESS;
}

// https://tc39.es/ecma262/#sec-parse-script
nh_ecmascript_Script *nh_ecmascript_parseScript(
    char *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding)
{
    nh_ecmascript_Script *Script_p = (nh_ecmascript_Script*)nh_core_allocate(sizeof(nh_ecmascript_Script));
    NH_CORE_CHECK_MEM_2(NULL, Script_p)

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
        return NULL;
    }

    nh_core_Array DirtyInputElements, CleanInputElements;
    NH_CORE_CHECK_2(NULL, nh_ecmascript_prepareText(Script_p, &UnicodeCodepoints, &DirtyInputElements, &CleanInputElements))

    nh_ecmascript_ParseResult Result = nh_ecmascript_parseCST(CleanInputElements, NH_ECMASCRIPT_PARSE_NODE_SCRIPT);
    if (Result.Node_p == NULL || Result.SyntaxErrors.size > 0) {
        nh_core_free(Script_p);
        return NULL;;
    }

    bool branch_p[1024] = {0};
    nh_ecmascript_logParseTree(Script_p, Result.Node_p, NULL, 0, branch_p);
    Script_p->ECMAScriptCode_p = Result.Node_p;

    return Script_p;
}

// https://tc39.es/ecma262/#sec-runtime-semantics-scriptevaluation
NH_API_RESULT nh_ecmascript_evaluateScript(
    nh_ecmascript_Script *Script_p)
{
    nh_ecmascript_Environment *GlobalEnvironment_p = Script_p->Realm_p->GlobalEnvironment_p;

    nh_ecmascript_ExecutionContext *ScriptContext_p = nh_ecmascript_allocateExecutionContext();
//    NH_CORE_CHECK_MEM(NewContext_p)

    ScriptContext_p->Function_p = NULL;
    ScriptContext_p->Realm_p    = Script_p->Realm_p;

    ScriptContext_p->ScriptOrModule.isScript = true;
    ScriptContext_p->ScriptOrModule.handle_p = Script_p;

    ScriptContext_p->VariableEnvironment_p = GlobalEnvironment_p; 
    ScriptContext_p->LexicalEnvironment_p  = GlobalEnvironment_p; 
// TODO Suspend the currently running execution context. 

//    nh_core_pushStack(&nh_ecmascript_getCurrentAgent()->ExecutionContextStack, ScriptContext_p);

    nh_ecmascript_ParseNode *ScriptBody_p = Script_p->ECMAScriptCode_p->Children.pp[0];
    nh_ecmascript_Completion Result = nh_ecmascript_globalDeclarationInstantiation(ScriptBody_p, GlobalEnvironment_p);

    if (Result.type == NH_ECMASCRIPT_COMPLETION_NORMAL) {
        Result = nh_ecmascript_evaluateScriptBody(ScriptBody_p);
    }

    return 0;
//    return Result;
}
