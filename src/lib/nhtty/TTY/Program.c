// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Program.h"
#include "TTY.h"

#include "../Editor/Editor.h"
#include "../Shell/Shell.h"
#include "../Logger/Logger.h"
#include "../Common/Macros.h"

#include "../../nhcore/Loader/Loader.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhdocs/Tagger/Tagger.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// PROGRAM PROTOTYPE  ==============================================================================

nh_tty_ProgramPrototype *nh_tty_createProgramPrototype(
    NH_TTY_PROGRAM_E type)
{
NH_TTY_BEGIN()

    nh_tty_ProgramPrototype *Prototype_p = NULL;

    switch (type)
    {
        case NH_TTY_PROGRAM_EDITOR :
            Prototype_p = nh_tty_createEditorPrototype();
            break;
        case NH_TTY_PROGRAM_LOGGER :
            Prototype_p = nh_tty_createLoggerPrototype();
            break;
        case NH_TTY_PROGRAM_SHELL :
            Prototype_p = nh_tty_createShellPrototype();
            break;
        case NH_TTY_PROGRAM_TAGGER :
            NH_LOADER.load_f(NH_MODULE_DOCS, 0);
            nh_docs_createTaggerPrototype_f createTaggerPrototype_f = 
                nh_core_loadExistingSymbol(NH_MODULE_DOCS, 0, "nh_docs_createTaggerPrototype");
            NH_TTY_CHECK_NULL_2(NULL, createTaggerPrototype_f)
            Prototype_p = createTaggerPrototype_f();
            break;
    }

NH_TTY_END(Prototype_p)
}

// PROGRAM INSTANCE ================================================================================

nh_tty_Program *nh_tty_createProgramInstance(
    nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once)
{
NH_TTY_BEGIN()

    nh_tty_Program *Program_p = nh_core_allocate(sizeof(nh_tty_Program));
    NH_TTY_CHECK_MEM_2(NULL, Program_p)

    Program_p->once        = once;
    Program_p->refresh     = NH_FALSE;
    Program_p->close       = NH_FALSE;
    Program_p->Prototype_p = Prototype_p;
    Program_p->handle_p    = Prototype_p->Callbacks.init_f ? 
        Prototype_p->Callbacks.init_f(Prototype_p->initArg_p) : NULL; 
 
NH_TTY_END(Program_p)
}

NH_TTY_RESULT nh_tty_destroyProgramInstance(
    nh_tty_Program *Program_p)
{
NH_TTY_BEGIN()

    if (Program_p->once) {
        Program_p->Prototype_p->Callbacks.destroyPrototype_f(Program_p->Prototype_p);
    }

    if (Program_p && Program_p->Prototype_p && Program_p->Prototype_p->Callbacks.destroy_f) {
        Program_p->Prototype_p->Callbacks.destroy_f(Program_p->handle_p);
    }

    nh_core_free(Program_p);
 
NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// INTERFACE =======================================================================================

NH_TTY_RESULT nh_tty_addProgram(
    nh_tty_TTY *TTY_p, nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once)
{
NH_TTY_BEGIN()

//    if (once) {
//        nh_tty_MacroTile *Tile_p = TTY_p->Window_p->Tile_p;
//
//        // Remove program with same name on tile if necessary.
//        for (int i = 0; Tile_p->Canvas.Programs_p && i < Tile_p->Canvas.Programs_p->length; ++i) {
//            nh_tty_Program *Program_p = ((nh_tty_Program*)Tile_p->Canvas.Programs_p->p)+i;
//            if (nh_encoding_compareUTF32(Program_p->Prototype_p->Name.p, Prototype_p->Name.p)) {
//                NH_TTY_CHECK(nh_tty_destroyProgramInstance(((nh_tty_Program*)Tile_p->Canvas.Programs_p->p)+i))
//                nh_core_removeFromArray(Tile_p->Canvas.Programs_p, i, 1);
//                if (Program_p->once) {
//                    Program_p->Prototype_p->Callbacks.destroyPrototype_f(Program_p->Prototype_p);
//                }
//            }
//        }
//
//        // Add canvas tile to tile.
//        NH_TTY_CHECK(nh_tty_appendCanvasTile(&Tile_p->Canvas, Prototype_p, &TTY_p->Views, once))
//
//        // Switch to new program.
//        Tile_p->Canvas.currentProgram = Tile_p->Canvas.Programs_p->length-1; 
//
//        // Prevent black screen.
//        TTY_p->Window_p->refreshScreen = NH_TRUE;
//
//        NH_TTY_END(NH_TTY_SUCCESS)
//    }

    // Check if there already exists a program with this name.
    for (int i = 0; i < TTY_p->Prototypes.size; ++i) {
        if (nh_encoding_compareUTF32(((nh_tty_ProgramPrototype*)TTY_p->Prototypes.pp[i])->Name.p, Prototype_p->Name.p)) {
            NH_TTY_END(NH_TTY_ERROR_BAD_STATE)
        }
    }

    // Add program prototype to other program prototypes.
    nh_core_appendToList(&TTY_p->Prototypes, Prototype_p);

    // If necessary, Add program prototype instance to all current tiles.
    nh_List Tiles = nh_tty_getTiles(TTY_p->Window_p->RootTile_p);
    for (int i = 0; i < Tiles.size; ++i) {
        nh_tty_Tile *Tile_p = Tiles.pp[i];
        NH_BOOL add = NH_TRUE;
        for (int j = 0; NH_TTY_MACRO_TAB(Tile_p)->MicroWindow.Tabs_p && j < NH_TTY_MACRO_TAB(Tile_p)->MicroWindow.Tabs_p->size; ++j) {
            nh_tty_ProgramPrototype *MicroTileProgramPrototype_p = 
                ((nh_tty_MicroTab*)NH_TTY_MACRO_TAB(Tile_p)->MicroWindow.Tabs_p->pp[j])->Prototype_p;
            if (!MicroTileProgramPrototype_p) {continue;}
            if (nh_encoding_compareUTF32(MicroTileProgramPrototype_p->Name.p, Prototype_p->Name.p)) {
                add = NH_FALSE;
            }
        }
        if (!add) {continue;}
        for (int j = 0; j < 9; ++j) {
            NH_TTY_CHECK(nh_tty_appendMicroTab(
                &NH_TTY_MACRO_TAB_2(Tile_p, j)->MicroWindow, TTY_p->Prototypes.pp[TTY_p->Prototypes.size - 1], once
            ))
        }
    }
    nh_core_freeList(&Tiles, NH_FALSE);

    // Prevent black screen. 
    TTY_p->Window_p->refreshGrid1 = NH_TRUE;

NH_TTY_END(NH_TTY_SUCCESS)
}

