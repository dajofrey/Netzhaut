#ifndef NH_MONITOR_LOGGER_H
#define NH_MONITOR_LOGGER_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../nh-core/Util/String.h"
#include "../nh-core/Util/List.h"
#include "../nh-core/Util/Time.h"

// STRUCTS =========================================================================================

typedef struct nh_monitor_LoggerNode {
    char *name_p;
    nh_core_List Children;
    nh_core_List Messages;
    struct nh_monitor_LoggerNode *Parent_p;
} nh_monitor_LoggerNode;

typedef struct nh_monitor_Logger {
    nh_monitor_LoggerNode Root;
    int client_fd;
    int state;
    bool pause;
    nh_core_SystemTime PauseStart;
    nh_core_SystemTime LastFlush;
    nh_api_logCallback_f callback_f;
    nh_core_String Buffer;
    int totalLogs;
} nh_monitor_Logger;

// FUNCTIONS =======================================================================================

nh_monitor_Logger *nh_monitor_getLogger(
);

void nh_monitor_initLogger(
    nh_monitor_Logger *Logger_p
);

void nh_monitor_freeLogger(
    nh_monitor_Logger *Logger_p
);

NH_API_RESULT nh_monitor_log(
    char *node_p, char *options_p, char *message_p
);

void nh_monitor_getUniqueLogId(
    char *logId_p
);

void nh_monitor_dump( 
    char *node_p
); 

NH_API_RESULT nh_monitor_handleLogMessage(
    nh_monitor_Logger *Logger_p, char *node_p, char *options_p, char *message_p
);

NH_API_RESULT nh_monitor_startLoggerWorkload(
);

#endif // NH_MONITOR_LOGGER_H
