#ifndef NH_MAKE_PARSER_FUNCTIONS_H
#define NH_MAKE_PARSER_FUNCTIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Parser.h"

#include "../Core/Runtime.h"
#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_executeFunction(
        nh_make_Runtime *Runtime_p, nh_make_Function *Function_p
    ); 

    NH_MAKE_RESULT nh_make_executeBlock(
        nh_make_Runtime *Runtime_p, nh_make_Block *Block_p
    );

    NH_MAKE_BOOL nh_make_compareIf(
        nh_make_Runtime *Runtime_p, nh_make_If *If_p, NH_MAKE_BOOL b
    );

/** @} */

#endif // NH_MAKE_PARSER_FUNCTIONS_H
