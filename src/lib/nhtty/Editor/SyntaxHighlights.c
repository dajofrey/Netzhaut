// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "SyntaxHighlights.h"
#include "TreeListing.h"
#include "Editor.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Process.h"
#include "../../nhcore/System/Memory.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// WORDS ===========================================================================================

static NH_BYTE *cTypeKeywords_pp[] = {
    "auto",
    "char",
    "const",
    "double",
    "enum",
    "extern",
    "float",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "short",
    "signed",
    "static",
    "struct",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "_Alignas",
    "_Atomic",
    "_Bool",
    "_Complex",
    "_Decimal128",
    "_Decimal32",
    "_Decimal64",
    "_Imaginary",
    "_Noreturn",
    "_Thread_local",
};

static NH_BYTE *cControlKeywords_pp[] = {
    "break",
    "case",
    "continue",
    "default",
    "do",
    "else",
    "for",
    "goto",
    "if",
    "return",
    "sizeof",
    "switch",
    "while",
    "_Alignof",
    "_Generic",
    "_Static_assert",
};

//// INIT ============================================================================================
//
//static NH_TTY_RESULT nh_tty_insertHighlightWord(
//    nh_encoding_UTF32String *String_p, nh_encoding_UTF32String Word, NH_BYTE *prefix_p, NH_BYTE *suffix_p)
//{
//NH_TTY_BEGIN()
//
//    nh_encoding_UTF32String Prefix = nh_decodeUTF8Text(prefix_p);
//    nh_encoding_UTF32String Suffix = nh_decodeUTF8Text(suffix_p);
//
//    nh_core_appendToArray(String_p, Prefix.p, Prefix.length);
//    nh_core_appendToArray(String_p, Word.p, Word.length);
//    nh_core_appendToArray(String_p, Suffix.p, Suffix.length);
//
//    nh_core_freeArray(&Prefix);
//    nh_core_freeArray(&Suffix);
//
//NH_TTY_END(NH_TTY_SUCCESS)
//}
//
//nh_encoding_UTF32String nh_tty_highlightWord(
//    nh_encoding_UTF32String Word, nh_encoding_UTF32String *String_p, NH_BYTE *prefix_p, NH_BYTE *suffix_p)
//{
//NH_TTY_BEGIN()
//
//    nh_encoding_UTF32String NewString = nh_core_initArray(sizeof(NH_ENCODING_UTF32), 64);
//
//    for (int i = 0; i < String_p->length; ++i) 
//    {
//        NH_ENCODING_UTF32 *codepoint_p = &((NH_ENCODING_UTF32*)String_p->p)[i];
//
//        if (*codepoint_p == ((NH_ENCODING_UTF32*)Word.p)[0]) 
//        {
//            NH_BOOL match = NH_TRUE;
//            for (int j = 1; j < Word.length; ++j) {
//                if (((NH_ENCODING_UTF32*)Word.p)[j] != codepoint_p[j] || j + i >= String_p->length) {
//                    match = NH_FALSE;
//                    break;
//                }
//            }
//
//            if (match) {
//                nh_tty_insertHighlightWord(&NewString, Word, prefix_p, suffix_p);
//                i += Word.length - 1;
//                continue;
//            }
//        }
//
//        nh_core_appendToArray(&NewString, codepoint_p, 1);
//    }
//
//NH_TTY_END(NewString)
//}
//
//nh_encoding_UTF32String nh_tty_highlightWords(
//    NH_BYTE *words_pp[], size_t words, nh_encoding_UTF32String *String_p, NH_BYTE *prefix_p, NH_BYTE *suffix_p)
//{
//NH_TTY_BEGIN()
//
//    nh_encoding_UTF32String String = nh_core_initArray(sizeof(NH_ENCODING_UTF32), 64);
//
//    for (int i = 0; i < words; ++i) 
//    {
//        NH_BYTE p[255] = {'\0'};
//        nh_encoding_UTF32String Word = nh_decodeUTF8Text(words_pp[i]);
//        nh_encoding_UTF32String NewString = nh_tty_highlightWord(Word, String_p, prefix_p, suffix_p);
//        nh_core_freeArray(&String);
//        String = NewString;
//        String_p = &String;
//        nh_core_freeArray(&Word);
//    }
//
//NH_TTY_END(String)
//}
//
//// C ===============================================================================================
//
//nh_encoding_UTF32String nh_tty_cSyntaxHighlights(
//    nh_encoding_UTF32String *String_p)
//{
//NH_TTY_BEGIN()
//
//    nh_encoding_UTF32String String1 = nh_tty_highlightWords(
//        cTypeKeywords_pp, sizeof(cTypeKeywords_pp) / sizeof(cTypeKeywords_pp[0]), String_p, "\e[0;32m", "\e[0m"
//    ); 
//    nh_encoding_UTF32String String2 = nh_tty_highlightWords(
//        cControlKeywords_pp, sizeof(cControlKeywords_pp) / sizeof(cControlKeywords_pp[0]), &String1, "\e[1;33m", "\e[0m"
//    ); 
//    nh_core_freeArray(&String1);
//
//NH_TTY_END(String2)
//}
//
//// CPP =============================================================================================
//
//nh_encoding_UTF32String nh_tty_cppSyntaxHighlights(
//    nh_encoding_UTF32String *String_p)
//{
//NH_TTY_BEGIN()
//NH_TTY_END(nh_core_initArray(0, 0))
//}
//
//// CODE TYPE =======================================================================================

NH_TTY_TEXT nh_tty_getTextType(
    nh_encoding_UTF32String *Path_p)
{
NH_TTY_BEGIN()

    if (Path_p == NULL) {NH_TTY_END(NH_TTY_TEXT_UNDEFINED)}

    if (Path_p->length > 1) {
        if (Path_p->p[Path_p->length - 2] == '.' && Path_p->p[Path_p->length - 1] == 'c') {
            NH_TTY_END(NH_TTY_TEXT_C)
        }
        if (Path_p->p[Path_p->length - 2] == '.' && Path_p->p[Path_p->length - 1] == 'h') {
            NH_TTY_END(NH_TTY_TEXT_C)
        }
    }

NH_TTY_END(NH_TTY_TEXT_UNDEFINED)
}

