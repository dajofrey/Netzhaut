// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "GUI.h"
#include "X11.h"

#include "../Common/Macro.h"
#include "../Common/API.h"
#include NH_MAKE_FLOW
#include NH_MAKE_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// DATA ============================================================================================

NH_MAKE_BOOL NH_MAKE_GUI = NH_MAKE_FALSE;
static NH_MAKE_BOOL run = NH_MAKE_TRUE;

// BUILD ===========================================================================================

NH_MAKE_RESULT nh_make_runGUI()
{
NH_MAKE_BEGIN()

    nh_make_X11_createWindow("netzhaut-Installer", 300, 300);
    while (run) {nh_make_X11_getInput();}
    
NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_handleGUIExpose()
{
NH_MAKE_BEGIN()

    nh_make_X11_drawTextLine(5, 5, "Building via GUI is planned, but not implemented yet. Please use the command-line for now.");

NH_MAKE_END(NH_MAKE_SUCCESS)
}

void nh_make_handleGUIExit()
{
NH_MAKE_BEGIN()

    run = NH_MAKE_FALSE;

NH_MAKE_SILENT_END()
}

