#ifndef NH_TTY_FILE_H
#define NH_TTY_FILE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../TTY/Program.h"
#include "../Common/Includes.h"

typedef struct nh_tty_TreeListingNode nh_tty_TreeListingNode;
typedef struct nh_tty_EditorView nh_tty_EditorView;
typedef struct nh_tty_FileEditorView nh_tty_FileEditorView;

#endif

/** @addtogroup lib_nhtty_enums
 *  @{
 */

    typedef enum NH_TTY_FILE {
        NH_TTY_FILE_UNDEFINED,
        NH_TTY_FILE_TEXT,
        NH_TTY_FILE_CHANGES,
    } NH_TTY_FILE;

/** @} */


/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_File {
        nh_tty_TreeListingNode *Node_p;
        NH_TTY_FILE type;
        void *handle_p;
        NH_BOOL readOnly;
    } nh_tty_File;

    typedef struct nh_tty_TextFileView {
        int screenCursorX;
        int screenCursorY;
        int rowOffset;
        int colOffset;
    } nh_tty_TextFileView;

    typedef struct nh_tty_FileView {
        nh_tty_File *File_p;
        nh_tty_TextFileView TextFile;
        int height;
        int width;
    } nh_tty_FileView;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_FileView nh_tty_initFileView(
        nh_tty_File *File_p
    );
    
    NH_TTY_RESULT nh_tty_createFileViews(
        nh_tty_FileEditorView *View_p, nh_tty_File *File_p
    );
    
    NH_TTY_RESULT nh_tty_destroyFileViews(
        nh_tty_FileEditorView *View_p, nh_tty_File *File_p
    );
    
    nh_tty_FileView *nh_tty_getFileView(
        nh_tty_EditorView *View_p, nh_tty_File *File_p
    );
    
    nh_List nh_tty_getFileViews(
        nh_tty_Program *Program_p, nh_tty_File *File_p
    );
    
    NH_TTY_RESULT nh_tty_updateFileViews(
        nh_tty_EditorView *View_p
    );
    
    NH_TTY_FILE nh_tty_getFileType(
        nh_encoding_UTF32String *Path_p
    );

    NH_TTY_RESULT nh_tty_renderFile(
        nh_tty_File *File_p
    );
    
    NH_TTY_RESULT nh_tty_writeFile(
        nh_tty_File *File_p
    );
    
    NH_TTY_RESULT nh_tty_clearFileSearch(
        nh_tty_File *File_p
    );
    
    NH_TTY_RESULT nh_tty_searchFile(
        nh_tty_File *File_p, NH_ENCODING_UTF32 *str_p, int length
    );
    
    NH_TTY_RESULT nh_tty_handleFileInput(
        nh_tty_Program *Program_p, nh_tty_File *File_p, NH_ENCODING_UTF32 c, NH_BOOL insertMode, 
        NH_BOOL *refresh_p
    );

    NH_TTY_RESULT nh_tty_drawFileRow(
        nh_tty_Program *Program_p, nh_tty_File *File_p, nh_tty_FileView *View_p, nh_tty_Glyph *Glyphs_p,
        int row
    );

/** @} */

#endif 
