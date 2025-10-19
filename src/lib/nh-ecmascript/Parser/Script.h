#ifndef NH_ECMASCRIPT_SCRIPT_H
#define NH_ECMASCRIPT_SCRIPT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Engine/Realm.h"
#include "../Engine/Environment.h"

#include "../Parser/CST.h"
#include "../Parser/AST.h"
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
        nh_ecmascript_ASTNode *ECMAScriptCode_p;
        nh_ecmascript_ScriptHostDefined HostDefined;
    } nh_ecmascript_Script;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-parse-script
    nh_ecmascript_Script *nh_ecmascript_parseScript(
        char *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding
    );

/** @} */

#endif
