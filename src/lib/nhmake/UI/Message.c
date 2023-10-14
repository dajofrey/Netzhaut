// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Message.h"
#include "GUI.h"

#include "../Core/Thread.h"
#include "../Core/Runtime.h"
#include "../Core/Utils.h"
#include "../Common/Macros.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// DATA ============================================================================================

static unsigned int messages = 0;

// MESSAGE =========================================================================================

NH_MAKE_RESULT nh_make_operationf(
    NH_BYTE *format_p, ...)
{
NH_MAKE_BEGIN()

    nh_make_Thread *Thread_p = _nh_make_getThread();
    nh_make_Runtime *Runtime_p = Thread_p && Thread_p->Runtime_p ? Thread_p->Runtime_p : &NH_MAKE_DEFAULT_RUNTIME;
    NH_BYTE *name_p = nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0];

    if (Runtime_p->quiet) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    if (Runtime_p->GUI)
    {

    }
    else 
    {
        va_list args;
        va_start(args, format_p);
    
        NH_BYTE message_p[4096] = {'\0'};
        vsprintf(message_p, format_p, args);
    
        va_end(args);
    
        if (messages == 0) {
            printf("%s: -> \e[1;32m%s\e[0m\n%s:\n", name_p, message_p, name_p);
        }
        else {printf("%s:\n%s: -> \e[1;32m%s\e[0m\n%s:\n", name_p, name_p, message_p, name_p);}
    
        messages = 0;
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_messagef(
    NH_BYTE *format_p, ...)
{
NH_MAKE_BEGIN()

    nh_make_Thread *Thread_p = _nh_make_getThread();
    nh_make_Runtime *Runtime_p = Thread_p && Thread_p->Runtime_p ? Thread_p->Runtime_p : &NH_MAKE_DEFAULT_RUNTIME;
    NH_BYTE *name_p = nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0];

    if (Runtime_p->quiet) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    if (Runtime_p->GUI)
    {

    }
    else 
    {
        va_list args;

        va_start(args, format_p);
        int size = vsnprintf(NULL, 0, format_p, args)+1;
        va_end(args);

        NH_BYTE *message_p = malloc(size);
        NH_MAKE_CHECK_NULL(message_p)
        memset(message_p, 0, size);

        va_start(args, format_p);
        vsnprintf(message_p, size, format_p, args);
        va_end(args);

        printf("%s: %s\n", name_p, message_p);
        messages++;

        free(message_p);
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_noticef(
    NH_BYTE *format_p, ...)
{
NH_MAKE_BEGIN()

    nh_make_Thread *Thread_p = _nh_make_getThread();
    nh_make_Runtime *Runtime_p = Thread_p && Thread_p->Runtime_p ? Thread_p->Runtime_p : &NH_MAKE_DEFAULT_RUNTIME;
    NH_BYTE *name_p = nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0];

    if (Runtime_p->quiet) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    if (Runtime_p->GUI)
    {

    }
    else 
    {
        va_list args;

        va_start(args, format_p);
        int size = vsnprintf(NULL, 0, format_p, args);
        va_end(args);

        NH_BYTE *message_p = malloc(size + 1);
        NH_MAKE_CHECK_NULL(message_p)
        memset(message_p, 0, size + 1);

        va_start(args, format_p);
        vsnprintf(message_p, size + 1, format_p, args);
        va_end(args);

        printf("%s: \e[1;35mNOTICE:\e[0m %s\n", name_p, message_p);
        messages++;

        free(message_p);
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_externalMessage(
    NH_BYTE *prepend_p, NH_BYTE *message_p)
{
NH_MAKE_BEGIN()

    nh_make_Thread *Thread_p = _nh_make_getThread();
    nh_make_Runtime *Runtime_p = Thread_p && Thread_p->Runtime_p ? Thread_p->Runtime_p : &NH_MAKE_DEFAULT_RUNTIME;
    NH_BYTE *name_p = nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0];

    if (Runtime_p->quiet) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    if (Runtime_p->GUI)
    {

    }
    else 
    {
        NH_BYTE *p = message_p;
        for (int i = 0; i < strlen(message_p); ++i) 
        {
            if (i + 1 == strlen(message_p)) {i++;}

            if (message_p[i] == '\n' || message_p[i] == '\0') 
            {
                NH_BYTE replace = message_p[i];
                message_p[i] = '\0';
                printf("%s%s\n", prepend_p, p);
                p = &message_p[i + 1];
                message_p[i] = replace;
                messages++;
            }
        }
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_exitMessage(
    NH_MAKE_RESULT result)
{
NH_MAKE_BEGIN()

    nh_make_Thread *Thread_p = _nh_make_getThread();
    nh_make_Runtime *Runtime_p = Thread_p && Thread_p->Runtime_p ? Thread_p->Runtime_p : &NH_MAKE_DEFAULT_RUNTIME;
    NH_BYTE *name_p = nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0];

    if (Runtime_p->quiet) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    if (Runtime_p->GUI)
    {

    }
    else 
    {
        if (result) {
            printf("%s: -> \e[1;31mERROR\e[0m %s\n", name_p, NH_MAKE_RESULTS_PP[result]);
            printf("%s: -> \e[1;31mEXECUTION ERROR\e[0m -> \e[1;31mEXITING\e[0m\n", name_p);
        }
        else {printf("%s:\n%s: -> \e[1;32mEXECUTION SUCCESS\e[0m -> \e[1;32mEXITING\e[0m\n", name_p, name_p);}
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

