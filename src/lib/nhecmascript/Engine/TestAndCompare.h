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

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

// https://www.262.ecma-international.org/11.0/index.html#sec-testing-and-comparison-operations

//    NH_BOOL nh_ecmascript_requireObjectCoercible(
//        argument
//    );
//
//    NH_BOOL nh_ecmascript_isArray(
//        argument
//    );

    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isCallable(
        nh_ecmascript_Any Value
    );

//    NH_BOOL nh_ecmascript_isConstructor(
//        nh_ecmascript_Type Value
//    );

    nh_ecmascript_Completion nh_ecmascript_isExtensible(
        nh_ecmascript_Object *Object_p
    );

//    NH_BOOL nh_ecmascript_isInteger(
//        argument
//    );
//
//    NH_BOOL nh_ecmascript_isNonNegativeInteger(
//        argument
//    );
//
//    NH_BOOL nh_ecmascript_isPropertyKey(
//        nh_ecmascript_Type Argument
//    );
//
//    NH_BOOL nh_ecmascript_isRegExp(
//        argument
//    );
//
//    NH_BOOL nh_ecmascript_isStringPrefix(
//        p, q
//    );
//
//    NH_BOOL nh_ecmascript_sameValue(
//        x, y
//    );
//
//    NH_BOOL nh_ecmascript_sameValueZero(
//        x, y
//    );
//
//    NH_BOOL nh_ecmascript_sameValueNonNumeric(
//        x, y
//    );
//
//    NH_BOOL nh_ecmascript_abstractRelationalComparison(
//    );
//
//    NH_BOOL nh_ecmascript_abstractEqualityComparison(
//    );
//
//    NH_BOOL nh_ecmascript_strictEqualityComparison(
//    );

/** @} */

#endif
