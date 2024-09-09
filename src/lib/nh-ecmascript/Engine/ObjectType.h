#ifndef NH_ECMASCRIPT_OBJECT_TYPE_H
#define NH_ECMASCRIPT_OBJECT_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "InternalSlots.h"
#include "Properties.h"

#include "../../nh-core/Util/List.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_Object nh_ecmascript_Object;

    typedef struct nh_ecmascript_InternalMethods {
        nh_ecmascript_Object *(*getPrototypeOf_f) (
            nh_ecmascript_Object *This_p
        );
        NH_ECMASCRIPT_BOOLEAN (*setPrototypeOf_f) (
            nh_ecmascript_Object *This_p
        );
        NH_ECMASCRIPT_BOOLEAN (*isExtensible_f) (
            nh_ecmascript_Object *This_p
        );
        NH_ECMASCRIPT_BOOLEAN (*preventExtensions_f) (
            nh_ecmascript_Object *This_p
        );
        nh_ecmascript_PropertyDescriptor (*getOwnProperty_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey
        );
        NH_ECMASCRIPT_BOOLEAN (*defineOwnProperty_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_PropertyDescriptor Desc
        );
        NH_ECMASCRIPT_BOOLEAN (*hasProperty_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey
        );
        nh_ecmascript_Any (*get_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any Receiver
        );
        NH_ECMASCRIPT_BOOLEAN (*set_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any V, nh_ecmascript_Any Receiver
        );
        NH_ECMASCRIPT_BOOLEAN (*delete_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey
        );
        nh_List (*ownPropertyKeys_f) (
            nh_ecmascript_Object *This_p
        );
        nh_ecmascript_Completion (*call_f) (
            nh_ecmascript_Object *This_p, nh_ecmascript_Any ThisArgument, nh_List ArgumentsList
        );
        nh_ecmascript_Object *(*construct_f) (
            nh_List Arguments, nh_ecmascript_Object *Target_p
        );
    } nh_ecmascript_InternalMethods;

    typedef struct nh_ecmascript_Object {
        nh_ecmascript_Properties Properties;
        nh_ecmascript_InternalSlots InternalSlots;
        nh_ecmascript_InternalMethods *InternalMethods_p;
    } nh_ecmascript_Object;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

// corresponds to https://tc39.es/ecma262/#sec-operations-on-objects

    nh_ecmascript_Object *nh_ecmascript_abstractMakeBasicObject(
        const int *lookup_p, int lookupLength
    );

    nh_ecmascript_Completion nh_ecmascript_abstractDefinePropertyOrThrow(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Any ProportyKey, nh_ecmascript_PropertyDescriptor Descriptor
    );

    NH_BOOL nh_ecmascript_abstractHasProperty(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey
    );

    NH_BOOL nh_ecmascript_abstractHasOwnProperty(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey
    );

    NH_BOOL nh_ecmascript_abstractIsExtensible(
        nh_ecmascript_Object *Object_p
    );

    nh_ecmascript_Completion nh_ecmascript_abstractSet(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Any Key, nh_ecmascript_Any Value, NH_BOOL _throw
    );

    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_abstractCreateDataProperty(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any Value
    );

    nh_ecmascript_Completion nh_ecmascript_abstractCall(
        nh_ecmascript_Any F, nh_ecmascript_Any Value, nh_List Arguments
    );

/** @} */

#endif
