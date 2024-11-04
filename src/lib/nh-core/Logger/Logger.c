// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Logger.h"
#include "../System/Thread.h"
#include "../System/Process.h"
#include "../System/Memory.h"

#include <string.h>
#include <stdio.h>

// DATA =======================================================================================

#define MAX_LOGGER_CALLBACKS 8
static nh_api_logCallback_f CALLBACKS_PP[MAX_LOGGER_CALLBACKS] = {NULL};

nh_core_Logger NH_LOGGER;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_core_addLogCallback(
    nh_api_logCallback_f logCallback_f)
{
    NH_API_RESULT result = NH_API_ERROR_BAD_STATE;
    for (int i = 0; i < MAX_LOGGER_CALLBACKS; ++i) {
        if (CALLBACKS_PP[i] == NULL) {
            CALLBACKS_PP[i] = logCallback_f;
            result = NH_API_SUCCESS;
            break;
        }
    }
 
    return result;
}

NH_API_RESULT nh_core_initLogger()
{
    NH_LOGGER.Root.name_p   = NULL;
    NH_LOGGER.Root.Parent_p = NULL;
    NH_LOGGER.Root.Children = nh_core_initList(8);
    NH_LOGGER.Root.Messages = nh_core_initList(255);

    for (int i = 0; i < MAX_LOGGER_CALLBACKS; ++i) {
        CALLBACKS_PP[i] = NULL;
    }

    return NH_API_SUCCESS;
}

static void nh_core_freeLoggerNode(
    nh_core_LoggerNode *Node_p)
{
    nh_core_free(Node_p->name_p);
    nh_core_freeList(&Node_p->Messages, true);

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_core_freeLoggerNode(Node_p->Children.pp[i]);
    }

    nh_core_freeList(&Node_p->Children, true);

    return;
}

NH_API_RESULT nh_core_freeLogger()
{
    nh_core_freeLoggerNode(&NH_LOGGER.Root);

    return NH_API_SUCCESS;
}

typedef struct nh_core_LoggerOption {
    nh_core_String Name;
    nh_core_String Value;
} nh_core_LoggerOption;

typedef struct nh_core_LoggerOptions {
    int replace;
} nh_core_LoggerOptions;

static nh_core_LoggerNode *nh_core_getLoggerNode(
    nh_core_LoggerNode *Node_p, char *node_p)
{
    if (*node_p) 
    {
        nh_core_LoggerNode *Next_p = NULL;
        if (*node_p == ':') {node_p = node_p + 1;}

        char current_p[255] = {'\0'};
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
    nh_core_Array *Options_p)
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

static NH_API_RESULT nh_core_addLogMessage(
    nh_core_LoggerNode *Node_p, nh_core_LoggerOptions Options, char *message_p)
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

static NH_API_RESULT nh_core_updateLogger(
    char *node_p, char *options_p, char *message_p)
{
    nh_core_LoggerNode *Node_p = nh_core_getLoggerNode(&NH_LOGGER.Root, node_p);
    if (Node_p == NULL) {return NH_API_ERROR_BAD_STATE;}

    nh_core_Array ParsedOptions = nh_core_initArray(sizeof(nh_core_LoggerOption), 1);
    nh_core_parseLoggerOptions(options_p, &ParsedOptions);

    nh_core_LoggerOptions Options = nh_processLoggerOptions(&ParsedOptions);

    nh_core_addLogMessage(Node_p, Options, message_p);
    
    for (int i = 0; i < ParsedOptions.length; ++i) {
        nh_core_LoggerOption *Option_p = &((nh_core_LoggerOption*)ParsedOptions.p)[i];
        nh_core_freeString(&Option_p->Name);
        nh_core_freeString(&Option_p->Value);
    }

    nh_core_freeArray(&ParsedOptions);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_sendLogMessage(
    char *node_p, char *options_p, char *message_p)
{
    if (!node_p || !message_p) {return NH_API_ERROR_BAD_STATE;}

    // send to logger
    if (nh_core_updateLogger(node_p, options_p, message_p) != NH_API_SUCCESS) {return NH_API_ERROR_BAD_STATE;}

    // send to callbacks
    for (int i = 0; i < MAX_LOGGER_CALLBACKS && CALLBACKS_PP[i] != NULL; ++i) {
        CALLBACKS_PP[i](node_p, options_p, message_p);
    }

    // send to IPC

    return NH_API_SUCCESS;
}

void nh_core_getUniqueLogId(
    char *logId_p)
{
    int thread = nh_core_getThreadIndex();
    nh_core_SystemTime SystemTime = nh_core_getSystemTime();
    sprintf(logId_p, "THREAD<%d>TIME<%lus,%lums>", thread, SystemTime.seconds, SystemTime.milliseconds);

    return;
}

void nh_core_dump(
    char *node_p)
{
    nh_core_LoggerNode *Node_p = nh_core_getLoggerNode(&NH_LOGGER.Root, node_p);
    if (Node_p) {
        for (int i = 0; i < Node_p->Messages.size; ++i) {
            puts(Node_p->Messages.pp[i]);
        }
    }
}
