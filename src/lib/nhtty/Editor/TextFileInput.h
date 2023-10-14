#ifndef NH_TTY_TEXT_FILE_INPUT_H
#define NH_TTY_TEXT_FILE_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "FileEditor.h"
#include "SyntaxHighlights.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_handleTextFileInput(
        nh_List *Views_p, nh_tty_File *File_p, NH_ENCODING_UTF32 c, NH_BOOL insertMode,  
        NH_BOOL *refresh_p
    );

/** @} */

#endif 
