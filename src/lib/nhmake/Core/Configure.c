// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Configure.h"
#include "Thread.h"
#include "File.h"

#include "../Parser/Functions.h"
#include "../Parser/Variables.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// ADD =============================================================================================

NH_MAKE_RESULT nh_make_addFile(
    nh_make_Runtime *Runtime_p, NH_BYTE *path_p)
{
NH_MAKE_BEGIN()

    NH_MAKE_CHECK_NULL(Runtime_p)
    NH_MAKE_CHECK_NULL(path_p)

    NH_MAKE_CHECK(nh_make_appendFile(&Runtime_p->FileArray, path_p))

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// SET =============================================================================================

void nh_make_setFunctionCallback(
    nh_make_Runtime *Runtime_p, nh_make_functionCallback_f functionCallback_f)
{
NH_MAKE_BEGIN()

    Runtime_p->functionCallback_f = functionCallback_f;

NH_MAKE_SILENT_END()
}

void nh_make_setSourceContextCallback(
    nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f)
{
NH_MAKE_BEGIN()

    Runtime_p->sourceContextCallback_f = sourceContextCallback_f;

NH_MAKE_SILENT_END()
}

void nh_make_setBeforeBuildCallback(
    nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f)
{
NH_MAKE_BEGIN()

    Runtime_p->beforeBuildCallback_f = sourceContextCallback_f;

NH_MAKE_SILENT_END()
}

void nh_make_setAfterBuildCallback(
    nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f)
{
NH_MAKE_BEGIN()

    Runtime_p->afterBuildCallback_f = sourceContextCallback_f;

NH_MAKE_SILENT_END()
}

void nh_make_setQuiet(
    nh_make_Runtime *Runtime_p, NH_MAKE_BOOL quiet)
{
NH_MAKE_BEGIN()

    Runtime_p->quiet = quiet;

NH_MAKE_SILENT_END()
}

void nh_make_setShowParseTree(
    nh_make_Runtime *Runtime_p, NH_MAKE_BOOL showParseTree)
{
NH_MAKE_BEGIN()

    Runtime_p->showParseTree = showParseTree;

NH_MAKE_SILENT_END()
}

NH_MAKE_RESULT nh_make_setVariable(
    nh_make_Runtime *Runtime_p, NH_BYTE *variable_p, NH_BYTE **values_pp, int valueCount)
{
NH_MAKE_BEGIN()
NH_MAKE_END(nh_make_updateVariable(&Runtime_p->VariableArray, variable_p, values_pp, valueCount))
}

