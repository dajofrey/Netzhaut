// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TopBar.h"
#include "Program.h"
#include "TTY.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"
#include "../../nhcore/Util/Time.h"

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

// INIT/FREE =======================================================================================

nh_tty_TopBar nh_tty_initTopBar()
{
NH_TTY_BEGIN()

    nh_tty_TopBar TopBar;

    TopBar.Suggestions.Time = nh_core_getSystemTime();
    TopBar.Suggestions.ProgramSuggestion_p = NULL;
    TopBar.Suggestions.CommandSuggestion_p = NULL;
    TopBar.Suggestions.interval = 0.5;
    TopBar.Suggestions.update = NH_FALSE;
    TopBar.Suggestions.programIndex = -1;
    TopBar.Suggestions.commandIndex = -1;

    TopBar.Message.block      = NH_FALSE;
    TopBar.Message.Text       = nh_encoding_initUTF32(32);
    TopBar.Message.args_p     = NULL;
    TopBar.Message.callback_f = NULL;

    TopBar.state    = -1;
    TopBar.hasFocus = NH_FALSE;
    TopBar.refresh  = NH_FALSE;
    TopBar.Command  = nh_encoding_initUTF32(32);
    TopBar.History  = nh_core_initArray(sizeof(nh_encoding_UTF32String), 255);
    TopBar.cursorX  = 0;

NH_TTY_END(TopBar)
}

NH_TTY_RESULT nh_tty_freeTopBar(
    nh_tty_TopBar *TopBar_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < TopBar_p->History.length; ++i) {
        nh_encoding_freeUTF32(((nh_encoding_UTF32String*)TopBar_p->History.p)+i);
    }
    nh_core_freeArray(&TopBar_p->History);
    nh_encoding_freeUTF32(&TopBar_p->Command);
    nh_encoding_freeUTF32(&TopBar_p->Message.Text);

    *TopBar_p = nh_tty_initTopBar(NULL);

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_getTopBarCursor(
    nh_tty_TopBar *TopBar_p, int *x_p, int *y_p, NH_BOOL topTile)
{
NH_TTY_BEGIN()

    *x_p = TopBar_p->cursorX;
    *y_p = 1;

NH_TTY_END(NH_TTY_SUCCESS)
}

// COMMAND =========================================================================================

static NH_TTY_RESULT nh_tty_appendToCommand(
    nh_tty_TopBar *TopBar_p, NH_ENCODING_UTF32 *str_p, int length)
{
NH_TTY_BEGIN()

    nh_encoding_appendUTF32(&TopBar_p->Command, str_p, length);
    TopBar_p->cursorX += length;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static void nh_tty_clearCommand(
    nh_tty_TopBar *TopBar_p)
{
NH_TTY_BEGIN()

    nh_encoding_freeUTF32(&TopBar_p->Command);
    TopBar_p->Command = nh_encoding_initUTF32(32);
    TopBar_p->cursorX = 0;

NH_TTY_SILENT_END()
}

// CONSOLE STATE ===================================================================================

static NH_TTY_RESULT nh_tty_resetTopBar(
    nh_tty_TopBar *TopBar_p)
{
NH_TTY_BEGIN()

    TopBar_p->state = -1;
    TopBar_p->hasFocus = NH_FALSE;
    nh_tty_clearCommand(TopBar_p);

NH_TTY_END(NH_TTY_SUCCESS)
}

void nh_tty_toggleTopBar(
    nh_tty_TopBar *TopBar_p)
{
NH_TTY_BEGIN()

    if (!TopBar_p->hasFocus) {
        TopBar_p->hasFocus = NH_TRUE;
        TopBar_p->state = 0;
    }
    else if (TopBar_p->state == 0) {
        if (((nh_tty_TTY*)nh_core_getWorkloadArg())->Prototypes.size < 2) {
            nh_tty_resetTopBar(TopBar_p);
        } else {
            TopBar_p->state = 1;
        }
    }
    else if (TopBar_p->state == 1) {
        nh_tty_resetTopBar(TopBar_p);
    }

    nh_tty_clearCommand(TopBar_p);

NH_TTY_SILENT_END()
}

// INTERNAL COMMAND ================================================================================

static int nh_tty_matchPrograms(
    nh_List *Tabs_p, nh_encoding_UTF32String *Line_p)
{
NH_TTY_BEGIN()

    nh_List Names = nh_core_initList(16);

    for (int i = 0; i < Tabs_p->size; ++i) {
        nh_core_appendToList(&Names, &((nh_tty_MicroTab*)Tabs_p->pp[i])->Prototype_p->Name); 
    } 

    nh_encoding_UTF32String *String_p = nh_encoding_getBestMatch(&Names, Line_p);
    nh_core_freeList(&Names, NH_FALSE);

    for (int i = 0; i < Tabs_p->size; ++i) {
        if (String_p ==  &((nh_tty_MicroTab*)Tabs_p->pp[i])->Prototype_p->Name) {
            NH_TTY_END(i)
        }
    } 

NH_TTY_END(-1)
}

static NH_TTY_RESULT nh_tty_executeProgramSwitchCommand(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p)
{
NH_TTY_BEGIN()

    int program = TopBar_p->Command.length > 0 ?
        nh_tty_matchPrograms(Window_p->Tabs_p, &TopBar_p->Command) : -1;

    if (program >= 0)
    {
        Window_p->current = program;
        NH_TTY_CHECK(nh_tty_resetTopBar(TopBar_p))
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// PROGRAM COMMAND ==================================================================================

static int nh_tty_matchProgramCommands(
    nh_encoding_UTF32String *CommandNames_p, int commands, nh_encoding_UTF32String *Line_p)
{
NH_TTY_BEGIN()

    nh_List Commands = nh_core_initList(commands);
    for (int i = 0; i < commands; ++i) {
        nh_core_appendToList(&Commands, CommandNames_p+i); 
    } 

    nh_encoding_UTF32String *Command_p = nh_encoding_getBestMatch(&Commands, Line_p);

    int index = -1;
    for (int i = 0; i < Commands.size; ++i) {
        if (Command_p == Commands.pp[i]) {index = i; break;}
    } 

    nh_core_freeList(&Commands, NH_FALSE);

NH_TTY_END(index)
}

static NH_TTY_RESULT nh_tty_getArguments(
    nh_tty_TopBar *TopBar_p, nh_List *Arguments_p)
{
NH_TTY_BEGIN()

    *Arguments_p = nh_core_initList(8);
    nh_encoding_UTF32String *Argument_p = NULL;

    int offset = 0;
    for (offset = 0; offset < TopBar_p->Command.length && TopBar_p->Command.p[offset] != ' '; ++offset);
    offset++;

    for (int i = offset; i < TopBar_p->Command.length; ++i) {
        if (Argument_p && TopBar_p->Command.p[i] == ' ') {
            nh_core_appendToList(Arguments_p, Argument_p);
            Argument_p = NULL;
        }
        if (!Argument_p && TopBar_p->Command.p[i] != ' ') {
            Argument_p = nh_core_allocate(sizeof(nh_encoding_UTF32String));
            NH_TTY_CHECK_MEM(Argument_p)
            *Argument_p = nh_encoding_initUTF32(64);
        }
        if (Argument_p) {
            nh_encoding_appendUTF32Codepoint(Argument_p, TopBar_p->Command.p[i]);
        }
    }

    if (Argument_p) {nh_core_appendToList(Arguments_p, Argument_p);}

NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_executeProgramCommand(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p)
{
NH_TTY_BEGIN()

    nh_tty_Program *Program_p = nh_tty_getCurrentProgram(Window_p);

    if (Program_p->Prototype_p->Callbacks.handleCommand_f != NULL) 
    {
        nh_List Arguments = nh_core_initList(8);
        NH_TTY_CHECK(nh_tty_getArguments(TopBar_p, &Arguments))

        Program_p->command = TopBar_p->Suggestions.commandIndex;
        NH_TTY_RESULT result = Program_p->Prototype_p->Callbacks.handleCommand_f(Program_p, &Arguments);

        switch (result)
        {
            case NH_TTY_ERROR_UNKNOWN_COMMAND :
                NH_TTY_CHECK(nh_tty_setDefaultMessage(TopBar_p, NH_TTY_MESSAGE_MISC_UNKNOWN_COMMAND))
                break;
             case NH_TTY_ERROR_INVALID_ARGUMENT :
                NH_TTY_CHECK(nh_tty_setDefaultMessage(TopBar_p, NH_TTY_MESSAGE_MISC_INVALID_ARGUMENT))
                break;
        }

        for (int i = 0; i < Arguments.size; ++i) {
            nh_encoding_freeUTF32(Arguments.pp[i]);
        }
        nh_core_freeList(&Arguments, NH_TRUE);
    }
    else {
        NH_TTY_CHECK(nh_tty_setDefaultMessage(TopBar_p, NH_TTY_MESSAGE_MISC_UNKNOWN_COMMAND))
    }

    NH_TTY_CHECK(nh_tty_resetTopBar(TopBar_p))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

NH_TTY_RESULT nh_tty_handleTopBarInput(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p, nh_wsi_KeyboardEvent Event)
{
NH_TTY_BEGIN()

    if (Event.trigger != NH_WSI_TRIGGER_PRESS) {NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)}

    NH_ENCODING_UTF32 c = Event.codepoint;

    switch (c)
    {
        case '\n' :
        case '\r' :
            if (TopBar_p->state == 0) {
                NH_TTY_DIAGNOSTIC_END(nh_tty_executeProgramCommand(TopBar_p, Window_p))
            }
            else {
                NH_TTY_CHECK(nh_tty_executeProgramSwitchCommand(TopBar_p, Window_p))
            }

        case '\x1b':
            NH_TTY_DIAGNOSTIC_END(nh_tty_resetTopBar(TopBar_p))

        case 127:
        case 8:
            if (TopBar_p->Command.length > 0) {
                nh_encoding_removeUTF32Tail(&TopBar_p->Command, 1);
                TopBar_p->cursorX -= 1;
            }
            NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS) 
    }
    
    NH_TTY_CHECK(nh_tty_appendToCommand(TopBar_p, &c, 1))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static nh_tty_Glyph nh_tty_getGlyphHelper(
    NH_ENCODING_UTF32 codepoint)
{
NH_TTY_BEGIN()

    nh_tty_Glyph Glyph;
    memset(&Glyph, 0, sizeof(nh_tty_Glyph));
    Glyph.Attributes.reverse = NH_TRUE;
    Glyph.codepoint = codepoint;
    Glyph.mark |= NH_TTY_MARK_LINE_HORIZONTAL | NH_TTY_MARK_ACCENT;

NH_TTY_END(Glyph)
}

static int nh_tty_drawTopBarCommand(
    nh_tty_TopBar *TopBar_p, nh_tty_Glyph **Glyphs_pp, int *cols_p)
{
NH_TTY_BEGIN()

    nh_tty_Glyph Glyph = nh_tty_getGlyphHelper(' ');

    int length = TopBar_p->Command.length > *cols_p ? *cols_p : TopBar_p->Command.length;

    for (int i = 0; i < TopBar_p->Command.length && i < length; ++i) {
        Glyph.codepoint = TopBar_p->Command.p[i];
        (*Glyphs_pp)[i] = Glyph;
    }

    *cols_p = *cols_p - length;
    *Glyphs_pp = (*Glyphs_pp) + length;

NH_TTY_END(NH_TTY_SUCCESS)
}

static void nh_tty_setNextCommandSuggestionIndex(
    nh_tty_TopBar *TopBar_p, nh_tty_ProgramPrototype *Prototype_p)
{
NH_TTY_BEGIN() 

    if (TopBar_p->Suggestions.commandIndex == -1) {
        TopBar_p->Suggestions.commandIndex = 0;
        NH_TTY_SILENT_END()
    }
    if (TopBar_p->Suggestions.commandIndex >= Prototype_p->commands-1) {
        TopBar_p->Suggestions.commandIndex = 0;
        NH_TTY_SILENT_END()
    }
    TopBar_p->Suggestions.commandIndex++;

NH_TTY_SILENT_END()
}

static void nh_tty_setNextProgramSuggestionIndex(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p)
{
NH_TTY_BEGIN() 

    if (TopBar_p->Suggestions.programIndex == -1) {
        TopBar_p->Suggestions.programIndex = 0;
	NH_TTY_SILENT_END()
    }
    if (TopBar_p->Suggestions.programIndex >= Window_p->Tabs_p->size-1) {
	TopBar_p->Suggestions.programIndex = 0;
	NH_TTY_SILENT_END()
    }
    TopBar_p->Suggestions.programIndex++;

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT nh_tty_drawTopBarCommandSuggestion(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p, nh_tty_Glyph *Glyphs_p, int cols)
{
NH_TTY_BEGIN()

    nh_tty_Program *Program_p = nh_tty_getCurrentProgram(Window_p);

    Glyphs_p[2] = nh_tty_getGlyphHelper(':');
    Glyphs_p[3] = nh_tty_getGlyphHelper(' ');
 
    if (TopBar_p->Suggestions.commandIndex == -1 || !Program_p || !Program_p->Prototype_p->commands) {
        Glyphs_p[4] = nh_tty_getGlyphHelper('n');
        Glyphs_p[5] = nh_tty_getGlyphHelper('o');
        Glyphs_p[6] = nh_tty_getGlyphHelper('n');
        Glyphs_p[7] = nh_tty_getGlyphHelper('e');
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    Glyphs_p += 4;
    cols -= 4;

    int length = Program_p->Prototype_p->CommandNames_p[TopBar_p->Suggestions.commandIndex].length;
    for (int i = 0; i < length; ++i) {
        Glyphs_p[i] = nh_tty_getGlyphHelper(
            Program_p->Prototype_p->CommandNames_p[TopBar_p->Suggestions.commandIndex].p[i]);
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_drawTopBarProgramSuggestion(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p, nh_tty_Glyph *Glyphs_p, int cols)
{
NH_TTY_BEGIN()

    Glyphs_p[2] = nh_tty_getGlyphHelper(':');
    Glyphs_p[3] = nh_tty_getGlyphHelper(' ');
 
    if (TopBar_p->Suggestions.programIndex == -1 || !Window_p->Tabs_p) {
        Glyphs_p[4] = nh_tty_getGlyphHelper('n');
        Glyphs_p[5] = nh_tty_getGlyphHelper('o');
        Glyphs_p[6] = nh_tty_getGlyphHelper('n');
        Glyphs_p[7] = nh_tty_getGlyphHelper('e');
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    nh_tty_ProgramPrototype *Prototype_p = 
        ((nh_tty_MicroTab*)Window_p->Tabs_p->pp[TopBar_p->Suggestions.programIndex])->Prototype_p;
    NH_TTY_CHECK_NULL(Prototype_p)

    Glyphs_p += 4;
    cols -= 4;

    for (int i = 0; i < Prototype_p->Name.length && i < cols; ++i) {
        Glyphs_p[i] = nh_tty_getGlyphHelper(Prototype_p->Name.p[i]);
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_setSuggestionIndex(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p, NH_BOOL command)
{
NH_TTY_BEGIN()

    nh_tty_ProgramPrototype *Prototype_p = nh_tty_getCurrentProgram(Window_p)->Prototype_p;
 
    // Check if current is perfect match.
    NH_BOOL match = TopBar_p->Command.length ? NH_TRUE : NH_FALSE;
    if (command) {
        if (TopBar_p->Suggestions.commandIndex == -1) {
            match = NH_FALSE;
        }
        else if (!Prototype_p->CommandNames_p) {
            match = NH_FALSE;
        }
        else if (TopBar_p->Command.length != 
            Prototype_p->CommandNames_p[TopBar_p->Suggestions.commandIndex].length) {
                match = NH_FALSE;
        }
        for (int i = 0; match && i < TopBar_p->Command.length; ++i) {
            if (TopBar_p->Command.p[i] != 
                Prototype_p->CommandNames_p[TopBar_p->Suggestions.commandIndex].p[i]) {
                match = NH_FALSE;
            }
        }
    } else {
        if (TopBar_p->Suggestions.programIndex == -1) {
            match = NH_FALSE;
        }
        else {
            Prototype_p = 
                ((nh_tty_MicroTab*)Window_p->Tabs_p->pp[TopBar_p->Suggestions.programIndex])->Prototype_p;
            NH_TTY_CHECK_NULL(Prototype_p)
        }
        if (match && TopBar_p->Command.length != Prototype_p->Name.length) {
            match = NH_FALSE;
        }
        for (int i = 0; match && i < TopBar_p->Command.length && i < Prototype_p->Name.length; ++i) {
            if (TopBar_p->Command.p[i] != Prototype_p->Name.p[i]) {
                match = NH_FALSE;
            }
        }
    }

    // If perfect match, we don't need a new index.
    if (match) {
        NH_TTY_END(NH_TTY_SUCCESS)
    }

    // Find semi match or none.
    NH_BOOL ok = NH_FALSE;
    int count = 0;
    int previousCommandSuggestionIndex = TopBar_p->Suggestions.commandIndex; 
    while (!ok) {
        if (command) {
            nh_tty_setNextCommandSuggestionIndex(TopBar_p, Prototype_p);
        } else {
            nh_tty_setNextProgramSuggestionIndex(TopBar_p, Window_p);
        }
        if (TopBar_p->Command.length == 0) {
            ok = NH_TRUE;
        }
        match = NH_TRUE;
        if (command) {
            if (!Prototype_p->CommandNames_p) {
                match = NH_FALSE;
            }
            else if (TopBar_p->Command.length >
                Prototype_p->CommandNames_p[TopBar_p->Suggestions.commandIndex].length) {
                    match = NH_FALSE;
            }
            for (int i = 0; match && i < TopBar_p->Command.length; ++i) {
                if (TopBar_p->Command.p[i] != 
                    Prototype_p->CommandNames_p[TopBar_p->Suggestions.commandIndex].p[i]) {
                    match = NH_FALSE;
                }
            }
            if (match) {ok = NH_TRUE;}
            if (++count > Prototype_p->commands) {
                TopBar_p->Suggestions.commandIndex = previousCommandSuggestionIndex;
                break;
            }
        } else {
            Prototype_p = 
                ((nh_tty_MicroTab*)Window_p->Tabs_p->pp[TopBar_p->Suggestions.programIndex])->Prototype_p;
            NH_TTY_CHECK_NULL(Prototype_p)
            for (int i = 0; i < TopBar_p->Command.length && i < Prototype_p->Name.length; ++i) {
                if (TopBar_p->Command.p[i] != Prototype_p->Name.p[i]) {
                    match = NH_FALSE;
                }
            }
            if (match) {ok = NH_TRUE;}
            if (++count > Window_p->Tabs_p->size) {
                TopBar_p->Suggestions.programIndex = -1;
                break;
            }
        }
    }
 
NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_drawTopBarSuggestion(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p, nh_tty_Glyph *Glyphs_p, int cols)
{
NH_TTY_BEGIN()

    if (TopBar_p->Suggestions.update) {
        nh_tty_setSuggestionIndex(TopBar_p, Window_p, NH_TRUE);
        nh_tty_setSuggestionIndex(TopBar_p, Window_p, NH_FALSE);
        TopBar_p->Suggestions.update = NH_FALSE;
    }

    if (TopBar_p->state == 0) {
	NH_TTY_DIAGNOSTIC_END(nh_tty_drawTopBarCommandSuggestion(TopBar_p, Window_p, Glyphs_p, cols))
    }
    if (TopBar_p->state == 1) {
	NH_TTY_DIAGNOSTIC_END(nh_tty_drawTopBarProgramSuggestion(TopBar_p, Window_p, Glyphs_p, cols))
    }
    if (TopBar_p->state == 2) {
	NH_TTY_DIAGNOSTIC_END(nh_tty_drawTopBarProgramSuggestion(TopBar_p, Window_p, Glyphs_p, cols))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
}

static NH_TTY_RESULT nh_tty_drawTopBarMessage(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Window_p, nh_tty_Glyph *Glyphs_p, int cols)
{
NH_TTY_BEGIN()

    if (TopBar_p->Message.Text.length == 0) {
        nh_tty_Program *Program_p = nh_tty_getCurrentProgram(Window_p);
        if (!Program_p || !Program_p->Prototype_p) {NH_TTY_END(NH_TTY_SUCCESS)}
        if (Program_p->Prototype_p->Callbacks.drawTopbar_f) {
            NH_TTY_CHECK(Program_p->Prototype_p->Callbacks.drawTopbar_f(Program_p, Glyphs_p, cols))
        } else {
            nh_tty_ProgramPrototype *Prototype_p = Program_p->Prototype_p;
            for (int i = 0; i < cols && i < Program_p->Prototype_p->Name.length; ++i) {
                Glyphs_p[i] = nh_tty_getGlyphHelper(Program_p->Prototype_p->Name.p[i]);
            }
        }
    } else {
        for (int i = 0, j = 0; i < cols && j < TopBar_p->Message.Text.length; ++i, ++j) {
            Glyphs_p[i] = nh_tty_getGlyphHelper(TopBar_p->Message.Text.p[j]);
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_drawTopBarText(
     nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *MicroWindow_p, nh_tty_Glyph *Glyphs_p, int cols)
{
NH_TTY_BEGIN()

    if (!TopBar_p->hasFocus) 
    {
        nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

        NH_ENCODING_UTF32 t1_p[15] = {' ', '(', 'M', 'I', 'C', 'R', 'O', ' ', 'T', 'I', 'L', 'I', 'N', 'G', ')'};
        NH_ENCODING_UTF32 t2_p[15] = {' ', '(', 'M', 'A', 'C', 'R', 'O', ' ', 'T', 'I', 'L', 'I', 'N', 'G', ')'};

        nh_tty_MacroWindow *MacroWindow_p = TTY_p->Window_p;
        if (MacroWindow_p->Tiling.stage == NH_TTY_TILING_STAGE_DONE) {
            nh_tty_clearMessage(TopBar_p);
        }

        NH_TTY_CHECK(nh_tty_drawTopBarMessage(TopBar_p, MicroWindow_p, Glyphs_p, cols))

        nh_tty_TopBar *TopBarCompare_p = &NH_TTY_MACRO_TAB(MacroWindow_p->Tile_p)->TopBar;

        if (MacroWindow_p->Tiling.stage != NH_TTY_TILING_STAGE_DONE) {
            if (TopBarCompare_p != TopBar_p) {NH_TTY_END(NH_TTY_SUCCESS)}
            if (MacroWindow_p->Tiling.mode == NH_TTY_TILING_MODE_MICRO) {
                for (int i = cols-15, j = 0; j < 15; ++i, ++j) {
                    Glyphs_p[i].codepoint = t1_p[j];
                }
            } else if (MacroWindow_p->Tiling.mode == NH_TTY_TILING_MODE_MACRO) {
                for (int i = cols-15, j = 0; j < 15; ++i, ++j) {
                    Glyphs_p[i].codepoint = t2_p[j];
                }
            }
        }
        NH_TTY_END(NH_TTY_SUCCESS)
    }

    if (TopBar_p->Command.length > 0) {
        NH_TTY_CHECK(nh_tty_drawTopBarCommand(TopBar_p, &Glyphs_p, &cols))
    }

    NH_TTY_CHECK(nh_tty_drawTopBarSuggestion(TopBar_p, MicroWindow_p, Glyphs_p, cols))

    NH_ENCODING_UTF32 cmd_p[14]  = {' ', '(', 'E', 'X', 'E', 'C', 'U', 'T', 'E', ' ', 'C', 'M', 'D', ')'};
    NH_ENCODING_UTF32 prog_p[14] = {' ', '(', 'S', 'W', 'I', 'T', 'C', 'H', ' ', 'P', 'R', 'O', 'G', ')'};

    if (TopBar_p->state == 0 && cols > 14) {
        for (int i = cols-15, j = 0; j < 14; ++i, ++j) {
            Glyphs_p[i].codepoint = cmd_p[j];
        }
    }
    if (TopBar_p->state == 1 && cols > 14) {
        for (int i = cols-15, j = 0; j < 14; ++i, ++j) {
            Glyphs_p[i].codepoint = prog_p[j];
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_drawTopBarRow(
    nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *MicroWindow_p, nh_tty_Glyph *Glyphs_p, int cols, int row,
    NH_BOOL standardIO)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    for (int i = 0; i < cols; ++i) {
        Glyphs_p[i] = nh_tty_getGlyphHelper(' ');
        if (!TopBar_p && !MicroWindow_p && !standardIO) {
            Glyphs_p[i].codepoint = 'q';
            Glyphs_p[i].Attributes.reverse = NH_FALSE;
            Glyphs_p[i].mark |= NH_TTY_MARK_LINE_GRAPHICS;
        }
        Glyphs_p[i].mark |= NH_TTY_MARK_LINE_HORIZONTAL;
    }

    if (TopBar_p && MicroWindow_p) {
        NH_TTY_CHECK(nh_tty_drawTopBarText(TopBar_p, MicroWindow_p, Glyphs_p, cols))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// UPDATE ==========================================================================================

NH_TTY_RESULT nh_tty_updateTopBar(
    nh_tty_TopBar *TopBar_p)
{
NH_TTY_BEGIN()

    nh_SystemTime Now = nh_core_getSystemTime();

    if (nh_core_getSystemTimeDiffInSeconds(TopBar_p->Suggestions.Time, Now) 
	>= TopBar_p->Suggestions.interval) 
    {
        TopBar_p->Suggestions.Time = Now;
        TopBar_p->Suggestions.update = NH_TRUE;
        TopBar_p->refresh = NH_TRUE;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

