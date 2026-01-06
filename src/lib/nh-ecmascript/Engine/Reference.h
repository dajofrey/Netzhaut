#ifndef NH_ECMASCRIPT_REFERENCE_H
#define NH_ECMASCRIPT_REFERENCE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Realm.h"
#include "Environment.h"

#include "../Parser/CST.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

typedef enum {
    NH_ECMASCRIPT_REF_BASE_UNRESOLVABLE,
    NH_ECMASCRIPT_REF_BASE_OBJECT,
    NH_ECMASCRIPT_REF_BASE_ENVIRONMENT,
    NH_ECMASCRIPT_REF_BASE_PRIMITIVE // For things like "string".length
} nh_ecmascript_ReferenceBaseType;

typedef struct nh_ecmascript_Reference {
    // 1. Base Value: The 'location' of the reference
    nh_ecmascript_ReferenceBaseType baseType;
    union {
        nh_ecmascript_Object      *Object_p;
        nh_ecmascript_Environment *Environment_p;
        nh_ecmascript_Value        Primitive; // For primitive coercion
    } Base;

    // 2. Referenced Name: The property name or variable name
    // (Must be String or Symbol)
    nh_ecmascript_Value ReferencedName;

    // 3. Strict Flag
    bool strict;

    // 4. ThisValue: Only used for Super references
    // If it's empty, we use the Base as 'this'.
    struct {
        bool hasValue;
        nh_ecmascript_Value Value;
    } ThisValue;
} nh_ecmascript_Reference;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_Reference nh_ecmascript_initReference(
    );

    nh_ecmascript_Reference nh_ecmascript_getIdentifierReference(
        nh_ecmascript_Environment *Environment_p, nh_ecmascript_Value name, bool strict
    );

    bool nh_ecmascript_isPropertyReference(
        nh_ecmascript_Reference Reference
    );
    
    nh_ecmascript_Completion nh_ecmascript_getValue(
        nh_ecmascript_Reference Reference, nh_ecmascript_Realm *Realm_p
    );

    nh_ecmascript_Completion nh_ecmascript_putValue(
        nh_ecmascript_Reference Reference, nh_ecmascript_Value Value, nh_ecmascript_Realm *Realm_p
    );

/** @} */

#endif
