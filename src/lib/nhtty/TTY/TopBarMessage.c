// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TopBarMessage.h"
#include "TopBar.h"
#include "TTY.h"

#include "../Common/Macros.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhcore/System/Thread.h"

#include <stdio.h>
#include <stdlib.h>

// MESSAGES ========================================================================================

NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_TILING_FOCUS_P[] = {93, 32, 102, 111, 99, 117, 115, 32, 116, 105, 108, 101};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_TILING_INSERT_P[] = {'[', 'w', ']', '[', 'a', ']', '[', 's', ']', '[', 'd', ']', ' ', 'm', 'o', 'v', 'e', ' ', '[', 'e', 'n', 't', 'e', 'r', ']', ' ', 'i', 'n', 's', 'e', 'r', 't'};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_TILING_WASD_P[] = {'[', 'w', ']', '[', 'a', ']', '[', 's', ']', '[', 'd', ']', ' ', 'a', 'd', 'd'};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_TILING_WASDF_P[] = {'[', 'w', ']', '[', 'a', ']', '[', 's', ']', '[', 'd', ']', ' ', 'a', 'd', 'd', ' ', '[', 'f', ']', ' ', 's', 'p', 'l', 'i', 't'};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_TILING_FOCUS_SWITCHED_P[] = {70, 111, 99, 117, 115, 32, 115, 119, 105, 116, 99, 104, 101, 100, 32, 116, 111, 32, 116, 104, 105, 115, 32, 116, 105, 108, 101};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_TILING_ALREADY_FOCUSED_P[] = {84, 105, 108, 101, 32, 105, 115, 32, 97, 108, 114, 101, 97, 100, 121, 32, 102, 111, 99, 117, 115, 101, 100};

NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_SHOW_TREE_P[] = {83, 104, 111, 119, 32, 116, 114, 101, 101, 32, 108, 105, 115, 116, 105, 110, 103, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_HIDE_TREE_P[] = {72, 105, 100, 101, 32, 116, 114, 101, 101, 32, 108, 105, 115, 116, 105, 110, 103, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_INSERT_ACTIVATED_P[] = {73, 110, 115, 101, 114, 116, 32, 109, 111, 100, 101, 32, 97, 99, 116, 105, 118, 97, 116, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_INSERT_DEACTIVATED_P[] = {73, 110, 115, 101, 114, 116, 32, 109, 111, 100, 101, 32, 100, 101, 97, 99, 116, 105, 118, 97, 116, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_PREVIEW_ENABLED_P[] = {80, 114, 101, 118, 105, 101, 119, 32, 101, 110, 97, 98, 108, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_PREVIEW_DISABLED_P[] = {80, 114, 101, 118, 105, 101, 119, 32, 100, 105, 115, 97, 98, 108, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_READ_ONLY_P[] = {70, 105, 108, 101, 32, 105, 115, 32, 99, 117, 114, 114, 101, 110, 116, 108, 121, 32, 114, 101, 97, 100, 111, 110, 108, 121, 33};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_NEW_FILE_P[] = {78, 101, 119, 32, 102, 105, 108, 101, 32, 99, 114, 101, 97, 116, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_IGNORE_INPUT_P[] = {'I', 'g', 'n', 'o', 'r', 'e', ' ', 'i', 'n', 'p', 'u', 't', ':', ' '};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_FILE_EDIT_P[] = {'E', 'd', 'i', 't', ':', ' '};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_FILE_REMOVED_P[] = {70, 105, 108, 101, 32, 114, 101, 109, 111, 118, 101, 100, 58, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_FILE_SAVED_P[] = {70, 105, 108, 101, 32, 115, 97, 118, 101, 100, 58, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_FILE_CLOSED_P[] = {70, 105, 108, 101, 32, 99, 108, 111, 115, 101, 100, 58, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_FILE_OPENED_P[] = {70, 105, 108, 101, 32, 111, 112, 101, 110, 101, 100, 58, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_FILE_ALREADY_EXISTS_P[] = {70, 105, 108, 101, 32, 97, 108, 114, 101, 97, 100, 121, 32, 101, 120, 105, 115, 116, 115, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_NEW_ROOT_P[] = {78, 101, 119, 32, 108, 105, 115, 116, 105, 110, 103, 45, 114, 111, 111, 116, 58, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_EMPTY_DIRECTORY_P[] = {69, 109, 112, 116, 121, 32, 100, 105, 114, 101, 99, 116, 111, 114, 121, 58, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_TAB_TO_SPACES_ENABLED_P[] = {84, 97, 98, 32, 116, 111, 32, 115, 112, 97, 99, 101, 115, 32, 101, 110, 97, 98, 108, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_TAB_TO_SPACES_DISABLED_P[] = {84, 97, 98, 32, 116, 111, 32, 115, 112, 97, 99, 101, 115, 32, 100, 105, 115, 97, 98, 108, 101, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_EDITOR_NUMBER_OF_TAB_SPACES_P[] = {78, 117, 109, 98, 101, 114, 32, 111, 102, 32, 116, 97, 98, 32, 115, 112, 97, 99, 101, 115, 58, 32};

NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_BINARY_QUERY_YES_NO_P[] = {63, 32, 91, 121, 93, 32, 121, 101, 115, 32, 91, 110, 93, 32, 110, 111};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_P[] = {68, 101, 108, 101, 116, 101, 32};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_INTERRUPTED_P[] = {68, 101, 108, 101, 116, 101, 32, 105, 110, 116, 101, 114, 114, 117, 112, 116, 101, 100};

NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_MISC_NO_CURRENT_PROGRAM_P[] = {78, 111, 32, 99, 117, 114, 114, 101, 110, 116, 32, 112, 114, 111, 103, 114, 97, 109};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_MISC_BASIC_CONTROLS_P[] = {91, 99, 116, 114, 108, 43, 113, 93, 32, 101, 120, 105, 116, 32, 91, 99, 116, 114, 108, 43, 101, 93, 32, 99, 111, 110, 115, 111, 108, 101, 32, 91, 99, 116, 114, 108, 43, 114, 93, 32, 116, 105, 108, 105, 110, 103};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_MISC_UNKNOWN_COMMAND_P[] = {85, 110, 107, 110, 111, 119, 110, 32, 99, 111, 109, 109, 97, 110, 100, 46};
NH_ENCODING_UTF32 NH_TTY_MESSAGE_EN_MISC_INVALID_ARGUMENT_P[] = {73, 110, 118, 97, 108, 105, 100, 32, 65, 114, 103, 117, 109, 101, 110, 116, 46};

// GET MESSAGE =====================================================================================

NH_ENCODING_UTF32 *nh_tty_getMessage(
    NH_TTY_MESSAGE message, int *length_p)
{
NH_TTY_BEGIN()

    NH_ENCODING_UTF32 *message_p = NULL;

    switch (message)
    {
        case NH_TTY_MESSAGE_TILING_FOCUS :
            message_p = NH_TTY_MESSAGE_EN_TILING_FOCUS_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_FOCUS_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_TILING_INSERT:
            message_p = NH_TTY_MESSAGE_EN_TILING_INSERT_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_INSERT_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_TILING_WASD  :
            message_p = NH_TTY_MESSAGE_EN_TILING_WASD_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_WASD_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_TILING_WASDF :
            message_p = NH_TTY_MESSAGE_EN_TILING_WASDF_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_WASDF_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_TILING_FOCUS_SWITCHED : 
            message_p = NH_TTY_MESSAGE_EN_TILING_FOCUS_SWITCHED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_FOCUS_SWITCHED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_TILING_ALREADY_FOCUSED : 
            message_p = NH_TTY_MESSAGE_EN_TILING_ALREADY_FOCUSED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_TILING_ALREADY_FOCUSED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_SHOW_TREE  :
            message_p = NH_TTY_MESSAGE_EN_EDITOR_SHOW_TREE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_SHOW_TREE_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_HIDE_TREE :
            message_p = NH_TTY_MESSAGE_EN_EDITOR_HIDE_TREE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_HIDE_TREE_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_INSERT_ACTIVATED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_INSERT_ACTIVATED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_INSERT_ACTIVATED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_INSERT_DEACTIVATED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_INSERT_DEACTIVATED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_INSERT_DEACTIVATED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_PREVIEW_ENABLED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_PREVIEW_ENABLED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_PREVIEW_ENABLED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_PREVIEW_DISABLED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_PREVIEW_DISABLED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_PREVIEW_DISABLED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_READ_ONLY : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_READ_ONLY_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_READ_ONLY_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_NEW_FILE : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_NEW_FILE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_NEW_FILE_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_IGNORE_INPUT : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_IGNORE_INPUT_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_IGNORE_INPUT_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_FILE_EDIT : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_FILE_EDIT_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_FILE_EDIT_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_FILE_REMOVED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_FILE_REMOVED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_FILE_REMOVED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_FILE_SAVED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_FILE_SAVED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_FILE_SAVED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_FILE_CLOSED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_FILE_CLOSED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_FILE_CLOSED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_FILE_OPENED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_FILE_OPENED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_FILE_OPENED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_FILE_ALREADY_EXISTS : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_FILE_ALREADY_EXISTS_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_FILE_ALREADY_EXISTS_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_NEW_ROOT : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_NEW_ROOT_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_NEW_ROOT_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_EMPTY_DIRECTORY : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_EMPTY_DIRECTORY_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_EMPTY_DIRECTORY_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_TAB_TO_SPACES_ENABLED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_TAB_TO_SPACES_ENABLED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_TAB_TO_SPACES_ENABLED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_TAB_TO_SPACES_DISABLED : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_TAB_TO_SPACES_DISABLED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_TAB_TO_SPACES_DISABLED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_EDITOR_NUMBER_OF_TAB_SPACES : 
            message_p = NH_TTY_MESSAGE_EN_EDITOR_NUMBER_OF_TAB_SPACES_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_EDITOR_NUMBER_OF_TAB_SPACES_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_BINARY_QUERY_YES_NO : 
            message_p = NH_TTY_MESSAGE_EN_BINARY_QUERY_YES_NO_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_BINARY_QUERY_YES_NO_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_BINARY_QUERY_DELETE : 
            message_p = NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_BINARY_QUERY_DELETE_INTERRUPTED : 
            message_p = NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_INTERRUPTED_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_BINARY_QUERY_DELETE_INTERRUPTED_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_MISC_NO_CURRENT_PROGRAM : 
            message_p = NH_TTY_MESSAGE_EN_MISC_NO_CURRENT_PROGRAM_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_MISC_NO_CURRENT_PROGRAM_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_MISC_BASIC_CONTROLS :
            message_p = NH_TTY_MESSAGE_EN_MISC_BASIC_CONTROLS_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_MISC_BASIC_CONTROLS_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_MISC_UNKNOWN_COMMAND :
            message_p = NH_TTY_MESSAGE_EN_MISC_UNKNOWN_COMMAND_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_MISC_UNKNOWN_COMMAND_P)/sizeof(NH_ENCODING_UTF32);
            break;
        case NH_TTY_MESSAGE_MISC_INVALID_ARGUMENT :
            message_p = NH_TTY_MESSAGE_EN_MISC_INVALID_ARGUMENT_P;
            *length_p = sizeof(NH_TTY_MESSAGE_EN_MISC_INVALID_ARGUMENT_P)/sizeof(NH_ENCODING_UTF32);
            break;
    }

NH_TTY_END(message_p)
}

// SET =============================================================================================

static NH_TTY_RESULT nh_tty_setMessage(
    nh_tty_TopBar *TopBar_p, NH_ENCODING_UTF32 *message_p, int length)
{
NH_TTY_BEGIN()

    nh_tty_TopBarMessage *Message_p = NULL;

    if (TopBar_p) {
        Message_p = &TopBar_p->Message;
    } else {
        Message_p = &NH_TTY_MACRO_TAB(((nh_tty_TTY*)nh_core_getWorkloadArg())->Window_p->Tile_p)->TopBar.Message;
    }

    nh_encoding_freeUTF32(&Message_p->Text);
    nh_encoding_appendUTF32(&Message_p->Text, message_p, length);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_clearMessage(
    nh_tty_TopBar *TopBar_p)
{
NH_TTY_BEGIN()

    nh_tty_TopBarMessage *Message_p = NULL;

    if (TopBar_p) {
        Message_p = &TopBar_p->Message;
    } else {
        Message_p = &NH_TTY_MACRO_TAB(((nh_tty_TTY*)nh_core_getWorkloadArg())->Window_p->Tile_p)->TopBar.Message;
    }

    nh_encoding_freeUTF32(&Message_p->Text);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_setDefaultMessage(
    nh_tty_TopBar *TopBar_p, NH_TTY_MESSAGE message)
{
NH_TTY_BEGIN()

    int length = 0;
    NH_ENCODING_UTF32 *codepoints_p = nh_tty_getMessage(message, &length);
    NH_TTY_CHECK(nh_tty_setMessage(TopBar_p, codepoints_p, length))

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_setCustomMessage(
    nh_tty_TopBar *TopBar_p, NH_ENCODING_UTF32 *message_p, int messageLength)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK(nh_tty_setMessage(TopBar_p, message_p, messageLength))

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_setCustomSuffixMessage(
    nh_tty_TopBar *TopBar_p, NH_TTY_MESSAGE prefix, NH_ENCODING_UTF32 *suffix_p, int suffixLength)
{
NH_TTY_BEGIN()

    int length = 0;
    NH_ENCODING_UTF32 *codepoints_p = nh_tty_getMessage(prefix, &length);

    nh_encoding_UTF32String Message = nh_encoding_initUTF32(128);
    nh_encoding_appendUTF32(&Message, codepoints_p, length);
    nh_encoding_appendUTF32(&Message, suffix_p, suffixLength);

    NH_TTY_CHECK(nh_tty_setMessage(TopBar_p, Message.p, length + suffixLength))

    nh_encoding_freeUTF32(&Message);

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_setBinaryQueryMessage(
    nh_tty_TopBar *TopBar_p, NH_ENCODING_UTF32 *query_p, int length, void *args_p, 
    NH_TTY_RESULT (*callback_f)(nh_wsi_KeyboardEvent Event, NH_BOOL *continue_p))
{
NH_TTY_BEGIN()

//    nh_tty_TopBarMessage *Message_p = &((nh_tty_MacroTile*)((nh_tty_TTY*)nh_core_getWorkloadArg())->Window_p->Tile_p->p)->TopBar.Message;
//    if (Message_p->block) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}
//
//    int yesNoLength;
//    NH_ENCODING_UTF32 *yesNo_p = nh_tty_getMessage(NH_TTY_MESSAGE_BINARY_QUERY_YES_NO, &yesNoLength);
//
//    nh_encoding_UTF32String Question = nh_encoding_initUTF32(128);
//    nh_encoding_appendUTF32(&Question, query_p, length);
//    nh_encoding_appendUTF32(&Question, yesNo_p, yesNoLength);
//
//    NH_TTY_CHECK(nh_tty_setMessage(TopBar_p, Question.p, Question.length))
//
//    Message_p->block = NH_TRUE;
//    Message_p->args_p = args_p;
//    Message_p->callback_f = callback_f;
//
//    nh_encoding_freeUTF32(&Question);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

