#ifndef NH_WEBIDL_TYPE_H
#define NH_WEBIDL_TYPE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Object.h"
#include "Parser.h"

#include "../Common/Includes.h"

#include "../../nh-core/Util/BigInt.h"

typedef enum NH_WEBIDL_TYPE { 
    NH_WEBIDL_TYPE_WILDCARD, 
    NH_WEBIDL_TYPE_ANY, 
    NH_WEBIDL_TYPE_UNDEFINED, 
    NH_WEBIDL_TYPE_BOOLEAN, 
    NH_WEBIDL_TYPE_BYTE, 
    NH_WEBIDL_TYPE_OCTET, 
    NH_WEBIDL_TYPE_SHORT, 
    NH_WEBIDL_TYPE_UNSIGNED_SHORT, 
    NH_WEBIDL_TYPE_LONG, 
    NH_WEBIDL_TYPE_UNSIGNED_LONG, 
    NH_WEBIDL_TYPE_LONG_LONG, 
    NH_WEBIDL_TYPE_UNSIGNED_LONG_LONG, 
    NH_WEBIDL_TYPE_FLOAT, 
    NH_WEBIDL_TYPE_UNRESTRICTED_FLOAT, 
    NH_WEBIDL_TYPE_DOUBLE, 
    NH_WEBIDL_TYPE_UNRESTRICTED_DOUBLE, 
    NH_WEBIDL_TYPE_BIGINT, 
    NH_WEBIDL_TYPE_DOMSTRING, 
    NH_WEBIDL_TYPE_BYTESTRING, 
    NH_WEBIDL_TYPE_USVSTRING, 
    NH_WEBIDL_TYPE_OBJECT, 
    NH_WEBIDL_TYPE_SYMBOL, 
} NH_WEBIDL_TYPE; 

typedef struct nh_webidl_Any { 
    NH_WEBIDL_TYPE type; 
    void *value_p; 
} nh_webidl_Any; 

// TYPEDEFS ======================================================================================== 

typedef struct nh_webidl_ParseNode nh_webidl_ParseNode; 

typedef char NH_WEBIDL_BYTE; 
typedef unsigned char NH_WEBIDL_OCTET; 
typedef short NH_WEBIDL_SHORT; 
typedef unsigned short NH_WEBIDL_UNSIGNED_SHORT; 
typedef long NH_WEBIDL_LONG; 
typedef unsigned long NH_WEBIDL_UNSIGNED_LONG; 
typedef long long NH_WEBIDL_LONG_LONG; 
typedef unsigned long long NH_WEBIDL_UNSIGNED_LONG_LONG; 
typedef float NH_WEBIDL_FLOAT; 
typedef float NH_WEBIDL_UNRESTRICTED_FLOAT; 
typedef double NH_WEBIDL_DOUBLE; 
typedef double NH_WEBIDL_UNRESTRICTED_DOUBLE; 
typedef unsigned long NH_WEBIDL_UNSIGNED_LONG; 
typedef long long NH_WEBIDL_LONG_LONG; 
typedef unsigned long long NH_WEBIDL_UNSIGNED_LONG_LONG; 

typedef nh_encoding_UTF8String nh_webidl_DOMString; 
typedef nh_core_String nh_webidl_ByteString; 
typedef nh_encoding_UTF32String nh_webidl_USVString; 
typedef nh_core_BigInt nh_webidl_BigInt; 

/** @addtogroup lib_nh-webidl_functions Functions
 *  @{
 */

    NH_WEBIDL_TYPE nh_webidl_getType(
        nh_webidl_ParseNode *Type_p
    );

    size_t nh_webidl_getAllocationSize(
        NH_WEBIDL_TYPE type
    );

/** @} */

#endif
