// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Log.h"
#include "../Header/Mouse.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Config.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Output.h"

#include NH_DEBUG

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

void Nh_IO_logMouse( // TODO multithreading
    NH_IO_Input *Input_p)
{
NH_BEGIN()

    if (Nh_getConfig()->Flags.Log.mouse)
    {
        char *str_p = Nh_IO_dumpMouse(&Input_p->Mouse);
        if (str_p != NULL && Nh_addOutput(NH_OUTPUT_CHARS, str_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(str_p);
        }
    }

NH_SILENT_END()
}

