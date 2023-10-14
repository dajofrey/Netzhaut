// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Editor.h"
#include "TextFile.h"

#include "../TTY/TTY.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Logger.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhencoding/Common/Macros.h"
#include "../../nhencoding/Encodings/UTF32.h"
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

static nh_tty_EditorView nh_tty_initEditorView()
{
NH_TTY_BEGIN()

    nh_tty_EditorView View;

    memset(&View, 0, sizeof(nh_tty_EditorView));
    View.FileEditor.FileViews = nh_core_initList(8);

NH_TTY_END(View)
}

static void nh_tty_freeEditorView(
    nh_tty_EditorView *View_p)
{
NH_TTY_BEGIN()

    nh_core_freeList(&View_p->FileEditor.FileViews, NH_TRUE);
 
NH_TTY_SILENT_END()
}

// UPDATE ==========================================================================================

static void nh_tty_closeTreeListingNode(
    nh_tty_FileEditor *Editor_p, nh_tty_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

//    for (int i = 0; i < Node_p->Children.size; ++i) {
//        nh_tty_TreeListingNode *Child_p = Node_p->Children.pp[i];
//        nh_encoding_freeUTF32(&Child_p->Path);
//        nh_tty_closeTreeListingNode(Editor_p, Child_p);
//    }
//
//    if (Node_p->File_p) {nh_tty_closeFile(Editor_p, Node_p->File_p);}
//
//    nh_core_freeList(&Node_p->Children, NH_TRUE);

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT nh_tty_handleDeletedNodes(
    nh_tty_FileEditor *Editor_p, nh_tty_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

//#ifdef __unix__
//
//    struct dirent **namelist_pp = NULL;
//    nh_encoding_UTF8String CurrentPath = nh_encoding_encodeUTF8(Node_p->Path.p, Node_p->Path.length);
//
//    int n = scandir(CurrentPath.p, &namelist_pp, 0, alphasort);
//    if (n < 0) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}
//
//    nh_encoding_freeUTF8(&CurrentPath);
//
//    for (int i = 0; i < Node_p->Children.size; ++i)
//    {
//        nh_tty_TreeListingNode *Child_p = Node_p->Children.pp[i];
//        if (Child_p->unsaved) {continue;}
//        
//        NH_BOOL match = NH_FALSE;
//        for (int j = 0; j < n; ++j)
//        {
//            if (!strcmp(namelist_pp[j]->d_name, ".") || !strcmp(namelist_pp[j]->d_name, "..")) {continue;}
//
//            nh_encoding_UTF32String NewPath = nh_encoding_initUTF32(128);
//            nh_encoding_UTF32String FileName = nh_encoding_decodeUTF8(namelist_pp[j]->d_name, strlen(namelist_pp[j]->d_name), NULL);
//
//            if (Node_p->Path.p[Node_p->Path.length - 1] != '/') {
//                nh_encoding_appendUTF32(&NewPath, Node_p->Path.p, Node_p->Path.length);
//                nh_encoding_appendUTF32Codepoint(&NewPath, '/');
//                nh_encoding_appendUTF32(&NewPath, FileName.p, FileName.length);
//            } else {
//                nh_encoding_appendUTF32(&NewPath, Node_p->Path.p, Node_p->Path.length);
//                nh_encoding_appendUTF32(&NewPath, FileName.p, FileName.length);
//            }
//
//            if (nh_encoding_compareUTF32(&Child_p->Path, &NewPath)) {match = NH_TRUE;}
//
//            nh_encoding_freeUTF32(&NewPath);
//            nh_encoding_freeUTF32(&FileName);
//        }
//
//        if (!match) {
//            nh_tty_closeTreeListingNode(Editor_p, Child_p);
//            nh_core_removeFromList(&Node_p->Children, NH_TRUE, i);
//            i--;
//            *updated_p = NH_TRUE;
//            continue;
//        }
//
//        if (Child_p->Children.size > 0) {
//            NH_TTY_CHECK(nh_tty_handleDeletedNodes(Editor_p, Child_p, updated_p))
//        }
//    }
//
//    if (namelist_pp) {
//        for (int i = 0; i < n; ++i) {
//            free(namelist_pp[i]);
//        }
//        free(namelist_pp);
//    }
//
//#elif defined(_WIN32) || defined(WIN32)
//
//    printf("microsoft windows not supported\n");
//    exit(0);
//
//#endif

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_updateEditor(
    nh_tty_Program *Program_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;

    if (!Editor_p->treeListing) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    nh_SystemTime Now = nh_core_getSystemTime();
    if (nh_core_getSystemTimeDiffInSeconds(Editor_p->LastUpdate, Now) < Editor_p->updateIntervalInSeconds) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    NH_TTY_CHECK(nh_tty_handleDeletedNodes(&Editor_p->FileEditor, Editor_p->TreeListing.Root_p))

    Editor_p->LastUpdate = Now;
    Editor_p->TreeListing.dirty = NH_TRUE;
    Program_p->refresh = NH_TRUE;
    Editor_p->LastUpdate = Now;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static NH_TTY_RESULT nh_tty_handleEditorInput(
    nh_tty_Program *Program_p, nh_wsi_Event Event)
{
NH_TTY_BEGIN()

    if (Event.type != NH_WSI_EVENT_KEYBOARD) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}
    if (Event.Keyboard.trigger != NH_WSI_TRIGGER_PRESS) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    NH_ENCODING_UTF32 c = Event.Keyboard.codepoint;
    nh_tty_Editor *Editor_p = Program_p->handle_p;

    switch (c) 
    {
        case 'i' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}
        case CTRL_KEY('i') :
        case 27 :
            Editor_p->insertMode = c == 27 ? NH_FALSE : !Editor_p->insertMode; 
            NH_TTY_CHECK(nh_tty_setDefaultMessage(
                NULL, Editor_p->insertMode ? NH_TTY_MESSAGE_EDITOR_INSERT_ACTIVATED : NH_TTY_MESSAGE_EDITOR_INSERT_DEACTIVATED 
            ))
            Program_p->refresh = NH_TRUE;
            break;

        case 'f' :
        case 'g' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}

            // Switching file focus is mostly done in the file editor using the next function.
            NH_TTY_CHECK(nh_tty_cycleThroughFiles(Program_p, c))
            Program_p->refresh = NH_TRUE;
            break;

        case 'w' :
        case 'a' :
        case 's' :
        case 'd' : if (Editor_p->insertMode) {goto FILE_EDITOR_INPUT;}
            if  (Editor_p->treeListing) {
                NH_TTY_CHECK(nh_tty_handleTreeListingInput(Program_p, c))
            }
            Editor_p->treeListing = NH_TRUE;
            Program_p->refresh = NH_TRUE;
            break;

        default :
            goto FILE_EDITOR_INPUT;
    }
 
NH_TTY_END(NH_TTY_SUCCESS)

FILE_EDITOR_INPUT :
    NH_TTY_CHECK(nh_tty_handleFileEditorInput(Program_p, c))

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static NH_TTY_RESULT nh_tty_drawEditor(
    nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;
    Editor_p->View.height = height;

    // First, normalize glyphs.
    for (int i = 0; i < width; ++i) {
        Glyphs_p[i].codepoint = ' ';
    }

    // Draw tree listing row if necessary.
    if (Editor_p->treeListing) {
        NH_TTY_CHECK(nh_tty_drawTreeListingRow(Program_p, Glyphs_p, width, height, row))
        Glyphs_p += Editor_p->View.TreeListing.width;
    }
    
    // Draw file editor row.
    NH_TTY_CHECK(nh_tty_drawFileEditorRow(
        Program_p, Glyphs_p, Editor_p->treeListing ? width - Editor_p->View.TreeListing.width : width, 
        height, row
    ))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_drawEditorTopbar(
    nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;
    nh_tty_File *File_p = nh_core_getFromLinkedList(&Editor_p->FileEditor.Files, Editor_p->FileEditor.current);

    NH_BYTE topbar_p[1024] = {0};
    memset(topbar_p, ' ', 1024);

    if (File_p) {
        nh_encoding_UTF8String Path = nh_encoding_encodeUTF8(File_p->Node_p->Path.p, File_p->Node_p->Path.length);
        int offset = (width / 2) - (strlen(Path.p) / 2);
        if (offset >= 0) {
            sprintf(topbar_p+offset, "%s", Path.p);
        }
        nh_encoding_freeUTF8(&Path);
    } else {
        NH_BYTE buf_p[] = "No file open";
        int offset = (width / 2) - (strlen(buf_p) / 2);
        if (offset >= 0) {
            sprintf(topbar_p+offset, "%s", buf_p);
        }
    }

    for (int i = 0; i < width; ++i) {
        Glyphs_p[i].codepoint = topbar_p[i];
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// CURSOR ==========================================================================================

static NH_TTY_RESULT nh_tty_getEditorCursor(
    nh_tty_Program *Program_p, int *x_p, int *y_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;
    nh_tty_FileView *FileView_p = NULL;

    int x = Editor_p->treeListing ? Editor_p->View.TreeListing.width : 0;

    nh_tty_File *File_p = NULL;
    for (int i = Editor_p->View.FileEditor.currentOffset; i < Editor_p->FileEditor.Files.count; ++i) {
        File_p = nh_core_getFromLinkedList(&Editor_p->FileEditor.Files, i);
        FileView_p = nh_tty_getFileView(&Editor_p->View, File_p);
        if (i == Editor_p->FileEditor.current) {break;}
        x += FileView_p->width;
    }

    if (File_p != NULL && File_p->type == NH_TTY_FILE_TEXT) 
    {
        nh_tty_TextFile *TextFile_p = File_p->handle_p;
        *x_p = FileView_p->TextFile.screenCursorX + TextFile_p->lineNumberOffset + x;
        *y_p = FileView_p->TextFile.screenCursorY;
    }
    else {
        *x_p = -1;
        *y_p = -1;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// COMMANDS ========================================================================================

static NH_TTY_RESULT nh_tty_executeEditorCommand(
    nh_tty_Program *Program_p, nh_List *Arguments_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;

//    switch (Program_p->command)
//    {
//        case NH_TTY_EDITOR_COMMAND_PREVIEW:
//
//            Editor_p->TreeListing.preview = !Editor_p->TreeListing.preview; 
//            NH_TTY_CHECK(nh_tty_setDefaultMessage(
//               &nh_tty_getTTY()->Tab_p->Tile_p->Status,
//               Editor_p->TreeListing.preview ? NH_TTY_MESSAGE_EDITOR_PREVIEW_ENABLED : NH_TTY_MESSAGE_EDITOR_PREVIEW_DISABLED
//            ))
//            Program_p->refresh = NH_TRUE;
//            break;
//
//        case NH_TTY_EDITOR_COMMAND_TREE:
//
//            Editor_p->treeListing = !Editor_p->treeListing; 
//            NH_TTY_CHECK(nh_tty_setDefaultMessage(
//               &nh_tty_getTTY()->Tab_p->Tile_p->Status,
//               Editor_p->treeListing ? NH_TTY_MESSAGE_EDITOR_SHOW_TREE : NH_TTY_MESSAGE_EDITOR_HIDE_TREE 
//            ))
//            Program_p->refresh = NH_TRUE;
//            break;
//
//        case NH_TTY_EDITOR_COMMAND_NEW:
//        {
//            if (Arguments_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_INVALID_ARGUMENT)}
//            nh_encoding_UTF32String *Argument_p = Arguments_p->pp[0];
//
//            nh_tty_TreeListingNode *Node_p = 
//                nh_tty_insertTreeListingNode(&Editor_p->TreeListing, Argument_p->p, Argument_p->length);
//            NH_TTY_CHECK_NULL(Node_p)
//
//            nh_tty_File *File_p = nh_tty_openFile(Program_p, Node_p, NH_FALSE);
//            NH_TTY_CHECK_NULL(File_p)
//
//            NH_TTY_CHECK(nh_tty_setDefaultMessage(
//                &nh_tty_getTTY()->Tab_p->Tile_p->Status, NH_TTY_MESSAGE_EDITOR_NEW_FILE
//            ))
//
//            Program_p->refresh = NH_TRUE;
//            NH_TTY_CHECK(nh_tty_setTreeListingCursor(Program_p, File_p))
//            break;
//        }
//
//        case NH_TTY_EDITOR_COMMAND_SEARCH:
//        {
//            if (Editor_p->FileEditor.Files.count > 0) 
//            {
//                nh_tty_File *File_p = 
//                    nh_core_getFromLinkedList(&Editor_p->FileEditor.Files, Editor_p->FileEditor.current);
//
//                if (Arguments_p->size == 0) {
//                    NH_TTY_CHECK(nh_tty_clearFileSearch(File_p))
//                }
//                else {
//                    nh_encoding_UTF32String *Argument_p = Arguments_p->pp[0];
//                    NH_TTY_CHECK(nh_tty_searchFile(File_p, Argument_p->p, Argument_p->length))
//                }
//            }
//            break;
//        }
//
//        case NH_TTY_EDITOR_COMMAND_TAB_SPACES:
//
//            if (Arguments_p->size == 1) {
//                nh_encoding_UTF32String *Argument_p = Arguments_p->pp[0];
//                nh_encoding_UTF8String Argument = nh_encoding_encodeUTF8(Argument_p->p, Argument_p->length);
//                Editor_p->FileEditor.tabSpaces = strtol(Argument.p, NULL, 10);
//                nh_encoding_freeUTF8(&Argument);
//            }
//
//            NH_BYTE tabSpaces_p[16];
//            sprintf(tabSpaces_p, "%d", Editor_p->FileEditor.tabSpaces);
//            nh_encoding_UTF32String TabSpaces = nh_encoding_decodeUTF8(tabSpaces_p, strlen(tabSpaces_p), NULL);
//
//            NH_TTY_CHECK(nh_tty_setCustomMessage(
//               &nh_tty_getTTY()->Tab_p->Tile_p->Status, NH_TTY_MESSAGE_EDITOR_NUMBER_OF_TAB_SPACES,
//               TabSpaces.p, TabSpaces.length
//            ))
//
//            nh_encoding_freeUTF32(&TabSpaces);
//            break;
//
//        case NH_TTY_EDITOR_COMMAND_TAB_TO_SPACES:
//
//            Editor_p->FileEditor.tabToSpaces = !Editor_p->FileEditor.tabToSpaces;
//            NH_TTY_CHECK(nh_tty_setDefaultMessage(
//               &nh_tty_getTTY()->Tab_p->Tile_p->Status, Editor_p->FileEditor.tabToSpaces ? 
//               NH_TTY_MESSAGE_EDITOR_TAB_TO_SPACES_ENABLED : NH_TTY_MESSAGE_EDITOR_TAB_TO_SPACES_DISABLED
//            ))
//            break;
//
//        default : NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_UNKNOWN_COMMAND)
//    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INIT/DESTROY ====================================================================================

static void *nh_tty_initEditor(
    void *arg_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = nh_core_allocate(sizeof(nh_tty_Editor));
    NH_TTY_CHECK_MEM_2(NULL, Editor_p)

    Editor_p->View        = nh_tty_initEditorView();
    Editor_p->focus       = 0;
    Editor_p->insertMode  = NH_FALSE;
    Editor_p->treeListing = NH_TRUE;
    Editor_p->TreeListing = nh_tty_initTreeListing();
    Editor_p->FileEditor  = nh_tty_initFileEditor();

    Editor_p->LastUpdate = nh_core_getSystemTime();
    Editor_p->updateIntervalInSeconds = 0.2;

NH_TTY_END(Editor_p)
}

static void nh_tty_destroyEditor(
    void *handle_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = handle_p;

    nh_tty_freeEditorView(&Editor_p->View);
    nh_tty_freeFileEditor(&Editor_p->FileEditor);
    nh_tty_freeTreeListing(&Editor_p->TreeListing);

    nh_core_free(Editor_p);

NH_TTY_SILENT_END()
}

// PROTOTYPE =======================================================================================

typedef enum NH_TTY_EDITOR_COMMAND_E {
    NH_TTY_EDITOR_COMMAND_PREVIEW = 0,
    NH_TTY_EDITOR_COMMAND_TREE,
    NH_TTY_EDITOR_COMMAND_NEW,
    NH_TTY_EDITOR_COMMAND_E_COUNT,
} NH_TTY_EDITOR_COMMAND_E;

static void nh_tty_destroyEditorPrototype(
    nh_tty_ProgramPrototype *Prototype_p)
{
NH_TTY_BEGIN()

    nh_encoding_freeUTF32(&Prototype_p->Name);
    nh_encoding_freeUTF32(&Prototype_p->CommandNames_p[0]);
    nh_encoding_freeUTF32(&Prototype_p->CommandNames_p[1]);
    nh_encoding_freeUTF32(&Prototype_p->CommandNames_p[2]);
    nh_core_free(Prototype_p->CommandNames_p);
    nh_core_free(Prototype_p);

NH_TTY_SILENT_END()
}

nh_tty_ProgramPrototype *nh_tty_createEditorPrototype()
{
NH_TTY_BEGIN()

    nh_tty_ProgramPrototype *Prototype_p = nh_core_allocate(sizeof(nh_tty_ProgramPrototype));
    NH_TTY_CHECK_MEM_2(NULL, Prototype_p)

    memset(Prototype_p, 0, sizeof(nh_tty_ProgramPrototype));

    Prototype_p->Callbacks.init_f = nh_tty_initEditor;
    Prototype_p->Callbacks.draw_f = nh_tty_drawEditor;
    Prototype_p->Callbacks.drawTopbar_f = nh_tty_drawEditorTopbar;
    Prototype_p->Callbacks.handleInput_f = nh_tty_handleEditorInput;
    Prototype_p->Callbacks.getCursorPosition_f = nh_tty_getEditorCursor;
    Prototype_p->Callbacks.update_f = nh_tty_updateEditor;
    Prototype_p->Callbacks.handleCommand_f = nh_tty_executeEditorCommand;
    Prototype_p->Callbacks.destroyPrototype_f = nh_tty_destroyEditorPrototype;
    Prototype_p->Callbacks.destroy_f = nh_tty_destroyEditor;

    NH_ENCODING_UTF32 name_p[7] = {'e', 'd', 'i', 't', 'o', 'r'};
    Prototype_p->Name = nh_encoding_initUTF32(6);
    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&Prototype_p->Name, name_p, 6))

    nh_encoding_UTF32String *CommandNames_p =
        nh_core_allocate(sizeof(nh_encoding_UTF32String)*NH_TTY_EDITOR_COMMAND_E_COUNT);
    NH_TTY_CHECK_MEM_2(NULL, CommandNames_p)

    NH_ENCODING_UTF32 command1_p[7] = {'p', 'r', 'e', 'v', 'i', 'e', 'w'};
    NH_ENCODING_UTF32 command2_p[4] = {'t', 'r', 'e', 'e'};
    NH_ENCODING_UTF32 command3_p[3] = {'n', 'e', 'w'};

    CommandNames_p[NH_TTY_EDITOR_COMMAND_PREVIEW] = nh_encoding_initUTF32(7);
    CommandNames_p[NH_TTY_EDITOR_COMMAND_TREE] = nh_encoding_initUTF32(4);
    CommandNames_p[NH_TTY_EDITOR_COMMAND_NEW] = nh_encoding_initUTF32(3);

    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&CommandNames_p[NH_TTY_EDITOR_COMMAND_PREVIEW], command1_p, 7))
    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&CommandNames_p[NH_TTY_EDITOR_COMMAND_TREE], command2_p, 4))
    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&CommandNames_p[NH_TTY_EDITOR_COMMAND_NEW], command3_p, 3))

    Prototype_p->CommandNames_p = CommandNames_p;
    Prototype_p->commands = NH_TTY_EDITOR_COMMAND_E_COUNT;

NH_TTY_END(Prototype_p)
}

