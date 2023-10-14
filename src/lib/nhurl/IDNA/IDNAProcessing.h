#ifndef NH_URL_IDNA_PROCESSING_H
#define NH_URL_IDNA_PROCESSING_H

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

    NH_URL_RESULT nh_url_unicodeToASCII(
        nh_encoding_UTF32String *DomainName_p, NH_BOOL transitionalProcessing
    );

/** @} */

#endif
