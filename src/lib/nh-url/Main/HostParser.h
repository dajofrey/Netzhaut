#ifndef NH_URL_HOST_PARSER_H
#define NH_URL_HOST_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

  typedef uint16_t NH_URL_IPV6_PIECE; 
  typedef uint32_t NH_URL_IPV4_ADDRESS; 

  typedef enum NH_URL_HOST { 
      NH_URL_HOST_DOMAIN, 
      NH_URL_HOST_IPV4_ADDRESS, 
      NH_URL_HOST_IPV6_ADDRESS, 
      NH_URL_HOST_OPAQUE, 
      NH_URL_HOST_EMPTY, 
  } NH_URL_HOST; 

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

/** @addtogroup lib_nh-url_functions
 *  @{
 */

    NH_API_RESULT nh_url_parseHost(
        nh_encoding_UTF32String Input, bool isNotSpecial, nh_url_Host *Host_p
    );

/** @} */

#endif
