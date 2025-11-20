#ifndef NH_WEBIDL_HOST_GLOBALS_H
#define NH_WEBIDL_HOST_GLOBALS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

void nh_webidl_initHostGlobals(
);

nh_api_HostGlobals nh_webidl_getHostGlobals(
);

nh_api_HostGlobals nh_webidl_getHostGlobalsForHTML(
);

#endif
