// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Main.h"
#include "UnicodeData.h"
#include "WebIDL.h"

#include "../../lib/nhapi/nhmake.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// MAIN ============================================================================================

static NH_MAKE_RESULT functionCallback(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p)
{
    if (!strcmp(Function_p->name_p, "createUnicodeData")) {
        return createUnicodeData(Runtime_p);
    }
    else if (!strcmp(Function_p->name_p, "processWebIDL")) {
        return processWebIDL(Runtime_p);
    }

    return NH_MAKE_SUCCESS;
}

int main(
    int argc, char **argv_pp)
{
    nh_make_initialize();

    nh_make_Runtime *Runtime_p = nh_make_createRuntime("nhmake");

    NH_BYTE *procDir_p = nh_make_getProcessDirectory();
    nh_make_setVariable(Runtime_p, "WRK_DIR", &procDir_p, 1);

    nh_make_addFile(Runtime_p, ".nhmake/override.nhmake");
    nh_make_addFile(Runtime_p, ".nhmake/source.nhmake");
    nh_make_addFile(Runtime_p, ".nhmake/options.nhmake");
    nh_make_addFile(Runtime_p, ".nhmake/idl.nhmake");
    nh_make_addFile(Runtime_p, ".nhmake/test.nhmake");

    nh_make_setFunctionCallback(Runtime_p, functionCallback);

    nh_make_run(Runtime_p, argc - 1, argv_pp + 1);
    while (nh_make_isRunning()) {nh_make_sleepMs(100);}

    nh_make_destroyRuntime(Runtime_p);
    nh_make_terminate();
} 

