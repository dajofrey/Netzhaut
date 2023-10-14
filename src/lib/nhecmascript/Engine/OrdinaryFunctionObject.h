#ifndef NH_ECMASCRIPT_ORDINARY_FUNCTION_OBJECT_H
#define NH_ECMASCRIPT_ORDINARY_FUNCTION_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "InternalSlots.h"
#include "Properties.h"
#include "ObjectType.h"
#include "Parser.h"
#include "Environment.h"

#include "../../nhcore/Util/List.h"

#endif

/** @addtogroup lib_nhecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_THIS_MODE {
        NH_ECMASCRIPT_THIS_MODE_LEXICAL,
        NH_ECMASCRIPT_THIS_MODE_NON_LEXICAL,
        NH_ECMASCRIPT_THIS_MODE_STRICT,
        NH_ECMASCRIPT_THIS_MODE_GLOBAL,
    } NH_ECMASCRIPT_THIS_MODE;

/** @} */

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-ordinaryfunctioncreate
    nh_ecmascript_Object *nh_ecmascript_ordinaryFunctionCreate(
        nh_ecmascript_Object *Prototype_p, void *SourceText_p, nh_ecmascript_ParseNode *ParameterList_p, 
        nh_ecmascript_ParseNode *Body_p, NH_ECMASCRIPT_THIS_MODE thisMode, nh_ecmascript_Environment *Scope_p 
    );

    nh_ecmascript_Completion nh_ecmascript_makeConstructor(
        nh_ecmascript_Object *Function_p, NH_BOOL writablePrototype, nh_ecmascript_Object *Prototype_p
    );

    nh_ecmascript_Completion nh_ecmascript_setFunctionLength(
        nh_ecmascript_Object *Function_p, NH_ECMASCRIPT_NUMBER len
    );

    nh_ecmascript_Completion nh_ecmascript_setFunctionName(
        nh_ecmascript_Object *Function_p, nh_ecmascript_Any Name, NH_BYTE *prefix_p
    );

/** @} */

#endif
