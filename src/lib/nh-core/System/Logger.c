// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Logger.h"
#include "Thread.h"
#include "Process.h"
#include "Memory.h"

#include "../Common/Macros.h"

#include <string.h>
#include <stdio.h>

// CALLBACKS =======================================================================================

#define NH_MAX_LOGGER_CALLBACKS 127
static nh_core_logCallback_f callbacks_pp[NH_MAX_LOGGER_CALLBACKS] = {NULL};

NH_CORE_RESULT nh_core_addLogCallback(
    nh_core_logCallback_f logCallback_f)
{
NH_CORE_BEGIN()

    NH_CORE_RESULT result = NH_CORE_ERROR_BAD_STATE;
    for (int i = 0; i < NH_MAX_LOGGER_CALLBACKS; ++i) {
        if (callbacks_pp[i] == NULL) {
            callbacks_pp[i] = logCallback_f;
            result = NH_CORE_SUCCESS;
            break;
        }
    }
 
NH_CORE_END(result)
}

// INIT/DESTROY ====================================================================================

nh_core_Logger NH_LOGGER;

NH_CORE_RESULT nh_core_initLogger()
{
NH_CORE_BEGIN()

    NH_LOGGER.Root.name_p   = NULL;
    NH_LOGGER.Root.Parent_p = NULL;
    NH_LOGGER.Root.Children = nh_core_initList(8);
    NH_LOGGER.Root.Messages = nh_core_initList(255);

    for (int i = 0; i < NH_MAX_LOGGER_CALLBACKS; ++i) {
        callbacks_pp[i] = NULL;
    }

NH_CORE_END(NH_CORE_SUCCESS)
}

static void nh_core_freeLoggerNode(
    nh_core_LoggerNode *Node_p)
{
NH_CORE_BEGIN()

    nh_core_free(Node_p->name_p);
    nh_core_freeList(&Node_p->Messages, NH_TRUE);

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_core_freeLoggerNode(Node_p->Children.pp[i]);
    }

    nh_core_freeList(&Node_p->Children, NH_TRUE);

NH_CORE_SILENT_END()
}

NH_CORE_RESULT nh_core_freeLogger()
{
NH_CORE_BEGIN()

    nh_core_freeLoggerNode(&NH_LOGGER.Root);

NH_CORE_END(NH_CORE_SUCCESS)
}

// LOGGER ==========================================================================================

typedef struct nh_core_LoggerOption {
    nh_String Name;
    nh_String Value;
} nh_core_LoggerOption;

typedef struct nh_core_LoggerOptions {
    int replace;
} nh_core_LoggerOptions;

static nh_core_LoggerNode *nh_core_getLoggerNode(
    nh_core_LoggerNode *Node_p, NH_BYTE *node_p)
{
    if (*node_p) 
    {
        nh_core_LoggerNode *Next_p = NULL;
        if (*node_p == ':') {node_p = node_p + 1;}

        NH_BYTE current_p[255] = {'\0'};
        memset(current_p, 0, 255);
        int length;
        for (length = 0; node_p[length] && node_p[length] != ':'; ++length);
        if (length > 254) {return NULL;}
        memcpy(current_p, node_p, length);

        for (int i = 0; i < Node_p->Children.size; ++i) {
            nh_core_LoggerNode *Child_p = Node_p->Children.pp[i];
            if (!strcmp(Child_p->name_p, current_p)) {Next_p = Child_p; break;}
        }

        if (Next_p == NULL) 
        {
            Next_p = nh_core_allocate(sizeof(nh_core_LoggerNode));
            if (Next_p == NULL) {return NULL;}

            Next_p->name_p = nh_core_allocateBytes(current_p);
            if (Next_p->name_p == NULL) {return NULL;}

            Next_p->Parent_p = Node_p;
            Next_p->Children = _nh_core_initList(8);
            Next_p->Messages = _nh_core_initList(255);

            nh_core_appendToList(&Node_p->Children, Next_p);
        }

        return nh_core_getLoggerNode(Next_p, node_p + strlen(current_p));
    }

    return Node_p;
}

static void nh_core_parseLoggerOptions(
    NH_BYTE *options_p, nh_Array *Options_p)
{
    if (!options_p) {return;}

    nh_String Name = nh_core_initString(15);
    nh_String Value = nh_core_initString(15);

    while (*options_p && *options_p != '=' && *options_p != ';') {
        nh_core_appendToString(&Name, options_p, 1);
        options_p++;
    }
    if (!*options_p || *options_p != '=') {
        if (Name.length > 0) {
            nh_core_LoggerOption *Option_p = nh_core_incrementArray(Options_p);
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
        nh_core_LoggerOption *Option_p = nh_core_incrementArray(Options_p);
        Option_p->Name = Name;
        Option_p->Value = Value;
    }
    if (*options_p == ';') {
        nh_core_parseLoggerOptions(options_p + 1, Options_p);
    }
    return;
}

static nh_core_LoggerOptions nh_processLoggerOptions(
    nh_Array *Options_p)
{
    nh_core_LoggerOptions Options;
    Options.replace = -1;

    for (int i = 0; i < Options_p->length; ++i)
    {
        nh_core_LoggerOption *Option_p = &((nh_core_LoggerOption*)Options_p->p)[i];

        if (!strcmp(Option_p->Name.p, "replace")) {
            Options.replace = atoi(Option_p->Value.p);
        }
    }

    return Options;
}

static NH_CORE_RESULT nh_core_addLogMessage(
    nh_core_LoggerNode *Node_p, nh_core_LoggerOptions Options, NH_BYTE *message_p)
{
    NH_BYTE *allocated_p = nh_core_allocate(strlen(message_p) + 1);
    if (allocated_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    strcpy(allocated_p, message_p);

    if (Options.replace >= 0 && Node_p->Messages.size > Options.replace) {
        nh_core_free(Node_p->Messages.pp[Options.replace]);
        Node_p->Messages.pp[Options.replace] = allocated_p;
    }
    else {
        nh_core_appendToList(&Node_p->Messages, allocated_p);
    }

    return NH_CORE_SUCCESS;
}

static NH_CORE_RESULT nh_core_updateLogger(
    NH_BYTE *node_p, NH_BYTE *options_p, NH_BYTE *message_p)
{
    nh_core_LoggerNode *Node_p = nh_core_getLoggerNode(&NH_LOGGER.Root, node_p);
    if (Node_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    nh_Array ParsedOptions = nh_core_initArray(sizeof(nh_core_LoggerOption), 1);
    nh_core_parseLoggerOptions(options_p, &ParsedOptions);

    nh_core_LoggerOptions Options = nh_processLoggerOptions(&ParsedOptions);

    nh_core_addLogMessage(Node_p, Options, message_p);
    
    for (int i = 0; i < ParsedOptions.length; ++i) {
        nh_core_LoggerOption *Option_p = &((nh_core_LoggerOption*)ParsedOptions.p)[i];
        nh_core_freeString(&Option_p->Name);
        nh_core_freeString(&Option_p->Value);
    }

    nh_core_freeArray(&ParsedOptions);

    return NH_CORE_SUCCESS;
}

// MESSAGE =========================================================================================

NH_CORE_RESULT nh_core_sendLogMessage(
    NH_BYTE *node_p, NH_BYTE *options_p, NH_BYTE *message_p)
{
    if (!node_p || !message_p) {return NH_CORE_ERROR_BAD_STATE;}

    // send to logger
    if (nh_core_updateLogger(node_p, options_p, message_p) != NH_CORE_SUCCESS) {return NH_CORE_ERROR_BAD_STATE;}

    // send to callbacks
    for (int i = 0; i < NH_MAX_LOGGER_CALLBACKS && callbacks_pp[i] != NULL; ++i) {
        callbacks_pp[i](node_p, options_p, message_p);
    }

//    // send to forks
//    if (NH_PROCESS_POOL.forks > 0)
//    {
//        NH_BYTE *messageIPC_p = nh_core_allocate(strlen(message_p) + 11);
//        if (messageIPC_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}
//        sprintf(messageIPC_p, "NH_IPC_LOG%s", message_p);
//    
//        for (int i = 0; i < NH_MAX_FORKS; ++i) {
//            if (NH_PROCESS_POOL.Forks_p[i].id != 0) {
//                _nh_core_writeToProcess(&NH_PROCESS_POOL.Forks_p[i], messageIPC_p, strlen(messageIPC_p), NH_FALSE);
//            }
//        }
//    
//        nh_core_free(messageIPC_p);
//    }

    return NH_CORE_SUCCESS;
}

// FLOW ============================================================================================

#define NH_FLOW_INDENT 2

NH_CORE_RESULT _nh_begin(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
    nh_Thread *Thread_p = nh_core_getThread();
    if (Thread_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    NH_BYTE message_p[1024] = {'\0'};
    memset(message_p, ' ', sizeof(NH_BYTE) * (Thread_p->depth * NH_FLOW_INDENT));
    sprintf(message_p + strlen(message_p), "-> %s (%s)\n", function_p, file_p);

    Thread_p->depth++;

    NH_BYTE node_p[63] = {'\0'};
    sprintf(node_p, "nh-core:Flow:Thread%d", nh_core_getThreadIndex());

    return nh_core_sendLogMessage(node_p, NULL, message_p);
}

NH_CORE_RESULT _nh_end(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
    nh_Thread *Thread_p = nh_core_getThread();
    if (Thread_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    Thread_p->depth--;

    NH_BYTE message_p[1024] = {'\0'};
    memset(message_p, ' ', sizeof(NH_BYTE) * (Thread_p->depth * NH_FLOW_INDENT));
    sprintf(message_p + strlen(message_p), "<- %s (%s)\n", function_p, file_p);

    NH_BYTE node_p[63] = {'\0'};
    sprintf(node_p, "nh-core:Flow:Thread%d", nh_core_getThreadIndex());

    return nh_core_sendLogMessage(node_p, NULL, message_p);
}

NH_CORE_RESULT _nh_diagnosticEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p, const NH_BYTE *result_p, int line, NH_BOOL success)
{
    nh_Thread *Thread_p = nh_core_getThread();
    if (Thread_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    Thread_p->depth--;

    NH_BYTE message_p[1024] = {'\0'};
    memset(message_p, ' ', sizeof(NH_BYTE) * (Thread_p->depth * NH_FLOW_INDENT));

    if (success) {
        sprintf(message_p + strlen(message_p), "<- %s (%s)\n", function_p, file_p);
    }
    else {
        sprintf(message_p + strlen(message_p), "<- %s (%s LINE %d) \e[1;31m%s\e[0m\n", function_p, file_p, line, result_p);
    }

    NH_BYTE node_p[63] = {'\0'};
    sprintf(node_p, "nh-core:Flow:Thread%d", nh_core_getThreadIndex());

    return nh_core_sendLogMessage(node_p, NULL, message_p);
}

// LOG ID ==========================================================================================

void nh_core_getUniqueLogId(
    NH_BYTE *logId_p)
{
NH_CORE_BEGIN()

    int thread = nh_core_getThreadIndex();
    nh_SystemTime SystemTime = nh_core_getSystemTime();
    sprintf(logId_p, "THREAD<%d>TIME<%lus,%lums>", thread, SystemTime.seconds, SystemTime.milliseconds);

NH_CORE_SILENT_END()
}
