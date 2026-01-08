#ifndef NH_ECMASCRIPT_TEST_AND_COMPARE_H
#define NH_ECMASCRIPT_TEST_AND_COMPARE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Object.h"

#endif

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

// https://www.262.ecma-international.org/11.0/index.html#sec-testing-and-comparison-operations

    nh_ecmascript_Completion nh_ecmascript_requireObjectCoercible(
        nh_ecmascript_Value v, nh_ecmascript_Realm *Realm_p
    );

    bool nh_ecmascript_isArray(
        nh_ecmascript_Value v, nh_ecmascript_Realm *Realm_p
    );

    bool nh_ecmascript_isObject(
        nh_ecmascript_Value Value
    );


    bool nh_ecmascript_isCallable(
        nh_ecmascript_Value Value
    );

    bool nh_ecmascript_isUndefined(
        nh_ecmascript_Value Value
    );


    bool nh_ecmascript_isConstructor(
        nh_ecmascript_Value Value
    );

    nh_ecmascript_Completion nh_ecmascript_isExtensible(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Realm *Realm_p
    );

//    bool nh_ecmascript_isInteger(
//        argument
//    );
//
//    bool nh_ecmascript_isNonNegativeInteger(
//        argument
//    );
//
//    bool nh_ecmascript_isPropertyKey(
//        nh_ecmascript_Type Argument
//    );
//
//    bool nh_ecmascript_isRegExp(
//        argument
//    );
//
//    bool nh_ecmascript_isStringPrefix(
//        p, q
//    );

    bool nh_ecmascript_sameValue(
        nh_ecmascript_Value x, nh_ecmascript_Value y
    );

//    bool nh_ecmascript_sameValueZero(
//        x, y
//    );
//
    bool nh_ecmascript_sameValueNonNumeric(
        nh_ecmascript_Value x, nh_ecmascript_Value y
    );

//    bool nh_ecmascript_abstractRelationalComparison(
//    );
//
//    bool nh_ecmascript_abstractEqualityComparison(
//    );
//
//    bool nh_ecmascript_strictEqualityComparison(
//    );

/** @} */

#endif
