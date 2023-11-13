// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// INCLUDES ========================================================================================

#include "TTY.h"
#include "View.h"
#include "StandardIO.h"
#include "Draw.h"
#include "Macro.h"
#include "ContextMenu.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Thread.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Process.h"
#include "../../nhcore/System/Logger.h"
#include "../../nhcore/Util/RingBuffer.h"

#include "../../nhencoding/Base/UnicodeDataHelper.h"
#include "../../nhencoding/Encodings/UTF8.h"
#include "../../nhencoding/Encodings/UTF32.h"

#include <string.h>
#include <stdio.h>

// CLIPBOARD =======================================================================================

NH_TTY_RESULT nh_tty_resetClipboard()
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    for (int i = 0; i < TTY_p->Clipboard.Lines.length; ++i) {
        nh_encoding_UTF32String *String_p = 
            &((nh_encoding_UTF32String*)TTY_p->Clipboard.Lines.p)[i];
        nh_encoding_freeUTF32(String_p);
    }

    nh_core_freeArray(&TTY_p->Clipboard.Lines);
    TTY_p->Clipboard.Lines = nh_core_initArray(sizeof(nh_encoding_UTF32String), 32);

NH_TTY_END(NH_TTY_SUCCESS)
}

nh_encoding_UTF32String *nh_tty_newClipboardLine()
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    nh_encoding_UTF32String *Line_p = nh_core_incrementArray(&TTY_p->Clipboard.Lines);

    NH_TTY_CHECK_NULL_2(NULL, Line_p)
    *Line_p = nh_encoding_initUTF32(32);

NH_TTY_END(Line_p)
}

nh_tty_Clipboard *nh_tty_getClipboard()
{
NH_TTY_BEGIN()
NH_TTY_END(&((nh_tty_TTY*)nh_core_getWorkloadArg())->Clipboard)
}

// INIT/FREE =======================================================================================
// The next functions comprise the in/exit points of nhtty.

static void *nh_tty_initTTY(
    nh_core_Workload *Workload_p)
{
NH_TTY_BEGIN()

    static NH_BYTE *path_p = "nhtty/TTY/TTY.c";
    static NH_BYTE *name_p = "TTY Workload";
    Workload_p->path_p = path_p;
    Workload_p->name_p = name_p;
    Workload_p->module = NH_MODULE_TTY;

    nh_tty_TTY *TTY_p = nh_core_allocate(sizeof(nh_tty_TTY));
    NH_TTY_CHECK_MEM_2(NULL, TTY_p)

    memset(TTY_p, 0, sizeof(nh_tty_TTY));

    if (Workload_p->args_p) {
        strcpy(TTY_p->namespace_p, Workload_p->args_p);
    } else {
        sprintf(TTY_p->namespace_p, "%p", TTY_p);
    }

    TTY_p->Windows = nh_core_initList(10);
    TTY_p->Views = nh_core_initList(8);
    TTY_p->Prototypes = nh_core_initList(8);
    TTY_p->Clipboard.Lines = nh_core_initArray(sizeof(nh_encoding_UTF32String), 32);

    TTY_p->hasFocus = NH_TRUE;
    TTY_p->Borders.on = NH_TRUE;
    TTY_p->TopBars.on = NH_TRUE;

    TTY_p->Preview.blink = NH_TRUE;
    TTY_p->Preview.blinkFrequency = 0.5;
    TTY_p->Preview.LastBlink = nh_core_getSystemTime();

    NH_TTY_CHECK_2(NULL, nh_core_initRingBuffer(
        &TTY_p->Events, 128, sizeof(nh_wsi_Event), NULL 
    ))

    NH_TTY_CHECK_NULL_2(NULL, nh_tty_insertAndFocusWindow(TTY_p, 0))

NH_TTY_END(TTY_p)
}

static void nh_tty_freeTTY(
    void *p)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = p;

    nh_tty_destroyWindows(TTY_p);
 
    nh_core_freeArray(&(TTY_p->Clipboard.Lines));
    nh_core_freeRingBuffer(&(TTY_p->Events));

    if (nh_tty_claimsStandardIO(TTY_p)) {
        nh_tty_unclaimStandardIO(TTY_p);
    }

    for (int i = 0; i < TTY_p->Prototypes.size; ++i) {
        ((nh_tty_ProgramPrototype*)TTY_p->Prototypes.pp[i])
            ->Callbacks.destroyPrototype_f(TTY_p->Prototypes.pp[i]);
    }
    nh_core_freeList(&TTY_p->Prototypes, NH_FALSE);

    for (int i = 0; i < TTY_p->Views.size; ++i) {
        nh_tty_destroyView(TTY_p, TTY_p->Views.pp[i]);
    }
    nh_core_freeList(&TTY_p->Views, NH_FALSE);

    nh_core_free(TTY_p);

NH_TTY_SILENT_END()
}

// RUN LOOP ========================================================================================
// The next functions comprise the top-level of the nhtty run loop.

static NH_TTY_RESULT nh_tty_handleInput(
    nh_tty_TTY *TTY_p)
{
NH_TTY_BEGIN()

    if (nh_tty_claimsStandardIO(TTY_p)) {
        NH_TTY_CHECK(nh_tty_readStandardInput(TTY_p))
    }

    while (1)
    {
        nh_wsi_Event *Event_p =
            nh_core_incrementRingBufferMarker(&TTY_p->Events, &TTY_p->Events.Marker);

        if (Event_p == NULL) {break;}

        NH_TTY_CHECK(nh_tty_handleMacroWindowInput(TTY_p->Window_p, *Event_p))

        for (int i = 0; i < TTY_p->Views.size; ++i) {
            NH_TTY_CHECK(nh_tty_forwardEvent(TTY_p->Views.pp[i], *Event_p))
        }
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_handleWindowResize(
    nh_tty_TTY *TTY_p, nh_tty_View *View_p)
{
NH_TTY_BEGIN()

    NH_BOOL updated = NH_FALSE;

    NH_TTY_CHECK(nh_tty_getViewSize(View_p))
    NH_TTY_CHECK(nh_tty_updateView(View_p, &updated))

    if (updated) {
        if (TTY_p->Window_p->MouseMenu_p) {
            TTY_p->Window_p->MouseMenu_p = NULL;
        }
	TTY_p->Window_p->refreshGrid1 = NH_TRUE;
	TTY_p->Window_p->refreshGrid2 = NH_TRUE;
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

static NH_SIGNAL nh_tty_runTTY(
    void *tty_p)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = tty_p;
    NH_BOOL idle = NH_TRUE;

    for (int i = 0; i < TTY_p->Views.size; ++i) {
        NH_TTY_CHECK_2(NH_SIGNAL_ERROR, nh_tty_handleWindowResize(TTY_p, TTY_p->Views.pp[i]))
    }

    NH_TTY_CHECK_2(NH_SIGNAL_ERROR, nh_tty_handleInput(TTY_p))
    NH_TTY_CHECK_2(NH_SIGNAL_ERROR, nh_tty_updateMacroWindow(TTY_p->Window_p))

    if (TTY_p->Window_p->refreshGrid1) {
        NH_TTY_CHECK_2(NH_SIGNAL_ERROR, nh_tty_refreshGrid1(TTY_p))
    }
    if (TTY_p->Window_p->refreshGrid2) {
        NH_TTY_CHECK_2(NH_SIGNAL_ERROR, nh_tty_refreshGrid2(TTY_p))
    }

    if (TTY_p->Window_p->refreshCursor || TTY_p->Window_p->refreshGrid1) {
        NH_TTY_CHECK_2(NH_SIGNAL_ERROR, nh_tty_refreshCursor(TTY_p))
        idle = NH_FALSE;
    }

    TTY_p->Window_p->refreshGrid1 = NH_FALSE;
    TTY_p->Window_p->refreshGrid2 = NH_FALSE;
    TTY_p->Window_p->refreshCursor = NH_FALSE;
 
    if (TTY_p->Window_p->close) {
        // First, close all dependent workloads.
        for (int i = 0; i < TTY_p->Views.size; ++i) {
            nh_tty_View *View_p = TTY_p->Views.pp[i];
            nh_core_Workload *Workload_p = nh_core_getWorkload(View_p->p);
            if (Workload_p) {
                nh_core_deactivateWorkload(Workload_p);
            }
        }
        NH_TTY_END(NH_SIGNAL_FINISH)
    }

NH_TTY_END(idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK)
}

// COMMANDS ========================================================================================
// The next functions are executed by nh_tty_cmd_* functions.

typedef enum NH_TTY_COMMAND_E {
    NH_TTY_COMMAND_ADD_PROGRAM,
    NH_TTY_COMMAND_CLAIM_STANDARD_IO,
    NH_TTY_COMMAND_UNCLAIM_STANDARD_IO,
    NH_TTY_COMMAND_SEND_EVENT,
} NH_TTY_COMMAND_E;

typedef struct nh_tty_AddProgramData {
    NH_TTY_PROGRAM_E type;
    NH_BOOL once;
} nh_tty_AddProgramData;

/**
 * Here, most commands that came in through the API are handled.
 */
static NH_SIGNAL nh_tty_runTTYCommand(
    void *tty_p, nh_core_WorkloadCommand *Command_p)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = tty_p;

    switch (Command_p->type)
    {
        case NH_TTY_COMMAND_ADD_PROGRAM :
            nh_tty_ProgramPrototype *Prototype_p = 
                nh_tty_createProgramPrototype(((nh_tty_AddProgramData*)Command_p->p)->type);
            NH_TTY_CHECK_NULL_2(NH_SIGNAL_ERROR, Prototype_p)
            NH_TTY_CHECK(nh_tty_addProgram(TTY_p, Prototype_p, ((nh_tty_AddProgramData*)Command_p->p)->once))
            break;
        case NH_TTY_COMMAND_CLAIM_STANDARD_IO :
            NH_TTY_CHECK(nh_tty_claimStandardIO(TTY_p))
            break;
        case NH_TTY_COMMAND_UNCLAIM_STANDARD_IO :
            NH_TTY_CHECK(nh_tty_unclaimStandardIO(TTY_p))
            break;
        case NH_TTY_COMMAND_SEND_EVENT :
            nh_wsi_Event *Event2_p = nh_core_advanceRingBuffer(&TTY_p->Events);
            NH_TTY_CHECK_NULL(Event2_p)
            *Event2_p = *((nh_wsi_Event*)Command_p->p);
            break;
    }

NH_TTY_END(NH_SIGNAL_OK)
}

// API =============================================================================================
// The next functions are called by lib/netzhaut/nhtty.h functions.

nh_tty_TTY *nh_tty_openTTY(
    NH_BYTE *config_p)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_activateWorkload(
        nh_tty_initTTY, nh_tty_runTTY, nh_tty_freeTTY, nh_tty_runTTYCommand, config_p, NH_TRUE
    );

NH_TTY_END(TTY_p)
}

NH_TTY_RESULT nh_tty_closeTTY(
    nh_tty_TTY *TTY_p)
{
NH_TTY_BEGIN()

    nh_core_deactivateWorkload(nh_core_getWorkload(TTY_p));

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_cmd_addProgram(
    nh_tty_TTY *TTY_p, NH_TTY_PROGRAM_E type, NH_BOOL once)
{
NH_TTY_BEGIN()

    nh_tty_AddProgramData Data = {0};
    Data.once = once;
    Data.type = type;

    nh_core_executeWorkloadCommand(TTY_p, NH_TTY_COMMAND_ADD_PROGRAM, &Data, sizeof(nh_tty_AddProgramData));

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_cmd_claimStandardIO(
    nh_tty_TTY *TTY_p)
{
NH_TTY_BEGIN()

    nh_core_executeWorkloadCommand(TTY_p, NH_TTY_COMMAND_CLAIM_STANDARD_IO, NULL, 0);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_cmd_unclaimStandardIO(
    nh_tty_TTY *TTY_p)
{
NH_TTY_BEGIN()

    nh_core_executeWorkloadCommand(TTY_p, NH_TTY_COMMAND_UNCLAIM_STANDARD_IO, NULL, 0);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_cmd_sendEvent(
    nh_tty_TTY *TTY_p, nh_wsi_Event Event)
{
NH_TTY_BEGIN()

    switch (Event.type) {
        case NH_WSI_EVENT_KEYBOARD :
        case NH_WSI_EVENT_MOUSE :
        case NH_WSI_EVENT_WINDOW :
            nh_core_executeWorkloadCommand(TTY_p, NH_TTY_COMMAND_SEND_EVENT, &Event, sizeof(nh_wsi_Event));
        default :
            // Ignore other events.
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

