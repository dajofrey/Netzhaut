// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-monitor.h"
#include "nh-core.h"

#include "../nh-core/Loader/Loader.h"

// TYPEDEFS ========================================================================================

typedef void *(*nh_monitor_createMonitorInterface_f)(
);
typedef void (*nh_monitor_freeMonitorInterface_f)(
    void *Interface_p
);

// FUNCTIONS =======================================================================================

void *nh_api_createMonitorInterface()
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_monitor_createMonitorInterface_f createMonitorInterface_f = !Loader_P ? NULL : Loader_P->loadSymbol_f(NH_MODULE_MONITOR, 0, "nh_monitor_createMonitorInterface");
    return createMonitorInterface_f ? createMonitorInterface_f() : NULL;
}

void nh_api_freeMonitorInterface(
    void *Interface_p)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_monitor_freeMonitorInterface_f freeMonitorInterface_f = !Loader_P ? NULL : Loader_P->loadSymbol_f(NH_MODULE_MONITOR, 0, "nh_monitor_freeMonitorInterface");
    freeMonitorInterface_f(Interface_p);
}
