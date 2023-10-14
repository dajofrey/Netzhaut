// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine 
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDES ========================================================================================

#include "../../lib/nhapi/nhapi.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// DATA ============================================================================================

typedef struct StartUpInput {
    NH_BOOL no_unload;
} StartUpInput;

static StartUpInput Input;

// HELPER ==========================================================================================

static int handleStartUpInput(
    int argc, char **argv_pp)
{
    Input.no_unload = NH_FALSE;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv_pp[i], "--no_unload")) {
            Input.no_unload = NH_TRUE;
        }
        else {
            puts("--no_unload Don't unload Netzhaut modules at shut-down.");
            puts("            Useful for stack tracing.");
            return 1;
        }
    }

    return 0;
}

// MAIN ============================================================================================
 
int main(int argc, char **argv_pp) 
{
    if (handleStartUpInput(argc, argv_pp)) {return 1;}
    if (nh_api_initialize(NH_LOADER_SCOPE_SYSTEM, NULL, NULL, 0) != NH_CORE_SUCCESS) {return 1;}

    nh_api_registerConfig("/etc/nhtty.conf", 15);

    nh_tty_TTY *TTY_p = nh_api_openTTY(NULL);
    if (!TTY_p) {return 1;}

    if (nh_api_claimStandardIO(TTY_p)) {
        return 1;
    }
    if (nh_api_addProgram(TTY_p, NH_TTY_PROGRAM_SHELL, NH_FALSE) != NH_TTY_SUCCESS) {
        return 1;
    }
    if (nh_api_addProgram(TTY_p, NH_TTY_PROGRAM_EDITOR, NH_FALSE) != NH_TTY_SUCCESS) {
        return 1;
    }
    if (nh_api_addProgram(TTY_p, NH_TTY_PROGRAM_LOGGER, NH_FALSE) != NH_TTY_SUCCESS) {
        return 1;
    }

    while (nh_api_getWorkload(TTY_p)) {
        if (!nh_api_run()) {usleep(10000);}
    }

    nh_api_terminate();
}

