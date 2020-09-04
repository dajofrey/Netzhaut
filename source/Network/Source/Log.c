// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Log.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Config.h"
#include "../../Core/Header/String.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Output.h"

#include NH_DEBUG

#include <string.h>
#include <stdio.h>

// LOG =============================================================================================

void Nh_Net_logReceive(
    char *data_p)
{
NH_BEGIN()

    if (Nh_getConfig()->Flags.Log.receive == NH_TRUE)
    {
        Nh_String *String_p = Nh_allocateString(NULL);
        if (Nh_appendFormatToString(String_p, "%s\n", data_p) != NH_SUCCESS) {NH_SILENT_END()}

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);

        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);
        }
    }

NH_SILENT_END()
}

