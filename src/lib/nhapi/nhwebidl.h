#ifndef NH_WEBIDL_H
#define NH_WEBIDL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhcore.h"
#include "nhencoding.h"

#endif

/** @defgroup api_nhwebidl nhwebidl
 *  @brief Create windows for rendering.
 */

/** @addtogroup api_nhwebidl
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_WEBIDL_RESULT {
        NH_WEBIDL_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_WEBIDL_ERROR_NULL_POINTER,
        NH_WEBIDL_ERROR_BAD_STATE,
        NH_WEBIDL_ERROR_MEMORY_ALLOCATION,
    } NH_WEBIDL_RESULT;

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

// STRUCTS =========================================================================================

    typedef struct nh_webidl_Specification {
        NH_BYTE name_p[255];
        nh_Array Fragments;
    } nh_webidl_Specification;

    typedef struct nh_webidl_InterfaceInheritance {
        const NH_BYTE *interface_p;
        const NH_BYTE *specification_p;
    } nh_webidl_InterfaceInheritance;

    typedef struct nh_webidl_Interface {
        NH_BOOL partial;
        NH_BYTE *name_p;
        nh_Array Members;
        nh_webidl_Specification *Specification_p;
        nh_webidl_InterfaceInheritance *Inheritance_p;
    } nh_webidl_Interface;

    typedef struct nh_webidl_Object {
        nh_webidl_Interface *Interface_p;
        struct nh_webidl_Object *Parent_p;
        struct nh_webidl_Object *Child_p;
        nh_List Attributes;
        nh_List Parts;
        void *internal_p;
    } nh_webidl_Object;

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
    typedef nh_String nh_webidl_ByteString;
    typedef nh_encoding_UTF32String nh_webidl_USVString;
    typedef nh_BigInt nh_webidl_BigInt;

/** @} */

#endif // NH_WEBIDL_H
