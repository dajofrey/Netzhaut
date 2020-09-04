// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Log.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Output.h"
#include "../../Core/Header/Config.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

void Nh_Gfx_logVulkan( // TODO multithreading
    char *message_p)
{
NH_BEGIN()

    if (Nh_getConfig()->Flags.Log.vulkan) {Nh_addOutput(NH_OUTPUT_CHARS, message_p, NH_FALSE);}

NH_SILENT_END()
}

