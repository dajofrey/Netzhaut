// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TextFileInput.h"
#include "TextFile.h"
#include "Editor.h"
#include "TreeListing.h"

#include "../TTY/TTY.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Process.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/File.h"
#include "../../nhcore/Common/Macros.h"

#include "../../nhcss/Parser/Parser.h"
#include "../../nhhtml/Parser/Parser.h"
#include "../../nhwebidl/Runtime/Parser.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"
#include "../../nhencoding/Common/Macros.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// CLIPBOARD =======================================================================================

static NH_TTY_RESULT nh_tty_setClipboard(
    nh_tty_TextFile *TextFile_p, NH_BOOL append, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    if (!append) {nh_tty_resetClipboard();}

    for (int i = 0; i < TextFile_p->Lines.size; ++i) 
    {
        nh_tty_TextFileLine *Line_p = TextFile_p->Lines.pp[i];
        nh_encoding_UTF32String *Copy_p = NULL;

	if (!Line_p->Copy.length && Line_p->copy) {
            Copy_p = nh_tty_newClipboardLine();
            NH_TTY_CHECK_MEM(Copy_p)
	    Line_p->copy = NH_FALSE;
            continue;
	}

        for (int j = 0; j < Line_p->Copy.length; ++j) {
            if (((NH_BOOL*)Line_p->Copy.p)[j]) {
                if (!Copy_p) {
                    Copy_p = nh_tty_newClipboardLine();
                    NH_TTY_CHECK_MEM(Copy_p)
                }
                nh_encoding_appendUTF32(Copy_p, &Line_p->Codepoints.p[j], 1);
                ((NH_BOOL*)Line_p->Copy.p)[j] = NH_FALSE;
            }
        }

	Line_p->copy = NH_FALSE;

        if (Copy_p) {
            NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, i))
            *refresh_p = NH_TRUE;
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_insertClipboard(
    nh_List *Views_p, nh_tty_File *File_p, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    nh_tty_TextFile *TextFile_p = File_p->handle_p;
    nh_tty_Clipboard *Clipboard_p = nh_tty_getClipboard();

    // Move cursor.
    if (Clipboard_p->Lines.length > 1) 
    {
        int fileCursorX = TextFile_p->fileCursorX;
        NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 13, NH_FALSE, refresh_p))
        NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'k', NH_FALSE, refresh_p))

        while (TextFile_p->fileCursorX < fileCursorX) {
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'l', NH_FALSE, refresh_p))
        }
    }

    for (int i = 0; i < Clipboard_p->Lines.length; ++i) 
    {
        nh_encoding_UTF32String *ClipboardLine_p = &((nh_encoding_UTF32String*)Clipboard_p->Lines.p)[i];

	if (ClipboardLine_p->length == 0) {
	    NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'o', NH_FALSE, refresh_p))
	    continue;
	}
        for (int j = 0; j < ClipboardLine_p->length; ++j) {
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, ClipboardLine_p->p[j], NH_TRUE, refresh_p))
        }
        if (i + 1 < Clipboard_p->Lines.length) {
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'o', NH_FALSE, refresh_p))
        }
    }

    *refresh_p = NH_TRUE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static NH_TTY_RESULT nh_tty_handleFileCursorXTarget(
    nh_List *Views_p, nh_tty_File *File_p, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    nh_tty_TextFile *TextFile_p = File_p->handle_p;
    nh_tty_TextFileLine *Line_p = nh_core_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY);
    NH_TTY_CHECK_NULL(Line_p)

    int *colOffsets_p = nh_core_allocate(sizeof(int) * Views_p->size);
    NH_TTY_CHECK_MEM(colOffsets_p)

    for (int i = 0; i < Views_p->size; ++i) {
        colOffsets_p[i] = ((nh_tty_FileView*)Views_p->pp[i])->TextFile.colOffset;
        ((nh_tty_FileView*)Views_p->pp[i])->TextFile.colOffset = 0;
        ((nh_tty_FileView*)Views_p->pp[i])->TextFile.screenCursorX = 0;
    }

    int xTarget = TextFile_p->fileCursorXTarget;
    TextFile_p->fileCursorX = 0;

    if (xTarget > Line_p->Codepoints.length) {
        while (TextFile_p->fileCursorX < Line_p->Codepoints.length) {
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'l', NH_FALSE, refresh_p))
        }
        TextFile_p->fileCursorXTarget = xTarget;
    }
    else {
        while (xTarget > TextFile_p->fileCursorX) {
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'l', NH_FALSE, refresh_p))
        }
        TextFile_p->fileCursorXTarget = xTarget;
    }

    for (int i = 0; i < Views_p->size; ++i) {
        if (((nh_tty_FileView*)Views_p->pp[i])->TextFile.colOffset != colOffsets_p[i]) {
            *refresh_p = NH_TRUE;
        }
    }

    nh_core_free(colOffsets_p);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_handleCopySelection(
    nh_tty_TextFile *TextFile_p, nh_tty_TextFileLine *Line_p, NH_ENCODING_UTF32 c, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    if (TextFile_p->select < 0) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    switch (c)
    {
        case 'j' :
            for (int i = 0; i < Line_p->Copy.length; ++i) { 
                ((NH_BOOL*)Line_p->Copy.p)[i] = TextFile_p->select < TextFile_p->fileCursorY;
            }
	    Line_p->copy = NH_TRUE;
            NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY - 1))
            break;

        case 'k' :
            for (int i = 0; i < Line_p->Copy.length; ++i) { 
                ((NH_BOOL*)Line_p->Copy.p)[i] = TextFile_p->select > TextFile_p->fileCursorY + 1;
            }
	    Line_p->copy = NH_FALSE;
            NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY + 1))
            break;

        case 'l' :
            ((NH_BOOL*)Line_p->Copy.p)[TextFile_p->fileCursorX - 1] = NH_TRUE;
            NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY))
            break;

        case 'h' :
            ((NH_BOOL*)Line_p->Copy.p)[TextFile_p->fileCursorX + 1] = NH_FALSE;
            NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY))
            break;
    }

    *refresh_p = NH_TRUE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// READ ============================================================================================

static NH_TTY_RESULT nh_tty_handleTextFileViews(
    nh_List *Views_p, nh_tty_TextFile *TextFile_p, NH_ENCODING_UTF32 c, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    switch (c)
    {
        case 'j' : 
            for (int i = 0; i < Views_p->size; ++i) {
                nh_tty_FileView *FileView_p = Views_p->pp[i];
                if (FileView_p->TextFile.screenCursorY < FileView_p->height-1) {
                    FileView_p->TextFile.screenCursorY++;
                } else {
                    FileView_p->TextFile.rowOffset++;
                    *refresh_p = NH_TRUE;
                }
            }
            break; 

        case 'k' :
            for (int i = 0; i < Views_p->size; ++i) {
                nh_tty_FileView *FileView_p = Views_p->pp[i];
                if (FileView_p->TextFile.screenCursorY > 0) {FileView_p->TextFile.screenCursorY--;}
                else if (FileView_p->TextFile.rowOffset > 0) {
                    FileView_p->TextFile.rowOffset--;
                    *refresh_p = NH_TRUE;
                }
            }
            break; 

        case 'l' :
            for (int i = 0; i < Views_p->size; ++i) {
                nh_tty_FileView *FileView_p = Views_p->pp[i];
                if (FileView_p->TextFile.screenCursorX < FileView_p->width - TextFile_p->lineNumberOffset - 1) {FileView_p->TextFile.screenCursorX++;}
                else {
                    FileView_p->TextFile.colOffset++;
                    *refresh_p = NH_TRUE;
                }
            }
            break;

        case 'h' :
            for (int i = 0; i < Views_p->size; ++i) {
                nh_tty_FileView *FileView_p = Views_p->pp[i];
                if (FileView_p->TextFile.screenCursorX > 0) {FileView_p->TextFile.screenCursorX--;}
                else if (FileView_p->TextFile.colOffset > 0) {
                    FileView_p->TextFile.colOffset--;
                    *refresh_p = NH_TRUE;
                }
            }
            break;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_handleReadOperation(
    nh_List *Views_p, nh_tty_File *File_p, NH_ENCODING_UTF32 c, NH_BOOL insertMode, NH_BOOL *refresh_p,
    NH_BOOL *read_p)
{
NH_TTY_BEGIN()

    nh_tty_TextFile *TextFile_p = File_p->handle_p;
    nh_tty_TextFileLine *Line_p = nh_core_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY);
    NH_TTY_CHECK_NULL(Line_p)

    if (insertMode) {
        switch (c) {
            case 'j' :
            case 'k' :
            case 'l' :
            case 'h' :
            case 'c' : NH_TTY_END(NH_TTY_SUCCESS)
        }
    }

    switch (c)
    {
        case 'j' : 
        case 'k' :
        case 'l' :
        case 'h' :
        case 'c' :
        case CTRL_KEY('j') : 
        case CTRL_KEY('k') : 
        case CTRL_KEY('l') : 
        case CTRL_KEY('h') : 
        case CTRL_KEY('c') : *read_p = NH_TRUE;
    }

    switch (c)
    {
        case 'j' : 
        case CTRL_KEY('j') : 

            if (TextFile_p->fileCursorY < TextFile_p->Lines.size - 1)
            {
                TextFile_p->fileCursorY++;

                NH_TTY_CHECK(nh_tty_handleTextFileViews(Views_p, TextFile_p, 'j', refresh_p))
                NH_TTY_CHECK(nh_tty_handleFileCursorXTarget(Views_p, File_p, refresh_p))
                NH_TTY_CHECK(nh_tty_handleCopySelection(TextFile_p, Line_p, 'j', refresh_p))

                // Force refresh.
                *refresh_p = NH_TRUE;
            }
            break; 

        case 'k' :
        case CTRL_KEY('k') : 

            if (TextFile_p->fileCursorY > 0)
            {
                TextFile_p->fileCursorY--;

                NH_TTY_CHECK(nh_tty_handleTextFileViews(Views_p, TextFile_p, 'k', refresh_p))
                NH_TTY_CHECK(nh_tty_handleFileCursorXTarget(Views_p, File_p, refresh_p))
                NH_TTY_CHECK(nh_tty_handleCopySelection(TextFile_p, Line_p, 'k', refresh_p))

                // Force refresh.
                *refresh_p = NH_TRUE;
            }
            break; 

        case 'l' :
        case CTRL_KEY('l') : 

            if (TextFile_p->fileCursorX < Line_p->Codepoints.length) 
            {
                TextFile_p->fileCursorX++;
                TextFile_p->fileCursorXTarget = TextFile_p->fileCursorX;

                NH_TTY_CHECK(nh_tty_handleTextFileViews(Views_p, TextFile_p, 'l', refresh_p))
                NH_TTY_CHECK(nh_tty_handleCopySelection(TextFile_p, Line_p, 'l', refresh_p))

                // Force refresh.
                *refresh_p = NH_TRUE;
            }
            break;

        case 'h' :
        case CTRL_KEY('h') : 

            if (TextFile_p->fileCursorX > 0) 
            {
                TextFile_p->fileCursorX--;
                TextFile_p->fileCursorXTarget = TextFile_p->fileCursorX;

                NH_TTY_CHECK(nh_tty_handleTextFileViews(Views_p, TextFile_p, 'h', refresh_p))
                NH_TTY_CHECK(nh_tty_handleCopySelection(TextFile_p, Line_p, 'h', refresh_p))

                // Force refresh.
                *refresh_p = NH_TRUE;
            }
            break;

         case 'c' :
         case CTRL_KEY('c') :

             if (TextFile_p->select >= 0) {
                 NH_TTY_CHECK(nh_tty_setClipboard(TextFile_p, NH_FALSE, refresh_p))
                 TextFile_p->select = -1;
             }
             else {TextFile_p->select = TextFile_p->fileCursorY;}
             break;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// WRITE ===========================================================================================

NH_TTY_RESULT nh_tty_handleWriteOperation(
    nh_List *Views_p, nh_tty_File *File_p, NH_ENCODING_UTF32 c, NH_BOOL insertMode, NH_BOOL *refresh_p, 
    NH_BOOL *write_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = nh_tty_getCurrentProgram(&NH_TTY_MACRO_TAB(((nh_tty_TTY*)nh_core_getWorkloadArg())->Window_p->Tile_p)->MicroWindow)->handle_p;
    nh_tty_TextFile *TextFile_p = File_p->handle_p;
    nh_tty_TextFileLine *Line_p = nh_core_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY);
    NH_TTY_CHECK_NULL(Line_p)

    switch (c)
    {
        case 9 : // tab
        case 13  : // enter
        case 127 : // delete
        case CTRL_KEY('o') : // new line
        case CTRL_KEY('x') : 
        case CTRL_KEY('u') : // delete
        case CTRL_KEY('p') : // forward delete
        case CTRL_KEY('v') : // paste 
            *write_p = NH_TRUE;
    }

    if (*write_p) {*refresh_p = NH_TRUE;}

    switch (c)
    {
        case CTRL_KEY('o') :

            NH_TTY_CHECK_MEM(nh_tty_newTextFileLine(TextFile_p, TextFile_p->fileCursorY + 1))
            nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY + 1);
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 'j', NH_FALSE, refresh_p))
            break;
 
        case CTRL_KEY('p') :

            NH_TTY_CHECK(nh_tty_removeFromTextFileLine(Line_p, TextFile_p->fileCursorX, 1))
            NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY))
            break;

        case CTRL_KEY('x') :

            if (TextFile_p->fileCursorY >= TextFile_p->Lines.size - 1) {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('k'), NH_FALSE, refresh_p))
                NH_CORE_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_core_removeFromList(&TextFile_p->Lines, NH_TRUE, TextFile_p->fileCursorY + 1))
            }
            else {
                NH_CORE_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_core_removeFromList(&TextFile_p->Lines, NH_TRUE, TextFile_p->fileCursorY))
            }
            break;

        case 127 : // delete
        case CTRL_KEY('u') :

            if (TextFile_p->fileCursorX > 0) {
                NH_TTY_CHECK(nh_tty_removeFromTextFileLine(Line_p, TextFile_p->fileCursorX - 1, 1))
                NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY))
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('h'), NH_FALSE, refresh_p))
            }
            else {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('k'), NH_FALSE, refresh_p))

                nh_tty_TextFileLine *PreviousLine_p = 
                    nh_core_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY);
                int length = PreviousLine_p->Codepoints.length;

                while (TextFile_p->fileCursorX < length) {
                    NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('l'), NH_FALSE, refresh_p))
                }

                for (int i = 0; i < Line_p->Codepoints.length; ++i) {
                    NH_TTY_CHECK(nh_tty_handleTextFileInput(
                        Views_p, File_p, Line_p->Codepoints.p[i], NH_TRUE, refresh_p
                    ))
                }

                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('j'), NH_FALSE, refresh_p))
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('x'), NH_FALSE, refresh_p))
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('k'), NH_FALSE, refresh_p))

                for (int i = 0; i < length; ++i) {
                    NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('l'), NH_FALSE, refresh_p))
                }
            }
            break;

        case CTRL_KEY('v') : // insert

            NH_TTY_CHECK(nh_tty_insertClipboard(Views_p, File_p, refresh_p))
            break;

        case 9 : // tab

            if (Editor_p->FileEditor.tabToSpaces) {
                for (int i = 0; i < Editor_p->FileEditor.tabSpaces; ++i) {
                    NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, 32, NH_TRUE, refresh_p))
                }
            } else {*write_p = NH_FALSE;}
            break;

        case 13 : // carriage-return aka enter
        {
            int fileCursorX = TextFile_p->fileCursorX;
            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('o'), NH_FALSE, refresh_p))

            for (int i = fileCursorX; i < Line_p->Codepoints.length; ++i) {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, Line_p->Codepoints.p[i], NH_TRUE, refresh_p))
            }
            while (TextFile_p->fileCursorX > 0) {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('h'), NH_FALSE, refresh_p))
            }

            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('k'), NH_FALSE, refresh_p))

            while (TextFile_p->fileCursorX < fileCursorX) {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('l'), NH_FALSE, refresh_p))
            }
            while (TextFile_p->fileCursorX < Line_p->Codepoints.length) {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('p'), NH_FALSE, refresh_p))
            }

            NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('j'), NH_FALSE, refresh_p))
            while (TextFile_p->fileCursorX > 0) {
                NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('h'), NH_FALSE, refresh_p))
            }

            break;
        }
    }
 
NH_TTY_END(NH_TTY_SUCCESS)
}

// HANDLE INPUT ====================================================================================

NH_TTY_RESULT nh_tty_handleTextFileInput(
    nh_List *Views_p, nh_tty_File *File_p, NH_ENCODING_UTF32 c, NH_BOOL insertMode, NH_BOOL *refresh_p)
{
NH_TTY_BEGIN()

    NH_BOOL read = NH_FALSE;
    NH_TTY_CHECK(nh_tty_handleReadOperation(Views_p, File_p, c, insertMode, refresh_p, &read))
    if (read || File_p->readOnly) {NH_TTY_END(NH_TTY_SUCCESS)}

    NH_BOOL write = NH_FALSE;
    NH_TTY_CHECK(nh_tty_handleWriteOperation(Views_p, File_p, c, insertMode, refresh_p, &write))
    if (write) {NH_TTY_END(NH_TTY_SUCCESS)}

    if (insertMode) {
        nh_tty_TextFile *TextFile_p = File_p->handle_p;
        nh_tty_TextFileLine *Line_p = nh_core_getFromList(&TextFile_p->Lines, TextFile_p->fileCursorY);
        NH_TTY_CHECK(nh_tty_insertIntoTextFileLine(Line_p, TextFile_p->fileCursorX, c))
        NH_TTY_CHECK(nh_tty_handleTextFileInput(Views_p, File_p, CTRL_KEY('l'), NH_FALSE, refresh_p))
        NH_TTY_CHECK(nh_tty_renderTextFileLine(TextFile_p, TextFile_p->fileCursorY))
        *refresh_p = NH_TRUE;
    }
 
    if (!insertMode) {
        nh_tty_setCustomSuffixMessage(NULL, NH_TTY_MESSAGE_EDITOR_IGNORE_INPUT, &c, 1);
        *refresh_p = NH_TRUE;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

