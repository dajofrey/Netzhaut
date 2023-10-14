#ifndef NH_MAKE_PARSER_VARIABLES_H
#define NH_MAKE_PARSER_VARIABLES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_Variable {
        NH_BYTE *name_p;
        int valueCount;
        NH_BYTE **values_pp;
    } nh_make_Variable;

    typedef struct nh_make_VariableArray {
        int length;
        nh_make_Variable *Variables_p;
    } nh_make_VariableArray;

/** @} */

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef nh_make_ValueArray (*nh_make_getVariableValues_f)(
        nh_make_Runtime *Runtime_p, NH_BYTE *variable_p
    );

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    void nh_make_initVariableArray(
        nh_make_VariableArray *Array_p
    );

    nh_make_Variable *nh_make_getVariable(
        nh_make_VariableArray *Array_p, NH_BYTE *name_p
    );

    NH_MAKE_RESULT nh_make_appendToVariable(
        nh_make_VariableArray *Array_p, NH_BYTE *variable_p, NH_BYTE **values_pp, int valueCount
    );

    NH_MAKE_RESULT nh_make_updateVariable(
        nh_make_VariableArray *Array_p, NH_BYTE *variable_p, NH_BYTE **values_pp, int valueCount
    );

    NH_BYTE *nh_make_substituteVariables(
        nh_make_VariableArray *Array_p, NH_BYTE *string_p
    );

    nh_make_ValueArray nh_make_getVariableValues(
        nh_make_Runtime *Runtime_p, NH_BYTE *variable_p
    );

/** @} */

#endif // NH_MAKE_PARSER_VARIABLES_H
