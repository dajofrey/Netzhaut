#ifndef NH_TTY_TEXT_FILE_H
#define NH_TTY_TEXT_FILE_H

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

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_TextFileLine {
	NH_BOOL copy;
        nh_Array Copy;
        nh_Array Unsaved;
        nh_Array Search;
        nh_encoding_UTF32String Codepoints;
        nh_encoding_UTF32String RenderCodepoints;
    } nh_tty_TextFileLine;

    typedef struct nh_tty_TextFile {
        NH_TTY_TEXT textType;
        int lineNumberOffset;
        int fileCursorXTarget;
        int fileCursorX;
        int fileCursorY;
        int select;
        nh_List Lines;
    } nh_tty_TextFile;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_TextFileLine *nh_tty_newTextFileLine(
        nh_tty_TextFile *TextFile_p, int index
    );
    
    NH_TTY_RESULT nh_tty_insertIntoTextFileLine(
        nh_tty_TextFileLine *Line_p, int index, NH_ENCODING_UTF32 c
    );
    
    NH_TTY_RESULT nh_tty_removeFromTextFileLine(
        nh_tty_TextFileLine *Line_p, int index, int length
    );

    nh_tty_TextFile *nh_tty_openTextFile(
        nh_encoding_UTF32String *Path_p 
    );

    NH_TTY_RESULT nh_tty_closeTextFile(
        nh_tty_TextFile *File_p
    );

    NH_TTY_RESULT nh_tty_clearTextFileSearch(
        nh_tty_TextFile *TextFile_p
    );

    NH_TTY_RESULT nh_tty_searchTextFile(
        nh_tty_TextFile *TextFile_p, NH_ENCODING_UTF32 *str_p, int length
    );

    NH_TTY_RESULT nh_tty_writeTextFile(
        nh_tty_TextFile *File_p, nh_encoding_UTF32String *Path_p
    );

    NH_TTY_RESULT nh_tty_renderTextFileLine(
        nh_tty_TextFile *TextFile_p, int line
    );

    NH_TTY_RESULT nh_tty_drawTextFileLine(
        nh_tty_Program *Program_p, nh_tty_TextFile *TextFile_p, nh_tty_FileView *FileView_p, 
        nh_tty_Glyph *Glyphs_p, int line
    );

/** @} */

#endif 
