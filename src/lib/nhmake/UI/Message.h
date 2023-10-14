#ifndef NH_MAKE_UI_MESSAGE_H
#define NH_MAKE_UI_MESSAGE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Result.h"

#endif

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef NH_MAKE_RESULT (*nh_make_messagef_f)(
        NH_BYTE *format_p, ...
    );

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_operationf(
        NH_BYTE *format_p, ...
    );
    
    NH_MAKE_RESULT nh_make_messagef(
        NH_BYTE *format_p, ...
    );

    NH_MAKE_RESULT nh_make_noticef(
        NH_BYTE *format_p, ...
    );

    NH_MAKE_RESULT nh_make_externalMessage(
        NH_BYTE *prepend_p, NH_BYTE *message_p
    );

    NH_MAKE_RESULT nh_make_exitMessage(
        NH_MAKE_RESULT result
    );

/** @} */

#endif // NH_MAKE_UI_MESSAGE_H
