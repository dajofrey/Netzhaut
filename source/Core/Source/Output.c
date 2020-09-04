// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Output.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"
#include "../Header/HashMap.h"

#include "../../JavaScript/Header/Parser.h"
#include "../../CSS/Header/Sheet.h"
#include "../../HTML/Header/Document.h"
#include "../../TTY/Header/Terminal.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

// DATA ============================================================================================

Nh_List Outputs;
Nh_List OutputCallbacks;

// DECLARE =========================================================================================

NH_RESULT Nh_initOutputs(
    Nh_OutputCallback Callback)
{
NH_BEGIN()

    NH_INIT_LIST(Outputs)
    NH_INIT_LIST(OutputCallbacks)

    if (Nh_TTY_getTerminal()->Flags.run) {
        NH_CHECK(Nh_addListItem(&OutputCallbacks, Nh_TTY_writeLog))
    }
    if (Callback != NULL) {NH_CHECK(Nh_addListItem(&OutputCallbacks, Callback))}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_freeOutputs()
{
NH_BEGIN()

    for (int i = 0; i < Outputs.count; ++i) {
        Nh_freeOutput(Nh_getListItem(&Outputs, i));
    }
    Nh_destroyList(&Outputs, true);
    Nh_destroyList(&OutputCallbacks, false);
    
NH_END(NH_SUCCESS)
}

void Nh_freeOutput(
    Nh_Output *Output_p)
{
NH_BEGIN()

    switch (Output_p->type)
    {
        case NH_OUTPUT_HTML_DOCUMENT : Nh_HTML_destroyDocument(Output_p->pointer, NULL, NH_FALSE); break;
        case NH_OUTPUT_JS_SCRIPT     : Nh_JS_unparseScript(Output_p->pointer); break;
        case NH_OUTPUT_CSS_SHEET     : Nh_CSS_destroySheet(Output_p->pointer); break;
        case NH_OUTPUT_CHARS         : Nh_free(Output_p->pointer); break;
        default                      : break; 
    }
    Output_p->pointer = NULL;
    Nh_destroyListItemFromPointer(&Outputs, Output_p, true);

NH_SILENT_END()
}

NH_RESULT Nh_addOutput(
    NH_OUTPUT type, void *pointer, NH_BOOL force)
{
NH_BEGIN()

    if (force == NH_FALSE && OutputCallbacks.count <= 0) {NH_END(NH_ERROR_BAD_STATE)}

    Nh_Output *Output_p = Nh_allocate(sizeof(Nh_Output));
    NH_CHECK_MEM(Output_p)

    Output_p->type    = type;
    Output_p->pointer = pointer;

    NH_CHECK(Nh_addListItem(&Outputs, Output_p))

    for (int i = 0; i < OutputCallbacks.count; ++i) {
        ((Nh_OutputCallback*)Nh_getListItem(&OutputCallbacks, i))(Output_p);
    }

NH_END(NH_SUCCESS)
}

int Nh_getOutputCallbackCount()
{
NH_BEGIN()
NH_END(OutputCallbacks.count)
}

