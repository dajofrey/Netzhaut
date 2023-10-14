#ifndef NH_ECMASCRIPT_TYPE_H
#define NH_ECMASCRIPT_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>

typedef struct nh_ecmascript_Object nh_ecmascript_Object;

#endif

/** @addtogroup lib_nhecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_TYPE {
        NH_ECMASCRIPT_TYPE_UNDEFINED,
        NH_ECMASCRIPT_TYPE_NULL,
        NH_ECMASCRIPT_TYPE_BOOLEAN,
        NH_ECMASCRIPT_TYPE_STRING,
        NH_ECMASCRIPT_TYPE_SYMBOL,
        NH_ECMASCRIPT_TYPE_NUMBER,
        NH_ECMASCRIPT_TYPE_BIG_INT,
        NH_ECMASCRIPT_TYPE_OBJECT,
    } NH_ECMASCRIPT_TYPE;

/** @} */

/** @addtogroup lib_nhecmascript_macros
 *  @{
 */

    #define NH_ECMASCRIPT_UNDEFINED 0
    #define NH_ECMASCRIPT_NULL NULL
    #define NH_ECMASCRIPT_BOOLEAN NH_BOOL
    #define NH_ECMASCRIPT_NUMBER double
    #define nh_ecmascript_String nh_encoding_UTF8String

/** @} */

/** @addtogroup lib_nhecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_Symbol {
        nh_ecmascript_String *Description_p;
    } nh_ecmascript_Symbol;

    typedef struct nh_ecmascript_Any {
        NH_ECMASCRIPT_TYPE type;
        void *handle_p;
    } nh_ecmascript_Any;

/** @} */

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

// corresponds to https://tc39.es/ecma262/#sec-type-conversion

    void nh_ecmascript_toPrimitive(
        nh_ecmascript_Any Input, NH_ECMASCRIPT_TYPE preferredType 
    );

    void nh_ecmascript_toBoolean(
        nh_ecmascript_Any Argument 
    );

    void nh_ecmascript_toNumeric(
        nh_ecmascript_Any Value
    );

    void nh_ecmascript_toNumber(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toIntegerOrInfinity(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toInt32(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toUint32(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toInt16(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toUint16( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toInt8( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toUint8( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toUint8Clamp( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toBigInt( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_stringToBigInt( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toBigInt64( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toBigUint64( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toString( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toObject( 
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toPropertyKey(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toLength(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_canonicalNumericIndexString(
        nh_ecmascript_Any Argument
    );

    void nh_ecmascript_toIndex(
        nh_ecmascript_Any Value
    );

// misc 

    nh_ecmascript_Any nh_ecmascript_initAny(
        NH_ECMASCRIPT_TYPE type, void *handle_p
    );

    nh_ecmascript_Any nh_ecmascript_wrapUndefined(
    );

    nh_ecmascript_Any nh_ecmascript_wrapNull(
    );

    nh_ecmascript_Any nh_ecmascript_wrapBoolean(
        NH_ECMASCRIPT_BOOLEAN boolean
    );

    nh_ecmascript_Any nh_ecmascript_wrapString(
        nh_ecmascript_String *String_p
    );

    nh_ecmascript_Any nh_ecmascript_wrapObject(
        nh_ecmascript_Object *Object_p
    );

    nh_ecmascript_String *nh_ecmascript_allocateString(
        size_t chunkSize
    );
    
    NH_ECMASCRIPT_RESULT nh_ecmascript_appendFormatToString(
        nh_ecmascript_String *String_p, NH_BYTE *p, size_t len
    );

    nh_ecmascript_Any nh_ecmascript_wrapNumber(
        NH_ECMASCRIPT_NUMBER *number_p;
    );

    NH_ECMASCRIPT_NUMBER *nh_ecmascript_allocateNumber(
        NH_ECMASCRIPT_NUMBER number
    );

/** @} */

#endif
