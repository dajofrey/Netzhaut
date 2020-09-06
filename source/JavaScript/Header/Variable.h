#ifndef NH_JS_VARIABLE_H
#define NH_JS_VARIABLE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/List.h"

#endif

/** @addtogroup JSStructs Structs
 *  \ingroup JS
 *  @{
 */

    typedef struct Nh_JS_Variable {
        NH_JS_TYPE type;
        NH_JS_KEYWORD kind;
        int depth;
        bool freeData;
        char *name_p;
        void *data_p;
    } Nh_JS_Variable;

/** @} */

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    NH_RESULT Nh_JS_addVariable(
        Nh_List *Variables_p, char *name_p, int depth, NH_JS_KEYWORD kind
    );

    NH_RESULT Nh_JS_updateScriptVariables(
        Nh_JS_Script *Script_p
    );

    void Nh_JS_resetScriptVariables(
        Nh_JS_Script *Script_p
    );

    void Nh_JS_resetBlockVariables(
        Nh_JS_Parsable *Parsable_p
    );

    Nh_JS_Variable *Nh_JS_getVariable(
        Nh_List *Variables_p, int index
    );
    
    Nh_JS_Variable *Nh_JS_getVariableFromName(
        Nh_JS_Parsable *Parsable_p, char *name_p
    );

/** @} */

#endif
