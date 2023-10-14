// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ContextMenu.h"
#include "TTY.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"
#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define COMMAND_NAME 'C', 'o', 'm', 'm', 'a', 'n', 'd',
#define PROGRAM_NAME 'P', 'r', 'o', 'g', 'r', 'a', 'm',

#define ARROW_RIGHT 0x2192
#define ARROW_LEFT 0x2190
#define ARROW_DOWN 0x2191
#define ARROW_UP 0x2193

// ACTION ==========================================================================================

static NH_BOOL nh_tty_pathMatchesContextMenu(
    nh_tty_ContextMenu *Menu_p, const NH_ENCODING_UTF32 **pp, int count)
{
NH_TTY_BEGIN()

    for (int i = count-1; i >= 0; --i) {
        if (!Menu_p || Menu_p->Name.length == 0) {
            NH_TTY_END(NH_FALSE)
        }
        for (int j = 0; j < Menu_p->Name.length; ++j) {
            if (pp[i][j] == 0 || pp[i][j] != Menu_p->Name.p[j]) {
                NH_TTY_END(NH_FALSE)
                break;
            }
        }
        Menu_p = Menu_p->Parent_p;
    }

NH_TTY_END(NH_TRUE)
}

static int nh_tty_isContextMenuTiling(
    nh_tty_ContextMenu *Menu_p)
{
NH_TTY_BEGIN()

    static const NH_ENCODING_UTF32 tiling_p[] = {'T', 'i', 'l', 'i', 'n', 'g', 0};
    static const NH_ENCODING_UTF32 macro_p[] = {'M', 'a', 'c', 'r', 'o', 0};
    static const NH_ENCODING_UTF32 micro_p[] = {'M', 'i', 'c', 'r', 'o', 0};
    static const NH_ENCODING_UTF32 append_p[] = {'A', 'p', 'p', 'e', 'n', 'd', 0};
    static const NH_ENCODING_UTF32 split_p[] = {'S', 'p', 'l', 'i', 't', 0};
    static const NH_ENCODING_UTF32 arrowUp_p[] = {ARROW_UP, 0};
    static const NH_ENCODING_UTF32 arrowLeft_p[] = {ARROW_LEFT, 0};
    static const NH_ENCODING_UTF32 arrowRight_p[] = {ARROW_RIGHT, 0};
    static const NH_ENCODING_UTF32 arrowDown_p[] = {ARROW_DOWN, 0};

    static const NH_ENCODING_UTF32 *paths_ppp[][16] = {
        {tiling_p, micro_p, arrowUp_p, append_p},
        {tiling_p, micro_p, arrowRight_p, append_p},
        {tiling_p, micro_p, arrowDown_p, append_p},
        {tiling_p, micro_p, arrowLeft_p, append_p},
        {tiling_p, micro_p, arrowUp_p, split_p},
        {tiling_p, micro_p, arrowRight_p, split_p},
        {tiling_p, micro_p, arrowDown_p, split_p},
        {tiling_p, micro_p, arrowLeft_p, split_p},
        {tiling_p, macro_p, arrowUp_p, append_p},
        {tiling_p, macro_p, arrowRight_p, append_p},
        {tiling_p, macro_p, arrowDown_p, append_p},
        {tiling_p, macro_p, arrowLeft_p, append_p},
        {tiling_p, macro_p, arrowUp_p, split_p},
        {tiling_p, macro_p, arrowRight_p, split_p},
        {tiling_p, macro_p, arrowDown_p, split_p},
        {tiling_p, macro_p, arrowLeft_p, split_p},
    };

    for (int i = 0; i < 16; ++i) {
        if (nh_tty_pathMatchesContextMenu(Menu_p, paths_ppp[i], 4)) {
            NH_TTY_END(i)
        }
    }

NH_TTY_END(-1)
}

static int nh_tty_isContextMenuTabing(
    nh_tty_ContextMenu *Menu_p)
{
NH_TTY_BEGIN()

    static const NH_ENCODING_UTF32 tabing_p[] = {'T', 'a', 'b', 'i', 'n', 'g', 0};
    static const NH_ENCODING_UTF32 macro_p[] = {'M', 'a', 'c', 'r', 'o', 0};
    static const NH_ENCODING_UTF32 micro_p[] = {'M', 'i', 'c', 'r', 'o', 0};
    static const NH_ENCODING_UTF32 p1[] = {'1', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p2[] = {'2', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p3[] = {'3', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p4[] = {'4', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p5[] = {'5', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p6[] = {'6', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p7[] = {'7', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p8[] = {'8', 1, ' ', 0};
    static const NH_ENCODING_UTF32 p9[] = {'9', 1, ' ', 0};
 
    static const NH_ENCODING_UTF32 *paths_ppp[][18] = {
        {tabing_p, macro_p, p1},
        {tabing_p, macro_p, p2},
        {tabing_p, macro_p, p3},
        {tabing_p, macro_p, p4},
        {tabing_p, macro_p, p5},
        {tabing_p, macro_p, p6},
        {tabing_p, macro_p, p7},
        {tabing_p, macro_p, p8},
        {tabing_p, macro_p, p9},
        {tabing_p, micro_p, p1},
        {tabing_p, micro_p, p2},
        {tabing_p, micro_p, p3},
        {tabing_p, micro_p, p4},
        {tabing_p, micro_p, p5},
        {tabing_p, micro_p, p6},
        {tabing_p, micro_p, p7},
        {tabing_p, micro_p, p8},
        {tabing_p, micro_p, p9},
    };

    for (int i = 0; i < 18; ++i) {
        if (nh_tty_pathMatchesContextMenu(Menu_p, paths_ppp[i], 3)) {
            NH_TTY_END(i)
        }
    }

NH_TTY_END(-1)
}

static int nh_tty_handleContextMenuTiling(
    int action)
{
NH_TTY_BEGIN()

    nh_wsi_KeyboardEvent Event;
    nh_tty_MacroWindow *Window_p = ((nh_tty_TTY*)nh_core_getWorkloadArg())->Window_p;
 
    switch (action)
    {
        // Handle micro tiling (append).
        case 0 :
        case 1 :
        case 2 :
        case 3 :
            Event.trigger = NH_WSI_TRIGGER_PRESS;
            Event.codepoint = NH_TTY_TILING_KEY;
            Event.state |= NH_WSI_MODIFIER_CONTROL;
            nh_tty_handleTilingInput(Window_p, Event); 

            if (action == 0) {
                Event.codepoint = NH_TTY_INSERT_TILE_TOP_KEY;
            } else if (action == 1) {
                Event.codepoint = NH_TTY_INSERT_TILE_RIGHT_KEY;
            } else if (action == 2) {
                Event.codepoint = NH_TTY_INSERT_TILE_BOTTOM_KEY;
            } else if (action == 3) {
                Event.codepoint = NH_TTY_INSERT_TILE_LEFT_KEY;
            }
 
            Event.state = 0;
            nh_tty_handleTilingInput(Window_p, Event);
            break;

        // Handle micro tiling (split).
        case 4 :
        case 5 :
        case 6 :
        case 7 :
            Event.trigger = NH_WSI_TRIGGER_PRESS;
            Event.codepoint = NH_TTY_TILING_KEY;
            Event.state |= NH_WSI_MODIFIER_CONTROL;
            nh_tty_handleTilingInput(Window_p, Event); 

            Event.state = 0;
            Event.codepoint = NH_TTY_SPLIT_KEY;
            nh_tty_handleTilingInput(Window_p, Event); 

            if (action == 4) {
                Event.codepoint = NH_TTY_INSERT_TILE_TOP_KEY;
            } else if (action == 5) {
                Event.codepoint = NH_TTY_INSERT_TILE_RIGHT_KEY;
            } else if (action == 6) {
                Event.codepoint = NH_TTY_INSERT_TILE_BOTTOM_KEY;
            } else if (action == 7) {
                Event.codepoint = NH_TTY_INSERT_TILE_LEFT_KEY;
            }
 
            nh_tty_handleTilingInput(Window_p, Event);
            break;
 
        // Handle macro tiling (append).
        case 8 :
        case 9 :
        case 10 :
        case 11 :
            Event.trigger = NH_WSI_TRIGGER_PRESS;
            Event.codepoint = NH_TTY_TILING_KEY;
            Event.state |= NH_WSI_MODIFIER_CONTROL;
            nh_tty_handleTilingInput(Window_p, Event); 
            nh_tty_handleTilingInput(Window_p, Event); 

            if (action == 10) {
                Event.codepoint = NH_TTY_INSERT_TILE_TOP_KEY;
            } else if (action == 9) {
                Event.codepoint = NH_TTY_INSERT_TILE_RIGHT_KEY;
            } else if (action == 8) {
                Event.codepoint = NH_TTY_INSERT_TILE_BOTTOM_KEY;
            } else if (action == 11) {
                Event.codepoint = NH_TTY_INSERT_TILE_LEFT_KEY;
            }

            Event.state = 0;
            nh_tty_handleTilingInput(Window_p, Event);
            break;

        // Handle macro tiling (split).
        case 12 :
        case 13 :
        case 14 :
        case 15 :
            Event.trigger = NH_WSI_TRIGGER_PRESS;
            Event.codepoint = NH_TTY_TILING_KEY;
            Event.state |= NH_WSI_MODIFIER_CONTROL;
            nh_tty_handleTilingInput(Window_p, Event); 
            nh_tty_handleTilingInput(Window_p, Event); 

            Event.codepoint = NH_TTY_SPLIT_KEY;
            Event.state = 0;
            nh_tty_handleTilingInput(Window_p, Event); 

            if (action == 14) {
                Event.codepoint = NH_TTY_INSERT_TILE_TOP_KEY;
            } else if (action == 13) {
                Event.codepoint = NH_TTY_INSERT_TILE_RIGHT_KEY;
            } else if (action == 12) {
                Event.codepoint = NH_TTY_INSERT_TILE_BOTTOM_KEY;
            } else if (action == 15) {
                Event.codepoint = NH_TTY_INSERT_TILE_LEFT_KEY;
            }
 
            nh_tty_handleTilingInput(Window_p, Event);
            break;
    }

NH_TTY_END(-1)
}

// CREATE/FREE =====================================================================================

static nh_tty_ContextMenu *nh_tty_parseContextMenu(
    NH_ENCODING_UTF32 **menu_pp, nh_tty_ContextMenu *Parent_p)
{
NH_TTY_BEGIN()

    nh_tty_ContextMenu *Menu_p = nh_core_allocate(sizeof(nh_tty_ContextMenu));
    NH_TTY_CHECK_MEM_2(NULL, Menu_p)

    Menu_p->Parent_p = Parent_p;
    Menu_p->active = NH_FALSE;
    Menu_p->hit = NH_FALSE;
    Menu_p->Name = nh_encoding_initUTF32(16); 
    Menu_p->Position.x = 0;
    Menu_p->Position.y = 0;
    Menu_p->Items = nh_core_initList(8);

    NH_BOOL curly = NH_FALSE;

    while (**menu_pp != 0) {
        if (**menu_pp == '{' || (**menu_pp == ',' && curly)) {
            (*menu_pp)++;
            nh_tty_ContextMenu *Child_p = nh_tty_parseContextMenu(menu_pp, Menu_p);
            NH_TTY_CHECK_NULL_2(NULL, Child_p)
            nh_core_appendToList(&Menu_p->Items, Child_p);
            curly = NH_TRUE;
            continue;
        }
        else if (**menu_pp == '}') {
            if (curly) {
                curly = NH_FALSE;
                (*menu_pp)++;
                continue;
            }
            NH_TTY_END(Menu_p)
        }
        else if (**menu_pp == ',') {
            NH_TTY_END(Menu_p)
        }
        else {
            nh_encoding_appendUTF32Codepoint(&Menu_p->Name, **menu_pp);
            (*menu_pp)++;
        }
    }

NH_TTY_END(Menu_p)
}

void nh_tty_freeContextMenu(
    nh_tty_ContextMenu *Menu_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_tty_freeContextMenu(Menu_p->Items.pp[i]);
    }
    nh_core_freeList(&Menu_p->Items, NH_FALSE);
    nh_encoding_freeUTF32(&Menu_p->Name);
    nh_core_free(Menu_p);

NH_TTY_SILENT_END()
}

// POSITION ========================================================================================

static void nh_tty_computeContextMenuPosition(
    nh_tty_ContextMenu *Menu_p, int x, int y, int maxX, int maxY)
{
NH_TTY_BEGIN()

    // Get menu width.

    int width = 0;
    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_encoding_UTF32String *Name_p = &((nh_tty_ContextMenu*)Menu_p->Items.pp[i])->Name;
        if (Name_p->length > width) {width = Name_p->length;}
    }

    Menu_p->Position.x = x;
    Menu_p->Position.y = y;

    // Make sure that context menu respects boundaries.
    if (Menu_p->Position.y + Menu_p->Items.size + 1 > maxY) {
        Menu_p->Position.y -= (Menu_p->Position.y + Menu_p->Items.size + 1) - maxY;
    }
    if (Menu_p->Position.y == 0) {
        Menu_p->Position.y = 1;
    }
    if (Menu_p->Position.x == 0) {
        Menu_p->Position.x = 1;
    }
    if (Menu_p->Position.x + width + 1 > maxX) {
        Menu_p->Position.x -= (Menu_p->Position.x + width + 1) - maxX;
        NH_TTY_SILENT_END() // TODO
    }

    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_tty_computeContextMenuPosition(Menu_p->Items.pp[i], x + width + 2, Menu_p->Position.y + i, maxX, maxY);
    }

NH_TTY_SILENT_END()
}

static NH_BOOL nh_tty_compareContextMenuName(
    nh_tty_ContextMenu *Menu_p, NH_BYTE *name_p)
{
NH_TTY_BEGIN()

    nh_encoding_UTF8String Name = nh_encoding_encodeUTF8(Menu_p->Name.p, Menu_p->Name.length);
    NH_BOOL result = !strcmp(Name.p, name_p);
    nh_encoding_freeUTF8(&Name);

NH_TTY_END(result)
}

nh_tty_ContextMenu *nh_tty_isContextMenuHit(
    nh_tty_ContextMenu *Menu_p, nh_tty_ContextMenu *Parent_p, NH_BOOL recursive, int x, int y)
{
NH_TTY_BEGIN()

    nh_tty_Config Config = nh_tty_getConfig();
    int x2 = Config.Sidebar.state == NH_TTY_SIDEBAR_STATE_LEFT ? x + 2 : x;
    int y2 = Menu_p->Position.y;

    int width = 0;
    for (int i = 0; Parent_p && i < Parent_p->Items.size; ++i) {
        nh_encoding_UTF32String *Name_p = &((nh_tty_ContextMenu*)Parent_p->Items.pp[i])->Name;
        if (Name_p->length > width) {width = Name_p->length;}
        if (Parent_p->Items.pp[i] == Menu_p) {y2 = Parent_p->Position.y + i;}
    }

    if (y2 == y && Parent_p && Parent_p->Position.x <= x2 && (Parent_p->Position.x + width) >= x2) {
        nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();
        int maxCols = ((nh_tty_View*)TTY_p->Views.pp[0])->cols;
        int width2 = 0;
        for (int i = 0; i < Menu_p->Items.size; ++i) {
            nh_encoding_UTF32String *Name_p = &((nh_tty_ContextMenu*)Menu_p->Items.pp[i])->Name;
            if (Name_p->length > width2) {width2 = Name_p->length;}
        }
        if ((Menu_p->Position.x + width2 + 2 < maxCols && Parent_p->Position.x + width + 2 < maxCols) || Menu_p->Items.size == 0) {
            NH_TTY_END(Menu_p)
        }
    }

    for (int i = 0; (Menu_p->active || Menu_p->hit) && i < Menu_p->Items.size && recursive; ++i) {
        nh_tty_ContextMenu *Result_p = nh_tty_isContextMenuHit(Menu_p->Items.pp[i], Menu_p, recursive, x, y);
        if (Result_p) {NH_TTY_END(Result_p)}
    }

NH_TTY_END(NULL)
}

void nh_tty_updateContextMenuHit(
    nh_tty_ContextMenu *Menu_p, nh_tty_ContextMenu *Parent_p, int x, int y, NH_BOOL activate)
{
NH_TTY_BEGIN()

    NH_BOOL newHit = !Menu_p->hit;
    Menu_p->hit = nh_tty_isContextMenuHit(Menu_p, Parent_p, NH_FALSE, x, y) != NULL && Menu_p->Name.length > 0;

    if (Menu_p->hit) {
        Menu_p->active = NH_TRUE;
    }
    if (Menu_p->active && Parent_p) {
        for (int i = 0; i < Parent_p->Items.size; ++i) {
            ((nh_tty_ContextMenu*)Parent_p->Items.pp[i])->active = Parent_p->Items.pp[i] == Menu_p;
            ((nh_tty_ContextMenu*)Parent_p->Items.pp[i])->hit = Parent_p->Items.pp[i] == Menu_p;
        }
    }

    if (newHit && Menu_p->hit) {
    }

    for (int i = 0; (Menu_p->active || Menu_p->hit) && i < Menu_p->Items.size; ++i) {
        nh_tty_updateContextMenuHit(Menu_p->Items.pp[i], Menu_p, x, y, activate);
        if (((nh_tty_ContextMenu*)Menu_p->Items.pp[i])->hit) {Menu_p->hit = NH_TRUE;}
    }

NH_TTY_SILENT_END()
}

// MOUSE MENU ======================================================================================
// Mouse menu functions.

nh_tty_ContextMenu *nh_tty_createMouseMenu(
    int x, int y)
{
NH_TTY_BEGIN()

    nh_encoding_UTF32String Menu = nh_encoding_initUTF32(128);
    nh_encoding_appendUTF32Codepoint(&Menu, '{');

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    nh_tty_Program *Program_p = nh_tty_getCurrentProgram(&NH_TTY_MACRO_TAB(TTY_p->Window_p->Tile_p)->MicroWindow);

    // Generate command menu.
    if (Program_p && Program_p->Prototype_p && Program_p->Prototype_p->commands > 0) {
        for (int i = 0; i < Program_p->Prototype_p->commands; ++i) {
            nh_encoding_UTF32String *Name_p = Program_p->Prototype_p->CommandNames_p+i;
            nh_encoding_appendUTF32(&Menu, Name_p->p, Name_p->length); 
            if (i < Program_p->Prototype_p->commands-1) {nh_encoding_appendUTF32Codepoint(&Menu, ',');}
        }
        nh_encoding_appendUTF32Codepoint(&Menu, ',');

        // Generate separator.
        nh_encoding_appendUTF32Codepoint(&Menu, '{'); 
        nh_encoding_appendUTF32Codepoint(&Menu, '}'); 
        nh_encoding_appendUTF32Codepoint(&Menu, ','); 
    }

    // Generate program menu.
    if (TTY_p->Prototypes.size > 0) {
        NH_ENCODING_UTF32 apps_p[] = {PROGRAM_NAME '{'};
        nh_encoding_appendUTF32(&Menu, apps_p, sizeof(apps_p)/sizeof(apps_p[0]));
        int width = 0;
        for (int i = 0; i < TTY_p->Prototypes.size; ++i) {
            if (((nh_tty_ProgramPrototype*)TTY_p->Prototypes.pp[i])->Name.length > width) {
                width = ((nh_tty_ProgramPrototype*)TTY_p->Prototypes.pp[i])->Name.length;
            }
        }
        for (int i = 0; i < TTY_p->Prototypes.size; ++i) {
            nh_tty_ProgramPrototype *Prototype_p = TTY_p->Prototypes.pp[i];
            nh_encoding_appendUTF32(&Menu, Prototype_p->Name.p, Prototype_p->Name.length); 
            for (int j = width - Prototype_p->Name.length; j > 0; --j) {
                nh_encoding_appendUTF32Codepoint(&Menu, ' '); 
            }
            nh_encoding_appendUTF32Codepoint(&Menu, 1); 
            nh_encoding_appendUTF32Codepoint(&Menu, Program_p->Prototype_p == Prototype_p ? 0x2022 : ' '); 
            if (i < TTY_p->Prototypes.size-1) {nh_encoding_appendUTF32Codepoint(&Menu, ',');}
        }
        nh_encoding_appendUTF32Codepoint(&Menu, '}');
        nh_encoding_appendUTF32Codepoint(&Menu, ',');
    }

    // Generate tiling menu.
    NH_ENCODING_UTF32 tiling_p[] = {
        'T', 'i', 'l', 'i', 'n', 'g', '{',
            'M', 'a', 'c', 'r', 'o', '{',
                ARROW_RIGHT, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}', ',',
                ARROW_LEFT, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}', ',',
                ARROW_DOWN, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}', ',',
                ARROW_UP, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}',
            '}', ',',
            'M', 'i', 'c', 'r', 'o', '{',
                ARROW_RIGHT, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}', ',',
                ARROW_LEFT, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}', ',',
                ARROW_DOWN, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}', ',',
                ARROW_UP, '{', 'S', 'p', 'l', 'i', 't', ',', 'A', 'p', 'p', 'e', 'n', 'd', '}',
            '}',
        '}',
    };
    nh_encoding_appendUTF32(&Menu, tiling_p, sizeof(tiling_p)/sizeof(tiling_p[0]));
    nh_encoding_appendUTF32Codepoint(&Menu, ',');

    // Generate tabing menu.
    NH_ENCODING_UTF32 tabing_p[] = {
        'T', 'a', 'b', 'i', 'n', 'g', '{',
            'M', 'a', 'c', 'r', 'o', '{',
                '1', 1, ' ', ',', '2', 1, ' ', ',', '3', 1, ' ', ',', '4', 1, ' ', ',', '5', 1, ' ', ',', '6', 1, ' ', ',', '7', 1, ' ', ',', '8', 1, ' ', ',', '9', 1, ' ',
            '}', ',',
            'M', 'i', 'c', 'r', 'o', '{',
                '1', 1, ' ', ',', '2', 1, ' ', ',', '3', 1, ' ', ',', '4', 1, ' ', ',', '5', 1, ' ', ',', '6', 1, ' ', ',', '7', 1, ' ', ',', '8', 1, ' ', ',', '9', 1, ' ',
            '}',
        '}', ',', 
        'D', 'e', 'b', 'u', 'g',
    };

    tabing_p[15 + nh_core_getListIndex(&TTY_p->Windows, TTY_p->Window_p) * 4] = 0x2022;
    tabing_p[58 + ((nh_tty_MacroTile*)TTY_p->Window_p->Tile_p->p)->current * 4] = 0x2022;

    nh_encoding_appendUTF32(&Menu, tabing_p, sizeof(tabing_p)/sizeof(tabing_p[0]));
    nh_encoding_appendUTF32Codepoint(&Menu, '{');

    NH_ENCODING_UTF32 x_p[16];
    NH_ENCODING_UTF32 y_p[16];

    int xLength = nh_encoding_integerToUTF32(x, x_p, 64);
    int yLength = nh_encoding_integerToUTF32(y, y_p, 64);
 
    nh_encoding_appendUTF32Codepoint(&Menu, 'x');
    nh_encoding_appendUTF32Codepoint(&Menu, ':');
    for (int i = 0; i < xLength; ++i) {
        nh_encoding_appendUTF32Codepoint(&Menu, x_p[i]);
    }
    nh_encoding_appendUTF32Codepoint(&Menu, ',');

    nh_encoding_appendUTF32Codepoint(&Menu, 'y');
    nh_encoding_appendUTF32Codepoint(&Menu, ':');
    for (int i = 0; i < yLength; ++i) {
        nh_encoding_appendUTF32Codepoint(&Menu, y_p[i]);
    }
    nh_encoding_appendUTF32Codepoint(&Menu, ',');

    nh_tty_Glyph Glyph = ((nh_tty_View*)TTY_p->Views.pp[0])->Grid1_p[y].Glyphs_p[x];

    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.bold ? "Attr.bold:1," : "Attr.bold:0,", 12);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.faint ? "Attr.faint:1," : "Attr.faint:0,", 13);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.italic ? "Attr.italic:1," : "Attr.italic:0,", 14);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.underline ? "Attr.underline:1," : "Attr.underline:0,", 17);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.blink ? "Attr.blink:1," : "Attr.blink:0,", 13);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.reverse ? "Attr.reverse:1," : "Attr.reverse:0,", 15);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.invisible ? "Attr.invisible:1," : "Attr.invisible:0,", 17);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.struck ? "Attr.struck:1," : "Attr.struck:0,", 14);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.wrap ? "Attr.wrap:1," : "Attr.wrap:0,", 12);
    nh_encoding_appendUTF8ToUTF32(&Menu, Glyph.Attributes.wide ? "Attr.wide:1}" : "Attr.wide:0}", 12);
 
    nh_encoding_appendUTF32Codepoint(&Menu, '}');

    NH_ENCODING_UTF32 *p = Menu.p;
    nh_tty_ContextMenu *Menu_p = nh_tty_parseContextMenu(&p, NULL);
    NH_TTY_CHECK_NULL_2(NULL, Menu_p)

    nh_tty_computeContextMenuPosition(Menu_p, x, y, ((nh_tty_View*)TTY_p->Views.pp[0])->cols, ((nh_tty_View*)TTY_p->Views.pp[0])->rows);

    nh_encoding_freeUTF32(&Menu);

NH_TTY_END(Menu_p)
}

NH_TTY_RESULT nh_tty_handleMouseMenuPress(
    nh_tty_ContextMenu *Menu_p)
{
NH_TTY_BEGIN()

    if (!Menu_p->Parent_p || Menu_p->Name.length == 0) {NH_TTY_END(NH_TTY_SUCCESS)}

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    // Handle tiling or tabing if necessary.
    int tiling = nh_tty_isContextMenuTiling(Menu_p);
    if (tiling >= 0) {
        nh_tty_handleContextMenuTiling(tiling);
        nh_wsi_KeyboardEvent Event;
        Event.trigger = NH_WSI_TRIGGER_PRESS;
        Event.codepoint = 13;
        NH_TTY_END(nh_tty_handleTilingInput(TTY_p->Window_p, Event))
    }

    int tabing = nh_tty_isContextMenuTabing(Menu_p);
    if (tabing >= 0) {
        if (tabing < 9) {
            NH_TTY_CHECK(nh_tty_insertAndFocusWindow(TTY_p, tabing))
        } else {
            ((nh_tty_MacroTile*)TTY_p->Window_p->Tile_p->p)->current = tabing - 9; 
        }
        NH_TTY_END(NH_TTY_SUCCESS)
    }

    // Handle program switch if necessary.
    NH_ENCODING_UTF32 program_p[] = {PROGRAM_NAME 0};
    if (nh_encoding_compareUTF32(Menu_p->Parent_p->Name.p, program_p)) {
        for (int i = 0; i < Menu_p->Parent_p->Items.size; ++i) {
            nh_tty_ContextMenu *Prog_p = Menu_p->Parent_p->Items.pp[i];
            if (Prog_p->active) {
                NH_TTY_MACRO_TAB(((nh_tty_TTY*)nh_core_getWorkloadArg())->Window_p->Tile_p)->MicroWindow.current = i;
            } 
        }
    } else {
        // Handle command execution if necessary.
        for (int i = 0; i < Menu_p->Parent_p->Items.size; ++i) {
            nh_tty_ContextMenu *Cmd_p = Menu_p->Parent_p->Items.pp[i];
            if (Cmd_p->active) {
                nh_tty_Program *Program_p = nh_tty_getCurrentProgram(&NH_TTY_MACRO_TAB(TTY_p->Window_p->Tile_p)->MicroWindow);
                if (Program_p->Prototype_p->Callbacks.handleCommand_f) {
                    Program_p->command = i;
                    Program_p->Prototype_p->Callbacks.handleCommand_f(Program_p, NULL);
                }
            } 
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static NH_TTY_RESULT nh_tty_drawContextMenuRecursively(
    nh_tty_ContextMenu *Menu_p, nh_tty_Row *Grid_p)
{
NH_TTY_BEGIN()

    if (Menu_p == NULL || Menu_p->Items.size == 0 || Menu_p->hit == NH_FALSE && Menu_p->active == NH_FALSE) {
        NH_TTY_END(NH_TTY_SUCCESS)
    }

    int width = 0;
    int height = Menu_p->Items.size;

    // Get menu width.
    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_encoding_UTF32String *String_p = &((nh_tty_ContextMenu*)Menu_p->Items.pp[i])->Name;
        if (String_p->length > width) {width = String_p->length;}
    }

    // Draw item names.
    for (int row = Menu_p->Position.y, i = 0; row < Menu_p->Position.y+height; ++row, ++i) {
        nh_tty_ContextMenu *Child_p = Menu_p->Items.pp[i];
        if (Child_p->hit) {
            for (int col = Menu_p->Position.x, j = 0; j < width && Child_p->Name.p[j] != 1; ++col, ++j) {
                Grid_p[row].Glyphs_p[col].Attributes.reverse = NH_TRUE;
            }
        }
        for (int col = Menu_p->Position.x, j = 0; col < Menu_p->Position.x+width; ++col, ++j) {
            Grid_p[row].Glyphs_p[col].codepoint = Child_p->Name.length > j ? Child_p->Name.p[j] : ' ';
            // Draw horizontal separator if required.
            if (Child_p->Name.length == 0) {
                Grid_p[row].Glyphs_p[col].codepoint = 'q';
                Grid_p[row].Glyphs_p[col].mark |= NH_TTY_MARK_LINE_GRAPHICS;
            }
            // Draw vertical separator if required.
            else if (Child_p->Name.p[j] == 1) {
                Grid_p[row].Glyphs_p[col].codepoint = 'x';
                Grid_p[row].Glyphs_p[col].mark |= NH_TTY_MARK_LINE_GRAPHICS;
            }
        }
    }

    // Add basic outer lines.
    for (int row = Menu_p->Position.y, i = 0; row < Menu_p->Position.y+height; ++row, ++i) {
        Grid_p[row].Glyphs_p[Menu_p->Position.x-1].codepoint = 'x';
        Grid_p[row].Glyphs_p[Menu_p->Position.x-1].mark |= NH_TTY_MARK_LINE_GRAPHICS;
        Grid_p[row].Glyphs_p[Menu_p->Position.x+width].codepoint = 'x';
        Grid_p[row].Glyphs_p[Menu_p->Position.x+width].mark |= NH_TTY_MARK_LINE_GRAPHICS;
    }

    for (int col = Menu_p->Position.x-1, j = 0; col < Menu_p->Position.x+width+1; ++col, ++j) {
        Grid_p[Menu_p->Position.y-1].Glyphs_p[col].codepoint = 'q';
        Grid_p[Menu_p->Position.y-1].Glyphs_p[col].mark |= NH_TTY_MARK_LINE_GRAPHICS;
        Grid_p[Menu_p->Position.y+height].Glyphs_p[col].codepoint = 'q';
        Grid_p[Menu_p->Position.y+height].Glyphs_p[col].mark |= NH_TTY_MARK_LINE_GRAPHICS;
    }

    Grid_p[Menu_p->Position.y-1].Glyphs_p[Menu_p->Position.x-1].codepoint = 'l';
    Grid_p[Menu_p->Position.y-1].Glyphs_p[Menu_p->Position.x-1].mark |= NH_TTY_MARK_LINE_GRAPHICS;
    Grid_p[Menu_p->Position.y-1].Glyphs_p[Menu_p->Position.x+width].codepoint = 'k';
    Grid_p[Menu_p->Position.y-1].Glyphs_p[Menu_p->Position.x+width].mark |= NH_TTY_MARK_LINE_GRAPHICS;
    Grid_p[Menu_p->Position.y+height].Glyphs_p[Menu_p->Position.x-1].codepoint = 'm';
    Grid_p[Menu_p->Position.y+height].Glyphs_p[Menu_p->Position.x-1].mark |= NH_TTY_MARK_LINE_GRAPHICS;
    Grid_p[Menu_p->Position.y+height].Glyphs_p[Menu_p->Position.x+width].codepoint = 'j';
    Grid_p[Menu_p->Position.y+height].Glyphs_p[Menu_p->Position.x+width].mark |= NH_TTY_MARK_LINE_GRAPHICS;

    // Add horizontal outer-line separator |- piece.
    for (int row = Menu_p->Position.y; row < Menu_p->Position.y+height; ++row) {
        if (Grid_p[row].Glyphs_p[Menu_p->Position.x].codepoint == 'q' 
        &&  Grid_p[row].Glyphs_p[Menu_p->Position.x].mark & NH_TTY_MARK_LINE_GRAPHICS) {
            Grid_p[row].Glyphs_p[Menu_p->Position.x-1].codepoint = 't';
        }
    } 

    // Add horizontal outer-line separator -| piece.
    for (int row = Menu_p->Position.y; row < Menu_p->Position.y+height; ++row) {
        if (Grid_p[row].Glyphs_p[Menu_p->Position.x+width-1].codepoint == 'q' 
        &&  Grid_p[row].Glyphs_p[Menu_p->Position.x+width-1].mark & NH_TTY_MARK_LINE_GRAPHICS) {
            Grid_p[row].Glyphs_p[Menu_p->Position.x+width].codepoint = 'u';
        }
    }
 
    // Add vertical outer-line separator T piece.
    for (int col = Menu_p->Position.x; col < Menu_p->Position.x+width; ++col) {
        if (Grid_p[Menu_p->Position.y].Glyphs_p[col].codepoint == 'x' 
        &&  Grid_p[Menu_p->Position.y].Glyphs_p[col].mark & NH_TTY_MARK_LINE_GRAPHICS) {
            Grid_p[Menu_p->Position.y-1].Glyphs_p[col].codepoint = 'w';
        }
    }
 
    // Add vertical outer-line separator bottom piece.
    for (int col = Menu_p->Position.x; col < Menu_p->Position.x+width; ++col) {
        if (Grid_p[Menu_p->Position.y+height-1].Glyphs_p[col].codepoint == 'x' 
        &&  Grid_p[Menu_p->Position.y+height-1].Glyphs_p[col].mark & NH_TTY_MARK_LINE_GRAPHICS) {
            Grid_p[Menu_p->Position.y+height].Glyphs_p[col].codepoint = 'v';
        }
    }
 
    // Recursion.
    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_tty_ContextMenu *Child_p = Menu_p->Items.pp[i];
        NH_TTY_CHECK(nh_tty_drawContextMenuRecursively(Child_p, Grid_p))
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_refreshGrid2(
    nh_tty_TTY *TTY_p)
{
NH_TTY_BEGIN()

    nh_tty_View *View_p = TTY_p->Views.pp[0];

    for (int row = 0; row < View_p->rows; ++row) {
        memset(View_p->Grid2_p[row].Glyphs_p, 0, sizeof(nh_tty_Glyph)*View_p->cols);
    }
 
    NH_TTY_CHECK(nh_tty_drawContextMenuRecursively(TTY_p->Window_p->MouseMenu_p, View_p->Grid2_p))
    NH_TTY_CHECK(nh_tty_forwardGrid2(View_p))

NH_TTY_END(NH_TTY_SUCCESS)
}

