#ifndef NH_TTY_SYNTAX_HIGHLIGHT_H
#define NH_TTY_SYNTAX_HIGHLIGHT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhtty_enums
 *  @{
 */

    typedef enum NH_TTY_TEXT {
        NH_TTY_TEXT_UNDEFINED,
        NH_TTY_TEXT_C,
        NH_TTY_TEXT_CPP,
    } NH_TTY_TEXT;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_encoding_UTF32String nh_tty_cSyntaxHighlights(
        nh_encoding_UTF32String *String_p
    );

    nh_encoding_UTF32String nh_tty_cppSyntaxHighlights(
        nh_encoding_UTF32String *String_p
    );
    
    NH_TTY_TEXT nh_tty_getTextType(
        nh_encoding_UTF32String *Path_p 
    );

/** @} */

#endif 
