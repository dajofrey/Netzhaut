#ifndef NH_URL_HELPER_H
#define NH_URL_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-url_enums
 *  @{
 */

    typedef enum NH_URL_SPECIAL_SCHEME {
        NH_URL_SPECIAL_SCHEME_UNDEFINED,
        NH_URL_SPECIAL_SCHEME_FTP,
        NH_URL_SPECIAL_SCHEME_FILE,
        NH_URL_SPECIAL_SCHEME_HTTP,
        NH_URL_SPECIAL_SCHEME_HTTPS,
        NH_URL_SPECIAL_SCHEME_WS,
        NH_URL_SPECIAL_SCHEME_WSS,
    } NH_URL_SPECIAL_SCHEME;

/** @} */

/** @addtogroup lib_nh-url_functions
 *  @{
 */

    NH_URL_SPECIAL_SCHEME nh_url_getSpecialScheme(
        NH_ENCODING_UTF32 *scheme_p
    );
    
    NH_BOOL nh_url_isURLCodepoint(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_url_isForbiddenHostCodepoint(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_url_inC0PercentEncodeSet(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_url_isWindowsDriveLetter(
        nh_encoding_UTF32String *String_p
    );
    
    NH_BOOL nh_url_isNormalizedWindowsDriveLetter(
        nh_encoding_UTF32String *String_p
    );
    
    NH_BOOL nh_url_stringStartsWithWindowsDriveLetter(
        nh_encoding_UTF32String *String_p
    );

/** @} */

#endif
