#ifndef NH_TTY_TOPBAR_MESSAGE_H
#define NH_TTY_TOPBAR_MESSAGE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

typedef struct nh_tty_TopBar nh_tty_TopBar;

#endif

/** @addtogroup lib_nhtty_enums
 *  @{
 */

    typedef enum NH_TTY_MESSAGE {
        NH_TTY_MESSAGE_TILING_FOCUS,
        NH_TTY_MESSAGE_TILING_INSERT,
        NH_TTY_MESSAGE_TILING_WASD,
        NH_TTY_MESSAGE_TILING_WASDF,
        NH_TTY_MESSAGE_TILING_FOCUS_SWITCHED,
        NH_TTY_MESSAGE_TILING_ALREADY_FOCUSED,
        NH_TTY_MESSAGE_EDITOR_SHOW_TREE,
        NH_TTY_MESSAGE_EDITOR_HIDE_TREE,
        NH_TTY_MESSAGE_EDITOR_INSERT_ACTIVATED,
        NH_TTY_MESSAGE_EDITOR_INSERT_DEACTIVATED,
        NH_TTY_MESSAGE_EDITOR_PREVIEW_ENABLED,
        NH_TTY_MESSAGE_EDITOR_PREVIEW_DISABLED,
        NH_TTY_MESSAGE_EDITOR_READ_ONLY,
        NH_TTY_MESSAGE_EDITOR_NEW_FILE,
        NH_TTY_MESSAGE_EDITOR_IGNORE_INPUT,
        NH_TTY_MESSAGE_EDITOR_FILE_EDIT,
        NH_TTY_MESSAGE_EDITOR_FILE_REMOVED,
        NH_TTY_MESSAGE_EDITOR_FILE_SAVED,
        NH_TTY_MESSAGE_EDITOR_FILE_CLOSED,
        NH_TTY_MESSAGE_EDITOR_FILE_OPENED,
        NH_TTY_MESSAGE_EDITOR_FILE_ALREADY_EXISTS,
        NH_TTY_MESSAGE_EDITOR_NEW_ROOT,
        NH_TTY_MESSAGE_EDITOR_EMPTY_DIRECTORY,
        NH_TTY_MESSAGE_EDITOR_TAB_TO_SPACES_ENABLED,
        NH_TTY_MESSAGE_EDITOR_TAB_TO_SPACES_DISABLED,
        NH_TTY_MESSAGE_EDITOR_NUMBER_OF_TAB_SPACES,
        NH_TTY_MESSAGE_BINARY_QUERY_YES_NO,
        NH_TTY_MESSAGE_BINARY_QUERY_DELETE,
        NH_TTY_MESSAGE_BINARY_QUERY_DELETE_INTERRUPTED,
        NH_TTY_MESSAGE_MISC_NO_CURRENT_PROGRAM,
        NH_TTY_MESSAGE_MISC_BASIC_CONTROLS,
        NH_TTY_MESSAGE_MISC_UNKNOWN_COMMAND,
        NH_TTY_MESSAGE_MISC_INVALID_ARGUMENT,
    } NH_TTY_MESSAGE;

/** @} */

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_TopBarMessage {
        NH_BOOL block;
        nh_encoding_UTF32String Text;
        void *args_p;
        NH_TTY_RESULT (*callback_f)(nh_wsi_KeyboardEvent Event, NH_BOOL *continue_p);
    } nh_tty_TopBarMessage;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_ENCODING_UTF32 *nh_tty_getMessage(
        NH_TTY_MESSAGE message, int *length_p
    );

    NH_TTY_RESULT nh_tty_clearMessage(
        nh_tty_TopBar *TopBar_p
    );

    NH_TTY_RESULT nh_tty_setDefaultMessage(
        nh_tty_TopBar *TopBar_p, NH_TTY_MESSAGE message
    );
    
    NH_TTY_RESULT nh_tty_setCustomMessage(
        nh_tty_TopBar *TopBar_p, NH_ENCODING_UTF32 *suffix_p, int suffixLength
    );

    NH_TTY_RESULT nh_tty_setCustomSuffixMessage(
        nh_tty_TopBar *TopBar_p, NH_TTY_MESSAGE prefix, NH_ENCODING_UTF32 *suffix_p, 
        int suffixLength
    );

    NH_TTY_RESULT nh_tty_setBinaryQueryMessage(
        nh_tty_TopBar *TopBar_p, NH_ENCODING_UTF32 *query_p, int length, void *args_p, 
        NH_TTY_RESULT (*callback_f)(nh_wsi_KeyboardEvent Event, NH_BOOL *continue_p)
    );

/** @} */

#endif 
