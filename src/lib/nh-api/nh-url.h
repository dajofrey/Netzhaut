#ifndef NH_URL_H
#define NH_URL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-core.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @defgroup api_nh-url nh-url
 *  @brief TODO.
 */

/** @addtogroup api_nh-url
 *  @{
 */

// TYPEDEFS ========================================================================================

    typedef uint16_t NH_URL_PORT;
    typedef uint16_t NH_URL_IPV6_PIECE;
    typedef uint32_t NH_URL_IPV4_ADDRESS;

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_URL_RESULT {
        NH_URL_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_URL_ERROR_NULL_POINTER,
        NH_URL_ERROR_BAD_STATE,
        NH_URL_ERROR_MEMORY_ALLOCATION,
    } NH_URL_RESULT;

    typedef enum NH_URL_HOST {
        NH_URL_HOST_DOMAIN,
        NH_URL_HOST_IPV4_ADDRESS,
        NH_URL_HOST_IPV6_ADDRESS,
        NH_URL_HOST_OPAQUE,
        NH_URL_HOST_EMPTY,
    } NH_URL_HOST;

//  STRUCTS ========================================================================================

    typedef struct nh_url_DomainOrOpaqueHost {
        NH_URL_HOST type;
        nh_ASCIIString String;
    } nh_url_DomainOrOpaqueHost;

    typedef struct nh_url_HostIPv4Address {
        NH_URL_HOST type;
        NH_URL_IPV4_ADDRESS address;
    } nh_url_HostIPv4Address;

    typedef struct nh_url_HostIPv6Address {
        NH_URL_HOST type;
        NH_URL_IPV6_PIECE pieces_p[8];
    } nh_url_HostIPv6Address;

    typedef union nh_url_Host {
        NH_URL_HOST type;
        nh_url_DomainOrOpaqueHost DomainOrOpaque;
        nh_url_HostIPv4Address IPv4;
        nh_url_HostIPv6Address IPv6;
    } nh_url_Host;

    typedef struct nh_url_URL {
        nh_encoding_UTF32String Scheme; 
        nh_encoding_UTF32String UserName;
        nh_encoding_UTF32String Password;
        nh_url_Host *Host_p; 
        NH_URL_PORT *port_p;
        nh_Array Path;                       /**<May contain a path in form of \ref nh_String elements. Otherwise, length is 0.*/
        nh_String *Query_p;    /**<Can be NULL.*/
        nh_String *Fragment_p; /**<Can be NULL.*/
        NH_BOOL cannotBeABaseURL;
    } nh_url_URL;

/** @} */

#endif // NH_URL_H
