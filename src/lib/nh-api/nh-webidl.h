#ifndef NH_API_NH_WEBIDL_H
#define NH_API_NH_WEBIDL_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-core.h"
#include "nh-encoding.h"

typedef struct nh_api_HostGlobal {
    char *specification_p;
    char *interface_p;
} nh_api_HostGlobal;

typedef struct nh_api_HostGlobals {
    nh_api_HostGlobal *p;
    unsigned int size;
} nh_api_HostGlobals;

nh_api_HostGlobals nh_api_getHostGlobals(
);

nh_api_HostGlobals nh_api_getHostGlobalsForHTML(
);

#endif // NH_WEBIDL_H
