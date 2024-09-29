#ifndef NH_URL_PARSER_H
#define NH_URL_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "HostParser.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-url_enums
 *  @{
 */

    typedef enum NH_URL_PARSER_STATE {
        NH_URL_PARSER_STATE_UNDEFINED = 0,
        NH_URL_PARSER_STATE_SCHEME_START,
        NH_URL_PARSER_STATE_SCHEME,
        NH_URL_PARSER_STATE_NO_SCHEME,
        NH_URL_PARSER_STATE_SPECIAL_RELATIVE_OR_AUTHORITY,
        NH_URL_PARSER_STATE_PATH_OR_AUTHORITY,
        NH_URL_PARSER_STATE_RELATIVE,
        NH_URL_PARSER_STATE_RELATIVE_SLASH,
        NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_SLASHES,
        NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_IGNORE_SLASHES,
        NH_URL_PARSER_STATE_AUTHORITY,
        NH_URL_PARSER_STATE_HOST,
        NH_URL_PARSER_STATE_HOSTNAME,
        NH_URL_PARSER_STATE_PORT,
        NH_URL_PARSER_STATE_FILE,
        NH_URL_PARSER_STATE_FILE_SLASH,
        NH_URL_PARSER_STATE_FILE_HOST,
        NH_URL_PARSER_STATE_PATH_START,
        NH_URL_PARSER_STATE_PATH,
        NH_URL_PARSER_STATE_CANNOT_BE_A_BASE_URL_PATH,
        NH_URL_PARSER_STATE_QUERY,
        NH_URL_PARSER_STATE_FRAGMENT,
    } NH_URL_PARSER_STATE;

/** @} */

// TYPEDEFS ======================================================================================== 

    typedef uint16_t NH_URL_PORT; 

    typedef struct nh_url_URL { 
        nh_encoding_UTF32String Scheme; 
        nh_encoding_UTF32String UserName; 
        nh_encoding_UTF32String Password; 
        nh_url_Host *Host_p; 
        NH_URL_PORT *port_p; 
        nh_Array Path;                       /**<May contain a path in form of \ref nh_String elements.*/ 
        nh_String *Query_p;    /**<Can be NULL.*/ 
        nh_String *Fragment_p; /**<Can be NULL.*/ 
        bool cannotBeABaseURL; 
    } nh_url_URL; 

/** @addtogroup lib_nh-url_functions
 *  @{
 */

    // https://url.spec.whatwg.org/#concept-basic-url-parser
    nh_url_URL *nh_url_parseBasicURL(
        nh_encoding_UTF32String Input, nh_url_URL *Base_p, nh_url_URL *URL_p, NH_URL_PARSER_STATE stateOverride
    );

    nh_url_URL *nh_url_runURLParser(
        nh_encoding_UTF32String Input, nh_url_URL *Base_p, nh_url_URL *URL_p
    );

/** @} */

#endif
