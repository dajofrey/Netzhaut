#ifndef NH_CORE_LOGGER_LOGGER_H
#define NH_CORE_LOGGER_LOGGER_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Util/String.h"
#include "../Util/List.h"
#include "../Util/Time.h"

#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct nh_core_LoggerNode {
    char *name_p;
    nh_core_List Children;
    nh_core_List Messages;
    struct nh_core_LoggerNode *Parent_p;
} nh_core_LoggerNode;

typedef struct nh_core_Logger {
    nh_core_LoggerNode Root;
    int client_fd;
    int state;
    bool pause;
    nh_core_SystemTime PauseStart;
    nh_core_SystemTime LastFlush;
    nh_api_logCallback_f callback_f;
    nh_core_String Buffer;
    int totalLogs;
} nh_core_Logger;

// FUNCTIONS =======================================================================================

nh_core_Logger *nh_core_getLogger(
);

void nh_core_initLogger(
    nh_core_Logger *Logger_p
);

void nh_core_freeLogger(
    nh_core_Logger *Logger_p
);

NH_API_RESULT nh_core_log(
    char *node_p, char *options_p, char *message_p
);

void nh_core_getUniqueLogId(
    char *logId_p
);

void nh_core_dump( 
    char *node_p
); 

NH_API_RESULT nh_core_handleLogMessage(
    nh_core_Logger *Logger_p, char *node_p, char *options_p, char *message_p
);

NH_API_RESULT nh_core_startLoggerWorkload(
);

#endif // NH_CORE_LOGGER_LOGGER_H
