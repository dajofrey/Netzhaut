// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Variables.h"
#include "Tokenizer.h"

#include "../Core/Runtime.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// INIT ============================================================================================

void nh_make_initVariableArray(
    nh_make_VariableArray *Array_p)
{
NH_MAKE_BEGIN()

    Array_p->length = 0;
    Array_p->Variables_p = NULL;

NH_MAKE_SILENT_END()
}

// UPDATE ==========================================================================================

nh_make_Variable *nh_make_getVariable(
    nh_make_VariableArray *Array_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    for (int i = 0; i < Array_p->length; ++i) {
        if (!strcmp(name_p, Array_p->Variables_p[i].name_p)) {
            NH_MAKE_END(&Array_p->Variables_p[i])
        }
    }

NH_MAKE_END(NULL)
}

static nh_make_Variable *nh_make_createVariable(
    nh_make_VariableArray *Array_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    if (!Array_p->Variables_p) {
        Array_p->Variables_p = malloc(sizeof(nh_make_Variable));
        NH_MAKE_CHECK_NULL_2(NULL, Array_p->Variables_p)
    }
    else {
        Array_p->Variables_p = realloc(Array_p->Variables_p, sizeof(nh_make_Variable) * (Array_p->length + 1));
        NH_MAKE_CHECK_NULL_2(NULL, Array_p->Variables_p)
    }
 
    nh_make_Variable *Variable_p = &Array_p->Variables_p[Array_p->length];

    Variable_p->values_pp = NULL;
    Variable_p->valueCount = 0;

    Variable_p->name_p = malloc(strlen(name_p) + 1);
    NH_MAKE_CHECK_NULL_2(NULL, Variable_p->name_p)
    sprintf(Variable_p->name_p, name_p);

    Array_p->length++;

NH_MAKE_END(Variable_p)
}

NH_MAKE_RESULT nh_make_appendToVariable(
    nh_make_VariableArray *Array_p, NH_BYTE *variable_p, NH_BYTE **values_pp, int valueCount)
{
NH_MAKE_BEGIN()

    nh_make_Variable *Variable_p = nh_make_getVariable(Array_p, variable_p);
    if (!Variable_p) {
        Variable_p = nh_make_createVariable(Array_p, variable_p);
    }
    NH_MAKE_CHECK_NULL(Variable_p)

    Variable_p->values_pp = realloc(Variable_p->values_pp, sizeof(NH_BYTE*) * (Variable_p->valueCount + valueCount));
    NH_MAKE_CHECK_NULL(Variable_p->values_pp)

    for (int i = Variable_p->valueCount, j = 0; i < valueCount + Variable_p->valueCount; ++i, ++j) {
        Variable_p->values_pp[i] = malloc(strlen(values_pp[j]) + 1);
        NH_MAKE_CHECK_NULL(Variable_p->values_pp[i])
        sprintf(Variable_p->values_pp[i], values_pp[j]);
    }

    Variable_p->valueCount += valueCount;

NH_MAKE_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_updateVariable(
    nh_make_VariableArray *Array_p, NH_BYTE *variable_p, NH_BYTE **values_pp, int valueCount)
{
NH_MAKE_BEGIN()

    nh_make_Variable *Variable_p = nh_make_getVariable(Array_p, variable_p);
    if (!Variable_p) {
        Variable_p = nh_make_createVariable(Array_p, variable_p);
    }
    NH_MAKE_CHECK_NULL(Variable_p)

    if (Variable_p->values_pp) {
        for (int i = 0; i < Variable_p->valueCount; ++i) {
            free(Variable_p->values_pp[i]);
        }
        free(Variable_p->values_pp);
    }

    Variable_p->values_pp = malloc(sizeof(NH_BYTE*) * valueCount);
    NH_MAKE_CHECK_NULL(Variable_p->values_pp)

    for (int i = 0; i < valueCount; ++i) {
        Variable_p->values_pp[i] = malloc(strlen(values_pp[i]) + 1);
        NH_MAKE_CHECK_NULL(Variable_p->values_pp[i])
        sprintf(Variable_p->values_pp[i], values_pp[i]);
    }

    Variable_p->valueCount = valueCount;

    if (!strcmp(Variable_p->name_p, "WRK_DIR")) {
        chdir(Variable_p->values_pp[0]);
    }

NH_MAKE_END(NH_MAKE_SUCCESS)
}

// REPLACE =========================================================================================

static NH_BYTE *nh_make_substituteVariableCallsInString(
    nh_make_VariableArray *Array_p, NH_BYTE *string_p)
{
NH_MAKE_BEGIN()

    NH_BYTE *begin_p = NULL;

    for (int i = 0; i < strlen(string_p); ++i) {
        if (string_p[i] == '$' && i + 1 < strlen(string_p) && string_p[i + 1] == '{') {
            begin_p = &string_p[i];
            break;
        }
    }

    if (begin_p) 
    {
        NH_BYTE *end_p = begin_p;
        while (*end_p != '}') {++end_p;}
        *end_p = 0;

        int nameLength = strlen(begin_p + 2);
        nh_make_Variable *Var_p = nh_make_getVariable(Array_p, begin_p + 2);
        *end_p = '}';

        if (!Var_p) {NH_MAKE_END(string_p)}

        NH_BYTE *newString_p = malloc((strlen(string_p) - strlen(Var_p->name_p) - 3) + strlen(Var_p->values_pp[0]) + 1);
        NH_MAKE_CHECK_NULL_2(NULL, newString_p)

        *end_p = 0;
        *begin_p = 0;

        sprintf(newString_p, string_p);
        sprintf(newString_p + strlen(newString_p), Var_p->values_pp[0]);
        sprintf(newString_p + strlen(newString_p), end_p + 1); 

        free(string_p);
        NH_MAKE_END(nh_make_substituteVariableCallsInString(Array_p, newString_p))
    }

NH_MAKE_END(string_p)
}

NH_BYTE *nh_make_substituteVariables(
    nh_make_VariableArray *Array_p, NH_BYTE *string_p)
{
NH_MAKE_BEGIN()

    NH_BYTE *newString_p = malloc(strlen(string_p) + 1);
    NH_MAKE_CHECK_NULL_2(NULL, newString_p)

    sprintf(newString_p, string_p);

NH_MAKE_END(nh_make_substituteVariableCallsInString(Array_p, newString_p)) 
}

nh_make_ValueArray nh_make_getVariableValues(
    nh_make_Runtime *Runtime_p, NH_BYTE *variable_p)
{
NH_MAKE_BEGIN()

    nh_make_Variable *Variable_p = nh_make_getVariable(&Runtime_p->VariableArray, variable_p);

    nh_make_ValueArray Values;
    Values.values_pp = NULL;
    Values.length = 0;

    if (Variable_p) {
        Values.values_pp = Variable_p->values_pp;
        Values.length = Variable_p->valueCount;
    }

NH_MAKE_END(Values)
}

