// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-monitor.h"
#include "../nh-core/Loader/Loader.h"

// FUNCTIONS =======================================================================================

void *nh_api_createMonitorInterface()
{
    typedef void *(*nh_core_createMonitorInterface_f)();
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_core_createMonitorInterface_f createMonitorInterface_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_MONITOR, 0, "nh_monitor_createMonitorInterface");
    return createMonitorInterface_f ? createMonitorInterface_f() : NULL;
}

void nh_api_freeMonitorInterface(
    void *Interface_p)
{
    typedef void (*nh_core_freeMonitorInterface_f)(void *Interface_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_core_freeMonitorInterface_f freeMonitorInterface_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_MONITOR, 0, "nh_monitor_freeMonitorInterface");
    freeMonitorInterface_f(Interface_p);
}
