#ifndef NH_WEBIDL_RUNTIME_H
#define NH_WEBIDL_RUNTIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Parser.h"
#include "../Common/Includes.h"
#include "../ECMAScriptBinding/Interface.h"
#include "../../nh-core/Util/Array.h"

#endif

/** @addtogroup lib_nh-webidl_structs
 *  @{
 */

    typedef struct nh_webidl_Fragment {
        char name_p[255];
        nh_webidl_Specification *Specification_p;
        nh_webidl_FragmentParseResult ParseResult;
        nh_Array Interfaces;
    } nh_webidl_Fragment;

    typedef struct nh_webidl_Runtime {
        nh_Array Specifications;
    } nh_webidl_Runtime;

/** @} */

/** @addtogroup lib_nh-webidl_vars
 *  @{
 */

    extern nh_webidl_Runtime NH_WEBIDL_RUNTIME;

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    NH_API_RESULT nh_webidl_initRuntime(
    );

    NH_API_RESULT nh_webidl_load(
        char *specification_p, char *fragmentName_p, nh_webidl_FragmentParseResult ParseResult
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

    nh_List nh_webidl_getCompositeInterfaces(
        nh_webidl_Interface *PartialInterface_p
    );

/** @} */

#endif
