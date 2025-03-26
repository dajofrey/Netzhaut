#ifndef NH_API_NH_MONITOR_H 
#define NH_API_NH_MONITOR_H 

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// FUNCTIONS =======================================================================================

void *nh_api_createMonitorInterface(
);

void nh_api_freeMonitorInterface(
    void *Interface_p
);

#endif // NH_API_NH_MONITOR_H
