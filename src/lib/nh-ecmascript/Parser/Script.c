// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Script.h"
#include "../Engine/ExecutionContext.h"
#include "../Engine/Agent.h"

#include "../Interpreter/Evaluation.h"
#include "../Parser/Lexer.h"
#include "../Parser/AST.h"
#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

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
    NH_CORE_CHECK_2(NULL, nh_ecmascript_runLexer(Script_p, &UnicodeCodepoints, &DirtyInputElements, &CleanInputElements))

    nh_ecmascript_ParseResult Result = nh_ecmascript_parseCST(CleanInputElements, NH_ECMASCRIPT_PARSE_NODE_SCRIPT);
    if (Result.Node_p == NULL || Result.SyntaxErrors.size > 0) {
        nh_core_free(Script_p);
        return NULL;;
    }

    bool branch_p[1024] = {0};
    nh_ecmascript_logParseTree(Script_p, Result.Node_p, NULL, 0, branch_p);

    nh_ecmascript_ASTNode *AST_p = nh_ecmascript_parseAST(Result.Node_p);
    Script_p->ECMAScriptCode_p = AST_p;
    
    return Script_p;
}
