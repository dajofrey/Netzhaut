// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TreeListing.h"
#include "Editor.h"

#include "../TTY/TTY.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Common/Macros.h"

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

// HELPER ==========================================================================================

static int nh_tty_isRegularFile(
    nh_tty_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    if (Node_p->unsaved) {NH_TTY_END(NH_TRUE)}

    nh_encoding_UTF8String Path = nh_encoding_encodeUTF8(Node_p->Path.p, Node_p->Path.length);
    struct stat path_stat;
    stat(Path.p, &path_stat);
    nh_encoding_freeUTF8(&Path);

NH_TTY_END(S_ISREG(path_stat.st_mode))
}

static NH_TTY_RESULT nh_tty_getNodeList(
    nh_List *List_p, nh_tty_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    nh_core_appendToList(List_p, Node_p);

    if (!Node_p->open) {NH_TTY_END(NH_TTY_SUCCESS)}

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_tty_getNodeList(List_p, Node_p->Children.pp[i]);
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

static nh_tty_TreeListingNode *nh_tty_getCurrentNode(
    nh_tty_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    int current = 0; 
    nh_List Nodes = nh_core_initList(128);
    nh_tty_getNodeList(&Nodes, Listing_p->Root_p);

    nh_tty_TreeListingNode *Current_p = Nodes.pp[Listing_p->current];

    nh_core_freeList(&Nodes, NH_FALSE);

NH_TTY_END(Current_p)
}

static nh_tty_TreeListingNode *nh_tty_getTreeListingNode(
    nh_tty_TreeListing *Listing_p, nh_encoding_UTF32String *Path_p)
{
NH_TTY_BEGIN()

    int current = 0; 
    nh_List Nodes = nh_core_initList(128);
    nh_tty_getNodeList(&Nodes, Listing_p->Root_p);

    nh_tty_TreeListingNode *Result_p = NULL;
    for (int i = 0; i < Nodes.size; ++i) {
        if (nh_encoding_compareUTF32(((nh_tty_TreeListingNode*)Nodes.pp[i])->Path.p, Path_p->p)) {
            Result_p = Nodes.pp[i];
            break;
        }
    }

    nh_core_freeList(&Nodes, NH_FALSE);

NH_TTY_END(Result_p)
}

// CREATE ==========================================================================================

static nh_tty_TreeListingNode *nh_tty_createTreeListingNode(
    nh_tty_TreeListingNode *Parent_p, nh_encoding_UTF32String Path, nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    nh_tty_TreeListingNode *Node_p = nh_core_allocate(sizeof(nh_tty_TreeListingNode));
    NH_TTY_CHECK_MEM_2(NULL, Node_p)

    Node_p->open      = NH_FALSE;
    Node_p->unsaved   = NH_FALSE;
    Node_p->Path      = Path;
    Node_p->Children  = nh_core_initList(16);
    Node_p->Parent_p  = Parent_p;
    Node_p->File_p    = File_p;

    Node_p->Overflow.offset    = 0;
    Node_p->Overflow.LastShift = nh_core_getSystemTime();

NH_TTY_END(Node_p)
}

static NH_TTY_RESULT nh_tty_freeTreeListingNode(
    nh_tty_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    nh_encoding_freeUTF32(&Node_p->Path);

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_tty_freeTreeListingNode(Node_p->Children.pp[i]);
    }
    nh_core_freeList(&Node_p->Children, NH_TRUE);

NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_openNode(
    nh_tty_TreeListingNode *Node_p)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK_NULL(Node_p)

    if (Node_p->Path.length <= 0 || Node_p->Children.size > 0 || nh_tty_isRegularFile(Node_p)) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

#ifdef __unix__ 

    struct dirent **namelist_pp;
    nh_encoding_UTF8String CurrentPath = nh_encoding_encodeUTF8(Node_p->Path.p, Node_p->Path.length);

    int n = scandir(CurrentPath.p, &namelist_pp, 0, alphasort);
    if (n < 0) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}

    nh_encoding_freeUTF8(&CurrentPath);

    for (int i = 0; i < n; ++i) 
    {
        if (strcmp(namelist_pp[i]->d_name, ".") && strcmp(namelist_pp[i]->d_name, "..")) 
        {
            nh_encoding_UTF32String NewPath = nh_encoding_initUTF32(128);
            nh_encoding_UTF32String FileName = nh_encoding_decodeUTF8(namelist_pp[i]->d_name, strlen(namelist_pp[i]->d_name), NULL);

            if (Node_p->Path.p[Node_p->Path.length - 1] != '/') {
                nh_encoding_appendUTF32(&NewPath, Node_p->Path.p, Node_p->Path.length);
                nh_encoding_appendUTF32Codepoint(&NewPath, '/');
                nh_encoding_appendUTF32(&NewPath, FileName.p, FileName.length);
            } else {
                nh_encoding_appendUTF32(&NewPath, Node_p->Path.p, Node_p->Path.length);
                nh_encoding_appendUTF32(&NewPath, FileName.p, FileName.length);
            }

            nh_encoding_freeUTF32(&FileName);

            nh_tty_TreeListingNode *New_p = nh_tty_createTreeListingNode(Node_p, NewPath, NULL);
            NH_TTY_CHECK_MEM(New_p)

            NH_CORE_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_core_appendToList(&Node_p->Children, New_p))
        }

        free(namelist_pp[i]);
    }

    if (Node_p->Children.size == 0) {
        NH_TTY_CHECK(nh_tty_setCustomSuffixMessage(
            NULL, NH_TTY_MESSAGE_EDITOR_EMPTY_DIRECTORY, Node_p->Path.p, Node_p->Path.length
        ))
    }

    free(namelist_pp);

#elif defined(_WIN32) || defined(WIN32)

    printf("microsoft windows not supported\n");
    exit(0);

#endif

    Node_p->open = NH_TRUE;

NH_TTY_END(NH_TTY_SUCCESS)
}

nh_tty_TreeListingNode *nh_tty_insertTreeListingNode(
    nh_tty_TreeListing *Listing_p, NH_ENCODING_UTF32 *name_p, int length)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK_NULL_2(NULL, name_p)
    if (length <= 0) {NH_TTY_END(NULL)}

    nh_tty_TreeListingNode *Current_p = nh_tty_getCurrentNode(Listing_p);

    int offset = Current_p->Path.length;
    if (nh_tty_isRegularFile(Current_p)) {
        for (offset = Current_p->Path.length - 1; Current_p->Path.p[offset] != '/'; --offset);
    }

    offset = Current_p->Path.length - offset;

    nh_encoding_UTF32String Path = nh_encoding_initUTF32(128);
    nh_encoding_appendUTF32(&Path, Current_p->Path.p, Current_p->Path.length - offset);
    nh_encoding_appendUTF32Codepoint(&Path, '/');
    nh_encoding_appendUTF32(&Path, name_p, length);

    nh_tty_TreeListingNode *New_p = NULL;

    if (!nh_tty_getTreeListingNode(Listing_p, &Path)) {
        if (Current_p->Children.size > 0) {
            New_p = nh_tty_createTreeListingNode(Current_p, Path, NULL);
            NH_CORE_CHECK_2(NULL, nh_core_appendToList(&Current_p->Children, New_p))
        }
        else if (Current_p->Parent_p != NULL) {
            New_p = nh_tty_createTreeListingNode(Current_p->Parent_p, Path, NULL);
            NH_CORE_CHECK_2(NULL, nh_core_appendToList(&Current_p->Parent_p->Children, New_p))
        }
        else {NH_TTY_END(NULL)}
        NH_TTY_CHECK_MEM_2(NULL, New_p)
        New_p->unsaved = NH_TRUE;
        Listing_p->dirty = NH_TRUE;
    }
    else {
        NH_TTY_CHECK_2(NULL, nh_tty_setDefaultMessage(NULL, NH_TTY_MESSAGE_EDITOR_FILE_ALREADY_EXISTS))
        nh_encoding_freeUTF32(&Path);
    }

NH_TTY_END(New_p)
}

// INPUT ===========================================================================================

static NH_TTY_RESULT nh_tty_removeFile(
    nh_tty_Editor *Editor_p)
{
NH_TTY_BEGIN()

    nh_tty_TreeListingNode *Node_p = nh_tty_getCurrentNode(&Editor_p->TreeListing);
    nh_encoding_UTF8String Path = nh_encoding_encodeUTF8(Node_p->Path.p, Node_p->Path.length);

    remove(Path.p);
    nh_encoding_freeUTF8(&Path);

    NH_TTY_CHECK(nh_tty_setCustomSuffixMessage(
        NULL, NH_TTY_MESSAGE_EDITOR_FILE_REMOVED, Node_p->Path.p, Node_p->Path.length
    ))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_delete(
    nh_wsi_KeyboardEvent Event, NH_BOOL *continue_p)
{
NH_TTY_BEGIN()

    if (Event.trigger != NH_WSI_TRIGGER_PRESS) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    NH_ENCODING_UTF32 c = Event.codepoint;

    if (c == 'y' || c == 'n') 
    {
        if (c == 'n') {
            NH_TTY_CHECK(nh_tty_setDefaultMessage(NULL, NH_TTY_MESSAGE_BINARY_QUERY_DELETE_INTERRUPTED))
        }
        if (c == 'y') {
//            nh_tty_Program *Program_p = nh_tty_getCurrentProgram(&nh_tty_getTTY()->Tab_p->Tile_p->TopBar);
//            nh_tty_removeFile(Program_p->handle_p);
//            NH_TTY_CHECK(nh_tty_handleTreeListingInput(
//                Program_p, ((nh_tty_Editor*)Program_p->handle_p)->height, 'w'
//            ))
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_setCurrentToRoot(
    nh_tty_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    nh_tty_TreeListingNode *Current_p = nh_tty_getCurrentNode(Listing_p);

    if (Current_p->Children.size > 0) {
        for (nh_tty_TreeListingNode *Parent_p = Current_p; Parent_p = Parent_p->Parent_p;) {
            Parent_p->open = NH_FALSE;
        }
        Listing_p->Root_p = Current_p;
    }

    NH_TTY_CHECK(nh_tty_setCustomSuffixMessage(
        NULL, NH_TTY_MESSAGE_EDITOR_NEW_ROOT, Listing_p->Root_p->Path.p, Listing_p->Root_p->Path.length
    ))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_setParentToRoot(
    nh_tty_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    if (Listing_p->Root_p->Parent_p != NULL) {
        Listing_p->Root_p = Listing_p->Root_p->Parent_p;
        Listing_p->Root_p->open = NH_TRUE;
    }
    else {
        nh_encoding_UTF32String Path = nh_encoding_initUTF32(128);
        nh_encoding_appendUTF32(&Path, Listing_p->Root_p->Path.p, Listing_p->Root_p->Path.length);

        while (Path.p[Path.length - 1] != '/') {nh_encoding_removeUTF32Tail(&Path, 1);}
        if (Path.length > 1) {nh_encoding_removeUTF32Tail(&Path, 1);}

        nh_tty_TreeListingNode *OldRoot_p = Listing_p->Root_p;

        Listing_p->Root_p = nh_tty_createTreeListingNode(NULL, Path, NULL);
        NH_TTY_CHECK_MEM(Listing_p->Root_p)

        OldRoot_p->Parent_p = Listing_p->Root_p;
        NH_TTY_CHECK(nh_tty_openNode(Listing_p->Root_p))

        NH_BOOL isChild = NH_FALSE;
        for (int i = 0; i < Listing_p->Root_p->Children.size; ++i) {
            nh_tty_TreeListingNode *Child_p = Listing_p->Root_p->Children.pp[i];
            if (nh_encoding_compareUTF32(Child_p->Path.p, OldRoot_p->Path.p)) {
                nh_encoding_freeUTF32(&Child_p->Path);
                nh_core_free(Listing_p->Root_p->Children.pp[i]);
                Listing_p->Root_p->Children.pp[i] = OldRoot_p;
                isChild = NH_TRUE;
                break;
            }
        }

        if (!isChild) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}
    }

    NH_TTY_CHECK(nh_tty_setCustomSuffixMessage(
        NULL, NH_TTY_MESSAGE_EDITOR_NEW_ROOT, Listing_p->Root_p->Path.p, Listing_p->Root_p->Path.length
    ))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static void nh_tty_updateTreeListingView(
    nh_tty_Editor *Editor_p, int key)
{
NH_TTY_BEGIN()

    switch (key) 
    {
        case 'w' :
        {
            nh_tty_TreeListingView *View_p = &Editor_p->View.TreeListing;
            if (View_p->current > 0) {View_p->current--;}
            else if (View_p->offset > 0) {View_p->offset--;} 
            break;
        }
        case 's' :
        {
            nh_tty_TreeListingView *View_p = &Editor_p->View.TreeListing;
            if (View_p->current < Editor_p->View.height - 1) {View_p->current++;}
            else {View_p->offset++;}
            break;
        }
    }

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT nh_tty_closeFileFromTreeListing(
    nh_tty_Program *Program_p, nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;

    NH_TTY_CHECK(nh_tty_setCustomSuffixMessage(
        NULL, NH_TTY_MESSAGE_EDITOR_FILE_CLOSED, File_p->Node_p->Path.p, File_p->Node_p->Path.length
    ))
 
    nh_tty_destroyFileViews(&Editor_p->View.FileEditor, File_p);
    NH_TTY_CHECK(nh_tty_closeFile(&Editor_p->FileEditor, File_p))
 
NH_TTY_END(NH_TTY_SUCCESS)
}

static void nh_tty_moveCursorVertically(
    nh_tty_Program *Program_p, int key) 
{
NH_TTY_BEGIN()

    nh_tty_FileEditor *FileEditor_p = &((nh_tty_Editor*)Program_p->handle_p)->FileEditor;
    nh_tty_TreeListing *Listing_p = &((nh_tty_Editor*)Program_p->handle_p)->TreeListing;

    nh_List Nodes = nh_core_initList(32);
    nh_tty_getNodeList(&Nodes, Listing_p->Root_p);

    switch (key) 
    {
        case 'w' :
        case CTRL_KEY('w') :

            if (Listing_p->current > 0) {
                Listing_p->current--;
                nh_tty_updateTreeListingView(Program_p->handle_p, 'w');
            }
            break;

        case 's' :
        case CTRL_KEY('s') :

            if (Listing_p->current < Nodes.size - 1) { 
                Listing_p->current++;
                nh_tty_updateTreeListingView(Program_p->handle_p, 's');
            }
            break;
    }

    nh_core_freeList(&Nodes, NH_FALSE);

    if (Listing_p->preview && FileEditor_p != NULL)
    {
        if (Listing_p->Preview_p != NULL) {
            nh_tty_closeFileFromTreeListing(Program_p, Listing_p->Preview_p->File_p);   
            Listing_p->Preview_p->File_p = NULL;
            Listing_p->Preview_p = NULL;
        }

        nh_tty_TreeListingNode *Current_p = nh_tty_getCurrentNode(Listing_p);

        if (Current_p->File_p == NULL && Current_p->Path.length > 0 && nh_tty_isRegularFile(Current_p)) {
            Current_p->File_p = nh_tty_openFile(Program_p, Current_p, NH_TRUE);
            Listing_p->Preview_p = Current_p; 
        }
    }

NH_TTY_SILENT_END()
}

NH_TTY_RESULT nh_tty_handleTreeListingInput(
    nh_tty_Program *Program_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    nh_tty_Editor *Editor_p = Program_p->handle_p;
    nh_tty_EditorView *View_p = &Editor_p->View;
    nh_tty_TreeListing *Listing_p = &Editor_p->TreeListing;
    nh_tty_TreeListingNode *Current_p = nh_tty_getCurrentNode(Listing_p);

    switch (c)
    {
        case 'q' :

            if (View_p->TreeListing.offset > 0) {View_p->TreeListing.offset--;}
            break;

        case 'e' :

            if (View_p->TreeListing.offset < View_p->TreeListing.maxOffset) {View_p->TreeListing.offset++;}
            break;

        case 'w' :
        case 's' :
        case CTRL_KEY('w') :
        case CTRL_KEY('s') :

            nh_tty_moveCursorVertically(Program_p, c);
            break;

        case 'a' :
        case CTRL_KEY('a') :

            if (Listing_p->current == 0) {
                View_p->TreeListing.offset = 0;
                nh_tty_setParentToRoot(Listing_p);
            } 
            else if (Current_p->Children.size > 0 && Current_p->open) {
                Current_p->open = NH_FALSE;
            }
            else if (Current_p->Children.size == 0 && Current_p->File_p != NULL && Current_p->Path.length > 0) {
                NH_TTY_CHECK(nh_tty_closeFileFromTreeListing(Program_p, Current_p->File_p))   
                Current_p->File_p = NULL;
                NH_TTY_CHECK(nh_tty_cycleThroughFiles(Program_p, 'f'))
            }
            else if (Current_p->Children.size == 0 && Current_p->File_p != NULL) {
                NH_TTY_CHECK(nh_tty_closeFileFromTreeListing(Program_p, Current_p->File_p))
                nh_core_removeFromList2(&Current_p->Parent_p->Children, NH_TRUE, Current_p);
                NH_TTY_CHECK(nh_tty_handleTreeListingInput(Program_p, 'w'))
            }
            else { // delete ?
                nh_encoding_UTF32String Question = nh_encoding_initUTF32(128);
                int deleteLength;
                NH_ENCODING_UTF32 *delete_p = nh_tty_getMessage(NH_TTY_MESSAGE_BINARY_QUERY_DELETE, &deleteLength);
                NH_ENCODING_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_encoding_appendUTF32(&Question, delete_p, deleteLength))
                NH_ENCODING_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_encoding_appendUTF32(&Question, Current_p->Path.p, Current_p->Path.length))
                NH_TTY_CHECK(nh_tty_setBinaryQueryMessage(NULL, Question.p, Question.length, NULL, nh_tty_delete))
                nh_encoding_freeUTF32(&Question);
            }
            break;

        case 'd' :
        case CTRL_KEY('d') :

            if (Listing_p->Preview_p != NULL) {
                Listing_p->Preview_p->File_p->readOnly = NH_FALSE;
                Listing_p->Preview_p = NULL;
            }
            else if (Current_p->Children.size > 0 && !Current_p->open) {
                Current_p->open = NH_TRUE;
            }
            else if (Current_p->Children.size == 0 && !nh_tty_isRegularFile(Current_p)) {
                NH_TTY_CHECK(nh_tty_openNode(Current_p))
            }
            else if (Current_p->Children.size == 0 && nh_tty_isRegularFile(Current_p)) {
                if (Current_p->File_p == NULL) {
                    Current_p->File_p = nh_tty_openFile(Program_p, Current_p, NH_FALSE);
                }
                else {
                    NH_TTY_CHECK(nh_tty_writeFile(Current_p->File_p))
                    Current_p->unsaved = NH_FALSE;
                }
            } else {
                nh_tty_setCurrentToRoot(Listing_p);
                Listing_p->current = 0;
                View_p->TreeListing.offset = 0;
                View_p->TreeListing.current = 0;
            }
            break;
    }
 
    Listing_p->dirty = NH_TRUE;

NH_TTY_END(NH_TTY_SUCCESS)
}

// RENDER ==========================================================================================

static nh_tty_Glyph nh_tty_glyph(
    NH_ENCODING_UTF32 codepoint, NH_BOOL reverse)
{
NH_TTY_BEGIN()

    nh_tty_Glyph Glyph;
    memset(&Glyph, 0, sizeof(nh_tty_Glyph));
    Glyph.Attributes.reverse = reverse;
    Glyph.codepoint = codepoint;

NH_TTY_END(Glyph)
}

static NH_TTY_RESULT nh_tty_renderTreeListingNode(
    nh_tty_TreeListingNode *Node_p, NH_BOOL isCurrent, nh_tty_Glyph *Row_p, int *length_p, 
    NH_BOOL first, nh_tty_TreeListingView *View_p)
{
NH_TTY_BEGIN()

    nh_tty_TreeListingNode *Parent_p = Node_p->Parent_p;

    int prefixLength = 0, suffixLength = 0;

    nh_tty_Glyph Prefix_p[64];  // Holds stuff that comes before the file name.
    nh_tty_Glyph Suffix_p[512]; // Holds the file name.

    // Render prefix.
    while (Parent_p != NULL && Parent_p->open) {
        prefixLength += 2; 
        Parent_p = Parent_p->Parent_p;
    }

    for (int i = 0; i < prefixLength; i++) {
        Prefix_p[i] = nh_tty_glyph(' ', NH_FALSE);
        if (i == prefixLength - 2) {Prefix_p[i] = nh_tty_glyph('-', NH_FALSE);}
        if (i == prefixLength - 1 && isCurrent) {Prefix_p[i] = nh_tty_glyph('>', NH_FALSE);}
    }

    if (isCurrent && !first) {
        Prefix_p[prefixLength++] = nh_tty_glyph(' ', NH_FALSE);
    }

    // Render suffix.
    if (Node_p->Path.length > 0)
    {
        int pathOffset = 0;

        for (int i = 0; i < Node_p->Path.length; ++i) {
            if (Node_p->Path.p[i] == '/') {pathOffset = i;}
        }

        pathOffset++;

        for (int i = 0, j = pathOffset; j < Node_p->Path.length; ++i, ++j) {
            Suffix_p[i] = nh_tty_glyph(Node_p->Path.p[j], Node_p->File_p != NULL);
        }

        suffixLength += Node_p->Path.length - pathOffset;

        if (!nh_tty_isRegularFile(Node_p)) {
            Suffix_p[suffixLength++] = nh_tty_glyph('/', NH_FALSE); 
        }
    }

    // Render based on view with overflow handling, or just render everything
    if (View_p) {
        for (int i = 0; i < prefixLength && i < View_p->width; ++i) {
            Row_p[i] = Prefix_p[i];
        }
        if (prefixLength + suffixLength > View_p->width) {
            nh_SystemTime Now = nh_core_getSystemTime();
            if (nh_core_getSystemTimeDiffInSeconds(Node_p->Overflow.LastShift, Now) >= 0.2f) {
                Node_p->Overflow.LastShift = Now;
                Node_p->Overflow.offset++;
                if ((Node_p->Overflow.offset % (suffixLength+1)) == 0) {Node_p->Overflow.offset = 0;}
            }

            int count = 0;
            for (int i = prefixLength, j = Node_p->Overflow.offset; i < View_p->width && j < suffixLength; ++i, ++j) {
                count++;
                Row_p[i] = Suffix_p[j];
            }

            for (int i = prefixLength + count + 1, j = 0; i < View_p->width && j < suffixLength; ++i, ++j) {
                Row_p[i] = Suffix_p[j];
            }
        }
        else {
            for (int i = prefixLength, j = 0; j < suffixLength && i < View_p->width; ++i, ++j) {
                Row_p[i] = Suffix_p[j];
            }
        }
    }
    else {
        for (int i = 0; i < prefixLength; ++i) {
            Row_p[i] = Prefix_p[i];
        }
        for (int i = 0; i < suffixLength; ++i) {
            Row_p[i+prefixLength] = Suffix_p[i];
        }
    }

    if (length_p) {
        *length_p = prefixLength + suffixLength;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_calculateTreeListingWidth(
    nh_tty_TreeListing *Listing_p, nh_tty_TreeListingView *View_p, int editorWidth)
{
NH_TTY_BEGIN()

    nh_List Nodes = nh_core_initList(64);
    NH_TTY_CHECK(nh_tty_getNodeList(&Nodes, Listing_p->Root_p))
    nh_tty_TreeListingNode *Current_p = nh_tty_getCurrentNode(Listing_p);

    View_p->width = 0;
    nh_tty_Glyph Row_p[1024] = {'\0'};

    for (int i = 0; i < Nodes.size; ++i) {
        int length = 0;
        NH_TTY_CHECK(nh_tty_renderTreeListingNode(Nodes.pp[i], Nodes.pp[i] == Current_p, Row_p, &length, i == 0, NULL))
        if (length > View_p->width) {View_p->width = length;}
    }

    // Restrict tree-listing width.
    if (View_p->width > editorWidth/3) {
        View_p->maxOffset = View_p->width - editorWidth/3;
        View_p->width = editorWidth/3;
    } else {
        View_p->maxOffset = 0;
    }

    nh_core_freeList(&Nodes, NH_FALSE);

NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_renderTreeListing(
    nh_tty_TreeListing *Listing_p, nh_tty_TreeListingView *View_p, int editorWidth)
{
NH_TTY_BEGIN()

    nh_List Nodes = nh_core_initList(32);
    NH_TTY_CHECK(nh_tty_getNodeList(&Nodes, Listing_p->Root_p))
    nh_tty_TreeListingNode *Current_p = nh_tty_getCurrentNode(Listing_p);

    for (int i = 0; i < Listing_p->RenderLines.length; ++i) {
        nh_core_freeArray(&((nh_Array*)Listing_p->RenderLines.p)[i]);
    }
    nh_core_freeArray(&Listing_p->RenderLines);
    Listing_p->RenderLines = nh_core_initArray(sizeof(nh_Array), Nodes.size);

    NH_TTY_CHECK(nh_tty_calculateTreeListingWidth(Listing_p, View_p, editorWidth))

    for (int row = 0; row < Nodes.size; ++row) 
    {
        nh_Array *Line_p = nh_core_incrementArray(&Listing_p->RenderLines);
        *Line_p = nh_core_initArray(sizeof(nh_tty_Glyph), 32);
        nh_tty_TreeListingNode *Node_p = nh_core_getFromList(&Nodes, row);
        NH_TTY_CHECK_NULL(Node_p)

        nh_tty_Glyph Glyphs_p[1024] = {'\0'};
        for (int col = 0; col < View_p->width; ++col) {
            Glyphs_p[col].codepoint = ' ';
        }

        NH_TTY_CHECK(nh_tty_renderTreeListingNode(Node_p, Node_p == Current_p, Glyphs_p, NULL, row == 0, View_p))

        for (int col = 0; col < View_p->width; ++col) {
            *((nh_tty_Glyph*)nh_core_incrementArray(Line_p)) = Glyphs_p[col];
        }
    }  

    nh_core_freeList(&Nodes, NH_FALSE);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_treeListingNeedsRefresh(
    nh_tty_TreeListing *Listing_p)
{
NH_TTY_BEGIN()

    nh_List Nodes = nh_core_initList(32);
    NH_TTY_CHECK(nh_tty_getNodeList(&Nodes, Listing_p->Root_p))
    nh_core_freeList(&Nodes, NH_FALSE);

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT nh_tty_drawTreeListingRow(
    nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width, int height, int row)
{
NH_TTY_BEGIN()

    nh_tty_TreeListing *Listing_p = &((nh_tty_Editor*)Program_p->handle_p)->TreeListing;
    nh_tty_TreeListingView *View_p = &((nh_tty_Editor*)Program_p->handle_p)->View.TreeListing;

    if (Listing_p->dirty) {
        if (strlen(Listing_p->wrkDir_p) == 0) {
            getcwd(Listing_p->wrkDir_p, 2048);
            Listing_p->Root_p->Path = nh_encoding_decodeUTF8(Listing_p->wrkDir_p, strlen(Listing_p->wrkDir_p), NULL);
            nh_tty_openNode(Listing_p->Root_p);
        }
        NH_TTY_CHECK(nh_tty_renderTreeListing(Listing_p, View_p, width))
        Listing_p->dirty = NH_FALSE;
    }

    row += View_p->offset; 

    if (row < Listing_p->RenderLines.length) {
        nh_Array *RenderLine_p = &((nh_Array*)Listing_p->RenderLines.p)[row];
        for (int i = 0; i < RenderLine_p->length; ++i) {
            Glyphs_p[i] = ((nh_tty_Glyph*)RenderLine_p->p)[i];
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// CURSOR ==========================================================================================

NH_TTY_RESULT nh_tty_setTreeListingCursor(
    nh_tty_Program *Program_p, nh_tty_File *File_p)
{
NH_TTY_BEGIN()

    nh_List Nodes = nh_core_initList(16);
    nh_tty_TreeListing *Listing_p = &((nh_tty_Editor*)Program_p->handle_p)->TreeListing;

    nh_tty_getNodeList(&Nodes, Listing_p->Root_p);

    for (int i = 0; i < Nodes.size; ++i) {
        if (((nh_tty_TreeListingNode*)Nodes.pp[i])->File_p == File_p) {
            if (Listing_p->current < i) {
                int diff = i - Listing_p->current;
                while (diff-- > 0) {
                    nh_tty_moveCursorVertically(Program_p, CTRL_KEY('s'));
                }
            }
            else if (Listing_p->current > i) {
                int diff = Listing_p->current - i;
                while (diff-- > 0) {
                    nh_tty_moveCursorVertically(Program_p, CTRL_KEY('w'));
                }
            }
            break;
        }
    }

    nh_core_freeList(&Nodes, NH_FALSE);
    Listing_p->dirty = NH_TRUE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INIT/DESTROY ====================================================================================

nh_tty_TreeListing nh_tty_initTreeListing()
{
NH_TTY_BEGIN()

    nh_tty_TreeListing Listing;

    Listing.Root_p      = nh_tty_createTreeListingNode(NULL, nh_encoding_initUTF32(0), NULL);
    Listing.current     = 0;
    Listing.RenderLines = nh_core_initArray(sizeof(nh_String), 255);
    Listing.dirty       = NH_TRUE;
    Listing.preview     = NH_FALSE;
    Listing.Preview_p   = NULL;

    memset(Listing.wrkDir_p, 0, 2048);

NH_TTY_END(Listing)
}

NH_TTY_RESULT nh_tty_freeTreeListing(
    nh_tty_TreeListing *TreeListing_p)
{
NH_TTY_BEGIN()

    nh_tty_freeTreeListingNode(TreeListing_p->Root_p);
    nh_core_free(TreeListing_p->Root_p);
    for (int i = 0; i < TreeListing_p->RenderLines.length; ++i) {
        nh_core_freeString(((nh_String*)TreeListing_p->RenderLines.p)+i);
    }
    nh_core_freeArray(&TreeListing_p->RenderLines);
 
NH_TTY_END(NH_TTY_SUCCESS)
}

