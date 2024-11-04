#ifndef NH_WSI_COMMON_LOG_H
#define NH_WSI_COMMON_LOG_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../Window/Window.h"

void nh_wsi_logEvent(
    nh_wsi_Window *Window_p, nh_api_WSIEvent *Event_p
);

#endif // NH_WSI_COMMON_LOG_H
