// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tiling.h"
#include "TTY.h"
#include "Macro.h"
#include "Micro.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// INIT/FREE =======================================================================================

nh_tty_Tile *nh_tty_createTile(
    void *p, NH_TTY_TILE_TYPE_E type, nh_tty_Tile *Parent_p, int index)
{
NH_TTY_BEGIN()

    nh_tty_Tile *Tile_p = nh_core_allocate(sizeof(nh_tty_Tile));
    NH_TTY_CHECK_MEM_2(NULL, Tile_p)

    Tile_p->type = type;
    Tile_p->p = p;
    Tile_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
    Tile_p->rightSeparator = NH_FALSE;
    Tile_p->refresh  = NH_FALSE;
    Tile_p->Children = nh_core_initLinkedList();
    Tile_p->Parent_p = Parent_p;
    Tile_p->Prev_p   = NULL;
    Tile_p->close    = NH_FALSE;

    if (Parent_p != NULL) {
        nh_core_insertIntoLinkedList(&Parent_p->Children, Tile_p, index);
    }

NH_TTY_END(Tile_p)
}

// Helper function for destroying a tile without side effects.
static void nh_tty_destroyTile(
    nh_tty_Tile *Tile_p)
{
NH_TTY_BEGIN()

    if (Tile_p->p) {
        switch (Tile_p->type) {
            case NH_TTY_TILE_TYPE_MACRO : nh_tty_destroyMacroTile(Tile_p->p); break;
            case NH_TTY_TILE_TYPE_MICRO : nh_tty_destroyMicroTile(Tile_p->p); break;
        }
    }

    nh_core_destroyLinkedList(&Tile_p->Children, NH_FALSE); 
    nh_core_free(Tile_p);

NH_TTY_SILENT_END()
}

static nh_tty_Tile *nh_tty_getNextFocusTile(
    nh_tty_Tile *Tile_p)
{
NH_TTY_BEGIN()

    while (Tile_p->Children.count > 0) {
        Tile_p = nh_core_getFromLinkedList(&Tile_p->Children, 0);
    }

NH_TTY_END(Tile_p)
}

NH_TTY_RESULT nh_tty_closeTile(
    nh_tty_Tile *Tile_p, void *p)
{
NH_TTY_BEGIN()

    if (!Tile_p) {NH_TTY_END(NH_TTY_SUCCESS)}

    if (!Tile_p->Parent_p) {

        // Handle root tile.
        if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) {
            ((nh_tty_MacroWindow*)p)->Tile_p = NULL;
            ((nh_tty_MacroWindow*)p)->LastFocus_p = NULL;
            ((nh_tty_MacroWindow*)p)->RootTile_p = NULL;
        } else if (Tile_p->type == NH_TTY_TILE_TYPE_MICRO) {
            ((nh_tty_MicroTab*)p)->Tile_p = NULL;
            ((nh_tty_MicroTab*)p)->LastFocus_p = NULL;
            ((nh_tty_MicroTab*)p)->RootTile_p = NULL;
        }
        nh_tty_destroyTile(Tile_p);

    } else if (Tile_p->Parent_p->Children.count == 2) {
        // Get neighbor tile.
        int index = nh_core_getLinkedListIndex(&Tile_p->Parent_p->Children, Tile_p);
        nh_tty_Tile *Other_p = nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, index == 0 ? 1 : 0);

        if (Tile_p->Parent_p->Parent_p) {
            for (int i = 0; i < Tile_p->Parent_p->Parent_p->Children.count; ++i) {
                if (nh_core_getFromLinkedList(&Tile_p->Parent_p->Parent_p->Children, i) == Tile_p->Parent_p) {
                    nh_core_setInLinkedList(&Tile_p->Parent_p->Parent_p->Children, i, Other_p);
                    Other_p->Parent_p = Tile_p->Parent_p->Parent_p;
                    Other_p->Prev_p = NULL;
                    break;
                }
            }

            nh_tty_Tile *Focus_p = nh_tty_getNextFocusTile(Other_p);

            // Set other tile as focused tile.
            if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) {
                ((nh_tty_MacroWindow*)p)->Tile_p = Focus_p;
                ((nh_tty_MacroWindow*)p)->LastFocus_p = Focus_p;
            } else if (Tile_p->type == NH_TTY_TILE_TYPE_MICRO) {
                ((nh_tty_MicroTab*)p)->Tile_p = Focus_p;
                ((nh_tty_MicroTab*)p)->LastFocus_p = Focus_p;
            }

            // Just destroy Tile_p and parent tile..
            nh_tty_destroyTile(Tile_p->Parent_p);
            nh_tty_destroyTile(Tile_p);
        } else {
            nh_tty_Tile *Focus_p = nh_tty_getNextFocusTile(Other_p);

            // Set other tile as new root tile and set focus.
            if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) {
		((nh_tty_MacroWindow*)p)->Tile_p = Focus_p;
                ((nh_tty_MacroWindow*)p)->LastFocus_p = Focus_p;
                ((nh_tty_MacroWindow*)p)->RootTile_p = Other_p;
            } else if (Tile_p->type == NH_TTY_TILE_TYPE_MICRO) {
                ((nh_tty_MicroTab*)p)->Tile_p = Focus_p;
                ((nh_tty_MicroTab*)p)->LastFocus_p = Focus_p;
                ((nh_tty_MicroTab*)p)->RootTile_p = Other_p;
            }

            // Destroy tiles and update Other_p.
            nh_tty_destroyTile(Tile_p->Parent_p);
            nh_tty_destroyTile(Tile_p);
            Other_p->Parent_p = NULL;
            Other_p->Prev_p = NULL;
        }
    } else {
        // Get neighbor tile index.
        int index = nh_core_getLinkedListIndex(&Tile_p->Parent_p->Children, Tile_p);
        index = index == 0 ? 1 : index - 1;

        // Get next focused tile.
        nh_tty_Tile *Next_p = nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, index);
        nh_tty_Tile *Focus_p = nh_tty_getNextFocusTile(Next_p);

        // Switch focus to neighbor tile.
        if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) {
            ((nh_tty_MacroWindow*)p)->Tile_p = Focus_p;
            ((nh_tty_MacroWindow*)p)->LastFocus_p = Focus_p;
        } else if (Tile_p->type == NH_TTY_TILE_TYPE_MICRO) {
            ((nh_tty_MicroTab*)p)->Tile_p = Focus_p;
            ((nh_tty_MicroTab*)p)->LastFocus_p = Focus_p;
        }

        // Remove and destroy Tile_p. 
        nh_core_removeFromLinkedList2(&Tile_p->Parent_p->Children, Tile_p, NH_FALSE);
        nh_tty_destroyTile(Tile_p);
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// GET =============================================================================================

static int nh_tty_getTileNumber(
    nh_tty_Tile *Current_p, nh_tty_Tile *Stop_p, int *nr_p)
{
NH_TTY_BEGIN()

    if (Current_p == Stop_p) {NH_TTY_END(*nr_p)}

    for (int i = 0; i < Current_p->Children.count; ++i) {
        nh_tty_Tile *Child_p = nh_core_getFromLinkedList(&Current_p->Children, i);
        if (Child_p->Children.count <= 0) {*nr_p += 1;}
        int result = nh_tty_getTileNumber(Child_p, Stop_p, nr_p);
        if (result > -1) {NH_TTY_END(result)}
    }

NH_TTY_END(-1)
}

// d -> right -> horizontal orientation -> 
// a -> left ->
// w -> up ->
static nh_tty_Tile *nh_tty_getTileFromNumber(
    nh_tty_Tile *Current_p, int stop, int *nr_p)
{
NH_TTY_BEGIN()

    if (*nr_p == stop) {NH_TTY_END(Current_p)}

    for (int i = 0; i < Current_p->Children.count; ++i) {
        nh_tty_Tile *Child_p = nh_core_getFromLinkedList(&Current_p->Children, i);
        if (Child_p->Children.count <= 0) {*nr_p += 1;}
        nh_tty_Tile *Result_p = nh_tty_getTileFromNumber(Child_p, stop, nr_p);
        if (Result_p != NULL) {NH_TTY_END(Result_p)}
    }

NH_TTY_END(NULL)
}

nh_List nh_tty_getTiles(
    nh_tty_Tile *Root_p)
{
NH_TTY_BEGIN()

    nh_List List = nh_core_initList(4);

    for (int i = 0; Root_p != NULL;++i) {
        int start = 0;
        nh_tty_Tile *Tile_p = nh_tty_getTileFromNumber(Root_p, i, &start);
        if (Tile_p == NULL) {break;}
        nh_core_appendToList(&List, Tile_p);
    }

NH_TTY_END(List)
}

static int nh_tty_getTileIndex(
    nh_tty_Tile *Tile_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < Tile_p->Parent_p->Children.count; ++i) {
        nh_tty_Tile *Child_p = nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, i);
        if (Child_p == Tile_p) {NH_TTY_END(i)}
    }

NH_TTY_END(-1)
}

// SWITCH ==========================================================================================

static NH_TTY_RESULT nh_tty_leaveTilingAndFocusTile(
    nh_tty_MacroWindow *Window_p, nh_tty_Tile *Focus_p)
{
NH_TTY_BEGIN()

    if (Focus_p->type == NH_TTY_TILE_TYPE_MICRO) {
        NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p = Focus_p;
    } else {
        Window_p->Tile_p = Focus_p;
    }

    Window_p->Tiling.stage = NH_TTY_TILING_STAGE_DONE;
    Window_p->Tiling.mode  = NH_TTY_TILING_MODE_MICRO;

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();
    TTY_p->InsertTile_p = NULL;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static nh_tty_Tile *nh_tty_getNextDirectionalTile(
    nh_tty_Tile *Tile_p, int direction)
{
NH_TTY_BEGIN()

    if (!Tile_p->Parent_p) {
        NH_TTY_END(NULL)
    }

    switch (direction) {
        case 1 : // right
        case 3 : // left
            if (Tile_p->Parent_p->orientation != NH_TTY_TILE_ORIENTATION_VERTICAL) {
                NH_TTY_END(NULL)
            }
            for (int i = 0; i < Tile_p->Parent_p->Children.count; ++i) {
                nh_tty_Tile *Child_p = nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, i);
                if (Child_p == Tile_p) {
                    if (direction == 3 && i > 0) {
                        NH_TTY_END(nh_core_getFromLinkedList(&Tile_p->Parent_p->Children,i-1))
                    }
                    if (direction == 1 && (i+1 < Tile_p->Parent_p->Children.count)) {
                        NH_TTY_END(nh_core_getFromLinkedList(&Tile_p->Parent_p->Children,i+1))
                    }
                }
            }
            break;
        case 0 : // top
        case 2 : // bottom
            if (Tile_p->Parent_p->orientation != NH_TTY_TILE_ORIENTATION_HORIZONTAL) {
                NH_TTY_END(NULL)
            }
            for (int i = 0; i < Tile_p->Parent_p->Children.count; ++i) {
                nh_tty_Tile *Child_p = nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, i);
                if (Child_p == Tile_p) {
                    if (direction == 0 && i > 0) {
                        NH_TTY_END(nh_core_getFromLinkedList(&Tile_p->Parent_p->Children,i-1))
                    }
                    if (direction == 2 && (i+1 < Tile_p->Parent_p->Children.count)) {
                        NH_TTY_END(nh_core_getFromLinkedList(&Tile_p->Parent_p->Children,i+1))
                    }
                }
            }
            break;
    }

NH_TTY_END(NULL)
}

nh_tty_Tile *nh_tty_switchTile(
    nh_tty_MacroWindow *Window_p, nh_tty_Tile *MacroTile_p, int direction)
{
NH_TTY_BEGIN()

    nh_tty_Tile *MicroTile_p = NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(MacroTile_p))->Tile_p;

    nh_tty_Tile *Result_p = NULL;
    while (!Result_p && MicroTile_p) {
        Result_p = nh_tty_getNextDirectionalTile(MicroTile_p, direction);
        MicroTile_p = MicroTile_p->Parent_p;
    }

    while (!Result_p && MacroTile_p) {
        Result_p = nh_tty_getNextDirectionalTile(MacroTile_p, direction);
        MacroTile_p = MacroTile_p->Parent_p;
    }

    if (Result_p) {
        Result_p = nh_tty_getNextFocusTile(Result_p);
        nh_tty_leaveTilingAndFocusTile(Window_p, Result_p);
    }

NH_TTY_END(Result_p)
}

// UPDATE ==========================================================================================

static NH_TTY_RESULT nh_tty_computeTileSizeRecursively(
    nh_tty_Tile *Tile_p, int viewRows, int viewCols)
{
NH_TTY_BEGIN()

    Tile_p->rightSeparator = NH_FALSE;

    if (Tile_p->Parent_p == NULL) {
        Tile_p->rowPosition = 0;
        Tile_p->colPosition = 0;
        Tile_p->rowSize = viewRows;
        Tile_p->colSize = viewCols;
    }
    else {
        Tile_p->rowPosition = Tile_p->rowPosition;
        Tile_p->colPosition = Tile_p->colPosition;
        Tile_p->rowSize = Tile_p->rowSize;
        Tile_p->colSize = Tile_p->colSize;
    }

    // Compute vertical borders.
    Tile_p->rightSeparator = Tile_p->colPosition + Tile_p->colSize < viewCols;

    for (int i = 0; i < Tile_p->Children.count; ++i) 
    {
        nh_tty_Tile *Child_p = nh_core_getFromLinkedList(&Tile_p->Children, i);

        switch (Tile_p->orientation)
        {
            case NH_TTY_TILE_ORIENTATION_VERTICAL :
                Child_p->colSize     = Tile_p->colSize / Tile_p->Children.count;
                Child_p->colPosition = Tile_p->colPosition + Child_p->colSize * i;
                Child_p->rowSize     = Tile_p->rowSize;
                Child_p->rowPosition = Tile_p->rowPosition;
                if (Tile_p->colSize % Tile_p->Children.count > 0 && i == Tile_p->Children.count - 1) {
                    Child_p->colSize += Tile_p->colSize % Tile_p->Children.count;
                }
                break;
            case NH_TTY_TILE_ORIENTATION_HORIZONTAL :
                Child_p->colSize     = Tile_p->colSize;
                Child_p->colPosition = Tile_p->colPosition;
                Child_p->rowSize     = Tile_p->rowSize / Tile_p->Children.count;
                Child_p->rowPosition = Tile_p->rowPosition + Child_p->rowSize * i;
                if (Tile_p->rowSize % Tile_p->Children.count > 0 && i == Tile_p->Children.count - 1) {
                    Child_p->rowSize += Tile_p->rowSize % Tile_p->Children.count;
                }
                break;
        }

        NH_TTY_CHECK(nh_tty_computeTileSizeRecursively(Child_p, viewRows, viewCols))
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_updateTiling(
    nh_tty_Tile *RootTile_p, int viewRows, int viewCols)
{
NH_TTY_BEGIN()

    NH_TTY_CHECK(nh_tty_computeTileSizeRecursively(RootTile_p, viewRows, viewCols))

NH_TTY_END(NH_TTY_SUCCESS)
}

// INSERT ==========================================================================================

static void nh_tty_moveInsertTile(
    nh_tty_Tile *Tile_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    int index = nh_tty_getTileIndex(Tile_p);
    if (index == -1) {NH_TTY_SILENT_END()}

    switch (c) 
    {
        case NH_TTY_INSERT_TILE_TOP_KEY :
            Tile_p->Parent_p->orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            if (index > 0) {
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index, nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, index - 1));
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index - 1, Tile_p);
            }
            break;

        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
            Tile_p->Parent_p->orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            if (index < Tile_p->Parent_p->Children.count - 1) {
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index, nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, index + 1));
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index + 1, Tile_p);
            }
            break;

        case NH_TTY_INSERT_TILE_LEFT_KEY :
            Tile_p->Parent_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            if (index > 0) {
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index, nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, index - 1));
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index - 1, Tile_p);
            }
            break;

        case NH_TTY_INSERT_TILE_RIGHT_KEY :
            Tile_p->Parent_p->orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            if (index < Tile_p->Parent_p->Children.count - 1) {
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index, nh_core_getFromLinkedList(&Tile_p->Parent_p->Children, index + 1));
                nh_core_setInLinkedList(&Tile_p->Parent_p->Children, index + 1, Tile_p);
            }
            break;
    }

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT nh_tty_insertEmptyTile(
    nh_tty_Tile *Parent_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    int orientation = 0;

    switch (c) 
    {
        case NH_TTY_INSERT_TILE_TOP_KEY    :
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
            orientation = NH_TTY_TILE_ORIENTATION_HORIZONTAL;
            break;
        case NH_TTY_INSERT_TILE_LEFT_KEY  :
        case NH_TTY_INSERT_TILE_RIGHT_KEY :
            orientation = NH_TTY_TILE_ORIENTATION_VERTICAL;
            break;
    }

    if (Parent_p->Children.count > 0 && Parent_p->orientation != orientation) {
        // Changing orientation is not allowed because it's confusing.
        NH_TTY_END(NH_TTY_ERROR_BAD_STATE)
    }

    // Configure orientation.
    Parent_p->orientation = orientation;

    // Insert tile. The inserted tile is created without data.
    switch (c)
    {
        case NH_TTY_INSERT_TILE_TOP_KEY  :
        case NH_TTY_INSERT_TILE_LEFT_KEY :
            if (Parent_p->type == NH_TTY_TILE_TYPE_MACRO) {
                if (Parent_p->Children.count == 0) {
                    nh_List *List_pp[9] = {};
                    for (int i = 0; i < 9; ++i) {
                        List_pp[i] = NH_TTY_MACRO_TAB_2(Parent_p, i)->MicroWindow.Tabs_p;
                    }
                    TTY_p->InsertTile_p     = nh_tty_createMacroTile(Parent_p, NULL, 0);
                    TTY_p->Window_p->Tile_p = nh_tty_createMacroTile(Parent_p, List_pp, 1);
                    // Remove data references from parent tile.
                    nh_core_freeList(&NH_TTY_MACRO_TILE(Parent_p)->MacroTabs, NH_TRUE);
                    NH_TTY_MACRO_TILE(Parent_p)->current = -1;
                } else {
                    TTY_p->InsertTile_p = nh_tty_createMacroTile(Parent_p, NULL, 0);
                }
            } else {
                if (Parent_p->Children.count == 0) {
                    TTY_p->InsertTile_p = nh_tty_createMicroTile(Parent_p, NULL, 0);
                    nh_tty_createMicroTile(Parent_p, NH_TTY_MICRO_TILE(Parent_p)->Program_p, 1);
                    // Remove data references from parent tile.
                    NH_TTY_MICRO_TILE(Parent_p)->Program_p = NULL;
                } else {
                    TTY_p->InsertTile_p = nh_tty_createMicroTile(Parent_p, NULL, 0);
                }
            }
            break;
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
        case NH_TTY_INSERT_TILE_RIGHT_KEY  :
            if (Parent_p->type == NH_TTY_TILE_TYPE_MACRO) {
                if (Parent_p->Children.count == 0) {
                    nh_List *List_pp[9] = {};
                    for (int i = 0; i < 9; ++i) {
                        List_pp[i] = NH_TTY_MACRO_TAB_2(Parent_p, i)->MicroWindow.Tabs_p;
                    }
                    TTY_p->Window_p->Tile_p = nh_tty_createMacroTile(Parent_p, List_pp, 0);
                    TTY_p->InsertTile_p     = nh_tty_createMacroTile(Parent_p, NULL, 1);
                    // Remove data references from parent tile.
                    nh_core_freeList(&NH_TTY_MACRO_TILE(Parent_p)->MacroTabs, NH_TRUE);
                    NH_TTY_MACRO_TILE(Parent_p)->current = -1;
                } else {
                    TTY_p->InsertTile_p = nh_tty_createMacroTile(Parent_p, NULL, Parent_p->Children.count);
                }
            } else {
                if (Parent_p->Children.count == 0) {
                    nh_tty_createMicroTile(Parent_p, NH_TTY_MICRO_TILE(Parent_p)->Program_p, 0);
                    TTY_p->InsertTile_p = nh_tty_createMicroTile(Parent_p, NULL, 1);
                    // Remove data references from parent tile.
                    NH_TTY_MICRO_TILE(Parent_p)->Program_p = NULL;
                } else {
                    TTY_p->InsertTile_p = nh_tty_createMicroTile(Parent_p, NULL, Parent_p->Children.count);
                }
            }
            break;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_addTile(
    nh_tty_Tile *Tile_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    if (Tile_p->Parent_p == NULL || Tile_p->Parent_p->Children.count == 0) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}

NH_TTY_END(nh_tty_insertEmptyTile(Tile_p->Parent_p, c))
}

static NH_TTY_RESULT nh_tty_splitTile(
    nh_tty_Tile *Tile_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    if (Tile_p->Children.count != 0) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}

NH_TTY_END(nh_tty_insertEmptyTile(Tile_p, c))
}

// RESET ===========================================================================================

NH_TTY_RESULT nh_tty_resetTiling(
    nh_tty_MacroWindow *Window_p)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    if (Window_p->Tiling.mode == NH_TTY_TILING_MODE_MICRO) {
        switch (Window_p->Tiling.stage)
        {
            case NH_TTY_TILING_STAGE_OVERVIEW :
                NH_TTY_CHECK(nh_tty_leaveTilingAndFocusTile(Window_p, NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->LastFocus_p))
                break;

            case NH_TTY_TILING_STAGE_INSERT :
                nh_tty_closeTile(TTY_p->InsertTile_p, NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p)));
                NH_TTY_CHECK(nh_tty_leaveTilingAndFocusTile(Window_p, NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->LastFocus_p))
                break;
        }
    } else {
        switch (Window_p->Tiling.stage)
        {
            case NH_TTY_TILING_STAGE_OVERVIEW :
                NH_TTY_CHECK(nh_tty_leaveTilingAndFocusTile(Window_p, Window_p->LastFocus_p))
                break;

            case NH_TTY_TILING_STAGE_INSERT :
                nh_tty_closeTile(TTY_p->InsertTile_p, Window_p);
                NH_TTY_CHECK(nh_tty_leaveTilingAndFocusTile(Window_p, Window_p->LastFocus_p))
                break;
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// MESSAGES ========================================================================================

static NH_TTY_RESULT nh_tty_updateTilingMessages(
    nh_tty_MacroWindow *Window_p)
{
NH_TTY_BEGIN()

    if (Window_p->Tiling.mode == NH_TTY_TILING_MODE_MACRO) {
        if (Window_p->Tile_p->Parent_p == NULL) {
            NH_TTY_CHECK(nh_tty_setDefaultMessage(&NH_TTY_MACRO_TAB(Window_p->Tile_p)->TopBar, NH_TTY_MESSAGE_TILING_WASD))
        } else {
            NH_TTY_CHECK(nh_tty_setDefaultMessage(&NH_TTY_MACRO_TAB(Window_p->Tile_p)->TopBar, NH_TTY_MESSAGE_TILING_WASDF))
        }
    } else if (Window_p->Tiling.mode == NH_TTY_TILING_MODE_MICRO) {
        if (NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p->Parent_p == NULL) {
            NH_TTY_CHECK(nh_tty_setDefaultMessage(&NH_TTY_MACRO_TAB(Window_p->Tile_p)->TopBar, NH_TTY_MESSAGE_TILING_WASD))
        } else {
            NH_TTY_CHECK(nh_tty_setDefaultMessage(&NH_TTY_MACRO_TAB(Window_p->Tile_p)->TopBar, NH_TTY_MESSAGE_TILING_WASDF))
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static NH_TTY_RESULT nh_tty_handlePotentialMacroTileInsertion(
    nh_tty_MacroWindow *Window_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    if (c == 13) 
    {
        // Insert data into new macro tile.
        for (int i = 0; i < 9; ++i) {
            NH_TTY_MACRO_TAB_2(TTY_p->InsertTile_p, i)->MicroWindow.Tabs_p = nh_tty_createMicroTabs(TTY_p);
            NH_TTY_MACRO_TAB_2(TTY_p->InsertTile_p, i)->MicroWindow.current = 0; 
        }

        NH_TTY_DIAGNOSTIC_END(nh_tty_leaveTilingAndFocusTile(Window_p, TTY_p->InsertTile_p))
    }

    switch (c) 
    {
        case NH_TTY_INSERT_TILE_TOP_KEY    :
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
        case NH_TTY_INSERT_TILE_LEFT_KEY   :
        case NH_TTY_INSERT_TILE_RIGHT_KEY  :

            nh_tty_moveInsertTile(TTY_p->InsertTile_p, c);
            break;

        default :
            NH_TTY_CHECK(nh_tty_resetTiling(Window_p))
    }

NH_TTY_DIAGNOSTIC_END(nh_tty_updateTilingMessages(Window_p))
}

static NH_TTY_RESULT nh_tty_handlePotentialMicroTileInsertion(
    nh_tty_MacroWindow *Window_p, NH_ENCODING_UTF32 c)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    if (c == 13)
    {
        // Insert data into new micro tile.
        NH_TTY_MICRO_TILE(TTY_p->InsertTile_p)->Program_p = nh_tty_createProgramInstance(
            NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Prototype_p, NH_FALSE);

        NH_TTY_DIAGNOSTIC_END(nh_tty_leaveTilingAndFocusTile(Window_p, TTY_p->InsertTile_p))
    }

    switch (c)
    {
        case NH_TTY_INSERT_TILE_TOP_KEY    :
        case NH_TTY_INSERT_TILE_BOTTOM_KEY :
        case NH_TTY_INSERT_TILE_LEFT_KEY   :
        case NH_TTY_INSERT_TILE_RIGHT_KEY  :

            nh_tty_moveInsertTile(TTY_p->InsertTile_p, c);
            break;

        default :
            NH_TTY_CHECK(nh_tty_resetTiling(Window_p))
    }

NH_TTY_DIAGNOSTIC_END(nh_tty_updateTilingMessages(Window_p))
}

static NH_TTY_RESULT nh_tty_handleMacroTilingInput(
    nh_tty_MacroWindow *Window_p, nh_wsi_KeyboardEvent Event)
{
NH_TTY_BEGIN()

    NH_ENCODING_UTF32 c = Event.codepoint;

    switch (Window_p->Tiling.stage)
    {
        case NH_TTY_TILING_STAGE_OVERVIEW :

            if (c == NH_TTY_SPLIT_KEY) {
                // Split tile.
                NH_TTY_CHECK(nh_tty_splitTile(Window_p->Tile_p, NH_TTY_INSERT_TILE_RIGHT_KEY))
                Window_p->Tiling.stage = NH_TTY_TILING_STAGE_INSERT;
                NH_TTY_CHECK(nh_tty_updateTilingMessages(Window_p))
            }
            else if (c == NH_TTY_INSERT_TILE_LEFT_KEY || c == NH_TTY_INSERT_TILE_RIGHT_KEY || c == NH_TTY_INSERT_TILE_TOP_KEY || c == NH_TTY_INSERT_TILE_BOTTOM_KEY) {
                // Append or split tile.
                if (Window_p->Tile_p->Parent_p == NULL) {
                    NH_TTY_CHECK(nh_tty_splitTile(Window_p->Tile_p, c))
                } else {
                    NH_TTY_CHECK(nh_tty_addTile(Window_p->Tile_p, c))
                }
                Window_p->Tiling.stage = NH_TTY_TILING_STAGE_INSERT;
                NH_TTY_CHECK(nh_tty_updateTilingMessages(Window_p))
            } else {
                NH_TTY_CHECK(nh_tty_resetTiling(Window_p))
            }
            break;

        case NH_TTY_TILING_STAGE_INSERT :

            NH_TTY_CHECK(nh_tty_handlePotentialMacroTileInsertion(Window_p, c))
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_handleMicroTilingInput(
    nh_tty_MacroWindow *Window_p, nh_wsi_KeyboardEvent Event)
{
NH_TTY_BEGIN()

    NH_ENCODING_UTF32 c = Event.codepoint;

    switch (Window_p->Tiling.stage)
    {
        case NH_TTY_TILING_STAGE_DONE :

            if (c == NH_TTY_TILING_KEY) {
                Window_p->Tiling.stage = NH_TTY_TILING_STAGE_OVERVIEW; 
                Window_p->LastFocus_p = Window_p->Tile_p;
                NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->LastFocus_p = 
                    NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p;
                NH_TTY_CHECK(nh_tty_updateTilingMessages(Window_p))
            }
            break;

        case NH_TTY_TILING_STAGE_OVERVIEW :

            if (c == NH_TTY_TILING_KEY) {
                Window_p->Tiling.mode = NH_TTY_TILING_MODE_MACRO;
                NH_TTY_CHECK(nh_tty_updateTilingMessages(Window_p))
            } 
            else if (c == NH_TTY_SPLIT_KEY) {
                // Split tile.
                NH_TTY_CHECK(nh_tty_splitTile(NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p, NH_TTY_INSERT_TILE_RIGHT_KEY))
                Window_p->Tiling.stage = NH_TTY_TILING_STAGE_INSERT;
                NH_TTY_CHECK(nh_tty_updateTilingMessages(Window_p))
            }
            else if (c == NH_TTY_INSERT_TILE_LEFT_KEY || c == NH_TTY_INSERT_TILE_RIGHT_KEY || c == NH_TTY_INSERT_TILE_TOP_KEY || c == NH_TTY_INSERT_TILE_BOTTOM_KEY) {
                // Append or split tile.
                if (NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p->Parent_p == NULL) {
                    NH_TTY_CHECK(nh_tty_splitTile(NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p, c))
                } else {
                    NH_TTY_CHECK(nh_tty_addTile(NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(Window_p->Tile_p))->Tile_p, c))
                }
                Window_p->Tiling.stage = NH_TTY_TILING_STAGE_INSERT;
                NH_TTY_CHECK(nh_tty_updateTilingMessages(Window_p))
            }
            else {
                NH_TTY_CHECK(nh_tty_resetTiling(Window_p))
            }
            break;

        case NH_TTY_TILING_STAGE_INSERT :

            NH_TTY_CHECK(nh_tty_handlePotentialMicroTileInsertion(Window_p, c))
            break;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_handleTilingInput(
    nh_tty_MacroWindow *Window_p, nh_wsi_KeyboardEvent Event)
{
NH_TTY_BEGIN()

    if (Event.trigger != NH_WSI_TRIGGER_PRESS) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    if (Window_p->Tiling.mode == NH_TTY_TILING_MODE_MICRO) {
        NH_TTY_CHECK(nh_tty_handleMicroTilingInput(Window_p, Event))
    } else if (Window_p->Tiling.mode == NH_TTY_TILING_MODE_MACRO) {
        NH_TTY_CHECK(nh_tty_handleMacroTilingInput(Window_p, Event))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

