#ifndef NH_WEBIDL_GLOBAL_H
#define NH_WEBIDL_GLOBAL_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Parser.h"
#include "Interface.h"

#include "../Common/Includes.h"
#include "../../nh-core/Util/Array.h"

/** @addtogroup lib_nh-webidl_structs
 *  @{
 */

    typedef struct nh_webidl_Fragment {
        char name_p[255];
        nh_webidl_Specification *Specification_p;
        nh_webidl_FragmentParseResult ParseResult;
        nh_core_Array Interfaces;
    } nh_webidl_Fragment;

    typedef struct nh_webidl_Global {
        nh_core_Array Specifications;
    } nh_webidl_Global;

/** @} */

/** @addtogroup lib_nh-webidl_vars
 *  @{
 */

    extern nh_webidl_Global NH_WEBIDL_GLOBAL;

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    NH_API_RESULT nh_webidl_createGlobal(
    );

    nh_webidl_Specification *nh_webidl_getSpecification(
        char *specification_p
    );

    nh_webidl_Fragment *nh_webidl_getFragment(
        char *specification_p, char *fragment_p
    );

    nh_webidl_Interface *nh_webidl_getInterface(
        char *specification_p, char *interface_p
    );

    nh_core_List nh_webidl_getCompositeInterfaces(
        nh_webidl_Interface *PartialInterface_p
    );

/** @} */

#endif
