#ifndef NH_ECMASCRIPT_REFERENCE_H
#define NH_ECMASCRIPT_REFERENCE_H

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

    typedef struct nh_ecmascript_ReferenceBase {
        bool unresolvable;
        nh_ecmascript_Any Value;
        nh_ecmascript_Environment *Environment_p;
    } nh_ecmascript_ReferenceBase;

    typedef struct nh_ecmascript_ReferenceValue {
        bool empty;
        nh_ecmascript_Any Payload;
    } nh_ecmascript_ReferenceValue;

    typedef struct nh_ecmascript_Reference {
        nh_ecmascript_ReferenceBase Base;
        nh_ecmascript_Any ReferencedName;
        NH_ECMASCRIPT_BOOLEAN strict;
        nh_ecmascript_ReferenceValue ThisValue;
    } nh_ecmascript_Reference;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_Reference nh_ecmascript_initReference(
    );

    nh_ecmascript_Reference nh_ecmascript_getIdentifierReference(
        nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN strict
    );

    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isPropertyReference(
        nh_ecmascript_Reference Reference
    );
    
    nh_ecmascript_Completion nh_ecmascript_getValue(
        nh_ecmascript_Completion Completion, nh_ecmascript_Reference Reference
    );

    nh_ecmascript_Completion nh_ecmascript_putValue(
        nh_ecmascript_Reference Reference, nh_ecmascript_Any Value
    );

/** @} */

#endif
