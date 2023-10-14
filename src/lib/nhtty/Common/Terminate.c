// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "Macros.h"
#include "IndexMap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_TTY_RESULT nh_tty_terminate()
{
NH_TTY_BEGIN()

    nh_tty_freeIndexMap();

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

