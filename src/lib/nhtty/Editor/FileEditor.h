#ifndef NH_TTY_FILE_EDITOR_H
#define NH_TTY_FILE_EDITOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "File.h"

#include "../TTY/Program.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_FileEditorView {
        nh_List FileViews;
        int currentOffset;
        int maxOnScreen;
        int fromLeft;
        int width;
        int helpScroll;
    } nh_tty_FileEditorView;

    typedef struct nh_tty_FileEditor {
        nh_LinkedList Files;
        nh_Array Copies;
        int current;
        int minCols;
        int tabSpaces;
        NH_BOOL tabToSpaces;
    } nh_tty_FileEditor;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_FileEditor nh_tty_initFileEditor(
    );

    NH_TTY_RESULT nh_tty_freeFileEditor(
        nh_tty_FileEditor *FileEditor_p
    );

    nh_tty_File *nh_tty_openFile(
        nh_tty_Program *Program_p, nh_tty_TreeListingNode *Node_p, NH_BOOL readOnly
    );

    NH_TTY_RESULT nh_tty_closeFile(
        nh_tty_FileEditor *Editor_p, nh_tty_File *File_p
    );

    NH_TTY_RESULT nh_tty_cycleThroughFiles(
        nh_tty_Program *Program_p, NH_ENCODING_UTF32 c
    );

    NH_TTY_RESULT nh_tty_handleFileEditorInput(
        nh_tty_Program *Program_p, NH_ENCODING_UTF32 c
    );

    NH_TTY_RESULT nh_tty_drawFileEditorRow(
        nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width, int height, int row
    );

/** @} */

#endif 
