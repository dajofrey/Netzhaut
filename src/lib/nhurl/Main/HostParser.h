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

/** @addtogroup lib_nhurl_functions
 *  @{
 */

    NH_URL_RESULT nh_url_parseHost(
        nh_encoding_UTF32String Input, NH_BOOL isNotSpecial, nh_url_Host *Host_p
    );

/** @} */

#endif
