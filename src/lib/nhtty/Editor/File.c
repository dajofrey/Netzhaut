// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "File.h"
#include "FileEditor.h"
#include "Editor.h"
#include "TreeListing.h"
#include "TextFile.h"
#include "TextFileInput.h"
#include "SyntaxHighlights.h"

#include "../TTY/TTY.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Process.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhencoding/Encodings/UTF8.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// VIEW ============================================================================================

nh_tty_FileView nh_tty_initFileView(
    nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    nh_tty_FileView View;

    View.File_p = File_p;
    View.width  = 0;
    View.height = 0;    

    View.TextFile.screenCursorX = 0;
    View.TextFile.screenCursorY = 0;
    View.TextFile.rowOffset = 0;
    View.TextFile.colOffset = 0;

NH_TTY_END(View)
}

NH_TTY_RESULT nh_tty_createFileViews(
    nh_tty_FileEditorView *View_p, nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    nh_tty_FileView *FileView_p = nh_core_allocate(sizeof(nh_tty_FileView));
    NH_TTY_CHECK_MEM(FileView_p)
    *FileView_p = nh_tty_initFileView(File_p);
    nh_core_appendToList(&View_p->FileViews, FileView_p);

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_destroyFileViews(
    nh_tty_FileEditorView *View_p, nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    nh_tty_FileView *FileView_p = NULL;
    for (int j = 0; j < View_p->FileViews.size; ++j) {
        FileView_p = View_p->FileViews.pp[j];
        if (FileView_p->File_p == File_p) {break;}
        FileView_p = NULL;
    }
    if (FileView_p) {nh_core_removeFromList2(&View_p->FileViews, NH_TRUE, FileView_p);}

NH_TTY_END(NH_TTY_SUCCESS)
}

nh_tty_FileView *nh_tty_getFileView(
    nh_tty_EditorView *View_p, nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < View_p->FileEditor.FileViews.size; ++i) {
        nh_tty_FileView *FileView_p = View_p->FileEditor.FileViews.pp[i];
        if (FileView_p->File_p == File_p) {NH_TTY_END(FileView_p)}
    }
  
NH_TTY_END(NULL)
}

NH_TTY_RESULT nh_tty_updateFileViews(
    nh_tty_EditorView *View_p)
{
NH_TTY_BEGIN()

    int oddLeftOver, fileWidth = View_p->FileEditor.width;

    if (View_p->FileEditor.FileViews.size > 0) {
        int count = View_p->FileEditor.FileViews.size;
        if (View_p->FileEditor.FileViews.size > View_p->FileEditor.maxOnScreen) {
            count = View_p->FileEditor.maxOnScreen;
        }
        fileWidth = View_p->FileEditor.width / count;
        oddLeftOver = View_p->FileEditor.width % (count * fileWidth);
    }

    for (int j = 0; j < View_p->FileEditor.FileViews.size; ++j) {
        nh_tty_FileView *FileView_p = View_p->FileEditor.FileViews.pp[j];
        if (j == View_p->FileEditor.FileViews.size - 1) {fileWidth += oddLeftOver;}
        FileView_p->width = fileWidth;
        FileView_p->height = View_p->height;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// TYPE ============================================================================================

NH_TTY_FILE nh_tty_getFileType(
    nh_encoding_UTF32String *Path_p)
{
NH_TTY_BEGIN()

    NH_TTY_FILE type = NH_TTY_FILE_TEXT;
    nh_encoding_UTF8String Path = nh_encoding_encodeUTF8(Path_p->p, Path_p->length);

    if (strstr(Path.p, "netzhaut/CHANGES")) {
        type = NH_TTY_FILE_CHANGES;
    }

    nh_encoding_freeUTF8(&Path);

NH_TTY_END(type)
}

// RENDER ==========================================================================================

NH_TTY_RESULT nh_tty_renderFile(
    nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            for (int i = 0; i < ((nh_tty_TextFile*)File_p->handle_p)->Lines.size; ++i) {
                NH_TTY_CHECK(nh_tty_renderTextFileLine(File_p->handle_p, i))
            }
            break;
        case NH_TTY_FILE_CHANGES :
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// WRITE ===========================================================================================

NH_TTY_RESULT nh_tty_writeFile(
    nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            NH_TTY_CHECK(nh_tty_writeTextFile(File_p->handle_p, &File_p->Node_p->Path))
            break;
        case NH_TTY_FILE_CHANGES :
            break;
    }

    NH_TTY_CHECK(nh_tty_setCustomSuffixMessage(
        NULL, NH_TTY_MESSAGE_EDITOR_FILE_SAVED, File_p->Node_p->Path.p, File_p->Node_p->Path.length
    ))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// SEARCH ==========================================================================================

NH_TTY_RESULT nh_tty_clearFileSearch(
    nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            nh_tty_clearTextFileSearch(File_p->handle_p);
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_searchFile(
    nh_tty_File *File_p, NH_ENCODING_UTF32 *str_p, int length)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            NH_TTY_CHECK(nh_tty_searchTextFile(File_p->handle_p, str_p, length))
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

NH_TTY_RESULT nh_tty_handleFileInput(
    nh_tty_Program *Program_p, nh_tty_File *File_p, NH_ENCODING_UTF32 c, NH_BOOL insertMode, 
    NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    if (File_p->readOnly) {
        *refresh_p = NH_TRUE;
        NH_TTY_CHECK(nh_tty_setDefaultMessage(NULL, NH_TTY_MESSAGE_EDITOR_READ_ONLY))
    }

    nh_List FileViews = ((nh_tty_Editor*)Program_p->handle_p)->View.FileEditor.FileViews;

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            NH_TTY_CHECK(nh_tty_handleTextFileInput(&FileViews, File_p, c, insertMode, refresh_p))
            break;    
        case NH_TTY_FILE_CHANGES :
            break;    
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT nh_tty_drawFileRow(
    nh_tty_Program *Program_p, nh_tty_File *File_p, nh_tty_FileView *View_p, nh_tty_Glyph *Glyphs_p, 
    int row)
{
NH_TTY_BEGIN()

    switch (File_p->type)
    {
        case NH_TTY_FILE_TEXT :
            nh_tty_drawTextFileLine(Program_p, File_p->handle_p, View_p, Glyphs_p, row);
            break;
        case NH_TTY_FILE_CHANGES :
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

