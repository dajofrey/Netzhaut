// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminal.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Thread.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Process.h"
#include "../../nhcore/Util/RingBuffer.h"
#include "../../nhcore/Util/List.h"
#include "../../nhcore/Config/Config.h"

#include "../../nhencoding/Encodings/UTF8.h"
#include "../../nhencoding/Encodings/UTF32.h"

#include "../../nhgfx/Base/Viewport.h"
#include "../../nhgfx/Fonts/FontManager.h"
#include "../../nhgfx/Fonts/Text.h"

#include "../../nhwsi/Window/Window.h"
#include "../../nhwsi/Window/WindowSettings.h"

#include <string.h>
#include <stdio.h>

// INIT/FREE =======================================================================================
// The next functions comprise the in/exit points of nhterminal.

typedef struct nh_terminal_Args {
    nh_tty_TTY *TTY_p;
    NH_BYTE *namespace_p;
} nh_terminal_Args;

static void *nh_terminal_initTerminal(
    nh_core_Workload *Workload_p)
{
NH_TERMINAL_BEGIN()

    static NH_BYTE *name_p = "Terminal Emulator";
    static NH_BYTE *path_p = "nhterminal/Terminal/Terminal.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_TERMINAL;

    nh_terminal_Terminal *Terminal_p = nh_core_allocate(sizeof(nh_terminal_Terminal));
    NH_TERMINAL_CHECK_MEM_2(NULL, Terminal_p)

    memset(Terminal_p, 0, sizeof(nh_terminal_Terminal));

    Terminal_p->ctrl = NH_FALSE;
    Terminal_p->leftMouse = NH_FALSE;
    Terminal_p->TTY_p = ((nh_terminal_Args*)Workload_p->args_p)->TTY_p;
    Terminal_p->View_p = nh_tty_createView(Terminal_p->TTY_p, Terminal_p, NH_FALSE);
    NH_TERMINAL_CHECK_MEM_2(NULL, Terminal_p->View_p)

    if (((nh_terminal_Args*)Workload_p->args_p)->namespace_p) {
        strcpy(Terminal_p->namespace_p, ((nh_terminal_Args*)Workload_p->args_p)->namespace_p);
    } else {
        sprintf(Terminal_p->namespace_p, "%p", Terminal_p);
    }

    NH_TERMINAL_CHECK_2(NULL, nh_terminal_initGraphics(&Terminal_p->Graphics))
 
    NH_TERMINAL_CHECK_2(NULL, nh_terminal_initGrid(&Terminal_p->Grid))
    NH_TERMINAL_CHECK_2(NULL, nh_terminal_initGrid(&Terminal_p->Grid2))

NH_TERMINAL_END(Terminal_p)
}

static void nh_terminal_freeTerminal(
    void *terminal_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Terminal *Terminal_p = terminal_p;

    nh_terminal_freeGraphics(&Terminal_p->Graphics);
    nh_terminal_freeGrid(&Terminal_p->Grid);
    nh_terminal_freeGrid(&Terminal_p->Grid2);

    nh_core_free(Terminal_p);

NH_TERMINAL_SILENT_END()
}

// RUN LOOP ========================================================================================
// The next functions comprise the top-level of the nhterminal run loop.

static NH_TERMINAL_RESULT nh_terminal_updateSize(
    nh_terminal_Terminal *Terminal_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Config Config = nh_terminal_getConfig();
 
    nh_gfx_FontInstance *FontInstance_p = nh_gfx_claimFontInstance(
        Terminal_p->Graphics.State.Fonts.pp[Terminal_p->Graphics.State.font], 
        Config.fontSize
    );

    nh_gfx_Text Text;
    NH_ENCODING_UTF32 c = 'e';

    NH_GFX_RESULT failure = 1;
    for (int i = 0; i < Terminal_p->Graphics.State.Fonts.size; ++i) {
        failure = nh_gfx_createTextFromFont(&Text, &c, 1, Config.fontSize,
            Terminal_p->Graphics.State.Fonts.pp[i]);
        if (!failure) {
            Terminal_p->Graphics.State.font = i;
            break;
        }
    }

    if (failure) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}
    
    NH_TERMINAL_CHECK(nh_terminal_updateGrid(&Terminal_p->Grid, &Terminal_p->Graphics.State, &Text))
    NH_TERMINAL_CHECK(nh_terminal_updateGrid(&Terminal_p->Grid2, &Terminal_p->Graphics.State, &Text))

    NH_TERMINAL_CHECK(nh_terminal_updateGraphicsData(&Terminal_p->Graphics.Data1, &Terminal_p->Grid))
    NH_TERMINAL_CHECK(nh_terminal_updateGraphicsData(&Terminal_p->Graphics.Data2, &Terminal_p->Grid2))

    // Update view size, subtract gap tiles.
    Terminal_p->View_p->cols = Terminal_p->Grid.cols-1;
    Terminal_p->View_p->rows = Terminal_p->Grid.rows-1;
    Terminal_p->View_p->Size = Terminal_p->Grid.Size;

    NH_TERMINAL_CHECK(nh_terminal_updateGraphicsData(&Terminal_p->Graphics.Data1, &Terminal_p->Grid))
    NH_TERMINAL_CHECK(nh_terminal_updateGraphicsData(&Terminal_p->Graphics.Data2, &Terminal_p->Grid2))

    nh_gfx_freeText(&Text);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_updateSizeIfRequired(
    nh_terminal_Terminal *Terminal_p, NH_BOOL *update_p)
{
NH_TERMINAL_BEGIN()

    if (Terminal_p->Graphics.State.Viewport_p->Settings.Size.width == Terminal_p->Grid.Size.width
    &&  Terminal_p->Graphics.State.Viewport_p->Settings.Size.height == Terminal_p->Grid.Size.height) {
        NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
    }

    NH_TERMINAL_CHECK(nh_terminal_updateSize(Terminal_p))

    *update_p = NH_TRUE;

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_handleEvent(
    nh_terminal_Terminal *Terminal_p, nh_wsi_Event *Event_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Config Config = nh_terminal_getConfig();
 
    if (Event_p->type == NH_WSI_EVENT_MOUSE) {
        if (Event_p->Mouse.type == NH_WSI_MOUSE_SCROLL && Terminal_p->ctrl) {
            int newFontSize = Config.fontSize + (Event_p->Mouse.trigger == NH_WSI_TRIGGER_UP ? 1 : -1);

            if (newFontSize < 10 || newFontSize > 60) {
                // Out of bounds.
                NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
            }

            nh_core_overwriteGlobalConfigSettingInt(
                Terminal_p->namespace_p, NH_MODULE_TERMINAL, nh_terminal_getSettingName(NH_TERMINAL_SETTING_FONT_SIZE), newFontSize);

            nh_gfx_unclaimFontInstance(nh_gfx_claimFontInstance(
                Terminal_p->Graphics.State.Fonts.pp[Terminal_p->Graphics.State.font], 
                newFontSize));

            NH_TERMINAL_CHECK(nh_terminal_updateSize(Terminal_p))
        }
        if (Event_p->Mouse.type == NH_WSI_MOUSE_LEFT && Event_p->Mouse.trigger == NH_WSI_TRIGGER_PRESS) {
            Terminal_p->leftMouse = NH_TRUE;
            nh_SystemTime Now = nh_core_getSystemTime();
            if (nh_core_getSystemTimeDiffInSeconds(Terminal_p->LastClick, Now) <= 0.3f) {
                nh_wsi_toggleWindowSize_f toggleWindowSize_f = nh_core_loadExistingSymbol(NH_MODULE_WSI, 0, "nh_wsi_toggleWindowSize");
                if (toggleWindowSize_f && Event_p->Mouse.Position.y < Terminal_p->Grid.TileSize.height) {
                    toggleWindowSize_f(Terminal_p->Graphics.State.Viewport_p->Surface_p->Window_p);
                } 
            }
            Terminal_p->LastClick = Now;
        } 
        if (Event_p->Mouse.type == NH_WSI_MOUSE_LEFT && Event_p->Mouse.trigger == NH_WSI_TRIGGER_RELEASE) {
            Terminal_p->leftMouse = NH_FALSE;
        } 
        if (Event_p->Mouse.type == NH_WSI_MOUSE_MOVE && Terminal_p->leftMouse) {
            nh_wsi_moveWindow_f moveWindow_f = nh_core_loadExistingSymbol(NH_MODULE_WSI, 0, "nh_wsi_moveWindow");
            if (moveWindow_f && Event_p->Mouse.Position.y < Terminal_p->Grid.TileSize.height) {
                moveWindow_f(Terminal_p->Graphics.State.Viewport_p->Surface_p->Window_p);
                if (Terminal_p->Graphics.State.Viewport_p->Surface_p->Window_p->type == NH_WSI_TYPE_X11) {
                    Terminal_p->leftMouse = NH_FALSE;
                }
            } 
        }
    }

    if (Event_p->type == NH_WSI_EVENT_KEYBOARD) {
        if (Event_p->Keyboard.special == NH_WSI_KEY_CONTROL_L || Event_p->Keyboard.special == NH_WSI_KEY_CONTROL_R) {
            if (Event_p->Keyboard.trigger == NH_WSI_TRIGGER_PRESS) {
                Terminal_p->ctrl = NH_TRUE;
            }
            if (Event_p->Keyboard.trigger == NH_WSI_TRIGGER_RELEASE) {
                Terminal_p->ctrl = NH_FALSE;
            }
        }
    }

    if (Event_p->type == NH_WSI_EVENT_WINDOW) {
        Terminal_p->leftMouse = NH_FALSE;
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_handleInputIfRequired(
    nh_terminal_Terminal *Terminal_p, NH_BOOL *update_p)
{
NH_TERMINAL_BEGIN()

    nh_Array *Array_p = NULL;
    nh_wsi_Event *Event_p = NULL;

    for (int row = 0; row < Terminal_p->Grid.rows; ++row) {
        memset(Terminal_p->Grid.updates_pp[row], NH_FALSE, Terminal_p->Grid.cols*sizeof(NH_BOOL));
    }
    for (int row = 0; row < Terminal_p->Grid2.rows; ++row) {
        memset(Terminal_p->Grid2.updates_pp[row], NH_FALSE, Terminal_p->Grid2.cols*sizeof(NH_BOOL));
    }

    do {
        Array_p = nh_core_incrementRingBufferMarker(
            &Terminal_p->View_p->Forward.Tiles, &Terminal_p->View_p->Forward.Tiles.Marker);

        if (!Array_p) {break;}

        for (int i = 0; i < Array_p->length; ++i) {
            nh_terminal_TileUpdate *Update_p = ((nh_terminal_TileUpdate*)Array_p->p)+i;
            if (Update_p->row >= Terminal_p->Grid.rows || Update_p->col >= Terminal_p->Grid.cols) {
                // Ignore invalid update.
                continue;
            }
            if (Update_p->cursor) {
                NH_TERMINAL_CHECK(nh_terminal_updateTile(
                    &Terminal_p->Grid, &Terminal_p->Graphics.State, Update_p, update_p))
            } else if (Update_p->Glyph.mark & NH_TTY_MARK_ELEVATED) {
                Terminal_p->Grid2.Updates_pp[Update_p->row][Update_p->col] = *Update_p;
                Terminal_p->Grid2.updates_pp[Update_p->row][Update_p->col] = NH_TRUE;
            } else {
                Terminal_p->Grid.Updates_pp[Update_p->row][Update_p->col] = *Update_p;
                Terminal_p->Grid.updates_pp[Update_p->row][Update_p->col] = NH_TRUE;
            }
        }

    } while (Array_p);

    // Update tiles.
    for (int row = 0; row < Terminal_p->Grid.rows; ++row) {
        for (int col = 0; col < Terminal_p->Grid.cols; ++col) {
            if (Terminal_p->Grid.updates_pp[row][col] == NH_FALSE) {continue;}
            NH_TERMINAL_CHECK(nh_terminal_updateTile(
                &Terminal_p->Grid, &Terminal_p->Graphics.State, &Terminal_p->Grid.Updates_pp[row][col], update_p))
        }
    }
    for (int row = 0; row < Terminal_p->Grid2.rows; ++row) {
        for (int col = 0; col < Terminal_p->Grid2.cols; ++col) {
            if (Terminal_p->Grid2.updates_pp[row][col] == NH_FALSE) {continue;}
            NH_TERMINAL_CHECK(nh_terminal_updateTile(
                &Terminal_p->Grid2, &Terminal_p->Graphics.State, &Terminal_p->Grid2.Updates_pp[row][col], update_p))
        }
    }

    do {
        Array_p = nh_core_incrementRingBufferMarker(
            &Terminal_p->View_p->Forward.Boxes, &Terminal_p->View_p->Forward.Boxes.Marker);

        if (!Array_p) {break;}

        NH_TERMINAL_CHECK(nh_terminal_updateBoxes(&Terminal_p->Grid, &Terminal_p->Graphics.State, Array_p))

    } while (Array_p);

    do {
        Event_p = nh_core_incrementRingBufferMarker(
            &Terminal_p->View_p->Forward.Events, &Terminal_p->View_p->Forward.Events.Marker);

        if (!Event_p) {break;}

        NH_TERMINAL_CHECK(nh_terminal_handleEvent(Terminal_p, Event_p))

    } while (Event_p);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_SIGNAL nh_terminal_runTerminal(
    void *args_p) 
{
NH_TERMINAL_BEGIN()

    NH_TERMINAL_CHECK_NULL_2(NH_SIGNAL_ERROR, args_p)
    nh_terminal_Terminal *Terminal_p = args_p;

    if (!Terminal_p->Graphics.State.Viewport_p) {NH_TERMINAL_END(NH_SIGNAL_IDLE)}

    NH_BOOL update = NH_FALSE;
 
    NH_TERMINAL_CHECK_2(NH_SIGNAL_ERROR, nh_terminal_updateSizeIfRequired(Terminal_p, &update))
    NH_TERMINAL_CHECK_2(NH_SIGNAL_ERROR, nh_terminal_handleInputIfRequired(Terminal_p, &update))
    if (nh_terminal_updateBlinkOrGradient(&Terminal_p->Graphics.State)) {
        update = NH_TRUE;
    }

    if (update) {
        NH_TERMINAL_CHECK_2(NH_SIGNAL_ERROR, nh_terminal_updateGraphicsData(&Terminal_p->Graphics.Data1,
            &Terminal_p->Grid))
        NH_TERMINAL_CHECK_2(NH_SIGNAL_ERROR, nh_terminal_updateGraphicsData(&Terminal_p->Graphics.Data2,
            &Terminal_p->Grid2))
        NH_TERMINAL_CHECK_2(NH_SIGNAL_ERROR, nh_terminal_renderGraphics(&Terminal_p->Graphics, 
            &Terminal_p->Grid, &Terminal_p->Grid2))
        NH_TERMINAL_END(NH_SIGNAL_OK)
    }

NH_TERMINAL_END(update == NH_TRUE ? NH_SIGNAL_OK : NH_SIGNAL_IDLE)
}

// COMMANDS ========================================================================================
// The next functions are executed by nh_terminal_cmd_* functions.

typedef enum NH_TERMINAL_COMMAND_E {
    NH_TERMINAL_COMMAND_SET_VIEWPORT,
} NH_TERMINAL_COMMAND_E;

/**
 * Here, most commands that came in through the API are handled.
 */
static NH_SIGNAL nh_terminal_runTerminalCommand(
    void *terminal_p, nh_core_WorkloadCommand *Command_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Terminal *Terminal_p = terminal_p;

    switch (Command_p->type)
    {
        case NH_TERMINAL_COMMAND_SET_VIEWPORT :
            if (nh_gfx_claimViewport(Command_p->p, NH_GFX_VIEWPORT_OWNER_TERMINAL, Terminal_p) != NH_GFX_SUCCESS) {
                NH_TERMINAL_END(NH_SIGNAL_ERROR)
            }
            nh_terminal_handleViewportChange(&Terminal_p->Graphics, Command_p->p);
            nh_terminal_handleViewportChange(&Terminal_p->Graphics2, Command_p->p);
            break;
    }

NH_TERMINAL_END(NH_SIGNAL_OK)
}

// API =============================================================================================
// The next functions are called by lib/netzhaut/nhterminal.h functions.

nh_terminal_Terminal *nh_terminal_openTerminal(
    NH_BYTE *namespace_p, nh_tty_TTY *TTY_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Args Args;
    Args.TTY_p = TTY_p;
    Args.namespace_p = namespace_p;

    nh_terminal_Terminal *Terminal_p = nh_core_activateWorkload(
        nh_terminal_initTerminal, nh_terminal_runTerminal, nh_terminal_freeTerminal,
        nh_terminal_runTerminalCommand, &Args, NH_TRUE);

NH_TERMINAL_END(Terminal_p)
}

NH_TERMINAL_RESULT nh_terminal_cmd_setViewport(
    nh_terminal_Terminal *Terminal_p, nh_gfx_Viewport *Viewport_p)
{
NH_TERMINAL_BEGIN()

    nh_core_executeWorkloadCommand(Terminal_p, NH_TERMINAL_COMMAND_SET_VIEWPORT, Viewport_p, 0);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

