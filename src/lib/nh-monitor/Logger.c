// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Logger.h"
#include "Client.h"

#include "Common/Config.h"

#include "../nh-core/System/Thread.h"
#include "../nh-core/System/Process.h"
#include "../nh-core/System/Memory.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

// FUNCTIONS =======================================================================================

nh_monitor_Logger *nh_monitor_getLogger()
{
    static nh_monitor_Logger Logger;
    return &Logger;
}

void nh_monitor_setLogCallback(
    nh_monitor_Logger *Logger_p, nh_api_logCallback_f logCallback_f)
{
    if (Logger_p == NULL) {Logger_p = nh_monitor_getLogger();}
    Logger_p->callback_f = logCallback_f;
}

void nh_monitor_initLogger(
    nh_monitor_Logger *Logger_p)
{
    if (Logger_p == NULL) {Logger_p = nh_monitor_getLogger();}
   
    memset(Logger_p, 0, sizeof(nh_monitor_Logger));

    Logger_p->Root.name_p   = NULL;
    Logger_p->Root.Parent_p = NULL;
    Logger_p->Root.Children = nh_core_initList(8);
    Logger_p->Root.Messages = nh_core_initList(255);
    Logger_p->callback_f = NULL;
    Logger_p->Buffer = nh_core_initString(256001);
}

static void nh_monitor_freeLoggerNode(
    nh_monitor_LoggerNode *Node_p)
{
    nh_core_free(Node_p->name_p);
    nh_core_freeList(&Node_p->Messages, true);

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_monitor_freeLoggerNode(Node_p->Children.pp[i]);
    }

    nh_core_freeList(&Node_p->Children, true);
}

void nh_monitor_freeLogger(
    nh_monitor_Logger *Logger_p)
{
    if (Logger_p == NULL) {Logger_p = nh_monitor_getLogger();}

    nh_monitor_freeLoggerNode(&(Logger_p->Root));
}

typedef struct nh_monitor_LoggerOption {
    nh_core_String Name;
    nh_core_String Value;
} nh_monitor_LoggerOption;

typedef struct nh_monitor_LoggerOptions {
    int replace;
} nh_monitor_LoggerOptions;

static nh_monitor_LoggerNode *nh_monitor_getLoggerNode(
    nh_monitor_LoggerNode *Node_p, char *node_p)
{
    if (*node_p) 
    {
        nh_monitor_LoggerNode *Next_p = NULL;
        if (*node_p == ':') {node_p = node_p + 1;}

        char current_p[255] = {'\0'};
        memset(current_p, 0, 255);
        int length;
        for (length = 0; node_p[length] && node_p[length] != ':'; ++length);
        if (length > 254) {return NULL;}
        memcpy(current_p, node_p, length);

        for (int i = 0; i < Node_p->Children.size; ++i) {
            nh_monitor_LoggerNode *Child_p = Node_p->Children.pp[i];
            if (!strcmp(Child_p->name_p, current_p)) {Next_p = Child_p; break;}
        }

        if (Next_p == NULL) 
        {
            Next_p = nh_core_allocate(sizeof(nh_monitor_LoggerNode));
            if (Next_p == NULL) {return NULL;}

            Next_p->name_p = nh_core_allocateBytes(current_p);
            if (Next_p->name_p == NULL) {return NULL;}

            Next_p->Parent_p = Node_p;
            Next_p->Children = nh_core_initList(8);
            Next_p->Messages = nh_core_initList(255);

            nh_core_appendToList(&Node_p->Children, Next_p);
        }

        return nh_monitor_getLoggerNode(Next_p, node_p + strlen(current_p));
    }

    return Node_p;
}

static void nh_monitor_parseLoggerOptions(
    char *options_p, nh_core_Array *Options_p)
{
    if (!options_p) {return;}

    nh_core_String Name = nh_core_initString(15);
    nh_core_String Value = nh_core_initString(15);

    while (*options_p && *options_p != '=' && *options_p != ';') {
        nh_core_appendToString(&Name, options_p, 1);
        options_p++;
    }
    if (!*options_p || *options_p != '=') {
        if (Name.length > 0) {
            nh_monitor_LoggerOption *Option_p = nh_core_incrementArray(Options_p);
            Option_p->Name = Name;
            Option_p->Value = Value;
        }
        return;
    }
    options_p++;
    while (*options_p && *options_p != ';') {
        nh_core_appendToString(&Value, options_p, 1);
        options_p++;
    }
    if (Value.length > 0) {
        nh_monitor_LoggerOption *Option_p = nh_core_incrementArray(Options_p);
        Option_p->Name = Name;
        Option_p->Value = Value;
    }
    if (*options_p == ';') {
        nh_monitor_parseLoggerOptions(options_p + 1, Options_p);
    }
    return;
}

static nh_monitor_LoggerOptions nh_monitor_processLoggerOptions(
    nh_core_Array *Options_p)
{
    nh_monitor_LoggerOptions Options;
    Options.replace = -1;

    for (int i = 0; i < Options_p->length; ++i)
    {
        nh_monitor_LoggerOption *Option_p = &((nh_monitor_LoggerOption*)Options_p->p)[i];

        if (!strcmp(Option_p->Name.p, "replace")) {
            Options.replace = atoi(Option_p->Value.p);
        }
    }

    return Options;
}

static NH_API_RESULT nh_monitor_addLogMessage(
    nh_monitor_LoggerNode *Node_p, nh_monitor_LoggerOptions Options, char *message_p)
{
    char *allocated_p = nh_core_allocate(strlen(message_p) + 1);
    if (allocated_p == NULL) {return NH_API_ERROR_BAD_STATE;}

    strcpy(allocated_p, message_p);

    if (Options.replace >= 0 && Node_p->Messages.size > Options.replace) {
        nh_core_free(Node_p->Messages.pp[Options.replace]);
        Node_p->Messages.pp[Options.replace] = allocated_p;
    }
    else {
        nh_core_appendToList(&Node_p->Messages, allocated_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_monitor_updateLogger(
    nh_monitor_Logger *Logger_p, char *node_p, char *options_p, char *message_p)
{
    nh_monitor_LoggerNode *Node_p = nh_monitor_getLoggerNode(&Logger_p->Root, node_p);
    if (Node_p == NULL) {return NH_API_ERROR_BAD_STATE;}

    nh_core_Array ParsedOptions = nh_core_initArray(sizeof(nh_monitor_LoggerOption), 1);
    nh_monitor_parseLoggerOptions(options_p, &ParsedOptions);
    nh_monitor_LoggerOptions Options = nh_monitor_processLoggerOptions(&ParsedOptions);
    nh_monitor_addLogMessage(Node_p, Options, message_p);
    
    for (int i = 0; i < ParsedOptions.length; ++i) {
        nh_monitor_LoggerOption *Option_p = &((nh_monitor_LoggerOption*)ParsedOptions.p)[i];
        nh_core_freeString(&Option_p->Name);
        nh_core_freeString(&Option_p->Value);
    }

    nh_core_freeArray(&ParsedOptions);
    Logger_p->totalLogs++;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_monitor_handleLogMessage(
    nh_monitor_Logger *Logger_p, char *node_p, char *options_p, char *message_p)
{
    if (node_p == NULL || message_p == NULL || Logger_p == NULL) {return NH_API_ERROR_BAD_STATE;}

    // send to logger
    if (nh_monitor_updateLogger(Logger_p, node_p, options_p, message_p) != NH_API_SUCCESS) {
        return NH_API_ERROR_BAD_STATE;
    }

    // send to callback
    if (Logger_p->callback_f != NULL) {Logger_p->callback_f(node_p, options_p, message_p);}

    // send to peer-monitor
    if (Logger_p->state == 2) {
        if (Logger_p->Buffer.length >= 256000) {
            nh_core_waitForCompletion(nh_core_getWorkload(Logger_p), NH_SIGNAL_OK);
        }
        char send_p[256] = {0};
        snprintf(send_p, sizeof(send_p)-1, "%s\x1F%s\x1F%s", node_p, options_p, message_p);
        nh_core_appendToString(&Logger_p->Buffer, send_p, 256);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_monitor_log(
    char *node_p, char *options_p, char *message_p)
{
    return nh_monitor_handleLogMessage(nh_monitor_getLogger(), node_p, options_p, message_p);
}

void nh_monitor_getUniqueLogId(
    char *logId_p)
{
    int thread = nh_core_getThreadIndex();
    nh_core_SystemTime SystemTime = nh_core_getSystemTime();
    sprintf(logId_p, "THREAD<%d>TIME<%lus,%lums>", thread, SystemTime.seconds, SystemTime.milliseconds);
}

void nh_monitor_dump(
    char *node_p)
{
    nh_monitor_LoggerNode *Node_p = nh_monitor_getLoggerNode(&nh_monitor_getLogger()->Root, node_p);
    if (Node_p) {
        for (int i = 0; i < Node_p->Messages.size; ++i) {
            puts(Node_p->Messages.pp[i]);
        }
    }
}

static NH_SIGNAL nh_monitor_runLoggerWorkload(
    void *args_p)
{
    nh_monitor_Logger *Logger_p = args_p;

    if (nh_monitor_getConfig().client_port == 0) {
        if (Logger_p->state > 0) {
            // TODO
            Logger_p->state = 0;
        }
        return NH_SIGNAL_IDLE;
    }

    if (Logger_p->pause) {
        if (nh_core_getSystemTimeDiffInSeconds(Logger_p->PauseStart, nh_core_getSystemTime()) < 1.0) {
            return NH_SIGNAL_IDLE;
        }
        Logger_p->pause = false;
    }

    switch (Logger_p->state) {
        case 0 :
            if ((Logger_p->client_fd = nh_monitor_connectToMonitor(nh_monitor_getConfig().client_port)) > 0) {
                Logger_p->state = 1;
            }
            break;
        case 1 :
            if (nh_monitor_checkIfMonitorConnectionEstablished(Logger_p->client_fd) == NH_API_SUCCESS) {
                Logger_p->state = 2;
            } else {
                nh_monitor_disconnectFromMonitor(Logger_p->client_fd);
                Logger_p->client_fd = 0;
                Logger_p->state = 0;
                Logger_p->pause = true;
                Logger_p->PauseStart = nh_core_getSystemTime();
                Logger_p->LastFlush = nh_core_getSystemTime();
            }
            break;
        case 2 :
        {
            bool flush = nh_core_getSystemTimeDiffInSeconds(Logger_p->LastFlush, nh_core_getSystemTime()) > 0.5;
 
            if (Logger_p->Buffer.length < 256000 && !flush) {
                return NH_SIGNAL_IDLE;
            }
            if (Logger_p->Buffer.length <= 0) {
                return NH_SIGNAL_IDLE;
            }

            int send = nh_monitor_sendMessageToMonitor(
                Logger_p->client_fd, Logger_p->Buffer.p, Logger_p->Buffer.length);
            while (send == -1) {
                send = nh_monitor_sendMessageToMonitor(Logger_p->client_fd, Logger_p->Buffer.p, Logger_p->Buffer.length);
            }
            nh_core_freeString(&Logger_p->Buffer);
            Logger_p->Buffer = nh_core_initString(256001);
            while (nh_monitor_waitForMonitorAck(Logger_p->client_fd) == -1) {
                usleep(10);
            }
            Logger_p->LastFlush = nh_core_getSystemTime();
            break;
        }
    }

    return NH_SIGNAL_OK;
}

static void *nh_monitor_initLoggerWorkload(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "Logger Workload";
    static char *path_p = "nh-monitor/Monitor/Logger.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_CORE;
    Workload_p->args_p = nh_monitor_getLogger();

    nh_monitor_getLogger()->state = 0;
    nh_monitor_getLogger()->pause = false;

    if (nh_monitor_getConfig().block) {
        while (nh_monitor_getLogger()->state != 2) {
            if (nh_monitor_runLoggerWorkload(Workload_p->args_p) == NH_SIGNAL_IDLE) {
                usleep(100);
            }
        }
    }

    return Workload_p->args_p;
}

NH_API_RESULT nh_monitor_startLoggerWorkload() 
{
    nh_core_activateWorkload(nh_monitor_initLoggerWorkload, nh_monitor_runLoggerWorkload, NULL, NULL, NULL, false);
    return NH_API_SUCCESS;
}
