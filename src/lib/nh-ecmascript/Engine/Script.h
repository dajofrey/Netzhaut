#ifndef NH_ECMASCRIPT_SCRIPT_H
#define NH_ECMASCRIPT_SCRIPT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Realm.h"
#include "Parser.h"
#include "Environment.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_ScriptHostDefined {
        int nothingYet;
    } nh_ecmascript_ScriptHostDefined;

    // corresponds to https://tc39.es/ecma262/#sec-script-records
    typedef struct nh_ecmascript_Script {
        nh_ecmascript_Realm *Realm_p;
        nh_ecmascript_Environment *Environment_p;
        nh_ecmascript_ParseNode *ECMAScriptCode_p;
        nh_ecmascript_ScriptHostDefined HostDefined;
    } nh_ecmascript_Script;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    // Used by API.
    typedef nh_ecmascript_Script *(*nh_ecmascript_parseScript_f)( 
        NH_BYTE *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding 
    ); 

    // https://tc39.es/ecma262/#sec-parse-script
    nh_ecmascript_Script *nh_ecmascript_parseScript(
        NH_BYTE *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding
    );

    // https://www.ecma-international.org/ecma-262/#sec-runtime-semantics-scriptevaluation
    nh_ecmascript_Completion nh_ecmascript_evaluateScript(
        nh_ecmascript_Script *Script_p
    );

/** @} */

#endif
