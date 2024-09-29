#ifndef NH_ECMASCRIPT_TEST_AND_COMPARE_H
#define NH_ECMASCRIPT_TEST_AND_COMPARE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "ObjectType.h"

#endif

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

// https://www.262.ecma-international.org/11.0/index.html#sec-testing-and-comparison-operations

//    bool nh_ecmascript_requireObjectCoercible(
//        argument
//    );
//
//    bool nh_ecmascript_isArray(
//        argument
//    );

    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isCallable(
        nh_ecmascript_Any Value
    );

//    bool nh_ecmascript_isConstructor(
//        nh_ecmascript_Type Value
//    );

    nh_ecmascript_Completion nh_ecmascript_isExtensible(
        nh_ecmascript_Object *Object_p
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
//
//    bool nh_ecmascript_sameValue(
//        x, y
//    );
//
//    bool nh_ecmascript_sameValueZero(
//        x, y
//    );
//
//    bool nh_ecmascript_sameValueNonNumeric(
//        x, y
//    );
//
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
