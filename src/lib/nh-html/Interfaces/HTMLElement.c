// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTMLElement.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// API =============================================================================================

void nh_html_setToken(
    void *HTMLElement_p, nh_html_Token *Token_p)
{
    ((nh_webidl_Object*)HTMLElement_p)->internal_p = Token_p;
}

nh_html_Token *nh_html_getToken(
    void *HTMLElement_p)
{
    return ((nh_webidl_Object*)HTMLElement_p)->internal_p;
}
